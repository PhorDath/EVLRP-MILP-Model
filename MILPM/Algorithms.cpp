#include "Algorithms.h"

// this method receives a route that was modified by a neighborhood method, it will fix all parameters basicaly calculating all the route again with the nodes fixed
vector<vertex> Algorithms::fixRoute(vector<vertex> sol)
{
	// fix the route
	sol.at(0).bLevel = inst->Q;
	sol.at(0).lTime = 0;
	sol.at(0).vLoad = inst->c;
	sol.at(0).wTime = 0;
	sol.at(0).aTime = 0;

	for (int i = 1; i < sol.size(); i++) {
		vertex va = sol.at(i - 1);
		vertex vb = sol.at(i);
		node na = inst->getNodeByKey(sol.at(i - 1).key);
		node nb = inst->getNodeByKey(sol.at(i).key);

		int distAB = inst->dist(na, nb);
		int tTimeAB = inst->getTD(na, nb);

		vb.vLoad = va.vLoad - nb.demand; // vehicle load
		vb.bLevel = va.bLevel - distAB; // battery level
		vb.aTime = va.lTime + tTimeAB; // arrival time
		if (vb.aTime < nb.readyTime) { // if there is a wait time
			vb.wTime = nb.readyTime - vb.aTime; // wait time
		}
		else {
			vb.wTime = 0; // wait time
		}
		vb.lTime = vb.aTime + vb.wTime + nb.serviceTime; // leave time
		if (nb.type == "f") { // if node b is a bss
			// ad the time of swap battery in the departure time
			vb.bLevel = inst->Q;
			vb.lTime += inst->ct;
		}

		sol.at(i) = vb;
	}

	if (sol.size() > 2) {
		// if there is wait time on the second node in the route,
		// we can delay the depart time in the depot and have zero wait time in the second node
		if (sol.at(1).wTime > 0) {
			node na = inst->getNodeByKey(sol.at(0).key);
			node nb = inst->getNodeByKey(sol.at(1).key);

			sol.at(0).lTime = nb.readyTime - inst->getTD(na, nb); // delay the departure time in the depot, wait time in node b + travel time from a to b
			sol.at(1).aTime = sol.at(0).lTime + inst->getTD(na, nb);
			sol.at(1).wTime = 0; // set wait time to zero
			
		}
	} 
	return sol;
}

route Algorithms::optRoute(route r)
{
	return route();
}

bool Algorithms::tcCoverage(vector<bool> coverage)
{

	return false;
}

set<int> Algorithms::getListBSS(Solution s)
{
	set<int> ret;
	for (auto i : s.routes) {
		for (auto j : i) {
			if (inst->getNodeByKey(j.key).type == "f") {
				ret.insert(j.key);
			}
		}
	}
	return ret;
}

int Algorithms::getNumC(route r)
{
	int C = 0;
	for (auto i : r) {
		if (inst->getNodeByKey(i.key).type == "c") {
			C++;
		}
	}
	return C;
}

vector<int> Algorithms::getListC(route r)
{
	vector<int> C;

	int pos = 0;
	for (auto i : r) {
		if (inst->getNodeByKey(i.key).type == "c") {
			C.push_back(pos);
		}
		pos++;
	}

	return C;
}

int Algorithms::getNumD(Solution s)
{
	set<int> d;
	for (auto r : s.routes) {
		d.insert(r.front().key);
	}

	return d.size();
}

int Algorithms::getPreviousStatioPos(route& r, int n)
{
	while (inst->getNodeByKey(r.at(n).key).type != "f" && inst->getNodeByKey(r.at(n).key).type != "d") {
		n--;
		if (n == -1) {
			throw "error!";
		}
	}

	return n;
}


void Algorithms::getSol(ostream & strm, Solution sol)
{
	strm << inst->fileName << endl;
	strm << inst->dir << endl;

	strm << "FO: " << sol.FO << endl;
	for (auto i : sol.FOp) {
		strm << i << " ";
	}
	strm << endl;

	strm << "Stations\n";

	vector<bool> y;
	y.resize(inst->nodes.size(), false);

	for (auto r : sol.routes) {
		for (auto v : r) {
			node n = inst->getNodeByKey(v.key);
			if ((n.type == "f" || n.type == "f_d") && v.bLevel == inst->Q) {
				y.at(n.ogKey) = true;
			}

			if (n.type == "c" && v.recharge == true) {
				y.at(n.ogKey) = true;
			}
		}
	}

	for (int i = 0; i < y.size(); i++) {
		if (y.at(i) == true) {
			strm << i << " ";
		}
	}
	strm << endl;

	strm << "Routes\n";

	for (auto r : sol.routes) {
		for (auto v : r) {
			strm << v.key << " ";
		}
		strm << endl;
	}

	strm << "Detailed routes\n";

	for (auto r : sol.routes) {
		strm << "Route start at node " << r.front().key << endl << endl;
		for (int i = 0; i < r.size() - 1; i++) {
			node na, nb;
			na = inst->getNodeByKey(r.at(i).key);
			nb = inst->getNodeByKey(r.at(i + 1).key);

			strm << "Travel from " << na.key << " to " << nb.key << endl;
			strm << "Distance travelled: " << inst->dist(na, nb) << endl;
			strm << "Vehicle leave node " << na.key << " at " << r.at(i).lTime << endl;
			strm << "Vehicle arrives in node " << nb.key << " at " << r.at(i + 1).aTime << endl;
			strm << "Vehicle wait " << r.at(i + 1).wTime << endl;
			strm << "Battery level in node " << nb.key << ": " << r.at(i + 1).bLevel << endl;
			strm << "Battery is recharged in " << r.at(i + 1).recharged << endl;
			strm << "Recharge " << r.at(i + 1).recharge << endl;
			strm << "Vahicle load in node " << nb.key << ": " << r.at(i + 1).vLoad << endl;

			strm << endl;
		}
	}
}

void Algorithms::solutionToXML(Solution s)
{
	ptree tree;
	tree.add("routes.<xmlattr>.version", "1.0");
	int i = 1;

	for (route r : s.routes) {
		ptree& rt = tree.add("routes.route", "");
		for (vertex v : r) {
			ptree& vertex = rt.add("vertex", "");
			vertex.add("key", v.key);
			vertex.add("arrivalTime", v.aTime);
			vertex.add("waitingTime", v.wTime);
			vertex.add("departureTime", v.lTime);
			vertex.add("batteryLevel", v.bLevel);
			vertex.add("vehicleLoad", v.vLoad);
			vertex.add("recharged", v.recharged);

			ptree& data = vertex.add("data", "");
			data.add("id", v.n.id);
			data.add("type", v.n.type);
			data.add("x", v.n.x);
			data.add("y", v.n.y);
			data.add("demand", v.n.demand);
			data.add("serviceTime", v.n.serviceTime);
			data.add("readyTime", v.n.readyTime);
			data.add("dueDate", v.n.dueDate);

		}
		i++;
	}

	//write_xml(file, tree);
}

Solution Algorithms::createOptimialSolution1()
{
	vector<int> a{ 0, 7, 6, 3, 8, 10, 11, 5, 9, 12 };
	vector<vertex> r;
	for (auto i : a) {
		vertex v;
		v.key = i;
		r.push_back(v);
	}
	r = fixRoute(r);

	Solution s;
	s.routes.push_back(r);
	s = procSol(s);

	return s;
}
/*
void Algorithms::adaptInstance()
{
	if (inst->type != 3) {
		throw "cant adapt this instance\n";
	}

	
	
	if (inst->numC >= 50) {
		vector<pair<int, long long int>> dists;
		// choose depots
		int i = 0;
		for (node n1 : inst->nodes) {
			if (n1.type != "d") {
				long long int sum = 0;
				for (node n2 : inst->nodes) {
					sum += inst->dist(n1.key, n2.key);
					sum += inst->dist(n2.key, n1.key);
				}
				pair<int, long long int> aux = { i, sum };
				dists.push_back(aux);
			}
		}

		int d = inst->numC % (25) - 1;
		partial_sort(dists.begin(), dists.begin() + d, dists.end(), [](pair<int, long long int> a, pair<int, long long int> b)-> bool {return a.second < b.second; });

	}

	


	// choose stations candidates
	MCLPModel model(inst->dir, inst->fileName);
	vector<int> y = model.getY();

	inst->type = 2;
	fstream file;
	file.open(inst->dir + inst->fileName + "_a.txt", ios::in | ios::app);
	if (file.is_open() == false) {
		cout << "Error opening file " << inst->fileName + "_a.txt" << endl;
		cout << "In directory " << inst->dir << endl;
		exit(1);
	}

	int ig;
	string str;

	file << "depots_number                       " << inst->numD;
	file << "facilities_number                   " << inst->numF;
	file << "customers_number                    " << inst->numC;
	file << "maximum_num_statios                 " << inst->o; // maximum number of stations to be sited
	file << "LB                                  " << inst->LB;
	file << "UB                                  " << inst->UB;
	file << "M                                   " << inst->M;
	file << "distance_matrix_dimension           " << inst->mDim; // distance matrix dimension
	file << "depot_cost                " << "usd      " << inst->depotCost;
	file << "bss_cost                  " << "usd      " << inst->bssCost;
	file << "brs_cost                  " << "usd      " << inst->brsCost;
	file << "bss_energy_cost           " << "usd      " << inst->bssEnergyCost;
	file << "brs_energy_cost           " << "usd      " << inst->brsEnergyCost;
	file << "vehicle_cost              " << "usd      " << inst->vehicleCost;
	file << "driver_wage               " << "usd      " << inst->driverWage;
	file << "depot_lifetime            " << "year     " << inst->depotLifetime;
	file << "bss_lifetime              " << "year     " << inst->bssLifetime;
	file << "brs_lifetime              " << "year     " << inst->brsLifetime;
	file << "vehicle_lifetime          " << "year     " << inst->vehicleLifetime;
	file << "vehicle_range             " << "km       " << inst->vehicleRange; // vehicle autonomy
	file << "battery_capacity          " << "kwh      " << inst->Q; // battery capacity
	file << "battery_swap_time         " << "second   " << inst->ct; // battery swap time
	file << "battery_consumption_rate  " << "x        " << inst->r; // battery consumption rate
	file << "recharging_rate           " << "x        " << inst->g; // recharging rate
	file << "vehicle_load              " << "kg       " << inst->C; // vehicle load capacity
	file << "vehicle_speed             " << "km/g     " << inst->v; // speed



	int numNodes = inst->numD + inst->numF + inst->numC;


	// write nodes
	node a;
	for (int i = 0; i < numNodes; i++) {
		file >> a.key >> a.id >> a.type >> a.x >> a.y >> a.demand >> a.readyTime >> a.dueDate >> a.serviceTime >> a.ogKey;
		inst->nodes.push_back(a);
	}

	inst->distanceMatrix.resize(inst->mDim);
	for (int i = 0; i < inst->mDim; i++) {
		inst->distanceMatrix.at(i).resize(inst->mDim);
		for (int j = 0; j < inst->mDim; j++) {
			float d;
			file >> d;
			d = d;// / 1000; // convert from meters to kilometers
			inst->distanceMatrix.at(i).at(j) = d;
		}
	}


	// give all depot nodes a specific id
	int id = 1;
	for (int i = 0; i < inst->nodes.size(); i++) {
		if (inst->nodes.at(i).type == "d") {
			inst->nodes.at(i).id_n = id;
			id++;
		}
	}

	// adding depot arrival nodes
	vector<node> UD0 = inst->set_UD0();
	node n;
	for (int i = 0; i < UD0.size(); i++) {
		UD0.at(i).ref2 = i;
		n = UD0.at(i);
		n.type = "a";
		inst->nodes.push_back(n);
	}

	// organize intial nodes key
	for (int i = 0; i < inst->nodes.size(); i++) {
		inst->nodes.at(i).key = i;
		inst->nodes.at(i).ref = -1;
		inst->nodes.at(i).ref2 = i;
		//nodes.at(i).id_n = i;
	}
}
*/
string Algorithms::getRow(Solution s)
{
	string res = "";
	res += inst->fileName + ",";
	res += to_string(s.status) + ",";
	res += to_string(s.FO) + ",";
	res += to_string(s.FOINIT) + ",";
	res += to_string(s.runtime);
	cout << res << endl;


	cout << s.status << s.FO << " " << s.FOINIT << " " << s.runtime << endl;
	return res;
}


void Algorithms::printPartialRoute(vector<vertex> r)
{
	cout << fixed << "Partial route \n";
	cout << "Route start at node " << r.front().key << endl << endl;
	for (int i = 0; i < r.size() - 1; i++) {
		node na, nb;
		na = inst->getNodeByKey(r.at(i).key);
		nb = inst->getNodeByKey(r.at(i + 1).key);

		cout << "Travel from " << na.key << " to " << nb.key << endl;
		cout << "Vehicle leave node " << na.key << " at " << r.at(i).lTime << endl;
		cout << "Vehicle arrives in node " << nb.key << " at " << r.at(i + 1).aTime << endl;
		cout << "Vehicle wait " << r.at(i + 1).wTime << endl;
		cout << "Distance travelled: " << inst->dist(na, nb) << endl;
		cout << "Time travelled: " << inst->getTD(na, nb) << endl;
		cout << "Battery level in node " << nb.key << ": " << r.at(i + 1).bLevel << endl;
		cout << "Battery is recharged in " << r.at(i + 1).recharged << endl;
		cout << "Recharge " << r.at(i + 1).recharge << endl;
		cout << "Vahicle load in node " << nb.key << ": " << r.at(i + 1).vLoad << endl;
		
		cout << endl;
	}

}

void Algorithms::debugSol(vector<vector<vertex>> s)
{
	for (auto r : s) {
		for (auto v : r) {
			cout << v.key << " ";
		}
		cout << endl;
	}
	cout << "with fo " << FO(s) << endl;
	cout << "feasibility " << eval(s) << ": \n";
	for (auto i : fullEval(s)) {
		cout << i << " ";
	} 

	cout << endl;
}

Algorithms::Algorithms()
{
}

int Algorithms::loadInstance(string dir, string fileName, int type)
{
	inst = new instance(dir, fileName, type);

	return 0;
}

Solution Algorithms::procSol(Solution s)
{
	s.FOp = FOComplete(s.routes);
	s.FO = s.FOp.front();
	s.inf = fullEval(s.routes);
	//s.inf = fullEval(s);

	return s;
}

float Algorithms::FO(vector<vector<vertex>> sol)
{
	auto UD0 = inst->set_UD0();
	auto UD1 = inst->set_UD1();
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();
	auto V01 = inst->set_V01();

	float fo = 0;

	// depot cost
	float depotCost = 0;

	vector<int> qt(UD0.size(), 0);
	for (auto route : sol) {
		qt.at(route.front().key) = 1;
	}

	int numDepots = 0;
	for (auto i : qt) {
		if (i == 1) {
			numDepots += 1;
		}
	}

	depotCost = numDepots * inst->depotCost;

	// bss location cost
	float bssCost = 0;

	qt.resize(inst->nodes.size(), 0);

	for (auto route : sol) {
		for (auto v : route) {
			if (inst->getNodeByKey(v.key).type == "f") {
				qt.at(inst->getNodeByKey(v.key).ogKey) = 1;
			}			
		}
	}
	int numBSS = 0;
	for (auto i : qt) {
		numBSS += i;
	}
	bssCost = numBSS * inst->bssCost;

	// brs location cost
	float brsCost = 0;

	qt.resize(inst->nodes.size(), 0);

	for (auto route : sol) {
		for (auto v : route) {
			if (inst->getNodeByKey(v.key).type == "c" && v.recharge == 1) {
				qt.at(inst->getNodeByKey(v.key).ogKey) = 1;
			}
		}
	}
	int numBRS = 0;
	for (auto i : qt) {
		numBRS += i;
	}
	brsCost = numBRS * inst->brsCost;

	// driving cost
	float drivingCost = 0;

	for (auto route : sol) {
		for (int i = 0; i < route.size() - 1; i++) {
			drivingCost += inst->dist(route.at(i).key, route.at(i + 1).key) * inst->driverWage;
		}
	}

	// vehicle fixed cost
	float vehicleCost = sol.size() * inst->vehicleCost;

	// energy cost
	float energyCost = 0;

	for (auto route : sol) {
		for (auto v : route) {
			if (inst->getNodeByKey(v.key).type == "c") {
				energyCost += v.recharged * inst->brsEnergyCost;
			}
			else if (inst->getNodeByKey(v.key).type == "f" || inst->getNodeByKey(v.key).type == "f_d") {
				energyCost += v.recharged * inst->bssEnergyCost;
			}			
		}
	}

	// operational cost (bss)
	float operationalCost = 0;

	for (auto route : sol) {
		for (auto v : route) {
			if (inst->getNodeByKey(v.key).type == "f" || inst->getNodeByKey(v.key).type == "f_d") {
				operationalCost += 1;
			}
		}
	}
	operationalCost *= inst->bssUseCost;

	fo = depotCost + bssCost + brsCost + drivingCost + vehicleCost + energyCost + operationalCost;


	float p = FOP(sol);

	fo_parcels = { depotCost, bssCost, brsCost, drivingCost, vehicleCost, energyCost, operationalCost , p};

	return fo + p;
}

int Algorithms::routeFO(route r)
{
	auto UD0 = inst->set_UD0();
	auto UD1 = inst->set_UD1();
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();
	auto V01 = inst->set_V01();

	int fo = 0;

	// depot cost
	fo += inst->depotCost;

	// bss location cost
	set<int> bss;
	for (vertex v : r) {
		if (inst->getNodeByKey(v.key).type == "f") {
			bss.insert(v.key);
		}
	}
	fo += bss.size() * inst->bssCost;

	// brs location cost
	set<int> brs;

	for (vertex v : r) {
		if (inst->getNodeByKey(v.key).type == "c" && v.recharge == 1) {
			brs.insert(v.key);
		}
	}
	fo += brs.size() * inst->brsCost;

	// driving cost
	int drivingCost = 0;

	for (int i = 0; i < r.size() - 1; i++) {
		drivingCost += inst->dist(r.at(i).key, r.at(i + 1).key) * inst->driverWage;
	}
	

	// vehicle fixed cost
	fo += inst->vehicleCost;

	// energy cost
	int energyCost = 0;

	for (vertex v : r) {
		if (inst->getNodeByKey(v.key).type == "c") {
			fo += v.recharged * inst->brsEnergyCost;
		}
		else if (inst->getNodeByKey(v.key).type == "f" || inst->getNodeByKey(v.key).type == "f_d") {
			fo += v.recharged * inst->bssEnergyCost;
		}
	}
	
	// operational cost (bss)
	int operationalCost = 0;
	for (vertex v : r) {
		if (inst->getNodeByKey(v.key).type == "f" || inst->getNodeByKey(v.key).type == "f_d") {
			operationalCost += 1;
		}
	}
	fo += operationalCost * inst->bssUseCost;

	return fo;
}

vector<float> Algorithms::FOComplete(routes sol)
{
	//routes sol = s.routes;

	auto UD0 = inst->set_UD0();
	auto UD1 = inst->set_UD1();
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();
	auto V01 = inst->set_V01();

	float fo = 0;
	vector<float> fo_parcels;

	// depot cost
	vector<int> qt(UD0.size(), 0);
	/*
	float depotCost = 0;
	vector<int> qt(UD0.size(), 0);
	for (auto route : sol) {
		qt.at(route.front().key) = 1;
	}
	float numDepots = 0;
	for (auto i : qt) {
		if (i == 1) {
			numDepots += 1;
		}
	}
	*/
	float depotCost = 0;
	float numDepots = 0;

	set<int> qt_;
	for (auto route : sol) {
		qt_.insert(route.front().key);
	}
	numDepots = qt_.size();
	// depotCost = numDepots * (inst->depotCost / inst->numC);
	depotCost = numDepots * (inst->depotCost / inst->depotLifetime);

	// bss cost
	float bssCost = 0;

	fill(qt.begin(), qt.end(), 0);
	qt.resize(inst->nodes.size(), 0);

	for (route r : sol) {
		for (vertex v : r) {
			if (inst->getNodeByKey(v.key).type == "f" || inst->getNodeByKey(v.key).type == "f_d") {
				node n = inst->getNodeByKey(v.key);
				qt.at(n.ref2) = 1;

			}
		}
	}
	int numBSS = 0;
	for (auto i : qt) {
		numBSS += i;
	}
	// bssCost = numBSS * (inst->bssCost / inst->numC);
	bssCost = numBSS * (inst->bssCost / inst->bssLifetime);

	// vehicle fixed cost
	float vehicleCost = sol.size() * (inst->vehicleCost / inst->vehicleLifetime);
	
	// driving cost
	float drivingCost = 0;
	for (route r : sol) {
		for (int i = 0; i < r.size() - 1; i++) {
			auto n = inst->getNodeByKey(r.at(i).key);
			drivingCost += inst->getTD(r.at(i).key, r.at(i + 1).key) * inst->driverWage;
			if (n.type == "c") {
				drivingCost += n.serviceTime * inst->driverWage;
			}
		}
	}

	// energy cost in brs
	float energy = 0;
	for (route r : sol) {
		for (vertex v : r) {
			node n = inst->getNodeByKey(v.key);
			if (n.type == "c" || n.type == "c_d") {
				energy += v.recharged;
			}				
		}
	}
	float brsEnergyCost = energy * inst->brsEnergyCost;

	// energy cost in bss
	float bssEnergyCost = 0;// = energy * inst->bssEnergyCost;
	for (route r : sol) {
		energy = 0;
		for (vertex v : r) {
			node n = inst->getNodeByKey(v.key);
			if (n.type == "f" || n.type == "f_d") {
				energy += v.recharged;
			}				
		}
		bssEnergyCost += energy * inst->bssEnergyCost;
	}
	

	energy = 0;
	// bss use cost
	for (route r : sol) {
		for (vertex v : r) {
			node n = inst->getNodeByKey(v.key);
			if (n.type == "f" || n.type == "f_d") {
				energy += 1;
			}
		}
	}
	float bssUseCost = energy * 50;

	fo = depotCost + bssCost + vehicleCost + drivingCost + brsEnergyCost + bssEnergyCost + bssUseCost;

	//int p = FOP(sol);
	float p = 0;

	float FO_ = depotCost * inst->depotLifetime + bssCost * inst->bssLifetime + vehicleCost * inst->vehicleLifetime + drivingCost + brsEnergyCost + bssEnergyCost + bssUseCost;

	fo_parcels = { fo + p, depotCost, bssCost, vehicleCost, drivingCost, brsEnergyCost, bssEnergyCost, bssUseCost, p, FO_};

	return fo_parcels;
}

Solution Algorithms::fixSol(Solution s, route r_new, int pos)
{
	vector<float> FOp = s.FOp;
	vector<float> newFOp = FOp;

	float cost_r_old = 0;
	float cost_r = 0;

	// unmodified route cost
	route r_old = s.routes.at(pos);

	// driving cost old
	float drivingCost = 0;
	for (int i = 0; i < r_old.size() - 1; i++) {
		auto n = inst->getNodeByKey(r_old.at(i).key);
		drivingCost += inst->getTD(r_old.at(i).key, r_old.at(i + 1).key) * inst->driverWage;
		if (n.type == "c") {
			drivingCost += n.serviceTime * inst->driverWage;
		}
	}	
	newFOp.at(4) -= drivingCost;

	// driving cost new
	drivingCost = 0;
	for (int i = 0; i < r_new.size() - 1; i++) {
		auto n = inst->getNodeByKey(r_new.at(i).key);
		drivingCost += inst->getTD(r_new.at(i).key, r_new.at(i + 1).key) * inst->driverWage;
		if (n.type == "c") {
			drivingCost += n.serviceTime * inst->driverWage;
		}
	}
	newFOp.at(4) += drivingCost;

	// energy cost in brs old
	float energy = 0;
	cost_r_old = 0;
	for (vertex v : r_old) {
		node n = inst->getNodeByKey(v.key);
		if (n.type == "c" || n.type == "c_d") {
			energy += v.recharged;
		}
	}	
	cost_r_old += energy * inst->brsEnergyCost;
	newFOp.at(5) -= cost_r_old;

	// energy cost in brs new
	energy = 0;
	cost_r = 0;
	for (vertex v : r_new) {
		node n = inst->getNodeByKey(v.key);
		if (n.type == "c" || n.type == "c_d") {
			energy += v.recharged;
		}
	}
	cost_r += energy * inst->brsEnergyCost;
	newFOp.at(5) += cost_r;

	// energy cost in bss old
	energy = 0;
	cost_r_old = 0;
	for (vertex v : r_old) {
		node n = inst->getNodeByKey(v.key);
		if (n.type == "f" || n.type == "f_d") {
			energy += v.recharged;
		}
	}
	cost_r_old += energy * inst->bssEnergyCost;
	newFOp.at(6) -= cost_r_old;

	// energy cost in bss new
	energy = 0;
	cost_r = 0;
	for (vertex v : r_new) {
		node n = inst->getNodeByKey(v.key);
		if (n.type == "f" || n.type == "f_d") {
			energy += v.recharged;
		}
	}
	cost_r += energy * inst->bssEnergyCost;
	newFOp.at(6) += cost_r;

	// bss use cost old
	energy = 0;
	cost_r_old = 0;
	for (vertex v : r_old) {
		node n = inst->getNodeByKey(v.key);
		if (n.type == "f" || n.type == "f_d") {
			energy += 1;
		}
	}	
	cost_r_old += energy * inst->bssUseCost;
	newFOp.at(7) -= cost_r_old;

	// bss use cost new
	energy = 0;
	cost_r = 0;
	for (vertex v : r_new) {
		node n = inst->getNodeByKey(v.key);
		if (n.type == "f" || n.type == "f_d") {
			energy += 1;
		}
	}
	cost_r += energy * inst->bssUseCost;
	newFOp.at(7) += cost_r;

	//s.routes.at(pos) = r_new;

	//set<int> F_new = getListBSS(s);
	//int cost_bss_r = F_new.size() * inst->bssCost;

	// get bss in the new route
	/*
	vector<int> BSS_old;
	for (vertex v : s.routes.at(pos)) {
		BSS_old.push_back(v.key);
	}

	vector<int> BSS_new;
	for (vertex v : r_new) {
		BSS_new.push_back(v.key);
	}
	*/

	s.inf = evalRoute(r_new);

	int s1 = 0;
	for (auto i : r_old) {
		if (inst->getNodeByKey(i.key).type == "c") {
			s1++;
		}
	}
	int s2 = 0;
	for (auto i : r_new) {
		if (inst->getNodeByKey(i.key).type == "c") {
			s2++;
		}
	}

	if (s1 != s2) {
		s.inf.push_back("customers_coverage");
	}

	// fo_parcels = { fo + p, depotCost, bssCost, vehicleCost, drivingCost, brsEnergyCost, bssEnergyCost, bssUseCost, p, FO_};

	newFOp.at(0) -= (FOp.at(2) + FOp.at(4) + FOp.at(5) + FOp.at(6) + FOp.at(7));
	newFOp.at(0) += (newFOp.at(2) - newFOp.at(4) + newFOp.at(5) + newFOp.at(6) + newFOp.at(7));
	
	
	s.FOp = newFOp;

	s.routes.at(pos) = r_new;
	set<int> BSS;
	for (route r : s.routes) {
		for (vertex v : r) {
			if (inst->getNodeByKey(v.key).type == "f") {
				BSS.insert(v.key);
			}
		}
	}
	s.FOp.at(2) = BSS.size() * (inst->bssCost / inst->bssLifetime);
	
	s.FOp.at(0) = s.FOp.at(1) + s.FOp.at(2) + s.FOp.at(3) + s.FOp.at(4) + s.FOp.at(5) + s.FOp.at(6) + s.FOp.at(7);
	s.FO = s.FOp.at(0);

	return s;
}

vector<float> Algorithms::FOComplete_old(vector<vector<vertex>> sol)
{
	auto UD0 = inst->set_UD0();
	auto UD1 = inst->set_UD1();
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();
	auto V01 = inst->set_V01();

	float fo = 0;
	vector<float> fo_parcels;

	// depot cost
	float depotCost = 0;

	vector<int> qt(UD0.size(), 0);
	for (auto route : sol) {
		qt.at(route.front().key) = 1;
	}

	int numDepots = 0;
	for (auto i : qt) {
		if (i == 1) {
			numDepots += 1;
		}
	}

	depotCost = numDepots * inst->depotCost;

	// bss location cost
	float bssCost = 0;

	qt.resize(inst->nodes.size(), 0);

	for (auto route : sol) {
		for (auto v : route) {
			if (inst->getNodeByKey(v.key).type == "f") {
				qt.at(inst->getNodeByKey(v.key).ogKey) = 1;
			}
		}
	}
	int numBSS = 0;
	for (auto i : qt) {
		numBSS += i;
	}
	bssCost = numBSS * inst->bssCost;

	// brs location cost
	float brsCost = 0;

	qt.resize(inst->nodes.size(), 0);

	for (auto route : sol) {
		for (auto v : route) {
			if (inst->getNodeByKey(v.key).type == "c" && v.recharge == 1) {
				qt.at(inst->getNodeByKey(v.key).ogKey) = 1;
			}
		}
	}
	int numBRS = 0;
	for (auto i : qt) {
		numBRS += i;
	}
	brsCost = numBRS * inst->brsCost;

	// driving cost
	float drivingCost = 0;

	for (auto route : sol) {
		for (int i = 0; i < route.size() - 1; i++) {
			drivingCost += inst->dist(route.at(i).key, route.at(i + 1).key) * inst->driverWage;
		}
	}

	// vehicle fixed cost
	float vehicleCost = sol.size() * inst->vehicleCost;

	// energy cost
	float energyCost = 0;

	for (auto route : sol) {
		for (auto v : route) {
			if (inst->getNodeByKey(v.key).type == "c") {
				energyCost += v.recharged * inst->brsEnergyCost;
			}
			else if (inst->getNodeByKey(v.key).type == "f" || inst->getNodeByKey(v.key).type == "f_d") {
				energyCost += v.recharged * inst->bssEnergyCost;
			}
		}
	}

	// operational cost (bss)
	float operationalCost = 0;

	for (auto route : sol) {
		for (auto v : route) {
			if (inst->getNodeByKey(v.key).type == "f" || inst->getNodeByKey(v.key).type == "f_d") {
				operationalCost += 1;
			}
		}
	}
	operationalCost *= inst->bssUseCost;

	fo = depotCost + bssCost + brsCost + drivingCost + vehicleCost + energyCost + operationalCost;

	float p = FOP(sol);

	fo_parcels = { fo + p, depotCost, bssCost, brsCost, drivingCost, vehicleCost, energyCost, operationalCost , p };

	return fo_parcels;
}

// punishment cost
// we calculate an aditional cost to add in the objective function to make make a punishment to infeasible solutions
int Algorithms::FOP(vector<vector<vertex>> sol)
{
	int sum1 = 0;
	int sum2 = 0;
	int sum3 = 0;
	for (auto route : sol) {

		for (auto v : route) {
			node n = inst->getNodeByKey(v.key);

			// time windows
			if (v.aTime > n.dueDate) {
				sum1 += v.aTime - n.dueDate;
			}

			// battery lvl
			if (v.bLevel < 0) {
				sum2 += abs(v.bLevel);
			}

			// vehicle load
			if (v.vLoad < 0) {
				sum3 += abs(v.vLoad);
			}
			else if (v.vLoad > inst->c) {
				sum3 += abs(v.vLoad);
			}
		}
	}

	return (inst->nodes.size()) * (sum1 + sum2 + sum3);
}

int Algorithms::eval(vector<vector<vertex>> sol)
{
	for (auto route : sol) {
		// check if the current route start and finish in the same depot
		if (inst->getNodeByKey(route.front().key).ogKey != inst->getNodeByKey(route.back().key).ogKey) {
			return 5;
		}

		for (auto v : route) {
			node n = inst->getNodeByKey(v.key);

			// checking time windows
			if (n.type == "c") {
				if (v.aTime + v.wTime > n.dueDate + n.serviceTime || v.aTime + v.wTime < n.readyTime) {
					return 1;
				}
			}

			// checking battery lvl
			if (v.bLevel < 0 || v.bLevel > inst->Q) {
				return 2;
			}

			// checking vehicle load
			if (v.vLoad < 0 || v.vLoad > inst->c) {
				return 3;
			}
		}
	}
	
	for (auto n : inst->nodes) {
		// checking if all customers are being suplied
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
				return 4;
			}
		}
	}

	return 0;
}

// make a full evaluation of a given solution and return a vector with all infeasibilities found
vector<string> Algorithms::fullEval(vector<vector<vertex>> sol)
{
	set<string> ret;
	for (auto route : sol) {
		// check if the current route start and finish in the same depot
		if (inst->getNodeByKey(route.front().key).ogKey != inst->getNodeByKey(route.back().key).ogKey) {
			ret.insert("route_beg_end");
		}


		if (inst->getNodeByKey(route.back().key).type != "a") {
			ret.insert("route_end");
		}

		for (auto v : route) {
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
				cout << "Vertex " << v.key << " and " << v.bLevel << endl;
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
	for (auto n : inst->nodes) {		
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

	// checking demand supply
	int totalDemand = 0;
	for (node n : inst->nodes) {
		totalDemand += n.demand;
	}
	for (int i = 0; i < sol.size(); i++) {
		for (int j = 0; j < sol.at(i).size(); j++) {

		}
	}

	// checking vertex sequence, if there are two repeated consecutive vertexes
	bool a = false;
	for (route r : sol) {
		for (int i = 0; i < r.size() - 1; i++) {
			if(r.at(i).key == r.at(i + 1).key) {
				ret.insert("consecutive_vertex");
				a = true;
				break;
			}
		}
		if (a == true) {
			break;
		}
	}

	for (route r : sol) {
		if (inst->getNodeByKey(r.front().key).type != "d") {
			ret.insert("begin");
		}
		if (inst->getNodeByKey(r.back().key).type != "a") {
			ret.insert("end");
		}
	}

	vector<string> r;
	for (string s : ret) {
		r.push_back(s);
	}

	return r;
}

vector<string> Algorithms::fullEval(Solution s)
{
	set<string> ret;
	for (route r : s.routes) {
		// check if the current route start and finish in the same depot
		if (r.front().n.ogKey != r.back().n.ogKey) {
			ret.insert("route_beg_end");
		}

		for (int i = 1; i < r.size(); i++) {
			// checking time windows
			vertex v1 = r.at(i);
			vertex v2 = r.at(i + 1);
			if (v2.aTime != v1.aTime + v1.n.serviceTime + inst->getTD(v1.key, v2.key)) {
				ret.insert("time_window");
			}
			
			// checking battery lvl
			if (v1.bLevel < 0 || v1.bLevel > inst->Q) {
				// cout << "Vertex " << v.key << " and " << v.bLevel << endl;
				ret.insert("battery_level");
			}

			// checking vehicle load
			if (v1.vLoad < 0 || v1.vLoad > inst->c) {
				// cout << v.key << endl;
				ret.insert("vehicle_load");
			}
		}
	}

	// checking if all customers are being suplied
	for (auto n : inst->nodes) {
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

	// checking demand supply
	int totalDemand = 0;
	for (node n : inst->nodes) {
		totalDemand += n.demand;
	}
	for (int i = 0; i < sol.size(); i++) {
		for (int j = 0; j < sol.at(i).size(); j++) {

		}
	}

	vector<string> r;
	for (string s : ret) {
		r.push_back(s);
	}

	return r;
}

vector<string> Algorithms::evalRoute(route r)
{
	set<string> ret;

	// check if the current route start and finish in the same depot
	if (inst->getNodeByKey(r.front().key).ogKey != inst->getNodeByKey(r.back().key).ogKey) {
		ret.insert("route_beg_end");
	}

	for (auto v : r) {
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

	// checking vertex sequence, if there are two repeated consecutive vertexes
	bool a = false;

	for (int i = 0; i < r.size() - 1; i++) {
		if (r.at(i).key == r.at(i + 1).key) {
			ret.insert("consecutive_vertex");
			a = true;
			break;
		}
	}	

	if (inst->getNodeByKey(r.front().key).type != "d") {
		ret.insert("begin");
	}
	if (inst->getNodeByKey(r.back().key).type != "a") {
		ret.insert("end");
	}
	

	vector<string> ret2;
	for (string s : ret) {
		ret2.push_back(s);
	}

	return ret2;
}

void Algorithms::printInstance()
{
	inst->print(cout);
}

void Algorithms::getSol(Solution sol, ostream & strm)
{
	strm << inst->fileName << endl;
	strm << inst->dir << endl;

	strm << "FO: " << sol.FO << endl;
	for (auto i : sol.FOp) {
		strm << i << " ";
	}
	strm << endl;

	strm << "Stations\n";

	vector<bool> y;
	y.resize(inst->nodes.size(), false);

	for (auto r : sol.routes) {
		for (auto v : r) {
			node n = inst->getNodeByKey(v.key);
			if ((n.type == "f" || n.type == "f_d") && v.bLevel == inst->Q) {
				y.at(n.ogKey) = true;
			}

			if (n.type == "c" && v.recharge == true) {
				y.at(n.ogKey) = true;
			}
		}
	}

	for (int i = 0; i < y.size(); i++) {
		if (y.at(i) == true) {
			strm << i << " ";
		}
	}
	strm << endl;

	strm << "Routes\n";

	for (auto r : sol.routes) {
		for (auto v : r) {
			strm << v.key << " ";
		}
		strm << endl;
	}

	strm << "Detailed routes\n";

	for (auto r : sol.routes) {
		strm << "Route start at node " << r.front().key << endl << endl;
		for (int i = 0; i < r.size() - 1; i++) {
			node na, nb;
			na = inst->getNodeByKey(r.at(i).key);
			nb = inst->getNodeByKey(r.at(i + 1).key);

			strm << fixed;
			strm << "Travel from " << na.key << " to " << nb.key << endl;
			strm << "Distance travelled: " << inst->dist(na, nb) << endl;
			strm << "Time travelled: " << inst->getTD(na, nb) << endl;
			strm << "Vehicle leave node " << na.key << " at " << r.at(i).lTime << endl;
			strm << "Vehicle arrives in node " << nb.key << " at " << r.at(i + 1).aTime << endl;
			strm << "Vehicle wait " << r.at(i + 1).wTime << endl;
			strm << "Battery level in node " << nb.key << ": " << r.at(i + 1).bLevel << endl;
			strm << "Battery is recharged in " << r.at(i + 1).recharged << endl;
			strm << "Recharge " << r.at(i + 1).recharge << endl;
			strm << "Vahicle load in node " << nb.key << ": " << r.at(i + 1).vLoad << endl;

			strm << endl;
		}
	}
}

void Algorithms::getSol2(Solution sol, ostream & strm)
{
	for (auto r : sol.routes) {
		for (auto v : r) {
			strm << v.key << " ";
		}
		strm << endl;
	}

	strm << fixed;
	for (int i = 0; i < sol.routes.size(); i++) {
		strm << "route " << i << endl;
		for (int j = 0; j < sol.routes.at(i).size() - 1; j++) {
			vertex v = sol.routes.at(i).at(j);
			strm << "node           : " << v.key << endl;
			strm << "battery level  : " << v.bLevel << endl;
			strm << "arrival time   : " << v.aTime << endl;
			strm << "waiting time   : " << v.wTime << endl;
			strm << "service time   : " << inst->getNodeByKey(v.key).serviceTime << endl;
			strm << "departure time : " << v.lTime << endl;
			strm << "vehicle load   : " << v.vLoad << endl;			
			strm << "recharge       : " << v.recharge << endl;
			strm << "recharged      : " << v.recharged << endl;
			strm << endl;

			vertex w = sol.routes.at(i).at(j + 1);

			strm << "arc from    " << v.key << " to " << w.key << endl;
			strm << "distance    " << inst->dist(v.key, w.key) << endl;
			strm << "time        " << inst->getTD(v.key, w.key) << endl;
			strm << "battery req " << inst->getBatteryUsed(v.key, w.key) << endl;
			strm << endl;
		}

		vertex v = sol.routes.at(i).back();
		strm << "node           : " << v.key << endl;
		strm << "battery level  : " << v.bLevel << endl;
		strm << "arrival time   : " << v.aTime << endl;
		strm << "waiting time   : " << v.wTime << endl;
		strm << "service time   : " << inst->getNodeByKey(v.key).serviceTime << endl;
		strm << "departure time : " << v.lTime << endl;
		strm << "vehicle load   : " << v.vLoad << endl;
		strm << "recharge       : " << v.recharge << endl;
		strm << "recharged      : " << v.recharged << endl;
		strm << endl;

		strm << endl;
	}

}

Algorithms::~Algorithms()
{

}
