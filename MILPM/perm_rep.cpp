#include "perm_rep.h"

solution perm_rep::permutationToSolution(permutation p)
{
	vector<bool> coverage(inst->nodes.size(), false);

	solution s;
	s.perm = p;

	// adding the first vertex to the first route
	int v = p.front(); // get the first element in the permutation

	vertex new_v; // create the vertex to be added
	new_v.key = v;

	s.routes.resize(1); // set the number of route to one
	s.routes.front().push_back(new_v);

	p.erase(p.begin()); // remove the first element that were already added

	// search for the closest depot from the fisrt inserted node
	// we will use it to estimate the arrival and departure time in each node
	// we intent to minimize the number of vertexes infeasible due to time window constraints
	int minTT = INT_MAX;
	int dpt = -1;
	auto UD0 = inst->set_UD0();
	for (node n : UD0) {
		int tt = inst->getTD(n.key, s.routes.front().front().key);
		if (tt < minTT) {
			minTT = tt;
			dpt = n.key;
		}
	}
	// set the fisrt vertex`s arrival and departure time
	s.routes.front().front().aTime = minTT;
	s.routes.front().front().lTime = minTT + inst->getNodeByKey(s.routes.front().front().key).serviceTime;

	vector<int> weight; // this vector will store the amount of freight required to in the route // it will be use to determine if the routes is already full and cant supply more customers
	weight.push_back(inst->getNodeByKey(v).demand);

	//
	for (auto i : p) { // iterate sequentially over the permutation adding the customers to a route
		node n = inst->getNodeByKey(i);


		// search which route can receive the next permutation node
		int j = 0;
		for (j = 0; j < weight.size(); j++) {
			vertex back = s.routes.at(j).back();
			int time = back.lTime + inst->getTD(back.key, n.key);

			if (n.demand + weight.at(j) <= inst->C && time <= n.dueDate) { // check the route total freight and stipulated time windows

				// create the new vertex
				new_v.key = n.key;
				new_v.aTime = back.lTime + inst->getTD(back.key, new_v.key);
				new_v.lTime = new_v.aTime + n.serviceTime;

				s.routes.at(j).push_back(new_v); // insert node
				weight.at(j) += n.demand; // update the total freight weight in the route
				break;
			}
		}
		// new route will be created
		if (j == weight.size()) { // if no route can fit the next node we shall create a new one
			// create the new vertex
			new_v.key = n.key;

			vector<vertex> temp;
			temp.push_back(new_v); // create the new route with the first node
			s.routes.push_back(temp); // add the new route to solution
			weight.push_back(n.demand); // add the route's weight in the weight vector

			minTT = INT_MAX;
			dpt = -1;
			//auto UD0 = inst->set_UD0();
			for (node n : UD0) {
				int tt = inst->getTD(n.key, s.routes.at(j).front().key);
				if (tt < minTT) {
					minTT = tt;
					dpt = n.key;
				}
			}
			// set the fisrt vertex`s arrival and departure time
			s.routes.at(j).front().aTime = minTT;
			s.routes.at(j).front().lTime = minTT + inst->getNodeByKey(s.routes.at(j).front().key).serviceTime;

			//new_v.aTime = inst->getTD(s.routes.at(j).back().key, new_v.key);
			//new_v.lTime = new_v.aTime + n.serviceTime;


		}

	}

	s = addDepots(s);
	try {
		double n = inst->set_R().size();
		if (n >= 2) {
			n = floor(n * 0.1) + 2;
		}
		else if (n == 1) {
			n = 1;
		}
		else if (n == 0) {
			// throw an error?
		}

		//cout << n << endl;

		s = addStations(s);
	}
	catch (IsolatedNode &e) {
		cout << e.what() << endl;
	}

	s = procSol(s);

	return s;
}

solution perm_rep::permutationToSolution(permutation p, int cn)
{
	vector<bool> coverage(inst->nodes.size(), false);

	solution s;
	s.perm = p;

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
	for (auto i : p) { // iterate sequentially over the permutation adding the customers to a route
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
	s = addStations(s, cn);
	s = procSol(s); // maybe will generate a bug where the permutation will be erased

	return s;
}

solution perm_rep::permutationToSolutionGrasp(permutation p)
{
	vector<bool> coverage(inst->nodes.size(), false);

	solution s;
	s.perm = p;

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
	for (auto i : p) { // iterate sequentially over the permutation adding the customers to a route
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
	s = addStationsGrasp(s);
	s = procSol(s); // maybe will generate a bug where the permutation will be erased

	return s;
}

solution perm_rep::addDepots(solution s)
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
		// int id = n.id_n;

		// insert the arrival depot node
		int a = -1;
		for (node no : UD1) {
			if (n.id_n == no.id_n) { // search the arrival node
				a = n.key;
				break;
			}
		}
		if (a == -1) {
			cout << "failed to find the depot's " << v.key << " arrival node" << endl;
		}
		v.key = a;
		r.push_back(v);


		s.routes.at(w) = r; // add updated route to solution

		w++;
	}

	for (int i = 0; i < s.routes.size(); i++) {
		s.routes.at(i).front().bLevel = inst->Q;
		s.routes.at(i).front().lTime = 0;
		s.routes.at(i).front().vLoad = inst->C;
		s.routes.at(i).front().aTime = 0;
		s.routes.at(i).front().wTime = 0;
	}

	return s;
}

solution perm_rep::addStations(solution s)
{
	// set all vehicles to full energy
	for (int j = 0; j < s.routes.size(); j++) {
		s.routes.at(j).front().bLevel = inst->Q;
	}

	vector<int> bssSequence;

	for (int j = 0; j < s.routes.size(); j++) {
		int curr = 1;

		while (curr < s.routes.at(j).size()) {
			int prev = curr - 1;
			node currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
			node prevNode = inst->getNodeByKey(s.routes.at(j).at(prev).key);

			// computing parameters

			// compute battery level in the current node
			s.routes.at(j).at(curr).bLevel = s.routes.at(j).at(prev).bLevel - inst->getBatteryUsed(s.routes.at(j).at(prev).key, s.routes.at(j).at(curr).key);

			// compute arrival time in the current node
			s.routes.at(j).at(curr).aTime = s.routes.at(j).at(prev).lTime + inst->getTD(prevNode.key, currNode.key);

			// compute waiting time in the current node
			if (s.routes.at(j).at(curr).aTime < currNode.readyTime) {
				s.routes.at(j).at(curr).wTime = currNode.readyTime - s.routes.at(j).at(curr).aTime;
			}
			else { // no waiting time
				s.routes.at(j).at(curr).wTime = 0;
			}

			// compute de departure time in the current node
			s.routes.at(j).at(curr).lTime = s.routes.at(j).at(curr).aTime + s.routes.at(j).at(curr).wTime + inst->getNodeByKey(currNode.key).serviceTime;

			// compute vehcile load in the current node
			s.routes.at(j).at(curr).vLoad = s.routes.at(j).at(prev).vLoad - currNode.demand;

			// checking due date
			if (s.routes.at(j).at(curr).aTime > currNode.dueDate) {
				// if the current node is reached after the due date, we will remove this node from the route and push it to the next

				vertex v = s.routes.at(j).at(curr); // store the node key that will be removed				
				s.routes.at(j).erase(s.routes.at(j).begin() + curr); // remove the node from the route

				// checking if we are in the last route
				if (j < s.routes.size() - 1) {
					int rSize = s.routes.at(j + 1).size();
					s.routes.at(j + 1).insert(s.routes.at(j + 1).begin() + rSize - 1, v); // insert just before the last node
					curr--;
				}
				// if we are in the last route we have to create a new route
				else {
					route r; // new route

					// search for the closest, already in use, depot from the first customer node in the route
					vertex dpt;
					dpt.key = -1;
					int minDist = INT_MAX;
					for (route r : s.routes) {
						int dist = inst->dist(r.front().key, v.key);

						if (dist < minDist) {
							minDist = dist;
							dpt.key = r.front().key;
						}
					}

					// add depot in the route
					dpt.bLevel = inst->Q;
					dpt.lTime = 0;
					dpt.vLoad = inst->C;
					dpt.aTime = 0;
					dpt.wTime = 0;

					r.push_back(dpt);

					// add depot arrival node
					r.push_back(dpt); // same key as the depot dispatch node // bug may ocurr here due to key

					// insert customer in the route
					r.insert(r.begin() + 1, v);

					// insert route in the solution
					s.routes.push_back(r);
					curr--;
				}


			}
			// checking battery level
			else if (s.routes.at(j).at(curr).bLevel < 0) {
				// if not enough battery we have to insert a bss in the route

				int minBU = 0; // minimum battery usage
				int meanBU = 0;

				while (s.routes.at(j).at(curr).bLevel - minBU < 0) {
					int put = false;
					curr--;
					minBU = INT_MAX;

					currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
					int bss;
					// search for the closes bss
					if (currNode.type != "f") {
						vector<node> R = inst->set_R(); // get all stations

						// search for the closest bss from the stop node
						for (node r : R) {

							int BU = inst->getBatteryUsed(s.routes.at(j).at(curr).key, r.key); // inst->dist(r.key, b);

							// get the lowest dist
							if (BU < minBU && r.key != s.routes.at(j).at(curr).key) {
								bss = r.key;
								minBU = BU;
							}
						}
					}
					else {
						vector<node> R = inst->set_R(); // get all stations
						vector<node> R2 = inst->set_R(); // get all stations

						//
						int curr__ = curr - 1;
						bssSequence.clear();
						while (inst->getNodeByKey(s.routes.at(j).at(curr__).key).type == "f") {
							bssSequence.push_back(s.routes.at(j).at(curr__).key);
							curr__--;
						}

						// remove all bss that initially precced the current one in order to avoid loops
						for (int key : bssSequence) {
							for (int i = 0; i < R.size(); i++) {
								if (R.at(i).key == key) {
									R.erase(R.begin() + i);
								}
							}
						}

						if (R.size() == 0) { // this means that the next node is isolated and cant be reached even by a chain of bss
							// remove the isolated node and push it to the next route
							vertex aux = s.routes.at(j).at(curr + 1);
							s.routes.at(j).erase(s.routes.at(j).begin() + curr + 1);

							// push to next route
							if (j < s.routes.size() - 2) { // if we have routes to push
								int size = s.routes.at(j + 1).size();
								s.routes.at(j + 1).insert(s.routes.at(j + 1).begin() + size - 1, aux);

							}
							else { // no more routes to push, create a new one
								route r;

								vertex bss;
								int minDist = INT_MAX;
								for (route r : s.routes) {
									int dist = inst->dist(r.front().key, aux.key);

									if (dist < minDist) {
										minDist = dist;
										bss = r.front();
									}
								}

								r.push_back(bss);
								r.push_back(aux);
								r.push_back(bss);

								s.routes.push_back(r); // bug?

							}

							// we also remove all the previous chain of bss
							node n = inst->getNodeByKey(s.routes.at(j).at(curr).key);
							while (n.type == "f") {
								s.routes.at(j).erase(s.routes.at(j).begin() + curr);
								curr--;
								n = inst->getNodeByKey(s.routes.at(j).at(curr).key);
							}

							// throw IsolatedNode(s.routes.at(j).at(curr + 1).key, j, s.perm);							
						}

						// search for the closest bss from the stop node
						for (node r : R) {
							int BU = inst->getBatteryUsed(s.routes.at(j).at(curr).key, r.key); // inst->dist(r.key, b);

							// get the lowest dist
							if (BU < minBU && r.key != s.routes.at(j).at(curr).key) {
								bss = r.key;
								minBU = BU;
							}
						}
					}

					// compute amount of energy the vehcile can recharge during service time in the current routes segment
					int curr_ = curr;
					int energy = 0;
					while (true) {

						node n = inst->getNodeByKey(s.routes.at(j).at(curr_).key);

						if (n.type == "f" || n.type == "d") {
							break;
						}

						energy += inst->g * n.serviceTime;
						curr_--;
					}

					int cEnergy = (s.routes.at(j).at(curr).bLevel + energy);

					if (s.routes.at(j).at(curr).bLevel - minBU >= 0) { // if battery is enough to go from current node and closest bss, insert bss in the route
						vertex v;
						v.key = bss;

						s.routes.at(j).insert(s.routes.at(j).begin() + curr + 1, v); // insert in the routes

						currNode = inst->getNodeByKey(s.routes.at(j).at(curr + 1).key);

						s.routes.at(j).at(curr + 1).bLevel = inst->Q;
						s.routes.at(j).at(curr + 1).recharge = true;
						s.routes.at(j).at(curr + 1).vLoad = s.routes.at(j).at(curr).vLoad;
						s.routes.at(j).at(curr + 1).recharged = inst->Q - (s.routes.at(j).at(curr).bLevel - minBU); // compute the amount of energy recharged
						s.routes.at(j).at(curr + 1).aTime = s.routes.at(j).at(curr).lTime + inst->getTD(currNode.key, bss);
						s.routes.at(j).at(curr + 1).lTime = s.routes.at(j).at(curr + 1).aTime + currNode.serviceTime + inst->ct;

						curr++;
						put = true;

					}
					else if ((s.routes.at(j).at(curr).bLevel + energy) - minBU >= 0) { // considering recharge during service time
						// recharge during service time
						curr_ = curr;
						energy = 0;
						while (true) {

							node n = inst->getNodeByKey(s.routes.at(j).at(curr_).key);

							// force to recharge just enough to reach the bss
							if (n.type == "f" || n.type == "d" || s.routes.at(j).at(curr).bLevel + energy > minBU) {
								break;
							}

							s.routes.at(j).at(curr_).recharge = true;
							s.routes.at(j).at(curr_).recharged = inst->g * n.serviceTime;
							s.routes.at(j).at(curr_).bLevel += s.routes.at(j).at(curr_).recharged;
							if (s.routes.at(j).at(curr_).bLevel > inst->Q) {
								s.routes.at(j).at(curr_).bLevel = inst->Q;
							}

							energy += inst->g * n.serviceTime;
							curr_--;
						}

						// insert bss vertex in the route
						vertex v;
						v.key = bss;
						v.bLevel = inst->Q;
						v.recharge = true;
						v.vLoad = s.routes.at(j).at(prev).vLoad;
						v.recharged = inst->Q - (s.routes.at(j).at(curr).bLevel - minBU); // compute the amount of energy recharged
						v.aTime = s.routes.at(j).at(prev).lTime + inst->getTD(prevNode.key, bss);
						v.lTime = s.routes.at(j).at(curr).aTime + currNode.serviceTime + inst->ct;

						s.routes.at(j).insert(s.routes.at(j).begin() + curr + 1, v); // insert in the routes

						curr++;
						put = true;
					}
					else {
						//bssSequence.clear();
					}

					if (put == true) {
						break;
					}
				}
			}
			curr++;
		}
	}
	return s;
	/*
	for (auto r : s.routes) {
		for (auto v : r) {
			cout << v.key << " ";
		}
		cout << endl;
	}

	vector<int> sum; // store the amount of energy required to perform the routes

	// compute the amount of energy required to perform the entire route
	for (route r : s.routes) {
		sum.push_back(0);

		for (int i = 0; i < r.size() - 1; i++) {
			node n1 = inst->getNodeByKey(r.at(i).key);
			node n2 = inst->getNodeByKey(r.at(i + 1).key);

			sum.back() += inst->getBatteryUsed(n1.key, n2.key);
		}

	}

	// set all vehicles to full energy
	for (int j = 0; j < s.routes.size(); j++) {
		s.routes.at(j).front().bLevel = inst->Q;
	}

	vector<int> counter(s.routes.size(), 1); // store the maximum node in each route the vehicle can travel with its battery charge
	vector<int> counterP;

	vector<bool> end(s.routes.size(), false); // record the routes that have already beeing fully traveled
	vector<bool> hold(s.routes.size(), false); // record which routes are waiting to have it vehicle to refuel
	vector<int> energy; // store the amount of energy the vehicle can recharge during service time in the partial route defined by counterP and counter

	// iterate over all routes, computing in which node the vehicle runs out of energy
	int it = 0;
	while (true) {
		cout << "it: " << it << endl;
		it++;
		string st;
		getline(cin, st);

		energy.resize(s.routes.size(), 0);

		counterP = counter;

		// compute the vehicle autonomy in each route j
		// here we determine the node in the route where the vehicle cant reach due to its battery capacity
		// the vector counter will count until the vehicle has any amount of negative energy, meaning that it will need to perform a recharge or a battery swap
		for (int j = 0; j < s.routes.size(); j++) { // for each route
			energy.at(j) = 0;

			while (counter.at(j) < s.routes.at(j).size()) { // iterate over the route j
				if (counter.at(j) > s.routes.at(j).size()) {
					cout << "Error\n";
					exit(1);
				}

				if(j == 0)
					cout << "counter: " << counter.at(j) << endl;


				int curr = counter.at(j); // current node in route j
				int prev = curr - 1;

				int keyCurr = s.routes.at(j).at(curr).key;
				int keyPrev = s.routes.at(j).at(curr - 1).key;

				node currNode = inst->getNodeByKey(keyCurr);

				// compute battery level in the current node
				s.routes.at(j).at(curr).bLevel = s.routes.at(j).at(prev).bLevel - inst->dist(s.routes.at(j).at(prev).key, s.routes.at(j).at(curr).key);

				// compute arrival time in the current node
				s.routes.at(j).at(curr).aTime = s.routes.at(j).at(prev).lTime + inst->getTD(keyPrev, keyCurr);

				// compute waiting time in the current node
				if (s.routes.at(j).at(curr).aTime < currNode.readyTime) {
					s.routes.at(j).at(curr).wTime = currNode.readyTime - s.routes.at(j).at(curr).aTime;
				}
				else { // no waiting time
					s.routes.at(j).at(curr).wTime = 0;
				}

				// compute de departure time in the current node
				s.routes.at(j).at(curr).lTime = s.routes.at(j).at(curr).aTime + s.routes.at(j).at(curr).wTime + inst->getNodeByKey(keyCurr).serviceTime;

				// compute vehcile load in the current node
				s.routes.at(j).at(curr).vLoad = s.routes.at(j).at(prev).vLoad - currNode.demand;

				// compute the amount of energy that can be charged during service time
				int n = s.routes.at(j).at(curr).key;
				energy.at(j) += inst->getNodeByKey(n).serviceTime * inst->g;

				if (s.routes.at(j).at(curr).bLevel >= 0) { // if battery is enough to travel to next node
					counter.at(j)++; // keep going
				}
				else {
					hold.at(j) = true; // indicates that the vehicle must be recharged
					break; // go to next route
				}

				// stop criteria
				int key = currNode.key;
				string type = inst->getNodeByKey(key).type;
				if (type == "a") { // counter.at(j) > s.routes.at(j).size()
					end.at(j) = true;
					//break;
				}
			}
		}

		// check if all vehicles are on hold, it means that all vehicles are waiting to be recharged
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

		// if all vehicles waiting to recharge (on hold) we must find a good spot to them to do so
		if (recharge == true) {
			cout << "all vehicles are on hold\n";

			for (int j = 0; j < s.routes.size(); j++) { // for each route
				if (counter.at(j) > s.routes.at(j).size()) {
					cout << "Error\n";
					exit(1);
				}

				if (hold.at(j) == true && end.at(j) == false) { // if vehicle is in need of energy
					// try to insert each bss in between the hold position and the previous one

					int a = s.routes.at(j).at(counter.at(j) - 1).key; // previous position key
					int b = s.routes.at(j).at(counter.at(j)).key; // hold position key

					int minDist = INT_MAX;
					int minMean = INT_MAX;

					vector<node> R = inst->set_R(); // get all stations
					int n;
					// search for the closest bss fromthe hold node
					for (node r : R) {

						int AR = inst->getBatteryUsed(a, r.key); // inst->dist(a, r.key);
						int RB = inst->getBatteryUsed(r.key, b); // inst->dist(r.key, b);
						int meanAB = (AR + RB) / 2;

						// get the lowest mean
						if (meanAB < minMean) {
							minMean = meanAB;
						}
						// get the lowest dist
						if (RB < minDist) {
							n = r.key;
							minDist = RB;
						}

					}
					// first option
					// if vehicle can reach the closest bss it will move there in order to recharge

					if (s.routes.at(j).at(counter.at(j)).bLevel >= minDist) {
						cout << "Option 1\n";
						cout << "Inserting BSS " << n << " in solution\n";

						vertex v;
						v.key = n;
						s.routes.at(j).insert(s.routes.at(j).begin() + counter.at(j) + 1, v); // insert the closest bss in the route j
					}
					// second option
					//check if the vehicle can charge in all privious nodes in the route in order to reach bss
					else if (s.routes.at(j).at(counter.at(j)).bLevel + energy.at(j) >= minDist) {
						cout << "option 2\n";
						cout << "Vehicle will recharge during service time" << endl;
						// recharge during service

						for (int i = counterP.at(j); i < counter.at(j); i++) {
							int key = s.routes.at(j).at(i).key;
							cout << inst->getNodeByKey(key).serviceTime * inst->g << endl;
							s.routes.at(j).at(i).bLevel += inst->getNodeByKey(key).serviceTime * inst->g;
							s.routes.at(j).at(i).recharged = inst->getNodeByKey(key).serviceTime * inst->g; // indicates the amount of battery recharged on each part of the route
							s.routes.at(j).at(i).recharge = true; // indicates that the vechilce was charged
						}
					}
					// third option
					// if there is not enough battery in the vehicle, even using service time to recharge, we will try to trace back the route to find a suitable place so the vehcile can recharge
					else if (s.routes.at(j).at(counter.at(j)).bLevel + energy.at(j) < minDist) {
						// traceback the route
						cout << "Option 3\n";

						int endPos = counter.at(j); // -1;
						int begPos = counterP.at(j);

						cout << begPos << " - " << endPos << endl;

						bool inserted = false; // indicates that a bss was inserted in the solution

						while (endPos > begPos) {
							// tracing back

							// get nearest bss from the previous node (b)
							int min = INT_MAX;
							int minMean = INT_MAX;

							vector<node> R = inst->set_R(); // get all stations
							int bss = -1;
							// search the nearest bss of endPos
							for (node r : R) {
								int buAR = inst->getBatteryUsed(s.routes.at(j).at(endPos).key, r.key);

								int buRB = inst->getBatteryUsed(r.key, s.routes.at(j).at(endPos + 1).key);

								// lowest mean
								int mean = (buAR + buRB) / 2;
								if (mean < min) {
									 minMean = mean;
								}

								// get the lowest dist
								if (buAR < min) {
									bss = r.key;
									min = buAR;
								}
							}

							if (s.routes.at(j).at(endPos).bLevel - min >= 0) { // checking of its possible to reach the nearest bss
								cout << "bss " << bss << " will be inserted after " << endPos << endl;
								cout << "min: " << min << endl;

								for (auto v : s.routes.at(j)) {
									cout << v.key << " ";
								}
								cout << endl;

								vertex v;
								v.key = bss;
								v.bLevel = inst->Q;
								v.recharge = true;

								v.recharged = inst->Q - (s.routes.at(j).at(endPos).bLevel - inst->getBatteryUsed(endPos, bss)); // compute the amount of energy recharged
								s.routes.at(j).insert(s.routes.at(j).begin() + endPos + 1, v); // insert in the routes

								counter.at(j) = endPos + 2;
								counterP.at(j) = endPos + 2;

								inserted = true;

								for (auto v : s.routes.at(j)) {
									cout << v.key << " ";
								}
								cout << endl;

								for (auto v : s.routes.at(j)) {
									cout << v.bLevel << " ";
								}
								cout << endl;

								break;
							}

							if (inserted == true) { // stop insertion
								break;
							}

							endPos--;
						}
					}
				}
			}
		}

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
	*/
}

solution perm_rep::addStations(solution s, int n)
{
	// set all vehicles to full energy
	for (int j = 0; j < s.routes.size(); j++) {
		s.routes.at(j).front().bLevel = inst->Q;
	}

	vector<int> bssSequence;

	for (int j = 0; j < s.routes.size(); j++) {
		int curr = 1;

		while (curr < s.routes.at(j).size()) {
			int prev = curr - 1;
			node currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
			node prevNode = inst->getNodeByKey(s.routes.at(j).at(prev).key);

			// computing parameters

			// compute battery level in the current node
			s.routes.at(j).at(curr).bLevel = s.routes.at(j).at(prev).bLevel - inst->getBatteryUsed(s.routes.at(j).at(prev).key, s.routes.at(j).at(curr).key);

			// compute arrival time in the current node
			s.routes.at(j).at(curr).aTime = s.routes.at(j).at(prev).lTime + inst->getTD(prevNode.key, currNode.key);

			// compute waiting time in the current node
			if (s.routes.at(j).at(curr).aTime < currNode.readyTime) {
				s.routes.at(j).at(curr).wTime = currNode.readyTime - s.routes.at(j).at(curr).aTime;
			}
			else { // no waiting time
				s.routes.at(j).at(curr).wTime = 0;
			}

			// compute de departure time in the current node
			s.routes.at(j).at(curr).lTime = s.routes.at(j).at(curr).aTime + s.routes.at(j).at(curr).wTime + inst->getNodeByKey(currNode.key).serviceTime;

			// compute vehcile load in the current node
			s.routes.at(j).at(curr).vLoad = s.routes.at(j).at(prev).vLoad - currNode.demand;

			// checking due date
			if (s.routes.at(j).at(curr).aTime > currNode.dueDate) {
				// if the current node is reached after the due date, we will remove this node from the route and push it to the next

				vertex v = s.routes.at(j).at(curr); // store the node key that will be removed				
				s.routes.at(j).erase(s.routes.at(j).begin() + curr); // remove the node from the route

				// checking if we are in the last route
				if (j < s.routes.size() - 1) {
					int rSize = s.routes.at(j + 1).size();
					s.routes.at(j + 1).insert(s.routes.at(j + 1).begin() + rSize - 1, v); // insert just before the last node
					curr--;
				}
				// if we are in the last route we have to create a new route
				else {
					route r; // new route

					// search for the closest, already in use, depot from the first customer node in the route
					vertex dpt;
					dpt.key = -1;
					int minDist = INT_MAX;
					for (route r : s.routes) {
						int dist = inst->dist(r.front().key, v.key);

						if (dist < minDist) {
							minDist = dist;
							dpt.key = r.front().key;
						}
					}

					// add depot in the route
					dpt.bLevel = inst->Q;
					dpt.lTime = 0;
					dpt.vLoad = inst->C;
					dpt.aTime = 0;
					dpt.wTime = 0;

					r.push_back(dpt);

					// add depot arrival node
					r.push_back(dpt); // same key as the depot dispatch node // bug may ocurr here due to key

					// insert customer in the route
					r.insert(r.begin() + 1, v);

					// insert route in the solution
					s.routes.push_back(r);
					curr--;
				}


			}
			// checking battery level
			else if (s.routes.at(j).at(curr).bLevel < 0) {
				// if not enough battery we have to insert a bss in the route

				int minBU = 0; // minimum battery usage
				int meanBU = 0;

				while (s.routes.at(j).at(curr).bLevel - minBU < 0) {
					int put = false;
					curr--;
					minBU = INT_MAX;

					currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);

					int bss;
					// search for the closest bss
					if (currNode.type != "f") {
						/*
						vector<node> R = inst->set_R(); // get all stations

						// search for the closest bss from the stop node
						for (node r : R) {

							int BU = inst->getBatteryUsed(s.routes.at(j).at(curr).key, r.key); // inst->dist(r.key, b);

							// get the lowest dist
							if (BU < minBU && r.key != s.routes.at(j).at(curr).key) {
								bss = r.key;
								minBU = BU;
							}
						}
						*/

						// generate a candidate list with size n

						struct aux {
							int id, BU;
						};
						vector<aux> cand;

						// compute all the distances from the current node to all bss
						vector<node> R = inst->set_R(); // get all stations
						for (node r : R) {

							int BU = inst->getBatteryUsed(s.routes.at(j).at(curr).key, r.key);
							aux a;
							a.id = r.key;
							a.BU = BU;

							cand.push_back(a);
						}

						// paritaly sort the list
						partial_sort(cand.begin(), cand.begin() + n, cand.end(),
							[](aux a, aux b) -> bool {
							return a.BU < b.BU;
						}
						);

						int c = Random::get(0, n);

						//cout << "c: " << c << endl;

						bss = cand.at(c).id;
						minBU = cand.at(c).BU;
					}
					else {
						vector<node> R = inst->set_R(); // get all stations
						//vector<node> R2 = inst->set_R(); // get all stations

						//
						int curr__ = curr - 1;
						bssSequence.clear();
						while (inst->getNodeByKey(s.routes.at(j).at(curr__).key).type == "f") {
							bssSequence.push_back(s.routes.at(j).at(curr__).key);
							curr__--;
						}

						// remove all bss that initially precced the current one in order to avoid loops
						for (int key : bssSequence) {
							for (int i = 0; i < R.size(); i++) {
								if (R.at(i).key == key) {
									R.erase(R.begin() + i);
								}
							}
						}

						if (R.size() == 0) { // this means that the next node is isolated and cant be reached even by a chain of bss
							// remove the isolated node and push it to the next route
							vertex aux = s.routes.at(j).at(curr + 1);
							s.routes.at(j).erase(s.routes.at(j).begin() + curr + 1);

							// push to next route
							if (j < s.routes.size() - 2) { // if we have routes to push
								int size = s.routes.at(j + 1).size();
								s.routes.at(j + 1).insert(s.routes.at(j + 1).begin() + size - 1, aux);

							}
							else { // no more routes to push, create a new one
								route r;

								vertex bss;
								int minDist = INT_MAX;
								for (route r : s.routes) {
									int dist = inst->dist(r.front().key, aux.key);

									if (dist < minDist) {
										minDist = dist;
										bss = r.front();
									}
								}

								r.push_back(bss);
								r.push_back(aux);
								r.push_back(bss);

								s.routes.push_back(r); // bug?

							}

							// we also remove all the previous chain of bss
							node n = inst->getNodeByKey(s.routes.at(j).at(curr).key);
							while (n.type == "f") {
								s.routes.at(j).erase(s.routes.at(j).begin() + curr);
								curr--;
								n = inst->getNodeByKey(s.routes.at(j).at(curr).key);
							}

							// throw IsolatedNode(s.routes.at(j).at(curr + 1).key, j, s.perm);							
						}

						// search for the closest bss from the stop node
						for (node r : R) {
							int BU = inst->getBatteryUsed(s.routes.at(j).at(curr).key, r.key); // inst->dist(r.key, b);

							// get the lowest dist
							if (BU < minBU && r.key != s.routes.at(j).at(curr).key) {
								bss = r.key;
								minBU = BU;
							}
						}
					}

					// compute amount of energy the vehcile can recharge during service time in the current routes segment
					int curr_ = curr;
					int energy = 0;
					while (true) {

						node n = inst->getNodeByKey(s.routes.at(j).at(curr_).key);

						if (n.type == "f" || n.type == "d") {
							break;
						}

						energy += inst->g * n.serviceTime;
						curr_--;
					}

					int cEnergy = (s.routes.at(j).at(curr).bLevel + energy);

					if (s.routes.at(j).at(curr).bLevel - minBU >= 0) { // if battery is enough to go from current node and closest bss, insert bss in the route
						vertex v;
						v.key = bss;

						s.routes.at(j).insert(s.routes.at(j).begin() + curr + 1, v); // insert in the routes

						currNode = inst->getNodeByKey(s.routes.at(j).at(curr + 1).key);

						s.routes.at(j).at(curr + 1).bLevel = inst->Q;
						s.routes.at(j).at(curr + 1).recharge = true;
						s.routes.at(j).at(curr + 1).vLoad = s.routes.at(j).at(curr).vLoad;
						s.routes.at(j).at(curr + 1).recharged = inst->Q - (s.routes.at(j).at(curr).bLevel - minBU); // compute the amount of energy recharged
						s.routes.at(j).at(curr + 1).aTime = s.routes.at(j).at(curr).lTime + inst->getTD(currNode.key, bss);
						s.routes.at(j).at(curr + 1).lTime = s.routes.at(j).at(curr + 1).aTime + currNode.serviceTime + inst->ct;

						curr++;
						put = true;

					}
					else if ((s.routes.at(j).at(curr).bLevel + energy) - minBU >= 0) { // considering recharge during service time
						// recharge during service time
						curr_ = curr;
						energy = 0;
						while (true) {

							node n = inst->getNodeByKey(s.routes.at(j).at(curr_).key);

							// force to recharge just enough to reach the bss
							if (n.type == "f" || n.type == "d" || s.routes.at(j).at(curr).bLevel + energy > minBU) {
								break;
							}

							s.routes.at(j).at(curr_).recharge = true;
							s.routes.at(j).at(curr_).recharged = inst->g * n.serviceTime;
							s.routes.at(j).at(curr_).bLevel += s.routes.at(j).at(curr_).recharged;
							if (s.routes.at(j).at(curr_).bLevel > inst->Q) {
								s.routes.at(j).at(curr_).bLevel = inst->Q;
							}

							energy += inst->g * n.serviceTime;
							curr_--;
						}

						// insert bss vertex in the route
						vertex v;
						v.key = bss;
						v.bLevel = inst->Q;
						v.recharge = true;
						v.vLoad = s.routes.at(j).at(prev).vLoad;
						v.recharged = inst->Q - (s.routes.at(j).at(curr).bLevel - minBU); // compute the amount of energy recharged
						v.aTime = s.routes.at(j).at(prev).lTime + inst->getTD(prevNode.key, bss);
						v.lTime = s.routes.at(j).at(curr).aTime + currNode.serviceTime + inst->ct;

						s.routes.at(j).insert(s.routes.at(j).begin() + curr + 1, v); // insert in the routes

						curr++;
						put = true;
					}
					else {
						//bssSequence.clear();
					}

					if (put == true) {
						break;
					}
				}
			}
			curr++;
		}
	}
	return s;
}

solution perm_rep::addStationsGrasp(solution s)
{
	solution best = s;

	int maxIt = 25;

	int counter = 0;
	while (counter < maxIt) {
		// building
		solution sl = s; // permutationToSolution(s.perm, -1);

		// local search
		solution best = permutationToSolution(sl.perm);
		best = procSol(best);
		best.perm = sl.perm;

		bool improv = true;
		while (improv == true) {
			improv = false;
			solution curr = best;

			for (int i = 0; i < sl.perm.size() - 1; i++) {
				for (int j = i + 1; j < sl.perm.size(); j++) {
					permutation pl = curr.perm;

					// swap nodes in the permutation
					int aux = pl.at(j);
					pl.at(j) = pl.at(i);
					pl.at(i) = aux;

					solution sl = permutationToSolution(pl, -1);
					//sl = procSol(sl);

					if (sl.FO < best.FO) {
						best = sl;
						improv = true;
						return best;
					}
				}
			}
		}
		sl = best;
		// end local search

		if (sl.FO < best.FO) {
			best = sl;
			counter = 0;
		}
		else {
			counter++;
		}

	}

	return addStations(s);
}

permutation perm_rep::randomPermutation()
{
	permutation p;

	vector<node> C = inst->set_C(); // get the set of all customers

	//seed
	srand(time(NULL));

	while (C.size() > 0) {
		//int pos = rand() % C.size(); // draws a customer to be added in the permutation
		int c = C.size();
		auto pos = Random::get(0, c - 1);

		p.push_back(C.at(pos).key); // add the customer node in the permutation

		// erase customer from vector C
		//C.erase(remove(C.begin(), C.end(), C.at(pos)), C.end()); // remove erase idiom
		C.erase(C.begin() + pos);

	}

	return p;
}

route perm_rep::computeRoute(route sol)
{
	// fix the route
	sol.front().bLevel = inst->Q;
	sol.front().lTime = 0;
	sol.front().vLoad = inst->C;
	sol.front().wTime = 0;
	sol.front().aTime = 0;

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

node perm_rep::nearestBSS(int key)
{
	return node();
}

int perm_rep::availableRoute(solution s, int n)
{

	return 0;
}

solution perm_rep::localSearch(permutation p)
{

	solution best = permutationToSolution(p);
	best = procSol(best);
	best.perm = p;

	bool improv = true;
	while (improv == true) {
		improv = false;
		solution curr = best;

		for (int i = 0; i < p.size() - 1; i++) {
			for (int j = i + 1; j < p.size(); j++) {
				// cout << i << " - " << j << endl;

				permutation pl = curr.perm;

				// swap nodes in the permutation
				int aux = pl.at(j);
				pl.at(j) = pl.at(i);
				pl.at(i) = aux;

				solution sl = permutationToSolution(pl);
				sl = procSol(sl);

				if (sl.FO < best.FO) {
					best = sl;
					improv = true;
					return best;
				}
			}
		}
	}

	return best;
}

solution perm_rep::localSearch2(permutation p)
{
	solution best = permutationToSolution(p);
	best = procSol(best);
	best.perm = p;

	bool improv = true;
	while (improv == true) {
		improv = false;
		solution curr = best;

		for (int i = 0; i < p.size() - 1; i++) {
			for (int j = i + 1; j < p.size(); j++) {
				permutation pl = curr.perm;

				// swap nodes in the permutation
				int aux = pl.at(j);
				pl.at(j) = pl.at(i);
				pl.at(i) = aux;

				solution sl = permutationToSolution(pl, -1);
				sl = procSol(sl);

				if (sl.FO < best.FO) {
					best = sl;
					improv = true;
					return best;
				}
			}
		}
	}

	return best;
}

permutation perm_rep::opt2(permutation p, int beg, int end)
{
	while (beg <= end) {
		int aux = p.at(beg);
		p.at(beg) = p.at(end);
		p.at(end) = aux;
		beg++;
		end--;
	}

	//solution s = permutationToSolution(p);

	return p;
}

solution perm_rep::GA(int popSize, int eliteP, int maxGen)
{
	int pressure;
	int eliteSize = ceil(popSize * (eliteP / 100.0));

	// creating initial random population
	vector<solution> pop;
	int sum = 0;
	for (int i = 0; i < popSize; i++) {
		permutation p = randomPermutation();
		solution s;

		// elite
		if (i < eliteSize) {
			s = localSearch(p);
			sum += s.FO;
		}
		else {
			s = permutationToSolution(p);
			sum += s.FO;
		}

		s.perm = p;
		pop.push_back(s);
	}
	int mean = sum / popSize;

	// get best solution
	solution best;
	best.FO = INT_MAX;
	for (solution s : pop) {
		if (s.FO < best.FO) {
			best = s;
		}
	}

	cout << "init = " << best.FO << endl;

	bool improv = true;
	int gen = 0;
	while (gen < maxGen) { // stop criteria, max number of generations without improvments
		cout << "gen: " << gen << endl;
		improv = false;
		// generating an offspring population
		vector<solution> offspring = generateOffspringPop(pop, eliteSize);
		pop = offspring;

		// get best solution so far		
		for (solution s : pop) {
			if (s.FO < best.FO) {
				best = s;
				improv = true;
				cout << "improvment!!\n";
				cout << best.FO << endl;
			}
		}

		if (improv == false) {
			gen++;
		}

	}

	return best;
}

vector<solution> perm_rep::generateOffspringPop(vector<solution> pop, int eliteSize)
{
	vector<solution> offspring;

	// select elite
	// the elite is the 20% fittest indivudual in the current population
	sort(pop.begin(), pop.end(), compIndividual);

	//int exeSize = (eliteSize * (eliteSize - 1)); // this is the total of possible crossovers done between elite members 

	// generating exe offspring population
	int max = ceil(pop.size() * 0.5);
	int count = 0;
	bool end = false;
	for (int i = 0; i < eliteSize - 1; i++) {
		for (int j = i + 1; j < eliteSize; j++) {
			vector<permutation> os = crossover(pop.at(i).perm, pop.at(j).perm);

			// mutation
			int x = Random::get(0, 100); // each individual has a 50% chance of suffering a mutation
			if (x < 50) {
				os.at(0) = mutation(os.at(0));
			}
			x = Random::get(0, 100);
			if (x < 50) {
				os.at(1) = mutation(os.at(1));
			}

			offspring.push_back(permutationToSolution(os.at(0)));
			count++;
			if (count == max) {
				end = true;
				break;
			}

			offspring.push_back(permutationToSolution(os.at(1)));
			count++;
			if (count == max) {
				end = true;
				break;
			}

		}
		if (end = true) {
			break;
		}
	}
	int exe = count;

	// generating exr offspring
	max = ceil(pop.size() * 0.3) + (max - count); // the number of exr individuals is 30% of the population size plus the remaining amount of individuals from the exe that couldnt be generated
	count = 0;
	end = false;
	for (int i = 0; i < eliteSize; i++) {
		for (int j = eliteSize; j < pop.size(); j++) {
			vector<permutation> os = crossover(pop.at(i).perm, pop.at(j).perm);

			// mutation
			int x = Random::get(0, 100); // each individual has a 50% chance of suffering a mutation
			if (x < 50) {
				os.at(0) = mutation(os.at(0));
			}
			x = Random::get(0, 100);
			if (x < 50) {
				os.at(1) = mutation(os.at(1));
			}

			offspring.push_back(permutationToSolution(os.at(0)));
			count++;
			if (count == max) {
				end = true;
				break;
			}

			offspring.push_back(permutationToSolution(os.at(1)));
			count++;
			if (count == max) {
				end = true;
				break;
			}
		}
		if (end = true) {
			break;
		}
	}
	int exr = count;

	// generating rxr offspring
	max = pop.size() - (exe + exr); // the number of exr individuals is 30% of the population size plus the remaining amount of individuals from the exe that couldnt be generated
	count = 0;
	end = false;
	for (int i = eliteSize; i < pop.size() - 1; i++) {
		for (int j = eliteSize + 1; j < pop.size(); j++) {
			vector<permutation> os = crossover(pop.at(i).perm, pop.at(j).perm);

			// mutation
			int x = Random::get(0, 100); // each individual has a 50% chance of suffering a mutation
			if (x < 50) {
				os.at(0) = mutation(os.at(0));
			}
			x = Random::get(0, 100);
			if (x < 50) {
				os.at(1) = mutation(os.at(1));
			}

			offspring.push_back(permutationToSolution(os.at(0)));
			count++;
			if (count == max) {
				end = true;
				break;
			}

			offspring.push_back(permutationToSolution(os.at(1)));
			count++;
			if (count == max) {
				end = true;
				break;
			}
		}
		if (end = true) {
			break;
		}
	}

	return offspring;
}

vector<permutation> perm_rep::crossover(permutation p1, permutation p2)
{
	int n = floor(p1.size() / 2);

	permutation newP1, newP2;

	for (int i = 0; i < n; i++) {
		newP1.push_back(p1.at(i));
	}
	for (int i = 0; i < n; i++) {
		newP2.push_back(p2.at(i));
	}

	// new p1
	bool more = true;
	for (int i = 0; i < p2.size(); i++) {
		bool isin = false;
		for (int j = 0; j < newP1.size(); j++) {
			if (p2.at(i) == newP1.at(j)) {
				isin = true;
				break;
			}
		}
		if (isin == false)
			newP1.push_back(p2.at(i));
	}
	// new p2
	more = true;
	for (int i = 0; i < p1.size(); i++) {
		bool isin = false;
		for (int j = 0; j < newP2.size(); j++) {
			if (p1.at(i) == newP2.at(j)) {
				isin = true;
				break;
			}
		}
		if (isin == false)
			newP2.push_back(p1.at(i));
	}

	vector<permutation> p;
	p.push_back(newP1);
	p.push_back(newP2);

	return p;
}

permutation perm_rep::mutation(permutation p)
{
	// 2 swap
	int mid = floor(p.size() / 2);
	int end = p.size() - 1;

	int n1 = Random::get(0, end);
	int n2 = Random::get(0, end);
	while (n2 == n1) {
		n2 = Random::get(0, end);
	}

	int aux = p.at(n1);
	p.at(n1) = p.at(n2);
	p.at(n2) = aux;

	return p;
}

vector<float> perm_rep::calcFitness(vector<solution> pop)
{
	vector<float> fitness;

	// get the individual with the lowest OF
	solution best = pop.front();
	for (solution s : pop) {
		if (s.FO < best.FO) {
			best = s;
		}
	}

	// based on the lowest OF individual we calculate the population fitness
	for (solution s : pop) {
		fitness.push_back(best.FO / s.FO);
	}

	return fitness;
}

solution perm_rep::sA(int initTemp, int finalTemp, float coolingRate, int maxIt, int maxRuntime)
{
	cout << "SA\n";

	int t0 = initTemp;
	int tf = finalTemp;

	solution curr = greedDD();
	int FOINIT = curr.FO;

	//permutation p = { 11, 7, 5, 10, 8, 6, 9 }; // very bad permutation for UK 12 instance
	//permutation p = { 23, 13, 10, 17, 26, 14, 24, 22, 21, 18, 15, 16, 11, 8, 19, 20, 12, 25, 9 }; // bad permutation for uk 75 03

	solution best = curr;
	int temp = t0;

	cout << "initial solution: " << best.FO << endl;

	auto t1 = std::chrono::high_resolution_clock::now();
	long long duration;

	do {
		bool improv = false;
		int counter = 0;
		cout << "temp: " << temp << endl;
		do {
			// cout << "c: " << counter << endl;
			// select a neighbor from the current solution
			permutation pl = curr.perm;
			int beg = Random::get(0, int(pl.size()) - 2);
			int end = Random::get(beg + 1, int(pl.size()) - 1);
			pl = opt2(pl, beg, end);
			solution nSol = localSearch(pl);

			// store the best solution so far
			if (nSol.FO < best.FO) {
				best = nSol;
				counter = 0;
				cout << "Improvment\n";
				cout << "FO: " << best.FO << endl;
			}

			// search-space exploration
			//cout << nSol.FO << " - " << curr.FO << endl;
			double delta = (nSol.FO - curr.FO); //
			if (delta < 0.0) { // accept improvment solution
				curr = nSol;
				counter++;
			}
			else { // 
				int a = -delta / temp;
				double prob = pow(EulerConstant, a);

				double value = Random::get(0.0, 1.0);

				if (value < prob) {
					curr = nSol;
				}

				counter++;
			}

			// measure runtime
			auto t2 = std::chrono::high_resolution_clock::now();
			long long duration2 = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();

			// stop if max runtime is reached
			if (duration2 > maxRuntime) {
				break;
			}

		} while (counter <= maxIt); // number of iterations without improvments

		temp -= coolingRate; // cooling

		// measure runtime
		auto t2 = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
		cout << "duration: " << duration << endl;

	} while (temp > tf && duration < maxRuntime);


	auto t2 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
	best.time = duration;
	best.FOINIT = FOINIT;

	if (best.inf.size() == 0) {
		best.status = 1;
	}
	else {
		best.status = -1;
	}

	return best;
}

solution perm_rep::greedDD()
{
	permutation p;

	vector<node> C = inst->set_C(); // get the set of all customers
	sort(C.begin(), C.end(), compDD);

	for (node c : C) {
		p.push_back(c.key);
	}

	solution s = permutationToSolution(p);

	return s;
}

solution perm_rep::greedRT()
{
	permutation p;

	vector<node> C = inst->set_C(); // get the set of all customers
	sort(C.begin(), C.end(), compRT);

	for (node c : C) {
		p.push_back(c.key);
	}

	solution s = permutationToSolution(p);

	return s;
}

int perm_rep::test2()
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

int perm_rep::test3()
{

	permutation p1 = randomPermutation();
	permutation p2 = randomPermutation();

	for (auto i : p1) {
		cout << i << " ";
	}
	cout << endl;

	for (auto i : p2) {
		cout << i << " ";
	}
	cout << endl;

	vector<permutation> p = crossover(p1, p2);

	for (auto i : p.at(0)) {
		cout << i << " ";
	}
	cout << endl;

	for (auto i : p.at(1)) {
		cout << i << " ";
	}
	cout << endl;

	return 0;
}

int perm_rep::test4()
{

	solution s = sA(1000, 100, 100, 50, 300);
	//solution s = GA(100, 10, 50);

	for (auto r : s.routes) {
		for (auto v : r) {
			cout << v.key << " ";
		}
		cout << endl;
	}

	cout << "FO: " << s.FO << endl;
	for (auto i : s.inf) {
		cout << i << " ";
	}
	cout << endl;

	return 0;
}

int perm_rep::test5()
{
	permutation p;// = randomPermutation();

	//p = { 11, 10, 6, 5, 7, 9, 8 };
	p = { 5, 11, 10, 8, 6, 7, 9 };

	solution s1 = permutationToSolution(p);

	cout << "FO: " << s1.FO << endl;
	for (auto r : s1.routes) {
		for (auto v : r) {
			cout << v.key << " ";
		}
		cout << endl;
	}
	for (auto i : s1.inf) {
		cout << i << " ";
	}
	cout << endl;

	solution s2 = permutationToSolutionGrasp(p);

	cout << "FO: " << s2.FO << endl;
	for (auto r : s2.routes) {
		for (auto v : r) {
			cout << v.key << " ";
		}
		cout << endl;
	}
	for (auto i : s2.inf) {
		cout << i << " ";
	}
	cout << endl;

	return 0;
}