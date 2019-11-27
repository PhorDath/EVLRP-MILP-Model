#include "Algorithms.h"


// check if all customers are covered
bool Algorithms::allCovered()
{
	for (int i = 0; i < inst->nodes.size(); i++) {
		if (coverage.at(i) == false && inst->nodes.at(i).type == "c") {
			return false;
		}
	}
	return true;
}

// choose a depot node to be the route begining
int Algorithms::depotHeuristc()
{
	// get the distances sum from the arcs leaving each depot for clients nodes not yet covered
	int d = -1, d_sum = 99999999; // will hold the depot index with the lowest ditance sum 
	vector<int> sums;
	sums.resize(inst->nodes.size(), 0);
	for (node i : inst->nodes) {
		if (i.type == "d") { // if node i is a depot node
			for (node j : inst->nodes) {			
				if (j.type == "c" && isCovered(j.key) == false) {
					sums.at(i.key) += inst->dist(i.ogKey, j.ogKey);
				}				
			}
			// check if the current sum is the lowest so far
			if (sums.at(i.key) < d_sum) {
				d = i.key;
				d_sum = sums.at(i.key);
			}
		}		
	}

	// the depot d will be returned as the inital node for the new route
	return d;
}

// check if a node identified by the key is covered
bool Algorithms::isCovered(int key)
{
	return coverage.at(key);
}

// choose the next node to be added in a route
// the node must have the lowest ready time. also we check if is possible to reach a station node with the battery level left when the vehicle arrives at the node.
int Algorithms::chooseNextNode(vector<vertex> partialRoute)
{
	return getNearestFeasibleNode(partialRoute); // get the nearest customer node;
}

// v is the las vertex in the partial route
int Algorithms::getNearestFeasibleNode(vector<vertex> &route)
{
	vertex v = route.back();
	node n;
	n.key = -1;
	int dist = 999999999;
	for (int i = 0; i < inst->nodes.size(); i++) {
		node a = inst->getNodeByKey(v.key);
		node b = inst->nodes.at(i);

		if (a.ogKey != b.ogKey) { // original keys must be different
			if (route.size() == 2 && b.type == "a") {
				if(inst->getNodeByKey(route.at(0).key).type == "d" && inst->getNodeByKey(route.at(1).key).type == "f")
					continue;
			}
			if ((a.type == "d" && b.type == "f") || (a.type == "d" && b.type == "f_d")) { // doesnt allow a route that begins in the depot and goes directly to a bss																						  //cout << 11 << endl;
				continue;
			}
			if ((a.type == "f" && b.type == "f") || (a.type == "f_d" && b.type == "f") || (a.type == "f" && b.type == "f_d") || (a.type == "f_d" && b.type == "f_d")) { // doesnt allow the vehicle travel from a bss to another bss
				//cout << 21 << endl;
				continue;
			}
			if (b.type == "f" && v.bLevel - inst->dist(a, b) >= 0.30 * inst->Q) {
				//cout << 31 << endl;
				//continue;
			}
			if (a.type == "d" && b.type == "a") {
				//cout << 41 << endl;
				continue;
			}
			if (b.type == "d") {
				//cout << 51 << endl;
				continue;
			}
			if (a.type == "d" && b.type == "d") {
				//cout << 61 << endl;
				continue;
			}
			if (b.type == "a" && b.id_n != inst->getNodeByKey(route.front().key).id_n) {
				//cout << 71 << endl;
				continue;
			}

			int distKI = inst->dist(a, b);
			vector<int> f = checkFeasibility(route, b);
			vector<int> t = { 1, 1, 1 };

			if (b.key == 12) {
				cout << checkFeasibility(route, b).at(0) << checkFeasibility(route, b).at(1) << checkFeasibility(route, b).at(2) << endl;
			}

			if (distKI < dist && coverage.at(i) == false && f == t) {
				// check if this node will not be isolated and lead to a infeasible solution
				vector<vertex> route_aux = addVertexToRoute(route, b.key);

				int nextNode = getNearestFeasibleNode2(route_aux);

				if (nextNode != -1) {
					dist = distKI;
					n = inst->nodes.at(i);					
				}

				coverage.at(b.key) = false;
			}
		}
	}
	return n.key;
}

// similar of getNearestFeasibleNode, but it will not check if the node can lead to isolation
int Algorithms::getNearestFeasibleNode2(vector<vertex> route)
{
	vertex v = route.back();
	node n;
	n.key = -1;
	int dist = 999999999;
	for (int i = 0; i < inst->nodes.size(); i++) {
		node a = inst->getNodeByKey(v.key);
		node b = inst->nodes.at(i);

		if (a.ogKey != b.ogKey) { // original keys must be different
			vector<int> f = checkFeasibility(route, b);
			int distKI = inst->dist(a, b);
			vector<int> t = { 1, 1, 1 };

			if (distKI < dist && coverage.at(i) == false && f == t) {
				dist = distKI;
				n = inst->nodes.at(i);
			}
		}
	}

	return n.key;
}

// this version of getNearestFeasibleNode will not check ifthe vertex will get isolated, its similar to the first verion 
int Algorithms::getNearestFeasibleNode3(vector<vertex> route)
{
	vertex v = route.back();
	node n;
	n.key = -1;
	int dist = 999999999;
	for (int i = 0; i < inst->nodes.size(); i++) {
		node a = inst->getNodeByKey(v.key);
		node b = inst->nodes.at(i);

		if (a.ogKey != b.ogKey) { // original keys must be different			
			if ((a.type == "f" && b.type == "f") || (a.type == "f_d" && b.type == "f") || (a.type == "f" && b.type == "f_d") || (a.type == "f_d" && b.type == "f_d")) { // doesnt allow the vehicle travel from a bss to another bss
				//cout << 13 << endl;
				continue;
			}
			if (a.type == "d" && b.type == "a") {
				//cout << 23 << endl;
				continue;
			}
			if (a.type == "d" && (b.type == "f" || b.type == "f_d") ) {
				//cout << 33 << endl;
				continue;
			}
			if (b.type == "d") {
				//cout << 43 << endl;
				continue;
			}
			if (a.type == "d" && b.type == "d") {
				//cout << 53 << endl;
				continue;
			}
			if (b.type == "a" && b.id_n != inst->getNodeByKey(route.front().key).id_n) {
				//cout << 63 << endl;
				continue;
			}

			int distKI = inst->dist(a, b);
			vector<int> f = checkFeasibility(route, b);
			vector<int> t = { 1, 1, 1 };

			if (distKI < dist && coverage.at(i) == false && f == t) {
				dist = distKI;
				n = inst->nodes.at(i);

			}
		}
	}
	
	return n.key;
}

// get nearest feasible station
int Algorithms::getNearestFeasibleNode4(vector<vertex> route)
{
	vertex v = route.back();
	node n;
	n.key = -1;
	int dist = 999999999;
	for (int i = 0; i < inst->nodes.size(); i++) {
		node a = inst->getNodeByKey(v.key);
		node b = inst->nodes.at(i);

		if (a.ogKey != b.ogKey) { // original keys must be different			
			if ((a.type == "f" && b.type == "f") || (a.type == "f_d" && b.type == "f") || (a.type == "f" && b.type == "f_d") || (a.type == "f_d" && b.type == "f_d")) { // doesnt allow the vehicle travel from a bss to another bss
				//cout << 21 << endl;
				continue;
			}
			if (a.type == "d" && b.type == "a") {
				//cout << 41 << endl;
				continue;
			}
			if (b.type == "d") {
				//cout << 51 << endl;
				continue;
			}
			if (a.type == "d" && b.type == "d") {
				//cout << 61 << endl;
				continue;
			}

			int distKI = inst->dist(a, b);
			vector<int> f = checkFeasibility(route, b);
			vector<int> t = { 1, 1, 1 };

			if (distKI < dist && coverage.at(i) == false && f == t) {
				// check if this node will not be isolated and lead to a infeasible solution
				vector<vertex> route_aux = addVertexToRoute(route, b.key);

				int nextNode = getNearestFeasibleNode2(route_aux);

				if (nextNode != -1) {
					dist = distKI;
					n = inst->nodes.at(i);
				}

				coverage.at(b.key) = false;
			}
		}
	}
	return n.key;
}

int Algorithms::getNearestNode(vector<vertex> route)
{
	vertex v = route.back();
	node n;
	n.key = -1;
	int dist = 999999999;
	for (int i = 0; i < inst->nodes.size(); i++) {
		node a = inst->getNodeByKey(v.key);
		node b = inst->nodes.at(i);

		if (a.ogKey != b.ogKey) { // original keys must be different			
			if ((a.type == "d" && b.type == "f") || (a.type == "d" && b.type == "f_d")) { // doesnt allow a route that begins in the depot and goes directly to a bss
				continue;
			}
			if ((a.type == "f" && b.type == "f") || (a.type == "f_d" && b.type == "f") || (a.type == "f" && b.type == "f_d") || (a.type == "f_d" && b.type == "f_d")) { // doesnt allow the vehicle travel from a bss to another bss
				continue;
			}

			int distKI = inst->dist(a, b);
			if (distKI <= dist && coverage.at(i) == false) {
				dist = distKI;
				n = inst->nodes.at(i);
			}
			
		}
	}
	return n.key;
}

int Algorithms::getNearestStationSite(int key)
{
	int n = 0, lDist = 999999999;
	for (auto i : inst->nodes) {
		if (i.type == "f") { // if the nodes represents a bss stations site
			node k = inst->getNodeByKey(key);
			int distKI = inst->dist(k.ogKey, i.ogKey); // get the distance between 
			if (distKI < lDist) {
				lDist = distKI;
				n = key;
			}
		}
	}
	return n;
}

// get the nearest customer node by the key 'key' not yet covered
int Algorithms::getNearestCustomerNotCovered(int key)
{
	int n = 0, lDist = 999999999;
	for (auto i : inst->nodes) {
		if (i.type == "c") { // if the nodes represents a customer
			node k = inst->getNodeByKey(key);
			int distKI = inst->dist(k.ogKey, i.ogKey); // get the distance between 
			if (distKI < lDist && isCovered(key) == false) {
				lDist = distKI;
				n = key;
			}
		}
	}
	return n;
}

// get the node with the lowest ready time and that is not covered yet and that generates a feasible partial route
int Algorithms::getLowestRTimeNotCovered(vector<vertex> partialRoute)
{
	node n;
	n.serviceTime = 99999999;
	for (node i : inst->nodes) {
		if (i.type == "c" && coverage.at(i.key) == false) { // check if node is a customer node and is not yet covered
			if (i.readyTime < n.serviceTime) { // check if current node has smaller ready time
				vector<int> t = { 1, 1, 1 };
				if (checkFeasibility(partialRoute, i) == t) {
					n = i;
				}				
			}
		}
	}
	return n.key;
}

// check if adding a node to a route cause infeasibility
vector<int> Algorithms::checkFeasibility(vector<vertex> route, node c)
{
	vector<int> ret;

	// calculate route variables
	vertex a = route.back(); node an = inst->getNodeByKey(a.key);
	node bn = c; // inst->getNodeByKey(b.key);
	float distanceAB = inst->dist(an, bn);
	float travelTimeAB = inst->getTD(an, bn);

	// battery level
	if (a.bLevel - distanceAB < 0) {
		ret.push_back(0); // if infeasible by battery level criteria, 0
	}
	else {
		ret.push_back(1);
	}
	// vehicle load
	if (a.vLoad - bn.demand < 0) {
		ret.push_back(0); // if infeasible by vehicle load criteria, 0
	}
	else {
		ret.push_back(1);
	}
	// arrival time
	//if (c.key == 12) {
		//cout << a.lTime << " " << travelTimeAB << " " << bn.serviceTime << " " << bn.dueDate << endl;
	//}
	if (a.lTime + travelTimeAB + bn.serviceTime > bn.dueDate ){//|| a.lTime + travelTimeAB < bn.readyTime) {
		ret.push_back(0); // if infeasible by time window criteria, 0
	}
	else {
		ret.push_back(1);
	}

	return ret;
}

vector<vertex> Algorithms::arrangeRoute(vector<vertex> route, int key)
{
	// add new vertex in the route
	vertex v;
	v.key = key;
	route.push_back(v);

	// arrange route
	if (route.size() > 1) {
		for (int i = 1; i < route.size(); i++) {
			vertex a = route.at(i - 1); node an = inst->getNodeByKey(a.key);
			vertex b = route.at(i);     node bn = inst->getNodeByKey(b.key);
			float distanceAB = inst->dist(an, bn);
			float travelTimeAB = inst->getTD(an, bn);

			// battery level
			route.at(i).bLevel = a.bLevel - distanceAB;

			// vehicle load
			route.at(i).vLoad = a.vLoad - bn.demand;

			// arrival time
			route.at(i).aTime = a.aTime + travelTimeAB;

		}
	}

	return route;
}

vector<vertex> Algorithms::addVertexToRoute(vector<vertex> route, int key)
{
	vertex a = route.back(); node an = inst->getNodeByKey(a.key);
	vertex b; b.key = key;   node bn = inst->getNodeByKey(b.key);
	float distanceAB = inst->dist(an, bn);
	float travelTimeAB = inst->getTD(an, bn);

	// if previous vertex is a depot we must calculate the left time from it
	if (an.type == "d") {
		route.back().lTime = bn.readyTime - travelTimeAB + 1;
		if (route.back().lTime < 0) {
			route.back().lTime = 0;
		}
		a = route.back();
	}
	
	if (bn.type == "c" || bn.type == "a") {
		b.bLevel = a.bLevel - distanceAB; // battery level
		b.vLoad = a.vLoad - bn.demand; // vehicle load
		b.aTime = a.lTime + travelTimeAB; // arrival time
		b.lTime = b.aTime + bn.serviceTime; // left time

		// need to check if vehicle arrives earlier then the service time and calculate the wait time
		if (bn.type == "c" && b.aTime < bn.readyTime) {
			b.wTime = bn.readyTime - b.aTime;
			b.lTime += b.wTime; // sum the wait time in the exit time

			// vehicle will always recharge during service time
			// recharge during service time
			int amountRecharged = inst->getNodeByKey(b.key).serviceTime * inst->g; // service time * recharging rate
			b.recharged = amountRecharged; //amountRecharged;
			b.bLevel = amountRecharged;//route.back().bLevel + amountRecharged; // recharge to full capacity
			b.recharge = true;
		}

		route.push_back(b); // add vertex to route
	}
	else if (bn.type == "f") {
		b.bLevel = inst->Q; // battery level
		b.vLoad = a.vLoad - bn.demand; // vehicle load
		b.aTime = a.lTime + travelTimeAB; // arrival time
		b.lTime = b.aTime + inst->ct; // left time

		route.push_back(b); // add vertex to route
	}

	// set the caverage vector
	if (bn.type != "a" || bn.type != "f") {
		coverage.at(b.key) = true;
	}
	if (bn.type == "a") {
		coverage.at(b.key) = false;
	}
	if (bn.type == "f") {
		coverage.at(b.key) = false;
	}

	return route;
}

vector<vertex> Algorithms::addInfVertexToRoute(vector<vertex> route, int key, vector<int> f)
{
	vertex prev = route.back(); node prev_n = inst->getNodeByKey(prev.key);
	vertex next; next.key = key;   node next_n = inst->getNodeByKey(next.key);
	float distanceAB = inst->dist(prev_n, next_n);
	float travelTimeAB = inst->getTD(prev_n, next_n);

	vector<int> new_f = checkFeasibility(route, next_n);

	route = addVertexToRoute(route, key);

	//coverage.at(key) = true;
	// set the caverage vector
	if (next_n.type != "a" || next_n.type != "f") {
		coverage.at(next_n.key) = true;
	}
	if (next_n.type == "a") {
		coverage.at(next_n.key) = false;
	}
	if (next_n.type == "f") {
		coverage.at(next_n.key) = false;
	}

	

	// recharge during service time
	int amountRecharged = next_n.serviceTime * inst->g; // service time * recharging rate

	route.back().recharged = inst->Q - route.back().bLevel; //amountRecharged;
	route.back().recharged = amountRecharged;

	route.back().bLevel = inst->Q;//route.back().bLevel + amountRecharged; // recharge to full capacity
	route.back().bLevel = route.back().bLevel + amountRecharged; // recharge to full capacity

	route.back().lTime += route.back().recharged / inst->g;
	route.back().recharge = true;

	
	return route;
}

string Algorithms::getRow()
{
	string res = "";
	res += inst->fileName + " ; ";
	res += inst->solution.FO + " ; ";
	for (auto i : fo_parcels) {
		res += i;
		res += " ";
	}
	return res;
}

int Algorithms::test()
{
	greed();
	localSearch(sol, 1);
	return 0;
}

int Algorithms::localSearch(vector<vector<vertex>> sol, int stopPolice)
{
	// get which nodes has a bss facility sited
	vector<bool> facilities;
	facilities.resize(inst->nodes.size(), false);
	for (auto r : sol) {
		for (auto v : r) {
			node n = inst->getNodeByKey(v.key);
			if (n.type == "f") {
				facilities.at(n.ogKey) = true;
			}
		}
	}




	return 0;
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
		cout << "Battery level in node " << nb.key << ": " << r.at(i + 1).bLevel << endl;
		cout << "Battery is recharged in " << r.at(i + 1).recharged << endl;
		cout << "Recharge " << r.at(i + 1).recharge << endl;
		cout << "Vahicle load in node " << nb.key << ": " << r.at(i + 1).vLoad << endl;
		
		cout << endl;
	}

}

Algorithms::Algorithms()
{
}

int Algorithms::loadInstance(string dir, string fileName, int type)
{
	inst = new instance(dir, fileName, type);

	return 0;
}

int Algorithms::greed()
{
	cout << "greed\n\n";
	//vector<bool> covered; // this vector will store which nodes are already covered 
	coverage.resize(inst->nodes.size(), false);
	vector<bool> bu;
	bool retry = false;

	while (allCovered() == false) { // while not all vertex are covered
		vector<vertex> route;
		vertex v;
		int nextNodeKey;

		// choosing depots locations
		// criteria: the depot node with the minimum sum of all distances to client nodes not coverd yet
		v.key = depotHeuristc();
		v.bLevel = inst->Q;
		v.vLoad = inst->C;	
		v.lTime = 0;
		v.aTime = 0;
		route.push_back(v);
		coverage.at(v.key) = true; // now node v.key appears in the route

		cout << "node " << v.key << " added in the route as a depot\n";

		// add nodes to the route until it ends with an "a" node
		while (true) {
			cout << route.back().key << endl;

			nextNodeKey = getNearestFeasibleNode(route); //chooseNextNode(route);		

			vector<int> feasibility = checkFeasibility(route, inst->getNodeByKey(nextNodeKey));
			node n = inst->getNodeByKey(nextNodeKey);

			//cout << "nextNodeKey: " << nextNodeKey << endl;


			// if the node is isolated, we have to try to fix the solution
			if (nextNodeKey == -1) {

				int otherNode = getNearestFeasibleNode3(route);

				if (otherNode != -1) {
					n = inst->getNodeByKey(otherNode);
					route = addInfVertexToRoute(route, otherNode, feasibility);

				}
				else {
					// if the depot is isolated
					if (inst->getNodeByKey(route.back().key).type == "d") {
						int yetAnotherNode = getNearestFeasibleNode4(route);
						route = addVertexToRoute(route, yetAnotherNode);
					}
					else {
						cout << "It was not possible to find a viable node near " << route.back().key << endl;


						printPartialRoute(route);

						cout << "nextNodeKey: " << nextNodeKey << endl;
						cout << "otherNode  : " << otherNode << endl;
						cout << "route.back().key: " << route.back().key << endl;

						for (int i = 0; i < coverage.size(); i++) {
							cout << i << " - " << coverage.at(i) << endl;
						}

						for (int i = 0; i < inst->nodes.size(); i++) {
							cout << i << " = " << fixed << inst->dist(route.back().key, inst->nodes.at(i).key) << endl;
						}
						cout << endl;
						exit(1);
					}			
				}
			}
			else {
				vector<int> t = { 1, 1, 1 };
				vector<int> t2 = { 0, 1, 1 };

				vector<int> f = { 0, 0, 0 };

				if (feasibility == t) { // if node is feasible
					//cout << nextNodeKey << " is feasible\n";
					route = addVertexToRoute(route, nextNodeKey);

				}
				else {
					// if node is infeasible due to battery lvls, it is still possible to add the node if we modify the previous node in the route
					//cout << nextNodeKey << " is infeasible\n";
					route = addInfVertexToRoute(route, nextNodeKey, feasibility);
				}
			}

			// check if the route start and end in the same depot
			if ((inst->getNodeByKey(route.front().key).id_n == inst->getNodeByKey(route.back().key).id_n || inst->getNodeByKey(route.front().key).ogKey == inst->getNodeByKey(route.back().key).ogKey) && route.size() >= 2) { // if the node choosen is the arrival node of the initial depot node, the route is finished
				sol.push_back(route);
				//printPartialRoute(route);
				
				break;
			}

		}
	}

	int ev = eval(sol);
	if (ev != 0) {
		cout << "Solution found is not feasible.\n";
		cout << ev << endl;
	}

	inst->solution.FO = FO(sol);

	row = getRow();

	cout << row << endl;

	//printSol();

	return 0;
}

int Algorithms::FO(vector<vector<vertex>> sol)
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
	int depotCost = 0;

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
	int bssCost = 0;

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
	int brsCost = 0;

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
	int drivingCost = 0;

	for (auto route : sol) {
		for (int i = 0; i < route.size() - 1; i++) {
			drivingCost += inst->dist(route.at(i).key, route.at(i + 1).key) * inst->driverWage;
		}
	}

	// vehicle fixed cost
	int vehicleCost = sol.size() * inst->vehicleCost;

	// energy cost
	int energyCost = 0;

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
	int operationalCost = 0;

	for (auto route : sol) {
		for (auto v : route) {
			if (inst->getNodeByKey(v.key).type == "f" || inst->getNodeByKey(v.key).type == "f_d") {
				operationalCost += 1;
			}
		}
	}
	operationalCost *= inst->bssUseCost;

	fo = depotCost + bssCost + brsCost + drivingCost + vehicleCost + energyCost + operationalCost;

	fo_parcels = { depotCost, bssCost, brsCost, drivingCost, vehicleCost, energyCost, operationalCost };

	return fo;
}

int Algorithms::eval(vector<vector<vertex>> sol)
{
	for (auto route : sol) {
		for (auto v : route) {
			node n = inst->getNodeByKey(v.key);

			// checking time windows
			if (n.type == "c") {
				if (v.aTime + v.wTime > n.dueDate + n.serviceTime || v.aTime + v.wTime < n.readyTime) {
					cout << "Vertex " << v.key << " arrival is " << v.aTime << " and its ready time and due date is " << n.readyTime << " and " << n.dueDate << endl;
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
	return 0;
}


/*

vertex a = route.back();
node an = inst->getNodeByKey(a.key);

vertex b;
b.key = nextNodeKey;
node bn = inst->getNodeByKey(b.key);

float distanceAB = inst->dist(an, bn);
float travelTimeAB = inst->getTD(an, bn);

// if the vertex is infeasible on because of battery level, arrange previous node
if (feasibility.at(0) == 0 && feasibility.at(1) == 1 && feasibility.at(2) == 1) {
	if (an.type == "c") { // if previous node is a customer, recharge the battery
		route.back().recharge = true;
		route.back().recharged = inst->Q - route.back().bLevel;
		route.back().bLevel = inst->Q; // recharge to full capacity

		route = addVertexToRoute(route, nextNodeKey);
	}
}
else {

}

if (bn.type == "c" || bn.type == "a") {
	// battery level
	b.recharged = inst->Q - b.bLevel; // store the amount of battery recharged
	b.bLevel = inst->Q; // recharge the battery to full capacity
	b.recharge = true; // indicates that a battery was recharged in the customer

	// vehicle load
	b.vLoad = a.vLoad - bn.demand;

	// arrival time
	b.aTime = a.aTime + travelTimeAB;

	// add vertex to route
	route.push_back(b);
}
else if (bn.type == "r") {
	// battery level
	b.bLevel = inst->Q;

	// vehicle load
	b.vLoad = a.vLoad - bn.demand; // the demand on stations node is zero

	// arrival time
	b.aTime = a.aTime + travelTimeAB;

	// add vertex to route
	route.push_back(b);
}
// else if (bn.type == "a") {


// if the predecessor is a bss, add the operation time of changing the battery
if (an.type == "r") {
	b.aTime += inst->ct;
}
// if battery was recharged in the previous customer node, add the recharging time in the arrival time
if (an.type == "c" && a.recharge == true) {
	//b.aTime ==
}


*/

void Algorithms::printInstance()
{
	inst->print(cout);
}

void Algorithms::printSol()
{
	for (auto i : sol) {
		for (auto j : i) {
			cout << j.key << " ";
		}
		cout << endl;
	}
}

void Algorithms::getSol(ostream & strm)
{
	strm << "Greed algorithm\n";
	strm << inst->fileName << endl;
	strm << inst->dir << endl;

	strm << "FO: " << inst->solution.FO << endl;
	for (auto i : fo_parcels) {
		strm << i << " ";
	}
	strm << endl;

	strm << "Stations\n";

	vector<bool> y;
	y.resize(inst->nodes.size(), false);

	for (auto r : sol) {
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

	for (auto r : sol) {
		for (auto v : r) {
			strm << v.key << " ";
		}
		strm << endl;
	}

	strm << "Detailed routes\n";

	for (auto r : sol) {
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

Algorithms::~Algorithms()
{

}
