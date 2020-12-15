//  Controlling traffic lights at crossroads using a neural network
//
//	Adam Gawlik ~ University of Science and Technology in Kraków
//

#include "lights_nn.h"


struct Lights lights;

void lights_init(struct Lights * lig, struct Environment * env, struct Statistic * stat, struct NN * net){
	int time;
	char buffer[64];   // print i2s error over uart
	int num_of_chars = 0;
	strcpy(lig->id,"light_1");
	lig->env_ptr = env;
	lig->stat_ptr = stat;
	lig->net_ptr = net;
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
	for (int i = 0; i < 4; i++){
		lig->direction_traffic[i] = 0;
		lig->event_on_light[i] = 0;
	}
	env_init(lig->env_ptr, lig);
	stat_init(lig->stat_ptr, lig->env_ptr, lig);
	network_init(lig->net_ptr, lig->stat_ptr, lig->env_ptr, lig);
	update_lights(lig);
	start_time_measure();
	network_transfer(lig->net_ptr);
	time = stop_time_measure();
	if (verbosity > 1){
		num_of_chars = sprintf(buffer, "nn transfer time = %0dms\n", time);
		HAL_UART_Transmit(&huart3, (uint8_t*)buffer, num_of_chars, 80);
	}
	//print_layers(lig->net_ptr);
}

void update_hardware_all(struct Lights * lig){
	if (lig->current_phase == INACTIVE){
		HAL_GPIO_TogglePin(TL1_Y_GPIO_Port, TL1_Y_Pin);
		HAL_GPIO_TogglePin(TL0_Y_GPIO_Port, TL0_Y_Pin);
	} else {
		switch(lig->traffic_light_0_s){
			case RED:
				HAL_GPIO_WritePin(TL0_R_GPIO_Port, TL0_R_Pin, GPIO_PIN_SET); //PIN_RESET or PIN_SET
				HAL_GPIO_WritePin(TL0_Y_GPIO_Port, TL0_Y_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(TL0_G_GPIO_Port, TL0_G_Pin, GPIO_PIN_RESET);
				break;
			case RED_YELLOW:
				HAL_GPIO_WritePin(TL0_R_GPIO_Port, TL0_R_Pin, GPIO_PIN_SET); //PIN_RESET or PIN_SET
				HAL_GPIO_WritePin(TL0_Y_GPIO_Port, TL0_Y_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(TL0_G_GPIO_Port, TL0_G_Pin, GPIO_PIN_RESET);
				break;
			case YELLOW:
				HAL_GPIO_WritePin(TL0_R_GPIO_Port, TL0_R_Pin, GPIO_PIN_RESET); //PIN_RESET or PIN_SET
				HAL_GPIO_WritePin(TL0_Y_GPIO_Port, TL0_Y_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(TL0_G_GPIO_Port, TL0_G_Pin, GPIO_PIN_RESET);
				break;
			case GREEN:
				HAL_GPIO_WritePin(TL0_R_GPIO_Port, TL0_R_Pin, GPIO_PIN_RESET); //PIN_RESET or PIN_SET
				HAL_GPIO_WritePin(TL0_Y_GPIO_Port, TL0_Y_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(TL0_G_GPIO_Port, TL0_G_Pin, GPIO_PIN_SET);
				break;
			default:
				HAL_GPIO_WritePin(TL0_R_GPIO_Port, TL0_R_Pin, GPIO_PIN_RESET); //PIN_RESET or PIN_SET
				HAL_GPIO_WritePin(TL0_Y_GPIO_Port, TL0_Y_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(TL0_G_GPIO_Port, TL0_G_Pin, GPIO_PIN_RESET);
		}

		switch(lig->traffic_light_1_s){
			case RED:
				HAL_GPIO_WritePin(TL1_R_GPIO_Port, TL1_R_Pin, GPIO_PIN_SET); //PIN_RESET or PIN_SET
				HAL_GPIO_WritePin(TL1_Y_GPIO_Port, TL1_Y_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(TL1_G_GPIO_Port, TL1_G_Pin, GPIO_PIN_RESET);
				break;
			case RED_YELLOW:
				HAL_GPIO_WritePin(TL1_R_GPIO_Port, TL1_R_Pin, GPIO_PIN_SET); //PIN_RESET or PIN_SET
				HAL_GPIO_WritePin(TL1_Y_GPIO_Port, TL1_Y_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(TL1_G_GPIO_Port, TL1_G_Pin, GPIO_PIN_RESET);
				break;
			case YELLOW:
				HAL_GPIO_WritePin(TL1_R_GPIO_Port, TL1_R_Pin, GPIO_PIN_RESET); //PIN_RESET or PIN_SET
				HAL_GPIO_WritePin(TL1_Y_GPIO_Port, TL1_Y_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(TL1_G_GPIO_Port, TL1_G_Pin, GPIO_PIN_RESET);
				break;
			case GREEN:
				HAL_GPIO_WritePin(TL1_R_GPIO_Port, TL1_R_Pin, GPIO_PIN_RESET); //PIN_RESET or PIN_SET
				HAL_GPIO_WritePin(TL1_Y_GPIO_Port, TL1_Y_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(TL1_G_GPIO_Port, TL1_G_Pin, GPIO_PIN_SET);
				break;
			default:
				HAL_GPIO_WritePin(TL1_R_GPIO_Port, TL1_R_Pin, GPIO_PIN_RESET); //PIN_RESET or PIN_SET
				HAL_GPIO_WritePin(TL1_Y_GPIO_Port, TL1_Y_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(TL1_G_GPIO_Port, TL1_G_Pin, GPIO_PIN_RESET);
		}

		switch(lig->zebra_light_0_s){
			case RED:
				HAL_GPIO_WritePin(ZL0_R_GPIO_Port, ZL0_R_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(ZL0_G_GPIO_Port, ZL0_G_Pin, GPIO_PIN_RESET);
				break;
			case GREEN:
				HAL_GPIO_WritePin(ZL0_R_GPIO_Port, ZL0_R_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(ZL0_G_GPIO_Port, ZL0_G_Pin, GPIO_PIN_SET);
				break;
			default:
				HAL_GPIO_WritePin(ZL0_R_GPIO_Port, ZL0_R_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(ZL0_G_GPIO_Port, ZL0_G_Pin, GPIO_PIN_RESET);
		}

		switch(lig->zebra_light_1_s){
			case RED:
				HAL_GPIO_WritePin(ZL1_R_GPIO_Port, ZL1_R_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(ZL1_G_GPIO_Port, ZL1_G_Pin, GPIO_PIN_RESET);
				break;
			case GREEN:
				HAL_GPIO_WritePin(ZL1_R_GPIO_Port, ZL1_R_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(ZL1_G_GPIO_Port, ZL1_G_Pin, GPIO_PIN_SET);
				break;
			default:
				HAL_GPIO_WritePin(ZL1_R_GPIO_Port, ZL1_R_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(ZL1_G_GPIO_Port, ZL1_G_Pin, GPIO_PIN_RESET);
		}
	}
}

void update_lights(struct Lights * lig){
	if (lig->time_to_action == 0){
		update_lights_FSM(lig);   // takes 0ms
		update_hardware_all(lig);
		update_stat(lig->stat_ptr);
		if (lig->current_phase != INACTIVE){
			if (verbosity > 0){
				print_lights(lig);  // causes blinking delay
			}
		}
		print_stat(lig->stat_ptr);
	}
	lig->time_to_action--;
	lig->current_time++;
	if (lig->current_time == 86400){
		lig->current_time = 0;
		lig->current_day++;
	}
	lig->current_cycle_time++;

}

void turn_on_lights(struct Lights * lig){
	lig->current_phase = YEL_1;
	lig->time_to_action = 0;
	update_lights(lig);
}

void check_num_of_cars(struct Lights * lig){

	for (int i = 0; i < 4; i++){
		lig->direction_traffic[i] = lig->env_ptr->num_of_arrivals[i];
		lig->left_in_last_cycle[i] = lig->env_ptr->num_of_left[i];
	}

}

void enter_inactive_lights(struct Lights * lig){
	lig->time_to_action = 1;
	lig->traffic_light_1_s = DISABLED;
	lig->zebra_light_0_s = DISABLED;
	lig->traffic_light_0_s = DISABLED;
	lig->zebra_light_1_s = DISABLED;
}

void update_lights_FSM(struct Lights * lig){ // updates mostly only the needed lights <<< take note of that

	switch(lig->current_phase){
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
	}
}

void end_of_cycle_routine(struct Lights * lig){
	int time;
	char buffer[64];   // print i2s error over uart
	int num_of_chars = 0;

	check_num_of_cars(lig);
	calculate_performance(lig->stat_ptr);
	predict_network_output(lig->net_ptr);
	load_input(lig->net_ptr);
	transfer_network(lig->net_ptr);
	print_layers(lig->net_ptr);

	start_time_measure();
	teach_network(lig->net_ptr);
	time = stop_time_measure();
	if (verbosity > 1){
		num_of_chars = sprintf(buffer, "nn learning time = %0dms\n", time);
		HAL_UART_Transmit(&huart3, (uint8_t*)buffer, num_of_chars, 20);
	}
	lig->current_cycle_time = 0;
	lig->green_0_time = lig->min_green_time + lig->green_0_add_time;
	lig->green_1_time = lig->min_green_time + lig->green_1_add_time;
	lig->green_zebra_1_blink_wait = lig->both_red_time + lig->r_to_g_yellow_time + lig->green_0_time
																		   + lig->g_to_r_yellow_time;
	lig->green_zebra_0_blink_wait = lig->green_zebra_1_blink_wait + lig->both_red_time + lig->r_to_g_yellow_time
																  + lig->green_1_time + lig->g_to_r_yellow_time;
}


void update_lights_halfsec(struct Lights * lig){
	if (lig->zebra_light_0_s == GREEN && ((lig->green_zebra_0_blink_wait - lig->current_cycle_time) < lig->zebra_green_blink_time)){
		HAL_GPIO_TogglePin(ZL0_G_GPIO_Port, ZL0_G_Pin);
	}
	if (lig->zebra_light_1_s == GREEN && ((lig->green_zebra_1_blink_wait - lig->current_cycle_time) < lig->zebra_green_blink_time)){
		HAL_GPIO_TogglePin(ZL1_G_GPIO_Port, ZL1_G_Pin);
	}
}

void sec_to_hour(int * hour, int time){
	hour[2] = time / 3600;
	hour[1] = (time % 3600)/60;
	hour[0] = time % 60;
}

void print_lights(struct Lights * lig){
	char buffer[64];   // print i2s error over uart
	int num_of_chars = 0;
	int time[3];
	sec_to_hour(time, lig->current_time);
	num_of_chars = sprintf(buffer, "@ %0dh:%0dm:%0ds, on %s, state of %s is: %s\n", time[2], time[1], time[0], day_of_week_string[lig->current_day],
																					lig->id, lights_phase_t_string[lig->current_phase]);
	HAL_UART_Transmit(&huart3, (uint8_t*)buffer, num_of_chars, 80);
	num_of_chars = sprintf(buffer, "TL0 = %s,\t TL1 = %s,\t ZL0 = %s,\t ZL1 = %s\n", lights_t_string[lig->traffic_light_0_s],
			lights_t_string[lig->traffic_light_1_s], lights_t_string[lig->zebra_light_0_s], lights_t_string[lig->zebra_light_1_s]);
	HAL_UART_Transmit(&huart3, (uint8_t*)buffer, num_of_chars, 80);
	num_of_chars = sprintf(buffer, "green_0_time = %d[s],\t green_1_time = %d[s]\n", lig->green_0_time, lig->green_1_time );
	HAL_UART_Transmit(&huart3, (uint8_t*)buffer, num_of_chars, 80);
	num_of_chars = sprintf(buffer, "zebra_0_blink  = %d[s],\t zebra_1_blink = %d[s]\n", lig->green_zebra_0_blink_wait, lig->green_zebra_1_blink_wait );
	HAL_UART_Transmit(&huart3, (uint8_t*)buffer, num_of_chars, 80);
	num_of_chars = sprintf(buffer, "traffic is : {%d, %d, %d, %d}\n", lig->direction_traffic[0], lig->direction_traffic[1], lig->direction_traffic[2], lig->direction_traffic[3]);
	HAL_UART_Transmit(&huart3, (uint8_t*)buffer, num_of_chars, 80);
	num_of_chars = sprintf(buffer, "left : {%d, %d, %d, %d}\n", lig->left_in_last_cycle[0], lig->left_in_last_cycle[1], lig->left_in_last_cycle[2], lig->left_in_last_cycle[3]);
	HAL_UART_Transmit(&huart3, (uint8_t*)buffer, num_of_chars, 80);
	num_of_chars = sprintf(buffer, "arr in this cycle: {%d, %d, %d, %d}\n", lig->env_ptr->arrived_in_last_cycle[0], lig->env_ptr->arrived_in_last_cycle[1],
															   lig->env_ptr->arrived_in_last_cycle[2], lig->env_ptr->arrived_in_last_cycle[3]);
	HAL_UART_Transmit(&huart3, (uint8_t*)buffer, num_of_chars, 80);
	num_of_chars = sprintf(buffer, "event_on_light: {%d, %d, %d, %d}\n", lig->event_on_light[0], lig->event_on_light[1],
																		 lig->event_on_light[2], lig->event_on_light[3]);
	HAL_UART_Transmit(&huart3, (uint8_t*)buffer, num_of_chars, 80);
	num_of_chars = sprintf(buffer, "current cycle time = %d[s]\n", lig->current_cycle_time );
	HAL_UART_Transmit(&huart3, (uint8_t*)buffer, num_of_chars, 80);
}




















