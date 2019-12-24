#ifndef RESULT_H
#define RESULT_H

#include <iostream>

#include "help_types.h"


struct Result
{
	// Includes results of optimization.
	int iters;			// Amount of iterations
	double extr;		// Target function extremum
	Vals xOpt;			// Optimized values of target function's args

	Result() : iters(0), extr(0.0) {}
	Result(int iters, double extr, const Vals &x_opt);
	Result(const Result &r);

	void setNew(const Result &r);

	friend std::ostream& operator <<(std::ostream &os, const Result &res);
};

#endif // RESULT_H
