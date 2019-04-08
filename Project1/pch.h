#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#include <math.h>
#include <map>
#include <algorithm>


#define DEGBUG_MODE 1

#define CAR_PARA_NUM 5
#define ROAD_PARA_NUM 7
#define CROSS_PARA_NUM 5
//#define ROAD_BASE_NUM 5000
#define CROSS_BRANCH 4
//#define CROSS_NUM_CORRECTION 1
#define COORDINATE_INTI 10000
#define INT_MIN_INTI 10000
#define MAX_ROAD_NUM 4
#define MAX_TIME 1800			//计划跑多少时间
#define TIME_UNIT 10			//时间间隔，因为每秒不可能出发一张车，所以每十秒出发几张车
#define TIME_DELAY 50			//出发时间延时，最晚计划出发时间
#define PUNISH 4				//路口的惩罚项
#define PUNIDH_NUM 2



class Cross;
class Road;

class Car
{
public:
	int car_id;
	int begin_id;
	int end_id;
	int speed_limit;
	int plan_run_time;

	int actual_run_time;
	int speed;
	int distance_s1;
	bool state;
	std::list<Cross*> cross_list;
	std::list<Road*> road_list;
	std::list<Road*>::iterator current_road;
	std::list<Road*>::iterator next_road;
	Cross * car_cross[2];

	Car(std::string s, std::string::size_type loc[CAR_PARA_NUM]);
	bool operator==(const Car & obj2) const;
	//bool operator>(const Car & obj2) const;
	//void cross_to_road_list(std::list<int> &in);
	void cross_generate_road_list();
	void car_count_cross();
	void car_adjust_run_time();
	void car_to_cross_list();
};


class Road
{
public:
	int road_id;
	int road_len;
	int speed_limit;
	int road_num;
	int begin_id;
	int end_id;
	bool two_way;

	int time_para;
	int cross_punish;

	Cross *road_cross[2];
	//std::vector<std::vector<Car*>> *lane;

	Road(int id);
	Road(std::string s, std::string::size_type loc[ROAD_PARA_NUM]);
	
	bool operator==(const Road & obj2) const;
	void connect_with_cross(Cross &cross, int flag);
	void another_cross(Cross *in, Cross * &out);
	
};

class Cross
{
public:
	int cross_id;
	Road* cross_road[4];
	Cross* father;
	std::list<Car*> car_list;

	int map_x;
	int map_y;
	int f;
	int g;
	int count;
	int run_car_per_time;
	int run_car_per_road[4];

	Cross(std::string s, std::string::size_type loc[CROSS_PARA_NUM], std::vector<Road> &road);
	Cross(int id);

	bool operator==(const Cross & obj2) const;
	void clc_fg();
	bool can_pass(int i);
	void clc_count();
	void calculate_run_car_per_time();
	void calculate_run_car_per_road();
	void sort_car_list();
	void sort_car_time_list();
	void give_car_list_time();
	void adjust_car_list_time();
	void clc_xy();
	bool can_pass2(int i);
};





/*-----------------------------function--------------------------------------*/
void cross_to_map(std::vector<Cross> &cross, std::vector<Road> &road);
void road_to_cross(std::vector<Road> &road, std::vector<Cross> &cross);
void car_to_cross(std::vector<Car> &car, std::vector<Cross> &cross);
void car_scheduling(Car &car, std::vector<Road> &road, std::vector<Cross> &cross);
//bool compare(Car *a, Car *b);
void cross_to_map2(std::vector<Cross> &cross);
