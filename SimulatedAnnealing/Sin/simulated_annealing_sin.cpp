#include "../simulated_annealing.h"
#include <cstdlib>	// needed for random
#include <ctime>	// needed for random seed

#define PI 3.14159265

/******************************************************************//**

   SimulatedAnnealingSin

   Example of the use of the SimulatedAnnealing class
   Will try to find an angle at which sin(x) is (close to) 1 
   using Simulated Annealing

   
***************************************************************************/

class SimulatedAnnealingSin:public SimulatedAnnealing<double, double>{
public:
	
	double* giveRandomNeighbour(const double& lastSolution) const;

	double calcDistanceToTarget(const double& solution) const;

//	void printStatus(double solution, double temp);

	SimulatedAnnealingSin(const double& startSolution, const double& target, double starttemp, double precision, double alpha):SimulatedAnnealing(startSolution, target, starttemp, precision, alpha){};

protected:
	
	double calcSin(const double& degrees) const;

};

double SimulatedAnnealingSin::calcSin(const double& degrees) const{ // specific for problem

	return sin(1.0*degrees*(PI)/180);

}

double* SimulatedAnnealingSin::giveRandomNeighbour(const double& lastAngle) const{ // should be overwritten
	
	int randomDegrees = rand()%21 - 10; // will look for a random neighbour within 10 degrees
	double* newAngle = new double(lastAngle + randomDegrees);
	return newAngle;

}

double SimulatedAnnealingSin::calcDistanceToTarget(const double& angle) const{ // should be overwritten

	double distance = calcSin(angle)-*TARGET;
	if(distance > 0){
		return distance;
	}else{
		return (-distance);
	}

}

int main(int argc, char *argv[]){
	srand((unsigned)time(0)); // create random seed every time we solve

    SimulatedAnnealingSin sas(30.0, 1.0, 500, 0.000001, 0.4);
    sas.solve();
    return 0;
}

