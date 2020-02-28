#pragma once
#include "Algorithms.h"

class routes_rep :
	public Algorithms
{
private:
	// greed
	bool allCovered();
	int depotHeuristc();
	int depotHeuristic(vector<bool> coverage);
	bool isCovered(int key);
	int chooseNextNode(vector<vertex> partialRoute);
	int getNearestNode(vector<vertex> route);
	int getNearestFeasibleNode(vector<vertex> &route);
	int getNearestFeasibleNode2(vector<vertex> route);
	int getNearestFeasibleNode3(vector<vertex> route);
	int getNearestFeasibleNode4(vector<vertex> route);
	int getNearestStationSite(int key);
	int getNearestCustomerNotCovered(int key);
	int getLowestRTimeNotCovered(vector<vertex> partialRoute);
	vector<int> checkFeasibility(vector<vertex> route, node c);
	vector<vertex> arrangeRoute(vector<vertex> route, int key);
	vector<vertex> addVertexToRoute(vector<vertex> route, int key);
	vector<vertex> addInfVertexToRoute(vector<vertex> route, int key, vector<int> inf);

	// greed 2
	int getLowestDemand(vector<bool> c); // get lowest demand from customers not supplied yet
	bool checkCustomersCoverage(vector<bool> coverage);
	int chooseNextNode(route r, vector<bool> coverage);

	// metaheuristics
	solution shake(solution s, string n);
	solution nChange(solution s, solution best, int &k);

	// searches
	solution search_2opt(solution sol, solution best);
	solution search_2swap(solution sol, solution best);
	solution search_iRouteSwap(solution sol, solution best);

	// local searches
	solution localSearch_2opt(solution sol);
	solution localSearch_2swap(solution sol);
	solution localSearch_iRouteSwap(solution sol);

	// neighborhoods
	solution opt2(solution sol, int route, int beg, int end);
	solution swap2(solution sol, int route, int beg, int end);
	solution iRouteSwap2(solution sol, int route1, int route2, int p1, int p2);

	solution greed();
	solution greed2();
	solution VNS(solution init);

public:
	int test();
};

