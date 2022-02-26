#
#	Makefile for running tests
#
BUILD_FOLDER=build
SHARED_FOLDER=../shared
CC=gcc

CFLAGS=-I$(SHARED_FOLDER) -I../control_station/controller
VPATH = ../shared:../control_station/controller

# WARNING: make sure that the right "packet.o" gets compiled
# TODO: it would be nice if I can compile everything inside a specific folder (shared) + target-specific stuff
OBJECTS = queue.o packet.o pkt_receiver.o terminal.o crc.o
OBJECTS += pkt_receiver_test.o

OBJDIR = build/

pkt_receiver_test: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)


# Object implicit rules
crc.o: crc.h
queue.o: queue.h
packet.o: packet.h
terminal.o: terminal.h
pkt_receiver.o: pkt_receiver.h

runTest: pkt_receiver_test
	./pkt_receiver_test

.PHONY: clean
clean:
	rm *.o pkt_receiver_test

