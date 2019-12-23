#include "options.h"


using namespace std;


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
