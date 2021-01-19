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
#include "MCLPModel.h"

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
	IsolatedNode(const int n){
		msg = "node " + to_string(n) + " isolated";
	}

	const char * what() const throw () {
		return msg.c_str();
	}
};
class PermutationInf : public exception {

private:
	permutation p;
	string msg;

public:
	PermutationInf(const vector<int> perm) {
		p = perm;
		msg = "permutation ";
		for (int i : p) {
			msg += (to_string(i) + " ");
		}
		msg += " is infeasible\n";
	}

	const char* what() const throw () {
		return msg.c_str();
	}
};
class OutOfBSS : public exception {

private:
	string msg;

public:
	OutOfBSS() {
		msg = "out_of_stations";
	}

	const char* what() const throw () {
		return msg.c_str();
	}
};
class SplitRoute : public exception {

private:
	string msg;
	int point;

public:
	SplitRoute(int point) {
		this->point = point;
		msg = "split_route";
	}

	int getPoint() {
		return point;
	}

	const char* what() const throw () {
		return msg.c_str();
	}
};
class BegEnd : public exception {

private:
	string msg;

public:
	BegEnd(const int beg, const int end) {
		msg = "invalid beg ( " + to_string(beg) + " ) and end ( " + to_string(end) + " argument";
	}

	const char* what() const throw () {
		return msg.c_str();
	}
};
class InvalidRoute : public exception {

private:
	string msg;

public:
	InvalidRoute(const int r) {
		msg = "invalid route ( " + to_string(r) + " )";
	}

	const char* what() const throw () {
		return msg.c_str();
	}
};
class InvalidRouteEnd : public exception {

private:
	string msg;

public:
	InvalidRouteEnd(const int r, const int end) {
		msg = "invalid route (" + to_string(r) + ") end ( " + to_string(end) + " )";
	}

	const char* what() const throw () {
		return msg.c_str();
	}
};
class Unreachable : public exception {

private:
	string msg;

public:
	Unreachable(const int beg, const int end) {
		msg = to_string(end) + " unreachable";
	}

	const char* what() const throw () {
		return msg.c_str();
	}
};
class UnfeasibleInstance : public exception {

private:
	string msg;

public:
	UnfeasibleInstance(string name) {
		msg = name + " is unfeasible";
	}

	const char* what() const throw () {
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
	chrono::steady_clock::time_point start;
	int prevTime = 0;

	// aux	
	Solution procSol(Solution s); // processes solution
	vector<vector<vertex>> getAllRoutesByBSS(vector<vector<vertex>> sol);
	float FO(vector<vector<vertex>> sol);
	int routeFO(route r);
	vector<float> FOComplete(routes sol);
	vector<float> FOComplete_NonAmortized(routes sol);
	Solution fixSol(Solution s, route r, int pos);
	vector<float> FOComplete_old(vector<vector<vertex>> sol);
	int FOP(vector<vector<vertex>> sol);
	int eval(vector<vector<vertex>> sol);
	vector<string> fullEval(vector<vector<vertex>> sol);
	vector<string> fullEval(Solution s);
	vector<string> evalRoute(route r);
	vector<string> evalRoute_(route r);
	vector<vertex> fixRoute(vector<vertex> sol);
	route optRoute(route r);
	
	bool tcCoverage(vector<bool> coverage); // check if all customers are already beeing supplied by a route

	set<int> getListBSS(Solution s);

	// permrep
	int getNumC(route r);
	vector<int> getListC(route r);
	int getNumD(Solution s);
	int getPreviousStatioPos(route& r, int n);

	// extra	
	void printPartialRoute(vector<vertex> r);
	void debugSol(vector<vector<vertex>> s);

public:
	instance *inst;
	long long maxRuntime;

	Algorithms();
	~Algorithms();

	int loadInstance(string dir, string fileName, int type);
	bool removeBSS(vector<string> BSS);
	bool removeDPT(vector<string> DPT);
	void printInstance();
	//void getSol(ostream &strm);
	void getSol(Solution sol, ostream &strm);
	//void getSol(Solution sol, ostream &strm);
	void getSol2(Solution sol, ostream &strm);
	string getRow(Solution s);

	//
	
	void getSol(ostream &strm, Solution sol);
	void getSol2(ostream &strm, Solution sol);
	void writeSolutionBin(string file, Solution s);
	Solution readSolutionBin(string file);
	void writeSolutionTxt(string file, Solution s);
	Solution readSolutionTxt(string file);
	void writeRoutes(ostream &strm, Solution s);


	void solutionToXML(Solution s);

	Solution createOptimialSolution1();

	void adaptInstance();

	
};

