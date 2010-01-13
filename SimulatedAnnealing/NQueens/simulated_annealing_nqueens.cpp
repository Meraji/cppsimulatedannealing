#include "../simulated_annealing.h"
#include "n_queens_board.h"
#include <cstdlib>	// needed for random
#include <ctime>	// needed for random seed

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

class SimulatedAnnealingNQueens:public SimulatedAnnealing<NQueensBoard, int>{
public:
	
	NQueensBoard* giveRandomNeighbour (const NQueensBoard& lastSolution) const;

	double calcDistanceToTarget (const NQueensBoard& solution) const;

	void printStatus (const NQueensBoard& solution, double temp);

	SimulatedAnnealingNQueens(const NQueensBoard& startSolution, const int& target, double starttemp, double precision, double alpha):SimulatedAnnealing(startSolution, target, starttemp, precision, alpha), errors(-1){};

private:
	int errors;

};


NQueensBoard* SimulatedAnnealingNQueens::giveRandomNeighbour(const NQueensBoard &lastSolution) const{
	return lastSolution.returnRandomNeighbour();
}

double SimulatedAnnealingNQueens::calcDistanceToTarget(const NQueensBoard &solution) const{
	return solution.getErrors()-(*TARGET);
}

void SimulatedAnnealingNQueens::printStatus(const NQueensBoard& solution, double temp){
	if(errors < 0 || errors > solution.getErrors()){
		errors = solution.getErrors();
		std::cout << "Temp: " << temp << std::endl;
		std::cout << "Errors: " << solution.getErrors() << std::endl << std::endl;
		//_CrtDumpMemoryLeaks();
	}
	//std::cout << "Temp: " << temp << std::endl;
	//std::cout << "Errors: " << solution.getErrors() << std::endl;
	//solution.print();
}

int main(int argc, char *argv[]){
	srand((unsigned)time(0));

	NQueensBoard startSolution(100);
	SimulatedAnnealingNQueens sanq(startSolution, 0, 5*10E5, 1, 0.6);
	sanq.solve();

	//std::clock_t start;

	//double temp = 10;



	//for(int i=30;i<1000;i+=10){
	//	start = std::clock();
	//	NQueensBoard startSolution(i);
	//	SimulatedAnnealingNQueens sanq(startSolution, 0, 5*temp, 1, 0.6);
	//	sanq.solve();
	//	temp*=10;
	//	std::cout << "Time needed for " << i << " : " << (std::clock() - start)/(double)CLOCKS_PER_SEC << std::endl;
	//}



    return 0;
}


