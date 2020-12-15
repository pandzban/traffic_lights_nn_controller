//  Controlling traffic lights at crossroads using a neural network
//
//	Adam Gawlik ~ University of Science and Technology in Kraków
//

#ifndef INC_STAT_NN_H_
#define INC_STAT_NN_H_

#include "main.h"
#include "usart.h"
#include "stm32h7xx_it.h"
#include "env_nn.h"
#include "lights_nn.h"


#define NUM_OF_LIGHTS 4
#define CYCLES_BACK 6
#define QUEUE_EL 96
#define FIRST_QUEUE 16
#define SECOND_QUEUE 8


struct Statistic{


	int num_of_arrivals[NUM_OF_LIGHTS];
	int num_of_left[NUM_OF_LIGHTS];
	int arrived_in_last_cycle[NUM_OF_LIGHTS]; // just for debugging

	struct Environment * env_ptr;
	struct Lights * lig_ptr;


	// variables used for performacne calculation
	int wait[NUM_OF_LIGHTS][CYCLES_BACK];
	int curr[NUM_OF_LIGHTS];
	int left[NUM_OF_LIGHTS];
	int lights_duration[NUM_OF_LIGHTS][CYCLES_BACK];
	int direction_priority[NUM_OF_LIGHTS];

	int num_of_cars_first[NUM_OF_LIGHTS][FIRST_QUEUE];
	int num_of_cars_second[NUM_OF_LIGHTS][SECOND_QUEUE];
	int counting6;
	int counting5;
	int queues_initialized;
	int queues_ready;

	int stat_q[NUM_OF_LIGHTS][QUEUE_EL];
	int head_q[NUM_OF_LIGHTS];
	int num_of_q[NUM_OF_LIGHTS];

};

extern struct Statistic stat;

void stat_init(struct Statistic * stat, struct Environment * env, struct Lights * lig);
void check_traffic_stat(struct Statistic * stat);
void update_stat(struct Statistic * stat);
void print_net(struct Statistic * stat);
void enqueue_cars(struct Statistic * stat);
void print_stat(struct Statistic * stat);
void calculate_performance(struct Statistic * stat);
void add_to_queue(struct Statistic * stat, int val, int ind);
int mean_queue(struct Statistic * stat, int ind);

#endif /* INC_STAT_NN_H_ */
