#ifndef px2DGeometry_h__
#define px2DGeometry_h__

#include "pxDef.h"

class px2DGeometry
{
public:
	static bool PointEqual(const pxPoint& point1, const pxPoint& point2);
	static bool PointNearLine(const pxPoint& point, const pxPoint line_points[2]);
	static bool PointInRectangle(const pxPoint& point, const pxPoint& leftup, const pxPoint& rightbottom);
	static bool PointNearPoint(const pxPoint& point1, const pxPoint& point2);
	static bool PointNearBezier(const pxPoint& point, pxPoint* lp, int point_nr);
	
	static bool AngleBetween(double angle, double angle_start, double angle_end);

	static bool PointLeftShotLine(const pxPoint& point, const pxPoint line_points[2]);//point水平向左射线与线段相交
	static bool PointLeftShotArc(const pxPoint& point, const pxPoint line_points[2]);//point水平向左射线与线段相交
	static bool PointLeftShotBezier(const pxPoint& point, const pxPoint line_points[2]);//point水平向左射线与线段相交

	static double PointToLineDistanceX(const pxPoint& point, const pxPoint line_points[2]);
	static double PointToLineDistanceY(const pxPoint& point, const pxPoint line_points[2]);
	static double CoordDistance(double c1, double c2);

	static double PointDistance(const pxPoint& point1, const pxPoint& point2);
	static double LineAngle(const pxPoint& origin, const pxPoint& point);

	static void NormalizeRect(pxPoint& leftup, pxPoint& rightbottom);
	static void NormalizeAngle(const pxPoint& point, double& angle);//顺时针方向角度;

	static void RotatePoint(const pxPoint& refPoint, double angle, pxPoint& point);//以refPoint为基点 顺时针旋转point
	static void RotateRect(const pxPoint& refPoint, double angle, pxPoint point[4]);//以refPoint为基点 顺时针旋转矩形

	static void Init4PointRect(const pxPoint& leftup, const pxPoint& rightbottom, pxPoint point[4]);
	static void ConcateRect(pxPoint& leftup, pxPoint& rightbottom, const pxPoint point[4]);
	static void ConcateRect(pxPoint& leftup, pxPoint& rightbottom, const pxPoint& lu, const pxPoint& rb);

	static void RectAddTolerance(pxPoint& leftup, pxPoint& rightbottom);	
	static void AdjsutTolerance(double xscale, double yscale);
	static double GetTolerance();

	static void FlipPoint(const pxPoint& refPoint, bool flip_x, bool flip_y, pxPoint& point);
public:
	static pxPoint Bezier( pxPoint* lp, int point_nr, double t );
	static pxPoint Bezier( const pxPoint& start, const pxPoint& end, double t );

protected:
	static double g_xtolerance;
	static double g_ytolerance;
};

#define PX_CORNER_COUNT 8

extern int g_bound_rect_dx[PX_CORNER_COUNT];
extern int g_bound_rect_dy[PX_CORNER_COUNT];
extern int g_bound_rect_tranform_mark[PX_CORNER_COUNT];

#endif // px2DGeometry_h__
