#
#	Controller makefile
#
BUILD_FOLDER=build
SHARED_FOLDER=../../shared
CC=gcc
CFLAGS=-I$(SHARED_FOLDER)

VPATH = $(SHARED_FOLDER)

controller: controller.o terminal.o joystick.o queue.o packet.o pkt_receiver.o crc.o
	$(CC) $(CFLAGS) -o $(BUILD_FOLDER)/controller $(BUILD_FOLDER)/controller.o $(BUILD_FOLDER)/terminal.o $(BUILD_FOLDER)/joystick.o $(BUILD_FOLDER)/queue.o $(BUILD_FOLDER)/packet.o $(BUILD_FOLDER)/pkt_receiver.o $(BUILD_FOLDER)/crc.o

packet.o: $(SHARED_FOLDER)/packet.c $(SHARED_FOLDER)/packet.h
	$(CC) $(CFLAGS) -o $(BUILD_FOLDER)/$(@) -c $(SHARED_FOLDER)/packet.c

queue.o: $(SHARED_FOLDER)/queue.c $(SHARED_FOLDER)/queue.h
	$(CC) $(CFLAGS) -o $(BUILD_FOLDER)/$(@) -c $(SHARED_FOLDER)/queue.c

controller.o: controller.c controller.h $(SHARED_FOLDER)/packet.h joystick.h
	$(CC) $(CFLAGS) -o $(BUILD_FOLDER)/$(@) -c controller.c

terminal.o: terminal.c terminal.h
	$(CC) $(CFLAGS) -o $(BUILD_FOLDER)/$(@) -c terminal.c

joystick.o: joystick.c controller.h joystick.h joystick_driver.h
	$(CC) $(CFLAGS) -o $(BUILD_FOLDER)/$(@) -c joystick.c

pkt_receiver.o: pkt_receiver.c pkt_receiver.h
	$(CC) $(CFLAGS) -o $(BUILD_FOLDER)/$(@) -c pkt_receiver.c

crc.o: crc.c crc.h
	$(CC) $(CFLAGS) -o $(BUILD_FOLDER)/$(@) -c $<

run: controller
	./$(BUILD_FOLDER)/controller

runall:
	cd ../../drone/main/; make upload-run

.PHONY: clean
clean:
	rm -rf build/*


