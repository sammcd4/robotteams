#ifndef CPOLYGON_H
#define CPOLYGON_H
//---------------------------------------------------------------------------
#include "chai3d.h"
#include <vector>
//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;

//--------------------------------

// Polygon shape inherited from cMesh class
class rtsPolygon : public cMesh
{

public:

	//-----------------------------------------------------------------------
    // CONSTRUCTORS & DESTRUCTOR:
    //-----------------------------------------------------------------------

    //! Constructor of rtsPolygon, calling appropriate cMesh constructor
	rtsPolygon(vector<cVector3d> a_polygonNodes, vector<cVector3d> a_triangleNodes, double a_height) :cMesh()
	{
		// set polygon nodes
		m_polygonNodes = a_polygonNodes;

		// set number of nodes
		m_numNodes = m_polygonNodes.size();

		// set triangle construction nodes
		m_triangleNodes = a_triangleNodes;

		// set height
		m_height = a_height;

		// default the mesh to being shown
		m_showMesh = true;
	}

    //! Destructor of rtsPolygon.
    virtual ~rtsPolygon();

	//-----------------------------------------------------------------------
    // METHODS:
    //-----------------------------------------------------------------------
	
	// create box mesh
	void createPolygon();

	// get corner vector by index for collisions
	cVector3d getCornerVector(int a_corner);

	// set polygon color
	void setColor(cColorf a_color);

	// set show mesh
	void setShowMesh(bool a_showMesh) { m_showMesh = a_showMesh; }

	// get show mesh
	bool getShowMesh() { return m_showMesh; }

	// get polygon nodes
	vector<cVector3d> getPolygonNodes() { return m_polygonNodes; }

	// get center of mass location
	cVector3d getCenterOfMass() { return m_centerOfMass; }

protected:

	//! Render the mesh itself.
	virtual void render(cRenderOptions& a_options);

	//-----------------------------------------------------------------------
    // MEMBERS:
    //-----------------------------------------------------------------------
	
	// vector of polygon nodes
	vector<cVector3d> m_polygonNodes;

	// number of polygon nodes
	int m_numNodes;

	// vector of triangle construction nodes
	vector<cVector3d> m_triangleNodes;

	// polygon height
	double m_height;

	// store corner vectors relative to current position for collisions
	vector<cVector3d> cornerVectors;

	// show mesh (can enable or disable this without effecting haptics)
	bool m_showMesh;

	// center of mass
	cVector3d m_centerOfMass;
};

#endif // CPOLYGON_H
