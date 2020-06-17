#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
//#include "instance.h"
#include "Util.h"

using namespace std;
using boost::property_tree::ptree;
using boost::property_tree::write_xml;
using boost::property_tree::xml_writer_settings;

struct arc {
	int beg, end;
	double value, value2;
};

struct edge {
	int beg, end;
	float value;
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

struct vertex {
	node n;

	int key, bLevel, vLoad, aTime, lTime, wTime = 0;
	bool recharge = false;
	int recharged = 0;
};

typedef vector<vertex> route;
typedef vector<route> routes;

struct item {
	int key;
	int nodeKey;
};
typedef vector<int> permutation;

class Solution
{
public:
	string generator;
	string instance;

	double FO;
	double FO_;
	vector<float> FOp;
	vector<string> inf;
	vector<vector<vertex>> routes;
	vector<float> routeCost;
	set<int> stations;
	permutation perm;
	double runtime;
	double FOINIT;
	int status;
	double gap;

	int numVehicles = 0;
	long long int totalDistance = 0;
	long long int totalTime;
	vector<arc> arcs;

	arc getArc(int a, int b);


public:
	Solution();

	void strmSol(ostream& strm);
	void printSol();
	void saveXML(string file);
	void strmFO(ostream& strm);

	//

	int set(vector<vector<vertex>> s);
	void getRoutes();
	void insertArc(int beg, int end, int value);

	// aux
	void debug(ostream& strm);

	~Solution();
};

bool compIndividual(Solution a, Solution b);
bool compRT(node a, node b);
bool compDD(node a, node b);
bool compKey(node a, node b);

