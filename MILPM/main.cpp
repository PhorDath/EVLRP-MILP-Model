// teste 6

#include <iostream>
#include "Model.h"
#include "perm_rep.h"
#include "Experiments.h"

using namespace std;

const string dir1 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/MDEVLRPTW-BSPR/Instances/";
const string dir2 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/SSG14/";
const string dir3 = "D:/Victor/Pos-Graduacao/UFV/Research/Instances/prplib/";
const string file1 = "c101C5.txt";
const string file2 = "c101_21.xml";
const string file3 = "UK100_01.txt";

int main() {

	exp_sa(dir3);

	return 0;
	//Model model(dir3, "UK10_01.txt", 0, 3);

	//return 0;

	perm_rep alg;
	alg.loadInstance(dir3, "UK10_02.txt", 3);

	solution s = alg.sA(1000, 100, 100, 50, 300);
	cout << "FO: " << s.FO << endl;
	cout << "FOp: ";
	for (int p : s.FOp) {
		cout << p << " ";
	}
	cout << endl;
	for (auto r : s.routes) {
		for (auto v : r) {
			cout << v.key << " ";
		}
		cout << endl;
	}




	return 0;
}
