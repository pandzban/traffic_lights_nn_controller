//  Controlling traffic lights at crossroads using a neural network
//
//	Adam Gawlik ~ University of Science and Technology in Kraków
//

#include "lights_nn.h"


struct Lights lights;

void lights_init(struct Lights* lig, struct Environment* env, struct Statistic* stat, struct NN* net) {
	int time;
	char buffer[64];   // print i2s error over uart
	int num_of_chars = 0;
	strcpy(lig->id, "light_1");
	lig->env_ptr = env;
	lig->stat_ptr = stat;
	lig->net_ptr = net;
	strings_init(lig);
	lig->current_phase = INACTIVE;
	lig->current_day = MON;
	lig->current_time = 54000;
	lig->current_cycle_time = 0;
	lig->zebra_green_blink_time = 4;
	lig->both_red_time = 2; // default 2
	lig->r_to_g_yellow_time = 2; // default 2
	lig->g_to_r_yellow_time = 3; // default 3
	lig->min_green_time = 4; // default 6
	lig->green_0_add_time = 8; // default 3
	lig->green_1_add_time = 9; // default 10
	lig->zebra_change_time = 2; // default 2
	lig->time_to_action = 0;//lig->both_red_time;
	lig->activated = 0;
	lig->traffic_light_0_s = DISABLED;
	lig->traffic_light_1_s = DISABLED;
	lig->zebra_light_0_s = DISABLED;
	lig->zebra_light_1_s = DISABLED;
	lig->green_0_time = lig->min_green_time + lig->green_0_add_time;
	lig->green_1_time = lig->min_green_time + lig->green_1_add_time;
	lig->green_zebra_0_blink_wait = lig->both_red_time + lig->r_to_g_yellow_time + lig->green_0_time
		+ lig->g_to_r_yellow_time;
	lig->green_zebra_1_blink_wait = lig->green_zebra_0_blink_wait + lig->both_red_time + lig->r_to_g_yellow_time
		+ lig->green_1_time + lig->g_to_r_yellow_time;
	for (int i = 0; i < 4; i++) {
		lig->direction_traffic[i] = 0;
		lig->event_on_light[i] = 0;
	}
	env_init(lig->env_ptr, lig);
	stat_init(lig->stat_ptr, lig->env_ptr, lig);
	network_init(lig->net_ptr, lig->stat_ptr, lig->env_ptr, lig);
	update_lights(lig);
	network_transfer(lig->net_ptr);
}

int calculate_length(struct Lights* lig) {
	return (lig->both_red_time * 2) + (2 * lig->r_to_g_yellow_time) + lig->green_0_time + lig->green_1_time + (2 * lig->g_to_r_yellow_time);
}

void strings_init(struct Lights* lig) {
	strcpy(lig->day_of_week_string[0], "mon");
	strcpy(lig->day_of_week_string[1], "tue");
	strcpy(lig->day_of_week_string[2], "wed");
	strcpy(lig->day_of_week_string[3], "thu");
	strcpy(lig->day_of_week_string[4], "fri");
	strcpy(lig->day_of_week_string[5], "sat");
	strcpy(lig->day_of_week_string[6], "sun");

	strcpy(lig->lights_t_string[0], "RED");
	strcpy(lig->lights_t_string[1], "RED_YELLOW");
	strcpy(lig->lights_t_string[2], "YELLOW");
	strcpy(lig->lights_t_string[3], "GREEN");
	strcpy(lig->lights_t_string[4], "DISABLED");

	strcpy(lig->lights_phase_t_string[0], "BOTH_RED_0");
	strcpy(lig->lights_phase_t_string[1], "RED_YEL_0");
	strcpy(lig->lights_phase_t_string[2], "GREEN_0");
	strcpy(lig->lights_phase_t_string[3], "YEL_0");
	strcpy(lig->lights_phase_t_string[4], "BOTH_RED_1");
	strcpy(lig->lights_phase_t_string[5], "RED_YEL_1");
	strcpy(lig->lights_phase_t_string[6], "GREEN_1");
	strcpy(lig->lights_phase_t_string[7], "YEL_1");
	strcpy(lig->lights_phase_t_string[8], "INACTIVE");
}

void update_lights(struct Lights* lig) {
	if (lig->time_to_action == 0) {
		update_lights_FSM(lig);   // takes 0ms
		update_stat(lig->stat_ptr);
		if (lig->current_phase != INACTIVE) {
			print_lights(lig);  // causes blinking delay
		}
		print_stat(lig->stat_ptr);
	}
	lig->time_to_action--;
	lig->current_time++;
	if ((lig->current_time % 3600) == 0) {
		new_hour_routine(lig);
	}
	if (lig->current_time == 86400) {
		hour_counter = 0;
		lig->current_time = 0;
		if (change_day) {
			change_state(lig);
		}
		new_day_routine(lig);
	}
	lig->current_cycle_time++;

}

void new_hour_routine(struct Lights* lig) {
	new_hour = true;
	hour_counter++;
}

void new_day_routine(struct Lights* lig) {
	day_counter++;
	new_day = true;
	/*priority_queue.push_back(tuple<int, int, int, int>(21, 20, 19));*/
}

void change_state(struct Lights* lig) {
	switch (lig->current_day) {
	case MON:
		lig->current_day = TUE;
		break;
	case TUE:
		lig->current_day = WED;
		break;
	case WED:
		lig->current_day = THU;
		break;
	case THU:
		lig->current_day = FRI;
		break;
	case FRI:
		lig->current_day = SAT;
		break;
	case SAT:
		lig->current_day = SUN;
		break;
	case SUN:
		lig->current_day = MON;
		break;
	}
}



void turn_on_lights(struct Lights* lig) {
	lig->current_phase = YEL_1;
	lig->time_to_action = 0;
	update_lights(lig);
}

void check_num_of_cars(struct Lights* lig) {

	for (int i = 0; i < 4; i++) {
		lig->direction_traffic[i] = lig->env_ptr->num_of_arrivals[i];
		lig->left_in_last_cycle[i] = lig->env_ptr->num_of_left[i];
	}

}

void enter_inactive_lights(struct Lights* lig) {
	lig->time_to_action = 1;
	lig->traffic_light_1_s = DISABLED;
	lig->zebra_light_0_s = DISABLED;
	lig->traffic_light_0_s = DISABLED;
	lig->zebra_light_1_s = DISABLED;
}

void update_lights_FSM(struct Lights* lig) { // updates mostly only the needed lights <<< take note of that

	switch (lig->current_phase) {
	case BOTH_RED_0:
		lig->current_phase = RED_YEL_0;
		lig->time_to_action = lig->r_to_g_yellow_time;
		lig->traffic_light_0_s = RED_YELLOW;
		lig->traffic_light_1_s = RED;
		lig->zebra_light_1_s = GREEN;
		lig->zebra_light_0_s = RED;
		break;
	case RED_YEL_0:
		lig->current_phase = GREEN_0;
		lig->time_to_action = lig->green_0_time;
		lig->traffic_light_0_s = GREEN;
		lig->traffic_light_1_s = RED;//
		//lig->zebra_light_1_s = GREEN;//
		//lig->zebra_light_0_s = RED;//
		break;
	case GREEN_0:
		lig->current_phase = YEL_0;
		lig->time_to_action = lig->g_to_r_yellow_time;
		lig->traffic_light_0_s = YELLOW;
		lig->traffic_light_1_s = RED;//
		//lig->zebra_light_1_s = GREEN;//
		//lig->zebra_light_0_s = RED;//
		break;
	case YEL_0:
		lig->current_phase = BOTH_RED_1;
		lig->time_to_action = lig->both_red_time;
		lig->traffic_light_0_s = RED;
		lig->traffic_light_1_s = RED;//
		//lig->zebra_light_0_s = RED;//
		lig->zebra_light_1_s = RED;
		break;
	case BOTH_RED_1:
		lig->current_phase = RED_YEL_1;
		lig->time_to_action = lig->r_to_g_yellow_time;
		lig->traffic_light_1_s = RED_YELLOW;
		lig->zebra_light_0_s = GREEN;
		lig->traffic_light_0_s = RED;//
		//lig->zebra_light_1_s = RED;//

		break;
	case RED_YEL_1:
		lig->current_phase = GREEN_1;
		lig->time_to_action = lig->green_1_time;
		lig->traffic_light_1_s = GREEN;
		//lig->zebra_light_0_s = GREEN;//
		lig->traffic_light_0_s = RED;//
		//lig->zebra_light_1_s = RED;//
		break;
	case GREEN_1:
		lig->current_phase = YEL_1;
		lig->time_to_action = lig->g_to_r_yellow_time;
		lig->traffic_light_1_s = YELLOW;
		//lig->zebra_light_0_s = GREEN;//
		//lig->traffic_light_0_s = RED;//
		//lig->zebra_light_1_s = RED;///
		break;
	case YEL_1:
		lig->current_phase = BOTH_RED_0;
		end_of_cycle_routine(lig);
		lig->time_to_action = lig->both_red_time;
		lig->traffic_light_1_s = RED;
		lig->zebra_light_0_s = RED;
		lig->traffic_light_0_s = RED;//
		lig->zebra_light_1_s = RED;//
		break;
	case INACTIVE:
		lig->time_to_action = 1;
		lig->traffic_light_0_s = DISABLED;//
		lig->traffic_light_1_s = DISABLED;//
		lig->zebra_light_0_s = DISABLED;
		lig->zebra_light_1_s = DISABLED;
		//		default:
		//			lig->current_phase = BOTH_RED_0;
		//			end_of_cycle_routine(lig);
		//			lig->time_to_action = lig->both_red_time;
		//			lig->traffic_light_0_s = RED;
		//			lig->zebra_light_1_s = RED;
	}

}

void end_of_cycle_routine(struct Lights* lig) {
	int time;

	check_num_of_cars(lig);
	calculate_performance(lig->stat_ptr);
	enqueue_performance(lig->stat_ptr);
	predict_network_output(lig->net_ptr);
	load_input(lig->net_ptr);
	transfer_network(lig->net_ptr);
	enqueue_output(lig->net_ptr);
	print_layers(lig->net_ptr);
 
	teach_network(lig->net_ptr);

	lig->current_cycle_time = 0;
	lig->green_0_time = lig->min_green_time + lig->green_0_add_time;
	lig->green_1_time = lig->min_green_time + lig->green_1_add_time;
	lig->green_zebra_1_blink_wait = lig->both_red_time + lig->r_to_g_yellow_time + lig->green_0_time
		+ lig->g_to_r_yellow_time;
	lig->green_zebra_0_blink_wait = lig->green_zebra_1_blink_wait + lig->both_red_time + lig->r_to_g_yellow_time
		+ lig->green_1_time + lig->g_to_r_yellow_time;
}

void sec_to_hour(int* hour, int time) {
	hour[2] = time / 3600;
	hour[1] = (time % 3600) / 60;
	hour[0] = time % 60;
}

void print_lights(struct Lights* lig) {
	int time[3];
	sec_to_hour(time, lig->current_time);
	if (Verbosity > 0) {
		cout << "@ " << time[2] << "h:" << time[1] << "m:" << time[0] << "s, on " << lig->day_of_week_string[lig->current_day] << ", state of " << lig->id << " is: " << lig->lights_phase_t_string[lig->current_phase] << endl;
		cout << "TL0 = " << lig->lights_t_string[lig->traffic_light_0_s] << "\t TL1 = " << lig->lights_t_string[lig->traffic_light_1_s] << "\t ZL0 = " << lig->lights_t_string[lig->zebra_light_0_s] << "\t ZL1 = " << lig->lights_t_string[lig->zebra_light_1_s] << endl;
		cout << "green_0_time = " << lig->green_0_time << "[s], \t green_1_time = " << lig->green_1_time << "[s]" << endl;
		cout << "traffic is : {" << lig->direction_traffic[0] << ", " << lig->direction_traffic[1] << ", " << lig->direction_traffic[2] << ", " << lig->direction_traffic[3] << "}" << endl;
		cout << "left: " << lig->left_in_last_cycle[0] << ", " << lig->left_in_last_cycle[1] << ", " << lig->left_in_last_cycle[2] << ", " << lig->left_in_last_cycle[3] << "}" << endl;
		cout << "arr in this cycle: {" << lig->env_ptr->arrived_in_last_cycle[0] << ", " << lig->env_ptr->arrived_in_last_cycle[1] << ", " << lig->env_ptr->arrived_in_last_cycle[2] << ", " << lig->env_ptr->arrived_in_last_cycle[3] << "}" << endl;
		cout << "event_on_light: {" << lig->event_on_light[0] << ", " << lig->event_on_light[1] << ", " << lig->event_on_light[2] << ", " << lig->event_on_light[3] << endl;
		cout << "current cycle time = " << lig->current_cycle_time << "[s]" << endl;
	}
}
