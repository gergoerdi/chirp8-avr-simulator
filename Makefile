target=	runner
cc_srcs = Board SPIRAM LCD
simavr = /home/cactus/prog/arduino/libs

IPATH = .
IPATH += ${simavr}/include/simavr

LIBDIR = $(simavr)/lib
LDFLAGS += -lpthread

all: obj ${target}

include Makefile.common

board = ${OBJ}/${target}.elf

${board} : ${OBJ}/${target}.cc.o ${foreach src, ${cc_srcs}, ${OBJ}/${src}.cc.o}

${target}: ${board}
	@echo $@ done

clean: clean-${OBJ}
#	rm -rf *.a *.axf ${target} *.vcd *.hex
