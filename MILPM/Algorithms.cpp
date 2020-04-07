#include "Algorithms.h"

// this method receives a route that was modified by a neighborhood method, it will fix all parameters basicaly calculating all the route again with the nodes fixed
vector<vertex> Algorithms::fixRoute(vector<vertex> sol)
{
	// fix the route
	sol.at(0).bLevel = inst->Q;
	sol.at(0).lTime = 0;
	sol.at(0).vLoad = inst->C;
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

string Algorithms::getRow(Solution s)
{
	string res = "";
	res += inst->fileName + ";";
	res += to_string(s.status) + ";";
	res += to_string(s.FO) + ";";
	res += to_string(s.FOINIT) + ";";
	res += to_string(s.runtime) + "\n";
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
	float numDepots = 0;
	for (auto i : qt) {
		if (i == 1) {
			numDepots += 1;
		}
	}
	// depotCost = numDepots * (inst->depotCost / inst->numC);
	depotCost = numDepots * (inst->depotCost);

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
	bssCost = numBSS * (inst->bssCost);

	// vehicle fixed cost
	float vehicleCost = sol.size() * (inst->vehicleCost);
	
	// driving cost
	float drivingCost = 0;

	for (route r : sol) {
		for (int i = 0; i < r.size() - 1; i++) {
			drivingCost += inst->dist(r.at(i).key, r.at(i + 1).key) * inst->driverWage;
		}
	}

	// energy cost in brs
	float energy = 0;
	for (auto route : sol) {
		for (auto v : route) {
			if (v.n.type == "c" || v.n.type == "c_d") {
				energy += v.recharged;
			}				
		}
	}
	float brsEnergyCost = energy * inst->brsEnergyCost;

	// energy cost in bss
	energy = 0;
	for (auto route : sol) {
		for (auto v : route) {
			if (v.n.type == "f" || v.n.type == "f_d") {
				energy += v.recharged;
			}				
		}
	}
	float bssEnergyCost = energy * inst->bssEnergyCost;

	fo = depotCost + bssCost + vehicleCost + drivingCost + brsEnergyCost + bssEnergyCost;

	//int p = FOP(sol);
	float p = 0;

	fo_parcels = { fo + p, depotCost, bssCost, vehicleCost, drivingCost, brsEnergyCost, bssEnergyCost, p };

	return fo_parcels;
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
			else if (v.vLoad > inst->C) {
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
			if (v.vLoad < 0 || v.vLoad > inst->C) {
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
				// cout << "Vertex " << v.key << " and " << v.bLevel << endl;
				ret.insert("battery_level");
			}

			// checking vehicle load
			if (v.vLoad < 0 || v.vLoad > inst->C) {
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
				ret.insert("vertex_sequence");
				a = true;
				break;
			}
		}
		if (a == true) {
			break;
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
			if (v1.vLoad < 0 || v1.vLoad > inst->C) {
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
