#include "pr_quadtree.h"
#include "math.h"
#include <assert.h>
#include <queue>
#include <cstdlib>	// needed for random
#include <ctime>

//using namespace std;

//
// Point
//

void Point::print(std::ostream& output) const{
	output << "Point: x=" << x << " y=" << y << std::endl;
}

Point::Point(double x, double y){
	this->x=x;
	this->y=y;
}

Point::Point(const Point& p){
	this->x=p.x;
	this->y=p.y;
}

double Point::getx() const{
	return x;
}

double Point::gety() const{
	return y;
}

std::ostream& operator<<(std::ostream& output, const Point& pt){
	pt.print(output);
	return output;
}

bool Point::operator ==(const Point &other) const{
	return (x==other.x && y==other.y);
}

bool Point::operator !=(const Point &other) const{
	return !(*this==other);
}

//
// Region
//

Region::Region(double xmin, double xmax, double ymin, double ymax, int level){
	assert((xmax-xmin)==(ymax-ymin));

	this->xmin = xmin;
	this->xmax = xmax;
	this->ymin = ymin;
	this->ymax = ymax;

	children = 0;
	point = 0;
	this->level = level;
}

Region::Region(const Region &region){
	this->point = new Point(*region.point);
	if(region.children != 0){
		this->children = new Region*[4];
		for(int i=0; i<4; i++){
			children[i] = new Region(*region.children[i]);
		}
	}
	this->level = region.level;
	this->xmin = region.xmin;
	this->xmax = region.xmax;
	this->ymin = region.ymin;
	this->ymax = region.ymax;
}

bool Region::addPoint(double x, double y){
	if(x >= xmin &&  x <= xmax && y >= ymin && y <= ymax){

		if(isLeaf() && point == 0){
			//empty leaf, should only happen once
			point = new Point(x,y);

		}else if(point==0){
			// we need to  move down the tree
			addtocorrectchildregion(x, y);

		}else if((x!=point->getx() || y!=point->gety())){ // existing point has to be moved into new layer together with new point
			children = new Region*[4]; //make new table of children
			for(int i=0; i<4; i++){
				children[i] = 0;
			}

			double old_x = point->getx();
			double old_y = point->gety();

			delete point;

			point = 0;

			addtocorrectchildregion(old_x, old_y);
			addtocorrectchildregion(x, y);	
		}else{
			std::cout << "Couldn't add " << x << "," << y << std::endl;
			return false;
		}
		return true;
	}else{
		std::cout << "Invalid domain for " << x << "," << y << std::endl;
		return false;
	}
}

Region::~Region(){
	if(children!=0){
		for(int i=0; i<4; i++){
			if(children[i]!=0)
				delete children[i];
		}
		delete [] children;
		children = 0;
	}
	delete point;
	point = 0;
}

bool Region::removePoint(double x, double y){

	std::stack<Region*> trajectory;
	buildTrajectory(x,y, trajectory);

	if(trajectory.empty()){
		return false;

	}else{
		Region* currentRegion = trajectory.top();
		trajectory.pop();
		assert(currentRegion->isLeaf());

		if(currentRegion == this){
			delete currentRegion->point;
			currentRegion->point = 0;
		}else{
			delete currentRegion; // should remove region and point!
			Region* regionAbove = trajectory.top(); //no pop yet, we'll still have to check if it has to be merged so it stays in the stack for now

			for(int i=0; i<4; i++){ //set correct childpointer to null
				if(regionAbove->children[i]==currentRegion){
					regionAbove->children[i]=0;
				}
			}
			while(!trajectory.empty()){
				if(regionAbove->shouldMerge()){
					regionAbove->merge();
				}
				regionAbove = trajectory.top();
				trajectory.pop();
			}

		}
		return true;
	}
}

bool Region::shouldMerge() const{
	return (!isLeaf() && countPoints()==1);
}

Region* Region::getParentOfOnlyPoint(){ //CAUTION: won't check if there is more than one point anymore!
	if(isLeaf()){
		return this;
	}else{
		for(int i=0; i<4; i++){
			if(children[i]!=0){
				Region* r = children[i]->getParentOfOnlyPoint();
				if(r != 0) return r;
			}
		}
	}
	return 0; //the request the last point of an empty region... shouldn't happen
}

void Region::merge(){
	Region* temp = getParentOfOnlyPoint();
	assert(temp!=0); // amount of points should be exactly one once we call merge
	point = temp->point;
	temp->point=0;
	for(int i = 0; i<4; i++){
		delete children[i];
		children[i]=0;
	}
	delete [] children;
	children = 0;
}

int Region::countPoints() const{
	if(isLeaf()){
		return point == 0 ? 0 : 1;
	}else{
		int count = 0;
		for(int i=0; i<4; i++){
			if(children[i]!=0){
				count+=children[i]->countPoints();
			}
		}
		return count;
	}	
}

bool Region::isLeaf() const{
	return children == 0;
}

void Region::buildTrajectory(double x, double y, std::stack<Region*> &trajectory){ // will either return a trajectory to the point or an empty trajectory
	if(!this->isLeaf()){ //then there is no point present in this region
		// descends for the search
		trajectory.push(this);
		children[selectregion(x,y)]->buildTrajectory(x, y, trajectory);

	}else if(this->point != 0 && (this->point->getx()==x && this->point->gety()==y)){
		//FOUND
		trajectory.push(this);

	}else{
		//NOT FOUND -> empty trajectory
		while(!trajectory.empty()) trajectory.pop();
	}
}

void Region::print(std::ostream& output) const{
	output << "**Region at level: " << level << " xmin=" << xmin << " xmax=" << xmax << " ymin=" << ymin << " ymax=" << ymax << std::endl;
	if(this->isLeaf()){
		if(point != 0)
			output << "   Point: " << *point << std::endl;
		else
			output << "   Point: " << "ABSENT" << std::endl;
	}else{
		output << "   Switching to children..." << std::endl;
		for(int i=0;i<4; i++){
			output << "   Child " << level << "." << i << ": " << std::endl;
			if(children[i]!=0)
				output << *children[i] << std::endl;
			else
				output << "     ABSENT" << std::endl;
		}
	}
}

std::ostream& operator<<(std::ostream& output, Region& reg){
	reg.print(output);
	return output;
}

int Region::selectregion(double x, double y) const{

	double middle_x = getMidX();
	double middle_y = getMidY();

	if(x<middle_x){
		if(y<middle_y){
			return 0;
		}else{
			return 2;
		}
	}else{
		if(y<middle_y){
			return 1;
		}else{
			return 3;
		}
	}
}

void Region::addtocorrectchildregion(double x, double y){
	int region = selectregion(x,y);

	double middle_x = getMidX();
	double middle_y = getMidY();

	if(children[region]==0){
		switch(region){
			case 0:
				children[region]= new Region(xmin,middle_x, ymin, middle_y, level+1);
				break;
			case 1:
				children[region]= new Region(middle_x,xmax, ymin, middle_y, level+1);
				break;
			case 2:
				children[region]= new Region(xmin,middle_x, middle_y, ymax, level+1);
				break;
			case 3:
				children[region]= new Region(middle_x,xmax, middle_y, ymax, level+1);
				break;
		}


	}
	children[region]->addPoint(x,y);
}

double Region::getMidX() const{
	return (xmax+xmin)/2;
}

double Region::getMidY() const{
	return (ymax+ymin)/2;
}

double calcDistanceSquare(double x1, double y1, double x2, double y2){
	//cout << "Calc distance between ("<<x1<<","<<y1<<") and ("<<x2<<","<<y2<<") : "<<(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) << endl;
	return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
}

double calcDistanceSquare(double x, double y, Point* p){
	if(p==0){
		return -1;
	}else{
		return calcDistanceSquare(x,y,p->getx(),p->gety());
	}
}

double calcDistanceSquare(Point* p1, Point* p2){
	if(p2!=0){
		return calcDistanceSquare(p2->getx(), p2->gety(), p1);
	}else{
		return -1;
	}
}

Region* Region::findParentOfClosestPoint(double x, double y){
	std::queue<Region*> queue;
	Region* currentClosest = 0;
	queue.push(this);

	while(!queue.empty()){
		Region* currentRegion = queue.front();
		queue.pop();
		//cout << "Pop" << endl;
		if(currentRegion->isLeaf()){ //then the closest point of this region would be the point in the region itsself
			if(currentClosest == 0 || currentClosest->point == 0){
				currentClosest = currentRegion;
				//cout << "Current closest set because no point has been found yet" << endl;
				
				//cout << "Press any key to continue..." << endl;
				//cin.get();
			}
			else if(currentRegion->point != 0 && 
				(currentClosest->point != 0 && calcDistanceSquare(x,y,currentRegion->point)<calcDistanceSquare(x,y,currentClosest->point))
				){
				currentClosest = currentRegion;
				//cout << "Closer point found" << endl;
				
				//cout << "Press any key to continue..." << endl;
				//cin.get();
			}
		}else{
			for(int i=0; i<4; i++){
				if(currentRegion->children[i] != 0){
					if(currentClosest == 0 || currentClosest->point == 0){
						queue.push(currentRegion->children[i]);
					}else if(currentRegion->children[i]->calcMinimumDistanceSquare(x, y) < calcDistanceSquare(x,y,currentClosest->point)){
						//cout << "Possible closer point in childregion so adding it to queue" << endl;
						queue.push(currentRegion->children[i]);
					}
				}
			}
		}
	}
	if(currentClosest != 0 && currentClosest->point!=0)
		return currentClosest;
	else
		return 0;
}

double Region::calcMinimumDistanceSquare(double x, double y){
	if(x<=xmin){
		if(y<=ymin){
			//1
			return calcDistanceSquare(x,y,xmin,ymin);
		}else if(y>ymax){
			//7
			return calcDistanceSquare(x,y,xmin,ymax);	
		}else{
			//8
			return calcDistanceSquare(x,y,xmin,y);
		}
	}else if(x>xmax){
		if(y<=ymin){
			//3
			return calcDistanceSquare(x,y,xmax,ymin);
		}else if(y>ymax){
			//5
			return calcDistanceSquare(x,y,xmax,ymax);
		}else{
			//4
			return calcDistanceSquare(x,y,xmax,y);
		}
	}else{
		if(y<=ymin){
			//2
			return calcDistanceSquare(x,y,x,ymin);
		}else if(y>ymax){
			//6
			return calcDistanceSquare(x,y,x,ymax);
		}else{
			//9
			return 0;
		}
	}
}

double Region::calcTotalDistance(Point* p){
	double distance = 0;
	if(this->isLeaf()){
		double distanceSquare = calcDistanceSquare(this->point, p);
		if(distanceSquare>0){
			distance = sqrt(distanceSquare);
		}
	}else{
		for(int i=0; i<4; i++){
			if(this->children[i]!=0){
				double h = this->children[i]->calcTotalDistance(p);
				if(h>0) distance+=h;
			}
		}
	}
	return distance;
}

Point* Region::getPoint(){
	if(isLeaf()){
		return this->point;
	}else{
		return 0;
	}
}

double Region::getRandX() const{
	return rand()%int(xmax-xmin+1)+xmin;
}

double Region::getRandY() const{
	return rand()%int(ymax-ymin+1)+ymin;
}

bool Region::isEmpty() const{
	return isLeaf() && point == 0;
}

/*int main(){
	Region reg(-1, 5, 2, 8, 0);

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

	//Looking for point closest to 4,6
	Region* parent = reg.findParentOfClosestPoint(4,6);
	if(parent != 0){
		cout << "Closest point: " << *parent;
	}

	//Looking for point closest to 3.5,4
	parent = reg.findParentOfClosestPoint(3.5,4);
	if(parent != 0){
		cout << "Closest point: " << *parent << endl;
		//calculatin total distance of point closest to 3.5,4
		cout << "TOTAL DISTANCE: " ;
		cout << reg.calcTotalDistance(parent->getPoint()) << endl;
	}


	//Starting the removal of all points


	reg.removePoint(100,100); //onbestaand punt

	cout << "After removal of inexistant point\n\n" << reg << endl;

	cout << "\n========================================================\n\n" << endl;

	reg.removePoint(0,2);

	cout << "After removal \n\n" << reg << endl;

	cout << "\n========================================================\n\n" << endl;

	reg.removePoint(3,3);

	cout << "After second removal \n\n" << reg << endl; 

	cout << "\n========================================================\n\n" << endl;

	reg.removePoint(0,4);

	cout << "After third removal \n\n" << reg << endl; 

	cout << "\n========================================================\n\n" << endl;

	reg.removePoint(4,7);

	cout << "After fourth removal \n\n" << reg << endl;

	cout << "\n========================================================\n\n" << endl;

	reg.removePoint(2,3);

	cout << "After last removal \n\n" << reg << endl;

	cout << "\n========================================================\n\n" << endl;

	cout << "Press any key to continue..." << endl;
	cin.get();
	return 0;
}*/
