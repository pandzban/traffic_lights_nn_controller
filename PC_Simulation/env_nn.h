//  Controlling traffic lights at crossroads using a neural network
//
//	Adam Gawlik ~ University of Science and Technology in Kraków
//

#pragma once

#define EVENT_IDLE 90000

#include "lights_nn.h"
#include "stat_nn.h"
#include "main.h"


#define NUM_OF_LIGHTS 4

struct Environment {
	int current_sec;
	float current_base;
	int num_of_arrivals[NUM_OF_LIGHTS];
	int num_of_left[NUM_OF_LIGHTS];
	int arrived_in_last_cycle[NUM_OF_LIGHTS]; // just for debugging


	int event_en;
	int event_start;
	int event_cnt;
	int event_lights;
	float event_scalar;

	int wait_per2;

	struct Lights* lig_ptr;
};

void env_init(struct Environment* env, struct Lights* lig);
void event_init(struct Environment* env);
void end_of_day_routine(struct Environment* env);
void event_routine(struct Environment* env);
void event_eod_routine(struct Environment* env);
void assert_event(struct Environment* env);
void increment_time(struct Environment* env);
void count_leaving(struct Environment* env, int verbosity);
