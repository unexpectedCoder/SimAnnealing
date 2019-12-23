#include "result.h"


using namespace std;


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
