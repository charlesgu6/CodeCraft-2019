#include "pch.h"
#include "A_star.h"
//#include "simulation.h"


int main(int argc, char *argv[])
{
	int cross_id_punish[PUNIDH_NUM] = { 11,205 };
	std::cout << "Begin" << std::endl;
#if DEGBUG_MODE==0
	if (argc < 5) {
		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
		exit(1);
	}

	std::string carPath(argv[1]);
	std::string roadPath(argv[2]);
	std::string crossPath(argv[3]);
	std::string answerPath(argv[4]);

#else
	std::string carPath = "C:\\Users\\Charles\\Documents\\config\\car.txt";
	std::string roadPath = "C:\\Users\\Charles\\Documents\\config\\road.txt";
	std::string crossPath = "C:\\Users\\Charles\\Documents\\config\\cross.txt";
	std::string answerPath = "C:\\Users\\Charles\\Documents\\config\\answer.txt";
	std::string tempPath = "C:\\Users\\Charles\\Documents\\config\\temp.txt";
	std::string ans_tempPath = "C:\\Users\\Charles\\Documents\\config\\ans_temp.txt";
#endif

	std::cout << "carPath is " << carPath << std::endl;
	std::cout << "roadPath is " << roadPath << std::endl;
	std::cout << "crossPath is " << crossPath << std::endl;
	std::cout << "answerPath is " << answerPath << std::endl;

	// TODO:read input filebuf
	// TODO:process
	// TODO:write output file

	// TODO:read input filebuf
	/*---------------------------------------------------------------------------------------*/
	std::string s;	//用于保存从文件中读出的一行内容
	std::vector<Car> car;		//储存车内容
	std::vector<Road> road;		//储存道路内容
	std::vector<Cross> cross;	//储存路口内容

	std::ifstream fin;
	std::ofstream fout;

	fin.open(carPath);
	assert(fin.is_open());

	std::getline(fin, s);
	while(std::getline(fin,s))
	{
		std::string::size_type loc[CAR_PARA_NUM];
		loc[0] = 0;

		s.erase(0, 1);
		s.erase(s.end()-1);
		
		for (int i = 1; i < CAR_PARA_NUM; i++)
		{
			loc[i] = s.find(",",loc[i - 1]+1);
		}

		Car car_temp(s,loc);
		car.push_back(car_temp);

		//std::cout << s << std::endl;
	}
	fin.close();

	fin.open(roadPath);
	assert(fin.is_open());

	std::getline(fin, s);
	while (std::getline(fin, s))
	{
		std::string::size_type loc[ROAD_PARA_NUM];
		loc[0] = 0;

		s.erase(0, 1);
		s.erase(s.end() - 1);
		
		for (int i = 1; i < ROAD_PARA_NUM; i++)
		{
			loc[i] = s.find(",", loc[i - 1] + 1);
		}

		Road road_temp(s, loc);
		road.push_back(road_temp);
	}
	fin.close();

	fin.open(crossPath);
	assert(fin.is_open());

	std::getline(fin, s);
	while (std::getline(fin, s))
	{
		std::string::size_type loc[CROSS_PARA_NUM];
		loc[0] = 0;

		s.erase(0, 1);
		s.erase(s.end() - 1);

		for (int i = 1; i < CROSS_PARA_NUM; i++)
		{
			loc[i] = s.find(",", loc[i - 1] + 1);
		}

		Cross cross_temp(s, loc,road);
		cross.push_back(cross_temp);
	}
	fin.close();
	
	// TODO:process
	/*------------------------------------------------------------------------------*/
	road_to_cross(road, cross);		//建立road到cross的联系
	//cross_to_map(cross,road);		//计算cross的x，y值
	cross_to_map2(cross);			//计算cross的x，y值

#if DEGBUG_MODE == 1
	fout.open(tempPath);
	assert(fout.is_open());

	for (unsigned int i = 0; i < cross.size(); i++)
	{
		fout << cross[i].cross_id << ", " << cross[i].map_x << ", " << cross[i].map_y << std::endl;
	}
	fout.close();
#endif



	car_to_cross(car, cross);		//每张车匹配起点终点地址

	
	//int two_way = 1;
	//for (unsigned int k = 0; k < road.size(); k++)		//建立道路和车道的联系，注意后面需要用delete删除动态分配的空间
	//{
	//	if (road[k].two_way)
	//		two_way = 2;
	//	else
	//		two_way = 1;

	//	std::vector<std::vector<Car*> > *temp = new std::vector<std::vector<Car*> >;
	//	(*temp).resize(road[k].road_num*two_way, std::vector<Car*>(road[k].road_len));
	//	
	//	for (int i = 0; i < road[k].road_num*two_way; i++)
	//		for (int j = 0; j < road[k].road_len; j++)
	//			(*temp)[i][j] = NULL;
	//	road[k].lane = temp;
	//}
	
	//(*road[0].lane)[0][1] = &(car[0]);

	//std::vector<std::vector<Car*>> p[6];
	//Car *p_car;
	//std::vector<Car*> p_vector_car;
	//p_car = &car[0];
	//p_vector_car.push_back(p_car);
	//std::map<int, Cost> cost_table;
	//for (unsigned int i = 0; i < cross.size(); i++)
	//{
	//	cost_table.insert(std::pair<int, Cost>(cross[i].cross_id, Cost()));
	//}

	std::vector<Cross>::iterator it;
	for (int i = 0; i < PUNIDH_NUM; i++)
	{
		it = std::find(cross.begin(), cross.end(), Cross(cross_id_punish[i]));
		if(it==cross.end())
		{ }
		else
		{
			for (int j = 0; j < MAX_ROAD_NUM; j++)
			{
				if (it->cross_road[j] == NULL)
				{

				}
				else
				{
					it->cross_road[j]->cross_punish = it->cross_road[j]->cross_punish + 1;
				}
			}
		}
		
	}
		

	for (unsigned int i = 0; i < car.size(); i++)	//A*算法计算无车时最优路径
		car_A_star(car[i], cross);

	for (unsigned int i = 0; i < car.size(); i++)	//由计算得到的cross地址得到road地址
		car[i].cross_generate_road_list();

	//for (unsigned int i = 0; i < cross.size(); i++)	//清除之前cross的统计
	//	cross[i].clc_count();

	//for (unsigned int i = 0; i < car.size(); i++)	//进行cross的统计
	//	car[i].car_count_cross();

	//for (unsigned int i = 0; i < car.size(); i++)	//调整出发时间
	//	car[i].car_adjust_run_time();

	for (unsigned int i = 0; i < car.size(); i++)	//将车放入车库
		car[i].car_to_cross_list();

	for (unsigned int i = 0; i < cross.size(); i++)	//根据车库中车的数量决定没单元时间跑多少车
		cross[i].calculate_run_car_per_time();

	for (unsigned int i = 0; i < cross.size(); i++)	//决定每条路跑多少量车
		cross[i].calculate_run_car_per_road();

	//cross[0].car_list.sort(std::greater<Car*>());
	//cross[0].sort_car_list();
	for (unsigned int i = 0; i < cross.size(); i++)	//对每个cross车库按速度由大到小进行排序（冒泡）
		cross[i].sort_car_list();

	//cross[0].sort_car_time_list();
	for (unsigned int i = 0; i < cross.size(); i++)	//按路口的道路数量进行排序
		cross[i].sort_car_time_list();

	//cross[0].give_car_list_time();
	for (unsigned int i = 0; i < cross.size(); i++)	//给车库list赋值时间
		cross[i].give_car_list_time();

	//cross[0].adjust_car_list_time();
	for (unsigned int i = 0; i < cross.size(); i++)	//调整车库list赋值时间
		cross[i].adjust_car_list_time();

	for (unsigned int i = 0; i < car.size(); i++)
		car[i].actual_run_time = car[i].actual_run_time + TIME_DELAY;

#if DEGBUG_MODE==1
	/*for (unsigned int i = 0; i < car.size(); i++)
	{
		std::list<Road*>::iterator it = car[i].road_list.begin();
		std::cout << "(" << car[i].car_id << ", " << car[i].actual_run_time << ", " << (*it)->road_id;
		it++;
		for (; it != car[i].road_list.end(); it++)
		{
			std::cout << ", " << (*it)->road_id;
		}
		std::cout << ")" << std::endl;
	}*/
	/*for (unsigned int i = 0; i < car.size(); i++)
	{
		std::list<int> car_road_list;
		car[i].cross_to_road_list(car_road_list);

		std::list<int>::iterator it = car_road_list.begin();
		std::cout << "(" << (*it);
		it++;
		for (; it != car_road_list.end(); it++)
		{
			std::cout << ", " << (*it);
		}
		std::cout << ")" << std::endl;
	}*/
	fout.open(tempPath);
	assert(fout.is_open());

	for (unsigned int i = 0; i < cross.size(); i++)
	{
		fout << cross[i].cross_id << ", " << cross[i].count << std::endl;
	}
	fout.close();

	fout.open(ans_tempPath);
	assert(fout.is_open());

	for (unsigned int i = 0; i < cross.size(); i++)
	{
		fout << cross[i].cross_id << std::endl;
		for (int j = 0; j < CROSS_BRANCH; j++)
		{
			if(cross[i].cross_road[j]==NULL)
				fout << -1 << ", " << std::endl;
			else
				fout << cross[i].cross_road[j]->road_id << ", " << std::endl;
		}
			
		for (int j = 0; j < CROSS_BRANCH; j++)
		{
			if (cross[i].cross_road[j] == NULL)
				fout << -1 << ", " << std::endl;
			else
				fout << cross[i].cross_road[j]->road_num << ", " << std::endl;
		}
			
		for (std::list<Car*>::iterator it = cross[i].car_list.begin(); it != cross[i].car_list.end(); it++)
			fout << (*it)->car_id << ", " << (*(*it)->road_list.begin())->road_id << std::endl;

		fout << std::endl << std::endl << std::endl;
		
	}
	fout.close();

#endif
	
	fout.open(answerPath);
	assert(fout.is_open());

	for (unsigned int i = 0; i < car.size(); i++)
	{
		std::list<Road*>::iterator it = car[i].road_list.begin();
		fout << "(" << car[i].car_id << ", " << car[i].actual_run_time << ", " << (*it)->road_id;
		it++;
		for (; it != car[i].road_list.end(); it++)
		{
			fout << ", " << (*it)->road_id;
		}
		fout << ")" << std::endl;
	}
	/*for (unsigned int i = 0; i < car.size(); i++)
	{
		std::list<int> car_road_list;
		car[i].cross_to_road_list(car_road_list);

		std::list<int>::iterator it = car_road_list.begin();
		fout << "(" << (*it);
		it++;
		for (; it != car_road_list.end(); it++)
		{
			fout << ", " << (*it);
		}
		fout << ")" << std::endl;
	}*/
	fout.close();


	return 0;
}