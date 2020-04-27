#pragma once
#include <BRKGA.h>
#include <MTRand.h>
#include "Algorithms.h"

typedef pair<double, unsigned> ValueKeyPair;

class perm_rep : public Algorithms
{
private:
	string dirOutput;

	Solution permutationToSolution(permutation p);
	Solution permutationToSolution(permutation p, int cn); // cn is the cendidate`s list`s size
	Solution permutationToSolutionGrasp(permutation p);
	Solution addDepots(Solution s);
	Solution addStations(Solution s); // add stations to the solution by a greedy criteria (closest ) 
	route addStations(route rt); // add stations to the solution by a greedy criteria (closest ) 
	Solution addStations_model(Solution s); // add stations to the solution by a greedy criteria (closest ) 
	Solution addStations(Solution s, int n); // add stations to the solution. it uses a greedy criteria and a candidate list
	Solution addStationsGrasp(Solution s); // grasp

	//
	pair<int, int> closestBSS(Solution& s, int route, int key); // search for the closest bss of a node with a given key, return the pair (bss key, distance)
	pair<int, int> closestBSS(route& r, int key); // search for the closest bss of a node with a given key, return the pair (bss key, distance)
	pair<int, int> closestBSS(Solution& s, int route, int key, int m); // search for the closest bss of a node with a given key and given a candidate list determined by the parameter m (list size), return the pair (bss key, distance)

	permutation randomPermutation();
	route computeRoute(route sol);

	node nearestBSS(int key);

	int availableRoute(Solution s, int n); // check if there is an vailable route to insert the node n

	Solution localSearch(permutation p);
	Solution localSearch2(permutation p);
	//solution greed();
	permutation opt2(permutation p, int beg, int end);

	//GA
	vector<Solution> generateOffspringPop(vector<Solution> pop, int eliteSize);
	vector<permutation> crossover(permutation p1, permutation p2);
	permutation mutation(permutation s);
	vector<float> calcFitness(vector<Solution> pop);

	//SA
	// solution sA(int initTemp, int finalTemp, float coolingRate, int maxIt, int maxRuntime);

	// GRASP
	// this GRASP algorithm is responsible to improve the addDepot method
	Solution GRASP(Solution s);
	Solution localSearch_GRASP(Solution s);
	Solution routeSplit(Solution s, int n, int m); // split route n in two in a given position m (m < s.route.at(n).size() - 2)
	Solution bssReplacement(Solution s, int key); // Replace a bss defined by n and swap  it for others, aready placed, bss

	// BGKGA	
	vector<ValueKeyPair> v;

	// greed
	Solution greedDD();
	Solution greedRT();

public:

	Solution GA(int popSize, int eliteP, int maxGen);
	Solution sA(int initTemp, int finalTemp, float coolingRate, int maxIt, int maxRuntime);
	Solution BRKGA_();
	Solution testPermutation(permutation p);
	float decode(vector<double> chromosome);
	void setOutputDir(string dir);

	int test1();
	int test2();
	int test3();
	int test4();
	int test5();
	
	// aux
	float getTravelCost(Solution s);
};

