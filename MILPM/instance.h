#pragma once

#ifndef INSTANCE_H
#define INSTANCE_H

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
#include "Util.h"

using namespace std;

class NodeNotFound : public exception {

private:
	string msg;

public:
	NodeNotFound(int key) {
		msg = "Node with key " + to_string(key) + " not found\n";
	}

	const char * what() const throw () {
		return msg.c_str();
	}
};

class instance
{
public:
	string dir, fileName;
	bool isOpen;
	int type = 0; // store the type of instance that was opened
	vector<node> nodes;
	int numD = 0;
	int numF = 0;
	int numC = 0;
	float Q; // battery capacity
	float c; // Vehicle load capacity
	double r; // battery consumption rate
	float g; // inverse refueling rate
	float v; // average Velocity
	int t = 3; // maximum number of depots
	// used for the uk instances
	int mDim;
	vector<vector<float>> distanceMatrix; // used only for some instances
	vector<edge> edges;
	float maxDist, minDist;
	set<int> st;
	set<int> dp;

	// extra parameters
	int revis = 1; // number of dummy nodes
	int fullRechargeTime, rechargeTime;
	int LB, UB;
	int o; // Maximum number of recharging points to locate.
	int ct; // Swapping time of battery, was defined as ten percent of the total battery charge time by conventional means
	int M;
	int maxD = 0;
	int maxV = 0;
	int maxB = 0;

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
	float bssUseCost = 50; // cost of using the bss

	vector<node> UD0;
	vector<node> UD1;
	vector<node> UD;
	vector<node> C;
	vector<node> R;
	vector<node> S;
	vector<node> V;
	vector<node> C0;
	vector<node> V0;
	vector<node> V1;
	vector<node> V01;

	void setDefaultParameters();
	void readInstace();
	void readPaz();
	void readSSG14();
	void readprplib(); // uk instances	
	void readUKAdapt();
	void addDummyNodes();
	vector<node> chooseStationsLocationRandom(vector<node> &customers);
	vector<node> chooseStationsLocationMCLP(vector<node> &customers);
	vector<node> chooseStationsLocationMCLP2(vector<node>& customers);
	vector<node> removeNodesByIndex(vector<node> customers, set<int> ind);
	void rearrangeDMatrix(vector<vector<float>> &m);
	void createEdgesVector();
	void printNode(node n);

	//void adaptInstance();
	
	void writeAdapt(ostream& file);

	//float dist(node a, node b);

public:
	instance();
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

	vector<node> set_UD0_();
	vector<node> set_UD1_();
	vector<node> set_UD_();
	vector<node> set_C_();
	vector<node> set_R_();
	vector<node> set_SK_(int n);
	vector<node> set_S_();
	vector<node> set_V_();
	vector<node> set_C0_();
	vector<node> set_V0_();
	vector<node> set_V1_();
	vector<node> set_V01_();

	vector<node> sortSet(vector<node> set);

	float dist(int a, int b);
	float dist(node a, node b); // distance between points
	float distEdges(int keya, int keyb); // get the distance between two nodes on the edge list
	float getBatteryUsed(int keya, int keyb); // get the amount of energy used to travel from a to b
	float getTD(node a, node b); // get travel time between nodes a and b
	float getTD(int a, int b); // get travel time between nodes a and b
	float getS(int key);
	float getCT();

	vector<node> vectorUnion(vector<node> a, vector<node> b);

	void print2(ostream & strm);
	void print3(ostream & strm);
	void print(ostream &stream);
	void printSet(vector<node> set);
	node getNodeByKeyN(int key);
	node getNodeByKey(int key);
	void adaptAll();
	void adaptUKInstance(); // uk instances
	void adaptUKInstance_n();
	bool isDepot(int key);
	int getArrival(int key);
	void removeBSS(vector<string> BSS);
	void removeDPT(vector<string> DPT);

	~instance();

	
};

void adaptAll(string dir);
void adaptAll_n();
#endif