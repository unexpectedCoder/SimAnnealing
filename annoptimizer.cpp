#include "annoptimizer.h"


AnnealingOptimizer::AnnealingOptimizer(const Annealing &ann, int threads_num)
{
	annealing = new Annealing(ann);
	if (threads_num == 1)
	{
		annealing->anneal();
		annealing->printResult();
	}
}
