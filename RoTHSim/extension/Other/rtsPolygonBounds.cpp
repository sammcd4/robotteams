#include "rtsPolygonBounds.h"

rtsPolygonBounds::rtsPolygonBounds()
{

}

rtsPolygonBounds::rtsPolygonBounds(vector<cVector3d> a_polygonPts)
{
	m_pts = a_polygonPts;
	m_tol = 0.01;

	// check for rectangle
	if (m_pts.size() == 4)
	{
		bool rectangle = false;
		cVector3d s1 = m_pts[1] - m_pts[0];
		cVector3d s2 = m_pts[2] - m_pts[1];
		cVector3d s3 = m_pts[3] - m_pts[2];
		cVector3d s4 = m_pts[0] - m_pts[3];
		m_s1 = s1;
		m_s2 = s2;
		m_s3 = s3;
		m_s4 = s4;

		double test1 = cDot(s1, s2);
		double test2 = cDot(s3, s4);

		// check for parallel sides
		if ((cDot(s1, s2) < m_tol) && (cDot(s3, s4) < m_tol))
		{
			cVector3d cycle = s1 + s2 + s3 + s4;
			// check for connected sides
			if (cycle.length() <= m_tol)
			{
				rectangle = true;
			}
		}

		if (rectangle)
		{
			double proj1 = cDot(s1, cVector3d(1, 0, 0));
			double proj2 = cDot(s1, cVector3d(0, 1, 0));

			// check if rotated
			if ((fabs(proj1) < m_tol) || (fabs(proj2) < m_tol))
			{
				m_type = unrotatedRect;
				setBounds();
			}
			else m_type = rotatedRect;
		}
		else
		{
			m_type = nonRect;
		}
	}
	else
	{
		// not a rectangle
		m_type = nonRect;
	}
}

rtsPolygonBounds::~rtsPolygonBounds()
{

}

bool rtsPolygonBounds::isInside(cVector3d a_pt)
{
	if (m_type == unrotatedRect)
	{
		bool inside = false;
		if (a_pt.y() <= m_north)
		{
			if (a_pt.y() >= m_south)
			{
				if (a_pt.x() <= m_east)
				{
					if (a_pt.x() >= m_west)
					{
						inside = true;
					}
				}
			}
		}

		return inside;
	}
	else if (m_type == rotatedRect)
	{
		cVector3d M = a_pt - m_pts[0];
		if ((0 <= cDot(M, m_s1)) && (cDot(M, m_s1) <= (cDot(m_s1, m_s1)) && (0 <= cDot(M, -m_s4)) && (cDot(M, -m_s4) <= (cDot(-m_s4, -m_s4)))))
		{
			return true;
		}
		else return false;
	}
	else if (m_type == nonRect)
	{
		// further implementation needed
		return false;
	}

	return false;
}

void rtsPolygonBounds::setBounds()
{
	// only use if unrotated rectangle
	if (m_type == unrotatedRect)
	{
		double n(-C_LARGE), s(C_LARGE), e(-C_LARGE), w(C_LARGE);
		for (int i = 0; i < 4; i++)
		{
			cVector3d pt = m_pts[i];
			double x = pt.x();
			double y = pt.y();
			if (y > n)
				n = y;
			if (y < s)
				s = y;
			if (x > e)
				e = x;
			if (x < w)
				w = x;
		}
		m_north = n;
		m_south = s;
		m_east = e;
		m_west = w;
	}
}