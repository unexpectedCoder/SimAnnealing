#include <iostream>
#include <math.h>

#include "annoptimizer.h"


using namespace std;


double f(const Vals &x);
double f4(const Vals &x);
double rozen(const Vals &x);
double ackley(const Vals &x);
double bill(const Vals &x);
double booth(const Vals &x);
double mathias(const Vals &x);


int main()
{
	cout << "Optimization using simulated annealing.\nBMSTU, 2020\n\n";

	// Target function (space) dimension
	int dim = 0;
	while (true)
	{
		cout << "Enter target function dimension: "; cin >> dim;
		if (dim > 0) break;
		cout << "Error: incorrect dimension value!\nPlease, try again...\n";
	}
	// Boundaries of arguments
	double *xmin = new double[dim];
	double *xmax = new double[dim];
	cout << "Enter values boundaries:\n";
	for (int i = 0; i < dim; ++i)
	{
		while (true)
		{
			cout << " - min x" << i + 1 << ": "; cin >> xmin[i];
			cout << " - max x" << i + 1 << ": "; cin >> xmax[i];
			if (xmin[i] < xmax[i]) break;
			cout << "Error: min(x) must be < max(x)!\nPlease, try again...\n";
		}
	}

	// Init algorithm
	try
	{
		char choice;

		Bounds bnds(xmin, xmax, dim);
		cout << bnds << endl;

		Options opt;
		cout << "Use default algorithm options? (+/-): "; cin >> choice;
		if (choice != '+')
		{
			while (true)
			{
				cout << "\nAlgorithm Settings:\ntemperature:\n";
				cout << " - start (" << opt.tStart << " by default): "; cin >> opt.tStart;
				cout << " - end (" << opt.tEnd << " by default): "; cin >> opt.tEnd;
				if (opt.tStart > opt.tEnd) break;
				cout << "Error: temp at the beginning must be > in the end!\nPlease, try again...\n";
			}
			while (true)
			{
				cout << "temperature decrement coefficient r = (0; 1) for reducing law T(k) = r*T(k-1) ("
						 << opt.reduce << " by default): ";
				cin >> opt.reduce;
				if (opt.reduce > 0.0 && opt.reduce < 1.0) break;
				cout << "Error: 'r' must be in range (0; 1)!\nPlease, try again...\n";
			}
			while (true)
			{
				cout << "maximum amount of iterations (" << opt.maxIters << " by default): "; cin >> opt.maxIters;
				if (opt.maxIters > 0) break;
				cout << "Error: maximum amount of iterations must be > 0!\nPlease, try again...\n";
			}
		}
		cout << opt << endl;

		// Threading
		int nThreads = 1;
		cout << "Use multithreading? (+/-): "; cin >> choice;
		if (choice == '+')
			while (true)
			{
				cout << " - enter amount of threads: "; cin >> nThreads;
				if (nThreads > 1) break;
				cout << "Error: amount of threads must be > 1 for multithreading!\nPlease, try again...\n";
			}

		// Optimization
		Annealing ann(bill, dim, bnds, opt);
		AnnealingOptimizer optimizer(ann);
	}
	catch(const char* ex)
	{
		cout << ex << endl;
	}

	return 0;
}


double f(const Vals &x)
{
	return pow(x[0], 2.0);
}

double f4(const Vals &x)
{
	return x[0]*x[0] + x[1]*x[1] + x[2]*x[2] + x[3]*x[3] -
			0.1*cos(3*x[0]) - 0.2*cos(2*x[1]) - 0.5*cos(5*x[2]) - 0.6*cos(x[3]);
}

double rozen(const Vals &x)
{
	return pow(1.0 - x[0], 2.0) + 100 * pow(x[1] - x[0] * x[0], 2.0);
}

double ackley(const Vals &x)
{
	return -20 * exp(-0.2 * sqrt(0.5 * (x[0] * x[0] + x[1] * x[1]))) -
			exp(0.5 * (cos(2 * M_PI * x[0]) + cos(2 * M_PI * x[1]))) +
			exp(1) + 20.0;
}

double bill(const Vals &x)
{
	return pow(1.5 - x[0] + x[0] * x[1], 2.0) +
			pow(2.25 - x[0] + x[0] * x[1] * x[1], 2.0) +
			pow(2.625 - x[0] + x[0] * x[1] * x[1] * x[1], 2.0);
}

double booth(const Vals &x)
{
	return pow(x[0] + 2.0 * x[1] - 7.0, 2.0) +
			pow(2.0 * x[0] + x[1] - 5.0, 2.0);
}

double mathias(const Vals &x)
{
	return 0.26 * (x[0] * x[0] + x[1] * x[1]) - 0.48 * x[0] * x[1];
}
