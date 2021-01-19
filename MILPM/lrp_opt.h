#pragma once

#include <gurobi_c++.h>
#include <map>
#include <chrono>
#include <boost/filesystem.hpp>
#include <BRKGA.h>
//#include <brkga_mp_ipr.hpp>
#include "perm_rep.h"
#include "instance.h"
#include "Decoder.h"
#include "MTRand.h"
#include "perm_rep.h"

class lrp_opt
{
public:
	string region;
	
	string dir;	
	set<string> cities;
	map<pair<string, string>, float> distMatrix;
	map<string, int> stFreq;
	map<string, int> dpFreq;

	string getDate();
	float totalCost(vector<Solution> sols, string dir, bool write);
	float totalCost_Desarmotized(vector<Solution> sols, string dir, bool write);

	vector<string> chooseBSS(int p, set<string> cities, map<pair<string, string>, float> dists, map<string, int> stFreq);
	vector<string> chooseBSS_GA();
	vector<string> chooseBSS_BRKGA();
	vector<string> chooseBSS_model(set<string> cities, map<pair<string, string>, float> dists, map<string, int> stFreq);

	vector<Solution> vns(string dir1, string output);
	vector<Solution> vnsl(string dir1, string output, vector<string> BSSs, vector<string> DPTs);

public:
	int pct;
	vector<string> BSSs, DPTs;
	lrp_opt(string dir, string regionName, int pct);
	lrp_opt(string regionName, int pct, string dir, map<string, int> stFreq);
	lrp_opt(string regionName, int pct, string dir, map<string, int> stFreq, map<string, int> dpFreq);
	bool opt();
	bool opt_bss();
	vector<string> opt_brkga();


};

