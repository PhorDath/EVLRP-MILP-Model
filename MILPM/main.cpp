/*
	This project is in development by Victor Hugo Vidigal Corrêa from the Federal University of Viçosa (UFV) as his master's project.
	This project includes many algorithm and mathematical models for the Multi-Depot Electrical Location-Routing with Time-Windows, Partial Recharging and Battery Swap Stations. 
	The dissertation with the details on the algorithms can be found in the UFV's dissrtations and thesis repository.
*/

//#define MODEL // this will decide weather the gurobi mathematical model will be compiled or not

#include <iostream>
#include "Model.h"
#include "perm_rep.h"
#include "test.h"
#include "Experiments.h"
#include "Adaptation.h"

using namespace std;

const string dir1 = "D:/OneDrive/Victor/Pos-Graduacao/Research/Instances/MDEVLRPTW-BSPR/Instances/";
const string dir2 = "D:/OneDrive/Victor/Pos-Graduacao/Research/Instances/SSG14/";
const string dir3 = "D:/OneDrive/Victor/Pos-Graduacao/Research/Instances/prplib/";
const string dir4 = "D:/OneDrive/Victor/Pos-Graduacao/Research/Instances/test/25/";
const string dir_brelrp = "D:/OneDrive/Victor/Pos-Graduacao/Research/Instances/brelrp/";
const string file1 = "c101C5.txt";
const string file2 = "c101_21.xml";
const string file3 = "UK100_11.txt";
const string file = "UK200_01.txt";

int main(int argc, char *argv[]) {
	int op = -1;
	string a = "vns";

	if (argc > 1) {
		if (a == "vns") {
			string inst = "";
			int itMax = 0, maxTime = 0;

			vector<string> arguments(argv + 1, argv + argc);

			inst = arguments.front();
			for (unsigned int i = 1; i < arguments.size(); i += 2)
			{
				if (arguments[i] == "--ITMAX")
					itMax = atoi(arguments[i + 1].c_str());
				else if (arguments[i] == "--MAXTIME")
					maxTime = atoi(arguments[i + 1].c_str());
			}

			//cout << inst << " " << itMax << " " << maxTime << endl;

			perm_rep alg;

			alg.loadInstance("", inst, 5);
			alg.output = false;
			Solution s = alg.VNS(itMax, maxTime);
			cout << s.FO;
			return 0;
		}
		else if (a == "sa") {
			string inst = "";
			int itMax = 0, maxTime = 0;

			vector<string> arguments(argv + 1, argv + argc);

			inst = arguments.front();
			for (unsigned int i = 1; i < arguments.size(); i += 2)
			{
				if (arguments[i] == "--ITMAX")
					itMax = atoi(arguments[i + 1].c_str());
				else if (arguments[i] == "--MAXTIME")
					maxTime = atoi(arguments[i + 1].c_str());
			}

			//cout << inst << " " << itMax << " " << maxTime << endl;

			perm_rep alg;

			alg.loadInstance("", inst, 5);
			alg.output = false;
			Solution s = alg.VNS(itMax, maxTime);
			cout << s.FO;
			return 0;
		}
		
	}
	else {
		perm_rep alg;

		int itMax = 5, maxTime = 20;

		string dir = "D:/OneDrive/Victor/Pos-Graduacao/Research/calibration/instances/";
		//string inst = "mg_central_20_27.brelrp";
		string inst = "mg_alto_paranaiba_20_09.brelrp";

		alg.loadInstance(dir, inst, 5);
		alg.output = false;
		Solution s = alg.VNS(itMax, maxTime);
		cout << s.FO;
		return 0;
	}
	

	if (op == 10) {
		perm_rep alg;
		alg.test_routePartition();
		alg.test_opt2_route();
		alg.test_neigh();
		alg.test_shiftCustomer();
		alg.test_localSearch_r();
		alg.test_shakeRandom_r(4);
		alg.test_VNS();		
		alg.test_GRASP();
		alg.test_SA();
		alg.test_addStations();
		alg.test_swapDepot();
		alg.test_changeDepot();
		alg.test_permutationToSolution();
		alg.test_permutationToSolutionAlt();
		alg.test_splitRoute();
		alg.test_new_evaluation();
		alg.test_bssReplacement_r();
		alg.test_unionRoute();
	}
	else if (op == 11) {
		//exp_vns2(dir_brelrp + "central/80/");
		//exp_vns2(dir_brelrp + "central/80/");

		//exp_opt(dir_brelrp, 1);
		exp_opt(dir_brelrp, 2);
		/*
		//vector<int> pcts = { 20, 50, 80 };
		//vector<string> zonas = { "mata", "alto_paranaiba", "sul_de_minas", "rio_doce", "jequitinhonha_mucuri", "triangulo", "centro_oeste", "central" };
		// "jequitinhonha_mucuri" ver instancias de 80%
		
		vector<int> pcts = { 20, 50, 80 };
		vector<string> zonas = {"mata", "sul_de_minas","central" };
		//vector<string> zonas = { "triangulo" };
		int n = 10;

		string date = getDate();
		boost::filesystem::create_directory(dir_brelrp + "output");
		boost::filesystem::create_directory(dir_brelrp + "output/" + date);

		fstream csv;
		csv.open(dir_brelrp + "output/" + date + "/result.csv", ios::out | ios::ate);
		if (csv.is_open() == false) {
			cout << "Error opening file result.csv\n";
			cout << "On directory " << dir_brelrp + "output/" + date + "/" << endl;
			//return;
		}

		csv << "inst,D,S,C,VNS,VNSl,delta,usedpct,time\n";

		for (auto z : zonas) {
			for (auto p : pcts) {
				auto start = std::chrono::high_resolution_clock::now();

				lrp_opt alg(dir_brelrp, z, p);
				alg.date = date;
				alg.n = n;
				alg.opt();

				auto end = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

				csv << z + "_" + to_string(p) << "," << alg.numd << "," << alg.nums << "," << alg.numc << "," << alg.avgVNS_b << "," << alg.avgVNSl_b << "," << alg.avgVNS_b - alg.avgVNSl_b << "," << alg.usedpct << "," << duration << endl;
			}
		}

		csv.close();
		*/
		
		//exp_vns3(dir_brelrp);

		/*
		vector<int> pcts = { 20, 50, 80 };
		vector<string> zonas = { "alto_paranaiba", "centro_oeste", "rio_doce", "triangulo", "mata", "sul_de_minas","central" };
		for (auto i : zonas) {
			for (auto j : pcts) {
				cout << dir_brelrp + i + "/" + to_string(j) + "/" << endl;
				exp_vns2(dir_brelrp + i + "/" + to_string(j) + "/");
			}
		}
		*/

		/*
		string s = "jequitinhonha_mucuri/old";
		exp_vns2(dir_brelrp + s + "/20/");
		exp_vns2(dir_brelrp + s + "/50/");
		exp_vns2(dir_brelrp + s + "/80/");
		*/

		//exp_vns2(dir_brelrp + "alto_paranaiba/20/");
		//exp_vns2(dir_brelrp + "alto_paranaiba/50/");
		//exp_vns2(dir_brelrp + "alto_paranaiba/80/");

		//exp_vns2(dir_brelrp + "centro_oeste/20/");
		//exp_vns2(dir_brelrp + "centro_oeste/50/");
		//exp_vns2(dir_brelrp + "centro_oeste/80/");

		
		//exp_vns2(dir_brelrp + "rio_doce/20/");
		//exp_vns2(dir_brelrp + "rio_doce/50/");
		//exp_vns2(dir_brelrp + "rio_doce/80/");

		//exp_vns2(dir_brelrp + "sul_de_minas/20/");
		//exp_vns2(dir_brelrp + "sul_de_minas/50/");
		//exp_vns2(dir_brelrp + "sul_de_minas/80/");

		//exp_vns2(dir_brelrp + "triangulo/20/");
		//exp_vns2(dir_brelrp + "triangulo/50/");
		//exp_vns2(dir_brelrp + "triangulo/80/");

		//exp_vns2(dir_brelrp + "jequitinhonha_mucuri/old/20/");
		//exp_vns2(dir_brelrp + "jequitinhonha_mucuri/old/50/");
		//exp_vns2(dir_brelrp + "jequitinhonha_mucuri/old/80/");

		//exp_vns2(dir_brelrp + "mata/20/");
		//exp_vns2(dir_brelrp + "mata/50/");
		//exp_vns2(dir_brelrp + "mata/80/");

		//exp_vns2(dir_brelrp + "central/20/");
		//exp_vns2(dir_brelrp + "central/50/");
		//exp_vns2(dir_brelrp + "central/80/");

		//exp_vns2(dir_brelrp + "alto_paranaiba/80/");
		//exp_vns2(dir_brelrp + "centro_oeste/80/");
		//exp_vns2(dir_brelrp + "rio_doce/80/");
		//exp_vns2(dir_brelrp + "sul_de_minas/80/");
		//exp_vns2(dir_brelrp + "triangulo/80/");
		//exp_vns2(dir_brelrp + "mata/80/");
		//exp_vns2(dir_brelrp + "central/80/");

		//exp_vnsl(dir_brelrp + "alto_paranaiba/20/");
		//exp_vnsl(dir_brelrp + "alto_paranaiba/50/");
		//exp_vnsl(dir_brelrp + "alto_paranaiba/80/");

		//exp_vnsl(dir_brelrp + "centro_oeste/20/");
		//exp_vnsl(dir_brelrp + "centro_oeste/50/");
		//exp_vnsl(dir_brelrp + "centro_oeste/80/");

		//exp_vnsl(dir_brelrp + "rio_doce/20/");
		//exp_vnsl(dir_brelrp + "rio_doce/50/");
		//exp_vnsl(dir_brelrp + "rio_doce/80/");

		//exp_vnsl(dir_brelrp + "sul_de_minas/20/");
		//exp_vnsl(dir_brelrp + "sul_de_minas/50/");
		//exp_vnsl(dir_brelrp + "sul_de_minas/80/");

		//exp_vnsl(dir_brelrp + "triangulo/20/");
		//exp_vnsl(dir_brelrp + "triangulo/50/");
		//exp_vnsl(dir_brelrp + "triangulo/80/");

		//exp_vnsl(dir_brelrp + "jequitinhonha_mucuri/old/20/");
		//exp_vnsl(dir_brelrp + "jequitinhonha_mucuri/old/50/");
		//exp_vnsl(dir_brelrp + "jequitinhonha_mucuri/old/80/");

		//exp_vnsl(dir_brelrp + "mata/20/");
		//exp_vnsl(dir_brelrp + "mata/50/");
		//exp_vnsl(dir_brelrp + "mata/80/");

		//exp_vnsl(dir_brelrp + "central/20/");
		//exp_vnsl(dir_brelrp + "central/50/");
		//exp_vnsl(dir_brelrp + "central/80/");

		//exp_vnsl(dir_brelrp + "alto_paranaiba/80/");
		//exp_vnsl(dir_brelrp + "centro_oeste/80/");
		//exp_vnsl(dir_brelrp + "rio_doce/80/");
		//exp_vnsl(dir_brelrp + "sul_de_minas/80/");
		//exp_vnsl(dir_brelrp + "triangulo/80/");
		//exp_vnsl(dir_brelrp + "mata/80/");
		//exp_vnsl(dir_brelrp + "central/80/");

		//exp_sa(dir3);
		//exp_sa(dir4);
		//exp_model(dir3);
/*
		vector<int> pcts = { 20, 50, 80 };
		vector<string> zonas = { "alto_paranaiba", "centro_oeste", "rio_doce", "triangulo", "mata", "sul_de_minas","central" };
		for (auto i : zonas) {
			for (auto j : pcts) {
				cout << dir_brelrp + i + "/" + to_string(j) + "/" << endl;
				exp_model(dir_brelrp + i + "/" + to_string(j) + "/");
			}
		}
*/
		//exp_vns(dir4);

		//exp_model(dir_brelrp + "25/");
		//for (int i = 0; i < 10; i++)
		//	exp_vns2(dir_brelrp + "central/50/"); // correto

		//for (int i = 0; i < 11; i++)
		//	exp_vnsl(dir_brelrp + "central/50/");
		//exp_lowerbound(dir_brelrp + "50/", "mg50_01.brelrp");
		//exp_lowerbound(dir3, "UK10_01.txt.ukelrp");


		//exp_vns3(dir4);
		//exp_vns3(dir4);
		//genBigMatrix(dir3);
		//exp_instances(dir3);
		//exp_brkga(dir3);
		//exp_sa(dir3);
		//exp_grasp(dir3);
		//exp_instances(dir3);
	}	
	else if (op == 12) {
		Solution init;
		perm_rep alg;
		alg.loadInstance(dir3, file, 3);
		alg.printInstance();

		try {			
			init = alg.testPermutation({}); // UK10_01.txt // optmal

			init.debug(cout);
		}
		catch (PermutationInf e) {
			cout << e.what() << endl;
		}

		return 0;
	}
	else if (op == 13) {
		#ifdef MODEL		
		Model model(dir_brelrp, "mg50_01.brelrp", 0, 5);
		model.printInst();
		model.optmize(0);
		Solution s1 = model.getSolution();
		s1.saveXML("test.xml");
		s1.debug(cout);
		cout << endl;
		return 0;
		#endif
	}
	else if (op == 14) {
		perm_rep alg;
		alg.loadInstance(dir3, file, 3);
		alg.printInstance();
		Solution s = alg.sA(1000, 100, 100, 10, 600);
		s.debug(cout);
		cout << endl;
		return 0;

	}
	else if (op == 15) {
		perm_rep alg;
		alg.loadInstance(dir_brelrp + "50/", "mg50_01.brelrp", 5);
		alg.printInstance();
		alg.setOutputDir(dir_brelrp + "output/");
		Solution s = alg.VNS(25, 300);
	}
	else if (op == 16){
		perm_rep alg;
		alg.loadInstance(dir4, file, 3);
		alg.printInstance();
		vector<string> BSSs = { "Shipley",
								"Mexborough",
								"Wombwell",
								"Staveley",
								"Horbury",
								"Kearsley",
								"Shirebrook",
								"Stoke-on-Trent",
								"Liverpool",
								"Kimberley",
								"Knaresborough",
								"Litherland",
								"North_Ferriby",
								"Alsager",
								"Ashby_de_la_Zouch",
								"Cotgrave",
								"Denton",
								"Golborne",
								"Kirkham",
								"Northwich" };
		vector<string> depot = {"Royston"};
		Solution s = alg.VNSL(BSSs, 25, 600);
		s.debug(cout);
		cout << endl;
		return 0;
	}
	else if (op == 17) {
		getAllData(dir3);
		//adapt_model(dir4);
		//adaptAll_n();
		//adaptAll("D:/Victor/Pos-Graduacao/UFV/Research/Instances/test/200/");
		return 0;
	}
	else if (op == 18) {
		perm_rep alg;
		alg.loadInstance(dir_brelrp, "mg_00.brelrp", 5);
		alg.printInstance();
		Solution s = alg.VNS(25, 300);
		s.debug(cout);

		//instance inst("D:/Victor/Pos-Graduacao/UFV/Research/Implementation/instances_generator/", "test.txt", 5);
		//inst.print(cout);
	}
}
