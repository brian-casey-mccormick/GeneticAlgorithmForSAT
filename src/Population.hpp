#ifndef Population_hpp
#define Population_hpp

#include "FixedLengthBinaryString.hpp"
#include "SATProblemTypes.hpp"
#include <vector>

using namespace std;

class Population
{
private:

	long totalFitness;
	long maxGenerations;
	long maxFlipsPerGeneration;
	double probSexualReproduction;
	double probMutation;
	double probGreedyFlip;
	vector<FixedLengthBinaryString> fixedLengthBinaryStringVector;

public:
	
	Population()
	{
		totalFitness = -999;
		maxGenerations = -999;
		maxFlipsPerGeneration = -999;
		probSexualReproduction = -999.0;
		probMutation = -999.0;
		probGreedyFlip = -999.0;
	}

	Population(const Population &populationIn)
	{
		totalFitness = populationIn.totalFitness;
		maxGenerations = populationIn.maxGenerations;
		maxFlipsPerGeneration = populationIn.maxFlipsPerGeneration;

		probSexualReproduction = populationIn.probSexualReproduction;
		probMutation = populationIn.probMutation;
		probGreedyFlip = populationIn.probGreedyFlip;

		for (long i=0; i<populationIn.fixedLengthBinaryStringVector.size(); i++)
		{
			fixedLengthBinaryStringVector.push_back(populationIn.fixedLengthBinaryStringVector[i]);
		}
	}

	Population& operator = (const Population &populationIn)
	{
		if (this != &populationIn)
		{
			totalFitness = populationIn.totalFitness;
			maxGenerations = populationIn.maxGenerations;
			maxFlipsPerGeneration = populationIn.maxFlipsPerGeneration;

			probSexualReproduction = populationIn.probSexualReproduction;
			probMutation = populationIn.probMutation;
			probGreedyFlip = populationIn.probGreedyFlip;

			for (long i=0; i<populationIn.fixedLengthBinaryStringVector.size(); i++)
			{
				fixedLengthBinaryStringVector[i] = populationIn.fixedLengthBinaryStringVector[i];
			}
		}

		return *this;
	}

	~Population()
	{
		fixedLengthBinaryStringVector.clear();
	}

	FixedLengthBinaryString &GetFixedLengthBinaryString(long i)
	{
		return fixedLengthBinaryStringVector[i];
	}

	void SetFixedLengthBinaryString(long i, FixedLengthBinaryString &flbsIn)
	{
		fixedLengthBinaryStringVector[i] = flbsIn;
	}

	long GetSizeOfPopulation()
	{
		return fixedLengthBinaryStringVector.size();
	}

	long GetMaximumNumberOfGenerations()
	{
		return maxGenerations;
	}

	void ReadAndInitializePopulationParameters(long sizeOfFixedLengthBinaryString, long numClauses);
	void ComputeAndSetTotalPopulationFitness();
	void ComputeFitnessProportionalSelectionData();
	void PerformFitnessProportionalSelections();
	void PerformStandardOnePointCrossover(FixedLengthBinaryString &flbs1, FixedLengthBinaryString &flbs2);
	void PerformStandardTwoPointCrossover(FixedLengthBinaryString &flbs1, FixedLengthBinaryString &flbs2);
	void PerformCrossovers();
	void PerformGeneticMutations();
	bool PerformLocalSearch(long &optimalIndex, ThreeSATProbInCNF &threeSATProbInCNF);
	void Print();
};

#endif
