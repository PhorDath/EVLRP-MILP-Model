#pragma once
#include "perm_rep.h"
#include "instance.h"

class Decoder {
public:
	string dir;
	vector<string> cities;
	Decoder(string dir, vector<string> cities) {
		this->dir = dir;
		this->cities = cities;
	}
	double decode(const std::vector<double>& chromosome) const {
		// Here we store the fitness of ‘chromosome’
		double myFitness = 0.0;

		////////////
		vector<string> BSS;
		for (int i = 0; i < chromosome.size(); i++) {
			if (chromosome.at(i) > 0.3) {
				BSS.push_back(cities.at(i));
			}
		}

		////////////
		perm_rep alg;

		fstream all;
		all.open(dir + "all.txt", ios::in);
		if (all.is_open() == false) {
			throw runtime_error("Could not open file all.txt");
		}

		vector<Solution> sols;

		bool ok = true;

		string line;
		int count = 0;
		while (getline(all, line)) {
			perm_rep alg;
			alg.loadInstance(dir, line, 5);
			try {
				Solution s = alg.greedl(BSS);
				sols.push_back(s);
			}
			catch (PermutationInf& e) {
				ok = false;
				count++;
				//break;
			}
			catch (exception& e) {
				ok = false;
				count++;
				//break;
			}
		}

		if (ok == true) {
			all.close();
			float of = perm_rep::totalCost_(sols, dir);
			return of;
		}
		else {
			all.close();
			return INT_MAX;// *count;
		}
		all.close();

		return myFitness;
	}
};