#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <random>
#include <iomanip>
#include <math.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include "Solution.h"
//#include "MCLPModel.h"

using namespace std;

struct node {
	int key;
	string id, type;
	int x, y, demand;
	int readyTime, dueDate, serviceTime;
	int ref, ref2;
	int id_n = -1; // model's id parameter 
	int ogKey;
};

struct edge {
	int beg, end;
	float value;
};

bool compKey(node a, node b);

class instance
{
public:
	string dir, fileName;
	int type = 0; // store the type of instance that was opened
	vector<node> nodes;
	int numD = 0;
	int numF = 0;
	int numC = 0;
	float Q; // battery capacity
	float C; // Vehicle load capacity
	double r; // battery consumption rate
	float g; // inverse refueling rate
	float v; // average Velocity

	Solution solution;

	// used for the uk instances
	int mDim;
	vector<vector<float>> distanceMatrix; // used only for some instances
	vector<edge> edges;
	float maxDist, minDist;
	set<int> st;

	// extra parameters
	int revis = 1; // number of dummy nodes
	int fullRechargeTime, rechargeTime;
	int LB, UB;
	int o; // Maximum number of recharging points to locate.
	int ct; // Swapping time of battery, was defined as ten percent of the total battery charge time by conventional means
	int M;

	float alfa; // percentagem of customers that will be transformed in stattions siting locations, used on the UK instances

	// parameters for onbective function with cost
	float bssCost; // cost of siting a battery swap station
	float brsCost; // cost of siting a battery recharging station
	double driverWage; // cost per unit travelled
	float vehicleCost; // fixed cost of a vehicle, https://www.theverge.com/2019/9/19/20873947/amazon-electric-delivery-van-rivian-jeff-bezos-order
	float brsEnergyCost; // cost per energy unit in the battery recharging stations
	float bssEnergyCost; // cost per energy unit in the battery swap stations
	float depotCost; // cost of siting a depot
	int stationCap; //station capacity
	float vehicleRange;
	float depotLifetime;
	float bssLifetime;
	float brsLifetime;
	float vehicleLifetime;
	float bssUseCost = 400; // cost of using the bss


	void setDefaultParameters();
	void readInstace();
	void readPaz();
	void readSSG14();
	void readprplib(); // uk instances
	void readUKAdapt();
	void addDummyNodes();
	vector<node> chooseStationsLocationRandom(vector<node> &customers);
	vector<node> chooseStationsLocationMCLP(vector<node> &customers);
	vector<node> removeNodesByIndex(vector<node> customers, set<int> ind);
	void rearrangeDMatrix(vector<vector<float>> &m);
	void createEdgesVector();
	void printNode(node n);

	//float dist(node a, node b);

public:
	instance(string file);
	instance(string dir, string file);
	instance(string dir, string file, int type);

	void test();

	vector<node> set_UD0();
	vector<node> set_UD1();
	vector<node> set_UD();
	vector<node> set_C();
	vector<node> set_R();
	vector<node> set_SK(int n);
	vector<node> set_S();
	vector<node> set_V();
	vector<node> set_C0();
	vector<node> set_V0();
	vector<node> set_V1();
	vector<node> set_V01();

	vector<node> sortSet(vector<node> set);

	float dist(int a, int b);
	float dist(node a, node b); // distance between points
	float distEdges(int keya, int keyb); // get the distance between two nodes on the edge list
	float getTD(node a, node b); // get travel time between nodes a and b
	float getS(int key);
	float getCT();

	void print2(ostream & strm);
	void print3(ostream & strm);
	void print(ostream &stream);
	void printSet(vector<node> set);
	void printSolution(ostream &stream);
	node getNodeByKey(int key);
	~instance();
};