#include "Model_PMedians.h"

void Model_PMedians::setup(GRBModel& model)
{
}

void Model_PMedians::result(GRBModel& model)
{
}

Model_PMedians::Model_PMedians(string dir, int size)
{
	this->dir = dir;
	//this->fileName = fileName;	

	//vector<int> ls = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	vector<int> ls = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	for (auto i : ls) {

		string s = "";
		if (i < 10) {
			s = "UK200_0" + to_string(i) + ".txt";
			instance i(dir, s, 4);
			depots.push_back(i.nodes.front());
			insts.push_back(i);
		}
		else {
			s = "UK200_" + to_string(i) + ".txt";
			instance i(dir, s, 4);
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
				occurency.insert(pair<string, set<int>>(n.id, { j }));
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

void Model_PMedians::model()
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

		setup(model);
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
