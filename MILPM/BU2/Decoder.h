#ifndef DECODER_H
#define DECODER_H

#include <list>
#include <vector>
#include <algorithm>
#include <utility> 

#include "perm_rep.h"

using namespace std;

class Decoder {
public:
	Decoder(perm_rep& a);
	~Decoder();

	double decode(const vector<double>& chromosome) const;

private:
	perm_rep &alg;
	instance inst;
	

};

#endif // !DECODER_H