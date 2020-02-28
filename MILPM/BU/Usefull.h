#pragma once
#include <string>
//#include <Python.h>
using namespace std;

struct vertex {
	int key, bLevel, vLoad, aTime, lTime, wTime = 0;
	bool recharge = false;
	int recharged = 0;
};

struct arc {
	int beg, end;
	double value;
};

struct node {
	int key;
	string id, type;
	int x, y, demand;
	int readyTime, dueDate, serviceTime;
	int ref, ref2;
	int id_n = -1; // model's id parameter 
	int ogKey;
};

bool compKey(node a, node b);

struct edge {
	int beg, end;
	float value;
};

int drawGraph(string dir, string fileName, string func, string arg);
