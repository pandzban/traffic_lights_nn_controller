//  Controlling traffic lights at crossroads using a neural network
//
//	Adam Gawlik ~ University of Science and Technology in Kraków
//

#define SEC_IN_DAY 86400

#include "env_nn.h"

float poly_base[SEC_IN_DAY];

float proportions[4] = { 0.981, 0.9805, 0.9805, 0.980 }; // must be bigger than 0.98, varies about 1.0 to 1.6 {0.98, 0.99, 0.98574, 0.9854}

float poly_coeff[7][7] = { {0.00026881367670067902, -0.01974067562264755771, 0.54752882463043950345, -7.10767764042573713112, 41.86356042310739411505, -70.77297198883485407350, 559.15023646427698622574},
		{0.00025283056621372326, -0.01877119742495133095, 0.52450544614063354665, -6.81506524504588462321, 39.74399417616048424406, -64.46273740642828897762, 558.36991088906756885990},
		{0.00026702450048245120, -0.01958003647223818811, 0.53958300752952315094, -6.90170621123071281744, 39.46819640439692022937, -61.13401617598599102621, 556.79051864996131371299},
		{0.00025586463848696058, -0.01894220281750370882, 0.52833132431179730126, -6.86855333305116744214, 40.29973158511869968379, -67.45032088815284510019, 566.06853227605824940838},
		{0.00021776984610360627, -0.01578631946706315556, 0.43125985372262498618, -5.53523490263135720113, 32.83252339012138065755, -57.94444816524614338960, 570.19301618059523661941},
		{-0.00015317471124564773, 0.01122649016171861115, -0.29976587621563272057, 3.43293552121981271341, -14.90217128203983421031, 19.26696978209445632046, 581.71821405311357011669},
		{-0.00012760196248228154, 0.00957767915645544622, -0.25954747345100342981, 2.94507893540707854640, -11.55804401223477384519, 7.43291550205724149691, 584.90889101931259119738} };


void env_init(struct Environment* env, struct Lights* lig) {
	env->current_sec = lig->current_time; // change the start time
	env->current_base = 0;
	env->lig_ptr = lig;
	env->wait_per2 = 0;
	event_init(env);
	for (int i = 0; i < NUM_OF_LIGHTS; i++) {
		env->num_of_arrivals[i] = 0;
		env->num_of_left[i] = 0;
		env->arrived_in_last_cycle[i] = 0;
	}

}

void event_init(struct Environment* env) {
	env->event_en = 0;
	env->event_cnt = 0;
	env->event_start = EVENT_IDLE;
	env->event_lights = 0;
	env->event_scalar = 1.0;
}

void increment_time(struct Environment* env) {
	float val, temp;
	float val2[NUM_OF_LIGHTS];
	uint32_t tempx[NUM_OF_LIGHTS];
	float randx[NUM_OF_LIGHTS] = { 0.0, 0.0, 0.0, 0.0 };
	env->current_sec++;
	event_routine(env);
	if (env->current_sec == 86400) {
		end_of_day_routine(env);
	}
	int day = env->lig_ptr->current_day;
	for (int i = 0; i < NUM_OF_LIGHTS; i++) {
		tempx[i] = rand();
		randx[i] = (float)tempx[i] / RAND_MAX;
	}
	env->current_base = ((float)env->current_sec / SEC_IN_DAY) * 24.0;
	temp = env->current_base;
	val = poly_coeff[day][0] * powf(temp, 6) + poly_coeff[day][1] * powf(temp, 5) + poly_coeff[day][2] * powf(temp, 4) + poly_coeff[day][3] * powf(temp, 3)
		+ poly_coeff[day][4] * powf(temp, 2) + poly_coeff[day][5] * temp + poly_coeff[day][6];
	val /= 512;
	for (int i = 0; i < NUM_OF_LIGHTS; i++) {
		val2[i] = (val * proportions[i] * randx[i]);
		if ((env->event_en == 1) && (i == env->event_lights)) {
			val2[i] *= env->event_scalar;      // scale while event
		}
		val2[i] = floor(val2[i]);
		env->num_of_arrivals[i] += (int)val2[i];
		env->arrived_in_last_cycle[i] += (int)val2[i];
	}
	count_leaving(env, 0); // set verbosity
	enqueue_cars(env->lig_ptr->stat_ptr);
}

void end_of_day_routine(struct Environment* env) {
	env->current_sec = 0;
	event_eod_routine(env);
}

void event_routine(struct Environment* env) {
	if (env->current_sec == env->event_start) {
		env->event_en = 1;
		env->event_start = EVENT_IDLE;
		assert_event(env);
		if (Verbosity > 0) {
			cout << "EVENT START!" << endl;
		}
	}
	if (env->event_en == 1) {
		if (env->event_cnt == 0) {
			env->event_en = 0;
			if (Verbosity > 0) {
				cout << "EVENT END!" << endl;
			}
			assert_event(env);
			env->event_scalar = 1.0;
		}
		else {
			env->event_cnt--;
		}
	}
}

void event_eod_routine(struct Environment* env) {
	uint32_t tempx;
	int temp;
	tempx = rand();
	temp = tempx % 1000;
	if (temp > 500) {
		if (temp > 750) {
			env->event_scalar = 0.0;
		}
		else {
			env->event_scalar = 0.0;
		}
		tempx = rand();
		temp = tempx % 8000;
		env->event_cnt = temp;
		tempx = rand();
		temp = tempx % 21600;
		temp = temp * (rand() % 4);
		env->event_start = temp;
		tempx = rand();
		temp = tempx % 1000;
		if (temp < 250) {
			env->event_lights = 0;
		}
		else if (temp < 500) {
			env->event_lights = 1;
		}
		else if (temp < 750) {
			env->event_lights = 2;
		}
		else {
			env->event_lights = 3;
		}
		if (Verbosity > 0) {
			cout << "ev_start = " << env->event_start << ", ev_cnt = " << env->event_cnt << ", scal = " << env->event_scalar << ", lig = " << env->event_lights << endl;
		}
	}
	else {
		env->event_start = EVENT_IDLE;
	}


}

void assert_event(struct Environment* env) {
	for (int i = 0; i < 4; i++) {
		env->lig_ptr->event_on_light[i] = 0;
	}
	if (env->event_en == 1) {
		env->lig_ptr->event_on_light[env->event_lights] = 1;
	}
}

void count_leaving(struct Environment* env, int verbosity) {
	if (verbosity != 0) {
		if (Verbosity > 1) {
			cout << "env_nor : {" << env->num_of_arrivals[0] << ", " << env->num_of_arrivals[1] << ", " << env->num_of_arrivals[2] << ", " << env->num_of_arrivals[3] << "}" << endl;
			cout << "env_arriv : {" << env->arrived_in_last_cycle[0] << ", " << env->arrived_in_last_cycle[1] << ", " << env->arrived_in_last_cycle[2] << ", " << env->arrived_in_last_cycle[3] << "}" << endl;
		}
	}
	if (env->lig_ptr->current_phase == GREEN_0) {
		if (env->num_of_arrivals[0] > 0) {
			env->num_of_left[0]++;
			env->num_of_arrivals[0]--;
		}
		if (env->num_of_arrivals[2] > 0) {
			env->num_of_left[2]++;
			env->num_of_arrivals[2]--;
		}
	}
	else if (env->lig_ptr->current_phase == GREEN_1) {
		if (env->num_of_arrivals[1] > 0) {
			env->num_of_left[1]++;
			env->num_of_arrivals[1]--;
		}
		if (env->num_of_arrivals[3] > 0) {
			env->num_of_left[3]++;
			env->num_of_arrivals[3]--;
		}
	}
	else if (env->lig_ptr->current_phase == RED_YEL_0) { // zeroing the left count on new cycle
		for (int i = 0; i < NUM_OF_LIGHTS; i++) {
			env->num_of_left[i] = 0;
		}
	}
	else if (env->lig_ptr->current_phase == BOTH_RED_0) {
		for (int i = 0; i < NUM_OF_LIGHTS; i++) {
			env->arrived_in_last_cycle[i] = 0;
		}
	}
	if (verbosity != 0) {
		if (Verbosity > 1) {
			cout << "env_left : {" << env->num_of_left[0] << ", " << env->num_of_left[1] << ", " << env->num_of_left[2] << ", " << env->num_of_left[3] << ", " << endl;
		}
	}
}







