//  Controlling traffic lights at crossroads using a neural network
//
//	Adam Gawlik ~ University of Science and Technology in Kraków
//

#include "stat_nn.h"

void stat_init(struct Statistic* stat, struct Environment* env, struct Lights* lig) {
	for (int i = 0; i < 4; i++) {
		stat->num_of_arrivals[i] = 0;
		stat->num_of_left[i] = 0;
		stat->arrived_in_last_cycle[i] = 0;
	}
	stat->env_ptr = env;
	stat->lig_ptr = lig;

	for (int i = 0; i < NUM_OF_LIGHTS; i++) {
		for (int k = 0; k < CYCLES_BACK; k++) {
			stat->wait[i][k] = 0;
			stat->lights_duration[i][k] = 0;
		}
		stat->curr[i] = 0;
		stat->left[i] = 0;
		for (int k = 0; k < FIRST_QUEUE; k++) {
			stat->num_of_cars_first[i][k] = 0;
		}
		for (int k = 0; k < SECOND_QUEUE; k++) {
			stat->num_of_cars_second[i][k] = 0;
		}
	}
	for (int k = 0; k < NUM_OF_LIGHTS; k++) {
		for (int i = 0; i < QUEUE_EL; i++) {
			stat->stat_q[k][i] = 0;
		}
		stat->head_q[k] = 0;
		stat->num_of_q[k] = 0;
	}
	stat->counting6 = 0;
	stat->counting5 = 0;
	stat->queues_initialized = 0;
	stat->queues_ready = 0;
}

void enqueue_cars(struct Statistic* stat) {
	if (stat->counting5 == 5) {
		stat->counting5 = 0;
		if (stat->counting6 == 6) {
			stat->counting6 = 0;
			for (int i = 0; i < NUM_OF_LIGHTS; i++) {
				for (int k = SECOND_QUEUE - 1; k > 0; k--) {
					stat->num_of_cars_second[i][k] = stat->num_of_cars_second[i][k - 1];
				}
				stat->num_of_cars_second[i][0] = stat->num_of_cars_first[i][FIRST_QUEUE - 1];
			}
		}
		else {
			stat->counting6++;
		}
		if (stat->queues_initialized < 80) {
			stat->queues_initialized++;
		}
		else if (stat->queues_initialized == 80) {
			stat->queues_initialized = 99;
			stat->queues_ready = 1;
		}
		for (int i = 0; i < NUM_OF_LIGHTS; i++) {
			for (int k = FIRST_QUEUE - 1; k > 0; k--) {
				stat->num_of_cars_first[i][k] = stat->num_of_cars_first[i][k - 1];
			}
			stat->num_of_cars_first[i][0] = stat->env_ptr->num_of_arrivals[i];
		}
		print_net(stat);
	}
	else {
		stat->counting5++;
	}
}

void check_traffic_stat(struct Statistic* stat) {
	for (int i = 0; i < 4; i++) {
		stat->num_of_arrivals[i] = stat->env_ptr->num_of_arrivals[i];
		stat->num_of_left[i] = stat->env_ptr->num_of_left[i];
		stat->arrived_in_last_cycle[i] = stat->env_ptr->arrived_in_last_cycle[i];
	}
}

void calculate_performance(struct Statistic* stat) {
	int curr_prev[NUM_OF_LIGHTS];
	int temp;
	for (int i = 0; i < NUM_OF_LIGHTS; i++) {
		curr_prev[i] = stat->curr[i];
		stat->curr[i] = stat->num_of_arrivals[i];
		stat->left[i] = stat->num_of_left[i];
		for (int k = (CYCLES_BACK - 1); k > 0; k--) {
			stat->lights_duration[i][k] = stat->lights_duration[i][k - 1];
			stat->wait[i][k] = stat->wait[i][k - 1] - stat->left[i];
			if (stat->wait[i][k] < 0) {
				stat->wait[i][k] = 0;
			}
		}
		temp = calculate_length(stat->lig_ptr);
		if (i == 0 || i == 2) {   // multiply by lights length
			//stat->lights_duration[i][0] = stat->lig_ptr->green_1_time + stat->lig_ptr->green_0_time;; // initially green_0_time
			stat->lights_duration[i][0] = 1; // temp;
		}
		else {
			//stat->lights_duration[i][0] = stat->lig_ptr->green_0_time + stat->lig_ptr->green_1_time; // initially green_1_time
			stat->lights_duration[i][0] = 1; // temp;
		}
		stat->wait[i][0] = curr_prev[i] - stat->left[i];
		if (stat->wait[i][0] < 0) {
			stat->wait[i][0] = 0;
		}
		stat->direction_priority[i] = 0;
		for (int k = 0; k < CYCLES_BACK; k++) {
			stat->direction_priority[i] += stat->wait[i][k] * stat->lights_duration[i][0];
		}
		stat->direction_priority[i] += stat->num_of_arrivals[i] * stat->lights_duration[i][0];
		add_to_queue(stat, stat->direction_priority[i], i);
		if (Verbosity > 1) {
			cout << "wait: {" << stat->wait[i][0] << ", " << stat->wait[i][1] << ", " << stat->wait[i][2] << ", " << stat->wait[i][3] << ", " << stat->wait[i][4] << ", " << stat->wait[i][5] << "}" << endl;
			cout << "lig_dur: {" << stat->lights_duration[i][0] << ", " << stat->lights_duration[i][1] << ", " << stat->lights_duration[i][2]
				<< ", " << stat->lights_duration[i][3] << ", " << stat->lights_duration[i][4] << ", " << stat->lights_duration[i][5] << "}" << endl;
			cout << "dir " << i << " priority: " << stat->direction_priority[i] << endl;
			cout << "queue(mean =" << mean_queue(stat, i) << "): ";
			for (int x = 0; x < QUEUE_EL; x++) {
				cout << " " << stat->stat_q[i][x];
			}
			cout << endl;
		}
	}
}


int mean_queue(struct Statistic* stat, int ind) {
	float mean = 0.0;
	int sum = 0;
	int max = stat->num_of_q[ind];
	for (int i = 0; i < max; i++) {
		sum += stat->stat_q[ind][i];
	}
	mean = (float)sum / max;
	return (int)mean;
}


void add_to_queue(struct Statistic* stat, int val, int ind) {
	stat->stat_q[ind][stat->head_q[ind]++] = val;
	stat->head_q[ind] %= QUEUE_EL;
	if (stat->num_of_q[ind] < QUEUE_EL) {
		stat->num_of_q[ind]++;
	}
}

void update_stat(struct Statistic* stat) {
	check_traffic_stat(stat);
}

void print_net(struct Statistic* stat) {
	char buffer[60]; // for printing errors
	int num_of_chars = 0;
	if (Verbosity > 1) {
		cout << "at time = " << stat->env_ptr->current_sec << endl;
		cout << "q_init = " << stat->queues_initialized << ", q_ready = " << stat->queues_ready << endl;
		cout << "counting5 = " << stat->counting5 << endl << "coutning6 = " << stat->counting6 << endl;
		cout << "num_of_cars_first: ";
		for (int i = 0; i < NUM_OF_LIGHTS; i++) {
			cout << "\ndir" << i << ": ";
			for (int k = 0; k < FIRST_QUEUE; k++) {
				cout << stat->num_of_cars_first[i][k] << ", ";
			}
		}
		cout << "\nnum_of_cars_second: ";
		for (int i = 0; i < NUM_OF_LIGHTS; i++) {
			cout << "\ndir" << i << ": ";
			for (int k = 0; k < SECOND_QUEUE; k++) {
				cout << stat->num_of_cars_second[i][k] << ", ";
			}
		}
		cout << endl;
	}
}

void print_stat(struct Statistic* stat) {

	char buffer[60]; // for printing errors
	int num_of_chars = 0;
	if (Verbosity > 0) {
		cout << "stat current traffic: {" << stat->num_of_arrivals[0] << ", " << stat->num_of_arrivals[1] << ", " << stat->num_of_arrivals[2] << ", " << stat->num_of_arrivals[3] << "}" << endl;
		cout << "stat current arrived in last cycle: {" << stat->arrived_in_last_cycle[0] << ", " << stat->arrived_in_last_cycle[1] << ", " << stat->arrived_in_last_cycle[2] << ", " << stat->arrived_in_last_cycle[3] << "}" << endl;
		cout << "stat current left in last cycle: {" << stat->num_of_left[0] << ", " << stat->num_of_left[1] << ", " << stat->num_of_left[2] << ", " << stat->num_of_left[3] << "}" << endl << endl;
	}
}

