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

#pragma once

#include "common.hpp"
#include "crashdump.hpp"
#include "csrs.hpp"

/******************************************************************************
 *
 *   Structures
 *
 ******************************************************************************/

typedef struct
{
    char name[NVD_REG_NAME_LEN];
    uint8_t seg;
    uint8_t bus;
    uint8_t dev;
    uint8_t func;
    uint16_t offset;
    uint8_t size;
} RegEntry;

const RegEntry regs[] = {
    // Register name, Seg, Bus, Dev, Func, Offset
    {"smb_cmd_cfg_spd0", 0x0, 0xd, 0xb, 0, 0x80, 4},
    {"smb_cmd_cfg_spd1", 0x0, 0xd, 0xb, 1, 0x80, 4},
    {"smb_data_cfg_spd0", 0x0, 0xd, 0xb, 0, 0x88, 4},
    {"smb_data_cfg_spd1", 0x0, 0xd, 0xb, 1, 0x88, 4},
    {"smb_status_cfg_spd0", 0x0, 0xd, 0xb, 0, 0x84, 4},
    {"smb_status_cfg_spd1", 0x0, 0xd, 0xb, 1, 0x84, 4},
};

EPECIStatus regRd(int addr, RegEntry reg, uint32_t* val, int fd, uint8_t* cc);

EPECIStatus regWr(int addr, RegEntry reg, uint32_t val, int fd, uint8_t* cc);

NVDStatus smbusRd(int addr, uint8_t bus, uint8_t saddr, uint8_t command,
                  uint8_t bytewidth, uint32_t* val, int fd, uint8_t* cc,
                  bool check);

NVDStatus smbusWr(int addr, uint8_t bus, uint8_t saddr, uint8_t command,
                  uint8_t bytewidth, uint32_t val, int fd, uint8_t* cc,
                  bool check);

NVDStatus csrRd(int addr, uint8_t dimm, SmBusCsrEntry csr, uint32_t* val,
                int fd, uint8_t* cc, bool check);

NVDStatus csrWr(int addr, uint8_t dimm, SmBusCsrEntry csr, uint32_t val, int fd,
                uint8_t* cc, bool check);
