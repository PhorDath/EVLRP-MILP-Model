#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <random>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <boost/foreach.hpp>

using namespace std;

struct node{
	int key;
	string id, type;
	int x, y, demand, readyTime, dueDate, serviceTime;
	int ref, ref2;
	int id_n; // model's id parameter 
};

struct edge {
	int b, e;
	float value;
};

bool compKey(node a, node b);

class instance
{
public:
	string dir, fileName;
	vector<node> nodes;
	int numD = 0;
	int numF = 0;
	int numC = 0;
	float Q; // battery capacity
	float C; // Vehicle load capacity
	float r; // battery consumption rate
	float g; // inverse refueling rate
	float v; // average Velocity

	// used for the uk instances
	vector<vector<float>> distanceMatrix; // used only for some instances
	vector<edge> edges;

	// extra parameters
	int revis = 1; // number of dummy nodes
	int fullRechargeTime, rechargeTime;
	int LB, UB;
	int o; // Maximum number of recharging points to locate.
	int ct; // Swapping time of battery, was defined as ten percent of the total battery charge time by conventional means
	int M;

	float bssc = 10000; // cost of siting a battery swap station
	float brsc = 1000; // cost of siting a battery recharging station
	float cput = 2; // cost per unit travelled
	float vehicleFixedCost = 1000; // fixed cost of a vehicle
	float costPerEnergyUnitA = 5; // cost per energy unit in the battery recharging stations
	float costPerEnergyUnitb = 2; // cost per energy unit in the battery swap stations
	float depotCost = 2000; // cost of siting a depot
	int stationCap = 2; //station capacity

	float alfa = 0.2; // percentagem of customers that will be transformed in stattions siting locations, used on the UK instances

	void readInstace();
	void readPaz();
	void readSSG14();
	void readprplib(); // uk instances

	void addDummyNodes();
	vector<node> chooseStationsLocation(vector<node> &customers);
	vector<node> removeNodesByIndex(vector<node> customers, set<int> ind);
	void rearrangeDMatrix(vector<vector<float>> &m);
	void printNode(node n);

public:
	instance(string file);
	instance(string dir, string file);
	instance(string dir, string file, int t);

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

	void print();
	void printSet(vector<node> set);
	~instance();
};

