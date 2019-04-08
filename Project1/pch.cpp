#include "pch.h"

/*--------------------------class----------------------------------------*/
Car::Car(std::string s, std::string::size_type loc[CAR_PARA_NUM])
{
	car_id = std::stoi(s.substr(0, loc[1]));
	begin_id = std::stoi(s.substr(loc[1] + 2, loc[2] - loc[1] - 2));
	end_id = std::stoi(s.substr(loc[2] + 2, loc[3] - loc[2] - 2));
	speed_limit = std::stoi(s.substr(loc[3] + 2, loc[4] - loc[3] - 2));
	plan_run_time = std::stoi(s.substr(loc[4] + 2));

	actual_run_time = plan_run_time;
	speed = 0;
	distance_s1 = 0;
	state = 0;
}
bool Car::operator==(const Car & obj2) const
{
	if (this->car_id == obj2.car_id)
		return true;
	else
		return false;
}
//bool Car::operator>(const Car & obj2) const
//{
//	return this->speed_limit > obj2.speed_limit;
//}
//template<> 
//class std::greater<Car*>
//{
//	bool operator()(const Car* a1, const Car* a2) const
//	{
//		return (*a1).speed_limit > (*a2).speed_limit;
//	}
//};
//void Car::cross_to_road_list(std::list<int> &in)
//{
//	int last_cross_id, current_cross_id[2];
//	std::list<Cross*>::iterator it, it_last;
//	it = this->cross_list.begin();
//	//it = (it_last++);
//	it_last = it;
//	it++;
//	for (; it != this->cross_list.end(); it++)
//	{
//		for (int i = 0; i < CROSS_BRANCH; i++)
//		{
//			if ((*it)->cross_road[i] == NULL)
//				continue;
//			last_cross_id = (*it_last)->cross_id;
//			current_cross_id[0] = (*it)->cross_road[i]->begin_id;
//			current_cross_id[1] = (*it)->cross_road[i]->end_id;
//			if (last_cross_id == current_cross_id[0] || last_cross_id == current_cross_id[1])
//			{
//				in.push_back((*it)->cross_road[i]->road_id);
//				break;
//			}
//		}
//		it_last = it;
//	}
//	in.push_front(this->plan_run_time);
//	in.push_front(this->car_id);
//}
void Car::cross_generate_road_list()
{
	int last_cross_id, current_cross_id[2];
	std::list<Cross*>::iterator it, it_last;
	it = this->cross_list.begin();
	//it = (it_last++);
	it_last = it;
	it++;
	for (; it != this->cross_list.end(); it++)
	{
		for (int i = 0; i < CROSS_BRANCH; i++)
		{
			if ((*it)->cross_road[i] == NULL)
				continue;
			last_cross_id = (*it_last)->cross_id;
			current_cross_id[0] = (*it)->cross_road[i]->begin_id;
			current_cross_id[1] = (*it)->cross_road[i]->end_id;
			if (last_cross_id == current_cross_id[0] || last_cross_id == current_cross_id[1])
			{
				this->road_list.push_back((*it)->cross_road[i]);
				break;
			}
		}
		it_last = it;
	}
	//in.push_front(this->plan_run_time);
	//in.push_front(this->car_id);
}
void Car::car_count_cross()
{
	for (std::list<Cross*>::iterator it = this->cross_list.begin(); it != this->cross_list.end(); it++)
	{
		(*it)->count++;
	}
}
void Car::car_adjust_run_time()
{
	this->actual_run_time = this->actual_run_time * 100;
}
void Car::car_to_cross_list()
{
	std::list<Cross*>::iterator it;
	it = this->cross_list.begin();
	(*it)->car_list.push_back(this);
}

Road::Road(std::string s, std::string::size_type loc[ROAD_PARA_NUM])
{
	road_id = std::stoi(s.substr(0, loc[1]));
	road_len = std::stoi(s.substr(loc[1] + 2, loc[2] - loc[1] - 2));
	speed_limit = std::stoi(s.substr(loc[2] + 2, loc[3] - loc[2] - 2));
	road_num = std::stoi(s.substr(loc[3] + 2, loc[4] - loc[3] - 2));
	begin_id = std::stoi(s.substr(loc[4] + 2, loc[5] - loc[4] - 2));
	end_id = std::stoi(s.substr(loc[5] + 2, loc[6] - loc[5] - 2));
	two_way = std::stoi(s.substr(loc[6] + 2));

	time_para = (int)ceil(road_len / speed_limit);
	cross_punish = 0;
	
	//std::vector<std::vector<Car*>> lane_temp(road_num);
	//lane = &lane_temp;
}
Road::Road(int id)
{
	road_id = id;
	road_len = 0;
	speed_limit = 0;
	road_num = 0;
	begin_id = 0;
	end_id = 0;
	two_way = 0;
}
bool Road::operator==(const Road & obj2) const
{
	if (this->road_id == obj2.road_id)
		return true;
	else
		return false;
}
void Road::connect_with_cross(Cross &cross, int flag) 
{
	if (flag == 0)
		this->road_cross[0] = &cross;
	else
		this->road_cross[1] = &cross;
}
void Road::another_cross(Cross *in, Cross * &out)
{
	if (this->road_cross[0] == in)
		out = this->road_cross[1];
	else
		out = this->road_cross[0];

}

Cross::Cross(std::string s, std::string::size_type loc[CROSS_PARA_NUM], std::vector<Road> &road_vector)
{
	int road_id[4];
	map_x = COORDINATE_INTI;
	map_y = COORDINATE_INTI;

	cross_id = std::stoi(s.substr(0, loc[1]));
	for (int i = 0; i < 3; i++)
	{
		road_id[i]= std::stoi(s.substr(loc[i+1] + 2, loc[i+2] - loc[i+1] - 2));
	}
	road_id[3]= std::stoi(s.substr(loc[4] + 2));

	for (int i = 0; i < 4; i++)
	{
		//int road_id_temp = road_id[i];
		if (road_id[i] == -1)
			cross_road[i] = NULL;
		else
		{
			std::vector<Road>::iterator it;
			it = std::find(road_vector.begin(), road_vector.end(), Road(road_id[i]));
			cross_road[i] = &*it;
			//*road[i] = *std::find(road_vector.begin(), road_vector.end(), Road(road_id_temp));
			//std::cout << "[Cross::Cross()] Warning: road rank does not match!" << std::endl;	//road的txt文件中，road_id不是顺序排列的
			
		}
	}

}
Cross::Cross(int id)
{
	cross_id = id;
}
bool Cross::operator==(const Cross & obj2) const
{
	if (this->cross_id == obj2.cross_id)
		return true;
	else
		return false;
}
void Cross::clc_fg()
{
	this->f = 0;
	this->g = 0;
	this->father = NULL;
}
bool Cross::can_pass(int i)
{
	if (this->cross_road[i] == NULL)
		return false;
	if (this->cross_road[i]->two_way || this->cross_id == this->cross_road[i]->begin_id)
		return true;
	else
		return false;
}
void Cross::clc_count()
{
	this->count = 0;
}
void Cross::calculate_run_car_per_time()
{
	this->run_car_per_time = ceil(this->car_list.size() * double(TIME_UNIT) / MAX_TIME);
}
void Cross::calculate_run_car_per_road()
{
	for (int i = 0; i < CROSS_BRANCH; i++)
		this->run_car_per_road[i] = 0;
	int sum = 0;
	for (int i = 0; i < CROSS_BRANCH; i++)
	{
		if (this->can_pass(i))
			sum = sum + this->cross_road[i]->road_num;
	}
	
	for (int i = 0; i < CROSS_BRANCH; i++)
	{
		if (this->can_pass(i))
			this->run_car_per_road[i] = ceil(this->run_car_per_time*double(this->cross_road[i]->road_num) / sum);
	}
}
void Cross::sort_car_list()
{
	std::list<Car*>::iterator it_cur;
	std::list<Car*>::iterator it_next;
	Car *temp;

	it_next = this->car_list.begin();
	it_cur = it_next;
	it_next++;

	bool flag = true;
	while (flag)
	{
		flag = false;
		for (it_next = this->car_list.begin(), it_cur = it_next, it_next++; it_next != car_list.end(); it_cur++, it_next++)
		{
			if ((*it_cur)->speed_limit < (*it_next)->speed_limit)
			{
				temp = *it_cur;
				*it_cur = *it_next;
				*it_next = temp;
				flag = true;
			}
		}
	}
}
void Cross::sort_car_time_list()
{
	std::list<Car*>::iterator p_cur;
	std::list<Car*>::iterator p_find;
	Car* temp;

	p_cur = this->car_list.begin();
	while (p_cur != this->car_list.end())
	{
		for (int i = 0; i < CROSS_BRANCH; i++)
		{
			for (int j = 0; j < this->run_car_per_road[i]; j++)
			{
				if ((*(*p_cur)->road_list.begin())->road_id == this->cross_road[i]->road_id)
					p_cur++;
				else
				{
					for (p_find = p_cur; p_find != this->car_list.end(); p_find++)
					{
						if ((*(*p_find)->road_list.begin())->road_id == this->cross_road[i]->road_id)
							break;
					}
					if (p_find == this->car_list.end())
					{
						p_cur++;
						break;
					}
					else
					{
						temp = (*p_cur);
						*p_cur = *p_find;
						*p_find = temp;
						p_cur++;
					}
				}
				if (p_cur == this->car_list.end())
					break;
			}
			if (p_cur == this->car_list.end())
				break;
		}
	}
}
void Cross::give_car_list_time()
{
	int sum_num = 0;
	int time = 0;
	for (int i = 0; i < CROSS_BRANCH; i++)
		sum_num = sum_num + this->run_car_per_road[i];
	for (std::list<Car*>::iterator it = this->car_list.begin(); it != this->car_list.end();)
	{
		time++;
		for (int j = 0; j < sum_num; j++)
		{
			(*it)->actual_run_time = time;
			it++;
			if (it == this->car_list.end())
				break;
		}
		/*if (it == this->car_list.end())
			break;*/
	}
}
void Cross::adjust_car_list_time()
{
	int sum_num = 0;
	int add_time = 0;

	for (std::list<Car*>::iterator it = this->car_list.begin(); it != this->car_list.end(); it++)
		(*it)->actual_run_time = (*it)->actual_run_time*TIME_UNIT;

	for (int i = 0; i < CROSS_BRANCH; i++)
		sum_num = sum_num + this->run_car_per_road[i];
	add_time = floor(double(TIME_UNIT) / sum_num);

	std::list<Car*>::iterator it_last = this->car_list.begin();
	std::list<Car*>::iterator it_cur = it_last;
	//it_cur++;
	for (; it_cur != this->car_list.end();)
	{
		for (int j = 0; j < sum_num; j++)
		{
			if (j == 0)
			{
				it_last = it_cur;
				it_cur++;
				if (it_cur == this->car_list.end())
					break;
			}
			else
			{
				(*it_cur)->actual_run_time = (*it_last)->actual_run_time + add_time;
				it_cur++;
				it_last++;
				if (it_cur == this->car_list.end())
					break;
			}
		}
	}
}
void Cross::clc_xy()
{
	this->map_x = 0;
	this->map_y = 0;
}
bool Cross::can_pass2(int i)
{
	if (this->cross_road[i] == NULL)
		return false;
	else
		return true;
}

/*--------------------------function---------------------------------*/
void cross_to_map(std::vector<Cross> &cross, std::vector<Road> &road)
{
	int begin_id_temp, end_id_temp;
	int cross_id_temp;
	int cross_branch_temp;
	std::vector<Cross>::iterator  begin_cross;
	std::vector<Cross>::iterator  end_cross;
	cross[0].map_x = 0;
	cross[0].map_y = 0;
	for (unsigned int i = 1; i < cross.size(); i++)
	{
		cross_id_temp = cross[i].cross_id;
		for (int j = 0; j < CROSS_BRANCH; j++)
		{
			cross_branch_temp = -1;
			if(cross[i].cross_road[j]!=NULL)
			{
				begin_id_temp = cross[i].cross_road[j]->begin_id;
				end_id_temp = cross[i].cross_road[j]->end_id;
				begin_cross = std::find(cross.begin(), cross.end(), Cross(begin_id_temp));
				end_cross = std::find(cross.begin(), cross.end(), Cross(end_id_temp));
				if (begin_id_temp == cross_id_temp)
				{
					if (end_cross->map_x != COORDINATE_INTI)
					{
						cross_branch_temp = j;
						break;
					}
				}
				else
				{
					if (begin_cross->map_x != COORDINATE_INTI)
					{
						cross_branch_temp = j;
						break;
					}
				}
			}
		}
		switch (cross_branch_temp)
		{
		case -1:
		{
			std::cout << "[cross_to_map]: Can dicide map: " << cross_id_temp << std::endl;
			break;
		}
		case 0:
		{
			if (begin_id_temp == cross_id_temp)
			{
				cross[i].map_x = end_cross->map_x;
				cross[i].map_y = end_cross->map_y - cross[i].cross_road[cross_branch_temp]->road_len;
			}
			else
			{
				cross[i].map_x = begin_cross->map_x;
				cross[i].map_y = begin_cross->map_y - cross[i].cross_road[cross_branch_temp]->road_len;
			}
			break;
		}
		case 1:
		{
			if (begin_id_temp == cross_id_temp)
			{
				cross[i].map_x = end_cross->map_x - cross[i].cross_road[cross_branch_temp]->road_len;
				cross[i].map_y = end_cross->map_y;
			}
			else
			{
				cross[i].map_x = begin_cross->map_x - cross[i].cross_road[cross_branch_temp]->road_len;
				cross[i].map_y = begin_cross->map_y ;
			}
			break;
		}
		case 2:
		{
			if (begin_id_temp == cross_id_temp)
			{
				cross[i].map_x = end_cross->map_x;
				cross[i].map_y = end_cross->map_y + cross[i].cross_road[cross_branch_temp]->road_len;
			}
			else
			{
				cross[i].map_x = begin_cross->map_x;
				cross[i].map_y = begin_cross->map_y + cross[i].cross_road[cross_branch_temp]->road_len;
			}
			break;
		}
		case 3:
		{
			if (begin_id_temp == cross_id_temp)
			{
				cross[i].map_x = end_cross->map_x + cross[i].cross_road[cross_branch_temp]->road_len;
				cross[i].map_y = end_cross->map_y;
			}
			else
			{
				cross[i].map_x = begin_cross->map_x + cross[i].cross_road[cross_branch_temp]->road_len;
				cross[i].map_y = begin_cross->map_y;
			}
			break;
		}
		default:
			break;
		}
	}
}
void road_to_cross(std::vector<Road> &road, std::vector<Cross> &cross)
{
	std::vector<Cross>::iterator cross_temp;
	for (unsigned int i = 0; i < road.size(); i++)
	{
		cross_temp = std::find(cross.begin(), cross.end(), Cross(road[i].begin_id));
		road[i].road_cross[0] = &*cross_temp;
		cross_temp = std::find(cross.begin(), cross.end(), Cross(road[i].end_id));
		road[i].road_cross[1] = &*cross_temp;
	}
}
void car_to_cross(std::vector<Car> &car, std::vector<Cross> &cross)
{
	std::vector<Cross>::iterator cross_temp;
	for (unsigned int i = 0; i < car.size(); i++)
	{
		cross_temp = std::find(cross.begin(), cross.end(), Cross(car[i].begin_id));
		car[i].car_cross[0] = &*cross_temp;
		cross_temp = std::find(cross.begin(), cross.end(), Cross(car[i].end_id));
		car[i].car_cross[1] = &*cross_temp;
	}
}
void car_scheduling(Car &car, std::vector<Road> &road, std::vector<Cross> &cross)
{
	car.cross_list.push_back(car.car_cross[0]);
	Cross *p_temp;
	p_temp = car.cross_list.back();
	while (p_temp != car.car_cross[1])
	{

	}
}
//bool compare(std::list<Car*>::iterator a, std::list<Car*>::iterator b)
//{
//	return (*a)->speed_limit > (*b)->speed_limit;
//}

void cross_to_map2(std::vector<Cross> &cross)
{
	for (unsigned int i = 0; i < cross.size(); i++)
		cross[i].clc_xy();

	std::list<Cross*> open_set;
	std::list<Cross*> close_set;

	Cross *temp = NULL, *can_pass_temp = NULL;
	int temp_x, temp_y;
	std::list<Cross*>::iterator it_open;
	std::list<Cross*>::iterator it_close;

	temp = &cross[0];
	open_set.push_back(temp);

	while (open_set.size() != 0)
	{
		temp = *open_set.begin();
		open_set.erase(open_set.begin());
		close_set.push_back(temp);

		for (int i = 0; i < CROSS_BRANCH; i++)
		{
			if (temp->can_pass2(i))
			{
				temp->cross_road[i]->another_cross(temp, can_pass_temp);
				switch (i)
				{
				case 0:
					//can_pass_temp->map_x = temp->map_x;
					//can_pass_temp->map_y = temp->map_y + temp->cross_road[i]->road_len;
					temp_x = temp->map_x;
					temp_y = temp->map_y + temp->cross_road[i]->road_len;
					break;
				case 1:
					temp_x = temp->map_x + temp->cross_road[i]->road_len;
					temp_y = temp->map_y;
					break;
				case 2:
					temp_x = temp->map_x;
					temp_y = temp->map_y - temp->cross_road[i]->road_len;
					break;
				case 3:
					temp_x = temp->map_x - temp->cross_road[i]->road_len;
					temp_y = temp->map_y;
					break;
				default:
					break;
				}
				it_close = std::find(close_set.begin(), close_set.end(), can_pass_temp);
				it_open = std::find(open_set.begin(), open_set.end(), can_pass_temp);
				if (it_close != close_set.end())
				{
					if (temp_x != can_pass_temp->map_x || temp_y != can_pass_temp->map_y)
						std::cout << "[cross_to_map2]:cross[" << can_pass_temp->cross_id << "] have two x,y" << std::endl;
				}
				else
				{
					if(it_open != open_set.end())
					{ }
					else
					{
						open_set.push_back(can_pass_temp);
						can_pass_temp->map_x = temp_x;
						can_pass_temp->map_y = temp_y;
					}
				}
			}
		}
	}
}

