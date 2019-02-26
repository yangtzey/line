#include"line.h"
#include <cmath>
#include <stdio.h>

#define POWTWO(__value__) (pow(__value__, 2))
#define ISZERO(__value__) (fabs(__value__) <= 1e-5)

namespace mathbase
{

bool line::setpoint(points ps)
{
	if (ps.size() != 2)
	{
		return false;
	}
	point startp = ps[0];
	point endp = ps[1];
	if (startp.x==endp.x&&startp.y==endp.y)
	{
		return false;
	}
	m_points = ps;
	return true;
}

bool line::setpoint(point startp, point endp)
{
	if (startp.x==endp.x&&startp.y==endp.y)
	{
		return false;
	}
	m_points.push_back(startp);
	m_points.push_back(endp);
	return true;
}

bool line::getSlope(double& slope)
{
	double k = 0;
	point startp = m_points[0];
	point endp = m_points[1];
	if (startp.x == endp.x)
	{
		return false;
	}
	else
	{

		k = (endp.y-startp.y)/(endp.x-startp.x);
		slope = k;
	}
	return true;
}

point line::getStartPos()
{
	return m_points[0];
}

point line::getEndPos()
{
	return m_points[1];
}

bool line::getParallel(double translation,line& outline)
{
	if (m_points.size() != 2)
	{
		return false;
	}
	if (translation == 0)
	{
		outline.setpoint(m_points);
	}
	else
	{
		point startp = m_points[0];
		point endp = m_points[1];
		point starto(0,0);
		point endo(0,0);
		if (startp.x == endp.x)
		{
			double t = endp.y > startp.y?1:-1; 
			starto.x = startp.x + translation*t;
			starto.y = startp.y;
			endo.x = endp.x + translation*t;
			endo.y = endp.y;
		}
		else if (startp.y == endp.y)
		{
			double t = endp.x > startp.x?-1:1;
			starto.y = startp.y + translation*t;
			starto.x = startp.x;
			endo.y = endp.y + translation*t;
			endo.x = endp.x;
		}
		else
		{
			double k = -(endp.x-startp.x)/(endp.y-startp.y);
			double b1 = startp.y - startp.x*k;
			starto = getLineDistanceStartPos(k,b1,startp,endp,translation);
			double b2 = endp.y - endp.x*k;
			endo = getLineDistanceEndPos(k,b2,endp,startp,translation);
		}
		points ps;
		ps.push_back(starto);
		ps.push_back(endo);	
		outline.setpoint(ps);
	}	
	return true;
}



bool line::getProjectionPos(point ps,bool& online,point& outpoint)
{
	if (m_points.size() != 2)
	{
		return false;
	}

	point startp = m_points[0];
	point endp = m_points[1];
	point starto(0,0);
	point endo(0,0);
	if (startp.x == endp.x)
	{
		outpoint.x=startp.x;
		outpoint.y = ps.y;
		if ((startp.y >= ps.y && ps.y >= endp.y)
			||(startp.y <= ps.y && ps.y <= endp.y))
		{
			online = true; 
		}
		else
		{
			online = false;
		}	
	}
	else if (startp.y == endp.y)
	{
		outpoint.y=startp.y;
		outpoint.x = ps.x;
		if ((startp.x >= ps.x && ps.x >= endp.x)
			||(startp.x <= ps.x && ps.x <= endp.x))
		{
			online = true; 
		}
		else
		{
			online = false;
		}
	}
	else
	{
		double k1 = (endp.y-startp.y)/(endp.x-startp.x);
		double b1 = startp.y - startp.x*k1;

		if ((ps.x*k1 + b1) == ps.y)
		{
			if ((startp.x >= ps.x && ps.x >= endp.x)
				||(startp.x <= ps.x && ps.x <= endp.x))
			{
				online = true; 
			}
			else
			{
				online = false;
			}
			outpoint.y = ps.y;
			outpoint.x = ps.x;
		}
		else
		{
			double x0 = ps.x;
			double y0 = ps.y;

			double x1 = (k1*y0+x0-k1*b1)/(POWTWO(k1)+1);
			double y1 = (POWTWO(k1)*y0+k1*x0+b1)/(POWTWO(k1)+1);

			if ((startp.x >= x1 && x1 >= endp.x)
				||(startp.x <= x1 && x1 <= endp.x))
			{
				online = true; 
			}
			else
			{
				online = false;
			}
			outpoint.y = y1;
			outpoint.x = x1;
		}

		
	}

	return true;
}


point line::getLineDistancePos(double k,double n,point pos,double distance)
{
	//points rPoints;
	double a = pow(k,2) + 1;
	double b = k*(n-pos.y)*2.0 - pos.x*2.0;
	double c = pow(pos.x,2)+ pow(n-pos.y,2) -pow(distance,2);
	double x1,y1,x2,y2 = 0.0;
	if (ISZERO(b))
	{
		double sqrtValue = -a*c*4.0;
		double v = (double)sqrt(sqrtValue);
		x1 = (v)/(a*2.0);
		y1 = k*x1+n;
		x2 = (-sqrt(sqrtValue))/(a*2.0);
		y2 = k*x2+n;
	}
	else
	{
		x1 = (-b + sqrt((double)pow(b,2)-a*c*4.0))/(a*2.0);
		y1 = k*x1+n;
		x2 = (-b - sqrt((double)pow(b,2)-a*c*4.0))/(a*2.0);
		y2 = k*x2+n;
	}
	point op(0,0);
	printf("===k: %f\n", k);
	printf("===d: %f\n", distance);
	if(k>0)
	{
		if (distance>0)
		{
			if (x1>x2)
			{
				op.set(x1,y1);
			}
			else
			{
				op.set(x2,y2);				
			}	
		}
		else
		{
			if (x1<x2)
			{
				op.set(x1,y1);				
			}
			else
			{
				op.set(x2,y2);
			}
		}
	}
	else
	{
		if (distance>0)
		{
			if (x1>x2)
			{
				op.set(x1,y1);
			}
			else
			{
				op.set(x2,y2);
			}	
		}
		else
		{
			if (x2>x1)
			{
				op.set(x2,y2);
			}
			else
			{
				op.set(x1,y1);
			}
		}
	}
	printf("x1: %f y1: %f \nx2: %f y2: %f\n", x1, y1,x2,y2);
	return op;
}
point line::getLineDistanceStartPos(double k,double n,point pos,point epos,double distance)
{
	//points rPoints;
	double a = pow(k,2) + 1;
	double b = k*(n-pos.y)*2.0 - pos.x*2.0;
	double c = pow(pos.x,2)+ pow(n-pos.y,2) -pow(distance,2);
	double x1,y1,x2,y2 = 0.0;
	if (ISZERO(b))
	{
		double sqrtValue = -a*c*4.0;
		double v = (double)sqrt(sqrtValue);
		x1 = (v)/(a*2.0);
		y1 = k*x1+n;
		x2 = (-sqrt(sqrtValue))/(a*2.0);
		y2 = k*x2+n;
	}
	else
	{
		x1 = (-b + sqrt((double)pow(b,2)-a*c*4.0))/(a*2.0);
		y1 = k*x1+n;
		x2 = (-b - sqrt((double)pow(b,2)-a*c*4.0))/(a*2.0);
		y2 = k*x2+n;
	}
	point op(0,0);
	printf("===k: %f\n", k);
	printf("===d: %f\n", distance);
	if (k < 0)
	{
		if(pos.x < epos.x)
		{
			if (distance>0)
			{
				if (x1>x2)
				{
					op.set(x1,y1);
				}
				else
				{
					op.set(x2,y2);				
				}	
			}
			else
			{
				if (x1<x2)
				{
					op.set(x1,y1);				
				}
				else
				{
					op.set(x2,y2);
				}
			}
		}
		else
		{
			if (distance>0)
			{
				if (x1<x2)
				{
					op.set(x1,y1);
				}
				else
				{
					op.set(x2,y2);
				}	
			}
			else
			{
				if (x2>x1)
				{
					op.set(x2,y2);
				}
				else
				{
					op.set(x1,y1);
				}
			}
		}
	}
	else
	{
		if(pos.x < epos.x)
		{
			if (distance>0)
			{
				if (x1<x2)
				{
					op.set(x1,y1);
				}
				else
				{
					op.set(x2,y2);				
				}	
			}
			else
			{
				if (x1>x2)
				{
					op.set(x1,y1);				
				}
				else
				{
					op.set(x2,y2);
				}
			}
		}
		else
		{
			if (distance>0)
			{
				if (x1>x2)
				{
					op.set(x1,y1);
				}
				else
				{
					op.set(x2,y2);
				}	
			}
			else
			{
				if (x2<x1)
				{
					op.set(x2,y2);
				}
				else
				{
					op.set(x1,y1);
				}
			}
		}
	}
	
	printf("x1: %f y1: %f \nx2: %f y2: %f\n", x1, y1,x2,y2);
	return op;
}

point line::getLineDistanceEndPos(double k,double n,point pos,point spos,double distance)
{
	//points rPoints;
	double a = pow(k,2) + 1;
	double b = k*(n-pos.y)*2.0 - pos.x*2.0;
	double c = pow(pos.x,2)+ pow(n-pos.y,2) -pow(distance,2);
	double x1,y1,x2,y2 = 0.0;
	if (ISZERO(b))
	{
		double sqrtValue = -a*c*4.0;
		double v = (double)sqrt(sqrtValue);
		x1 = (v)/(a*2.0);
		y1 = k*x1+n;
		x2 = (-sqrt(sqrtValue))/(a*2.0);
		y2 = k*x2+n;
	}
	else
	{
		x1 = (-b + sqrt((double)pow(b,2)-a*c*4.0))/(a*2.0);
		y1 = k*x1+n;
		x2 = (-b - sqrt((double)pow(b,2)-a*c*4.0))/(a*2.0);
		y2 = k*x2+n;
	}
	point op(0,0);
	printf("===k: %f\n", k);
	printf("===d: %f\n", distance);
	if (k < 0)
	{
		if(spos.x < pos.x)
		{
			if (distance>0)
			{
				if (x1>x2)
				{
					op.set(x1,y1);
				}
				else
				{
					op.set(x2,y2);				
				}	
			}
			else
			{
				if (x1<x2)
				{
					op.set(x1,y1);				
				}
				else
				{
					op.set(x2,y2);
				}
			}
		}
		else
		{
			if (distance>0)
			{
				if (x1<x2)
				{
					op.set(x1,y1);
				}
				else
				{
					op.set(x2,y2);
				}	
			}
			else
			{
				if (x2>x1)
				{
					op.set(x2,y2);
				}
				else
				{
					op.set(x1,y1);
				}
			}
		}
	}
	else
	{
		if(spos.x < pos.x)
		{
			if (distance>0)
			{
				if (x1<x2)
				{
					op.set(x1,y1);
				}
				else
				{
					op.set(x2,y2);				
				}	
			}
			else
			{
				if (x1>x2)
				{
					op.set(x1,y1);				
				}
				else
				{
					op.set(x2,y2);
				}
			}
		}
		else
		{
			if (distance>0)
			{
				if (x1>x2)
				{
					op.set(x1,y1);
				}
				else
				{
					op.set(x2,y2);
				}	
			}
			else
			{
				if (x2<x1)
				{
					op.set(x2,y2);
				}
				else
				{
					op.set(x1,y1);
				}
			}
		}
	}
	
	printf("x1: %f y1: %f \nx2: %f y2: %f\n", x1, y1,x2,y2);
	return op;
}


bool line::getIntersection(line l,point& ps)
{	
	double k1 = 0;
	double k2 = 0;
	if (!l.getSlope(k1) || !getSlope(k2))
	{
		if (!l.getSlope(k1) && getSlope(k2))
		{
			double b2 = m_points[0].y - k2*m_points[0].x;
			if (k2 != 0)
			{
				ps.set(l.getStartPos().x,l.getStartPos().x*k2+b2);
			}
			else
			{
				ps.set(l.getStartPos().x,m_points[0].y);
			}		
			
		}
		else if(l.getSlope(k1) && !getSlope(k2))
		{
			double b1 = l.getStartPos().y - k1*l.getStartPos().x;

			if (k1 != 0)
			{
				ps.set(m_points[0].x,m_points[0].x*k1+b1);
			}
			else
			{
				ps.set(m_points[0].x,l.getStartPos().y);
			}
		}
		else
		{
			return false;
		}

	}
	else
	{
		double b1 = l.getStartPos().y - k1*l.getStartPos().x;
		double b2 = m_points[0].y - k2*m_points[0].x;
		if (k1 == k2)
		{
			return false;
		}
		double x = (b2-b1)/(k1-k2);
		double y = k1*x + b1;
		ps.set(x,y);
		return true;	
	}	

}

}

