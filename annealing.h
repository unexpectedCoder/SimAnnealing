#ifndef ANNEALING_H
#define ANNEALING_H

#include <QObject>
#include <QMutex>

#include <iostream>
#include <vector>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "options.h"
#include "bounds.h"
#include "result.h"


#define TXT_FILE "../../AnnealingOptimization/results.txt"
#define DAT_FILE "../../AnnealingOptimization/results.dat"


class Annealing : public QObject
{
	Q_OBJECT

public:
	explicit Annealing(double (*func)(const Vals&), int dim, const Bounds &bnds, const Options &opt, QObject *parent = nullptr);
	explicit Annealing(const Annealing &a, QObject *parent = nullptr);
	~Annealing();

	const Result& getResult() const;
	int getDimension() const;
	void printResult() const;

private:						// Functions
	void formStartState();
	void toNewState(const Vals &new_s);
	void reduceTemp();
	double genNewState(int i);
	bool isChangeState(double dE, double t);

	void formResult();
	void writeResultFile();

private:						// Variables
	QMutex mutex;

	double (*f)(const Vals&);
	// For random generator
	const gsl_rng_type *gsl_T;
	gsl_rng *gsl_r;
	// Vars
	int dim;								// Target function dimension
	int iteration;					// Iteration number
	double T;								// Current temperature
	Vals s;									// Current state
	double E;								// Current energy

	Result result;					// Saving result of the optimization

	Bounds *bounds;
	Options *options;

public slots:
	void anneal();

signals:
	void completed();
};

#endif // ANNEALING_H
