/*
    Testing the ae mapping functionality
	
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#define MAX_SPEED 450
#define MIN_SPEED 150
#define R_SHIFT     6  
#define IN_R_SHIFT  7

int16_t joystick_map(int16_t val);
int16_t joystick_map_lift(int16_t val);
void print_ae();

void update_ae(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift);
void map_roll(int16_t val_roll);
void map_pitch(int16_t val_pitch);
void map_yaw(int16_t val_yaw);
void map_lift(int16_t val_lift);


int16_t ae[4];

void map_roll(int16_t val_roll)
{	
	
	//achieve roll by the rpm difference of ae[3] and ae[1]
	uint16_t roll;
	roll = joystick_map(val_roll);

	if ( val_roll>0 )
	{
		ae[1] = ae[1] + roll;
		ae[3] = ae[3] - roll;
	}
	else
	{
		//other direction  
		ae[1] = ae[1] - roll;
		ae[3] = ae[3] + roll;
	}
	

	// printf("roll: ae[1]=%d, ae[3]=%d\n", ae[1],ae[3]);

}


void map_pitch(int16_t val_pitch)
{	
	
	//achieve pitch by the rpm difference of ae[0] and ae[2] 
	uint16_t pitch;
	pitch = joystick_map(val_pitch); 

    // printf("Pitch value: %d\n", pitch);

	if ( val_pitch > 0 )
	{
		ae[0] = ae[0] + pitch;
		ae[2] = ae[2] - pitch;
        printf("pitch: ae[0]=%d, ae[2]=%d\n", ae[0],ae[2]);
	}
	else
	{
		//other direction 
		ae[0] = ae[0] - pitch;
		ae[2] = ae[2] + pitch;
	}
	
	// printf("pitch: ae[0]=%d, ae[2]=%d\n", ae[0],ae[2]);

}


void map_yaw(int16_t val_yaw)
{	
	
	//achieve yaw by the rpm difference of ae[3] and ae[1] & ae[0] and ae[2]
	uint16_t yaw;
	
	yaw = joystick_map(val_yaw);

    printf("Before shift: %d\n", yaw);
	yaw = yaw >> 1;
    printf("After shift: %d\n", yaw);

	if ( val_yaw > 0 )
	{
		ae[3] = ae[3] + yaw;
		ae[1] = ae[1] + yaw;

		ae[2] = ae[2] - yaw;
		ae[0] = ae[0] - yaw;

        // printf("yaw: ae[0]=%d, ae[1]=%d, ae[2]=%d, ae[3]=%d\n",ae[0],ae[1],ae[2],ae[3]);

	}
	else
	{
		//another direction 
		ae[3] = ae[3] - yaw;
		ae[1] = ae[1] - yaw;

		ae[2] = ae[2] + yaw;
		ae[0] = ae[0] + yaw;

	}
		
	// printf("yaw: ae[0]=%d, ae[1]=%d, ae[2]=%d, ae[3]=%d\n",ae[0],ae[1],ae[2],ae[3]);

}


void map_lift(int16_t val_lift)
{	
	//achieve lift by changing the rpm of ae[3] and ae[1] & ae[0] and ae[2]
	uint16_t lift;
	lift = joystick_map_lift(val_lift);
	ae[0] = lift;
	ae[1] = lift;
	ae[2] = lift;
	ae[3] = lift;
	printf("lift: ae[0]=%d, ae[1]=%d, ae[2]=%d, ae[3]=%d\n",ae[0],ae[1],ae[2],ae[3]);
}

/*------------------------------------------------------------------
 *  mapping function 
 *  change 0~32767 to 0~128 
 *------------------------------------------------------------------
 */
int16_t joystick_map(int16_t val)
{

	if ( val > 0 )
	{
		val = val>>R_SHIFT;
		return val;
	}
	else 
	{
		val = ~val+1;
		val = val>>R_SHIFT;
		return val;
	}
}


int16_t joystick_map_lift(int16_t val)
{
	int32_t newVal = val + INT16_MAX + 1;		// +1 bcs negative range is 1 bigger than positive in int16_t
	newVal = newVal >> 7;

	return newVal;
}


void update_ae(int16_t roll, int16_t pitch, int16_t yaw, int16_t lift)
{
    map_lift(lift);         // This needs to be set first!
    map_roll(roll);
    map_pitch(pitch);
    map_yaw(yaw);
}

void print_ae()
{
    printf("ae values: \t%d\t%d\t%d\t%d\n\n", ae[0], ae[1], ae[2], ae[3]);
}

int main (int argc, char **argv)
{

    // Initialize ae
    for (int i=0; i<4; i++) {
        ae[i] = 0;
    }

	update_ae(0, 0, 0, INT16_MIN);
	update_ae(0, 0, 0, -3000);
	update_ae(0, 0, 0, 0);
	update_ae(0, 0, 0, 3000);
	update_ae(0, 0, 0, INT16_MAX);



    return 0;
}