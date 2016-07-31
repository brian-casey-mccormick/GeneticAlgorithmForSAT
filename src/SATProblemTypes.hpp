#ifndef _SATProblemTypes_hpp
#define _SATProblemTypes_hpp

#include "FixedLengthBinaryString.hpp"
#include <vector>

using namespace std;

class ThreeSATProbInCNF
{
public:

	long numVariables;
	long numClauses;
	long numVariablePositions;

	vector<long> varIndexAtLoc;
	vector<bool> varComplementedAtLoc;
	
	void ReadProblem();
	void Print();

	long DetermineBitThatMaximizesFitnessWhenFlipped(FixedLengthBinaryString &flbs);
	long ComputeFitness(FixedLengthBinaryString &flbs);
	void ComputeAndSetFitnessAndClauseData(FixedLengthBinaryString &flbs);
	long RandomlyChooseBitInUnsatisfiedClause(FixedLengthBinaryString &flbs, long clauseIndex);
	long DetermineBitInUnsatisfiedClauseThatBreaksFewestClausesWhenFlipped(FixedLengthBinaryString &flbs, long &minNumberOfClausesBroken, long &clauseIndex);
	long ComputeNumberOfClausesBroken(long varIndex, FixedLengthBinaryString &flbs);
};

#endif