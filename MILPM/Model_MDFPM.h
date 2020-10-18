#pragma once
#include <vector>
#include <string>
#include <map>
#include <set>
#include <gurobi_c++.h>
#include <chrono>
#include "instance.h"

// multi-day=facility pmedian model
class Model_MDFPM
{
private:
	string dir, fileName;
	vector<instance> insts;
	set<string> sCities;
	map<pair<string, string>, int> dists;
	vector<node> depots;
	set<string> depotsList;
	int R;// = 10000; //10000;
	int D;// = 161000;
	int p;// = sCities.size() * 0.15;

	int maxDist = 0, minDist = 0;

	void setup(GRBModel& model);
	void setup_d(GRBModel& model);
	void varX(GRBModel& model);
	GRBVar getX(GRBModel& model, string i, string j, int k);
	void varY(GRBModel& model);
	GRBVar getY(GRBModel& model, string i, int k);
	void varZ(GRBModel& model);
	GRBVar getZ(GRBModel& model, string i);
	void fo(GRBModel& model);
	void c1(GRBModel& model);
	void c2(GRBModel& model);
	void c3(GRBModel& model);
	void c4(GRBModel& model);
	void c4_d(GRBModel& model);
	void c5(GRBModel& model);
	void c6(GRBModel& model);
	void c7(GRBModel& model);
	void c7_d(GRBModel& model);
	void c8_d(GRBModel& model);
	void c9(GRBModel& model);
	void result(GRBModel& model);

public:
	Model_MDFPM(string dir, int size);
	void model();
};

