#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "line.h"

using namespace mathbase;

int main()
{	
	points plist1;
	point point1(0.0,0.0);
	point point2(10.0,10.0);
	plist1.push_back(point1);
	plist1.push_back(point2);

	points plist2;
	point point3(0.0,10.0);
	point point4(10.0,0.0);
	plist2.push_back(point3);
	plist2.push_back(point4);

	line line1;
	line1.setpoint(plist1);

	line line2;
	line2.setpoint(plist2);

	line tline1;
	line tline2;
	line1.getParallel(10.0,tline1);//
	line2.getParallel(-1.0,tline2);//

	point pIntersection;
	tline1.getIntersection(tline2,pIntersection);

	printf("Intersection(x:%f,y:%f)\n",pIntersection.x, pIntersection.y );

    return 0;
}