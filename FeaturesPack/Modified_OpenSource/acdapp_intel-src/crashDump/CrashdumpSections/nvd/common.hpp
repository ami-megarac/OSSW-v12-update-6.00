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

#include "crashdump.hpp"

typedef enum
{
    NVD_SUCCESS = 0,
    NVD_FAIL,
    NVD_REG_FAIL,
    NVD_SMB_FAIL,
    NVD_SMB_PECI_FAIL,
    NVD_SMB_STATUS_CHECK_FAIL,
    NVD_CSR_FAIL,
} NVDStatus;

typedef enum
{
    ekv = 0x8086097a,
    bwv = 0x8086097b,
} NVDIDS;

#define SMBUS_TIMEOUT_INTERVAL_MS 1
#define SMBUS_TIMEOUT_MS 10
#define MAX_DIMM_PER_BUS 8
#define BASE_ADDR 0xb0
#define NVD_REG_NAME_LEN 34
#define NVD_UINT32_FMT "0x%" PRIx32 ""
#define NVD_NA "N/A"
