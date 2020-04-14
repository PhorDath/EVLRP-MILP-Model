#include "Model.h"

Model::Model(string fileName)
{
	this->directory = "";
	this->fileName = fileName;
	inst = new instance("", fileName);
	M = inst->M;
	model();
}

Model::Model(string dir, string fileName)
{
	this->directory = dir;
	this->fileName = fileName;
	inst = new instance(dir, fileName);
	M = inst->M;
	model();
}

Model::Model(string dir, string fileName, int w)
{
	this->directory = dir;
	this->fileName = fileName;
	inst = new instance(dir, fileName);
	this->w = w;
	M = inst->M;
	model();
}

Model::Model(string dir, string fileName, int w, int t)
{
	this->directory = dir;
	this->fileName = fileName;
	inst = new instance(dir, fileName, t);
	this->w = w;
	M = inst->M;
	model();
}

Model::Model(string dir, string fileName, int w, string dirOutput)
{
	this->directory = dir;
	this->fileName = fileName;
	inst = new instance(dir, fileName);
	this->w = w;
	M = inst->M;
	this->dirOutput = dirOutput;
	model();
}

Model::Model(string dir, string fileName, int w, int t, string dirOutput)
{
	this->directory = dir;
	this->fileName = fileName;

	inst = new instance(dir, fileName, t);

	this->w = w;
	M = inst->M;
	this->dirOutput = dirOutput;
	model();
}

void Model::model1_indC(GRBModel & model)
{
	string newName = "model1_" + fileName + "_indC";
	model.set(GRB_StringAttr_ModelName, newName);

	var_x(model);
	var_y_model1(model);
	var_t(model);
	var_d(model);
	var_q(model);
	var_u(model);
	fo(model);
	c2(model);
	c3(model);
	c4(model);
	c5(model);
	c6(model);
	c7(model);
	c8(model);
	c9(model);
	c10(model);
	c11(model);
	c12(model);
	c13(model);
	c14(model);
	c15(model);
	c16(model);
	c17(model);
	c18(model);
	c19(model);
	c20(model);
	c21(model);
	c22(model);
	c43(model);
	c44(model);
}

void Model::model1_bigM(GRBModel & model)
{
	string newName = "model1_" + fileName + "_bigM";
	model.set(GRB_StringAttr_ModelName, newName);

	var_x(model);
	var_y_model1(model);
	var_t(model);
	var_d(model);
	var_q(model);
	var_u(model);
	fo(model);
	c2(model);
	c3(model);
	c4(model);
	c5(model);
	c6(model);
	c7_M(model);
	c8_M(model);
	c9_M(model);
	c10_M(model);
	c11_M(model);
	c12(model);
	c13_M(model);
	c14_M(model);
	c15(model);
	c16_M(model);
	c17(model);
	c18_M(model);
	c19_M(model);
	c20_M(model);
	c21_M(model);
	c22(model);
	c43(model);
	c44(model);
}

void Model::model2_indC(GRBModel & model)
{
	string newName = "model2_" + fileName + "_indC";
	model.set(GRB_StringAttr_ModelName, newName);

	var_x(model);
	var_y_model2(model);
	var_t(model);
	var_d(model);
	var_q(model);
	var_u(model);
	var_w_model2(model);
	fo(model);	
	c2(model);	
	c3(model);
	c4(model);
	c5(model);
	c9(model);
	c12(model);
	c15(model);
	c16(model);
	c17(model);
	c18(model);
	c19(model);
	c20(model);
	c21(model);
	c22(model);
	c43(model);
	c44(model);
	c27(model);
	c28(model);
	c29(model);
	c30(model);
	c31(model);
	c32(model);
	c33(model);
}

void Model::model2_bigM(GRBModel & model)
{
	string newName = "model2_" + fileName + "_bigM";
	model.set(GRB_StringAttr_ModelName, newName);

	var_x(model);
	var_y_model2(model);
	var_t(model);
	var_d(model);
	var_q(model);
	var_u(model);
	var_w_model2(model);
	fo(model);
	c2(model);
	c3(model);
	c4(model);
	c5(model);
	c9_M(model);
	c12(model);
	c15(model);
	c16_M(model);
	c17(model);
	c18_M(model);
	c19_M(model);
	c20_M(model);
	c21_M(model);
	c22(model);
	c43(model);
	c44(model);
	c27(model);
	c28_M(model);
	c29_M(model);
	c30_M(model);
	c31_M(model);
	c32_M(model);
	c33(model);
}

void Model::model3_indC(GRBModel & model)
{
	string newName = "model3_" + fileName + "_indC";
	model.set(GRB_StringAttr_ModelName, newName);

	var_x(model);
	var_y_model2(model);
	var_t(model);
	var_d(model);
	var_q(model);
	var_u(model);
	var_w_model3(model);

	fo(model);

	c2(model);
	c3(model);
	c4(model);
	c5(model);
	c7(model);
	c8(model);
	c9(model);
	c11(model);
	c12(model);
	c14(model);
	c15(model);
	c16(model);
	c17(model);
	c18(model);
	c19(model);
	c20(model);
	c21(model);
	c22(model);
	c43(model);
	c44(model);

	c27(model);

	c36(model);
	c37(model);
	c38(model);
	c39(model);
	c40(model);
	c41(model);

}

void Model::model3_bigM(GRBModel & model)
{
	string newName = "model3_" + fileName + "_bigM";
	model.set(GRB_StringAttr_ModelName, newName);

	var_x(model);
	var_y_model2(model);
	var_t(model);
	var_d(model);
	var_q(model);
	var_u(model);
	var_w_model3(model);

	fo(model);

	c2(model);
	c3(model);
	c4(model);
	c5(model);
	c7_M(model);
	c8_M(model);
	c9_M(model);
	c11_M(model);
	c12(model);
	c14_M(model);
	c15(model);
	c16_M(model);
	c17(model);
	c18_M(model);
	c19_M(model);
	c20_M(model);
	c21_M(model);
	c22(model);
	c43(model);
	c44(model);

	c27(model);

	c36_M(model);
	c37_M(model);
	c38_M(model);
	c39_M(model);
	c40_M(model);
	c41(model);
}

/*
	var_x(model);
	var_y_model2(model);
	var_t(model);
	var_d(model);
	var_q(model);
	var_u(model);
	var_w_model3(model);
	var_z(model);
	fo_cost(model);
	c2(model);
	c3(model);
	c4(model);
	c5(model);
	c7(model);
	c8_M(model);
	c9_M(model);
	c11_M(model);
	c12(model);
	c14_M(model);
	c15(model);
	c16_M(model);
	c17(model);
	c18_M(model);
	c19_M(model);
	c20_M(model);
	c21_M(model);
	c22(model);
	c43(model);
	c44(model);

	c27(model);

	c36_M(model);
	c37_M(model);
	c38_M(model);
	c39_M(model);
	c40_M(model);
	c41(model);

	c45(model);
	c46(model);
*/

void Model::model4(GRBModel & model)
{
	string newName = "model4_" + fileName;
	model.set(GRB_StringAttr_ModelName, newName);

	var_x(model);
	var_y_model2(model);
	var_t(model);
	var_d(model);
	var_q(model);
	var_u(model);
	var_w_model3(model); // var_w_model3(model);
	var_z(model);

	fo_cost(model);


	c2(model);
	c3(model);
	c4(model);
	c5(model);
	c7(model);
	c8_M(model);
	c9_M(model);
	c11_M(model);
	c12(model);
	c14_M(model);
	c15(model);
	c16_M(model);
	c17(model);
	c18_M(model);
	c19_M(model);
	c20_M(model);
	c21_M(model);
	c22(model);
	c43(model);
	c44(model);

	c27(model);

	c36_M(model);
	c37_M(model);
	c38_M(model);
	c39_M(model);
	c40_M(model);
	c41(model);

	c45(model);
	c46(model);

	/*
	c2(model);
	c3(model);
	c4(model);
	c5(model);
	c7(model);
	c8(model);
	c9(model);
	c11(model);
	c12(model);
	c14(model);
	c15(model);
	c16(model);
	c17(model);
	c18(model);
	c19(model);
	c20(model);
	c21(model);
	c22(model);
	c43(model);
	c44(model);

	c27(model);

	c36(model);
	c37(model);
	c38(model);
	c39(model);
	c40(model);
	c41(model);

	c45(model);
	c46(model);
	*/
}

void Model::setup(GRBModel &model)
{
	if (w == 0) {
		model4(model);
	}
	else if (w == 1) {
		model1_bigM(model);
	}
	else if (w == 2) {
		model1_bigM(model);
	}
	else if (w == 3) {
		model2_indC(model);
	}
	else if (w == 4) {
		model2_bigM(model);
	}
	else if (w == 5) {
		model3_indC(model);
	}
	else if (w == 6) {
		model3_bigM(model);
	}
}

void Model::result(GRBModel & model)
{
	string fN = dirOutput + model.get(GRB_StringAttr_ModelName) + ".sol";
	if (dirOutput == "") {
		boost::filesystem::create_directory(directory + "output/");
		fN = directory + "output/" + model.get(GRB_StringAttr_ModelName) + ".sol";
	}

	int status = model.get(GRB_IntAttr_Status);
	if (status == GRB_UNBOUNDED) {
		cout << "The model can't be solved because there is no bound" << endl;
		model.write(fN);
	}
	else if (status == GRB_OPTIMAL) {
		cout << "Optimal solution found!" << endl;
		cout << "The objective function value is " << model.get(GRB_DoubleAttr_ObjVal) << endl;
		model.write(fN);
	}
	else if (status == GRB_TIME_LIMIT) {
		cout << "Limit time reached!" << endl;
		cout << "The best solution found is " << model.get(GRB_DoubleAttr_ObjVal) << endl;
		model.write(fN);
	}
	else if (status == GRB_INFEASIBLE) {
		cout << "The model is infeasible" << endl;
		model.write(fN);
	}
	else {
		cout << "Status: " << status << endl;
		model.write(fN);
	}
	getRow(model);

	// write more information in the solution file
	fstream file;
	file.open(fN, ios::out | ios::app);
	if (file.is_open() == false) {
		cout << "Error opening the solution file to write more information" << endl;
	}
	else {
		file << "#" << endl;

		auto V01 = inst->set_V01();
		V01 = inst->sortSet(V01);

		file << V01.size() << endl;
		for (auto i : V01) {
			file << i.key << " " << i.x << " " << i.y << " " << i.type << " " << i.ref2 << endl;
		}

		inst->solution.FO = model.get(GRB_DoubleAttr_ObjVal);
		inst->solution.runTime = model.get(GRB_DoubleAttr_Runtime);
		inst->solution.gap = model.get(GRB_DoubleAttr_MIPGap) * 100;
		inst->solution.status = model.get(GRB_IntAttr_Status);
		if (w == 0) {
			vector<float> costs = getFOParcels(model);
			inst->solution.dCost = costs.at(0); // depot cost
			inst->solution.sCost = costs.at(1); // stations cost
			inst->solution.dwCost = costs.at(2); // driver wage cost
			inst->solution.vCost = costs.at(3); // vehicle cost
			inst->solution.eCost = costs.at(4); // energy cost		
			inst->solution.bssUseCost = costs.at(5);
		}
		inst->solution.numVehicles = getNumVehicles(model);
		
		getSolution(model);
		inst->printSolution(cout);
		inst->printSolution(file);
		inst->print(file);

		/*
		vector<node> C = inst->set_C();
		vector<node> R = inst->set_R();

		vector<node> CUR = vectorUnion(C, R);
		for (node n : CUR) {
			GRBVar y = getY(model, n.key);
			if (y.get(GRB_DoubleAttr_X) != 0) {
				file << n.key << " ";
			}
		}
		file << endl << "Arcs used: \n";
		vector<node> V0 = inst->set_V0();
		vector<node> V1 = inst->set_V1();

		for (auto i : V0) {
			for (auto j : V1) {
				if (i.key != j.key) {
					GRBVar xij = getX(model, i.key, j.key);

					if (xij.get(GRB_DoubleAttr_X) != 0) {
						file << "(" << i.key << ", " << j.key << ")" << endl;
					}
				}				
			}
		}*/
		
		file.close();
		

	}

	cout << "Generating image\n";
	// call python script to generate visual representation
	char name[] = "D:/Victor/Pos-Graduacao/UFV/Research/Implementation/Model/MDEVLRPTW-BSPR/Dataviz/main";
	string dir = "D:/Victor/Pos-Graduacao/UFV/Research/Implementation/Model/MDEVLRPTW-BSPR/Dataviz/";
	dir = "";
	string f = "main";
	string func = "draw";
	string arg = fN;
	
	//Util::drawGraph(dir, f, func, arg);

}

struct x {
	int a, b;
};

/*
void Model::getSolution(GRBModel & model)
{
	// get all xij != 0
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();

	for (auto i : V0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar xij = getX(model, i.key, j.key);
				if (xij.get(GRB_DoubleAttr_X) != 0) {

				}
			}
		}
	}

	auto R = inst->set_R();
	auto C = inst->set_C();
	auto CUR = vectorUnion(C, R);

	vector<int> y;
	for (auto i : CUR) {
		GRBVar yi = getY(model, i.key);
		if (yi.get(GRB_DoubleAttr_X) != 0) {
			y.push_back(i.key);
		}
	}
}
*/

void Model::getRow(GRBModel & model)
{
	string res = "";
	res += model.get(GRB_StringAttr_ModelName) + " ; ";
	res += to_string(model.get(GRB_IntAttr_Status)) + " ; ";
	res += to_string(model.get(GRB_DoubleAttr_ObjVal)) + " ; "; 
	res += to_string(model.get(GRB_DoubleAttr_MIPGap) * 100) + " ; ";
	res += to_string(model.get(GRB_DoubleAttr_Runtime));
	row = res;
}

void Model::model()
{
	inst->print3(cout);

	GRBEnv env = GRBEnv(true);
	env.start();

	try {
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "ELRP_" + fileName);

		setup(model);

		model.update();

		//model.write(this->directory + this->fileName + ".lp");
		model.write("lp.lp");
		model.getEnv().set(GRB_DoubleParam_TimeLimit, TMAX);
		model.optimize();
		
		result(model);
		
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}

}

void Model::print()
{
}

vector<node> Model::vectorUnion(vector<node> a, vector<node> b) 
{
	bool isin;
	for (auto i : b) {
		isin = false;
		for (auto j : a) {
			if (i.key == j.key) {
				isin = true;
				break;
			}
		}
		if (isin == false) {
			a.push_back(i);
		}
	}

	return a;
}

vector<node> Model::vectorSub(vector<node> a, vector<node> b)
{
	vector<node> res;
	for (auto i : a) {
		bool isin = false;
		for (auto j : b) {
			if (i.key == j.key) {
				isin = true;
			}
		}

		if (isin == false) {
			res.push_back(i);
		}
	}
	return res;
}

vector<float> Model::getFOParcels(GRBModel &model)
{
	float dCost = 0; // depot cost
	float sCost = 0; // stations cost
	float dwCost = 0; // driver wage cost
	float vCost = 0; // vehicle cost
	float eCost = 0; // energy cost
	float bssUseCost = 0;
	   
	auto UD0 = inst->set_UD0();
	auto UD1 = inst->set_UD1();
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();

	// depot location cost
	for (auto i : UD0) {
		dCost += getZ(model, i.key).get(GRB_DoubleAttr_X) * (inst->depotCost / inst->numC);
	}

	// bss and brs location cost
	for (auto i : C) {
		GRBVar yi = getY(model, i.key);
		sCost += (inst->brsCost / inst->numC) * yi.get(GRB_DoubleAttr_X);
	}
	for (auto i : R) {
		GRBVar yi = getY(model, i.key);
		sCost += (inst->bssCost / inst->numC) * yi.get(GRB_DoubleAttr_X);
	}


	// driving cost
	for (auto i : V0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				//string var = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";
				GRBVar xij = getX(model, i.key, j.key);
				dwCost += inst->driverWage * inst->dist(i, j) * xij.get(GRB_DoubleAttr_X); // model.getVarByName(var);
			}
		}
	}

	// vehicle fixed cost
	for (auto i : UD0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar xij = getX(model, i.key, j.key);
				vCost += (inst->vehicleCost / inst->numC) * xij.get(GRB_DoubleAttr_X);
			}
		}
	}

	// energy cost
	vector<node> SK;
	for (auto h : C) {
		auto SKc = inst->set_SK(h.key);
		SK.insert(SK.end(), SKc.begin(), SKc.end());
	}

	auto CUSKc = vectorUnion(C, SK);

	for (auto i : CUSKc) {
		GRBVar wi = getW(model, i.key);

		eCost += inst->brsEnergyCost * wi.get(GRB_DoubleAttr_X);
	}

	// cost of using a bss (energy + operation cost)
	// operational cost
	for (node i : V0) {


		auto S = inst->set_S();

		vector<node> RUS = vectorUnion(R, S);

		for (node j : RUS) {
			if (i.key != j.key) {
				GRBVar xij = getX(model, i.key, j.key);
				bssUseCost += xij.get(GRB_DoubleAttr_X) * inst->bssUseCost;
			}
		}

	}
	
	// cost of the energy recharged at the depot in the end of the route
	for (node i : UD1) {
		GRBVar qi = getQ(model, i.key);
		eCost += qi.get(GRB_DoubleAttr_X) * inst->bssCost;
	}

	return { dCost, sCost, dwCost, vCost, eCost , bssUseCost};
}

int Model::getNumVehicles(GRBModel & model)
{
	vector<node> UD0 = inst->set_UD0();
	vector<node> V1 = inst->set_V1();

	vehiclesNum = 0;
	for (node a : UD0) {
		for (node b : V1) {
			GRBVar xij = getX(model, a.key, b.key);
			vehiclesNum += xij.get(GRB_DoubleAttr_X);
		}		
	}
	return vehiclesNum;
}

void Model::getSolution(GRBModel &model)
{
	if(w == 0)
		getFOParcels(model);

	vector<node> V0 = inst->set_V0();
	vector<node> V1 = inst->set_V1();
	vector<node> V01 = inst->set_V01();
	vector<node> V = inst->set_V();
	vector<node> C = inst->set_C();
	vector<node> R = inst->set_R();
	vector<node> CUR = vectorUnion(C, R);

	int totalV = V01.size();

	// put used arcs in the solution
	for (auto i : V0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar xij = getX(model, i.key, j.key);
				float value = xij.get(GRB_DoubleAttr_X);
				if (value < 0.1 && value > -0.1) {
					value = 0;
				}
				else{
					value = 1;
				}

				if (value != 0) {
					inst->solution.insertArc(i.key, j.key, inst->dist(i, j));
				}
			}
		}
	}

	// generate the routes
	getRoutes();

	// put sited stations in the solution
	for (auto i : CUR) {
		GRBVar yi = getY(model, i.key);
		if (yi.get(GRB_DoubleAttr_X) != 0) {
			inst->solution.insertStation(i.key);
		}
	}

	// put the Moment of time in which a vehicle arrives
	inst->solution.arrivalTime.resize(totalV, 0);
	for (node i : V01) {
		GRBVar ti = getT(model, i.key);

		inst->solution.arrivalTime.at(i.key) = ti.get(GRB_DoubleAttr_X);

	}

	// put the Amount of freight left when the vehicle arrives
	inst->solution.freightLeft.resize(totalV, 0);
	for (auto i : V01) {
		GRBVar di = getD(model, i.key);
		inst->solution.freightLeft.at(i.key) = di.get(GRB_DoubleAttr_X);
	}

	// put the Amount of energy left in the battery when the vehicle arrives
	inst->solution.batteryLevel.resize(totalV, 0);
	for (auto i : V01) {
		GRBVar qi = getQ(model, i.key);
		inst->solution.batteryLevel.at(i.key) = qi.get(GRB_DoubleAttr_X);
	}

	vector<node> SK;
	for (auto h : C) {
		auto SKc = inst->set_SK(h.key);
		SK.insert(SK.end(), SKc.begin(), SKc.end());
	}

	auto CUSKc = vectorUnion(C, SK);
	// Amount of energy to be charged
	inst->solution.energyCharged.resize(totalV, 0);
	for (auto i : CUSKc) {
		GRBVar wi = getW(model, i.key);
		inst->solution.energyCharged.at(i.key) = wi.get(GRB_DoubleAttr_X);
	}
}

void Model::getRoutes()
{
	vector<arc> arcs = inst->solution.arcs;


	// get the depots
	vector<int> depots;
	for (node i : inst->nodes) {
		if (i.type == "d") {
			depots.push_back(i.key);
		}
	}

	vector<int> pos;
	// get the inital arc of each route
	for (int dpt : depots) { // for each depot
		int count = 0;
		for (arc i : arcs) { // for each arc
		//for (int i = 0; i < arcs.size(); i++) { // for each arc
			//arc a = arcs.at(i);
			arc a = i;
			if (a.beg == dpt) { // if the arc begins in the current depot
				// vector<int> route = {a.beg, a.end};
				vertex va;
				va.key = a.beg;
				vertex vb;
				vb.key = a.end;

				vector<vertex> route = { va, vb };

				inst->solution.routes.push_back(route); // add the inital arc in each route
				//inst->solution.arcs.erase(inst->solution.arcs.begin() + i); // erase the arc
				//arcs.erase(arcs.begin() + count); // erase the arc
				pos.push_back(count);
				
			}
			count++;
		}
	}

	sort(pos.begin(), pos.end(), greater<int>());
	// erase all depots arcs
	for (int i : pos) {
		arcs.erase(arcs.begin() + i);
	}

	// get the rest of the route
	while (arcs.size() > 0) {
		for (int b = 0; b < arcs.size(); b++) { // for each arc


			arc a = arcs.at(b);
			
			//  search in the current built routes if its time to add the arc a
			for (int i = 0; i < inst->solution.routes.size(); i++) {
				bool found = false;
				for (int j = 0; j < inst->solution.routes.at(i).size(); j++) {
					if (inst->solution.routes.at(i).at(j).key == a.beg) { // if the current end of the route is equal to the begin of the arc
						vertex va;
						va.key = a.end;
						inst->solution.routes.at(i).push_back(va); // add the end of the arc in the route
						arcs.erase(arcs.begin() + b); // erase the arc
						found = true;
						break;
					}
				}
				if (found == true) {
					break;
				}
			}
		}
	}
}

Model::~Model()
{
}

void Model::var_x(GRBModel & model)
{
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();

	for (auto i : V0) {
		vector<GRBVar> aux;
		for (auto j : V1) {
			if (i.key != j.key) {
				string varName = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";
				aux.push_back(model.addVar(0, 1, 1, GRB_BINARY, varName));
			}
		}
		x.push_back(aux);
	}
	model.update();
}

void Model::var_y_model1(GRBModel & model)
{
	auto set_R = inst->set_R();

	for (auto i : set_R) {
		string varName = "y(" + to_string(i.key) + ")";
		y.push_back(model.addVar(0, 1, 1, GRB_BINARY, varName));
	}
	model.update();
}

void Model::var_y_model2(GRBModel & model)
{
	auto R = inst->set_R();
	auto C = inst->set_C();
	auto CUR = vectorUnion(C, R);

	for (auto i : CUR) {
		string varName = "y(" + to_string(i.key) + ")";
		y.push_back(model.addVar(0, 1, 1, GRB_BINARY, varName));
	}
	model.update();
}

void Model::var_t(GRBModel & model)
{
	auto V01 = inst->set_V01();

	for (auto i : V01) {
		string varName = "t(" + to_string(i.key) + ")";
		y.push_back(model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, varName));
	}
	model.update();
}

void Model::var_d(GRBModel & model)
{
	auto V01 = inst->set_V01();

	for (auto i : V01) {
		string varName = "d(" + to_string(i.key) + ")";
		d.push_back(model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, varName));
	}
	model.update();
}

void Model::var_q(GRBModel & model)
{
	auto V01 = inst->set_V01();

	for (auto i : V01) {
		string varName = "q(" + to_string(i.key) + ")";
		q.push_back(model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, varName));
	}
	model.update();
}

void Model::var_u(GRBModel & model)
{
	auto V1 = inst->set_V1();

	for (auto i : V1) {
		string varName = "u(" + to_string(i.key) + ")";
		u.push_back(model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, varName));
	}
	model.update();
}

void Model::var_w_model2(GRBModel & model)
{
	auto V = inst->set_V();

	for (auto i : V) {
		string varName = "w(" + to_string(i.key) + ")";
		y.push_back(model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, varName));
	}
	model.update();
}

void Model::var_w_model3(GRBModel & model)
{
	auto C = inst->set_C();

	vector<node> SK;
	for (auto h : C) {
		auto SKc = inst->set_SK(h.key);
		SK.insert(SK.end(), SKc.begin(), SKc.end());
	}

	auto CUSKc = vectorUnion(C, SK);

	for (auto i : CUSKc) {

		string varName = "w(" + to_string(i.key) + ")";
		y.push_back(model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, varName));
	}
	model.update();
}

void Model::var_z(GRBModel & model)
{
	auto UD0 = inst->set_UD0();

	for (auto i : UD0) {
		string varName = "z(" + to_string(i.key) + ")";
		model.addVar(0, 1, 1, GRB_BINARY, varName);
	}
	model.update();
}

void Model::fo(GRBModel & model)
{
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();

	GRBLinExpr fo = 0;
	for (auto i : V0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				string var = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";
				// cout << var << endl;
				// cout << model.getVarByName(var).get(GRB_StringAttr_VarName) << endl;
				fo += inst->dist(i, j) * model.getVarByName(var);
			}
		}
	}
	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

void Model::fo_cost(GRBModel & model)
{
	auto UD0 = inst->set_UD0();
	auto UD1 = inst->set_UD1();
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();
	auto V01 = inst->set_V01();

	GRBLinExpr fo = 0;

	// depot location cost
	for (auto i : UD0) {
		fo += getZ(model, i.key) * (inst->depotCost / inst->numC);
	}

	// bss and brs location cost
	for (auto i : C) {
		GRBVar yi = getY(model, i.key);
		fo += (inst->brsCost / inst->numC) * yi;
	}
	for (auto i : R) {
		GRBVar yi = getY(model, i.key);
		fo += (inst->bssCost / inst->numC) * yi;
	}

	// driving cost
	for (auto i : V0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				//string var = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";
				GRBVar xij = getX(model, i.key, j.key);
				fo += inst->driverWage * inst->dist(i, j) * xij; // model.getVarByName(var);
			}
		}
	}

	// vehicle fixed cost
	for (auto i : UD0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar xij = getX(model, i.key, j.key);
				fo += (inst->vehicleCost / inst->numC) * xij;
			}
		}
	}

	// energy cost in brs
	vector<node> SK;
	for (auto h : C) {
		auto SKc = inst->set_SK(h.key);
		SK.insert(SK.end(), SKc.begin(), SKc.end());
	}
	auto CUSKc = vectorUnion(C, SK);
	for (auto i : CUSKc) {
		GRBVar wi = getW(model, i.key);
		fo += inst->brsEnergyCost * wi;
	}
	// cost of using a bss (energy + operation cost)
	// operational cost
	for (node i : V0) {
		GRBLinExpr sum = 0;

		auto S = inst->set_S();

		vector<node> RUS = vectorUnion(R, S);

		for (node j : RUS) {
			if (i.key != j.key) {
				GRBVar xij = getX(model, i.key, j.key);
				sum += xij * inst->bssUseCost;
			}
		}
		fo += sum;
	}
	/*
	// energy cost
	for (auto i : R) {
		GRBVar wi = getW(model, i.key);
		fo += wi * inst->bssEnergyCost;
	}
	*/

	// cost of the energy recharged at the depot in the end of the route
	for (node i : UD1) {
		GRBVar qi = getQ(model, i.key);
		fo += qi * inst->bssCost;
	}
	
	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

void Model::fo_cost_2(GRBModel & model)
{
	auto UD0 = inst->set_UD0();
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();

	GRBLinExpr fo = 0;
	
	// bss and brs location cost
	for (auto i : C) {
		GRBVar yi = getY(model, i.key);
		fo += (inst->brsCost) * yi;
	}	
	for (auto i : R) {
		GRBVar yi = getY(model, i.key);
		fo += (inst->bssCost) * yi;
	}

	// driving cost
	for (auto i : V0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				//string var = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";
				GRBVar xij = getX(model, i.key, j.key);
				fo += inst->driverWage * inst->dist(i, j) * xij; // model.getVarByName(var);
			}
		}
	}

	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

void Model::c2(GRBModel & model)
{
	auto C = inst->set_C();
	auto V1 = inst->set_V1();
	
	for (auto i : C) {
		GRBLinExpr c = 0;
		for (auto j : V1) {
			if (i.key != j.key) {
				//string varName = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";
				//GRBVar xij = model.getVarByName(varName);
				GRBVar xij = getX(model, i.key, j.key);
				c += xij;
			}
		}
		model.addConstr(c == 1, "c2(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c3(GRBModel & model)
{
	auto RUS = vectorUnion(inst->set_R(), inst->set_S());
	auto V1 = inst->set_V1();

	for (auto i : RUS) {
		GRBLinExpr c = 0;
		for (auto j : V1) {
			if (i.key != j.key) {
				//string varName = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";
				//GRBVar xij = model.getVarByName(varName);
				GRBVar xij = getX(model, i.key, j.key);
				c += xij;
			}
		}
		model.addConstr(c <= 1, "c3(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c4(GRBModel & model)
{
	auto V = inst->set_V();
	auto V1 = inst->set_V1();
	auto V0 = inst->set_V0();

	for (auto j : V) {
		GRBLinExpr c1 = 0;
		for (auto i : V1) {
			if (i.key != j.key) {
				//string varName = "x(" + to_string(j.key) + "," + to_string(i.key) + ")";
				//GRBVar xji = model.getVarByName(varName);
				GRBVar xji = getX(model, j.key, i.key);
				c1 += xji;
			}
		}
		GRBLinExpr c2 = 0;
		for (auto i : V0) {
			if (i.key != j.key) {
				//string varName = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";
				//GRBVar xij = model.getVarByName(varName);
				GRBVar xij = getX(model, i.key, j.key);
				c2 += xij;
			}
		}
		model.addConstr(c1 == c2, "c4(" + to_string(j.key) + ")");
	}
	model.update();
}

void Model::c5(GRBModel & model) // verify
{
	auto UD0 = inst->set_UD0();
	auto V1 = inst->set_V1();

	for (auto i : UD0) {
		GRBLinExpr c = 0;
		for (auto j : V1) {
			//string varName = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";
			//GRBVar xij = model.getVarByName(varName);
			GRBVar xij = getX(model, i.key, j.key);
			c += xij;
		}
		model.addConstr(c <= inst->UB, "c5_1(" + to_string(i.key) + ")");
		model.addConstr(c >= inst->LB, "c5_2(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c6(GRBModel & model)
{
	auto R = inst->set_R();

	GRBLinExpr c = 0;
	for (auto i : R) {
		string varName = "y(" + to_string(i.key) + ")";
		GRBVar yi = model.getVarByName(varName);
		c += yi;
	}
	model.addConstr(c <= inst->o, "c6");
	model.update();
}

void Model::c7(GRBModel & model)
{
	auto R = inst->set_R();
	auto V0 = inst->set_V0();

	for (auto i : R) {
		GRBLinExpr c = 0;
		for (auto j : V0) {
			if (i.key != j.key) {
				string varName = "x(" + to_string(j.key) + "," + to_string(i.key) + ")";
				GRBVar xji = model.getVarByName(varName);
				c += xji;
			}
		}
		string varName = "y(" + to_string(i.key) + ")";
		GRBVar yi = model.getVarByName(varName);
		model.addGenConstrIndicator(yi, false, c == 0, "c7(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c7_M(GRBModel & model)
{
	auto R = inst->set_R();
	auto V0 = inst->set_V0();

	for (auto i : R) {
		GRBLinExpr c = 0;
		for (auto j : V0) {
			if (i.key != j.key) {
				//string varName = "x(" + to_string(j.key) + "," + to_string(i.key) + ")";
				//GRBVar xji = model.getVarByName(varName);
				GRBVar xji = getX(model, j.key, i.key);
				c += xji;
			}
		}
		//string varName = "y(" + to_string(i.key) + ")";
		//GRBVar yi = model.getVarByName(varName);
		GRBVar yi = getY(model, i.key);
		model.addConstr(c <= M * yi, "c7_M(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c8(GRBModel & model)
{
	auto R = inst->set_R();
	auto V0 = inst->set_V0();
	
	for (auto i : R) {
		auto SK = inst->set_SK(i.key);
		for (auto h : SK) {
			GRBLinExpr c = 0;
			for (auto j : V0) {
				if (h.key != j.key) {
					string varName = "x(" + to_string(j.key) + "," + to_string(h.key) + ")";
					GRBVar xjh = model.getVarByName(varName);
					c += xjh;
				}
			}
			string varName = "y(" + to_string(i.key) + ")";
			GRBVar yi = model.getVarByName(varName);
			model.addGenConstrIndicator(yi, false, c == 0, "c8(" + to_string(i.key) + "," + to_string(h.key) + ")");
		}
	}
	model.update();
}

void Model::c8_M(GRBModel & model)
{
	auto R = inst->set_R();
	auto V0 = inst->set_V0();

	for (auto i : R) {
		auto SKi = inst->set_SK(i.key);

		for (auto h : SKi) {
			GRBLinExpr c = 0;
			for (auto j : V0) {
				if (h.key != j.key) {
					string varName = "x(" + to_string(j.key) + "," + to_string(h.key) + ")";
					GRBVar xjh = model.getVarByName(varName);
					c += xjh;
				}
			}
			string varName = "y(" + to_string(i.key) + ")";
			GRBVar yi = model.getVarByName(varName);
			model.addConstr(c <= M * yi, "c8(" + to_string(i.key) + "," + to_string(h.key) + ")");
		}
	}
	model.update();
}

void Model::c9(GRBModel & model)
{
	auto C0 = inst->set_C0();
	auto V1 = inst->set_V1();

	for (auto i : C0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				string varName = "t(" + to_string(i.key) + ")";
				GRBVar ti = model.getVarByName(varName);

				varName = "t(" + to_string(j.key) + ")";
				GRBVar tj = model.getVarByName(varName);

				varName = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";
				GRBVar xij = model.getVarByName(varName);

				GRBLinExpr c = ti + (inst->getTD(i, j) + i.serviceTime) * xij;

				model.addGenConstrIndicator(xij, true, tj >= c, "c9(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c9_M(GRBModel & model)
{
	auto C0 = inst->set_C0();
	auto V1 = inst->set_V1(); // i != j

	for (auto i : C0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				string varName = "t(" + to_string(i.key) + ")";
				GRBVar ti = model.getVarByName(varName);

				varName = "t(" + to_string(j.key) + ")";
				GRBVar tj = model.getVarByName(varName);

				varName = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";
				GRBVar xij = model.getVarByName(varName);

				GRBLinExpr c = ti + (inst->getTD(i, j) + i.serviceTime) * xij; // verify getS

				model.addConstr(tj >= c - M * (1 - xij), "c9_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c10(GRBModel & model)
{
	auto V1 = inst->set_V1();
	auto C = inst->set_C();

	for (auto j : V1) {		
		for (auto h : C) {
			vector<node> SK = inst->set_SK(h.key);
			for (auto i : SK) {
				if (i.key != j.key) {
					GRBVar ti = getT(model, i.key);
					GRBVar tj = getT(model, j.key);
					GRBVar xij = getX(model, i.key, j.key);
					float tdij = inst->getTD(i, j);

					GRBLinExpr c = ti + tdij * xij;
					
					model.addGenConstrIndicator(xij, true, tj >= c, "c10(" + to_string(i.key) + "," + to_string(j.key) + ")");
				}
			}			
		}
	}
	model.update();
}

void Model::c10_M(GRBModel & model)
{
	auto V1 = inst->set_V1();
	auto C = inst->set_C();

	for (auto j : V1) {
		for (auto h : C) {
			vector<node> SKh = inst->set_SK(h.key);
			for (auto i : SKh) {
				if (i.key != j.key) {
					GRBVar ti = getT(model, i.key);
					GRBVar tj = getT(model, j.key);
					GRBVar xij = getX(model, i.key, j.key);
					float tdij = inst->getTD(i, j);

					GRBLinExpr c = ti + tdij * xij;

					model.addConstr(tj >= c - M * (1 - xij), "c10_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
				}
			}
		}
	}
	model.update();
}

void Model::c11(GRBModel & model)
{
	auto V1 = inst->set_V1();
	auto R = inst->set_R();

	for (auto j : V1) {
		for (auto h : R) {
			auto SK = inst->set_SK(h.key);
			auto RUSK = vectorUnion(R, SK);
			for (auto i : RUSK) {
				if (i.key != j.key) {
					GRBVar ti = getT(model, i.key);
					GRBVar tj = getT(model, j.key);
					GRBVar xij = getX(model, i.key, j.key);
					float tdij = inst->getTD(i, j);
					float ct = inst->getCT();

					GRBLinExpr c = ti + tdij * xij + ct * xij;

					model.addGenConstrIndicator(xij, true, tj >= c, "c11(" + to_string(i.key) + "," + to_string(j.key) + ")");
				}
			}
		}
	}
	model.update();
}

void Model::c11_M(GRBModel & model)
{
	auto V1 = inst->set_V1();
	auto R = inst->set_R();

	for (auto j : V1) {
		for (auto h : R) {
			auto SKh = inst->set_SK(h.key);
			auto RUSK = vectorUnion(R, SKh);
			for (auto i : RUSK) {
				if (i.key != j.key) {
					GRBVar ti = getT(model, i.key);
					GRBVar tj = getT(model, j.key);
					GRBVar xij = getX(model, i.key, j.key);
					float tdij = inst->getTD(i, j);
					float ct = inst->getCT();

					GRBLinExpr c = ti + tdij * xij + ct * xij;

					model.addConstr(tj >= c - M * (1 - xij), "c11_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
				}
			}
		}
	}
	model.update();
}

void Model::c12(GRBModel & model)
{
	auto V01 = inst->set_V01();

	for (auto i : V01) {
		GRBVar ti = getT(model, i.key);

		model.addConstr(ti >= i.readyTime, "c12_1(" + to_string(i.key) + ")");
		model.addConstr(ti <= i.dueDate, "c12_2(" + to_string(i.key) + ")");		
	}
	model.update();
}

void Model::c13(GRBModel & model)
{
	auto V1 = inst->set_V1();
	auto UD0 = inst->set_UD0();
	auto C = inst->set_C();

	for (auto j : V1) {
		for (auto h : C) {
			auto SK = inst->set_SK(h.key);

			auto UD0UCUSK = vectorUnion(UD0, C);
			UD0UCUSK = vectorUnion(UD0UCUSK, SK);

			for (auto i : UD0UCUSK) {
				if (i.key != j.key) {
					GRBVar qi = getQ(model, i.key);
					GRBVar qj = getQ(model, j.key);
					GRBVar xij = getX(model, i.key, j.key);
					float diij = inst->dist(i, j);
					float c = inst->r;

					GRBLinExpr c_ = qi - c * diij * xij;

					model.addGenConstrIndicator(xij, true, qj <= c_, "c13(" + to_string(i.key) + "," + to_string(j.key) + ")");
				}
			}
		}
	}
	model.update();
}

void Model::c13_M(GRBModel & model)
{
	auto V1 = inst->set_V1();
	auto UD0 = inst->set_UD0();
	auto C = inst->set_C();

	for (auto j : V1) {
		for (auto h : C) {
			auto SKh = inst->set_SK(h.key);

			auto UD0UCUSKh = vectorUnion(UD0, C);
			UD0UCUSKh = vectorUnion(UD0UCUSKh, SKh);

			for (auto i : UD0UCUSKh) {
				if (i.key != j.key) {
					GRBVar qi = getQ(model, i.key);
					GRBVar qj = getQ(model, j.key);
					GRBVar xij = getX(model, i.key, j.key);
					float diij = inst->dist(i, j);
					float c = inst->r;

					GRBLinExpr c_ = qi - c * diij * xij;

					model.addConstr(qj <= c_ + M * (1 - xij), "c13_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
				}
			}
		}
	}
	model.update();
}

void Model::c14(GRBModel & model)
{
	auto V1 = inst->set_V1();
	auto R = inst->set_R();

	for (auto j : V1) {
		for (auto h : R) {
			auto SK = inst->set_SK(h.key);

			auto RUSK = vectorUnion(R, SK);
			
			for (auto i : RUSK) {
				if (i.key != j.key) {
					GRBVar qj = getQ(model, j.key);
					GRBVar xij = getX(model, i.key, j.key);
					float diij = inst->dist(i, j);
					float c = inst->r;
					float Q = inst->Q;

					GRBLinExpr c14 = Q - c * diij * xij;

					model.addGenConstrIndicator(xij, true, qj <= c14, "c14(" + to_string(i.key) + "," + to_string(j.key) + ")");
				}
			}
		}
	}
	model.update();
}

void Model::c14_M(GRBModel & model)
{
	auto V1 = inst->set_V1();
	auto R = inst->set_R();

	for (auto j : V1) {
		for (auto h : R) {
			auto SKh = inst->set_SK(h.key);

			auto RUSKh = vectorUnion(R, SKh);

			for (auto i : RUSKh) {
				if (i.key != j.key) {
					GRBVar qj = getQ(model, j.key);
					GRBVar xij = getX(model, i.key, j.key);
					float diij = inst->dist(i, j);
					float c = inst->r;
					float Q = inst->Q;

					// GRBLinExpr c14 = Q - c * diij * xij;

					model.addConstr(qj <= Q - c * diij * xij + M * (1 - xij), "c14_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
				}
			}
		}
	}
	model.update();
}

void Model::c15(GRBModel & model)
{
	auto UD0 = inst->set_UD0();

	for (auto i : UD0) {
		GRBVar qi = getQ(model, i.key);

		model.addConstr(qi <= inst->Q, "c15(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c16(GRBModel & model)
{
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();

	for (auto i : V0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar di = getD(model, i.key);
				GRBVar dj = getD(model, j.key);
				GRBVar xij = getX(model, i.key, j.key);
				float pi = i.demand; // inst->nodes.at(i.key).demand; // may cause problem at some point

				GRBLinExpr c = di - pi * xij;

				model.addGenConstrIndicator(xij, true, dj <= c, "c16(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c16_M(GRBModel & model)
{
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();

	for (auto i : V0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar di = getD(model, i.key);
				GRBVar dj = getD(model, j.key);
				GRBVar xij = getX(model, i.key, j.key);
				float pi = i.demand; // inst->nodes.at(i.key).demand; // may cause problem at some point

				// GRBLinExpr c = di - pi * xij;

				model.addConstr(dj <= di - pi * xij + M * (1 - xij), "c16_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c17(GRBModel & model)
{
	auto UD0 = inst->set_UD0();

	for (auto i : UD0) {
		GRBVar di = getD(model, i.key);

		model.addConstr(di <= inst->C, "c17(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c18(GRBModel & model)
{
	auto V = inst->set_V();
	auto UD0 = inst->set_UD0();

	for (auto i : UD0) {
		for (auto j : V) {
			
			GRBVar uj = getU(model, j.key);
			GRBVar xij = getX(model, i.key, j.key);
			float idi = i.id_n; // verify, error is possible due to nodes index

			model.addGenConstrIndicator(xij, true, uj >= xij * idi, "c18(" + to_string(i.key) + "," + to_string(j.key) + ")");
			
		}
	}
	model.update();
}

void Model::c18_M(GRBModel & model)
{
	auto V = inst->set_V();
	auto UD0 = inst->set_UD0();

	for (auto i : UD0) {
		for (auto j : V) {

			GRBVar uj = getU(model, j.key);
			GRBVar xij = getX(model, i.key, j.key);
			float idi = i.id_n;

			model.addConstr(uj >= xij * idi - M * (1 - xij), "c18_M(" + to_string(i.key) + "," + to_string(j.key) + ")");

		}
	}
	model.update();
}

void Model::c19(GRBModel & model)
{
	auto V = inst->set_V();
	auto UD0 = inst->set_UD0();

	for (auto i : UD0) {
		for (auto j : V) {

			GRBVar uj = getU(model, j.key);
			GRBVar xij = getX(model, i.key, j.key);
			float idi = i.id_n; // verify, error is possible due to nodes index

			model.addGenConstrIndicator(xij, true, uj <= xij * idi, "c19(" + to_string(i.key) + "," + to_string(j.key) + ")");

		}
	}
	model.update();
}

void Model::c19_M(GRBModel & model)
{
	auto UD0 = inst->set_UD0();
	auto V = inst->set_V();

	for (auto i : UD0) {
		for (auto j : V) {

			GRBVar uj = getU(model, j.key);
			GRBVar xij = getX(model, i.key, j.key);
			float idi = i.id_n; 

			model.addConstr(uj <= xij * idi + M * (1 - xij), "c19_M(" + to_string(i.key) + "," + to_string(j.key) + ")");

		}
	}
	model.update();
}

void Model::c20(GRBModel & model)
{
	auto V = inst->set_V();
	auto V1 = inst->set_V1();

	for (auto i : V) {
		for (auto j : V1) {
			if (i.key != j.key) { // verify
				GRBVar xij = getX(model, i.key, j.key);
				GRBVar ui = getU(model, i.key);
				GRBVar uj = getU(model, j.key);

				model.addGenConstrIndicator(xij, true, uj >= ui, "c20(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}			
		}
	}
	model.update();
}

void Model::c20_M(GRBModel & model)
{
	auto V = inst->set_V();
	auto V1 = inst->set_V1();

	for (auto i : V) {
		for (auto j : V1) {
			//cout << i.key << " - " << j.key << endl;
			if (i.key != j.key) { // verify
				GRBVar xij = getX(model, i.key, j.key);
				GRBVar ui = getU(model, i.key);
				GRBVar uj = getU(model, j.key);

				model.addConstr(uj >= ui - M * (1 - xij), "c20_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}			
		}
	}
	model.update();
}

void Model::c21(GRBModel & model)
{
	auto V = inst->set_V();
	auto V1 = inst->set_V1();

	for (auto i : V) {
		for (auto j : V1) {
			if (i.key != j.key) { // verify
				GRBVar xij = getX(model, i.key, j.key);
				GRBVar ui = getU(model, i.key);
				GRBVar uj = getU(model, j.key);

				model.addGenConstrIndicator(xij, true, uj <= ui, "c21(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c21_M(GRBModel & model)
{
	auto V = inst->set_V();
	auto V1 = inst->set_V1();

	for (auto i : V) {
		for (auto j : V1) {
			if (i.key != j.key) { // verify
				GRBVar xij = getX(model, i.key, j.key);
				GRBVar ui = getU(model, i.key);
				GRBVar uj = getU(model, j.key);

				model.addConstr(uj <= ui + M * (1 - xij), "c21(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c22(GRBModel & model)
{
	auto UD1 = inst->set_UD1();

	for (auto j : UD1) {
		GRBVar uj = getU(model, j.key);

		model.addConstr(uj == j.id_n, "c22(" + to_string(j.key) + ")");
	}
	model.update();
}

void Model::c27(GRBModel & model)
{
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto CUR = vectorUnion(C, R);

	GRBLinExpr c = 0;
	for (auto i : CUR) {
		GRBVar yi = getY(model, i.key);
		c += yi;
	}
	model.addConstr(c <= inst->o, "c27");
	model.update();
}

void Model::c28(GRBModel & model)
{
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto CUR = vectorUnion(C, R);

	for (auto i : CUR) {
		GRBVar wi = getW(model, i.key);
		GRBVar yi = getY(model, i.key);
		model.addGenConstrIndicator(yi, true, wi >= 0, "c28(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c28_M(GRBModel & model)
{
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto CUR = vectorUnion(C, R);

	for (auto i : CUR) {
		GRBVar wi = getW(model, i.key);
		GRBVar yi = getY(model, i.key);
		model.addConstr(wi <= inst->M * yi, "c28_M(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c29(GRBModel & model)
{
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto CUR = vectorUnion(C, R);

	for (auto i : CUR) {
		auto SKi = inst->set_SK(i.key);
		for (auto h : SKi) {
			GRBVar wh = getW(model, h.key);
			GRBVar yi = getY(model, i.key);
			model.addGenConstrIndicator(yi, true, wh >= 0, "c29(" + to_string(i.key) + ")");
		}		
	}
	model.update();
}

void Model::c29_M(GRBModel & model)
{
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto CUR = vectorUnion(C, R);

	for (auto i : CUR) {
		auto SKi = inst->set_SK(i.key);
		for (auto h : SKi) {
			GRBVar wh = getW(model, h.key);
			GRBVar yi = getY(model, i.key);
			model.addConstr(wh <= inst->M * yi, "c29_M(" + to_string(i.key) + ")");
		}
	}
	model.update();
}

void Model::c30(GRBModel & model)
{
	auto V = inst->set_V();
	auto V1 = inst->set_V1();

	for (auto i : V) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar ti = getT(model, i.key);
				GRBVar tj = getT(model, j.key);				
				float tdij = inst->getTD(i, j);
				GRBVar xij = getX(model, i.key, j.key);
				float r = inst->g;
				GRBVar wi = getW(model, i.key);
				model.addGenConstrIndicator(xij, true, tj >= ti + tdij * xij + r * wi, "c30(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c30_M(GRBModel & model)
{
	auto V = inst->set_V();
	auto V1 = inst->set_V1();

	for (auto i : V) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar ti = getT(model, i.key);
				GRBVar tj = getT(model, j.key);
				float tdij = inst->getTD(i, j);
				GRBVar xij = getX(model, i.key, j.key);
				float r = inst->g;
				GRBVar wi = getW(model, i.key);
				model.addConstr(tj >= ti + tdij * xij + r * wi - inst->M * (1 - xij), "c30_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c31(GRBModel & model)
{
	auto UD0 = inst->set_UD0();
	auto V1 = inst->set_V1();

	for (auto i : UD0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar qi = getQ(model, i.key);
				GRBVar qj = getQ(model, j.key);
				float dij = inst->dist(i, j);
				GRBVar xij = getX(model, i.key, j.key);
				float c = inst->r;
				model.addGenConstrIndicator(xij, true, qj <= qi - c * dij * xij, "c31(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c31_M(GRBModel & model)
{
	auto UD0 = inst->set_UD0();
	auto V1 = inst->set_V1();

	for (auto i : UD0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar qi = getQ(model, i.key);
				GRBVar qj = getQ(model, j.key);
				float dij = inst->dist(i, j);
				GRBVar xij = getX(model, i.key, j.key);
				float c = inst->r;
				model.addConstr(qj <= qi - c * dij * xij + inst->M * (1 - xij), "c31_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c32(GRBModel & model)
{
	auto V = inst->set_V();
	auto V1 = inst->set_V1();

	for (auto i : V) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar qi = getQ(model, i.key);
				GRBVar qj = getQ(model, j.key);
				GRBVar wi = getW(model, i.key);
				float dij = inst->dist(i, j);
				GRBVar xij = getX(model, i.key, j.key);
				float c = inst->r;
				model.addGenConstrIndicator(xij, true, qj <= qi + wi - c * dij * xij, "c32(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c32_M(GRBModel & model)
{
	auto V = inst->set_V();
	auto V1 = inst->set_V1();

	for (auto i : V) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar qi = getQ(model, i.key);
				GRBVar qj = getQ(model, j.key);
				GRBVar wi = getW(model, i.key);
				float dij = inst->dist(i, j);
				GRBVar xij = getX(model, i.key, j.key);
				float c = inst->r;
				model.addConstr(qj <= qi + wi - c * dij * xij + inst->M * (1 - xij), "c32_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c33(GRBModel & model)
{
	auto V = inst->set_V(); 

	for (auto i : V) {
		GRBVar qi = getQ(model, i.key);
		GRBVar wi = getW(model, i.key);
		model.addConstr(qi + wi <= inst->Q, "c33(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c36(GRBModel & model)
{
	auto C = inst->set_C();

	for (auto i : C) {
		GRBVar wi = getW(model, i.key);
		GRBVar yi = getY(model, i.key);
		model.addGenConstrIndicator(yi, true, wi >= 0, "c36(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c36_M(GRBModel & model)
{
	auto C = inst->set_C();

	for (auto i : C) {
		GRBVar wi = getW(model, i.key);
		GRBVar yi = getY(model, i.key);
		model.addConstr(wi <= inst->M * yi, "c36_M(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c37(GRBModel & model)
{
	auto C = inst->set_C();

	for (auto i : C) {
		auto SKi = inst->set_SK(i.key);
		for (auto h : SKi) {
			GRBVar wh = getW(model, h.key);
			GRBVar yi = getY(model, i.key);
			model.addGenConstrIndicator(yi, true, wh >= 0, "c37(" + to_string(i.key) + ")");
		}		
	}
	model.update();
}

void Model::c37_M(GRBModel & model)
{
	auto C = inst->set_C();

	for (auto i : C) {
		auto SKi = inst->set_SK(i.key);
		for (auto h : SKi) {
			GRBVar wh = getW(model, h.key);
			GRBVar yi = getY(model, i.key);
			model.addConstr(wh <= inst->M * yi, "c37_M(" + to_string(i.key) + ")");
		}
	}
	model.update();
}

void Model::c38(GRBModel & model)
{
	auto C = inst->set_C();
	auto V1 = inst->set_V1();

	for (auto h : C) {
		auto SKh = inst->set_SK(h.key);
		auto CUSKh = vectorUnion(C, SKh);
		for (auto i : CUSKh) {
			for (auto j : V1) {
				if (i.key != j.key) {
					GRBVar tj = getT(model, j.key);
					GRBVar ti = getT(model, i.key);
					float tdij = inst->getTD(i, j);
					GRBVar xij = getX(model, i.key, j.key);
					float r = inst->g;
					GRBVar wi = getW(model, i.key);
					model.addGenConstrIndicator(xij, true, tj >= ti + tdij * xij + r * wi, "c38(" + to_string(i.key) + "," + to_string(j.key) + ")");
				}
			}
		}		
	}
	model.update();
}

void Model::c38_M(GRBModel & model)
{
	auto C = inst->set_C();
	auto V1 = inst->set_V1();

	for (auto h : C) {
		auto SKh = inst->set_SK(h.key);
		auto CUSKh = vectorUnion(C, SKh);
		for (auto i : CUSKh) {
			for (auto j : V1) {
				if (i.key != j.key) {
					GRBVar tj = getT(model, j.key);
					GRBVar ti = getT(model, i.key);
					float tdij = inst->getTD(i, j);
					GRBVar xij = getX(model, i.key, j.key);
					float r = inst->g;
					GRBVar wi = getW(model, i.key);
					model.addConstr(tj >= ti + tdij * xij + r * wi - inst->M * (1 - xij), "c38_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
				}
			}
		}
	}
	model.update();
}

void Model::c39(GRBModel & model)
{
	auto UD0 = inst->set_UD0();
	auto V1 = inst->set_V1();

	for (auto i : UD0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar qj = getQ(model, j.key);
				GRBVar qi = getQ(model, i.key);
				float c = inst->r;
				float dij = inst->dist(i, j);
				GRBVar xij = getX(model, i.key, j.key);
				model.addGenConstrIndicator(xij, true, qj <= qi - c * dij * xij, "c39(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c39_M(GRBModel & model)
{
	auto UD0 = inst->set_UD0();
	auto V1 = inst->set_V1();

	for (auto i : UD0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar qj = getQ(model, j.key);
				GRBVar qi = getQ(model, i.key);
				float c = inst->r;
				float dij = inst->dist(i, j);
				GRBVar xij = getX(model, i.key, j.key);
				model.addConstr(qj <= qi - c * dij * xij + inst->M * (1 - xij), "c39_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c40(GRBModel & model)
{
	auto C = inst->set_C();
	auto V1 = inst->set_V1();

	for (auto h : C) {
		auto SKh = inst->set_SK(h.key);
		auto CUSKh = vectorUnion(C, SKh);
		for (auto i : CUSKh) {
			for (auto j : V1) {
				if (i.key != j.key) {
					GRBVar qj = getQ(model, j.key);
					GRBVar qi = getQ(model, i.key);
					GRBVar wi = getW(model, i.key);
					float c = inst->r;
					float dij = inst->dist(i, j);
					GRBVar xij = getX(model, i.key, j.key);
					model.addGenConstrIndicator(xij, true, qj <= qi + wi - c * dij * xij, "c40(" + to_string(i.key) + "," + to_string(j.key) + ")");
				}
			}
		}
	}
	model.update();
}

void Model::c40_M(GRBModel & model)
{
	auto C = inst->set_C();
	auto V1 = inst->set_V1();

	for (auto h : C) {
		auto SKh = inst->set_SK(h.key);
		auto CUSKh = vectorUnion(C, SKh);
		for (auto i : CUSKh) {
			for (auto j : V1) {
				if (i.key != j.key) {
					GRBVar qj = getQ(model, j.key);
					GRBVar qi = getQ(model, i.key);
					GRBVar wi = getW(model, i.key);
					float c = inst->r;
					float dij = inst->dist(i, j);
					GRBVar xij = getX(model, i.key, j.key);
					model.addConstr(qj <= qi + wi - c * dij * xij + inst->M * (1 - xij), "c40_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
				}
			}
		}
	}
	model.update();
}

void Model::c41(GRBModel & model)
{
	auto C = inst->set_C();

	for (auto h : C) {
		auto SKh = inst->set_SK(h.key);
		auto CUSKh = vectorUnion(C, SKh);

		for (auto i : CUSKh) {
			GRBVar qi = getQ(model, i.key);
			GRBVar wi = getW(model, i.key);
			float Q = inst->Q;

			model.addConstr(qi + wi <= Q, "c41(" + to_string(i.key) + ")");
		}
	}
	model.update();
}

void Model::c43(GRBModel & model)
{
	auto V = inst->set_V();
	auto S = inst->set_S();

	auto V_S = vectorSub(V, S);

	for (auto i : V_S) {
		auto SKi = inst->set_SK(i.key);
		for (auto j : SKi) {
			if (i.key != j.key) {
				GRBVar xij = getX(model, i.key, j.key);

				model.addConstr(xij == 0, "c43(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c44(GRBModel & model)
{
	auto V = inst->set_V();
	auto S = inst->set_S();

	auto V_S = vectorSub(V, S);

	for (auto j : V_S) {
		auto SKj = inst->set_SK(j.key);
		for (auto i : SKj) {
			if (i.key != j.key) {
				GRBVar xji = getX(model, j.key, i.key);

				model.addConstr(xji == 0, "c44(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

// review
void Model::c45(GRBModel & model)
{
	auto UD0 = inst->set_UD0();
	auto V1 = inst->set_V1();
	
	for (auto i : UD0) {
		GRBLinExpr e = 0;
		for (auto j : V1) {
			// if(i.key != j.key)
			GRBVar xij = getX(model, i.key, j.key);
			e += xij;		}		
		GRBVar zi = getZ(model, i.key);
		model.addConstr(e <= inst->M * zi, "c45(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c46(GRBModel & model)
{

}

GRBVar Model::getX(GRBModel &model, int keya, int keyb)
{
	string varName = "x(" + to_string(keya) + "," + to_string(keyb) + ")";
	try {
		
		GRBVar xij = model.getVarByName(varName);
		return xij;
	}
	catch (GRBException e) {
		cout << "Error getting variable: " << varName << endl;
		cout << "Erroe code: " << e.getErrorCode() << endl;
		cout << "Message: " << e.getMessage() << endl;
		exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		exit(1);
	}
}

GRBVar Model::getY(GRBModel & model, int key)
{
	string varName = "y(" + to_string(key) + ")";
	try {
		GRBVar yi = model.getVarByName(varName);
		return yi;
	}
	catch (GRBException e) {
		cout << "Error getting variable: " << varName << endl;
		cout << "Error code: " << e.getErrorCode() << endl;
		cout << "Message: " << e.getMessage() << endl;
		cout << e.getMessage() << endl;
		exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		exit(1);
	}
}

GRBVar Model::getT(GRBModel &model, int key)
{
	string varName = "t(" + to_string(key) + ")";
	try {		
		GRBVar ti = model.getVarByName(varName);
		return ti;
	}
	catch (GRBException e) {
		cout << "Error getting variable: " << varName << endl;
		cout << "Error code: " << e.getErrorCode() << endl;
		cout << "Message: " << e.getMessage() << endl;
		cout << e.getMessage() << endl;
		exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		exit(1);
	}
}

GRBVar Model::getQ(GRBModel & model, int key)
{
	string varName = "q(" + to_string(key) + ")";
	try {
		GRBVar qi = model.getVarByName(varName);
		return qi;
	}
	catch (GRBException e) {
		cout << "Error getting variable: " << varName << endl;
		cout << "Error code: " << e.getErrorCode() << endl;
		cout << "Message: " << e.getMessage() << endl;
		cout << e.getMessage() << endl;
		exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		exit(1);
	}
}

GRBVar Model::getD(GRBModel & model, int key)
{
	string varName = "d(" + to_string(key) + ")";
	try {
		GRBVar di = model.getVarByName(varName);
		return di;
	}
	catch (GRBException e) {
		cout << "Error getting variable: " << varName << endl;
		cout << "Error code: " << e.getErrorCode() << endl;
		cout << "Message: " << e.getMessage() << endl;
		cout << e.getMessage() << endl;
		exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		exit(1);
	}
}

GRBVar Model::getU(GRBModel & model, int key)
{
	string varName = "u(" + to_string(key) + ")";
	try {
		GRBVar ui = model.getVarByName(varName);
		return ui;
	}
	catch (GRBException e) {
		cout << "Error getting variable: " << varName << endl;
		cout << "Error code: " << e.getErrorCode() << endl;
		cout << "Message: " << e.getMessage() << endl;
		cout << e.getMessage() << endl;
		exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		exit(1);
	}
}

GRBVar Model::getW(GRBModel & model, int key)
{
	string varName = "w(" + to_string(key) + ")";
	try {
		GRBVar wi = model.getVarByName(varName);
		return wi;
	}
	catch (GRBException e) {
		cout << "Error getting variable: " << varName << endl;
		cout << "Error code: " << e.getErrorCode() << endl;
		cout << "Message: " << e.getMessage() << endl;
		cout << e.getMessage() << endl;
		exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		exit(1);
	}
}

GRBVar Model::getZ(GRBModel & model, int key)
{
	string varName = "z(" + to_string(key) + ")";
	try {
		GRBVar zi = model.getVarByName(varName);
		return zi;
	}
	catch (GRBException e) {
		cout << "Error getting variable: " << varName << endl;
		cout << "Error code: " << e.getErrorCode() << endl;
		cout << "Message: " << e.getMessage() << endl;
		//exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		//exit(1);
	}
}

