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

typedef struct
{
    uint8_t opcode;
    uint8_t subopcode;
    uint32_t* indw;
    uint8_t indwSize;
} ApiParams;

const char dimmMap[16][10] = {
    // cpu, imc, channel, slot
    "dimm%x000", "dimm%x001", "dimm%x010", "dimm%x011",
    "dimm%x100", "dimm%x101", "dimm%x110", "dimm%x111",
    "dimm%x200", "dimm%x201", "dimm%x210", "dimm%x211",
    "dimm%x300", "dimm%x301", "dimm%x310", "dimm%x311",
};

#define API_CHECK_INTERVAL_MS 100
#define API_FAIL 0
#define API_MAX_RETRIES 20

uint16_t discovery(const int addr, const int fd, uint16_t* mask);

uint32_t api(const int addr, const uint8_t dimm, const ApiParams params,
             uint32_t* payload, const uint32_t payloadSize, const int fd,
             uint8_t* cc);

uint32_t identifyDimm(const int addr, const uint8_t dimm, uint32_t* payload,
                      const uint32_t payloadSize, const int fd, uint8_t* cc);

uint32_t getSramFwDebugLog(const int addr, const uint8_t dimm,
                           uint32_t* payload, const uint32_t payloadSize,
                           const int fd, uint8_t* cc);

uint32_t getSpiFwDebugLog(const int addr, const uint8_t dimm, uint32_t* payload,
                          const uint32_t payloadSize, const int fd,
                          uint8_t* cc);
