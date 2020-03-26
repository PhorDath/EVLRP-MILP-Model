#pragma once
#include "Algorithms.h"

class perm_rep : public Algorithms
{
private:
	string dirOutput;

	Solution permutationToSolution(permutation p);
	Solution permutationToSolution(permutation p, int cn); // cn is the cendidate`s list`s size
	Solution permutationToSolutionGrasp(permutation p);
	Solution addDepots(Solution s);
	Solution addStations(Solution s); // add stations to the solution by a greedy criteria (closest ) 
	Solution addStations2(Solution s); // add stations to the solution by a greedy criteria (closest ) 
	Solution addStations(Solution s, int n); // add stations to the solution. it uses a greedy criteria and a candidate list
	Solution addStationsGrasp(Solution s); // grasp

	permutation randomPermutation();
	route computeRoute(route sol);

	node nearestBSS(int key);

	int availableRoute(Solution s, int n); // check if there is an vailable route to insert the node n

	Solution localSearch(permutation p);
	Solution localSearch2(permutation p);
	//solution greed();
	permutation opt2(permutation p, int beg, int end);

	// GA
	vector<Solution> generateOffspringPop(vector<Solution> pop, int eliteSize);
	vector<permutation> crossover(permutation p1, permutation p2);
	permutation mutation(permutation s);
	vector<float> calcFitness(vector<Solution> pop);

	// SA
	// solution sA(int initTemp, int finalTemp, float coolingRate, int maxIt, int maxRuntime);

	// GRASP
	// this GRASP algorithm is responsible to improve the addDepot method
	Solution GRASP(Solution s);

	// greed
	Solution greedDD();
	Solution greedRT();

public:

	Solution GA(int popSize, int eliteP, int maxGen);
	Solution sA(int initTemp, int finalTemp, float coolingRate, int maxIt, int maxRuntime);
	void setOutputDir(string dir);

	int test1();
	int test2();
	int test3();
	int test4();
	int test5();
	
	// aux
	float getTravelCost(Solution s);
};

