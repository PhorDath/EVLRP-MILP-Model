#include "Solution.h"



Solution::Solution()
{

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

void Solution::insertStation(int s)
{
	stations.push_back(s);
}

void Solution::print()
{
	cout << endl;
	if (stations.size() == 0) {
		cout << "No stations sited.\n";
	}
	else {
		cout << "Stations will be sited at : ";
		for (auto i : stations) {
			cout << i << " ";
		}
	}
	
	cout << endl;
	for (int i = 0; i < routes.size(); i++) {
		cout << "Route " << i + 1 << " begin at " << routes.at(i).at(0).key << endl;
		cout << "  The initial battery level is " << routes.at(i).at(0).bLevel << endl;

		for (int j = 1; j < routes.at(i).size(); j++) {
			int beg = routes.at(i).at(j - 1).key;
			int end = routes.at(i).at(j).key;
			cout << "Travel from ";
			cout << beg << " to " << end << endl;
			cout << "Arrival in " << end << " at           : " << arrivalTime.at(end) << endl;
			cout << "Battery level in " << end << "        : " << batteryLevel.at(end) << endl;
			// check if freight is left at end
			if (freightLeft.at(end) != 0) {
				cout << "Freight left in " << end << "         : " << freightLeft.at(end) << endl;
			}
			if (energyCharged.at(end) != 0) {
				cout << "Energy recharged in " << end << "     : " << energyCharged.at(end) << endl;
			}
			cout << endl;
		}
		cout << "___________________________________________________\n";		
	}
}


Solution::~Solution()
{
}
