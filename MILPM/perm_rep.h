#pragma once
//#include <MTRand.h>
#include <map>
#include "Algorithms.h"

#define DEBUG
//#define DEBUG_GRASP
//#define DEBUG_VNS
//#define DEBUG_HILL_DESCENT

class perm_rep : public Algorithms
{
private:
	string dirOutput;
	vector<string> neighbors = { }; //  "swapD" 

	Solution permutationToSolution(permutation p);
	Solution permutationToSolution_();
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

	//
	pair<int, int> closestBSS(Solution& s, int route, int key); // search for the closest bss of a node with a given key, return the pair (bss key, distance)
	pair<int, int> closestBSS(route& r, int key); // search for the closest bss of a node with a given key, return the pair (bss key, distance)
	pair<int, int> closestBSS(route& r, int key, int n); // search for the closest bss of a node with a given key, return the pair (bss key, distance)
	pair<int, int> closestBSS(Solution& s, int route, int key, int m); // search for the closest bss of a node with a given key and given a candidate list determined by the parameter m (list size), return the pair (bss key, distance)
	int minimalDeviationBSS(route r, int v);
	int minimalDeviationBSS(int v1, int v2);

	pair<int, int> chooseBSS(route& rt, int beg, int end); // search for the closest BSS from the target node (the BSS must be reachable from the initial node)

	permutation randomPermutation();
	route computeRoute(Solution s, int r);
	route computeRoute(route sol); // similar to addStations but add stations to a given route
	node nearestBSS(int key);
	int availableRoute(Solution s, int n); // check if there is an vailable route to insert the node n
	void computeNode(Solution& s, int r, int n); // compute the n node from the route r bases in the node n-1
	vertex computeVertex(vertex v1, vertex v2); // compute the n node from the route r bases in the node n-1
	vertex computeVertex(vertex v1, node n);
	void initDepot(vertex& v);

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

	//SA
	// solution sA(int initTemp, int finalTemp, float coolingRate, int maxIt, int maxRuntime);

	// this GRASP algorithm is responsible to improve the addDepot method
	Solution GRASP_p(Solution s, int maxIt, int maxRunTime);
	Solution bVNS_r(Solution s, int itMax); // VNS // localSearch_GRASP
	Solution rVNS_r(Solution s); // VNS // localSearch_GRASP
	Solution localSearch_r_old(Solution s, string n);
	Solution localSearch_r(Solution s, string n);
	Solution localSearch_r(Solution s, string n, bool fimp);
	Solution hillDescent_r_2opt(Solution s, bool fimp);
	Solution hillDescent_r_shiftC(Solution s, bool fimp);
	Solution hillDescent_r_insertR(Solution s, bool fimp);
	Solution hillDescent_r_changeD(Solution s, bool fimp);
	Solution hillDescent_r_changeDV2(Solution s, bool fimp);
	Solution hillDescent_r_swapD(Solution s, bool fimp);
	Solution hillDescent_r_bssReplacement(Solution s, bool fimp);
	Solution hillDescent_r_splitR(Solution s, bool fimp); // implementation
	Solution hillDescent_r_unionR(Solution s, bool fimp); // implementation
	Solution hillDescent_r(Solution s);
	Solution VNS(Solution s, int itMax, int maxTime);
	

	Solution routeSplit_r(Solution s, int rt, int pos); // split route n in two in a given position m (m < s.route.at(n).size() - 2)
	Solution routeUnion_r(Solution s, int r1, int r2);
	Solution routeInsertion_r(Solution s, int r1, int p1, int r2, int p2);	
	Solution opt2_route_r(Solution s, int j, int beg, int end);
	Solution shiftCustomer_r(Solution s, int j, int c, int q);	
	Solution changeDepot(Solution s, int keyDPT);
	Solution changeDepotV2(Solution s, int keyA, int keyB);
	Solution swapDepot(Solution s, int kDPTA, int kDPTB);
	Solution bssReplacement_r_(Solution s, int key); // Replace a bss defined by n and swap  it for others, aready placed, bss
	Solution bssReplacement_r(Solution s, int bss1);
	Solution shakeRandom_r(Solution s, string n);

	// greed
	Solution greed();
	Solution greed_();
	Solution greed2();
	Solution greedDist();
	Solution greedDD();
	Solution greedRT();

	// aux
	vector<pair<int, int>> getCustomersPos(vector<vertex> route);
	vector<vertex> getCustomers(vector<vertex> route);

public:
	permutation perm;

	bool output = true;

	//perm_rep();
	~perm_rep();

	Solution greedl(vector<string> BSS); // greed limiting bss

	Solution GRASP(int maxIt, int maxRuntime);
	Solution sA(int initTemp, int finalTemp, float coolingRate, int maxIt, int maxRuntime);
	Solution bVNS(int itMax, int maxTime);
	Solution VNS(int itMax, int maxTime);
	Solution VNSL(vector<string> BSS, int itMax, int maxTime); // this version of the VNS will limit the BSSs according to the specified ones
	Solution VNSL(vector<string> BSS, vector<string> DPT, int itMax, int maxTime);
	Solution lowerBound();

	vector<string> chooseBSS_(int p, string regionNameFile, string regionsCitiesFile, string dmfile, map<string, int> stFreq);
	vector<string> chooseBSS(int p, set<string> cities, map<pair<string, string>, float> dists, map<string, int> stFreq);
	vector<string> chooseBSS_model(set<string> cities, map<pair<string, string>, float> dists, map<string, int> stFreq);

	static map<string, int> getBSSFreq(vector<Solution> sols);
	static map<string, int> getDepotFreq(vector<Solution> sols);
	void setOutputDir(string dir);
	bool getArcsNodes(Solution& s);

	Solution testPermutation(permutation p);
	int test_permutationToSolutionAlt();
	int test_permutationToSolution();
	int test_addStations();
	int test_shakeRandom_r(int i);
	int test_routePartition();
	int test_opt2_route();
	int test_shiftCustomer();
	int test_swapDepot();
	int test_changeDepot();
	int test_splitRoute();
	int test_unionRoute();
	int test_bssReplacement_r();
	int test_neigh();
	int test_localSearch_r();
	int test_VNS();
	int test_GRASP();
	int test_SA();
	int test_new_evaluation();	
	int test2();
	int test3();
	int test4();
	int test5();
	
	// aux
	float getTravelCost(Solution s);
	static float totalCost(vector<Solution> sols);
	static float totalCost4(vector<Solution> sols);
};