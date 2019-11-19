#pragma once
#include "instance.h"

using namespace std;

struct vertex {
	int key, bLevel, vLoad, aTime, lTime;
	bool recharge = false;
	int recharged = 0;
};

class Algorithms
{
private:
	instance *inst;

	vector<bool> coverage;
	vector<vector<vertex>> sol; // solution vector

	// greed
	bool allCovered();
	int depotHeuristc();
	bool isCovered(int key);
	int chooseNextNode(vector<vertex> partialRoute);
	int getNearestNode(vector<vertex> route);
	int getNearestFeasibleNode(vector<vertex> route);
	int getNearestStationSite(int key);
	int getNearestCustomerNotCovered(int key);
	int getLowestRTimeNotCovered(vector<vertex> partialRoute);
	vector<int> checkFeasibility(vector<vertex> route, node c);
	vector<vertex> arrangeRoute(vector<vertex> route, int key);
	vector<vertex> addVertexToRoute(vector<vertex> route, int key);
	vector<vertex> addInfVertexToRoute(vector<vertex> route, int key, vector<int> inf);

	void printPartialRoute(vector<vertex> r);

public:
	Algorithms();
	int loadInstance(string dir, string fileName, int type);
	int greed();
	
	void printInstance();
	void printSol();

	~Algorithms();
};

