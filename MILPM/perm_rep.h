#pragma once
#include "Algorithms.h"

class perm_rep : public Algorithms
{
private:
	solution permutationToSolution(permutation p);
	solution permutationToSolution(permutation p, int cn); // cn is the cendidate`s list`s size
	solution permutationToSolutionGrasp(permutation p);
	solution addDepots(solution s);
	solution addStations(solution s); // add stations to the solution by a greedy criteria (closest ) 
	solution addStations2(solution s); // add stations to the solution by a greedy criteria (closest ) 
	solution addStations(solution s, int n); // add stations to the solution. it uses a greedy criteria and a candidate list
	solution addStationsGrasp(solution s); // grasp

	permutation randomPermutation();
	route computeRoute(route sol);

	node nearestBSS(int key);

	int availableRoute(solution s, int n); // check if there is an vailable route to insert the node n

	solution localSearch(permutation p);
	solution localSearch2(permutation p);
	//solution greed();
	permutation opt2(permutation p, int beg, int end);

	// GA
	solution GA(int popSize, int eliteP, int maxGen);
	vector<solution> generateOffspringPop(vector<solution> pop, int eliteSize);
	vector<permutation> crossover(permutation p1, permutation p2);
	permutation mutation(permutation s);
	vector<float> calcFitness(vector<solution> pop);

	// SA
	// solution sA(int initTemp, int finalTemp, float coolingRate, int maxIt, int maxRuntime);

	// greed
	solution greedDD();
	solution greedRT();

public:
	solution sA(int initTemp, int finalTemp, float coolingRate, int maxIt, int maxRuntime);

	int test2();
	int test3();
	int test4();
	int test5();
};

