#include "annoptimizer.h"

#include <QByteArray>
#include <QFile>

#include <iostream>
#include <fstream>


using namespace std;


AnnealingOptimizer::AnnealingOptimizer(const Annealing &ann, int threads_num, QObject *parent)
	: QObject(parent)
{
	if (threads_num < 1)
		throw "Error in <AnnealingOptimizer>: threads number must be > 0!";

	annealing = new Annealing(ann);

	nThreads = threads_num;
	threads = new QThread*[nThreads];
	anns = new Annealing*[nThreads];

	for (int i = 0; i < nThreads; ++i)
	{
		threads[i] = new QThread;
		anns[i] = new Annealing(*annealing);
		anns[i]->moveToThread(threads[i]);

		connect(anns[i], &Annealing::completed, threads[i], &QThread::quit);
		connect(threads[i], &QThread::started, anns[i], &Annealing::anneal);
		connect(threads[i], &QThread::finished, anns[i], &Annealing::deleteLater);
	}
}


AnnealingOptimizer::~AnnealingOptimizer()
{
	for (int i = 0; i < nThreads; ++i)
	{
		threads[i]->quit();
		threads[i]->wait();
	}
	delete [] anns;
	delete [] threads;
}


void AnnealingOptimizer::optimize()
{
	for (int i = 0; i < nThreads; ++i)
		threads[i]->start();
}
