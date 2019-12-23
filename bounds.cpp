#include "bounds.h"


using namespace std;


Bounds::Bounds(const double *bmin, const double *bmax, int dim)
{
	this->bmin = nullptr;
	this->bmax = nullptr;

	if (dim > 0)
	{
		this->dim = dim;

		this->bmin = new double[this->dim];
		this->bmax = new double[this->dim];
		for (int i = 0; i < this->dim; ++i)
		{
			if (bmax[i] < bmin[i])
				throw "Error in <Bounds>: maximum of value must be > minimum of value!";
			this->bmin[i] = bmin[i];
			this->bmax[i] = bmax[i];
		}
	}
	else throw "Error in <Bounds>: array size must be > 0!";
}


Bounds::Bounds(const Bounds &b)
{
	dim = b.dim;
	bmin = new double[dim];
	bmax = new double[dim];
	for (int i = 0; i < dim; ++i)
	{
		bmin[i] = b.bmin[i];
		bmax[i] = b.bmax[i];
	}
}


Bounds::~Bounds()
{
	if (bmin != nullptr) delete [] bmin;
	else delete bmin;
	if (bmax != nullptr) delete [] bmax;
	else delete bmax;
}


bool Bounds::isEmpty() const
{
	if (bmin == nullptr)
		return true;
	return false;
}


ostream& operator<<(ostream &os, const Bounds &b)
{
	os << "BOUNDS\n";
	os << " - dimension: " << b.dim << '\n';
	for (int i = 0; i < b.dim; ++i)
		os << " - x" << i + 1 << ": (" << b.bmin[i] << "; " << b.bmax[i] << ")\n";
	return os;
}
