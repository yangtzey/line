#ifndef LINE_H
#define LINE_H

#include"point.h"

namespace mathbase
{


class line
{
public:
	line()
	{

	}
	~line()
	{

	}
	bool setpoint(points ps);
	bool setpoint(point startp, point endp);
	bool getSlope(double& slope);
	point getStartPos();
	point getEndPos();
	//right side is plus
	bool getParallel(double translation,line& outline);
	point getLineDistancePos(double k,double n,point pos,double distance);
	point getLineDistanceStartPos(double k,double n,point pos,point epos,double distance);
	point getLineDistanceEndPos(double k,double n,point pos,point spos,double distance);
	bool getProjectionPos(point ps,bool& online,point& outpoint);
	bool getIntersection(line l,point& ps);
	
private:
	points m_points;
	double m_slope;	
};


}
#endif // namespace math base
