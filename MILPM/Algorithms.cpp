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

solution Algorithms::permutationToSolution(permutation p)
{
	vector<bool> coverage(inst->nodes.size(), false);

	solution s;

	// adding the first vertex to the first route
	int v = p.front(); // get the first element in the permutation
	
	vertex new_v; // create the vertex to be added
	new_v.key = v;

	s.routes.resize(1); // set the number of route to one
	s.routes.front().push_back(new_v);

	p.erase(p.begin()); // remove the first element that were already added

	vector<int> weight; // this vector will store the amount of freight required to in the route
	weight.push_back(inst->getNodeByKey(v).demand); 

	//
	for(auto i : p) { // iterate sequentially over the permutation adding the customers to a route
		node n = inst->getNodeByKey(i);
		new_v.key = n.key;

		// search which route can receive the next permutation node
		int j = 0;
		for (j = 0; j < weight.size(); j++) {
			if (n.demand + weight.at(j) <= inst->C) { // we put the node in the first route that fits it
				s.routes.at(j).push_back(new_v); // insert node
				weight.at(j) += n.demand; // update the total freight weight in the route
				break;
			}
		}
		if (j == weight.size()) { // if no route can fit the next node
			vector<vertex> temp;
			temp.push_back(new_v); // create the new route with the first node
			s.routes.push_back(temp); // add the new route to solution
			weight.push_back(n.demand); // add the route's weight in the weight vector
		}
	}

	s = addDepots(s);
	s = addStations(s);

	return s;
}

solution Algorithms::addDepots(solution s)
{
	//vector<vector<int>> dm; // matrix dm (distance matrix) where dm[i][j] (i = depot and j = route)
	// this matrix will store for each route x depot combination the sum of the distances depot -> initial node and the last node -> depot

	vector<node> UD0 = inst->set_UD0();
	vector<node> UD1 = inst->set_UD1();

	//
	int w = 0;
	for (route r : s.routes) {

		vector<int> m;

		int lowestMean = INT_MAX;
		node n;

		for (int i = 0; i < UD0.size(); i++) {
			
			int d1 = inst->dist(UD0.at(i), inst->getNodeByKey(r.front().key));
			

			int d2 = inst->dist(inst->getNodeByKey(r.back().key), UD1.at(i));

			m.push_back(ceil((d1 + d2) / 2));

			if (m.at(i) < lowestMean) {
				lowestMean = m.at(i);
				n = UD0.at(i);
			}
		}

		// insert the depot
		vertex v;
		v.key = n.key;
		r.insert(r.begin(), v);

		// insert the arrival depot node
		int a;
		for (node n : UD1) {
			if (n.ogKey == v.key) { // search the arrival node
				a = n.key;
				break;
			}
		}
		v.key = a;
		r.push_back(v);


		s.routes.at(w) = r; // add updated route to solution

		w++;
	}


	return s;
}

solution Algorithms::addStations(solution s)
{
	// compute the amount of energy required to perform the routes
	vector<int> sum;

	for (route r : s.routes) {
		sum.push_back(0);
		
		for (int i = 0; i < r.size() - 1; i++) {
			node n1 = inst->getNodeByKey(r.at(i).key);
			node n2 = inst->getNodeByKey(r.at(i + 1).key);

			sum.back() += inst->dist(n1, n2);
		}

	}

	for (int i : sum) {
		cout << i << " - " << i / inst->Q << endl;
	}

	// set all vehicles to full energy
	for (int j = 0; j < s.routes.size(); j++) {
		s.routes.at(j).front().bLevel = inst->Q;
	}

	vector<int> counter(s.routes.size(), 1); // store the maximum node in each route the vehicle can travel with its battery charge
	vector<bool> end(s.routes.size(), false); // record the routes that have already beeing fully traveled
	vector<bool> hold(s.routes.size(), false); // record which routes are waiting to have it vehicle to refuel
	vector<int> energy; // store the amount of energy the vehicle can recharge during service time
	
	// iterate simuntaneously over all routes, computing in which node the vehicle runs out of energy 

	while (true) {
		for (int j = 0; j < s.routes.size(); j++) { // for each route

			energy.resize(s.routes.size(), 0);

			while (true) { // iterate over the route j
				if (counter.at(j) < s.routes.at(j).size()) { // check if it is the routes' end
					int curr = counter.at(j); // current node in route j


					// compute battery level in the current node
					s.routes.at(j).at(curr).bLevel = s.routes.at(j).at(curr - 1).bLevel - inst->dist(s.routes.at(j).at(curr - 1).key, s.routes.at(j).at(curr).key); 

					if (s.routes.at(j).at(curr).bLevel >= 0) { // if battery is enough to travel to next node
						counter.at(j)++; // keep going
					}
					else {
						hold.at(j) = true; // indicates that the vehicle must be recharged
						break; // go to next route
					}
					
				}
				else {
					end.at(j) = true; // indicate the routes' end
					break;
				}
			}
		}

		// check if all routes are on hold, it means that all vehicles are waiting to be recharged
		int recharge = true;
		for (int i = 0; i < s.routes.size(); i++) {
			if (end.at(i) == false && hold.at(i) == false) {
				recharge = false;
			}
		}
		for (route r : s.routes) {
			for (vertex v : r) {
				cout << v.bLevel << " ";
			}
			cout << endl;
		}

		// all vehicles waiting to recharge we must find a good spot to them to do so
		if (recharge == true) {

			for (int j = 0; j < s.routes.size(); j++) {
				if (hold.at(j) == true) { // if vehicle is in need of energy
					// try to insert each bss in between the hold position and the previous one

					int a = s.routes.at(j).at(counter.at(j) - 1).key; // previous position key
					int b = s.routes.at(j).at(counter.at(j)).key; // hold position key

					int minDist = INT_MAX;
					int minMean = INT_MAX;

					vector<node> R = inst->set_R(); // get all stations
					int n;
					for (node r : R) {

						int AR = inst->getBatteryUsed(a, r.key); // inst->dist(a, r.key);
						int RB = inst->getBatteryUsed(r.key, b); // inst->dist(r.key, b);
						int meanAB = (AR + RB) / 2;

						// get the lowest mean
						if (meanAB < minMean) {
							minMean = meanAB;
						}
						// get the lowest dist
						if (AR < minDist) {
							n = r.key;
							minDist = AR;
						}

						cout << "distance from " << a << " to " << r.key << " is " << AR << endl;
						cout << "distance from " << r.key << " to " << b << " is " << RB << endl;

						
					}
					cout << "aqui\n";
					cout << minDist << endl;

					// if vehicle can reach the closest bss it will move there in order to recharge
					if (s.routes.at(j).at(counter.at(j)).bLevel >= minDist) {
						cout << "Inserting BSS " << n << " in solution\n";

						vertex v;
						v.key = n;
						s.routes.at(j).insert(s.routes.at(j).begin() + counter.at(j) - 1, v);
					}
					if()


					cout << "aqui2\n";
					exit(1);

				}
			}


		}
		exit(1);

		// check if all vehicles have reached the routes' end
		int all = false;
		for (bool e : end) {
			if (e == true) {
				all = true;
			}
		}
		// stop when all routes were explored until the end
		if (all == false) {
			break;
		}

	}

	



	return s;
}

permutation Algorithms::randomPermutation()
{
	permutation p;

	vector<node> C = inst->set_C(); // get the set of all customers

	//seed
	srand(time(NULL));

	while (C.size() > 0) {
		int pos = rand() % C.size(); // draws a customer to be added in the permutation

		p.push_back(C.at(pos).key); // add the customer node in the permutation

		// erase customer from vector C
		//C.erase(remove(C.begin(), C.end(), C.at(pos)), C.end()); // remove erase idiom
		C.erase(C.begin() + pos);

	}

	return p;
}

int Algorithms::availableRoute(solution s, int n)
{

	return 0;
}

solution Algorithms::generateSolution(permutation p)
{
	solution s;
	return s;
}

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

int Algorithms::depotHeuristic(vector<bool> coverage)
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

int Algorithms::getLowestDemand(vector<bool> c)
{
	int l = inst->nodes.front().demand;
	for (node n : inst->nodes) {
		if (n.demand < l) {
			l = n.demand;
		}
	}
	return l;
}

bool Algorithms::checkCustomersCoverage(vector<bool> coverage)
{
	for (int i = 0; i < coverage.size(); i++) {
		if (inst->nodes.at(i).type == "c" && coverage.at(i) == false) {
			return false;
		}
	}
	return true;
}

int Algorithms::chooseNextNode(route r, vector<bool> coverage) {
	route rl = r;

	for (node n : inst->nodes) {
		vertex v;
		v.key = n.key;

		//rl.push_back()
	}
	return 0;
	
}

solution Algorithms::localSearch_2opt(solution sol)
{
	solution best = sol;
	solution curr = sol;

	bool improv = true;
	int i = 0;
	while (improv == true) {
		improv = false;
		for(int i = 0; i < sol.routes.size(); i++){ // iterate over routes
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

solution Algorithms::localSearch_2swap(solution sol)
{

	solution best = sol;
	solution curr = sol;

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

solution Algorithms::localSearch_iRouteSwap(solution sol)
{
	solution best = sol;

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

								solution s = iRouteSwap2(sol, i, j, a, b);
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

solution Algorithms::VNS(solution init)
{
	srand(time(nullptr));

	solution x = init;

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

			solution xl = shake(x, n.at(k - 1)); // shaking
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

solution Algorithms::createOptimialSolution1()
{
	vector<int> a{ 0, 7, 6, 3, 8, 10, 11, 5, 9, 12 };
	vector<vertex> r;
	for (auto i : a) {
		vertex v;
		v.key = i;
		r.push_back(v);
	}
	r = fixRoute(r);

	solution s;
	s.routes.push_back(r);
	s = procSol(s);

	return s;
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

// shake function receives a solution and perform a shake with a given n neighboorhood structure
// the function can generate an infeasible solution
solution Algorithms::shake(solution s, string n)
{
	if (n == "2swap") {

		int route = rand() % s.routes.size();
		int beg = (rand() % (s.routes.at(route).size() - 3)) + 1;

		int end = (rand() % s.routes.at(route).size());
		while (end <= beg || end == s.routes.at(route).size()) {
			end = (rand() % s.routes.at(route).size());
		}
		solution new_s = swap2(s, route, beg, end);
		new_s = procSol(new_s);

		return new_s;
	}
	else if (n == "2opt") {
		solution new_s;

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

		solution new_s = iRouteSwap2(s, route1, route2, beg, end);
		new_s = procSol(new_s);

		return new_s;
	}
	else {
	}
}

solution Algorithms::nChange(solution s, solution best, int &k)
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

solution Algorithms::search_2opt(solution sol, solution best)
{
	bool improv = false;
	solution curr;
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

solution Algorithms::search_2swap(solution sol, solution best)
{
	bool improv = false;
	solution curr;
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

solution Algorithms::search_iRouteSwap(solution sol, solution best)
{
	bool improv = false;
	solution curr;
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

solution Algorithms::opt2(solution sol, int route, int beg, int end)
{
	if (beg > end) {
		throw MovementFailed();
	}

	solution newSol;
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

solution Algorithms::swap2(solution sol, int r, int beg, int end)
{
	solution newSol;
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
solution Algorithms::iRouteSwap2(solution sol, int route1, int route2, int p1, int p2)
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

int Algorithms::test()
{
	cout << "Greed\n";
	solution s1 = greed();
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

	solution s2 = createOptimialSolution1();
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

int Algorithms::test2()
{

	permutation p = randomPermutation();

	for (auto i : p) {
		cout << i << " ";
	}
	cout << endl;

	solution s = permutationToSolution(p);

	vector<int> w(s.routes.size(), 0);

	for (int i = 0; i < s.routes.size(); i++) {
		for (int j = 0; j < s.routes.at(i).size(); j++) {
			node n = inst->getNodeByKey(s.routes.at(i).at(j).key);
			w.at(i) += n.demand;
		}
	}

	for (int i = 0; i < s.routes.size(); i++) {
		cout << w.at(i) << ": ";
		for (int j = 0; j < s.routes.at(i).size(); j++) {
			cout << s.routes.at(i).at(j).key << " ";
		}
		cout << endl;
	}

	return 0;
}

Algorithms::Algorithms()
{
}

int Algorithms::loadInstance(string dir, string fileName, int type)
{
	inst = new instance(dir, fileName, type);

	return 0;
}

solution Algorithms::greed()
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
	row = getRow();
	
	solution s;
	s.routes = sol;
	s = procSol(s);

	return s;
}

solution Algorithms::greed2()
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
	return solution();
}

solution Algorithms::procSol(solution s)
{
	s.FOp = FOComplete(s.routes);
	s.FO = s.FOp.front();
	s.inf = fullEval(s.routes);

	return s;
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

	int p = FOP(sol);

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

vector<int> Algorithms::FOComplete(vector<vector<vertex>> sol)
{
	auto UD0 = inst->set_UD0();
	auto UD1 = inst->set_UD1();
	auto C = inst->set_C();
	auto R = inst->set_R();
	auto V0 = inst->set_V0();
	auto V1 = inst->set_V1();
	auto V01 = inst->set_V01();

	int fo = 0;
	vector<int> fo_parcels;

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

	int p = FOP(sol);

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
				ret.insert("vehicle_load");
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
				ret.insert("customers_coverage");
			}
		}
	}

	vector<string> r;
	for (string s : ret) {
		r.push_back(s);
	}


	return r;
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

void Algorithms::printSol(solution s)
{
	cout << "FO: " << s.FO << " - ";
	for (auto i : s.FOp) {
		cout << i << " ";
	}
	cout << endl;
	cout << "Inf: ";
	for (auto i : s.inf) {
		cout << i << " ";
	}
	cout << endl;
	for (auto r : s.routes) {
		for (auto v : r) {
			cout << v.key << " ";
		}
		cout << endl;
	}
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

void Algorithms::getSol(Solution sol, ostream & strm)
{
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

void Algorithms::getSol(solution sol, ostream & strm)
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

Algorithms::~Algorithms()
{

}
