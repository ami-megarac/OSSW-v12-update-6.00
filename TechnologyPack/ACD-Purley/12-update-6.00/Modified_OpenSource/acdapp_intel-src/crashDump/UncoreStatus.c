/*
// Copyright (C) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions
// and limitations under the License.
//
//
// SPDX-License-Identifier: Apache-2.0
*/

#include "common.h"
#ifndef SPX_BMC_ACD
#include <cJSON.h>
#endif
#include "UncoreStatus.h"

extern BOOL IsCpuPresent(UINT8 CpuNum);

static const SUncoreStatusRegPci sUncoreStatusPci[] =
{
	// Register,                                                Bus,    Dev,    Func,   Offset, Size
	{ "pxp_dmi_plat",                          					0,		0,		0,		0x000d,  US_REG_BYTE },
	{ "pxp_dmi_hdr",                           					0,		0,		0,		0x000e,  US_REG_BYTE },
	{ "pxp_b0d00f0_bist",                      					0,		0,		0,		0x000f,  US_REG_BYTE },
	{ "pxp_b0d00f0_secsts",                    					0,		0,		0,		0x001e,  US_REG_WORD },
	{ "pxp_dmi_intl",                          					0,		0,		0,		0x003c,  US_REG_BYTE },
	{ "pxp_dmi_intpin",                        					0,		0,		0,		0x003d,  US_REG_BYTE },
	{ "pxp_b0d00f0_scapid",                    					0,		0,		0,		0x0040,  US_REG_BYTE },
	{ "pxp_b0d00f0_snxtptr",                   					0,		0,		0,		0x0041,  US_REG_BYTE },
	{ "pxp_b0d00f0_svid",                      					0,		0,		0,		0x0044,  US_REG_WORD },
	{ "pxp_dmi_dmircbar",                      					0,		0,		0,		0x0050, US_REG_DWORD },
	{ "pxp_dmi_msicapid",                      					0,		0,		0,		0x0060,  US_REG_BYTE },
	{ "pxp_dmi_msinxtptr",                     					0,		0,		0,		0x0061,  US_REG_BYTE },
	{ "pxp_dmi_msimsgctl",                     					0,		0,		0,		0x0062,  US_REG_WORD },
	{ "pxp_dmi_msgadr",                        					0,		0,		0,		0x0064, US_REG_DWORD },
	{ "pxp_dmi_msgdat",                        					0,		0,		0,		0x0068, US_REG_DWORD },
	{ "pxp_dmi_msimsk",                        					0,		0,		0,		0x006c, US_REG_DWORD },
	{ "pxp_dmi_msipending",                    					0,		0,		0,		0x0070, US_REG_DWORD },
	{ "pxp_dmi_pxpcapid",                      					0,		0,		0,		0x0090,  US_REG_BYTE },
	{ "pxp_dmi_pxpnxtptr",                     					0,		0,		0,		0x0091,  US_REG_BYTE },
	{ "pxp_dmi_pxpcap",                        					0,		0,		0,		0x0092,  US_REG_WORD },
	{ "pxp_b0d00f0_devsts",                    					0,		0,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b0d00f0_lnkcap",                    					0,		0,		0,		0x009c, US_REG_DWORD },
	{ "pxp_b0d00f0_lnkcon",                    					0,		0,		0,		0x00a0,  US_REG_WORD },
	{ "pxp_b0d00f0_lnksts",                    					0,		0,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b0d00f0_sltcap",                    					0,		0,		0,		0x00a4, US_REG_DWORD },
	{ "pxp_b0d00f0_sltcon",                    					0,		0,		0,		0x00a8,  US_REG_WORD },
	{ "pxp_b0d00f0_sltsts",                    					0,		0,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_dmi_rootcon",                       					0,		0,		0,		0x00ac,  US_REG_WORD },
	{ "pxp_dmi_rootcap",                       					0,		0,		0,		0x00ae,  US_REG_WORD },
	{ "pxp_b0d00f0_lnkcap2",                   					0,		0,		0,		0x00bc, US_REG_DWORD },
	{ "pxp_b0d00f0_lnkcon2",                   					0,		0,		0,		0x00c0,  US_REG_WORD },
	{ "pxp_b0d00f0_lnksts2",                   					0,		0,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_dmi_ppd",                           					0,		0,		0,		0x00d4,  US_REG_BYTE },
	{ "pxp_dmi_pmcap",                         					0,		0,		0,		0x00e0, US_REG_DWORD },
	{ "pxp_dmi_pmcsr",                         					0,		0,		0,		0x00e4, US_REG_DWORD },
	{ "pxp_dmi_devsts",                        					0,		0,		0,		0x00f2,  US_REG_WORD },
	{ "pxp_dmi_apicbase",                      					0,		0,		0,		0x0140,  US_REG_WORD },
	{ "pxp_dmi_apiclimit",                     					0,		0,		0,		0x0142,  US_REG_WORD },
	{ "pxp_b0d00f0_uncerrsts",                 					0,		0,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b0d00f0_uncerrmsk",                 					0,		0,		0,		0x0150, US_REG_DWORD },
	{ "pxp_b0d00f0_uncerrsev",                 					0,		0,		0,		0x0154, US_REG_DWORD },
	{ "pxp_b0d00f0_corerrsts",                 					0,		0,		0,		0x0158, US_REG_DWORD },
	{ "pxp_dmi_hdrlog0",                       					0,		0,		0,		0x0164, US_REG_DWORD },
	{ "pxp_dmi_hdrlog1",                       					0,		0,		0,		0x0168, US_REG_DWORD },
	{ "pxp_dmi_hdrlog2",                       					0,		0,		0,		0x016c, US_REG_DWORD },
	{ "pxp_dmi_hdrlog3",                       					0,		0,		0,		0x0170, US_REG_DWORD },
	{ "pxp_b0d00f0_rperrcmd",                  					0,		0,		0,		0x0174, US_REG_DWORD },
	{ "pxp_b0d00f0_rperrsts",                  					0,		0,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b0d00f0_errsid",                    					0,		0,		0,		0x017c, US_REG_DWORD },
	{ "pxp_dmi_perfctrlsts_0",                 					0,		0,		0,		0x0180, US_REG_DWORD },
	{ "pxp_dmi_perfctrlsts_1",                 					0,		0,		0,		0x0184, US_REG_DWORD },
	{ "pxp_b0d00f0_miscctrlsts_0",             					0,		0,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b0d00f0_miscctrlsts_1",             					0,		0,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b0d00f0_pcie_iou_bif_ctrl",         					0,		0,		0,		0x0190,  US_REG_WORD },
	{ "pxp_dmi_dmictrl",                       					0,		0,		0,		0x01a0, US_REG_QWORD },
	{ "pxp_dmi_dmists",                        					0,		0,		0,		0x01a8, US_REG_DWORD },
	{ "pxp_dmi_lnksts",                        					0,		0,		0,		0x01b2,  US_REG_WORD },
	{ "pxp_dmi_lnksts2",                       					0,		0,		0,		0x01c2,  US_REG_WORD },
	{ "pxp_b0d00f0_pcie_ler_ss_ctrlsts",       					0,		0,		0,		0x01e4, US_REG_DWORD },
	{ "pxp_b0d00f0_xpcorerrsts",               					0,		0,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b0d00f0_xpcorerrmsk",               					0,		0,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b0d00f0_xpuncerrsts",               					0,		0,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b0d00f0_xpuncerrmsk",               					0,		0,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b0d00f0_xpuncerrsev",               					0,		0,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b0d00f0_xpuncerrptr",               					0,		0,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b0d00f0_uncedmask",                 					0,		0,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b0d00f0_rpedmask",                  					0,		0,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b0d00f0_xpuncedmask",               					0,		0,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b0d00f0_xpcoredmask",               					0,		0,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b0d00f0_xpglberrsts",               					0,		0,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b0d00f0_xpglberrptr",               					0,		0,		0,		0x0232,  US_REG_WORD },
	{ "pxp_dmi_pxp2cap",                       					0,		0,		0,		0x0250, US_REG_DWORD },
	{ "pxp_b0d00f0_lnkcon3",                   					0,		0,		0,		0x0254, US_REG_DWORD },
	{ "pxp_b0d00f0_lnerrsts",                  					0,		0,		0,		0x0258, US_REG_DWORD },
	{ "pxp_dmi_ln0eq",                         					0,		0,		0,		0x025c,  US_REG_WORD },
	{ "pxp_dmi_ln1eq",                         					0,		0,		0,		0x025e,  US_REG_WORD },
	{ "pxp_dmi_ln2eq",                         					0,		0,		0,		0x0260,  US_REG_WORD },
	{ "pxp_dmi_ln3eq",                         					0,		0,		0,		0x0262,  US_REG_WORD },
	{ "pxp_b0d00f0_ler_cap",                   					0,		0,		0,		0x0280, US_REG_DWORD },
	{ "pxp_b0d00f0_ler_hdr",                   					0,		0,		0,		0x0284, US_REG_DWORD },
	{ "pxp_b0d00f0_ler_ctrlsts",               					0,		0,		0,		0x0288, US_REG_DWORD },
	{ "pxp_dmi_ler_uncerrmsk",                 					0,		0,		0,		0x028c, US_REG_DWORD },
	{ "pxp_dmi_ler_xpuncerrmsk",               					0,		0,		0,		0x0290, US_REG_DWORD },
	{ "pxp_dmi_ler_rperrmsk",                  					0,		0,		0,		0x0294, US_REG_DWORD },
	{ "pxp_b0d00f0_rppioerr_cap",              					0,		0,		0,		0x0298, US_REG_DWORD },
	{ "pxp_dmi_rppioerr_hdr",                  					0,		0,		0,		0x029c, US_REG_DWORD },
	{ "pxp_b0d00f0_rppioerr_hf",               					0,		0,		0,		0x02a0, US_REG_DWORD },
	{ "pxp_b0d00f0_rppioerr_status",           					0,		0,		0,		0x02a4, US_REG_DWORD },
	{ "pxp_b0d00f0_rppioerr_mask",             					0,		0,		0,		0x02a8, US_REG_DWORD },
	{ "pxp_dmi_rppioerr_hdrlog0",              					0,		0,		0,		0x02ac, US_REG_DWORD },
	{ "pxp_dmi_rppioerr_hdrlog1",              					0,		0,		0,		0x02b0, US_REG_DWORD },
	{ "pxp_dmi_rppioerr_hdrlog2",              					0,		0,		0,		0x02b4, US_REG_DWORD },
	{ "pxp_dmi_rppioerr_hdrlog3",              					0,		0,		0,		0x02b8, US_REG_DWORD },
	{ "pxp_dmi_xppmdfxmat0",                   					0,		0,		0,		0x02f0, US_REG_DWORD },
	{ "pxp_dmi_xppmdfxmat1",                   					0,		0,		0,		0x02f4, US_REG_DWORD },
	{ "pxp_dmi_xppmdfxmsk0",                   					0,		0,		0,		0x02f8, US_REG_DWORD },
	{ "pxp_dmi_xppmdfxmsk1",                   					0,		0,		0,		0x02fc, US_REG_DWORD },
	{ "pxp_dmi_mcast_cap_hdr",                 					0,		0,		0,		0x0300, US_REG_DWORD },
	{ "pxp_dmi_mcast_cap_ext",                 					0,		0,		0,		0x0304, US_REG_DWORD },
	{ "pxp_dmi_mcast_cap",                     					0,		0,		0,		0x030c,  US_REG_WORD },
	{ "pxp_dmi_mcast_ctrl",                    					0,		0,		0,		0x030e,  US_REG_WORD },
	{ "pxp_dmi_mcast_base",                    					0,		0,		0,		0x0310, US_REG_QWORD },
	{ "pxp_dmi_mcast_rcv",                     					0,		0,		0,		0x0318, US_REG_QWORD },
	{ "pxp_dmi_mcast_blk_all",                 					0,		0,		0,		0x0320, US_REG_QWORD },
	{ "pxp_dmi_mcast_blk_unt",                 					0,		0,		0,		0x0328, US_REG_QWORD },
	{ "pxp_dmi_mcast_overlay_bar",             					0,		0,		0,		0x0330, US_REG_QWORD },
	{ "pxp_dmi_xppmdl0",                       					0,		0,		0,		0x0480, US_REG_DWORD },
	{ "pxp_dmi_xppmdl1",                       					0,		0,		0,		0x0484, US_REG_DWORD },
	{ "pxp_dmi_xppmcl0",                       					0,		0,		0,		0x0488, US_REG_DWORD },
	{ "pxp_dmi_xppmcl1",                       					0,		0,		0,		0x048c, US_REG_DWORD },
	{ "pxp_dmi_xppmdh",                        					0,		0,		0,		0x0490,  US_REG_WORD },
	{ "pxp_dmi_xppmch",                        					0,		0,		0,		0x0492,  US_REG_WORD },
	{ "pxp_dmi_xppmr0",                        					0,		0,		0,		0x0494, US_REG_DWORD },
	{ "pxp_dmi_xppmr1",                        					0,		0,		0,		0x0498, US_REG_DWORD },
	{ "pxp_dmi_xppmevl0",                      					0,		0,		0,		0x049c, US_REG_DWORD },
	{ "pxp_dmi_xppmevl1",                      					0,		0,		0,		0x04a0, US_REG_DWORD },
	{ "pxp_dmi_xppmevh0",                      					0,		0,		0,		0x04a4, US_REG_DWORD },
	{ "pxp_dmi_xppmevh1",                      					0,		0,		0,		0x04a8, US_REG_DWORD },
	{ "pxp_dmi_xppmer0",                       					0,		0,		0,		0x04ac, US_REG_DWORD },
	{ "pxp_dmi_xppmer1",                       					0,		0,		0,		0x04b0, US_REG_DWORD },
	{ "pxp_dmi_plsr0",                         					0,		0,		0,		0x04b4, US_REG_DWORD },
	{ "pxp_dmi_plsr1",                         					0,		0,		0,		0x04b8, US_REG_DWORD },
	{ "pxp_dmi_plsr2",                         					0,		0,		0,		0x04bc, US_REG_DWORD },
	{ "pxp_b0d00f0_dlucntr",                   					0,		0,		0,		0x04c0, US_REG_DWORD },
	{ "pxp_b0d00f0_xpcorerrcounter",           					0,		0,		0,		0x04d0, US_REG_DWORD },
	{ "pxp_b0d00f0_xpcorerrthreshold",         					0,		0,		0,		0x04d4,  US_REG_WORD },
	{ "cb0_devsts",                            					0,		4,		0,		0x009a,  US_REG_WORD },
	{ "cb0_dmauncerrsts",                      					0,		4,		0,		0x0148, US_REG_DWORD },
	{ "cb1_devsts",                            					0,		4,		1,		0x009a,  US_REG_WORD },
	{ "cb2_devsts",                            					0,		4,		2,		0x009a,  US_REG_WORD },
	{ "cb3_devsts",                            					0,		4,		3,		0x009a,  US_REG_WORD },
	{ "cb4_devsts",                            					0,		4,		4,		0x009a,  US_REG_WORD },
	{ "cb5_devsts",                            					0,		4,		5,		0x009a,  US_REG_WORD },
	{ "cb6_devsts",                            					0,		4,		6,		0x009a,  US_REG_WORD },
	{ "cb7_devsts",                            					0,		4,		7,		0x009a,  US_REG_WORD },
	{ "iio_mmcfg_base_b0d05f0",                					0,		5,		0,		0x0090, US_REG_QWORD },
	{ "iio_mmcfg_limit_b0d05f0",               					0,		5,		0,		0x0098, US_REG_QWORD },
	{ "iio_tseg_b0d05f0",                      					0,		5,		0,		0x00a8, US_REG_QWORD },
	{ "iio_tolm_b0d05f0",                      					0,		5,		0,		0x00d0, US_REG_DWORD },
	{ "iio_tohm_0_b0d05f0",                    					0,		5,		0,		0x00d4, US_REG_DWORD },
	{ "iio_tohm_1_b0d05f0",                    					0,		5,		0,		0x00d8, US_REG_DWORD },
	{ "iio_ncmem_base_0_b0d05f0",              					0,		5,		0,		0x00e0, US_REG_DWORD },
	{ "iio_ncmem_base_1_b0d05f0",              					0,		5,		0,		0x00e4, US_REG_DWORD },
	{ "iio_ncmem_limit_0_b0d05f0",             					0,		5,		0,		0x00e8, US_REG_DWORD },
	{ "iio_ncmem_limit_1_b0d05f0",             					0,		5,		0,		0x00ec, US_REG_DWORD },
	{ "iio_mencmem_base_0_b0d05f0",            					0,		5,		0,		0x00f0, US_REG_DWORD },
	{ "iio_mencmem_base_1_b0d05f0",            					0,		5,		0,		0x00f4, US_REG_DWORD },
	{ "iio_mencmem_limit_0_b0d05f0",           					0,		5,		0,		0x00f8, US_REG_DWORD },
	{ "iio_mencmem_limit_1_b0d05f0",           					0,		5,		0,		0x00fc, US_REG_DWORD },
	{ "iio_vtuncerrsts_b0d05f0",               					0,		5,		0,		0x01a8, US_REG_DWORD },
	{ "iio_vtuncerrptr_b0d05f0",               					0,		5,		0,		0x01b4,  US_REG_BYTE },
	{ "iio_mmioh_non_interleave_b0d05f0",      					0,		5,		0,		0x0340, US_REG_QWORD },
	{ "iio_mmioh_interleave_b0d05f0",          					0,		5,		0,		0x0348, US_REG_QWORD },
	{ "b0d5f1_sltsts0",                        					0,		5,		1,		0x010e,  US_REG_WORD },
	{ "b0d5f1_sltsts1",                        					0,		5,		1,		0x011e,  US_REG_WORD },
	{ "b0d5f1_sltsts2",                        					0,		5,		1,		0x012e,  US_REG_WORD },
	{ "b0d5f1_sltsts3",                        					0,		5,		1,		0x013e,  US_REG_WORD },
	{ "iio_irpperrsv_b0d05f2",                 					0,		5,		2,		0x0080, US_REG_QWORD },
	{ "iio_pcierrsv_b0d05f2",                  					0,		5,		2,		0x0094, US_REG_DWORD },
	{ "iio_viral_b0d05f2",                     					0,		5,		2,		0x00a0, US_REG_DWORD },
	{ "iio_lnerrmask_b0d05f2",                 					0,		5,		2,		0x019c, US_REG_DWORD },
	{ "iio_lferrmask_b0d05f2",                 					0,		5,		2,		0x01a0, US_REG_DWORD },
	{ "iio_lcerrmask_b0d05f2",                 					0,		5,		2,		0x01a4, US_REG_DWORD },
	{ "iio_lcerrst_b0d05f2",                   					0,		5,		2,		0x01a8, US_REG_DWORD },
	{ "iio_lcferrst_b0d05f2",                  					0,		5,		2,		0x01ac, US_REG_DWORD },
	{ "iio_lcnerrst_b0d05f2",                  					0,		5,		2,		0x01b8, US_REG_DWORD },
	{ "iio_lnerrst_b0d05f2",                   					0,		5,		2,		0x01c0, US_REG_DWORD },
	{ "iio_lferrst_b0d05f2",                   					0,		5,		2,		0x01c4, US_REG_DWORD },
	{ "iio_lerrctl_b0d05f2",                   					0,		5,		2,		0x01c8, US_REG_DWORD },
	{ "iio_lsysst_b0d05f2",                    					0,		5,		2,		0x01cc, US_REG_DWORD },
	{ "iio_lsysctl_b0d05f2",                   					0,		5,		2,		0x01d0, US_REG_DWORD },
	{ "iio_sysmap_b0d05f2",                    					0,		5,		2,		0x01d4, US_REG_DWORD },
	{ "iio_lbitmap_b0d05f2",                   					0,		5,		2,		0x01d8, US_REG_DWORD },
	{ "iio_lfferrst_b0d05f2",                  					0,		5,		2,		0x01dc, US_REG_DWORD },
	{ "iio_lfnerrst_b0d05f2",                  					0,		5,		2,		0x01e8, US_REG_DWORD },
	{ "iio_lnferrst_b0d05f2",                  					0,		5,		2,		0x01ec, US_REG_DWORD },
	{ "iio_lnnerrst_b0d05f2",                  					0,		5,		2,		0x01f8, US_REG_DWORD },
	{ "iio_irpp0errst_b0d05f2",                					0,		5,		2,		0x0230, US_REG_DWORD },
	{ "iio_irpp0errctl_b0d05f2",               					0,		5,		2,		0x0234, US_REG_DWORD },
	{ "iio_irpp0fferrst_b0d05f2",              					0,		5,		2,		0x0238, US_REG_DWORD },
	{ "iio_irpp0fnerrst_b0d05f2",              					0,		5,		2,		0x023c, US_REG_DWORD },
	{ "iio_irpp0fferrhd0_b0d05f2",             					0,		5,		2,		0x0240, US_REG_DWORD },
	{ "iio_irpp0fferrhd1_b0d05f2",             					0,		5,		2,		0x0244, US_REG_DWORD },
	{ "iio_irpp0fferrhd2_b0d05f2",             					0,		5,		2,		0x0248, US_REG_DWORD },
	{ "iio_irpp0fferrhd3_b0d05f2",             					0,		5,		2,		0x024c, US_REG_DWORD },
	{ "iio_irpp0nferrst_b0d05f2",              					0,		5,		2,		0x0250, US_REG_DWORD },
	{ "iio_irpp0nnerrst_b0d05f2",              					0,		5,		2,		0x0254, US_REG_DWORD },
	{ "iio_irpp0errcntsel_b0d05f2",            					0,		5,		2,		0x0268, US_REG_DWORD },
	{ "iio_irpp0errcnt_b0d05f2",               					0,		5,		2,		0x026c, US_REG_DWORD },
	{ "iio_irpp0plsr0_b0d05f2",                					0,		5,		2,		0x0270, US_REG_DWORD },
	{ "iio_irpp0plsr1_b0d05f2",                					0,		5,		2,		0x0274, US_REG_DWORD },
	{ "iio_irpp0plsr2_b0d05f2",                					0,		5,		2,		0x0278, US_REG_DWORD },
	{ "iio_tcerrst_b0d05f2",                   					0,		5,		2,		0x0288, US_REG_DWORD },
	{ "iio_tcerrctl_b0d05f2",                  					0,		5,		2,		0x028c, US_REG_DWORD },
	{ "iio_itcerrsev_b0d05f2",                 					0,		5,		2,		0x0290, US_REG_DWORD },
	{ "iio_otcerrsev_b0d05f2",                 					0,		5,		2,		0x0294, US_REG_DWORD },
	{ "iio_tcerrcntsel_b0d05f2",               					0,		5,		2,		0x0298, US_REG_DWORD },
	{ "iio_tcerrcnt_b0d05f2",                  					0,		5,		2,		0x029c, US_REG_DWORD },
	{ "iio_itcfferrst_b0d05f2",                					0,		5,		2,		0x02a0, US_REG_DWORD },
	{ "iio_itcfferrhd0_b0d05f2",               					0,		5,		2,		0x02a4, US_REG_DWORD },
	{ "iio_itcfferrhd1_b0d05f2",               					0,		5,		2,		0x02a8, US_REG_DWORD },
	{ "iio_itcfferrhd2_b0d05f2",               					0,		5,		2,		0x02ac, US_REG_DWORD },
	{ "iio_itcfferrhd3_b0d05f2",               					0,		5,		2,		0x02b0, US_REG_DWORD },
	{ "iio_itcfnerrst_b0d05f2",                					0,		5,		2,		0x02b4, US_REG_DWORD },
	{ "iio_itcnferrst_b0d05f2",                					0,		5,		2,		0x02b8, US_REG_DWORD },
	{ "iio_itcnnerrst_b0d05f2",                					0,		5,		2,		0x02cc, US_REG_DWORD },
	{ "iio_otcfferrst_b0d05f2",                					0,		5,		2,		0x02d0, US_REG_DWORD },
	{ "iio_otcfferrhd0_b0d05f2",               					0,		5,		2,		0x02d4, US_REG_DWORD },
	{ "iio_otcfferrhd1_b0d05f2",               					0,		5,		2,		0x02d8, US_REG_DWORD },
	{ "iio_otcfferrhd2_b0d05f2",               					0,		5,		2,		0x02dc, US_REG_DWORD },
	{ "iio_otcfferrhd3_b0d05f2",               					0,		5,		2,		0x02e0, US_REG_DWORD },
	{ "iio_otcfnerrst_b0d05f2",                					0,		5,		2,		0x02e4, US_REG_DWORD },
	{ "iio_otcnferrst_b0d05f2",                					0,		5,		2,		0x02e8, US_REG_DWORD },
	{ "iio_otcnferrhd0_b0d05f2",               					0,		5,		2,		0x02ec, US_REG_DWORD },
	{ "iio_otcnnerrst_b0d05f2",                					0,		5,		2,		0x02fc, US_REG_DWORD },
	{ "iio_iioerrctl_b0d05f2",                 					0,		5,		2,		0x0304, US_REG_DWORD },
	{ "pxp_b0d07f0_devsts",                    					0,		7,		0,		0x004a,  US_REG_WORD },
	{ "pxp_b0d07f0_lnksts",                    					0,		7,		0,		0x0052,  US_REG_WORD },
	{ "pxp_b0d07f0_lnksts2",                   					0,		7,		0,		0x0072,  US_REG_WORD },
	{ "pxp_b0d07f0_sltsts2",                   					0,		7,		0,		0x007a,  US_REG_WORD },
	{ "pxp_b0d07f4_devsts",                    					0,		7,		4,		0x004a,  US_REG_WORD },
	{ "pxp_b0d07f4_lnksts",                    					0,		7,		4,		0x0052,  US_REG_WORD },
	{ "pxp_b0d07f4_lnksts2",                   					0,		7,		4,		0x0072,  US_REG_WORD },
	{ "pxp_b0d07f4_sltsts2",                   					0,		7,		4,		0x007a,  US_REG_WORD },
	{ "pxp_b0d07f7_devsts",                    					0,		7,		7,		0x004a,  US_REG_WORD },
	{ "pxp_b0d07f7_lnksts",                    					0,		7,		7,		0x0052,  US_REG_WORD },
	{ "pxp_b0d07f7_lnksts2",                   					0,		7,		7,		0x0072,  US_REG_WORD },
	{ "pxp_b0d07f7_sltsts2",                   					0,		7,		7,		0x007a,  US_REG_WORD },
	{ "pxp_b0d07f7_tswdbgerrstdis1",           					0,		7,		7,		0x035c, US_REG_DWORD },
	{ "ubox_ncevents_ubox_gl_err_cfg_cfg_b0d08f0",				0,		8,		0,		0x0090, US_REG_DWORD },
	{ "ubox_ncevents_ubox_cts_err_mask_cfg_b0d08f0",			0,		8,		0,		0x0094, US_REG_DWORD },
	{ "ubox_ncevents_ierrloggingreg_b0d08f0",  					0,		8,		0,		0x00a4, US_REG_DWORD },
	{ "ubox_ncevents_mcerrloggingreg_b0d08f0", 					0,		8,		0,		0x00a8, US_REG_DWORD },
	{ "ubox_ncevents_emca_core_csmi_log_cfg_b0d08f0",			0,		8,		0,		0x00b0, US_REG_DWORD },
	{ "ubox_ncevents_emca_core_msmi_log_cfg_b0d08f0",			0,		8,		0,		0x00b8, US_REG_DWORD },
	{ "ubox_ncevents_emca_en_core_ierr_to_msmi_cfg_b0d08f0",	0,		8,		0,		0x00d8, US_REG_DWORD },
	{ "ubox_ncevents_uboxerrsts_cfg_b0d08f0",  					0,		8,		0,		0x00e4, US_REG_DWORD },
	{ "ubox_ncevents_uboxerrctl_cfg_b0d08f0",  					0,		8,		0,		0x00e8, US_REG_DWORD },
	{ "ubox_ncevents_uboxerrmisc_cfg_b0d08f0", 					0,		8,		0,		0x00ec, US_REG_DWORD },
	{ "ubox_ncevents_uboxerrmisc2_cfg_b0d08f0",					0,		8,		0,		0x00f0, US_REG_DWORD },
	{ "ubox_ncevents_iio_gsysst_cfg_b0d08f0",  					0,		8,		0,		0x0204, US_REG_DWORD },
	{ "ubox_ncevents_iio_gsysctl_cfg_b0d08f0", 					0,		8,		0,		0x0208, US_REG_DWORD },
	{ "ubox_ncevents_iio_errpinctl_cfg_b0d08f0",				0,		8,		0,		0x020c, US_REG_DWORD },
	{ "ubox_ncevents_iio_errpinsts_cfg_b0d08f0",				0,		8,		0,		0x0210, US_REG_DWORD },
	{ "ubox_ncevents_iio_errpindat_cfg_b0d08f0",				0,		8,		0,		0x0214, US_REG_DWORD },
	{ "ubox_ncevents_iio_gf_errst_cfg_b0d08f0",					0,		8,		0,		0x0218, US_REG_DWORD },
	{ "ubox_ncevents_iio_gf_1st_errst_cfg_b0d08f0",				0,		8,		0,		0x021c, US_REG_DWORD },
	{ "ubox_ncevents_iio_gf_nxt_errst_cfg_b0d08f0",				0,		8,		0,		0x0220, US_REG_DWORD },
	{ "ubox_ncevents_iio_gnf_errst_cfg_b0d08f0",				0,		8,		0,		0x0224, US_REG_DWORD },
	{ "ubox_ncevents_iio_gnf_1st_errst_cfg_b0d08f0",			0,		8,		0,		0x0228, US_REG_DWORD },
	{ "ubox_ncevents_iio_gnf_nxt_errst_cfg_b0d08f0",			0,		8,		0,		0x022c, US_REG_DWORD },
	{ "ubox_ncevents_iio_gc_errst_cfg_b0d08f0",					0,		8,		0,		0x0230, US_REG_DWORD },
	{ "ubox_ncevents_iio_gc_1st_errst_cfg_b0d08f0",				0,		8,		0,		0x0234, US_REG_DWORD },
	{ "ubox_ncevents_iio_gc_nxt_errst_cfg_b0d08f0",				0,		8,		0,		0x0238, US_REG_DWORD },
	{ "biosnonstickyscratchpad7",              					0,		8,		2,		0x007c, US_REG_DWORD },
	{ "biosnonstickyscratchpad8",              					0,		8,		2,		0x0080, US_REG_DWORD },
	{ "biosnonstickyscratchpad9",              					0,		8,		2,		0x0084, US_REG_DWORD },
	{ "biosnonstickyscratchpad10",             					0,		8,		2,		0x0088, US_REG_DWORD },
	{ "biosnonstickyscratchpad11",             					0,		8,		2,		0x008c, US_REG_DWORD },
	{ "biosnonstickyscratchpad12",             					0,		8,		2,		0x0090, US_REG_DWORD },
	{ "biosnonstickyscratchpad13",             					0,		8,		2,		0x0094, US_REG_DWORD },
	{ "biosnonstickyscratchpad14",             					0,		8,		2,		0x0098, US_REG_DWORD },
	{ "biosnonstickyscratchpad15",             					0,		8,		2,		0x009c, US_REG_DWORD },
	{ "biosscratchpad0",                       					0,		8,		2,		0x00a0, US_REG_DWORD },
	{ "biosscratchpad1",                       					0,		8,		2,		0x00a4, US_REG_DWORD },
	{ "biosscratchpad2",                       					0,		8,		2,		0x00a8, US_REG_DWORD },
	{ "biosscratchpad3",                       					0,		8,		2,		0x00ac, US_REG_DWORD },
	{ "biosscratchpad4",                       					0,		8,		2,		0x00b0, US_REG_DWORD },
	{ "biosscratchpad5",                       					0,		8,		2,		0x00b4, US_REG_DWORD },
	{ "biosscratchpad6",                       					0,		8,		2,		0x00b8, US_REG_DWORD },
	{ "biosscratchpad7",                       					0,		8,		2,		0x00bc, US_REG_DWORD },
	{ "biosnonstickyscratchpad0",              					0,		8,		2,		0x00c0, US_REG_DWORD },
	{ "biosnonstickyscratchpad1",              					0,		8,		2,		0x00c4, US_REG_DWORD },
	{ "biosnonstickyscratchpad2",              					0,		8,		2,		0x00c8, US_REG_DWORD },
	{ "ubox_ncdecs_cpubusno_cfg_b0d08f2",      					0,		8,		2,		0x00cc, US_REG_DWORD },
	{ "ubox_ncdecs_cpubusno1_cfg_b0d08f2",     					0,		8,		2,		0x00d0, US_REG_DWORD },
	{ "ubox_ncdecs_cpubusno_valid_cfg_b0d08f2",					0,		8,		2,		0x00d4, US_REG_DWORD },
	{ "ubox_ncuracu_smictrl_cfg_b0d08f2",      					0,		8,		2,		0x00d8, US_REG_DWORD },
	{ "ubox_ncdecs_mmcfg_rule_cfg_b0d08f2",    					0,		8,		2,		0x00dc, US_REG_DWORD },
	{ "biosnonstickyscratchpad3",              					0,		8,		2,		0x00e8, US_REG_DWORD },
	{ "biosnonstickyscratchpad4",              					0,		8,		2,		0x00ec, US_REG_DWORD },
	{ "biosnonstickyscratchpad5",              					0,		8,		2,		0x00f4, US_REG_DWORD },
	{ "biosnonstickyscratchpad6",              					0,		8,		2,		0x00f8, US_REG_DWORD },
	{ "ntb_b1d00f0_lnksts",                    					1,		0,		0,		0x01a2,  US_REG_WORD },
	{ "ntb_b1d00f0_sltsts",                    					1,		0,		0,		0x01aa,  US_REG_WORD },
	{ "ntb_b1d00f0_lnksts2",                   					1,		0,		0,		0x01c2,  US_REG_WORD },
	{ "pxp_b1d00f0_pbus",                      					1,		0,		0,		0x0018,  US_REG_BYTE },
	{ "pxp_b1d00f0_secbus",                    					1,		0,		0,		0x0019,  US_REG_BYTE },
	{ "pxp_b1d00f0_subbus",                    					1,		0,		0,		0x001a,  US_REG_BYTE },
	{ "pxp_b1d00f0_iobas",                     					1,		0,		0,		0x001c,  US_REG_BYTE },
	{ "pxp_b1d00f0_iolim",                     					1,		0,		0,		0x001d,  US_REG_BYTE },
	{ "pxp_b1d00f0_secsts",                    					1,		0,		0,		0x001e,  US_REG_WORD },
	{ "pxp_b1d00f0_mbas",                      					1,		0,		0,		0x0020,  US_REG_WORD },
	{ "pxp_b1d00f0_mlim",                      					1,		0,		0,		0x0022,  US_REG_WORD },
	{ "pxp_b1d00f0_pbas",                      					1,		0,		0,		0x0024,  US_REG_WORD },
	{ "pxp_b1d00f0_plim",                      					1,		0,		0,		0x0026,  US_REG_WORD },
	{ "pxp_b1d00f0_pbasu",                     					1,		0,		0,		0x0028, US_REG_DWORD },
	{ "pxp_b1d00f0_plimu",                     					1,		0,		0,		0x002c, US_REG_DWORD },
	{ "pxp_b1d00f0_devsts",                    					1,		0,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b1d00f0_lnksts",                    					1,		0,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b1d00f0_sltsts",                    					1,		0,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_b1d00f0_lnksts2",                   					1,		0,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_b1d00f0_uncerrsts",                 					1,		0,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b1d00f0_corerrsts",                 					1,		0,		0,		0x0158, US_REG_DWORD },
	{ "pxp_b1d00f0_rperrsts",                  					1,		0,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b1d00f0_errsid",                    					1,		0,		0,		0x017c, US_REG_DWORD },
	{ "pxp_b1d00f0_miscctrlsts_0",             					1,		0,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b1d00f0_miscctrlsts_1",             					1,		0,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b1d00f0_xpcorerrsts",               					1,		0,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b1d00f0_xpcorerrmsk",               					1,		0,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b1d00f0_xpuncerrsts",               					1,		0,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b1d00f0_xpuncerrmsk",               					1,		0,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b1d00f0_xpuncerrsev",               					1,		0,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b1d00f0_xpuncerrptr",               					1,		0,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b1d00f0_uncedmask",                 					1,		0,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b1d00f0_coredmask",                 					1,		0,		0,		0x021c, US_REG_DWORD },
	{ "pxp_b1d00f0_rpedmask",                  					1,		0,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b1d00f0_xpuncedmask",               					1,		0,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b1d00f0_xpcoredmask",               					1,		0,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b1d00f0_xpglberrsts",               					1,		0,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b1d00f0_xpglberrptr",               					1,		0,		0,		0x0232,  US_REG_WORD },
	{ "pxp_b1d00f0_lnerrsts",                  					1,		0,		0,		0x0258, US_REG_DWORD },
	{ "pxp_b1d00f0_ler_ctrlsts",               					1,		0,		0,		0x0288, US_REG_DWORD },
	{ "pxp_b1d00f0_rppioerr_cap",              					1,		0,		0,		0x0298, US_REG_DWORD },
	{ "pxp_b1d00f0_rppioerr_status",           					1,		0,		0,		0x02a4, US_REG_DWORD },
	{ "pxp_b1d01f0_pbus",                      					1,		1,		0,		0x0018,  US_REG_BYTE },
	{ "pxp_b1d01f0_secbus",                    					1,		1,		0,		0x0019,  US_REG_BYTE },
	{ "pxp_b1d01f0_subbus",                    					1,		1,		0,		0x001a,  US_REG_BYTE },
	{ "pxp_b1d01f0_iobas",                     					1,		1,		0,		0x001c,  US_REG_BYTE },
	{ "pxp_b1d01f0_iolim",                     					1,		1,		0,		0x001d,  US_REG_BYTE },
	{ "pxp_b1d01f0_secsts",                    					1,		1,		0,		0x001e,  US_REG_WORD },
	{ "pxp_b1d01f0_mbas",                      					1,		1,		0,		0x0020,  US_REG_WORD },
	{ "pxp_b1d01f0_mlim",                      					1,		1,		0,		0x0022,  US_REG_WORD },
	{ "pxp_b1d01f0_pbas",                      					1,		1,		0,		0x0024,  US_REG_WORD },
	{ "pxp_b1d01f0_plim",                      					1,		1,		0,		0x0026,  US_REG_WORD },
	{ "pxp_b1d01f0_pbasu",                     					1,		1,		0,		0x0028, US_REG_DWORD },
	{ "pxp_b1d01f0_plimu",                     					1,		1,		0,		0x002c, US_REG_DWORD },
	{ "pxp_b1d01f0_devsts",                    					1,		1,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b1d01f0_lnksts",                    					1,		1,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b1d01f0_sltsts",                    					1,		1,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_b1d01f0_lnksts2",                   					1,		1,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_b1d01f0_uncerrsts",                 					1,		1,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b1d01f0_corerrsts",                 					1,		1,		0,		0x0158, US_REG_DWORD },
	{ "pxp_b1d01f0_rperrsts",                  					1,		1,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b1d01f0_errsid",                    					1,		1,		0,		0x017c, US_REG_DWORD },
	{ "pxp_b1d01f0_miscctrlsts_0",             					1,		1,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b1d01f0_miscctrlsts_1",             					1,		1,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b1d01f0_xpcorerrsts",               					1,		1,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b1d01f0_xpcorerrmsk",               					1,		1,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b1d01f0_xpuncerrsts",               					1,		1,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b1d01f0_xpuncerrmsk",               					1,		1,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b1d01f0_xpuncerrsev",               					1,		1,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b1d01f0_xpuncerrptr",               					1,		1,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b1d01f0_uncedmask",                 					1,		1,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b1d01f0_coredmask",                 					1,		1,		0,		0x021c, US_REG_DWORD },
	{ "pxp_b1d01f0_rpedmask",                  					1,		1,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b1d01f0_xpuncedmask",               					1,		1,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b1d01f0_xpcoredmask",               					1,		1,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b1d01f0_xpglberrsts",               					1,		1,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b1d01f0_xpglberrptr",               					1,		1,		0,		0x0232,  US_REG_WORD },
	{ "pxp_b1d01f0_lnerrsts",                  					1,		1,		0,		0x0258, US_REG_DWORD },
	{ "pxp_b1d01f0_ler_ctrlsts",               					1,		1,		0,		0x0288, US_REG_DWORD },
	{ "pxp_b1d01f0_rppioerr_cap",              					1,		1,		0,		0x0298, US_REG_DWORD },
	{ "pxp_b1d01f0_rppioerr_status",           					1,		1,		0,		0x02a4, US_REG_DWORD },
	{ "pxp_b1d02f0_pbus",                      					1,		2,		0,		0x0018,  US_REG_BYTE },
	{ "pxp_b1d02f0_secbus",                    					1,		2,		0,		0x0019,  US_REG_BYTE },
	{ "pxp_b1d02f0_subbus",                    					1,		2,		0,		0x001a,  US_REG_BYTE },
	{ "pxp_b1d02f0_iobas",                     					1,		2,		0,		0x001c,  US_REG_BYTE },
	{ "pxp_b1d02f0_iolim",                     					1,		2,		0,		0x001d,  US_REG_BYTE },
	{ "pxp_b1d02f0_secsts",                    					1,		2,		0,		0x001e,  US_REG_WORD },
	{ "pxp_b1d02f0_mbas",                      					1,		2,		0,		0x0020,  US_REG_WORD },
	{ "pxp_b1d02f0_mlim",                      					1,		2,		0,		0x0022,  US_REG_WORD },
	{ "pxp_b1d02f0_pbas",                      					1,		2,		0,		0x0024,  US_REG_WORD },
	{ "pxp_b1d02f0_plim",                      					1,		2,		0,		0x0026,  US_REG_WORD },
	{ "pxp_b1d02f0_pbasu",                     					1,		2,		0,		0x0028, US_REG_DWORD },
	{ "pxp_b1d02f0_plimu",                     					1,		2,		0,		0x002c, US_REG_DWORD },
	{ "pxp_b1d02f0_devsts",                    					1,		2,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b1d02f0_lnksts",                    					1,		2,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b1d02f0_sltsts",                    					1,		2,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_b1d02f0_lnksts2",                   					1,		2,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_b1d02f0_uncerrsts",                 					1,		2,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b1d02f0_corerrsts",                 					1,		2,		0,		0x0158, US_REG_DWORD },
	{ "pxp_b1d02f0_rperrsts",                  					1,		2,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b1d02f0_errsid",                    					1,		2,		0,		0x017c, US_REG_DWORD },
	{ "pxp_b1d02f0_miscctrlsts_0",             					1,		2,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b1d02f0_miscctrlsts_1",             					1,		2,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b1d02f0_xpcorerrsts",               					1,		2,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b1d02f0_xpcorerrmsk",               					1,		2,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b1d02f0_xpuncerrsts",               					1,		2,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b1d02f0_xpuncerrmsk",               					1,		2,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b1d02f0_xpuncerrsev",               					1,		2,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b1d02f0_xpuncerrptr",               					1,		2,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b1d02f0_uncedmask",                 					1,		2,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b1d02f0_coredmask",                 					1,		2,		0,		0x021c, US_REG_DWORD },
	{ "pxp_b1d02f0_rpedmask",                  					1,		2,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b1d02f0_xpuncedmask",               					1,		2,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b1d02f0_xpcoredmask",               					1,		2,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b1d02f0_xpglberrsts",               					1,		2,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b1d02f0_xpglberrptr",               					1,		2,		0,		0x0232,  US_REG_WORD },
	{ "pxp_b1d02f0_lnerrsts",                  					1,		2,		0,		0x0258, US_REG_DWORD },
	{ "pxp_b1d02f0_ler_ctrlsts",               					1,		2,		0,		0x0288, US_REG_DWORD },
	{ "pxp_b1d02f0_rppioerr_cap",              					1,		2,		0,		0x0298, US_REG_DWORD },
	{ "pxp_b1d02f0_rppioerr_status",           					1,		2,		0,		0x02a4, US_REG_DWORD },
	{ "pxp_b1d03f0_pbus",                      					1,		3,		0,		0x0018,  US_REG_BYTE },
	{ "pxp_b1d03f0_secbus",                    					1,		3,		0,		0x0019,  US_REG_BYTE },
	{ "pxp_b1d03f0_subbus",                    					1,		3,		0,		0x001a,  US_REG_BYTE },
	{ "pxp_b1d03f0_iobas",                     					1,		3,		0,		0x001c,  US_REG_BYTE },
	{ "pxp_b1d03f0_iolim",                     					1,		3,		0,		0x001d,  US_REG_BYTE },
	{ "pxp_b1d03f0_secsts",                    					1,		3,		0,		0x001e,  US_REG_WORD },
	{ "pxp_b1d03f0_mbas",                      					1,		3,		0,		0x0020,  US_REG_WORD },
	{ "pxp_b1d03f0_mlim",                      					1,		3,		0,		0x0022,  US_REG_WORD },
	{ "pxp_b1d03f0_pbas",                      					1,		3,		0,		0x0024,  US_REG_WORD },
	{ "pxp_b1d03f0_plim",                      					1,		3,		0,		0x0026,  US_REG_WORD },
	{ "pxp_b1d03f0_pbasu",                     					1,		3,		0,		0x0028, US_REG_DWORD },
	{ "pxp_b1d03f0_plimu",                     					1,		3,		0,		0x002c, US_REG_DWORD },
	{ "pxp_b1d03f0_devsts",                    					1,		3,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b1d03f0_lnksts",                    					1,		3,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b1d03f0_sltsts",                    					1,		3,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_b1d03f0_lnksts2",                   					1,		3,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_b1d03f0_uncerrsts",                 					1,		3,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b1d03f0_corerrsts",                 					1,		3,		0,		0x0158, US_REG_DWORD },
	{ "pxp_b1d03f0_rperrsts",                  					1,		3,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b1d03f0_errsid",                    					1,		3,		0,		0x017c, US_REG_DWORD },
	{ "pxp_b1d03f0_miscctrlsts_0",             					1,		3,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b1d03f0_miscctrlsts_1",             					1,		3,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b1d03f0_xpcorerrsts",               					1,		3,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b1d03f0_xpcorerrmsk",               					1,		3,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b1d03f0_xpuncerrsts",               					1,		3,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b1d03f0_xpuncerrmsk",               					1,		3,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b1d03f0_xpuncerrsev",               					1,		3,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b1d03f0_xpuncerrptr",               					1,		3,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b1d03f0_uncedmask",                 					1,		3,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b1d03f0_coredmask",                 					1,		3,		0,		0x021c, US_REG_DWORD },
	{ "pxp_b1d03f0_rpedmask",                  					1,		3,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b1d03f0_xpuncedmask",               					1,		3,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b1d03f0_xpcoredmask",               					1,		3,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b1d03f0_xpglberrsts",               					1,		3,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b1d03f0_xpglberrptr",               					1,		3,		0,		0x0232,  US_REG_WORD },
	{ "pxp_b1d03f0_lnerrsts",                  					1,		3,		0,		0x0258, US_REG_DWORD },
	{ "pxp_b1d03f0_ler_ctrlsts",               					1,		3,		0,		0x0288, US_REG_DWORD },
	{ "pxp_b1d03f0_rppioerr_cap",              					1,		3,		0,		0x0298, US_REG_DWORD },
	{ "pxp_b1d03f0_rppioerr_status",           					1,		3,		0,		0x02a4, US_REG_DWORD },
	{ "iio_mmcfg_base_b1d05f0",                					1,		5,		0,		0x0090, US_REG_QWORD },
	{ "iio_mmcfg_limit_b1d05f0",               					1,		5,		0,		0x0098, US_REG_QWORD },
	{ "iio_tseg_b1d05f0",                      					1,		5,		0,		0x00a8, US_REG_QWORD },
	{ "iio_tolm_b1d05f0",                      					1,		5,		0,		0x00d0, US_REG_DWORD },
	{ "iio_tohm_0_b1d05f0",                    					1,		5,		0,		0x00d4, US_REG_DWORD },
	{ "iio_tohm_1_b1d05f0",                    					1,		5,		0,		0x00d8, US_REG_DWORD },
	{ "iio_ncmem_base_0_b1d05f0",              					1,		5,		0,		0x00e0, US_REG_DWORD },
	{ "iio_ncmem_base_1_b1d05f0",              					1,		5,		0,		0x00e4, US_REG_DWORD },
	{ "iio_ncmem_limit_0_b1d05f0",             					1,		5,		0,		0x00e8, US_REG_DWORD },
	{ "iio_ncmem_limit_1_b1d05f0",             					1,		5,		0,		0x00ec, US_REG_DWORD },
	{ "iio_mencmem_base_0_b1d05f0",            					1,		5,		0,		0x00f0, US_REG_DWORD },
	{ "iio_mencmem_base_1_b1d05f0",            					1,		5,		0,		0x00f4, US_REG_DWORD },
	{ "iio_mencmem_limit_0_b1d05f0",           					1,		5,		0,		0x00f8, US_REG_DWORD },
	{ "iio_mencmem_limit_1_b1d05f0",           					1,		5,		0,		0x00fc, US_REG_DWORD },
	{ "iio_vtuncerrsts_b1d05f0",               					1,		5,		0,		0x01a8, US_REG_DWORD },
	{ "iio_vtuncerrptr_b1d05f0",               					1,		5,		0,		0x01b4,  US_REG_BYTE },
	{ "iio_mmioh_non_interleave_b1d05f0",      					1,		5,		0,		0x0340, US_REG_QWORD },
	{ "iio_mmioh_interleave_b1d05f0",          					1,		5,		0,		0x0348, US_REG_QWORD },
	{ "iio_irpperrsv_b1d05f2",                 					1,		5,		2,		0x0080, US_REG_QWORD },
	{ "iio_pcierrsv_b1d05f2",                  					1,		5,		2,		0x0094, US_REG_DWORD },
	{ "iio_viral_b1d05f2",                     					1,		5,		2,		0x00a0, US_REG_DWORD },
	{ "iio_lnerrmask_b1d05f2",                 					1,		5,		2,		0x019c, US_REG_DWORD },
	{ "iio_lferrmask_b1d05f2",                 					1,		5,		2,		0x01a0, US_REG_DWORD },
	{ "iio_lcerrmask_b1d05f2",                 					1,		5,		2,		0x01a4, US_REG_DWORD },
	{ "iio_lcerrst_b1d05f2",                   					1,		5,		2,		0x01a8, US_REG_DWORD },
	{ "iio_lcferrst_b1d05f2",                  					1,		5,		2,		0x01ac, US_REG_DWORD },
	{ "iio_lcnerrst_b1d05f2",                  					1,		5,		2,		0x01b8, US_REG_DWORD },
	{ "iio_lnerrst_b1d05f2",                   					1,		5,		2,		0x01c0, US_REG_DWORD },
	{ "iio_lferrst_b1d05f2",                   					1,		5,		2,		0x01c4, US_REG_DWORD },
	{ "iio_lerrctl_b1d05f2",                   					1,		5,		2,		0x01c8, US_REG_DWORD },
	{ "iio_lsysst_b1d05f2",                    					1,		5,		2,		0x01cc, US_REG_DWORD },
	{ "iio_lsysctl_b1d05f2",                   					1,		5,		2,		0x01d0, US_REG_DWORD },
	{ "iio_sysmap_b1d05f2",                    					1,		5,		2,		0x01d4, US_REG_DWORD },
	{ "iio_lbitmap_b1d05f2",                   					1,		5,		2,		0x01d8, US_REG_DWORD },
	{ "iio_lfferrst_b1d05f2",                  					1,		5,		2,		0x01dc, US_REG_DWORD },
	{ "iio_lfnerrst_b1d05f2",                  					1,		5,		2,		0x01e8, US_REG_DWORD },
	{ "iio_lnferrst_b1d05f2",                  					1,		5,		2,		0x01ec, US_REG_DWORD },
	{ "iio_lnnerrst_b1d05f2",                  					1,		5,		2,		0x01f8, US_REG_DWORD },
	{ "iio_irpringerrsv_b1d05f2",              					1,		5,		2,		0x0200, US_REG_QWORD },
	{ "iio_irpringerrst_b1d05f2",              					1,		5,		2,		0x0208, US_REG_DWORD },
	{ "iio_irpringerrctl_b1d05f2",             					1,		5,		2,		0x020c, US_REG_DWORD },
	{ "iio_irpringfferrst_b1d05f2",            					1,		5,		2,		0x0210, US_REG_DWORD },
	{ "iio_irpringfnerrst_b1d05f2",            					1,		5,		2,		0x0214, US_REG_DWORD },
	{ "iio_irpringnferrst_b1d05f2",            					1,		5,		2,		0x0218, US_REG_DWORD },
	{ "iio_irpringnnerrst_b1d05f2",            					1,		5,		2,		0x021c, US_REG_DWORD },
	{ "iio_irpringerrcntsel_b1d05f2",          					1,		5,		2,		0x0220, US_REG_DWORD },
	{ "iio_irpringerrcnt_b1d05f2",             					1,		5,		2,		0x0224, US_REG_DWORD },
	{ "iio_irpringmisc_b1d05f2",               					1,		5,		2,		0x0228, US_REG_DWORD },
	{ "iio_irpp0errst_b1d05f2",                					1,		5,		2,		0x0230, US_REG_DWORD },
	{ "iio_irpp0errctl_b1d05f2",               					1,		5,		2,		0x0234, US_REG_DWORD },
	{ "iio_irpp0fferrst_b1d05f2",              					1,		5,		2,		0x0238, US_REG_DWORD },
	{ "iio_irpp0fnerrst_b1d05f2",              					1,		5,		2,		0x023c, US_REG_DWORD },
	{ "iio_irpp0fferrhd0_b1d05f2",             					1,		5,		2,		0x0240, US_REG_DWORD },
	{ "iio_irpp0fferrhd1_b1d05f2",             					1,		5,		2,		0x0244, US_REG_DWORD },
	{ "iio_irpp0fferrhd2_b1d05f2",             					1,		5,		2,		0x0248, US_REG_DWORD },
	{ "iio_irpp0fferrhd3_b1d05f2",             					1,		5,		2,		0x024c, US_REG_DWORD },
	{ "iio_irpp0nferrst_b1d05f2",              					1,		5,		2,		0x0250, US_REG_DWORD },
	{ "iio_irpp0nnerrst_b1d05f2",              					1,		5,		2,		0x0254, US_REG_DWORD },
	{ "iio_irpp0errcntsel_b1d05f2",            					1,		5,		2,		0x0268, US_REG_DWORD },
	{ "iio_irpp0errcnt_b1d05f2",               					1,		5,		2,		0x026c, US_REG_DWORD },
	{ "iio_irpp0plsr0_b1d05f2",                					1,		5,		2,		0x0270, US_REG_DWORD },
	{ "iio_irpp0plsr1_b1d05f2",                					1,		5,		2,		0x0274, US_REG_DWORD },
	{ "iio_irpp0plsr2_b1d05f2",                					1,		5,		2,		0x0278, US_REG_DWORD },
	{ "iio_tcerrst_b1d05f2",                   					1,		5,		2,		0x0288, US_REG_DWORD },
	{ "iio_tcerrctl_b1d05f2",                  					1,		5,		2,		0x028c, US_REG_DWORD },
	{ "iio_itcerrsev_b1d05f2",                 					1,		5,		2,		0x0290, US_REG_DWORD },
	{ "iio_otcerrsev_b1d05f2",                 					1,		5,		2,		0x0294, US_REG_DWORD },
	{ "iio_tcerrcntsel_b1d05f2",               					1,		5,		2,		0x0298, US_REG_DWORD },
	{ "iio_tcerrcnt_b1d05f2",                  					1,		5,		2,		0x029c, US_REG_DWORD },
	{ "iio_itcfferrst_b1d05f2",                					1,		5,		2,		0x02a0, US_REG_DWORD },
	{ "iio_itcfferrhd0_b1d05f2",               					1,		5,		2,		0x02a4, US_REG_DWORD },
	{ "iio_itcfferrhd1_b1d05f2",               					1,		5,		2,		0x02a8, US_REG_DWORD },
	{ "iio_itcfferrhd2_b1d05f2",               					1,		5,		2,		0x02ac, US_REG_DWORD },
	{ "iio_itcfferrhd3_b1d05f2",               					1,		5,		2,		0x02b0, US_REG_DWORD },
	{ "iio_itcfnerrst_b1d05f2",                					1,		5,		2,		0x02b4, US_REG_DWORD },
	{ "iio_itcnferrst_b1d05f2",                					1,		5,		2,		0x02b8, US_REG_DWORD },
	{ "iio_itcnnerrst_b1d05f2",                					1,		5,		2,		0x02cc, US_REG_DWORD },
	{ "iio_otcfferrst_b1d05f2",                					1,		5,		2,		0x02d0, US_REG_DWORD },
	{ "iio_otcfferrhd0_b1d05f2",               					1,		5,		2,		0x02d4, US_REG_DWORD },
	{ "iio_otcfferrhd1_b1d05f2",               					1,		5,		2,		0x02d8, US_REG_DWORD },
	{ "iio_otcfferrhd2_b1d05f2",               					1,		5,		2,		0x02dc, US_REG_DWORD },
	{ "iio_otcfferrhd3_b1d05f2",               					1,		5,		2,		0x02e0, US_REG_DWORD },
	{ "iio_otcfnerrst_b1d05f2",                					1,		5,		2,		0x02e4, US_REG_DWORD },
	{ "iio_otcnferrst_b1d05f2",                					1,		5,		2,		0x02e8, US_REG_DWORD },
	{ "iio_otcnferrhd0_b1d05f2",               					1,		5,		2,		0x02ec, US_REG_DWORD },
	{ "iio_otcnnerrst_b1d05f2",                					1,		5,		2,		0x02fc, US_REG_DWORD },
	{ "iio_iioerrctl_b1d05f2",                 					1,		5,		2,		0x0304, US_REG_DWORD },
	{ "pxp_b1d07f0_devsts",                    					1,		7,		0,		0x004a,  US_REG_WORD },
	{ "pxp_b1d07f0_lnksts",                    					1,		7,		0,		0x0052,  US_REG_WORD },
	{ "pxp_b1d07f0_lnksts2",                   					1,		7,		0,		0x0072,  US_REG_WORD },
	{ "pxp_b1d07f0_sltsts2",                   					1,		7,		0,		0x007a,  US_REG_WORD },
	{ "pxp_b1d07f1_devsts",                    					1,		7,		1,		0x004a,  US_REG_WORD },
	{ "pxp_b1d07f1_lnksts",                    					1,		7,		1,		0x0052,  US_REG_WORD },
	{ "pxp_b1d07f1_lnksts2",                   					1,		7,		1,		0x0072,  US_REG_WORD },
	{ "pxp_b1d07f1_sltsts2",                   					1,		7,		1,		0x007a,  US_REG_WORD },
	{ "pxp_b1d07f2_devsts",                    					1,		7,		2,		0x004a,  US_REG_WORD },
	{ "pxp_b1d07f2_lnksts",                    					1,		7,		2,		0x0052,  US_REG_WORD },
	{ "pxp_b1d07f2_lnksts2",                   					1,		7,		2,		0x0072,  US_REG_WORD },
	{ "pxp_b1d07f2_sltsts2",                   					1,		7,		2,		0x007a,  US_REG_WORD },
	{ "pxp_b1d07f3_devsts",                    					1,		7,		3,		0x004a,  US_REG_WORD },
	{ "pxp_b1d07f3_lnksts",                    					1,		7,		3,		0x0052,  US_REG_WORD },
	{ "pxp_b1d07f3_lnksts2",                   					1,		7,		3,		0x0072,  US_REG_WORD },
	{ "pxp_b1d07f3_sltsts2",                   					1,		7,		3,		0x007a,  US_REG_WORD },
	{ "pxp_b1d07f4_devsts",                    					1,		7,		4,		0x004a,  US_REG_WORD },
	{ "pxp_b1d07f4_lnksts",                    					1,		7,		4,		0x0052,  US_REG_WORD },
	{ "pxp_b1d07f4_lnksts2",                   					1,		7,		4,		0x0072,  US_REG_WORD },
	{ "pxp_b1d07f4_sltsts2",                   					1,		7,		4,		0x007a,  US_REG_WORD },
	{ "pxp_b1d07f7_devsts",                    					1,		7,		7,		0x004a,  US_REG_WORD },
	{ "pxp_b1d07f7_lnksts",                    					1,		7,		7,		0x0052,  US_REG_WORD },
	{ "pxp_b1d07f7_lnksts2",                   					1,		7,		7,		0x0072,  US_REG_WORD },
	{ "pxp_b1d07f7_sltsts2",                   					1,		7,		7,		0x007a,  US_REG_WORD },
	{ "pxp_b1d07f7_tswdbgerrstdis0",           					1,		7,		7,		0x0358, US_REG_DWORD },
	{ "cha0_mc_route_table_cfg",               					1,		14,		0,		0x00b4, US_REG_DWORD },
	{ "cha1_mc_route_table_cfg",               					1,		14,		1,		0x00b4, US_REG_DWORD },
	{ "cha2_mc_route_table_cfg",               					1,		14,		2,		0x00b4, US_REG_DWORD },
	{ "cha3_mc_route_table_cfg",               					1,		14,		3,		0x00b4, US_REG_DWORD },
	{ "cha4_mc_route_table_cfg",               					1,		14,		4,		0x00b4, US_REG_DWORD },
	{ "cha5_mc_route_table_cfg",               					1,		14,		5,		0x00b4, US_REG_DWORD },
	{ "cha6_mc_route_table_cfg",               					1,		14,		6,		0x00b4, US_REG_DWORD },
	{ "cha7_mc_route_table_cfg",               					1,		14,		7,		0x00b4, US_REG_DWORD },
	{ "cha8_mc_route_table_cfg",               					1,		15,		0,		0x00b4, US_REG_DWORD },
	{ "cha9_mc_route_table_cfg",               					1,		15,		1,		0x00b4, US_REG_DWORD },
	{ "cha10_mc_route_table_cfg",              					1,		15,		2,		0x00b4, US_REG_DWORD },
	{ "cha11_mc_route_table_cfg",              					1,		15,		3,		0x00b4, US_REG_DWORD },
	{ "cha12_mc_route_table_cfg",              					1,		15,		4,		0x00b4, US_REG_DWORD },
	{ "cha13_mc_route_table_cfg",              					1,		15,		5,		0x00b4, US_REG_DWORD },
	{ "cha14_mc_route_table_cfg",              					1,		15,		6,		0x00b4, US_REG_DWORD },
	{ "cha15_mc_route_table_cfg",              					1,		15,		7,		0x00b4, US_REG_DWORD },
	{ "cha16_mc_route_table_cfg",              					1,		16,		0,		0x00b4, US_REG_DWORD },
	{ "cha17_mc_route_table_cfg",              					1,		16,		1,		0x00b4, US_REG_DWORD },
	{ "cha18_mc_route_table_cfg",              					1,		16,		2,		0x00b4, US_REG_DWORD },
	{ "cha19_mc_route_table_cfg",              					1,		16,		3,		0x00b4, US_REG_DWORD },
	{ "cha20_mc_route_table_cfg",              					1,		16,		4,		0x00b4, US_REG_DWORD },
	{ "cha21_mc_route_table_cfg",              					1,		16,		5,		0x00b4, US_REG_DWORD },
	{ "cha22_mc_route_table_cfg",              					1,		16,		6,		0x00b4, US_REG_DWORD },
	{ "cha23_mc_route_table_cfg",              					1,		16,		7,		0x00b4, US_REG_DWORD },
	{ "cha24_mc_route_table_cfg",              					1,		17,		0,		0x00b4, US_REG_DWORD },
	{ "cha25_mc_route_table_cfg",              					1,		17,		1,		0x00b4, US_REG_DWORD },
	{ "cha26_mc_route_table_cfg",              					1,		17,		2,		0x00b4, US_REG_DWORD },
	{ "cha27_mc_route_table_cfg",              					1,		17,		3,		0x00b4, US_REG_DWORD },
	{ "pcu_cr_package_power_sku_cfg",          					1,		30,		0,		0x0080, US_REG_QWORD },
	{ "pcu_cr_package_power_sku_unit_cfg",     					1,		30,		0,		0x008c, US_REG_DWORD },
	{ "pcu_cr_package_energy_status_cfg",      					1,		30,		0,		0x0090, US_REG_DWORD },
	{ "pcu_cr_mem_trml_temperature_report_0",  					1,		30,		0,		0x0094, US_REG_DWORD },
	{ "pcu_cr_mem_trml_temperature_report_1",  					1,		30,		0,		0x0098, US_REG_DWORD },
	{ "pcu_cr_mem_trml_temperature_report_2",  					1,		30,		0,		0x009c, US_REG_DWORD },
	{ "pcu_cr_package_temperature_cfg",        					1,		30,		0,		0x00c8, US_REG_DWORD },
	{ "pcu_cr_mrc_odt_power_saving_cfg",       					1,		30,		0,		0x00d0, US_REG_DWORD },
	{ "pcu_cr_p_state_limits_cfg",             					1,		30,		0,		0x00d8, US_REG_DWORD },
	{ "pcu_cr_package_therm_margin_cfg",       					1,		30,		0,		0x00e0, US_REG_DWORD },
	{ "pcu_cr_package_rapl_limit_cfg",         					1,		30,		0,		0x00e8, US_REG_QWORD },
	{ "pcu_cr_bios_reset_cpl_cfg",             					1,		30,		1,		0x0094, US_REG_DWORD },
	{ "pcu_cr_mc_bios_req_cfg",                					1,		30,		1,		0x0098, US_REG_DWORD },
	{ "pcu_cr_mc_bios_data_cfg",               					1,		30,		1,		0x009c, US_REG_DWORD },
	{ "pcu_cr_csr_desired_cores_cfg2",         					1,		30,		1,		0x00a0, US_REG_DWORD },
	{ "pcu_cr_csr_desired_cores_cfg",          					1,		30,		1,		0x00a4, US_REG_DWORD },
	{ "pcu_cr_m_comp_cfg",                     					1,		30,		1,		0x00b8, US_REG_DWORD },
	{ "pcu_cr_sskpd_cfg",                      					1,		30,		1,		0x00d0, US_REG_QWORD },
	{ "pcu_cr_device_shared_idle_duration_cfg",					1,		30,		1,		0x00e4, US_REG_DWORD },
	{ "pcu_cr_device_dedicated_idle_duration_cfg",				1,		30,		1,		0x00e8, US_REG_DWORD },
	{ "pcu_cr_device_idle_duration_override_cfg",				1,		30,		1,		0x00ec, US_REG_DWORD },
	{ "pcu_cr_ia_perf_limit_reasons_cfg",      					1,		30,		1,		0x00f0, US_REG_DWORD },
	{ "pcu_cr_energy_debug_cfg",               					1,		30,		1,		0x00f8, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_cfg",         					1,		30,		2,		0x007c, US_REG_DWORD },
	{ "pcu_cr_core_fivr_err_log_cfg",          					1,		30,		2,		0x0080, US_REG_DWORD },
	{ "pcu_cr_package_rapl_perf_status",       					1,		30,		2,		0x0088, US_REG_DWORD },
	{ "pcu_cr_dts_config1_cfg",                					1,		30,		2,		0x0094, US_REG_DWORD },
	{ "pcu_cr_dts_config2_cfg",                					1,		30,		2,		0x0098, US_REG_DWORD },
	{ "pcu_cr_dts_config3_cfg",                					1,		30,		2,		0x009c, US_REG_DWORD },
	{ "pcu_cr_global_pkg_c_s_control_register",					1,		30,		2,		0x00a0, US_REG_DWORD },
	{ "pcu_cr_global_nid_socket_0_to_3_map",   					1,		30,		2,		0x00a4, US_REG_DWORD },
	{ "pcu_cr_dram_power_info_cfg",            					1,		30,		2,		0x00a8, US_REG_QWORD },
	{ "pcu_cr_dram_rapl_perf_status_cfg",      					1,		30,		2,		0x00d8, US_REG_DWORD },
	{ "pcu_cr_io_bandwidth_p_limit_control",   					1,		30,		2,		0x00e8, US_REG_DWORD },
	{ "pcu_cr_mca_err_src_log",                					1,		30,		2,		0x00ec, US_REG_DWORD },
	{ "pcu_cr_dram_plane_power_limit_cfg",     					1,		30,		2,		0x00f0, US_REG_QWORD },
	{ "pcu_cr_config_tdp_control_cfg",         					1,		30,		3,		0x0060, US_REG_DWORD },
	{ "pcu_cr_config_tdp_nominal_cfg",         					1,		30,		3,		0x0064, US_REG_DWORD },
	{ "pcu_cr_config_tdp_level1_cfg",          					1,		30,		3,		0x0070, US_REG_QWORD },
	{ "pcu_cr_config_tdp_level2_cfg",          					1,		30,		3,		0x0078, US_REG_QWORD },
	{ "pcu_cr_cap_hdr_cfg",                    					1,		30,		3,		0x0080, US_REG_DWORD },
	{ "pcu_cr_capid0_cfg",                     					1,		30,		3,		0x0084, US_REG_DWORD },
	{ "pcu_cr_capid1_cfg",                     					1,		30,		3,		0x0088, US_REG_DWORD },
	{ "pcu_cr_capid2_cfg",                     					1,		30,		3,		0x008c, US_REG_DWORD },
	{ "pcu_cr_capid3_cfg",                     					1,		30,		3,		0x0090, US_REG_DWORD },
	{ "pcu_cr_capid4_cfg",                     					1,		30,		3,		0x0094, US_REG_DWORD },
	{ "pcu_cr_capid5_cfg",                     					1,		30,		3,		0x0098, US_REG_DWORD },
	{ "pcu_cr_capid6_cfg",                     					1,		30,		3,		0x009c, US_REG_DWORD },
	{ "pcu_cr_flex_ratio_cfg",                 					1,		30,		3,		0x00a0, US_REG_QWORD },
	{ "pcu_cr_capid7_cfg",                     					1,		30,		3,		0x00a8, US_REG_DWORD },
	{ "pcu_cr_resolved_cores_cfg",             					1,		30,		3,		0x00b4, US_REG_DWORD },
	{ "pcu_cr_fused_cores_cfg",                					1,		30,		3,		0x00b8, US_REG_DWORD },
	{ "pcu_cr_global_nid_socket_4_to_7_map",   					1,		30,		4,		0x007c, US_REG_DWORD },
	{ "pcu_cr_cpubusno_cfg",                   					1,		30,		4,		0x0094, US_REG_DWORD },
	{ "pcu_cr_cpubusno1_cfg",                  					1,		30,		4,		0x0098, US_REG_DWORD },
	{ "pcu_cr_cpubusno_valid_cfg",             					1,		30,		4,		0x009c, US_REG_DWORD },
	{ "pcu_cr_mcp_thermal_report_1_cfg",       					1,		30,		4,		0x00d0, US_REG_DWORD },
	{ "pcu_cr_mcp_thermal_report_2_cfg",       					1,		30,		4,		0x00d8, US_REG_DWORD },
	{ "pcu_cr_unc_tsc_snapshot",               					1,		30,		4,		0x00e0, US_REG_QWORD },
	{ "pcu_cr_tsc_hp_offset",                  					1,		30,		4,		0x00e8, US_REG_QWORD },
	{ "pcu_cr_pcu_first_ierr_tsc_lo_cfg",      					1,		30,		4,		0x00f0, US_REG_DWORD },
	{ "pcu_cr_pcu_first_ierr_tsc_hi_cfg",      					1,		30,		4,		0x00f4, US_REG_DWORD },
	{ "pcu_cr_pcu_first_mcerr_tsc_lo_cfg",     					1,		30,		4,		0x00f8, US_REG_DWORD },
	{ "pcu_cr_pcu_first_mcerr_tsc_hi_cfg",     					1,		30,		4,		0x00fc, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_ch_cfg_0",    					1,		30,		5,		0x0040, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_ch_cfg_1",    					1,		30,		5,		0x0044, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_ch_cfg_2",    					1,		30,		5,		0x0048, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_ch_cfg_3",    					1,		30,		5,		0x004c, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_ch_cfg_4",    					1,		30,		5,		0x0050, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_ch_cfg_5",    					1,		30,		5,		0x0054, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_ch_cfg_6",    					1,		30,		5,		0x0058, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_ch_cfg_7",    					1,		30,		5,		0x005c, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_ch_cfg_8",    					1,		30,		5,		0x0060, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_ch_cfg_9",    					1,		30,		5,		0x0064, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_ch_cfg_10",   					1,		30,		5,		0x0068, US_REG_DWORD },
	{ "pcu_cr_dram_energy_status_ch_cfg_11",   					1,		30,		5,		0x006c, US_REG_DWORD },
	{ "pcu_cr_smb_status_cfg_0",               					1,		30,		5,		0x00a8, US_REG_DWORD },
	{ "pcu_cr_smb_status_cfg_1",               					1,		30,		5,		0x00ac, US_REG_DWORD },
	{ "pcu_cr_smb_status_cfg_2",               					1,		30,		5,		0x00b0, US_REG_DWORD },
	{ "pcu_cr_mh_ext_stat_cfg",                					1,		30,		5,		0x00e0, US_REG_DWORD },
	{ "pcu_cr_mh_dfx_config_cfg",              					1,		30,		5,		0x00e4, US_REG_DWORD },
	{ "pcu_cr_lbitmap",                        					1,		30,		5,		0x00ec, US_REG_DWORD },
	{ "ntb_b2d00f0_lnksts",                    					2,		0,		0,		0x01a2,  US_REG_WORD },
	{ "ntb_b2d00f0_sltsts",                    					2,		0,		0,		0x01aa,  US_REG_WORD },
	{ "ntb_b2d00f0_lnksts2",                   					2,		0,		0,		0x01c2,  US_REG_WORD },
	{ "pxp_b2d00f0_pbus",                      					2,		0,		0,		0x0018,  US_REG_BYTE },
	{ "pxp_b2d00f0_secbus",                    					2,		0,		0,		0x0019,  US_REG_BYTE },
	{ "pxp_b2d00f0_subbus",                    					2,		0,		0,		0x001a,  US_REG_BYTE },
	{ "pxp_b2d00f0_iobas",                     					2,		0,		0,		0x001c,  US_REG_BYTE },
	{ "pxp_b2d00f0_iolim",                     					2,		0,		0,		0x001d,  US_REG_BYTE },
	{ "pxp_b2d00f0_secsts",                    					2,		0,		0,		0x001e,  US_REG_WORD },
	{ "pxp_b2d00f0_mbas",                      					2,		0,		0,		0x0020,  US_REG_WORD },
	{ "pxp_b2d00f0_mlim",                      					2,		0,		0,		0x0022,  US_REG_WORD },
	{ "pxp_b2d00f0_pbas",                      					2,		0,		0,		0x0024,  US_REG_WORD },
	{ "pxp_b2d00f0_plim",                      					2,		0,		0,		0x0026,  US_REG_WORD },
	{ "pxp_b2d00f0_pbasu",                     					2,		0,		0,		0x0028, US_REG_DWORD },
	{ "pxp_b2d00f0_plimu",                     					2,		0,		0,		0x002c, US_REG_DWORD },
	{ "pxp_b2d00f0_devsts",                    					2,		0,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b2d00f0_lnksts",                    					2,		0,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b2d00f0_sltsts",                    					2,		0,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_b2d00f0_lnksts2",                   					2,		0,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_b2d00f0_uncerrsts",                 					2,		0,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b2d00f0_corerrsts",                 					2,		0,		0,		0x0158, US_REG_DWORD },
	{ "pxp_b2d00f0_rperrsts",                  					2,		0,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b2d00f0_errsid",                    					2,		0,		0,		0x017c, US_REG_DWORD },
	{ "pxp_b2d00f0_miscctrlsts_0",             					2,		0,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b2d00f0_miscctrlsts_1",             					2,		0,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b2d00f0_xpcorerrsts",               					2,		0,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b2d00f0_xpcorerrmsk",               					2,		0,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b2d00f0_xpuncerrsts",               					2,		0,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b2d00f0_xpuncerrmsk",               					2,		0,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b2d00f0_xpuncerrsev",               					2,		0,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b2d00f0_xpuncerrptr",               					2,		0,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b2d00f0_uncedmask",                 					2,		0,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b2d00f0_coredmask",                 					2,		0,		0,		0x021c, US_REG_DWORD },
	{ "pxp_b2d00f0_rpedmask",                  					2,		0,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b2d00f0_xpuncedmask",               					2,		0,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b2d00f0_xpcoredmask",               					2,		0,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b2d00f0_xpglberrsts",               					2,		0,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b2d00f0_xpglberrptr",               					2,		0,		0,		0x0232,  US_REG_WORD },
	{ "pxp_b2d00f0_lnerrsts",                  					2,		0,		0,		0x0258, US_REG_DWORD },
	{ "pxp_b2d00f0_ler_ctrlsts",               					2,		0,		0,		0x0288, US_REG_DWORD },
	{ "pxp_b2d00f0_rppioerr_cap",              					2,		0,		0,		0x0298, US_REG_DWORD },
	{ "pxp_b2d00f0_rppioerr_status",           					2,		0,		0,		0x02a4, US_REG_DWORD },
	{ "pxp_b2d01f0_pbus",                      					2,		1,		0,		0x0018,  US_REG_BYTE },
	{ "pxp_b2d01f0_secbus",                    					2,		1,		0,		0x0019,  US_REG_BYTE },
	{ "pxp_b2d01f0_subbus",                    					2,		1,		0,		0x001a,  US_REG_BYTE },
	{ "pxp_b2d01f0_iobas",                     					2,		1,		0,		0x001c,  US_REG_BYTE },
	{ "pxp_b2d01f0_iolim",                     					2,		1,		0,		0x001d,  US_REG_BYTE },
	{ "pxp_b2d01f0_secsts",                    					2,		1,		0,		0x001e,  US_REG_WORD },
	{ "pxp_b2d01f0_mbas",                      					2,		1,		0,		0x0020,  US_REG_WORD },
	{ "pxp_b2d01f0_mlim",                      					2,		1,		0,		0x0022,  US_REG_WORD },
	{ "pxp_b2d01f0_pbas",                      					2,		1,		0,		0x0024,  US_REG_WORD },
	{ "pxp_b2d01f0_plim",                      					2,		1,		0,		0x0026,  US_REG_WORD },
	{ "pxp_b2d01f0_pbasu",                     					2,		1,		0,		0x0028, US_REG_DWORD },
	{ "pxp_b2d01f0_plimu",                     					2,		1,		0,		0x002c, US_REG_DWORD },
	{ "pxp_b2d01f0_devsts",                    					2,		1,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b2d01f0_lnksts",                    					2,		1,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b2d01f0_sltsts",                    					2,		1,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_b2d01f0_lnksts2",                   					2,		1,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_b2d01f0_uncerrsts",                 					2,		1,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b2d01f0_corerrsts",                 					2,		1,		0,		0x0158, US_REG_DWORD },
	{ "pxp_b2d01f0_rperrsts",                  					2,		1,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b2d01f0_errsid",                    					2,		1,		0,		0x017c, US_REG_DWORD },
	{ "pxp_b2d01f0_miscctrlsts_0",             					2,		1,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b2d01f0_miscctrlsts_1",             					2,		1,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b2d01f0_xpcorerrsts",               					2,		1,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b2d01f0_xpcorerrmsk",               					2,		1,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b2d01f0_xpuncerrsts",               					2,		1,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b2d01f0_xpuncerrmsk",               					2,		1,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b2d01f0_xpuncerrsev",               					2,		1,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b2d01f0_xpuncerrptr",               					2,		1,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b2d01f0_uncedmask",                 					2,		1,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b2d01f0_coredmask",                 					2,		1,		0,		0x021c, US_REG_DWORD },
	{ "pxp_b2d01f0_rpedmask",                  					2,		1,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b2d01f0_xpuncedmask",               					2,		1,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b2d01f0_xpcoredmask",               					2,		1,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b2d01f0_xpglberrsts",               					2,		1,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b2d01f0_xpglberrptr",               					2,		1,		0,		0x0232,  US_REG_WORD },
	{ "pxp_b2d01f0_lnerrsts",                  					2,		1,		0,		0x0258, US_REG_DWORD },
	{ "pxp_b2d01f0_ler_ctrlsts",               					2,		1,		0,		0x0288, US_REG_DWORD },
	{ "pxp_b2d01f0_rppioerr_cap",              					2,		1,		0,		0x0298, US_REG_DWORD },
	{ "pxp_b2d01f0_rppioerr_status",           					2,		1,		0,		0x02a4, US_REG_DWORD },
	{ "pxp_b2d02f0_pbus",                      					2,		2,		0,		0x0018,  US_REG_BYTE },
	{ "pxp_b2d02f0_secbus",                    					2,		2,		0,		0x0019,  US_REG_BYTE },
	{ "pxp_b2d02f0_subbus",                    					2,		2,		0,		0x001a,  US_REG_BYTE },
	{ "pxp_b2d02f0_iobas",                     					2,		2,		0,		0x001c,  US_REG_BYTE },
	{ "pxp_b2d02f0_iolim",                     					2,		2,		0,		0x001d,  US_REG_BYTE },
	{ "pxp_b2d02f0_secsts",                    					2,		2,		0,		0x001e,  US_REG_WORD },
	{ "pxp_b2d02f0_mbas",                      					2,		2,		0,		0x0020,  US_REG_WORD },
	{ "pxp_b2d02f0_mlim",                      					2,		2,		0,		0x0022,  US_REG_WORD },
	{ "pxp_b2d02f0_pbas",                      					2,		2,		0,		0x0024,  US_REG_WORD },
	{ "pxp_b2d02f0_plim",                      					2,		2,		0,		0x0026,  US_REG_WORD },
	{ "pxp_b2d02f0_pbasu",                     					2,		2,		0,		0x0028, US_REG_DWORD },
	{ "pxp_b2d02f0_plimu",                     					2,		2,		0,		0x002c, US_REG_DWORD },
	{ "pxp_b2d02f0_devsts",                    					2,		2,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b2d02f0_lnksts",                    					2,		2,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b2d02f0_sltsts",                    					2,		2,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_b2d02f0_lnksts2",                   					2,		2,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_b2d02f0_uncerrsts",                 					2,		2,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b2d02f0_corerrsts",                 					2,		2,		0,		0x0158, US_REG_DWORD },
	{ "pxp_b2d02f0_rperrsts",                  					2,		2,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b2d02f0_errsid",                    					2,		2,		0,		0x017c, US_REG_DWORD },
	{ "pxp_b2d02f0_miscctrlsts_0",             					2,		2,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b2d02f0_miscctrlsts_1",             					2,		2,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b2d02f0_xpcorerrsts",               					2,		2,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b2d02f0_xpcorerrmsk",               					2,		2,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b2d02f0_xpuncerrsts",               					2,		2,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b2d02f0_xpuncerrmsk",               					2,		2,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b2d02f0_xpuncerrsev",               					2,		2,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b2d02f0_xpuncerrptr",               					2,		2,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b2d02f0_uncedmask",                 					2,		2,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b2d02f0_coredmask",                 					2,		2,		0,		0x021c, US_REG_DWORD },
	{ "pxp_b2d02f0_rpedmask",                  					2,		2,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b2d02f0_xpuncedmask",               					2,		2,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b2d02f0_xpcoredmask",               					2,		2,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b2d02f0_xpglberrsts",               					2,		2,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b2d02f0_xpglberrptr",               					2,		2,		0,		0x0232,  US_REG_WORD },
	{ "pxp_b2d02f0_lnerrsts",                  					2,		2,		0,		0x0258, US_REG_DWORD },
	{ "pxp_b2d02f0_ler_ctrlsts",               					2,		2,		0,		0x0288, US_REG_DWORD },
	{ "pxp_b2d02f0_rppioerr_cap",              					2,		2,		0,		0x0298, US_REG_DWORD },
	{ "pxp_b2d02f0_rppioerr_status",           					2,		2,		0,		0x02a4, US_REG_DWORD },
	{ "pxp_b2d03f0_pbus",                      					2,		3,		0,		0x0018,  US_REG_BYTE },
	{ "pxp_b2d03f0_secbus",                    					2,		3,		0,		0x0019,  US_REG_BYTE },
	{ "pxp_b2d03f0_subbus",                    					2,		3,		0,		0x001a,  US_REG_BYTE },
	{ "pxp_b2d03f0_iobas",                     					2,		3,		0,		0x001c,  US_REG_BYTE },
	{ "pxp_b2d03f0_iolim",                     					2,		3,		0,		0x001d,  US_REG_BYTE },
	{ "pxp_b2d03f0_secsts",                    					2,		3,		0,		0x001e,  US_REG_WORD },
	{ "pxp_b2d03f0_mbas",                      					2,		3,		0,		0x0020,  US_REG_WORD },
	{ "pxp_b2d03f0_mlim",                      					2,		3,		0,		0x0022,  US_REG_WORD },
	{ "pxp_b2d03f0_pbas",                      					2,		3,		0,		0x0024,  US_REG_WORD },
	{ "pxp_b2d03f0_plim",                      					2,		3,		0,		0x0026,  US_REG_WORD },
	{ "pxp_b2d03f0_pbasu",                     					2,		3,		0,		0x0028, US_REG_DWORD },
	{ "pxp_b2d03f0_plimu",                     					2,		3,		0,		0x002c, US_REG_DWORD },
	{ "pxp_b2d03f0_devsts",                    					2,		3,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b2d03f0_lnksts",                    					2,		3,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b2d03f0_sltsts",                    					2,		3,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_b2d03f0_lnksts2",                   					2,		3,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_b2d03f0_uncerrsts",                 					2,		3,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b2d03f0_corerrsts",                 					2,		3,		0,		0x0158, US_REG_DWORD },
	{ "pxp_b2d03f0_rperrsts",                  					2,		3,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b2d03f0_errsid",                    					2,		3,		0,		0x017c, US_REG_DWORD },
	{ "pxp_b2d03f0_miscctrlsts_0",             					2,		3,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b2d03f0_miscctrlsts_1",             					2,		3,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b2d03f0_xpcorerrsts",               					2,		3,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b2d03f0_xpcorerrmsk",               					2,		3,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b2d03f0_xpuncerrsts",               					2,		3,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b2d03f0_xpuncerrmsk",               					2,		3,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b2d03f0_xpuncerrsev",               					2,		3,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b2d03f0_xpuncerrptr",               					2,		3,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b2d03f0_uncedmask",                 					2,		3,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b2d03f0_coredmask",                 					2,		3,		0,		0x021c, US_REG_DWORD },
	{ "pxp_b2d03f0_rpedmask",                  					2,		3,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b2d03f0_xpuncedmask",               					2,		3,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b2d03f0_xpcoredmask",               					2,		3,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b2d03f0_xpglberrsts",               					2,		3,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b2d03f0_xpglberrptr",               					2,		3,		0,		0x0232,  US_REG_WORD },
	{ "pxp_b2d03f0_lnerrsts",                  					2,		3,		0,		0x0258, US_REG_DWORD },
	{ "pxp_b2d03f0_ler_ctrlsts",               					2,		3,		0,		0x0288, US_REG_DWORD },
	{ "pxp_b2d03f0_rppioerr_cap",              					2,		3,		0,		0x0298, US_REG_DWORD },
	{ "pxp_b2d03f0_rppioerr_status",           					2,		3,		0,		0x02a4, US_REG_DWORD },
	{ "iio_mmcfg_base_b2d05f0",                					2,		5,		0,		0x0090, US_REG_QWORD },
	{ "iio_mmcfg_limit_b2d05f0",               					2,		5,		0,		0x0098, US_REG_QWORD },
	{ "iio_tseg_b2d05f0",                      					2,		5,		0,		0x00a8, US_REG_QWORD },
	{ "iio_tolm_b2d05f0",                      					2,		5,		0,		0x00d0, US_REG_DWORD },
	{ "iio_tohm_0_b2d05f0",                    					2,		5,		0,		0x00d4, US_REG_DWORD },
	{ "iio_tohm_1_b2d05f0",                    					2,		5,		0,		0x00d8, US_REG_DWORD },
	{ "iio_ncmem_base_0_b2d05f0",              					2,		5,		0,		0x00e0, US_REG_DWORD },
	{ "iio_ncmem_base_1_b2d05f0",              					2,		5,		0,		0x00e4, US_REG_DWORD },
	{ "iio_ncmem_limit_0_b2d05f0",             					2,		5,		0,		0x00e8, US_REG_DWORD },
	{ "iio_ncmem_limit_1_b2d05f0",             					2,		5,		0,		0x00ec, US_REG_DWORD },
	{ "iio_mencmem_base_0_b2d05f0",            					2,		5,		0,		0x00f0, US_REG_DWORD },
	{ "iio_mencmem_base_1_b2d05f0",            					2,		5,		0,		0x00f4, US_REG_DWORD },
	{ "iio_mencmem_limit_0_b2d05f0",           					2,		5,		0,		0x00f8, US_REG_DWORD },
	{ "iio_mencmem_limit_1_b2d05f0",           					2,		5,		0,		0x00fc, US_REG_DWORD },
	{ "iio_vtuncerrsts_b2d05f0",               					2,		5,		0,		0x01a8, US_REG_DWORD },
	{ "iio_vtuncerrptr_b2d05f0",               					2,		5,		0,		0x01b4,  US_REG_BYTE },
	{ "iio_mmioh_non_interleave_b2d05f0",      					2,		5,		0,		0x0340, US_REG_QWORD },
	{ "iio_mmioh_interleave_b2d05f0",          					2,		5,		0,		0x0348, US_REG_QWORD },
	{ "iio_irpperrsv_b2d05f2",                 					2,		5,		2,		0x0080, US_REG_QWORD },
	{ "iio_pcierrsv_b2d05f2",                  					2,		5,		2,		0x0094, US_REG_DWORD },
	{ "iio_viral_b2d05f2",                     					2,		5,		2,		0x00a0, US_REG_DWORD },
	{ "iio_lnerrmask_b2d05f2",                 					2,		5,		2,		0x019c, US_REG_DWORD },
	{ "iio_lferrmask_b2d05f2",                 					2,		5,		2,		0x01a0, US_REG_DWORD },
	{ "iio_lcerrmask_b2d05f2",                 					2,		5,		2,		0x01a4, US_REG_DWORD },
	{ "iio_lcerrst_b2d05f2",                   					2,		5,		2,		0x01a8, US_REG_DWORD },
	{ "iio_lcferrst_b2d05f2",                  					2,		5,		2,		0x01ac, US_REG_DWORD },
	{ "iio_lcnerrst_b2d05f2",                  					2,		5,		2,		0x01b8, US_REG_DWORD },
	{ "iio_lnerrst_b2d05f2",                   					2,		5,		2,		0x01c0, US_REG_DWORD },
	{ "iio_lferrst_b2d05f2",                   					2,		5,		2,		0x01c4, US_REG_DWORD },
	{ "iio_lerrctl_b2d05f2",                   					2,		5,		2,		0x01c8, US_REG_DWORD },
	{ "iio_lsysst_b2d05f2",                    					2,		5,		2,		0x01cc, US_REG_DWORD },
	{ "iio_lsysctl_b2d05f2",                   					2,		5,		2,		0x01d0, US_REG_DWORD },
	{ "iio_sysmap_b2d05f2",                    					2,		5,		2,		0x01d4, US_REG_DWORD },
	{ "iio_lbitmap_b2d05f2",                   					2,		5,		2,		0x01d8, US_REG_DWORD },
	{ "iio_lfferrst_b2d05f2",                  					2,		5,		2,		0x01dc, US_REG_DWORD },
	{ "iio_lfnerrst_b2d05f2",                  					2,		5,		2,		0x01e8, US_REG_DWORD },
	{ "iio_lnferrst_b2d05f2",                  					2,		5,		2,		0x01ec, US_REG_DWORD },
	{ "iio_lnnerrst_b2d05f2",                  					2,		5,		2,		0x01f8, US_REG_DWORD },
	{ "iio_irpringerrsv_b2d05f2",              					2,		5,		2,		0x0200, US_REG_QWORD },
	{ "iio_irpringerrst_b2d05f2",              					2,		5,		2,		0x0208, US_REG_DWORD },
	{ "iio_irpringerrctl_b2d05f2",             					2,		5,		2,		0x020c, US_REG_DWORD },
	{ "iio_irpringfferrst_b2d05f2",            					2,		5,		2,		0x0210, US_REG_DWORD },
	{ "iio_irpringfnerrst_b2d05f2",            					2,		5,		2,		0x0214, US_REG_DWORD },
	{ "iio_irpringnferrst_b2d05f2",            					2,		5,		2,		0x0218, US_REG_DWORD },
	{ "iio_irpringnnerrst_b2d05f2",            					2,		5,		2,		0x021c, US_REG_DWORD },
	{ "iio_irpringerrcntsel_b2d05f2",          					2,		5,		2,		0x0220, US_REG_DWORD },
	{ "iio_irpringerrcnt_b2d05f2",             					2,		5,		2,		0x0224, US_REG_DWORD },
	{ "iio_irpringmisc_b2d05f2",               					2,		5,		2,		0x0228, US_REG_DWORD },
	{ "iio_irpp0errst_b2d05f2",                					2,		5,		2,		0x0230, US_REG_DWORD },
	{ "iio_irpp0errctl_b2d05f2",               					2,		5,		2,		0x0234, US_REG_DWORD },
	{ "iio_irpp0fferrst_b2d05f2",              					2,		5,		2,		0x0238, US_REG_DWORD },
	{ "iio_irpp0fnerrst_b2d05f2",              					2,		5,		2,		0x023c, US_REG_DWORD },
	{ "iio_irpp0fferrhd0_b2d05f2",             					2,		5,		2,		0x0240, US_REG_DWORD },
	{ "iio_irpp0fferrhd1_b2d05f2",             					2,		5,		2,		0x0244, US_REG_DWORD },
	{ "iio_irpp0fferrhd2_b2d05f2",             					2,		5,		2,		0x0248, US_REG_DWORD },
	{ "iio_irpp0fferrhd3_b2d05f2",             					2,		5,		2,		0x024c, US_REG_DWORD },
	{ "iio_irpp0nferrst_b2d05f2",              					2,		5,		2,		0x0250, US_REG_DWORD },
	{ "iio_irpp0nnerrst_b2d05f2",              					2,		5,		2,		0x0254, US_REG_DWORD },
	{ "iio_irpp0errcntsel_b2d05f2",            					2,		5,		2,		0x0268, US_REG_DWORD },
	{ "iio_irpp0errcnt_b2d05f2",               					2,		5,		2,		0x026c, US_REG_DWORD },
	{ "iio_irpp0plsr0_b2d05f2",                					2,		5,		2,		0x0270, US_REG_DWORD },
	{ "iio_irpp0plsr1_b2d05f2",                					2,		5,		2,		0x0274, US_REG_DWORD },
	{ "iio_irpp0plsr2_b2d05f2",                					2,		5,		2,		0x0278, US_REG_DWORD },
	{ "iio_tcerrst_b2d05f2",                   					2,		5,		2,		0x0288, US_REG_DWORD },
	{ "iio_tcerrctl_b2d05f2",                  					2,		5,		2,		0x028c, US_REG_DWORD },
	{ "iio_itcerrsev_b2d05f2",                 					2,		5,		2,		0x0290, US_REG_DWORD },
	{ "iio_otcerrsev_b2d05f2",                 					2,		5,		2,		0x0294, US_REG_DWORD },
	{ "iio_tcerrcntsel_b2d05f2",               					2,		5,		2,		0x0298, US_REG_DWORD },
	{ "iio_tcerrcnt_b2d05f2",                  					2,		5,		2,		0x029c, US_REG_DWORD },
	{ "iio_itcfferrst_b2d05f2",                					2,		5,		2,		0x02a0, US_REG_DWORD },
	{ "iio_itcfferrhd0_b2d05f2",               					2,		5,		2,		0x02a4, US_REG_DWORD },
	{ "iio_itcfferrhd1_b2d05f2",               					2,		5,		2,		0x02a8, US_REG_DWORD },
	{ "iio_itcfferrhd2_b2d05f2",               					2,		5,		2,		0x02ac, US_REG_DWORD },
	{ "iio_itcfferrhd3_b2d05f2",               					2,		5,		2,		0x02b0, US_REG_DWORD },
	{ "iio_itcfnerrst_b2d05f2",                					2,		5,		2,		0x02b4, US_REG_DWORD },
	{ "iio_itcnferrst_b2d05f2",                					2,		5,		2,		0x02b8, US_REG_DWORD },
	{ "iio_itcnnerrst_b2d05f2",                					2,		5,		2,		0x02cc, US_REG_DWORD },
	{ "iio_otcfferrst_b2d05f2",                					2,		5,		2,		0x02d0, US_REG_DWORD },
	{ "iio_otcfferrhd0_b2d05f2",               					2,		5,		2,		0x02d4, US_REG_DWORD },
	{ "iio_otcfferrhd1_b2d05f2",               					2,		5,		2,		0x02d8, US_REG_DWORD },
	{ "iio_otcfferrhd2_b2d05f2",               					2,		5,		2,		0x02dc, US_REG_DWORD },
	{ "iio_otcfferrhd3_b2d05f2",               					2,		5,		2,		0x02e0, US_REG_DWORD },
	{ "iio_otcfnerrst_b2d05f2",                					2,		5,		2,		0x02e4, US_REG_DWORD },
	{ "iio_otcnferrst_b2d05f2",                					2,		5,		2,		0x02e8, US_REG_DWORD },
	{ "iio_otcnferrhd0_b2d05f2",               					2,		5,		2,		0x02ec, US_REG_DWORD },
	{ "iio_otcnnerrst_b2d05f2",                					2,		5,		2,		0x02fc, US_REG_DWORD },
	{ "iio_iioerrctl_b2d05f2",                 					2,		5,		2,		0x0304, US_REG_DWORD },
	{ "pxp_b2d07f0_devsts",                    					2,		7,		0,		0x004a,  US_REG_WORD },
	{ "pxp_b2d07f0_lnksts",                    					2,		7,		0,		0x0052,  US_REG_WORD },
	{ "pxp_b2d07f0_lnksts2",                   					2,		7,		0,		0x0072,  US_REG_WORD },
	{ "pxp_b2d07f0_sltsts2",                   					2,		7,		0,		0x007a,  US_REG_WORD },
	{ "pxp_b2d07f1_devsts",                    					2,		7,		1,		0x004a,  US_REG_WORD },
	{ "pxp_b2d07f1_lnksts",                    					2,		7,		1,		0x0052,  US_REG_WORD },
	{ "pxp_b2d07f1_lnksts2",                   					2,		7,		1,		0x0072,  US_REG_WORD },
	{ "pxp_b2d07f1_sltsts2",                   					2,		7,		1,		0x007a,  US_REG_WORD },
	{ "pxp_b2d07f2_devsts",                    					2,		7,		2,		0x004a,  US_REG_WORD },
	{ "pxp_b2d07f2_lnksts",                    					2,		7,		2,		0x0052,  US_REG_WORD },
	{ "pxp_b2d07f2_lnksts2",                   					2,		7,		2,		0x0072,  US_REG_WORD },
	{ "pxp_b2d07f2_sltsts2",                   					2,		7,		2,		0x007a,  US_REG_WORD },
	{ "pxp_b2d07f3_devsts",                    					2,		7,		3,		0x004a,  US_REG_WORD },
	{ "pxp_b2d07f3_lnksts",                    					2,		7,		3,		0x0052,  US_REG_WORD },
	{ "pxp_b2d07f3_lnksts2",                   					2,		7,		3,		0x0072,  US_REG_WORD },
	{ "pxp_b2d07f3_sltsts2",                   					2,		7,		3,		0x007a,  US_REG_WORD },
	{ "pxp_b2d07f4_devsts",                    					2,		7,		4,		0x004a,  US_REG_WORD },
	{ "pxp_b2d07f4_lnksts",                    					2,		7,		4,		0x0052,  US_REG_WORD },
	{ "pxp_b2d07f4_lnksts2",                   					2,		7,		4,		0x0072,  US_REG_WORD },
	{ "pxp_b2d07f4_sltsts2",                   					2,		7,		4,		0x007a,  US_REG_WORD },
	{ "pxp_b2d07f7_devsts",                    					2,		7,		7,		0x004a,  US_REG_WORD },
	{ "pxp_b2d07f7_lnksts",                    					2,		7,		7,		0x0052,  US_REG_WORD },
	{ "pxp_b2d07f7_lnksts2",                   					2,		7,		7,		0x0072,  US_REG_WORD },
	{ "pxp_b2d07f7_sltsts2",                   					2,		7,		7,		0x007a,  US_REG_WORD },
	{ "pxp_b2d07f7_tswdbgerrstdis0",           					2,		7,		7,		0x0358, US_REG_DWORD },
	{ "imc0_m2mem_mode",                       					2,		8,		0,		0x0080, US_REG_DWORD },
	{ "imc0_m2mem_topology",                   					2,		8,		0,		0x0088, US_REG_DWORD },
	{ "imc0_m2mem_nid2kti",                    					2,		8,		0,		0x0090, US_REG_DWORD },
	{ "imc0_m2mem_credits",                    					2,		8,		0,		0x00a0, US_REG_DWORD },
	{ "imc0_m2mem_creditthresholds",           					2,		8,		0,		0x00a4, US_REG_DWORD },
	{ "imc0_m2mem_sad2tad",                    					2,		8,		0,		0x00a8, US_REG_DWORD },
	{ "imc0_m2mem_dad",                        					2,		8,		0,		0x00ac, US_REG_DWORD },
	{ "imc0_m2mem_pad0",                       					2,		8,		0,		0x00b0, US_REG_QWORD },
	{ "imc0_m2mem_pad1",                       					2,		8,		0,		0x00b8, US_REG_QWORD },
	{ "imc0_m2mem_pad2",                       					2,		8,		0,		0x00c0, US_REG_QWORD },
	{ "imc0_m2mem_prefetchsad",                					2,		8,		0,		0x00d0, US_REG_DWORD },
	{ "imc0_m2mem_prefsadconfig",              					2,		8,		0,		0x00d4, US_REG_DWORD },
	{ "imc0_m2mem_memory",                     					2,		8,		0,		0x00d8, US_REG_DWORD },
	{ "imc0_m2mem_sysfeatures0",               					2,		8,		0,		0x00e0, US_REG_DWORD },
	{ "imc0_m2mem_defeatures0",                					2,		8,		0,		0x00e4, US_REG_DWORD },
	{ "imc0_m2mem_defeatures1",                					2,		8,		0,		0x00e8, US_REG_DWORD },
	{ "imc0_m2mem_timeout",                    					2,		8,		0,		0x0104, US_REG_DWORD },
	{ "imc0_m2mem_m2mbiosquiesce",             					2,		8,		0,		0x010c, US_REG_DWORD },
	{ "imc0_m2mem_mci_status_shadow",          					2,		8,		0,		0x0120, US_REG_QWORD },
	{ "imc0_m2mem_mci_addr_shadow0",           					2,		8,		0,		0x0128, US_REG_DWORD },
	{ "imc0_m2mem_mci_addr_shadow1",           					2,		8,		0,		0x012c, US_REG_DWORD },
	{ "imc0_m2mem_mci_misc_shadow",            					2,		8,		0,		0x0130, US_REG_QWORD },
	{ "imc0_m2mem_err_cntr_ctl",               					2,		8,		0,		0x0140, US_REG_DWORD },
	{ "imc0_m2mem_err_cntr",                   					2,		8,		0,		0x0144, US_REG_DWORD },
	{ "imc1_m2mem_mode",                       					2,		9,		0,		0x0080, US_REG_DWORD },
	{ "imc1_m2mem_topology",                   					2,		9,		0,		0x0088, US_REG_DWORD },
	{ "imc1_m2mem_nid2kti",                    					2,		9,		0,		0x0090, US_REG_DWORD },
	{ "imc1_m2mem_credits",                    					2,		9,		0,		0x00a0, US_REG_DWORD },
	{ "imc1_m2mem_creditthresholds",           					2,		9,		0,		0x00a4, US_REG_DWORD },
	{ "imc1_m2mem_sad2tad",                    					2,		9,		0,		0x00a8, US_REG_DWORD },
	{ "imc1_m2mem_dad",                        					2,		9,		0,		0x00ac, US_REG_DWORD },
	{ "imc1_m2mem_pad0",                       					2,		9,		0,		0x00b0, US_REG_QWORD },
	{ "imc1_m2mem_pad1",                       					2,		9,		0,		0x00b8, US_REG_QWORD },
	{ "imc1_m2mem_pad2",                       					2,		9,		0,		0x00c0, US_REG_QWORD },
	{ "imc1_m2mem_prefetchsad",                					2,		9,		0,		0x00d0, US_REG_DWORD },
	{ "imc1_m2mem_prefsadconfig",              					2,		9,		0,		0x00d4, US_REG_DWORD },
	{ "imc1_m2mem_memory",                     					2,		9,		0,		0x00d8, US_REG_DWORD },
	{ "imc1_m2mem_sysfeatures0",               					2,		9,		0,		0x00e0, US_REG_DWORD },
	{ "imc1_m2mem_defeatures0",                					2,		9,		0,		0x00e4, US_REG_DWORD },
	{ "imc1_m2mem_defeatures1",                					2,		9,		0,		0x00e8, US_REG_DWORD },
	{ "imc1_m2mem_timeout",                    					2,		9,		0,		0x0104, US_REG_DWORD },
	{ "imc1_m2mem_m2mbiosquiesce",             					2,		9,		0,		0x010c, US_REG_DWORD },
	{ "imc1_m2mem_mci_status_shadow",          					2,		9,		0,		0x0120, US_REG_QWORD },
	{ "imc1_m2mem_mci_addr_shadow0",           					2,		9,		0,		0x0128, US_REG_DWORD },
	{ "imc1_m2mem_mci_addr_shadow1",           					2,		9,		0,		0x012c, US_REG_DWORD },
	{ "imc1_m2mem_mci_misc_shadow",            					2,		9,		0,		0x0130, US_REG_QWORD },
	{ "imc1_m2mem_err_cntr_ctl",               					2,		9,		0,		0x0140, US_REG_DWORD },
	{ "imc1_m2mem_err_cntr",                   					2,		9,		0,		0x0144, US_REG_DWORD },
	{ "imc0_c0_dimmmtr_0",                     					2,		10,		0,		0x0080, US_REG_DWORD },
	{ "imc0_c0_dimmmtr_1",                     					2,		10,		0,		0x0084, US_REG_DWORD },
	{ "imc0_c0_dimmmtr_2",                     					2,		10,		0,		0x0088, US_REG_DWORD },
	{ "imc0_mc_route_table_cfg",               					2,		10,		0,		0x07c0, US_REG_DWORD },
	{ "imc0_tadbase_0",                        					2,		10,		0,		0x0850, US_REG_DWORD },
	{ "imc0_tadbase_1",                        					2,		10,		0,		0x0854, US_REG_DWORD },
	{ "imc0_tadbase_2",                        					2,		10,		0,		0x0858, US_REG_DWORD },
	{ "imc0_tadbase_3",                        					2,		10,		0,		0x085c, US_REG_DWORD },
	{ "imc0_tadbase_4",                        					2,		10,		0,		0x0860, US_REG_DWORD },
	{ "imc0_tadbase_5",                        					2,		10,		0,		0x0864, US_REG_DWORD },
	{ "imc0_tadbase_6",                        					2,		10,		0,		0x0868, US_REG_DWORD },
	{ "imc0_tadbase_7",                        					2,		10,		0,		0x086c, US_REG_DWORD },
	{ "imc0_imc0_poison_source",               					2,		10,		0,		0x0980, US_REG_QWORD },
	{ "imc0_imc1_poison_source",               					2,		10,		0,		0x0988, US_REG_QWORD },
	{ "imc0_imc2_poison_source",               					2,		10,		0,		0x0990, US_REG_QWORD },
	{ "imc0_c0_ddrt_retry_fsm_state",          					2,		10,		1,		0x0904, US_REG_DWORD },
	{ "imc0_c0_ddrt_fnv0_event0",              					2,		10,		1,		0x0a60, US_REG_DWORD },
	{ "imc0_c0_ddrt_fnv0_event1",              					2,		10,		1,		0x0a64, US_REG_DWORD },
	{ "imc0_c0_ddrt_fnv1_event0",              					2,		10,		1,		0x0a70, US_REG_DWORD },
	{ "imc0_c0_ddrt_fnv1_event1",              					2,		10,		1,		0x0a74, US_REG_DWORD },
	{ "imc0_c0_ddrt_retry_status",             					2,		10,		1,		0x0a98, US_REG_DWORD },
	{ "imc0_c0_ddrt_dimm_info_dimm0",          					2,		10,		1,		0x0aa0, US_REG_DWORD },
	{ "imc0_c0_ddrt_dimm_info_dimm1",          					2,		10,		1,		0x0aa4, US_REG_DWORD },
	{ "imc0_c0_ddrt_error",                    					2,		10,		1,		0x0d24, US_REG_DWORD },
	{ "imc0_c0_ddrt_err_log_1st",              					2,		10,		1,		0x0d80, US_REG_DWORD },
	{ "imc0_c0_ddrt_err_log_next",             					2,		10,		1,		0x0d84, US_REG_DWORD },
	{ "imc0_c0_correrrcnt_0",                  					2,		10,		3,		0x0104, US_REG_DWORD },
	{ "imc0_c0_correrrcnt_1",                  					2,		10,		3,		0x0108, US_REG_DWORD },
	{ "imc0_c0_correrrcnt_2",                  					2,		10,		3,		0x010c, US_REG_DWORD },
	{ "imc0_c0_correrrcnt_3",                  					2,		10,		3,		0x0110, US_REG_DWORD },
	{ "imc0_c0_retry_rd_err_log_address2",     					2,		10,		3,		0x0114, US_REG_DWORD },
	{ "imc0_c0_detection_debug_log_address2",  					2,		10,		3,		0x0118, US_REG_DWORD },
	{ "imc0_c0_correrrthrshld_0",              					2,		10,		3,		0x011c, US_REG_DWORD },
	{ "imc0_c0_correrrthrshld_1",              					2,		10,		3,		0x0120, US_REG_DWORD },
	{ "imc0_c0_correrrthrshld_2",              					2,		10,		3,		0x0124, US_REG_DWORD },
	{ "imc0_c0_correrrthrshld_3",              					2,		10,		3,		0x0128, US_REG_DWORD },
	{ "imc0_c0_detection_debug_log_address1",  					2,		10,		3,		0x012c, US_REG_DWORD },
	{ "imc0_c0_correrrorstatus",               					2,		10,		3,		0x0134, US_REG_DWORD },
	{ "imc0_c0_devtag_cntl_0",                 					2,		10,		3,		0x0140,  US_REG_BYTE },
	{ "imc0_c0_devtag_cntl_1",                 					2,		10,		3,		0x0141,  US_REG_BYTE },
	{ "imc0_c0_devtag_cntl_2",                 					2,		10,		3,		0x0142,  US_REG_BYTE },
	{ "imc0_c0_devtag_cntl_3",                 					2,		10,		3,		0x0143,  US_REG_BYTE },
	{ "imc0_c0_devtag_cntl_4",                 					2,		10,		3,		0x0144,  US_REG_BYTE },
	{ "imc0_c0_devtag_cntl_5",                 					2,		10,		3,		0x0145,  US_REG_BYTE },
	{ "imc0_c0_devtag_cntl_6",                 					2,		10,		3,		0x0146,  US_REG_BYTE },
	{ "imc0_c0_devtag_cntl_7",                 					2,		10,		3,		0x0147,  US_REG_BYTE },
	{ "imc0_c0_retry_rd_err_log_misc",         					2,		10,		3,		0x0148, US_REG_DWORD },
	{ "imc0_c0_detection_debug_log",           					2,		10,		3,		0x014c, US_REG_DWORD },
	{ "imc0_c0_retry_rd_err_log_parity",       					2,		10,		3,		0x0150, US_REG_DWORD },
	{ "imc0_c0_retry_rd_err_log",              					2,		10,		3,		0x0154, US_REG_DWORD },
	{ "imc0_c0_retry_rd_err_log_address1",     					2,		10,		3,		0x015c, US_REG_DWORD },
	{ "imc0_c0_detection_debug_log_parity",    					2,		10,		3,		0x016c, US_REG_DWORD },
	{ "imc0_c0_detection_debug_log_locator",   					2,		10,		3,		0x019c, US_REG_DWORD },
	{ "imc0_c0_link_error",                    					2,		10,		3,		0x0308, US_REG_DWORD },
	{ "imc0_c0_link_retry_sb_err_count",       					2,		10,		3,		0x0400, US_REG_DWORD },
	{ "imc0_c0_link_retry_err_limits",         					2,		10,		3,		0x040c, US_REG_DWORD },
	{ "imc0_c0_link_err_fsm_state",            					2,		10,		3,		0x0420, US_REG_DWORD },
	{ "imc0_c1_dimmmtr_0",                     					2,		10,		4,		0x0080, US_REG_DWORD },
	{ "imc0_c1_dimmmtr_1",                     					2,		10,		4,		0x0084, US_REG_DWORD },
	{ "imc0_c1_dimmmtr_2",                     					2,		10,		4,		0x0088, US_REG_DWORD },
	{ "imc0_c1_ddrt_retry_fsm_state",          					2,		10,		5,		0x0904, US_REG_DWORD },
	{ "imc0_c1_ddrt_fnv0_event0",              					2,		10,		5,		0x0a60, US_REG_DWORD },
	{ "imc0_c1_ddrt_fnv0_event1",              					2,		10,		5,		0x0a64, US_REG_DWORD },
	{ "imc0_c1_ddrt_fnv1_event0",              					2,		10,		5,		0x0a70, US_REG_DWORD },
	{ "imc0_c1_ddrt_fnv1_event1",              					2,		10,		5,		0x0a74, US_REG_DWORD },
	{ "imc0_c1_ddrt_retry_status",             					2,		10,		5,		0x0a98, US_REG_DWORD },
	{ "imc0_c1_ddrt_dimm_info_dimm0",          					2,		10,		5,		0x0aa0, US_REG_DWORD },
	{ "imc0_c1_ddrt_dimm_info_dimm1",          					2,		10,		5,		0x0aa4, US_REG_DWORD },
	{ "imc0_c1_ddrt_error",                    					2,		10,		5,		0x0d24, US_REG_DWORD },
	{ "imc0_c1_ddrt_err_log_1st",              					2,		10,		5,		0x0d80, US_REG_DWORD },
	{ "imc0_c1_ddrt_err_log_next",             					2,		10,		5,		0x0d84, US_REG_DWORD },
	{ "imc0_c1_correrrcnt_0",                  					2,		10,		7,		0x0104, US_REG_DWORD },
	{ "imc0_c1_correrrcnt_1",                  					2,		10,		7,		0x0108, US_REG_DWORD },
	{ "imc0_c1_correrrcnt_2",                  					2,		10,		7,		0x010c, US_REG_DWORD },
	{ "imc0_c1_correrrcnt_3",                  					2,		10,		7,		0x0110, US_REG_DWORD },
	{ "imc0_c1_retry_rd_err_log_address2",     					2,		10,		7,		0x0114, US_REG_DWORD },
	{ "imc0_c1_detection_debug_log_address2",  					2,		10,		7,		0x0118, US_REG_DWORD },
	{ "imc0_c1_correrrthrshld_0",              					2,		10,		7,		0x011c, US_REG_DWORD },
	{ "imc0_c1_correrrthrshld_1",              					2,		10,		7,		0x0120, US_REG_DWORD },
	{ "imc0_c1_correrrthrshld_2",              					2,		10,		7,		0x0124, US_REG_DWORD },
	{ "imc0_c1_correrrthrshld_3",              					2,		10,		7,		0x0128, US_REG_DWORD },
	{ "imc0_c1_detection_debug_log_address1",  					2,		10,		7,		0x012c, US_REG_DWORD },
	{ "imc0_c1_correrrorstatus",               					2,		10,		7,		0x0134, US_REG_DWORD },
	{ "imc0_c1_devtag_cntl_0",                 					2,		10,		7,		0x0140,  US_REG_BYTE },
	{ "imc0_c1_devtag_cntl_1",                 					2,		10,		7,		0x0141,  US_REG_BYTE },
	{ "imc0_c1_devtag_cntl_2",                 					2,		10,		7,		0x0142,  US_REG_BYTE },
	{ "imc0_c1_devtag_cntl_3",                 					2,		10,		7,		0x0143,  US_REG_BYTE },
	{ "imc0_c1_devtag_cntl_4",                 					2,		10,		7,		0x0144,  US_REG_BYTE },
	{ "imc0_c1_devtag_cntl_5",                 					2,		10,		7,		0x0145,  US_REG_BYTE },
	{ "imc0_c1_devtag_cntl_6",                 					2,		10,		7,		0x0146,  US_REG_BYTE },
	{ "imc0_c1_devtag_cntl_7",                 					2,		10,		7,		0x0147,  US_REG_BYTE },
	{ "imc0_c1_retry_rd_err_log_misc",         					2,		10,		7,		0x0148, US_REG_DWORD },
	{ "imc0_c1_detection_debug_log",           					2,		10,		7,		0x014c, US_REG_DWORD },
	{ "imc0_c1_retry_rd_err_log_parity",       					2,		10,		7,		0x0150, US_REG_DWORD },
	{ "imc0_c1_retry_rd_err_log",              					2,		10,		7,		0x0154, US_REG_DWORD },
	{ "imc0_c1_retry_rd_err_log_address1",     					2,		10,		7,		0x015c, US_REG_DWORD },
	{ "imc0_c1_detection_debug_log_parity",    					2,		10,		7,		0x016c, US_REG_DWORD },
	{ "imc0_c1_detection_debug_log_locator",   					2,		10,		7,		0x019c, US_REG_DWORD },
	{ "imc0_c1_link_error",                    					2,		10,		7,		0x0308, US_REG_DWORD },
	{ "imc0_c1_link_retry_sb_err_count",       					2,		10,		7,		0x0400, US_REG_DWORD },
	{ "imc0_c1_link_retry_err_limits",         					2,		10,		7,		0x040c, US_REG_DWORD },
	{ "imc0_c1_link_err_fsm_state",            					2,		10,		7,		0x0420, US_REG_DWORD },
	{ "imc0_c2_dimmmtr_0",                     					2,		11,		0,		0x0080, US_REG_DWORD },
	{ "imc0_c2_dimmmtr_1",                     					2,		11,		0,		0x0084, US_REG_DWORD },
	{ "imc0_c2_dimmmtr_2",                     					2,		11,		0,		0x0088, US_REG_DWORD },
	{ "imc0_c2_ddrt_retry_fsm_state",          					2,		11,		1,		0x0904, US_REG_DWORD },
	{ "imc0_c2_ddrt_fnv0_event0",              					2,		11,		1,		0x0a60, US_REG_DWORD },
	{ "imc0_c2_ddrt_fnv0_event1",              					2,		11,		1,		0x0a64, US_REG_DWORD },
	{ "imc0_c2_ddrt_fnv1_event0",              					2,		11,		1,		0x0a70, US_REG_DWORD },
	{ "imc0_c2_ddrt_fnv1_event1",              					2,		11,		1,		0x0a74, US_REG_DWORD },
	{ "imc0_c2_ddrt_retry_status",             					2,		11,		1,		0x0a98, US_REG_DWORD },
	{ "imc0_c2_ddrt_dimm_info_dimm0",          					2,		11,		1,		0x0aa0, US_REG_DWORD },
	{ "imc0_c2_ddrt_dimm_info_dimm1",          					2,		11,		1,		0x0aa4, US_REG_DWORD },
	{ "imc0_c2_ddrt_error",                    					2,		11,		1,		0x0d24, US_REG_DWORD },
	{ "imc0_c2_ddrt_err_log_1st",              					2,		11,		1,		0x0d80, US_REG_DWORD },
	{ "imc0_c2_ddrt_err_log_next",             					2,		11,		1,		0x0d84, US_REG_DWORD },
	{ "imc0_c2_correrrcnt_0",                  					2,		11,		3,		0x0104, US_REG_DWORD },
	{ "imc0_c2_correrrcnt_1",                  					2,		11,		3,		0x0108, US_REG_DWORD },
	{ "imc0_c2_correrrcnt_2",                  					2,		11,		3,		0x010c, US_REG_DWORD },
	{ "imc0_c2_correrrcnt_3",                  					2,		11,		3,		0x0110, US_REG_DWORD },
	{ "imc0_c2_retry_rd_err_log_address2",     					2,		11,		3,		0x0114, US_REG_DWORD },
	{ "imc0_c2_detection_debug_log_address2",  					2,		11,		3,		0x0118, US_REG_DWORD },
	{ "imc0_c2_correrrthrshld_0",              					2,		11,		3,		0x011c, US_REG_DWORD },
	{ "imc0_c2_correrrthrshld_1",              					2,		11,		3,		0x0120, US_REG_DWORD },
	{ "imc0_c2_correrrthrshld_2",              					2,		11,		3,		0x0124, US_REG_DWORD },
	{ "imc0_c2_correrrthrshld_3",              					2,		11,		3,		0x0128, US_REG_DWORD },
	{ "imc0_c2_detection_debug_log_address1",  					2,		11,		3,		0x012c, US_REG_DWORD },
	{ "imc0_c2_correrrorstatus",               					2,		11,		3,		0x0134, US_REG_DWORD },
	{ "imc0_c2_devtag_cntl_0",                 					2,		11,		3,		0x0140,  US_REG_BYTE },
	{ "imc0_c2_devtag_cntl_1",                 					2,		11,		3,		0x0141,  US_REG_BYTE },
	{ "imc0_c2_devtag_cntl_2",                 					2,		11,		3,		0x0142,  US_REG_BYTE },
	{ "imc0_c2_devtag_cntl_3",                 					2,		11,		3,		0x0143,  US_REG_BYTE },
	{ "imc0_c2_devtag_cntl_4",                 					2,		11,		3,		0x0144,  US_REG_BYTE },
	{ "imc0_c2_devtag_cntl_5",                 					2,		11,		3,		0x0145,  US_REG_BYTE },
	{ "imc0_c2_devtag_cntl_6",                 					2,		11,		3,		0x0146,  US_REG_BYTE },
	{ "imc0_c2_devtag_cntl_7",                 					2,		11,		3,		0x0147,  US_REG_BYTE },
	{ "imc0_c2_retry_rd_err_log_misc",         					2,		11,		3,		0x0148, US_REG_DWORD },
	{ "imc0_c2_detection_debug_log",           					2,		11,		3,		0x014c, US_REG_DWORD },
	{ "imc0_c2_retry_rd_err_log_parity",       					2,		11,		3,		0x0150, US_REG_DWORD },
	{ "imc0_c2_retry_rd_err_log",              					2,		11,		3,		0x0154, US_REG_DWORD },
	{ "imc0_c2_retry_rd_err_log_address1",     					2,		11,		3,		0x015c, US_REG_DWORD },
	{ "imc0_c2_detection_debug_log_parity",    					2,		11,		3,		0x016c, US_REG_DWORD },
	{ "imc0_c2_detection_debug_log_locator",   					2,		11,		3,		0x019c, US_REG_DWORD },
	{ "imc0_c2_link_error",                    					2,		11,		3,		0x0308, US_REG_DWORD },
	{ "imc0_c2_link_retry_sb_err_count",       					2,		11,		3,		0x0400, US_REG_DWORD },
	{ "imc0_c2_link_retry_err_limits",         					2,		11,		3,		0x040c, US_REG_DWORD },
	{ "imc0_c2_link_err_fsm_state",            					2,		11,		3,		0x0420, US_REG_DWORD },
	{ "imc1_c0_dimmmtr_0",                     					2,		12,		0,		0x0080, US_REG_DWORD },
	{ "imc1_c0_dimmmtr_1",                     					2,		12,		0,		0x0084, US_REG_DWORD },
	{ "imc1_c0_dimmmtr_2",                     					2,		12,		0,		0x0088, US_REG_DWORD },
	{ "imc1_mc_route_table_cfg",               					2,		12,		0,		0x07c0, US_REG_DWORD },
	{ "imc1_tadbase_0",                        					2,		12,		0,		0x0850, US_REG_DWORD },
	{ "imc1_tadbase_1",                        					2,		12,		0,		0x0854, US_REG_DWORD },
	{ "imc1_tadbase_2",                        					2,		12,		0,		0x0858, US_REG_DWORD },
	{ "imc1_tadbase_3",                        					2,		12,		0,		0x085c, US_REG_DWORD },
	{ "imc1_tadbase_4",                        					2,		12,		0,		0x0860, US_REG_DWORD },
	{ "imc1_tadbase_5",                        					2,		12,		0,		0x0864, US_REG_DWORD },
	{ "imc1_tadbase_6",                        					2,		12,		0,		0x0868, US_REG_DWORD },
	{ "imc1_tadbase_7",                        					2,		12,		0,		0x086c, US_REG_DWORD },
	{ "imc1_imc0_poison_source",               					2,		12,		0,		0x0980, US_REG_QWORD },
	{ "imc1_imc1_poison_source",               					2,		12,		0,		0x0988, US_REG_QWORD },
	{ "imc1_imc2_poison_source",               					2,		12,		0,		0x0990, US_REG_QWORD },
	{ "imc1_c0_ddrt_retry_fsm_state",          					2,		12,		1,		0x0904, US_REG_DWORD },
	{ "imc1_c0_ddrt_fnv0_event0",              					2,		12,		1,		0x0a60, US_REG_DWORD },
	{ "imc1_c0_ddrt_fnv0_event1",              					2,		12,		1,		0x0a64, US_REG_DWORD },
	{ "imc1_c0_ddrt_fnv1_event0",              					2,		12,		1,		0x0a70, US_REG_DWORD },
	{ "imc1_c0_ddrt_fnv1_event1",              					2,		12,		1,		0x0a74, US_REG_DWORD },
	{ "imc1_c0_ddrt_retry_status",             					2,		12,		1,		0x0a98, US_REG_DWORD },
	{ "imc1_c0_ddrt_dimm_info_dimm0",          					2,		12,		1,		0x0aa0, US_REG_DWORD },
	{ "imc1_c0_ddrt_dimm_info_dimm1",          					2,		12,		1,		0x0aa4, US_REG_DWORD },
	{ "imc1_c0_ddrt_error",                    					2,		12,		1,		0x0d24, US_REG_DWORD },
	{ "imc1_c0_ddrt_err_log_1st",              					2,		12,		1,		0x0d80, US_REG_DWORD },
	{ "imc1_c0_ddrt_err_log_next",             					2,		12,		1,		0x0d84, US_REG_DWORD },
	{ "imc1_c0_correrrcnt_0",                  					2,		12,		3,		0x0104, US_REG_DWORD },
	{ "imc1_c0_correrrcnt_1",                  					2,		12,		3,		0x0108, US_REG_DWORD },
	{ "imc1_c0_correrrcnt_2",                  					2,		12,		3,		0x010c, US_REG_DWORD },
	{ "imc1_c0_correrrcnt_3",                  					2,		12,		3,		0x0110, US_REG_DWORD },
	{ "imc1_c0_retry_rd_err_log_address2",     					2,		12,		3,		0x0114, US_REG_DWORD },
	{ "imc1_c0_detection_debug_log_address2",  					2,		12,		3,		0x0118, US_REG_DWORD },
	{ "imc1_c0_correrrthrshld_0",              					2,		12,		3,		0x011c, US_REG_DWORD },
	{ "imc1_c0_correrrthrshld_1",              					2,		12,		3,		0x0120, US_REG_DWORD },
	{ "imc1_c0_correrrthrshld_2",              					2,		12,		3,		0x0124, US_REG_DWORD },
	{ "imc1_c0_correrrthrshld_3",              					2,		12,		3,		0x0128, US_REG_DWORD },
	{ "imc1_c0_detection_debug_log_address1",  					2,		12,		3,		0x012c, US_REG_DWORD },
	{ "imc1_c0_correrrorstatus",               					2,		12,		3,		0x0134, US_REG_DWORD },
	{ "imc1_c0_devtag_cntl_0",                 					2,		12,		3,		0x0140,  US_REG_BYTE },
	{ "imc1_c0_devtag_cntl_1",                 					2,		12,		3,		0x0141,  US_REG_BYTE },
	{ "imc1_c0_devtag_cntl_2",                 					2,		12,		3,		0x0142,  US_REG_BYTE },
	{ "imc1_c0_devtag_cntl_3",                 					2,		12,		3,		0x0143,  US_REG_BYTE },
	{ "imc1_c0_devtag_cntl_4",                 					2,		12,		3,		0x0144,  US_REG_BYTE },
	{ "imc1_c0_devtag_cntl_5",                 					2,		12,		3,		0x0145,  US_REG_BYTE },
	{ "imc1_c0_devtag_cntl_6",                 					2,		12,		3,		0x0146,  US_REG_BYTE },
	{ "imc1_c0_devtag_cntl_7",                 					2,		12,		3,		0x0147,  US_REG_BYTE },
	{ "imc1_c0_retry_rd_err_log_misc",         					2,		12,		3,		0x0148, US_REG_DWORD },
	{ "imc1_c0_detection_debug_log",           					2,		12,		3,		0x014c, US_REG_DWORD },
	{ "imc1_c0_retry_rd_err_log_parity",       					2,		12,		3,		0x0150, US_REG_DWORD },
	{ "imc1_c0_retry_rd_err_log",              					2,		12,		3,		0x0154, US_REG_DWORD },
	{ "imc1_c0_retry_rd_err_log_address1",     					2,		12,		3,		0x015c, US_REG_DWORD },
	{ "imc1_c0_detection_debug_log_parity",    					2,		12,		3,		0x016c, US_REG_DWORD },
	{ "imc1_c0_detection_debug_log_locator",   					2,		12,		3,		0x019c, US_REG_DWORD },
	{ "imc1_c0_link_error",                    					2,		12,		3,		0x0308, US_REG_DWORD },
	{ "imc1_c0_link_retry_sb_err_count",       					2,		12,		3,		0x0400, US_REG_DWORD },
	{ "imc1_c0_link_retry_err_limits",         					2,		12,		3,		0x040c, US_REG_DWORD },
	{ "imc1_c0_link_err_fsm_state",            					2,		12,		3,		0x0420, US_REG_DWORD },
	{ "imc1_c1_dimmmtr_0",                     					2,		12,		4,		0x0080, US_REG_DWORD },
	{ "imc1_c1_dimmmtr_1",                     					2,		12,		4,		0x0084, US_REG_DWORD },
	{ "imc1_c1_dimmmtr_2",                     					2,		12,		4,		0x0088, US_REG_DWORD },
	{ "imc1_c1_ddrt_retry_fsm_state",          					2,		12,		5,		0x0904, US_REG_DWORD },
	{ "imc1_c1_ddrt_fnv0_event0",              					2,		12,		5,		0x0a60, US_REG_DWORD },
	{ "imc1_c1_ddrt_fnv0_event1",              					2,		12,		5,		0x0a64, US_REG_DWORD },
	{ "imc1_c1_ddrt_fnv1_event0",              					2,		12,		5,		0x0a70, US_REG_DWORD },
	{ "imc1_c1_ddrt_fnv1_event1",              					2,		12,		5,		0x0a74, US_REG_DWORD },
	{ "imc1_c1_ddrt_retry_status",             					2,		12,		5,		0x0a98, US_REG_DWORD },
	{ "imc1_c1_ddrt_dimm_info_dimm0",          					2,		12,		5,		0x0aa0, US_REG_DWORD },
	{ "imc1_c1_ddrt_dimm_info_dimm1",          					2,		12,		5,		0x0aa4, US_REG_DWORD },
	{ "imc1_c1_ddrt_error",                    					2,		12,		5,		0x0d24, US_REG_DWORD },
	{ "imc1_c1_ddrt_err_log_1st",              					2,		12,		5,		0x0d80, US_REG_DWORD },
	{ "imc1_c1_ddrt_err_log_next",             					2,		12,		5,		0x0d84, US_REG_DWORD },
	{ "imc1_c1_correrrcnt_0",                  					2,		12,		7,		0x0104, US_REG_DWORD },
	{ "imc1_c1_correrrcnt_1",                  					2,		12,		7,		0x0108, US_REG_DWORD },
	{ "imc1_c1_correrrcnt_2",                  					2,		12,		7,		0x010c, US_REG_DWORD },
	{ "imc1_c1_correrrcnt_3",                  					2,		12,		7,		0x0110, US_REG_DWORD },
	{ "imc1_c1_retry_rd_err_log_address2",     					2,		12,		7,		0x0114, US_REG_DWORD },
	{ "imc1_c1_detection_debug_log_address2",  					2,		12,		7,		0x0118, US_REG_DWORD },
	{ "imc1_c1_correrrthrshld_0",              					2,		12,		7,		0x011c, US_REG_DWORD },
	{ "imc1_c1_correrrthrshld_1",              					2,		12,		7,		0x0120, US_REG_DWORD },
	{ "imc1_c1_correrrthrshld_2",              					2,		12,		7,		0x0124, US_REG_DWORD },
	{ "imc1_c1_correrrthrshld_3",              					2,		12,		7,		0x0128, US_REG_DWORD },
	{ "imc1_c1_detection_debug_log_address1",  					2,		12,		7,		0x012c, US_REG_DWORD },
	{ "imc1_c1_correrrorstatus",               					2,		12,		7,		0x0134, US_REG_DWORD },
	{ "imc1_c1_devtag_cntl_0",                 					2,		12,		7,		0x0140,  US_REG_BYTE },
	{ "imc1_c1_devtag_cntl_1",                 					2,		12,		7,		0x0141,  US_REG_BYTE },
	{ "imc1_c1_devtag_cntl_2",                 					2,		12,		7,		0x0142,  US_REG_BYTE },
	{ "imc1_c1_devtag_cntl_3",                 					2,		12,		7,		0x0143,  US_REG_BYTE },
	{ "imc1_c1_devtag_cntl_4",                 					2,		12,		7,		0x0144,  US_REG_BYTE },
	{ "imc1_c1_devtag_cntl_5",                 					2,		12,		7,		0x0145,  US_REG_BYTE },
	{ "imc1_c1_devtag_cntl_6",                 					2,		12,		7,		0x0146,  US_REG_BYTE },
	{ "imc1_c1_devtag_cntl_7",                 					2,		12,		7,		0x0147,  US_REG_BYTE },
	{ "imc1_c1_retry_rd_err_log_misc",         					2,		12,		7,		0x0148, US_REG_DWORD },
	{ "imc1_c1_detection_debug_log",           					2,		12,		7,		0x014c, US_REG_DWORD },
	{ "imc1_c1_retry_rd_err_log_parity",       					2,		12,		7,		0x0150, US_REG_DWORD },
	{ "imc1_c1_retry_rd_err_log",              					2,		12,		7,		0x0154, US_REG_DWORD },
	{ "imc1_c1_retry_rd_err_log_address1",     					2,		12,		7,		0x015c, US_REG_DWORD },
	{ "imc1_c1_detection_debug_log_parity",    					2,		12,		7,		0x016c, US_REG_DWORD },
	{ "imc1_c1_detection_debug_log_locator",   					2,		12,		7,		0x019c, US_REG_DWORD },
	{ "imc1_c1_link_error",                    					2,		12,		7,		0x0308, US_REG_DWORD },
	{ "imc1_c1_link_retry_sb_err_count",       					2,		12,		7,		0x0400, US_REG_DWORD },
	{ "imc1_c1_link_retry_err_limits",         					2,		12,		7,		0x040c, US_REG_DWORD },
	{ "imc1_c1_link_err_fsm_state",            					2,		12,		7,		0x0420, US_REG_DWORD },
	{ "imc1_c2_dimmmtr_0",                     					2,		13,		0,		0x0080, US_REG_DWORD },
	{ "imc1_c2_dimmmtr_1",                     					2,		13,		0,		0x0084, US_REG_DWORD },
	{ "imc1_c2_dimmmtr_2",                     					2,		13,		0,		0x0088, US_REG_DWORD },
	{ "imc1_c2_ddrt_retry_fsm_state",          					2,		13,		1,		0x0904, US_REG_DWORD },
	{ "imc1_c2_ddrt_fnv0_event0",              					2,		13,		1,		0x0a60, US_REG_DWORD },
	{ "imc1_c2_ddrt_fnv0_event1",              					2,		13,		1,		0x0a64, US_REG_DWORD },
	{ "imc1_c2_ddrt_fnv1_event0",              					2,		13,		1,		0x0a70, US_REG_DWORD },
	{ "imc1_c2_ddrt_fnv1_event1",              					2,		13,		1,		0x0a74, US_REG_DWORD },
	{ "imc1_c2_ddrt_retry_status",             					2,		13,		1,		0x0a98, US_REG_DWORD },
	{ "imc1_c2_ddrt_dimm_info_dimm0",          					2,		13,		1,		0x0aa0, US_REG_DWORD },
	{ "imc1_c2_ddrt_dimm_info_dimm1",          					2,		13,		1,		0x0aa4, US_REG_DWORD },
	{ "imc1_c2_ddrt_error",                    					2,		13,		1,		0x0d24, US_REG_DWORD },
	{ "imc1_c2_ddrt_err_log_1st",              					2,		13,		1,		0x0d80, US_REG_DWORD },
	{ "imc1_c2_ddrt_err_log_next",             					2,		13,		1,		0x0d84, US_REG_DWORD },
	{ "imc1_c2_correrrcnt_0",                  					2,		13,		3,		0x0104, US_REG_DWORD },
	{ "imc1_c2_correrrcnt_1",                  					2,		13,		3,		0x0108, US_REG_DWORD },
	{ "imc1_c2_correrrcnt_2",                  					2,		13,		3,		0x010c, US_REG_DWORD },
	{ "imc1_c2_correrrcnt_3",                  					2,		13,		3,		0x0110, US_REG_DWORD },
	{ "imc1_c2_retry_rd_err_log_address2",     					2,		13,		3,		0x0114, US_REG_DWORD },
	{ "imc1_c2_detection_debug_log_address2",  					2,		13,		3,		0x0118, US_REG_DWORD },
	{ "imc1_c2_correrrthrshld_0",              					2,		13,		3,		0x011c, US_REG_DWORD },
	{ "imc1_c2_correrrthrshld_1",              					2,		13,		3,		0x0120, US_REG_DWORD },
	{ "imc1_c2_correrrthrshld_2",              					2,		13,		3,		0x0124, US_REG_DWORD },
	{ "imc1_c2_correrrthrshld_3",              					2,		13,		3,		0x0128, US_REG_DWORD },
	{ "imc1_c2_detection_debug_log_address1",  					2,		13,		3,		0x012c, US_REG_DWORD },
	{ "imc1_c2_correrrorstatus",               					2,		13,		3,		0x0134, US_REG_DWORD },
	{ "imc1_c2_devtag_cntl_0",                 					2,		13,		3,		0x0140,  US_REG_BYTE },
	{ "imc1_c2_devtag_cntl_1",                 					2,		13,		3,		0x0141,  US_REG_BYTE },
	{ "imc1_c2_devtag_cntl_2",                 					2,		13,		3,		0x0142,  US_REG_BYTE },
	{ "imc1_c2_devtag_cntl_3",                 					2,		13,		3,		0x0143,  US_REG_BYTE },
	{ "imc1_c2_devtag_cntl_4",                 					2,		13,		3,		0x0144,  US_REG_BYTE },
	{ "imc1_c2_devtag_cntl_5",                 					2,		13,		3,		0x0145,  US_REG_BYTE },
	{ "imc1_c2_devtag_cntl_6",                 					2,		13,		3,		0x0146,  US_REG_BYTE },
	{ "imc1_c2_devtag_cntl_7",                 					2,		13,		3,		0x0147,  US_REG_BYTE },
	{ "imc1_c2_retry_rd_err_log_misc",         					2,		13,		3,		0x0148, US_REG_DWORD },
	{ "imc1_c2_detection_debug_log",           					2,		13,		3,		0x014c, US_REG_DWORD },
	{ "imc1_c2_retry_rd_err_log_parity",       					2,		13,		3,		0x0150, US_REG_DWORD },
	{ "imc1_c2_retry_rd_err_log",              					2,		13,		3,		0x0154, US_REG_DWORD },
	{ "imc1_c2_retry_rd_err_log_address1",     					2,		13,		3,		0x015c, US_REG_DWORD },
	{ "imc1_c2_detection_debug_log_parity",    					2,		13,		3,		0x016c, US_REG_DWORD },
	{ "imc1_c2_detection_debug_log_locator",   					2,		13,		3,		0x019c, US_REG_DWORD },
	{ "imc1_c2_link_error",                    					2,		13,		3,		0x0308, US_REG_DWORD },
	{ "imc1_c2_link_retry_sb_err_count",       					2,		13,		3,		0x0400, US_REG_DWORD },
	{ "imc1_c2_link_retry_err_limits",         					2,		13,		3,		0x040c, US_REG_DWORD },
	{ "imc1_c2_link_err_fsm_state",            					2,		13,		3,		0x0420, US_REG_DWORD },
	{ "ntb_b3d00f0_lnksts",                    					3,		0,		0,		0x01a2,  US_REG_WORD },
	{ "ntb_b3d00f0_sltsts",                    					3,		0,		0,		0x01aa,  US_REG_WORD },
	{ "ntb_b3d00f0_lnksts2",                   					3,		0,		0,		0x01c2,  US_REG_WORD },
	{ "pxp_b3d00f0_pbus",                      					3,		0,		0,		0x0018,  US_REG_BYTE },
	{ "pxp_b3d00f0_secbus",                    					3,		0,		0,		0x0019,  US_REG_BYTE },
	{ "pxp_b3d00f0_subbus",                    					3,		0,		0,		0x001a,  US_REG_BYTE },
	{ "pxp_b3d00f0_iobas",                     					3,		0,		0,		0x001c,  US_REG_BYTE },
	{ "pxp_b3d00f0_iolim",                     					3,		0,		0,		0x001d,  US_REG_BYTE },
	{ "pxp_b3d00f0_secsts",                    					3,		0,		0,		0x001e,  US_REG_WORD },
	{ "pxp_b3d00f0_mbas",                      					3,		0,		0,		0x0020,  US_REG_WORD },
	{ "pxp_b3d00f0_mlim",                      					3,		0,		0,		0x0022,  US_REG_WORD },
	{ "pxp_b3d00f0_pbas",                      					3,		0,		0,		0x0024,  US_REG_WORD },
	{ "pxp_b3d00f0_plim",                      					3,		0,		0,		0x0026,  US_REG_WORD },
	{ "pxp_b3d00f0_pbasu",                     					3,		0,		0,		0x0028, US_REG_DWORD },
	{ "pxp_b3d00f0_plimu",                     					3,		0,		0,		0x002c, US_REG_DWORD },
	{ "pxp_b3d00f0_devsts",                    					3,		0,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b3d00f0_lnksts",                    					3,		0,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b3d00f0_sltsts",                    					3,		0,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_b3d00f0_lnksts2",                   					3,		0,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_b3d00f0_uncerrsts",                 					3,		0,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b3d00f0_corerrsts",                 					3,		0,		0,		0x0158, US_REG_DWORD },
	{ "pxp_b3d00f0_rperrsts",                  					3,		0,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b3d00f0_errsid",                    					3,		0,		0,		0x017c, US_REG_DWORD },
	{ "pxp_b3d00f0_miscctrlsts_0",             					3,		0,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b3d00f0_miscctrlsts_1",             					3,		0,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b3d00f0_xpcorerrsts",               					3,		0,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b3d00f0_xpcorerrmsk",               					3,		0,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b3d00f0_xpuncerrsts",               					3,		0,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b3d00f0_xpuncerrmsk",               					3,		0,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b3d00f0_xpuncerrsev",               					3,		0,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b3d00f0_xpuncerrptr",               					3,		0,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b3d00f0_uncedmask",                 					3,		0,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b3d00f0_coredmask",                 					3,		0,		0,		0x021c, US_REG_DWORD },
	{ "pxp_b3d00f0_rpedmask",                  					3,		0,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b3d00f0_xpuncedmask",               					3,		0,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b3d00f0_xpcoredmask",               					3,		0,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b3d00f0_xpglberrsts",               					3,		0,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b3d00f0_xpglberrptr",               					3,		0,		0,		0x0232,  US_REG_WORD },
	{ "pxp_b3d00f0_lnerrsts",                  					3,		0,		0,		0x0258, US_REG_DWORD },
	{ "pxp_b3d00f0_ler_ctrlsts",               					3,		0,		0,		0x0288, US_REG_DWORD },
	{ "pxp_b3d00f0_rppioerr_cap",              					3,		0,		0,		0x0298, US_REG_DWORD },
	{ "pxp_b3d00f0_rppioerr_status",           					3,		0,		0,		0x02a4, US_REG_DWORD },
	{ "pxp_b3d01f0_pbus",                      					3,		1,		0,		0x0018,  US_REG_BYTE },
	{ "pxp_b3d01f0_secbus",                    					3,		1,		0,		0x0019,  US_REG_BYTE },
	{ "pxp_b3d01f0_subbus",                    					3,		1,		0,		0x001a,  US_REG_BYTE },
	{ "pxp_b3d01f0_iobas",                     					3,		1,		0,		0x001c,  US_REG_BYTE },
	{ "pxp_b3d01f0_iolim",                     					3,		1,		0,		0x001d,  US_REG_BYTE },
	{ "pxp_b3d01f0_secsts",                    					3,		1,		0,		0x001e,  US_REG_WORD },
	{ "pxp_b3d01f0_mbas",                      					3,		1,		0,		0x0020,  US_REG_WORD },
	{ "pxp_b3d01f0_mlim",                      					3,		1,		0,		0x0022,  US_REG_WORD },
	{ "pxp_b3d01f0_pbas",                      					3,		1,		0,		0x0024,  US_REG_WORD },
	{ "pxp_b3d01f0_plim",                      					3,		1,		0,		0x0026,  US_REG_WORD },
	{ "pxp_b3d01f0_pbasu",                     					3,		1,		0,		0x0028, US_REG_DWORD },
	{ "pxp_b3d01f0_plimu",                     					3,		1,		0,		0x002c, US_REG_DWORD },
	{ "pxp_b3d01f0_devsts",                    					3,		1,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b3d01f0_lnksts",                    					3,		1,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b3d01f0_sltsts",                    					3,		1,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_b3d01f0_lnksts2",                   					3,		1,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_b3d01f0_uncerrsts",                 					3,		1,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b3d01f0_corerrsts",                 					3,		1,		0,		0x0158, US_REG_DWORD },
	{ "pxp_b3d01f0_rperrsts",                  					3,		1,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b3d01f0_errsid",                    					3,		1,		0,		0x017c, US_REG_DWORD },
	{ "pxp_b3d01f0_miscctrlsts_0",             					3,		1,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b3d01f0_miscctrlsts_1",             					3,		1,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b3d01f0_xpcorerrsts",               					3,		1,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b3d01f0_xpcorerrmsk",               					3,		1,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b3d01f0_xpuncerrsts",               					3,		1,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b3d01f0_xpuncerrmsk",               					3,		1,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b3d01f0_xpuncerrsev",               					3,		1,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b3d01f0_xpuncerrptr",               					3,		1,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b3d01f0_uncedmask",                 					3,		1,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b3d01f0_coredmask",                 					3,		1,		0,		0x021c, US_REG_DWORD },
	{ "pxp_b3d01f0_rpedmask",                  					3,		1,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b3d01f0_xpuncedmask",               					3,		1,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b3d01f0_xpcoredmask",               					3,		1,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b3d01f0_xpglberrsts",               					3,		1,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b3d01f0_xpglberrptr",               					3,		1,		0,		0x0232,  US_REG_WORD },
	{ "pxp_b3d01f0_lnerrsts",                  					3,		1,		0,		0x0258, US_REG_DWORD },
	{ "pxp_b3d01f0_ler_ctrlsts",               					3,		1,		0,		0x0288, US_REG_DWORD },
	{ "pxp_b3d01f0_rppioerr_cap",              					3,		1,		0,		0x0298, US_REG_DWORD },
	{ "pxp_b3d01f0_rppioerr_status",           					3,		1,		0,		0x02a4, US_REG_DWORD },
	{ "pxp_b3d02f0_pbus",                      					3,		2,		0,		0x0018,  US_REG_BYTE },
	{ "pxp_b3d02f0_secbus",                    					3,		2,		0,		0x0019,  US_REG_BYTE },
	{ "pxp_b3d02f0_subbus",                    					3,		2,		0,		0x001a,  US_REG_BYTE },
	{ "pxp_b3d02f0_iobas",                     					3,		2,		0,		0x001c,  US_REG_BYTE },
	{ "pxp_b3d02f0_iolim",                     					3,		2,		0,		0x001d,  US_REG_BYTE },
	{ "pxp_b3d02f0_secsts",                    					3,		2,		0,		0x001e,  US_REG_WORD },
	{ "pxp_b3d02f0_mbas",                      					3,		2,		0,		0x0020,  US_REG_WORD },
	{ "pxp_b3d02f0_mlim",                      					3,		2,		0,		0x0022,  US_REG_WORD },
	{ "pxp_b3d02f0_pbas",                      					3,		2,		0,		0x0024,  US_REG_WORD },
	{ "pxp_b3d02f0_plim",                      					3,		2,		0,		0x0026,  US_REG_WORD },
	{ "pxp_b3d02f0_pbasu",                     					3,		2,		0,		0x0028, US_REG_DWORD },
	{ "pxp_b3d02f0_plimu",                     					3,		2,		0,		0x002c, US_REG_DWORD },
	{ "pxp_b3d02f0_devsts",                    					3,		2,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b3d02f0_lnksts",                    					3,		2,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b3d02f0_sltsts",                    					3,		2,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_b3d02f0_lnksts2",                   					3,		2,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_b3d02f0_uncerrsts",                 					3,		2,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b3d02f0_corerrsts",                 					3,		2,		0,		0x0158, US_REG_DWORD },
	{ "pxp_b3d02f0_rperrsts",                  					3,		2,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b3d02f0_errsid",                    					3,		2,		0,		0x017c, US_REG_DWORD },
	{ "pxp_b3d02f0_miscctrlsts_0",             					3,		2,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b3d02f0_miscctrlsts_1",             					3,		2,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b3d02f0_xpcorerrsts",               					3,		2,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b3d02f0_xpcorerrmsk",               					3,		2,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b3d02f0_xpuncerrsts",               					3,		2,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b3d02f0_xpuncerrmsk",               					3,		2,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b3d02f0_xpuncerrsev",               					3,		2,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b3d02f0_xpuncerrptr",               					3,		2,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b3d02f0_uncedmask",                 					3,		2,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b3d02f0_coredmask",                 					3,		2,		0,		0x021c, US_REG_DWORD },
	{ "pxp_b3d02f0_rpedmask",                  					3,		2,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b3d02f0_xpuncedmask",               					3,		2,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b3d02f0_xpcoredmask",               					3,		2,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b3d02f0_xpglberrsts",               					3,		2,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b3d02f0_xpglberrptr",               					3,		2,		0,		0x0232,  US_REG_WORD },
	{ "pxp_b3d02f0_lnerrsts",                  					3,		2,		0,		0x0258, US_REG_DWORD },
	{ "pxp_b3d02f0_ler_ctrlsts",               					3,		2,		0,		0x0288, US_REG_DWORD },
	{ "pxp_b3d02f0_rppioerr_cap",              					3,		2,		0,		0x0298, US_REG_DWORD },
	{ "pxp_b3d02f0_rppioerr_status",           					3,		2,		0,		0x02a4, US_REG_DWORD },
	{ "pxp_b3d03f0_pbus",                      					3,		3,		0,		0x0018,  US_REG_BYTE },
	{ "pxp_b3d03f0_secbus",                    					3,		3,		0,		0x0019,  US_REG_BYTE },
	{ "pxp_b3d03f0_subbus",                    					3,		3,		0,		0x001a,  US_REG_BYTE },
	{ "pxp_b3d03f0_iobas",                     					3,		3,		0,		0x001c,  US_REG_BYTE },
	{ "pxp_b3d03f0_iolim",                     					3,		3,		0,		0x001d,  US_REG_BYTE },
	{ "pxp_b3d03f0_secsts",                    					3,		3,		0,		0x001e,  US_REG_WORD },
	{ "pxp_b3d03f0_mbas",                      					3,		3,		0,		0x0020,  US_REG_WORD },
	{ "pxp_b3d03f0_mlim",                      					3,		3,		0,		0x0022,  US_REG_WORD },
	{ "pxp_b3d03f0_pbas",                      					3,		3,		0,		0x0024,  US_REG_WORD },
	{ "pxp_b3d03f0_plim",                      					3,		3,		0,		0x0026,  US_REG_WORD },
	{ "pxp_b3d03f0_pbasu",                     					3,		3,		0,		0x0028, US_REG_DWORD },
	{ "pxp_b3d03f0_plimu",                     					3,		3,		0,		0x002c, US_REG_DWORD },
	{ "pxp_b3d03f0_devsts",                    					3,		3,		0,		0x009a,  US_REG_WORD },
	{ "pxp_b3d03f0_lnksts",                    					3,		3,		0,		0x00a2,  US_REG_WORD },
	{ "pxp_b3d03f0_sltsts",                    					3,		3,		0,		0x00aa,  US_REG_WORD },
	{ "pxp_b3d03f0_lnksts2",                   					3,		3,		0,		0x00c2,  US_REG_WORD },
	{ "pxp_b3d03f0_uncerrsts",                 					3,		3,		0,		0x014c, US_REG_DWORD },
	{ "pxp_b3d03f0_corerrsts",                 					3,		3,		0,		0x0158, US_REG_DWORD },
	{ "pxp_b3d03f0_rperrsts",                  					3,		3,		0,		0x0178, US_REG_DWORD },
	{ "pxp_b3d03f0_errsid",                    					3,		3,		0,		0x017c, US_REG_DWORD },
	{ "pxp_b3d03f0_miscctrlsts_0",             					3,		3,		0,		0x0188, US_REG_DWORD },
	{ "pxp_b3d03f0_miscctrlsts_1",             					3,		3,		0,		0x018c, US_REG_DWORD },
	{ "pxp_b3d03f0_xpcorerrsts",               					3,		3,		0,		0x0200, US_REG_DWORD },
	{ "pxp_b3d03f0_xpcorerrmsk",               					3,		3,		0,		0x0204, US_REG_DWORD },
	{ "pxp_b3d03f0_xpuncerrsts",               					3,		3,		0,		0x0208, US_REG_DWORD },
	{ "pxp_b3d03f0_xpuncerrmsk",               					3,		3,		0,		0x020c, US_REG_DWORD },
	{ "pxp_b3d03f0_xpuncerrsev",               					3,		3,		0,		0x0210, US_REG_DWORD },
	{ "pxp_b3d03f0_xpuncerrptr",               					3,		3,		0,		0x0214,  US_REG_BYTE },
	{ "pxp_b3d03f0_uncedmask",                 					3,		3,		0,		0x0218, US_REG_DWORD },
	{ "pxp_b3d03f0_coredmask",                 					3,		3,		0,		0x021c, US_REG_DWORD },
	{ "pxp_b3d03f0_rpedmask",                  					3,		3,		0,		0x0220, US_REG_DWORD },
	{ "pxp_b3d03f0_xpuncedmask",               					3,		3,		0,		0x0224, US_REG_DWORD },
	{ "pxp_b3d03f0_xpcoredmask",               					3,		3,		0,		0x0228, US_REG_DWORD },
	{ "pxp_b3d03f0_xpglberrsts",               					3,		3,		0,		0x0230,  US_REG_WORD },
	{ "pxp_b3d03f0_xpglberrptr",               					3,		3,		0,		0x0232,  US_REG_WORD },
	{ "pxp_b3d03f0_lnerrsts",                  					3,		3,		0,		0x0258, US_REG_DWORD },
	{ "pxp_b3d03f0_ler_ctrlsts",               					3,		3,		0,		0x0288, US_REG_DWORD },
	{ "pxp_b3d03f0_rppioerr_cap",              					3,		3,		0,		0x0298, US_REG_DWORD },
	{ "pxp_b3d03f0_rppioerr_status",           					3,		3,		0,		0x02a4, US_REG_DWORD },
	{ "iio_mmcfg_base_b3d05f0",                					3,		5,		0,		0x0090, US_REG_QWORD },
	{ "iio_mmcfg_limit_b3d05f0",               					3,		5,		0,		0x0098, US_REG_QWORD },
	{ "iio_tseg_b3d05f0",                      					3,		5,		0,		0x00a8, US_REG_QWORD },
	{ "iio_tolm_b3d05f0",                      					3,		5,		0,		0x00d0, US_REG_DWORD },
	{ "iio_tohm_0_b3d05f0",                    					3,		5,		0,		0x00d4, US_REG_DWORD },
	{ "iio_tohm_1_b3d05f0",                    					3,		5,		0,		0x00d8, US_REG_DWORD },
	{ "iio_ncmem_base_0_b3d05f0",              					3,		5,		0,		0x00e0, US_REG_DWORD },
	{ "iio_ncmem_base_1_b3d05f0",              					3,		5,		0,		0x00e4, US_REG_DWORD },
	{ "iio_ncmem_limit_0_b3d05f0",             					3,		5,		0,		0x00e8, US_REG_DWORD },
	{ "iio_ncmem_limit_1_b3d05f0",             					3,		5,		0,		0x00ec, US_REG_DWORD },
	{ "iio_mencmem_base_0_b3d05f0",            					3,		5,		0,		0x00f0, US_REG_DWORD },
	{ "iio_mencmem_base_1_b3d05f0",            					3,		5,		0,		0x00f4, US_REG_DWORD },
	{ "iio_mencmem_limit_0_b3d05f0",           					3,		5,		0,		0x00f8, US_REG_DWORD },
	{ "iio_mencmem_limit_1_b3d05f0",           					3,		5,		0,		0x00fc, US_REG_DWORD },
	{ "iio_vtuncerrsts_b3d05f0",               					3,		5,		0,		0x01a8, US_REG_DWORD },
	{ "iio_vtuncerrptr_b3d05f0",               					3,		5,		0,		0x01b4,  US_REG_BYTE },
	{ "iio_mmioh_non_interleave_b3d05f0",      					3,		5,		0,		0x0340, US_REG_QWORD },
	{ "iio_mmioh_interleave_b3d05f0",          					3,		5,		0,		0x0348, US_REG_QWORD },
	{ "iio_irpperrsv_b3d05f2",                 					3,		5,		2,		0x0080, US_REG_QWORD },
	{ "iio_pcierrsv_b3d05f2",                  					3,		5,		2,		0x0094, US_REG_DWORD },
	{ "iio_viral_b3d05f2",                     					3,		5,		2,		0x00a0, US_REG_DWORD },
	{ "iio_lnerrmask_b3d05f2",                 					3,		5,		2,		0x019c, US_REG_DWORD },
	{ "iio_lferrmask_b3d05f2",                 					3,		5,		2,		0x01a0, US_REG_DWORD },
	{ "iio_lcerrmask_b3d05f2",                 					3,		5,		2,		0x01a4, US_REG_DWORD },
	{ "iio_lcerrst_b3d05f2",                   					3,		5,		2,		0x01a8, US_REG_DWORD },
	{ "iio_lcferrst_b3d05f2",                  					3,		5,		2,		0x01ac, US_REG_DWORD },
	{ "iio_lcnerrst_b3d05f2",                  					3,		5,		2,		0x01b8, US_REG_DWORD },
	{ "iio_lnerrst_b3d05f2",                   					3,		5,		2,		0x01c0, US_REG_DWORD },
	{ "iio_lferrst_b3d05f2",                   					3,		5,		2,		0x01c4, US_REG_DWORD },
	{ "iio_lerrctl_b3d05f2",                   					3,		5,		2,		0x01c8, US_REG_DWORD },
	{ "iio_lsysst_b3d05f2",                    					3,		5,		2,		0x01cc, US_REG_DWORD },
	{ "iio_lsysctl_b3d05f2",                   					3,		5,		2,		0x01d0, US_REG_DWORD },
	{ "iio_sysmap_b3d05f2",                    					3,		5,		2,		0x01d4, US_REG_DWORD },
	{ "iio_lbitmap_b3d05f2",                   					3,		5,		2,		0x01d8, US_REG_DWORD },
	{ "iio_lfferrst_b3d05f2",                  					3,		5,		2,		0x01dc, US_REG_DWORD },
	{ "iio_lfnerrst_b3d05f2",                  					3,		5,		2,		0x01e8, US_REG_DWORD },
	{ "iio_lnferrst_b3d05f2",                  					3,		5,		2,		0x01ec, US_REG_DWORD },
	{ "iio_lnnerrst_b3d05f2",                  					3,		5,		2,		0x01f8, US_REG_DWORD },
	{ "iio_irpringerrsv_b3d05f2",              					3,		5,		2,		0x0200, US_REG_QWORD },
	{ "iio_irpringerrst_b3d05f2",              					3,		5,		2,		0x0208, US_REG_DWORD },
	{ "iio_irpringerrctl_b3d05f2",             					3,		5,		2,		0x020c, US_REG_DWORD },
	{ "iio_irpringfferrst_b3d05f2",            					3,		5,		2,		0x0210, US_REG_DWORD },
	{ "iio_irpringfnerrst_b3d05f2",            					3,		5,		2,		0x0214, US_REG_DWORD },
	{ "iio_irpringnferrst_b3d05f2",            					3,		5,		2,		0x0218, US_REG_DWORD },
	{ "iio_irpringnnerrst_b3d05f2",            					3,		5,		2,		0x021c, US_REG_DWORD },
	{ "iio_irpringerrcntsel_b3d05f2",          					3,		5,		2,		0x0220, US_REG_DWORD },
	{ "iio_irpringerrcnt_b3d05f2",             					3,		5,		2,		0x0224, US_REG_DWORD },
	{ "iio_irpringmisc_b3d05f2",               					3,		5,		2,		0x0228, US_REG_DWORD },
	{ "iio_irpp0errst_b3d05f2",                					3,		5,		2,		0x0230, US_REG_DWORD },
	{ "iio_irpp0errctl_b3d05f2",               					3,		5,		2,		0x0234, US_REG_DWORD },
	{ "iio_irpp0fferrst_b3d05f2",              					3,		5,		2,		0x0238, US_REG_DWORD },
	{ "iio_irpp0fnerrst_b3d05f2",              					3,		5,		2,		0x023c, US_REG_DWORD },
	{ "iio_irpp0fferrhd0_b3d05f2",             					3,		5,		2,		0x0240, US_REG_DWORD },
	{ "iio_irpp0fferrhd1_b3d05f2",             					3,		5,		2,		0x0244, US_REG_DWORD },
	{ "iio_irpp0fferrhd2_b3d05f2",             					3,		5,		2,		0x0248, US_REG_DWORD },
	{ "iio_irpp0fferrhd3_b3d05f2",             					3,		5,		2,		0x024c, US_REG_DWORD },
	{ "iio_irpp0nferrst_b3d05f2",              					3,		5,		2,		0x0250, US_REG_DWORD },
	{ "iio_irpp0nnerrst_b3d05f2",              					3,		5,		2,		0x0254, US_REG_DWORD },
	{ "iio_irpp0errcntsel_b3d05f2",            					3,		5,		2,		0x0268, US_REG_DWORD },
	{ "iio_irpp0errcnt_b3d05f2",               					3,		5,		2,		0x026c, US_REG_DWORD },
	{ "iio_irpp0plsr0_b3d05f2",                					3,		5,		2,		0x0270, US_REG_DWORD },
	{ "iio_irpp0plsr1_b3d05f2",                					3,		5,		2,		0x0274, US_REG_DWORD },
	{ "iio_irpp0plsr2_b3d05f2",                					3,		5,		2,		0x0278, US_REG_DWORD },
	{ "iio_tcerrst_b3d05f2",                   					3,		5,		2,		0x0288, US_REG_DWORD },
	{ "iio_tcerrctl_b3d05f2",                  					3,		5,		2,		0x028c, US_REG_DWORD },
	{ "iio_itcerrsev_b3d05f2",                 					3,		5,		2,		0x0290, US_REG_DWORD },
	{ "iio_otcerrsev_b3d05f2",                 					3,		5,		2,		0x0294, US_REG_DWORD },
	{ "iio_tcerrcntsel_b3d05f2",               					3,		5,		2,		0x0298, US_REG_DWORD },
	{ "iio_tcerrcnt_b3d05f2",                  					3,		5,		2,		0x029c, US_REG_DWORD },
	{ "iio_itcfferrst_b3d05f2",                					3,		5,		2,		0x02a0, US_REG_DWORD },
	{ "iio_itcfferrhd0_b3d05f2",               					3,		5,		2,		0x02a4, US_REG_DWORD },
	{ "iio_itcfferrhd1_b3d05f2",               					3,		5,		2,		0x02a8, US_REG_DWORD },
	{ "iio_itcfferrhd2_b3d05f2",               					3,		5,		2,		0x02ac, US_REG_DWORD },
	{ "iio_itcfferrhd3_b3d05f2",               					3,		5,		2,		0x02b0, US_REG_DWORD },
	{ "iio_itcfnerrst_b3d05f2",                					3,		5,		2,		0x02b4, US_REG_DWORD },
	{ "iio_itcnferrst_b3d05f2",                					3,		5,		2,		0x02b8, US_REG_DWORD },
	{ "iio_itcnnerrst_b3d05f2",                					3,		5,		2,		0x02cc, US_REG_DWORD },
	{ "iio_otcfferrst_b3d05f2",                					3,		5,		2,		0x02d0, US_REG_DWORD },
	{ "iio_otcfferrhd0_b3d05f2",               					3,		5,		2,		0x02d4, US_REG_DWORD },
	{ "iio_otcfferrhd1_b3d05f2",               					3,		5,		2,		0x02d8, US_REG_DWORD },
	{ "iio_otcfferrhd2_b3d05f2",               					3,		5,		2,		0x02dc, US_REG_DWORD },
	{ "iio_otcfferrhd3_b3d05f2",               					3,		5,		2,		0x02e0, US_REG_DWORD },
	{ "iio_otcfnerrst_b3d05f2",                					3,		5,		2,		0x02e4, US_REG_DWORD },
	{ "iio_otcnferrst_b3d05f2",                					3,		5,		2,		0x02e8, US_REG_DWORD },
	{ "iio_otcnferrhd0_b3d05f2",               					3,		5,		2,		0x02ec, US_REG_DWORD },
	{ "iio_otcnnerrst_b3d05f2",                					3,		5,		2,		0x02fc, US_REG_DWORD },
	{ "iio_iioerrctl_b3d05f2",                 					3,		5,		2,		0x0304, US_REG_DWORD },
	{ "pxp_b3d07f0_devsts",                    					3,		7,		0,		0x004a,  US_REG_WORD },
	{ "pxp_b3d07f0_lnksts",                    					3,		7,		0,		0x0052,  US_REG_WORD },
	{ "pxp_b3d07f0_lnksts2",                   					3,		7,		0,		0x0072,  US_REG_WORD },
	{ "pxp_b3d07f0_sltsts2",                   					3,		7,		0,		0x007a,  US_REG_WORD },
	{ "pxp_b3d07f1_devsts",                    					3,		7,		1,		0x004a,  US_REG_WORD },
	{ "pxp_b3d07f1_lnksts",                    					3,		7,		1,		0x0052,  US_REG_WORD },
	{ "pxp_b3d07f1_lnksts2",                   					3,		7,		1,		0x0072,  US_REG_WORD },
	{ "pxp_b3d07f1_sltsts2",                   					3,		7,		1,		0x007a,  US_REG_WORD },
	{ "pxp_b3d07f2_devsts",                    					3,		7,		2,		0x004a,  US_REG_WORD },
	{ "pxp_b3d07f2_lnksts",                    					3,		7,		2,		0x0052,  US_REG_WORD },
	{ "pxp_b3d07f2_lnksts2",                   					3,		7,		2,		0x0072,  US_REG_WORD },
	{ "pxp_b3d07f2_sltsts2",                   					3,		7,		2,		0x007a,  US_REG_WORD },
	{ "pxp_b3d07f3_devsts",                    					3,		7,		3,		0x004a,  US_REG_WORD },
	{ "pxp_b3d07f3_lnksts",                    					3,		7,		3,		0x0052,  US_REG_WORD },
	{ "pxp_b3d07f3_lnksts2",                   					3,		7,		3,		0x0072,  US_REG_WORD },
	{ "pxp_b3d07f3_sltsts2",                   					3,		7,		3,		0x007a,  US_REG_WORD },
	{ "pxp_b3d07f4_devsts",                    					3,		7,		4,		0x004a,  US_REG_WORD },
	{ "pxp_b3d07f4_lnksts",                    					3,		7,		4,		0x0052,  US_REG_WORD },
	{ "pxp_b3d07f4_lnksts2",                   					3,		7,		4,		0x0072,  US_REG_WORD },
	{ "pxp_b3d07f4_sltsts2",                   					3,		7,		4,		0x007a,  US_REG_WORD },
	{ "pxp_b3d07f7_devsts",                    					3,		7,		7,		0x004a,  US_REG_WORD },
	{ "pxp_b3d07f7_lnksts",                    					3,		7,		7,		0x0052,  US_REG_WORD },
	{ "pxp_b3d07f7_lnksts2",                   					3,		7,		7,		0x0072,  US_REG_WORD },
	{ "pxp_b3d07f7_sltsts2",                   					3,		7,		7,		0x007a,  US_REG_WORD },
	{ "pxp_b3d07f7_tswdbgerrstdis0",           					3,		7,		7,		0x0358, US_REG_DWORD },
	{ "kti0_lcl",                              					3,		14,		0,		0x0084, US_REG_DWORD },
	{ "kti0_ls",                               					3,		14,		0,		0x008c, US_REG_DWORD },
	{ "kti0_les",                              					3,		14,		0,		0x0090, US_REG_DWORD },
	{ "kti0_lp0",                              					3,		14,		0,		0x0094, US_REG_DWORD },
	{ "kti0_lp1",                              					3,		14,		0,		0x0098, US_REG_DWORD },
	{ "kti0_viral",                            					3,		14,		0,		0x009c, US_REG_DWORD },
	{ "kti0_csmithres",                        					3,		14,		0,		0x00a0, US_REG_DWORD },
	{ "kti0_cerrlogctrl",                      					3,		14,		0,		0x00a4, US_REG_DWORD },
	{ "kti0_bios_err_st",                      					3,		14,		0,		0x00a8, US_REG_QWORD },
	{ "kti0_bios_err_misc",                    					3,		14,		0,		0x00b0, US_REG_QWORD },
	{ "kti0_bios_err_ad",                      					3,		14,		0,		0x00b8, US_REG_QWORD },
	{ "kti0_crcerrcnt",                        					3,		14,		0,		0x00c0, US_REG_DWORD },
	{ "kti0_errdis0",                          					3,		14,		0,		0x00c4, US_REG_DWORD },
	{ "kti0_errdis1",                          					3,		14,		0,		0x00c8, US_REG_DWORD },
	{ "kti0_errcnt0_cntr",                     					3,		14,		0,		0x00d4, US_REG_DWORD },
	{ "kti0_errcnt1_cntr",                     					3,		14,		0,		0x00dc, US_REG_DWORD },
	{ "kti0_errcnt2_cntr",                     					3,		14,		0,		0x00e4, US_REG_DWORD },
	{ "kti0_reut_ph_ctr1",                     					3,		14,		1,		0x012c, US_REG_DWORD },
	{ "kti0_reut_ph_ctr2",                     					3,		14,		1,		0x0130, US_REG_DWORD },
	{ "kti0_reut_ph_tds",                      					3,		14,		1,		0x013c, US_REG_DWORD },
	{ "kti0_reut_ph_rds",                      					3,		14,		1,		0x0140, US_REG_DWORD },
	{ "kti0_reut_ph_pis",                      					3,		14,		1,		0x014c, US_REG_DWORD },
	{ "kti0_reut_ph_pss",                      					3,		14,		1,		0x0154, US_REG_DWORD },
	{ "kti0_reut_ph_cls",                      					3,		14,		1,		0x0160, US_REG_DWORD },
	{ "kti0_reut_ph_pls",                      					3,		14,		1,		0x0164, US_REG_DWORD },
	{ "kti0_reut_ph_wes",                      					3,		14,		1,		0x0168, US_REG_DWORD },
	{ "kti0_reut_ph_kes",                      					3,		14,		1,		0x016c, US_REG_DWORD },
	{ "kti0_reut_ph_das",                      					3,		14,		1,		0x0170, US_REG_DWORD },
	{ "kti0_reut_ph_ldc",                      					3,		14,		1,		0x0178, US_REG_DWORD },
	{ "kti0_reut_ph_ttlecr",                   					3,		14,		1,		0x01b4, US_REG_DWORD },
	{ "kti0_reut_ph_ttres",                    					3,		14,		1,		0x01c0, US_REG_DWORD },
	{ "kti0_dbgerrst0",                        					3,		14,		2,		0x03a0, US_REG_DWORD },
	{ "kti0_dbgerrstdis0",                     					3,		14,		2,		0x03a4, US_REG_DWORD },
	{ "kti0_dbgerrst1",                        					3,		14,		2,		0x03a8, US_REG_DWORD },
	{ "kti0_dbgerrstdis1",                     					3,		14,		2,		0x03ac, US_REG_DWORD },
	{ "kti0_dfx_lck_ctl_cfg",                  					3,		14,		2,		0x0800, US_REG_DWORD },
	{ "kti0_pq_csr_mcsr0",                     					3,		14,		2,		0x0820, US_REG_DWORD },
	{ "kti0_pq_csr_mcsr1",                     					3,		14,		2,		0x0824, US_REG_DWORD },
	{ "kti0_pq_csr_mcsr3",                     					3,		14,		2,		0x082c, US_REG_DWORD },
	{ "kti0_ph_rxl0c_err_sts",                 					3,		14,		2,		0x09f0, US_REG_DWORD },
	{ "kti0_ph_rxl0c_err_log0",                					3,		14,		2,		0x09f4, US_REG_DWORD },
	{ "kti0_ph_rxl0c_err_log1",                					3,		14,		2,		0x09f8, US_REG_DWORD },
	{ "kti1_lcl",                              					3,		15,		0,		0x0084, US_REG_DWORD },
	{ "kti1_ls",                               					3,		15,		0,		0x008c, US_REG_DWORD },
	{ "kti1_les",                              					3,		15,		0,		0x0090, US_REG_DWORD },
	{ "kti1_lp0",                              					3,		15,		0,		0x0094, US_REG_DWORD },
	{ "kti1_lp1",                              					3,		15,		0,		0x0098, US_REG_DWORD },
	{ "kti1_viral",                            					3,		15,		0,		0x009c, US_REG_DWORD },
	{ "kti1_csmithres",                        					3,		15,		0,		0x00a0, US_REG_DWORD },
	{ "kti1_cerrlogctrl",                      					3,		15,		0,		0x00a4, US_REG_DWORD },
	{ "kti1_bios_err_st",                      					3,		15,		0,		0x00a8, US_REG_QWORD },
	{ "kti1_bios_err_misc",                    					3,		15,		0,		0x00b0, US_REG_QWORD },
	{ "kti1_bios_err_ad",                      					3,		15,		0,		0x00b8, US_REG_QWORD },
	{ "kti1_crcerrcnt",                        					3,		15,		0,		0x00c0, US_REG_DWORD },
	{ "kti1_errdis0",                          					3,		15,		0,		0x00c4, US_REG_DWORD },
	{ "kti1_errdis1",                          					3,		15,		0,		0x00c8, US_REG_DWORD },
	{ "kti1_errcnt0_cntr",                     					3,		15,		0,		0x00d4, US_REG_DWORD },
	{ "kti1_errcnt1_cntr",                     					3,		15,		0,		0x00dc, US_REG_DWORD },
	{ "kti1_errcnt2_cntr",                     					3,		15,		0,		0x00e4, US_REG_DWORD },
	{ "kti1_reut_ph_ctr1",                     					3,		15,		1,		0x012c, US_REG_DWORD },
	{ "kti1_reut_ph_ctr2",                     					3,		15,		1,		0x0130, US_REG_DWORD },
	{ "kti1_reut_ph_tds",                      					3,		15,		1,		0x013c, US_REG_DWORD },
	{ "kti1_reut_ph_rds",                      					3,		15,		1,		0x0140, US_REG_DWORD },
	{ "kti1_reut_ph_pis",                      					3,		15,		1,		0x014c, US_REG_DWORD },
	{ "kti1_reut_ph_pss",                      					3,		15,		1,		0x0154, US_REG_DWORD },
	{ "kti1_reut_ph_cls",                      					3,		15,		1,		0x0160, US_REG_DWORD },
	{ "kti1_reut_ph_pls",                      					3,		15,		1,		0x0164, US_REG_DWORD },
	{ "kti1_reut_ph_wes",                      					3,		15,		1,		0x0168, US_REG_DWORD },
	{ "kti1_reut_ph_kes",                      					3,		15,		1,		0x016c, US_REG_DWORD },
	{ "kti1_reut_ph_das",                      					3,		15,		1,		0x0170, US_REG_DWORD },
	{ "kti1_reut_ph_ldc",                      					3,		15,		1,		0x0178, US_REG_DWORD },
	{ "kti1_reut_ph_ttlecr",                   					3,		15,		1,		0x01b4, US_REG_DWORD },
	{ "kti1_reut_ph_ttres",                    					3,		15,		1,		0x01c0, US_REG_DWORD },
	{ "kti1_dbgerrst0",                        					3,		15,		2,		0x03a0, US_REG_DWORD },
	{ "kti1_dbgerrstdis0",                     					3,		15,		2,		0x03a4, US_REG_DWORD },
	{ "kti1_dbgerrst1",                        					3,		15,		2,		0x03a8, US_REG_DWORD },
	{ "kti1_dbgerrstdis1",                     					3,		15,		2,		0x03ac, US_REG_DWORD },
	{ "kti1_dfx_lck_ctl_cfg",                  					3,		15,		2,		0x0800, US_REG_DWORD },
	{ "kti1_pq_csr_mcsr0",                     					3,		15,		2,		0x0820, US_REG_DWORD },
	{ "kti1_pq_csr_mcsr1",                     					3,		15,		2,		0x0824, US_REG_DWORD },
	{ "kti1_pq_csr_mcsr3",                     					3,		15,		2,		0x082c, US_REG_DWORD },
	{ "kti1_ph_rxl0c_err_sts",                 					3,		15,		2,		0x09f0, US_REG_DWORD },
	{ "kti1_ph_rxl0c_err_log0",                					3,		15,		2,		0x09f4, US_REG_DWORD },
	{ "kti1_ph_rxl0c_err_log1",                					3,		15,		2,		0x09f8, US_REG_DWORD },
	{ "kti2_lcl",                              					3,		16,		0,		0x0084, US_REG_DWORD },
	{ "kti2_ls",                               					3,		16,		0,		0x008c, US_REG_DWORD },
	{ "kti2_les",                              					3,		16,		0,		0x0090, US_REG_DWORD },
	{ "kti2_lp0",                              					3,		16,		0,		0x0094, US_REG_DWORD },
	{ "kti2_lp1",                              					3,		16,		0,		0x0098, US_REG_DWORD },
	{ "kti2_viral",                            					3,		16,		0,		0x009c, US_REG_DWORD },
	{ "kti2_csmithres",                        					3,		16,		0,		0x00a0, US_REG_DWORD },
	{ "kti2_cerrlogctrl",                      					3,		16,		0,		0x00a4, US_REG_DWORD },
	{ "kti2_bios_err_st",                      					3,		16,		0,		0x00a8, US_REG_QWORD },
	{ "kti2_bios_err_misc",                    					3,		16,		0,		0x00b0, US_REG_QWORD },
	{ "kti2_bios_err_ad",                      					3,		16,		0,		0x00b8, US_REG_QWORD },
	{ "kti2_crcerrcnt",                        					3,		16,		0,		0x00c0, US_REG_DWORD },
	{ "kti2_errdis0",                          					3,		16,		0,		0x00c4, US_REG_DWORD },
	{ "kti2_errdis1",                          					3,		16,		0,		0x00c8, US_REG_DWORD },
	{ "kti2_errcnt0_cntr",                     					3,		16,		0,		0x00d4, US_REG_DWORD },
	{ "kti2_errcnt1_cntr",                     					3,		16,		0,		0x00dc, US_REG_DWORD },
	{ "kti2_errcnt2_cntr",                     					3,		16,		0,		0x00e4, US_REG_DWORD },
	{ "kti2_reut_ph_ctr1",                     					3,		16,		1,		0x012c, US_REG_DWORD },
	{ "kti2_reut_ph_ctr2",                     					3,		16,		1,		0x0130, US_REG_DWORD },
	{ "kti2_reut_ph_tds",                      					3,		16,		1,		0x013c, US_REG_DWORD },
	{ "kti2_reut_ph_rds",                      					3,		16,		1,		0x0140, US_REG_DWORD },
	{ "kti2_reut_ph_pis",                      					3,		16,		1,		0x014c, US_REG_DWORD },
	{ "kti2_reut_ph_pss",                      					3,		16,		1,		0x0154, US_REG_DWORD },
	{ "kti2_reut_ph_cls",                      					3,		16,		1,		0x0160, US_REG_DWORD },
	{ "kti2_reut_ph_pls",                      					3,		16,		1,		0x0164, US_REG_DWORD },
	{ "kti2_reut_ph_wes",                      					3,		16,		1,		0x0168, US_REG_DWORD },
	{ "kti2_reut_ph_kes",                      					3,		16,		1,		0x016c, US_REG_DWORD },
	{ "kti2_reut_ph_das",                      					3,		16,		1,		0x0170, US_REG_DWORD },
	{ "kti2_reut_ph_ldc",                      					3,		16,		1,		0x0178, US_REG_DWORD },
	{ "kti2_reut_ph_ttlecr",                   					3,		16,		1,		0x01b4, US_REG_DWORD },
	{ "kti2_reut_ph_ttres",                    					3,		16,		1,		0x01c0, US_REG_DWORD },
	{ "kti2_dbgerrst0",                        					3,		16,		2,		0x03a0, US_REG_DWORD },
	{ "kti2_dbgerrstdis0",                     					3,		16,		2,		0x03a4, US_REG_DWORD },
	{ "kti2_dbgerrst1",                        					3,		16,		2,		0x03a8, US_REG_DWORD },
	{ "kti2_dbgerrstdis1",                     					3,		16,		2,		0x03ac, US_REG_DWORD },
	{ "kti2_dfx_lck_ctl_cfg",                  					3,		16,		2,		0x0800, US_REG_DWORD },
	{ "kti2_pq_csr_mcsr0",                     					3,		16,		2,		0x0820, US_REG_DWORD },
	{ "kti2_pq_csr_mcsr1",                     					3,		16,		2,		0x0824, US_REG_DWORD },
	{ "kti2_pq_csr_mcsr3",                     					3,		16,		2,		0x082c, US_REG_DWORD },
	{ "kti2_ph_rxl0c_err_sts",                 					3,		16,		2,		0x09f0, US_REG_DWORD },
	{ "kti2_ph_rxl0c_err_log0",                					3,		16,		2,		0x09f4, US_REG_DWORD },
	{ "kti2_ph_rxl0c_err_log1",                					3,		16,		2,		0x09f8, US_REG_DWORD },
	{ "m3kti0_m3egrerrmsk0123",                					3,		18,		0,		0x0118, US_REG_DWORD },
	{ "m3kti0_m3egrerrmsk4567",                					3,		18,		0,		0x011c, US_REG_DWORD },
	{ "m3kti0_m3egrerrlog0",                   					3,		18,		0,		0x0120, US_REG_DWORD },
	{ "m3kti0_m3egrerrlog1",                   					3,		18,		0,		0x0124, US_REG_DWORD },
	{ "m3kti0_m3egrerrlog2",                   					3,		18,		0,		0x0128, US_REG_DWORD },
	{ "m3kti0_m3egrerrlog3",                   					3,		18,		0,		0x012c, US_REG_DWORD },
	{ "m3kti0_m3egrerrlog4",                   					3,		18,		0,		0x0130, US_REG_DWORD },
	{ "m3kti0_m3egrerrlog5",                   					3,		18,		0,		0x0134, US_REG_DWORD },
	{ "m3kti0_m3egrerrlog6",                   					3,		18,		0,		0x0138, US_REG_DWORD },
	{ "m3kti0_m3egrerrlog7",                   					3,		18,		0,		0x013c, US_REG_DWORD },
	{ "m3kti0_m3egrerrmsk8",                   					3,		18,		0,		0x0148, US_REG_DWORD },
	{ "m3kti0_m3ingerrmask0",                  					3,		18,		0,		0x0158, US_REG_DWORD },
	{ "m3kti0_m3ingerrmask1",                  					3,		18,		0,		0x015c, US_REG_DWORD },
	{ "m3kti0_m3ingerrlog0",                   					3,		18,		0,		0x0160, US_REG_DWORD },
	{ "m3kti0_m3ingerrlog1",                   					3,		18,		0,		0x0164, US_REG_DWORD },
	{ "m3kti0_m3ingerrlog2",                   					3,		18,		0,		0x0168, US_REG_DWORD },
	{ "m3kti0_m3ingerrlog3",                   					3,		18,		0,		0x016c, US_REG_DWORD },
	{ "m3kti1_m3egrerrmsk0123",                					3,		18,		4,		0x0118, US_REG_DWORD },
	{ "m3kti1_m3egrerrmsk4567",                					3,		18,		4,		0x011c, US_REG_DWORD },
	{ "m3kti1_m3egrerrlog0",                   					3,		18,		4,		0x0120, US_REG_DWORD },
	{ "m3kti1_m3egrerrlog1",                   					3,		18,		4,		0x0124, US_REG_DWORD },
	{ "m3kti1_m3egrerrlog2",                   					3,		18,		4,		0x0128, US_REG_DWORD },
	{ "m3kti1_m3egrerrlog3",                   					3,		18,		4,		0x012c, US_REG_DWORD },
	{ "m3kti1_m3egrerrlog4",                   					3,		18,		4,		0x0130, US_REG_DWORD },
	{ "m3kti1_m3egrerrlog5",                   					3,		18,		4,		0x0134, US_REG_DWORD },
	{ "m3kti1_m3egrerrlog6",                   					3,		18,		4,		0x0138, US_REG_DWORD },
	{ "m3kti1_m3egrerrlog7",                   					3,		18,		4,		0x013c, US_REG_DWORD },
	{ "m3kti1_m3egrerrmsk8",                   					3,		18,		4,		0x0148, US_REG_DWORD },
	{ "m3kti1_m3ingerrmask0",                  					3,		18,		4,		0x0158, US_REG_DWORD },
	{ "m3kti1_m3ingerrmask1",                  					3,		18,		4,		0x015c, US_REG_DWORD },
	{ "m3kti1_m3ingerrlog0",                   					3,		18,		4,		0x0160, US_REG_DWORD },
	{ "m3kti1_m3ingerrlog1",                   					3,		18,		4,		0x0164, US_REG_DWORD },
	{ "m3kti1_m3ingerrlog2",                   					3,		18,		4,		0x0168, US_REG_DWORD },
	{ "m3kti1_m3ingerrlog3",                   					3,		18,		4,		0x016c, US_REG_DWORD },
	{ "m2u_pcie0_rpegrctrlconfig0_r2egrisoerrlog0",				3,		21,		0,		0x0060, US_REG_DWORD },
	{ "m2u_pcie0_rpegrctrlconfig0_r2egrisoerrlog1",				3,		21,		0,		0x0064, US_REG_DWORD },
	{ "m2u_pcie0_rpegrctrlconfig0_r2egrprqerrlog0",				3,		21,		0,		0x0068, US_REG_DWORD },
	{ "m2u_pcie0_rpegrctrlconfig0_r2egrprqerrlog1",				3,		21,		0,		0x006c, US_REG_DWORD },
	{ "m2u_pcie0_rpegrctrlconfig0_r2egrerrlog",					3,		21,		0,		0x00b0, US_REG_DWORD },
	{ "m2d_pcie1_rpegrctrlconfig1_r2egrisoerrlog0",				3,		22,		0,		0x0060, US_REG_DWORD },
	{ "m2d_pcie1_rpegrctrlconfig1_r2egrisoerrlog1",				3,		22,		0,		0x0064, US_REG_DWORD },
	{ "m2d_pcie1_rpegrctrlconfig1_r2egrprqerrlog0",				3,		22,		0,		0x0068, US_REG_DWORD },
	{ "m2d_pcie1_rpegrctrlconfig1_r2egrprqerrlog1",				3,		22,		0,		0x006c, US_REG_DWORD },
	{ "m2d_pcie1_rpegrctrlconfig1_r2egrerrlog",					3,		22,		0,		0x00b0, US_REG_DWORD },
	{ "m2_mpc1_rpegrctrlconfig4_r2egrisoerrlog0",				3,		22,		4,		0x0060, US_REG_DWORD },
	{ "m2_mpc1_rpegrctrlconfig4_r2egrisoerrlog1",				3,		22,		4,		0x0064, US_REG_DWORD },
	{ "m2_mpc1_rpegrctrlconfig4_r2egrprqerrlog0",				3,		22,		4,		0x0068, US_REG_DWORD },
	{ "m2_mpc1_rpegrctrlconfig4_r2egrprqerrlog1",				3,		22,		4,		0x006c, US_REG_DWORD },
	{ "m2_mpc1_rputlctrlconfig4_r2glerrcfg",   					3,		22,		4,		0x00a8, US_REG_DWORD },
	{ "m2_mpc1_rpegrctrlconfig4_r2egrerrlog",  					3,		22,		4,		0x00b0, US_REG_DWORD },
	{ "m2u_pcie1_rpegrctrlconfig2_r2egrisoerrlog0",				3,		23,		0,		0x0060, US_REG_DWORD },
	{ "m2u_pcie1_rpegrctrlconfig2_r2egrisoerrlog1",				3,		23,		0,		0x0064, US_REG_DWORD },
	{ "m2u_pcie1_rpegrctrlconfig2_r2egrprqerrlog0",				3,		23,		0,		0x0068, US_REG_DWORD },
	{ "m2u_pcie1_rpegrctrlconfig2_r2egrprqerrlog1",				3,		23,		0,		0x006c, US_REG_DWORD },
	{ "m2u_pcie1_rpegrctrlconfig2_r2egrerrlog",					3,		23,		0,		0x00b0, US_REG_DWORD },
};

static const SUncoreStatusRegPciMmio sUncoreStatusPciMmio[] =
{
    // Register,                            Bar,        Bus,        Dev,        Func,       Offset,         SizeCode
    { "cb_bar0_chanerr",                {{  .bar = 0,   .bus = 0,   .dev = 4,   .func = 0,  .reg = 0x00a8,  .lenCode = US_MMIO_DWORD }}},
    { "cb_bar1_chanerr",                {{  .bar = 0,   .bus = 0,   .dev = 4,   .func = 1,  .reg = 0x00a8,  .lenCode = US_MMIO_DWORD }}},
    { "cb_bar2_chanerr",                {{  .bar = 0,   .bus = 0,   .dev = 4,   .func = 2,  .reg = 0x00a8,  .lenCode = US_MMIO_DWORD }}},
    { "cb_bar3_chanerr",                {{  .bar = 0,   .bus = 0,   .dev = 4,   .func = 3,  .reg = 0x00a8,  .lenCode = US_MMIO_DWORD }}},
    { "cb_bar4_chanerr",                {{  .bar = 0,   .bus = 0,   .dev = 4,   .func = 4,  .reg = 0x00a8,  .lenCode = US_MMIO_DWORD }}},
    { "cb_bar5_chanerr",                {{  .bar = 0,   .bus = 0,   .dev = 4,   .func = 5,  .reg = 0x00a8,  .lenCode = US_MMIO_DWORD }}},
    { "cb_bar6_chanerr",                {{  .bar = 0,   .bus = 0,   .dev = 4,   .func = 6,  .reg = 0x00a8,  .lenCode = US_MMIO_DWORD }}},
    { "cb_bar7_chanerr",                {{  .bar = 0,   .bus = 0,   .dev = 4,   .func = 7,  .reg = 0x00a8,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd0_fltsts_b0d05f0",        {{  .bar = 0,   .bus = 0,   .dev = 5,   .func = 0,  .reg = 0x0034,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd0_fltsts_b1d05f0",        {{  .bar = 0,   .bus = 1,   .dev = 5,   .func = 0,  .reg = 0x0034,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd0_fltsts_b2d05f0",        {{  .bar = 0,   .bus = 2,   .dev = 5,   .func = 0,  .reg = 0x0034,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd0_fltsts_b3d05f0",        {{  .bar = 0,   .bus = 3,   .dev = 5,   .func = 0,  .reg = 0x0034,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd0_glbsts_b0d05f0",        {{  .bar = 0,   .bus = 0,   .dev = 5,   .func = 0,  .reg = 0x001c,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd0_glbsts_b1d05f0",        {{  .bar = 0,   .bus = 1,   .dev = 5,   .func = 0,  .reg = 0x001c,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd0_glbsts_b2d05f0",        {{  .bar = 0,   .bus = 2,   .dev = 5,   .func = 0,  .reg = 0x001c,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd0_glbsts_b3d05f0",        {{  .bar = 0,   .bus = 3,   .dev = 5,   .func = 0,  .reg = 0x001c,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd1_fltsts_b0d05f0",        {{  .bar = 0,   .bus = 0,   .dev = 5,   .func = 0,  .reg = 0x1034,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd1_fltsts_b1d05f0",        {{  .bar = 0,   .bus = 1,   .dev = 5,   .func = 0,  .reg = 0x1034,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd1_fltsts_b2d05f0",        {{  .bar = 0,   .bus = 2,   .dev = 5,   .func = 0,  .reg = 0x1034,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd1_fltsts_b3d05f0",        {{  .bar = 0,   .bus = 3,   .dev = 5,   .func = 0,  .reg = 0x1034,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd1_glbsts_b0d05f0",        {{  .bar = 0,   .bus = 0,   .dev = 5,   .func = 0,  .reg = 0x101c,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd1_glbsts_b1d05f0",        {{  .bar = 0,   .bus = 1,   .dev = 5,   .func = 0,  .reg = 0x101c,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd1_glbsts_b2d05f0",        {{  .bar = 0,   .bus = 2,   .dev = 5,   .func = 0,  .reg = 0x101c,  .lenCode = US_MMIO_DWORD }}},
    { "iio_vtd1_glbsts_b3d05f0",        {{  .bar = 0,   .bus = 3,   .dev = 5,   .func = 0,  .reg = 0x101c,  .lenCode = US_MMIO_DWORD }}},
    { "ntb_b1d00f0_devsts_pb01base",    {{  .bar = 0,   .bus = 1,   .dev = 0,   .func = 0,  .reg = 0x459a,  .lenCode =  US_MMIO_WORD }}},
    { "ntb_b1d00f0_devsts_sb01base",    {{  .bar = 2,   .bus = 1,   .dev = 0,   .func = 0,  .reg = 0x059a,  .lenCode =  US_MMIO_WORD }}},
    { "ntb_b1d00f0_lnksts_pb01base",    {{  .bar = 0,   .bus = 1,   .dev = 0,   .func = 0,  .reg = 0x45a2,  .lenCode =  US_MMIO_WORD }}},
    { "ntb_b1d00f0_lnksts_sb01base",    {{  .bar = 2,   .bus = 1,   .dev = 0,   .func = 0,  .reg = 0x05a2,  .lenCode =  US_MMIO_WORD }}},
    { "ntb_b2d00f0_devsts_pb01base",    {{  .bar = 0,   .bus = 2,   .dev = 0,   .func = 0,  .reg = 0x459a,  .lenCode =  US_MMIO_WORD }}},
    { "ntb_b2d00f0_devsts_sb01base",    {{  .bar = 2,   .bus = 2,   .dev = 0,   .func = 0,  .reg = 0x059a,  .lenCode =  US_MMIO_WORD }}},
    { "ntb_b2d00f0_lnksts_pb01base",    {{  .bar = 0,   .bus = 2,   .dev = 0,   .func = 0,  .reg = 0x45a2,  .lenCode =  US_MMIO_WORD }}},
    { "ntb_b2d00f0_lnksts_sb01base",    {{  .bar = 2,   .bus = 2,   .dev = 0,   .func = 0,  .reg = 0x05a2,  .lenCode =  US_MMIO_WORD }}},
    { "ntb_b3d00f0_devsts_pb01base",    {{  .bar = 0,   .bus = 3,   .dev = 0,   .func = 0,  .reg = 0x459a,  .lenCode =  US_MMIO_WORD }}},
    { "ntb_b3d00f0_devsts_sb01base",    {{  .bar = 2,   .bus = 3,   .dev = 0,   .func = 0,  .reg = 0x059a,  .lenCode =  US_MMIO_WORD }}},
    { "ntb_b3d00f0_lnksts_pb01base",    {{  .bar = 0,   .bus = 3,   .dev = 0,   .func = 0,  .reg = 0x45a2,  .lenCode =  US_MMIO_WORD }}},
    { "ntb_b3d00f0_lnksts_sb01base",    {{  .bar = 2,   .bus = 3,   .dev = 0,   .func = 0,  .reg = 0x05a2,  .lenCode =  US_MMIO_WORD }}},
};

static const SUncoreStatusRegIio sUncoreStatusIio[] =
{
    { "iio_cstack_mc_%s", 0 },
    { "iio_pstack0_mc_%s", 1 },
    { "iio_pstack1_mc_%s", 2 },
    { "iio_pstack2_mc_%s", 3 },
    { "iio_pstack3_mc_%s", 4 },
};

/******************************************************************************
*
*   uncoreStatusPciRaw
*
*   This function writes the Uncore Status PCI registers to raw file
*
******************************************************************************/
#ifdef BUILD_RAW
static void uncoreStatusPciRaw(UINT8 u8Cpu, const char * regName, SUncoreStatusRegRawData * sRegData, FILE * fpRaw)
{
#ifdef SPX_BMC_ACD
    UN_USED(u8Cpu);
    UN_USED(regName);
#endif
    // Add the Uncore Status register data to the Uncore Status dump
    fwrite(sRegData, sizeof(SUncoreStatusRegRawData), 1, fpRaw);
}
#endif //BUILD_RAW

/******************************************************************************
*
*   uncoreStatusPciJson
*
*   This function formats the Uncore Status PCI registers into a JSON object
*
******************************************************************************/
#ifdef BUILD_JSON
static void uncoreStatusPciJson(UINT8 u8Cpu, const char * regName, SUncoreStatusRegRawData * sRegData, cJSON * pJsonChild)
{
    cJSON * socket;
    char jsonItemString[US_JSON_STRING_LEN];

    // Add the socket number item to the Uncore Status JSON structure only if it doesn't already exist
    snprintf(jsonItemString, US_JSON_STRING_LEN, US_JSON_SOCKET_NAME, u8Cpu);
    if ((socket = cJSON_GetObjectItemCaseSensitive(pJsonChild, jsonItemString)) == NULL) {
        cJSON_AddItemToObject(pJsonChild, jsonItemString, socket = cJSON_CreateObject());
    }

    // Format the Uncore Status register data out to the .json debug file
    if (sRegData->bInvalid) {
		snprintf(jsonItemString, US_JSON_STRING_LEN, "%s0x%02x", UA_STRING, sRegData->uValue.u32[0]);
        cJSON_AddStringToObject(socket, regName, jsonItemString);
		return;
    } 
	
	if (sRegData->u8CC == PECI_CC_SUCCESS) {
		snprintf(jsonItemString, US_JSON_STRING_LEN, "0x%08x%08x",
			sRegData->uValue.u32[1], sRegData->uValue.u32[0]);
	}
	else {
		snprintf(jsonItemString, US_JSON_STRING_LEN, "0x%08x%08x,%s0x%02x",
			sRegData->uValue.u32[1], sRegData->uValue.u32[0],
			UA_STRING, sRegData->u8CC);
	}
	cJSON_AddStringToObject(socket, regName, jsonItemString);
}
#endif //BUILD_JSON

/******************************************************************************
*
*   uncoreStatusPci
*
*   This function gathers the Uncore Status PCI registers
*
******************************************************************************/
static ESTATUS uncoreStatusPci(FILE * fpRaw, cJSON * pJsonChild)
{
    UINT32 i;
    UINT8 u8Cpu;
    UINT8 u8Dword;
    unsigned int work, bus, device, func, reg;
    SRdPCIConfigLocalReq sRdPCIConfigLocalReq;
    SRdPCIConfigLocalRes sRdPCIConfigLocalRes;
#ifdef BUILD_JSON
    char regName[US_JSON_STRING_LEN];
#endif //BUILD_JSON

    for (u8Cpu = CPU0_ID; u8Cpu < MAX_CPU; u8Cpu++) {
        if (FALSE == IsCpuPresent(u8Cpu)) {
            continue;
        }

        PRINT(PRINT_DBG, PRINT_INFO, "Platform Debug - Uncore Status PCI Registers %d\n", u8Cpu);

        // Get the Uncore Status PCI Registers
        for (i = 0; i < (sizeof(sUncoreStatusPci) / sizeof(SUncoreStatusRegPci)); i++) {
#ifndef SPX_BMC_ACD
            SUncoreStatusRegRawData sRegData = {};
#else
			SUncoreStatusRegRawData sRegData;
			memset(&sRegData, 0x0, sizeof(SUncoreStatusRegRawData));
                        sRegData.u8CC = PECI_CC_SUCCESS;
#endif
            PRINT(PRINT_DBG, PRINT_DEBUG2, "%s (CPU %d BDF %x:%x:%x reg %x) Local PCI Read\n", sUncoreStatusPci[i].regName, u8Cpu, sUncoreStatusPci[i].u8Bus, sUncoreStatusPci[i].u8Dev, sUncoreStatusPci[i].u8Func, sUncoreStatusPci[i].u16Reg);
            switch (sUncoreStatusPci[i].u8Size) {
                case US_REG_BYTE:
                case US_REG_WORD:
                case US_REG_DWORD:
                    memset(&sRdPCIConfigLocalReq, 0 , sizeof(SRdPCIConfigLocalReq));
                    memset(&sRdPCIConfigLocalRes, 0 , sizeof(SRdPCIConfigLocalRes));
                    sRdPCIConfigLocalReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
                    sRdPCIConfigLocalReq.sHeader.u8WriteLength = 0x05;
                    sRdPCIConfigLocalReq.sHeader.u8ReadLength = sUncoreStatusPci[i].u8Size + 1;
                    sRdPCIConfigLocalReq.u8CmdCode = 0xE1;
                    sRdPCIConfigLocalReq.u8HostID_Retry = 0x02;
                    bus = sUncoreStatusPci[i].u8Bus;
                    device = sUncoreStatusPci[i].u8Dev;
                    func = sUncoreStatusPci[i].u8Func;
                    reg = sUncoreStatusPci[i].u16Reg;
                    work = 0x00;
                    work |= (bus << 20) & 0x00F00000;
                    work |= (device << 15) & 0x000f8000;
                    work |= (func << 12) & 0x00007000;
                    work |= reg & 0x00000FFF;
                    sRdPCIConfigLocalReq.u8PCIConfigAddr[0] = work & 0x000000FF;
                    sRdPCIConfigLocalReq.u8PCIConfigAddr[1] = (work >> 8) & 0x000000FF;
                    sRdPCIConfigLocalReq.u8PCIConfigAddr[2] = (work >> 16) & 0x000000FF;
                    if (0 == PECI_RdPCIConfigLocal (&sRdPCIConfigLocalReq, &sRdPCIConfigLocalRes))
                    {
                        memcpy(&sRegData.uValue.u64, sRdPCIConfigLocalRes.u8Data, sUncoreStatusPci[i].u8Size);
                    }

					if (sRdPCIConfigLocalRes.u8CompletionCode != PECI_CC_SUCCESS) {
	                    sRegData.u8CC = sRdPCIConfigLocalRes.u8CompletionCode;

						if (sRdPCIConfigLocalRes.u8CompletionCode != PECI_CC_MAYBE_SUCCESS &&
							sRdPCIConfigLocalRes.u8CompletionCode != PECI_CC_CMD_PASSED) {
							PRINT(PRINT_DBG, PRINT_ERROR, "%s (CPU %d BDF %x:%x:%x reg %x) Local PCI Failed\n", sUncoreStatusPci[i].regName, u8Cpu, sUncoreStatusPci[i].u8Bus, sUncoreStatusPci[i].u8Dev, sUncoreStatusPci[i].u8Func, sUncoreStatusPci[i].u16Reg);
							sRegData.uValue.u32[0] = sRdPCIConfigLocalRes.u8CompletionCode;
							sRegData.bInvalid = TRUE;
						}

					}
                    break;
                case US_REG_QWORD:
                    for (u8Dword = 0; u8Dword < 2; u8Dword++) {
                        memset(&sRdPCIConfigLocalReq, 0 , sizeof(SRdPCIConfigLocalReq));
                        memset(&sRdPCIConfigLocalRes, 0 , sizeof(SRdPCIConfigLocalRes));
                        sRdPCIConfigLocalReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
                        sRdPCIConfigLocalReq.sHeader.u8WriteLength = 0x05;
                        sRdPCIConfigLocalReq.sHeader.u8ReadLength = sizeof(UINT32) + 1;
                        sRdPCIConfigLocalReq.u8CmdCode = 0xE1;
                        sRdPCIConfigLocalReq.u8HostID_Retry = 0x02;
                        bus = sUncoreStatusPci[i].u8Bus;
                        device = sUncoreStatusPci[i].u8Dev;
                        func = sUncoreStatusPci[i].u8Func;
                        reg = sUncoreStatusPci[i].u16Reg + (u8Dword * 4);
                        work = 0x00;
                        work |= (bus << 20) & 0x00F00000;
                        work |= (device << 15) & 0x000f8000;
                        work |= (func << 12) & 0x00007000;
                        work |= reg & 0x00000FFF;
                        sRdPCIConfigLocalReq.u8PCIConfigAddr[0] = work & 0x000000FF;
                        sRdPCIConfigLocalReq.u8PCIConfigAddr[1] = (work >> 8) & 0x000000FF;
                        sRdPCIConfigLocalReq.u8PCIConfigAddr[2] = (work >> 16) & 0x000000FF;
                        if (0 == PECI_RdPCIConfigLocal (&sRdPCIConfigLocalReq, &sRdPCIConfigLocalRes))
                        {
                            memcpy(&sRegData.uValue.u32[u8Dword], sRdPCIConfigLocalRes.u8Data, sizeof(UINT32));
                        }

                        if (sRdPCIConfigLocalRes.u8CompletionCode != PECI_CC_SUCCESS) {
							// The PECI completion code field is intiailized to 0x40. Only override it when we get other than 0x40.
							// That is, always keep the last non-0x40 completion code per register.
							sRegData.u8CC = sRdPCIConfigLocalRes.u8CompletionCode;

							if (sRdPCIConfigLocalRes.u8CompletionCode != PECI_CC_MAYBE_SUCCESS &&
								sRdPCIConfigLocalRes.u8CompletionCode != PECI_CC_CMD_PASSED) {

								PRINT(PRINT_DBG, PRINT_ERROR, "%s (CPU %d BDF %x:%x:%x reg %x) Local PCI Failed\n", sUncoreStatusPci[i].regName, u8Cpu, sUncoreStatusPci[i].u8Bus, sUncoreStatusPci[i].u8Dev, sUncoreStatusPci[i].u8Func, sUncoreStatusPci[i].u16Reg + (u8Dword * 4));
								sRegData.uValue.u32[0] = sRdPCIConfigLocalRes.u8CompletionCode;
								sRegData.bInvalid = TRUE;
								break;
							}
						}
                    }
                    break;
#ifdef SPX_BMC_ACD
                default:
				    break;
#endif
            }
            // Log this Uncore Status PCI Register
#ifdef BUILD_RAW
            uncoreStatusPciRaw(u8Cpu, sUncoreStatusPci[i].regName, &sRegData, fpRaw);
#endif //BUILD_RAW
#ifdef BUILD_JSON
            snprintf(regName, US_JSON_STRING_LEN, "B%02d_D%02d_F%d_0x%X",
                        sUncoreStatusPci[i].u8Bus,
                        sUncoreStatusPci[i].u8Dev,
                        sUncoreStatusPci[i].u8Func,
                        sUncoreStatusPci[i].u16Reg
                    );
            uncoreStatusPciJson(u8Cpu, regName, &sRegData, pJsonChild);
#endif //BUILD_JSON
        }
    }
    return ST_OK;
}

/******************************************************************************
*
*   uncoreStatusMmioRead
*
*   This function gathers the Uncore Status PCI MMIO registers
*
******************************************************************************/
static ESTATUS uncoreStatusMmioRead(UINT8 u8Cpu, UINT32 u32Param, UINT8 u8NumDwords, SUncoreStatusRegRawData * sUncoreStatusMmioRawData)
{
    SRdPkgConfigReq sRdPkgConfigReq;
    SRdPkgConfigRes sRdPkgConfigRes;
    SWrPkgConfigReq sWrPkgConfigReq;
    SWrPkgConfigRes sWrPkgConfigRes;
    UINT32 u32work;
    UINT8 u8Dword;
    UINT8 u8CC;

    // Open the MMIO dump sequence
    PRINT(PRINT_DBG, PRINT_INFO, "[DBG] - CPU #%d Uncore Status MMIO Param %x Sequence Opened\n", u8Cpu, u32Param);
    memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
    memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
    sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
    sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
    sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
    sWrPkgConfigReq.u8CmdCode = 0xA5;
    sWrPkgConfigReq.u8HostID_Retry = 0x02;
    sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
    sWrPkgConfigReq.u16Parameter = VCU_OPEN_SEQ;
    u32work = VCU_READ_LOCAL_MMIO_SEQ;
    memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
    PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
    u8CC = sWrPkgConfigRes.u8CompletionCode;
    if (u8CC != PECI_CC_SUCCESS) {
        // MMIO sequence failed, abort the sequence
        sUncoreStatusMmioRawData->uValue.u32[0] = sWrPkgConfigRes.u8CompletionCode;
		sUncoreStatusMmioRawData->bInvalid = TRUE;

		memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
        memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
        sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
        sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
        sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
        sWrPkgConfigReq.u8CmdCode = 0xA5;
        sWrPkgConfigReq.u8HostID_Retry = 0x02;
        sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
        sWrPkgConfigReq.u16Parameter = VCU_ABORT_SEQ;
        u32work = VCU_READ_LOCAL_MMIO_SEQ;
        memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
	PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
        PRINT(PRINT_DBG, PRINT_ERROR, "[DBG] - CPU #%d Uncore Status MMIO Param %x Sequence Failed (0x%x)\n", u8Cpu, u32Param, u8CC);
	return ST_HW_FAILURE;
    }

    // Set MMIO address
    PRINT(PRINT_DBG, PRINT_INFO, "[DBG] - CPU #%d Set MMIO Parameter %x\n", u8Cpu, u32Param);
    memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
    memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
    sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
    sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
    sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
    sWrPkgConfigReq.u8CmdCode = 0xA5;
    sWrPkgConfigReq.u8HostID_Retry = 0x02;
    sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
    sWrPkgConfigReq.u16Parameter = US_MMIO_PARAM;
    u32work = u32Param;
    memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
    PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);

    u8CC = sWrPkgConfigRes.u8CompletionCode;
    if (u8CC != PECI_CC_SUCCESS) {
        // MMIO sequence failed, abort the sequence
        sUncoreStatusMmioRawData->uValue.u32[0] = sWrPkgConfigRes.u8CompletionCode;
	sUncoreStatusMmioRawData->bInvalid = TRUE;

	memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
        memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
        sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
        sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
        sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
        sWrPkgConfigReq.u8CmdCode = 0xA5;
        sWrPkgConfigReq.u8HostID_Retry = 0x02;
        sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
        sWrPkgConfigReq.u16Parameter = VCU_ABORT_SEQ;
        u32work = VCU_READ_LOCAL_MMIO_SEQ;
        memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
	PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
        PRINT(PRINT_DBG, PRINT_ERROR, "[DBG] - CPU #%d Uncore Status MMIO Param %x Sequence Failed (0x%x)\n", u8Cpu, u32Param, u8CC);
	return ST_HW_FAILURE;
    }

	// initlize the completion code to 0x40
	sUncoreStatusMmioRawData->u8CC = PECI_CC_SUCCESS;

    // Get the MMIO data
    for (u8Dword = 0; u8Dword < u8NumDwords; u8Dword++) {
        memset(&sRdPkgConfigReq, 0 , sizeof(SRdPkgConfigReq));
        memset(&sRdPkgConfigRes, 0 , sizeof(SRdPkgConfigRes));
        sRdPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
        sRdPkgConfigReq.sHeader.u8WriteLength = 0x05;
        sRdPkgConfigReq.sHeader.u8ReadLength = sizeof(UINT32) + 1;
        sRdPkgConfigReq.u8CmdCode = 0xA1;
        sRdPkgConfigReq.u8HostID_Retry = 0x02;
        sRdPkgConfigReq.u8Index = MBX_INDEX_VCU;
        sRdPkgConfigReq.u16Parameter = VCU_READ;
        if (0 == PECI_RdPkgConfig (&sRdPkgConfigReq, &sRdPkgConfigRes))
        {
            memcpy(&sUncoreStatusMmioRawData->uValue.u32[u8Dword], sRdPkgConfigRes.u8Data, sizeof(UINT32));
        }

	u8CC = sRdPkgConfigRes.u8CompletionCode;
	if (u8CC != PECI_CC_SUCCESS) {
		// The PECI completion code field is intiailized to 0x40. Only override it when we get other than 0x40.
		// That is, always keep the last non-0x40 completion code per register.
		sUncoreStatusMmioRawData->u8CC = u8CC;

		// continue sequnce on 0x91 or 0x94
		if (u8CC != PECI_CC_MAYBE_SUCCESS && u8CC != PECI_CC_CMD_PASSED) {
			// MMIO sequence failed, abort the sequence
			sUncoreStatusMmioRawData->uValue.u32[0] = sRdPkgConfigRes.u8CompletionCode;
			sUncoreStatusMmioRawData->bInvalid = TRUE;

			memset(&sWrPkgConfigReq, 0, sizeof(SWrPkgConfigReq));
			memset(&sWrPkgConfigRes, 0, sizeof(SWrPkgConfigRes));
			sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
			sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
			sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
			sWrPkgConfigReq.u8CmdCode = 0xA5;
			sWrPkgConfigReq.u8HostID_Retry = 0x02;
			sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
			sWrPkgConfigReq.u16Parameter = VCU_ABORT_SEQ;
			u32work = VCU_READ_LOCAL_MMIO_SEQ;
			memcpy(sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
			PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
			PRINT(PRINT_DBG, PRINT_ERROR, "[DBG] - CPU #%d Uncore Status MMIO Param %x Sequence Failed (0x%x)\n", u8Cpu, u32Param, u8CC);
			return ST_HW_FAILURE;
		}
	}
    }

    // Close the MMIO sequence
    PRINT(PRINT_DBG, PRINT_INFO, "[DBG] - CPU #%d Uncore Status MMIO Param %x Sequence Closed\n", u8Cpu, u32Param);
    memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
    memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
    sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
    sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
    sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
    sWrPkgConfigReq.u8CmdCode = 0xA5;
    sWrPkgConfigReq.u8HostID_Retry = 0x02;
    sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
    sWrPkgConfigReq.u16Parameter = VCU_CLOSE_SEQ;
    u32work = VCU_READ_LOCAL_MMIO_SEQ;
    memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
    PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);

    return ST_OK;
}

/******************************************************************************
*
*   uncoreStatusPciMmio
*
*   This function gathers the Uncore Status PCI MMIO registers
*
******************************************************************************/
static ESTATUS uncoreStatusPciMmio(FILE * fpRaw, cJSON * pJsonChild)
{
    UINT32 i;
    UINT8 u8Cpu;
#ifdef BUILD_JSON
    char regName[US_JSON_STRING_LEN];
#endif //BUILD_JSON

    for (u8Cpu = CPU0_ID; u8Cpu < MAX_CPU; u8Cpu++) {
        if (FALSE == IsCpuPresent(u8Cpu)) {
            continue;
        }

        PRINT(PRINT_DBG, PRINT_INFO, "Platform Debug - Uncore Status PCI MMIO Registers %d\n", u8Cpu);

        // Get the Uncore Status PCI MMIO Registers
        for (i = 0; i < (sizeof(sUncoreStatusPciMmio) / sizeof(SUncoreStatusRegPciMmio)); i++) {
#ifndef SPX_BMC_ACD
            SUncoreStatusRegRawData sRegData = {};
#else
	    SUncoreStatusRegRawData sRegData;
	    memset(&sRegData, 0x0, sizeof(SUncoreStatusRegRawData));
#endif
            UINT32 u32MmioParam = sUncoreStatusPciMmio[i].uMmioReg.raw;
            UINT8 u8NumDwords = sUncoreStatusPciMmio[i].uMmioReg.fields.lenCode == US_MMIO_QWORD ? 2 : 1;

            // Get the MMIO data
            if (uncoreStatusMmioRead(u8Cpu, u32MmioParam, u8NumDwords, &sRegData) == ST_OK) {
                PRINT(PRINT_DBG, PRINT_INFO, "[DBG] - CPU #%d MMIO %x logged\n", u8Cpu, u32MmioParam);
            }

            // Log this Uncore Status PCI Register
#ifdef BUILD_RAW
            uncoreStatusPciRaw(u8Cpu, sUncoreStatusPci[i].regName, &sRegData, fpRaw);
#endif //BUILD_RAW
#ifdef BUILD_JSON
            snprintf(regName, US_JSON_STRING_LEN, "B%02d_D%02d_F%d_0x%X",
                        sUncoreStatusPciMmio[i].uMmioReg.fields.bus,
                        sUncoreStatusPciMmio[i].uMmioReg.fields.dev,
                        sUncoreStatusPciMmio[i].uMmioReg.fields.func,
                        sUncoreStatusPciMmio[i].uMmioReg.fields.reg
                    );
            uncoreStatusPciJson(u8Cpu, regName, &sRegData, pJsonChild);
#endif //BUILD_JSON
        }
    }
    return ST_OK;
}

/******************************************************************************
*
*   uncoreStatusMcaRead
*
*   This function gathers the Uncore Status MCA registers
*
******************************************************************************/
static ESTATUS uncoreStatusMcaRead(UINT8 u8Cpu, UINT32 u32Param, SUncoreStatusMcaRawData * sUncoreStatusMcaRawData)
{
    SRdPkgConfigReq sRdPkgConfigReq;
    SRdPkgConfigRes sRdPkgConfigRes;
    SWrPkgConfigReq sWrPkgConfigReq;
    SWrPkgConfigRes sWrPkgConfigRes;
    UINT32 u32work;
    UINT8 u8Dword;

    // Open the MCA Bank dump sequence
    PRINT(PRINT_DBG, PRINT_INFO, "[DBG] - CPU #%d Uncore Status MCA Param %x Sequence Opened\n", u8Cpu, u32Param);
    memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
    memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
    sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
    sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
    sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
    sWrPkgConfigReq.u8CmdCode = 0xA5;
    sWrPkgConfigReq.u8HostID_Retry = 0x02;
    sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
    sWrPkgConfigReq.u16Parameter = VCU_OPEN_SEQ;
    u32work = VCU_UNCORE_MCA_SEQ;
    memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
    PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);

    if (sWrPkgConfigRes.u8CompletionCode != PECI_CC_SUCCESS) {
        // MCA Bank sequence failed, abort the sequence
        sUncoreStatusMcaRawData->uRegData.u32Raw[0] = sWrPkgConfigRes.u8CompletionCode;
        memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
        memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
        sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
        sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
        sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
        sWrPkgConfigReq.u8CmdCode = 0xA5;
        sWrPkgConfigReq.u8HostID_Retry = 0x02;
        sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
        sWrPkgConfigReq.u16Parameter = VCU_ABORT_SEQ;
        u32work = VCU_UNCORE_MCA_SEQ;
        memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
	PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
        PRINT(PRINT_DBG, PRINT_ERROR, "[DBG] - CPU #%d Uncore Status MCA Param %x Sequence Failed\n", u8Cpu, u32Param);
        return ST_HW_FAILURE;
    }

    // Set MCA Bank number
    PRINT(PRINT_DBG, PRINT_INFO, "[DBG] - CPU #%d Set MCA Parameter %x\n", u8Cpu, u32Param);
    memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
    memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
    sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
    sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
    sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
    sWrPkgConfigReq.u8CmdCode = 0xA5;
    sWrPkgConfigReq.u8HostID_Retry = 0x02;
    sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
    sWrPkgConfigReq.u16Parameter = US_MCA_PARAM;
    u32work = u32Param;
    memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
    PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);

    if (sWrPkgConfigRes.u8CompletionCode != PECI_CC_SUCCESS) {
        // MCA Bank sequence failed, abort the sequence
        sUncoreStatusMcaRawData->uRegData.u32Raw[0] = sWrPkgConfigRes.u8CompletionCode;
        memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
        memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
        sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
        sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
        sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
        sWrPkgConfigReq.u8CmdCode = 0xA5;
        sWrPkgConfigReq.u8HostID_Retry = 0x02;
        sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
        sWrPkgConfigReq.u16Parameter = VCU_ABORT_SEQ;
        u32work = VCU_UNCORE_MCA_SEQ;
        memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
        PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
        PRINT(PRINT_DBG, PRINT_ERROR, "[DBG] - CPU #%d Uncore Status MCA Param %x Sequence Failed\n", u8Cpu, u32Param);
        return ST_HW_FAILURE;
    }

    // Get the MCA Bank Registers
    for (u8Dword = 0; u8Dword < US_NUM_MCA_DWORDS; u8Dword++) {
	// initialize the completion code
	if (u8Dword % 2 == 0)
		sUncoreStatusMcaRawData->u8CC[u8Dword / 2] = PECI_CC_SUCCESS;

        memset(&sRdPkgConfigReq, 0 , sizeof(SRdPkgConfigReq));
        memset(&sRdPkgConfigRes, 0 , sizeof(SRdPkgConfigRes));
        sRdPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
        sRdPkgConfigReq.sHeader.u8WriteLength = 0x05;
        sRdPkgConfigReq.sHeader.u8ReadLength = sizeof(UINT32) + 1;
        sRdPkgConfigReq.u8CmdCode = 0xA1;
        sRdPkgConfigReq.u8HostID_Retry = 0x02;
        sRdPkgConfigReq.u8Index = MBX_INDEX_VCU;
        sRdPkgConfigReq.u16Parameter = VCU_READ;
        if (0 == PECI_RdPkgConfig (&sRdPkgConfigReq, &sRdPkgConfigRes))
        {
            memcpy(&sUncoreStatusMcaRawData->uRegData.u32Raw[u8Dword], sRdPkgConfigRes.u8Data, sizeof(UINT32));
        }

	if (sRdPkgConfigRes.u8CompletionCode != PECI_CC_SUCCESS) {
		// The PECI completion code field is intiailized to 0x40. Only override it when we get other than 0x40.
		// That is, always keep the last non-0x40 completion code per register.
		sUncoreStatusMcaRawData->u8CC[u8Dword / 2] = sRdPkgConfigRes.u8CompletionCode;

		if (sRdPkgConfigRes.u8CompletionCode != PECI_CC_MAYBE_SUCCESS && sRdPkgConfigRes.u8CompletionCode != PECI_CC_CMD_PASSED) {

			// MCA Bank sequence failed, abort the sequence on non-0x40/0x91/0x94
			sUncoreStatusMcaRawData->uRegData.u32Raw[0] = sRdPkgConfigRes.u8CompletionCode;
			memset(&sWrPkgConfigReq, 0, sizeof(SWrPkgConfigReq));
			memset(&sWrPkgConfigRes, 0, sizeof(SWrPkgConfigRes));
			sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
			sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
			sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
			sWrPkgConfigReq.u8CmdCode = 0xA5;
			sWrPkgConfigReq.u8HostID_Retry = 0x02;
			sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
			sWrPkgConfigReq.u16Parameter = VCU_ABORT_SEQ;
			u32work = VCU_UNCORE_MCA_SEQ;
			memcpy(sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
			PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
			PRINT(PRINT_DBG, PRINT_ERROR, "[DBG] - CPU #%d Uncore Status MCA Param %x Sequence Failed\n", u8Cpu, u32Param);
			return ST_HW_FAILURE;
		}
	}
    }

    // Close the MCA Bank sequence
    PRINT(PRINT_DBG, PRINT_INFO, "[DBG] - CPU #%d Uncore Status MCA Param %x Sequence Closed\n", u8Cpu, u32Param);
    memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
    memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
    sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
    sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
    sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
    sWrPkgConfigReq.u8CmdCode = 0xA5;
    sWrPkgConfigReq.u8HostID_Retry = 0x02;
    sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
    sWrPkgConfigReq.u16Parameter = VCU_CLOSE_SEQ;
    u32work = VCU_UNCORE_MCA_SEQ;
    memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
    PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);

    return ST_OK;
}

/******************************************************************************
*
*   uncoreStatusCboRaw
*
*   This function writes the Uncore Status CBO MCA registers to raw file
*
******************************************************************************/
#ifdef BUILD_RAW
static void uncoreStatusCboRaw(UINT8 u8Cpu, UINT32 u32CboNum, SUncoreStatusMcaRawData * psUncoreStatusCboRawData, FILE * fpRaw)
{
#ifdef SPX_BMC_ACD
    UN_USED(u8Cpu);
    UN_USED(u32CboNum);
#endif
    // Add the Uncore Status CBO MCA data to the Uncore Status dump
    fwrite(psUncoreStatusCboRawData, sizeof(SUncoreStatusMcaRawData), 1, fpRaw);
}
#endif //BUILD_RAW

/******************************************************************************
*
*   uncoreStatusCboJson
*
*   This function formats the Uncore Status CBO MCA registers into a JSON object
*
******************************************************************************/
#ifdef BUILD_JSON
static void uncoreStatusCboJson(UINT8 u8Cpu, UINT32 u32CboNum, SUncoreStatusMcaRawData * psUncoreStatusCboRawData, cJSON * pJsonChild)
{
    cJSON * socket;
    char jsonItemString[US_JSON_STRING_LEN];
    char jsonNameString[US_JSON_STRING_LEN];
    UINT32 i;

    // Add the socket number item to the Uncore Status JSON structure only if it doesn't already exist
    snprintf(jsonItemString, US_JSON_STRING_LEN, US_JSON_SOCKET_NAME, u8Cpu);
    if ((socket = cJSON_GetObjectItemCaseSensitive(pJsonChild, jsonItemString)) == NULL) {
        cJSON_AddItemToObject(pJsonChild, jsonItemString, socket = cJSON_CreateObject());
    }

    // Format the Uncore Status CBO MCA data out to the .json debug file
    // Fill in NULL for this CBO MCA if it's not valid
    if (psUncoreStatusCboRawData->bInvalid) {
        for (i = 0; i < US_NUM_MCA_QWORDS; i++) {
            snprintf(jsonNameString, US_JSON_STRING_LEN, US_CBO_MCA_REG_NAME, u32CboNum, uncoreStatusMcaRegNames[i]);
	    snprintf(jsonItemString, US_JSON_STRING_LEN, "%s0x%02x", UA_STRING, psUncoreStatusCboRawData->uRegData.u32Raw[0]);
            cJSON_AddStringToObject(socket, jsonNameString, jsonItemString);
        }
		return;
    } 
	
    for (i = 0; i < US_NUM_MCA_QWORDS; i++) {
        snprintf(jsonNameString, US_JSON_STRING_LEN, US_CBO_MCA_REG_NAME, u32CboNum, uncoreStatusMcaRegNames[i]);

	if (psUncoreStatusCboRawData->u8CC[i] == PECI_CC_SUCCESS) {
		snprintf(jsonItemString, US_JSON_STRING_LEN, "0x%08x%08x",
			psUncoreStatusCboRawData->uRegData.u32Raw[(i * 2) + 1],
			psUncoreStatusCboRawData->uRegData.u32Raw[(i * 2)]);
	}
	else {
            snprintf(jsonItemString, US_JSON_STRING_LEN, "0x%08x%08x,%s0x%02x",
                     psUncoreStatusCboRawData->uRegData.u32Raw[(i * 2) + 1], 
		     psUncoreStatusCboRawData->uRegData.u32Raw[(i * 2)],
		     UA_STRING, psUncoreStatusCboRawData->u8CC[i]);
        }
	cJSON_AddStringToObject(socket, jsonNameString, jsonItemString);
    }
}
#endif //BUILD_JSON

/******************************************************************************
*
*   uncoreStatusCbo
*
*   This function gathers the Uncore Status CBO MCA registers
*
******************************************************************************/
static ESTATUS uncoreStatusCbo(FILE * fpRaw, cJSON * pJsonChild)
{
    SRdPkgConfigReq sRdPkgConfigReq;
    SRdPkgConfigRes sRdPkgConfigRes;
    UINT32 u32NumCbos;
    UINT32 i;
    UINT8 u8Cpu;

    for (u8Cpu = CPU0_ID; u8Cpu < MAX_CPU; u8Cpu++) {
        if (FALSE == IsCpuPresent(u8Cpu)) {
            continue;
        }

        PRINT(PRINT_DBG, PRINT_INFO, "Platform Debug - Uncore Status CBO MCA log %d\n", u8Cpu);
		
	// intialize to 0
	u32NumCbos = 0;

        // Get the maximum Thread ID of the processor
        memset(&sRdPkgConfigReq, 0 , sizeof(SRdPkgConfigReq));
        memset(&sRdPkgConfigRes, 0 , sizeof(SRdPkgConfigRes));
        sRdPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
        sRdPkgConfigReq.sHeader.u8WriteLength = 0x05;
        sRdPkgConfigReq.sHeader.u8ReadLength = sizeof(u32NumCbos) + 1;
        sRdPkgConfigReq.u8CmdCode = 0xA1;
        sRdPkgConfigReq.u8HostID_Retry = 0x02;
        sRdPkgConfigReq.u8Index = MBX_INDEX_CPU_ID;
        sRdPkgConfigReq.u16Parameter = PKG_ID_MAX_THREAD_ID;
        if (0 == PECI_RdPkgConfig (&sRdPkgConfigReq, &sRdPkgConfigRes))
        {
            memcpy(&u32NumCbos, sRdPkgConfigRes.u8Data, sizeof(u32NumCbos));
        }

	if (sRdPkgConfigRes.u8CompletionCode != PECI_CC_SUCCESS) {
		// chop off the thread count on 0x91/0x94 read if exceed
		if (sRdPkgConfigRes.u8CompletionCode == PECI_CC_MAYBE_SUCCESS || sRdPkgConfigRes.u8CompletionCode == PECI_CC_CMD_PASSED) {
			if (u32NumCbos > MAX_THREAD_ID)
				u32NumCbos = MAX_THREAD_ID;
		}
		else {
			// skip to the next CPU if not 0x40 and 0x91.
			continue;
		}
	}

        // Convert max thread ID to number of CBOs
        u32NumCbos = (u32NumCbos / 2) + 1;

        // Go through each CBO in this CPU
        for (i = 0; i < u32NumCbos; i++) {
#ifndef SPX_BMC_ACD
            SUncoreStatusMcaRawData sMcaData = {};
#else
            SUncoreStatusMcaRawData sMcaData;
	    memset(&sMcaData, 0x0, sizeof(SUncoreStatusMcaRawData));
#endif

            // Build the MCA parameter for this CBO
            UINT32 u32CboParam = ((i / US_NUM_CBO_BANKS) << 24) | US_MCA_UNMERGE | ((i % US_NUM_CBO_BANKS) + US_BASE_CBO_BANK);

            // Get the CBO MCA data
            if (uncoreStatusMcaRead(u8Cpu, u32CboParam, &sMcaData) != ST_OK) {
                sMcaData.bInvalid = TRUE;
            } else {
                PRINT(PRINT_DBG, PRINT_INFO, "[DBG] - CPU #%d CBO %d logged\n", u8Cpu, i);
            }

            // Log the MCA for this CBO
#ifdef BUILD_RAW
            uncoreStatusCboRaw(u8Cpu, i, &sMcaData, fpRaw);
#endif //BUILD_RAW
#ifdef BUILD_JSON
            uncoreStatusCboJson(u8Cpu, i, &sMcaData, pJsonChild);
#endif //BUILD_JSON
        }
    }

    return ST_OK;
}

/******************************************************************************
*
*   uncoreStatusIioRaw
*
*   This function writes the Uncore Status IIO MCA registers to raw file
*
******************************************************************************/
#ifdef BUILD_RAW
static void uncoreStatusIioRaw(UINT8 u8Cpu, const char * regName, SUncoreStatusMcaRawData * sMcaData, FILE * fpRaw)
{
#ifdef SPX_BMC_ACD
    UN_USED(u8Cpu);
    UN_USED(regName);
#endif
    // Add the Uncore Status IIO MCA data to the Uncore Status dump
    fwrite(sMcaData, sizeof(SUncoreStatusMcaRawData), 1, fpRaw);
}
#endif //BUILD_RAW

/******************************************************************************
*
*   uncoreStatusIioJson
*
*   This function formats the Uncore Status IIO MCA registers into a JSON object
*
******************************************************************************/
#ifdef BUILD_JSON
static void uncoreStatusIioJson(UINT8 u8Cpu, const char * regName, SUncoreStatusMcaRawData * sMcaData, cJSON * pJsonChild)
{
    cJSON * socket;
    char jsonItemString[US_JSON_STRING_LEN];
    char jsonNameString[US_JSON_STRING_LEN];
    UINT32 i;

    // Add the socket number item to the Uncore Status JSON structure only if it doesn't already exist
    snprintf(jsonItemString, US_JSON_STRING_LEN, US_JSON_SOCKET_NAME, u8Cpu);
    if ((socket = cJSON_GetObjectItemCaseSensitive(pJsonChild, jsonItemString)) == NULL) {
        cJSON_AddItemToObject(pJsonChild, jsonItemString, socket = cJSON_CreateObject());
    }

    // Format the Uncore Status IIO MCA data out to the .json debug file
    // Fill in NULL for this IIO MCA if it's not valid
    if (sMcaData->bInvalid) {
        for (i = 0; i < US_NUM_MCA_QWORDS; i++) {
            snprintf(jsonNameString, US_JSON_STRING_LEN, regName, uncoreStatusMcaRegNames[i]);
	    snprintf(jsonItemString, US_JSON_STRING_LEN, "%s0x%02x", UA_STRING, sMcaData->uRegData.u32Raw[0]);
            cJSON_AddStringToObject(socket, jsonNameString, jsonItemString);
        }
		return;
    } 
	
    for (i = 0; i < US_NUM_MCA_QWORDS; i++) {
	snprintf(jsonNameString, US_JSON_STRING_LEN, regName, uncoreStatusMcaRegNames[i]);

	if (sMcaData->u8CC[i] == PECI_CC_SUCCESS) {
		snprintf(jsonItemString, US_JSON_STRING_LEN, "0x%08x%08x",
			sMcaData->uRegData.u32Raw[(i * 2) + 1], sMcaData->uRegData.u32Raw[(i * 2)]);
	}
	else {
		snprintf(jsonItemString, US_JSON_STRING_LEN, "0x%08x%08x,%s0x%02x",
			sMcaData->uRegData.u32Raw[(i * 2) + 1], sMcaData->uRegData.u32Raw[(i * 2)],
			UA_STRING, sMcaData->u8CC[i]);
	}
	cJSON_AddStringToObject(socket, jsonNameString, jsonItemString);
    }
}
#endif //BUILD_JSON

/******************************************************************************
*
*   uncoreStatusIio
*
*   This function gathers the Uncore Status IIO MCA registers
*
******************************************************************************/
static ESTATUS uncoreStatusIio(FILE * fpRaw, cJSON * pJsonChild)
{
    UINT32 i;
    UINT8 u8Cpu;

#ifdef SPX_BMC_ACD
    UN_USED(u8Cpu);
#endif

    for (u8Cpu = CPU0_ID; u8Cpu < MAX_CPU; u8Cpu++) {
        if (FALSE == IsCpuPresent(u8Cpu)) {
            continue;
        }

        PRINT(PRINT_DBG, PRINT_INFO, "Platform Debug - Uncore Status IIO MCA log %d\n", u8Cpu);

        // Go through each IIO in this CPU
        for (i = 0; i < (sizeof(sUncoreStatusIio) / sizeof(SUncoreStatusRegIio)); i++) {
#ifndef SPX_BMC_ACD
            SUncoreStatusMcaRawData sMcaData = {};
#else
            SUncoreStatusMcaRawData sMcaData;
	    memset(&sMcaData, 0x0, sizeof(SUncoreStatusMcaRawData));
#endif
            // Build the MCA parameter for this IIO
            UINT32 u32IioParam = (sUncoreStatusIio[i].u8IioNum << 24 | US_MCA_UNMERGE | US_BASE_IIO_BANK);

            // Get the IIO MCA data
            if (uncoreStatusMcaRead(u8Cpu, u32IioParam, &sMcaData) != ST_OK) {
                sMcaData.bInvalid = TRUE;
            } else {
                PRINT(PRINT_DBG, PRINT_INFO, "[DBG] - CPU #%d IIO %d logged\n", u8Cpu, sUncoreStatusIio[i].u8IioNum);
            }

            // Log the MCA for this IIO
#ifdef BUILD_RAW
            uncoreStatusIioRaw(u8Cpu, sUncoreStatusIio[i].regName, &sMcaData, fpRaw);
#endif //BUILD_RAW
#ifdef BUILD_JSON
            uncoreStatusIioJson(u8Cpu, sUncoreStatusIio[i].regName, &sMcaData, pJsonChild);
#endif //BUILD_JSON
        }
    }

    return ST_OK;
}

/******************************************************************************
*
*   uncoreStatusCrashdumpRaw
*
*   This function writes the Uncore Status Crashdump registers to raw file
*
******************************************************************************/
#ifdef BUILD_RAW
static void uncoreStatusCrashdumpRaw(UINT8 u8Cpu, UINT32 u32NumReads, UINT32 * pu32UncoreCrashdump, FILE * fpRaw)
{
#ifdef SPX_BMC_ACD
    UN_USED(u8Cpu);
#endif
    // Add the u32NumReads to the Uncore Status dump
    fwrite(&u32NumReads, sizeof(u32NumReads), 1, fpRaw);

    // Add the Uncore Crashdump dump info to the Uncore Status dump
    fwrite(pu32UncoreCrashdump, (sizeof(UINT32) * u32NumReads), 1, fpRaw);
}
#endif //BUILD_RAW

/******************************************************************************
*
*   uncoreStatusCrashdumpJson
*
*   This function formats the Uncore Status Crashdump into a JSON object
*
******************************************************************************/
#ifdef BUILD_JSON
static void uncoreStatusCrashdumpJson(UINT8 u8Cpu, UINT32 u32NumReads, UINT32 * pu32UncoreCrashdump, UINT8 * pu8cc, cJSON * pJsonChild)
{
    UINT32 i;
    cJSON * socket;
    char jsonItemString[US_JSON_STRING_LEN];
    char jsonNameString[US_JSON_STRING_LEN];

    // Add the socket number item to the Uncore Status JSON structure only if it doesn't already exist
    snprintf(jsonItemString, US_JSON_STRING_LEN, US_JSON_SOCKET_NAME, u8Cpu);
    if ((socket = cJSON_GetObjectItemCaseSensitive(pJsonChild, jsonItemString)) == NULL) {
        cJSON_AddItemToObject(pJsonChild, jsonItemString, socket = cJSON_CreateObject());
    }

    // Add the Uncore Crashdump dump info to the Uncore Status dump JSON structure
    for (i = 0; i < u32NumReads; i++) {
        snprintf(jsonNameString, US_JSON_STRING_LEN, US_UNCORE_CRASH_DW_NAME, i);

	if (pu8cc[i] == PECI_CC_SUCCESS) {
		snprintf(jsonItemString, US_JSON_STRING_LEN, "0x%x", pu32UncoreCrashdump[i]);
	}
	else {
		snprintf(jsonItemString, US_JSON_STRING_LEN, "0x%x,%s0x%02x", pu32UncoreCrashdump[i], UA_STRING, pu8cc[i]);
	}

        cJSON_AddStringToObject(socket, jsonNameString, jsonItemString);
    }
}
#endif //BUILD_JSON

/******************************************************************************
*
*   uncoreStatusCrashdump
*
*   This function gathers the Uncore Status Crashdump
*
******************************************************************************/
static ESTATUS uncoreStatusCrashdump(FILE * fpRaw, cJSON * pJsonChild)
{
    SRdPkgConfigReq sRdPkgConfigReq;
    SRdPkgConfigRes sRdPkgConfigRes;
    SWrPkgConfigReq sWrPkgConfigReq;
    SWrPkgConfigRes sWrPkgConfigRes;
    UINT32 u32work;
    ESTATUS eStatus = ST_OK;
    UINT32 u32NumReads = 0;
    UINT32 u32ApiVersion = 0;
    UINT32 i;
    UINT8 u8Cpu;

    // Go through all CPUs
    for (u8Cpu = CPU0_ID; u8Cpu < MAX_CPU; u8Cpu++) {
        if (!IsCpuPresent(u8Cpu)) {
            continue;
        }
        // Start the Uncore Crashdump dump log
        PRINT(PRINT_DBG, PRINT_INFO, "Platform Debug - Uncore Crashdump dump log %d\n", u8Cpu);

        // Open the Uncore Crashdump dump sequence
        PRINT(PRINT_DBG, PRINT_INFO, "[DBG] - CPU #%d Uncore Crashdump Dump Sequence Opened\n", u8Cpu);
        memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
        memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
        sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
        sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
        sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
        sWrPkgConfigReq.u8CmdCode = 0xA5;
        sWrPkgConfigReq.u8HostID_Retry = 0x02;
        sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
        sWrPkgConfigReq.u16Parameter = VCU_OPEN_SEQ;
        u32work = VCU_UNCORE_CRASHDUMP_SEQ;
        memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
	PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);

	if (sWrPkgConfigRes.u8CompletionCode != PECI_CC_SUCCESS) {
            // Uncore Crashdump dump sequence failed, abort the sequence and go to the next CPU
            memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
            memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
            sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
            sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
            sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
            sWrPkgConfigReq.u8CmdCode = 0xA5;
            sWrPkgConfigReq.u8HostID_Retry = 0x02;
            sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
            sWrPkgConfigReq.u16Parameter = VCU_ABORT_SEQ;
            u32work = VCU_UNCORE_CRASHDUMP_SEQ;
            memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
	    PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
            PRINT(PRINT_DBG, PRINT_ERROR, "[DBG] - CPU #%d Uncore Crashdump dump Sequence Failed\n", u8Cpu);
            eStatus = ST_HW_FAILURE;
            continue;
        }

        // Set Uncore Crashdump dump parameter
        PRINT(PRINT_DBG, PRINT_INFO, "[DBG] - CPU #%d Set Uncore Crashdump Dump Parameter\n", u8Cpu);
        memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
        memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
        sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
        sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
        sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
        sWrPkgConfigReq.u8CmdCode = 0xA5;
        sWrPkgConfigReq.u8HostID_Retry = 0x02;
        sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
        sWrPkgConfigReq.u16Parameter = VCU_OPEN_SEQ;
        u32work = US_UCRASH_PARAM;
        memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
	PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);

	if (sWrPkgConfigRes.u8CompletionCode != PECI_CC_SUCCESS) {
            // Uncore Crashdump dump sequence failed, abort the sequence and go to the next CPU
            memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
            memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
            sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
            sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
            sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
            sWrPkgConfigReq.u8CmdCode = 0xA5;
            sWrPkgConfigReq.u8HostID_Retry = 0x02;
            sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
            sWrPkgConfigReq.u16Parameter = VCU_ABORT_SEQ;
            u32work = VCU_UNCORE_CRASHDUMP_SEQ;
            memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
	    PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
            PRINT(PRINT_DBG, PRINT_ERROR, "[DBG] - CPU #%d Uncore Crashdump dump Sequence Failed\n", u8Cpu);
            eStatus = ST_HW_FAILURE;
            continue;
        }

	// initialize to 0
	u32NumReads = 0;

        // Get the number of dwords to read
        memset(&sRdPkgConfigReq, 0 , sizeof(SRdPkgConfigReq));
        memset(&sRdPkgConfigRes, 0 , sizeof(SRdPkgConfigRes));
        sRdPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
        sRdPkgConfigReq.sHeader.u8WriteLength = 0x05;
        sRdPkgConfigReq.sHeader.u8ReadLength = sizeof(UINT32) + 1;
        sRdPkgConfigReq.u8CmdCode = 0xA1;
        sRdPkgConfigReq.u8HostID_Retry = 0x02;
        sRdPkgConfigReq.u8Index = MBX_INDEX_VCU;
        sRdPkgConfigReq.u16Parameter = US_UCRASH_START;
        if (0 == PECI_RdPkgConfig (&sRdPkgConfigReq, &sRdPkgConfigRes))
        {
            memcpy(&u32NumReads, sRdPkgConfigRes.u8Data, sizeof(UINT32));
        }

        if (sRdPkgConfigRes.u8CompletionCode == PECI_CC_DRIVER_ERROR) {
            // Uncore Crashdump dump sequence failed, abort the sequence and go to the next CPU
            memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
            memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
            sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
            sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
            sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
            sWrPkgConfigReq.u8CmdCode = 0xA5;
            sWrPkgConfigReq.u8HostID_Retry = 0x02;
            sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
            sWrPkgConfigReq.u16Parameter = VCU_ABORT_SEQ;
            u32work = VCU_UNCORE_CRASHDUMP_SEQ;
            memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
	    PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
            PRINT(PRINT_DBG, PRINT_ERROR, "[DBG] - CPU #%d Uncore Crashdump dump Sequence Failed\n", u8Cpu);
            eStatus = ST_HW_FAILURE;
            continue;
        }

        // Get the API version number
        memset(&sRdPkgConfigReq, 0 , sizeof(SRdPkgConfigReq));
        memset(&sRdPkgConfigRes, 0 , sizeof(SRdPkgConfigRes));
        sRdPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
        sRdPkgConfigReq.sHeader.u8WriteLength = 0x05;
        sRdPkgConfigReq.sHeader.u8ReadLength = sizeof(UINT32) + 1;
        sRdPkgConfigReq.u8CmdCode = 0xA1;
        sRdPkgConfigReq.u8HostID_Retry = 0x02;
        sRdPkgConfigReq.u8Index = MBX_INDEX_VCU;
        sRdPkgConfigReq.u16Parameter = VCU_READ;
        if (0 == PECI_RdPkgConfig (&sRdPkgConfigReq, &sRdPkgConfigRes))
        {
            memcpy(&u32ApiVersion, sRdPkgConfigRes.u8Data, sizeof(UINT32));
        }

        if (sRdPkgConfigRes.u8CompletionCode != PECI_CC_SUCCESS &&
	    sRdPkgConfigRes.u8CompletionCode != PECI_CC_MAYBE_SUCCESS && 
	    sRdPkgConfigRes.u8CompletionCode != PECI_CC_CMD_PASSED) {
            // Uncore Crashdump dump sequence failed, abort the sequence and go to the next CPU
            memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
            memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
            sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
            sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
            sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
            sWrPkgConfigReq.u8CmdCode = 0xA5;
            sWrPkgConfigReq.u8HostID_Retry = 0x02;
            sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
            sWrPkgConfigReq.u16Parameter = VCU_ABORT_SEQ;
            u32work = VCU_UNCORE_CRASHDUMP_SEQ;
            memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
	    PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
            PRINT(PRINT_DBG, PRINT_ERROR, "[DBG] - CPU #%d Uncore Crashdump dump Sequence Failed\n", u8Cpu);
            eStatus = ST_HW_FAILURE;
            continue;
        }

        // API version is included in the number of reads, so decrement by one
        if (u32NumReads != 0)
		u32NumReads--;

        // Get the raw data
        UINT32 * pu32UncoreCrashdump = (UINT32 *)calloc(u32NumReads, sizeof(UINT32));
        UINT8 * pu8cc = (UINT8 *)calloc(u32NumReads, sizeof(UINT8));
        
        if (pu32UncoreCrashdump == NULL || pu8cc == NULL) {
            // calloc failed, abort the sequence and go to the next CPU
            memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
            memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
            sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
            sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
            sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
            sWrPkgConfigReq.u8CmdCode = 0xA5;
            sWrPkgConfigReq.u8HostID_Retry = 0x02;
            sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
            sWrPkgConfigReq.u16Parameter = VCU_ABORT_SEQ;
            u32work = VCU_UNCORE_CRASHDUMP_SEQ;
            memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
	    PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
            PRINT(PRINT_DBG, PRINT_ERROR, "[DBG] - CPU #%d Uncore Crashdump dump Sequence Failed\n", u8Cpu);
            eStatus = ST_HW_FAILURE;

	    if (pu32UncoreCrashdump) {
		free(pu32UncoreCrashdump);
		pu32UncoreCrashdump = NULL;
	    } 

	    if (pu8cc) {
		free(pu8cc);
		pu8cc = NULL;
	    }

            continue;
        }
        for (i = 0; i < u32NumReads; i++) {
            memset(&sRdPkgConfigReq, 0 , sizeof(SRdPkgConfigReq));
            memset(&sRdPkgConfigRes, 0 , sizeof(SRdPkgConfigRes));
            sRdPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
            sRdPkgConfigReq.sHeader.u8WriteLength = 0x05;
            sRdPkgConfigReq.sHeader.u8ReadLength = sizeof(UINT32) + 1;
            sRdPkgConfigReq.u8CmdCode = 0xA1;
            sRdPkgConfigReq.u8HostID_Retry = 0x02;
            sRdPkgConfigReq.u8Index = MBX_INDEX_VCU;
            sRdPkgConfigReq.u16Parameter = VCU_READ;
            if (0 == PECI_RdPkgConfig (&sRdPkgConfigReq, &sRdPkgConfigRes))
            {
                memcpy(&pu32UncoreCrashdump[i], sRdPkgConfigRes.u8Data, sizeof(UINT32));
            }
            pu8cc[i] = sRdPkgConfigRes.u8CompletionCode;

	    if (sRdPkgConfigRes.u8CompletionCode != PECI_CC_SUCCESS &&
		sRdPkgConfigRes.u8CompletionCode != PECI_CC_MAYBE_SUCCESS && 
		sRdPkgConfigRes.u8CompletionCode != PECI_CC_CMD_PASSED) {

                // Uncore Crashdump dump sequence failed, note the number of dwords read and abort the sequence
                memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
                memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
                sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
                sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
                sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
                sWrPkgConfigReq.u8CmdCode = 0xA5;
                sWrPkgConfigReq.u8HostID_Retry = 0x02;
                sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
                sWrPkgConfigReq.u16Parameter = VCU_ABORT_SEQ;
                u32work = VCU_UNCORE_CRASHDUMP_SEQ;
                memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
	        PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);
                u32NumReads = i;
                break;
            }
        }

        // Close the Uncore Crashdump dump sequence
        PRINT(PRINT_DBG, PRINT_INFO, "[DBG] - CPU #%d Uncore Crashdump dump Sequence Closed\n", u8Cpu);
        memset(&sWrPkgConfigReq, 0 , sizeof(SWrPkgConfigReq));
        memset(&sWrPkgConfigRes, 0 , sizeof(SWrPkgConfigRes));
        sWrPkgConfigReq.sHeader.u8ClientAddr = PECI_BASE_ADDR + u8Cpu;
        sWrPkgConfigReq.sHeader.u8WriteLength = sizeof(UINT32) + 6;
        sWrPkgConfigReq.sHeader.u8ReadLength = 0x01;
        sWrPkgConfigReq.u8CmdCode = 0xA5;
        sWrPkgConfigReq.u8HostID_Retry = 0x02;
        sWrPkgConfigReq.u8Index = MBX_INDEX_VCU;
        sWrPkgConfigReq.u16Parameter = VCU_CLOSE_SEQ;
        u32work = VCU_UNCORE_CRASHDUMP_SEQ;
        memcpy (sWrPkgConfigReq.u8Data, &u32work, sizeof(UINT32));
	PECI_WrPkgConfig(&sWrPkgConfigReq, &sWrPkgConfigRes);

        // Log the Uncore Crashdump dump for this CPU
#ifdef BUILD_RAW
        if (fpRaw!= NULL) {
            uncoreStatusCrashdumpRaw(u8Cpu, u32NumReads, pu32UncoreCrashdump, fpRaw);
        }
#endif //BUILD_RAW
#ifdef BUILD_JSON
        if (pJsonChild != NULL) {
            uncoreStatusCrashdumpJson(u8Cpu, u32NumReads, pu32UncoreCrashdump, pu8cc, pJsonChild);
        }
#endif //BUILD_JSON

	if (pu32UncoreCrashdump) {
		free(pu32UncoreCrashdump);
		pu32UncoreCrashdump = NULL;
	}

	if (pu8cc) {
		free(pu8cc);
		pu8cc = NULL;
	}
    }

    return eStatus;
}

static UncoreStatusRead UncoreStatusTypes[] = {
    uncoreStatusPci,
    uncoreStatusPciMmio,
    uncoreStatusCbo,
    uncoreStatusIio,
    uncoreStatusCrashdump,
};

/******************************************************************************
*
*   logUncoreStatus
*
*   This function gathers the Uncore Status register contents and adds them to the
*   debug log.
*
******************************************************************************/
ESTATUS logUncoreStatus(FILE * fpRaw, FILE * fp, cJSON * pJsonChild)
{
    ESTATUS eStatus = ST_OK;
    UINT32 i;

#ifdef SPX_BMC_ACD
    UN_USED(fp);
#endif

    for (i = 0; i < (sizeof(UncoreStatusTypes) / sizeof(UncoreStatusTypes[0])); i++) {
        if (UncoreStatusTypes[i](fpRaw, pJsonChild) != ST_OK) {
            eStatus = ST_HW_FAILURE;
        }
    }

    return eStatus;
}
