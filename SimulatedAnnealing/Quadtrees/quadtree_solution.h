#ifndef __QUADTREE_SOLUTION_H
#define __QUADTREE_SOLUTION_H

#include "pr_quadtree.h"


//helping class created to use simulated annealing with my quadtree classes Region and Point
//we need it to be able to put the region and the currentfurthest in one object so we can see that as "a solution"
class QuadtreeSolution
{
	
	friend std::ostream& operator<<(std::ostream& output, const QuadtreeSolution& qts);

public:

	QuadtreeSolution(Region* regio);
	void setCurrentFurthest(Point * p);
	Region* getRegion() const;
	Point* getCurrentFurthest() const;

private:
	Point* currentFurthest;
	Region* region;
};

QuadtreeSolution::QuadtreeSolution(Region* region){
	this->region = region;
	assert(!this->region->isEmpty());
	Region* parent = region->findParentOfClosestPoint(region->getRandX(),region->getRandY());
	currentFurthest = parent->getPoint();
}

void QuadtreeSolution::setCurrentFurthest(Point *p){
	this->currentFurthest = p;
}

Region* QuadtreeSolution::getRegion() const{
	return region;
}

Point* QuadtreeSolution::getCurrentFurthest() const{
	return currentFurthest;
}

std::ostream& operator<<(std::ostream& output, const QuadtreeSolution& qts){
	
	std::cout << *qts.getCurrentFurthest() << std::endl;
	std::cout << "Total Distance: " << qts.getRegion()->calcTotalDistance(qts.getCurrentFurthest()) << std::endl;

	return output;
}

#endif
