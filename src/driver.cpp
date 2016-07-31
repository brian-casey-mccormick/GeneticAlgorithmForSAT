#include "FixedLengthBinaryString.hpp"
#include "SATProblemTypes.hpp"
#include "Population.hpp"
#include <ctime>
#include <stdio.h>

long iterationCount=0;

int main(void)
{
	//initialize to a random seed
	srand(time(0));

	//read 3-SAT problem in CNF
	ThreeSATProbInCNF threeSATProbInCNF;
	threeSATProbInCNF.ReadProblem();

	//initialize population data
	Population population;
	population.ReadAndInitializePopulationParameters(threeSATProbInCNF.numVariables, threeSATProbInCNF.numClauses);
	
	//perform greedy local search, if find solution, print it and return
	long optimalIndex;
	bool foundOptimal = population.PerformLocalSearch(optimalIndex, threeSATProbInCNF);

	if (foundOptimal)
	{
		printf("optimal found during first local search\n");
		printf("iteration count = %ld\n", iterationCount);
		population.GetFixedLengthBinaryString(optimalIndex).PrintFixedLengthBinaryString();
		return 0;
	}

	//retrieve the maximum number of generations
	long generation=0, maxGenerations = population.GetMaximumNumberOfGenerations();

	//loop up to maxGenerations, break out and print results if find an optimal solution
	while (generation < maxGenerations)
	{
		generation++;
		printf("generation %ld\n", generation);
		
		//perform basic genetic algorithm logic
		population.ComputeAndSetTotalPopulationFitness();
		population.ComputeFitnessProportionalSelectionData();
		population.PerformFitnessProportionalSelections();
		population.PerformCrossovers();
		population.PerformGeneticMutations();

		//perform greedy local search, if find solution, print it and return
		foundOptimal = population.PerformLocalSearch(optimalIndex, threeSATProbInCNF);

		if (foundOptimal)
		{
			printf("optimal found at generation %ld\n", generation);
			printf("iteration count = %ld\n", iterationCount);
			population.GetFixedLengthBinaryString(optimalIndex).PrintFixedLengthBinaryString();
			return 0;
		}
	}

	printf("optimal solution not found in %ld generations\n", maxGenerations);
	population.Print();

	return 0;
}
