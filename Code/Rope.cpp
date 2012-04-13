#include "Rope.h"

#include <iostream>
using namespace std;

Rope::Rope(){
}

Rope::Rope(MassPoint3D* start){
	// add the startpoints to the pointList
	//list<MassPoint3D*> pointList;
	pointList.push_front(start);
	//pointList.push_front(*start);

    cout<< "Hey, got one masspoint!, size=" << pointList.size() << endl;

	//springList = list<Spring*>;
}

void Rope::addNode(MassPoint3D* next){
	MassPoint3D* last = pointList.front();
	pointList.push_front(next);
	//pointList.push_front(*next);

	// Create the connection Springs:
	Spring* spring1 = new Spring(last, next, 3);
	//MassPoint3D last = pointList.front();
	//Spring* spring1 = new Spring(&last, next, 0.31);
	spring1->setSize(5);

	// Add Springs to the springList
	springList.push_front(spring1);

    cout << "Yeah, new point: pointList.size=" << pointList.size() << ", sprintList.size=" << springList.size() << endl;
}




