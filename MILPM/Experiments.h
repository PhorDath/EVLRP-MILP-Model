#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <iostream>
#include <gurobi_c++.h>
#include <boost/filesystem.hpp>
#include "Model.h"
#include "perm_rep.h"
#include "routes_rep.h"

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

string getDate();
void exp1(string dir1, string dir2);
void exp2(string dir1, string dir2);
void exp3(string dir1, string dir2, int type);
void exp4(string dir1, string dir2, int type);
void exp_greed();
void exp_SA();
int menuModel();
string menuInstance();
void instanceTypeMenu();
void expModels();

#endif EXPERIMENTS_H