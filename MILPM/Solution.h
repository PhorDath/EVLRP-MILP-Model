#pragma once
#include <iostream>
#include <vector>
#include "Usefull.h"
using namespace std;

typedef vector<vertex> route;
typedef vector<route> routes;

struct item {
	int key;
	int nodeKey;
};
typedef vector<int> permutation;

struct solution {
	int FO;
	vector<int> FOp;
	vector<string> inf;
	vector<vector<vertex>> routes;
	permutation perm;

	int time;
	int FOINIT;
	int status;
};

void strmSol(solution sol, ostream& strm);

class Solution
{
public:
	string generator;
	string instance;

	int FO;
	vector<int> FOp;
	vector<string> inf;
	vector<vector<vertex>> routes;
	permutation perm;
	int time;
	int FOINIT;
	int status;
	
	int numVehicles = 0;
	vector<arc> arcs;


public:
	Solution();
	int set(vector<vector<vertex>> s);
	void getRoutes();
	void insertArc(int beg, int end, int value);
	void insertStation(int s);
	void print();
	~Solution();
};

/*
class Solution
{
public:
	int FO = 0;
	// individual FO costs
	float dCost = 0; // depot cost
	float sCost = 0; // stations cost
	float dwCost = 0; // driver wage cost
	float vCost = 0; // vehicle cost
	float eCost = 0; // energy cost
	float bssUseCost = 0;
	float runTime = 0;
	float gap = 0;
	int status = 0;

	int numVehicles = 0;

	vector<vector<vertex>> routes;

	vector<arc> arcs;
	vector<int> stations;
	//vector<vector<int>> routes;
	vector<int> arrivalTime;
	vector<int> batteryLevel;
	vector<int> freightLeft;
	vector<int> energyCharged;


public:
	Solution();
	int set(vector<vector<vertex>> s);
	void getRoutes();
	void insertArc(int beg, int end, int value);
	void insertStation(int s);
	void print();
	~Solution();
};
*/

