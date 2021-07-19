#pragma once
#include "perm_rep.h"
#include "instance.h"

class Decoder {
public:
	string dir;
	vector<string> cities;
	float initialAlpha = 0.3;

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
			if (chromosome.at(i) > initialAlpha) {
				BSS.push_back(cities.at(i));
			}
		}

		////////////

		fstream all;
		all.open(dir + "all.txt", ios::in);
		if (all.is_open() == false) {
			throw runtime_error("Could not open file all.txt");
		}

		vector<Solution> sols;
		set<string> infs;

		bool ok = true;

		string line;
		int count = 0;
		int qt = 0;
		while (getline(all, line)) {
			
			perm_rep alg;
			alg.loadInstance(dir, line, 5);
			try {
				Solution s = alg.greedl(BSS);
				sols.push_back(s);
				for (auto i : s.inf) {
					infs.insert(i);
				}
			}
			catch (PermutationInf& e) {
				ok = false;
				count++;
			}
			catch (exception& e) {
				ok = false;
				count++;
			}
			qt++;
		}

		all.close();

		if (ok == true && infs.size() == 0) {
			infs.clear();
			float of = perm_rep::totalCost(sols);
			//cout << "of: " << of << endl;
			//cout << "alpha: " << 0.5 << endl;
			return of;
		}
		else {		
			if (count == qt) {
				return INT_MAX;
			}
			else {
				float of = perm_rep::totalCost(sols);
				of /= (qt - count);
				of *= qt;

				of = of * (count + 1) * (infs.size() + 1);
				//cout << of << endl;
				if (of == 0) {
					cout << "fuck\n";
				}

				return of;
			}			
		}
		all.close();
	}

	double decode_(const std::vector<double>& chromosome) const {
		// Here we store the fitness of ‘chromosome’
		double myFitness = 0.0;

		////////////
		vector<string> BSS;
		for (int i = 0; i < chromosome.size(); i++) {
			if (chromosome.at(i) > initialAlpha) {
				BSS.push_back(cities.at(i));
			}
		}

		////////////

		fstream all;
		all.open(dir + "all.txt", ios::in);
		if (all.is_open() == false) {
			throw runtime_error("Could not open file all.txt");
		}

		vector<Solution> sols;
		set<string> infs;

		bool ok = true;

		string line;
		int count = 0;
		while (getline(all, line)) {
			perm_rep alg;
			alg.loadInstance(dir, line, 5);
			try {
				Solution s = alg.greedl(BSS);
				sols.push_back(s);
				for (auto i : s.inf) {
					infs.insert(i);
				}
			}
			catch (PermutationInf& e) {
				ok = false;
			}
			catch (exception& e) {
				ok = false;
			}
		}

		all.close();

		if (ok == true && infs.size() == 0) {
			infs.clear();
			float of = perm_rep::totalCost(sols);
			//cout << "of: " << of << endl;
			//cout << "alpha: " << 0.5 << endl;
			return of;
		}
		else {

			// Here we store the fitness of ‘chromosome’
			double myFitness = 0.0;
			double alpha = initialAlpha;
			////////////

			while (alpha >= 0.0) {
				alpha -= 0.05;

				vector<string> BSS;
				for (int i = 0; i < chromosome.size(); i++) {
					if (chromosome.at(i) > alpha) {
						BSS.push_back(cities.at(i));
					}
				}

				////////////

				fstream all;
				all.open(dir + "all.txt", ios::in);
				if (all.is_open() == false) {
					throw runtime_error("Could not open file all.txt");
				}

				vector<Solution> sols;
				infs.clear(); //set<string> infs;

				bool ok = true;

				string line;
				int count = 0;
				while (getline(all, line)) {
					perm_rep alg;
					alg.loadInstance(dir, line, 5);
					try {
						Solution s = alg.greedl(BSS);
						sols.push_back(s);
						for (auto i : s.inf) {
							infs.insert(i);
						}

					}
					catch (PermutationInf& e) {
						ok = false;
					}
					catch (exception& e) {
						ok = false;
					}

				}

				all.close();

				if (ok == true && infs.size() == 0) {
					infs.clear();
					float of = perm_rep::totalCost(sols);
					//cout << "of: " << of << endl;
					//cout << "alpha: " << alpha << endl;
					return of;
				}
				else {
					infs.clear();
					continue;
				}

			}

			return INT_MAX;


			/*
			vector<pair<string, double>> BSSKey;

			for (int i = 0; i < chromosome.size(); i++) {
				BSSKey.push_back({cities.at(i), chromosome.at(i) });
			}
			sort(BSSKey.begin(), BSSKey.end(), [](pair<string, double> a, pair<string, double> b)->bool { return a.second > b.second; });

			for (int i = 25; i < 100; i += 5) {
				int n = cities.size() * float(float(i) / 100.0);

				BSS.clear();
				for (int j = 0; j < n; j++) {
					BSS.push_back(BSSKey.at(j).first);
				}

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
					return perm_rep::totalCost_(sols, dir);
				}

				all.close();
			}
			cout << "of: " << INT_MAX << endl;
			return INT_MAX;// *count;
			*/
		}
		all.close();
	}

	vector<string> getBSS(const std::vector<double>& chromosome) const {
		// Here we store the fitness of ‘chromosome’
		double myFitness = 0.0;

		////////////
		vector<string> BSS;
		for (int i = 0; i < chromosome.size(); i++) {
			if (chromosome.at(i) > initialAlpha) {
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
		set<string> infs;

		bool ok = true;

		string line;
		int count = 0;
		while (getline(all, line)) {
			perm_rep alg;
			alg.loadInstance(dir, line, 5);
			try {
				Solution s = alg.greedl(BSS);
				sols.push_back(s);
				for (auto i : s.inf) {
					infs.insert(i);
				}
			}
			catch (PermutationInf& e) {
				ok = false;
			}
			catch (exception& e) {
				ok = false;
			}
		}

		all.close();

		if (ok == true && infs.size() == 0) {

			float of = perm_rep::totalCost(sols);
			return BSS;
		}
		else {

			// Here we store the fitness of ‘chromosome’
			double myFitness = 0.0;
			double alpha = initialAlpha;
			////////////

			while (alpha >= 0.0) {
				alpha -= 0.05;

				vector<string> BSS;
				for (int i = 0; i < chromosome.size(); i++) {
					if (chromosome.at(i) > alpha) {
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
				set<string> infs;

				bool ok = true;

				string line;
				int count = 0;
				while (getline(all, line)) {
					perm_rep alg;
					alg.loadInstance(dir, line, 5);
					try {
						Solution s = alg.greedl(BSS);
						sols.push_back(s);
						for (auto i : s.inf) {
							infs.insert(i);
						}

					}
					catch (PermutationInf& e) {
						ok = false;
					}
					catch (exception& e) {
						ok = false;
					}
				}

				all.close();

				if (ok == true && infs.size() == 0) {
					float of = perm_rep::totalCost(sols); //
					return BSS;
				}
				else {
					continue;
				}

			}
		}
		all.close();
	}
};