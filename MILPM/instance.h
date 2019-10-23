#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

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

	// extra parameters
	int revis = 1; // number of dummy nodes
	int fullRechargeTime, rechargeTime;
	int LB, UB;
	int o; // Maximum number of recharging points to locate.
	int ct; // Swapping time of battery, was defined as ten percent of the total battery charge time by conventional means
	int M;

	float bssc = 10000;
	float brsc = 1000;
	float cput = 2;
	float vehicleFixedCost = 1000;
	float costPerEnergyUnitA = 5;
	float costPerEnergyUnitb = 2;
	float depotCost = 2000;
	int stationCap = 2;

	void readInstace();
	void readPaz();
	void readSSG14();
	void readprplib(); // uk instances

	void addDummyNodes();
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

