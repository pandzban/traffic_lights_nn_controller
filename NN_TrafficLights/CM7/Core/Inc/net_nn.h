//  Controlling traffic lights at crossroads using a neural network
//
//	Adam Gawlik ~ University of Science and Technology in Kraków
//

#ifndef INC_NET_NN_H_
#define INC_NET_NN_H_

#include "main.h"
#include "usart.h"
#include "rng.h"
#include "stm32h7xx_it.h"
#include "env_nn.h"
#include "lights_nn.h"

#define NUM_OF_INPUTS 119
#define NUM_OF_FIRST 48
#define NUM_OF_MIDDLE 32
#define NUM_OF_OUTPUTS 2

#define RANDOM 1

struct NN{

	float Input[NUM_OF_INPUTS];
	float Between[NUM_OF_FIRST];
	float Between2[NUM_OF_MIDDLE];
	float Output[NUM_OF_OUTPUTS];

	float Correct_Output[NUM_OF_OUTPUTS];

	float first_weights[NUM_OF_FIRST][NUM_OF_INPUTS];
	float middle_weights[NUM_OF_MIDDLE][NUM_OF_FIRST];
	float output_weights[NUM_OF_OUTPUTS][NUM_OF_MIDDLE];

	int num_of_inputs;
	int num_of_first;
	int num_of_middle;
	int num_of_outputs;

	float output_predict[NUM_OF_OUTPUTS];

	float beta;
	float wsp_ucz;

	struct Lights * lig_ptr;
	struct Environment * env_ptr;
	struct Statistic * stat_ptr;

};

extern float first_weights_ref[NUM_OF_FIRST][NUM_OF_INPUTS];
extern float middle_weights_ref[NUM_OF_MIDDLE][NUM_OF_FIRST];
extern float output_weights_ref[NUM_OF_OUTPUTS][NUM_OF_MIDDLE];

void network_init(struct NN * net, struct Statistic * stat, struct Environment * env, struct Lights * lig);
void network_transfer(struct NN * net);
void predict_network_output(struct NN* net);
void print_layers(struct NN * net);
void load_input(struct NN * net);
void teach_network(struct NN * net);
void network_learn(struct NN * net);
void transfer_network(struct NN * net);

#endif /* INC_NET_NN_H_ */
