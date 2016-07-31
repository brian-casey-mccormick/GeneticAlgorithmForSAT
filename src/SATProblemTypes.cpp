#include "SATProblemTypes.hpp"
#include "FixedLengthBinaryString.hpp"
#include "Support.hpp"
#include <stdio.h>

void ThreeSATProbInCNF::ReadProblem()
{
	char tempVal1;
	long tempVal2;

	FILE *fp=NULL;
	fp = fopen("cnf3-F-20-86.cnf", "r");
	assert(fp);

	fscanf(fp, "numVariables = %ld\n", &numVariables);
	fscanf(fp, "numClauses = %ld\n", &numClauses);
	
	numVariablePositions = 3 * numClauses;

	for (long i=0; i<numVariablePositions; i++)
	{
		//read clause
		fscanf(fp, "%c", &tempVal1);

		if (tempVal1 == '+')
		{
			varComplementedAtLoc.push_back(false);
		}
		else
		{
			varComplementedAtLoc.push_back(true);
		}

		fscanf(fp, "%ld", &tempVal2);
		varIndexAtLoc.push_back(tempVal2-1);

		//read final semicolon
		if ((i+1)%3 == 0)
		{
			fscanf(fp, "%c\n", &tempVal1);
		}
	}

	//close file
	fclose(fp);

	return;
}

void ThreeSATProbInCNF::Print()
{
	long i;

	printf("Three SAT Problem Definition\n");
	printf("\n");

	printf("numVariables = %ld\n", numVariables);
	printf("numClauses = %ld\n", numClauses);
	printf("numVariablePositions = %ld\n", numVariablePositions);
	printf("\n");

	for (i=0; i<numVariablePositions; i++)
	{
		printf("%ld ", varIndexAtLoc[i]);
	}

	printf("\n");

	for (i=0; i<numVariablePositions; i++)
	{
		printf("%ld ", varComplementedAtLoc[i]);
	}

	printf("\n");
	printf("\n");

	return;
}

long ThreeSATProbInCNF::DetermineBitThatMaximizesFitnessWhenFlipped(FixedLengthBinaryString &flbs)
{
	long i, fitness, bestFitness=-1;
	vector<long> temp;
	vector<long> best;

	//determine the best fitness
	for (i=0; i<numVariables; i++)
	{
		//flip bit, compute fitness, flip bit back
		flbs.FlipBit(i);
		fitness = ComputeFitness(flbs);
		flbs.FlipBit(i);

		//store list of fitness values
		temp.push_back(fitness);

		//keep track of bit flip that maximizes objective function
		if (fitness > bestFitness)
		{
			bestFitness=fitness;
		}
	}

	//get list of indices that have the best value
	for (i=0; i<numVariables; i++)
	{
		if (temp[i] == bestFitness)
		{
			best.push_back(i);
		}
	}

	//randomly pick one of the bits that when flipped maximized the fitness
	double r = Support::getRandom();
	long valInRange = (long)(r * best.size());
	long bitToFlip = best[valInRange];

	return bitToFlip;
}

long ThreeSATProbInCNF::ComputeFitness(FixedLengthBinaryString &flbs)
{
	long i, fitness=0;
	int val1, val2, val3;
	bool val1Complemented, val2Complemented, val3Complemented;

	for (i=0; i<numClauses; i++)
	{
		val1 = flbs(varIndexAtLoc[3*i]);
		val1Complemented = varComplementedAtLoc[3*i];
		
		val2 = flbs(varIndexAtLoc[3*i+1]);
		val2Complemented = varComplementedAtLoc[3*i+1];
		
		val3 = flbs(varIndexAtLoc[3*i+2]);
		val3Complemented = varComplementedAtLoc[3*i+2];

		bool clauseSatisfied = ((val1Complemented == true  && val1 == 0) ||
							    (val1Complemented == false && val1 == 1) ||
								(val2Complemented == true  && val2 == 0) ||
								(val2Complemented == false && val2 == 1) ||
								(val3Complemented == true  && val3 == 0) ||
								(val3Complemented == false && val3 == 1));

		if (clauseSatisfied == true)
		{
			fitness++;
		}
	}

	return fitness;
}

void ThreeSATProbInCNF::ComputeAndSetFitnessAndClauseData(FixedLengthBinaryString &flbs)
{
	long i, fitness=0;
	int val1, val2, val3;
	bool val1Complemented, val2Complemented, val3Complemented;

	for (i=0; i<numClauses; i++)
	{
		val1 = flbs(varIndexAtLoc[3*i]);
		val1Complemented = varComplementedAtLoc[3*i];
		
		val2 = flbs(varIndexAtLoc[3*i+1]);
		val2Complemented = varComplementedAtLoc[3*i+1];
		
		val3 = flbs(varIndexAtLoc[3*i+2]);
		val3Complemented = varComplementedAtLoc[3*i+2];

		bool clauseSatisfied = ((val1Complemented == true  && val1 == 0) ||
							    (val1Complemented == false && val1 == 1) ||
								(val2Complemented == true  && val2 == 0) ||
								(val2Complemented == false && val2 == 1) ||
								(val3Complemented == true  && val3 == 0) ||
								(val3Complemented == false && val3 == 1));

		if (clauseSatisfied == true)
		{
			fitness++;
			flbs.SetClauseSatisfiedFlag(i, true);
		}
		else
		{
			flbs.SetClauseSatisfiedFlag(i, false);
		}
	}

	flbs.SetFitness(fitness);

	return;
}

long ThreeSATProbInCNF::RandomlyChooseBitInUnsatisfiedClause(FixedLengthBinaryString &flbs, long clauseIndex)
{
	long bitToFlip;
	double r = Support::getRandom();

	if (r < 0.3333)
	{
		bitToFlip = varIndexAtLoc[3*clauseIndex];
	}
	else if (r < 0.6667)
	{
		bitToFlip = varIndexAtLoc[3*clauseIndex+1];
	}
	else
	{
		bitToFlip = varIndexAtLoc[3*clauseIndex+2];
	}

	return bitToFlip;
}

long ThreeSATProbInCNF::DetermineBitInUnsatisfiedClauseThatBreaksFewestClausesWhenFlipped(FixedLengthBinaryString &flbs, long &minNumberOfClausesBroken, long &clauseIndex)
{
	long i, cnt=0, size=0, bitToFlip=-1, numClausesBroken;
	vector<long> temp;
	vector<long> best;

	//initialize value
	minNumberOfClausesBroken=numClauses;

	//store number of broken clauses for 
	for (i=0; i<numClauses; i++)
	{
		if (flbs.GetClauseSatisfiedFlag(i) == false)
		{
			numClausesBroken = ComputeNumberOfClausesBroken(varIndexAtLoc[3*i], flbs);
			temp.push_back(numClausesBroken);
			
			if (numClausesBroken < minNumberOfClausesBroken)
			{
				minNumberOfClausesBroken = numClausesBroken;
			}

			numClausesBroken = ComputeNumberOfClausesBroken(varIndexAtLoc[3*i+1], flbs);
			temp.push_back(numClausesBroken);

			if (numClausesBroken < minNumberOfClausesBroken)
			{
				minNumberOfClausesBroken = numClausesBroken;
			}

			numClausesBroken = ComputeNumberOfClausesBroken(varIndexAtLoc[3*i+2], flbs);
			temp.push_back(numClausesBroken);

			if (numClausesBroken < minNumberOfClausesBroken)
			{
				minNumberOfClausesBroken = numClausesBroken;
			}
		}
		else
		{
			temp.push_back(-1);
			temp.push_back(-1);
			temp.push_back(-1);
		}
	}

	//get list of indices that have the lowest value
	for (i=0; i<numClauses; i++)
	{
		if (temp[3*i] == minNumberOfClausesBroken)
		{
			size++;
			best.push_back(3*i);
		}
		else
		{
			best.push_back(-1);
		}

		if (temp[3*i+1] == minNumberOfClausesBroken)
		{
			size++;
			best.push_back(3*i+1);
		}
		else
		{
			best.push_back(-1);
		}

		if (temp[3*i+2] == minNumberOfClausesBroken)
		{
			size++;
			best.push_back(3*i+2);
		}
		else
		{
			best.push_back(-1);
		}
	}

	//randomly pick one of the bits that broke the fewest clauses
	double r = Support::getRandom();
	long valInRange = (long)(r * size);
	
	//set the bit to flip and the associated clause
	for (i=0; i<numClauses; i++)
	{
		if (best[3*i] > -1)
		{
			if (valInRange == cnt++)
			{
				bitToFlip = varIndexAtLoc[3*i];
				clauseIndex = i;
				break;
			}
		}

		if (best[3*i+1] > -1)
		{
			if (valInRange == cnt++)
			{
				bitToFlip = varIndexAtLoc[3*i+1];
				clauseIndex = i;
				break;
			}
		}

		if (best[3*i+2] > -1)
		{
			if (valInRange == cnt++)
			{
				bitToFlip = varIndexAtLoc[3*i+2];
				clauseIndex = i;
				break;
			}
		}
	}

	//if bit to flip not set to appropriate value, logic error occured
	assert(bitToFlip >= 0);

	return bitToFlip;
}

long ThreeSATProbInCNF::ComputeNumberOfClausesBroken(long varIndex, FixedLengthBinaryString &flbs)
{
	long numClausesBroken=0;

	// flip bit for computations only
	flbs.FlipBit(varIndex);

	for (long i=0; i<numClauses; i++)
	{
		if (flbs.GetClauseSatisfiedFlag(i) == true)
		{
			if ((varIndex == varIndexAtLoc[3*i]) || 
				(varIndex == varIndexAtLoc[3*i+1]) || 
				(varIndex == varIndexAtLoc[3*i+2]))
			{
				int val1 = flbs(varIndexAtLoc[3*i]);
				bool val1Complemented = varComplementedAtLoc[3*i];
		
				int val2 = flbs(varIndexAtLoc[3*i+1]);
				bool val2Complemented = varComplementedAtLoc[3*i+1];
		
				int val3 = flbs(varIndexAtLoc[3*i+2]);
				bool val3Complemented = varComplementedAtLoc[3*i+2];

				bool clauseStillSatisfied = (val1Complemented == true  && val1 == 0) ||
											(val1Complemented == false && val1 == 1) ||
											(val2Complemented == true  && val2 == 0) ||
											(val2Complemented == false && val2 == 1) ||
											(val3Complemented == true  && val3 == 0) ||
											(val3Complemented == false && val3 == 1);

				if (clauseStillSatisfied == false)
				{
					numClausesBroken++;
				}
			}
		}
	}

	//flip bit back before returning the number of broken clauses
	flbs.FlipBit(varIndex);

	return numClausesBroken;
}
