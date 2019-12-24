#ifndef ANNOPTIMIZER_H
#define ANNOPTIMIZER_H

#include <QObject>
#include <QMutex>
#include <QThread>

#include <vector>

#include "annealing.h"


typedef std::vector<Result> Results;
typedef std::vector<Result>::const_iterator ResultsItr_c;


class AnnealingOptimizer : public QObject
{
	Q_OBJECT

public:
	explicit AnnealingOptimizer(const Annealing &ann, int threads_num, QObject *parent = nullptr);
	~AnnealingOptimizer();

	void optimize();

private:
	Annealing *annealing;

	int nThreads;
	QMutex mutex;
	QThread **threads;
	Annealing ** anns;

	Result result;			// The best result from all threads
	Results results;		// Results from all threads
};

#endif // ANNOPTIMIZER_H
