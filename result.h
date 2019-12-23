#ifndef RESULT_H
#define RESULT_H

#include <iostream>

#include "help_types.h"


struct Result
{
	// Includes results of optimization.
	int iters;			// Amount of iterations
	double extr;		// Target function extremum
	Vals vOpt;			// Optimized values

	Result(int iters, double extr, const Vals &v_opt);

	friend std::ostream& operator <<(std::ostream &os, const Result &res);
};

#endif // RESULT_H
