//  Controlling traffic lights at crossroads using a neural network
//
//	Adam Gawlik ~ University of Science and Technology in Kraków
//

#pragma once

#include<iostream>
#include<fstream>
#include<cmath>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<map>
#include<ctime>
#include<chrono>
#include<windows.h>
#include<iomanip> 

#pragma warning(disable:4996) // przez ctime


#define NUM_OF_INPUTS 119
#define NUM_OF_FIRST 48
#define NUM_OF_MIDDLE 32
#define NUM_OF_OUTPUTS 2

using namespace std;

int extern Verbosity;


extern vector <tuple<int, int, int, int>> priority_queue;
extern vector <tuple<int, int, int, int>> traffic_queue;
extern vector <tuple<float, float>> output_queue;
extern int day_counter;
extern int hour_counter;
extern bool new_day;
extern bool new_hour;
extern bool teaching_en;
extern float time_scale;
extern bool change_day;

void enqueue_output(struct NN* net);
void enqueue_performance(struct Statistic* stat);
void save_weights(struct NN* net);

