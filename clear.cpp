#include "joystickskrhabe010.h"
#include "ledmatrixled2472g.h"
// #include "i2copenclose.h"
// #include "humtemphts221.h"
#include <iostream>
// #include "presstemplps25h.h"
// #include smbus.h"

// using namespace std;

int main()
{
	init_LED_LED2472G();
	clear_allLED_LED2472G();
	close_LED_LED2472G();
	
	return 0;
}
