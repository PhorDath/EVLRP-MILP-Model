// teste 2

#include <iostream>
#include <gurobi_c++.h>
#include <boost/filesystem.hpp>
#include "Model.h"
#include "Algorithms.h"

using namespace std;

const string dir1 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/MDEVLRPTW-BSPR/Instances/";
const string dir2 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/SSG14/";
const string dir3 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/";
const string file1 = "c101C5.txt";
const string file2 = "c101_21.xml";
const string file3 = "UK100_01.txt";

string curDir = dir3;
int t;
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

void exp_greed(){
	fstream file;
	file.open(dir3 + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		cout << "On directory " << dir1 << endl;
		exit(1);
	}


	string date = getDate();
	string dirOutput = dir3;

	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// output file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}

	// header
	csv << "model ; status ; fo ; gap ; time \n";


	int count = 0;
	string line;
	while (getline(file, line)) {
		cout << line << endl;
		if (count == MAX) {
			break;
		}

		Algorithms alg;
		alg.loadInstance(dir3, "UK50_03.txt", 3);
		alg.printInstance();
		alg.greed();

		alg.getSol(cout);
		exit(1);

		fstream output;
		output.open(dirOutput + line + ".sol", ios::out | ios::app);
		if (output.is_open() == false) {
			cout << "Error creating output file " << line + ".sol\n";
			cout << "In the directory " << dirOutput << endl;
			exit(1);
		}

		alg.getSol(output);
		csv << alg.inst->fileName << " ; " << alg.inst->solution.FO << endl;
		output.close();

		count++;
	}

	file.close();
	//csv.close();

	return;
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
	while (op < 0 || op > 12) {
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

void instanceTypeMenu() {
	cout << "Which instances would you like to set? \n";
	cout << "  1 - Paz\n";
	cout << "  2 - Max\n";
	cout << "  3 - UK\n";
	cout << "  4 - UK adapted \n";
	int op;
	cin >> op;
	while (op < 1 && op > 3) {
		cout << "Invalid option, try again: ";
		cin >> op;
	}
	if (op == 1) {
		curDir = dir1;
		t = 0;
	}
	else if (op == 2) {
		curDir = dir2;
		t = 1;
	}
	else if (op == 3) {
		curDir = dir3;
		t = 2;
	}
	else if (op == 4) {
		curDir = dir3;
		t = 3;
	}
	cout << endl;
}

void expModels() {
	char again = 'y';

	while (again == 'y') {
		int op = menuModel();
		instanceTypeMenu();

		string file;
		if (op < 7 || op >9)
			if (op != 11)
				file = menuInstance();

		if (op == 0) {
			Model m(curDir, file, 0);
		}
		else if (op >= 1 && op <= 6) {
			Model m(curDir, file, op, t);
			//Model m(curDir, file, op);
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
				exp4(curDir, dirOutput, t);
			}
		}
		else if (op == 10) {
			Model m(curDir, file, 0, t);
		}
		else if (op == 12) {
			//MCLPModel m(curDir, file);
		}


		cout << "\nWhould you like to run more experiments? (y/n) \n";
		cin >> again;
		if (again == 'y') {
			//system("cls");
		}

	}
}

int main() {
	cout << "1 - Models \n";
	cout << "2 - algorithms \n";
	int op = 2;
	//cin >> op;
	if (op == 1) {
		expModels();
	}
	else if (op == 2) {
		Algorithms alg;
		alg.loadInstance(dir3, "UK10_01.txt", 3);
		alg.printInstance();
		alg.test2();
		//exp_greed();
	}



	return 0;
}
