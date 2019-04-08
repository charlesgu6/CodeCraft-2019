#include "A_star.h"	

//Cost::Cost()
//{
//	f = 0;
//	g = 0;
//}
//Cost::Cost(int a, int b)
//{
//	f = a;
//	g = b;
//}
//Compare::Compare(Cross *temp)
//{
//	p = temp;
//}


/*--------------------------function---------------------------------*/
void car_A_star(Car &car, std::vector<Cross> &cross)
{
	for (unsigned int i = 0; i < cross.size(); i++)
		cross[i].clc_fg();
	//std::priority_queue<Cross*, cmp1> open_set;
	std::list<Cross*> open_set;
	std::list<Cross*> close_set;

	Cross *temp = NULL, *can_pass_temp = NULL, *end_cross = NULL, *begin_cross = NULL;
	int g_temp, f_temp;
	std::list<Cross*>::iterator it_open;
	std::list<Cross*>::iterator it_close;

	
	begin_cross = car.car_cross[0];
	end_cross = car.car_cross[1];
	temp = begin_cross;
	open_set.push_back(temp);
	while (temp != end_cross)
	{
		//temp = open_set.top();
		find_open_min_and_drop(open_set, temp);
		//open_set.pop();
		close_set.push_back(temp);
		
		for (int i = 0; i < CROSS_BRANCH; i++)
		{
			if (temp->can_pass(i))	//这条路是通的
			{
				temp->cross_road[i]->another_cross(temp, can_pass_temp);	//赋值给can_pass_temp另一个路口地址
				//g_temp = temp->g + ceil(temp->cross_road[i]->road_len / temp->cross_road[i]->speed_limit);
				g_temp = temp->g + ceil(temp->cross_road[i]->road_len / temp->cross_road[i]->speed_limit) + (MAX_ROAD_NUM - temp->cross_road[i]->road_num)+ PUNISH*temp->cross_road[i]->cross_punish;		//加入lane数量进行优化
				f_temp = g_temp + ceil((abs(end_cross->map_x - can_pass_temp->map_x) + abs(end_cross->map_y - can_pass_temp->map_y)) / car.speed_limit);
				it_close = std::find(close_set.begin(), close_set.end(), can_pass_temp);
				it_open = std::find(open_set.begin(), open_set.end(), can_pass_temp);
				if (it_close != close_set.end())
				{
					if (g_temp < can_pass_temp->g)
					{
						can_pass_temp->g = g_temp;
						can_pass_temp->f = f_temp;
						can_pass_temp->father = temp;
					}
				}
				else
				{
					if (it_open != open_set.end())
					{ }
					else
					{
						open_set.push_back(can_pass_temp);
						can_pass_temp->father = temp;
						can_pass_temp->g = g_temp;
						can_pass_temp->f = f_temp;
					}

				}
			}
		}
		
	}
	
	temp = end_cross;
	while (temp != begin_cross)
	{
		car.cross_list.push_front(temp);
		temp = temp->father;
	}
	car.cross_list.push_front(temp);

}
void find_open_min_and_drop(std::list<Cross*> &in, Cross *& out)
{
	Cross *temp = NULL;
	//int min= INT_MIN_INTI;
	int min;
	std::list<Cross*>::iterator it = in.begin();
	std::list<Cross*>::iterator it_min = in.begin();
	min = (*it)->f;
	temp = (*it);
	it_min = it;
	for (; it != in.end(); it++)
	{
		if ((*it)->f < min)
		{
			min = (*it)->f;
			temp = (*it);
			it_min = it;
		}
	}
	/*while (it != in.end())
	{
		it++;
		if ((*it)->f < min)
		{
			min = (*it)->f;
			temp = (*it);
		}
	}*/
	out = temp;
	in.erase(it_min);
}