#include "instance.h"
#include "Model.h"

instance::instance()
{
}

instance::instance(string file)
{
	dir = "";
	this->fileName = file;
	readPaz();
}

instance::instance(string dir, string file)
{
	this->dir = dir;
	this->fileName = file;
	//readPaz();
}

instance::instance(string dir, string file, int type)
{
	//setDefaultParameters();
	this->dir = dir;
	this->fileName = file;
	this->type = type;
	readInstace();
}

void instance::test()
{
	print(cout);
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

void instance::print2(ostream & strm)
{
	strm << "depots_number                    " << numD << endl;
	strm << "facilities_number                " << numF << endl;
	strm << "customers_number                 " << numC << endl;
	strm << "maximum_num_statios              " << o << endl;
	strm << "LB                               " << LB << endl;
	strm << "UB                               " << UB << endl;
	strm << "M                                " << M << endl;
	strm << fixed;
	strm << "depot_cost                 usd   " << depotCost << endl;
	strm.unsetf(ios_base::fixed);
	strm << "bss_cost                   usd   " << bssCost << endl;
	strm << "brs_cost                   usd   " << brsCost << endl;
	strm << "bss_energy_cost            usd   " << bssEnergyCost << endl;
	strm << "brs_energy_cost            usd   " << brsEnergyCost << endl;
	strm << "vehicle_cost               usd   " << vehicleCost << endl;
	strm << "driver_wage                usd   " << driverWage << endl;
	strm << "depot_lifetime            year   " << depotLifetime << endl;
	strm << "bss_lifetime              year   " << bssLifetime << endl;
	strm << "brs_lifetime              year   " << brsLifetime << endl;
	strm << "vehicle_lifetime          year   " << vehicleLifetime << endl;
	strm << "vehicle_range              km    " << vehicleRange << endl;
	strm << "battery_capacity           kwh   " << Q << endl;
	strm << "battery_swap_time        second  " << ct << endl;
	strm << "battery_consumption_rate    x    " << r << endl;
	strm << "recharging_rate             x    " << g << endl;
	strm << "vehicle_load               kg    " << C << endl;
	strm << "vehicle_speed             km/h   " << v << endl << endl;

	int bigger = 0;
	for (auto i : nodes) {
		if (i.id.size() > bigger) {
			bigger = i.id.size();
		}
	}

	int esp = 3;
	for (auto i : nodes) {
		//strm << i.key << " " << i.id << " " << i.type << " " << i.x << " " << i.y << " " << i.demand << " " << i.readyTime << " " << i.dueDate << " " << i.serviceTime << " " << i.ref << endl;
		strm << setw(3) << i.key << setw(bigger + 3) << i.id << setw(6) << i.type << setw(6) << i.x << setw(6) << i.y << setw(6) << i.demand << setw(6) << i.readyTime << setw(7) << i.dueDate << setw(6) << i.serviceTime << setw(6) << i.ogKey << endl;
	}

	/*
	strm << endl;
	for (auto i : distanceMatrix) {
		for (auto j : i) {
			strm << j << " ";
		}
		strm << endl;
	}
	strm << endl;
	*/
}

void instance::print3(ostream & strm)
{
	strm << "depots_number                    " << numD << endl;
	strm << "facilities_number                " << numF << endl;
	strm << "customers_number                 " << numC << endl;
	strm << "maximum_num_statios              " << o << endl;
	strm << "LB                               " << LB << endl;
	strm << "UB                               " << UB << endl;
	strm << "M                                " << M << endl;
	strm << fixed;
	strm << "depot_cost                 usd   " << depotCost << endl;
	strm.unsetf(ios_base::fixed);
	strm << "bss_cost                   usd   " << bssCost << endl;
	strm << "brs_cost                   usd   " << brsCost << endl;
	strm << "bss_energy_cost            usd   " << bssEnergyCost << endl;
	strm << "brs_energy_cost            usd   " << brsEnergyCost << endl;
	strm << "vehicle_cost               usd   " << vehicleCost << endl;
	strm << "driver_wage                usd   " << driverWage << endl;
	strm << "depot_lifetime            year   " << depotLifetime << endl;
	strm << "bss_lifetime              year   " << bssLifetime << endl;
	strm << "brs_lifetime              year   " << brsLifetime << endl;
	strm << "vehicle_lifetime          year   " << vehicleLifetime << endl;
	strm << "vehicle_range              km    " << vehicleRange << endl;
	strm << "battery_capacity           kwh   " << Q << endl;
	strm << "battery_swap_time        second  " << ct << endl;
	strm << "battery_consumption_rate    x    " << r << endl;
	strm << "recharging_rate             x    " << g << endl;
	strm << "vehicle_load               kg    " << C << endl;
	strm << "vehicle_speed             km/h   " << v << endl << endl;

	int bigger = 0;
	for (auto i : nodes) {
		if (i.id.size() > bigger) {
			bigger = i.id.size();
		}
	}

	int esp = 3;
	for (auto i : nodes) {
		//strm << i.key << " " << i.id << " " << i.type << " " << i.x << " " << i.y << " " << i.demand << " " << i.readyTime << " " << i.dueDate << " " << i.serviceTime << " " << i.ref << endl;
		strm << setw(3) << i.key << setw(bigger + 3) << i.id << setw(6) << i.type << setw(6) << i.x << setw(6) << i.y << setw(6) << i.demand << setw(6) << i.readyTime << setw(7) << i.dueDate << setw(6) << i.serviceTime << setw(6) << i.ogKey << setw(6) << i.ref << setw(6) << i.ref2 << setw(6) << i.id_n << endl;

		//strm << i.key << " " << i.id << " " << i.type << " " << i.x << " " << i.y << " " << i.demand << " " << i.readyTime << " " << i.dueDate << " " << i.serviceTime << " " << i.ref << " " << i.id_n << endl;
	}

}

void instance::print(ostream &stream)
{
	stream << "\nData in " << fileName << " \n\n";
	stream << "Number of depots        : " << numD << endl;
	stream << "Number of stations      : " << numF << endl;
	stream << "Number of clients       : " << numC << endl;
	stream << "Stations: ";
	for (auto i : st) {
		cout << i << " ";
	}
	stream << endl;

	int esp = 4;
	stream << "Freight capacity of the vehicle (C)              : " << C << endl;
	stream << "Battery capacity (Q)                             : " << Q << endl;
	stream << "Swap time of battery (ct)                        : " << ct << endl;
	stream << "Consumption rate of energy per distance unit (r) : " << r << endl;
	stream << "Maximum number of statios to locate (o)          : " << o << endl;
	stream << "LB                                               : " << LB << endl;
	stream << "UB                                               : " << UB << endl;
	stream << "M                                                : " << M << endl;
	stream << "Speed (v)                                        : " << v << endl;
	stream << "Recharging rate (g)                              : " << g << endl;
	stream << "Max D                                            : " << maxD << endl;
	stream << "Max B                                            : " << maxB << endl;
	stream << "Max V                                            : " << maxV << endl;
	
	stream << endl;
	int e = 14;
	//cout << fixed;
	stream << "Depot cost              : " << depotCost << " | " << depotCost / depotLifetime << " | " << depotCost / numC << endl;
	stream << "BSS cost                : " << bssCost << " | " << bssCost / bssLifetime << " | " << bssCost / numC << endl;
	stream << "BRS cost                : " << brsCost << " | " << brsCost / brsLifetime << " | "  << brsCost / numC <<  endl;
	stream << "Vehicle cost            : " << vehicleCost << " | " << vehicleCost / vehicleLifetime << " | " << vehicleCost / numC << endl;
	stream << "Driver wage             : " << driverWage << endl;
	stream << "BRS energy cost         : " << brsEnergyCost << endl;
	stream << "BSS energy cost         : " << bssEnergyCost << endl;
	stream << "Vehicle range           : " << vehicleRange << endl;
	stream << "Depot lifetime          : " << depotLifetime << endl;
	stream << "BSS lifetime            : " << bssLifetime << endl;
	stream << "BRS lifetime            : " << brsLifetime << endl;
	stream << "Vehicle lifetime        : " << vehicleLifetime << endl;
	stream << endl;
	stream << setw(3) << "key" << setw(16) << "id" << setw(15) << "type" << setw(6) << "x" << setw(6) << "y" << setw(6) << "dem" << setw(6) << "rTime" << setw(6) << "dDate" << setw(6) << "sTime" << setw(6) << "ref" << setw(6) << "ref2" << setw(6) << "id_n" << setw(6) << "ogKey" << endl;
	for (auto i : nodes) {
		stream << setprecision(2) << setw(3) << i.key << setw(25) << i.id << setw(6) << i.type << setw(6) << i.x << setw(6) << i.y << setw(6) << i.demand << setw(6) << i.readyTime << setw(6) << i.dueDate << setw(6) << i.serviceTime << setw(6) << i.ref << setw(6) << i.ref2 << setw(6) << i.id_n << setw(6) << i.ogKey << endl;
	}
	return;
	stream << "Arcs: " << endl;
	stream << fixed << setprecision(2);
	for (node i : nodes) {
		for (node j : nodes) {
			stream << i.key << " --> " << j.key << ":  " << dist(i, j) << " - " << getTD(i, j) << endl;
		}
	}
	stream << endl;

}

void instance::printSet(vector<node> set)
{
	for (auto i : set) {
		cout << i.key << " " << i.id << " " << i.type << " " << i.x << " " << i.y << " " << i.id_n << "\n";
	}
}

node instance::getNodeByKey(int key)
{
	for (auto i : nodes) {
		if (i.key == key) {
			return i;
		}
	}
	throw NodeNotFound(key);
}

void instance::adaptAll()
{
	fstream all;
	all.open(dir + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw "error opening file all.txt";
	}

	string line;
	while (getline(all, line)) {
		cout << line << endl;
		fileName = line;
		try {
			adaptUKInstance();
		}
		catch (string s) {
			cout << s << endl;
		}
	}

	all.close();
}
/*
void instance::adaptUKInstance() // write
{
	if (type != 2) {
		throw "cant adapt this instance\n";
	}

	if (numC >= 50) {
		vector<pair<int, long long int>> dists;
		// choose depots
		int i = 0;
		for (node n1 : nodes) {
			if (n1.type != "d") {
				long long int sum = 0;
				for (node n2 : nodes) {
					sum += dist(n1.key, n2.key);
					sum += dist(n2.key, n1.key);
				}
				pair<int, long long int> aux = { i, sum };
				dists.push_back(aux);
			}
		}

		int d = numC % (25) - 1;
		partial_sort(dists.begin(), dists.begin() + d, dists.end(), [](pair<int, long long int> a, pair<int, long long int> b)-> bool {return a.second < b.second; });

	}

	fstream file;
	file.open(dir + fileName + "_a.txt", ios::in | ios::app);
	if (file.is_open() == false) {
		cout << "Error opening file " << fileName + "_a.txt" << endl;
		cout << "In directory " << dir << endl;
		exit(1);
	}

	int ig;
	string str;

	file << "depots_number                       " << numD;
	file << "facilities_number                   " << numF;
	file << "customers_number                    " << numC;
	file << "maximum_num_statios                 " << o; // maximum number of stations to be sited
	file << "LB                                  " << LB;
	file << "UB                                  " << UB;
	file << "M                                   " << M;
	file << "distance_matrix_dimension           " << mDim; // distance matrix dimension
	file << "depot_cost                " << "usd      " << depotCost;
	file << "bss_cost                  " << "usd      " << bssCost;
	file << "brs_cost                  " << "usd      " << brsCost;
	file << "bss_energy_cost           " << "usd      " << bssEnergyCost;
	file << "brs_energy_cost           " << "usd      " << brsEnergyCost;
	file << "vehicle_cost              " << "usd      " << vehicleCost;
	file << "driver_wage               " << "usd      " << driverWage;
	file << "depot_lifetime            " << "year     " << depotLifetime;
	file << "bss_lifetime              " << "year     " << bssLifetime;
	file << "brs_lifetime              " << "year     " << brsLifetime;
	file << "vehicle_lifetime          " << "year     " << vehicleLifetime;
	file << "vehicle_range             " << "km       " << vehicleRange; // vehicle autonomy
	file << "battery_capacity          " << "kwh      " << Q; // battery capacity
	file << "battery_swap_time         " << "second   " << ct; // battery swap time
	file << "battery_consumption_rate  " << "x        " << r; // battery consumption rate
	file << "recharging_rate           " << "x        " << g; // recharging rate
	file << "vehicle_load              " << "kg       " << C; // vehicle load capacity
	file << "vehicle_speed             " << "km/g     " << v; // speed



	int numNodes = numD + numF + numC;
	/*

	// write nodes
	node a;
	for (int i = 0; i < numNodes; i++) {
		file << a.key << a.id << a.type << a.x << a.y << a.demand << a.readyTime << a.dueDate << a.serviceTime << a.ogKey << endl;
	}

	distanceMatrix.resize(mDim);
	for (int i = 0; i < mDim; i++) {
		>distanceMatrix.at(i).resize(inst->mDim);
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
void instance::setDefaultParameters()
{


	bssCost = 500000; // cost of siting a battery swap station
	brsCost = 126500; // cost of siting a battery recharging station
	driverWage = 13.14 / 1000; // cost per m travelled
	vehicleCost = 70000; // fixed cost of a vehicle, https://www.theverge.com/2019/9/19/20873947/amazon-electric-delivery-van-rivian-jeff-bezos-order
	brsEnergyCost = 0.32 / 1000; // cost per energy unit in the battery recharging stations
	bssEnergyCost = 0.15 / 1000; // cost per energy unit in the battery swap stations
	depotCost = 92000000; // cost of siting a depot, https://www.bizjournals.com/seattle/news/2017/06/09/amazon-to-spend-200m-on-fulfillment-center-for-130.html
	stationCap = 2; //station capacity
	vehicleRange = 161;
	depotLifetime = 40 * 365;
	bssLifetime = 20 * 365;
	brsLifetime = 10 * 365;
	vehicleLifetime = 20 * 365;
	alfa = 0.2; // percentagem of customers that will be transformed in stattions siting locations, used on the UK instances

}

void instance::readInstace()
{
	if (type == 0) {
		readPaz();
	}
	if (type == 1) {
		readSSG14();
	}
	if (type == 2) {
		adaptUKInstance();
	}
	if (type == 3) {
		readUKAdapt();
	}
	maxDist = 0;
	minDist = 9999999;
	for (int i = 0; i < distanceMatrix.size(); i++) {
		for (int j = 0; j < distanceMatrix.at(i).size(); j++) {
			if (i != j){
				float d = distanceMatrix.at(i).at(j);
				if (d >= maxDist) {
					maxDist = d;
				}
				else if (d < maxDist) {
					minDist = d;
				}
			}			
		}
	}
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

	//vector<vector<int>> m;
	//m.resize(numC + 1);
	distanceMatrix.resize(numC + 1);
	for (int i = 0; i < numC + 1; i++) {
		distanceMatrix.at(i).resize(numC + 1);
		for (int j = 0; j < numC + 1; j++) {
			float d;
			file >> d;
			d = d;// / 1000; // convert from meters to kilometers
			distanceMatrix.at(i).at(j) = d;
		}
	}

	// read depot node
	node a;
	a.type = "d";
	a.x = -1;
	a.y = -1;
	a.id_n = 1;
	file >> a.key >> a.id >> a.demand >> a.readyTime >> a.dueDate >> a.serviceTime;


	// convert seconds to hours
	/*
	a.readyTime /= (60 * 60);
	a.dueDate /= (60 * 60);
	a.serviceTime /= (60 * 60);
	*/
	a.ogKey = a.key;
	this->nodes.push_back(a);

	
	// create the duplicated depot node as a siting place
	a.type = "f";
	a.id_n = -1;
	nodes.push_back(a);
	

	// read customers nodes
	vector<node> customers;
	for (int i = 0; i < numC; i++) {
		a.type = "c";
		file >> a.key >> a.id >> a.demand >> a.readyTime >> a.dueDate >> a.serviceTime;

		// convert seconds to hours
		/*
		a.readyTime /= (60 * 60);
		a.dueDate /= (60 * 60);
		a.serviceTime /= (60 * 60);
		*/
		a.ogKey = a.key;
		customers.push_back(a);
	}
	numC = customers.size();

	int numFacilities;
	file >> numFacilities;
	
	for (int i = 0; i < numFacilities; i++) {
		int aux;
		file >> aux;
		st.insert(aux);
	}
	
	// create the nodes of stations with selected customers. the customersNodes is modified without the stations
	vector<node> stations = chooseStationsLocationMCLP(customers);

	// insert the stations nodes
	nodes.insert(nodes.end(), stations.begin(), stations.end());

	// insert the customers nodes
	nodes.insert(nodes.end(), customers.begin(), customers.end());

	// adding depot arrival nodes
	for (auto i : nodes) {
		if (i.type == "d") {
			i.ref2 = i.key; // modification
			i.type = "a";
			nodes.push_back(i);
		}		
	}

	// organize intial nodes key
	for (int i = 0; i < nodes.size(); i++) {
		nodes.at(i).key = i;
		nodes.at(i).ref = -1;
		nodes.at(i).ref2 = i;
		//nodes.at(i).id_n = i;
	}

	addDummyNodes();

	// get number of clients, depots and stations
	numC = 0;
	numD = 0;
	numF = 0;
	for (node n : nodes) {
		if (n.type == "d") {
			numD++;
		}
		else if (n.type == "f") {
			numF++;
		}
		else if (n.type == "c") {
			numC++;
		}
	}

	// create a vector of edges to becames easier to get distances in this type of instance
	createEdgesVector();

	// calculating extra parameters
	Q = 161000;// *60 * 60;// The battery capacity of the vehicle is set to 80 kWh, as in the work of Davis et al. [40]. // the conversion between KWh to KWs was done
	v = v / 3.6; // convert the speed to m/s;
	vehicleRange = 161 * 1000; // meters
	r = (vehicleRange / Q); // vehicleRange
	g = 4;	
	fullRechargeTime = Q / g;
	ct = 600;
	o = numC + numF;
	LB = 0; // wrong
	UB = numC + numF;
	M = 2 * (Q + set_C().size() + set_UD0()[0].dueDate);
}

void instance::adaptUKInstance()
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

	//vector<vector<int>> m;
	//m.resize(numC + 1);
	distanceMatrix.resize(numC + 1);
	for (int i = 0; i < numC + 1; i++) {
		distanceMatrix.at(i).resize(numC + 1);
		for (int j = 0; j < numC + 1; j++) {
			float d;
			file >> d;
			d = d;// / 1000; // convert from meters to kilometers
			distanceMatrix.at(i).at(j) = d;
		}
	}

	// read depot node
	node a;
	a.type = "d";
	a.x = -1;
	a.y = -1;
	a.id_n = 1;
	file >> a.key >> a.id >> a.demand >> a.readyTime >> a.dueDate >> a.serviceTime;
	a.ogKey = a.key;
	this->nodes.push_back(a);

	// read customers nodes
	vector<node> customers;
	for (int i = 0; i < numC; i++) {
		a.type = "c";
		file >> a.key >> a.id >> a.demand >> a.readyTime >> a.dueDate >> a.serviceTime;
		a.ogKey = a.key;
		customers.push_back(a);
	}
	numC = customers.size();

	// read facilities
	int numFacilities;
	file >> numFacilities;
	for (int i = 0; i < numFacilities; i++) {
		int aux;
		file >> aux;
		st.insert(aux);
	}

	// create the nodes of stations with selected customers. the customersNodes is modified without the stations
	vector<node> stations = chooseStationsLocationMCLP2(customers);

	if (stations.size() == 0) {
		return;
	}

	// choose additional depots
	if (numC >= 50) {
		vector<pair<int, long long int>> dists;
		int i = 0;

		vector<node> C = customers;

		for (node n1 : C) {
			if (n1.type != "d" && n1.type != "a") {
				long long int sum = 0;
				for (node n2 : C) {
					int mean = floor(distanceMatrix.at(n1.key).at(n2.key) + distanceMatrix.at(n2.key).at(n1.key) / 2);
					sum += mean;
				}
				pair<int, long long int> aux = { n1.key, sum };
				dists.push_back(aux);
			}
		}

		sort(dists.begin(), dists.end(), [](pair<int, long long int> a, pair<int, long long int> b)-> bool {return a.second < b.second; });

		vector<int> idpts;
		int d = numC / (25) - 1;
		int num = 0;
		// choose d depots		
		for (auto i : dists) {
			if (num == d) {
				break;
			}

			bool isBSS = false;
			for (node j : stations) {
				if (j.ogKey == i.first) {
					isBSS = true;
					break;
				}
			}
			if (isBSS == false) {
				num++;
				idpts.push_back(i.first);
			}
		}

		vector<node> dpts;
		for (int i : idpts) {
			int a = 0;
			for (node c : customers) {
				if (c.key == i) {
					c.type = "d";
					c.demand = 0;
					c.dueDate = 32400;
					c.readyTime = 0;
					c.serviceTime = 0;
					dpts.push_back(c);
					customers.erase(customers.begin() + a);
				}
				//customers.erase(customers.begin() + a);
				a++;
			}
		}

		// insert extra depots
		nodes.insert(nodes.end(), dpts.begin(), dpts.end());
	}

	// duplicate depots nodes as bss
	int s = nodes.size();
	for (int i = 0; i < s; i++) {
		node n = nodes.at(i);
		n.type = "f";
		n.serviceTime = 600;
		n.id_n = -1;
		nodes.push_back(n);
	}

	// insert the stations nodes
	nodes.insert(nodes.end(), stations.begin(), stations.end());

	// insert the customers nodes
	nodes.insert(nodes.end(), customers.begin(), customers.end());

	/*
	// adding depot arrival nodes
	for(int i = 0; i < nodes.size(); i++){
	//for (auto i : nodes) {
		if (nodes.at(i).type == "d") {
			node n = nodes.at(i);
			n.ref2 = nodes.at(i).key; // modification
			n.type = "a";
			nodes.push_back(n);
		}
	}*/

	// organize intial nodes key
	for (int i = 0; i < nodes.size(); i++) {
		nodes.at(i).key = i;
		nodes.at(i).ref = -1;
		nodes.at(i).ref2 = i;
		//nodes.at(i).id_n = i;
	}

	//addDummyNodes();

	// get number of clients, depots and stations
	numC = 0;
	numD = 0;
	numF = 0;
	for (node n : nodes) {
		if (n.type == "d") {
			numD++;
		}
		else if (n.type == "f") {
			numF++;
		}
		else if (n.type == "c") {
			numC++;
		}
	}

	// create a vector of edges to becames easier to get distances in this type of instance
	//createEdgesVector();

	// calculating extra parameters
	Q = 80;// *60 * 60;// The battery capacity of the vehicle is set to 80 kWh, as in the work of Davis et al. [40]. // the conversion between KWh to KWs was done
	v = v;// / 3.6; // convert the speed to m/s;
	vehicleRange = 161; // meters
	r = 1;// (vehicleRange / Q); // vehicleRange
	g = 4;
	fullRechargeTime = Q / g;
	ct = 600;
	o = numC + numF;
	LB = 0; // wrong
	UB = numC + numF;
	M = 2 * (Q + set_C().size() + set_UD0()[0].dueDate);
	mDim = distanceMatrix.size();
	this->depotCost = 92000000;
	this->bssCost = 500000;
	this->brsCost = 126500;
	this->bssEnergyCost = 0.15;
	this->brsEnergyCost = 0.32;
	this->vehicleCost = 70000;
	this->driverWage = 13.14;
	this->depotLifetime = 40;
	this->bssLifetime = 20;
	this->brsLifetime = 10;
	this->vehicleLifetime = 20;

	// determine the maximum number of depots
	if (numD >= 2){
		int min = int(1);
		int max = int(numD / 2) + 1;
		maxD = Random::get(min, max);
	}
	else {
		maxD = numD;
	}

	// determine the maximum number of vehicles
	maxV = ceil(double(numC) * 0.1) + 1;

	// determine the maximum number of bss
	maxB = ceil(double(numF) * 0.2) + 1;
	o = maxB;

	fstream out;
	out.open(dir + fileName + ".ukelrp", ios::ate | ios::out);
	if (out.is_open() == false) {
		throw "error creating adapted instance for file " + fileName + "\n";
	}
	else{
		writeAdapt(out);
		out.close();
	}

}

void instance::readUKAdapt()
{
	type = 2;
	fstream file;
	file.open(dir + fileName + ".ukelrp", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file " << fileName + ".ukelrp" << endl;
		cout << "In directory " << dir << endl;

		isOpen = false;

		return;
	}
	isOpen = true;

	int ig;
	string str;

	file >> str >> numD;
	file >> str >> numF;
	file >> str >> numC;
	file >> str >> o; // maximum number of stations to be sited
	file >> str >> LB;
	file >> str >> UB;
	file >> str >> M;
	file >> str >> maxD;
	file >> str >> maxB;
	file >> str >> maxV;
	file >> str >> mDim; // distance matrix dimension
	file >> str >> str >> depotCost;
	file >> str >> str >> bssCost;
	file >> str >> str >> brsCost;
	file >> str >> str >> bssEnergyCost;
	file >> str >> str >> brsEnergyCost;
	file >> str >> str >> vehicleCost;
	file >> str >> str >> driverWage;
	file >> str >> str >> depotLifetime;
	file >> str >> str >> bssLifetime;
	file >> str >> str >> brsLifetime;
	file >> str >> str >> vehicleLifetime;
	file >> str >> str >> vehicleRange; // vehicle autonomy
	file >> str >> str >> Q; // battery capacity
	file >> str >> str >> ct; // battery swap time
	file >> str >> str >> r; // battery consumption rate
	file >> str >> str >> g; // recharging rate
	file >> str >> str >> C; // vehicle load capacity
	file >> str >> str >> v; // speed

	int numNodes = numD + numF + numC;

	// read nodes
	node a;
	for (int i = 0; i < numNodes; i++) {
		file >> a.key >> a.id >> a.type >> a.x >> a.y >> a.demand >> a.readyTime >> a.dueDate >> a.serviceTime >> a.ogKey;
		nodes.push_back(a);
	}

	distanceMatrix.resize(mDim);
	for (int i = 0; i < mDim; i++) {
		distanceMatrix.at(i).resize(mDim);
		for (int j = 0; j < mDim; j++) {
			float d;
			file >> d;
			d = d;// / 1000; // convert from meters to kilometers
			distanceMatrix.at(i).at(j) = d;
		}
	}

	
	// give all depot nodes a specific id
	int id = 1;
	for (int i = 0; i < nodes.size(); i++) {
		if (nodes.at(i).type == "d") {
			nodes.at(i).id_n = id;
			id++;
		}
	}

	// adding depot arrival nodes
	vector<node> UD0 = set_UD0();
	node n;
	for (int i = 0; i < UD0.size(); i++) {
		UD0.at(i).ref2 = i;
		n = UD0.at(i);
		n.type = "a";		
		nodes.push_back(n);
	}

	// organize intial nodes key
	for (int i = 0; i < nodes.size(); i++) {
		nodes.at(i).key = i;
		nodes.at(i).ref = -1;
		nodes.at(i).ref2 = i;
		//nodes.at(i).id_n = i;
	}

	addDummyNodes();

	// create a vector of edges to becames easier to get distances in this type of instance
	createEdgesVector();

	//o = numC + numF;
	//LB = 0; // wrong
	//UB = numC + numF;
	//M = 2 * (Q + set_C().size() + set_UD0()[0].dueDate);

	// measures units 
	driverWage /= 1000;
	depotLifetime *= 365;
	bssLifetime *= 365;
	brsLifetime *= 365;
	vehicleLifetime *= 365;
	//depotCost /= depotLifetime;
	//bssCost /= bssLifetime;
	//brsCost /= brsLifetime;
	//vehicleCost /= vehicleLifetime;
	vehicleRange *= 1000;
	bssEnergyCost /= 1000;
	brsEnergyCost /= 1000;
	Q = vehicleRange; // battery capacity
	ct; // battery swap time
	//r = 1; // battery consumption rate
	g = 54;//4; // recharging rate
	C; // vehicle load capacity
	//v = 90; // speed

	// calculating extra parameters
	//Q *= 1000;// *60 * 60;// The battery capacity of the vehicle is set to 80 kWh, as in the work of Davis et al. [40]. // the conversion between KWh to KWs was done
	o = numC + numF;
	LB = 0; // wrong
	UB = numC + numF;
	M = 2 * (Q + set_C().size() + set_UD0()[0].dueDate);
	
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
vector<node> instance::chooseStationsLocationRandom(vector<node> &customers)
{
	if (numC < 50) { // all clients are candidates
		vector<node> stations;
		for (node c : customers) {
			node r = c;
			r.type = "f";
			stations.push_back(r);
		}

		return stations;
	}
	else { // only 20% of clients are candidates
		set <int, less <int> > cStations;
		float num = ceil(alfa * numC); // number of stations

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
					n.type = "f";
					stations.push_back(n);
				}
			}
		}
		
		// remove the selecteds customers from the customer vector
		customers = removeNodesByIndex(customers, cStations);

		return stations;
	}
}

vector<node> instance::chooseStationsLocationMCLP(vector<node> &customers)
{
	vector<node> stations;
	
	for (int i : st) {
		node n = customers.at(i - 1);
		n.type = "f";
		stations.push_back(n);
	}

	// remove the selecteds customers from the customer vector
	customers = removeNodesByIndex(customers, st);

	return stations;
}

vector<node> instance::chooseStationsLocationMCLP2(vector<node>& customers)
{
	vector<node> stations;

	for (int i : st) {
		node n = customers.at(i - 1);
		n.type = "f";
		n.demand = 0;
		n.dueDate = 32400;
		n.readyTime = 0;
		n.serviceTime = 600;
		stations.push_back(n);
	}

	// remove the selecteds customers from the customer vector
	//customers = removeNodesByIndex(customers, st);

	return stations;
}

// receive a vector nodes and a set of indexes, then remove all nodes with the indexes in the index set
vector<node> instance::removeNodesByIndex(vector<node> customers, set<int> ind)
{
	vector<node> n;
	for (int j = 0; j < customers.size(); j++) {
		bool isin = false;
		for (int i : ind) {
			if (i == customers.at(j).key) {
				isin = true;
			}
		}
		if (isin == false) {
			n.push_back(customers.at(j));
		}
	}
	return n;
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

void instance::writeAdapt(ostream &file)
{
	int ig;
	string str;
	//file << fixed;// << setprecision(2);
	file << "depots_number                       " << numD << endl;
	file << "facilities_number                   " << numF << endl;
	file << "customers_number                    " << numC << endl;
	file << "maximum_num_statios                 " << o << endl; // maximum number of stations to be sited
	file << "LB                                  " << LB << endl;
	file << "UB                                  " << UB << endl;
	file << "M                                   " << M << endl;
	file << "max_depots                          " << maxD << endl;
	file << "max_bss                             " << maxB << endl;
	file << "max_vehicle                         " << maxV << endl;
	file << "distance_matrix_dimension           " << mDim << endl; // distance matrix dimension
	file << "depot_cost                 " << "usd      " << depotCost << endl;
	file << "bss_cost                   " << "usd      " << bssCost << endl;
	file << "brs_cost                   " << "usd      " << brsCost << endl;
	file << "bss_energy_cost            " << "usd      " << bssEnergyCost << endl;
	file << "brs_energy_cost            " << "usd      " << brsEnergyCost << endl;
	file << "vehicle_cost               " << "usd      " << vehicleCost << endl;
	file << "driver_wage                " << "usd      " << driverWage << endl;
	file << "depot_lifetime             " << "year     " << depotLifetime << endl;
	file << "bss_lifetime               " << "year     " << bssLifetime << endl;
	file << "brs_lifetime               " << "year     " << brsLifetime << endl;
	file << "vehicle_lifetime           " << "year     " << vehicleLifetime << endl;
	file << "vehicle_range              " << "km       " << vehicleRange << endl; // vehicle autonomy
	file << "battery_capacity           " << "kwh      " << Q << endl; // battery capacity
	file << "battery_swap_time          " << "second   " << ct << endl; // battery swap time
	file << "battery_consumption_rate   " << "x        " << r << endl; // battery consumption rate
	file << "recharging_rate            " << "x        " << g << endl; // recharging rate
	file << "vehicle_load               " << "kg       " << C << endl; // vehicle load capacity
	file << "vehicle_speed              " << "km/h     " << v << endl; // speed
	file << endl;

	int numNodes = numD + numF + numC;
	// write nodes
	file << fixed << setprecision(0);
	for (auto i : nodes) {
		file << setw(3) << i.key << setw(25) << i.id << setw(6) << i.type << setw(6) << i.x << setw(6) << i.y << setw(6) << i.demand << setw(6) << i.readyTime << setw(6) << i.dueDate << setw(6) << i.serviceTime << setw(6) << i.ogKey << endl;

		//file << setw(3) << i.key << setw(25) << i.id << setw(6) << i.type << setw(6) << i.x << setw(6) << i.y << setw(6) << i.demand << setw(6) << i.readyTime << setw(6) << i.dueDate << setw(6) << i.serviceTime << setw(6) << i.ref << setw(6) << i.ref2 << setw(6) << i.id_n << setw(6) << i.ogKey << endl;
	}
	file << endl;
	
	for (auto i : distanceMatrix) {
		for (auto j : i) {
			file << j << " ";
		}
		file << endl;
	}
	//////////////////////////////////////////////////////
}

float instance::dist(int a, int b)
{
	node na = getNodeByKey(a);
	node nb = getNodeByKey(b);
	if (type == 0 || type == 1)
		return sqrt(pow(nb.x - na.x, 2) + pow(nb.y - na.y, 2));
	else if (type == 2) {
		return distanceMatrix.at(na.ogKey).at(nb.ogKey);
		//return distEdges(na.ogKey, nb.ogKey);
	}
}

float instance::dist(node a, node b)
{
	if (type == 0 || type == 1)
		return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
	else if (type == 2) {
		return distanceMatrix.at(a.ogKey).at(b.ogKey);
		//return distEdges(na.ogKey, nb.ogKey);
	}
}

float instance::distEdges(int keya, int keyb)
{
	float dist = 0;
	for (edge e : edges) {
		if (e.beg == keya && e.end == keyb) {
			return e.value;
		}
	}
	cout << "Error edge not found between nodes " << keya << " and " << keyb << endl;
	return -1;
}

float instance::getBatteryUsed(int keya, int keyb)
{
	float distAB = dist(keya, keyb);
	return 1 * distAB;
}

float instance::getTD(node a, node b)
{
	float d = dist(a, b);
	return ceil(d / (v));
}

float instance::getTD(int a, int b)
{
	float d = dist(a, b);
	return ceil(d / (v));
}

float instance::getS(int key) // verify
{
	return nodes.at(key).serviceTime;
}

float instance::getCT()
{
	return ct;
}

vector<node> instance::vectorUnion(vector<node> a, vector<node> b)
{
	bool isin;
	for (auto i : b) {
		isin = false;
		for (auto j : a) {
			if (i.key == j.key) {
				isin = true;
				break;
			}
		}
		if (isin == false) {
			a.push_back(i);
		}
	}

	return a;
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
			n.type = "f";//"r";
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

void adaptAll()
{
	string dir3 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/";
	fstream all;
	all.open(dir3 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw "error opening file all.txt";
	}

	string line;
	while (getline(all, line)) {
		cout << line << endl;
		instance i(dir3, line);
		try {
			i.adaptUKInstance();
		}
		catch (string s) {
			cout << s << endl;
		}
	}

	all.close();
}
