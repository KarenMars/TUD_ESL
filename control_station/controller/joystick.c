/*
    joystick.c
    Joystick functions
    Author: Jure Vidmar
*/
#include <fcntl.h>
#include <unistd.h>

#include "joystick_driver.h"
#include "joystick.h"
#include "controller.h"
#include "packet.h"

extern queue rx_queue;

// Initialization stuff -> JUR: not sure if this is the correct place to define it
unsigned char axes    = 0;
unsigned char buttons = 0;
int version = 0x000800;
char name[NAME_LENGTH] = "Unknown";
int js_fd;


/*
    Parse joystick event and handle actions accordingly
    Implementing -> flight parameter updating, panic mode
*/
void process_js()
{
    
    // Read joystick events into js
    while (read(js_fd, &js, sizeof(struct js_event)) == sizeof(struct js_event))  {

        // Handle button presses
        if (js.type == JS_BTN_EVENT) {
            
            if ((js.number == JS_PANIC_BTN) && (js.value == 1)) {
                change_mode(MODE_PANIC);
                printf("PANIC!\n");
                // TODO in future -> what if packet fails? Do somekind of ACK receiving
            }
            
        }

        // Handle axis moving
        if (js.type == JS_AXIS_EVENT) {

            if (js.number == JS_ROLL_AXIS) {
                joy_params.roll = js.value;
            }

            if (js.number == JS_PITCH_AXIS) {
                joy_params.pitch = js.value;
            }

            if (js.number == JS_YAW_AXIS) {
                joy_params.yaw = js.value;
            }

            if (js.number == JS_LIFT_AXIS) {
                joy_params.lift = -js.value;        // Physical button reversed, that's why the '-'
            }

            // Calc flight params (joy + key) after every change
            update_fp();
        }
    }

}

/*
    Initializes joystick
    Modify parameters in joystick.h:
    @JOYSTICK_PATH  -> path to joystick device file
*/
int init_js()
{

    if ((js_fd = open(JOYSTICK_PATH, O_RDONLY)) < 0) {
    // if(1){
		perror("Joystick initialization failed!");
		return -1;
	}

    fcntl(js_fd, F_SETFL, O_NONBLOCK);

    ioctl(js_fd, JSIOCGVERSION, &version);
	ioctl(js_fd, JSIOCGAXES, &axes);
	ioctl(js_fd, JSIOCGBUTTONS, &buttons);
	ioctl(js_fd, JSIOCGNAME(NAME_LENGTH), name);

	printf("Joystick (%s) has %d axes and %d buttons. Driver version is %d.%d.%d.\n",
		name, axes, buttons, version >> 16, (version >> 8) & 0xff, version & 0xff);

    axis = calloc(axes, sizeof(int));
    button = calloc(buttons, sizeof(char));

    return 0;
}