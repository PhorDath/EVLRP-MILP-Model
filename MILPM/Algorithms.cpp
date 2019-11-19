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
	int nextNodeKey = getNearestFeasibleNode(partialRoute); // get the nearest customer node

	return nextNodeKey;
}

// v is the las vertex in the partial route
int Algorithms::getNearestFeasibleNode(vector<vertex> route)
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
			vector<int> t = { 1, 1, 1 };
			if (distKI <= dist && coverage.at(i) == false && checkFeasibility(route, b) == t) {
				dist = distKI;
				n = inst->nodes.at(i);
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
	node bn = c;// inst->getNodeByKey(b.key);
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
	if (a.lTime + travelTimeAB + bn.serviceTime > bn.dueDate) {
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
		route.back().lTime = bn.readyTime - travelTimeAB;
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

		route.push_back(b); // add vertex to route
	}
	else if (bn.type == "f") {
		b.bLevel = inst->Q; // battery level
		b.vLoad = a.vLoad - bn.demand; // vehicle load
		b.aTime = a.lTime + travelTimeAB; // arrival time
		b.lTime = b.aTime + inst->ct; // left time

		route.push_back(b); // add vertex to route
	}

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

	// if the vertex is infeasible only because of battery level, arrange previous node
	if (f.at(0) == 0 && f.at(1) == 1 && f.at(2) == 1) {
		//if (prev_n.type == "c") { // if previous node is a customer, recharge the battery
			route.back().recharge = true;

			// recharge during service time
			int amountRecharged = prev_n.serviceTime * inst->r; // service time * recharging rate

			route.back().recharged = route.back().bLevel + amountRecharged;
			route.back().bLevel = inst->Q; // recharge to full capacity
		//}
		
		vector<int> new_f = checkFeasibility(route, next_n);

		// if the new node generates a feasible solution with the arrenged route
		if (f.at(0) == 1 && f.at(1) == 1 && f.at(2) == 1) {
			route = addVertexToRoute(route, key);
		}
	}

	return route;
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
		route.push_back(v);
		coverage.at(v.key) = true; // now node v.key appears in the route

		cout << "node " << v.key << " added in the route as a depot\n";

		// add nodes to the route until it ends with a "a" node
		while (true) {
			nextNodeKey = chooseNextNode(route);

			vector<int> feasibility = checkFeasibility(route, inst->getNodeByKey(nextNodeKey));
			node n = inst->getNodeByKey(nextNodeKey);

			if (n.key == -1) {
				cout << "It is not possible to finish the current route\n";

			}

			vector<int> t = { 1, 1, 1 };
			vector<int> t2 = { 0, 1, 1 };

			vector<int> f = { 0, 0, 0 };

			cout << nextNodeKey << " is the next node \n";


			if (feasibility == t) { // if node is feasible
				route = addVertexToRoute(route, nextNodeKey);
				if (retry == true) {
					retry = false;
					coverage = bu;
					coverage.at(nextNodeKey) = true;
				}

				cout << nextNodeKey << " is feasible\n";
			}
			else if(feasibility == t2){ 
				// if node is infeasible due to battery lvls, it is still possible to add the node if we modify the previous node in the route
				route = addInfVertexToRoute(route, nextNodeKey, feasibility);
				if (retry == true) {
					retry = false;
					coverage = bu;
					coverage.at(nextNodeKey) = true;
				}

				cout << nextNodeKey << " is infeasible\n";
				
			}
			else { // if node is infeasible due to time windows or vehicle load criteria
				bu = coverage; // safe a copy of the coverage vector
				coverage.at(nextNodeKey) = true; // mark the node as added in the solution without adding it
				retry = true; // indicates that we will retry to add a node because this one is infeasible

			}

			// check if the route start and end in the same depot
			if (inst->getNodeByKey(route.front().key).id_n == inst->getNodeByKey(route.back().key).id_n && route.size() >= 2) { // if the node choosen is the arrival node of the initial depot node, the route is finished
				sol.push_back(route);
				printPartialRoute(route);
				break;			
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

Algorithms::~Algorithms()
{
}
