#pragma once
#include <fstream>
#include <random>
#include <chrono>
#include <exception>
#include <stdlib.h>
#include <time.h>
#include <iterator>
#include "instance.h"
#include "Usefull.h"
#include "effolkronium/random.hpp"

using namespace std;
using Random = effolkronium::random_static;

const double EulerConstant = exp(1.0);

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

class IsolatedNode : public exception {

private:
	string msg;

public:
	IsolatedNode(const int n, const int r, const vector<int> perm){
		msg = "node " + to_string(n) + " in route " + to_string(r) + " cant be reached\n";
		for (int i : perm) {
			msg += (to_string(i) + " ");
		}
		msg += "\n";
	}

	const char * what() const throw () {
		return msg.c_str();
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
	permutation perm;

	int time;
	int FOINIT;
	int status;
};

bool compIndividual(solution a, solution b);
bool compRT(node a, node b);
bool compDD(node a, node b);

class Algorithms
{
public:

	vector<bool> coverage;
	vector<vector<vertex>> sol; // solution vector
	vector<int> fo_parcels;
	string row = "";

	// aux	
	solution procSol(solution s); // processes solution
	vector<vector<vertex>> getAllRoutesByBSS(vector<vector<vertex>> sol);
	int FO(vector<vector<vertex>> sol);
	int routeFO(route r);
	vector<int> FOComplete(vector<vector<vertex>> sol);
	vector<int> FOComplete_old(vector<vector<vertex>> sol);
	int FOP(vector<vector<vertex>> sol);
	int eval(vector<vector<vertex>> sol);
	vector<string> fullEval(vector<vector<vertex>> sol);
	vector<vertex> fixRoute(vector<vertex> sol);
	route optRoute(route r);
	
	bool tcCoverage(vector<bool> coverage); // check if all customers are already beeing supplied by a route

	// extra	
	void printPartialRoute(vector<vertex> r);
	void debugSol(vector<vector<vertex>> s);
	void strmSol(solution sol, ostream& strm);

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
	void getSol2(solution sol, ostream &strm);
	string getRow(solution s);

	//
	
	void getSol(ostream &strm, solution sol);
	void getSol2(ostream &strm, solution sol);

	solution createOptimialSolution1();

	~Algorithms();
};

