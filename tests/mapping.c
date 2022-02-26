/*
    Value mapping to motors
    Compile with:
        gcc -o mapping -O0 -ggdb mapping.c 
    Run with:
        ./mapping <input_value>
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define INTERCEPT       18433
#define R_SHIFT         9 


int positive_map(int16_t val)
{
	val = val + INTERCEPT;
	val = val >> R_SHIFT;
	return val;
}

int negative_map(int16_t val)
{	
	//convert to positive 
	val = ~val+1;
	val = val + INTERCEPT;
	val = val >> R_SHIFT;
	return val;
}



int main(int argc, char **argv) 
{

    if (argc < 2) {
        printf("No arguments!\n");
        return -1;
    }

    // printf("Input: %s\n", argv[1]);
    int16_t inputVal = (int16_t) strtol(argv[1], NULL, 0);

    printf("Input: %d\t", inputVal);
    printf("Positive map: %d\t", positive_map(inputVal));
    printf("Negative map: %d\n", negative_map(inputVal));


    return 0;
}