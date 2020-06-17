#pragma once
#include <BRKGA.h>
#include <MTRand.h>
#include "Algorithms.h"

//#define DEBUG

typedef pair<double, unsigned> ValueKeyPair;

class perm_rep : public Algorithms
{
private:
	string dirOutput;

	Solution permutationToSolution(permutation p);
	Solution permutationToSolution(permutation p, int cn); // cn is the cendidate`s list`s size
	Solution permutationToSolutionAlt(permutation p);
	Solution permutationToSolutionGrasp(permutation p);

	Solution choosePermutationToSolution(permutation p, int n);

	Solution addDepots(Solution s);
	Solution addDepotsN(Solution s, int n);
	Solution addStations_(Solution s); // add stations to the solution by a greedy criteria (closest ) 
	Solution addStations(Solution s); // add stations to the solution by a greedy criteria (closest ) 
	route addStations(route rt);	
	Solution addStations(Solution s, int n); // add stations to the solution. it uses a greedy criteria and a candidate list
	Solution addStationsAlt(Solution s); // add stations in the route, 	
	route addStations(route rt, int n); // choose a bss to be sited
	route addStationsAlt(route rt); // choose a place to sit a bss

	Solution addStations_OLD(Solution s, int n);
	route addStations_OLD(route rt); // add stations to the solution by a greedy criteria (closest ) 
	Solution addStations_model(Solution s); // add stations to the solution by a greedy criteria (closest ) 

	bool rechargeSchedule(route &r, int beg, int end, int energy);
	vector<int> knapSack(route r, int beg, int end);

	//
	pair<int, int> closestBSS(Solution& s, int route, int key); // search for the closest bss of a node with a given key, return the pair (bss key, distance)
	pair<int, int> closestBSS(route& r, int key); // search for the closest bss of a node with a given key, return the pair (bss key, distance)
	pair<int, int> closestBSS(route& r, int key, int n); // search for the closest bss of a node with a given key, return the pair (bss key, distance)
	pair<int, int> closestBSS(Solution& s, int route, int key, int m); // search for the closest bss of a node with a given key and given a candidate list determined by the parameter m (list size), return the pair (bss key, distance)
	int minimalDeviationBSS(route r, int v);
	int minimalDeviationBSS(int v1, int v2);

	permutation randomPermutation();
	route computeRoute(Solution s, int r);
	route computeRoute(route sol); // similar to addStations but add stations to a given route

	node nearestBSS(int key);

	int availableRoute(Solution s, int n); // check if there is an vailable route to insert the node n

	route removeBSS_beg_end(route r);
	route removeBSS_beg(route r);
	route removeBSS_end(route r);
	route removeBSS(route r);
	route removeBSS(route r, int pos);

	Solution localSearch(permutation p);
	Solution localSearch_light(permutation p);
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
	Solution GRASP_p(Solution s, int maxIt, int maxRunTime);
	Solution bVNS_r(Solution s); // VNS // localSearch_GRASP
	Solution rVNS_r(Solution s); // VNS // localSearch_GRASP

	Solution localSearch_r(Solution s, string n);
	Solution routeSplit_r(Solution s, int n, int m); // split route n in two in a given position m (m < s.route.at(n).size() - 2)
	Solution bssReplacement_r(Solution s, int key); // Replace a bss defined by n and swap  it for others, aready placed, bss
	Solution routePartition_r(Solution s, int r1, int p1, int r2, int p2);
	Solution opt2_route_r(Solution s, int j, int beg, int end); // i = route, j = beg, i = end
	Solution shiftCustomer_r(Solution s, int j, int c, int q);	
	Solution changeDepot(Solution s, int r, int keyDPT);
	Solution swapDepot(Solution s, int kDPTA, int kDPTB);
	Solution shakeRandom_r(Solution s, int n);

	// BGKGA	
	vector<ValueKeyPair> v;

	// greed
	Solution greedDD();
	Solution greedRT();

public:
	Solution GRASP(int maxIt, int maxRuntime);
	Solution GA(int popSize, int eliteP, int maxGen);
	Solution sA(int initTemp, int finalTemp, float coolingRate, int maxIt, int maxRuntime);
	Solution BRKGA_();
	Solution testPermutation(permutation p);
	float decode(vector<double> chromosome);
	void setOutputDir(string dir);
	bool getArcsNodes(Solution& s);

	int test_permutationToSolutionAlt();
	int test_permutationToSolution();
	int test_addStations();
	int test_shakeRandom_r(int i);
	int test_routePartition();
	int test_opt2_route();
	int test_shiftCustomer();
	int test_swapDepot();
	int test_neigh();
	int test_localSearch_r();
	int test_VNS();
	int test_GRASP();
	int test_new_evaluation();
	
	int test2();
	int test3();
	int test4();
	int test5();
	
	// aux
	float getTravelCost(Solution s);
};

