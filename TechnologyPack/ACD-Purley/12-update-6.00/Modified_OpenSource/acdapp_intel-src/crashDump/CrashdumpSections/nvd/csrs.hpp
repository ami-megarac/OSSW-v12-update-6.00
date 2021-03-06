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

// Reference: Barlow Pass Controller EDS doc#610373
typedef struct
{
    char name[NVD_REG_NAME_LEN];
    uint8_t dev;
    uint8_t func;
    uint16_t offset;
} SmBusCsrEntry;

// Notes: The list order is important for accessing register BDF correctly.
const SmBusCsrEntry csrs[] = {
    {"d_ddrt_training_en", 0, 4, 0x8},
    {"d_ddrt_training_mpr", 0, 4, 0x1c},
    {"d_ddrt_training_mr3", 0, 4, 0x20},
    {"d_ddrt_training_other_en", 0, 4, 0xc},
    {"d_func_defeature1", 0, 4, 0x130},
    {"d_func_defeature2", 0, 4, 0x134},
    {"d_fw_status", 0, 4, 0x0},
    {"d_fw_status_h", 0, 4, 0x4},
    {"d_mc_acc_info", 0, 4, 0x38},
    {"d_mc_ctrl", 0, 4, 0x34},
    {"d_mc_rdata", 0, 4, 0x44},
    {"d_mc_stat", 0, 4, 0x40},
    {"d_mc_timeout", 0, 4, 0x3c},
    {"d_mc_wdata", 0, 4, 0x30},
    {"d_read_credit", 0, 4, 0x58},
    {"d_rst_mask", 0, 4, 0x14c},
    {"da_cxfifo_trainingreset", 0, 3, 0x800},
    {"da_ddrio_cmden", 0, 2, 0x70},
    {"da_ddrio_init_control", 0, 2, 0x6c},
    {"da_ddrt_dq_bcom_ctrl", 0, 2, 0x120},
    {"da_ddrt_latency", 0, 2, 0x114},
    {"da_ddrt_mpr3_page0", 0, 2, 0x20},
    {"da_ddrt_rx_dq_swz0", 0, 2, 0x38},
    {"da_ddrt_rx_dq_swz1", 0, 2, 0x3c},
    {"da_ddrt_rx_dq_swz2", 0, 2, 0x40},
    {"da_ddrt_rx_dq_swz3", 0, 2, 0x44},
    {"da_ddrt_rx_dq_swz4", 0, 2, 0x48},
    {"da_ddrt_training_defeature0", 0, 2, 0x300},
    {"da_ddrt_training_en", 0, 2, 0x30},
    {"da_ddrt_training_mr0", 0, 2, 0x0},
    {"da_ddrt_training_mr1", 0, 2, 0x4},
    {"da_ddrt_training_mr2", 0, 2, 0x8},
    {"da_ddrt_training_mr3", 0, 2, 0xc},
    {"da_ddrt_training_other_en", 0, 2, 0x34},
    {"da_ddrt_training_rc06", 0, 2, 0xa0},
    {"da_ddrt_training_rc0c", 0, 2, 0xb8},
    {"da_ddrt_training_rc0e", 0, 2, 0xc0},
    {"da_ddrt_training_rc0f", 0, 2, 0xc4},
    {"da_ddrt_training_rc4x", 0, 2, 0xd4},
    {"da_ddrt_training_rc6x", 0, 2, 0xdc},
    {"da_ddrt_tx_dq_swz0", 0, 2, 0x4c},
    {"da_ddrt_tx_dq_swz1", 0, 2, 0x50},
    {"da_ddrt_tx_dq_swz2", 0, 2, 0x54},
    {"da_ddrt_tx_dq_swz3", 0, 2, 0x58},
    {"da_ddrt_tx_dq_swz4", 0, 2, 0x5c},
    {"da_ecc_enable", 0, 2, 0x7c},
    {"da_emask", 0, 2, 0x510},
    {"da_rd_scr_lfsr", 0, 2, 0x104},
    {"da_wcrd_cnt", 0, 2, 0x13c},
    {"da_wr_scr_lfsr_h", 0, 2, 0x10c},
    {"da_wr_scr_lfsr_l", 0, 2, 0x108},
    {"da_wr_scramble_seed_sel", 0, 2, 0x110},
    {"da_write_credit", 0, 2, 0x128},
    {"dn_crd_cnt", 0, 4, 0x54},
    {"dn_ecc_enable", 0, 4, 0x48},
    {"dn_emask", 0, 4, 0x180},
    {"dn_spare_ctrl", 0, 4, 0x704},
    {"drdp_wcrd_config", 0, 4, 0x50},
    {"ds_ddrt_training_en", 0, 4, 0x928},
    {"fnv_debug_lock", 1, 4, 0x48},
    {"mb_smbus_abort", 0, 4, 0x920},
    {"mb_smbus_cmd", 0, 4, 0x804},
    {"mb_smbus_input_payload_0", 0, 4, 0x818},
    {"mb_smbus_input_payload_1", 0, 4, 0x81c},
    {"mb_smbus_input_payload_2", 0, 4, 0x820},
    {"mb_smbus_input_payload_3", 0, 4, 0x824},
    {"mb_smbus_input_payload_4", 0, 4, 0x828},
    {"mb_smbus_input_payload_5", 0, 4, 0x82c},
    {"mb_smbus_input_payload_6", 0, 4, 0x830},
    {"mb_smbus_input_payload_7", 0, 4, 0x834},
    {"mb_smbus_input_payload_8", 0, 4, 0x838},
    {"mb_smbus_input_payload_9", 0, 4, 0x83c},
    {"mb_smbus_input_payload_10", 0, 4, 0x840},
    {"mb_smbus_input_payload_11", 0, 4, 0x844},
    {"mb_smbus_input_payload_12", 0, 4, 0x848},
    {"mb_smbus_input_payload_13", 0, 4, 0x84c},
    {"mb_smbus_input_payload_14", 0, 4, 0x850},
    {"mb_smbus_input_payload_15", 0, 4, 0x854},
    {"mb_smbus_input_payload_16", 0, 4, 0x858},
    {"mb_smbus_input_payload_17", 0, 4, 0x85c},
    {"mb_smbus_input_payload_18", 0, 4, 0x860},
    {"mb_smbus_input_payload_19", 0, 4, 0x864},
    {"mb_smbus_input_payload_20", 0, 4, 0x868},
    {"mb_smbus_input_payload_21", 0, 4, 0x86c},
    {"mb_smbus_input_payload_22", 0, 4, 0x870},
    {"mb_smbus_input_payload_23", 0, 4, 0x874},
    {"mb_smbus_input_payload_24", 0, 4, 0x878},
    {"mb_smbus_input_payload_25", 0, 4, 0x87c},
    {"mb_smbus_input_payload_26", 0, 4, 0x880},
    {"mb_smbus_input_payload_27", 0, 4, 0x884},
    {"mb_smbus_input_payload_28", 0, 4, 0x888},
    {"mb_smbus_input_payload_29", 0, 4, 0x88c},
    {"mb_smbus_input_payload_30", 0, 4, 0x890},
    {"mb_smbus_input_payload_31", 0, 4, 0x894},
    {"mb_smbus_output_payload_0", 0, 4, 0x8a0},
    {"mb_smbus_output_payload_1", 0, 4, 0x8a4},
    {"mb_smbus_output_payload_2", 0, 4, 0x8a8},
    {"mb_smbus_output_payload_3", 0, 4, 0x8ac},
    {"mb_smbus_output_payload_4", 0, 4, 0x8b0},
    {"mb_smbus_output_payload_5", 0, 4, 0x8b4},
    {"mb_smbus_output_payload_6", 0, 4, 0x8b8},
    {"mb_smbus_output_payload_7", 0, 4, 0x8bc},
    {"mb_smbus_output_payload_8", 0, 4, 0x8c0},
    {"mb_smbus_output_payload_9", 0, 4, 0x8c4},
    {"mb_smbus_output_payload_10", 0, 4, 0x8c8},
    {"mb_smbus_output_payload_11", 0, 4, 0x8cc},
    {"mb_smbus_output_payload_12", 0, 4, 0x8d0},
    {"mb_smbus_output_payload_13", 0, 4, 0x8d4},
    {"mb_smbus_output_payload_14", 0, 4, 0x8d8},
    {"mb_smbus_output_payload_15", 0, 4, 0x8dc},
    {"mb_smbus_output_payload_16", 0, 4, 0x8e0},
    {"mb_smbus_output_payload_17", 0, 4, 0x8e4},
    {"mb_smbus_output_payload_18", 0, 4, 0x8e8},
    {"mb_smbus_output_payload_19", 0, 4, 0x8ec},
    {"mb_smbus_output_payload_20", 0, 4, 0x8f0},
    {"mb_smbus_output_payload_21", 0, 4, 0x8f4},
    {"mb_smbus_output_payload_22", 0, 4, 0x8f8},
    {"mb_smbus_output_payload_23", 0, 4, 0x8fc},
    {"mb_smbus_output_payload_24", 0, 4, 0x900},
    {"mb_smbus_output_payload_25", 0, 4, 0x904},
    {"mb_smbus_output_payload_26", 0, 4, 0x908},
    {"mb_smbus_output_payload_27", 0, 4, 0x90c},
    {"mb_smbus_output_payload_28", 0, 4, 0x910},
    {"mb_smbus_output_payload_29", 0, 4, 0x914},
    {"mb_smbus_output_payload_30", 0, 4, 0x918},
    {"mb_smbus_output_payload_31", 0, 4, 0x91c},
    {"mb_smbus_status", 0, 4, 0x898},
    {"msc_fnvio_ddrtdll_csr", 1, 4, 0x1b8},
    {"msc_fnvio_init_control_0", 1, 4, 0x180},
    {"msc_fnvio_init_control_1", 1, 4, 0x188},
    {"msc_pll_ctrl", 1, 4, 0x198},
    {"msc_sxp_pll_ctrl1", 1, 4, 0x194},
    {"msc_temp_crit_csr", 1, 4, 0x74},
    {"revision_mfg_id", 1, 4, 0xc},
    {"vendor_device_id", 1, 4, 0x8},
};

// Notes: The list order is important for accessing register BDF correctly.
typedef enum
{
    d_ddrt_training_en,
    d_ddrt_training_mpr,
    d_ddrt_training_mr3,
    d_ddrt_training_other_en,
    d_func_defeature1,
    d_func_defeature2,
    d_fw_status,
    d_fw_status_h,
    d_mc_acc_info,
    d_mc_ctrl,
    d_mc_rdata,
    d_mc_stat,
    d_mc_timeout,
    d_mc_wdata,
    d_read_credit,
    d_rst_mask,
    da_cxfifo_trainingreset,
    da_ddrio_cmden,
    da_ddrio_init_control,
    da_ddrt_dq_bcom_ctrl,
    da_ddrt_latency,
    da_ddrt_mpr3_page0,
    da_ddrt_rx_dq_swz0,
    da_ddrt_rx_dq_swz1,
    da_ddrt_rx_dq_swz2,
    da_ddrt_rx_dq_swz3,
    da_ddrt_rx_dq_swz4,
    da_ddrt_training_defeature0,
    da_ddrt_training_en,
    da_ddrt_training_mr0,
    da_ddrt_training_mr1,
    da_ddrt_training_mr2,
    da_ddrt_training_mr3,
    da_ddrt_training_other_en,
    da_ddrt_training_rc06,
    da_ddrt_training_rc0c,
    da_ddrt_training_rc0e,
    da_ddrt_training_rc0f,
    da_ddrt_training_rc4x,
    da_ddrt_training_rc6x,
    da_ddrt_tx_dq_swz0,
    da_ddrt_tx_dq_swz1,
    da_ddrt_tx_dq_swz2,
    da_ddrt_tx_dq_swz3,
    da_ddrt_tx_dq_swz4,
    da_ecc_enable,
    da_emask,
    da_rd_scr_lfsr,
    da_wcrd_cnt,
    da_wr_scr_lfsr_h,
    da_wr_scr_lfsr_l,
    da_wr_scramble_seed_sel,
    da_write_credit,
    dn_crd_cnt,
    dn_ecc_enable,
    dn_emask,
    dn_spare_ctrl,
    drdp_wcrd_config,
    ds_ddrt_training_en,
    fnv_debug_lock,
    mb_smbus_abort,
    mb_smbus_cmd,
    mb_smbus_input_payload_0,
    mb_smbus_input_payload_1,
    mb_smbus_input_payload_2,
    mb_smbus_input_payload_3,
    mb_smbus_input_payload_4,
    mb_smbus_input_payload_5,
    mb_smbus_input_payload_6,
    mb_smbus_input_payload_7,
    mb_smbus_input_payload_8,
    mb_smbus_input_payload_9,
    mb_smbus_input_payload_10,
    mb_smbus_input_payload_11,
    mb_smbus_input_payload_12,
    mb_smbus_input_payload_13,
    mb_smbus_input_payload_14,
    mb_smbus_input_payload_15,
    mb_smbus_input_payload_16,
    mb_smbus_input_payload_17,
    mb_smbus_input_payload_18,
    mb_smbus_input_payload_19,
    mb_smbus_input_payload_20,
    mb_smbus_input_payload_21,
    mb_smbus_input_payload_22,
    mb_smbus_input_payload_23,
    mb_smbus_input_payload_24,
    mb_smbus_input_payload_25,
    mb_smbus_input_payload_26,
    mb_smbus_input_payload_27,
    mb_smbus_input_payload_28,
    mb_smbus_input_payload_29,
    mb_smbus_input_payload_30,
    mb_smbus_input_payload_31,
    mb_smbus_output_payload_0,
    mb_smbus_output_payload_1,
    mb_smbus_output_payload_2,
    mb_smbus_output_payload_3,
    mb_smbus_output_payload_4,
    mb_smbus_output_payload_5,
    mb_smbus_output_payload_6,
    mb_smbus_output_payload_7,
    mb_smbus_output_payload_8,
    mb_smbus_output_payload_9,
    mb_smbus_output_payload_10,
    mb_smbus_output_payload_11,
    mb_smbus_output_payload_12,
    mb_smbus_output_payload_13,
    mb_smbus_output_payload_14,
    mb_smbus_output_payload_15,
    mb_smbus_output_payload_16,
    mb_smbus_output_payload_17,
    mb_smbus_output_payload_18,
    mb_smbus_output_payload_19,
    mb_smbus_output_payload_20,
    mb_smbus_output_payload_21,
    mb_smbus_output_payload_22,
    mb_smbus_output_payload_23,
    mb_smbus_output_payload_24,
    mb_smbus_output_payload_25,
    mb_smbus_output_payload_26,
    mb_smbus_output_payload_27,
    mb_smbus_output_payload_28,
    mb_smbus_output_payload_29,
    mb_smbus_output_payload_30,
    mb_smbus_output_payload_31,
    mb_smbus_status,
    msc_fnvio_ddrtdll_csr,
    msc_fnvio_init_control_0,
    msc_fnvio_init_control_1,
    msc_pll_ctrl,
    msc_sxp_pll_ctrl1,
    msc_temp_crit_csr,
    revision_mfg_id,
    vendor_device_id,
} SMBUS_CSRS;
