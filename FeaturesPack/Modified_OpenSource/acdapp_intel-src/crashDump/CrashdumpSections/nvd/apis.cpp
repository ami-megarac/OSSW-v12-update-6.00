/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2019 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in the
 * License.
 *
 ******************************************************************************/

#include "apis.hpp"

#include "csrs.hpp"
#include "registers.hpp"

#define RETURN_ON_API_ERR(status, statusApi, cc)                               \
    if (status != NVD_SUCCESS)                                                 \
    {                                                                          \
        fprintf(stderr, "NVD failed - status(%d): cc: (0x%x)\n", status, *cc); \
        return statusApi;                                                      \
    }

uint16_t discovery(const int addr, const int fd, uint16_t* mask)
{
    uint8_t cc = 0;
    uint16_t failMask = 0;
    uint32_t devid = 0;
    static const constexpr uint8_t maxNumOfDimms = 16;

    for (int i = 0; i < maxNumOfDimms; i++)
    {
        if ((csrRd(addr, i, csrs[vendor_device_id], &devid, fd, &cc, true) !=
             NVD_SUCCESS) &&
            PECI_CC_UA(cc))
        {
            SET_BIT(failMask, i);
        }
        switch (devid)
        {
            case ekv:
            case bwv:
                SET_BIT(*mask, i);
        }
    }

    return failMask;
}

uint32_t api(const int addr, const uint8_t dimm, const ApiParams params,
             uint32_t* payload, const uint32_t payloadSize, const int fd,
             uint8_t* cc)
{
    uint32_t val = 0;
    NVDStatus status =
        csrRd(addr, dimm, csrs[mb_smbus_cmd], &val, fd, cc, false);
    RETURN_ON_API_ERR(status, API_FAIL, cc);

    // Checking that doorbell bit was not set
    if (getFields(16, 16, val) == 1)
    {
        RETURN_ON_API_ERR(status, API_FAIL, cc);
    }

    // Set all the input registers
    int startIdx = mb_smbus_input_payload_0;
    for (int i = 0; i < params.indwSize; i++)
    {
        status =
            csrWr(addr, dimm, csrs[startIdx++], params.indw[i], fd, cc, false);
        RETURN_ON_API_ERR(status, API_FAIL, cc);
    }

    uint32_t cmdOpcode = bitField(0, 8, params.opcode);
    uint32_t cmdSubOpcode = bitField(8, 8, params.subopcode);
    uint32_t cmdDoorbell = bitField(16, 1, 1);
    uint32_t cmd = cmdOpcode | cmdSubOpcode;

    status = csrWr(addr, dimm, csrs[mb_smbus_cmd], cmd, fd, cc, false);
    RETURN_ON_API_ERR(status, API_FAIL, cc);
    status = csrWr(addr, dimm, csrs[mb_smbus_cmd], (cmd | cmdDoorbell), fd, cc,
                   false);
    RETURN_ON_API_ERR(status, API_FAIL, cc);

    int count = 0;
    timespec req = {};
    req.tv_nsec = API_CHECK_INTERVAL_MS * 1000 * 1000;

    while (count < API_MAX_RETRIES)
    {
        status = csrRd(addr, dimm, csrs[mb_smbus_status], &val, fd, cc, false);

        // if completed bit set and ratelimiting bit not set
        if (CHECK_BIT(val, 0) && !CHECK_BIT(val, 3))
        {
            break;
        }

        count++;
        nanosleep(&req, NULL);

        if (count > 3)
        {
            fprintf(stderr, "NVD: Warning - API Operation not completed,"
                            "retrying...\n");
        }
    }

    // operation not completed after retries
    if (!CHECK_BIT(val, 0))
    {
        fprintf(stderr,
                "NVD: Warning - unexpected status register value"
                "0x%x\n",
                status);
    }

    startIdx = mb_smbus_output_payload_0;
    for (int i = 0; i < payloadSize; i++)
    {
        status =
            csrRd(addr, dimm, csrs[startIdx++], &payload[i], fd, cc, false);
        RETURN_ON_API_ERR(status, API_FAIL, cc);
    }

    return val;
}

uint32_t identifyDimm(const int addr, const uint8_t dimm, uint32_t* payload,
                      const uint32_t payloadSize, const int fd, uint8_t* cc)
{
    ApiParams params = {};
    params.opcode = 0x1;
    params.subopcode = 0x0;
    params.indwSize = 0;
    return api(addr, dimm, params, payload, payloadSize, fd, cc);
}

uint32_t getFwDebugLogCommon(const int addr, const uint8_t dimm,
                             const uint8_t logAction, const uint32_t bytesize,
                             uint32_t* payload, const uint32_t payloadSize,
                             const int fd, uint8_t* cc)
{
    ApiParams params = {};
    params.opcode = 0x8;
    params.subopcode = 0x2;
    params.indwSize = 2;
    params.indw = (uint32_t*)calloc(params.indwSize, sizeof(uint32_t));
    if (params.indw == NULL)
    {
        fprintf(stderr, "NVD: Error allocating memory (size:%d)\n",
                params.indwSize);
        return API_FAIL;
    }

    uint32_t outdwSize = 32;
    uint32_t* outdw = (uint32_t*)calloc(outdwSize, sizeof(uint32_t));
    if (outdw == NULL)
    {
        fprintf(stderr, "NVD: Error allocating memory (size:%d)\n", outdwSize);
        return API_FAIL;
    }

    uint32_t laststatus = 0;
    uint32_t addr_128byteoffset = 0;

    uint32_t count = 0;
    for (uint64_t i = 0; i < bytesize; i += 128)
    {
        addr_128byteoffset = i >> 7;
        params.indw[0] = (addr_128byteoffset << 8) & 0xffffff00 | 1;
        params.indw[1] =
            (addr_128byteoffset >> 24) & 0xff | ((logAction & 0xff) << 8);

        laststatus = api(addr, dimm, params, outdw, outdwSize, fd, cc);

        for (int i = 0; i < outdwSize; i++)
        {
            payload[count] = outdw[i];
            count = count + 1;
        }
    }

    FREE(params.indw);
    FREE(outdw);

    return laststatus;
}

uint32_t getSramFwDebugLog(const int addr, const uint8_t dimm,
                           uint32_t* payload, const uint32_t payloadSize,
                           const int fd, uint8_t* cc)
{
    return getFwDebugLogCommon(addr, dimm, 0x2, 0x800, payload, payloadSize, fd,
                               cc);
}

uint32_t getSpiFwDebugLog(const int addr, const uint8_t dimm, uint32_t* payload,
                          const uint32_t payloadSize, const int fd, uint8_t* cc)
{
    return getFwDebugLogCommon(addr, dimm, 0x3, 0x800, payload, payloadSize, fd,
                               cc);
}
