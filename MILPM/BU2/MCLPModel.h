#pragma once
#include <gurobi_c++.h>
#include <vector>
#include <string>
#include "instance.h"

#define TMAX2 120

using namespace std;

class MCLPModel
{
private:
	string fileName;
	string dir;
	
	instance *inst;

	GRBVar P;
	vector<vector<GRBVar>> x;
	vector<GRBVar> y;

	int N;
	int Q;

	void var_p(GRBModel &model);
	void var_x(GRBModel &model);
	void var_y(GRBModel &model);

	void fo(GRBModel &model);
	void fo2(GRBModel & model);
	void fo3(GRBModel &model);
	void eq2(GRBModel &model);
	void eq3(GRBModel &model);
	void eq4(GRBModel &model);
	void eq5(GRBModel &model);
	void eq6(GRBModel &model);

public:
	MCLPModel(string dir, string fileName);
	MCLPModel(instance *inst);
	vector<int> getY();
	void model();
	void setup(GRBModel &model);
	~MCLPModel();
};

