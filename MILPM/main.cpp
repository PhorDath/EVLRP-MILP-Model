#include <iostream>
#include <gurobi_c++.h>
#include <boost/filesystem.hpp>
#include <Python.h>
#include "Model.h"

using namespace std;

const string dir1 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/MDEVLRPTW-BSPR/Instances/";
const string dir2 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/SSG14/";
const string dir3 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/";
const string file1 = "c101C5.txt";
const string file2 = "c101_21.xml";
const string file3 = "UK10_01.txt";

const string curDir = dir3;
const string fl = file3;
const int MAX = 10000;

string getDate() {
	string temp;

	// pega a data para colocar no nome do arquivo
	char str[26];
	time_t result = time(NULL);
	ctime_s(str, sizeof str, &result);

	temp = str;//temp + str;
	temp.erase(remove(temp.begin(), temp.end(), '\n'), temp.end()); // remove o \n da string
	temp = temp;// + ").txt";
	replace(temp.begin(), temp.end(), ':', '.');

	return temp;
}

void exp1(string dir1, string dir2) {
	fstream file;
	file.open(dir1 + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// output file
	fstream csv;
	csv.open(dir2 + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result_m1.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// header
	csv << "model ; status ; fo ; gap ; time \n";

	int count = 0;
	string line;
	while (getline(file, line)) {
		if (count == MAX) {
			break;
		}
		Model m1(dir1, line, 1, dir2);
		csv << m1.row + "\n";

		Model m2(dir1, line, 2, dir2);
		csv << m2.row + "\n";

		count++;
	}

	file.close();
	csv.close();
}

void exp2(string dir1, string dir2) {
	fstream file;
	file.open(dir1 + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// output file
	fstream csv;
	csv.open(dir2 + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result_m2.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// header
	csv << "model ; status ; fo ; gap ; time \n";

	int count = 0;
	string line;
	while (getline(file, line)) {
		if (count == MAX) {
			break;
		}
		Model m1(dir1, line, 3, dir2);
		csv << m1.row + "\n";

		Model m2(dir1, line, 4, dir2);
		csv << m2.row + "\n";

		count++;
	}

	file.close();
	csv.close();
}

void exp3(string dir1, string dir2, int type) {
	fstream file;
	file.open(dir1 + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// output file
	fstream csv;
	csv.open(dir2 + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result_m3.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// header
	csv << "model ; status ; fo ; gap ; time \n";

	int count = 0;
	string line;
	while (getline(file, line)) {
		if (count == MAX) {
			break;
		}
		Model m1(dir1, line, 5, type, dir2);
		csv << m1.row + "\n";

		//Model m2(dir1, line, 6, dir2);
		//csv << m2.row + "\n";

		count++;
	}

	file.close();
	csv.close();
}

void exp4(string dir1, string dir2, int type) {
	fstream file;
	file.open(dir1 + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// output file
	fstream csv;
	csv.open(dir2 + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result_m3.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}
	// header
	csv << "model ; status ; fo ; gap ; time \n";

	int count = 0;
	string line;
	while (getline(file, line)) {
		if (count == MAX) {
			break;
		}
		cout << line << endl;
		Model m1(dir1, line, 0, type, dir2);
		csv << m1.row + "\n";

		count++;
	}

	file.close();
	csv.close();
}

int menuModel() {
	//cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	cout << "\n\n\n\n";
	int op = 1;
	cout << "Which model? \n";
	cout << "  1  - Model 1 with indicator constraints \n";
	cout << "  2  - Model 1 with big M constraints \n";
	cout << "  3  - Model 2 with indicator constraints \n";
	cout << "  4  - Model 2 with big M constraints \n";
	cout << "  5  - Model 3 with indicator constraints \n";
	cout << "  6  - Model 3 with big M constraints \n";
	cout << "  7  - All experiments with Model 1 with indicator and big M constraints \n";
	cout << "  8  - All experiments with Model 2 with indicator and big M constraints \n";
	cout << "  9  - All experiments with Model 3 with indicator and big M constraints \n";
	cout << "  10 - Model 4 \n";
	cout << "  11 - All experiments with Model 4 with indicator constraints \n";
	cin >> op;
	while (op < 0 || op > 11) {
		cout << "Invalid option, enter a new one: ";
		cin >> op;
	}
	return op;
}

string menuInstance() {
	string fileName = "";

	vector<string> allFiles;

	fstream file;
	file.open(curDir + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt in directory " << curDir << endl;
		exit(1);
	}
	string line;
	while (getline(file, line)) {
		allFiles.push_back(line);
	}
	file.close();

	bool isin = false;

	while (fileName == "list" || fileName == "ls" || fileName == "" || isin == false) {
		cout << "\nWhich instance? \n";
		cin >> fileName;

		// check is file exists
		for (auto f : allFiles) {
			if (f == fileName) {
				isin = true;
			}
		}
		if (isin == false && (fileName != "list" && fileName != "ls")) {
			cout << "File " << fileName << "do not exist, try another" << endl;
		}

		// list all files aveilable
		if (fileName == "list" || fileName == "ls") {
			for (string f : allFiles) {
				cout << f << " | ";
			}
		}
	}

	return fileName;
}

int main() {
	/*
	instance j(dir1, file1, 0);
	j.test();

	instance i(dir3, "UK10_01.txt", 2);
	i.test();

	return 0;
	*/

	char again = 'y';

	while (again == 'y') {
		int op = menuModel();

		string file;
		if(op < 7 || op >9)
			if(op != 11)
				file = menuInstance();

		if (op == 0) {
			Model m(curDir, file, 0);
		}
		else if (op >= 1 && op <= 6) {
			Model m(curDir, file, op);
		}
		else if (op >= 7 && op <= 9 || op == 11) {
			string date = getDate();
			string dirOutput = curDir;

			// directory management
			/*
			if (!boost::filesystem::create_directory(dirOutput + "output")) {
				cout << "Error creating folder \"output\"" << endl;
				cout << "Output folder for this experiments will be created in the directory: " + dirOutput << endl;
			}
			else {
				dirOutput += "output/";
			}*/

			boost::filesystem::create_directory(dirOutput + "output");
			dirOutput += "output/";
			boost::filesystem::create_directory(dirOutput + "/" + date);
			dirOutput += date + "/";

			if (op == 7) {
				exp1(curDir, dirOutput);
			}
			else if (op == 8) {
				exp2(curDir, dirOutput);
			}
			else if (op == 9) {
				exp3(curDir, dirOutput, 1);
			}
			else if (op == 11) {
				exp4(curDir, dirOutput, 1);
			}
		}		
		else if (op == 10) {
			Model m(curDir, file, 0, 2);
		}


		cout << "\nWhould you like to run more experiments? (y/n) \n";
		cin >> again;
		if (again == 'y') {
			//system("cls");
		}
		
	}
	
	return 0;
}
