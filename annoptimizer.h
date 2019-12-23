#ifndef ANNOPTIMIZER_H
#define ANNOPTIMIZER_H

#include <QObject>
#include <QThread>

#include "annealing.h"


class AnnealingOptimizer : public QObject
{
	Q_OBJECT

public:
	AnnealingOptimizer(const Annealing &ann, int threads_num = 1);

private:
	Annealing *annealing;
};

#endif // ANNOPTIMIZER_H
