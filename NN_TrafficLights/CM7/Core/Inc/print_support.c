//  Controlling traffic lights at crossroads using a neural network
//
//	Adam Gawlik ~ University of Science and Technology in Kraków
//

#include "print_support.h"

void strings_init(void){
	strcpy(day_of_week_string[0], "mon");
	strcpy(day_of_week_string[1], "tue");
	strcpy(day_of_week_string[2], "wed");
	strcpy(day_of_week_string[3], "thu");
	strcpy(day_of_week_string[4], "fri");
	strcpy(day_of_week_string[5], "sat");
	strcpy(day_of_week_string[6], "sun");

	strcpy(lights_t_string[0], "RED");
	strcpy(lights_t_string[1], "RED_YELLOW");
	strcpy(lights_t_string[2], "YELLOW");
	strcpy(lights_t_string[3], "GREEN");
	strcpy(lights_t_string[4], "DISABLED");

	strcpy(lights_phase_t_string[0], "BOTH_RED_0");
	strcpy(lights_phase_t_string[1], "RED_YEL_0");
	strcpy(lights_phase_t_string[2], "GREEN_0");
	strcpy(lights_phase_t_string[3], "YEL_0");
	strcpy(lights_phase_t_string[4], "BOTH_RED_1");
	strcpy(lights_phase_t_string[5], "RED_YEL_1");
	strcpy(lights_phase_t_string[6], "GREEN_1");
	strcpy(lights_phase_t_string[7], "YEL_1");
	strcpy(lights_phase_t_string[8], "INACTIVE");
}


