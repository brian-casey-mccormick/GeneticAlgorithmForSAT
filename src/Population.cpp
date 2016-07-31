#include "Population.hpp"
#include "FixedLengthBinaryString.hpp"
#include "SATProblemTypes.hpp"
#include "Support.hpp"
#include <stdio.h>

long bestFitness=-1;
extern long iterationCount;

void Population::Print()
{
	printf("population data\n\n");

	for (long i=0; i<GetSizeOfPopulation(); i++)
	{
		printf("fixed length binary string %ld = ", i);
		fixedLengthBinaryStringVector[i].PrintFixedLengthBinaryString();
	}

	printf("totalFitness = %ld\n", totalFitness);
	printf("maximum number of generations = %ld\n", maxGenerations);
	printf("maximum flips per generation = %ld\n", maxFlipsPerGeneration);
	printf("probability of sexual reproduction = %lf\n", probSexualReproduction);
	printf("probability of mutation = %lf\n", probMutation);
	printf("probability of greedy flip = %lf\n", probGreedyFlip);

	printf("\n");
	printf("\n");

	return;
}

void Population::ReadAndInitializePopulationParameters(long sizeOfFixedLengthBinaryString, long numClauses)
{
	FILE *fp=NULL;
	fp = fopen("PopulationParameters.txt", "r");
	assert(fp);

	long numMembers;
	fscanf(fp, "numMembers = %ld ", &numMembers);
	fscanf(fp, "maxGenerations = %ld ", &maxGenerations);
	fscanf(fp, "maxFlipsPerGeneration = %ld ", &maxFlipsPerGeneration);
	fscanf(fp, "probSexualReproduction = %lf ", &probSexualReproduction);
	fscanf(fp, "probMutation = %lf ", &probMutation);
	fscanf(fp, "probGreedyFlip = %lf ", &probGreedyFlip);

	//allocate and store default values for parameters
	for (long i=0; i<numMembers; i++)
	{
		FixedLengthBinaryString flbs(sizeOfFixedLengthBinaryString, numClauses);
		fixedLengthBinaryStringVector.push_back(flbs);
	}

	fclose(fp);

	return;
}

void Population::ComputeAndSetTotalPopulationFitness()
{
	totalFitness=0;

	for (long i=0; i<GetSizeOfPopulation(); i++)
	{
		long fitness = fixedLengthBinaryStringVector[i].GetFitness();
		totalFitness+=fitness;
	}

	return;
}

void Population::ComputeFitnessProportionalSelectionData()
{
	double cumulativeFitness=0.0, fitness;

	for (long i=0; i<GetSizeOfPopulation(); i++)
	{
		fitness = (double)(fixedLengthBinaryStringVector[i].GetFitness());
		cumulativeFitness += fitness/(double)(totalFitness);
		fixedLengthBinaryStringVector[i].SetCumulativeFitness(cumulativeFitness);
	}

	return;
}

void Population::PerformFitnessProportionalSelections()
{
	Population tempPopulation(*this);

	for (long i=0; i<GetSizeOfPopulation(); i++)
	{
		double r = Support::getRandom();

		for (long j=0; j<GetSizeOfPopulation(); j++)
		{
			if (r <= fixedLengthBinaryStringVector[j].GetCumulativeFitness())
			{
				tempPopulation.SetFixedLengthBinaryString(i, fixedLengthBinaryStringVector[j]);
				break;
			}
		}
	}

	//store temporary population back to this population
	*this = tempPopulation;

	return;
}

void Population::PerformStandardOnePointCrossover(FixedLengthBinaryString &flbs1, FixedLengthBinaryString &flbs2)
{
	long sizeOfFixedLengthBinaryString = flbs1.GetFixedLengthBinaryStringSize();
	long numCrossoverLocations = sizeOfFixedLengthBinaryString-1;

	double r = Support::getRandom();
	long index = (long)(numCrossoverLocations * r);
	double value = index + 0.5;

	//perform standard two point crossover
	for (long i=0; i<sizeOfFixedLengthBinaryString; i++)
	{
		if (i > value)
		{
			int tempValue = flbs1(i);
			flbs1(i) = flbs2(i);
			flbs2(i) = tempValue;
		}
	}

	return;
}

void Population::PerformStandardTwoPointCrossover(FixedLengthBinaryString &flbs1, FixedLengthBinaryString &flbs2)
{
	long firstIndex=-1, secondIndex=-1;
	long sizeOfFixedLengthBinaryString = flbs1.GetFixedLengthBinaryStringSize();
	long numCrossoverLocations = sizeOfFixedLengthBinaryString-1;
	double r, lower=-1.0, upper=-1.0;

	while (firstIndex == secondIndex)
	{
		//get first index
		r = Support::getRandom();
		firstIndex = (long)(numCrossoverLocations * r);

		//get second index
		r = Support::getRandom();
		secondIndex = (long)(numCrossoverLocations * r);
	}

	//store values into lower and upper
	if (firstIndex < secondIndex)
	{
		lower = firstIndex + 0.5;
		upper = secondIndex + 0.5;
	}
	else
	{
		lower = secondIndex + 0.5;
		upper = firstIndex + 0.5;
	}

	//perform standard two point crossover
	for (long i=0; i<sizeOfFixedLengthBinaryString; i++)
	{
		if (i > lower && i < upper)
		{
			int tempValue = flbs1(i);
			flbs1(i) = flbs2(i);
			flbs2(i) = tempValue;
		}
	}

	return;
}

void Population::PerformCrossovers()
{
	double r;
	long numParents=0, rInSpecifiedRange;
	vector<FixedLengthBinaryString> flbsvParentList;
	vector<FixedLengthBinaryString> flbsvFinalList;

	//first store binary strings which perform sexual reproduction to a separate list
	for (long i=0; i<GetSizeOfPopulation(); i++)
	{
		r = Support::getRandom();
		
		//create two copies of this binary string for future reproduction
		if (r < probSexualReproduction)
		{
			numParents++;
			flbsvParentList.push_back(fixedLengthBinaryStringVector[i]);
		}
		else
		{
			flbsvFinalList.push_back(fixedLengthBinaryStringVector[i]);
		}
	}

	//create iterator
	vector<FixedLengthBinaryString>::iterator p;

	//if number of parents not even then randomly move one of them to final list
	if (numParents%2 == 1)
	{
		//generate a random number between 0 and numParents-1
		r = Support::getRandom();
		long rInSpecifiedRange = (long)(numParents * r);

		//move the parent into the final list
		numParents--;
		flbsvFinalList.push_back(flbsvParentList[rInSpecifiedRange]);
		
		//delete the parent from the parent list
		p = flbsvParentList.begin();
		p+=rInSpecifiedRange;
		flbsvParentList.erase(p);
	}

	long numLoops = numParents/2;
	long stringSize = fixedLengthBinaryStringVector[0].GetFixedLengthBinaryStringSize();
	long numClauses = fixedLengthBinaryStringVector[0].GetNumberOfClauses();
	FixedLengthBinaryString flbs1(stringSize, numClauses), flbs2(stringSize, numClauses);

	//pick parents and perform crossover and update lists appropriately
	for (long j=0; j<numLoops; j++)
	{
		//generate a random number between 0 and numParents-1
		r = Support::getRandom();
		rInSpecifiedRange = (long)(numParents * r);

		//save first parent to local data
		flbs1 = flbsvParentList[rInSpecifiedRange];

		//delete first parent from the parent list
		p = flbsvParentList.begin();
		p+=rInSpecifiedRange;
		flbsvParentList.erase(p);

		//decrement the number of parents remaining in the list
		numParents-=1;

		//generate a random number between 0 and numParents-1
		r = Support::getRandom();
		rInSpecifiedRange = (long)(numParents * r);

		//save second parent to local data
		flbs2 = flbsvParentList[rInSpecifiedRange];

		//delete second parent from the parent list
		p = flbsvParentList.begin();
		p+=rInSpecifiedRange;
		flbsvParentList.erase(p);

		//decrement the number of parents remaining in the list
		numParents-=1;

		//perform crossover
		//PerformStandardOnePointCrossover(flbs1, flbs2);
		PerformStandardTwoPointCrossover(flbs1, flbs2);

		//add both children to final list
		flbsvFinalList.push_back(flbs1);
		flbsvFinalList.push_back(flbs2);
	}

	//store the final list back into the population
	for (long k=0; k<GetSizeOfPopulation(); k++)
	{
		fixedLengthBinaryStringVector[k] = flbsvFinalList[k];
	}

	return;
}

void Population::PerformGeneticMutations()
{
	long flbsSize = fixedLengthBinaryStringVector[0].GetFixedLengthBinaryStringSize();

	for (long i=0; i<GetSizeOfPopulation(); i++)
	{
		for (long j=0; j<flbsSize; j++)
		{
			double r = Support::getRandom();

			if (r < probMutation)
			{
				fixedLengthBinaryStringVector[i].FlipBit(j);
			}
		}
	}

	return;
}

bool Population::PerformLocalSearch(long &optimalIndex, ThreeSATProbInCNF &threeSATProbInCNF)
{
	long bitToFlip, minNumberOfClausesBroken, clauseIndex;

	for (long i=0; i<GetSizeOfPopulation(); i++)
	{
		threeSATProbInCNF.ComputeAndSetFitnessAndClauseData(fixedLengthBinaryStringVector[i]);

		for (long j=0; j<maxFlipsPerGeneration; j++)
		{
			iterationCount++;

			if (iterationCount%1000==0)
			{
				printf("iterationCount = %ld\n", iterationCount);
			}

			bitToFlip = threeSATProbInCNF.DetermineBitInUnsatisfiedClauseThatBreaksFewestClausesWhenFlipped(fixedLengthBinaryStringVector[i], minNumberOfClausesBroken, clauseIndex);
			double r = Support::getRandom();

			//if bit flip breaks one or more clauses and probability for random flip met
			if (minNumberOfClausesBroken > 0 && r > probGreedyFlip)
			{
				//randomly choose a bit to flip
				bitToFlip = threeSATProbInCNF.RandomlyChooseBitInUnsatisfiedClause(fixedLengthBinaryStringVector[i], clauseIndex);
			}

			//flip the bit and update data
			fixedLengthBinaryStringVector[i].FlipBit(bitToFlip);
			threeSATProbInCNF.ComputeAndSetFitnessAndClauseData(fixedLengthBinaryStringVector[i]);

			//if current member is best so far, output its fitness
			long fitness = fixedLengthBinaryStringVector[i].GetFitness();

			if (fitness > bestFitness)
			{
				bestFitness = fitness;
				printf("bestFitness = %ld\n", bestFitness);
			}

			//if member is a solution, return and output results
			if (fitness == threeSATProbInCNF.numClauses)
			{
				optimalIndex = i;
				return true;
			}
		}
	}

	return false;
}
