#include "Model_setAdapt.h"


void Model_SA::setup(GRBModel& model)
{
	varX(model);
	varY(model);
	varZ(model);
	fo(model);	
	c1(model);
	c2(model);
	c3(model);
	c4(model);
	c5(model);
	c6(model);
	c7(model);
	c9(model);
}

void Model_SA::setup_d(GRBModel& model)
{
	varX(model);
	varY(model);
	varZ(model);
	fo(model);
	c1(model);
	c2(model);
	c3(model);
	//c4(model);
	c4_d(model);
	c5(model);
	c6(model);
	//c7(model);
	//c7_d(model);
	//c8_d(model);
}

void Model_SA::varX(GRBModel& model)
{
	cout << "var x\n";
	int k = 0;
	for (auto inst : insts) {
		for (auto n1 : inst.nodes) {
			for (auto n2 : inst.nodes) {
				if (n1.id != n2.id) {
					string varName = "x(" + to_string(k) + "," + n1.id + "," + n2.id + ")";
					//cout << varName << endl;
					model.addVar(0, 1, 1, GRB_BINARY, varName);

				}
			}
		}
		k++;
	}

	model.update();
}

GRBVar Model_SA::getX(GRBModel& model, string i, string j, int k)
{
	string varName = "x(" + to_string(k) + "," + i + "," + j + ")";
	GRBVar x = model.getVarByName(varName);
	return x;
}

void Model_SA::varY(GRBModel& model)
{
	cout << "var y\n";
	int k = 0;
	for (auto j : insts) {
		for (auto i : j.nodes) {
			model.addVar(0, 1, 1, GRB_BINARY, "y(" + to_string(k) + "," + i.id + ")");
		}
		k++;
	}

	model.update();
}

GRBVar Model_SA::getY(GRBModel& model, string i, int k)
{
	string varName = "y(" + to_string(k) + "," + i + ")";
	GRBVar y = model.getVarByName(varName);
	return y;
}

void Model_SA::varZ(GRBModel& model)
{
	cout << "var z\n";
	for (auto i : sCities) {
		model.addVar(0, 1, 1, GRB_BINARY, "z(" + i + ")");
	}
	model.update();
}

GRBVar Model_SA::getZ(GRBModel& model, string i)
{
	return model.getVarByName("z(" + i + ")");
}

void Model_SA::fo(GRBModel& model)
{
	cout << "fo\n";
	GRBLinExpr fo = 0;
	int k = 0;
	for (auto inst : insts) {
		for (auto n1 : inst.nodes) {			
			for (auto n2 : inst.nodes) {
				if (n1.id != n2.id) {
					int d = dists[{n1.id, n2.id}];

					GRBVar x = getX(model, n1.id, n2.id, k);
					fo += d * x;
				}
				
			}
		}
		k++;
	}
	model.setObjective(fo, GRB_MINIMIZE);
	model.update();
}

void Model_SA::c1(GRBModel& model)
{
	cout << "c1\n";
	int k = 0;
	for (auto inst : insts) {
		for (auto n1 : inst.nodes) {
			GRBLinExpr c1 = 0;
			for (auto n2 : inst.nodes) {
				if (n1.id != n2.id) {

					GRBVar x = getX(model, n1.id, n2.id, k);
					c1 += x;
				}
			}
			model.addConstr(c1 == 1, "c1(" + n1.id + "," + to_string(k) + ")");
		}		
		k++;
	}
	model.update();
}

void Model_SA::c2(GRBModel& model)
{
	cout << "c2\n";
	int k = 0;
	for (auto inst : insts) {
		for (auto n1 : inst.nodes) {
			for (auto n2 : inst.nodes) {
				if (n1.id != n2.id) {

					GRBVar x = getX(model, n1.id, n2.id, k);
					GRBVar y = getY(model, n2.id, k);

					model.addConstr(x <= y, "c2(" + n1.id + "," + n2.id + "," + to_string(k) + ")");
				}
			}
		}
		k++;
	}
	model.update();
}

void Model_SA::c3(GRBModel& model)
{
	cout << "c3\n";
	int k = 0;
	for (auto inst : insts) {
		for (auto n1 : inst.nodes) {
			for (auto n2 : inst.nodes) {
				if (n1.id != n2.id) {
					string varName;
					GRBVar x;
					int d;

					x = getX(model, n1.id, n2.id, k);
					d = dists[{n1.id, n2.id}];
					//model.addConstr(d * x <= R / 2, "c3(" + n1.id + "," + n2.id + "," + to_string(k) + ")");
					model.addConstr(d * x <= R, "c3(" + n1.id + "," + n2.id + "," + to_string(k) + ")");
				}
			}
		}
		k++;
	}
	model.update();
}

void Model_SA::c4(GRBModel& model)
{
	cout << "c4\n";
	int k = 0;
	for (auto inst : insts) {
		GRBLinExpr c = 0;
		for (auto n1 : inst.nodes) {

			GRBVar y = getY(model, n1.id, k);
			c += y;		
			
		}
		//model.addConstr(c <= ceil(inst.nodes.size() * 0.15), "c4(" + to_string(k) + ")");
		model.addConstr(c <= p, "c4(" + to_string(k) + ")");
		k++;
	}
	model.update();
}

void Model_SA::c4_d(GRBModel& model)
{
	cout << "c4\n";
	int k = 0;
	for (auto inst : insts) {
		GRBLinExpr c = 0;
		for (auto n1 : inst.nodes) {

			GRBVar y = getY(model, n1.id, k);
			c += y;

		}
		//model.addConstr(c <= ceil(inst.nodes.size() * 0.15), "c4(" + to_string(k) + ")");
		model.addConstr(c == p, "c4(" + to_string(k) + ")");
		k++;
	}
	model.update();
}

void Model_SA::c5(GRBModel& model)
{
	cout << "c5\n";
	for (auto city : sCities) {
		GRBLinExpr c = 0;
		int k = 0;
		for (auto inst : insts) {
			bool find = false;
			for (auto n : inst.nodes) {
				if (n.id == city) {
					auto y = getY(model, city, k);
					c += y;
					find = true;
					break;
				}
			}
			if (find == true) {
				break;
			}
			k++;
		}
		int M = sCities.size() + 1;
		GRBVar z = getZ(model, city);
		model.addConstr(c <= M * z, "c5(" + city + ")");
	}
}

void Model_SA::c6(GRBModel& model)
{
	cout << "c6\n";
	int k = 0;
	for (auto inst : insts) {
		for (auto n : inst.nodes) {
			GRBVar y, z;
			y = getY(model, n.id, k);
			z = getZ(model, n.id);

			model.addConstr(y == z, "c6(" + n.id + "," + to_string(k) + ")");
		}
		k++;
	}
	model.update();
}

void Model_SA::c7(GRBModel& model)
{
	cout << "c7\n";
	GRBLinExpr c = 0;
	for (auto city : sCities) {
		c += getZ(model, city);		
	}
	//model.addConstr(c <= sCities.size() * 0.15, "c7");
	model.addConstr(c <= p, "c7");
	model.update();
}

void Model_SA::c7_d(GRBModel& model)
{
	cout << "c7\n";
	GRBLinExpr c = 0;
	for (auto city : sCities) {
		c += getZ(model, city);
	}
	model.addConstr(c <= p, "c7");
	model.update();
}

void Model_SA::c8_d(GRBModel& model)
{
	cout << "c8\n";
	int k = 0;
	for (int i = 0; i < insts.size(); i++) {
		
		GRBVar y;
		y = getY(model, depots.at(i).id, i);

		model.addConstr(y == 1, "c8(" + depots.at(i).id + "," + to_string(i) + ")");

	}
	model.update();
}

void Model_SA::c9(GRBModel& model)
{
	fstream file;
	file.open("D:/Victor/Pos-Graduacao/UFV/Research/Instances/test/depots.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file depots.txt\n";
	}
	else {
		for (int j = 0; j < 20; j++) {

			int num;
			file >> num;
			for (int i = 0; i < num; i++) {
				string dpt;
				file >> dpt;

				depotsList.insert(dpt);
			}
		}
	}
	
	for (auto dpt : depotsList) {
		GRBVar z = getZ(model, dpt);
		model.addConstr(z == 0, "c9(" + dpt + ")");
	}
	model.update();
}

void Model_SA::result(GRBModel& model)
{
	map<string, vector<int>> r;
	map<int, vector<string>> r2;
	map<int, vector<int>> r2_;
	int k = 0;
	for (auto inst : insts) {
		for (auto n1 : inst.nodes) {
			GRBVar y = getY(model, n1.id, k);
			if (y.get(GRB_DoubleAttr_X) == 1) {
				if (r.find(n1.id) == r.end()) {
					r.insert(pair<string, vector<int>>(n1.id, { k }));
				}
				else {
					r[n1.id].push_back(k);
				}

				if (r2.find(k) == r2.end()) {
					r2.insert(pair<int, vector<string>>(k, { n1.id }));
				}
				else {
					r2[k].push_back(n1.id);
				}
			}
			
		}
		k++;
	}
	
	for (auto i : r) {
		cout << i.first << ": ";
		for (auto j : i.second) {
			cout << j << " ";
		}
		cout << endl;
	}
	cout << "----------\n";
	
	for (auto i : r2) {
		cout << i.first << " - " << i.second.size() << ": ";
		for (auto j : i.second) {
			cout << j << " ";
		}
		cout << endl;
	}

	for (auto i : r2) {
		vector<int> aux;
		r2_.insert(pair<int, vector<int>>(i.first, {}));
		for (auto j : i.second) {
			
			for (auto node : insts.at(i.first).nodes) {
				if (node.id == j) {
					r2_[i.first].push_back(node.key);
					//aux.push_back(node.key);
					break;
				}
			}

		}
		//r2_.insert(i.first, {});
		//r2_[i.first] = aux;
	}
	
	for (auto i : r2_) {
		cout << i.first << " - " << i.second.size() << ": ";
		for (auto j : i.second) {
			cout << j << " ";
		}
		cout << endl;
	}
}

Model_SA::Model_SA(string dir, int size)
{
	this->dir = dir;
	//this->fileName = fileName;	

	//vector<int> ls = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	vector<int> ls = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	for (auto i : ls) {

		if (i < 10) {
			instance i(dir, "UK" + to_string(size) + "_0" + to_string(i) + ".txt", 4);
			depots.push_back(i.nodes.front());
			insts.push_back(i);
		}
		else {
			instance i(dir, "UK" + to_string(size) + "_" + to_string(i) + ".txt", 4);
			depots.push_back(i.nodes.front());
			insts.push_back(i);
		}

		for (auto n : insts.back().nodes) {
			sCities.insert(n.id);
		}

		// generate the complete distance matrix
		int i = 0;
		for (auto n : insts.back().nodes) {
			int j = 0;
			for (auto m : insts.back().nodes) {
				auto c = dists.find({ n.id, m.id });

				if (c == dists.end()) {
					int d = insts.back().distanceMatrix[i][j];
					dists.insert(pair<pair<string, string>, int>({ n.id, m.id }, d));

					//if (n.id == "Yr_Wyddgrug") {
					//	cout << n.id << " - " << m.id << " " << d << endl;
					//}


					if (d < minDist) {
						minDist = d;
					}
					if (d > maxDist) {
						maxDist = d;
					}
				}
				else {
					dists[{ n.id, m.id }] = insts.back().distanceMatrix[i][j];
				}
				j++;
			}
			i++;
		}
	}

	// parameters
	D = 10000; //10000;

	//R = maxDist / 2; // depots
	//p = 8; // depots

	R = 161000 / 2; // stations
	p = sCities.size() * 0.15; // stations

	unordered_map<string, set<int>> occurency;

	for (int j = 0; j < insts.size(); j++) {
		for (node n : insts.at(j).nodes) {
			if (occurency.find(n.id) == occurency.end()) {
				occurency.insert(pair<string, set<int>>(n.id, {j}));
			}
			else {
				occurency[n.id].insert(j);
			}
			
		}
	}

	for (auto i : occurency) {
		if (i.second.size() == 20) {
			vsDepots.push_back(i.first);
		}
	}

	for (auto i : insts) {
		cout << vsDepots.size() << " ";
		for (auto j : vsDepots) {
			for (auto k : i.nodes) {
				if (j == k.id) {
					cout << k.key << " ";
				}
			}
		}
		cout << endl;
	}

	vector<pair<string, set<int>>> occurency2;
	for (auto i : occurency) {
		occurency2.push_back(i);
	}

	sort(occurency2.begin(), occurency2.end(), 
		[](pair<string, set<int>> p1, pair<string, set<int>> p2)->bool {
			return p1.second.size() > p2.second.size();
		});

	
}

void Model_SA::model()
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
		model.set(GRB_StringAttr_ModelName, "setAdapt_" + fileName);

		setup_d(model);
		model.update();

		auto t1 = std::chrono::high_resolution_clock::now();

		model.write("lp.lp");
		model.getEnv().set(GRB_DoubleParam_TimeLimit, 28800); // 28800
		model.optimize();

		auto t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();

		// save solution
		try {
			result(model);
			model.write("sol.sol");
		}
		catch (GRBException& e) {
			cout << e.getErrorCode() << endl;
			cout << e.getMessage() << endl;
		}

		return;
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}
