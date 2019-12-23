#ifndef OPTIONS_H
#define OPTIONS_H

#include <iostream>


struct Options
{
	// Saves the algorithm options.
	double tStart, tEnd;		// Temperatures at the beginning and in the end
	double reduce;					// Cooling coefficient
	int maxIters;						// Maximum amount of iterations

	Options(double t_start = 20000.0, double t_end = 0.01,
					double r = 0.99, int iters = 1000000) :
		tStart(t_start), tEnd(t_end), reduce(r), maxIters(iters)
	{}
	Options(const Options &opt);

	friend std::ostream& operator <<(std::ostream &os, const Options &opt);
};

#endif // OPTIONS_H
