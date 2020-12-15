//  Controlling traffic lights at crossroads using a neural network
//
//	Adam Gawlik ~ University of Science and Technology in Kraków
//



#include<iostream>
#include "main.h"
#include "lights_nn.h"
#include "env_nn.h"
#include "stat_nn.h"

#pragma warning(disable:4996) // due to use of ctime

using namespace std;
vector <tuple<int, int, int, int>> priority_queue;
vector <tuple<float, float>>  output_queue;
vector <tuple<int, int, int, int>> traffic_queue;
bool new_hour = false;
int day_counter = 0;
int hour_counter = 0;
bool new_day = false;
bool teaching_en = false;
bool change_day = true;

int Verbosity = 0;
float time_scale = 35.0; // or 30.0
void main()
{
	srand(time(NULL));

	struct Environment env1;
	struct Statistic stat1;
	struct NN network1;
	lights_init(&lights, &env1, &stat1, &network1);

	int timer15 = 0;

	while (true)
	{
		if (day_counter > 4) { 
			teaching_en = true;
		}
		if (day_counter == 1200) {
			break;
		}
		//Sleep(100);

		increment_time(&env1);
		update_lights(&lights);
		timer15++;

		if (timer15 == 5) {
			turn_on_lights(&lights);
			timer15++;
		}
	}
}

void enqueue_output(struct NN * net) {
	double temp[2];
	int tempx[4];
	//output_queue.push_back(tuple<float, float>(net->Output[0], net->Output[1]));
	//traffic_queue.push_back(tuple<int, int, int, int>(net->lig_ptr->direction_traffic[0], net->lig_ptr->direction_traffic[1], net->lig_ptr->direction_traffic[2], net->lig_ptr->direction_traffic[3]));
	if (new_hour) {
		temp[0] = 0.0;
		temp[1] = 0.0;
		tempx[0] = 0;
		tempx[1] = 0;
		tempx[2] = 0;
		tempx[3] = 0;
		for (int i = 0; i < output_queue.size(); i++) {
			temp[0] += get<0>(output_queue[i]);
			temp[1] += get<1>(output_queue[i]);
			tempx[0] += get<0>(traffic_queue[i]);
			tempx[1] += get<1>(traffic_queue[i]);
			tempx[2] += get<2>(traffic_queue[i]);
			tempx[3] += get<3>(traffic_queue[i]);
		}
		///*cout << ((float)tempx[0] / traffic_queue.size())  << " " << ((float)tempx[1] / traffic_queue.size()) << " " << ((float)tempx[2] / traffic_queue.size()) << " " << ((float)tempx[3] / traffic_queue.size()) << endl;*/
		//cout << ((float)tempx[0] / traffic_queue.size()) + ((float)tempx[2] / traffic_queue.size()) << " " << ((float)tempx[1] / traffic_queue.size()) + ((float)tempx[3] / traffic_queue.size()) << endl;
		/*cout << ((float)tempx[0] / traffic_queue.size())  << " " << ((float)tempx[1] / traffic_queue.size()) << " " << ((float)tempx[2] / traffic_queue.size()) << " " << ((float)tempx[3] / traffic_queue.size()) << endl;*/

		//cout << hour_counter << " " << temp[0] / output_queue.size()  << endl;  // trace

		//cout << ((float)tempx[0] / traffic_queue.size()) + ((float)tempx[2] / traffic_queue.size()) << endl;  // trace
		traffic_queue.clear();
		output_queue.clear();
		new_hour = false;
	}
}

void enqueue_performance(struct Statistic* stat) {
	long int temp[4];
	priority_queue.push_back(tuple<int, int, int, int>(stat->direction_priority[0], stat->direction_priority[1], stat->direction_priority[2], stat->direction_priority[3]));
	if (new_day) {
		cout << "NEW DAY " << day_counter << " " << stat->lig_ptr->day_of_week_string[stat->lig_ptr->current_day] << "!" << endl;
		temp[0] = 0;
		temp[1] = 0;
		temp[2] = 0;
		temp[3] = 0;
		for (int i = 0; i < priority_queue.size(); i++) {
			temp[0] += get<0>(priority_queue[i]);
			temp[1] += get<1>(priority_queue[i]);
			temp[2] += get<2>(priority_queue[i]);
			temp[3] += get<3>(priority_queue[i]);
		}
		auto at = *max_element(priority_queue.begin(), priority_queue.end());

		//cout << "mean priority for " << day_counter << " is: {" << ((float)temp[0] / priority_queue.size()) << ", " << ((float)temp[1] / priority_queue.size()) << ", " << ((float)temp[2] / priority_queue.size()) << ", " << ((float)temp[3] / priority_queue.size()) << "}" << endl;
		//cout << "sum priority : " << ((float)temp[0] / priority_queue.size()) + ((float)temp[1] / priority_queue.size()) + ((float)temp[2] / priority_queue.size()) + ((float)temp[3] / priority_queue.size()) << endl;
		cout << ((float)temp[0] / priority_queue.size()) + ((float)temp[1] / priority_queue.size()) + ((float)temp[2] / priority_queue.size()) + ((float)temp[3] / priority_queue.size()) << endl;
		if (((float)temp[0] / priority_queue.size()) + ((float)temp[1] / priority_queue.size()) + ((float)temp[2] / priority_queue.size()) + ((float)temp[3] / priority_queue.size()) < 10.0) {
			if (day_counter > 60)
			while (true) {
				save_weights(stat->lig_ptr->net_ptr);
				system("pause");
			}
		}
		if (day_counter > 70) {
			system("pause");
		}
		priority_queue.clear();
		new_day = false;
	}
}

void save_weights(struct NN* net) {		// function responsible for printing to file
	ofstream dst;
	int line_cnt = 0;
	dst.open("printed_weights.c");	// destination printed file
	Sleep(2);
	while (!dst.is_open()) {}
	cout << "Zapisuje" << endl;
	if (dst.is_open()) {
		dst << "#include \"net_nn.h\"" << endl << endl;
		dst << "/* Adam Gawlik ~ University of Science and Technology in Krakow */" << endl;
		dst << "/* automatically printed weights */" << endl;
		dst << "/* weights printed on day of simulation: " << day_counter << ", " << net->lig_ptr->day_of_week_string[net->lig_ptr->current_day] << " */" << endl << endl;
		dst << "float first_weights_ref[NUM_OF_FIRST][NUM_OF_INPUTS] = {";
		for (int i = 0; i < NUM_OF_FIRST; i++) {
			dst << "{";
			for (int k = 0; k < NUM_OF_INPUTS; k++) {
				dst << setprecision(15) << net->first_weights[i][k];
				if (line_cnt > 7) {
					dst << ",";
					dst << endl;
					line_cnt = 0;
				}
				else {
					if (k == NUM_OF_INPUTS - 1) {
						dst << " ";
					} else {
						dst << ", ";
					}
					line_cnt++;
				}
			}
			if (i == NUM_OF_FIRST - 1) {
				dst << "}";
			}
			else {
				dst << "}, ";
			}
		}
		dst << "};" << endl;
		dst << "float middle_weights_ref[NUM_OF_MIDDLE][NUM_OF_FIRST] = {";
		line_cnt = 0;
		for (int i = 0; i < NUM_OF_MIDDLE; i++) {
			dst << "{";
			for (int k = 0; k < NUM_OF_FIRST; k++) {
				dst << setprecision(15) << net->middle_weights[i][k];
				if (line_cnt > 7) {
					dst << ",";
					dst << endl;
					line_cnt = 0;
				}
				else {
					if (k == NUM_OF_FIRST - 1) {
						dst << " ";
					}
					else {
						dst << ", ";
					}
					line_cnt++;
				}
			}
			if (i == NUM_OF_MIDDLE - 1) {
				dst << "}";
			}
			else {
				dst << "}, ";
			}
		}
		dst << "};" << endl;
		dst << "float output_weights_ref[NUM_OF_OUTPUTS][NUM_OF_MIDDLE] = {";
		line_cnt = 0;
		for (int i = 0; i < NUM_OF_OUTPUTS; i++) {
			dst << "{";
			for (int k = 0; k < NUM_OF_MIDDLE; k++) {
				dst << setprecision(15) << net->output_weights[i][k];
				if (line_cnt > 7) {
					dst << ",";
					dst << endl;
					line_cnt = 0;
				}
				else {
					if (k == NUM_OF_MIDDLE - 1) {
						dst << " ";
					}
					else {
						dst << ", ";
					}
					line_cnt++;
				}
			}
			if (i == NUM_OF_OUTPUTS - 1) {
				dst << "}";
			}
			else {
				dst << "}, ";
			}
		}
		dst << "};" << endl;
		line_cnt = 0;
	}
	dst.close();
	cout << "Zapisano wagi do: printed_weights.c, mozna go skopiowac do projektu na mikrokontroler do folderu src" << endl;
}