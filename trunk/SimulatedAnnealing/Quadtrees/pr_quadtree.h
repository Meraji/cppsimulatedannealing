#ifndef __PR_QUADTREE_H
#define __PR_QUADTREE_H

#include <iostream>
#include <stack>

//using namespace std;

//
//
//              POINT CLASS
//
//


class Point{
	friend std::ostream& operator<<(std::ostream& output, const Point& pt);
	bool operator==(const Point &other) const;
	bool operator!=(const Point &other) const;

public:
	Point(double x, double y);
	Point(const Point& p);
	void print(std::ostream& output) const;
	double getx() const;
	double gety() const;


private:
	double x;
	double y;
};


//
//
//              REGION CLASS
//
//


class Region{
	//overschreven << operator
	friend std::ostream& operator<<(std::ostream& output, Region& reg);

public:
	Region(double xmin, double xmax, double ymin, double ymax, int level);
	Region(const Region& region);
	~Region();

	//add point
	bool addPoint(double x, double y);
	//remove point
	bool removePoint(double x, double y);
	//print the region
	void print(std::ostream& output) const;
	//finds the parent of the point closest to the given coordinates
	Region* findParentOfClosestPoint(double x, double y);
	//calculates the total distance of the point to all other points
	double calcTotalDistance(Point* p);
    //needed to get the point out of the closest parent
	Point* getPoint();
	//generates random coordinates withing the region's domain
	double getRandX() const;
	double getRandY() const;
		
	bool isEmpty() const;


private:
	
	//
	//Data members
	//
	Region** children; //array of pointers to Region objects
	Point* point; // every region can contain 1 Point
	double xmin, xmax, ymin, ymax;
	int level;

	//
	//Methods
	//
	//returns the index of one of the childregions based on the coordinates
	int selectregion(double x, double y) const;

	//adds a point to the correct childregion (which will be created if it doesn't exist yet)
	void addtocorrectchildregion(double x, double y);

	//determines if the region is a leafelement
	bool isLeaf() const;

	//will either return a trajectory to the point or an empty trajectory
	void buildTrajectory(double x, double y, std::stack<Region*>& trajectory);

	//determins whether or not this region should be merged (total amount of points in a subtree is 1)
	bool shouldMerge() const;

	//finds the parent region of the only point in a subtree
	Region* getParentOfOnlyPoint();

	//merges this region
	void merge();

	//counts the total amount of points in this region
	int countPoints() const;

	//bepaalt de minimale afstand die de punten van deze regio tot het gegeven punt zullen hebben
	double calcMinimumDistanceSquare(double x, double y);

	//calculate the coordinates of the middle of the region
	double getMidX() const;
	double getMidY() const;


};

#endif