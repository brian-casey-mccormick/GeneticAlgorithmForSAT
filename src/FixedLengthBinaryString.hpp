#ifndef _FixedLengthBinaryString_hpp
#define _FixedLengthBinaryString_hpp

#include "Support.hpp"
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <stdio.h>

using namespace std;

class FixedLengthBinaryString
{
private:

	long fitness;
	double cumulativeFitness;
	vector<int> binaryVal;
	vector<bool> clauseSatisfied;

public:

	FixedLengthBinaryString()
	{
		fitness = -999;
		cumulativeFitness = -999.0;
	}

	FixedLengthBinaryString(long stringSize, long numClauses)
	{
		long i;
		assert(stringSize > 0 && numClauses > 0);

		fitness = -999;
		cumulativeFitness = -999.0;

		for (i=0; i<stringSize; i++)
		{
			double r = Support::getRandom();

			if (r > 0.5)
			{
				binaryVal.push_back(1);
			}
			else
			{
				binaryVal.push_back(0);
			}
		}

		for (i=0; i<numClauses; i++)
		{
			clauseSatisfied.push_back(false);
		}
	}

	FixedLengthBinaryString(const FixedLengthBinaryString &flbsIn)
	{
		long i;
		fitness = flbsIn.fitness;
		cumulativeFitness = flbsIn.cumulativeFitness;

		for (i=0; i<flbsIn.binaryVal.size(); i++)
		{
			binaryVal.push_back(flbsIn.binaryVal[i]);
		}

		for (i=0; i<flbsIn.clauseSatisfied.size(); i++)
		{
			clauseSatisfied.push_back(flbsIn.clauseSatisfied[i]);
		}
	}

	FixedLengthBinaryString& operator = (const FixedLengthBinaryString &flbsIn)
	{
		if (this != &flbsIn)
		{
			long i;
			fitness = flbsIn.fitness;
			cumulativeFitness = flbsIn.cumulativeFitness;

			binaryVal.clear();
			clauseSatisfied.clear();

			for (i=0; i<flbsIn.binaryVal.size(); i++)
			{
				binaryVal.push_back(flbsIn.binaryVal[i]);
			}
	
			for (i=0; i<flbsIn.clauseSatisfied.size(); i++)
			{
				clauseSatisfied.push_back(flbsIn.clauseSatisfied[i]);
			}
		}

		return *this;
	}

	~FixedLengthBinaryString()
	{
		binaryVal.clear();
		clauseSatisfied.clear();
	}

	bool operator == (const FixedLengthBinaryString &flbsIn)
	{
		long binaryValSizeIn = flbsIn.binaryVal.size();

		if (binaryVal.size() != binaryValSizeIn)
		{
			return false;
		}
		
		for (long i=0; i<binaryValSizeIn; i++)
		{
			if (binaryVal[i] != flbsIn.binaryVal[i])
			{
				return false;
			}
		}

		return true;
	}

	void FlipBit(long posIn)
	{
		assert(posIn >= 0 && posIn < binaryVal.size());

		if (binaryVal[posIn] == 0)
		{
			binaryVal[posIn] = 1;
		}
		else
		{
			binaryVal[posIn] = 0;
		}
	}

	int &operator () (long posIn)
	{
		assert(posIn >= 0 && posIn < binaryVal.size());
		return binaryVal[posIn];
	}

	long GetFixedLengthBinaryStringSize()
	{
		return binaryVal.size();
	}

	long GetNumberOfClauses()
	{
		return clauseSatisfied.size();
	}

	void SetFitness(long fitnessIn)
	{
		fitness = fitnessIn;
	}

	long GetFitness()
	{
		return fitness;
	}

	void SetCumulativeFitness(double cumulativeFitnessIn)
	{
		cumulativeFitness = cumulativeFitnessIn;
	}

	double GetCumulativeFitness()
	{
		return cumulativeFitness;
	}

	bool GetClauseSatisfiedFlag(long posIn)
	{
		assert(posIn >= 0 && posIn < clauseSatisfied.size());
		return clauseSatisfied[posIn];
	}

	void SetClauseSatisfiedFlag(long posIn, bool valIn)
	{
		assert(posIn >= 0 && posIn < clauseSatisfied.size());
		clauseSatisfied[posIn] = valIn;
	}

	void PrintFixedLengthBinaryString()
	{
		for (long i=0; i<binaryVal.size(); i++)
		{
			printf("%ld ", binaryVal[i]);
		}

		printf("\n");
	}
};

#endif

