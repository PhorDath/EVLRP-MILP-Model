#include "lrp_opt.h"

vector<string> lrp_opt::chooseBSS(int p, set<string> cities, map<pair<string, string>, float> dists, map<string, int> stFreq)
{
	//int p = 70;
	int n = floor(stFreq.size() * (float(p) / 100.0));
	vector<string> choosenCities;

	// store the map stFreq in a vector so it can be sorted by the frequency
	vector<pair<string, int>> stFreqVec;
	for (pair<string, int> i : stFreq) {
		string city = i.first;
		int freq = i.second;

		stFreqVec.push_back(i);
	}

	sort(stFreqVec.begin(), stFreqVec.end(), [](pair<string, int> a, pair<string, int> b) -> bool {return a.second > b.second; });

	for (auto i : stFreqVec) {
		choosenCities.push_back(i.first);
		if (choosenCities.size() == n) {
			break;
		}
	}

	return choosenCities;
}

vector<string> lrp_opt::chooseBSS_GA(int p, set<string> cities, map<pair<string, string>, float> dists, map<string, int> stFreq)
{
	BRKGA<SampleDecoder, 100> GA();
	return vector<string>();
}

vector<string> lrp_opt::chooseBSS_model(set<string> cities, map<pair<string, string>, float> dists, map<string, int> stFreq)
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
		model.set(GRB_StringAttr_ModelName, "model");

		// var x
		// x = 1 if city i is supplied by the facility in city j. x = 0, otherwise. 
		for (string i : cities) {
			for (string j : cities) {
				model.addVar(0, 1, 1, GRB_BINARY, "x(" + i + "," + j + ")");
			}
		}
		model.update();

		// var y
		// y = 1 if a facility is opened in city j . y = 0, otherwise. 
		for (string j : cities) {
			model.addVar(0, 1, 1, GRB_BINARY, "y(" + j + ")");
		}
		model.update();

		// var d
		model.addVar(0, 1, 1, GRB_BINARY, "d");
		model.update();

		// fo
		GRBLinExpr of = 0;
		of = model.getVarByName("d");
		model.setObjective(of, GRB_MINIMIZE);
		model.update();

		// c1		
		for (string i : cities) {
			GRBLinExpr c1 = 0;
			for (string j : cities) {
				c1 += model.getVarByName("x(" + i + "," + j + ")");
			}
			model.addConstr(c1 == 1, "c1(" + i + ")");
		}
		model.update();

		// c2
		for (string i : cities) {
			for (string j : cities) {
				GRBVar xij = model.getVarByName("x(" + i + "," + j + ")");
				GRBVar yj = model.getVarByName("y(" + j + ")");
				model.addConstr(xij <= yj, "c2(" + i + "," + j + ")");
			}
		}
		model.update();

		// c3
		GRBLinExpr c3 = 0;
		for (string j : cities) {
			c3 += model.getVarByName("y(" + j + ")");
		}
		model.addConstr(c3 == floor(cities.size() * 0.2));
		model.update();

		auto t1 = std::chrono::high_resolution_clock::now();

		model.write("lp.lp");
		model.getEnv().set(GRB_DoubleParam_TimeLimit, 28800); // 28800
		model.optimize();

		auto t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();

		// save solution
		try {
			model.write("sol.sol");
		}
		catch (GRBException& e) {
			cout << e.getErrorCode() << endl;
			cout << e.getMessage() << endl;
		}

		return vector<string>();
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

lrp_opt::lrp_opt(string regionName, int pct, string dir, map<string, int> stFreq)
{
	this->region = regionName;
	this->pct = pct;
	this->dir = dir + regionName + "/" + to_string(pct) + "/";
	this->stFreq = stFreq;	

	string line;
	fstream file;

	// get city list		
	file.open(dir + regionName + "/" + regionName + ".txt", ios::in);
	if (!file.is_open()) {
		cout << "error opening file " << "all.txt" << endl;
		exit(1);
	}
	while (getline(file, line)) {
		cities.insert(line);
	}
	file.close();

	// get distance matrix
	/*
	file.open(dmfile, ios::in);
	if (!file.is_open()) {
		cout << "error opening file " << dmfile << endl;
		exit(1);
	}
	while (getline(file, line)) {
		vector<string> tokens;
		string token = "";
		for (int i = 0; i < line.size(); i++) {
			if (line.at(i) != ',') {
				token = token + line.at(i);
			}
			else {
				tokens.push_back(token);
				token = "";
			}
		}
		tokens.push_back(token);

		distMatrix[{tokens.at(0), tokens.at(1)}] = stof(tokens.at(2));
	}
	*/
}

vector<string> lrp_opt::opt()
{
	int startpct = 15;
	int lastpct = startpct;
	for (int i = startpct; i <= 100; i += 5) {
		vector<string> BSS = chooseBSS(i, cities, distMatrix, stFreq);
		perm_rep alg;
		
		fstream all;
		all.open(dir + "all.txt", ios::in);
		if (all.is_open() == false) {
			throw runtime_error("Could not open file all.txt");
		}

		vector<Solution> sols;
		
		bool ok = true;

		string line;
		while (getline(all, line)) {
			perm_rep alg;
			alg.loadInstance(dir, line, 5);			
			try {
				Solution s = alg.greedl(BSS);
				sols.push_back(s);
			}
			catch (PermutationInf& e) {
				ok = false;
				break;
			}
			catch (exception& e) {
				ok = false;
				break;
			}

		}

		if (ok == true) {
			lastpct = i;
			all.close();
			return BSS;
		}
		all.close();
	}
}
