#pragma once
#include <queue>

#include "pch.h"

//class Cost
//{
//public:
//	int f;
//	int g;
//
//	Cost();
//	Cost(int a, int b);
//};
//class Compare
//{
//public:
//	Cross *p;
//
//	Compare(Cross *temp);
//};
//struct cmp1
//{
//	bool operator ()(Cross *x, Cross *y)
//	{
//		return x->f > y->f;
//	}
//};


/*--------------------------function------------------------------------*/
void car_A_star(Car &car, std::vector<Cross> &cross);
void find_open_min_and_drop(std::list<Cross*> &in, Cross *& out);


