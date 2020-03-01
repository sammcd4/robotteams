#pragma once

#include "chai3d.h"
#include <vector>

using namespace chai3d;
using namespace std;

enum polygonType {
	unrotatedRect, rotatedRect, nonRect
};

// polygon bounds
class rtsPolygonBounds
{
public:

	// Constructor
	rtsPolygonBounds();

	//rtsPolygonBounds(const double a_north, const double a_south, const double a_east, const double a_west, const double a_angle = 0);

	rtsPolygonBounds(vector<cVector3d> a_polygonPts);

	~rtsPolygonBounds();

	bool isInside(cVector3d a_pt);

private:

	void setBounds();

	//-----------------------------------------------------------------------
	// MEMBER VARIABLES:
	//-----------------------------------------------------------------------

	// id number
	int m_id;

	vector<cVector3d> m_pts;

	double m_north;
	double m_south;
	double m_east;
	double m_west;
	cVector3d m_s1;
	cVector3d m_s2;
	cVector3d m_s3;
	cVector3d m_s4;

	// angle of rotation
	double m_angle;

	// type of polygon (0 - unrotated rectangle, 1 - rotated rectangle, 2
	polygonType m_type;

	double m_tol;
};

