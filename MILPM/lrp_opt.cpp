#include "lrp_opt.h"

string lrp_opt::getDate()
{
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

float lrp_opt::totalCost(vector<Solution> sols, string dir, bool write)
{
	string fName = "cost.txt";
	fstream file;
	if (write == true) {
		file.open(dir + fName, ios::out);
		while (file.is_open() == false) {
			cout << "Error creating file cost.txt in directory " << dir << endl;
			cout << "enter a new file to store the cost\n";
			cin >> fName;
			file.open(dir + fName, ios::out);
		}
	}

	set<string> depots;
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
	int cont = 1;

	for (auto s : sols) {

		drivingCost = s.FOp.at(4);
		brsEnergyCost = s.FOp.at(5);
		bssEnergyCost = s.FOp.at(6);
		bssUseCost = s.FOp.at(7);

		if (write == true) {
			file << cont << " " << s.FOp.at(1) << " " << s.FOp.at(2) << " " << s.FOp.at(3) << " " << drivingCost << " " << brsEnergyCost << " " << bssEnergyCost << " " << bssUseCost << " " << (s.FOp.at(1) + s.FOp.at(2) + s.FOp.at(3) + drivingCost + brsEnergyCost + bssEnergyCost + bssUseCost) << " " << drivingCost + brsEnergyCost + bssEnergyCost + bssUseCost << endl;
		}

		for (route r : s.routes) {
			for (vertex v : r) {
				if (v.n.type == "f") {
					stations.insert(v.n.id);
				}
				else if (v.n.type == "d") {
					depots.insert(v.n.id);
				}
			}
		}


		int numV = s.routes.size();
		if (numV > maxVehicles) {
			maxVehicles = numV;
		}

		if (cont == 1) {
			depotCost = sols.front().FOp.at(1) / depots.size();
			bssCost = sols.front().FOp.at(2) / stations.size();
			vehicleCost = sols.front().FOp.at(3) / sols.front().routes.size();
		}

		cost += drivingCost + brsEnergyCost + bssEnergyCost + bssUseCost;
		cont++;
	}

	cost += vehicleCost * maxVehicles;
	cost += bssCost * stations.size();
	cost += depotCost * depots.size();

	if (write == true) {
		file << "vehicle cost: " << vehicleCost * maxVehicles << endl;
		file << "bss cost: " << bssCost * stations.size() << endl;
		file << "depot cost: " << depotCost * depots.size() << endl;
		file << "total cost: " << cost << endl;
		file.close();
	}

	return cost;
}

float lrp_opt::totalCost_Desarmotized(vector<Solution> sols, string dir, bool write)
{
	int d = 30;
	string fName = "cost_d.txt";
	fstream file;
	if (write == true) {
		file.open(dir + fName, ios::out);
		while (file.is_open() == false) {
			cout << "Error creating file cost.txt in directory " << dir << endl;
			cout << "enter a new file to store the cost\n";
			cin >> fName;
			file.open(dir + fName, ios::out);
		}
	}

	set<string> depots;
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
	int cont = 1;

	for (auto s : sols) {

		drivingCost = s.FOp.at(4);
		brsEnergyCost = s.FOp.at(5);
		bssEnergyCost = s.FOp.at(6);
		bssUseCost = s.FOp.at(7);

		if (write == true) {
			file << cont << " " << s.FOp.at(1) << " " << s.FOp.at(2) << " " << s.FOp.at(3) << " " << drivingCost << " " << brsEnergyCost << " " << bssEnergyCost << " " << bssUseCost << " " << (s.FOp.at(1) + s.FOp.at(2) + s.FOp.at(3) + drivingCost + brsEnergyCost + bssEnergyCost + bssUseCost) << " " << drivingCost + brsEnergyCost + bssEnergyCost + bssUseCost << endl;
		}

		for (route r : s.routes) {
			for (vertex v : r) {
				if (v.n.type == "f") {
					stations.insert(v.n.id);
				}
				else if (v.n.type == "d") {
					depots.insert(v.n.id);
				}
			}
		}


		int numV = s.routes.size();
		if (numV > maxVehicles) {
			maxVehicles = numV;
		}

		if (cont == 1) {
			depotCost = sols.front().FOp.at(1) / depots.size();
			bssCost = sols.front().FOp.at(2) / stations.size();
			vehicleCost = sols.front().FOp.at(3) / sols.front().routes.size();
		}

		cost += drivingCost + brsEnergyCost + bssEnergyCost + bssUseCost;
		cont++;
	}

	cost += vehicleCost * maxVehicles * sols.size();
	cost += bssCost * stations.size() * sols.size();
	cost += depotCost * depots.size() * sols.size();

	if (write == true) {
		file << "vehicle cost: " << vehicleCost * maxVehicles * sols.size() << endl;
		file << "bss cost: " << bssCost * stations.size() * sols.size() << endl;
		file << "depot cost: " << depotCost * depots.size() * sols.size() << endl;
		file << "total cost: " << cost << endl;
		file.close();
	}

	return cost;
}

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

vector<string> lrp_opt::chooseBSS_GA()
{
	
	cout << "BRKGA\n";
	const unsigned n = stFreq.size(); // Size of chromosomes;
	const unsigned p = 20; // Size of population;
	const double pe = 0.20; // Fraction of population to be the elite-set
	const double pm = 0.10; // Fraction of population to be replaced by mutants
	const double rhoe = 0.70; // Probability offspring inherits elite parent allele
	const unsigned K = 3; // Number of independent populations
	const unsigned MAXT = 1; // Number of threads for parallel decoding

	vector<string> aux;
	for (auto i : stFreq) {
		aux.push_back(i.first);
	}
	Decoder decoder(this->dir, aux); // Initialize decoder

	const long unsigned rngSeed = time(0); // Seed random number generator
	MTRand rng(rngSeed); // Initialize random number generator

	// Initialize BRKGA-based heuristic
	BRKGA<Decoder, MTRand> algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);

	unsigned generation = 1; // Current generation
	const unsigned X_INTVL = 10; // Exchange best individuals every 100 generations
	const unsigned X_NUMBER = 2; // Exchange top 2 best
	const unsigned MAX_GENS = 5; // Run for 1000 generations
	const unsigned MAX_GENS_NO_IMPROVE = 5; // Run for 1000 generations
	
	// Iterations of the algorithm ...
	float bestFitness = algorithm.getBestFitness();
	int it = 0;
	int gen = 1;
	cout << "Initial objective value = " << algorithm.getBestFitness() << endl;
	do {
		cout << "generation: " << generation << endl;

		algorithm.evolve(); // Evolve the population for one generation
		if ((++generation) % X_INTVL == 0) {
			algorithm.exchangeElite(X_NUMBER); // Exchange top individuals
		}
		
		if (algorithm.getBestFitness() < bestFitness) {
			bestFitness = algorithm.getBestFitness();
			it = 0;
		}
		else {
			it++;
		}

		cout << "Best solution found so far has objective value = " << algorithm.getBestFitness() << endl;
		gen++;
	} 
	//while (gen < MAX_GENS);
	while (it < MAX_GENS_NO_IMPROVE);
	cout << "Best solution found has objective value = " << algorithm.getBestFitness() << endl;
	auto best = algorithm.getBestChromosome();

	vector<string> BSS = decoder.getBSS(best);
	
	return BSS;

	return vector<string>();
}

vector<string> lrp_opt::chooseBSS_BRKGA()
{
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

vector<Solution> lrp_opt::vns(string dir1, string output)
{
	boost::filesystem::create_directory(output + "vns/");
	output += "vns/";

	fstream all;	
	all.open(dir1 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// csv file
	fstream csv;
	csv.open(output + "result.csv", ios::out | ios::ate);
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
	stations.open(output + "stations.txt", ios::out | ios::app);
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
		depots.open(output + "depots.txt", ios::out | ios::app);
		if (stations.is_open() == false) {
			cout << "Error opening file depots.csv\n";
			cout << "On directory " << dir1 << endl;
			//return;
		}

		// csv file
		fstream csv;
		csv.open(output + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			//return;
		}

		//////////////////
		perm_rep alg;
		alg.loadInstance(dir1, line, 5);
		//alg.printInstance();
		alg.setOutputDir(output);
		Solution s = alg.VNS(25, 1200);
		sols.push_back(s);
		///////////////////
		this->numc = alg.inst->numC;
		this->numd = alg.inst->numD;
		this->nums = alg.inst->numF;
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

	totalCost(sols, output, true);
	totalCost_Desarmotized(sols, output, true);
	all.close();

	return sols;
}

vector<Solution> lrp_opt::vnsl(string dir1, string output, vector<string> BSSs, vector<string> DPTs)
{
	boost::filesystem::create_directory(output + "vnsl/");
	output += "vnsl/";

	fstream all;
	all.open(dir1 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// save BSSs
	fstream bss;
	bss.open(output + "bss.txt", ios::out | ios::app);
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
	csv.open(output + "result.csv", ios::out | ios::ate);
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
	stations.open(output + "stations.txt", ios::out | ios::app);
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
		depots.open(output + "depots.txt", ios::out | ios::app);
		if (stations.is_open() == false) {
			cout << "Error opening file depots.csv\n";
			cout << "On directory " << dir1 << endl;
			//return;
		}

		// csv file
		fstream csv;
		csv.open(output + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			//return;
		}

		//////////////////
		perm_rep alg;
		alg.loadInstance(dir1, line, 5);
		//alg.printInstance();
		alg.setOutputDir(output);
		Solution s = alg.VNSL(BSSs, DPTs, 25, 1200);
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

	totalCost(sols, output, true);
	totalCost_Desarmotized(sols, output, true);
	all.close();

	return sols;
}

lrp_opt::lrp_opt(string dir, string regionName, int pct)
{
	this->region = regionName;
	this->pct = pct;
	this->dir = dir; // +regionName + "/" + to_string(pct) + "/";

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

lrp_opt::lrp_opt(string regionName, int pct, string dir, map<string, int> stFreq, map<string, int> dpFreq)
{
	this->region = regionName;
	this->pct = pct;
	this->dir = dir;
	this->stFreq = stFreq;
	this->dpFreq = dpFreq;
	
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
}

bool lrp_opt::opt()
{
	

	this->dirOutput = dir + "output/" + this->date + "/";
	boost::filesystem::create_directory(this->dirOutput + region + "/");
	boost::filesystem::create_directory(this->dirOutput + region + "/" + to_string(pct) + "/");

	string dir3 = this->dirOutput + region + "/" + to_string(pct) + "/";

	fstream csv; // csv file
	csv.open(dir3 + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir3 << endl;
		//return;
	}
	csv << "n,inst,VNS,VNSl,delta,pct,time\n";

	this->avgVNS_a = 0;
	this->avgVNSl_a = 0;
	this->avgVNS_b = 0;
	this->avgVNSl_b = 0;

	for (int i = 0; i < n; i++) {
		auto start = std::chrono::high_resolution_clock::now();
		

		boost::filesystem::create_directory(this->dirOutput + region + "/" + to_string(pct) + "/" + to_string(i) + "/");

		// prepare the output directory
		string dirOutput = this->dirOutput + region + "/" + to_string(pct) + "/" + to_string(i) + "/";
		boost::filesystem::create_directory(dirOutput + "/");

		string dir2 = dir + region + "/" + to_string(pct) + "/";

		vector<Solution> sols = vns(dir2, dirOutput);

		auto tcost1_a = totalCost(sols, "", false);
		auto tcost1_b = totalCost_Desarmotized(sols, "", false);
		
		this->stFreq = perm_rep::getBSSFreq(sols);
		this->dpFreq = perm_rep::getDepotFreq(sols);

		//string citiesfile = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/brelrp/alto_paranaiba/alto_paranaiba.txt";
		//string dmfile = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/brelrp/";

		//lrp_opt m(region, pct, dir, freq, freqD);
		//vector<string> BSSs = m.opt_brkga();	

		this->usedpct = opt_bss();
		//opt_brkga();

		vector<string> DPTs = this->DPTs;
		vector<string> BSSs = this->BSSs;

		for (auto i : DPTs) {
			cout << i << endl;
		}
		for (auto i : BSSs) {
			cout << i << endl;
		}

		vector<Solution> sols2 = vnsl(dir2, dirOutput, BSSs, DPTs);
		auto tcost2_a = totalCost(sols2, "", false);
		auto tcost2_b = totalCost_Desarmotized(sols2, "", false);

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

		avgVNS_a += tcost1_a;
		avgVNSl_a += tcost2_a;
		avgVNS_b += tcost1_b;
		avgVNSl_b += tcost2_b;

		csv << to_string(i) << "," << region << "_" << to_string(pct) << "," << to_string(tcost1_b) << "," << to_string(tcost2_b) << "," << to_string(tcost1_b - tcost2_b) << "," << usedpct << "," << duration << endl;
	}

	avgVNS_a /= this->n;
	avgVNSl_a /= this->n;
	avgVNS_b /= this->n;
	avgVNSl_b /= this->n;

	csv.close();

	return true;
}

int lrp_opt::opt_bss()
{
	int startpct = 70;
	bool inf = false;

	vector<string> DPTs;
	/*
	vector<pair<string, int>> temp;
	for (auto i : dpFreq) {
		temp.push_back({ i.first, i.second });
	}
	sort(temp.begin(), temp.end(), [](pair<string, int> a, pair<string, int> b) -> bool {return a.second > b.second; });
	DPTs.push_back(temp.front().first);
	*/
	for (auto i : dpFreq) {
		DPTs.push_back(i.first);
	}
	this->DPTs = DPTs;

	for (int i = startpct; i <= 100; i += 5) {
		cout << i << endl;
		inf = false;
		vector<string> BSS = chooseBSS(i, cities, distMatrix, stFreq);
		perm_rep alg;
		
		fstream all;
		string dir = this->dir + region + "/" + to_string(pct) + "/";
		all.open(dir + "all.txt", ios::in);
		if (all.is_open() == false) {
			throw runtime_error("Could not open file all.txt");
		}

		vector<Solution> sols;
		set<string> infs;
		bool ok = true;
		string line;
		while (getline(all, line)) {
			perm_rep alg;
			alg.loadInstance(dir, line, 5);	


			alg.removeDPT(DPTs);
			try {
				Solution s = alg.greedl(BSS);
				sols.push_back(s);
				//for (auto i : s.inf) {
				//	infs.insert(i);
				//}
				if (s.inf.size() > 0) {
					inf = true;
					all.close();
					break;
				}
			}
			catch (PermutationInf& e) {
				ok = false;
				all.close();
				break;
			}
			catch (exception& e) {
				ok = false;
				all.close();
				break;
			}

		}
		all.close();

		//if (inf == true) {
		//	continue;
		//}

		if (ok == true && inf == false) {

			this->BSSs = BSS;
			return i;
		}
		
	}
}

vector<string> lrp_opt::opt_brkga()
{
	return chooseBSS_GA();
}
