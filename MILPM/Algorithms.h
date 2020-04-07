#pragma once
#include <fstream>
#include <random>
#include <chrono>
#include <exception>
#include <stdlib.h>
#include <time.h>
#include <iterator>
#include "Solution.h"
#include "instance.h"
#include "Util.h"
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

class Algorithms
{
public:

	vector<bool> coverage;
	vector<vector<vertex>> sol; // solution vector
	vector<float> fo_parcels;
	string row = "";

	// aux	
	Solution procSol(Solution s); // processes solution
	vector<vector<vertex>> getAllRoutesByBSS(vector<vector<vertex>> sol);
	float FO(vector<vector<vertex>> sol);
	int routeFO(route r);
	vector<float> FOComplete(vector<vector<vertex>> sol);
	vector<float> FOComplete_old(vector<vector<vertex>> sol);
	int FOP(vector<vector<vertex>> sol);
	int eval(vector<vector<vertex>> sol);
	vector<string> fullEval(vector<vector<vertex>> sol);
	vector<string> fullEval(Solution s);
	vector<vertex> fixRoute(vector<vertex> sol);
	route optRoute(route r);
	
	bool tcCoverage(vector<bool> coverage); // check if all customers are already beeing supplied by a route

	// extra	
	void printPartialRoute(vector<vertex> r);
	void debugSol(vector<vector<vertex>> s);

public:
	instance *inst;

	Algorithms();

	int loadInstance(string dir, string fileName, int type);
	void printInstance();
	//void getSol(ostream &strm);
	void getSol(Solution sol, ostream &strm);
	//void getSol(Solution sol, ostream &strm);
	void getSol2(Solution sol, ostream &strm);
	string getRow(Solution s);

	//
	
	void getSol(ostream &strm, Solution sol);
	void getSol2(ostream &strm, Solution sol);

	Solution createOptimialSolution1();

	~Algorithms();
};

