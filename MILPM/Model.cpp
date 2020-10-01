#include "Model.h"

Model::Model(string fileName)
{
	this->directory = "";
	this->fileName = fileName;
	inst = new instance("", fileName);
	M = inst->M;

}

Model::Model(string dir, string fileName)
{
	this->directory = dir;
	this->fileName = fileName;
	inst = new instance(dir, fileName);
	M = inst->M;
	row += fileName;

}

Model::Model(string dir, string fileName, int w)
{
	this->directory = dir;
	this->fileName = fileName;
	inst = new instance(dir, fileName);
	this->w = w;
	M = inst->M;
	row += fileName;

}

Model::Model(string dir, string fileName, int w, int t)
{
	this->directory = dir;
	this->fileName = fileName;
	inst = new instance(dir, fileName, t);
	this->w = w;
	M = inst->M;
	row += fileName;
	//model();
}

Model::Model(string dir, string fileName, int w, string dirOutput)
{
	this->directory = dir;
	this->fileName = fileName;
	inst = new instance(dir, fileName);
	this->w = w;
	M = inst->M;
	this->dirOutput = dirOutput;
	row += fileName;
	//model();
}

Model::Model(string dir, string fileName, int w, int t, string dirOutput)
{
	this->directory = dir;
	this->fileName = fileName;

	inst = new instance(dir, fileName, t);

	this->w = w;
	M = inst->M;
	this->dirOutput = dirOutput;
	row += fileName;
	//model();
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
	var_y_model3(model);
	var_t(model);
	var_d(model);
	var_q(model);
	var_u(model);
	var_w_model3(model); // var_w_model3(model);
	var_z(model);
	var_v(model);

	depotCostFO(model);	// depot cost
	bssCostFO(model);	// bss cost
	vehicleFixedCostFO(model);	// vehicle fixed cost
	drivingCostFO(model);	// driving cost
	brsEnergyCostFO(model);	// energy cost in brs
	bssEnergyCostFO(model);	// energy cost in bss
	bssUseCostFO(model);
	fo_cost_3(model);

	//fo_cost(model);

	c2(model); // 2
	c3(model); // 3
	c4(model); // 4
	c5(model); // 5
	c7_M(model); // 6
	c8_M(model); // 7
	c9_M(model); // 8
	c11_M(model); // 9
	c12(model); // 10
	c14_M(model); // 11
	c15(model); // 12
	c16_M(model); // 13
	c17(model); // 14
	c18_M(model); // 15
	c19_M(model); // 16
	c20_M(model); // 17
	c21_M(model); // 18
	c22(model); // 19
	c43(model); // 20
	c44(model); // 21

	c27_model4(model); // 22

	c36_M_model4(model); // 23
	c37_M_model4(model); // 24
	c38_M(model); // 25
	c39_M(model); // 26
	c40_M(model); // 27
	c41(model); // 28

	c45(model); // 29

	//c46(model);
	//c47(model);

	c48(model);
	//

	//c49(model);
	c50(model);

	//r3(model);
	//r4(model);

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

void Model::model_SBPO(GRBModel& model)
{
	string newName = "model4_" + fileName;
	model.set(GRB_StringAttr_ModelName, newName);

	var_x(model);
	var_y_model3(model);
	var_t(model);
	var_d(model);
	var_q(model);
	var_u(model);
	var_w_model3(model); // var_w_model3(model);
	var_z(model);
	var_v(model);

	depotCostFO(model);	// depot cost
	bssCostFO(model);	// bss cost
	vehicleFixedCostFO(model);	// vehicle fixed cost
	drivingCostFO(model);	// driving cost
	brsEnergyCostFO(model);	// energy cost in brs
	bssEnergyCostFO(model);	// energy cost in bss
	bssUseCostFO(model);
	fo_cost_3(model);

	//fo_cost(model);

	c2(model); // 2
	c3(model); // 3
	c4(model); // 4
	c5(model); // 5
	c7_M(model); // 6
	c8_M(model); // 7
	c9_M(model); // 8
	c11_M(model); // 9
	c12(model); // 10
	c14_M(model); // 11
	c15(model); // 12
	c16_M(model); // 13
	c17(model); // 14
	c18_M(model); // 15
	c19_M(model); // 16
	c20_M(model); // 17
	c21_M(model); // 18
	c22(model); // 19
	c43(model); // 20
	c44(model); // 21
	c27_model4(model); // 22
	c36_M_model4(model); // 23
	c37_M_model4(model); // 24
	c38_M(model); // 25
	c39_M(model); // 26
	c40_M(model); // 27
	c41(model); // 28
	c45(model);
	c48(model);
	c50(model);
}

void Model::model_test(GRBModel& model)
{
	model_SBPO(model);
	setInitialStations(model);

}

void Model::setup(GRBModel &model)
{
	if (w == -1) {
		model_test(model);
	}
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
		//model.write(fN);
	}
	else if (status == GRB_OPTIMAL) {
		cout << "Optimal solution found!" << endl;
		cout << "The objective function value is " << model.get(GRB_DoubleAttr_ObjVal) << endl;
		//model.write(fN);
	}
	else if (status == GRB_TIME_LIMIT) {
		cout << "Limit time reached!" << endl;
		cout << "The best solution found is " << model.get(GRB_DoubleAttr_ObjVal) << endl;
		//model.write(fN);
	}
	else if (status == GRB_INFEASIBLE) {
		cout << "The model is infeasible" << endl;
		//model.write(fN);
	}
	else {
		cout << "Status: " << status << endl;
		//model.write(fN);
	}
	

	double gap = model.get(GRB_DoubleAttr_MIPGap);
	cout << gap << endl;
	if (gap < 100) {
		model.write(fN);

		// write more information in the solution file

		fstream file;
		file.open(fN, ios::out | ios::app);
		if (file.is_open() == false) {
			cout << "Error opening the solution file to write more information" << endl;
		}
		else {
			file << "#" << endl;

			Solution s = getSolution(model);

			s.FO = model.get(GRB_DoubleAttr_ObjVal);
			//s.runtime = model.get(GRB_DoubleAttr_Runtime);
			s.runtime = duration;
			s.gap = model.get(GRB_DoubleAttr_MIPGap) * 100;
			s.status = model.get(GRB_IntAttr_Status);

			s.numVehicles = getNumVehicles(model);

			// get solution 

			s.inf = eval(s);
			solu = s;

			// get row for csv file
			getRow(model);

			//s.strmSol(file);
			s.debug(file);

			file.close();
		}
	}
}

void Model::getRow(GRBModel & model)
{
	string res = "";
	res += model.get(GRB_StringAttr_ModelName) + ",";
	res += to_string(model.get(GRB_IntAttr_Status)) + ",";
	res += to_string(model.get(GRB_DoubleAttr_ObjVal)) + ","; 
	res += to_string(model.get(GRB_DoubleAttr_MIPGap) * 100) + ",";
	//res += to_string(model.get(GRB_DoubleAttr_Runtime));
	res += to_string(duration);

	row = res;
}

Solution Model::model()
{

	GRBEnv env = GRBEnv(true);
	try {
		env.start();
	}
	catch (GRBException e) {
		cout << e.getMessage() << endl;
	}

	try {
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "ELRP_" + fileName);

		setup(model);
		model.update();

		if (initProv == true) {
			setInitialSolution(model, init);
			model.write("attr.attr");
		}

		auto t1 = std::chrono::high_resolution_clock::now();

		model.write("lp.lp");
		model.getEnv().set(GRB_DoubleParam_TimeLimit, TMAX);
		model.optimize();

		auto t2 = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();

		// save solution
		try{
			result(model);
			model.write("sol.sol");
		}
		catch (GRBException& e) {
			cout << e.getErrorCode() << endl;
			cout << e.getMessage() << endl;
		}
		
		return solu;
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}	
}

Solution Model::model(Solution s)
{
	GRBEnv env = GRBEnv(true);
	try {
		env.start();
	}
	catch (GRBException e) {
		cout << e.getMessage() << endl;
	}

	try {
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "ELRP_" + fileName);

		setup(model);
		model.update();

		model.getEnv().set(GRB_DoubleParam_TimeLimit, TMAX);
		model.optimize();

		// get solution 
		Solution s = getSolution(model);
		s.inf = eval(s);
		solu = s;

		// get row for csv file
		getRow(model);

		// save solution	
		result(model);

		this->solu = s;

		return s;
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

void Model::setInitialSolution(GRBModel& model, Solution s)
{
	int count = 0;

	vector<node> V0 = inst->set_V0();
	vector<node> V1 = inst->set_V();
	vector<node> V01 = inst->set_V01();
	vector<node> R = inst->set_R();
	vector<node> C = inst->set_C();
	vector<node> CUR = vectorUnion(C, R);

	vector<node> SK;
	for (auto h : R) {
		auto SKr = inst->set_SK(h.key);
		SK.insert(SK.end(), SKr.begin(), SKr.end());
	}
	vector<node> RUSKr = vectorUnion(R, SK);

	// set variable x
	// set all to 0
	for (node i : V0) {
		for (node j : V1) {
			if (i.key != j.key) {
				GRBVar xij = getX(model, i.key, j.key);
				xij.set(GRB_DoubleAttr_Start, 0);
			}
		}
	}
	// set used arcs to 1
	for (route r : s.routes) {
		for (int j = 0; j < r.size() - 1; j++) {
			GRBVar xij = getX(model, r.at(j).key, r.at(j + 1).key);
			xij.set(GRB_DoubleAttr_Start, 1);
		}
	}

	// set variable y
	for (node i : CUR) {
		GRBVar yi = getY(model, i.key); //string varName = "y(" + to_string(i.key) + ")";
		
		yi.set(GRB_DoubleAttr_Start, 0);
	}
	// set sited stations to 1
	for (route r : s.routes) {
		for (int j = 0; j < r.size(); j++) {
			if (r.at(j).n.type == "c" ||r.at(j).n.type == "f") {
				GRBVar yi = getY(model, r.at(j).key);
				yi.set(GRB_DoubleAttr_Start, 1);
			}
			else if (r.at(j).n.type == "c_d" || r.at(j).n.type == "f_d") {
				GRBVar yi = getY(model, r.at(j).n.ref2);
				yi.set(GRB_DoubleAttr_Start, 1);
			}
		}
	}

	// set variable t
	for (route r : s.routes) {
		for (vertex v : r) {
			GRBVar *ti = &getT(model, v.key);
			ti->set(GRB_DoubleAttr_Start, v.aTime + v.wTime);
		}
		// dealing with the last vertex in the route
		vertex v = r.back();
		node n = inst->getNodeByKey(v.key);
		if (v.aTime < n.dueDate) {
			GRBVar* ti = &getT(model, r.back().key);
			ti->set(GRB_DoubleAttr_Start, n.dueDate);
		}

	}

	// set variable d
	for (route r : s.routes) {
		
		vertex v = r.front();
		GRBVar di = getD(model, v.key);
		di.set(GRB_DoubleAttr_Start, v.vLoad);

		for (int i = 0; i < r.size() - 1; i++) {
		//for (vertex v : r) {
v = r.at(i);
vertex v2 = r.at(i + 1);
di = getD(model, v2.key);
di.set(GRB_DoubleAttr_Start, v.vLoad);
		}

		v = r.back();
		di = getD(model, v.key);
		di.set(GRB_DoubleAttr_Start, 0);
	}

	// set variable q
	for (route r : s.routes) {
		for (vertex v : r) {
			GRBVar qi = getQ(model, v.key);
			qi.set(GRB_DoubleAttr_Start, v.bLevel);
		}
	}

	// set variable u
	for (node i : V1) {
		GRBVar ui = getU(model, i.key);
		for (route r : s.routes) {
			node n = inst->getNodeByKey(r.front().key);
			for (vertex v : r) {
				if (i.key == v.key) {
					ui.set(GRB_DoubleAttr_Start, n.id_n);
				}

			}
		}
	}

	// set variable q
	for (route r : s.routes) {
		//for (vertex v : r) {
		for (int i = 0; i < r.size() - 1; i++) {
			vertex v = r.at(i);
			GRBVar qi = getQ(model, v.key);
			qi.set(GRB_DoubleAttr_Start, v.bLevel);
		}
		vertex v = r.back();
		if (v.bLevel < 0) {
			GRBVar qi = getQ(model, v.key);
			qi.set(GRB_DoubleAttr_Start, v.bLevel);
		}
		else {
			GRBVar qi = getQ(model, v.key);
			qi.set(GRB_DoubleAttr_Start, 0);
		}

	}

	// set variable w
	for (route r : s.routes) {
		for (vertex v : r) {
			if (v.n.type == "c" || v.n.type == "c_d") {
				GRBVar wi = getW(model, v.key);
				wi.set(GRB_DoubleAttr_Start, v.recharged);
			}
		}
	}

	// set variable v
	for (node n : RUSKr) {
		GRBVar vi = getV(model, n.key);
		vi.set(GRB_DoubleAttr_Start, 0);
	}
	for (route r : s.routes) {
		for (vertex v : r) {
			if (v.n.type == "f" || v.n.type == "f_d") {
				GRBVar vi = getV(model, v.key);
				vi.set(GRB_DoubleAttr_Start, v.recharged);
			}
		}
	}

	/*
	GRBVar p1 = model.getVarByName("depotCost");
	p1.set(GRB_DoubleAttr_Start, s.FOp.at(1));
	GRBVar p2 = model.getVarByName("bssCost");
	p2.set(GRB_DoubleAttr_Start, s.FOp.at(2));
	GRBVar p3 = model.getVarByName("vehicleFixedCost");
	p3.set(GRB_DoubleAttr_Start, s.FOp.at(3));
	GRBVar p4 = model.getVarByName("drivingCost");
	p4.set(GRB_DoubleAttr_Start, s.FOp.at(4));
	GRBVar p5 = model.getVarByName("brsEnergyCost");
	p5.set(GRB_DoubleAttr_Start, s.FOp.at(5));
	GRBVar p6 = model.getVarByName("bssEnergyCost");
	p6.set(GRB_DoubleAttr_Start, s.FOp.at(6));
	*/
	model.update();
}

void Model::setInitialStations(GRBModel& model)
{	
	vector<int> UK15_BSS = { 5 };
	vector<int> UK15_DPT = { 0 };
	vector<int> UK20_BSS = { 7 };
	vector<int> UK20_DPT = { 0 };
	vector<int> UK25_BSS = { 7 };
	vector<int> UK25_DPT = { 0 };
	vector<int> UK50_BSS = { 2, 4, 7 };
	vector<int> UK50_DPT = { 0, 1 };
	vector<int> UK75_BSS = { 6, 8, 9, 14, 18, 19 };
	vector<int> UK75_DPT = { 0 };
	vector<int> UK100_BSS = { 4, 7, 14, 18, 22, 28 };
	vector<int> UK100_DPT = { 0 };
	vector<int> UK150_BSS = { 14, 16, 22, 25, 30, 32, 34, 42 };
	vector<int> UK150_DPT = { 0 };
	vector<int> UK200_BSS = { 19, 22, 28, 31, 32, 33, 41, 43, 47 };
	vector<int> UK200_DPT = { 0 };

	vector<int> fixed;
	vector<int> fixed2;
	if (inst->fileName == "UK15_01.txt") {
		fixed = UK15_BSS;
		fixed2 = UK15_DPT;
	}
	else if (inst->fileName == "UK20_01.txt") {
		fixed = UK20_BSS;
		fixed2 = UK20_DPT;
	}
	else if (inst->fileName == "UK25_01.txt") {
		fixed = UK25_BSS;
		fixed2 = UK25_DPT;
	}
	else if (inst->fileName == "UK50_01.txt") {
		fixed = UK50_BSS;
		fixed2 = UK50_DPT;
	}
	else if (inst->fileName == "UK75_01.txt") {
		fixed = UK75_BSS;
		fixed2 = UK75_DPT;
	}
	else if (inst->fileName == "UK100_01.txt") {
		fixed = UK100_BSS;
		fixed2 = UK100_DPT;
	}
	else if (inst->fileName == "UK150_01.txt") {
		fixed = UK150_BSS;
		fixed2 = UK150_DPT;
	}
	else if (inst->fileName == "UK200_01.txt") {
		fixed = UK200_BSS;
		fixed2 = UK200_DPT;
	}

	// fix bss
	auto V1 = inst->set_V1();
	auto R = inst->set_R();
	/*
	for (int f : fixed) {
		GRBLinExpr c = 0;
		for (auto n : V1) {
			if (f != n.key){
				GRBVar x = getX(model, f, n.key);
				c += x;
			}
		}
		model.addConstr(c >= 1, "f(" + to_string(f) + ")");
		//GRBVar y = model.getVarByName("y(" + to_string(r) + ")");
		//model.addConstr(y == 1, "f1(" + to_string(r) + ")");
	}
	*/	
	for (auto r : R) {
		bool found = false;		
		for (auto f : fixed) {
			if (f == r.key) {
				found = true;
				break;
			}
		}
		if (found == true) {
			continue;
		}
		GRBLinExpr c = 0;
		for (auto n : V1) {
			if (r.key != n.key) {
				GRBVar x = getX(model, r.key, n.key);
				c += x;
			}
		}
		model.addConstr(c == 0, "f2(" + to_string(r.key) + ")");
	}

	// fix depots
	/*
	for (int f : fixed2) {
		GRBLinExpr c = 0;
		for (auto n : V1) {
			if (f != n.key) {
				GRBVar x = getX(model, f, n.key);
				c += x;
			}
		}
		model.addConstr(c >= 1, "f3(" + to_string(f) + ")");
	}
	*/
	for (auto r : R) {
		bool found = false;
		for (auto f : fixed2) {
			if (f == r.key) {
				found = true;
				break;
			}
		}
		if (found == true) {
			continue;
		}
		GRBLinExpr c = 0;
		for (auto n : V1) {
			if (r.key != n.key) {
				GRBVar x = getX(model, r.key, n.key);
				c += x;
			}
		}
		model.addConstr(c == 0, "f4(" + to_string(r.key) + ")");
	}

	model.update();
}

int Model::getArcs(GRBModel& model)
{
	int count = 0;

	vector<node> V0 = inst->set_V0();
	vector<node> V1 = inst->set_V1();

	for (auto i : V0) {
		vector<GRBVar> aux;
		for (auto j : V1) {
			if (i.key != j.key) {
				string varName = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";			
				GRBVar xij = model.getVarByName(varName);

				if (int(xij.get(GRB_DoubleAttr_X)) == 1) {
					count++;
				}
			}
		}
	}

	return count;
}

float Model::getTravelCost(GRBModel& model)
{
	int count = 0;

	vector<node> V0 = inst->set_V0();
	vector<node> V1 = inst->set_V1();

	for (auto i : V0) {
		vector<GRBVar> aux;
		for (auto j : V1) {
			if (i.key != j.key) {
				string varName = "x(" + to_string(i.key) + "," + to_string(j.key) + ")";
				GRBVar xij = model.getVarByName(varName);

				if (xij.get(GRB_DoubleAttr_X) == 1) {
					//cout << varName << endl;
					//cout << fixed << setprecision(2) << inst->dist(i.key, j.key) << endl;
					//cout << fixed << setprecision(2) << inst->dist(i.key, j.key) * inst->driverWage << endl;
					count += inst->getTD(i.key, j.key) * inst->driverWage;
				}
			}
		}
	}

	return count;
}

Solution Model::getSolution()
{
	return solu;
}

Solution Model::optmize()
{
	return model();
}

Solution Model::optmize(int m)
{
	this->w = m;
	return model();
}

void Model::setIS(Solution s)
{
	init = s;
	initProv = true;
}

Solution Model::analyze(Solution s)
{
	return model(s);
}

void Model::printInst()
{
	inst->print(cout);
}

bool Model::fileOpened()
{
	return inst->isOpen;
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

Solution Model::getSolution(GRBModel& model)
{
	Solution s;

	s.FO = model.get(GRB_DoubleAttr_ObjVal);
	s.FOp.push_back(model.get(GRB_DoubleAttr_ObjVal));
	GRBVar p1 = model.getVarByName("depotCost");
	s.FOp.push_back(p1.get(GRB_DoubleAttr_X));
	p1 = model.getVarByName("bssCost");
	s.FOp.push_back(p1.get(GRB_DoubleAttr_X));
	p1 = model.getVarByName("vehicleFixedCost");
	s.FOp.push_back(p1.get(GRB_DoubleAttr_X));
	p1 = model.getVarByName("drivingCost");
	s.FOp.push_back(p1.get(GRB_DoubleAttr_X));
	p1 = model.getVarByName("brsEnergyCost");
	s.FOp.push_back(p1.get(GRB_DoubleAttr_X));
	p1 = model.getVarByName("bssEnergyCost");
	s.FOp.push_back(p1.get(GRB_DoubleAttr_X));
	p1 = model.getVarByName("bssUseCost");
	s.FOp.push_back(p1.get(GRB_DoubleAttr_X));
	s.FOp.push_back(0);

	// put used arcs in the solution
	vector<node> V01 = inst->set_V01();

	// get routes
	s.routes = getRoutes2(model);

	// get arrival time
	for (node n : V01) {
		GRBVar ti = getT(model, n.key);
		double value = ti.get(GRB_DoubleAttr_X);

		for (int i = 0; i < s.routes.size(); i++) {
			for (int j = 0; j < s.routes.at(i).size(); j++) {
				if (s.routes.at(i).at(j).key == n.key) {
					s.routes.at(i).at(j).aTime = value;
				}
			}
		}
	}
	// compute departure time
	for (int i = 0; i < s.routes.size(); i++) {
		for (int j = 0; j < s.routes.at(i).size(); j++) {			
			s.routes.at(i).at(j).lTime = s.routes.at(i).at(j).aTime + inst->getNodeByKey(s.routes.at(i).at(j).key).serviceTime;
		}
	}
	// compute wait time
	for (int i = 0; i < s.routes.size(); i++) {
		for (int j = 0; j < s.routes.at(i).size(); j++) {
			s.routes.at(i).at(j).wTime = inst->getNodeByKey(s.routes.at(i).at(j).key).readyTime - s.routes.at(i).at(j).aTime;
			if (s.routes.at(i).at(j).wTime < 0) {
				s.routes.at(i).at(j).wTime = 0;
			}
		}
	}

	// get vehicle load
	for (node n : V01) {
		GRBVar di = getD(model, n.key);
		double value = di.get(GRB_DoubleAttr_X);

		for (int i = 0; i < s.routes.size(); i++) {
			for (int j = 0; j < s.routes.at(i).size(); j++) {
				if (s.routes.at(i).at(j).key == n.key) {
					s.routes.at(i).at(j).vLoad = value;
				}
			}
		}
	}
	
	// get amount of energy left, bLevel
	for (node n : V01) {
		GRBVar qi = getQ(model, n.key);
		double value = qi.get(GRB_DoubleAttr_X);

		for (int i = 0; i < s.routes.size(); i++) {
			for (int j = 0; j < s.routes.at(i).size(); j++) {
				
				
				if (s.routes.at(i).at(j).key == n.key && (n.type == "f" || n.type == "f_d")){
					s.routes.at(i).at(j).bLevel = value; //inst->Q;
				}
				else if (s.routes.at(i).at(j).key == n.key) {
					s.routes.at(i).at(j).bLevel = value;
				}
				
				
			}
		}
	}

	// Amount of energy to be charged in brs
	vector<node> C = inst->set_C();
	vector<node> SK;
	for (auto h : C) {
		auto SKc = inst->set_SK(h.key);
		SK.insert(SK.end(), SKc.begin(), SKc.end());
	}
	vector<node> CUSKc = vectorUnion(C, SK);
	
	for (node n : CUSKc) {
		GRBVar wi = getW(model, n.key);
		double value = wi.get(GRB_DoubleAttr_X);

		for (int i = 0; i < s.routes.size(); i++) {
			for (int j = 0; j < s.routes.at(i).size(); j++) {
				if (s.routes.at(i).at(j).key == n.key) {
					s.routes.at(i).at(j).recharged = value;
				}
			}
		}
	}

	// Amount of energy to be charged in bss
	vector<node> R = inst->set_R();
	SK.clear();
	for (auto h : R) {
		auto SKr = inst->set_SK(h.key);
		SK.insert(SK.end(), SKr.begin(), SKr.end());
	}
	vector<node> RUSKr = vectorUnion(R, SK);

	for (node n : RUSKr) {
		GRBVar vi = getV(model, n.key);
		double value = vi.get(GRB_DoubleAttr_X);

		for (int i = 0; i < s.routes.size(); i++) {
			for (int j = 0; j < s.routes.at(i).size(); j++) {
				if (s.routes.at(i).at(j).key == n.key) {
					s.routes.at(i).at(j).recharged = value;
				}
			}
		}
	}

	// get nodes
	for (route& r : s.routes) {
		for (vertex& v : r) {
			for (node n : inst->nodes) {
				if (v.key == n.key) {
					v.n = n;
					break;
				}
			}
		}
	}
	// get arcs
	for (route& r : s.routes) {
		for (int i = 0; i < r.size() - 1; i++) {
			arc a;
			a.beg = r.at(i).key;
			a.end = r.at(i + 1).key;
			a.value = inst->dist(a.beg, a.end);
			a.value2 = inst->getTD(a.beg, a.end);
			s.arcs.push_back(a);
		}
	}

	// get permutation
	vector<int> p;
	for (route& r : s.routes) {
		for (vertex& v : r) {
			if (v.n.type == "c" || v.n.type == "c_d") {
				p.push_back(v.key);
			}
		}
	}
	s.perm = p;

	return s;
}

routes Model::getRoutes2(GRBModel &model)
{
	routes r;

	// get used arcs
	vector<arc> arcs;

	vector<node> V0 = inst->set_V0();
	vector<node> V1 = inst->set_V1();
	vector<node> V01 = inst->set_V01();
	int totalV = V01.size();

	for (auto i : V0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				GRBVar xij = getX(model, i.key, j.key);
				float value = xij.get(GRB_DoubleAttr_X);
				if (value < 0.1 && value > -0.1) {
					value = 0;
				}
				else {
					value = 1;
				}

				if (value != 0) {
					arc a;
					a.beg = i.key;
					a.end = j.key;
					a.value = inst->dist(i, j);
					arcs.push_back(a);
				}
			}
		}
	}

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

				r.push_back(route); // add the inital arc in each route
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
			for (int i = 0; i < r.size(); i++) {
				bool found = false;
				for (int j = 0; j < r.at(i).size(); j++) {
					if (r.at(i).at(j).key == a.beg) { // if the current end of the route is equal to the begin of the arc
						vertex va;
						va.key = a.end;
						r.at(i).push_back(va); // add the end of the arc in the route
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

	for (int i = 0; i < r.size(); i++) {
		for (int j = 0; j < r.at(i).size(); j++) {
			r.at(i).at(j).n = inst->getNodeByKey(r.at(i).at(j).key);
		}
	}

	return r;
}

vector<string> Model::eval(Solution s)
{
	set<string> ret;

	routes sol = s.routes;

	for (route r : sol) {
		// check if the current route start and finish in the same depot
		if (inst->getNodeByKey(r.front().key).ogKey != inst->getNodeByKey(r.back().key).ogKey || inst->getNodeByKey(r.front().key).key != inst->getNodeByKey(r.back().key).key) {
			ret.insert("route_beg_end");
		}

		for (vertex v : r) {
			node n = inst->getNodeByKey(v.key);

			// checking time windows
			if (n.type == "c") {
				if (v.aTime + v.wTime > n.dueDate + n.serviceTime || v.aTime + v.wTime < n.readyTime) {
					// cout << "Vertex " << v.key << " arrival is " << v.aTime << " and its ready time and due date is " << n.readyTime << " and " << n.dueDate << endl;
					ret.insert("time_window");
				}
			}

			// checking battery lvl
			if (v.bLevel < 0 || v.bLevel > inst->Q) {
				// cout << "Vertex " << v.key << " and " << v.bLevel << endl;
				ret.insert("battery_level");
			}

			// checking vehicle load
			if (v.vLoad < 0 || v.vLoad > inst->c) {
				// cout << v.key << endl;
				ret.insert("vehicle_load");
			}
		}
	}

	// checking if all customers are being suplied
	for (node n : inst->nodes) {		
		if (n.type == "c") {
			int appeared = false;
			for (auto r : sol) {
				for (auto v : r) {
					if (n.key == v.key) {
						appeared = true;
					}
				}
			}
			if (appeared == false) {
				ret.insert("customers_coverage");
			}
		}
	}

	// checking if all demand is being supplied
	int demand = 0;
	for(node n : inst->nodes){
		demand += n.demand;
	}


	vector<string> r;
	for (string s : ret) {
		r.push_back(s);
	}

	return r;
}

Model::~Model()
{
}

void Model::var_x(GRBModel & model)
{
	vector<node> V0 = inst->set_V0();
	vector<node> V1 = inst->set_V1();

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
	vector<node> set_R = inst->set_R();

	for (node i : set_R) {
		string varName = "y(" + to_string(i.key) + ")";
		y.push_back(model.addVar(0, 1, 1, GRB_BINARY, varName));
	}
	model.update();
}

void Model::var_y_model2(GRBModel & model)
{
	vector<node> R = inst->set_R();
	vector<node> C = inst->set_C();
	vector<node> CUR = vectorUnion(C, R);

	for (node i : CUR) {
		string varName = "y(" + to_string(i.key) + ")";
		y.push_back(model.addVar(0, 1, 1, GRB_BINARY, varName));
	}
	model.update();
}

void Model::var_y_model4(GRBModel& model)
{
	vector<node> R = inst->set_R();

	for (node i : R) {
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

void Model::var_v(GRBModel& model)
{
	vector<node> R = inst->set_R();
	vector<node> SK;
	for (auto h : R) {
		auto SKr = inst->set_SK(h.key);
		SK.insert(SK.end(), SKr.begin(), SKr.end());
	}
	vector<node> RUSKr = vectorUnion(R, SK);	

	for (node i : RUSKr) {
		string varName = "v(" + to_string(i.key) + ")";
		model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, varName);
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
	vector<node> UD0 = inst->set_UD0();
	vector<node> UD1 = inst->set_UD1();
	vector<node> C = inst->set_C();
	vector<node> R = inst->set_R();
	vector<node> V0 = inst->set_V0();
	vector<node> V1 = inst->set_V1();
	vector<node> V01 = inst->set_V01();

	GRBLinExpr fo = 0;

	// depot cost
	for (auto i : UD0) {
		fo += getZ(model, i.key) * (inst->depotCost / inst->numC);
	}

	// bss cost
	for (auto i : R) {
		GRBVar yi = getY(model, i.key);
		fo += (inst->bssCost / inst->numC) * yi;
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

	// energy cost in bss
	vector<node> CUR = vectorUnion(C, R);
	GRBLinExpr sum = 0;
	for (node i : CUR) {
		if (i.type == "f" || i.type == "f_d") {
			GRBVar yi = getY(model, i.key);
			fo += yi * inst->Q * inst->bssEnergyCost;
		}	
	}
	
	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

void Model::depotCostFO(GRBModel& model)
{
	GRBVar v = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "depotCost");

	vector<node> UD0 = inst->set_UD0();

	GRBLinExpr e = 0;
	for (node i : UD0) {
		e += getZ(model, i.key) * (inst->depotCost / inst->depotLifetime);
		//e += getZ(model, i.key) * (inst->depotCost);
	}

	//GRBVar v = model.getVarByName("depotCost");

	model.addConstr(v == e, "depotCostR");
	model.update();
}

void Model::bssCostFO(GRBModel& model)
{
	GRBVar v = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "bssCost");

	vector<node> R = inst->set_R();

	GRBLinExpr e = 0;
	for (auto i : R) {
		e += getY(model, i.key) * (inst->bssCost / inst->bssLifetime);
		//e += getY(model, i.key) * (inst->bssCost);
	}

	model.addConstr(v == e, "bssCostR");
	model.update();
}

void Model::vehicleFixedCostFO(GRBModel& model)
{
	GRBVar v = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "vehicleFixedCost");

	vector<node> UD0 = inst->set_UD0();
	vector<node> V1 = inst->set_V1();

	GRBLinExpr e = 0;
	for (node i : UD0) {
		for (node j : V1) {
			if (i.key != j.key) {
				e += getX(model, i.key, j.key) * (inst->vehicleCost / inst->vehicleLifetime);
				//e += getX(model, i.key, j.key) * (inst->vehicleCost);
			}
		}
	}

	model.addConstr(v == e, "vehicleFixedCostR");
	model.update();
}

void Model::drivingCostFO(GRBModel& model)
{
	GRBVar v = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "drivingCost");

	vector<node> V0 = inst->set_V0();
	vector<node> V1 = inst->set_V1();

	GRBLinExpr e = 0;
	for (auto i : V0) {
		for (auto j : V1) {
			if (i.key != j.key) {
				e += getX(model, i.key, j.key) * inst->getTD(i, j) * (inst->driverWage);
			}
		}
	}

	vector<node> C = inst->set_C();
	for (node i : C) {
		e += i.serviceTime * inst->driverWage;
	}

	model.addConstr(v == e, "drivingCostR");
	model.update();
}

void Model::brsEnergyCostFO(GRBModel& model)
{
	GRBVar v = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "brsEnergyCost");

	vector<node> C = inst->set_C();
	vector<node> SK;

	GRBLinExpr e = 0;
	for (auto h : C) {
		auto SKc = inst->set_SK(h.key);
		SK.insert(SK.end(), SKc.begin(), SKc.end());
	}
	auto CUSKc = vectorUnion(C, SK);
	for (auto i : CUSKc) {
		GRBVar wi = getW(model, i.key);
		e += inst->brsEnergyCost * wi;
	}

	model.addConstr(v == e, "brsEnergyCostR");
	model.update();
}

void Model::bssEnergyCostFO(GRBModel& model)
{
	GRBVar v = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "bssEnergyCost");

	vector<node> R = inst->set_R();
	vector<node> SK;
	for (auto h : R) {
		auto SKr = inst->set_SK(h.key);
		SK.insert(SK.end(), SKr.begin(), SKr.end());
	}
	vector<node> RUSKr = vectorUnion(R, SK);

	// vector<node> C = inst->set_C();
	// vector<node> CUR = vectorUnion(C, R);

	GRBLinExpr e = 0;
	
	GRBLinExpr sum = 0;
	for (node i : RUSKr) {
		GRBVar vi = getV(model, i.key);
		e += inst->bssEnergyCost * vi;
		
	}

	model.addConstr(v == e, "bssEnergyCostR");
	model.update();
}

void Model::bssUseCostFO(GRBModel& model)
{
	GRBVar v = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, "bssUseCost");

	vector<node> V0 = inst->set_V0();
	
	vector<node> R = inst->set_R();
	vector<node> SK;
	for (auto h : R) {
		auto SKr = inst->set_SK(h.key);
		SK.insert(SK.end(), SKr.begin(), SKr.end());
	}
	vector<node> RUSKr = vectorUnion(R, SK);

	GRBLinExpr e = 0;
	for (node i : V0) {
		for (node j : RUSKr) {
			if (i.key != j.key) {
				GRBVar xij = getX(model, i.key, j.key);
				e += xij;
			}			
		}
	}

	model.addConstr(v == (e * 50), "bssUseCostR");
	model.update();
}

void Model::fo_cost_3(GRBModel& model)
{
	GRBVar p1 = model.getVarByName("depotCost");
	GRBVar p2 = model.getVarByName("bssCost");
	GRBVar p3 = model.getVarByName("vehicleFixedCost");
	GRBVar p4 = model.getVarByName("drivingCost");
	GRBVar p5 = model.getVarByName("brsEnergyCost");
	GRBVar p6 = model.getVarByName("bssEnergyCost");
	GRBVar p7 = model.getVarByName("bssUseCost");

	model.setObjective(p1 + p2 + p3 + p4 + p5 + p6 + p7, GRB_MINIMIZE);
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

				if (i.key == 5 && j.key == 15) {
					cout << "";
				}

				float td = inst->getTD(i, j);
				GRBLinExpr c = ti + (td + i.serviceTime) * xij; // verify getS

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

				model.addConstr(dj <= di - pi * xij + inst->c * (1 - xij), "c16_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::c165_M(GRBModel& model)
{
	auto V01 = inst->set_V01();

	GRBLinExpr c = 0;
	int sum = 0;
	for (auto i : V01) {
		GRBVar di = getD(model, i.key);
		float pi = i.demand; // inst->nodes.at(i.key).demand; // may cause problem at some point

		c += di;
		sum += pi;
	}
	model.addConstr(c == sum, "c165");

	model.update();
}

void Model::c17(GRBModel & model)
{
	auto UD0 = inst->set_UD0(); // UD0

	for (auto i : UD0) {
		GRBVar di = getD(model, i.key);

		model.addConstr(di <= inst->c, "c17(" + to_string(i.key) + ")");
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

void Model::c27_model4(GRBModel& model)
{
	vector<node> R = inst->set_R();

	GRBLinExpr c = 0;
	for (node i : R) {
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

void Model::c29_M_model4(GRBModel& model)
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

void Model::var_y_model3(GRBModel& model)
{
	vector<node> R = inst->set_R();
	vector<node> C = inst->set_C();
	vector<node> CUR = vectorUnion(C, R);

	for (node i : CUR) {
		string varName = "y(" + to_string(i.key) + ")";
		y.push_back(model.addVar(0, 1, 1, GRB_BINARY, varName));
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

void Model::c36_M_model4(GRBModel& model)
{
	vector<node> C = inst->set_C();

	for (node i : C) {
		GRBVar wi = getW(model, i.key);
		GRBVar yi = getY(model, i.key);
		model.addConstr(wi <= (i.serviceTime * inst->g) * yi, "c36_M(" + to_string(i.key) + ")");
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

void Model::c37_M_model4(GRBModel& model)
{
	auto C = inst->set_C();

	for (auto i : C) {
		auto SKi = inst->set_SK(i.key);
		for (auto h : SKi) {
			GRBVar wh = getW(model, h.key);
			GRBVar yi = getY(model, i.key);
			model.addConstr(wh <= (i.serviceTime * inst->g) * yi, "c37_M(" + to_string(i.key) + ")");
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
					model.addConstr(tj >= ti + tdij * xij + (wi / r) - inst->M * (1 - xij), "c38_M(" + to_string(i.key) + "," + to_string(j.key) + ")");
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

GRBVar Model::getS(GRBModel& model, int key)
{
	string varName = "s(" + to_string(key) + ")";
	try {
		GRBVar si = model.getVarByName(varName);
		return si;
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

void Model::var_f(GRBModel& model) 
{
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();

	for (auto i : V0) {
		vector<GRBVar> aux;
		for (auto j : V1) {
			if (i.key != j.key) {
				string varName = "f(" + to_string(i.key) + "," + to_string(j.key) + ")";
				aux.push_back(model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, varName));
			}
		}
		// x.push_back(aux);
	}
	model.update();
}

GRBVar Model::getF(GRBModel& model, int keya, int keyb)
{
	string varName = "f(" + to_string(keya) + "," + to_string(keyb) + ")";
	try {
		GRBVar fi = model.getVarByName(varName);
		return fi;
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

void Model::var_s(GRBModel& model)
{
	auto V01 = inst->set_V01();

	for (auto i : V01) {
		string varName = "s(" + to_string(i.key) + ")";
		d.push_back(model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, varName));
	}
	model.update();
}

void Model::r1(GRBModel& model)
{
	auto UD0 = inst->set_UD0(); // UD0

	for (node i : UD0) {
		GRBVar di = getD(model, i.key);

		model.addConstr(di <= inst->c, "r1(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::r2(GRBModel& model)
{
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();

	for (node i : V0) {
		for (node j : V1) {
			if (i.key != j.key) {
				GRBVar di = getD(model, i.key);
				GRBVar dj = getD(model, j.key);
				GRBVar xij = getX(model, i.key, j.key);
				float pi = i.demand; // inst->nodes.at(i.key).demand; // may cause problem at some point

				model.addConstr(dj <= di - pi * xij + M * (1 - xij), "r2(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::r3(GRBModel& model)
{
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();

	for (node i : V0) {
		for (node j : V1) {
			if (i.key != j.key) {
				GRBVar di = getD(model, i.key);
				GRBVar dj = getD(model, j.key);
				GRBVar xij = getX(model, i.key, j.key);
				float pi = i.demand; // inst->nodes.at(i.key).demand; // may cause problem at some point

				model.addQConstr((di - dj) * xij ==  pi, "r3(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
	}
	model.update();
}

void Model::r4(GRBModel& model)
{
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();

	GRBQuadExpr e1 = 0;
	GRBLinExpr e2 = 0;
	for (node i : V0) {
		for (node j : V1) {
			if (i.key != j.key) {
				GRBVar di = getD(model, i.key);
				GRBVar dj = getD(model, j.key);
				GRBVar xij = getX(model, i.key, j.key);
				float pj = j.demand; // inst->nodes.at(i.key).demand; // may cause problem at some point

				e1 += (di - dj) * xij;
				
				//model.addConstr((di - dj) - inst->M * xij <= pj * xij , "r4(" + to_string(i.key) + "," + to_string(j.key) + ")");
			}
		}
		e2 += i.demand;
	}

	model.addQConstr(e1 == e2, "r4");

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
		model.addConstr(e <= V1.size() * zi, "c45(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c46(GRBModel & model)
{
	vector<node> R = inst->set_R();

	for (node i : R) {
		GRBVar vi = getV(model, i.key);
		GRBVar qi = getQ(model, i.key);
		GRBVar yi = getY(model, i.key);
		model.addQConstr(vi == inst->Q * yi - qi * yi, "c46(" + to_string(i.key) + ")");
	}
	model.update();
}

void Model::c47(GRBModel& model)
{
	vector<node> R = inst->set_R();

	for (node i : R) {
		vector<node> SKi = inst->set_SK(i.key);
		for (node h : SKi) {
			GRBVar vh = getV(model, i.key);
			GRBVar qi = getQ(model, i.key);
			GRBVar yi = getY(model, i.key);
			model.addQConstr(vh == inst->Q * yi - qi * yi, "c47(" + to_string(i.key) + "," + to_string(h.key) + ")");
		}		
	}
	model.update();
}

void Model::c48(GRBModel& model)
{
	vector<node> V1 = inst->set_V1();

	vector<node> R = inst->set_R();
	vector<node> SK;
	for (auto h : R) {
		auto SKr = inst->set_SK(h.key);
		SK.insert(SK.end(), SKr.begin(), SKr.end());
	}
	vector<node> RUSKr = vectorUnion(R, SK);

	GRBLinExpr e = 0;
	for (node i : RUSKr) {
		
		GRBVar vi = getV(model, i.key);
		GRBVar qi = getQ(model, i.key);

		GRBLinExpr e = 0;
		for (node j : V1) {
			if (i.key != j.key) {
				GRBVar xij = getX(model, i.key, j.key);

				e += xij;			
			}
		}
		model.addQConstr(vi == inst->Q * e - qi * e, "c48(" + to_string(i.key) + ")");
	}

	model.update();
}

void Model::c49(GRBModel& model)
{
	vector<node> UD0 = inst->set_UD0();
	vector<node> V1 = inst->set_V1();

	GRBLinExpr e = 0;
	for (node i : UD0) {
		GRBVar zi = getZ(model, i.key);
		e += zi;

	}
	model.addConstr(e <= inst->maxD, "c49");
	model.update();
}

void Model::c50(GRBModel& model)
{
	vector<node> UD0 = inst->set_UD0();
	vector<node> V1 = inst->set_V1();

	GRBLinExpr e = 0;
	for (node i : UD0) {
		for (node j : V1) {
			GRBVar xij = getX(model, i.key, j.key);
			e += xij;
		}
	}
	model.addConstr(e <= inst->maxV, "c50");
	model.update();
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
		//exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		//exit(1);
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
		//exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		//exit(1);
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
		//exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		//exit(1);
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
		//exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		//exit(1);
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
		//exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		//exit(1);
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
		//exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		//exit(1);
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
		//exit(1);
	}
	catch (exception e) {
		cout << e.what() << endl;
		//exit(1);
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

GRBVar Model::getV(GRBModel& model, int key)
{
	string varName = "v(" + to_string(key) + ")";
	try {
		GRBVar vi = model.getVarByName(varName);
		return vi;
	}
	catch (GRBException e) {
		cout << "Error getting variable: " << varName << endl;
		cout << "Error code: " << e.getErrorCode() << endl;
		cout << "Message: " << e.getMessage() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

