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
	Solution shake(Solution s, string n);
	Solution nChange(Solution s, Solution best, int &k);

	// searches
	Solution search_2opt(Solution sol, Solution best);
	Solution search_2swap(Solution sol, Solution best);
	Solution search_iRouteSwap(Solution sol, Solution best);

	// local searches
	Solution localSearch_2opt(Solution sol);
	Solution localSearch_2swap(Solution sol);
	Solution localSearch_iRouteSwap(Solution sol);

	// neighborhoods
	Solution opt2(Solution sol, int route, int beg, int end);
	Solution swap2(Solution sol, int route, int beg, int end);
	Solution iRouteSwap2(Solution sol, int route1, int route2, int p1, int p2);

	Solution greed();
	Solution greed2();
	Solution VNS(Solution init);

public:
	int test();
};

