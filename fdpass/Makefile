CC=gcc
CFLAGS += -Wall -O0 -g3
srcs := $(shell find . -name \*.c)

.PHONY: all
all: $(srcs:%.c=%)

.PHONY: clean
clean:
	rm -f $(srcs:%.c=%) $(srcs:%.c=%.d)

.PRECIOUS : %.o
%.o : %.c ; $(COMPILE.c) $(OUTPUT_OPTION) $<

