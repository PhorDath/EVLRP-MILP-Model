#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct arc{
	int beg, end;
	double value;
};

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

	vector<arc> arcs;
	vector<int> stations;
	vector<vector<int>> routes;
	vector<int> arrivalTime;
	vector<int> batteryLevel;
	vector<int> freightLeft;
	vector<int> energyCharged;


public:
	Solution();
	void getRoutes();
	void insertArc(int beg, int end, int value);
	void insertStation(int s);
	void print();
	~Solution();
};

