#pragma once
#include <fstream>
#include <random>
#include <chrono>
#include <exception>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "instance.h"
#include "Usefull.h"

using namespace std;

class NoImprovementExcp : public exception {

	const char * what() const throw () {
		return "No improvement found";
	}
};

struct MovementFailed : public exception {
	const char * what() const throw () {
		return "Failing to perform a movement in the solution";
	}
};

typedef vector<vertex> route;
typedef vector<route> routes;
struct item {
	int key;
	int nodeKey;
};
typedef vector<int> permutation;

struct solution {
	int FO;
	vector<int> FOp;
	vector<string> inf;
	vector<vector<vertex>> routes;
};

class Algorithms
{
private:

	vector<bool> coverage;
	vector<vector<vertex>> sol; // solution vector
	vector<int> fo_parcels;
	string row = "";

	solution permutationToSolution(permutation p);
	solution addDepots(solution s);
	solution addStations(solution s);
	permutation randomPermutation();
	node nearestBSS(int key);

	int availableRoute(solution s, int n); // check if there is an vailable route to insert the node n

	// GA
	solution generateSolution(permutation p);

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

	// aux	
	solution procSol(solution s); // processes solution
	vector<vector<vertex>> getAllRoutesByBSS(vector<vector<vertex>> sol);
	int FO(vector<vector<vertex>> sol);
	int routeFO(route r);
	vector<int> FOComplete(vector<vector<vertex>> sol);
	int FOP(vector<vector<vertex>> sol);
	int eval(vector<vector<vertex>> sol);
	vector<string> fullEval(vector<vector<vertex>> sol);
	vector<vertex> fixRoute(vector<vertex> sol);
	route optRoute(route r);
	
	bool tcCoverage(vector<bool> coverage); // check if all customers are already beeing supplied by a route


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

	// extra	
	void printPartialRoute(vector<vertex> r);
	void debugSol(vector<vector<vertex>> s);

public:
	instance *inst;

	Algorithms();

	int loadInstance(string dir, string fileName, int type);
	void printInstance();
	void printSol(solution s);
	void printSol();
	void getSol(ostream &strm);
	void getSol(Solution sol, ostream &strm);
	void getSol(solution sol, ostream &strm);
	string getRow();

	int test();
	int test2();
	solution greed();
	solution greed2();
	solution VNS(solution init);

	solution createOptimialSolution1();

	~Algorithms();
};

