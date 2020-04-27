#include "MCLPModel.h"

void MCLPModel::var_p(GRBModel & model)
{
	P = model.addVar(0, GRB_INFINITY, 1, GRB_INTEGER, "p");
	model.update();
}

void MCLPModel::var_x(GRBModel & model)
{
	x.resize(N);
	for (int i = 0; i < N; i++) {
		x.at(i).resize(N);
		for (int j = 0; j < N; j++) {
			x.at(i).at(j) = model.addVar(0, 1, 1, GRB_BINARY, "x(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
	model.update();
}

void MCLPModel::var_y(GRBModel & model)
{
	y.resize(N);
	for (int j = 0; j < N; j++) {
		y.at(j) = model.addVar(0, 1, 1, GRB_BINARY, "y(" + to_string(j) + ")");
	}
	model.update();
}

void MCLPModel::fo(GRBModel & model)
{
	GRBLinExpr fo = 0;
	for (int j = 0; j < N; j++) {
		fo += y.at(j);
	}
	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

void MCLPModel::fo2(GRBModel & model)
{
	GRBLinExpr fo = P;
	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

void MCLPModel::fo3(GRBModel & model)
{
	GRBLinExpr fo = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i != j) {
				int dij = inst->dist(i, j);
				fo += dij + x.at(i).at(j);
			}
			
		}
	}
	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

void MCLPModel::eq2(GRBModel & model)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N;  j++) {
			int dij = inst->dist(i, j);
			model.addConstr(dij * x.at(i).at(j) <= Q, "c2(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
	model.update();
}

void MCLPModel::eq3(GRBModel & model)
{
	for (int i = 0; i < N; i++) {
		GRBLinExpr e = 0;
		for (int j = 0; j < N; j++) {
			e += x.at(i).at(j);
		}
		model.addConstr(e == 1, "c3(" + to_string(i) + ")");
	}
	model.update();
}

void MCLPModel::eq4(GRBModel & model)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			model.addConstr(x.at(i).at(j) <= y.at(j), "c4(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
	model.update();
}

void MCLPModel::eq5(GRBModel & model)
{
	GRBLinExpr e = 0;
	for (int j = 0; j < N; j++) {
		e += y.at(j);

	}
	model.addConstr(e == P, "c5");
	model.update();
}

void MCLPModel::eq6(GRBModel & model)
{
	model.addConstr(P >= ceil(inst->alfa * N), "c6");
	model.update();
}

MCLPModel::MCLPModel(string dir, string fileName)
{
	this->dir = dir;
	this->fileName = fileName;
	inst = new instance(dir, fileName, 2);

	//inst->print(cout);

	N = inst->nodes.size();
	Q = inst->minDist;

	cout << "Q: " << Q << endl;

	model();
}
/*
MCLPModel::MCLPModel(instance * inst)
{
	this->dir = dir;
	this->fileName = fileName;
	this->inst = inst;

	//inst->print(cout);

	N = inst->nodes.size();
	Q = inst->minDist;

	model();
}
*/
vector<int> MCLPModel::getY()
{
	vector<int> vec;
	for (int i = 0; i < y.size(); i++) {
		if (y.at(i).get(GRB_DoubleAttr_X) != 0) {
			vec.push_back(i);
		}
	}
	return vec;
}

void MCLPModel::model()
{
	GRBEnv env = GRBEnv(true);
	env.start();

	try {
		cout << "Running MCLP\n";
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "MCLP_" + fileName);

		setup(model);

		model.update();

		//model.write(this->directory + this->fileName + ".lp");
		model.write("lp.lp");
		model.getEnv().set(GRB_DoubleParam_TimeLimit, TMAX2);
		model.optimize();
		model.write("sol.sol");

	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

void MCLPModel::setup(GRBModel &model)
{
	var_p(model);
	var_x(model);
	var_y(model);
	fo2(model);
	eq2(model);
	eq3(model);
	eq4(model);
	eq5(model);
	eq6(model);
}

MCLPModel::~MCLPModel()
{

}
