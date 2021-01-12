#pragma once
#include "instance.h"

class SampleDecoder {
public:
	SampleDecoder();
	SampleDecoder();
	double decode(const std::vector< double >& chromosome) const {
		// Here we store the fitness of ‘chromosome’
		double myFitness = 0.0;
		return myFitness;
	}
};