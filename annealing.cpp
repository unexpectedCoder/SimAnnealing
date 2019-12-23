#include "annealing.h"

#include <math.h>
#include <time.h>


using namespace std;


Annealing::Annealing(double (*f)(const Vals&), int dim, const Bounds &bnds, const Options &opt)
{
	if (dim < 1)
		throw "Error in <Annealing>: target function dimension must be > 0!";

	this->f = f;

	this->dim = dim;
	bounds = new Bounds(bnds);
	options = new Options(opt);

	s.reserve(this->dim);
	result = nullptr;

	gsl_rng_env_setup();
	gsl_T = gsl_rng_default;
	gsl_r = gsl_rng_alloc(gsl_T);

	// Starting condition is similar for all threads
	E = 0.0;
	T = options->tStart;
	formStartState();
	iteration = 0;
}


Annealing::Annealing(const Annealing &a) : QObject()
{
	f = a.f;
	dim = a.dim;
	iteration = a.iteration;
	T = a.T;
	E = a.E;
	for (ValsItr_c itr = a.s.begin(); itr != a.s.end(); itr++)
		s.push_back(*itr);

	bounds = new Bounds(*a.bounds);
	options = new Options(*a.options);

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


void Annealing::anneal()
{
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
	emit calculated();
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
