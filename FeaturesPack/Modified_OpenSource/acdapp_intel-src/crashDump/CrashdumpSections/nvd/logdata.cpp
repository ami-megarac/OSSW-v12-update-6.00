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

#include "logdata.hpp"

#include "apis.hpp"
#include "crashdump.hpp"
#include "csrs.hpp"
#include "registers.hpp"

void logRegisters(const int addr, const uint8_t dimm, const int fd,
                  cJSON* jsonChild)
{
    uint8_t cc = 0;
    uint32_t val = 0;
    NVDStatus status = NVD_FAIL;
    char valStr[NVD_JSON_STRING_LEN];

    cJSON* registers = NULL;
    cJSON_AddItemToObject(jsonChild, "registers",
                          registers = cJSON_CreateObject());

    uint8_t reglist[] = {vendor_device_id, revision_mfg_id, d_fw_status,
                         d_fw_status_h};

    for (int i = 0; i < sizeof(reglist) / sizeof(reglist[0]); i++)
    {
        status = csrRd(addr, dimm, csrs[reglist[i]], &val, fd, &cc, false);
        if (status != NVD_SUCCESS)
        {
            cJSON_AddStringToObject(registers, csrs[reglist[i]].name, NVD_NA);
        }
        else
        {
            cd_snprintf_s(valStr, NVD_JSON_STRING_LEN, NVD_UINT32_FMT, val);
            cJSON_AddStringToObject(registers, csrs[reglist[i]].name, valStr);
        }
    }
}

void logStatusAndPayload(uint32_t status, uint32_t* payload,
                         uint32_t payloadSize, cJSON* jsonChild)
{
    char jsonItemString[NVD_JSON_STRING_LEN];
    cd_snprintf_s(jsonItemString, NVD_JSON_STRING_LEN, NVD_UINT32_FMT, status);
    cJSON_AddStringToObject(jsonChild, "status", jsonItemString);

    char buf[MAX_PAYLOAD_STR];
    strcpy_s(buf, sizeof("0x"), "0x");

    for (int i = (int)(payloadSize - 1); i >= 0; i--)
    {
        cd_snprintf_s(jsonItemString, sizeof(jsonItemString), "%.8" PRIx32,
                      payload[i]);
        strcat_s(buf, sizeof(buf), jsonItemString);
    }
    cJSON_AddStringToObject(jsonChild, "payload", buf);
}

void logIdentifyDimm(const int addr, const uint8_t dimm, const int fd,
                     cJSON* jsonChild)
{
    uint8_t cc = 0;
    uint32_t payloadSize = 20;
    uint32_t* payload = (uint32_t*)calloc(payloadSize, sizeof(uint32_t));

    if (payload != NULL)
    {
        cJSON* section = NULL;
        cJSON_AddItemToObject(jsonChild, "identify_dimm",
                              section = cJSON_CreateObject());
        uint32_t status =
            identifyDimm(addr, dimm, payload, payloadSize, fd, &cc);
        logStatusAndPayload(status, payload, payloadSize, section);
        FREE(payload);
    }
}

void logSramFwDebugLog(const int addr, const uint8_t dimm, const int fd,
                       cJSON* jsonChild)
{
    uint8_t cc = 0;
    uint32_t payloadSize = 512;
    uint32_t* payload = (uint32_t*)calloc(payloadSize, sizeof(uint32_t));

    if (payload != NULL)
    {
        cJSON* section = NULL;
        cJSON_AddItemToObject(jsonChild, "fw_debug_log_sram",
                              section = cJSON_CreateObject());
        uint32_t status =
            getSramFwDebugLog(addr, dimm, payload, payloadSize, fd, &cc);
        logStatusAndPayload(status, payload, payloadSize, section);
        FREE(payload);
    }
}

void logSpiFwDebugLog(const int addr, const uint8_t dimm, const int fd,
                      cJSON* jsonChild)
{
    uint8_t cc = 0;
    uint32_t payloadSize = 512;
    uint32_t* payload = (uint32_t*)calloc(payloadSize, sizeof(uint32_t));

    if (payload != NULL)
    {
        cJSON* section = NULL;
        cJSON_AddItemToObject(jsonChild, "fw_debug_log_spi",
                              section = cJSON_CreateObject());
        uint32_t status =
            getSpiFwDebugLog(addr, dimm, payload, payloadSize, fd, &cc);
        logStatusAndPayload(status, payload, payloadSize, section);
        FREE(payload);
    }
}

void logNvdJson(const int addr, const uint8_t cpuNum, cJSON* jsonChild)
{
    uint16_t mask = 0;
    int fd = -1;

    EPECIStatus peciStatus = peci_Lock(&fd, PECI_TIMEOUT_MS);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        fprintf(stderr, "peci lock failed!\n");
        return;
    }

    discovery(addr, fd, &mask);
    if (mask == 0)
    {
        fprintf(stderr, "cpu:%d NVD not found!\n", addr);
        peci_Unlock(fd);
        return;
    }

    for (uint32_t dimm = 0; dimm < sizeof(mask) * 8; dimm++)
    {
        if (!CHECK_BIT(mask, dimm))
        {
            continue;
        }

        char jsonStr[NVD_JSON_STRING_LEN];
        cJSON* dimmSection = NULL;
        cd_snprintf_s(jsonStr, sizeof(jsonStr), dimmMap[dimm], cpuNum);
        cJSON_AddItemToObject(jsonChild, jsonStr,
                              dimmSection = cJSON_CreateObject());
        logRegisters(addr, dimm, fd, dimmSection);
        logIdentifyDimm(addr, dimm, fd, dimmSection);
        logSramFwDebugLog(addr, dimm, fd, dimmSection);
        logSpiFwDebugLog(addr, dimm, fd, dimmSection);
    }

    peci_Unlock(fd);
}
