#include "routes_rep.h"

// check if all customers are covered
bool routes_rep::allCovered()
{
	for (int i = 0; i < inst->nodes.size(); i++) {
		if (coverage.at(i) == false && inst->nodes.at(i).type == "c") {
			return false;
		}
	}
	return true;
}

// choose a depot node to be the route begining
int routes_rep::depotHeuristc()
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

int routes_rep::depotHeuristic(vector<bool> coverage)
{
	// get the distances sum from the arcs leaving each depot for clients nodes not yet covered
	int d = -1, d_sum = 99999999; // will hold the depot index with the lowest ditance sum 
	vector<int> sums;
	sums.resize(inst->nodes.size(), 0);
	for (node i : inst->nodes) {
		if (i.type == "d") { // if node i is a depot node
			for (node j : inst->nodes) {
				if (j.type == "c" && coverage.at(j.key) == false) {
					sums.at(i.key) += inst->dist(i.ogKey, j.ogKey); // sum the distances of all arcs leaving the current depot
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
bool routes_rep::isCovered(int key)
{
	return coverage.at(key);
}

// choose the next node to be added in a route
// the node must have the lowest ready time. also we check if is possible to reach a station node with the battery level left when the vehicle arrives at the node.
int routes_rep::chooseNextNode(vector<vertex> partialRoute)
{
	return getNearestFeasibleNode(partialRoute); // get the nearest customer node;
}

// v is the las vertex in the partial route
int routes_rep::getNearestFeasibleNode(vector<vertex> &route)
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
				if (inst->getNodeByKey(route.at(0).key).type == "d" && inst->getNodeByKey(route.at(1).key).type == "f")
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
int routes_rep::getNearestFeasibleNode2(vector<vertex> route)
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
int routes_rep::getNearestFeasibleNode3(vector<vertex> route)
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
			if (a.type == "d" && (b.type == "f" || b.type == "f_d")) {
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
int routes_rep::getNearestFeasibleNode4(vector<vertex> route)
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

int routes_rep::getNearestNode(vector<vertex> route)
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

int routes_rep::getNearestStationSite(int key)
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
int routes_rep::getNearestCustomerNotCovered(int key)
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
int routes_rep::getLowestRTimeNotCovered(vector<vertex> partialRoute)
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
vector<int> routes_rep::checkFeasibility(vector<vertex> route, node c)
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
	if (a.lTime + travelTimeAB + bn.serviceTime > bn.dueDate) {//|| a.lTime + travelTimeAB < bn.readyTime) {
		ret.push_back(0); // if infeasible by time window criteria, 0
	}
	else {
		ret.push_back(1);
	}

	return ret;
}

vector<vertex> routes_rep::arrangeRoute(vector<vertex> route, int key)
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

vector<vertex> routes_rep::addVertexToRoute(vector<vertex> route, int key)
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

vector<vertex> routes_rep::addInfVertexToRoute(vector<vertex> route, int key, vector<int> f)
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

int routes_rep::getLowestDemand(vector<bool> c)
{
	int l = inst->nodes.front().demand;
	for (node n : inst->nodes) {
		if (n.demand < l) {
			l = n.demand;
		}
	}
	return l;
}

bool routes_rep::checkCustomersCoverage(vector<bool> coverage)
{
	for (int i = 0; i < coverage.size(); i++) {
		if (inst->nodes.at(i).type == "c" && coverage.at(i) == false) {
			return false;
		}
	}
	return true;
}

int routes_rep::chooseNextNode(route r, vector<bool> coverage) {
	route rl = r;

	for (node n : inst->nodes) {
		vertex v;
		v.key = n.key;

		//rl.push_back()
	}
	return 0;

}

Solution routes_rep::localSearch_2opt(Solution sol)
{
	Solution best = sol;
	Solution curr = sol;

	bool improv = true;
	int i = 0;
	while (improv == true) {
		improv = false;
		for (int i = 0; i < sol.routes.size(); i++) { // iterate over routes
			if (sol.routes.at(i).size() >= 4) { // safety comparison
				// search in the entire 2 opt neighborhood
				for (int a = 1; a < sol.routes.at(i).size() - 2; a++) { // begin in the the second vertex
					for (int b = a + 1; b < sol.routes.at(i).size() - 1; b++) {

						curr = opt2(sol, i, a, b);
						curr = procSol(curr);

						if (curr.FO < best.FO) {
							best = curr;
							improv = true;
						}

					}
				}
			}

		}
	}

	best = procSol(best);
	return best;
}

Solution routes_rep::localSearch_2swap(Solution sol)
{

	Solution best = sol;
	Solution curr = sol;

	bool improv = true;
	int i = 0;
	while (improv == true) {
		improv = false;
		curr = best;

		for (int r = 0; r < sol.routes.size(); r++) {
			if (sol.routes.at(r).size() >= 4) {
				// search in the entire 2 opt neighborhood
				for (int a = 1; a < sol.routes.at(i).size() - 2; a++) { // begin in the the second vertex
					for (int b = a + 1; b < sol.routes.at(i).size() - 1; b++) {

						curr = swap2(sol, r, a, b);
						curr = procSol(curr);

						if (curr.FO < best.FO) {
							best = curr;
							improv = true;
						}

					}
				}
			}

		}

	}

	best = procSol(best);
	return best;
}


Solution routes_rep::localSearch_iRouteSwap(Solution sol)
{
	Solution best = sol;

	bool improv = true;
	int i = 0;
	while (improv == true) {
		improv = false;

		if (sol.routes.size() >= 2) {
			// iterate over all routes combinations
			for (int i = 0; i < sol.routes.size() - 1; i++) {
				for (int j = i + 1; i < sol.routes.size(); i++) {
					if (i != j) {
						int r1End = sol.routes.at(i).size();
						int r2End = sol.routes.at(j).size();

						// iterate over all combinations of subroutes to swap
						for (int a = 1; a < r1End - 1; a++) {
							for (int b = 1; b < r2End - 1; b++) {

								Solution s = iRouteSwap2(sol, i, j, a, b);
								s = procSol(s);

								if (s.FO < best.FO) {
									best = s;
									improv = true;
								}
							}
						}
					}
				}
			}
		}
		sol = best;
	}

	best = procSol(best);
	return best;
}

Solution routes_rep::VNS(Solution init)
{
	srand(time(nullptr));

	Solution x = init;

	int itMax = 20;
	int kMax = 3;
	int tMax = 300;
	int its = 0;
	int k = 1;

	bool timeout = false;

	auto t1 = std::chrono::high_resolution_clock::now();

	vector<string> n = { "2opt", "2swap", "iRouteSwap2" };

	while (its < itMax || timeout == true) {
		k = 1;
		while (k <= kMax) {

			Solution xl = shake(x, n.at(k - 1)); // shaking
			xl = localSearch_iRouteSwap(xl); // local search

			// search in the neighborhood
			try {
				if (k == 1) { // search on first neighborhood
					x = search_2swap(xl, x);
				}
				else if (k == 2) { // search on second neighborhood
					x = search_2opt(xl, x);
				}
				else if (k == 3) { // search on third neighborhood
					x = search_iRouteSwap(xl, x);
				}

				//x = nChange(xl, x, k);
				k = 1;
				its = 0;
			}
			catch (NoImprovementExcp &e) {
				x = xl;
				k++;
				its++;
			}
		}
		its++;

		auto t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
		if (duration > tMax) {
			timeout = true;
		}

	}

	return x;
}


// shake function receives a solution and perform a shake with a given n neighboorhood structure
// the function can generate an infeasible solution
Solution routes_rep::shake(Solution s, string n)
{
	if (n == "2swap") {

		int route = rand() % s.routes.size();
		int beg = (rand() % (s.routes.at(route).size() - 3)) + 1;

		int end = (rand() % s.routes.at(route).size());
		while (end <= beg || end == s.routes.at(route).size()) {
			end = (rand() % s.routes.at(route).size());
		}
		Solution new_s = swap2(s, route, beg, end);
		new_s = procSol(new_s);

		return new_s;
	}
	else if (n == "2opt") {
		Solution new_s;

		int r = rand() % s.routes.size();
		int beg = (rand() % ((s.routes.at(r).size() / 2) - 1)) + 1;
		int end = (rand() % ((s.routes.at(r).size() / 2) - 1)) + (s.routes.at(r).size() / 2);

		new_s = opt2(s, r, beg, end);
		new_s.routes.at(r) = fixRoute(new_s.routes.at(r));

		new_s = procSol(new_s);

		return new_s;
	}
	else if (n == "iRouteSwap2") {

		int route1 = rand() % s.routes.size();
		int route2 = rand() % s.routes.size();
		while (route1 == route2) {
			cout << 1 << endl;
			route2 = rand() % s.routes.size();
		}

		int beg = (rand() % (s.routes.at(route1).size() - 2)) + 1;
		int end = (rand() % (s.routes.at(route2).size() - 2)) + 1;

		Solution new_s = iRouteSwap2(s, route1, route2, beg, end);
		new_s = procSol(new_s);

		return new_s;
	}
	else {
	}
}

Solution routes_rep::nChange(Solution s, Solution best, int &k)
{
	try {
		if (k == 1) { // search on first neighborhood
			s = search_2opt(s, best);
		}
		else if (k == 2) { // search on second neighborhood
			s = search_2swap(s, best);
		}
		else if (k == 3) { // search on third neighborhood
			s = search_iRouteSwap(s, best);
		}
	}
	catch (NoImprovementExcp &e) {
		throw e;
	}

	return s;
}

Solution routes_rep::search_2opt(Solution sol, Solution best)
{
	bool improv = false;
	Solution curr;
	//solution best = curr;

	int r = curr.routes.size();
	for (int i = 0; i < curr.routes.size(); i++) { // for each route in the solution
		if (curr.routes.at(i).size() >= 4) { // only search if route has more then 3 vertex
			for (int a = 1; a < curr.routes.at(i).size() - 2; a++) { // begin in the the second vertex
				for (int b = a + 1; b < curr.routes.at(i).size() - 1; b++) {

					curr = opt2(curr, i, a, b);
					curr = procSol(curr);

					if (curr.FO < best.FO) {
						best = curr;
						improv = true;
					}
				}
			}
		}
	}

	if (improv == false) {
		throw NoImprovementExcp();
	}

	return best;
}

Solution routes_rep::search_2swap(Solution sol, Solution best)
{
	bool improv = false;
	Solution curr;
	//solution best;

	int r = curr.routes.size();
	for (int i = 0; i < curr.routes.size(); i++) { // for each route in the solution
		if (curr.routes.at(i).size() >= 4) { // only search if route has more then 3 vertex
			for (int a = 1; a < curr.routes.at(i).size() - 2; a++) { // begin in the the second vertex
				for (int b = a + 1; b < curr.routes.at(i).size() - 1; b++) {

					curr = swap2(curr, i, a, b);
					curr = procSol(curr);

					if (curr.FO < best.FO) {
						best = curr;
						improv = true;
					}
				}
			}
		}
	}

	if (improv == false) {
		throw NoImprovementExcp();
	}

	return best;
}

Solution routes_rep::search_iRouteSwap(Solution sol, Solution best)
{
	bool improv = false;
	Solution curr;
	//solution best;

	for (int r1 = 0; r1 < sol.routes.size() - 1; r1++) {
		for (int r2 = r1 + 1; r2 < sol.routes.size(); r2++) {

			for (int a = 1; a < sol.routes.at(r1).size() - 2; a++) { // iterate over the first route
				for (int b = 1; b < sol.routes.at(r2).size() - 2; b++) { // iterate over the second route

					curr = iRouteSwap2(sol, r1, r2, a, b);
					curr = procSol(curr);

					if (curr.FO < best.FO) {
						best = curr;
						improv = true;
					}
				}
			}
		}
	}

	if (improv == false) {
		throw NoImprovementExcp();
	}

	return best;
}

Solution routes_rep::opt2(Solution sol, int route, int beg, int end)
{
	if (beg > end) {
		throw MovementFailed();
	}

	Solution newSol;
	if (route > sol.routes.size()) {
		return newSol;
	}

	newSol = sol;

	for (int i = beg, j = end; i < j; i++, j--) {
		vertex v = newSol.routes.at(route).at(i);
		newSol.routes.at(route).at(i) = newSol.routes.at(route).at(j);
		newSol.routes.at(route).at(j) = v;
	}

	newSol.routes.at(route) = fixRoute(newSol.routes.at(route));

	return newSol;
}

Solution routes_rep::swap2(Solution sol, int r, int beg, int end)
{
	Solution newSol;
	if (r > sol.routes.size()) {
		throw MovementFailed();
	}

	// movement
	newSol = sol;
	vertex v = newSol.routes.at(r).at(beg);
	newSol.routes.at(r).at(beg) = newSol.routes.at(r).at(end);
	newSol.routes.at(r).at(end) = v;


	newSol.routes.at(r) = fixRoute(newSol.routes.at(r)); // fix route after movement

	return newSol;
}

// swap intra route
Solution routes_rep::iRouteSwap2(Solution sol, int route1, int route2, int p1, int p2)
{
	if (p1 > sol.routes.at(route1).size()) {
		throw MovementFailed();
	}
	if (p2 > sol.routes.at(route2).size()) {
		throw MovementFailed();
	}

	int beg1 = p1;
	int end1 = sol.routes.at(route1).size();
	int beg2 = p2;
	int end2 = sol.routes.at(route2).size();

	route r1; // = sol.at(route1);
	route r2; // = sol.at(route2);

	// copy inital part of the routes
	for (int i = 0; i < beg1; i++) {
		r1.push_back(sol.routes.at(route1).at(i));
	}
	for (int i = 0; i < beg2; i++) {
		r2.push_back(sol.routes.at(route2).at(i));
	}

	// copy the rest of the route
	for (int i = beg2; i < end2 - 1; i++) {
		r1.push_back(sol.routes.at(route2).at(i));
	}
	for (int i = beg1; i < end1 - 1; i++) {
		r2.push_back(sol.routes.at(route1).at(i));
	}

	// copy the last node
	r1.push_back(sol.routes.at(route1).back());
	r2.push_back(sol.routes.at(route2).back());

	// fix routes
	r1 = fixRoute(r1);
	r2 = fixRoute(r2);

	sol.routes.at(route1) = r1;
	sol.routes.at(route2) = r2;

	return sol;
}

Solution routes_rep::greed()
{
	vector<vector<vertex>> sol;

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

		// add nodes to the route until it ends with an "a" node
		while (true) {
			//cout << route.back().key << endl;

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
						/*
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
						*/
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
		for (auto r : sol) {
			for (auto i : r) {
				cout << i.key << " ";
			}
			cout << endl;
		}
		cout << ev << endl;
	}
	int fo = FO(sol);
	

	Solution s;
	s.routes = sol;
	s = procSol(s);
	row = getRow(s);

	return s;
}



Solution routes_rep::greed2()
{
	vector<bool> coverage(false, inst->nodes.size());

	while (checkCustomersCoverage(coverage) == false) {

		// start a new route
		route r;

		// choose the depot
		vertex v;
		v.key = depotHeuristc();
		v.vLoad = inst->C;
		r.push_back(v);

		int lowestDemand = getLowestDemand(coverage);

		while (r.back().vLoad > lowestDemand) { // while the vehicle can theorically attend a customer

		}
	}
	return Solution();
}


int routes_rep::test()
{
	cout << "Greed\n";
	Solution s1 = greed();
	s1 = procSol(s1);

	cout << "FO: " << s1.FO << endl;
	for (auto i : s1.inf) {
		cout << i << " ";
	}
	cout << endl;
	for (auto r : s1.routes) {
		for (auto v : r) {
			cout << v.key << " ";
		}
		cout << endl;
	}

	Solution s2 = createOptimialSolution1();
	getSol(s2, cout);

	/*
	cout << "VNS\n";
	solution s2 = VNS(s1);
	s2 = procSol(s2);

	cout << "FO: " << s2.FO << endl;
	for (auto r : s2.routes) {
		for (auto v : r) {
			cout << v.key << " ";
		}
		cout << endl;
	}
	*/


	return 0;
}
