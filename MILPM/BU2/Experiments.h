#pragma once

#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <iostream>
#include <boost/filesystem.hpp>
#include "Model.h"
#include "perm_rep.h"
#include "routes_rep.h"
#include "test.h"

using namespace std;

const int MAX = 10000;

string getDate();

void exp_model(string dir);
void exp_sa(string dir1);
void exp_brkga(string dir1);

void test1();

///

void exp1(string dir1, string dir2);
void exp2(string dir1, string dir2);
void exp3(string dir1, string dir2, int type);
void exp4(string dir1, string dir2, int type);
void exp_greed(string dir1, string dir2, int type);
void exp_SA(string dir1, string dir2, int type);
int menuModel();
string menuInstance(string curDir);

#endif EXPERIMENTS_H