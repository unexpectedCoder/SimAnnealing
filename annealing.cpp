#include "annealing.h"

#include <math.h>
#include <time.h>


using namespace std;


Annealing::Annealing(int dim, const Bounds &bnds, const Options &opt)
{
	if (dim < 1)
		throw "Error in <Annealing>: target function dimension must be > 0!";

	this->dim = dim;
	iteration = 0;
	bounds = new Bounds(bnds);
	options = new Options(opt);

	s.reserve(this->dim);
	result = nullptr;

	gsl_rng_env_setup();
	gsl_T = gsl_rng_default;
	gsl_r = gsl_rng_alloc(gsl_T);
}

Annealing::~Annealing()
{
	delete bounds;
	delete options;
	gsl_rng_free(gsl_r);
}

const Result* Annealing::anneal(double (*f)(const Vals&))
{
	T = options->tStart;
	formStartState();
	E = f(s);
	iteration = 1;

	Vals s_new;
	do
	{
		s_new.clear();
		for (int i = 0; i < dim; ++i)
			s_new.push_back(genNewState(i));
		double E_new = f(s_new);

		if (E_new < E)
		{
			E = E_new;
			toNewState(s_new);
		}
		else
		{
			gsl_rng_set(gsl_r, clock());
			if (gsl_rng_uniform(gsl_r) < exp(-(E_new - E) / T))
			{
				E = E_new;
				toNewState(s_new);
			}
			else continue;
		}

		reduceTemp();
		++iteration;
	} while (T > options->tEnd && iteration < options->maxIters);

	formResult();
	return result;
}

void Annealing::printResult() const
{
	if (result != nullptr)
	{
		cout << "Optimization result:\n"
				 << " - minimum of function: " << result->extr << '\n'
				 << " - optimal values:\n";
		for (size_t i = 0; i < result->vOpt.size(); ++i)
			cout << "\tx" << i + 1 << ":\t" << result->vOpt[i] << '\n';
		cout << " - iterations: " << result->iters << '\n';
	}
	else
		cout << "Result is empty now..\n";
}

void Annealing::formStartState()
{
	for (int i = 0; i < dim; ++i)
		s.push_back(gsl_ran_gaussian(gsl_r, T) / T);
}

void Annealing::toNewState(const Vals &new_s)
{
	s.clear();
	for (Vals::const_iterator i = new_s.begin(); i != new_s.end(); i++)
		s.push_back(*i);
}

void Annealing::reduceTemp()
{
	T *= options->reduce;
}

double Annealing::genNewState(int i)
{
	gsl_rng_set(gsl_r, clock());
	double res = 0.0;

	if (!bounds->isEmpty())
	{
		do
		{
			res = gsl_ran_gaussian(gsl_r, T) + s[i];
		} while (res < bounds->bmin[i] || res > bounds->bmax[i]);
	}
	else
		res = gsl_ran_gaussian(gsl_r, T) + s[i];

	return res;
}

void Annealing::formResult()
{
	result = new Result(iteration, E, s);
}


// OPTIONS
Options::Options(const Options &opt)
{
	tStart = opt.tStart;
	tEnd = opt.tEnd;
	reduce = opt.reduce;
	maxIters = opt.maxIters;
}

ostream& operator <<(ostream &os, const Options &opt)
{
	os << "OPTIONS\n"
		 << " - start temperature: " << opt.tStart << '\n'
		 << " - end temperature: " << opt.tEnd << '\n'
		 << " - temperature reduce coefficien: " << opt.reduce << '\n'
		 << " - max amount of iterations: " << opt.maxIters << '\n';
	return os;
}


// BOUNDS
Bounds::Bounds(const double *bmin, const double *bmax, int dim)
{
	this->bmin = nullptr;
	this->bmax = nullptr;

	if (dim > 0)
	{
		this->dim = dim;

		this->bmin = new double[this->dim];
		this->bmax = new double[this->dim];
		for (int i = 0; i < this->dim; ++i)
		{
			if (bmax[i] < bmin[i])
				throw "Error in <Bounds>: maximum of value must be > minimum of value!";
			this->bmin[i] = bmin[i];
			this->bmax[i] = bmax[i];
		}
	}
	else throw "Error in <Bounds>: array size must be > 0!";
}

Bounds::Bounds(const Bounds &b)
{
	dim = b.dim;
	bmin = new double[dim];
	bmax = new double[dim];
	for (int i = 0; i < dim; ++i)
	{
		bmin[i] = b.bmin[i];
		bmax[i] = b.bmax[i];
	}
}

Bounds::~Bounds()
{
	if (bmin != nullptr) delete [] bmin;
	else delete bmin;
	if (bmax != nullptr) delete [] bmax;
	else delete bmax;
}

bool Bounds::isEmpty() const
{
	if (bmin == nullptr)
		return true;
	return false;
}

ostream& operator<<(ostream &os, const Bounds &b)
{
	os << "BOUNDS\n";
	os << " - dimension: " << b.dim << '\n';
	for (int i = 0; i < b.dim; ++i)
		os << " - x" << i + 1 << ": (" << b.bmin[i] << "; " << b.bmax[i] << ")\n";
	return os;
}


// RESULT
Result::Result(int iters, double extr, const Vals &v_opt)
{
	this->iters = iters;
	this->extr = extr;
	for (Vals::const_iterator i = v_opt.begin(); i != v_opt.end(); i++)
		vOpt.push_back(*i);
}

ostream& operator <<(ostream &os, const Result &res)
{
	os << "Optimization result:\n"
		 << " - minimum of function: " << res.extr << '\n'
		 << " - optimal values:\n";
	int k = 0;
	for (Vals::const_iterator i = res.vOpt.begin(); i != res.vOpt.end(); i++)
		os << "\tx" << ++k << ":\t" << *i << '\n';
	os << " - iterations: " << res.iters << '\n';
	return os;
}
