//  Controlling traffic lights at crossroads using a neural network
//
//	Adam Gawlik ~ University of Science and Technology in Kraków
//

#include "net_nn.h"

void network_init(struct NN* net, struct Statistic* stat, struct Environment* env, struct Lights* lig) {
	uint32_t randx;
	float randf;

	for (int i = 0; i < NUM_OF_INPUTS; i++) {
		net->Input[i] = 0.0;
	}
	for (int i = 0; i < NUM_OF_FIRST; i++) {
		net->Between[i] = 0.0;
	}
	for (int i = 0; i < NUM_OF_MIDDLE; i++) {
		net->Between2[i] = 0.0;
	}
	for (int i = 0; i < NUM_OF_OUTPUTS; i++) {
		net->Output[i] = 0.0;
		net->Correct_Output[i] = 0.0;
		net->output_predict[i] = 0.0;
	}

	for (int i = 0; i < NUM_OF_FIRST; i++) {
		for (int j = 0; j < NUM_OF_INPUTS; j++) {
			randx = rand();
			randf = (float)(randx % 1000) - 500.0;
			randf *= 0.0001;
			net->first_weights[i][j] = randf;
		}
	}

	for (int i = 0; i < NUM_OF_MIDDLE; i++) {
		for (int j = 0; j < NUM_OF_FIRST; j++) {
			randx = rand();
			randf = (float)(randx % 1000) - 500.0;
			randf *= 0.0001;
			net->middle_weights[i][j] = randf;
		}
	}

	for (int i = 0; i < NUM_OF_OUTPUTS; i++) {
		for (int j = 0; j < NUM_OF_MIDDLE; j++) {
			randx = rand();
			randf = (float)(randx % 1000) - 500.0;
			randf *= 0.0001;
			net->output_weights[i][j] = randf;
		}
	}

	net->num_of_inputs = NUM_OF_INPUTS;
	net->num_of_first = NUM_OF_FIRST;
	net->num_of_middle = NUM_OF_MIDDLE;
	net->num_of_outputs = NUM_OF_OUTPUTS;

	net->lig_ptr = lig;
	net->stat_ptr = stat;

	net->wsp_ucz = 0.015; // 0.05 max 0.2 // 0.025
	net->beta = 1.0;
}

void teach_network(struct NN* net) {
	if (net->stat_ptr->queues_ready == 1) {
		if (teaching_en) {
			network_learn(net);
		}
	}
}

void network_learn(struct NN* net) {	// use after transfer and output predict
	float weights_modified3[NUM_OF_FIRST][NUM_OF_INPUTS];
	float weights_modified2[NUM_OF_MIDDLE][NUM_OF_FIRST];
	float weights_modified1[NUM_OF_OUTPUTS][NUM_OF_MIDDLE];
	float Err[NUM_OF_OUTPUTS];
	float Err2[NUM_OF_MIDDLE];
	float temp, D;
	float delta = net->wsp_ucz;
	for (int g = 0; g < NUM_OF_OUTPUTS; g++) {
		temp = net->Output[g];
		D = net->output_predict[g];
		D = D - temp;
		Err[g] = D;
		D = D * (1 - temp) * temp;
		D = delta * D;
		for (int i = 0; i < NUM_OF_MIDDLE; i++) {
			weights_modified1[g][i] = net->Between2[i] * D;
		}
	}

	for (int g = 0; g < NUM_OF_MIDDLE; g++) {
		temp = net->Between2[g];
		D = 0.0;
		for (int m = 0; m < NUM_OF_OUTPUTS; m++) {
			D += net->output_weights[m][g] * Err[m];
		}
		Err2[g] = D;
		D = D * (1 - temp) * temp;
		D = delta * D;
		for (int i = 0; i < NUM_OF_FIRST; i++) {
			weights_modified2[g][i] = net->Between[i] * D;
		}
	}

	for (int g = 0; g < NUM_OF_FIRST; g++) {
		temp = net->Between[g];
		D = 0.0;
		for (int m = 0; m < NUM_OF_MIDDLE; m++) {
			D += net->middle_weights[m][g] * Err2[m];
		}
		D = D * (1 - temp) * temp;
		D = delta * D;
		for (int i = 0; i < NUM_OF_INPUTS; i++) {
			weights_modified3[g][i] = net->Input[i] * D;
		}
	}
	// update weights
	for (int i = 0; i < NUM_OF_OUTPUTS; i++) {
		if (Verbosity > 1) {
			cout << endl << "modified weights [" << i << "]: ";
		}
		for (int k = 0; k < NUM_OF_MIDDLE; k++) {
			net->output_weights[i][k] += weights_modified1[i][k];
			if (Verbosity > 1) {
				cout << weights_modified1[i][k] << ", ";
			}
		}
	}
	if (Verbosity > 1) {
		cout << endl;
	}
	for (int i = 0; i < NUM_OF_MIDDLE; i++) {
		for (int k = 0; k < NUM_OF_FIRST; k++) {
			net->middle_weights[i][k] += weights_modified2[i][k];
		}
	}

	for (int i = 0; i < NUM_OF_FIRST; i++) {
		for (int k = 0; k < NUM_OF_INPUTS; k++) {
			net->first_weights[i][k] += weights_modified3[i][k];
		}
	}

}


void load_input(struct NN* net) {
	if (net->stat_ptr->queues_ready == 1) {
		float temp;
		int tempi;
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 16; k++) {
				temp = ((float)(net->stat_ptr->num_of_cars_first[i][k] - 30)) / 60.0;
				if (temp < -0.5) {
					temp = -0.5;
				}
				else if (temp > 0.5) {
					temp = 0.5;
				}
				net->Input[k + (i * 24)] = temp;
			}
			for (int k = 16; k < 24; k++) {
				temp = ((float)(net->stat_ptr->num_of_cars_second[i][k - 16] - 30)) / 60.0;
				if (temp < -0.5) {
					temp = -0.5;
				}
				else if (temp > 0.5) {
					temp = 0.5;
				}
				net->Input[k + (i * 24)] = temp;
			}
		}
		for (int i = 0; i < 7; i++) {
			net->Input[i + 96] = -0.5;
		}
		tempi = net->lig_ptr->current_day;
		net->Input[tempi + 96] = 0.5;
		tempi = net->lig_ptr->current_time;
		for (int i = 0; i < 12; i++) {
			net->Input[103 + i] = -0.5;
			if ((tempi >= (i * 7200)) && (tempi < ((i + 1) * 7200))) {
				net->Input[103 + i] = 0.5;
			}
		}
		for (int i = 0; i < 4; i++) {
			if (net->lig_ptr->event_on_light[i] == 1) {
				net->Input[115 + i] = 0.5;
			}
			else {
				net->Input[115 + i] = -0.5;
			}
		}
	}

}

void transfer_network(struct NN* net) {
	float temp;
	network_transfer(net);
	temp = net->Output[0];
	if (temp > 1.0) {
		temp = 1.0;
	}
	else if (temp < 0.0) {
		temp = 0.0;
	}
	net->lig_ptr->green_0_add_time = (int)(temp * time_scale);
	if (Verbosity > 0) {
		cout << "new green add time 0: " << net->lig_ptr->green_0_add_time << ", temp = " << temp << endl;
	}
	temp = net->Output[1];
	if (temp > 1.0) {
		temp = 1.0;
	}
	else if (temp < 0.0) {
		temp = 0.0;
	}
	net->lig_ptr->green_1_add_time = (int)(temp * time_scale);
	if (Verbosity > 0) {
		cout << "new green add time 1: " << net->lig_ptr->green_1_add_time << ", temp = " << temp << endl;
	}
}


void network_transfer(struct NN* net) {
	float temp;
	float beta = net->beta;
	for (int k = 0; k < NUM_OF_FIRST; k++) {
		temp = 0.0;
		for (int i = 0; i < NUM_OF_INPUTS; i++) {
			temp += net->first_weights[k][i] * net->Input[i];
		}
		net->Between[k] = (1.0 / (1.0 + expf((-1.0) * (beta) * (temp))));
	}

	for (int k = 0; k < NUM_OF_MIDDLE; k++) {
		temp = 0.0;
		for (int i = 0; i < NUM_OF_FIRST; i++) {
			temp += net->middle_weights[k][i] * net->Between[i];
		}

		net->Between2[k] = (1.0 / (1.0 + expf((-1.0) * (beta) * (temp))));
	}

	for (int k = 0; k < NUM_OF_OUTPUTS; k++) {
		temp = 0.0;
		for (int i = 0; i < NUM_OF_MIDDLE; i++) {
			temp += net->output_weights[k][i] * net->Between2[i];
		}
		net->Output[k] = (1.0 / (1.0 + expf((-1.0) * (beta) * (temp))));
	}
}

void predict_network_output(struct NN* net) {
	int output[2];
	for (int i = 0; i < 2; i++) {
		output[i] = net->stat_ptr->direction_priority[i] + net->stat_ptr->direction_priority[i + 2];
		net->output_predict[i] = ((float)output[i]) / 1000.0;
	}
	if ((net->output_predict[0] - net->output_predict[1]) > 0.05) { // 0.3
		net->output_predict[0] = 1.0;
		net->output_predict[1] = 0.0;
	}
	else if ((net->output_predict[1] - net->output_predict[0]) > 0.05) {
		net->output_predict[1] = 1.0;
		net->output_predict[0] = 0.0;
	}
	else {
		for (int i = 0; i < 2; i++) {
			if (net->output_predict[i] < 0.008) { // 0.2
				net->output_predict[i] = 0.0;
			}
			else if (net->output_predict[i] >= 0.008) { // 0.2
				net->output_predict[i] = 1.0;
			}
		}
	}
	if (Verbosity > 0) {
		cout << "output = {" << output[0] << ", " << output[1] << "}" << endl;
		cout << "output_predict = {" << net->output_predict[0] << ", " << net->output_predict[1] << "}" << endl;
	}
}

void print_layers(struct NN* net) {
	if (Verbosity > 1) {
		cout << "Input of network = ";
		for (int i = 0; i < NUM_OF_INPUTS; i++) {
			cout << net->Input[i] << ", ";
			if ((i % 21) == 0) {
				cout << "/\n";
			}
		}
		cout << "\nBetween of network = ";
		for (int i = 0; i < NUM_OF_FIRST; i++) {
			cout << net->Between[i] << ", ";
			if ((i % 21) == 0) {
				cout << " /\n" << endl;
			}
		}
		cout << "\nBetween2 of network = ";
		for (int i = 0; i < NUM_OF_MIDDLE; i++) {
			cout << net->Between2[i] << ", ";
			if ((i % 21) == 0) {
				cout << " /\n";
			}
		}
		cout << "\nOutput of network = {" << net->Output[0] << ", " << net->Output[1] << "}" << endl;
		cout << "Predicted output = {" << net->output_predict[0] << ", " << net->output_predict[1] << "}" << endl;
	}
}
