#include "perm_rep.h"

Solution perm_rep::permutationToSolution(permutation p)
{
	vector<bool> coverage(inst->nodes.size(), false);

	Solution s;
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

			if (n.demand + weight.at(j) <= inst->c && time <= n.dueDate) { // check the route total freight and stipulated time windows

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

	Solution best;
	best.FO = INT_MAX;
	bool improv = false;
	for (auto n : inst->set_UD0()) {
		Solution curr = addDepotsN(s, n.key);

		try {
			curr = addStations(curr);
			curr = procSol(curr); // error

			if (curr.FO < best.FO && curr.inf.size() == 0) {
				best = curr;
				improv = true;
			}
		}
		catch (IsolatedNode& e) {
			cout << e.what() << endl;
		}
		catch (PermutationInf& e) {
			//cout << e.what() << endl;
			throw PermutationInf(s.perm);
		}
		catch (exception &e) {
			cout << e.what() << endl;
			throw PermutationInf(s.perm);
		}
	}
	if (improv == false) {
		for (auto i : inst->set_R()) {
			cout << i.key << " ";
		}
		cout << endl;
		throw "infeasible_solution";
	}

	return best;
}

Solution perm_rep::permutationToSolution_()
{
	Solution best;
	best.FO = INT_MAX;
	bool improv = false;
	for (auto n : inst->set_UD0()) {		
		Solution curr;

		vector<node> C = inst->set_C(); // get the set of all customers

		// create initial route route
		route r;

		// initial depot
		vertex dpt;
		dpt.key = n.key;
		dpt.aTime = 0;
		dpt.wTime = 0;
		dpt.lTime = 0;
		dpt.recharge = false;
		dpt.recharged = 0;
		dpt.bLevel = inst->Q;
		dpt.vLoad = inst->c;
		r.push_back(dpt);

		curr.routes.push_back(r);

		while (C.size() > 0) {
			
			bool insert = false;
			for (route& rt : curr.routes) {

				vertex v;
				int minBU;

				// search for the next node
				minBU = 9999999;
				for (node c : C) {

					node nodeC = inst->getNodeByKey(c.key);
					int BU = inst->getBatteryUsed(rt.back().key, c.key); // battery used
					int TT = inst->getTD(rt.back().key, c.key); // travel time		

					// compute node c relatively to last node in the current route
					vertex v_;
					v_.key = c.key;
					v_.aTime = rt.back().lTime + TT; // arrival time

					if (v_.aTime < nodeC.readyTime) { // waiting time
						v_.wTime = nodeC.readyTime - v_.aTime;
					}
					else {
						v_.wTime = 0;
					}
					v_.lTime = v_.aTime + v_.wTime + nodeC.serviceTime; // departure time
					v_.vLoad = rt.back().vLoad - nodeC.demand; // vehicle load
					v_.bLevel = rt.back().bLevel - BU; // battery level
					v_.recharged = nodeC.serviceTime * inst->g; // amount recharged
					if (v_.bLevel + v_.recharged > inst->Q) {
						v_.recharged = inst->Q - v_.bLevel;
					}

					if (BU < minBU && BU != 0) {
						minBU = BU;
						v = v_;
					}

					/*
					if (v.aTime + v.wTime >= nodeC.readyTime && v.lTime < nodeC.dueDate) { // check battery level and arrival time
						rt.push_back(v);
					}
					*/
				}

				node nodeV = inst->getNodeByKey(v.key);
				if (v.vLoad >= 0 && v.aTime < nodeV.dueDate) { // check battery level and arrival time
					rt.push_back(v);
					insert = true;

					// remove the city from the C vector
					vector<node> C_;
					for (node aux : C) {
						if (aux.key != v.key) {
							C_.push_back(aux);
						}
					}
					C = C_;
				}
				/*
				else { // create new route
					insert = false;
					break;
				}
				*/
			}

			if (insert == false) {
				route rt_;
				rt_.push_back(dpt);
				curr.routes.push_back(rt_);
				
			}
		}

		// add arrival node
		dpt.key = inst->getArrival(dpt.key);
		for (route& rt : curr.routes) {
			rt.push_back(dpt);
		}

		try {
			curr = addStations(curr);
			curr = procSol(curr); // error

			if (curr.FO < best.FO && curr.inf.size() == 0) {
				best = curr;
				improv = true;
			}
		}
		catch (IsolatedNode& e) {
			cout << e.what() << endl;
		}
		
	}
	if (improv == false) {
		for (auto i : inst->set_R()) {
			cout << i.key << " ";
		}
		cout << endl;
		throw "infeasible_solution";
	}

	return best;
}

Solution perm_rep::permutationToSolution(permutation p, int cn)
{
	vector<bool> coverage(inst->nodes.size(), false);

	Solution s;
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

			if (n.demand + weight.at(j) <= inst->c && time <= n.dueDate) { // check the route total freight and stipulated time windows

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
		s = GRASP_p(s, 10, INT_MAX);
	}
	catch (IsolatedNode& e) {
		cout << e.what() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
		throw PermutationInf(s.perm);
	}

	s = procSol(s);

	return s;

	/*
	vector<bool> coverage(inst->nodes.size(), false);

	Solution s;
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
	*/
}

Solution perm_rep::permutationToSolutionAlt(permutation p)
{
	vector<bool> coverage(inst->nodes.size(), false);

	Solution s;
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

			if (n.demand + weight.at(j) <= inst->c && time <= n.dueDate) { // check the route total freight and stipulated time windows

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

	///
	Solution best;
	best.FO = INT_MAX;
	for (auto n : inst->set_UD0()) {
		Solution curr = addDepotsN(s, n.key);

		try {
			//curr = GRASP_p(curr);
			curr = addStationsAlt(curr);

			curr = procSol(curr);

			if (curr.FO < best.FO && curr.inf.size() == 0) {
				best = curr;
			}
		}
		catch (IsolatedNode& e) {
			cout << e.what() << endl;
		}
		catch (exception e) {
			cout << e.what() << endl;
			throw PermutationInf(s.perm);
		}
	}

	return best;
}

Solution perm_rep::permutationToSolutionGrasp(permutation p)
{
	vector<bool> coverage(inst->nodes.size(), false);

	Solution s;
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

			if (n.demand + weight.at(j) <= inst->c && time <= n.dueDate) { // check the route total freight and stipulated time windows

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

	vector<pair<int, float>> dists;
	for (node d : inst->set_UD0()) {
		float dist = 0;
		for (route r : s.routes) {	
			float a = inst->dist(d, inst->getNodeByKey(r.front().key));
			float b = inst->dist(inst->getNodeByKey(r.front().key), d);
			dist += (a + b) / 2.0;
		}
		dists.push_back(pair<int, float>(d.key, dist));
	}
	sort(dists.begin(), dists.end());

	try {
		s = addDepotsN(s, dists.front().first);
		s = GRASP_p(s, 10, 30);
		//curr = addStationsAlt(curr);
		//curr = procSol(curr);
	}
	catch (IsolatedNode& e) {
		cout << e.what() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
		throw PermutationInf(s.perm);
	}
	return s;

	/*
	Solution best;
	best.FO = INT_MAX;
	for (auto n : inst->set_UD0()) {
		Solution curr = addDepotsN(s, n.key);

		try {
			curr = GRASP_p(curr);
			//curr = addStationsAlt(curr);

			//curr = procSol(curr);

			if (curr.FO < best.FO && curr.inf.size() == 0) {
				best = curr;
			}
		}
		catch (IsolatedNode& e) {
			cout << e.what() << endl;
		}
		catch (exception e) {
			cout << e.what() << endl;
			throw PermutationInf(s.perm);
		}
	}
	return best;
	*/
	
}

Solution perm_rep::choosePermutationToSolution(permutation p, int n)
{
	if (n == 1) {
		return permutationToSolution(p);
	}
	else if (n == 2){
		return permutationToSolutionGrasp(p);
	}
}

Solution perm_rep::addDepots(Solution s)
{
	//vector<vector<int>> dm; // matrix dm (distance matrix) where dm[i][j] (i = depot and j = route)
	// this matrix will store for each route x depot combination the sum of the distances depot -> initial node and the last node -> depot

	vector<node> UD0 = inst->set_UD0();
	vector<node> UD1 = inst->set_UD1();

	//
	int w = 0;
	int qt = 0;
	vector<int> added;
	for (route r : s.routes) {

		vector<int> m;

		int lowestMean = INT_MAX;
		vector<pair<node, int>> dpts;
		node n;

		for (int i = 0; i < UD0.size(); i++) {

			int d1 = inst->dist(UD0.at(i), inst->getNodeByKey(r.front().key));
			int d2 = inst->dist(inst->getNodeByKey(r.back().key), UD1.at(i));

			dpts.push_back({ UD0.at(i) ,ceil((d1 + d2) / 2) });
			/*
			m.push_back(ceil((d1 + d2) / 2));

			if (m.at(i) < lowestMean) {
				lowestMean = m.at(i);
				n = UD0.at(i);
			}
			*/
		}

		sort(dpts.begin(), dpts.end(), [](pair<node, int > dpt1, pair<node, int> dpt2) -> bool {return dpt1.second < dpt2.second; });

		if (qt >= inst->maxD) { // if we already have the maximum amount of depots choosen
			for (auto i : dpts) {
				bool f = false;
				for (auto j : added) {
					if (i.first.key == j) {
						n = i.first;
						f = true;
						break;
					}
				}
				if (f == true) {
					break;
				}
			}
		}
		else {
			n = dpts.front().first;
			qt++;
		}

		// insert the depot
		vertex v;
		v.key = n.key;
		r.insert(r.begin(), v);
		added.push_back(n.key);
		// int id = n.id_n;

		// insert the arrival depot node
		int a = -1;
		for (node no : UD1) {
			if (n.id_n == no.id_n) { // search the arrival node
				a = no.key;
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
		s.routes.at(i).front().vLoad = inst->c;
		s.routes.at(i).front().aTime = 0;
		s.routes.at(i).front().wTime = 0;
	}

	set<int> st;
	for (auto r : s.routes) {
		st.insert(r.front().key);
	}

	return s;
}

Solution perm_rep::addDepotsN(Solution s, int n)
{
	//vector<vector<int>> dm; // matrix dm (distance matrix) where dm[i][j] (i = depot and j = route)
// this matrix will store for each route x depot combination the sum of the distances depot -> initial node and the last node -> depot

	node d = inst->getNodeByKey(n);
	node a = inst->getNodeByKey(inst->getArrival(d.key));

	for (route& r : s.routes) {
		// insert the depot
		vertex v;
		v.key = d.key;
		r.insert(r.begin(), v);

		// insert the arrival depot node
		v.key = a.key;
		r.push_back(v);

	}

	for (int i = 0; i < s.routes.size(); i++) {
		s.routes.at(i).front().bLevel = inst->Q;
		s.routes.at(i).front().lTime = 0;
		s.routes.at(i).front().vLoad = inst->c;
		s.routes.at(i).front().aTime = 0;
		s.routes.at(i).front().wTime = 0;
	}

	return s;
}

Solution perm_rep::addStations_(Solution s)
{
	// set all vehicles to full energy
	for (int j = 0; j < s.routes.size(); j++) {
		s.routes.at(j).front().bLevel = inst->Q;
	}

	int last = 0;
	vector<int> bssSequence;

	for (int j = 0; j < s.routes.size(); j++) {
		int curr = 1;

		while (curr < s.routes.at(j).size()) {
			if (curr == 0) {
				throw "error";
			}
			int prev = curr - 1;
			node currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
			node prevNode = inst->getNodeByKey(s.routes.at(j).at(prev).key);

			vertex &cVertex = s.routes.at(j).at(curr);
			vertex pVertex = s.routes.at(j).at(prev);

			// computing parameters

			// compute battery level in the current node
			float bUsed = inst->getBatteryUsed(s.routes.at(j).at(prev).key, s.routes.at(j).at(curr).key);

			cVertex.bLevel = pVertex.bLevel + pVertex.recharged - bUsed;

			cVertex.recharged = 0;
			cVertex.recharge = false;

			// compute arrival time in the current node
			float TD = inst->getTD(prevNode.key, currNode.key);
			cVertex.aTime = pVertex.lTime + TD;
			//s.routes.at(j).at(curr).aTime = s.routes.at(j).at(prev).aTime + prevNode.serviceTime + inst->getTD(prevNode.key, currNode.key);

			// compute waiting time in the current node
			if (cVertex.aTime < currNode.readyTime) {
				cVertex.wTime = currNode.readyTime - cVertex.aTime;
			}
			else { // no waiting time
				cVertex.wTime = 0;
			}

			// compute de departure time in the current node
			cVertex.lTime = cVertex.aTime + cVertex.wTime + currNode.serviceTime;

			// compute vehcile load in the current node
			cVertex.vLoad = pVertex.vLoad - currNode.demand;

			// checking due date and vehicle load
			if (s.routes.at(j).at(curr).aTime > currNode.dueDate || s.routes.at(j).at(curr).vLoad < 0) { ////////////////////////////////////////////////
				// if the current node is reached after the due date, we will remove this node from the route and push it to the next one

				vertex v = s.routes.at(j).at(curr); // store the node key that will be removed				
				s.routes.at(j).erase(s.routes.at(j).begin() + curr); // remove the node from the route

				// checking if we are in the last route
				if (j < s.routes.size() - 1) { // not last route
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
					dpt.vLoad = inst->c;
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

				// remove any bss sited in order to this node to be reached and discharge battery in previous nodes
				currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
				while (currNode.type == "f" || currNode.type == "f_d") {
					s.routes.at(j).erase(s.routes.at(j).begin() + curr);
					
					curr--;
					
					currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
				}

				//
				currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
				int curr_ = curr;

				// here i am supposed to remove the the rechaged battery in the case of bss removal
				// i will loop up to this later
				/*
				for (int i = last; i < curr_; i++) {
					prevNode = inst->getNodeByKey(s.routes.at(j).at(curr_ - 1).key);
					if (prevNode.type != "f" && prevNode.type != "f_d" && prevNode.type != "d") {
						s.routes.at(j).at(curr_).bLevel -= s.routes.at(j).at(curr_ - 1).recharged; ////////////////////////
					}					
					s.routes.at(j).at(curr_).recharge = false;
					s.routes.at(j).at(curr_).recharged = 0;
					curr_--;

					currNode = inst->getNodeByKey(s.routes.at(j).at(curr_).key);
				}

				
				while (currNode.type != "f" && currNode.type != "f_d" && currNode.type != "d") {
					prevNode = inst->getNodeByKey(s.routes.at(j).at(curr_ - 1).key);
					if (prevNode.type != "f" && prevNode.type != "f_d" && prevNode.type != "d") {
						s.routes.at(j).at(curr_).bLevel -= s.routes.at(j).at(curr_ - 1).recharged; ////////////////////////
					}
					s.routes.at(j).at(curr_).recharge = false;
					s.routes.at(j).at(curr_).recharged = 0;
					curr_--;

					currNode = inst->getNodeByKey(s.routes.at(j).at(curr_).key);
				}*/

			}
			// checking battery level
			else if (s.routes.at(j).at(curr).bLevel < 0) {
				// if not enough battery, we have to insert a bss in the route

				int minBU = 0; // minimum battery usage
				int meanBU = 0;

				// try to put a bss 
				while (s.routes.at(j).at(curr).bLevel - minBU < 0) {
					int put = false;
					curr--;
					minBU = INT_MAX;

					currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
					int bss;
					
					// special case
					// if the route is traced back to the depot due to a node 
					if (currNode.type == "d") {
						vector<node> R = inst->set_R(); // get all stations

						// search for the closest bss from the stop node
						for (node r : R) {

							int BU1 = inst->getBatteryUsed(s.routes.at(j).at(curr).key, r.key);
							int BU2 = inst->getBatteryUsed(r.key, s.routes.at(j).at(curr + 1).key);
							
							// get the lowest dist
							if (BU2 < minBU && BU1 < s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged) {
								bss = r.key;
								minBU = BU2;
							}
						}
					}
					// search for the closest bss
					else if (currNode.type != "f") {
						//vector<node> R = inst->set_R(); // get all stations
						
						// search for the closest bss from the stop node
						pair<int, int> ret = closestBSS(s, j, curr);
						bss = ret.first;
						minBU = ret.second;

						/*
						for (node r : R) {

							int BU = inst->getBatteryUsed(s.routes.at(j).at(curr).key, r.key); // inst->dist(r.key, b);

							// get the lowest dist
							if (BU < minBU && r.key != s.routes.at(j).at(curr).key) {
								bss = r.key;
								minBU = BU;
							}
						}
						*/
					}
					// if the current node is a bss we have to watch out for a possible loop of bss
					// so we have to make sure it wont happen 
					else if (currNode.type == "f") {
						vector<node> R = inst->set_R(); // get all stations
						
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
						pair<int, int> ret = closestBSS(s, j, curr);
						bss = ret.first;
						minBU = ret.second;

						/*
						for (node r : R) {
							int BU = inst->getBatteryUsed(s.routes.at(j).at(curr).key, r.key); // inst->dist(r.key, b);

							// get the lowest dist
							if (BU < minBU && r.key != s.routes.at(j).at(curr).key) {
								bss = r.key;
								minBU = BU;
							}
						}
						*/
					}

					// compute the amount of energy the vehcile can recharge during service time in the current routes segment
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

					if ((s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged + energy) - minBU >= 0) { // considering recharge during service time
						// recharge during service time
						curr_ = curr;
						energy = 0;
						auto a = inst->getBatteryUsed(s.routes.at(j).at(curr).key, bss);
						// find the last customers before the lastest depot or customer
						while (true) {
							node n = inst->getNodeByKey(s.routes.at(j).at(curr_).key);
							energy += inst->g * n.serviceTime;

							// force to recharge just enough to reach the bss
							if (n.type == "f" || n.type == "d" || n.type == "f_d" || s.routes.at(j).at(curr).bLevel + energy > minBU) {
								break;
							}
							last = curr_;
							curr_--;
						}
						// recompute battery parameters
						int accumulated = 0;
						// compute first node
						node n = inst->getNodeByKey(s.routes.at(j).at(curr_).key);

						s.routes.at(j).at(curr_).recharge = true;
						s.routes.at(j).at(curr_).recharged = inst->g * n.serviceTime;
						accumulated += s.routes.at(j).at(curr_).recharged;
						// in the case of the battery level after partial recharging became overcharged
						if (s.routes.at(j).at(curr_).bLevel + s.routes.at(j).at(curr_).recharged > inst->Q) {
							s.routes.at(j).at(curr_).recharged = inst->Q - s.routes.at(j).at(curr_).bLevel;
						}

						// compute the rest
						for (int i = curr_ + 1; i <= curr; i++) {
							node n = inst->getNodeByKey(s.routes.at(j).at(i).key);

							//s.routes.at(j).at(i).bLevel += s.routes.at(j).at(i - 1).recharged;

							s.routes.at(j).at(i).recharge = true;
							s.routes.at(j).at(i).recharged = inst->g * n.serviceTime;

							s.routes.at(j).at(i).bLevel += accumulated;
							accumulated += s.routes.at(j).at(i).recharged;

							// in the case of the battery level after partial recharging became overcharged
							if (s.routes.at(j).at(i).bLevel + s.routes.at(j).at(i).recharged > inst->Q) {
								s.routes.at(j).at(i).recharged = inst->Q - s.routes.at(j).at(i).bLevel;
							}

						}

						prevNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);

						// insert bss vertex in the route

						//curr++;
						//put = true;
					}
					else if ((s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged + energy) - minBU >= 0) { // considering recharge during service time
						// recharge during service time
						curr_ = curr;
						energy = 0;
						auto a = inst->getBatteryUsed(s.routes.at(j).at(curr).key, bss);
						// find the last customers before the lastest depot or customer
						while (true) {
							node n = inst->getNodeByKey(s.routes.at(j).at(curr_).key);
							energy += inst->g * n.serviceTime;

							// force to recharge just enough to reach the bss
							if (n.type == "f" || n.type == "d" || n.type == "f_d" || s.routes.at(j).at(curr).bLevel + energy > minBU) {
								break;
							}
							last = curr_;
							curr_--;
						}
						// recompute battery parameters
						int accumulated = 0;
						// compute first node
						node n = inst->getNodeByKey(s.routes.at(j).at(curr_).key);

						s.routes.at(j).at(curr_).recharge = true;
						s.routes.at(j).at(curr_).recharged = inst->g * n.serviceTime;
						accumulated += s.routes.at(j).at(curr_).recharged;
						// in the case of the battery level after partial recharging became overcharged
						if (s.routes.at(j).at(curr_).bLevel + s.routes.at(j).at(curr_).recharged > inst->Q) {
							s.routes.at(j).at(curr_).recharged = inst->Q - s.routes.at(j).at(curr_).bLevel;
						}

						// compute the rest
						for (int i = curr_ + 1; i <= curr; i++) {
							node n = inst->getNodeByKey(s.routes.at(j).at(i).key);

							//s.routes.at(j).at(i).bLevel += s.routes.at(j).at(i - 1).recharged;
							
							s.routes.at(j).at(i).recharge = true;
							s.routes.at(j).at(i).recharged = inst->g * n.serviceTime;

							s.routes.at(j).at(i).bLevel += accumulated;
							accumulated += s.routes.at(j).at(i).recharged;

							// in the case of the battery level after partial recharging became overcharged
							if (s.routes.at(j).at(i).bLevel + s.routes.at(j).at(i).recharged > inst->Q) {
								s.routes.at(j).at(i).recharged = inst->Q - s.routes.at(j).at(i).bLevel;
							}

						}

						prevNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);

						// insert bss vertex in the route
						vertex v;
						v.key = bss;

						v.vLoad = s.routes.at(j).at(prev).vLoad; // compute vehicle load						
						
						v.aTime = s.routes.at(j).at(prev).lTime + inst->getTD(prevNode.key, bss); // compute arrival time
						
						v.lTime = v.aTime + inst->ct; // compute departure time			

						auto bUsed = inst->getBatteryUsed(s.routes.at(j).at(prev).key, v.key);
						v.bLevel = s.routes.at(j).at(prev).bLevel + s.routes.at(j).at(curr).recharged - bUsed; // compute battery level
		
						v.recharged = inst->Q - v.bLevel; // compute the amount of energy recharged

						v.recharge = true;

						s.routes.at(j).insert(s.routes.at(j).begin() + curr + 1, v); // insert in the routes

						curr++;
						put = true;
					}
					else if (s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged - minBU >= 0) { // if battery is enough to go from current node and closest bss, insert bss in the route
						vertex v;
						v.key = bss;

						s.routes.at(j).insert(s.routes.at(j).begin() + curr + 1, v); // insert in the routes

						currNode = inst->getNodeByKey(s.routes.at(j).at(curr + 1).key);

						s.routes.at(j).at(curr + 1).vLoad = s.routes.at(j).at(curr).vLoad; // compute vehicle load

						prevNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
						s.routes.at(j).at(curr + 1).aTime = s.routes.at(j).at(curr).lTime + inst->getTD(prevNode.key, bss); // compute arrival time
						s.routes.at(j).at(curr + 1).lTime = s.routes.at(j).at(curr + 1).aTime + currNode.serviceTime + inst->ct; // compute departure time

						// s.routes.at(j).at(curr + 1).bLevel = inst->Q;
						auto BU = inst->getBatteryUsed(s.routes.at(j).at(curr).key, s.routes.at(j).at(curr + 1).key);
						s.routes.at(j).at(curr + 1).bLevel = s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged - BU; // compute battery level
						// inst->getBatteryUsed(s.routes.at(j).at(curr).key, s.routes.at(j).at(curr + 1).key) = minBU

						int bLevel = s.routes.at(j).at(curr + 1).bLevel;
						s.routes.at(j).at(curr + 1).recharged = inst->Q - bLevel; // compute the amount of energy recharged
						// inst->Q - 
						s.routes.at(j).at(curr + 1).recharge = true;

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

Solution perm_rep::addStations(Solution s)
{
	/*
	for (auto i : s.perm) {
		cout << i << " ";
	}
	cout << endl;*/

	// set all vehicles to full energy
	for (int j = 0; j < s.routes.size(); j++) {
		s.routes.at(j).front().bLevel = inst->Q;
	}

	int last = 0;
	vector<int> bssSequence;

	for (int j = 0; j < s.routes.size(); j++) {
		int curr = 1;		
		try {
			try {
				s.routes.at(j) = addStations(s.routes.at(j));
			}
			catch (SplitRoute& e) { // if the route has an adjacent pair of nodes that cant be linked by the availabes BSSs we will split the route in those two points
				throw PermutationInf(s.perm);
				/*
				route r1, r2;

				for (int i = 0; i <= e.getPoint(); i++) {
					r1.push_back(s.routes.at(j).at(i));					
				}

				r2.push_back(r1.front()); // insert arrival node
				for (int i = e.getPoint() + 1; i < s.routes.at(j).size(); i++) {
					r2.push_back(s.routes.at(j).at(i));
				}

				r1.push_back(r2.back()); // insert departure node

				s.routes.at(j) = r1;
				s.routes.push_back(r2);

				//j--;

				continue;
				*/
			}			
			
			while (inst->getNodeByKey(s.routes.at(j).back().key).type != "a") {
				vertex v = s.routes.at(j).back();
				s.routes.at(j).erase(s.routes.at(j).end() - 1);

				if (j == s.routes.size() - 1) { // create a new route
					route r;
					r.push_back(s.routes.at(j).front());

					r.push_back(v);

					int i = s.routes.at(j).size() - 1;
					while (inst->getNodeByKey(s.routes.at(j).at(i).key).type != "a") {
						i--;
					}

					r.push_back(s.routes.at(j).at(i));

					s.routes.push_back(r);
				}
				else {
					s.routes.at(j + 1).insert(s.routes.at(j + 1).begin() + 1, v);
				}
			}
		}
		catch (exception &e) {
			//cout << e.what() << endl;
			return s;
			//throw PermutationInf(s.perm);
		}

	}
	//cout << "end" << endl;
	return s;
}

route perm_rep::addStations(route rt)
{
	vector<vertex> temp;
	int last = 0;
	vector<int> bssSequence;

	// initialize the first node
	rt.front().bLevel = inst->Q;

	int curr = 1;
	while (curr < rt.size()) {
		if (curr == 0) {
			throw "error";
		}
		int prev = curr - 1;
		node currNode = inst->getNodeByKey(rt.at(curr).key);
		node prevNode = inst->getNodeByKey(rt.at(prev).key);

		vertex& cVertex = rt.at(curr);
		vertex pVertex = rt.at(prev);

		// computing parameters

		// compute battery level in the current node
		float bUsed = inst->getBatteryUsed(rt.at(prev).key, rt.at(curr).key);
		//cout << prev << " - " << curr << " - " << bUsed << endl;
		cVertex.bLevel = pVertex.bLevel + pVertex.recharged - bUsed;

		if (cVertex.bLevel > inst->Q) {
			cout << cVertex.bLevel << endl;
		}

		if (currNode.type == "f" || currNode.type == "f_d") { // recharge in the bss
			cVertex.recharged = inst->Q - cVertex.bLevel;
			cVertex.recharge = true;
		}
		else { // recharge in the customer
			int recharge = inst->g * currNode.serviceTime;
			if (cVertex.bLevel + recharge > inst->Q) {
				cVertex.recharged = inst->Q - cVertex.bLevel;
				cVertex.recharge = true;
			}
			else {
				cVertex.recharged = recharge;
				cVertex.recharge = true;
			}
			
		}

		// compute arrival time in the current node
		float TD = inst->getTD(prevNode.key, currNode.key);
		cVertex.aTime = pVertex.lTime + TD;

		// compute waiting time in the current node
		if (cVertex.aTime < currNode.readyTime) {
			cVertex.wTime = currNode.readyTime - cVertex.aTime;
		}
		else { // no waiting time
			cVertex.wTime = 0;
		}

		// compute de departure time in the current node
		if (currNode.type == "f" || currNode.type == "f_d") {
			cVertex.lTime = cVertex.aTime + cVertex.wTime + inst->ct;
		}
		else {
			cVertex.lTime = cVertex.aTime + cVertex.wTime + currNode.serviceTime;
		}

		// compute vehcile load in the current node
		cVertex.vLoad = pVertex.vLoad - currNode.demand;

		if (currNode.type == "a" && rt.at(curr).bLevel >= 0) {
			break;
		}

		if (rt.at(curr).aTime > currNode.dueDate || rt.at(curr).vLoad < 0) { // if a vertex is infeasible to the time window constraint, push it to the end of the route
			if (currNode.type == "a") {
				curr--;

				while (inst->getNodeByKey(rt.at(curr).key).type == "f") {
					rt.erase(rt.begin() + curr);
					curr--;
				}

				vertex v = rt.at(curr);
				rt.erase(rt.begin() + curr);
				temp.push_back(v);
				
			}
			else {
				vertex v = rt.at(curr);
				rt.erase(rt.begin() + curr);
				temp.push_back(v);
				curr--;
			}			
		}
		//int bLevel = rt.at(curr).bLevel + rt.at(curr).recharged;
		if (rt.at(curr).bLevel < 0) {
			// if not enough battery, we have to insert a bss in the route

			int minBU = 0; // minimum battery usage
			int meanBU = 0;

			// try to recharge the battery

			// try to put a bss 
			bool placed = false;
			//while (rt.at(curr).bLevel - minBU < 0) {
			while (placed == false) {
				placed = true;
				int put = false;
				curr--;

				if (inst->getNodeByKey(rt.at(curr).key).type == "f" && inst->getNodeByKey(rt.at(curr + 1).key).type == "f") {
					throw IsolatedNode(rt.at(rt.size() - 2).key);
				}

				minBU = INT_MAX;

				currNode = inst->getNodeByKey(rt.at(curr).key);
				int bss;

				pair<int, int> ret;
				try {
					ret = chooseBSS(rt, curr, curr + 1);
				}
				catch (OutOfBSS& e) {
					cout << e.what() << endl;
					throw e;
				}
				catch (Unreachable& e) {
					placed = false;
					//throw e; ///////////////////
					continue;
				}
				catch (exception& e) {
					//cout << e.what() << endl;
				}

				bss = ret.first;
				minBU = ret.second;

				// insert bss
				vertex v;
				v.key = bss;
				rt.insert(rt.begin() + curr + 1, v);

				if (put == true) {
					break;
				}
			}
		}
		curr++;
	}

	// optmize battery	
	for (int i = 0; i < rt.size(); i++) {
		node n = inst->getNodeByKey(rt.at(i).key);

		if (n.type == "f" || n.type == "a") {
			int bLevel = rt.at(i).bLevel;

			int j = i - 1;
			while (inst->getNodeByKey(rt.at(j).key).type == "c" && bLevel > 0) {

				if (rt.at(j).recharged > 0) {
					if (rt.at(j).recharged >= bLevel) {
						rt.at(j).recharged -= bLevel;
						bLevel = 0;
					}
					else if (rt.at(j).recharged < bLevel) {
						rt.at(j).recharge = false;
						bLevel -= rt.at(j).recharged;
						rt.at(j).recharged = 0;
					}

				}
				j--;
			}

			rt.at(i).bLevel = bLevel;
		}
	}
	

	rt.insert(rt.end(), temp.begin(), temp.end());

	return rt;
}

route perm_rep::addStations_OLD(route rt)
{

	int last = 0;
	vector<int> bssSequence;

	// initialize the first node
	rt.front().bLevel = inst->Q;

	int curr = 1;
	while (curr < rt.size()) {
		if (curr == 0) {
			throw "error";
		}
		int prev = curr - 1;
		node currNode = inst->getNodeByKey(rt.at(curr).key);
		node prevNode = inst->getNodeByKey(rt.at(prev).key);

		vertex& cVertex = rt.at(curr);
		vertex pVertex = rt.at(prev);

		if (currNode.type == "a" && rt.at(curr).bLevel >= 0) {
			break;
		}

		// computing parameters

		// compute battery level in the current node
		float bUsed = inst->getBatteryUsed(rt.at(prev).key, rt.at(curr).key);

		cVertex.bLevel = pVertex.bLevel + pVertex.recharged - bUsed;

		if (currNode.type == "f" || currNode.type == "f_d") {
			cVertex.recharged = inst->Q - cVertex.bLevel;
			cVertex.recharge = true;
		}
		else {
			cVertex.recharged = 0;
			cVertex.recharge = false;
		}		

		// compute arrival time in the current node
		float TD = inst->getTD(prevNode.key, currNode.key);
		cVertex.aTime = pVertex.lTime + TD;

		// compute waiting time in the current node
		if (cVertex.aTime < currNode.readyTime) {
			cVertex.wTime = currNode.readyTime - cVertex.aTime;
		}
		else { // no waiting time
			cVertex.wTime = 0;
		}

		// compute de departure time in the current node
		if (currNode.type == "f" || currNode.type == "f_d") {
			cVertex.lTime = cVertex.aTime + cVertex.wTime + inst->ct;
		}
		else {
			cVertex.lTime = cVertex.aTime + cVertex.wTime + currNode.serviceTime;
		}
		
		// compute vehcile load in the current node
		cVertex.vLoad = pVertex.vLoad - currNode.demand;

		if (rt.at(curr).bLevel < 0) {
			// if not enough battery, we have to insert a bss in the route

			int minBU = 0; // minimum battery usage
			int meanBU = 0;

			// try to recharge the battery

			// try to put a bss 
			while (rt.at(curr).bLevel - minBU < 0) {
				int put = false;
				curr--;
				minBU = INT_MAX;

				currNode = inst->getNodeByKey(rt.at(curr).key);
				int bss;

				// special cases
				// if the route is traced back to the depot due to a node 
				if (currNode.type == "d") {
					vector<node> R = inst->set_R(); // get all stations

					// search for the closest bss from the stop node
					for (node r : R) {

						int BU1 = inst->getBatteryUsed(rt.at(curr).key, r.key);
						int BU2 = inst->getBatteryUsed(r.key, rt.at(curr + 1).key);

						// get the lowest dist
						if (BU2 < minBU && BU1 < rt.at(curr).bLevel + rt.at(curr).recharged) {
							bss = r.key;
							minBU = BU2;
						}
					}
				}
				// search for the closest bss
				else if (currNode.type != "f") {
					//vector<node> R = inst->set_R(); // get all stations

					// search for the closest bss from the stop node
					pair<int, int> ret = closestBSS(rt, curr);
					bss = ret.first;
					minBU = ret.second;

				}
				// if the current node is a bss we have to watch out for a possible loop of bss
				// so we have to make sure it wont happen 
				else if (currNode.type == "f") {
					vector<node> R = inst->set_R(); // get all stations

					//
					int curr__ = curr - 1;
					bssSequence.clear();
					while (inst->getNodeByKey(rt.at(curr__).key).type == "f") {
						bssSequence.push_back(rt.at(curr__).key);
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
						vertex aux = rt.at(curr + 1);
						rt.erase(rt.begin() + curr + 1);
						/*
						// push to next route
						if (j < rt.size() - 2) { // if we have routes to push
							int size = rt.size();
							rt.insert(rt.begin() + size - 1, aux);

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
						*/
						// we also remove all the previous chain of bss
						node n = inst->getNodeByKey(rt.at(curr).key);
						while (n.type == "f") {
							rt.erase(rt.begin() + curr);
							curr--;
							n = inst->getNodeByKey(rt.at(curr).key);
						}

						// throw IsolatedNode(s.routes.at(j).at(curr + 1).key, j, s.perm);							
					}

					// search for the closest bss from the stop node
					pair<int, int> ret = closestBSS(rt, curr);
					bss = ret.first;
					minBU = ret.second;
				}

				// compute the amount of energy the vehcile can recharge during service time in the current routes segment
				int curr_ = curr;
				int energy = 0;
				while (true) {

					node n = inst->getNodeByKey(rt.at(curr_).key);

					if (n.type == "f" || n.type == "d") {
						break;
					}

					energy += (inst->g * n.serviceTime) - rt.at(curr_).recharged;
					curr_--;
				}

				int d = inst->getBatteryUsed(rt.at(curr).key, rt.at(curr + 1).key);

				//if ((rt.at(curr).bLevel + rt.at(curr).recharged + energy) - minBU >= 0) { // considering recharge during service time to reach next customer
				if ((rt.at(curr).bLevel + rt.at(curr).recharged + energy) - d >= 0) {

					// recharge during service time
					curr_ = curr;
					energy = 0;
					auto a = inst->getBatteryUsed(rt.at(curr).key, bss);
					// find the last customers before the lastest depot or customer
					/*
					while (true) {
						node n = inst->getNodeByKey(rt.at(curr_).key);
						energy += inst->g * n.serviceTime;

						// force to recharge just enough to reach the bss
						if (n.type == "f" || n.type == "d" || n.type == "f_d" || rt.at(curr).bLevel + rt.at(curr).recharged + energy > minBU) {
							break;
						}
						last = curr_;
						curr_--;
					}
					*/
					// recompute battery parameters

					// recompute battery parameters
					
					/*					
					// compute first node
					node n = inst->getNodeByKey(rt.at(curr_).key);

					rt.at(curr_).recharge = true;
					rt.at(curr_).recharged = inst->g * n.serviceTime;
					accumulated += rt.at(curr_).recharged;
					// in the case of the battery level after partial recharging became overcharged
					if (rt.at(curr_).bLevel + rt.at(curr_).recharged > inst->Q) {
						rt.at(curr_).recharged = inst->Q - rt.at(curr_).bLevel;
					}

					for (int i = curr_; i <= curr; i++) {
						node n = inst->getNodeByKey(rt.at(i).key);

						//s.routes.at(j).at(i).bLevel += s.routes.at(j).at(i - 1).recharged;

						rt.at(i).recharge = true;
						rt.at(i).recharged = inst->g * n.serviceTime;

						rt.at(i).bLevel += accumulated;
						accumulated += rt.at(i).recharged;

						// in the case of the battery level after partial recharging became overcharged
						if (rt.at(i).bLevel + rt.at(i).recharged > inst->Q) {
							rt.at(i).recharged = inst->Q - rt.at(i).bLevel;
						}
					}*/

					// recharge the battery
					int accumulated = 0;
					//for (int i = curr_ - 1; i <= curr; i++) {
					int i = curr;
					while (accumulated < abs(rt.at(curr + 1).bLevel) && inst->getNodeByKey(rt.at(i).key).type == "c") {
						node n = inst->getNodeByKey(rt.at(i).key);

						if (rt.at(i).recharged == 0) {
							rt.at(i).recharge = true;
							rt.at(i).recharged = inst->g * n.serviceTime;
							accumulated += rt.at(i).recharged;
						}
						i--;
					}
					// recompute some parameters
					for (i = i + 1; i <= curr; i++) {
						node n = inst->getNodeByKey(rt.at(i).key);

						auto ds = inst->dist(rt.at(i).key, rt.at(i + 1).key);

						rt.at(i + 1).bLevel = rt.at(i).bLevel + rt.at(i).recharged - ds;

					}

					prevNode = inst->getNodeByKey(rt.at(curr).key);

					// insert bss vertex in the route

					//curr++; //////////////;
					//put = true;
				}
				else if (rt.at(curr).bLevel + rt.at(curr).recharged - minBU >= 0) { // if battery is enough to go from current node and closest bss, insert bss in the route
					vertex v;
					v.key = bss;

					rt.insert(rt.begin() + curr + 1, v); // insert in the routes

					currNode = inst->getNodeByKey(rt.at(curr + 1).key);

					rt.at(curr + 1).vLoad = rt.at(curr).vLoad; // compute vehicle load

					prevNode = inst->getNodeByKey(rt.at(curr).key);
					rt.at(curr + 1).aTime = rt.at(curr).lTime + inst->getTD(prevNode.key, bss); // compute arrival time
					rt.at(curr + 1).lTime = rt.at(curr + 1).aTime + currNode.serviceTime + inst->ct; // compute departure time

					// s.routes.at(j).at(curr + 1).bLevel = inst->Q;
					auto BU = inst->getBatteryUsed(rt.at(curr).key, rt.at(curr + 1).key);
					rt.at(curr + 1).bLevel = rt.at(curr).bLevel + rt.at(curr).recharged - BU; // compute battery level
					// inst->getBatteryUsed(s.routes.at(j).at(curr).key, s.routes.at(j).at(curr + 1).key) = minBU

					int bLevel = rt.at(curr + 1).bLevel;
					rt.at(curr + 1).recharged = inst->Q - bLevel; // compute the amount of energy recharged
					rt.at(curr + 1).recharge = true;

					curr++;
					put = true;

				}
				else if ((rt.at(curr).bLevel + rt.at(curr).recharged + energy) - minBU >= 0) { // considering recharge during service time and moving to a bss
					// recharge during service time
					curr_ = curr;
					energy = 0;
					auto a = inst->getBatteryUsed(rt.at(curr).key, bss);
					// find the last customers before the lastest depot or customer
					while (true) {
						node n = inst->getNodeByKey(rt.at(curr_).key);
						energy += inst->g * n.serviceTime;

						// force to recharge just enough to reach the bss
						if (n.type == "f" || n.type == "d" || n.type == "f_d" || rt.at(curr).bLevel + energy > minBU) {
							break;
						}
						last = curr_;
						curr_--;
					}
					// recompute battery parameters
					int accumulated = 0;
					// compute first node
					node n = inst->getNodeByKey(rt.at(curr_).key);

					rt.at(curr_).recharge = true;
					rt.at(curr_).recharged = inst->g * n.serviceTime;
					accumulated += rt.at(curr_).recharged;
					// in the case of the battery level after partial recharging became overcharged
					if (rt.at(curr_).bLevel + rt.at(curr_).recharged > inst->Q) {
						rt.at(curr_).recharged = inst->Q - rt.at(curr_).bLevel;
					}

					// compute the rest
					for (int i = curr_ + 1; i <= curr; i++) {
						node n = inst->getNodeByKey(rt.at(i).key);

						//s.routes.at(j).at(i).bLevel += s.routes.at(j).at(i - 1).recharged;

						rt.at(i).recharge = true;
						rt.at(i).recharged = inst->g * n.serviceTime;

						rt.at(i).bLevel += accumulated;
						accumulated += rt.at(i).recharged;

						// in the case of the battery level after partial recharging became overcharged
						if (rt.at(i).bLevel + rt.at(i).recharged > inst->Q) {
							rt.at(i).recharged = inst->Q - rt.at(i).bLevel;
						}

					}

					prevNode = inst->getNodeByKey(rt.at(curr).key);

					// insert bss vertex in the route
					vertex v;
					v.key = bss;

					v.vLoad = rt.at(prev).vLoad; // compute vehicle load						

					v.aTime = rt.at(prev).lTime + inst->getTD(prevNode.key, bss); // compute arrival time

					v.lTime = v.aTime + inst->ct; // compute departure time			

					auto bUsed = inst->getBatteryUsed(rt.at(prev).key, v.key);
					v.bLevel = rt.at(prev).bLevel + rt.at(curr).recharged - bUsed; // compute battery level

					v.recharged = inst->Q - v.bLevel; // compute the amount of energy recharged

					v.recharge = true;

					rt.insert(rt.begin() + curr + 1, v); // insert in the routes

					curr++;
					put = true;
				}
				else {
					//curr--;
					//cout << "fudeu" << endl;
				}

				if (put == true) {
					break;
				}
			}
		}
		curr++;
	}

	return rt;
}

Solution perm_rep::addStations_model(Solution s)
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

			vertex& cVertex = s.routes.at(j).at(curr);
			vertex pVertex = s.routes.at(j).at(prev);

			// computing parameters

			// compute battery level in the current node
			float bUsed = inst->getBatteryUsed(s.routes.at(j).at(prev).key, s.routes.at(j).at(curr).key);

			cVertex.bLevel = pVertex.bLevel + pVertex.recharged - bUsed;

			cVertex.recharged = 0;
			cVertex.recharge = false;

			// compute arrival time in the current node
			float TD = inst->getTD(prevNode.key, currNode.key);
			cVertex.aTime = pVertex.lTime + TD;
			//s.routes.at(j).at(curr).aTime = s.routes.at(j).at(prev).aTime + prevNode.serviceTime + inst->getTD(prevNode.key, currNode.key);

			// compute waiting time in the current node
			if (cVertex.aTime < currNode.readyTime) {
				cVertex.wTime = currNode.readyTime - cVertex.aTime;
			}
			else { // no waiting time
				cVertex.wTime = 0;
			}

			// compute de departure time in the current node
			cVertex.lTime = cVertex.aTime + cVertex.wTime + currNode.serviceTime;

			// compute vehcile load in the current node
			cVertex.vLoad = pVertex.vLoad - currNode.demand;

			// checking due date and vehicle load
			if (s.routes.at(j).at(curr).aTime > currNode.dueDate || s.routes.at(j).at(curr).vLoad < 0) { ////////////////////////////////////////////////
				// if the current node is reached after the due date, we will remove this node from the route and push it to the next one

				vertex v = s.routes.at(j).at(curr); // store the node key that will be removed				
				s.routes.at(j).erase(s.routes.at(j).begin() + curr); // remove the node from the route

				// checking if we are in the last route
				if (j < s.routes.size() - 1) { // not last route
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
					dpt.vLoad = inst->c;
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

				// remove any bss sited in order to this node to be reached
				currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
				while (currNode.type == "f" || currNode.type == "f_d") {
					s.routes.at(j).erase(s.routes.at(j).begin() + curr);

					curr--;

					currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
				}

			}
			// checking battery level
			else if (s.routes.at(j).at(curr).bLevel < 0) {
				// if not enough battery, we have to insert a bss in the route

				int minBU = 0; // minimum battery usage
				int meanBU = 0;

				// try to put a bss 
				while (s.routes.at(j).at(curr).bLevel - minBU < 0) {
					int put = false;
					curr--;
					minBU = INT_MAX;

					currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
					int bss;

					// special case
					// if the route is traced back to the depot due to a node 
					if (currNode.type == "d") {
						vector<node> R = inst->set_R(); // get all stations

						vector<node> SK;
						for (auto h : R) {
							auto SKr = inst->set_SK(h.key);
							SK.insert(SK.end(), SKr.begin(), SKr.end());
						}
						vector<node> RUSKr = inst->vectorUnion(R, SK);

						// search for the closest bss from the stop node
						for (node r : RUSKr) {

							int BU1 = inst->getBatteryUsed(s.routes.at(j).at(curr).key, r.key);
							int BU2 = inst->getBatteryUsed(r.key, s.routes.at(j).at(curr + 1).key);

							// search if the node is already in the solution
							bool found = false;
							for (route rt : s.routes) {
								for (vertex v : rt) {
									if (v.key == r.key) {
										found = true;
										break;
									}
								}
							}

							// get the lowest dist
							if (BU2 < minBU && BU1 < s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged && found == false) {
								bss = r.key;
								minBU = BU2;
							}
						}
					}
					// search for the closest bss
					else if (currNode.type != "f") {
						vector<node> R = inst->set_R(); // get all stations
						vector<node> SK;
						for (auto h : R) {
							auto SKr = inst->set_SK(h.key);
							SK.insert(SK.end(), SKr.begin(), SKr.end());
						}
						vector<node> RUSKr = inst->vectorUnion(R, SK);

						// search for the closest bss from the stop node
						for (node r : RUSKr) {
							int BU = inst->getBatteryUsed(s.routes.at(j).at(curr).key, r.key); // inst->dist(r.key, b);

							// get the lowest dist
							// search if the node is already in the solution
							bool found = false;
							for (route rt : s.routes) {
								for (vertex v : rt) {
									if (v.key == r.key) {
										found = true;
										break;
									}
								}
							}

							if (BU < minBU && r.key != s.routes.at(j).at(curr).key && found == false) {
								bss = r.key;
								minBU = BU;
							}
						}
					}
					// if the current node is a bss we have to watch out for a possible loop of bss
					// so we have to make sure it wont happen 
					else if (currNode.type == "f") {
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

						/////////////
						vector<node> SK;
						for (auto h : R) {
							auto SKr = inst->set_SK(h.key);
							SK.insert(SK.end(), SKr.begin(), SKr.end());
						}
						vector<node> RUSKr = inst->vectorUnion(R, SK);

						// search for the closest bss from the stop node
						for (node r : RUSKr) {
							int BU = inst->getBatteryUsed(s.routes.at(j).at(curr).key, r.key); // inst->dist(r.key, b);

							// search if the node is already in the solution
							bool found = false;
							for (route rt : s.routes) {
								for (vertex v : rt) {
									if (v.key == r.key) {
										found = true;
										break;
									}
								}
							}

							// get the lowest dist
							if (BU < minBU && r.key != s.routes.at(j).at(curr).key && found == false) {
								bss = r.key;
								minBU = BU;
							}
						}
					}

					// compute the amount of energy the vehcile can recharge during service time in the current routes segment
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

					if (s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged - minBU >= 0) { // if battery is enough to go from current node and closest bss, insert bss in the route
						vertex v;
						v.key = bss;

						s.routes.at(j).insert(s.routes.at(j).begin() + curr + 1, v); // insert in the routes

						currNode = inst->getNodeByKey(s.routes.at(j).at(curr + 1).key);

						s.routes.at(j).at(curr + 1).vLoad = s.routes.at(j).at(curr).vLoad; // compute vehicle load

						prevNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
						s.routes.at(j).at(curr + 1).aTime = s.routes.at(j).at(curr).lTime + inst->getTD(prevNode.key, bss); // compute arrival time
						s.routes.at(j).at(curr + 1).lTime = s.routes.at(j).at(curr + 1).aTime + currNode.serviceTime + inst->ct; // compute departure time

						// s.routes.at(j).at(curr + 1).bLevel = inst->Q;
						auto BU = inst->getBatteryUsed(s.routes.at(j).at(curr).key, s.routes.at(j).at(curr + 1).key);
						s.routes.at(j).at(curr + 1).bLevel = s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged - BU; // compute battery level
						// inst->getBatteryUsed(s.routes.at(j).at(curr).key, s.routes.at(j).at(curr + 1).key) = minBU

						int bLevel = s.routes.at(j).at(curr + 1).bLevel;
						s.routes.at(j).at(curr + 1).recharged = inst->Q - bLevel; // compute the amount of energy recharged
						// inst->Q - 
						s.routes.at(j).at(curr + 1).recharge = true;

						curr++;
						put = true;

					}
					else if ((s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged + energy) - minBU >= 0) { // considering recharge during service time
						// recharge during service time
						curr_ = curr;
						energy = 0;
						auto a = inst->getBatteryUsed(s.routes.at(j).at(curr).key, bss);
						// find the last customers before the lastest depot or customer
						while (true) {
							node n = inst->getNodeByKey(s.routes.at(j).at(curr_).key);
							energy += inst->g * n.serviceTime;

							// force to recharge just enough to reach the bss
							if (n.type == "f" || n.type == "d" || s.routes.at(j).at(curr).bLevel + energy > minBU) {
								break;
							}

							curr_--;
						}
						// recompute battery parameters
						int accumulated = 0;
						// compute first node
						node n = inst->getNodeByKey(s.routes.at(j).at(curr_).key);

						s.routes.at(j).at(curr_).recharge = true;
						s.routes.at(j).at(curr_).recharged = inst->g * n.serviceTime;
						accumulated += s.routes.at(j).at(curr_).recharged;
						// in the case of the battery level after partial recharging became overcharged
						if (s.routes.at(j).at(curr_).bLevel + s.routes.at(j).at(curr_).recharged > inst->Q) {
							s.routes.at(j).at(curr_).recharged = inst->Q - s.routes.at(j).at(curr_).bLevel;
						}

						// compute the rest
						for (int i = curr_ + 1; i <= curr; i++) {
							node n = inst->getNodeByKey(s.routes.at(j).at(i).key);

							s.routes.at(j).at(i).bLevel += s.routes.at(j).at(i - 1).recharged;

							s.routes.at(j).at(i).recharge = true;
							s.routes.at(j).at(i).recharged = inst->g * n.serviceTime;

							accumulated += s.routes.at(j).at(i).recharged;
							s.routes.at(j).at(i).bLevel += accumulated;

							// in the case of the battery level after partial recharging became overcharged
							if (s.routes.at(j).at(i).bLevel + s.routes.at(j).at(i).recharged > inst->Q) {
								s.routes.at(j).at(i).recharged = inst->Q - s.routes.at(j).at(i).bLevel;
							}

						}

						prevNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);

						// insert bss vertex in the route
						vertex v;
						v.key = bss;

						v.vLoad = s.routes.at(j).at(prev).vLoad; // compute vehicle load						

						v.aTime = s.routes.at(j).at(prev).lTime + inst->getTD(prevNode.key, bss); // compute arrival time

						v.lTime = v.aTime + inst->ct; // compute departure time			

						auto bUsed = inst->getBatteryUsed(s.routes.at(j).at(prev).key, v.key);
						v.bLevel = s.routes.at(j).at(prev).bLevel + s.routes.at(j).at(curr).recharged - bUsed; // compute battery level

						v.recharged = inst->Q - v.bLevel; // compute the amount of energy recharged

						v.recharge = true;

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

Solution perm_rep::addStations(Solution s, int n)
{
	// set all vehicles to full energy
	for (int j = 0; j < s.routes.size(); j++) {
		s.routes.at(j).front().bLevel = inst->Q;
	}

	int last = 0;
	vector<int> bssSequence;

	for (int j = 0; j < s.routes.size(); j++) {
		int curr = 1;
		try {
			s.routes.at(j) = addStations(s.routes.at(j), n);

			// remove infeasible nodes from the routes end
			while (inst->getNodeByKey(s.routes.at(j).back().key).type != "a") {
				vertex v = s.routes.at(j).back();
				s.routes.at(j).erase(s.routes.at(j).end() - 1);

				if (j == s.routes.size() - 1) { // create a new route
					route r;
					r.push_back(s.routes.at(j).front());

					r.push_back(v);

					int i = s.routes.at(j).size() - 1;
					while (inst->getNodeByKey(s.routes.at(j).at(i).key).type != "a") {
						i--;
					}

					r.push_back(s.routes.at(j).at(i));

					s.routes.push_back(r);
				}
				else {
					s.routes.at(j + 1).insert(s.routes.at(j + 1).begin() + 1, v);
				}
			}
		}
		catch (string s) {
			cout << s << endl;
		}
		catch (exception e) {
			cout << e.what() << endl;
			throw PermutationInf(s.perm);
		}

	}
	s = procSol(s);
	return s;
}

Solution perm_rep::addStationsAlt(Solution s)
{
	int last = 0;
	vector<int> bssSequence;

	for (int j = 0; j < s.routes.size(); j++) {
		int curr = 1;
		try {
			s.routes.at(j) = addStationsAlt(s.routes.at(j));


			while (inst->getNodeByKey(s.routes.at(j).back().key).type != "a") {
				vertex v = s.routes.at(j).back();
				s.routes.at(j).erase(s.routes.at(j).end() - 1);

				if (j == s.routes.size() - 1) { // create a new route
					route r;
					r.push_back(s.routes.at(j).front());

					r.push_back(v);

					int i = s.routes.at(j).size() - 1;
					while (inst->getNodeByKey(s.routes.at(j).at(i).key).type != "a") {
						i--;
					}

					r.push_back(s.routes.at(j).at(i));

					s.routes.push_back(r);
				}
				else {
					s.routes.at(j + 1).insert(s.routes.at(j + 1).begin() + 1, v);
				}
			}
		}
		catch (string s) {
			cout << s << endl;
		}
		catch (exception e) {
			cout << e.what() << endl;
			throw PermutationInf(s.perm);
		}

	}

	s = procSol(s);
	return s;
}

Solution perm_rep::addStations_OLD(Solution s, int n)
{
	// set all vehicles to full energy
	for (int j = 0; j < s.routes.size(); j++) {
		s.routes.at(j).front().bLevel = inst->Q;
	}

	int last = 0;
	vector<int> bssSequence;

	for (int j = 0; j < s.routes.size(); j++) {
		int curr = 1;

		while (curr < s.routes.at(j).size()) {
			if (curr == 0) {
				throw "error";
			}
			int prev = curr - 1;
			node currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
			node prevNode = inst->getNodeByKey(s.routes.at(j).at(prev).key);

			vertex& cVertex = s.routes.at(j).at(curr);
			vertex pVertex = s.routes.at(j).at(prev);

			// computing parameters

			// compute battery level in the current node
			float bUsed = inst->getBatteryUsed(s.routes.at(j).at(prev).key, s.routes.at(j).at(curr).key);

			cVertex.bLevel = pVertex.bLevel + pVertex.recharged - bUsed;

			cVertex.recharged = 0;
			cVertex.recharge = false;

			// compute arrival time in the current node
			float TD = inst->getTD(prevNode.key, currNode.key);
			cVertex.aTime = pVertex.lTime + TD;
			//s.routes.at(j).at(curr).aTime = s.routes.at(j).at(prev).aTime + prevNode.serviceTime + inst->getTD(prevNode.key, currNode.key);

			// compute waiting time in the current node
			if (cVertex.aTime < currNode.readyTime) {
				cVertex.wTime = currNode.readyTime - cVertex.aTime;
			}
			else { // no waiting time
				cVertex.wTime = 0;
			}

			// compute de departure time in the current node
			cVertex.lTime = cVertex.aTime + cVertex.wTime + currNode.serviceTime;

			// compute vehcile load in the current node
			cVertex.vLoad = pVertex.vLoad - currNode.demand;

			// checking due date and vehicle load
			if (s.routes.at(j).at(curr).aTime > currNode.dueDate || s.routes.at(j).at(curr).vLoad < 0) { ////////////////////////////////////////////////
				// if the current node is reached after the due date, we will remove this node from the route and push it to the next one

				vertex v = s.routes.at(j).at(curr); // store the node key that will be removed				
				s.routes.at(j).erase(s.routes.at(j).begin() + curr); // remove the node from the route

				// checking if we are in the last route
				if (j < s.routes.size() - 1) { // not last route
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
					dpt.vLoad = inst->c;
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

				// remove any bss sited in order to this node to be reached and discharge battery in previous nodes
				currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
				while (currNode.type == "f" || currNode.type == "f_d") {
					s.routes.at(j).erase(s.routes.at(j).begin() + curr);

					curr--;

					currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
				}

				//
				currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
				int curr_ = curr;

				// here i am supposed to remove the the rechaged battery in the case of bss removal
				// i will look up to this later
			}
			// checking battery level
			else if (s.routes.at(j).at(curr).bLevel < 0) {
				// if not enough battery, we have to insert a bss in the route

				int minBU = 0; // minimum battery usage
				int meanBU = 0;

				// try to put a bss 
				while (s.routes.at(j).at(curr).bLevel - minBU < 0) {
					int put = false;
					curr--;
					minBU = INT_MAX;

					currNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
					int bss;

					// special case
					// if the route is traced back to the depot due to a node 
					if (currNode.type == "d") {
						vector<node> R = inst->set_R(); // get all stations

						// search for the closest bss from the depot
						for (node r : R) {

							int BU1 = inst->getBatteryUsed(s.routes.at(j).at(curr).key, r.key);
							int BU2 = inst->getBatteryUsed(r.key, s.routes.at(j).at(curr + 1).key);

							// get the lowest dist
							if (BU2 < minBU && BU1 < s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged) {
								bss = r.key;
								minBU = BU2;
							}
						}
					}
					// search for the closest bss
					else if (currNode.type != "f") {
						//vector<node> R = inst->set_R(); // get all stations

						// search for the closest bss from the stop node
						pair<int, int> ret = closestBSS(s, j, curr, n);
						bss = ret.first;
						minBU = ret.second;

					}
					// if the current node is a bss we have to watch out for a possible loop of bss
					// so we have to make sure it wont happen 
					else if (currNode.type == "f") {
						vector<node> R = inst->set_R(); // get all stations

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
						pair<int, int> ret = closestBSS(s, j, curr, n);
						bss = ret.first;
						minBU = ret.second;

					}

					// compute the amount of energy the vehcile can recharge during service time in the current routes segment
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

					if (s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged - minBU >= 0) { // if battery is enough to go from current node and closest bss, insert bss in the route
						vertex v;
						v.key = bss;

						s.routes.at(j).insert(s.routes.at(j).begin() + curr + 1, v); // insert in the routes

						currNode = inst->getNodeByKey(s.routes.at(j).at(curr + 1).key);

						s.routes.at(j).at(curr + 1).vLoad = s.routes.at(j).at(curr).vLoad; // compute vehicle load

						prevNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);
						s.routes.at(j).at(curr + 1).aTime = s.routes.at(j).at(curr).lTime + inst->getTD(prevNode.key, bss); // compute arrival time
						s.routes.at(j).at(curr + 1).lTime = s.routes.at(j).at(curr + 1).aTime + currNode.serviceTime + inst->ct; // compute departure time

						// s.routes.at(j).at(curr + 1).bLevel = inst->Q;
						auto BU = inst->getBatteryUsed(s.routes.at(j).at(curr).key, s.routes.at(j).at(curr + 1).key);
						s.routes.at(j).at(curr + 1).bLevel = s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged - BU; // compute battery level
						// inst->getBatteryUsed(s.routes.at(j).at(curr).key, s.routes.at(j).at(curr + 1).key) = minBU

						int bLevel = s.routes.at(j).at(curr + 1).bLevel;
						s.routes.at(j).at(curr + 1).recharged = inst->Q - bLevel; // compute the amount of energy recharged
						// inst->Q - 
						s.routes.at(j).at(curr + 1).recharge = true;

						curr++;
						put = true;

					}
					else if ((s.routes.at(j).at(curr).bLevel + s.routes.at(j).at(curr).recharged + energy) - minBU >= 0) { // considering recharge during service time
						// recharge during service time
						curr_ = curr;
						energy = 0;
						auto a = inst->getBatteryUsed(s.routes.at(j).at(curr).key, bss);
						// find the last customers before the lastest depot or customer
						while (true) {
							node n = inst->getNodeByKey(s.routes.at(j).at(curr_).key);
							energy += inst->g * n.serviceTime;

							// force to recharge just enough to reach the bss
							if (n.type == "f" || n.type == "d" || n.type == "f_d" || s.routes.at(j).at(curr).bLevel + energy > minBU) {
								break;
							}
							last = curr_;
							curr_--;
						}
						// recompute battery parameters
						int accumulated = 0;
						// compute first node
						node n = inst->getNodeByKey(s.routes.at(j).at(curr_).key);

						s.routes.at(j).at(curr_).recharge = true;
						s.routes.at(j).at(curr_).recharged = inst->g * n.serviceTime;
						accumulated += s.routes.at(j).at(curr_).recharged;
						// in the case of the battery level after partial recharging became overcharged
						if (s.routes.at(j).at(curr_).bLevel + s.routes.at(j).at(curr_).recharged > inst->Q) {
							s.routes.at(j).at(curr_).recharged = inst->Q - s.routes.at(j).at(curr_).bLevel;
						}

						// compute the rest
						for (int i = curr_ + 1; i <= curr; i++) {
							node n = inst->getNodeByKey(s.routes.at(j).at(i).key);

							//s.routes.at(j).at(i).bLevel += s.routes.at(j).at(i - 1).recharged;

							s.routes.at(j).at(i).recharge = true;
							s.routes.at(j).at(i).recharged = inst->g * n.serviceTime;

							s.routes.at(j).at(i).bLevel += accumulated;
							accumulated += s.routes.at(j).at(i).recharged;

							// in the case of the battery level after partial recharging became overcharged
							if (s.routes.at(j).at(i).bLevel + s.routes.at(j).at(i).recharged > inst->Q) {
								s.routes.at(j).at(i).recharged = inst->Q - s.routes.at(j).at(i).bLevel;
							}

						}

						prevNode = inst->getNodeByKey(s.routes.at(j).at(curr).key);

						// insert bss vertex in the route
						vertex v;
						v.key = bss;

						v.vLoad = s.routes.at(j).at(prev).vLoad; // compute vehicle load						

						v.aTime = s.routes.at(j).at(prev).lTime + inst->getTD(prevNode.key, bss); // compute arrival time

						v.lTime = v.aTime + inst->ct; // compute departure time			

						auto bUsed = inst->getBatteryUsed(s.routes.at(j).at(prev).key, v.key);
						v.bLevel = s.routes.at(j).at(prev).bLevel + s.routes.at(j).at(curr).recharged - bUsed; // compute battery level

						v.recharged = inst->Q - v.bLevel; // compute the amount of energy recharged

						v.recharge = true;

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

route perm_rep::addStations(route rt, int n)
{
	vector<vertex> temp;
	int last = 0;
	vector<int> bssSequence;

	// initialize the first node
	rt.front().bLevel = inst->Q;

	int curr = 1;
	while (curr < rt.size()) {
		if (curr == 0) {
			throw "error";
		}
		int prev = curr - 1;
		node currNode = inst->getNodeByKey(rt.at(curr).key);
		node prevNode = inst->getNodeByKey(rt.at(prev).key);

		vertex& cVertex = rt.at(curr);
		vertex pVertex = rt.at(prev);

		if (currNode.type == "a" && rt.at(curr).bLevel >= 0) {
			break;
		}

		// computing parameters

		// compute battery level in the current node
		float bUsed = inst->getBatteryUsed(rt.at(prev).key, rt.at(curr).key);

		cVertex.bLevel = pVertex.bLevel + pVertex.recharged - bUsed;

		if (currNode.type == "f" || currNode.type == "f_d") {
			cVertex.recharged = inst->Q - cVertex.bLevel;
			cVertex.recharge = true;
		}
		else {
			cVertex.recharged = inst->g * currNode.serviceTime;
			cVertex.recharge = true;
		}

		// compute arrival time in the current node
		float TD = inst->getTD(prevNode.key, currNode.key);
		cVertex.aTime = pVertex.lTime + TD;

		// compute waiting time in the current node
		if (cVertex.aTime < currNode.readyTime) {
			cVertex.wTime = currNode.readyTime - cVertex.aTime;
		}
		else { // no waiting time
			cVertex.wTime = 0;
		}

		// compute de departure time in the current node
		if (currNode.type == "f" || currNode.type == "f_d") {
			cVertex.lTime = cVertex.aTime + cVertex.wTime + inst->ct;
		}
		else {
			cVertex.lTime = cVertex.aTime + cVertex.wTime + currNode.serviceTime;
		}

		// compute vehcile load in the current node
		cVertex.vLoad = pVertex.vLoad - currNode.demand;

		if (rt.at(curr).aTime > currNode.dueDate || rt.at(curr).vLoad < 0) { // if a vertex is infeasible to the time window constraint, push it to the end of the route
			if (currNode.type == "a") {
				curr--;

				while (inst->getNodeByKey(rt.at(curr).key).type == "f") {
					rt.erase(rt.begin() + curr);
					curr--;
				}

				vertex v = rt.at(curr);
				rt.erase(rt.begin() + curr);
				temp.push_back(v);

			}
			else {
				vertex v = rt.at(curr);
				rt.erase(rt.begin() + curr);
				temp.push_back(v);
				curr--;
			}

		}

		if (rt.at(curr).bLevel < 0) {
			// if not enough battery, we have to insert a bss in the route

			int minBU = 0; // minimum battery usage
			int meanBU = 0;

			// try to recharge the battery

			// try to put a bss 
			while (rt.at(curr).bLevel - minBU < 0) {
				int put = false;
				curr--;
				minBU = INT_MAX;

				currNode = inst->getNodeByKey(rt.at(curr).key);
				int bss;

				// special cases
				// if the route is traced back to the depot due to a node 
				if (currNode.type == "d") {
					vector<node> R = inst->set_R(); // get all stations

					// search for the closest bss from the stop node
					for (node r : R) {

						int BU1 = inst->getBatteryUsed(rt.at(curr).key, r.key);
						int BU2 = inst->getBatteryUsed(r.key, rt.at(curr + 1).key);

						// get the lowest dist
						if (BU2 < minBU && BU1 < rt.at(curr).bLevel + rt.at(curr).recharged) {
							bss = r.key;
							minBU = BU2;
						}
					}
				}
				// search for the closest bss
				else if (currNode.type != "f") {
					//vector<node> R = inst->set_R(); // get all stations

					// search for the closest bss from the stop node
					pair<int, int> ret = closestBSS(rt, curr);
					bss = ret.first;
					minBU = ret.second;

				}
				// if the current node is a bss we have to watch out for a possible loop of bss
				// so we have to make sure it wont happen 
				else if (currNode.type == "f") {
					vector<node> R = inst->set_R(); // get all stations

					//
					int curr__ = curr - 1;
					bssSequence.clear();
					while (inst->getNodeByKey(rt.at(curr__).key).type == "f") {
						bssSequence.push_back(rt.at(curr__).key);
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
						vertex aux = rt.at(curr + 1);
						rt.erase(rt.begin() + curr + 1);
						/*
						// push to next route
						if (j < rt.size() - 2) { // if we have routes to push
							int size = rt.size();
							rt.insert(rt.begin() + size - 1, aux);

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
						*/
						// we also remove all the previous chain of bss
						node n = inst->getNodeByKey(rt.at(curr).key);
						while (n.type == "f") {
							rt.erase(rt.begin() + curr);
							curr--;
							n = inst->getNodeByKey(rt.at(curr).key);
						}

						// throw IsolatedNode(s.routes.at(j).at(curr + 1).key, j, s.perm);							
					}

					// search for the closest bss from the stop node
					pair<int, int> ret = closestBSS(rt, curr);
					bss = ret.first;
					minBU = ret.second;
				}

				// compute the amount of energy the vehcile can recharge during service time in the current routes segment
				int curr_ = curr;
				int energy = 0;
				while (true) {

					node n = inst->getNodeByKey(rt.at(curr_).key);

					if (n.type == "f" || n.type == "d") {
						break;
					}

					energy += (inst->g * n.serviceTime) - rt.at(curr_).recharged;
					curr_--;
				}

				int d = inst->getBatteryUsed(rt.at(curr).key, rt.at(curr + 1).key);

				if (rt.at(curr).bLevel + rt.at(curr).recharged - minBU >= 0) { // if battery is enough to go from current node and closest bss, insert bss in the route
					vertex v;
					v.key = bss;

					rt.insert(rt.begin() + curr + 1, v); // insert in the routes

					currNode = inst->getNodeByKey(rt.at(curr + 1).key);

					rt.at(curr + 1).vLoad = rt.at(curr).vLoad; // compute vehicle load

					prevNode = inst->getNodeByKey(rt.at(curr).key);
					rt.at(curr + 1).aTime = rt.at(curr).lTime + inst->getTD(prevNode.key, bss); // compute arrival time
					rt.at(curr + 1).lTime = rt.at(curr + 1).aTime + currNode.serviceTime + inst->ct; // compute departure time

					// s.routes.at(j).at(curr + 1).bLevel = inst->Q;
					auto BU = inst->getBatteryUsed(rt.at(curr).key, rt.at(curr + 1).key);
					rt.at(curr + 1).bLevel = rt.at(curr).bLevel + rt.at(curr).recharged - BU; // compute battery level
					// inst->getBatteryUsed(s.routes.at(j).at(curr).key, s.routes.at(j).at(curr + 1).key) = minBU

					int bLevel = rt.at(curr + 1).bLevel;
					rt.at(curr + 1).recharged = inst->Q - bLevel; // compute the amount of energy recharged
					rt.at(curr + 1).recharge = true;

					curr++;
					put = true;

				}
				else if ((rt.at(curr).bLevel + rt.at(curr).recharged + energy) - minBU >= 0) { // considering recharge during service time and moving to a bss
					// recharge during service time
					curr_ = curr;
					energy = 0;
					auto a = inst->getBatteryUsed(rt.at(curr).key, bss);
					// find the last customers before the lastest depot or customer
					while (true) {
						node n = inst->getNodeByKey(rt.at(curr_).key);
						energy += inst->g * n.serviceTime;

						// force to recharge just enough to reach the bss
						if (n.type == "f" || n.type == "d" || n.type == "f_d" || rt.at(curr).bLevel + energy > minBU) {
							break;
						}
						last = curr_;
						curr_--;
					}
					// recompute battery parameters
					int accumulated = 0;
					// compute first node
					node n = inst->getNodeByKey(rt.at(curr_).key);

					rt.at(curr_).recharge = true;
					rt.at(curr_).recharged = inst->g * n.serviceTime;
					accumulated += rt.at(curr_).recharged;
					// in the case of the battery level after partial recharging became overcharged
					if (rt.at(curr_).bLevel + rt.at(curr_).recharged > inst->Q) {
						rt.at(curr_).recharged = inst->Q - rt.at(curr_).bLevel;
					}

					// compute the rest
					for (int i = curr_ + 1; i <= curr; i++) {
						node n = inst->getNodeByKey(rt.at(i).key);

						//s.routes.at(j).at(i).bLevel += s.routes.at(j).at(i - 1).recharged;

						rt.at(i).recharge = true;
						rt.at(i).recharged = inst->g * n.serviceTime;

						rt.at(i).bLevel += accumulated;
						accumulated += rt.at(i).recharged;

						// in the case of the battery level after partial recharging became overcharged
						if (rt.at(i).bLevel + rt.at(i).recharged > inst->Q) {
							rt.at(i).recharged = inst->Q - rt.at(i).bLevel;
						}

					}

					prevNode = inst->getNodeByKey(rt.at(curr).key);

					// insert bss vertex in the route
					vertex v;
					v.key = bss;

					v.vLoad = rt.at(prev).vLoad; // compute vehicle load						

					v.aTime = rt.at(prev).lTime + inst->getTD(prevNode.key, bss); // compute arrival time

					v.lTime = v.aTime + inst->ct; // compute departure time			

					auto bUsed = inst->getBatteryUsed(rt.at(prev).key, v.key);
					v.bLevel = rt.at(prev).bLevel + rt.at(curr).recharged - bUsed; // compute battery level

					v.recharged = inst->Q - v.bLevel; // compute the amount of energy recharged

					v.recharge = true;

					rt.insert(rt.begin() + curr + 1, v); // insert in the routes

					curr++;
					put = true;
				}

				if (inst->getNodeByKey(rt.at(curr).key).type == "d") {
					//cout << "fudeu" << endl;
				}

				if (put == true) {
					break;
				}
			}
		}
		curr++;
	}

	// optmize battery

	for (int i = 0; i < rt.size(); i++) {
		node n = inst->getNodeByKey(rt.at(i).key);

		if (n.type == "f" || n.type == "a") {
			int bLevel = rt.at(i).bLevel;

			int j = i - 1;
			while (inst->getNodeByKey(rt.at(j).key).type == "c" && bLevel > 0) {

				if (rt.at(j).recharged > 0) {
					if (rt.at(j).recharged >= bLevel) {
						rt.at(j).recharged -= bLevel;
						bLevel = 0;
					}
					else if (rt.at(j).recharged < bLevel) {
						rt.at(j).recharge = false;
						bLevel -= rt.at(j).recharged;
						rt.at(j).recharged = 0;
					}

				}
				j--;
			}

			rt.at(i).bLevel = bLevel;
		}
	}

	rt.insert(rt.end(), temp.begin(), temp.end());

	return rt;
}

route perm_rep::addStationsAlt(route rt)
{
	vector<vertex> temp;
	int last = 0;
	vector<int> bssSequence;

	// initialize the first node
	rt.front().bLevel = inst->Q;

	int curr = 1;
	while (curr < rt.size()) {
		if (curr == 0) {
			throw "error";
		}
		if (inst->getNodeByKey(rt.at(curr).key).type == "d") {
			throw "error";
		}
		int prev = curr - 1;
		node currNode = inst->getNodeByKey(rt.at(curr).key);
		node prevNode = inst->getNodeByKey(rt.at(prev).key);

		vertex& cVertex = rt.at(curr);
		vertex pVertex = rt.at(prev);

		if (currNode.type == "a" && rt.at(curr).bLevel >= 0) {
			break;
		}

		// computing parameters

		// compute battery level in the current node
		float bUsed = inst->getBatteryUsed(rt.at(prev).key, rt.at(curr).key);

		cVertex.bLevel = pVertex.bLevel + pVertex.recharged - bUsed;

		if (currNode.type == "f" || currNode.type == "f_d") {
			cVertex.recharged = inst->Q - cVertex.bLevel;
			cVertex.recharge = true;
		}
		else {
			cVertex.recharged = inst->g * currNode.serviceTime;
			cVertex.recharge = true;
		}

		// compute arrival time in the current node
		float TD = inst->getTD(prevNode.key, currNode.key);
		cVertex.aTime = pVertex.lTime + TD;

		// compute waiting time in the current node
		if (cVertex.aTime < currNode.readyTime) {
			cVertex.wTime = currNode.readyTime - cVertex.aTime;
		}
		else { // no waiting time
			cVertex.wTime = 0;
		}

		// compute de departure time in the current node
		if (currNode.type == "f" || currNode.type == "f_d") {
			cVertex.lTime = cVertex.aTime + cVertex.wTime + inst->ct;
		}
		else {
			cVertex.lTime = cVertex.aTime + cVertex.wTime + currNode.serviceTime;
		}

		// compute vehcile load in the current node
		cVertex.vLoad = pVertex.vLoad - currNode.demand;

		if (rt.at(curr).aTime > currNode.dueDate || rt.at(curr).vLoad < 0) { // if a vertex is infeasible to the time window constraint, push it to the end of the route
			if (currNode.type == "a") {
				curr--;

				while (inst->getNodeByKey(rt.at(curr).key).type == "f") {
					rt.erase(rt.begin() + curr);
					curr--;
				}

				vertex v = rt.at(curr);
				rt.erase(rt.begin() + curr);
				temp.push_back(v);

			}
			else {
				vertex v = rt.at(curr);
				rt.erase(rt.begin() + curr);
				temp.push_back(v);
				curr--;
			}
		}
		else if (rt.at(curr).bLevel < 0) {
			
			vector<pair<int, int>> candidates;

			int curr_ = curr - 1;
			int bss, pos;
/*
			if (inst->getNodeByKey(rt.at(curr_).key).type == "f") {
				pos = curr_;
				bss = closestBSS(rt, curr_).first;

				
				vector<node> R = inst->set_R(); // get all stations
					//
				int curr__ = curr - 1;
				bssSequence.clear();
				while (inst->getNodeByKey(rt.at(curr__).key).type == "f") {
					bssSequence.push_back(rt.at(curr__).key);
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

				// search for the closest bss from the stop node
				pair<int, int> ret = closestBSS(rt, curr);
				bss = ret.first;
				minBU = ret.second;
				
			}
			else {*/
				while ((rt.at(curr_).bLevel + rt.at(curr_).recharged <= inst->Q * 1) && inst->getNodeByKey(rt.at(curr_).key).type != "f" && inst->getNodeByKey(rt.at(curr_).key).type != "d") {

					int bss = minimalDeviationBSS(rt, curr_);

					if (bss != -1) {
						candidates.push_back(pair<int, int>(curr_, bss));
					}
					curr_--;
				}
				
				if (candidates.size() == 0) {
					//curr_--;
					int pBSS = getPreviousStatioPos(rt, curr_);

					vector<int> usedBSS;
					int count = pBSS;
					while (inst->getNodeByKey(rt.at(count).key).type == "f") {
						usedBSS.push_back(rt.at(count).key);
						count--;
					}
					curr_ = count + 1;

					vector<node> R = inst->set_R();

					for (auto j : usedBSS) {
						for (int i = 0; i < R.size(); i++) {						
							if(R.at(i).key == j) {
								R.erase(R.begin() + i);
								break;
							}
						}
					}

					if (R.size() == 0) {
						throw "Set R has zero elements after stations removal";
					}

					int minDev = INT_MAX;
					bss = -1;

					for (auto i : R) {
						int dev1 = inst->getBatteryUsed(rt.at(curr_).key, i.key);
						int dev2 = inst->getBatteryUsed(i.key, rt.at(curr_ + 1).key);
						int bLevel = rt.at(curr_).bLevel + rt.at(curr_).recharged;

						if (dev2 < minDev && dev1 < abs(bLevel)) {
							minDev = dev2;
							bss = i.key;
						}
					}
					pos = curr_;

				}
				else {
					int c = Random::get<int>(0, candidates.size() - 1);
					bss = candidates.at(c).second;
					pos = candidates.at(c).first;
				}	
			//}

			// choose a random place in the cadidate list to receive a bss
			vertex v;
			v.key = bss;

			v.vLoad = rt.at(pos).vLoad; // compute vehicle load						

			v.aTime = rt.at(pos).lTime + inst->getTD(rt.at(pos).key, v.key); // compute arrival time

			v.lTime = v.aTime + inst->ct; // compute departure time			

			auto bUsed = inst->getBatteryUsed(rt.at(pos).key, v.key);
			v.bLevel = rt.at(pos).bLevel + rt.at(pos).recharged - bUsed; // compute battery level

			v.recharged = inst->Q - v.bLevel; // compute the amount of energy recharged

			v.recharge = true;

			rt.insert(rt.begin() + pos + 1, v); // insert in the routes

			curr = pos + 1;
			
		}
		curr++;
	}

	// optmize battery

	for (int i = 0; i < rt.size(); i++) {
		node n = inst->getNodeByKey(rt.at(i).key);

		if (n.type == "f" || n.type == "a") {
			int bLevel = rt.at(i).bLevel;

			int j = i - 1;
			while (inst->getNodeByKey(rt.at(j).key).type == "c" && bLevel > 0) {

				if (rt.at(j).recharged > 0) {
					if (rt.at(j).recharged >= bLevel) {
						rt.at(j).recharged -= bLevel;
						bLevel = 0;
					}
					else if (rt.at(j).recharged < bLevel) {
						rt.at(j).recharge = false;
						bLevel -= rt.at(j).recharged;
						rt.at(j).recharged = 0;
					}

				}
				j--;
			}

			rt.at(i).bLevel = bLevel;
		}
	}


	rt.insert(rt.end(), temp.begin(), temp.end());

	return rt;
}

bool perm_rep::rechargeSchedule(route &r, int beg, int end, int energy)
{
	vector<pair<int, int>> aux;
	aux.resize(end - beg + 1);

	for (int i = 0; i < aux.size(); i++) {
		pair<int, int> a;

		vertex v = r.at(i + beg);
		node n = inst->getNodeByKey(v.key);

		a.first = n.key;
		a.second = n.serviceTime * inst->g;
	}

	sort(aux.begin(), aux.end(), [](pair<int, int> a, pair<int, int> b){return a.second < b.second; });

	int count = 0;
	for (int i = 0; i < aux.size(); i++) {
		vertex v = r.at(aux.at(i).first);
		node n = inst->getNodeByKey(v.key);

		r.at(v.key).recharge = n.serviceTime * inst->g;

		count += r.at(v.key).recharge;
	}

	return true;
}


pair<int, int> perm_rep::closestBSS(Solution& s, int route, int key)
{
	vector<node> R = inst->set_R();
	int minBU = INT_MAX;
	int bss = 0;

	// search for the closest bss from the stop node
	for (node r : R) {
		int BU = inst->getBatteryUsed(s.routes.at(route).at(key).key, r.key); // inst->dist(r.key, b);

		// get the lowest dist
		if (BU < minBU && r.key != s.routes.at(route).at(key).key) {
			bss = r.key;
			minBU = BU;
		}
	}
	return pair<int, int>(bss, minBU);
}

pair<int, int> perm_rep::closestBSS(route& rt, int key)
{
	vector<node> R = inst->set_R();
	int minBU = INT_MAX;
	int bss = 0;

	vector<pair<int, int>> stations;

	// search for the closest bss from the stop node
	for (node r : R) {
		int BU = inst->getBatteryUsed(rt.at(key).key, r.key); // inst->dist(r.key, b);

		stations.push_back(pair<int, int>(r.key, BU));

		// get the lowest dist
		if (BU < minBU && r.key != rt.at(key).key) {
			bss = r.key;
			minBU = BU;
		}		
	}

	if (inst->getNodeByKey(rt.at(key).key).type == "c") {
		return pair<int, int>(bss, minBU);
	}
	else {
		// get 'f' subroute
		vector<vertex> usedStations;
		int i = key;
		while (inst->getNodeByKey(rt.at(i).key).type == "f") {
			usedStations.push_back(rt.at(i));
			i--;
		}

		if (usedStations.size() == R.size()) {
			throw OutOfBSS();
		}

		sort(stations.begin(), stations.end(), [](pair<int, int> a, pair<int, int> b) {return a.second < b.second; });

		i = 0;
		for (auto s : stations) {
			bool isin = false;
			for (auto us : usedStations) {
				if (s.first == us.key) {
					isin = true;
					break;;
				}
			}
			if (isin == false) {
				return stations.at(i);
			}
			i++;
		}
	}
}

pair<int, int> perm_rep::closestBSS(route& rt, int key, int n)
{
	vector<node> R = inst->set_R();
	int minBU = INT_MAX;
	int bss = 0;

	vector<pair<int, int>> stations;

	// search for the closest bss from the stop node
	for (node r : R) {
		int BU = inst->getBatteryUsed(rt.at(key).key, r.key); // inst->dist(r.key, b);

		stations.push_back(pair<int, int>(r.key, BU));

		// get the lowest dist
		if (BU < minBU && r.key != rt.at(key).key) {
			bss = r.key;
			minBU = BU;
		}
	}

	if (inst->getNodeByKey(rt.at(key).key).type == "c") {
		return pair<int, int>(bss, minBU);
	}
	else {
		// get 'f' subroute
		vector<vertex> usedStations;
		int i = key;
		while (inst->getNodeByKey(rt.at(i).key).type == "f") {
			usedStations.push_back(rt.at(i));
			i--;
		}

		if (usedStations.size() == R.size()) {
			throw "error";
		}

		sort(stations.begin(), stations.end(), [](pair<int, int> a, pair<int, int> b) {return a.second < b.second; });

		i = 0;
		for (auto s : stations) {
			for (auto us : usedStations) {
				if (s.first == us.key) {
					stations.erase(stations.begin() + i);
					break;
				}
			}
			i++;
		}
	}

	if (stations.size() - 1 < n) {
		n = stations.size() - 1;
	}
	int s = Random::get<int>(0, n);

	return stations.at(n);
}

pair<int, int> perm_rep::closestBSS(Solution& s, int route, int key, int m)
{
	vector<node> R = inst->set_R();

	if (m > R.size()) {
		throw "candidate list's size is grater then R's size";
	}

	vector<pair<int, int>> dists;

	// create the candidate list
	for (node r : R) {
		// excluding the vertex if it already appears as the current vertex
		if (r.key != s.routes.at(route).at(key).key) {

			int BU = inst->getBatteryUsed(s.routes.at(route).at(key).key, r.key); // inst->dist(r.key, b);
			dists.push_back(pair<int, int>(r.key, BU));
		}
	}

	partial_sort(dists.begin(), dists.begin() + m, dists.end(),
		[](pair<int, int> a, pair<int, int> b) {
			return a.second < b.second;
		});

	int n = Random::get(0, m - 1);

	return dists.at(n);
}

int perm_rep::minimalDeviationBSS(route r, int v)
{
	if (v < 0 || v > r.size() - 2) {
		throw "error on minimalDeviationBSS";
	}

		vector<node> R = inst->set_R();

	int minDev = INT_MAX;
	int bss = -1;

	for (auto i : R) {
		int dev1 = inst->getBatteryUsed(r.at(v).key, i.key);
		int dev2 = inst->getBatteryUsed(i.key, r.at(v + 1).key);
		int bLevel = r.at(v).bLevel + r.at(v).recharged;

		if (dev2 < minDev && dev1 < abs(bLevel)) {
			minDev = dev2;
			bss = i.key;
		}
	}

	return bss;	
}

int perm_rep::minimalDeviationBSS(int v1, int v2)
{
	vector<node> R = inst->set_R();

	node a = inst->getNodeByKey(v1);
	node b = inst->getNodeByKey(v2);

	int minDev = INT_MAX;
	int bss = -1;

	for (auto i : R) {
		int dev = inst->dist(a.key, i.key);
		dev += inst->dist(i.key, b.key);

		if (dev < minDev) {
			dev = minDev;
			bss = i.key;
		}
	}

	return bss;
}

pair<int, int> perm_rep::chooseBSS(route& rt, int beg, int end)
{
	vector<node> R = inst->set_R();
	int minBU = INT_MAX;
	int bss = 0;

	vector<pair<int, int>> stations;

	// get used stations (stations sub route)
	vector<vertex> usedStations;
	int i = beg;
	while (inst->getNodeByKey(rt.at(i).key).type == "f") {
		usedStations.push_back(rt.at(i));
		i--;
	}

	vector<node> available_r;
	for (node r : R) {
		bool find = false;
		for (vertex v : usedStations) {
			if (r.key == v.key) {
				find = true;
				break;
			}
		}
	
		int BU1 = inst->getBatteryUsed(rt.at(beg).key, r.key); // AS
		int bLevel = rt.at(beg).bLevel + rt.at(beg).recharged;
		if (find == false && BU1 < bLevel) {
			available_r.push_back(r);
		}
	}

	if (available_r.size() == 0) {
		throw Unreachable(beg, end);
	}
	else {
		// search for the closest bss from the stop node
		for (node r : available_r) {
			int BU1 = inst->getBatteryUsed(rt.at(beg).key, r.key); // AS
			int BU2 = inst->getBatteryUsed(r.key, rt.at(end).key); // SB

			// get the lowest dist
			if (BU2 < minBU && r.key != rt.at(beg).key) {
				bss = r.key;
				minBU = BU2;
			}
		}

		return pair<int, int>(bss, minBU);
	}	
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
	sol.front().vLoad = inst->c;
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

route perm_rep::computeRoute(Solution s, int r)
{
	return route();
}

node perm_rep::nearestBSS(int key)
{
	return node();
}

int perm_rep::availableRoute(Solution s, int n)
{

	return 0;
}

void perm_rep::computeNode(Solution& s, int r, int n)
{
	if (n < 1) {
		cout << "Error\n";
		return;
	}
	int prev = n - 1;
	int curr = n;
	node currNode = inst->getNodeByKey(s.routes.at(r).at(curr).key);
	node prevNode = inst->getNodeByKey(s.routes.at(r).at(prev).key);

	vertex& cVertex = s.routes.at(r).at(curr);
	vertex pVertex = s.routes.at(r).at(prev);

	// computing parameters

	// compute battery level in the current node
	float bUsed = inst->getBatteryUsed(pVertex.key, cVertex.key);
	//cout << prev << " - " << curr << " - " << bUsed << endl;
	cVertex.bLevel = pVertex.bLevel + pVertex.recharged - bUsed;

	if (cVertex.bLevel > inst->Q) {
		cout << cVertex.bLevel << endl;
	}

	if (currNode.type == "f" || currNode.type == "f_d") { // recharge in the bss
		cVertex.recharged = inst->Q - cVertex.bLevel;
		cVertex.recharge = true;
	}
	else { // recharge in the customer
		int recharge = inst->g * currNode.serviceTime;
		if (cVertex.bLevel + recharge > inst->Q) {
			cVertex.recharged = inst->Q - cVertex.bLevel;
			cVertex.recharge = true;
		}
		else {
			cVertex.recharged = recharge;
			cVertex.recharge = true;
		}

	}

	// compute arrival time in the current node
	float TD = inst->getTD(prevNode.key, currNode.key);
	cVertex.aTime = pVertex.lTime + TD;

	// compute waiting time in the current node
	if (cVertex.aTime < currNode.readyTime) {
		cVertex.wTime = currNode.readyTime - cVertex.aTime;
	}
	else { // no waiting time
		cVertex.wTime = 0;
	}

	// compute de departure time in the current node
	if (currNode.type == "f" || currNode.type == "f_d") {
		cVertex.lTime = cVertex.aTime + cVertex.wTime + inst->ct;
	}
	else {
		cVertex.lTime = cVertex.aTime + cVertex.wTime + currNode.serviceTime;
	}

	// compute vehcile load in the current node
	cVertex.vLoad = pVertex.vLoad - currNode.demand;

	s.routes.at(r).at(curr) = cVertex;
}

vertex perm_rep::computeVertex(vertex v1, vertex v2)
{
	//int prev = n - 1;
	//int curr = n;
	node currNode = inst->getNodeByKey(v2.key);
	node prevNode = inst->getNodeByKey(v1.key);

	vertex& cVertex = v2;
	vertex pVertex = v1;

	// computing parameters

	// compute battery level in the current node
	float bUsed = inst->getBatteryUsed(pVertex.key, cVertex.key);
	//cout << prev << " - " << curr << " - " << bUsed << endl;
	cVertex.bLevel = pVertex.bLevel + pVertex.recharged - bUsed;

	if (cVertex.bLevel > inst->Q) {
		cout << cVertex.bLevel << endl;
	}

	if (currNode.type == "f" || currNode.type == "f_d") { // recharge in the bss
		cVertex.recharged = inst->Q - cVertex.bLevel;
		cVertex.recharge = true;
	}
	else { // recharge in the customer
		int recharge = inst->g * currNode.serviceTime;
		if (cVertex.bLevel + recharge > inst->Q) {
			cVertex.recharged = inst->Q - cVertex.bLevel;
			cVertex.recharge = true;
		}
		else {
			cVertex.recharged = recharge;
			cVertex.recharge = true;
		}

	}

	// compute arrival time in the current node
	float TD = inst->getTD(prevNode.key, currNode.key);
	cVertex.aTime = pVertex.lTime + TD;

	// compute waiting time in the current node
	if (cVertex.aTime < currNode.readyTime) {
		cVertex.wTime = currNode.readyTime - cVertex.aTime;
	}
	else { // no waiting time
		cVertex.wTime = 0;
	}

	// compute de departure time in the current node
	if (currNode.type == "f" || currNode.type == "f_d") {
		cVertex.lTime = cVertex.aTime + cVertex.wTime + inst->ct;
	}
	else {
		cVertex.lTime = cVertex.aTime + cVertex.wTime + currNode.serviceTime;
	}

	// compute vehcile load in the current node
	cVertex.vLoad = pVertex.vLoad - currNode.demand;

	return cVertex;
}

vertex perm_rep::computeVertex(vertex v1, node n)
{
	//int prev = n - 1;
//int curr = n;
	node currNode = n;
	node prevNode = inst->getNodeByKey(v1.key);

	vertex cVertex;
	cVertex.key = n.key;

	vertex pVertex = v1;

	// computing parameters

	// compute battery level in the current node
	float bUsed = inst->getBatteryUsed(pVertex.key, cVertex.key);
	//cout << prev << " - " << curr << " - " << bUsed << endl;
	cVertex.bLevel = pVertex.bLevel + pVertex.recharged - bUsed;

	if (cVertex.bLevel > inst->Q) {
		cout << cVertex.bLevel << endl;
	}

	if (currNode.type == "f" || currNode.type == "f_d") { // recharge in the bss
		cVertex.recharged = inst->Q - cVertex.bLevel;
		cVertex.recharge = true;
	}
	else { // recharge in the customer
		int recharge = inst->g * currNode.serviceTime;
		if (cVertex.bLevel + recharge > inst->Q) {
			cVertex.recharged = inst->Q - cVertex.bLevel;
			cVertex.recharge = true;
		}
		else {
			cVertex.recharged = recharge;
			cVertex.recharge = true;
		}

	}

	// compute arrival time in the current node
	float TD = inst->getTD(prevNode.key, currNode.key);
	cVertex.aTime = pVertex.lTime + TD;

	// compute waiting time in the current node
	if (cVertex.aTime < currNode.readyTime) {
		cVertex.wTime = currNode.readyTime - cVertex.aTime;
	}
	else { // no waiting time
		cVertex.wTime = 0;
	}

	// compute de departure time in the current node
	if (currNode.type == "f" || currNode.type == "f_d") {
		cVertex.lTime = cVertex.aTime + cVertex.wTime + inst->ct;
	}
	else {
		cVertex.lTime = cVertex.aTime + cVertex.wTime + currNode.serviceTime;
	}

	// compute vehcile load in the current node
	cVertex.vLoad = pVertex.vLoad - currNode.demand;

	return cVertex;
}

void perm_rep::initDepot(vertex& v)
{
	/*
	v.aTime = 0;
	v.wTime = 0;
	v.lTime = 0;
	v.recharge = false;
	v.recharged = 0;
	v.bLevel = inst->Q;
	v.vLoad = inst->c;
	*/

	v.aTime = 0;
	v.lTime = 0;
	v.wTime = 0;
	v.vLoad = inst->c;
	v.bLevel = inst->Q;
	v.recharge = false;
	v.recharged = 0;
}

route perm_rep::removeBSS_beg_end(route r)
{
	// remove bss from the begin
	int curr = 0;
	while (true) {
		if (r.size() > 0) {
			if (inst->getNodeByKey(r.at(curr).key).type == "f") {
				r.erase(r.begin() + curr);
				//curr++;
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}

	// remove bss from the end
	curr = r.size() - 1;
	while (true) {
		if (r.size() > 0) {
			if (inst->getNodeByKey(r.at(curr).key).type == "f") {
				r.erase(r.begin() + curr);
				curr--;
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}

	return r;
}

route perm_rep::removeBSS_beg(route r)
{
	// remove bss from the begin
	int curr = 0;
	while (inst->getNodeByKey(r.at(curr).key).type == "f") {
		r.erase(r.begin() + curr);
	}

	return r;
}

route perm_rep::removeBSS_end(route r)
{
	// remove bss from the end
	int curr = r.size();
	while (inst->getNodeByKey(r.at(curr).key).type == "f") {
		r.erase(r.begin() + curr);
		curr--;
	}

	return r;
}

route perm_rep::removeBSS(route r)
{
	// remove all bss from the route
	int curr = 0;
	while (curr < r.size()) {
		if (inst->getNodeByKey(r.at(curr).key).type == "f") {
			r.erase(r.begin() + curr);
		}
		else{
			curr++;
		}
		
		if (r.size() == 0) {
			break;
		}
	}

	return r;
}

route perm_rep::removeBSS(route r, int pos)
{
	// remove all bss from the route
	int curr = pos;
	if (pos >= r.size()) {
		throw "error";
	}
	while (curr < r.size()) {
		if (inst->getNodeByKey(r.at(curr).key).type == "f") {
			r.erase(r.begin() + curr);
		}
		else {
			curr++;
		}

		if (r.size() == 0) {
			break;
		}
	}

	return r;
}

Solution perm_rep::localSearch(permutation p)
{
	Solution best = permutationToSolution(p);
	//best = procSol(best);
	best.perm = p;

	bool improv = true;
	while (improv == true) {
		improv = false;
		Solution curr = best;

		for (int i = 0; i < p.size() - 1; i++) {
			for (int j = i + 1; j < p.size(); j++) {
				// cout << i << " - " << j << endl;

				permutation pl = curr.perm;
				// swap nodes in the permutation
				int aux = pl.at(j);
				pl.at(j) = pl.at(i);
				pl.at(i) = aux;

				try {
					Solution sl = permutationToSolutionGrasp(pl);
					//sl = procSol(sl);

					if (sl.FO < best.FO) {
						best = sl;
						improv = true;
						//return best;
						break;
					}
				}
				catch (PermutationInf e) {
					cout << e.what() << endl;
					continue;
				}
				catch (exception e) {
					cout << e.what() << endl;
					continue;
				}
				
			}
			if (improv == true) {
				break;
			}
		}
		if (improv == true) {
			break;
		}
	}
	return best;
}


Solution perm_rep::localSearch_light(permutation p)
{
	Solution best = permutationToSolution(p);
	best = procSol(best);
	best.perm = p;

	bool improv = true;
	while (improv == true) {
		improv = false;
		Solution curr = best;

		for (int i = 0; i < p.size() - 1; i++) {
			for (int j = i + 1; j < p.size(); j++) {
				// cout << i << " - " << j << endl;

				permutation pl = curr.perm;
				// swap nodes in the permutation
				int aux = pl.at(j);
				pl.at(j) = pl.at(i);
				pl.at(i) = aux;

				try {
					Solution sl = permutationToSolution(pl);
					//sl = procSol(sl);

					if (sl.FO < best.FO) {
						best = sl;
						improv = true;
						//return best;
						break;
					}
				}
				catch (PermutationInf e) {
					cout << e.what() << endl;
					continue;
				}
				catch (exception e) {
					cout << e.what() << endl;
					continue;
				}

			}
			if (improv == true) {
				break;
			}
		}
		if (improv == true) {
			break;
		}
	}
	return best;
}

Solution perm_rep::localSearch2(permutation p)
{
	Solution best = permutationToSolution(p);
	best = procSol(best);
	best.perm = p;

	bool improv = true;
	while (improv == true) {
		improv = false;
		Solution curr = best;

		for (int i = 0; i < p.size() - 1; i++) {
			for (int j = i + 1; j < p.size(); j++) {
				permutation pl = curr.perm;

				// swap nodes in the permutation
				int aux = pl.at(j);
				pl.at(j) = pl.at(i);
				pl.at(i) = aux;

				Solution sl = permutationToSolution(pl, -1);
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

Solution perm_rep::GRASP(int maxIt, int maxRuntime)
{
	Solution s = greedDD();
	s = GRASP_p(s, 10, INT_MAX);

	row = "";
	row = getRow(s);

	s.saveXML(dirOutput + inst->fileName + ".xml");

	return s;
}

Solution perm_rep::sA(int initTemp, int finalTemp, float coolingRate, int maxIt, int maxRuntime)
{
	cout << "SA\n";

	int t0 = initTemp;
	int tf = finalTemp;
	this->maxRuntime = maxRuntime;

	Solution curr = greedDD();
	int FOINIT = curr.FO;

	//curr = permutationToSolution({ 9, 11, 6, 10, 7, 8, 5 });
	//int FOINIT = curr.FO;

	Solution best = curr;
	int temp = t0;

	cout << "initial solution: " << fixed << best.FO << endl;

	int b = best.FO;

	start = std::chrono::high_resolution_clock::now();
	auto t1 = std::chrono::high_resolution_clock::now();
	long long duration;

	do {
		bool improv = false;
		int counter = 0;
		cout << "temp: " << temp << endl;
		do {
			//cout << "it: " << counter << endl;
			// select a neighbor from the current solution
			permutation pl = curr.perm;

			if (pl.size() == 0) {
				pl = randomPermutation();
			}

			int beg = 0;
			int end = 0;
			if (pl.size() >= 2) {
				beg = Random::get(0, int(pl.size()) - 2);
				end = Random::get(beg + 1, int(pl.size()) - 1);
			}

			//cout << beg << " " << end << " " << pl.size() << endl;

			//cout << "a1" << endl;
			pl = opt2(pl, beg, end);
			//cout << "a2" << endl;
			try {				
				Solution nSol;
				//nSol = localSearch_light(pl);
				
				if (temp <= initTemp * 0.3) {
					//nSol = localSearch(pl);
					nSol = permutationToSolutionGrasp(pl);
				}
				else {
					nSol = localSearch_light(pl);				
				}
								
				//Solution nSol = permutationToSolution(pl);
				//nSol = bVNS_r(nSol);

				//cout << "a3" << endl;
				// store the best solution so far
				if (nSol.FO < best.FO && nSol.inf.size() == 0) {
					cout << "Improvment (SA): " << best.FO << " --> " << nSol.FO << endl;
					best = nSol;
					counter = 0;
				}

				// search-space exploration
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
			}
			catch (exception e) {
				cout << e.what() << endl;
			}
			catch (string s) {
				cout << s << endl;
			}


			// measure runtime
			auto t2 = std::chrono::high_resolution_clock::now();
			long long duration2 = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

			// stop if max runtime is reached
			if (duration2 > this->maxRuntime) {
				break;
			}

		} while (counter <= maxIt); // number of iterations without improvments

		temp -= coolingRate; // cooling

		// measure runtime
		auto t2 = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
		cout << "duration: " << duration << endl;

	} while (temp >= tf && duration < this->maxRuntime);


	auto t2 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
	best.runtime = duration;
	best.FOINIT = FOINIT;

	if (best.inf.size() == 0) {
		best.status = 1;
	}
	else {
		best.status = -1;
	}

	best.FOINIT = b;

	row = "";
	row = getRow(best);

	// get nodes
	for (route& r : best.routes) {
		for (vertex& v : r) {
			for (node n : inst->nodes) {
				if (v.key == n.key) {
					v.n = n;
					break;
				}
			}
		}
	}
	// get arcs
	for (route& r : best.routes) {
		for (int i = 0; i < r.size() - 1; i++) {
			arc a;
			a.beg = r.at(i).key;
			a.end = r.at(i + 1).key;
			a.value = inst->dist(a.beg, a.end);
			a.value2 = inst->getTD(a.beg, a.end);
			best.arcs.push_back(a);
		}
	}

	fstream op;
	op.open(dirOutput + inst->fileName, ios::app);
	if (op.is_open() == false) {
		cout << "error creating output file\n";
		return best;
	}
	else {
		best.debug(op);
	}

	best.saveXML(dirOutput + inst->fileName + ".xml");

	return best;
}

Solution perm_rep::bVNS(int itMax, int maxTime)
{
	maxRuntime = maxTime;// 300;
	start = std::chrono::high_resolution_clock::now();

	Solution init;
	try {
		init = greedDD();

		Solution best = bVNS_r(init, 25);
		getArcsNodes(best);

		auto t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
		best.runtime = duration;
		best.FOINIT = init.FO;

		if (best.inf.size() == 0) {
			best.status = 1;
		}
		else {
			best.status = -1;
		}

		row = "";
		row = getRow(best);

		fstream op;
		op.open(dirOutput + inst->fileName, ios::app);
		if (op.is_open() == false) {
			cout << "error creating output file\n";
			return best;
		}
		else {
			best.debug(op);
		}

		best.saveXML(dirOutput + inst->fileName + ".xml");

		return best;
	}
	catch (string e) {
		cout << e << endl;
		exit(1);
	}
}

Solution perm_rep::VNS(int itMax, int maxTime)
{
	Solution init;
	bool find = false;

	//Solution LB = lowerBound();

	try {
		init = greed();
		cout << fixed << "Initial objective: " << init.FO << endl;
		find = true;
	}
	catch (UnfeasibleInstance& e) {
		throw PermutationInf({0});			
	}
	catch (exception &e) {
		find = false;
		cout << e.what() << endl;
	}

	if (find == true) {
		//return init;
		return VNS(init, itMax, maxTime);
	}
	else {
		return init;
	}
}

Solution perm_rep::VNSL(vector<string> BSS, int itMax, int maxTime)
{

	inst->removeBSS(BSS); // remove unwanted BSSs
	//inst->removeDPT({ "Meltham" });

	Solution init;
	bool find = false;

	try {
		init = greed();
		cout << fixed << "Initial objective: " << init.FO << endl;
		find = true;
	}
	catch (UnfeasibleInstance& e) {
		throw PermutationInf({ 0 });
	}
	catch (exception& e) {
		find = false;
		cout << e.what() << endl;
	}

	//auto p = randomPermutation();
	//Solution init = permutationToSolution(p);
	
	if (find == true) {
		//return init;
		return VNS(init, itMax, maxTime);
	}
	else {
		return init;
	}
}

Solution perm_rep::lowerBound() {
	Solution s;

	vector<node> N = inst->set_UD0();
	vector<node> C = inst->set_C();
	vector<int> usedC;

	vector<pair<node, int>> depots;

	for (auto n : N) {

		int sum = 0;
		for (auto c : inst->set_C()) {
			sum += inst->distanceMatrix.at(n.key).at(c.key);
		}
		depots.push_back(pair<node, int>(n, sum));
	}

	sort(depots.begin(), depots.end(), [](pair<node, int> a, pair<node, int> b)->bool { return a.second < b.second; });

	N.clear();
	for (auto i : depots) {
		N.push_back(i.first);
	}	

	for (node d : N) {		

		usedC.clear();
		for (node c : C) {
			route r;
			vertex v;

			// depot
			v.key = d.key;
			initDepot(v);
			r.push_back(v);

			// customer
			v.key = c.key;
			r.push_back(v);
			r.at(1) = computeVertex(r.at(0), r.at(1));

			// arrival
			v.key = inst->getArrival(d.key);
			r.push_back(v);
			r.at(2) = computeVertex(r.at(1), r.at(2));

			auto inf = evalRoute_(r);
			
			//vector<string> aux = { "route_beg_end", "time_window", "battery_level", "vehicle_load", "consecutive_vertex" };
			vector<string> aux = { "route_beg_end", "time_window", "vehicle_load", "consecutive_vertex" };
			bool ok = true;
			for (auto i : aux) {
				for (auto j : inf) {
					if (i == j) {
						ok = false;
						break;
					}
				}
			}

			if (ok == true) {
				s.routes.push_back(r);
				usedC.push_back(r.at(1).key);
			}
		}

		// remove the used customers from the C vector
		for (int i : usedC) {
			bool find = false;
			for (int j = 0; j < C.size(); j++) {
				if (i == C.at(j).key) {
					C.erase(C.begin() + j);
					break;
				}
			}
		}

		if (C.size() == 0) {
			break;
		}
		
	}

	s = procSol(s);

	getArcsNodes(s);

	return s;

	// search for the closest depot
/*
int dptkey = -1;
int mindist = INT_MAX;
for (auto d : inst->set_UD0()) {
	int dist = inst->distanceMatrix.at(d.key).at(c.key);

	if (dist < mindist) {
		mindist = dist;
		dptkey = d.key;
	}
}
*/
}

vector<string> perm_rep::chooseBSS_(int p, string regionNameFile, string regionsCitiesFile, string dmfile, map<string, int> stFreq)
{
	string region;
	set<string> cities;
	map<pair<string, string>, float> distMatrix;

	string line;
	fstream file;

	// get region
	file.open(regionNameFile, ios::in);
	if (!file.is_open()) {
		cout << "error opening file " << regionNameFile << endl;
		exit(1);
	}
	getline(file, region);
	file.close();

	// get city list	
	file.open(regionsCitiesFile, ios::in);
	if (!file.is_open()) {
		cout << "error opening file " << regionsCitiesFile << endl;
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

	/*
	for (int i = 25; i <= 100; i += 10) {
		vector<string> BSS = chooseBSS(p, cities, distMatrix, stFreq);

		inst->removeBSS(BSS); // remove unwanted BSSs
		//inst->removeDPT({ "Meltham" });

		Solution init;
		bool find = false;

		try {
			init = greed();
			cout << fixed << "Initial objective: " << init.FO << endl;
			find = true;
		}
		catch (UnfeasibleInstance& e) {
			throw PermutationInf({ 0 });
		}
		catch (exception& e) {
			find = false;
			//cout << e.what() << endl;
		}

		//auto p = randomPermutation();
		//Solution init = permutationToSolution(p);

		if (find == true) {
			
		}
	}
	*/
	return chooseBSS(p, cities, distMatrix, stFreq);
}

vector<string> perm_rep::chooseBSS(int p, set<string> cities, map<pair<string, string>, float> dists, map<string, int> stFreq)
{
	//int p = 70;
	int n = ceil(stFreq.size() * (p / 100));
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

vector<string> perm_rep::chooseBSS_model(set<string> cities, map<pair<string, string>, float> dists, map<string, int> stFreq)
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

		return vector<string> ();
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}

Solution perm_rep::VNS(Solution s, int itMax, int maxTime)
{
	start = std::chrono::high_resolution_clock::now();
	// "insertR", "2opt", "shiftC", "changeD", "bssReplacement", "splitR", "unionR"

	//neighbors = { "insertR", "2opt", "shiftC" };
	//neighbors = { "shiftC", "changeD", "2opt", "insertR" };
	//neighbors = { "shiftC", "bssReplacement", "2opt", "insertR" };
	//neighbors = { "bssReplacement" };
	// neighbors = { "partR", "2opt", "shiftC", "swapD", "changeD", "splitR", "joinR" }; 
	neighbors = { "unionR", "insertR", "2opt", "shiftC", "bssReplacement", "changeD" };// , "insertR", "2opt", "shiftC", "changeD", "bssReplacement" }; // "swapD" , ,   
	//neighbors = { "unionR" }; // ok
	//neighbors = { "insertR" }; // ok	
	//neighbors = { "2opt" }; // ok
	//neighbors = { "shiftC" }; // ok
	//neighbors = { "bssReplacement" }; // ok?
	//neighbors = { "changeD" }; // ok
	
	//neighbors = { "2opt" };7
	//neighbors = { "unionR" };
	//neighbors = { "insertR" };
	//neighbors = { "shiftC" };
	//neighbors = { "changeD" };
	//neighbors = { "bssReplacement" };

	maxRuntime = maxTime;
	Solution init = s;
	Solution best = s;

	int it = 0;
	int n = 0;
	int maxN = neighbors.size() - 1;
	do {
		try {			
			long long duration_;
			
			auto currTime = std::chrono::high_resolution_clock::now();
			duration_ = std::chrono::duration_cast<std::chrono::seconds>(currTime - start).count();
			cout << fixed << setprecision(2) << best.FO << " - " << duration_ << " - ";

			int n = Random::get(0, maxN);
			cout << this->neighbors.at(n) << " - ";
			cout << "shake - ";
			Solution sl = shakeRandom_r(best, this->neighbors.at(n));
			#ifdef DEBUG_VNS
			cout << "VNS local search" << endl;
			auto t1_ = std::chrono::high_resolution_clock::now();
			#endif
			cout << "ls\n";
			sl = localSearch_r(sl, this->neighbors.at(n), false); // "2opt"
			#ifdef DEBUG_VNS
			auto t2_ = std::chrono::high_resolution_clock::now();
			duration_ = std::chrono::duration_cast<std::chrono::seconds>(t2_ - t1_).count();
			cout << "VNS local search time = " << duration_ << endl;
			#endif

			

			if (sl.FO < best.FO && sl.inf.size() == 0) {
				cout << fixed << setprecision(2) << "VNS improvement: " << best.FO << " --> " << sl.FO << " --> " << neighbors.at(n) << endl;

				best = sl;
				s = sl;
				it = 0;
				//n = 0;
				n = n;
			}
			else {
				it++;
				n++;
				int n = Random::get(0, maxN);
				/*
				if (n > maxN) {
					n = maxN;
				}
				*/
			}
		}
		catch (string str) {
			//cout << str << endl;
		}

		// measure runtime
		auto t2 = std::chrono::high_resolution_clock::now();
		long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

		// stop if max runtime is reached
		if (duration >= maxRuntime) {
			break;
		}

	} while (it < itMax);

	getArcsNodes(best);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
	best.runtime = duration;
	best.FOINIT = init.FO;

	if (best.inf.size() == 0) {
		best.status = 1;
	}
	else {
		best.status = -1;
	}

	row = "";
	row = getRow(best);

	fstream op;
	op.open(dirOutput + inst->fileName, ios::app);
	if (op.is_open() == false) {
		cout << "error creating output file\n";
		return best;
	}
	else {
		best.debug(op);
	}

	best.saveXML(dirOutput + inst->fileName + ".xml");

	return best;
}

Solution perm_rep::testPermutation(permutation p)
{
	p = randomPermutation();
	for (auto i : p) {
		cout << i << " ";
	}
	cout << endl;
	Solution s = permutationToSolution(p);
	s = procSol(s);

	// get nodes
	for (route& r : s.routes) {
		for (vertex& v : r) {
			for (node n : inst->nodes) {
				if (v.key == n.key) {
					v.n = n;
					break;
				}
			}
		}
	}
	// get arcs
	for (route& r : s.routes) {
		for (int i = 0; i < r.size() - 1; i++) {
			arc a;
			a.beg = r.at(i).key;
			a.end = r.at(i + 1).key;
			a.value = inst->dist(a.beg, a.end);
			a.value2 = inst->getTD(a.beg, a.end);
			s.arcs.push_back(a);
		}
	}
	return s;
}

map<string, int> perm_rep::getBSSFreq(vector<Solution> sols)
{
	map<string, int> stFreq;
	for (auto j : sols) {
		for (string i : j.sStations) {
			map<string, int>::iterator it;
			it = stFreq.find(i);
			if (it == stFreq.end()) {
				stFreq.insert({ i, 1 });
			}
			else {
				stFreq[i]++;
			}
		}
	}
	
	return stFreq;
}

void perm_rep::setOutputDir(string dir)
{
	dirOutput = dir;
}

bool perm_rep::getArcsNodes(Solution& s)
{
	// get nodes
	for (route& r : s.routes) {
		for (vertex& v : r) {
			for (node n : inst->nodes) {
				if (v.key == n.key) {
					v.n = n;
					break;
				}
			}
		}
	}
	// get arcs
	for (route& r : s.routes) {
		for (int i = 0; i < r.size() - 1; i++) {
			arc a;
			a.beg = r.at(i).key;
			a.end = r.at(i + 1).key;
			a.value = inst->dist(a.beg, a.end);
			a.value2 = inst->getTD(a.beg, a.end);
			s.arcs.push_back(a);
		}
	}

	return true;
}

Solution perm_rep::GRASP_p(Solution s, int maxIt, int maxRunTime)
{
	//cout << "GRASP\n";
	int n = inst->set_R().size();
	if (n >= 2) {
		n = floor(double(n) * 0.2) + 2;
	}
	else if (n == 1) {
		n = 1;
	}
	else if (n == 0) {
		// throw an error?
	}

	Solution best;
	best.FO = INT_MAX;

	int it = 0;
	while (it < maxIt) {
		//cout << "it: " << it << endl;
		//cout << "a" << endl;
		//Solution sl = addStations(s, n);
		Solution sl = addStationsAlt(s);
		//cout << "b" << endl;
		// local search
		#ifdef DEBUG_GRASP
		cout << "Grasp local search" << endl;
		auto t1_ = std::chrono::high_resolution_clock::now();
		#endif

		sl = bVNS_r(sl, 5);
		//sl = hillDescent_r(sl);

		#ifdef DEBUG_GRASP
		auto t2_ = std::chrono::high_resolution_clock::now();
		long long duration_ = std::chrono::duration_cast<std::chrono::seconds>(t2_ - t1_).count();
		cout << "Grasp local search time = " << duration_ << endl;
		#endif

		//sl = localSearch_r(sl, "2opt");
		//cout << "c" << endl;
		if (sl.FO < best.FO && sl.inf.size() == 0) {
			#ifdef DEBUG_GRASP
			cout << "improvment grasp: " << best.FO << " --> " << sl.FO << endl;		
			#endif			
			best = sl;
			it = 0;
		}
		else {
			it++;
		}

		auto t2 = std::chrono::high_resolution_clock::now();
		long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

		if (duration > maxRuntime) {
			break;
		}
	}
	best = procSol(best);
	#ifdef DEBUG_GRASP
		cout << "GRASP_END\n";
	#endif
	//
	return best;
}

Solution perm_rep::bVNS_r(Solution s, int itMax)
{
	//cout << "VNS\n";
	Solution best = s;
	neighbors = { "insertR", "2opt", "shiftC" };

	int it = 0;
	//int itMax = 5;
	int n = 1;
	int maxN = 3;

	//auto t1 = std::chrono::high_resolution_clock::now();

	do {
		//cout << "it: " << it << endl;
		try {
			//cout << 1 << endl;
			//cout << "shake " << n << endl;
			
			Solution sl = shakeRandom_r(best, neighbors.at(n - 1));
			//cout << 2 << endl;
			#ifdef DEBUG_VNS
			cout << "VNS local search" << endl;
			auto t1_ = std::chrono::high_resolution_clock::now();
			#endif

			sl = localSearch_r(sl, "2opt");
			//sl = localSearch_r(sl, "shiftC");

			#ifdef DEBUG_VNS
			auto t2_ = std::chrono::high_resolution_clock::now();
			long long duration_ = std::chrono::duration_cast<std::chrono::seconds>(t2_ - t1_).count();
			cout << "VNS local search time = " << duration_ << endl;
			#endif
			
			//cout << 3 << endl;

			if (sl.FO < best.FO && sl.inf.size() == 0) {

				#ifdef DEBUG_VNS
				cout << fixed << setprecision(2) << "VNS improvement: " << best.FO << " --> " << sl.FO << endl;
				#endif

				best = sl;
				s = sl;
				it = 0;
				n = 1;
			}
			else {
				it++;
				n++;
				if (n > maxN) {
					n = maxN;
				}
			}
		}
		catch (string str) {
			cout << str << endl;
		}

		// measure runtime
		auto t2 = std::chrono::high_resolution_clock::now();
		long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

		// stop if max runtime is reached
		if (duration >= maxRuntime) {
			break;
		}

	} while (it < itMax);

	//cout << "VNS_END\n";
	return best;
}

Solution perm_rep::rVNS_r(Solution s)
{
	Solution best = s;

	int it = 0;
	int itMax = 100;
	int maxRuntime = 300;
	int n = 1;
	int maxN = 3;

	auto t1 = std::chrono::high_resolution_clock::now();

	do {
		try {

			s = shakeRandom_r(s, neighbors.at(n - 1));
			s = localSearch_r(s, "2opt");
		}
		catch (string s) {
			cout << s << endl;
		}

		if (s.FO < best.FO && s.inf.size() == 0) {
			//cout << fixed << "improvment: " << best.FO << " --> " << s.FO << endl;
			best = s;
			it = 0;
			n = 1;
		}
		else {
			it++;
			n++;

			if (n > maxN) {
				n = maxN;
			}

		}

		// measure runtime
		auto t2 = std::chrono::high_resolution_clock::now();
		long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();

		// stop if max runtime is reached
		if (duration >= maxRuntime) {
			break;
		}

	} while (it < itMax);

	return best;
}

Solution perm_rep::localSearch_r_old(Solution s, string n)
{	
	#ifdef DEBUG_HILL_DESCENT
	cout << "localSearch_r\n";
	#endif // DEBUG
	
	float FO = s.FO;
	Solution best = s;
	//Solution curr = best;
	best = procSol(best);
	bool improv = true;
	
	int count = 0;
	while (improv == true) {
		//cout << count << endl;
		improv = false;
		if (n == "2opt") {
			for (int i = 0; i < s.routes.size(); i++) {
				vector<int> customers = getListC(s.routes.at(i));
				if (customers.size() < 2) {
					continue;
				}		
				//for (int a = 0; a < customers.size() - 1; a++) {
					//for (int b = a + 1; b < customers.size(); b++) {
				for (int b = customers.size() - 1; b >= 1; b--) {
					for (int a = 0; a < b - 1; a++) {
					
						try {
							Solution sl = opt2_route_r(s, i, customers.at(a), customers.at(b));

							#ifdef DEBUG_HILL_DESCENT
							auto t2 = std::chrono::high_resolution_clock::now();
							long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
							if (duration - prevTime > 5) {
								cout << "time: " << duration << endl;
								prevTime = duration;
							}							
							#endif // DEBUG

							if (sl.FO < best.FO && sl.inf.size() == 0) {
								#ifdef DEBUG_HILL_DESCENT
								cout << fixed << "improvment local search: " << best.FO << " - " << sl.FO << endl;
								#endif // DEBUG

								//
								best = sl;
								improv = true;

								// measure runtime
								auto t2 = std::chrono::high_resolution_clock::now();
								long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

								// stop if max runtime is reached
								if (duration >= maxRuntime) {
									return best;
								}

								break;
							}
						}
						catch (string str) {
							cout << str << endl;
							continue;
						}
						catch (exception e) {
							cout << e.what() << endl;
							continue;
						}

					}
					if (improv == true) {
						break;
					}
				}
				if (improv == true) {
					break;
				}
			}
			s = best;
		}
		else if (n == "shiftC") {

			for(int r = 0; r < s.routes.size(); r++) {
				vector<int> customers = getListC(s.routes.at(r));
				if (customers.size() < 2) {
					continue;
				}

				for (int i = 0; i < customers.size() - 1; i++) {
					for (int j = i + 1; j < customers.size(); j++) {

						int a = customers.at(i);
						int b = customers.at(j) - customers.at(i);

						try {
							Solution sl = shiftCustomer_r(s, r, a, b);

							if (sl.FO < best.FO && sl.inf.size() == 0) {
								cout << fixed << "improvment: " << sl.FO << " --> " << best.FO << endl;
								best = sl;
								improv = true;

								// measure runtime
								auto t2 = std::chrono::high_resolution_clock::now();
								long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

								// stop if max runtime is reached
								if (duration >= maxRuntime) {
									return best;
								}

								break;
							}
						}
						catch (string s) {
							cout << s << endl;
						}
					}
					if (improv == true) {
						break;
					}
				}				
				if (improv == true) {
					break;
				}
			}
			//s = best;
		}
		else {
			throw "invalid neighborhood structure " + n;
		}
		s = best;
		count++;
	}
	//cout << FO << " - " << best.FO << endl;
	#ifdef DEBUG_HILL_DESCENT
	cout << "end_localSearch_r\n";
	#endif // DEBUG
	
	return best;
}

Solution perm_rep::localSearch_r(Solution s, string n)
{
	//"insertR", "2opt", "shiftC", "swapD", "changeD", "splitR", "unionR", "bssReplacement"

	bool op = false;
	if (n == "2opt") {
		return hillDescent_r_2opt(s, op);
	}
	else if (n == "shiftC") {
		return hillDescent_r_shiftC(s, op);
	}
	else if (n == "insertR") {
		return hillDescent_r_insertR(s, op);
	}
	else if (n == "changeD") {
		return hillDescent_r_changeD(s, op);
	}
	else if (n == "bssReplacement") {
		return hillDescent_r_bssReplacement(s, op);
	}
	else if (n == "swapD") {
		return hillDescent_r_swapD(s, op);
	}
	else if (n == "changeD") {
		return hillDescent_r_changeD(s, op);
	}
	else if (n == "splitR") {
		//return hillDescent_r_splitR(s);
		return hillDescent_r_splitR(s, op);
	}
	else if (n == "unionR") {
		//return hillDescent_r_unionR(s);
		return hillDescent_r_unionR(s, op);
	}
	else {
		throw n + " is not a valid neighborhood structure";
	}
}

Solution perm_rep::localSearch_r(Solution s, string n, bool fimp)
{
	bool op = fimp;
	if (n == "2opt") {
		return hillDescent_r_2opt(s, op);
	}
	else if (n == "shiftC") {
		return hillDescent_r_shiftC(s, op);
	}
	else if (n == "insertR") {
		return hillDescent_r_insertR(s, op);
	}
	else if (n == "changeD") {
		//return s;
		//return hillDescent_r_changeD(s, op);
		return hillDescent_r_changeDV2(s, op);
	}
	else if (n == "bssReplacement") {
		return hillDescent_r_bssReplacement(s, op);
	}
	else if (n == "swapD") {
		return hillDescent_r_swapD(s, op);
	}
	else if (n == "changeD") {
		return hillDescent_r_changeD(s, op);
	}
	else if (n == "splitR") {
		//return hillDescent_r_splitR(s);
		return hillDescent_r_splitR(s, op);
	}
	else if (n == "unionR") {
		//return hillDescent_r_unionR(s);
		return hillDescent_r_unionR(s, op);
	}
	else {
		throw n + " is not a valid neighborhood structure";
	}
}

Solution perm_rep::hillDescent_r_2opt(Solution s, bool fimp)
{
	#ifdef DEBUG_HILL_DESCENT
	cout << "hillDescent_r_2opt\n";
	#endif // DEBUG

	float FO = s.FO;
	Solution best = s;
	//Solution curr = best;
	best = procSol(best);
	bool improv = true;

	int count = 0;
	while (improv == true) {
		//cout << count << endl;
		improv = false;
		for (int i = 0; i < s.routes.size(); i++) {
			vector<int> customers = getListC(s.routes.at(i));
			if (customers.size() < 2) {
				continue;
			}

			for (int b = customers.size() - 1; b >= 1; b--) {
				for (int a = 0; a < b - 1; a++) {

					try {
						Solution sl = opt2_route_r(s, i, customers.at(a), customers.at(b));

						#ifdef DEBUG_HILL_DESCENT
						auto t2 = std::chrono::high_resolution_clock::now();
						long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
						if (duration - prevTime > 5) {
							cout << "time: " << duration << endl;
							prevTime = duration;
						}
						#endif // DEBUG

						if (sl.FO < best.FO && sl.inf.size() == 0) {
							#ifdef DEBUG_HILL_DESCENT
							cout << fixed << "improvment local search: " << best.FO << " - " << sl.FO << endl;
							#endif // DEBUG

							//
							best = sl;
							improv = true;

							// measure runtime
							auto t2 = std::chrono::high_resolution_clock::now();
							long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

							// stop if max runtime is reached
							if (duration >= maxRuntime) {
								return best;
							}

							if (fimp == true) // first improv
								break;
						}
					}
					catch (string str) {
						cout << str << endl;
						continue;
					}
					catch (exception &e) {
						cout << e.what() << endl;
						continue;
					}

				}
				if (improv == true && fimp == true) {
					break;
				}
			}
			if (improv == true && fimp == true) {
				break;
			}
		}
		s = best;
		count++;
	}

	#ifdef DEBUG_HILL_DESCENT
	cout << "end_localSearch_r\n";
	#endif // DEBUG

	return best;
}

Solution perm_rep::hillDescent_r_shiftC(Solution s, bool fimp)
{
	#ifdef DEBUG_HILL_DESCENT
	cout << "hillDescent_r_shiftC\n";
	#endif // DEBUG

	float FO = s.FO;
	Solution best = s;
	//Solution curr = best;
	best = procSol(best);
	bool improv = true;

	int count = 0;
	while (improv == true) {
		improv = false;		
		for (int r = 0; r < s.routes.size(); r++) {
			vector<int> customers = getListC(s.routes.at(r));
			if (customers.size() < 2) {
				continue;
			}

			for (int i = 0; i < customers.size() - 1; i++) {
				for (int j = i + 1; j < customers.size(); j++) {

					int a = customers.at(i);
					int b = customers.at(j) - customers.at(i);

					try {
						Solution sl = shiftCustomer_r(s, r, a, b);

						if (sl.FO < best.FO && sl.inf.size() == 0) {
							#ifdef DEBUG_HILL_DESCENT
							cout << fixed << "improvment: " << sl.FO << " --> " << best.FO << endl;
							#endif // DEBUG
							best = sl;
							improv = true;

							// measure runtime
							auto t2 = std::chrono::high_resolution_clock::now();
							long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

							// stop if max runtime is reached
							if (duration >= maxRuntime) {
								return best;
							}

							if (fimp == true) // first improv
								break;
						}
					}
					catch (string s) {
						cout << s << endl;
					}
				}
				if (improv == true && fimp == true) {
					break;
				}
			}
			if (improv == true && fimp == true) {
				break;
			}
		}
		s = best;
		count++;
		if (improv == true && fimp == true) {
			break;
		}
	}
	#ifdef DEBUG_HILL_DESCENT
	cout << "end_hillDescent_r_shiftC\n";
	#endif // DEBUG

	return best;
}

Solution perm_rep::hillDescent_r_insertR(Solution s, bool fimp)
{
	#ifdef DEBUG_HILL_DESCENT
	cout << "hillDescent_r_insertR\n";
	#endif // DEBUG

	float FO = s.FO;
	Solution best = s;
	best = procSol(best);
	bool improv = true;

	int count = 0;
	while (improv == true) {
		improv = false;

			
		for (int a = 0; a < s.routes.size(); a++) {
			vector<int> customersA = getListC(s.routes.at(a));
			if (customersA.size() < 2) {
				continue;
			}
			for (int b = 0; b < s.routes.size(); b++) {
				vector<int> customersB = getListC(s.routes.at(b));
				if (a != b) {
					for (int k = 1; k < customersA.size(); k++) {
						for (int l = 0; l < customersB.size(); l++) {
							try {
								Solution sl = routeInsertion_r(s, a, customersA.at(k), b, customersB.at(l));

								#ifdef DEBUG_HILL_DESCENT
								auto t2 = std::chrono::high_resolution_clock::now();
								long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
								if (duration - prevTime > 5) {
									cout << "time: " << duration << endl;
									prevTime = duration;
								}
								#endif // DEBUG

								if (sl.FO < best.FO && sl.inf.size() == 0) {
									#ifdef DEBUG_HILL_DESCENT
									cout << fixed << "improvment local search: " << best.FO << " - " << sl.FO << endl;
									#endif // DEBUG

									//
									best = sl;
									improv = true;

									// measure runtime
									auto t2 = std::chrono::high_resolution_clock::now();
									long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

									// stop if max runtime is reached
									if (duration >= maxRuntime) {
										return best;
									}

									if (fimp == true) // first improv
										break;
								}
							}
							catch (string str) {
								//cout << str << endl;
								continue;
							}
							catch (exception &e) {
								//cout << e.what() << endl;
								continue;
							}
						}
						if (improv == true && fimp == true) {
							break;
						}
					}
					if (improv == true && fimp == true) {
						break;
					}
				}
			}
			if (improv == true && fimp == true) {
				break;
			}
		}
		s = best;
		count++;
		if (improv == true && fimp == true) {
			break;
		}
	}

	#ifdef DEBUG_HILL_DESCENT
	cout << "end_localSearch_r\n";
	#endif // DEBUG

	return best;
}

Solution perm_rep::hillDescent_r_changeD(Solution s, bool fimp)
{
	#ifdef DEBUG_HILL_DESCENT
	cout << "hillDescent_r_changeD\n";
	#endif // DEBUG

	float FO = s.FO;
	Solution best = s;
	best = procSol(best);
	bool improv = true;

	int count = 0;
	while (improv == true) {
		improv = false;

		for (node n : inst->set_UD0()){
			try {
				//cout << n.key << endl;
				Solution sl = changeDepot(s, n.key);

				#ifdef DEBUG_HILL_DESCENT
				auto t2 = std::chrono::high_resolution_clock::now();
				long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
				if (duration - prevTime > 5) {
					cout << "time: " << duration << endl;
					prevTime = duration;
				}
				#endif // DEBUG

				if (sl.FO < best.FO && sl.inf.size() == 0) {
					#ifdef DEBUG_HILL_DESCENT
					cout << fixed << "improvment local search: " << best.FO << " - " << sl.FO << endl;
					#endif // DEBUG

					//
					best = sl;
					improv = true;

					// measure runtime
					auto t2 = std::chrono::high_resolution_clock::now();
					long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

					// stop if max runtime is reached
					if (duration >= maxRuntime) {
						return best;
					}

					if (fimp == true) // first improv
						break;
				}
			}
			catch (string str) {
				cout << str << endl;
				continue;
			}
			catch (exception e) {
				cout << e.what() << endl;
				continue;
			}
			if (improv == true && fimp == true) {
				break;
			}
		}
		s = best;
		count++;
		if (improv == true && fimp == true) {
			break;
		}
	}

	#ifdef DEBUG_HILL_DESCENT
	cout << "end_localSearch_r\n";
	#endif // DEBUG

	return best;
}

Solution perm_rep::hillDescent_r_changeDV2(Solution s, bool fimp)
{
	#ifdef DEBUG_HILL_DESCENT
	cout << "hillDescent_r_changeDV2\n";
	#endif // DEBUG

	float FO = s.FO;
	Solution best = s;
	best = procSol(best);
	bool improv = true;

	int count = 0;
	while (improv == true) {
		if (inst->set_UD0().size() == 1) {
			return s;
		}

		improv = false;

		set<int> useddpts;
		for (route r : s.routes) {
			useddpts.insert(r.front().key);
		}

		int keyA = Random::get<int>(0, useddpts.size() - 1);
		set<int>::iterator it = useddpts.begin();
		advance(it, keyA);
		keyA = *it;

		set<int> availabedpts;
		for (node i : inst->set_UD0()) {
			bool find = false;
			for (int j : useddpts) {
				if (i.key != j) {
					find = true;
				}
			}
			if (find == false) {
				availabedpts.insert(i.key);
			}
		}
		for (int i : useddpts) {
			if (i != keyA) {
				availabedpts.insert(i);
			}
		}

		if (useddpts.size() < 1 || availabedpts.size() < 1) {
			cout << "error\n"; /////////////
		}

		int keyB = Random::get<int>(0, availabedpts.size() - 1);
		it = availabedpts.begin();
		advance(it, keyB);
		keyB = *it;

		for (int i : useddpts) {
			for (int j : availabedpts) {
				try {
					Solution sl = changeDepotV2(s, i, j);

					#ifdef DEBUG_HILL_DESCENT
					auto t2 = std::chrono::high_resolution_clock::now();
					long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
					if (duration - prevTime > 5) {
						cout << "time: " << duration << endl;
						prevTime = duration;
					}
					#endif // DEBUG

					if (sl.FO < best.FO && sl.inf.size() == 0) {
						#ifdef DEBUG_HILL_DESCENT
						cout << fixed << "improvment local search: " << best.FO << " - " << sl.FO << endl;
						#endif // DEBUG

						//
						best = sl;
						improv = true;

						// measure runtime
						auto t2 = std::chrono::high_resolution_clock::now();
						long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

						// stop if max runtime is reached
						if (duration >= maxRuntime) {
							return best;
						}

						if (fimp == true) // first improv
							break;
					}
				}
				catch (string str) {
					cout << str << endl;
					continue;
				}
				catch (exception e) {
					cout << e.what() << endl;
					continue;
				}
				if (improv == true && fimp == true) {
					break;
				}
			}
			if (improv == true && fimp == true) {
				break;
			}
		}
		s = best;
		count++;
		if (improv == true && fimp == true) {
			break;
		}
	}

	#ifdef DEBUG_HILL_DESCENT
	cout << "end_localSearch_r\n";
	#endif // DEBUG

	return best;
}

Solution perm_rep::hillDescent_r_splitR(Solution s, bool fimp)
{
	#ifdef DEBUG_HILL_DESCENT
	cout << "hillDescent_r_splitR\n";
	#endif // DEBUG

	float FO = s.FO;
	Solution best = s;
	best = procSol(best);
	bool improv = true;

	int count = 0;
	while (improv == true) {
		improv = false;

		for (int i = 0; i < s.routes.size(); i++) {
			auto customers = getCustomersPos(s.routes.at(i));

			for (int j = 1; j < customers.size() - 1; j++) {
				try {
					Solution sl = routeSplit_r(s, i, customers.at(j).second);

					#ifdef DEBUG_HILL_DESCENT
					auto t2 = std::chrono::high_resolution_clock::now();
					long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
					if (duration - prevTime > 5) {
						cout << "time: " << duration << endl;
						prevTime = duration;
					}
					#endif // DEBUG

					if (sl.FO < best.FO && sl.inf.size() == 0) {
						#ifdef DEBUG_HILL_DESCENT
						cout << fixed << "improvment local search: " << best.FO << " - " << sl.FO << endl;
						#endif // DEBUG

						//
						best = sl;
						improv = true;

						// measure runtime
						auto t2 = std::chrono::high_resolution_clock::now();
						long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

						// stop if max runtime is reached
						if (duration >= maxRuntime) {
							return best;
						}

						if (fimp == true) // first improv
							break;
				}
			}
				catch (string str) {
					cout << str << endl;
					continue;
				}
				catch (exception e) {
					cout << e.what() << endl;
					continue;
				}
			}
			if (improv == true && fimp == true) {
				break;
			}
		}
		s = best;
		count++;
		if (improv == true && fimp == true) {
			break;
		}
	}

	#ifdef DEBUG_HILL_DESCENT
	cout << "end_localSearch_r\n";
	#endif // DEBUG

	return best;
}

Solution perm_rep::hillDescent_r_unionR(Solution s, bool fimp)
{
	#ifdef DEBUG_HILL_DESCENT
	cout << "hillDescent_r_unionR\n";
	#endif // DEBUG

	float FO = s.FO;
	Solution best = s;
	best = procSol(best);
	bool improv = true;

	int count = 0;
	while (improv == true) {
		improv = false;

		for (int i = 0; i < s.routes.size() - 1; i++) {
			auto customers_r1 = getCustomersPos(s.routes.at(i));

			for (int j = i + 1; j < s.routes.size(); j++) {
				auto customers_r2 = getCustomersPos(s.routes.at(j));

				try {
					//Solution sl = routeUnion_r(s, i, j, customers_r1.at(k).second, customers_r2.at(l).second);
					Solution sl = routeUnion_r(s, i, j);

					if (sl.FO < best.FO && sl.inf.size() == 0) {
						//
						best = sl;
						improv = true;

						// measure runtime
						auto t2 = std::chrono::high_resolution_clock::now();
						long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

						// stop if max runtime is reached
						if (duration >= maxRuntime) {
							return best;
						}

						if (fimp == true) // first improv
							break;
					}
					
				}
				catch (string str) {
					cout << str << endl;
					continue;
				}
				catch (exception &e) {
					//cout << e.what() << endl;
					continue;
				}
			}
			if (improv == true && fimp == true) {
				break;
			}
			s = best;
			count++;
		}
		if (improv == true && fimp == true) {
			break;
		}
	}

	return best;
}

Solution perm_rep::hillDescent_r_swapD(Solution s, bool fimp)
{
	#ifdef DEBUG_HILL_DESCENT
	cout << "hillDescent_r_swapD\n";
	#endif // DEBUG

	float FO = s.FO;
	Solution best = s;
	best = procSol(best);
	bool improv = true;

	int count = 0;
	while (improv == true) {
		improv = false;
		for (int a = 0; a < s.routes.size(); a++) {			
			try {
				Solution sl = swapDepot(s, a, a);

				#ifdef DEBUG_HILL_DESCENT
				auto t2 = std::chrono::high_resolution_clock::now();
				long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
				if (duration - prevTime > 5) {
					cout << "time: " << duration << endl;
					prevTime = duration;
				}
				#endif

				if (sl.FO < best.FO && sl.inf.size() == 0) {
					#ifdef DEBUG_HILL_DESCENT
					cout << fixed << "improvment local search: " << best.FO << " - " << sl.FO << endl;
					#endif // DEBUG

					//
					best = sl;
					improv = true;

					// measure runtime
					auto t2 = std::chrono::high_resolution_clock::now();
					long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

					// stop if max runtime is reached
					if (duration >= maxRuntime) {
						return best;
					}

					if (fimp == true) // first improv
						break;
				}
			}
			catch (string str) {
				cout << str << endl;
				continue;
			}
			catch (exception e) {
				cout << e.what() << endl;
				continue;
			}
		}
		s = best;
		count++;
		if (improv == true && fimp == true) {
			break;
		}
	}

	#ifdef DEBUG_HILL_DESCENT
	cout << "end_localSearch_r\n";
	#endif // DEBUG

	return best;
}

Solution perm_rep::hillDescent_r_bssReplacement(Solution s, bool fimp)
{
	#ifdef DEBUG_HILL_DESCENT
	cout << "hillDescent_r_swapD\n";
	#endif // DEBUG

	float FO = s.FO;
	Solution best = s;
	best = procSol(best);
	bool improv = true;

	int count = 0;
	while (improv == true) {
		improv = false;
		vector<int> stations;
		for (auto i : s.stations) {
			stations.push_back(i);
		}

		for (int i = 0; i < s.stations.size(); i++) {
			try {
				bool find = true;
				Solution sl;
				try {
					sl = bssReplacement_r(s, stations.at(i));
				}
				catch (exception e) {
					find = false;
					cout << e.what() << endl;
				}

				#ifdef DEBUG_HILL_DESCENT
				auto t2 = std::chrono::high_resolution_clock::now();
				long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
				if (duration - prevTime > 5) {
					cout << "time: " << duration << endl;
					prevTime = duration;
				}
				#endif // DEBUG

				if (sl.FO < best.FO && sl.inf.size() == 0 && find == true) {
					#ifdef DEBUG_HILL_DESCENT
					cout << fixed << "improvment local search: " << best.FO << " - " << sl.FO << endl;
					#endif // DEBUG

					//
					best = sl;
					improv = true;

					// measure runtime
					auto t2 = std::chrono::high_resolution_clock::now();
					long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

					// stop if max runtime is reached
					if (duration >= maxRuntime) {
						return best;
					}

					if (fimp == true) // first improv
						break;
				}
			}
			catch (string str) {
				cout << str << endl;
				continue;
			}
			catch (exception e) {
				cout << e.what() << endl;
				continue;
			}
		}
		s = best;
		count++;

	}
	#ifdef DEBUG_HILL_DESCENT
	cout << "end_localSearch_r\n";
	#endif // DEBUG

	return best;
}

Solution perm_rep::hillDescent_r(Solution s)
{
	#ifdef DEBUG_HILL_DESCENT
	cout << "localSearch_r\n";
	#endif // DEBUG

	float FO = s.FO;
	Solution best = s;
	best = procSol(best);
	bool improv = true;

	while (improv == true) {
		improv = false;
		
		for (int i = 0; i < s.routes.size(); i++) {
			vector<int> customers = getListC(s.routes.at(i));
			if (customers.size() < 2) {
				continue;
			}
			for (int b = customers.size() - 1; b >= 1; b--) {
				for (int a = 0; a < b - 1; a++) {
					try {

						Solution sl = opt2_route_r(s, i, customers.at(a), customers.at(b));

						#ifdef DEBUG
						auto t2 = std::chrono::high_resolution_clock::now();
						long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();
						if (duration - prevTime > 5) {
							cout << "time: " << duration << endl;
							prevTime = duration;
						}
						#endif // DEBUG

						if (sl.FO < best.FO && sl.inf.size() == 0) {
							#ifdef DEBUG_HILL_DESCENT
							cout << fixed << "improvment local search: " << best.FO << " - " << sl.FO << endl;
							#endif // DEBUG

							//
							best = sl;
							improv = true;

							// measure runtime
							auto t2 = std::chrono::high_resolution_clock::now();
							long long duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - start).count();

							// stop if max runtime is reached
							if (duration >= maxRuntime) {
								return best;
							}

							break;
						}
					}
					catch (string str) {
						cout << str << endl;
						continue;
					}
					catch (exception e) {
						cout << e.what() << endl;
						continue;
					}

				}
				if (improv == true) {
					break;
				}
			}
			if (improv == true) {
				break;
			}
		}
		s = best;
	}

	#ifdef DEBUG_HILL_DESCENT
	cout << "end_localSearch_r\n";
	#endif // DEBUG

	return best;
}

Solution perm_rep::routeSplit_r(Solution s, int rt, int pos)
{
	route r;
	route new_r;

	r.push_back(s.routes.at(rt).front());
	new_r.push_back(s.routes.at(rt).front());

	for (int i = 0; i < pos; i++) {
		vertex v = s.routes.at(rt).at(i);
		if (inst->getNodeByKey(v.key).type == "c") {
			r.push_back(v);
		}		
	}
	for (int i = pos; i < s.routes.at(rt).size(); i++) {
		vertex v = s.routes.at(rt).at(i);
		if (inst->getNodeByKey(v.key).type == "c") {
			new_r.push_back(v);
		}
	}

	vertex arrival = r.front();
	arrival.key = inst->getArrival(r.front().key);
	arrival.n = inst->getNodeByKey(arrival.key);

	r.push_back(arrival);
	new_r.push_back(arrival);

	r = addStations(r);
	new_r = addStations(new_r);

	s.routes.at(rt) = r;
	s.routes.push_back(new_r);	

	s = procSol(s);

	return s;
}

Solution perm_rep::routeUnion_r(Solution s, int r1, int r2)
{
	route r;
	vector<vertex> a_;
	vector<vertex> customers;

	vertex dpt;
	if (s.routes.size() >= 1) {
		if (s.routes.at(0).size() >= 1) {
			dpt = s.routes.at(0).at(0);
			initDepot(dpt);
		}
	}
	else {
		return s;
	}

	// get every customers in routes r1 and r2
	a_ = getCustomers(s.routes.at(r1));
	customers = a_;
	a_ = getCustomers(s.routes.at(r2));
	for (auto a : a_) {
		customers.push_back(a);
	}

	// erase routes r1 and r2
	if (r1 < r2) {
		s.routes.erase(s.routes.begin() + r1);
		s.routes.erase(s.routes.begin() + r2 - 1);
	}
	else {
		s.routes.erase(s.routes.begin() + r1);
		s.routes.erase(s.routes.begin() + r2);
	}	

	//vertex dpt = s.routes.front().front();
	//initDepot(dpt);

	route aux;
	vector<route> rts;
	aux.push_back(dpt);
	rts.push_back(aux);

	while (customers.size() > 0) {
		
		bool ins = false;
		bool newRoute = false;
		for (route& rt : rts) {

			// get closest customer
			int minBU = 99999999;
			vertex minC;
			
			int pos = 0;
			int i = 0;
			for (vertex c : customers) {
				if (rt.size() == 0) {
					cout << "FUDEU!!!\n";
				}
				int BU = inst->getBatteryUsed(rt.back().key, c.key);
				if (BU < minBU) {
					minBU = BU;
					minC = c;
					pos = i;
				}
				i++;
			}			
			minC = computeVertex(rt.at(rt.size() - 1), minC);
			node n = inst->getNodeByKey(minC.key);
			minC.n = n;

			bool find = false;
			for (auto c : customers) {
				if (c.key == minC.key) {
					find = true;
				}
			}
			if (find == false) {
				cout << "FUDEU MUITO!\n";
			}

			if (minC.vLoad > 0 && minC.aTime + n.serviceTime < n.dueDate) {
				rt.push_back(minC);
				ins = true;

				bool remove = false;
				for (int i = 0; i < customers.size(); i++) {
					if (customers.at(i).key == minC.key) {
						customers.erase(customers.begin() + i);
						remove = true;
						break;
					}
				}
				if (remove == false) {
					cout << "cant remove " << minC.key << " from ";
					for (auto j : customers) {
						cout << j.key << " ";
					}
					cout << endl;
					cout << "FUDEU!\n";
				}

				break;
			}	
		}

		if (ins == false && rts.back().size() == 1) { // movement is impossible
			return s;
		}
		else if (ins == false) {
			route temp;
			temp.push_back(rts.front().front());
			rts.push_back(temp);
			newRoute = true;
		}
	}

	for (route& rt : rts) {
		vertex v = rt.back();
		v.key = inst->getArrival(rt.front().key);
		rt.push_back(v);

		try {
			rt = addStations(rt);
		}
		catch (OutOfBSS &e) {
			return s;
		}
		s.routes.push_back(rt);
	}	

	Solution sl = procSol(s);

	if (sl.inf.size() == 0) {
		return sl;
	}
	else {
		return s;
	}	
}

// takes part of route a and insert into route b
Solution perm_rep::routeInsertion_r(Solution s, int a, int pa, int b, int pb)
{
	try {
		if (a == b) {
			return s;
			//throw "same_route";
		}
		if (pa > s.routes.at(a).size() - 1) {
			return s;
			//throw "cant_split_route_a";
		}
		if (pb > s.routes.at(b).size() - 1) {
			return s;
			//throw "cant_split_route_b";
		}

		route r1 = s.routes.at(a);
		route r2 = s.routes.at(b);

		vertex arrival = r1.back(); // store the arrival route r1 node
		r1.erase(r1.begin() + r1.size() - 1); // remove the last (arrival) node

		// get the part of the route r1 (subr) that will change to route r2
		route subr;

		int qt = r1.size() - pa;
		int i = 0;
		while (i < qt) {
			//for (int i = pa; i < r1.size(); i++) {
			vertex v = r1.at(pa);
			r1.erase(r1.begin() + pa);

			subr.push_back(v);
			i++;
		}

		// must remove bss from the beggin and the end of sub
		// end
		subr = removeBSS_beg_end(subr);

		if (subr.size() == 0) {
			//cout << "fudeu" << endl;
		}

		// remove bss from the route's end if there is any
		r1 = removeBSS_beg_end(r1);

		r1.push_back(arrival); // insert back the depot arrival node

		try {
			r1 = addStations(r1); // rearrange the first route
		}
		catch (IsolatedNode& e) {
			throw e;
		}
		catch (exception& e) {
			throw e;
		}

		arrival = r2.back(); // backup arrival route r1 arrival node 
		r2.erase(r2.begin() + r2.size() - 1); // remove the last (arrival) node

		// insert subroute in route r2
		for (auto i : subr) {
			r2.push_back(i);
		}

		r2.push_back(arrival); // insert back the arrival node in route r2

		try {
			r2 = addStations(r2);
		}
		catch (IsolatedNode& e) {
			throw e;
		}
		catch (exception& e) {
			throw e;
		}

		if (inst->getNodeByKey(r2.back().key).type != "a") {
			while (inst->getNodeByKey(r2.back().key).type != "a") {
				r2.erase(r2.end() - 1);
			}
		}

		if (inst->getNodeByKey(r1.back().key).type != "a") {
			return s;
			//throw "error";
		}
		if (inst->getNodeByKey(r2.back().key).type != "a") {
			return s;
			//throw "error";
		}

		s.routes.at(a) = r1;
		s.routes.at(b) = r2;

		s = procSol(s);

		//cout << "end\n";
		return s;
	}
	catch (exception &e) {
		//cout << e.what() << endl;
		return s;
		//throw e;
		//throw "error generating 2opt neighborhood with parameteres " + to_string(a) + " " + to_string(pa) + " " + to_string(b) + " " + to_string(pb);
	}
}

// 2opt
Solution perm_rep::opt2_route_r(Solution s, int j, int beg, int end)
{
	try {
		//cout << "2opt\n";

		if (beg >= end) {
			//throw "cant 2opt route with beg " + to_string(beg) + " and end " + to_string(end);
			throw BegEnd(beg, end);
		}
		if (j < 0 || j >= s.routes.size()) {
			//throw "route " + to_string(j) + " is out of bound";
			throw InvalidRoute(j);
		}
		if (inst->getNodeByKey(s.routes.at(j).back().key).type != "a") {
			throw InvalidRouteEnd(j, end);
		}

		route rt = s.routes.at(j);
		route prev, next, subr;

		int i = 0;
		for (i = 0; i < beg; i++) {
			if (i >= rt.size()) {
				throw "error";
			}
			prev.push_back(rt.at(i));
		}
		for (i; i <= end; i++) {
			if (i >= rt.size()) {
				throw "error";
			}
			subr.push_back(rt.at(i));
		}
		for (i; i < rt.size(); i++) {
			if (i >= rt.size()) {
				throw "error";
			}
			next.push_back(rt.at(i));
		}

		prev = removeBSS(prev);
		subr = removeBSS(subr);// remove bss
		next = removeBSS(next);

		if (next.size() == 0) {
			next.push_back(subr.back());
			subr.erase(subr.begin() + subr.size() - 1);
			//throw "error";
		}

		// invert subroute
		int a = 0, b = subr.size() - 1;
		while (a < b) {
			vertex v = subr.at(a);
			subr.at(a) = subr.at(b);
			subr.at(b) = v;
			a++; b--;
		}

		// build new route
		rt.clear();
		for (int j = 0; j < prev.size(); j++) {
			rt.push_back(prev.at(j));
		}
		for (int j = 0; j < subr.size(); j++) {
			rt.push_back(subr.at(j));
		}
		for (int j = 0; j < next.size(); j++) {
			rt.push_back(next.at(j));
		}

		try {
			rt = addStations(rt);
		}
		catch (IsolatedNode& e) {
			return s;
			//cout << e.what() << endl;
		}
		catch (exception& e) {
			return s;
			//cout << e.what() << endl;
		}

		if (inst->getNodeByKey(rt.back().key).type != "a") {
			//cout << "fudeu\n";
		}

		if (inst->getNodeByKey(rt.back().key).type != "a") {
			while (inst->getNodeByKey(rt.back().key).type != "a") {
				rt.erase(rt.end() - 1);
			}

		}

		if (inst->getNodeByKey(s.routes.at(j).back().key).type != "a") {
			throw "error";
		}

		//s = fixSol(s, rt, j);
		s.routes.at(j) = rt; /////////////////////////////
		s = procSol(s);

		//cout << "end\n";
		return s;
	}
	catch (exception &e) {
		//cout << e.what() << endl;
		//throw "error generating 2opt neighborhood with parameteres " + to_string(j) + " " + to_string(beg) + " " + to_string(end);
		throw e;
	}
	catch (string str) {
		//cout << str << endl;
		throw "error generating 2opt neighborhood with parameteres " + to_string(j) + " " + to_string(beg) + " " + to_string(end);
	}
}

// shift a customer by a given number of positions in the route
Solution perm_rep::shiftCustomer_r(Solution s, int j, int c, int q)
{
	try {
		//cout << "shift_customer\n";

		route rt = s.routes.at(j);

		if (c + q > rt.size() - 2) {
			throw "cant shift customer " + to_string(c) + " in " + to_string(q) + " positions";
		}

		if (inst->getNodeByKey(rt.at(c).key).type != "c") {
			throw "node in position " + to_string(c) + " in the route " + to_string(j) + " is not a customer";
		}

		vertex v = rt.at(c); // store the customer

		rt.erase(rt.begin() + c); // remove customer

		rt.insert(rt.begin() + q, v);

		rt = removeBSS(rt, c); /////

		try {
			rt = addStations(rt);
		}
		catch (exception& e) {
			return s;
		}

		if (inst->getNodeByKey(rt.back().key).type != "a") {
			//cout << "fudeu\n";
		}

		while (inst->getNodeByKey(rt.back().key).type != "a") {
			rt.erase(rt.end() - 1);
		}

		s.routes.at(j) = rt;

		s = procSol(s);

		//cout << "end\n";
		return s;
	}
	catch (exception &e) {
		cout << e.what() << endl;
		return s;
		//throw "error generating 2opt neighborhood with parameteres " + to_string(j) + " " + to_string(c) + " " + to_string(q);
	}

}

// change the solutions depot
Solution perm_rep::changeDepot(Solution s, int keyDPT)
{
	Solution sl = s;

	auto UD0 = inst->set_UD0();
	if (UD0.size() == 1) {
		return s;
	}
	if (!inst->isDepot(keyDPT)) {
		//cout << to_string(keyDPT) + " is not a depot" << endl;
		return s;
	}

	int i = 0;
	for (route& rt : sl.routes) {
		rt.front().key = keyDPT;
		rt.front().aTime = 0;
		rt.front().lTime = 0;
		rt.front().wTime = 0;
		rt.front().vLoad = inst->c;
		rt.front().bLevel = inst->Q;
		rt.front().recharge = false;
		rt.front().recharged = 0;
		rt.back().key = inst->getArrival(keyDPT);
		rt = removeBSS(rt);
		i++;
	}

	try {
		sl = addStations(sl);
		sl = procSol(sl);		
	}
	catch (OutOfBSS& e) {
		return s;
	}
	catch (IsolatedNode& e) {
		return s;
	}
	catch (exception& e) {
		return s;
	}
	return sl;
}

Solution perm_rep::changeDepotV2(Solution s, int keyA, int keyB) // troca todas ocorrencias do deposito A pelo B
{
	Solution sl = s;

	auto UD0 = inst->set_UD0();
	if (UD0.size() == 1) {
		return s;
	}
	if (!inst->isDepot(keyA) || !inst->isDepot(keyB)) {
		//cout << to_string(keyDPT) + " is not a depot" << endl;
		return s;
	}
	if (keyA == keyB) {
		return s;
	}

	int i = 0;
	for (route& rt : sl.routes) {
		if (rt.front().key == keyA) {
			rt.front().key = keyB;
			rt.front().aTime = 0;
			rt.front().lTime = 0;
			rt.front().wTime = 0;
			rt.front().vLoad = inst->c;
			rt.front().bLevel = inst->Q;
			rt.front().recharge = false;
			rt.front().recharged = 0;
			rt.back().key = inst->getArrival(keyB);			
		}
		rt = removeBSS(rt);
		i++;
	}

	try {
		sl = addStations(sl);
		sl = procSol(sl);
	}
	catch (OutOfBSS& e) {
		return s;
	}
	catch (IsolatedNode& e) {
		return s;
	}
	catch (exception& e) {
		return s;
	}
	return sl;
}

//
Solution perm_rep::swapDepot(Solution s, int kDPTA, int kDPTB)
{
	try {
		auto UD0 = inst->set_UD0();
		if (UD0.size() == 1) {
			throw "only onde depot";
		}
		if (!inst->isDepot(kDPTA)) {
			throw to_string(kDPTA) + " is not a depot";
		}
		if (!inst->isDepot(kDPTB)) {
			throw to_string(kDPTB) + " is not a depot";
		}

		set<int> dpts;
		bool find = false;
		for (auto i : s.routes) {
			dpts.insert(i.front().key);
			if (i.front().key == kDPTA) {
				find = true;
			}
		}
		if (find == false) {
			throw "depot " + to_string(kDPTA) + " is not currently in the solution";
		}

		for (route& rt : s.routes) {
			if (rt.front().key == kDPTA) {
				rt.front().key = kDPTB;
				rt.back().key = inst->getArrival(kDPTB);
				rt = removeBSS(rt);
				rt = addStations(rt);
			}
		}

		s = procSol(s);

		return s;
	}
	catch (exception e) {
		cout << e.what() << endl;
		throw "error generating 2opt neighborhood with parameteres " + to_string(kDPTA) + " " + to_string(kDPTB);
	}
}

Solution perm_rep::bssReplacement_r_(Solution s, int key)
{
	bool found = false;
	set<int> bss;
	// get a list of all bss beeing used
	for (route r : s.routes) {
		for (vertex v : r) {
			node curr = inst->getNodeByKey(v.key);
			if ((curr.type == "f" || curr.type == "f_d") && curr.key != key) {
				bss.insert(v.key);
			}
			if (curr.key == key) {
				found = true;
			}
		}
	}
	if (found == false) {
		throw "bss " + to_string(key) + " is not used in the given solution";
	}

	for (int i = 0; i < s.routes.size(); i++) {
		for (int j = 1; j < s.routes.at(i).size() - 1; j++) {
			if (s.routes.at(i).at(j).key == key) { // try to replace the bss

				// search for a replacement 
				for (int b : bss) {
					// check if the bss can fit the route
					vertex prev = s.routes.at(i).at(j - 1);
					vertex next = s.routes.at(i).at(j + 1);

					int dist_prev_repl = inst->dist(prev.key, b);
					int time_prev_repl = inst->getTD(s.routes.at(i).at(j - 1).key, b);

					vertex repl;
					repl.key = b;
					repl.bLevel = prev.bLevel + prev.recharged - dist_prev_repl;
					repl.aTime = prev.lTime + time_prev_repl;
					repl.wTime = 0;
					if (repl.aTime < inst->getNodeByKey(repl.key).readyTime) {
						repl.wTime = inst->getNodeByKey(repl.key).readyTime - repl.aTime;
					}
					repl.lTime = repl.wTime + repl.wTime + inst->ct;
					repl.recharge = true;
					repl.recharged = inst->Q - repl.bLevel;
					repl.vLoad = s.routes.at(i).at(j).vLoad;
					//

					int dist_repl_next = inst->dist(b, s.routes.at(i).at(j + 1).key);
					int time_repl_next = inst->getTD(b, s.routes.at(i).at(j + 1).key);

					// checking battery level and time windows
					if (repl.bLevel > 0 && (repl.bLevel + repl.recharged) - dist_repl_next >= next.bLevel && repl.lTime + time_repl_next > inst->getNodeByKey(next.key).dueDate) {

						// replace bss
						s.routes.at(i).at(j) = repl;

						// compute the rest of the route to  carrie the changes to the other nodes
						for (int k = j + 1; k < s.routes.at(i).size(); k++) {
							node curr = inst->getNodeByKey(s.routes.at(i).at(k).key);

							s.routes.at(i).at(k).bLevel = s.routes.at(i).at(k - 1).bLevel + s.routes.at(i).at(k - 1).recharged - inst->dist(s.routes.at(i).at(k - 1).key, s.routes.at(i).at(k).key); // compute battery level
							s.routes.at(i).at(k).aTime = s.routes.at(i).at(k - 1).lTime + inst->getTD(s.routes.at(i).at(k - 1).key, s.routes.at(i).at(k).key); // arrival time
							s.routes.at(i).at(k).wTime = 0; // waiting time
							if (s.routes.at(i).at(k).aTime < curr.readyTime) {
								s.routes.at(i).at(k).wTime = curr.readyTime - s.routes.at(i).at(k - 1).aTime;
							}

							if (curr.type == "f" || curr.type == "f_d") {
								s.routes.at(i).at(k).lTime = s.routes.at(i).at(k).wTime + s.routes.at(i).at(k).wTime + inst->ct; // departure time


								s.routes.at(i).at(k).recharged = inst->Q - s.routes.at(i).at(k).bLevel; // amount recharged
							}
							else {
								s.routes.at(i).at(k).lTime = s.routes.at(i).at(k).wTime + s.routes.at(i).at(k).wTime + curr.serviceTime; // departure time

								// s.routes.at(i).at(k).recharged = inst->Q - s.routes.at(i).at(k).bLevel; // amount recharged
							}
						}
					}
				}
			}
		}
	}

	return s;
}

Solution perm_rep::bssReplacement_r(Solution s, int bss1)
{
	set<int> routes; // list of routes with bss1
	set<int> BSSs; // list of used BSSs

	// get list of routes with bss1
	int j = 0;
	for (route r : s.routes) {
		for (vertex v : r) {
			if (v.key == bss1) {
				routes.insert(j);
			}
			else if (inst->getNodeByKey(v.key).type == "f") {
				BSSs.insert(v.key);
			}
		}
		j++;
	}

	vector<node> R = inst->set_R(); // backup the set R
	vector<node> newR;

	// recreate the set R only with the used BSSs
	for (int i : BSSs) {
		for (node n : R) {
			if (n.key == i) {
				newR.push_back(n);
				break;
			}
		}
	}

	inst->R = newR;

	try {
		for (int i : routes) {
			route r;
			r = removeBSS(s.routes.at(i));
			r = addStations(r);
			s.routes.at(i) = r;
		}
	}
	catch (exception &e) {
		//cout << e.what() << endl;
		inst->R = R;
		throw e;
	}
	inst->R = R;

	s = procSol(s);
	return s;
}

Solution perm_rep::shakeRandom_r(Solution s, string n)
{
	//cout << "shaking " << n << endl;
	if (n == "insertR") { // route partition
		if (s.routes.size() <= 1) { // cant use this neighborhood in solutions with only one route
			return s;
		}

		vector<int> rts;
		int i = 0;
		for (auto r : s.routes) {
			int tc = getNumC(r);
			if (tc >= 2) {
				rts.push_back(i);
			}
			i++;
		}
		if (rts.size() == 0) {
			//throw "none route can be partitioned";
			return s;
		}
		int r1 = Random::get<int>(0, rts.size() - 1);
		r1 = rts.at(r1);

		int r2 = Random::get<int>(0, s.routes.size() - 1);
		while (r1 == r2) {
			r2 = Random::get<int>(0, s.routes.size() - 1);
		}

		vector<int> customers = getListC(s.routes.at(r1));
		int p1 = Random::get<int>(1, customers.size() - 1);
		p1 = customers.at(p1);

		customers = getListC(s.routes.at(r2));
		//int p2 = Random::get<int>(0, s.routes.at(r2).size() - 2);
		int p2 = Random::get<int>(0, customers.size() - 1);
		p2 = customers.at(p2);

		try {
			//cout << "partitioning route " << r1 << " in " << p1 << " and inserting subr in route " << r2 << " in position " << p2 << endl;
			return routeInsertion_r(s, r1, p1, r2, p2);
		}
		catch (string e) {
			//cout << e << endl;
			//throw "error generating neighbor " + n;
			return s;
		}
		catch (exception e) {
			//cout << e.what() << endl;
			//throw "error generating neighbor " + n;
			return s;
		}

		//break;
	}
	else if (n == "2opt") {
		vector<int> rts;
		int i = 0;
		for (auto r : s.routes) {
			int tc = getNumC(r);
			if (tc >= 2) {
				rts.push_back(i);
			}
			i++;
		}
		if (rts.size() == 0) {
			return s;
			//throw "none route can be partitioned";
		}
		int r = Random::get<int>(0, rts.size() - 1);
		r = rts.at(r);

		vector<int> customers = getListC(s.routes.at(r));

		int p1 = Random::get<int>(0, customers.size() - 2);
		int p = p1;
		p1 = customers.at(p1);

		int p2 = Random::get<int>(p + 1, customers.size() - 1);
		p2 = customers.at(p2);

		try {
			return opt2_route_r(s, r, p1, p2);
		}
		catch (string e) {
			cout << e << endl;
			for (auto n : s.routes.at(r)) {
				cout << n.key << " ";
			}
			throw "error generating neighbor " + n;
		}
		catch (exception &e) {
			cout << e.what() << endl;
			throw "error generating neighbor " + n;
		}

		//break;
	}
	else if (n == "shiftC") {
		vector<int> rts;
		int i = 0;
		for (auto r : s.routes) {
			int tc = getNumC(r);
			if (tc >= 2) {
				rts.push_back(i);
			}
			i++;
		}
		if (rts.size() == 0) {
			return s;
			throw "cant pick a route";
		}
		int r = Random::get<int>(0, rts.size() - 1);
		r = rts.at(r);

		vector<int> cand = getListC(s.routes.at(r));
		int c = Random::get<int>(0, cand.size() - 2);
		int c_ = c;
		c = cand.at(c);

		int qt = Random::get<int>(1, s.routes.at(r).size() - c - 2);

		try {
			return shiftCustomer_r(s, r, c, qt);
		}
		catch (string e) {
			cout << e << endl;
			throw "error generating neighbor " + n;
		}
		catch (exception e) {
			cout << e.what() << endl;
			throw "error generating neighbor " + n;
		}

		//break;
	}
	else if (n == "swapD") {
		set<int> dpts;
		for (route& r : s.routes) {
			dpts.insert(r.front().key);
		}

		if (dpts.size() > 0) {
			// get a vector with all depots
			vector<int> vdpts;
			vdpts.assign(dpts.begin(), dpts.end());

			// choose randomly a depot
			int dptA = Random::get<int>(0, vdpts.size() - 1);
			dptA = vdpts[dptA];

			// choose randomly a new depot
			int newDpt = Random::get<int>(0, inst->set_UD0().size() - 1);
			while (inst->set_UD().at(newDpt).key == dptA) {
				newDpt = Random::get<int>(0, inst->set_UD0().size() - 1);
			}
			int dptB = inst->set_UD().at(newDpt).key;


			try {
				return swapDepot(s, dptA, dptB);
			}
			catch (string e) {
				cout << e << endl;
				throw "error generating neighbor " + n;
			}
			catch (exception e) {
				cout << e.what() << endl;
				throw "error generating neighbor " + n;
			}
		}
		else {
			return s;
		}

		//break;
	}
	else if (n == "changeD") {
		auto UD0 = inst->set_UD0();

		if (UD0.size() > 1) {
			set<int> useddpts;
			for (route r : s.routes) {
				useddpts.insert(r.front().key);
			}

			int keyA = Random::get<int>(0, useddpts.size() - 1);
			set<int>::iterator it = useddpts.begin();
			advance(it, keyA);
			keyA = *it;

			set<int> availabedpts;
			for (node i : inst->set_UD0()) {
				bool find = false;
				for (int j : useddpts) {
					if (i.key != j) {
						find = true;
					}
				}
				if (find == false) {
					availabedpts.insert(i.key);
				}
			}
			for (int i : useddpts) {
				if (i != keyA) {
					availabedpts.insert(i);
				}
			}

			if (useddpts.size() < 1 || availabedpts.size() < 1) {
				cout << "error\n"; /////////////
			}

			int keyB = Random::get<int>(0, availabedpts.size() - 1);
			it = availabedpts.begin();
			advance(it, keyB);
			keyB = *it;

			try {
				return changeDepotV2(s, keyA, keyB);
			}
			catch (exception& e) {
				return s;
			}
		}
		else {
			return s;
		}
	/*
		auto UD0 = inst->set_UD0();

		if (UD0.size() > 1) {
			int rt = Random::get<int>(0, s.routes.size() - 1);

			int i = 0;
			for (auto n : UD0) {
				if (n.key == s.routes.at(rt).front().key) {
					break;
				}
				i++;
			}
			UD0.erase(UD0.begin() + i);

			int dpt = Random::get<int>(0, UD0.size() - 1);
			dpt = UD0.at(dpt).key;

			try {
				return changeDepot(s, dpt);
			}
			catch (exception &e) {
				//cout << e.what() << endl;
				//throw e;//"error generating neighbor " + n;
				return s;				
			}
		}
		else {
			return s;
		}
	*/
	}
	else if (n == "splitR") {
		// get all routes that can be splited. in order to be splitted the route must have at least two customers
		vector<pair<int, int>> rts; // store the route position in the routes vector and the number od customers
		vector<pair<int, int>> cList;
		int i = 0;
		for (auto r : s.routes) {
			cList = getCustomersPos(r);
			if (cList.size() >= 2) {
				rts.push_back(pair<int, int>(i, cList.size()));
			}
			i++;
		}

		// choose random route
		if (rts.size() > 0) {
			int rt = Random::get<int>(0, rts.size() - 1);
			int p = Random::get<int>(1, rts.at(rt).second - 1);

			try {
				return routeSplit_r(s, rt, p);
			}
			catch (string e) {
				cout << e << endl;
				throw "error generating neighbor " + n;
			}

		}
		else {
			// cant generate this neighbor
			return s;
		}
	}
	else if (n == "unionR") {
		if (s.routes.size() > 1) {
			int r1 = Random::get<int>(0, s.routes.size() - 1);
			int r2 = Random::get<int>(0, s.routes.size() - 1);
			while (r1 == r2) {
				r2 = Random::get<int>(0, s.routes.size() - 1);
			}
			if (r2 < r1) {
				int temp = r1;
				r1 = r2;
				r2 = temp;
			}
			///////////////////////			
			//writeSolutionTxt("D:/Victor/Pos-Graduacao/UFV/Research/Implementation/Model/MDEVLRPTW-BSPR/MILPM/x64/Release/teste.txt", s);
			//writeRoutes(cout, s);
			//cout << "---------------\n";
			//Solution s2 = readSolutionTxt("D:/Victor/Pos-Graduacao/UFV/Research/Implementation/Model/MDEVLRPTW-BSPR/MILPM/x64/Release/teste.txt");
			//writeRoutes(cout, s2);
			//cout << r1 << " - " << r2 << endl;
			///////////////////////
			try {
				return routeUnion_r(s, r1, r2);
			}
			catch (exception& e) {
				return s;
				//throw e;
			}
			catch (string e) {
				return s;
				//cout << e << endl;				
				//throw "error generating neighbor " + n;
			}
		}
		else {
			return s;
		}

	}
	else if (n == "bssReplacement") {
		if (inst->set_R().size() == 1) {
			return s;
		}

		vector<int> BSSs;
		for (route r : s.routes) {
			for (vertex v : r) {
				if (inst->getNodeByKey(v.key).type == "f") {
					BSSs.push_back(v.key);
				}
			}
		}

		int bss1 = Random::get<int>(0, BSSs.size() - 1);

		Solution sl = s;
		try {
			sl = bssReplacement_r(s, BSSs.at(bss1));
		}
		catch (exception &e) {
			return s;
		}

		return sl;
	}
	else {
		throw n + " is not a valid neighborhoor structure";
	}
	// swapD changeD splitR joinR
}

Solution perm_rep::greed()
{
	Solution best;
	best.FO = INT_MAX;
	bool improv = false;
	Solution curr;
	vector<node> temp;
	vector<node> used;
	route prevr;

	auto N = inst->set_UD0();

	vector<pair<node, int>> depots;

	for (auto n : N) {

		int sum = 0;
		for (auto c : inst->set_C()) {
			sum += inst->distanceMatrix.at(n.key).at(c.key);
		}
		depots.push_back(pair<node, int>(n, sum));
	}

	sort(depots.begin(), depots.end(), [](pair<node, int> a, pair<node, int> b)->bool{ return a.second < b.second; });

	N.clear();
	for (auto i : depots) {
		N.push_back(i.first);
	}

	vector<node> C = inst->set_C(); // get the set of all customers
	for (auto n : N) {

		bool depotOK = true;
		while (depotOK == true) {
			// create initial route route
			route r;

			// initial depot
			vertex dpt;
			dpt.key = n.key;
			dpt.aTime = 0;
			dpt.wTime = 0;
			dpt.lTime = 0;
			dpt.recharge = false;
			dpt.recharged = 0;
			dpt.bLevel = inst->Q;
			dpt.vLoad = inst->c;
			dpt.n = n;
			r.push_back(dpt);

			bool cAvailable = true;
			route okRoute;
			bool findRoute = false;
			while (true) { // cAvailable == true
				if (C.size() == 0) {
					break;
				}

				int minBU;
				int key = -1;

				// search for the next node
				minBU = 9999999;

				for (node c : C) {
					node nodeC = inst->getNodeByKey(c.key);
					int BU = inst->getBatteryUsed(r.back().key, c.key); // battery used
					int TT = inst->getTD(r.back().key, c.key); // travel time		

					if (BU < minBU && BU != 0) {
						minBU = BU;
						key = c.key;
					}
				}

				route r_ = r;

				// add c
				vertex v;
				v.n = inst->getNodeByKey(key);
				v.key = key;
				r_.push_back(v);				

				prevr = r_; // store the route r before adding the arrival node and processing

				// add arrival
				v.key = inst->getArrival(dpt.key);
				r_.push_back(v);

				// compute route
				bool pass = true;
				try {
					r_ = addStations(r_);
				}
				catch (IsolatedNode &e) {
					pass = false;
				}

				// check route
				auto inf = evalRoute(r_);

				if (inf.size() == 0 && pass == true) {
					okRoute = r_; // store last feasible route

					r = prevr;

					findRoute = true;
					
					// remove the customers in okRoute from the set C
					vector<node> C_;
					for (node aux : C) {
						bool find = false;
						for (auto n : r_) {
							if (aux.key == n.key) {
								find = true;
							}
						}
						if (find == false) {
							C_.push_back(aux);
						}
						else {
							used.push_back(aux);
						}
					}
					C = C_;
					//continue;
				}
				else {
					
					// temporarialy remove the infeasible node from the set C so it will not be choosen by mistake
					vector<node> C_;
					for (node aux : C) {
						if (aux.key == key) {
							temp.push_back(aux);
						}
						else {
							C_.push_back(aux);
						}
					}
					C = C_;
					//continue;
				}

				//if (okRoute.back().vLoad == 0) {
				//	break;
				//}
				
			}

			for (auto i : temp) {
				C.push_back(i);
			}
			temp.clear();

			if (findRoute == true) {
				route rt;
				for (auto i : okRoute) {
					if (inst->getNodeByKey(i.key).type != "f") {
						rt.push_back(i);
					}
				}
				curr.routes.push_back(rt);				
			}
			else {
				break;
			}
		}

		if (C.size() == 0) {
			break;
		}
	}
		
	if (C.size() != 0) {
		throw UnfeasibleInstance(inst->fileName);
	}

	//auto a = inst->nodes.at(3);
	//auto b = inst->nodes.at(48);
	//int d = inst->distanceMatrix.at(3).at(48);
	//cout << d << endl;

	try {
		curr = addStations(curr);
		curr = procSol(curr); // error

		if (curr.FO < best.FO && curr.inf.size() == 0) {
			best = curr;
			improv = true;
		}
		best = curr;
		improv = true;
	}
	catch (IsolatedNode& e) {
		cout << e.what() << endl;
	}

	if (improv == false) {
		throw UnfeasibleInstance(inst->fileName);
	}

	return best;
}

Solution perm_rep::greedl(vector<string> BSS)
{
	inst->removeBSS(BSS); // remove unwanted BSSs

	Solution init;
	try {
		init = greed();
		//cout << fixed << "Initial objective: " << init.FO << endl;
		return init;
	}
	catch (UnfeasibleInstance& e) {
		throw PermutationInf({ 0 });
	}
	catch (exception& e) {
		cout << e.what() << endl;
	}
}

Solution perm_rep::greed_()
{
	Solution best;
	best.FO = INT_MAX;
	bool improv = false;
	for (auto n : inst->set_UD0()) {
		Solution curr;

		vector<node> C = inst->set_C(); // get the set of all customers

		// create initial route route
		route r;

		// initial depot
		vertex dpt;
		dpt.key = n.key;
		dpt.aTime = 0;
		dpt.wTime = 0;
		dpt.lTime = 0;
		dpt.recharge = false;
		dpt.recharged = 0;
		dpt.bLevel = inst->Q;
		dpt.vLoad = inst->c;
		r.push_back(dpt);

		curr.routes.push_back(r);

		while (C.size() > 0) {

			bool insert = false;
			for (route& rt : curr.routes) {

				vertex v;
				int minBU;

				// search for the next node
				minBU = 9999999;
				for (node c : C) {

					node nodeC = inst->getNodeByKey(c.key);
					int BU = inst->getBatteryUsed(rt.back().key, c.key); // battery used
					int TT = inst->getTD(rt.back().key, c.key); // travel time		

					// compute node c relatively to last node in the current route
					vertex v_;
					v_.key = c.key;
					v_.aTime = rt.back().lTime + TT; // arrival time

					if (v_.aTime < nodeC.readyTime) { // waiting time
						v_.wTime = nodeC.readyTime - v_.aTime;
					}
					else {
						v_.wTime = 0;
					}
					v_.lTime = v_.aTime + v_.wTime + nodeC.serviceTime; // departure time
					v_.vLoad = rt.back().vLoad - nodeC.demand; // vehicle load
					v_.bLevel = rt.back().bLevel - BU; // battery level
					v_.recharged = nodeC.serviceTime * inst->g; // amount recharged
					if (v_.bLevel + v_.recharged > inst->Q) {
						v_.recharged = inst->Q - v_.bLevel;
					}

					if (BU < minBU && BU != 0) {
						minBU = BU;
						v = v_;
					}
				}

				node nodeV = inst->getNodeByKey(v.key);
				if (v.vLoad >= 0 && v.aTime < nodeV.dueDate) { // check battery level and arrival time
					rt.push_back(v);
					insert = true;

					// remove the city from the C vector
					vector<node> C_;
					for (node aux : C) {
						if (aux.key != v.key) {
							C_.push_back(aux);
						}
					}
					C = C_;
				}
			}

			if (insert == false) {
				route rt_;
				rt_.push_back(dpt);
				curr.routes.push_back(rt_);

			}
		}

		// add arrival node
		dpt.key = inst->getArrival(dpt.key);
		for (route& rt : curr.routes) {
			rt.push_back(dpt);
		}

		try {
			curr = addStations(curr);
			curr = procSol(curr); // error

			if (curr.FO < best.FO && curr.inf.size() == 0) {
				best = curr;
				improv = true;
			}
		}
		catch (IsolatedNode& e) {
			cout << e.what() << endl;
		}

	}
	if (improv == false) {
		for (auto i : inst->set_R()) {
			cout << i.key << " ";
		}
		cout << endl;
		throw "infeasible_solution";
	}

	return best;
}

Solution perm_rep::greed2()
{
	Solution best;
	best.FO = INT_MAX;
	bool improv = false;
	Solution curr;
	int lastRCreated = -1;
	bool find = false;
	bool next = false;

	vector<node> C = inst->set_C(); // get the set of all customers
	for (auto n : inst->set_UD0()) {				

		// create initial route
		route r;

		// initial depot
		vertex dpt;
		dpt.key = n.key;
		dpt.aTime = 0;
		dpt.wTime = 0;
		dpt.lTime = 0;
		dpt.recharge = false;
		dpt.recharged = 0;
		dpt.bLevel = inst->Q;
		dpt.vLoad = inst->c;
		dpt.n = inst->getNodeByKey(dpt.key);
		r.push_back(dpt);

		curr.routes.push_back(r);
		lastRCreated++;

		next = false;
		while (C.size() > 0) {
			bool insert = false;
			for (route& rt : curr.routes) {
				// search for the closest node
				int minBU;
				minBU = 9999999;
				int pos = 0;
				node c_;
				find = false;
				// search for a customer that can be head from the last node in the current route
				vertex v;
				int distBackToDepot; int max1 = INT_MAX;
				int distPrevToCurr; float max2 = INT_MAX, min = INT_MAX;


				for (node c : C) {

					node nodeC = inst->getNodeByKey(c.key);

					int BU = inst->getBatteryUsed(rt.back().key, c.key); // battery used
					int TT = inst->getTD(rt.back().key, c.key); // travel time		

					v = computeVertex(rt.back(), c);

					// search for the a path back to the depot
					distBackToDepot = inst->distanceMatrix.at(c.key).at(rt.front().key);
					distPrevToCurr = inst->distanceMatrix.at(rt.back().key).at(c.key);
					double factor = float(distBackToDepot) / float(distPrevToCurr);

					if (factor < min) {
						min = factor;
					}

					// compute node c relatively to last node in the current route
					if (BU < minBU && BU != 0 && rt.back().vLoad - nodeC.demand > 0) {
						minBU = BU;
						c_ = c;
						find = true;
					}
					pos++;
				}

				v = computeVertex(rt.back(), c_);
				node nodeV = inst->getNodeByKey(v.key);
				v.n = nodeV;

				if (find == true) {
					

					if (v.aTime < nodeV.dueDate && v.bLevel > 0) { //
						rt.push_back(v);
						insert = true;

						// remove the city from the C vector
						vector<node> C_;
						for (node aux : C) {
							if (aux.key != v.key) {
								C_.push_back(aux);
							}
						}
						C = C_;
					}
					else if (v.aTime < nodeV.dueDate && v.bLevel < 0) { // if battery is not enough
						rt.push_back(v);

						int pos = rt.size() - 2;
						node n = inst->getNodeByKey(rt.at(pos).key);

						while (pos >= 0) { // try to find a bss to replace the battery
							pair<int, int> ret;
							int bss;
							int minB;
							try {
								ret = closestBSS(rt, pos);
							}
							catch (OutOfBSS& e) {
								cout << e.what() << endl;
							}
							catch (exception& e) {
								cout << e.what() << endl;
							}
							bss = ret.first;
							minBU = ret.second;

							int battery = rt.at(pos).bLevel - minBU;
							if (battery > 0) {
								vertex v = computeVertex(rt.at(pos), inst->getNodeByKey(bss));

								vector<int> aux;
								for (int i = pos + 1; i < rt.size(); i++) {
									aux.push_back(rt.at(i).key);
								}
								for (auto i : aux) {
									node n = inst->getNodeByKey(i);
									C.push_back(n);
								}

								rt.erase(rt.begin() + pos + 1, rt.end());
								rt.push_back(v);

								insert = true;
								break;
							}
							else {
								n = inst->getNodeByKey(rt.at(pos).key);
								pos--;
							}

							
						}
						if (insert == false) {
							rt.erase(rt.begin() + rt.size() - 1);
						}
					}

				}
				else {
					continue;
				}
			}
			if (insert == false) {
				if (curr.routes.at(lastRCreated).size() == 1) {
					curr.routes.erase(curr.routes.begin() + lastRCreated);
					lastRCreated--;
					next = true;
					break;
				}
				else {
					route rt_;
					rt_.push_back(dpt);
					curr.routes.push_back(rt_);
					lastRCreated++;
					continue;
				}
			}
		}

		// add arrival node
		dpt.key = inst->getArrival(dpt.key);
		for (route& rt : curr.routes) {
			rt.push_back(dpt);
		}

	}

	try {
		curr = addStations(curr);
		curr = procSol(curr); // error

		if (curr.FO < best.FO && curr.inf.size() == 0) {
			best = curr;
			improv = true;
		}
	}
	catch (IsolatedNode& e) {
		cout << e.what() << endl;
	}

	if (improv == false) {
		for (auto i : inst->set_R()) {
			cout << i.key << " ";
		}
		cout << endl;
		throw "infeasible_solution";
	}

	return best;
}

Solution perm_rep::greedDist()
{
	vector<node> C = inst->set_C(); // get the set of all customers

	// get the node with the earliest due date
	node minDueDate;
	minDueDate.dueDate = INT_MAX;
	for (node c : C) {
		if (minDueDate.dueDate > c.dueDate) {
			minDueDate = c;
		}
	}

	vector<node> nodes;
	nodes.push_back(minDueDate);

	vector<node> aux;
	for (node c : C) {
		if (c.key != minDueDate.key) {
			aux.push_back(c);
		}

	}
	C = aux;

	while (C.size() > 0) {
		int minDist = INT_MAX;

		node nextNode;
		for (node c : C) {
			//cout << nodes.back().key << " - " << c.key << endl;
			int dist = inst->getBatteryUsed(nodes.back().key, c.key);
			if (dist < minDist) {
				minDist = dist;
				nextNode = c;
			}
		}

		nodes.push_back(nextNode);

		aux.clear();
		for (node c : C) {
			if (nextNode.key != c.key) {
				aux.push_back(c);
			}
		}
		C = aux;
	}

	permutation p;
	for (node c : nodes) {
		p.push_back(c.key);
	}

	Solution s = permutationToSolution(p);

	return s;
}

Solution perm_rep::greedDD()
{
	permutation p;

	vector<node> C = inst->set_C(); // get the set of all customers
	sort(C.begin(), C.end(), compDD);

	for (node c : C) {
		p.push_back(c.key);
	}

	Solution s = permutationToSolution(p);

	return s;
}

Solution perm_rep::greedRT()
{
	permutation p;

	vector<node> C = inst->set_C(); // get the set of all customers
	sort(C.begin(), C.end(), compRT);

	for (node c : C) {
		p.push_back(c.key);
	}

	Solution s = permutationToSolution(p);

	return s;
}

vector<pair<int, int>> perm_rep::getCustomersPos(vector<vertex> route)
{
	vector<pair<int, int>> ret;
	int count = 0;
	for (auto n : route) {
		if (inst->getNodeByKey(n.key).type == "c") {
			ret.push_back(pair<int, int>(n.key, count));
		}
		count++;
	}
	return ret;
}

vector<vertex> perm_rep::getCustomers(vector<vertex> route)
{
	vector<vertex> ret;
	for (auto n : route) {
		if (inst->getNodeByKey(n.key).type == "c") {
			ret.push_back(n);
		}
	}
	return ret;
}

int perm_rep::test_permutationToSolutionAlt()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/", "UK100_01.txt", 3);
	printInstance();

	try {

		//Solution init = permutationToSolution({ 9, 11, 6, 10, 7, 8, 5 }); // UK10_01.txt // optmal

		//Solution init = permutationToSolution({ 5, 10, 6, 8, 11, 9, 7 });
		//Solution init = permutationToSolution({ 11, 8, 10, 6, 7, 5, 9 }); 

		//Solution init = permutationToSolution({ 92, 43, 50, 48, 100, 88, 85, 66, 77, 58, 52, 49, 70, 47, 87, 63, 59, 99, 86, 83, 37, 54, 74, 97, 72, 73, 45, 96, 65, 79, 67, 60, 89, 94, 61, 41, 30, 31, 36, 35, 78, 102, 38, 62, 55, 75, 56, 104, 40, 81, 84, 68, 34, 82, 29, 44, 98, 91, 71, 39, 46, 32, 93, 33, 101, 51, 80, 90, 64, 69, 53, 76, 103, 95, 42, 57 });

		permutation p;// = randomPermutation();
		p = { 55, 83, 41, 50, 87, 34, 68, 52, 102, 66, 44, 45, 101, 33, 56, 38, 95, 80, 48, 100, 82, 71, 93, 58, 29, 30, 92, 81, 47, 59, 54, 99, 53, 39, 65, 88, 49, 75, 72, 57, 104, 67, 84, 89, 64, 43, 78, 40, 98, 73, 32, 61, 96, 35, 77, 79, 42, 62, 103, 76, 85, 31, 51, 74, 91, 94, 97, 63, 46, 69, 90, 60, 70, 37, 86, 36 };
		for (auto i : p) {
			cout << i << ", ";
		}
		cout << endl;
		
		Solution init = permutationToSolutionAlt(p);
		getArcsNodes(init);
		init.debug(cout);

	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 0;
}

int perm_rep::test_permutationToSolution()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/", "UK10_01.txt", 3);
	printInstance();

	try {

		//Solution init = permutationToSolution({ 9, 11, 6, 10, 7, 8, 5 }); // UK10_01.txt // optmal

		//Solution init = permutationToSolution({ 5, 10, 6, 8, 11, 9, 7 });
		//Solution init = permutationToSolution({ 11, 8, 10, 6, 7, 5, 9 }); 

		//Solution init = permutationToSolution({ 92, 43, 50, 48, 100, 88, 85, 66, 77, 58, 52, 49, 70, 47, 87, 63, 59, 99, 86, 83, 37, 54, 74, 97, 72, 73, 45, 96, 65, 79, 67, 60, 89, 94, 61, 41, 30, 31, 36, 35, 78, 102, 38, 62, 55, 75, 56, 104, 40, 81, 84, 68, 34, 82, 29, 44, 98, 91, 71, 39, 46, 32, 93, 33, 101, 51, 80, 90, 64, 69, 53, 76, 103, 95, 42, 57 });
		
		permutation p = randomPermutation();
		for (auto i : p) {
			cout << i << ", ";
		}
		cout << endl;

		Solution init = permutationToSolution(p);
		
		getArcsNodes(init);
		init.debug(cout);

	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 0;
}

int perm_rep::test_addStations()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/", "UK10_01.txt", 3);
	printInstance();

	try {
		vector<int> vec = { 0, 10, 9, 7, 5, 11, 8, 12 };
		route r;
		for (auto i : vec) {
			vertex v;
			v.key = i;
			
			r.push_back(v);
		}

		r = addStations(r);
		
		for (auto i : r) {
			cout << i.key << " ";
		}
		cout << endl;
	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 0;
}

int perm_rep::test_shakeRandom_r(int i)
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/", "UK200_01.txt", 3);
	printInstance();

	try {

		//Solution init = testPermutation({ 9, 11, 6, 10, 7, 8, 5 });
		Solution init = permutationToSolution(randomPermutation());

		getArcsNodes(init);
		init.debug(cout);

		Solution shake = shakeRandom_r(init, "2opt");

		getArcsNodes(shake);
		shake = procSol(shake);
		shake.debug(cout);


		cout << init.FO << " -- " << shake.FO << endl;

		cout << "inf: ";
		for (auto i : shake.inf) {
			cout << i << " ";
		}
		cout << endl;

	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 0;
}

int perm_rep::test_routePartition()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/", "UK10_01.txt", 3);
	printInstance();

	Solution init;

	try {

		init = testPermutation({ 9, 11, 6, 10, 7, 8, 5 }); // UK10_01.txt // optmal

		init.debug(cout);

		init = routeInsertion_r(init, 0, 5, 1, 0);

		init = procSol(init);

		// get nodes
		for (route& r : init.routes) {
			for (vertex& v : r) {
				for (node n : inst->nodes) {
					if (v.key == n.key) {
						v.n = n;
						break;
					}
				}
			}
		}
		// get arcs
		for (route& r : init.routes) {
			for (int i = 0; i < r.size() - 1; i++) {
				arc a;
				a.beg = r.at(i).key;
				a.end = r.at(i + 1).key;
				a.value = inst->dist(a.beg, a.end);
				a.value2 = inst->getTD(a.beg, a.end);
				init.arcs.push_back(a);
			}
		}

		init.debug(cout);
	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 1;
}

int perm_rep::test_opt2_route()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/", "UK200_01.txt", 3);
	printInstance();

	Solution init;

	try {

		//init = testPermutation({ 9, 11, 6, 10, 7, 8, 5 }); // UK10_01.txt // optmal
		init = permutationToSolution(randomPermutation());

		getArcsNodes(init);

		init.debug(cout);

		int i, j, k;
		cin >> i;
		cin >> j;
		cin >> k;

		Solution n = opt2_route_r(init, i, j, k);

		getArcsNodes(n);
	
		n.debug(cout);

		for (auto r : init.routes.at(i)) {
			cout << r.key << " ";
		}
		cout << endl;
		for (auto r : n.routes.at(i)) {
			cout << r.key << " ";
		}
		cout << endl;
		cout << init.FO << " - " << n.FO << endl;
	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 1;
}

int perm_rep::test_shiftCustomer()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/", "UK10_01.txt", 3);
	printInstance();

	Solution init;

	try {

		init = testPermutation({ 9, 11, 6, 10, 7, 8, 5 }); // UK10_01.txt // optmal

		init.debug(cout);

		init = shiftCustomer_r(init, 0, 2, 6);

		init = procSol(init);

		// get nodes
		for (route& r : init.routes) {
			for (vertex& v : r) {
				for (node n : inst->nodes) {
					if (v.key == n.key) {
						v.n = n;
						break;
					}
				}
			}
		}
		// get arcs
		for (route& r : init.routes) {
			for (int i = 0; i < r.size() - 1; i++) {
				arc a;
				a.beg = r.at(i).key;
				a.end = r.at(i + 1).key;
				a.value = inst->dist(a.beg, a.end);
				a.value2 = inst->getTD(a.beg, a.end);
				init.arcs.push_back(a);
			}
		}

		init.debug(cout);
	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 1;
}

int perm_rep::test_swapDepot()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/test/", "UK200_01.txt", 3);
	printInstance();

	Solution init;

	try {

		init = testPermutation(randomPermutation());
		init.debug(cout);

		int a, b;
		cin >> a;
		cin >> b;

		Solution n = swapDepot(init, a, b);
		n = procSol(n);
		getArcsNodes(n);	
		n.debug(cout);

		cout << init.FO << " - " << n.FO << endl;

		for (auto i : n.inf) {
			cout << i << endl;
		}
	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 1;
}

int perm_rep::test_changeDepot()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/test/", "UK200_01.txt", 3);
	printInstance();

	Solution init;

	try {

		init = testPermutation(randomPermutation());
		init.debug(cout);

		int a;
		cin >> a;

		Solution n = changeDepot(init, a);
		n = procSol(n);
		getArcsNodes(n);
		n.debug(cout);

		cout << init.FO << " - " << n.FO << endl;

		for (auto i : n.inf) {
			cout << i << endl;
		}
	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 1;
}

int perm_rep::test_splitRoute()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/test/", "UK200_01.txt", 3);
	printInstance();

	Solution init;

	try {

		init = testPermutation(randomPermutation());
		init.debug(cout);

		int a, b;
		cin >> a;
		cin >> b;

		Solution n = routeSplit_r(init, a, b);
		n = procSol(n);
		getArcsNodes(n);
		n.debug(cout);

		cout << init.FO << " - " << n.FO << endl;

		for (auto i : n.inf) {
			cout << i << endl;
		}
	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 1;
}

int perm_rep::test_unionRoute()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/brelrp/50/", "mg50_01.brelrp", 5);
	printInstance();

	Solution init;

	try {

		init = readSolutionTxt("D:/Victor/Pos-Graduacao/UFV/Research/Implementation/Model/MDEVLRPTW-BSPR/MILPM/x64/Release/teste.txt");
		writeRoutes(cout, init);
		//init.debug(cout);

		int a = 2, b = 14;
		//cin >> a;
		//cin >> b;

		Solution n = routeUnion_r(init, a, b);
		n = procSol(n);
		getArcsNodes(n);
		n.debug(cout);

		cout << init.FO << " - " << n.FO << endl;

		for (auto i : n.inf) {
			cout << i << endl;
		}
	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 1;
}

int perm_rep::test_bssReplacement_r()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/test/200/", "UK200_01.txt", 3);
	printInstance();

	Solution init;

	try {

		init = testPermutation(randomPermutation());
		init.debug(cout);

		int a;
		cin >> a;

		Solution n = bssReplacement_r(init, a);
		n = procSol(n);
		getArcsNodes(n);
		n.debug(cout);

		cout << init.FO << " - " << n.FO << endl;

		for (auto i : n.inf) {
			cout << i << endl;
		}
	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 1;
}

int perm_rep::test_neigh()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/", "UK10_01.txt", 3);
	printInstance();

	Solution init;

	try {

		init = testPermutation({ 9, 11, 6, 10, 7, 8, 5 }); // UK10_01.txt // optmal

		init = procSol(init);

		// get nodes
		for (route& r : init.routes) {
			for (vertex& v : r) {
				for (node n : inst->nodes) {
					if (v.key == n.key) {
						v.n = n;
						break;
					}
				}
			}
		}
		// get arcs
		for (route& r : init.routes) {
			for (int i = 0; i < r.size() - 1; i++) {
				arc a;
				a.beg = r.at(i).key;
				a.end = r.at(i + 1).key;
				a.value = inst->dist(a.beg, a.end);
				a.value2 = inst->getTD(a.beg, a.end);
				init.arcs.push_back(a);
			}
		}

		init.debug(cout);

		init = routeInsertion_r(init, 0, 5, 1, 0);

		init = procSol(init);

		// get nodes
		for (route& r : init.routes) {
			for (vertex& v : r) {
				for (node n : inst->nodes) {
					if (v.key == n.key) {
						v.n = n;
						break;
					}
				}
			}
		}
		// get arcs
		for (route& r : init.routes) {
			for (int i = 0; i < r.size() - 1; i++) {
				arc a;
				a.beg = r.at(i).key;
				a.end = r.at(i + 1).key;
				a.value = inst->dist(a.beg, a.end);
				a.value2 = inst->getTD(a.beg, a.end);
				init.arcs.push_back(a);
			}
		}

		init.debug(cout);

		init = opt2_route_r(init, 1, 2, 5);

		init = procSol(init);

		// get nodes
		for (route& r : init.routes) {
			for (vertex& v : r) {
				for (node n : inst->nodes) {
					if (v.key == n.key) {
						v.n = n;
						break;
					}
				}
			}
		}
		// get arcs
		for (route& r : init.routes) {
			for (int i = 0; i < r.size() - 1; i++) {
				arc a;
				a.beg = r.at(i).key;
				a.end = r.at(i + 1).key;
				a.value = inst->dist(a.beg, a.end);
				a.value2 = inst->getTD(a.beg, a.end);
				init.arcs.push_back(a);
			}
		}

		init.debug(cout);
	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 0;
}

int perm_rep::test_localSearch_r()
{
	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/test/", "UK200_01.txt", 3);
	printInstance();

	cout << 1 << endl;
	auto p = randomPermutation();
	Solution init = permutationToSolution(p);
	cout << 2 << endl;

	Solution ls = localSearch_r(init, "shiftC");
	//Solution ls = localSearch_r(init, "2opt");
	cout << 3 << endl;
	getArcsNodes(ls);

	//ls.debug(cout);

	cout << "FO1: " << fixed << init.FO << endl;
	cout << "FO2: " << fixed << ls.FO << endl;

	for (auto i : ls.inf) {
		cout << i << " ";
	}
	cout << endl;

	return 0;
}

int perm_rep::test_VNS()
{
	maxRuntime = 600;// 300;
	start = std::chrono::high_resolution_clock::now();

	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/", "UK200_11.txt", 3);
	printInstance();

	//Solution init = permutationToSolution({ 9, 11, 6, 10, 7, 8, 5 }); 
	//Solution init = permutationToSolution(randomPermutation());
	Solution init = greedDD();
	//getArcsNodes(init);
	//init.debug(cout);
	//Solution init = permutationToSolution({ 11, 8, 10, 6, 7, 5, 9 });

	Solution vns = bVNS_r(init, 25);

	getArcsNodes(vns);
	vns.debug(cout);

	cout << init.FO << " - " << vns.FO << endl;

	return 0;
}

int perm_rep::test_GRASP()
{
	maxRuntime = 300;
	start = std::chrono::high_resolution_clock::now();

	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/", "UK200_01.txt", 3);
	printInstance();


	//Solution init = permutationToSolution({ 9, 11, 6, 10, 7, 8, 5 }); 0 10 9 7 5 11 8 12
	//Solution init = permutationToSolution({ 5, 6, 10, 7, 8, 11, 9 });
	//Solution init = permutationToSolution({ 10, 9, 7, 5, 11, 8, 6 });
	//Solution init = permutationToSolution({ 8, 11, 10, 7, 9 });
	// Solution init = permutationToSolution({ 7, 10, 11, 8, 9 });
	//Solution init = permutationToSolution(randomPermutation());
	Solution init = greedDD();
	getArcsNodes(init);
	init.debug(cout);
	
	init = GRASP_p(init, 10, INT_MAX);
	getArcsNodes(init);
	init.debug(cout);

	return 0;
}

int perm_rep::test_SA()
{
	maxRuntime = 300;
	start = std::chrono::high_resolution_clock::now();

	loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/", "UK200_01.txt", 3);
	printInstance();


	//Solution init = permutationToSolution({ 9, 11, 6, 10, 7, 8, 5 }); 0 10 9 7 5 11 8 12
	//Solution init = permutationToSolution({ 5, 6, 10, 7, 8, 11, 9 });
	//Solution init = permutationToSolution({ 10, 9, 7, 5, 11, 8, 6 });
	//Solution init = permutationToSolution({ 8, 11, 10, 7, 9 });
	// Solution init = permutationToSolution({ 7, 10, 11, 8, 9 });
	//Solution init = permutationToSolution(randomPermutation());
	Solution init = greedDD();
	getArcsNodes(init);
	init.debug(cout);

	init = GRASP_p(init, 10, INT_MAX);
	getArcsNodes(init);
	init.debug(cout);

	return 0;
}

int perm_rep::test_new_evaluation()
{
	

	try {
		loadInstance("D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/", "UK200_01.txt", 3);
		printInstance();

		//init = testPermutation({ 9, 11, 6, 10, 7, 8, 5 }); // UK10_01.txt // optmal
		//permutation p = randomPermutation();
		permutation p = { 192, 92, 115, 158, 78, 68, 83, 153, 144, 180, 60, 165, 151, 155, 206, 64, 87, 99, 184, 62, 162, 103, 58, 127, 193, 85, 187, 96, 131, 159, 104, 86, 108, 128, 175, 149, 176, 95, 139, 112, 133, 66, 121, 100, 202, 148, 110, 170, 105, 174, 84, 73, 168, 181, 111, 123, 195, 164, 147, 163, 90, 130, 189, 138, 93, 137, 116, 145, 188, 171, 77, 199, 182, 74, 207, 63, 88, 152, 61, 142, 57, 69, 129, 113, 169, 126, 173, 106, 114, 141, 190, 120, 208, 198, 67, 160, 81, 79, 178, 122, 89, 156, 177, 179, 186, 82, 97, 109, 59, 197, 70, 205, 136, 75, 172, 203, 118, 140, 200, 76, 72, 117, 146, 132, 167, 94, 91, 101, 143, 191, 65, 154, 161, 107, 196, 135, 201, 124, 125, 204, 102, 166, 134, 194, 98, 157, 71, 185, 150, 80, 183, 119 };
		Solution init = permutationToSolution(p);

		getArcsNodes(init);

		init.debug(cout);

		int i, j, k;
		i = 0;
		j = 2;
		k = 9;
		//cin >> i;
		//cin >> j;
		//cin >> k;

		Solution n = opt2_route_r(init, i, j, k);

		getArcsNodes(n);

		n.debug(cout);

		cout << init.FO << " - " << n.FO << endl;
	}
	catch (PermutationInf e) {
		cout << e.what() << endl;
	}

	return 1;
}

int perm_rep::test2()
{

	permutation p = randomPermutation();

	for (auto i : p) {
		cout << i << " ";
	}
	cout << endl;

	Solution s = permutationToSolution(p);

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
	return 1;
}

int perm_rep::test4()
{

	Solution s = sA(1000, 100, 100, 50, 300);
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

	Solution s1 = permutationToSolution(p);

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

	Solution s2 = permutationToSolutionGrasp(p);

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

float perm_rep::getTravelCost(Solution s)
{
	float count = 0;

	for (route r : s.routes) {
		for (int i = 0; i < r.size() - 1; i++) {
			//cout << r.at(i).key << " - " << r.at(i + 1).key << endl;
			//cout << inst->dist(r.at(i).key, r.at(i + 1).key) << endl;
			//cout << inst->dist(r.at(i).key, r.at(i + 1).key) * inst->driverWage << endl;
			count += inst->dist(r.at(i).key, r.at(i + 1).key) * inst->driverWage;
		}
	}
	return count;
}