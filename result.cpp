#include "result.h"


using namespace std;


Result::Result(int iters, double extr, const Vals &x_opt)
{
	this->iters = iters;
	this->extr = extr;
	for (ValsItr_c i = x_opt.begin(); i != x_opt.end(); i++)
		xOpt.push_back(*i);
}


Result::Result(const Result &r)
{
	iters = r.iters;
	extr = r.extr;
	for (ValsItr_c itr = r.xOpt.begin(); itr != r.xOpt.end(); itr++)
		xOpt.push_back(*itr);
}


void Result::setNew(const Result &r)
{
	iters = r.iters;
	extr = r.extr;

	xOpt.clear();
	for (ValsItr_c itr = r.xOpt.begin(); itr != r.xOpt.end(); itr++)
		xOpt.push_back(*itr);
}


ostream& operator <<(ostream &os, const Result &res)
{
	cout << " - extr = " << res.extr << endl;
	os << " - minimum of function: " << res.extr << '\n'
		 << " - optimal values:\n";
	int k = 0;
	for (Vals::const_iterator i = res.xOpt.begin(); i != res.xOpt.end(); i++)
		os << "\tx" << ++k << ":\t" << *i << '\n';
	os << " - iterations: " << res.iters << '\n';
	return os;
}
