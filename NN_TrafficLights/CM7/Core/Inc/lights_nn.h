//  Controlling traffic lights at crossroads using a neural network
//
//	Adam Gawlik ~ University of Science and Technology in Kraków
//

#ifndef INC_LIGHTS_NN_H_
#define INC_LIGHTS_NN_H_

#include "main.h"
#include "usart.h"
#include "env_nn.h"
#include "stat_nn.h"
#include "net_nn.h"
#include "print_support.h"
#include "stm32h7xx_it.h"

/* enums */  // needs to be registered inside print_support.h

enum day_of_week{MON, TUE, WED, THU, FRI, SAT, SUN};

enum lights_t{RED, RED_YELLOW, YELLOW, GREEN, DISABLED};

enum lights_phase_t{BOTH_RED_0, RED_YEL_0, GREEN_0, YEL_0, BOTH_RED_1, RED_YEL_1, GREEN_1, YEL_1, INACTIVE};

struct Lights{
		// name of the lights system
	char id[8];

	int current_time; // time during day in seconds
	int current_cycle_time;

	int time_to_action; // time to change lights

	int activated;
		// lights states declaration
	enum lights_t traffic_light_0_s, traffic_light_1_s;
	enum lights_t zebra_light_0_s, zebra_light_1_s;
	enum lights_phase_t current_phase;
	enum day_of_week current_day;
		//constant time values in seconds
	int both_red_time; // default 2
	int r_to_g_yellow_time; // default 2
	int g_to_r_yellow_time; // default 3
	int min_green_time; // default 6
	int green_0_add_time; // default 10
	int green_1_add_time; // default 3
	int zebra_change_time; // default 2
	int zebra_green_blink_time;

		//NEEDS TO BE RECALCULATED BEFORE EACH PHASE WRAP
	int green_0_time; // = min_green_time + green_0_add_time;
	int green_1_time; // = min_green_time + green_1_add_time;
	int green_zebra_0_blink_wait; // = both_red_time + r_to_g_yellow_time + green_0_time + g_to_r_yellow_time
	int green_zebra_1_blink_wait; // = green_zebra_0_blink_time + both_red_time + r_to_g_yellow_time + green_1_time + g_to_r_yellow_time;
	// not used
	// red_1_time would be : r_to_g_yellow + green_0_time + r_to_g_yellow + both_red_time
	// red_0_time would be : r_to_g_yellow + green_1_time + r_to_g_yellow + both_red_value

	int event_on_light[4];
	int direction_traffic[4];
	int left_in_last_cycle[4];

	struct Environment * env_ptr;
	struct Statistic * stat_ptr;
	struct NN * net_ptr;
};

extern struct Lights lights;

	// initiates starting states and values // not hardware!
void lights_init(struct Lights * lig, struct Environment * env, struct Statistic * stat, struct NN * net);
void update_hardware_all(struct Lights * lig);
void update_lights(struct Lights * lig);
void update_lights_FSM(struct Lights * lig);
//void hardware_routine(struct Lights * lig);
void check_num_of_cars(struct Lights * lig);
void end_of_cycle_routine(struct Lights * lig);
void turn_on_lights(struct Lights * lig);
void update_lights_halfsec(struct Lights * lig);
void enter_inactive_lights(struct Lights * lig);
void print_lights(struct Lights * lig);




#endif /* INC_LIGHTS_NN_H_ */



