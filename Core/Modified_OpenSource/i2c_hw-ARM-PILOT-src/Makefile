DEBUG := n
TARGET := i2c_hw
OBJS := i2c-hardware.o i2c-data.o i2c-module.o i2c-recovery.o i2c-transfer.o i2c-interrupt.o i2c-log.o

EXTRA_CFLAGS += -I${SPXINC}/helper
KBUILD_EXTRA_SYMBOLS += ${SPXINC}/helper/Module.symvers



-include ${TOOLDIR}/rules/Rules.make.drivers
