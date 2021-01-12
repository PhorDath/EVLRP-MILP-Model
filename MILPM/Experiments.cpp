#include "Experiments.h"

string getDate() {
	string temp;

	// pega a data para colocar no nome do arquivo
	char str[26];
	time_t result = time(NULL);
	ctime_s(str, sizeof str, &result);

	temp = str;//temp + str;
	temp.erase(remove(temp.begin(), temp.end(), '\n'), temp.end()); // remove o \n da string
	temp = temp;// + ").txt";
	replace(temp.begin(), temp.end(), ':', '.');

	return temp;
}

void exp_model(string dir1)
{
	fstream all;
	all.open(dir1 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// prepare the output directory
	string date = getDate();
	string dirOutput = dir1;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// csv file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}

	// header
	csv << "model,status,fo,gap,time\n";

	csv.close();

	int count = 0;
	string line;
	while (getline(all, line)) {
		cout << line << endl;

		// csv file
		fstream csv;
		csv.open(dirOutput + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		//////////////////
		Model model(dir1, line, 0, 5, dirOutput);
		if (model.fileOpened() == true) {
			model.printInst();
			model.optmize();
			Solution s = model.getSolution();
			s.saveXML(dirOutput + line + ".xml");
		}
		//////////////////

		csv << model.row << endl;
		csv.close();

		count++;
	}

	all.close();

	return;
}

void exp_sa(string dir1)
{
	fstream all;
	all.open(dir1 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// prepare the output directory
	string date = getDate();
	string dirOutput = dir1;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// csv file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}

	// header
	csv << "model,status,fo,fo_init,time\n";

	csv.close();

	int count = 0;
	string line;
	while (getline(all, line)) {
		cout << line << endl;

		// csv file
		fstream csv;
		csv.open(dirOutput + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		perm_rep alg;
		alg.loadInstance(dir1, line, 3);
		alg.setOutputDir(dirOutput);
		Solution s = alg.sA(1000, 100, 100, 50, 7200);
		

		csv << alg.row << endl;
		csv.close();

		count++;
	}

	all.close();

	return;
}

void exp_vns(string dir1) {
	fstream all;
	all.open(dir1 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// prepare the output directory
	string date = getDate();
	string dirOutput = dir1;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// csv file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}

	// header
	csv << "model,status,fo,fo_init,time\n";

	csv.close();

	int count = 0;
	string line;
	while (getline(all, line)) {
		cout << line << endl;

		// csv file
		fstream csv;
		csv.open(dirOutput + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		perm_rep alg;
		alg.loadInstance(dir1, line, 3);
		alg.setOutputDir(dirOutput);
		alg.printInstance();
		Solution s = alg.bVNS(25, 600);


		csv << alg.row << endl;
		csv.close();

		count++;
	}

	all.close();

	return;
}

void exp_vnsl(string dir1) {
	fstream all;
	all.open(dir1 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// prepare the output directory
	string date = getDate();
	string dirOutput = dir1;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// csv file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}

	// header
	csv << "model,status,fo,fo_init,time\n";
	csv.close();

	map<string, set<int>> occurency;
	vector<Solution> sols;

	int count = 1;
	string line;
	while (getline(all, line)) {
		cout << line << endl;

		fstream stations;
		stations.open(dirOutput + "stations.txt", ios::out | ios::app);
		if (stations.is_open() == false) {
			cout << "Error opening file stations.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		fstream depots;
		depots.open(dirOutput + "depots.txt", ios::out | ios::app);
		if (stations.is_open() == false) {
			cout << "Error opening file depots.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		// csv file
		fstream csv;
		csv.open(dirOutput + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		perm_rep alg;
		alg.loadInstance(dir1, line, 5);
		//alg.printInstance();
		alg.setOutputDir(dirOutput);
		vector<string> BSSs = {
									"Urucânia",
									"Reduto",
									"Senador_Cortes",
									"Pedra_Dourada",
									"Cipotânea",
									"Diogo_de_Vasconcelos",
									"Ewbank_da_Câmara",
									"Pedra_do_Anta",
									"Santa_Bárbara_do_Monte_Verde",
									"Patrocínio_do_Muriaé",
									"Rio_Novo",
									"Laranjal",
									"Pedra_Bonita",
									"Ervália",
									"Volta_Grande",
									"Cataguases",
									"Pedro_Teixeira",
									"Raul_Soares",
									"Lima_Duarte"
							};

		try {
			Solution s = alg.VNSL(BSSs, 25, 1200);
			sols.push_back(s);

			csv << alg.row << endl;

			map<string, int> stFreq;
			for (string i : s.sStations) {
				map<string, int>::iterator it;
				it = stFreq.find(i);
				if (it == stFreq.end()) {
					stFreq.insert({i, 0});
				}
				else {
					stFreq[i]++;
				}
			}

			//for (auto i )
			//stations << endl;


			/*
			for (auto i : s.sStations) {
				stations << i << " ";

				auto c = occurency.find(i);
				if (c == occurency.end()) {
					occurency.insert(pair<string, set<int>>(i, { count }));
				}
				else {
					occurency[i].insert(count);
				}
			}
			stations << endl;

			for (auto i : occurency) {
				cout << i.first << " ";
				for (auto j : i.second) {
					cout << j << " ";
				}
				cout << endl;
			}
			*/

			for (auto i : s.sDepots) {
				depots << i << " ";
			}
			depots << endl;
		}
		catch (PermutationInf& e) {
			cout << "Instance infeasible" << endl;
			continue;
		}
		catch (exception& e) {
			continue;
		}

		csv.close();
		stations.close();
		depots.close();

		count++;
	}

	totalCost(sols, dirOutput);
	all.close();

	return;
}

void exp_vns2(string dir1) {
	fstream all;
	all.open(dir1 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// prepare the output directory
	string date = getDate();
	string dirOutput = dir1;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// csv file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}

	// header
	csv << "model,status,fo,fo_init,time\n";	
	csv.close();

	map<string, set<int>> occurency;
	map<string, int> stFreq;
	vector<Solution> sols;

	fstream stations;
	stations.open(dirOutput + "stations.txt", ios::out | ios::app);
	if (stations.is_open() == false) {
		cout << "Error opening file stations.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}

	int count = 1;
	string line;
	while (getline(all, line)) {
		cout << line << endl;

		fstream depots;
		depots.open(dirOutput + "depots.txt", ios::out | ios::app);
		if (stations.is_open() == false) {
			cout << "Error opening file depots.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		// csv file
		fstream csv;
		csv.open(dirOutput + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		//////////////////
		perm_rep alg;
		alg.loadInstance(dir1, line, 5);
		//alg.printInstance();
		alg.setOutputDir(dirOutput);
		Solution s = alg.VNS(25, 1200);
		sols.push_back(s);
		///////////////////

		csv << alg.row << endl;

		for (string i : s.sStations) {
			map<string, int>::iterator it;
			it = stFreq.find(i);
			if (it == stFreq.end()) {
				stFreq.insert({ i, 1 });
			}
			else {
				stFreq[i]++;
			}
		}

		/*

		for (auto i : s.sStations) {			
			stations << i << " ";

			auto c = occurency.find(i);
			if (c == occurency.end()) {
				occurency.insert(pair<string, set<int>>(i, {count}) );
			}
			else {
				occurency[i].insert(count);
			}
		}
		stations << endl;

		for (auto i : occurency) {
			cout << i.first << " ";
			for (auto j : i.second) {
				cout << j << " ";
			}
			cout << endl;
		}
		*/

		for (auto i : s.sDepots) {
			depots << i << " ";
		}
		depots << endl;

		csv.close();
		
		depots.close();

		count++;
	}

	for (auto i : stFreq)
		stations << i.first << " " << i.second << endl;
	stations.close();

	//perm_rep alg;
	//alg.chooseBSS_(dir1 + "region.txt", "D:/Victor/Pos-Graduacao/UFV/Research/Instances/brelrp/alto_paranaiba/alto_paranaiba.txt", "D:/Victor/Pos-Graduacao/UFV/Research/Instances/brelrp/distmatrix_mg.txt", stFreq);

	totalCost(sols, dirOutput);
	all.close();

	return;
}

void exp_vns3(string dir1) {
	fstream all;
	all.open(dir1 + "all2.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// prepare the output directory
	string date = getDate();
	string dirOutput = dir1;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// csv file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}

	// header
	csv << "model,status,fo,fo_init,time\n";
	csv.close();

	map<string, set<int>> occurency;

	int count = 1;
	string line;
	while (getline(all, line)) {
		cout << line << endl;

		fstream stations;
		stations.open(dirOutput + "stations.txt", ios::out | ios::app);
		if (stations.is_open() == false) {
			cout << "Error opening file stations.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		fstream depots;
		depots.open(dirOutput + "depots.txt", ios::out | ios::app);
		if (stations.is_open() == false) {
			cout << "Error opening file depots.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		// csv file
		fstream csv;
		csv.open(dirOutput + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		perm_rep alg;
		alg.loadInstance(dir1, line, 3);
		alg.printInstance();
		alg.setOutputDir(dirOutput);
		Solution s = alg.VNS(25, 600);

		csv << alg.row << endl;

		for (auto i : s.sStations) {
			stations << i << " ";

			auto c = occurency.find(i);
			if (c == occurency.end()) {
				occurency.insert(pair<string, set<int>>(i, { count }));
			}
			else {
				occurency[i].insert(count);
			}
		}
		stations << endl;

		for (auto i : occurency) {
			cout << i.first << " ";
			for (auto j : i.second) {
				cout << j << " ";
			}
			cout << endl;
		}


		for (auto i : s.sDepots) {
			depots << i << " ";
		}
		depots << endl;

		csv.close();
		stations.close();
		depots.close();

		count++;
	}

	all.close();

	return;
}

void exp_grasp(string dir1)
{
	fstream all;
	all.open(dir1 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// prepare the output directory
	string date = getDate();
	string dirOutput = dir1;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// csv file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}

	// header
	csv << "model,status,fo,fo_init,time\n";

	csv.close();

	int count = 0;
	string line;
	while (getline(all, line)) {
		cout << line << endl;

		// csv file
		fstream csv;
		csv.open(dirOutput + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		perm_rep alg;
		alg.loadInstance(dir1, line, 3);
		alg.setOutputDir(dirOutput);
		Solution s = alg.GRASP(50, 300);


		csv << alg.row << endl;
		csv.close();

		count++;
	}

	all.close();

	return;
}

void exp_instances(string dir1)
{
	map<string, set<int>> cities;

	vector<int> ls = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	vector<instance> insts;
	for (auto i : ls) {

		if (i < 10) {
			instance i(dir1, "UK200_0" + to_string(i) + ".txt", 4);
			insts.push_back(i);
		}
		else {
			instance i(dir1, "UK200_" + to_string(i) + ".txt", 4);
			insts.push_back(i);
		}				
		insts.back().print(cout);
		//string s;
		//getline(cin, s);

		for (auto n : insts.back().nodes) {
			auto c = cities.find(n.id);
			if (c == cities.end()) {
				cities.insert(pair<string, set<int>>(n.id, set<int>({ i })));
			}
			else {
				cities[n.id].insert(i);
			}
		}
	}

	fstream file;
	file.open("ocurrency200.txt", ios::app | ios::in);
	if (file.is_open() == false) {
		cout << "Error creating file ocurrency200.txt\n";
		return;
	}

	for (auto i : cities) {
		file << i.first << " " << i.second.size() << " ";
		for (auto j : i.second) {
			file << j << " ";
		}
		file << endl;		
	}

	file.close();
}

void exp_lowerbound(string dir, string file)
{
	perm_rep alg;
	alg.loadInstance(dir, file, 5);
	Solution s = alg.lowerBound();
	s.debug(cout);
	return;
	fstream all;
	all.open(dir + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// prepare the output directory
	string date = getDate();
	string dirOutput = dir;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// csv file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir << endl;
		return;
	}

	// header
	csv << "model,status,fo,fo_init,time\n";
	csv.close();

	map<string, set<int>> occurency;
	vector<Solution> sols;

	int count = 1;
	string line;
	while (getline(all, line)) {
		cout << line << endl;

		fstream stations;
		stations.open(dirOutput + "stations.txt", ios::out | ios::app);
		if (stations.is_open() == false) {
			cout << "Error opening file stations.csv\n";
			cout << "On directory " << dir << endl;
			return;
		}

		fstream depots;
		depots.open(dirOutput + "depots.txt", ios::out | ios::app);
		if (stations.is_open() == false) {
			cout << "Error opening file depots.csv\n";
			cout << "On directory " << dir << endl;
			return;
		}

		// csv file
		fstream csv;
		csv.open(dirOutput + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir << endl;
			return;
		}

		//////////////////
		perm_rep alg;
		alg.loadInstance(dir, line, 5);
		Solution s = alg.lowerBound();
		//s.debug(cout);
		sols.push_back(s);
		///////////////////

		csv << alg.row << endl;

		for (auto i : s.sStations) {
			stations << i << " ";

			auto c = occurency.find(i);
			if (c == occurency.end()) {
				occurency.insert(pair<string, set<int>>(i, { count }));
			}
			else {
				occurency[i].insert(count);
			}
		}
		stations << endl;

		for (auto i : occurency) {
			cout << i.first << " ";
			for (auto j : i.second) {
				cout << j << " ";
			}
			cout << endl;
		}


		for (auto i : s.sDepots) {
			depots << i << " ";
		}
		depots << endl;

		csv.close();
		stations.close();
		depots.close();

		count++;
	}
	totalCost(sols, dirOutput);
	all.close();

	return;
}

void genBigMatrix(string dir1) {
	map<pair<string, string>, int> dists;
	set<string> cities;

	vector<int> ls = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	for (auto i : ls) {
		cout << i << endl;
		instance *inst;

		if (i < 10) {
			inst = new instance(dir1, "UK200_0" + to_string(i) + ".txt", 4);
		}
		else {
			inst = new instance(dir1, "UK200_" + to_string(i) + ".txt", 4);
		}
		//string s;
		//getline(cin, s);

		int i = 0;
		for (auto n : inst->nodes) {
			cities.insert(n.id);
			int j = 0;
			for (auto m : inst->nodes) {
				auto c = dists.find({ n.id, m.id });

				if (c == dists.end()) {
					dists.insert(pair<pair<string, string>, int>({ n.id, m.id }, inst->distanceMatrix[i][j]));
				}
				else {
					dists[{ n.id, m.id }] = inst->distanceMatrix[i][j];
				}
				j++;
			}
			i++;
		}
	}

	fstream file;
	file.open("dists200.txt", ios::ate | ios::in);
	if (file.is_open() == false) {
		cout << "error creating file dists200.txt\n";
		return;
	}

	file << cities.size() << endl;
	for (auto i : cities) {
		file << i << endl;
	}
	for (auto i : cities) {
		for (auto j : cities) {
			file << dists[{i, j}] << " ";
		}
		file << endl;
	}

	file.close();

	while (true) {
		string c1, c2;
		cout << "enter a city: ";
		cin >> c1;
		cout << "enter a city: ";
		cin >> c2;

		cout << "Distance between " << c1 << " and " << c2 << " is " << dists[{c1, c2}] << endl;

		if (c1 == "" && c2 == "") {
			break;
		}

	}

	/*
	for (auto i : dists) {
		cout << "( " << i.first.first << ", " << i.first.second << " )" << " - " << i.second << endl;
	}
	*/

}

void adapt_model(string dir)
{
	Model_SA m(dir, 25);
	//Model_PMedians m(dir, 200);
	m.model();
}

void test1()
{
	
}

vector<Solution> exp_vns_(string dir1)
{
	fstream all;
	all.open(dir1 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// prepare the output directory
	string date = getDate();
	string dirOutput = dir1;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// csv file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		//return;
	}

	// header
	csv << "model,status,fo,fo_init,time\n";
	csv.close();

	map<string, set<int>> occurency;
	map<string, int> stFreq;
	vector<Solution> sols;

	fstream stations;
	stations.open(dirOutput + "stations.txt", ios::out | ios::app);
	if (stations.is_open() == false) {
		cout << "Error opening file stations.csv\n";
		cout << "On directory " << dir1 << endl;
		//return;
	}

	int count = 1;
	string line;
	while (getline(all, line)) {
		cout << line << endl;

		fstream depots;
		depots.open(dirOutput + "depots.txt", ios::out | ios::app);
		if (stations.is_open() == false) {
			cout << "Error opening file depots.csv\n";
			cout << "On directory " << dir1 << endl;
			//return;
		}

		// csv file
		fstream csv;
		csv.open(dirOutput + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			//return;
		}

		//////////////////
		perm_rep alg;
		alg.loadInstance(dir1, line, 5);
		//alg.printInstance();
		alg.setOutputDir(dirOutput);
		Solution s = alg.VNS(25, 1200);
		sols.push_back(s);
		///////////////////

		csv << alg.row << endl;

		for (string i : s.sStations) {
			map<string, int>::iterator it;
			it = stFreq.find(i);
			if (it == stFreq.end()) {
				stFreq.insert({ i, 1 });
			}
			else {
				stFreq[i]++;
			}
		}

		/*

		for (auto i : s.sStations) {
			stations << i << " ";

			auto c = occurency.find(i);
			if (c == occurency.end()) {
				occurency.insert(pair<string, set<int>>(i, {count}) );
			}
			else {
				occurency[i].insert(count);
			}
		}
		stations << endl;

		for (auto i : occurency) {
			cout << i.first << " ";
			for (auto j : i.second) {
				cout << j << " ";
			}
			cout << endl;
		}
		*/

		for (auto i : s.sDepots) {
			depots << i << " ";
		}
		depots << endl;

		csv.close();

		depots.close();

		count++;
	}

	for (auto i : stFreq)
		stations << i.first << " " << i.second << endl;
	stations.close();

	totalCost(sols, dirOutput);
	all.close();

	return sols;
}

vector<Solution> exp_vnsl_(string dir1, vector<string> BSSs)
{
	fstream all;
	all.open(dir1 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// prepare the output directory
	string date = getDate();
	string dirOutput = dir1;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// save BSSs
	fstream bss;
	bss.open(dirOutput + "bss.txt", ios::out | ios::app);
	if (bss.is_open() == false) {
		cout << "Error opening file stations.csv\n";
		cout << "On directory " << dir1 << endl;
	}
	for (auto i : BSSs) {
		bss << i << endl;
	}
	bss.close();

	// csv file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		//return;
	}

	// header
	csv << "model,status,fo,fo_init,time\n";
	csv.close();

	map<string, set<int>> occurency;
	map<string, int> stFreq;
	vector<Solution> sols;

	fstream stations;
	stations.open(dirOutput + "stations.txt", ios::out | ios::app);
	if (stations.is_open() == false) {
		cout << "Error opening file stations.csv\n";
		cout << "On directory " << dir1 << endl;
		//return;
	}


	int count = 1;
	string line;
	while (getline(all, line)) {
		cout << line << endl;

		fstream depots;
		depots.open(dirOutput + "depots.txt", ios::out | ios::app);
		if (stations.is_open() == false) {
			cout << "Error opening file depots.csv\n";
			cout << "On directory " << dir1 << endl;
			//return;
		}

		// csv file
		fstream csv;
		csv.open(dirOutput + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			//return;
		}

		//////////////////
		perm_rep alg;
		alg.loadInstance(dir1, line, 5);
		//alg.printInstance();
		alg.setOutputDir(dirOutput);
		Solution s = alg.VNSL(BSSs, 25, 1200);
		sols.push_back(s);
		///////////////////

		csv << alg.row << endl;

		for (string i : s.sStations) {
			map<string, int>::iterator it;
			it = stFreq.find(i);
			if (it == stFreq.end()) {
				stFreq.insert({ i, 1 });
			}
			else {
				stFreq[i]++;
			}
		}

		for (auto i : s.sDepots) {
			depots << i << " ";
		}
		depots << endl;

		csv.close();
		depots.close();

		count++;
	}

	for (auto i : stFreq)
		stations << i.first << " " << i.second << endl;
	stations.close();

	totalCost(sols, dirOutput);
	all.close();

	return sols;
}

void exp(string dir, string region, int pct)
{
	string dir2 = dir + region + "/" + to_string(pct) + "/";
	vector<Solution> sols = exp_vns_(dir2);
	auto freq = perm_rep::getBSSFreq(sols);

	//string citiesfile = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/brelrp/alto_paranaiba/alto_paranaiba.txt";
	//string dmfile = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/brelrp/";

	lrp_opt m(region, pct, dir, freq);
	vector<string> BSSs = m.opt();	
	for (auto i : BSSs) {
		cout << i << endl;
	}

	vector<Solution> sols2 = exp_vnsl_(dir2, BSSs);
}

void exp1(string dir1, string dir2) {
	fstream file;
	file.open(dir1 + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// output file
	fstream csv;
	csv.open(dir2 + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result_m1.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// header
	csv << "model ; status ; fo ; gap ; time \n";

	int count = 0;
	string line;
	while (getline(file, line)) {
		if (count == MAX) {
			break;
		}
		Model m1(dir1, line, 1, dir2);
		csv << m1.row + "\n";

		Model m2(dir1, line, 2, dir2);
		csv << m2.row + "\n";

		count++;
	}

	file.close();
	csv.close();
}

void exp2(string dir1, string dir2) {
	fstream file;
	file.open(dir1 + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// output file
	fstream csv;
	csv.open(dir2 + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result_m2.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// header
	csv << "model ; status ; fo ; gap ; time \n";

	int count = 0;
	string line;
	while (getline(file, line)) {
		if (count == MAX) {
			break;
		}
		Model m1(dir1, line, 3, dir2);
		csv << m1.row + "\n";

		Model m2(dir1, line, 4, dir2);
		csv << m2.row + "\n";

		count++;
	}

	file.close();
	csv.close();
}

void exp3(string dir1, string dir2, int type) {
	fstream file;
	file.open(dir1 + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// output file
	fstream csv;
	csv.open(dir2 + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result_m3.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// header
	csv << "model ; status ; fo ; gap ; time \n";

	int count = 0;
	string line;
	while (getline(file, line)) {
		if (count == MAX) {
			break;
		}
		Model m1(dir1, line, 5, type, dir2);
		csv << m1.row + "\n";

		//Model m2(dir1, line, 6, dir2);
		//csv << m2.row + "\n";

		count++;
	}

	file.close();
	csv.close();
}

void exp4(string dir1, string dir2, int type) {
	fstream file;
	file.open(dir1 + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// output file
	fstream csv;
	csv.open(dir2 + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result_m3.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// header
	csv << "model;status;fo;gap;time\n";

	int count = 0;
	string line;
	while (getline(file, line)) {
		if (count == MAX) {
			break;
		}
		cout << line << endl;
		Model m1(dir1, line, 0, type, dir2);
		csv << m1.row + "\n";

		count++;
	}

	file.close();
	csv.close();
}

void exp_greed(string dir1, string dir2, int type) {
}

void exp_SA(string dir1, string dir2, int type) {

	fstream file;
	file.open(dir1 + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		cout << "On directory " << dir1 << endl;
		exit(1);
	}

	string date = getDate();
	string dirOutput = dir1;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// output file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}

	// header
	csv << "model;status;inf;fo;fop;fo_init;gap;time\n";

	int count = 0;
	string line;
	while (getline(file, line)) {
		cout << line << endl;
		if (count == MAX) {
			break;
		}

		perm_rep alg;
		alg.loadInstance(dir1, line, 3);
		alg.printInstance();
		//alg.sA();
		Solution s = alg.sA(1000, 100, 100, 30, 300);

		fstream output;
		output.open(dirOutput + line + ".sol", ios::out | ios::app);
		if (output.is_open() == false) {
			cout << "Error creating output file " << line + ".sol\n";
			cout << "In the directory " << dirOutput << endl;
			exit(1);
		}


		// alg.getSol(output, s);
		//alg.getSol2(output, s);
		output << endl << "#" << endl << endl;
		alg.getSol(output, s);

		string str = "";
		for (string i : s.inf) {
			str += (i + "\n");
		}

		string str2 = "";
		int i = 0;
		for (i = 0; i < s.FOp.size() - 1; i++) {
			str2 += (to_string(s.FOp.at(i)) + ", ");
		}
		str2 += (to_string(s.FOp.at(i)));

		csv << alg.inst->fileName << ";" << to_string(s.status) << ";" << str << ";" << s.FO << ";" << str2 << ";" << s.FOINIT << ";" << to_string(double(s.FO) / double(s.FOINIT)) << ";" << to_string(s.runtime) << endl;
		output.close();

		count++;
	}

	file.close();
	//csv.close();

	return;
}

int menuModel() {
	//cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	cout << "\n\n\n\n";
	int op = 1;
	cout << "Which model? \n";
	cout << "  1  - Model 1 with indicator constraints \n";
	cout << "  2  - Model 1 with big M constraints \n";
	cout << "  3  - Model 2 with indicator constraints \n";
	cout << "  4  - Model 2 with big M constraints \n";
	cout << "  5  - Model 3 with indicator constraints \n";
	cout << "  6  - Model 3 with big M constraints \n";
	cout << "  7  - All experiments with Model 1 with indicator and big M constraints \n";
	cout << "  8  - All experiments with Model 2 with indicator and big M constraints \n";
	cout << "  9  - All experiments with Model 3 with indicator and big M constraints \n";
	cout << "  10 - Model 4 \n";
	cout << "  11 - All experiments with Model 4 with indicator constraints \n";
	cin >> op;
	while (op < 0 || op > 12) {
		cout << "Invalid option, enter a new one: ";
		cin >> op;
	}
	return op;
}

string menuInstance(string curDir) {
	string fileName = "";

	vector<string> allFiles;

	fstream file;
	file.open(curDir + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt in directory " << curDir << endl;
		exit(1);
	}
	string line;
	while (getline(file, line)) {
		allFiles.push_back(line);
	}
	file.close();

	bool isin = false;

	while (fileName == "list" || fileName == "ls" || fileName == "" || isin == false) {
		cout << "\nWhich instance? \n";
		cin >> fileName;

		// check is file exists
		for (auto f : allFiles) {
			if (f == fileName) {
				isin = true;
			}
		}
		if (isin == false && (fileName != "list" && fileName != "ls")) {
			cout << "File " << fileName << "do not exist, try another" << endl;
		}

		// list all files aveilable
		if (fileName == "list" || fileName == "ls") {
			for (string f : allFiles) {
				cout << f << " | ";
			}
		}
	}

	return fileName;
}

float totalCost(vector<Solution> sols, string dir) {
	string fName = "cost.txt";
	fstream file;
	file.open(dir + fName, ios::out);
	while (file.is_open() == false) {
		cout << "Error creating file cost.txt in directory " << dir << endl;
		cout << "enter a new file to store the cost\n";
		cin >> fName;
		file.open(dir + fName, ios::out);
	}

	set<string> stations;

	float cost = 0;

	float depotCost = 0;
	float bssCost = 0;
	float vehicleCost = 0;
	float drivingCost = 0;	
	float brsEnergyCost = 0;
	float bssEnergyCost = 0;
	float bssUseCost = 0;	

	int maxVehicles = -1;
	int cont = 0;
	for (auto s : sols) {


		vehicleCost = s.FOp.at(3);
		drivingCost = s.FOp.at(4);
		brsEnergyCost = s.FOp.at(5);
		bssEnergyCost = s.FOp.at(6);
		bssUseCost = s.FOp.at(7);

		file << cont << ": " << drivingCost << " " << brsEnergyCost << " " << bssEnergyCost << " " << bssUseCost << endl;

		for (route r : s.routes) {
			for (vertex v : r) {
				if (v.n.type == "f") {
					stations.insert(v.n.id);
				}
			}
		}		
		
		if (cont == 0) {
			bssCost = s.FOp.at(2) / stations.size();
		}		

		int numV = s.routes.size();
		if (numV > maxVehicles) {
			maxVehicles = numV;
		}
		vehicleCost = vehicleCost / maxVehicles;

		cost += drivingCost + brsEnergyCost + bssEnergyCost + bssUseCost;
		cont++;
	}
	
	file << "vehicle cost: " << vehicleCost * maxVehicles << endl;

	bssCost *= stations.size();
	file << "bss cost: " << bssCost << endl;
	cost += bssCost;	

	depotCost += sols.front().FOp.at(1); // depot cost;
	file << "depot cost: " << depotCost << endl;
	cost += depotCost;

	file << "total cost: " << cost << endl;

	file.close();

	return cost;
}