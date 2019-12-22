#ifndef ANNEALING_H
#define ANNEALING_H

#include <iostream>
#include <vector>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


struct Bounds;
struct Options;
struct Result;


typedef std::vector<double> Vals;


class Annealing
{
public:
	Annealing(int dim, const Bounds &bnds, const Options &opt);
	~Annealing();

	const Result* anneal(double (*f)(const Vals&));
	void printResult() const;

private:
	void formStartState();
	void toNewState(const Vals &new_s);
	void reduceTemp();
	double genNewState(int i);
	bool isChangeState(double dE, double t);
	void formResult();

private:
	bool withBounds;
	// For random generator
	const gsl_rng_type *gsl_T;
	gsl_rng *gsl_r;
	// Vars
	int dim;					// Target function dimension
	int iteration;		// Iteration number
	double T;					// Current temperature
	Vals s;						// Current state
	double E;					// Current energy
	Result *result;		// Saving result of the optimization

protected:
	Bounds *bounds;
	Options *options;
};


struct Bounds
{
	// Saves boundaries of target function arguments.
	double *bmin, *bmax;	// Minimum and maximum variables value
	int dim;							// Dimension of target function

	Bounds(const double *bmin, const double *bmax, int dim);
	Bounds(const Bounds &b);
	~Bounds();

	bool isEmpty() const;

	friend std::ostream& operator<<(std::ostream &os, const Bounds &b);
};


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


struct Result
{
	// Includes results of optimization.
	int iters;			// Amount of iterations
	double extr;		// Target function extremum
	Vals vOpt;			// Optimized values

	Result(int iters, double extr, const Vals &v_opt);

	friend std::ostream& operator <<(std::ostream &os, const Result &res);
};

#endif // ANNEALING_H
