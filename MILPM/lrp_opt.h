#pragma once

#include <gurobi_c++.h>
#include <map>
#include <chrono>
#include <BRKGA.h>
#include "perm_rep.h"
#include "instance.h"
#include "Decoder.h"
#include "MTRand.h"

class lrp_opt
{
public:
	string region;
	int pct;
	string dir;	
	set<string> cities;
	map<pair<string, string>, float> distMatrix;
	map<string, int> stFreq;

	vector<string> chooseBSS(int p, set<string> cities, map<pair<string, string>, float> dists, map<string, int> stFreq);
	vector<string> chooseBSS_GA();
	vector<string> chooseBSS_model(set<string> cities, map<pair<string, string>, float> dists, map<string, int> stFreq);

public:
	lrp_opt(string regionName, int pct, string dir, map<string, int> stFreq);
	vector<string> opt_();
	vector<string> opt_brkga();


};

