#ifndef ANNEALING_H
#define ANNEALING_H

#include <QObject>
#include <QThread>

#include <iostream>
#include <vector>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "options.h"
#include "bounds.h"
#include "result.h"


class Annealing : public QObject
{
	Q_OBJECT

public:
	Annealing(double (*f)(const Vals&), int dim, const Bounds &bnds, const Options &opt);
	Annealing(const Annealing &a);
	~Annealing();

	void printResult() const;

private:						// Functions
	void formStartState();
	void toNewState(const Vals &new_s);
	void reduceTemp();
	double genNewState(int i);
	bool isChangeState(double dE, double t);
	void formResult();

private:						// Variables
	double (*f)(const Vals&);		// Target function
	// For random generator
	const gsl_rng_type *gsl_T;
	gsl_rng *gsl_r;
	// Vars
	int dim;										// Target function dimension
	int iteration;							// Iteration number
	double T;										// Current temperature
	Vals s;											// Current state
	double E;										// Current energy

	Result *result;							// Saving result of the optimization

protected:
	Bounds *bounds;
	Options *options;

public slots:
	void anneal();

signals:
	void calculated();
};

#endif // ANNEALING_H
