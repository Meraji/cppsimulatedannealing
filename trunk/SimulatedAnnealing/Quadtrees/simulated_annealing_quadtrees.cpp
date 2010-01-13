#include "../simulated_annealing.h"
#include "quadtree_solution.h"
#include <cstdlib>	// needed for random
#include <ctime>	// needed for random seed

class SimulatedAnnealingQuadtrees:public SimulatedAnnealing<QuadtreeSolution, double>{
public:
	
	QuadtreeSolution* giveRandomNeighbour (const QuadtreeSolution& lastSolution) const;

	double calcDistanceToTarget (const QuadtreeSolution& solution) const;

	void printStatus (const QuadtreeSolution& solution, double temp);

	bool shouldStopHook(const QuadtreeSolution& solution);

	SimulatedAnnealingQuadtrees(const QuadtreeSolution& startSolution, const double& target, double starttemp, double precision, double alpha);

private:
	int counter;
};


QuadtreeSolution* SimulatedAnnealingQuadtrees::giveRandomNeighbour(const QuadtreeSolution &lastSolution) const{

	QuadtreeSolution* copy = new QuadtreeSolution(lastSolution);

	double randX = lastSolution.getRegion()->getRandX();
	double randY = lastSolution.getRegion()->getRandY();

	std::cout << randX << "," << randY << std::endl;

	Region* newParent  =  copy->getRegion()->findParentOfClosestPoint(randX, randY);
	if(newParent != 0){
		copy->setCurrentFurthest(newParent->getPoint());
	}
	
	return copy;
}

double SimulatedAnnealingQuadtrees::calcDistanceToTarget(const QuadtreeSolution &solution) const{
	Region* region = solution.getRegion();
	double distance = region->calcTotalDistance(solution.getCurrentFurthest());
	if(distance == 0){
		return 2;
	}else{
		return 1/distance;
	}
}

void SimulatedAnnealingQuadtrees::printStatus(const QuadtreeSolution &solution, double temp){
	std::cout << "Temp: " << temp << std::endl;
	
	Region* region = solution.getRegion();
	double distance = region->calcTotalDistance(solution.getCurrentFurthest());

	std::cout << "Total Distance: " << distance << std::endl;

	std::cout << "Counter: " << counter << std::endl;
	std::cout << "Current Furthest: " << *solution.getCurrentFurthest() << std::endl << std::endl;
}

bool SimulatedAnnealingQuadtrees::shouldStopHook(const QuadtreeSolution &solution){
	counter++;
	return counter >= 500;
}

SimulatedAnnealingQuadtrees::SimulatedAnnealingQuadtrees(const QuadtreeSolution& startSolution, const double& target, 
														 double starttemp, double precision, double alpha):SimulatedAnnealing(startSolution, target, starttemp, precision, alpha), counter(0){

}

int main(int argc, char *argv[]){
	srand((unsigned)time(0));

	/*Region reg(-1, 5, 2, 8, 0);

	cout << "Before starting to add\n\n" << reg << endl;
	cout << "\n========================================================\n\n" << endl;

	reg.addPoint(-2,3);//x te klein
	reg.addPoint(9,3);//x te groot
	reg.addPoint(2,1);//y te klein
	reg.addPoint(2,9);//y te groot

	//reg.addPoint(-1,2);//randgevallen
	//reg.addPoint(5,8);//randgevallen


	reg.addPoint(2,3);
	cout << "After addition of first point\n\n" << reg << endl;
	cout << "\n========================================================\n\n" << endl;

	reg.addPoint(0,4);
	cout << "After addition of second point\n\n" << reg << endl;
	cout << "\n========================================================\n\n" << endl;

	reg.addPoint(0,2);
	cout << "After addition of third point\n\n" << reg << endl;
	cout << "\n========================================================\n\n" << endl;

	reg.addPoint(4,7);
	cout << "After addition of fourth point\n\n" << reg << endl;
	cout << "\n========================================================\n\n" << endl;

	reg.addPoint(3,3);
	cout << "After addition of fifth point\n\n" << reg << endl;
	cout << "\n========================================================\n\n" << endl;

	reg.addPoint(3,3);
	cout << "After addition of already present point\n\n" << reg << endl;
	cout << "\n========================================================\n\n" << endl;

	*/

	Region reg(-300,300,-300,300,0);


reg.addPoint(-223,-188);
reg.addPoint(24,26);
reg.addPoint(-132,143);
reg.addPoint(246,132);
reg.addPoint(209,0);
reg.addPoint(-67,259);
reg.addPoint(75,186);
reg.addPoint(-192,-144);
reg.addPoint(-12,260);
reg.addPoint(-129,198);
reg.addPoint(218,-234);
reg.addPoint(150,136);
reg.addPoint(31,62);
reg.addPoint(249,215);
reg.addPoint(-212,-112);
reg.addPoint(11,203);
reg.addPoint(-130,6);
reg.addPoint(167,-38);
reg.addPoint(-162,272);
reg.addPoint(-221,58);
reg.addPoint(163,82);
reg.addPoint(-221,105);
reg.addPoint(-162,-281);
reg.addPoint(-228,-230);
reg.addPoint(-161,-252);
reg.addPoint(-62,-194);
reg.addPoint(19,293);
reg.addPoint(74,-114);
reg.addPoint(-179,52);
reg.addPoint(-195,-5);
reg.addPoint(-224,-214);
reg.addPoint(-110,-281);
reg.addPoint(171,91);
reg.addPoint(155,211);
reg.addPoint(256,-161);
reg.addPoint(-65,291);
reg.addPoint(-293,-96);
reg.addPoint(-32,-46);
reg.addPoint(-152,17);
reg.addPoint(82,-191);
reg.addPoint(196,-196);
reg.addPoint(5,-40);





	
	
	QuadtreeSolution startSolution(&reg);

	SimulatedAnnealingQuadtrees saqt(startSolution, 0, 500, 0, 0.7);
	saqt.solve();


    return 0;
}


