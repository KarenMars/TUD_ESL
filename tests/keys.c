/*
    keys.c
    Testing keyboard
*/

#include <stdio.h>

int main (int argc, char **argv)
{

    int c;
    while ((c = getchar()) != 'q') {
        
        // Arrow key pressed
        if (c == '\033') {
            getchar();     // Skip the meta-character
            c = getchar();

            switch (c) {
            case 'A':
                printf("Arrow up!\n");
                break;
            
            case 'B':
                printf("Arrow down!\n");
                break;

            case 'C':
                printf("Arrow right!\n");
                break;

            case 'D':
                printf("Arrow left!\n");
                break;

            default:
                printf("Unknown button!\n");
                break;
            }
        }
    }

    return 0;
}