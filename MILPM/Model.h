#pragma once
#include <numeric>
#include <gurobi_c++.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "instance.h"
#include "Util.h"
#include "perm_rep.h"

#define TMAX 3600

using namespace std;

class Model	
{
private:
	long long duration;

	Solution solu;
	Solution init;
	bool initProv = false;

	string directory, fileName, dirOutput = "";
	instance *inst; // data
	int w = 6; // which model will be runned
	int vehiclesNum = 0;

	// extra parameters
	long unsigned int M;

	// decision variables
	vector<vector<GRBVar>> x;
	vector<GRBVar> y;
	vector<GRBVar> t;
	vector<GRBVar> d;
	vector<GRBVar> q;
	vector<GRBVar> u;

	void var_x(GRBModel &model);
	void var_y_model1(GRBModel &model);
	void var_y_model2(GRBModel &model);
	void var_y_model4(GRBModel& model);
	void var_t(GRBModel &model);
	void var_d(GRBModel &model);
	void var_q(GRBModel &model);
	void var_u(GRBModel &model);
	void var_w_model2(GRBModel &model);
	void var_w_model3(GRBModel &model);
	void var_z(GRBModel &model);
	void var_v(GRBModel& model);

	// objective function
	void fo(GRBModel &model);
	void fo_cost(GRBModel &model);

	// fo parts
	void depotCostFO(GRBModel &model);	// depot cost
	void bssCostFO(GRBModel& model);	// bss cost
	void vehicleFixedCostFO(GRBModel& model);	// vehicle fixed cost
	void drivingCostFO(GRBModel& model);	// driving cost
	void brsEnergyCostFO(GRBModel& model);	// energy cost in brs
	void bssEnergyCostFO(GRBModel& model);	// energy cost in bss
	void bssUseCostFO(GRBModel& model);
	void fo_cost_3(GRBModel& model);

	void fo_cost_2(GRBModel &model);

	// constraints

	// model 1	
	void c2(GRBModel &model); // from every vertex should be leaving only one arc
	void c3(GRBModel &model); // 
	void c4(GRBModel &model); // 
	void c5(GRBModel &model); // 
	void c6(GRBModel &model); // 
	void c7(GRBModel &model); // verify
	void c7_M(GRBModel &model); // 
	void c8(GRBModel &model); // 
	void c8_M(GRBModel &model); // 
	void c9(GRBModel &model); // 
	void c9_M(GRBModel &model); // 
	void c10(GRBModel &model); // 
	void c10_M(GRBModel &model); //
	void c11(GRBModel &model); // 
	void c11_M(GRBModel &model); // 
	void c12(GRBModel &model); // 
	void c13(GRBModel &model); // 
	void c13_M(GRBModel &model); // 
	void c14(GRBModel &model); // 
	void c14_M(GRBModel &model); // 
	void c15(GRBModel &model); // 
	void c16(GRBModel &model); // 
	void c16_M(GRBModel &model); // 
	void c165_M(GRBModel& model);
	void c17(GRBModel &model); // 
	void c18(GRBModel &model); // 
	void c18_M(GRBModel &model); // 
	void c19(GRBModel &model); // 
	void c19_M(GRBModel &model); // 
	void c20(GRBModel &model); // 
	void c20_M(GRBModel &model); // 
	void c21(GRBModel &model); // 
	void c21_M(GRBModel &model); // 
	void c22(GRBModel &model); // 

	// model 2
	void c27(GRBModel &model); // 
	void c27_model4(GRBModel& model);
	void c28(GRBModel &model); // 
	void c28_M(GRBModel &model); // 
	void c29(GRBModel &model); // 
	void c29_M(GRBModel &model); // 
	void c29_M_model4(GRBModel& model);
	void c30(GRBModel &model); // 
	void c30_M(GRBModel &model); // 
	void c31(GRBModel &model); // 
	void c31_M(GRBModel &model); // 
	void c32(GRBModel &model); // 
	void c32_M(GRBModel &model); // 
	void c33(GRBModel &model); // 

	// model 3
	void var_y_model3(GRBModel& model);

	void c36(GRBModel &model); // 
	void c36_M(GRBModel &model); // 
	void c36_M_model4(GRBModel& model);
	void c37(GRBModel &model); // 
	void c37_M(GRBModel &model); // 
	void c37_M_model4(GRBModel& model);
	void c38(GRBModel &model); // 
	void c38_M(GRBModel &model); // 
	void c39(GRBModel &model); // 
	void c39_M(GRBModel &model); // 
	void c40(GRBModel &model); // 
	void c40_M(GRBModel &model); // 
	void c41(GRBModel &model); // 

	void c43(GRBModel &model); // 
	void c44(GRBModel &model); // 

	// test

	GRBVar	getS(GRBModel& model, int key);
	void var_f(GRBModel& model);
	GRBVar getF(GRBModel& model, int keya, int keyb);
	void var_s(GRBModel& model);

	void r1(GRBModel &model);
	void r2(GRBModel& model);
	void r3(GRBModel& model);

	void r4(GRBModel& model);

	// model 4

	void c45(GRBModel &model); // determine which depots where opened

	void c46(GRBModel &model); // stations limit
	void c47(GRBModel& model);

	void c48(GRBModel& model);

	void c49(GRBModel& model); // limiting number of depots

	void c50(GRBModel& model); // limit vehicle number

	// extra functions
	GRBVar	getX	(GRBModel &model, int keya, int keyb);
	GRBVar	getY	(GRBModel &model, int key);
	GRBVar	getT	(GRBModel &model, int key);
	GRBVar	getQ	(GRBModel &model, int key);
	GRBVar	getD	(GRBModel &model, int key);
	GRBVar	getU	(GRBModel &model, int key);
	GRBVar	getW	(GRBModel &model, int key);
	GRBVar	getZ	(GRBModel &model, int key);
	GRBVar	getV	(GRBModel &model, int key);
	vector<node> vectorUnion	(vector<node> a, vector<node> b); // union between two vectors of nodes, the vectors are trated as sets
	vector<node> vectorSub		(vector<node> a, vector<node> b);
	vector<float> getFOParcels(GRBModel &model);
	int getNumVehicles(GRBModel &model);

	//
	//void getRoutes();

	Solution getSolution(GRBModel& model);
	routes getRoutes2(GRBModel& model); // return the routes

	//vector<string> eval(GRBModel &model);
	vector<string> eval(Solution s);

	void model1_indC(GRBModel& model);
	void model1_bigM(GRBModel& model);
	void model2_indC(GRBModel& model);
	void model2_bigM(GRBModel& model);
	void model3_indC(GRBModel& model);
	void model3_bigM(GRBModel& model);
	void model4(GRBModel& model);
	void model_SBPO(GRBModel& model);
	void model_test(GRBModel& model);
	void setup(GRBModel& model);
	void result(GRBModel& model);
	Solution model();
	Solution model(Solution s);
	void setInitialSolution(GRBModel& model, Solution s);
	void setInitialStations(GRBModel& model);

	// aux 
	int getArcs(GRBModel& model);
	float getTravelCost(GRBModel& model);

public:
	string	row = "";
			Model	(string fileName);
			Model	(string dir, string fileName);
			Model	(string dir, string fileName, int w);
			Model	(string dir, string fileName, int w, int t);
			Model	(string dir, string fileName, int w, string dirOutput);
			Model	(string dir, string fileName, int w, int t, string dirOutput);
	
	void		getRow		(GRBModel &model);	
	Solution	getSolution	();
	Solution	optmize();
	Solution	optmize(int model);
	void		setIS(Solution s);
	Solution	analyze(Solution s);
	void		printInst();
	bool fileOpened();
	~Model();
};