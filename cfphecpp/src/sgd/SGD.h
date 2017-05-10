/*
 * SGD.h
 *
 *  Created on: May 10, 2017
 *      Author: kimandrik
 */

#ifndef SGD_SGD_H_
#define SGD_SGD_H_

#include "SGDParams.h"

class SGD {
public:
	SGDParams params;

	// l(vecw) = -sum(log(1 + exp(-yi * vecw * vecxi)) - lambda / 2 * vecw * vecw
	// vecg(vecw) = nabla(l(vecw)) = sum(1 - sigmoid(yi * vecw * vecxi) * yi * vecxi - lambda * vecw
	// H = 1 / 4 * X * X - lambda * I
	// vecw := vecw - H * vecg(vecw)

};

#endif /* SGD_SGD_H_ */
