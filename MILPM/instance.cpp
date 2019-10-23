#include "instance.h"

bool compKey(node a, node b)
{
	return a.key < b.key;
}

instance::instance(string file)
{
	dir = "";
	this->fileName = file;
	readInstace();
}

instance::instance(string dir, string file)
{
	this->dir = dir;
	this->fileName = file;
	readInstace();
}

instance::instance(string dir, string file, int t)
{
	if (t == 0) {
		this->dir = dir;
		this->fileName = file;
		readPaz();
	}
	if (t == 1) {
		this->dir = dir;
		this->fileName = file;
		readSSG14();
	}
	if (t == 2) {
		this->dir = dir;
		this->fileName = file;
		readprplib();
	}
}

void instance::test()
{
	print();
}

// this functions can be optimized if nodes are read always in the same order (DFC)
vector<node> instance::set_UD0()
{
	vector<node> aux;
	for (auto i : nodes) {
		if (i.type == "d") {
			aux.push_back(i);
		}
	}
	return aux;
}

vector<node> instance::set_UD1()
{
	vector<node> aux;
	for (auto i : nodes) {
		if (i.type == "a") {
			aux.push_back(i);
		}
	}
	return aux;
}

vector<node> instance::set_UD()
{
	vector<node> UD0 = set_UD0();
	vector<node> UD1 = set_UD1();
	UD0.insert(UD0.end(), UD1.begin(), UD1.end());
	return UD0;
}

vector<node> instance::set_C()
{
	vector<node> aux;
	for (auto i : nodes) {
		if (i.type == "c") {
			aux.push_back(i);
		}
	}
	return aux;
}

vector<node> instance::set_R()
{
	vector<node> aux;
	for (auto i : nodes) {
		if (i.type == "f") {
			aux.push_back(i);
		}
	}
	return aux;
}

vector<node> instance::set_SK(int n)
{
	vector<node> temp;
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes.at(i).ref == n) {
			temp.push_back(nodes.at(i));
		}
	}
	return temp;
}

vector<node> instance::set_S()
{
	vector<node> temp;
	int total = 2 * numD + numF + numC;
	for (int i = 0; i < total; i++) {
		vector<node> aux;
		aux = set_SK(i);
		temp.insert(temp.end(), aux.begin(), aux.end());
	}
	return temp;
}

vector<node> instance::set_V()
{
	vector<node> C = set_C();
	vector<node> R = set_R();
	vector<node> S = set_S();	
	C.insert(C.end(), R.begin(), R.end());
	C.insert(C.end(), S.begin(), S.end());
	return C;
}


vector<node> instance::set_C0()
{
	vector<node> C = set_C();
	vector<node> UD0 = set_UD0();
	C.insert(C.end(), UD0.begin(), UD0.end());
	return C;
}

vector<node> instance::set_V0()
{
	vector<node> V = set_V();
	vector<node> UD0 = set_UD0();
	V.insert(V.end(), UD0.begin(), UD0.end());
	return V;
}

vector<node> instance::set_V1()
{
	vector<node> V = set_V();
	vector<node> UD1 = set_UD1();
	V.insert(V.end(), UD1.begin(), UD1.end());
	return V;
}

vector<node> instance::set_V01()
{
	vector<node> V = set_V();
	vector<node> UD0 = set_UD0();
	vector<node> UD1 = set_UD1();
	V.insert(V.end(), UD0.begin(), UD0.end());
	V.insert(V.end(), UD1.begin(), UD1.end());
	return V;
}

vector<node> instance::sortSet(vector<node> set)
{
	sort(set.begin(), set.end(), compKey);
	return set;
}

void instance::print()
{
	cout << numD << " " << numF << " " << numC << endl;
	for (auto i : nodes) {
		cout << i.key << " " << i.id << " " << i.type << " " << i.x << " " << i.y << " " << i.demand << " " << i.readyTime << " " << i.dueDate << " " << i.serviceTime << " " << i.ref << " " << i.id_n << endl;
	}
	cout << Q << endl;
	cout << C << endl;
	cout << r << endl;
	cout << g << endl;
	cout << v << endl;

	/*
	cout << "Set: \n";
	cout << "UD0: \n"; printSet(set_UD0());
	cout << "UD1: \n"; printSet(set_UD1());
	cout << "UD: \n"; printSet(set_UD());
	cout << "C: \n"; printSet(set_C());
	cout << "R: \n"; printSet(set_R());
	cout << "S: \n"; printSet(set_S());
	cout << "V: \n"; printSet(set_V());
	cout << "C0: \n"; printSet(set_C0());
	cout << "V0: \n"; printSet(set_V0());
	cout << "V1: \n"; printSet(set_V1());
	cout << "V01: \n"; printSet(set_V01());
	*/
}

void instance::printSet(vector<node> set)
{
	for (auto i : set) {
		cout << i.key << " " << i.id << " " << i.type << " " << i.x << " " << i.y << " " << i.id_n << "\n";
	}
}

void instance::readInstace()
{
	readPaz();
}

void instance::readPaz()
{
	type = 0;
	fstream file;
	file.open(dir + fileName, ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file " << fileName << endl;
		cout << "In directory " << dir << endl;
		exit(1);
	}

	string line;
	getline(file, line);

	node aux;
	int k = 0;
	while (true) {
		file >> aux.id;

		if (aux.id.at(0) == 'Q') {
			break;
		}

		aux.key = k;
		k++;
		aux.ref = -1;
		aux.ref2 = aux.key;

		file >> aux.type >> aux.x >> aux.y >> aux.demand >> aux.readyTime >> aux.dueDate >> aux.serviceTime;

		if (aux.type == "d") {
			numD++;
			aux.id_n = k;
		}			
		else if (aux.type == "f") {
			numF++;
			aux.id_n = -1;
		}			
		else if (aux.type == "c") {
			numC++;
			aux.id_n = -1;
		}		

		nodes.push_back(aux);

	}

	// adding depot arrival nodes
	int total = numC + numD + numF;
	for (auto i : nodes) {
		if (i.type != "d") {
			break;
		}
		i.ref2 = i.key; // modification
		i.key += total;
		i.type = "a";
		nodes.push_back(i);
	}

	// adding dummy nodes (set S)
	int size = nodes.size();
	int a = size;
	for (int j = 0; j < revis; j++) {
		for (int i = 0; i < size; i++) {
			if (nodes.at(i).type != "d" && nodes.at(i).type != "a" && nodes.at(i).type != "c") {
			//if (nodes.at(i).type != "" && nodes.at(i).type != "" && nodes.at(i).type != "") {
				node temp = nodes.at(i);
				temp.key = a;// total + numD; // new key
				a++;
				temp.type = temp.type + "_d"; // new type
				temp.ref = nodes.at(i).key; // reference of dummy node
				temp.ref2 = nodes.at(i).key; // reference of dummy node
				nodes.push_back(temp);
			}
		}
	}

	string ig;
	
	// getting Q
	file >> ig >> ig >> ig >> ig >> ig;
	ig.erase(std::remove(ig.begin(), ig.end(), '/'), ig.end());
	this->Q = stof(ig);

	// getting C
	file >> ig >> ig >> ig >> ig >> ig;
	ig.erase(std::remove(ig.begin(), ig.end(), '/'), ig.end());
	this->C = stof(ig);

	// getting r
	file >> ig >> ig >> ig >> ig >> ig;
	ig.erase(std::remove(ig.begin(), ig.end(), '/'), ig.end());
	this->r = stof(ig);

	// getting g
	file >> ig >> ig >> ig >> ig >> ig;
	ig.erase(std::remove(ig.begin(), ig.end(), '/'), ig.end());
	this->g = stof(ig);

	// getting v
	file >> ig >> ig >> ig >> ig >> ig;
	ig.erase(std::remove(ig.begin(), ig.end(), '/'), ig.end());
	this->v = stof(ig);
	
	// calculating extra parameters
	fullRechargeTime = Q * g;
	ct = 0.1 * fullRechargeTime;

	o = numC + numF;

	LB = 0; // wrong
	UB = numC + numF;
	M = 2 * (Q + set_C().size() + set_UD0()[0].dueDate);
}

void instance::readprplib()
{
	type = 2;
	fstream file;
	file.open(dir + fileName, ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file " << fileName << endl;
		cout << "In directory " << dir << endl;
		exit(1);
	}

	int ig;

	file >> numC;
	file >> ig >> C;
	file >> ig >> v;

	vector<vector<int>> m;
	m.resize(numC + 1);
	for (int i = 0; i < numC + 1; i++) {
		m.at(i).resize(numC + 1);
		for (int j = 0; j < numC + 1; j++) {
			file >> m.at(i).at(j);
		}
	}
	// create a vector of edges to becames easier to get distances in this type of instance
	createEdgesVector();


	// read depot node
	node a;
	a.type = "d";
	a.x = -1;
	a.y = -1;
	a.ref = 0;
	a.ref2 = 0;
	file >> a.key >> a.id >> a.demand >> a.readyTime >> a.dueDate >> a.serviceTime;
	this->nodes.push_back(a);

	// read customers nodes
	vector<node> customersNodes;
	for (int i = 0; i < numC; i++) {
		a.type = "c";
		file >> a.key >> a.id >> a.demand >> a.readyTime >> a.dueDate >> a.serviceTime;
		customersNodes.push_back(a);
	}
	numC = customersNodes.size();
	
	// create the nodes of stations with selected customers. the customersNodes is modified without the stations
	vector<node> stations = chooseStationsLocation(customersNodes);

	
	addDummyNodes();

	Q = 80; // The battery capacity of the vehicle is set to 80 kWh, as in the work of Davis et al. [40].


}

void instance::addDummyNodes()
{
	// adding dummy nodes (set S)
	int size = nodes.size();
	int a = size;
	for (int j = 0; j < revis; j++) {
		for (int i = 0; i < size; i++) {
			if (nodes.at(i).type != "d" && nodes.at(i).type != "a" && nodes.at(i).type != "c") {
				//if (nodes.at(i).type != "" && nodes.at(i).type != "" && nodes.at(i).type != "") {
				node temp = nodes.at(i);
				temp.key = a;// total + numD; // new key
				a++;
				temp.type = temp.type + "_d"; // new type
				temp.ref = nodes.at(i).key; // reference of dummy node
				temp.ref2 = nodes.at(i).key; // reference of dummy node
				nodes.push_back(temp);
			}
		}
	}
}

//  this function receives the vector with all customers nodes and return the nodes choosen to be stations sites, the original vector with customers is modified without those nodes
vector<node> instance::chooseStationsLocation(vector<node> &customers)
{
	set <int, less <int> > cStations;
	int num = (alfa * numC); // number of stations

	// select the customers node that will become stations sites
	default_random_engine generator;
	uniform_int_distribution<int> distribution(0, numC);
	while (cStations.size() < num) {
		int s = distribution(generator);
		cStations.insert(s);
	}

	// create a vector only with the selected customers to turn then into stations
	vector<node> stations;
	for (node n : customers) {
		for (int i : cStations) {
			if (i == n.key) {
				n.type = "r";
			}
		}
	}

	// remove the selecteds customers from the customer vector
	customers = removeNodesByIndex(customers, cStations);

	return stations;
}

// receive a vector nodes and a set of indexes, then remove all nodes with the indexes in the index set
vector<node> instance::removeNodesByIndex(vector<node> customers, set<int> ind)
{
	for (int i : ind) {
		for (node j : customers) {
			if (i == j.key) {
				remove(customers.begin(), customers.end(), j);
			}
		}
	}
	return customers;
}

void instance::rearrangeDMatrix(vector<vector<float>>& m)
{

}

// use the distance matrix to create the edges vector
void instance::createEdgesVector()
{
	for (int i = 0; i < distanceMatrix.size(); i++) {
		for (int j = 0; j < distanceMatrix.at(i).size(); j++) {
			edge e;
			e.beg = i;
			e.end = j;
			e.value = distanceMatrix.at(i).at(j);
			this->edges.push_back(e);
		}
	}
}

void instance::printNode(node i)
{
	cout << i.key << " " << i.id << " " << i.type << " " << i.x << " " << i.y << " " << i.demand << " " << i.readyTime << " " << i.dueDate << " " << i.serviceTime << " " << i.ref << " " << i.id_n << endl;
}

void instance::readSSG14()
{
	type = 1;
	// source: https://akrzemi1.wordpress.com/2011/07/13/parsing-xml-with-boost/

	// populate tree structure pt
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(dir + fileName, pt);

	// get depots
	int k = 0;
	BOOST_FOREACH(ptree::value_type const& v, pt.get_child("Instance").get_child("Vertices").get_child("Depot")) {
		if (v.first == "Vertex") {
			node n;

			n.key = k;
			k++;
			n.ref = -1;
			n.ref2 = n.key;
			n.id_n = k;

			n.id = v.second.get<std::string>("StrID");
			n.type = "d";
			n.x = v.second.get<float>("X");
			n.y = v.second.get<float>("Y");
			n.demand = v.second.get<float>("Demand");
			n.serviceTime = v.second.get<float>("ServiceTime");
			n.readyTime = v.second.get<float>("EarliestTimeOfArrival");
			n.dueDate = v.second.get<float>("LatestTimeOfArrival");
			//v.second.get("<xmlattr>.cancelled", false);
			nodes.push_back(n);
			numD++;
		}
	}
	// get stations
	BOOST_FOREACH(ptree::value_type const& v, pt.get_child("Instance").get_child("Vertices").get_child("Stations")) {
		if (v.first == "Vertex") {
			node n;

			n.key = k;
			k++;
			n.ref = -1;
			n.ref2 = n.key;
			n.id_n = -1;

			n.id = v.second.get<std::string>("StrID");
			n.type = "r";
			n.x = v.second.get<float>("X");
			n.y = v.second.get<float>("Y");
			n.serviceTime = v.second.get<float>("Demand");
			n.readyTime = v.second.get<float>("EarliestTimeOfArrival");
			n.dueDate = v.second.get<float>("LatestTimeOfArrival");
			//v.second.get("<xmlattr>.cancelled", false);
			nodes.push_back(n);
			numF++;
		}
	}

	// get customers
	BOOST_FOREACH(ptree::value_type const& v, pt.get_child("Instance").get_child("Vertices").get_child("Customers")) {
		if (v.first == "Vertex") {
			node n;

			n.key = k;
			k++;
			n.ref = -1;
			n.ref2 = n.key;
			n.id_n = -1;

			n.id = v.second.get<std::string>("StrID");
			n.type = "c";
			n.x = v.second.get<float>("X");
			n.y = v.second.get<float>("Y");
			n.serviceTime = v.second.get<float>("Demand");
			n.readyTime = v.second.get<float>("EarliestTimeOfArrival");
			n.dueDate = v.second.get<float>("LatestTimeOfArrival");
			//v.second.get("<xmlattr>.cancelled", false);
			nodes.push_back(n);
			numC++;
		}
	}
	BOOST_FOREACH(ptree::value_type const& v, pt.get_child("Instance")) {
		// getting Q
		if (v.first == "Parameters") {
			Q = v.second.get<float>("FuelCapacity");
		}

		// getting C
		if (v.first == "Parameters") {
			C = v.second.get<float>("StorageCapacity");
		}

		// getting r
		if (v.first == "Parameters") {
			r = v.second.get<float>("ConsumptionRate");
		}

		// getting g
		if (v.first == "Parameters") {
			g = v.second.get<float>("RefuelingRate");
		}

		// getting v
		if (v.first == "Parameters") {
			this->v = v.second.get<float>("VehicleVelocity");
		}
	}

	// adding depot arrival nodes
	int total = numC + numD + numF;
	for (auto i : nodes) {
		if (i.type != "d") {
			break;
		}
		i.ref2 = i.key; // modification
		i.key += total;
		i.type = "a";
		nodes.push_back(i);
	}

	// adding dummy nodes (set S)
	int size = nodes.size();
	int a = size;
	for (int j = 0; j < revis; j++) {
		for (int i = 0; i < size; i++) {
			if (nodes.at(i).type != "d" && nodes.at(i).type != "a" && nodes.at(i).type != "c") {
				//if (nodes.at(i).type != "" && nodes.at(i).type != "" && nodes.at(i).type != "") {
				node temp = nodes.at(i);
				temp.key = a;// total + numD; // new key
				a++;
				temp.type = temp.type + "_d"; // new type
				temp.ref = nodes.at(i).key; // reference of dummy node
				temp.ref2 = nodes.at(i).key; // reference of dummy node
				nodes.push_back(temp);
			}
		}
	}

	// calculating extra parameters
	fullRechargeTime = Q * g;
	ct = 0.1 * fullRechargeTime;

	o = numC + numF;

	LB = 0; // wrong
	UB = numC + numF;
	M = 2 * (Q + set_C().size() + set_UD0()[0].dueDate);
}

instance::~instance()
{
}
