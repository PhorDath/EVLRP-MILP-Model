#include "Solution.h"

arc Solution::getArc(int a, int b)
{
	for (arc ac : arcs) {
		if (ac.beg == a && ac.end == b) {
			return ac;
		}
	}
	throw "arc_not_found";
}

Solution::Solution()
{

}

void Solution::strmSol(ostream& strm)
{
	strm << "FO: " << FO << endl;
	strm << endl;

	strm << "FOp: ";
	for (float f : FOp) {
		strm << f << " ";
	}
	strm << endl;
	strm << endl;

	strm << "Inf: ";
	for (string s : inf) {
		strm << s << " ";
	}
	strm << endl;
	strm << endl;

	strm << "Routes: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.key << " ";
		}
		strm << endl;
	}
	strm << endl;
	strm << "vLoad: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.vLoad << " ";
		}
		strm << endl;
	}
	strm << endl;
	strm << "Blevel: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.bLevel << " ";
		}
		strm << endl;
	}
	strm << endl;
	strm << "aTime: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.aTime << " ";
		}
		strm << endl;
	}
	strm << endl;
	strm << "wTime: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.wTime << " ";
		}
		strm << endl;
	}
	strm << endl;
	strm << "lTime: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.lTime << " ";
		}
		strm << endl;
	}
	strm << endl;
	strm << "recharged: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.recharged << " ";
		}
		strm << endl;
	}
	strm << endl;
	strm << "recharge: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.recharge << " ";
		}
		strm << endl;
	}
	strm << endl;
}

void Solution::printSol()
{
	cout << "FO: " << FO << " - ";
	for (int i : FOp) {
		cout << i << " ";
	}
	cout << endl;
	cout << "Inf: ";
	for (string i : inf) {
		cout << i << " ";
	}
	cout << endl;
	for (route r : routes) {
		for (vertex v : r) {
			cout << v.key << " ";
		}
		cout << endl;
	}
}

void Solution::saveXML(string file)
{
	ptree tree;
	tree.add("routes.<xmlattr>.version", "1.0");
	int i = 1;
	
	for (route r : routes) {
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
	
	write_xml(file, tree);

}

void Solution::strmFO(ostream& strm)
{
	strm << "Objective function : " << FOp.front() << endl;
	strm << "Depot siting cost  : " << FOp.at(1) << endl;
	strm << "BSS siting cost    : " << FOp.at(2) << endl;
	strm << "Vehicle cost       : " << FOp.at(3) << endl;
	strm << "Driving cost       : " << FOp.at(4) << endl;
	strm << "BRS energy cost    : " << FOp.at(5) << endl;
	strm << "BSS energy cost    : " << FOp.at(6) << endl;
	strm << "Punishment         : " << FOp.at(7) << endl;
}

int Solution::set(vector<vector<vertex>> s)
{
	this->routes = s;
	
	return 0;
}

void Solution::getRoutes()
{
	vector<arc> initial;

	// get all arcs leaving the depots
	for (int i = 0; i < arcs.size(); i++) {

	}

}

void Solution::insertArc(int beg, int end, int value)
{
	arc a;
	a.beg = beg;
	a.end = end;
	a.value = value;

	arcs.push_back(a);
}

void Solution::debug(ostream& strm)
{
	strm << fixed << setprecision(2);
	strm << "Runtime: " << runtime << endl;
	strm << "Objective function    : " << FOp.front() << endl;
	strm << "	Depot siting cost  : " << FOp.at(1) << endl;
	strm << "	BSS siting cost    : " << FOp.at(2) << endl;
	strm << "	Vehicle cost       : " << FOp.at(3) << endl;
	strm << "	Driving cost       : " << FOp.at(4) << endl;
	strm << "	BRS energy cost    : " << FOp.at(5) << endl;
	strm << "	BSS energy cost    : " << FOp.at(6) << endl;
	strm << "	BSS use cost       : " << FOp.at(7) << endl;
	strm << "	Punishment         : " << FOp.at(8) << endl;
	strm << endl;
	strm << "Infeasibility: \n";
	for (string i : inf) {
		strm << "\t" << i << endl;
	}
	strm << "Permutation: ";
	for (int i : perm) {
		strm << i << ", ";
	}
	strm << endl;
	strm << "Routes: \n";
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.key << " ";
		}
		strm << endl;
	}
	strm << endl;
	
	strm << "Distances: \n";
	for (route r : routes) {
		for (int i = 0; i < r.size() - 1; i++) {
			try {
				strm << getArc(r.at(i).key, r.at(i + 1).key).value << " ";
			}
			catch (string s) {
				if (s == "arc_not_found") {
					cout << s << endl;
				}
			}
			catch (exception e) {
				cout << e.what() << endl;
			}
		}
		strm << endl;
	}
	strm << endl;

	strm << "Travelling time: \n";
	for (route r : routes) {
		for (int i = 0; i < r.size() - 1; i++) {
			try {
				strm << getArc(r.at(i).key, r.at(i + 1).key).value2 << " ";
			}
			catch (string s) {
				if (s == "arc_not_found") {
					cout << s << endl;
				}
			}
			catch (exception e) {
				cout << e.what() << endl;
			}
		}
		strm << endl;
	}
	strm << endl;

	strm << "Due date: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.n.dueDate << " ";
		}
		strm << endl;
	}
	strm << endl;

	strm << "Arrival time: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.aTime << " ";
		}
		strm << endl;
	}
	strm << endl;

	strm << "Ready time: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.n.readyTime << " ";
		}
		strm << endl;
	}
	strm << endl;

	strm << "Waiting time: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.wTime << " ";
		}
		strm << endl;
	}
	strm << endl;

	strm << "Service time: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.n.serviceTime << " ";
		}
		strm << endl;
	}
	strm << endl;

	strm << "Departure time: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.lTime << " ";
		}
		strm << endl;
	}
	strm << endl;

	strm << "Battery level: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.bLevel << " ";
		}
		strm << endl;
	}
	strm << endl;

	strm << "Vehicle load: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.vLoad << " ";
		}
		strm << endl;
	}
	strm << endl;

	strm << "Demands: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.n.demand << " ";
		}
		strm << endl;
	}
	strm << endl;

	strm << "Recharge: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.recharge << " ";
		}
		strm << endl;
	}
	strm << endl;

	strm << "Recharged: " << endl;
	for (route r : routes) {
		for (vertex v : r) {
			strm << v.recharged << " ";
		}
		strm << endl;
	}
	strm << endl;
}

Solution::~Solution()
{
}

bool compIndividual(Solution a, Solution b)
{
	return a.FO < b.FO;
}

bool compRT(node a, node b)
{
	return a.readyTime < b.readyTime;
}

bool compDD(node a, node b)
{
	return a.dueDate < b.dueDate;
}

bool compKey(node a, node b)
{
	return a.key < b.key;
}