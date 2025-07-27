CC = gcc
CFLAGS = -std=c99 -g
LDFLAGS = -lm

TARGETS = patience pstatistics

all: $(TARGETS)

patience: patience.o patience_utils.o
pstatistics: pstatistics.o patience_utils.o histogram.o

patience.o: patience.c patience_utils.h
pstatistics.o: pstatistics.c patience_utils.h histogram.h
patience_utils.o: patience_utils.c patience_utils.h
histogram.o: histogram.c histogram.h


clean:
	rm -f $(TARGETS) *.o