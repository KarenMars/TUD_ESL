#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{

    // Initialize crude timer
	time_t start = clock();

	int c;
	while (1) {

        // OUTPUT processing
        for (int i=0; i<10000; i++) {}

		// Check joystick
		for (int i=0; i<10000; i++) {}

		// INPUT processing
		for (int i=0; i<10000; i++) {}

		// Send flight packet every second
		if ((double)((clock() - start)/CLOCKS_PER_SEC) >= 1) {
			printf("HELLO!\n");
			start = clock();
		}

	}

    return 0;
}