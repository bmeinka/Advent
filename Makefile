llC = clang
CFLAGS = --std=c11 -Wall -Wextra -Wpedantic

years =  $(shell seq -w 2015 2020)
days := $(shell seq -w 25)
puzzles = $(foreach y,$(years),$(addprefix bin/$(y)/,$(days)))

bin_dir = $(addprefix bin/,$(years))

# optionally set the default target in target.mk
include target.mk

all: $(puzzles)

$(puzzles): bin/%:%.o | $(bin_dir)
	@echo $(days)
	$(CC) -o $@ $^

$(bin_dir):
	-mkdir -p $@

compile_flags.txt:
	echo $(CFLAGS) | tr " " "\n" > $@

.PHONY: clean
clean:
	-rm -f */*.o
	-rm -rf bin
