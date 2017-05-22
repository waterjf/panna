#include "precomp.h"
#include "px2DGeometry.h"

int g_bound_rect_dx[PX_CORNER_COUNT] = {0,0,0,1,2,2,2,1};
int g_bound_rect_dy[PX_CORNER_COUNT] = {0,1,2,2,2,1,0,0};
int g_bound_rect_tranform_mark[PX_CORNER_COUNT] = {
	pxTransformMark_NorthWest,
	pxTransformMark_West,
	pxTransformMark_SouthWest,
	pxTransformMark_South,
	pxTransformMark_SouthEast,
	pxTransformMark_East,
	pxTransformMark_NorthEast,
	pxTransformMark_North,
};

double px2DGeometry::g_xtolerance = PX_TOLERANCE;
double px2DGeometry::g_ytolerance = PX_TOLERANCE;

bool px2DGeometry::PointInRectangle( const pxPoint& point, const pxPoint& leftup, const pxPoint& rightbottom )
{
	return (point.m_x > leftup.m_x) && 
		(point.m_x < rightbottom.m_x) &&
		(point.m_y > leftup.m_y) &&
		(point.m_y < rightbottom.m_y);
}

bool px2DGeometry::PointNearPoint( const pxPoint& point1, const pxPoint& point2 )
{
	pxPoint leftup, rightbottom;
	leftup = point1;
	rightbottom = point1;
	RectAddTolerance(leftup, rightbottom);

	return PointInRectangle(point2, leftup, rightbottom);
}

void px2DGeometry::RectAddTolerance( pxPoint& leftup, pxPoint& rightbottom )
{
	leftup.m_x -= g_xtolerance;
	leftup.m_y -= g_ytolerance;
	rightbottom.m_x += g_xtolerance;
	rightbottom.m_y += g_ytolerance;
}

double px2DGeometry::PointToLineDistanceX( const pxPoint& p, const pxPoint lp[2] )
{
	double distance = lp[1].m_y - lp[0].m_y;
	if(distance > -COORD_EPSILON && distance < COORD_EPSILON)
	{//line 是水平线, 无水平交叉点
		distance = DBL_MAX;
	}
	else
	{
		distance = (p.m_y - lp[0].m_y) * (lp[1].m_x - lp[0].m_x) / distance;
		distance = distance + lp[0].m_x - p.m_x;
	}

	return distance;
}

double px2DGeometry::PointToLineDistanceY( const pxPoint& p, const pxPoint lp[2] )
{
	double distance = lp[1].m_x - lp[0].m_x;
	if(distance > -COORD_EPSILON && distance < COORD_EPSILON)
	{//line 是水平线, 无水平交叉点
		distance = DBL_MAX;
	}
	else
	{
		distance = (p.m_x - lp[0].m_x) * (lp[1].m_y - lp[0].m_y) / distance;
		distance = distance + lp[0].m_y - p.m_y;
	}

	return distance;
}

bool px2DGeometry::PointNearLine( const pxPoint& point, const pxPoint line_points[2] )
{
	pxPoint leftup, rightbottom;
	leftup = line_points[0];
	rightbottom = line_points[1];

	NormalizeRect(leftup, rightbottom);
	RectAddTolerance(leftup, rightbottom);

	if(PointInRectangle(point, leftup, rightbottom) )
	{//在外接矩形中
		double xdist = PointToLineDistanceX(point, line_points);
		double ydist = PointToLineDistanceY(point, line_points);

		if( (xdist < g_xtolerance && xdist > -g_xtolerance) ||
			(ydist < g_ytolerance && ydist > -g_ytolerance))
		{
			return true;
		}
	}

	return false;
}

bool px2DGeometry::PointLeftShotLine(const pxPoint& p, const pxPoint lp[2])
{
	//求水平交点x

	double x = lp[1].m_y - lp[0].m_y;
	if(x < -COORD_EPSILON || x > COORD_EPSILON)
	{//不是水平线段
		x = (p.m_y - lp[0].m_y) * (lp[1].m_x - lp[0].m_x) / x;
		x = x + lp[0].m_x;

		//交点需在左边且落在lp的x范围内
		if( (x < p.m_x) && 
			((x - min(lp[0].m_x, lp[1].m_x)) > -COORD_EPSILON) &&
			((x - max(lp[0].m_x, lp[1].m_x)) <  COORD_EPSILON) )
		{
			return true;
		}
	}

	return false;
}

void px2DGeometry::NormalizeRect( pxPoint& leftup, pxPoint& rightbottom )
{
	pxPoint min_point, max_point;

	min_point.m_x = min(leftup.m_x, rightbottom.m_x);
	min_point.m_y = min(leftup.m_y, rightbottom.m_y);
	max_point.m_x = max(leftup.m_x, rightbottom.m_x);
	max_point.m_y = max(leftup.m_y, rightbottom.m_y);

	leftup = min_point;
	rightbottom = max_point;
}

bool px2DGeometry::PointEqual(const pxPoint& point1, const pxPoint& point2)
{
	double x = point2.m_x - point1.m_x;
	double y = point2.m_y - point1.m_y;

	if(x < 0) x = -x;
	if(y < 0) y = -y;

	return (x < COORD_EPSILON && y < COORD_EPSILON);
}

void px2DGeometry::NormalizeAngle(const pxPoint& p, double& angle )
{
	if(angle < 0)
		angle = PI*2 + angle;

	if(p.m_x < 0)
	{
		if(p.m_y > 0)
			angle = PI*2 - angle;
	}
	else
	{
		if(p.m_y > 0)
			angle = PI*2 - angle;
	}

	angle = PI*2 - angle;
}

double px2DGeometry::PointDistance( const pxPoint& point1, const pxPoint& point2 )
{
	double cx = point2.m_x - point1.m_x;
	double cy = point2.m_y - point1.m_y;
	
	return sqrt(cx*cx+cy*cy);
}

double px2DGeometry::LineAngle( const pxPoint& origin, const pxPoint& point )
{
	double r = PointDistance(origin, point);
	double angle = 0;
	if(r > COORD_EPSILON)
	{
		angle = acos((point.m_x - origin.m_x ) / r);

		if((point.m_y - origin.m_y) < 0)
			angle = 2*PI - angle;
	}

	return angle;
}

double px2DGeometry::CoordDistance( double c1, double c2 )
{
	double c = c1 - c2;
	if(c < 0) c= -c;

	return c;
}

void px2DGeometry::ConcateRect( pxPoint& leftup, pxPoint& rightbottom, const pxPoint& lu, const pxPoint& rb )
{
	if(leftup.m_x > lu.m_x)
		leftup.m_x = lu.m_x;
	if(leftup.m_y > lu.m_y)
		leftup.m_y = lu.m_y;

	if(rightbottom.m_x < rb.m_x)
		rightbottom.m_x = rb.m_x;
	if(rightbottom.m_y < rb.m_y)
		rightbottom.m_y = rb.m_y;
}

void px2DGeometry::ConcateRect( pxPoint& leftup, pxPoint& rightbottom, const pxPoint point[4] )
{
	int n;
	for(n = 0; n < 4; n ++)
	{
		if(leftup.m_x > point[n].m_x)
			leftup.m_x = point[n].m_x;
		if(leftup.m_y > point[n].m_y)
			leftup.m_y = point[n].m_y;
	
		if(rightbottom.m_x < point[n].m_x)
			rightbottom.m_x = point[n].m_x;
		if(rightbottom.m_y < point[n].m_y)
			rightbottom.m_y = point[n].m_y;
	}
}

const int bezier_devide_count = 40;
const double bezier_delta_t = 1.0 / bezier_devide_count;

pxPoint px2DGeometry::Bezier(const pxPoint& start, const pxPoint& end, double t)
{
	pxPoint p;

	p.m_x = start.m_x + t * (end.m_x - start.m_x);
	p.m_y = (1 - t)*start.m_y + t*end.m_y;

	return p;
}

pxPoint px2DGeometry::Bezier( pxPoint* lp, int point_nr, double t )
{
	pxPoint* points = new pxPoint[point_nr];
	memcpy(points, lp, point_nr*sizeof(pxPoint));

	int line_count = point_nr - 1;
	while(line_count > 0)
	{
		int m;
		for(m = 0; m < line_count; m++)
		{
			points[m] = Bezier(points[m], points[m+1], t);
		}

		line_count --;
	}

	pxPoint p = points[0];
	delete[] points;

	return p;
}

bool px2DGeometry::PointNearBezier( const pxPoint& point, pxPoint* lp, int point_nr )
{//todo 用x求y，不用循环
	double t = 0;
	int n;
	for(n = 0; n < bezier_devide_count+1; n++)
	{
		pxPoint p = Bezier(lp, point_nr, t);

		if(px2DGeometry::PointNearPoint(p, point))
			return true;

		t += bezier_delta_t;
	}		

	return false;
}

bool px2DGeometry::AngleBetween( double angle, double angle_start, double angle_end )
{
	bool ret = false;

	if(angle > angle_start && angle < angle_end)
		ret = true;
	else if(angle_end < angle_start)//过0
	{//落在0~angle_end或 angle_start~2*PI?
		if((angle > 0 && angle < angle_end) ||
		   (angle > angle_start && angle < PI*2))
			ret = true;
	}

	return ret;
}

void px2DGeometry::RotatePoint( const pxPoint& refPoint, double angle, pxPoint& point )
{
	double r = px2DGeometry::PointDistance(point, refPoint);

	if(r > COORD_EPSILON)
	{
		//以refPoint为中心
		point = point - refPoint;
		//顺时针旋转angle
		double orgn_angle = acos(point.m_x / r);
		if(point.m_y < 0)
			orgn_angle = -orgn_angle;

		orgn_angle = orgn_angle + angle;
		if(orgn_angle > PI*2)
			orgn_angle = orgn_angle - PI*2;

		point.m_x = r * cos(orgn_angle);
		point.m_y = r * sin(orgn_angle);

		point += refPoint;
	}
}

void px2DGeometry::RotateRect( const pxPoint& refPoint, double angle, pxPoint point[4] )
{
	int n;
	for(n = 0; n < 4; n++)
		RotatePoint(refPoint, angle, point[n]);
}

void px2DGeometry::Init4PointRect( const pxPoint& leftup, const pxPoint& rightbottom, pxPoint point[4] )
{
	point[0] = leftup;
	point[2] = rightbottom;

	point[1].m_x = point[0].m_x;
	point[1].m_y = point[2].m_y;
	point[3].m_x = point[2].m_x;
	point[3].m_y = point[0].m_y;
}

void px2DGeometry::AdjsutTolerance( double xscale, double yscale )
{
	px2DGeometry::g_xtolerance = PX_TOLERANCE * xscale;
	px2DGeometry::g_ytolerance = PX_TOLERANCE * yscale;
}

double px2DGeometry::GetTolerance()
{
	return min(g_xtolerance, g_ytolerance);
}

void px2DGeometry::FlipPoint( const pxPoint& refPoint, bool flip_x, bool flip_y, pxPoint& point )
{
	if(flip_x)
		point.m_x += (refPoint.m_x - point.m_x) * 2;
	if(flip_y)
		point.m_y += (refPoint.m_y - point.m_y) * 2;
}
