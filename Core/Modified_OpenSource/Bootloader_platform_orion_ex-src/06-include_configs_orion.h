--- u-boot-2013.07/include/configs/orion.h	1969-12-31 19:00:00.000000000 -0500
+++ mywork/include/configs/orion.h	2015-03-24 06:17:58.404534828 -0400
@@ -0,0 +1,132 @@
+/*-------------------------------------------------------------------------------------------------------------------*/
+/* ----------------------------------------------ORION Configuration -------------------------------------------*/
+/*-------------------------------------------------------------------------------------------------------------------*/
+#ifndef __ORION_H__
+#define __ORION_H__
+
+/*---------------------------------------------------------------
+ *      Board and Processor Identification
+ *---------------------------------------------------------------*/
+#define CONFIG_PILOT4   1 /* Pilot4 SOC */
+
+#include <configs/board.cfg>
+#include <configs/pilot.cfg>
+#include <configs/common.cfg>
+
+/* File Systems to be suported */
+#define CONFIG_FS_FAT   1
+#define CONFIG_CMD_FAT    1
+#define CONFIG_CMD_EXT2   1
+#define CONFIG_CMD_EXT4   1
+#define CONFIG_FS_EXT2    1
+#define CONFIG_FS_EXT4    1
+#define CONFIG_FS_GENERIC 1
+
+#define CONFIG_CMD_LOADB  1	
+
+/*----------------------------------------------------------------
+ *        Override Defaults
+ *---------------------------------------------------------------*/
+#undef DEBUG
+#undef CONFIG_SYS_ALT_MEMTEST
+
+
+/*---------------------------------------------------------------
+ *      Serial Port Configuration
+ *---------------------------------------------------------------*/
+#define   CONFIG_SERIAL2  1/* Use Serial Port 2 as output on HORNET*/ 
+
+/*----------------------------------------------------------------
+ *        Default Memory Test Range
+ *---------------------------------------------------------------*/
+#define CONFIG_SYS_MEMTEST_START  0x80A00000
+#define CONFIG_SYS_MEMTEST_END    0x841FFFFF
+
+/*----------------------------------------------------------------
+ *           Timer Clock Frequency
+ *---------------------------------------------------------------*/
+#define UART_CLOCK         1846100   /* UART CLOCK in  HORNET*/
+#define SYSTEM_CLOCK       200000000 /*System CPU Clock - 200 MHz*/
+#define SYS_CLK            SYSTEM_CLOCK
+#define AHB_CLK            (SYS_CLK/2)
+#define P_CLK              25000000
+//#define CONFIG_SYS_HZ    P_CLK /* 25MHZ = 1sec commented to avoid rollover problems */
+#define CONFIG_SYS_HZ      1000 /* 1KHZ = 1sec to avoid roll over problems */
+
+/*----------------------------------------------------------------
+ *        I2C EEPROM Configuration
+ *---------------------------------------------------------------*/
+#define CONFIG_I2C_CHANNEL_ID			CONFIG_SPX_FEATURE_GLOBAL_UBOOT_ENABLE_I2C_BUS
+#define CONFIG_SYS_EEPROM_ADDR			CONFIG_SPX_FEATURE_I2C_EEPROM_ADDR
+#define	CONFIG_SYS_I2C_EEPROM_ADDR_LEN	CONFIG_SPX_FEATURE_I2C_EEPROM_ADDR_LEN
+
+/*----------------------------------------------------------------
+ *        Network Configuration
+ *---------------------------------------------------------------*/
+#undef CONFIG_MACADDR_IN_EEPROM
+#ifdef CONFIG_SPX_FEATURE_WRITE_MAC_TO_EEPROM_SUPPORT
+#define CONFIG_MACADDR_IN_EEPROM
+#define CONFIG_CMD_EEPROM 				1
+#else
+#define CONFIG_CMD_EEPROM 				0
+#endif
+
+#define CONFIG_PHY_GIGE       1 /* Include GbE speed/duplex detection */
+#define CONFIG_PHY_CMD_DELAY  1
+
+#ifdef CONFIG_SPX_FEATURE_GLOBAL_MAC0_PHYID
+#define CONFIG_PHY_ADDR0      CONFIG_SPX_FEATURE_GLOBAL_MAC0_PHYID
+#else
+#define CONFIG_PHY_ADDR0      0x2
+#endif
+
+#ifdef CONFIG_SPX_FEATURE_GLOBAL_MAC1_PHYID
+#define CONFIG_PHY_ADDR1      CONFIG_SPX_FEATURE_GLOBAL_MAC1_PHYID
+#else
+#define CONFIG_PHY_ADDR1      0x1
+#endif
+
+/*-----------------------------------------------------------------
+ *             Physical Memory Map of SDRAM and Flash
+ *----------------------------------------------------------------*/
+#define CONFIG_BIGPHYSAREA     "4096"
+#define CONFIG_SYS_LOAD_ADDR   0x80A00000
+
+#ifdef CONFIG_OF_DTBFMH
+/* Temporary address to load DTB blob */
+#define CONFIG_FDT_LOAD_ADDR   0x83000000
+/* DTB Relocation addr */
+#define CONFIG_FDT_RELOC_ADDR_STR  "0x81900000"
+#endif
+
+/*-----------------------------------------------------------------
+ * Board Specific Configs
+ *----------------------------------------------------------------*/
+#define CONFIG_ORION
+#define CONFIG_BOARD_NAME               "ORION"
+#define CONFIG_MEMTEST_ENABLE           0
+#define CONFIG_MEMTEST_RESULT           "idle"
+#define CONFIG_BOOT_SELECTOR            1
+#define CONFIG_MOST_RECENTLY_PROG_FW    1
+
+
+/*-----------------------------------------------------------------
+ * YAFU Specific Configs
+ *----------------------------------------------------------------*/
+#define YAFU_IMAGE_UPLOAD_LOCATION    (0x80A00000)
+#define YAFU_TEMP_SECTOR_BUFFER       (0x82A40000) 
+
+/*-----------------------------------------------------------------
+ * ECC Support 
+ *----------------------------------------------------------------*/
+#ifdef CONFIG_SPX_FEATURE_GLOBAL_MEMORY_ECC_ENABLE
+#define CONFIG_DRAM_ECC
+#endif
+
+/*-----------------------------------------------------------------
+ * Flash Operation Mode Mask
+ *----------------------------------------------------------------*/
+#define CONFIG_FLASH_OPERATION_MODE_MASK	0x00310031
+
+#endif
+
