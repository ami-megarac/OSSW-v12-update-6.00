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

#include "registers.hpp"

#include "crashdump.hpp"

#include <time.h>

// Notes: The list order is important for accessing register BDF correctly.
enum regname
{
    smb_cmd_cfg_spd0,
    smb_cmd_cfg_spd1,
    smb_data_cfg_spd0,
    smb_data_cfg_spd1,
    smb_status_cfg_spd0,
    smb_status_cfg_spd1,
};

#define RETURN_ON_ERR(status, cat, cc)                                         \
    if (status != PECI_CC_SUCCESS)                                             \
    {                                                                          \
        fprintf(stderr, "NVD failed - cat(%d): cc: (0x%x)\n", cat, *cc);       \
        return cat;                                                            \
    }

#define RETURN_ON_CSR_ERR(status, cc)                                          \
    if (status != NVD_SUCCESS)                                                 \
    {                                                                          \
        fprintf(stderr, "NVD failed - status(%d): cc: (0x%x)\n", status, *cc); \
        return status;                                                         \
    }

NVDStatus checkStatus(const int addr, const RegEntry reg, const int fd,
                      uint8_t* cc)
{
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = SMBUS_TIMEOUT_INTERVAL_MS * 1000 * 1000;
    int timeout_count = 0;
    uint32_t data = 0;
    EPECIStatus peciStatus = PECI_CC_SUCCESS;

    do
    {
        nanosleep(&req, NULL);
        timeout_count += SMBUS_TIMEOUT_INTERVAL_MS;
        peciStatus = regRd(addr, reg, &data, fd, cc);
        RETURN_ON_ERR(peciStatus, NVD_SMB_PECI_FAIL, cc);
        data = getFields(data, 1, 1);
        if ((data == 1) || (timeout_count >= SMBUS_TIMEOUT_MS))
        {
            RETURN_ON_ERR(peciStatus, NVD_SMB_STATUS_CHECK_FAIL, cc);
        }
    } while ((0 == getFields(data, 0, 0)) &&
             (timeout_count <= SMBUS_TIMEOUT_MS));

    return NVD_SUCCESS;
}

EPECIStatus regRd(int addr, RegEntry reg, uint32_t* val, int fd, uint8_t* cc)
{
    return peci_RdEndPointConfigPciLocal_seq(addr, reg.seg, reg.bus, reg.dev,
                                             reg.func, reg.offset, reg.size,
                                             (uint8_t*)val, fd, cc);
}

EPECIStatus regWr(int addr, RegEntry reg, uint32_t val, int fd, uint8_t* cc)
{
    return peci_WrEndPointConfig_seq(addr, PECI_ENDPTCFG_TYPE_LOCAL_PCI,
                                     reg.seg, reg.bus, reg.dev, reg.func,
                                     reg.offset, reg.size, val, fd, cc);
}

NVDStatus smbusRd(int addr, uint8_t bus, uint8_t saddr, uint8_t command,
                  uint8_t bytewidth, uint32_t* val, int fd, uint8_t* cc,
                  bool check)
{
    uint8_t smb_cmd_cfg_spd = (bus == 0 ? smb_cmd_cfg_spd0 : smb_cmd_cfg_spd1);
    uint8_t smb_data_cfg_spd =
        (bus == 0 ? smb_data_cfg_spd0 : smb_data_cfg_spd1);
    uint8_t smb_status_cfg_spd =
        (bus == 0 ? smb_status_cfg_spd0 : smb_status_cfg_spd1);
    uint32_t data = 0;
    EPECIStatus peciStatus = PECI_CC_SUCCESS;
    NVDStatus nvdStatus = NVD_SUCCESS;

    peciStatus = regRd(addr, regs[smb_cmd_cfg_spd], &data, fd, cc);
    RETURN_ON_ERR(peciStatus, NVD_SMB_PECI_FAIL, cc);

    // reference: ICX EDS Vol2 17.1
    setFields(&data, 19, 19, 1);
    setFields(&data, 29, 29, 1);
    setFields(&data, 17, 17, bytewidth - 1);
    setFields(&data, 16, 15, 0);
    setFields(&data, 14, 11, (saddr >> 4) & 0xf);
    setFields(&data, 10, 8, (saddr >> 1) & 7);
    setFields(&data, 7, 0, command);
    setFields(&data, 20, 20, 0);

    peciStatus = regWr(addr, regs[smb_cmd_cfg_spd], data, fd, cc);
    RETURN_ON_ERR(peciStatus, NVD_SMB_PECI_FAIL, cc);

    peciStatus = regRd(addr, regs[smb_data_cfg_spd], val, fd, cc);
    RETURN_ON_ERR(peciStatus, NVD_SMB_PECI_FAIL, cc);

    // use only on dimm discovery
    if (check)
    {
        nvdStatus = checkStatus(addr, regs[smb_status_cfg_spd], fd, cc);
    }

    *val = getFields(*val, 15, 0);

    return nvdStatus;
}

NVDStatus smbusWr(int addr, uint8_t bus, uint8_t saddr, uint8_t command,
                  uint8_t bytewidth, uint32_t val, int fd, uint8_t* cc,
                  bool check)
{
    uint8_t smb_cmd_cfg_spd = (bus == 0 ? smb_cmd_cfg_spd0 : smb_cmd_cfg_spd1);
    uint8_t smb_data_cfg_spd =
        (bus == 0 ? smb_data_cfg_spd0 : smb_data_cfg_spd1);
    uint8_t smb_status_cfg_spd =
        (bus == 0 ? smb_status_cfg_spd0 : smb_status_cfg_spd1);
    uint32_t data = 0;
    EPECIStatus peciStatus = PECI_CC_SUCCESS;
    NVDStatus nvdStatus = NVD_SUCCESS;

    peciStatus = regRd(addr, regs[smb_data_cfg_spd], &data, fd, cc);
    RETURN_ON_ERR(peciStatus, NVD_SMB_PECI_FAIL, cc);

    setFields(&data, 31, 16, val);
    peciStatus = regWr(addr, regs[smb_data_cfg_spd], data, fd, cc);
    RETURN_ON_ERR(peciStatus, NVD_SMB_PECI_FAIL, cc);

    peciStatus = regRd(addr, regs[smb_cmd_cfg_spd], &data, fd, cc);
    RETURN_ON_ERR(peciStatus, NVD_SMB_PECI_FAIL, cc);

    // reference: ICX EDS Vol2 17.1
    setFields(&data, 19, 19, 1);
    setFields(&data, 29, 29, 1);
    setFields(&data, 17, 17, bytewidth - 1);
    setFields(&data, 16, 15, 1);
    setFields(&data, 14, 11, (saddr >> 4) & 0xf);
    setFields(&data, 10, 8, (saddr >> 1) & 7);
    setFields(&data, 7, 0, command);
    setFields(&data, 20, 20, 0);

    peciStatus = regWr(addr, regs[smb_cmd_cfg_spd], data, fd, cc);
    RETURN_ON_ERR(peciStatus, NVD_SMB_PECI_FAIL, cc);

    // use only on dimm discovery
    if (check)
    {
        nvdStatus = checkStatus(addr, regs[smb_status_cfg_spd], fd, cc);
    }

    return nvdStatus;
}

NVDStatus csrRd(int addr, uint8_t dimm, SmBusCsrEntry csr, uint32_t* val,
                int fd, uint8_t* cc, bool check)
{
    uint8_t bus = dimm < MAX_DIMM_PER_BUS ? 0 : 1;
    uint8_t saddr = BASE_ADDR + (dimm % MAX_DIMM_PER_BUS) * 2;
    uint32_t cmdByteBegin = bitField(7, 1, 1);
    uint32_t cmdByteEnd = bitField(6, 1, 1);
    uint32_t cmdByteInternalCmd = bitField(2, 2, 0);
    uint32_t cmdByteSmBusCmd = bitField(0, 2, 1);
    uint32_t byteWidth = 2;
    uint32_t val0 = 0;
    uint32_t val1 = 0;
    uint32_t val2 = 0;
    NVDStatus status = NVD_FAIL;

    uint8_t cmdByte = cmdByteBegin | cmdByteInternalCmd | cmdByteSmBusCmd;
    status = smbusWr(addr, bus, saddr, cmdByte, byteWidth,
                     (csr.func & 0x7) | ((csr.dev & 0x1f) << 3), fd, cc, check);
    RETURN_ON_CSR_ERR(status, cc);

    cmdByte = cmdByteEnd | cmdByteInternalCmd | cmdByteSmBusCmd;
    status = smbusWr(addr, bus, saddr, cmdByte, byteWidth, csr.offset, fd, cc,
                     check);
    RETURN_ON_CSR_ERR(status, cc);

    cmdByte = cmdByteBegin | cmdByteInternalCmd | cmdByteSmBusCmd;
    status =
        smbusRd(addr, bus, saddr, cmdByte, byteWidth, &val0, fd, cc, check);
    RETURN_ON_CSR_ERR(status, cc);

    cmdByte = cmdByteInternalCmd | cmdByteSmBusCmd;
    status =
        smbusRd(addr, bus, saddr, cmdByte, byteWidth, &val1, fd, cc, check);
    RETURN_ON_CSR_ERR(status, cc);

    cmdByte = cmdByteEnd | cmdByteInternalCmd | cmdByteSmBusCmd;
    byteWidth = 1;
    status =
        smbusRd(addr, bus, saddr, cmdByte, byteWidth, &val2, fd, cc, check);
    RETURN_ON_CSR_ERR(status, cc);

    *val = ((val0 & 0xff) << 24) | ((val1 & 0xffff) << 8) | (val2 & 0xff);

    return NVD_SUCCESS;
}

NVDStatus csrWr(int addr, uint8_t dimm, SmBusCsrEntry csr, uint32_t val, int fd,
                uint8_t* cc, bool check)
{
    uint8_t bus = dimm < MAX_DIMM_PER_BUS ? 0 : 1;
    uint8_t saddr = BASE_ADDR + (dimm % MAX_DIMM_PER_BUS) * 2;
    uint32_t cmdByteBegin = bitField(7, 1, 1);
    uint32_t cmdByteEnd = bitField(6, 1, 1);
    uint32_t cmdByteInternalCmd = bitField(2, 2, 3);
    uint32_t cmdByteSmBusCmd = bitField(0, 2, 1);
    uint32_t byteWidth = 2;
    NVDStatus status = NVD_FAIL;

    uint8_t cmdByte = cmdByteBegin | cmdByteInternalCmd | cmdByteSmBusCmd;
    status = smbusWr(addr, bus, saddr, cmdByte, byteWidth,
                     (csr.func & 0x7) | ((csr.dev & 0x1f) << 3), fd, cc, check);
    RETURN_ON_CSR_ERR(status, cc);

    cmdByte = cmdByteInternalCmd | cmdByteSmBusCmd;
    status = smbusWr(addr, bus, saddr, cmdByte, byteWidth, csr.offset, fd, cc,
                     check);
    RETURN_ON_CSR_ERR(status, cc);

    cmdByte = cmdByteInternalCmd | cmdByteSmBusCmd;
    status = smbusWr(addr, bus, saddr, cmdByte, byteWidth, (val >> 16) & 0xffff,
                     fd, cc, check);
    RETURN_ON_CSR_ERR(status, cc);

    cmdByte = cmdByteEnd | cmdByteInternalCmd | cmdByteSmBusCmd;
    status = smbusWr(addr, bus, saddr, cmdByte, byteWidth, val & 0xffff, fd, cc,
                     check);
    RETURN_ON_CSR_ERR(status, cc);

    return NVD_SUCCESS;
}
