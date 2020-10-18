#pragma once
#include <vector>
#include <string>
#include <map>
#include <set>
#include <gurobi_c++.h>
#include <chrono>
#include <unordered_map>
#include "instance.h"

class Model_PMedians
{
private:
	string dir, fileName;
	vector<instance> insts;
	vector<node> depots;
	set<string> sCities;
	vector<string> vsDepots;
	map<pair<string, string>, int> dists;

	int R;// = 10000; //10000;
	int D;// = 161000;
	int p;// = sCities.size() * 0.15;
	int maxDist = 0, minDist = 0;

	void setup(GRBModel& model);
	void result(GRBModel& model);

public:
	Model_PMedians(string dir, int size);
	void model();
};

