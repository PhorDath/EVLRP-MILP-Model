#include "Decoder.h"

Decoder::Decoder(perm_rep& a) : alg(a) {
}

Decoder::~Decoder() { }

double Decoder::decode(const std::vector< double >& chromosome) const { // const?
	auto s = alg.decode(chromosome);

	return s;
}
