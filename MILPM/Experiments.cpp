#include "Experiments.h"

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

void exp_model(string dir1)
{
	fstream all;
	all.open(dir1 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// prepare the output directory
	string date = getDate();
	string dirOutput = dir1;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// csv file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}

	// header
	csv << "model;status;fo;gap;time\n";

	csv.close();

	int count = 0;
	string line;
	while (getline(all, line)) {
		cout << line << endl;

		// csv file
		fstream csv;
		csv.open(dirOutput + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		Model model(dir1, line, 0, 3, dirOutput);
		model.optmize();
		Solution s = model.getSolution();

		csv << model.row;
		csv.close();

		count++;
	}

	all.close();

	return;
}

void exp_sa(string dir1)
{
	fstream all;
	all.open(dir1 + "all.txt", ios::in);
	if (all.is_open() == false) {
		throw runtime_error("Could not open file all.txt");
	}

	// prepare the output directory
	string date = getDate();
	string dirOutput = dir1;
	boost::filesystem::create_directory(dirOutput + "output");
	dirOutput += "output/";
	boost::filesystem::create_directory(dirOutput + "/" + date);
	dirOutput += date + "/";

	// csv file
	fstream csv;
	csv.open(dirOutput + "result.csv", ios::out | ios::ate);
	if (csv.is_open() == false) {
		cout << "Error opening file result.csv\n";
		cout << "On directory " << dir1 << endl;
		return;
	}

	// header
	csv << "model;status;fo;fo_init;time\n";

	csv.close();

	int count = 0;
	string line;
	while (getline(all, line)) {
		cout << line << endl;

		// csv file
		fstream csv;
		csv.open(dirOutput + "result.csv", ios::out | ios::app);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir1 << endl;
			return;
		}

		perm_rep alg;
		alg.loadInstance(dir1, line, 3);
		alg.setOutputDir(dirOutput);
		Solution s = alg.sA(1000, 100, 100, 50, 300);

		csv << alg.row;
		csv.close();

		count++;
	}

	all.close();

	return;
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
	csv << "model;status;fo;gap;time\n";

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

void exp_greed(string dir1, string dir2, int type) {
}

void exp_SA(string dir1, string dir2, int type) {

	fstream file;
	file.open(dir1 + "all.txt", ios::in);
	if (file.is_open() == false) {
		cout << "Error opening file all.txt\n";
		cout << "On directory " << dir1 << endl;
		exit(1);
	}

	string date = getDate();
	string dirOutput = dir1;
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
	csv << "model;status;inf;fo;fop;fo_init;gap;time\n";

	int count = 0;
	string line;
	while (getline(file, line)) {
		cout << line << endl;
		if (count == MAX) {
			break;
		}

		perm_rep alg;
		alg.loadInstance(dir1, line, 3);
		alg.printInstance();
		//alg.sA();
		Solution s = alg.sA(1000, 100, 100, 30, 300);

		fstream output;
		output.open(dirOutput + line + ".sol", ios::out | ios::app);
		if (output.is_open() == false) {
			cout << "Error creating output file " << line + ".sol\n";
			cout << "In the directory " << dirOutput << endl;
			exit(1);
		}


		// alg.getSol(output, s);
		//alg.getSol2(output, s);
		output << endl << "#" << endl << endl;
		alg.getSol(output, s);

		string str = "";
		for (string i : s.inf) {
			str += (i + "\n");
		}

		string str2 = "";
		int i = 0;
		for (i = 0; i < s.FOp.size() - 1; i++) {
			str2 += (to_string(s.FOp.at(i)) + ", ");
		}
		str2 += (to_string(s.FOp.at(i)));

		csv << alg.inst->fileName << ";" << to_string(s.status) << ";" << str << ";" << s.FO << ";" << str2 << ";" << s.FOINIT << ";" << to_string(double(s.FO) / double(s.FOINIT)) << ";" << to_string(s.runtime) << endl;
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

string menuInstance(string curDir) {
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
