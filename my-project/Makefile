PROJECT = synthfirm
BUILD_DIR = bin

CFILES = src/main.c src/dac.c src/timer.c src/osc.c src/music.c src/input.c src/common.c src/processing/foirrhp.c src/processing/folp.c src/processing/envelope.c src/wavetable.c

# TODO - you will need to edit these two lines!
DEVICE=stm32f103c8
OOCD_FILE = board/bluepill_f103c8.cfg

LDLIBS += -lm 

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
