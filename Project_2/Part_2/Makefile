TARGETS= Part1 Part2

CROSS_TOOL =
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -Werror -pthread

all: clean assemble $(TARGETS) 

$(TARGETS):
		$(CC_C) $(CFLAGS) $@.c -lm -o $@

clean:
		rm -f $(TARGETS)
