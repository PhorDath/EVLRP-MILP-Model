#include "Adaptation.h"

void getAllData(string dir)
{
	map<pair<string, string>, int> dists;
	vector<pair<string, vector<node>>> nodes;

	//vector<int> instSizes = { 10, 15, 20, 25, 50, 75, 100, 150, 200 };
	vector<int> instSizes = { 200 };
	vector<string> instNumbers = { "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20" };
	
	for (int i : instSizes) {	
		//file += to_string(i) + "/";
		for (string j : instNumbers) {
			//string file = dir;
			string file = "UK" + to_string(i) + "_" + j + ".txt";			
			instance i(dir, file, 4);

			for (auto m : i.nodes) {
				for (auto n : i.nodes) {
					dists.insert(pair<pair<string, string>, int>(pair<string, string>(m.id, n.id), i.distanceMatrix.at(m.key).at(n.key)));
				}

				bool find = false;
				for (auto& s : nodes) {
					if (s.first == m.id) {
						s.second.push_back(m);
						find = true;
						break;
					}
				}
				if (find == false) {
					nodes.push_back(pair<string, vector<node>>(m.id, { m }));
				}
				
				
			}

			cout << file << endl;
		}
	}

	fstream file;
	file.open("data.txt", ios::app | ios::in);
	if (file.is_open() == false) {
		cout << "Error opening data.txt\n";
	}
	else {
		file << nodes.size() << endl;
		for (auto i : nodes) {
			int counter = 0;
			for (auto j : i.second) {
				if (j.type != "d") {
					counter++;
				}
			}

			file << i.first << " " << counter << endl;
			for (auto j : i.second) {
				if (j.type != "d")
					file << j.type << " " << j.demand << " " << j.readyTime << " " << j.serviceTime << " " << j.dueDate << endl;
			}
		}
		file << dists.size() << endl;
		for (auto i : dists) {
			file << i.first.first << " " << i.first.second << " " << i.second << endl;
		}

		file.close();
	}
	file.close();

	
}
