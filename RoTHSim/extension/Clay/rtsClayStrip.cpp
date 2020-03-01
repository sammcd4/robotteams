
//---------------------------------------------------------------------------
#include "rtsClayStrip.h"
//---------------------------------------------------------------------------
typedef GLUquadric GLUquadricObj;
//---------------------------------------------------------------------------

//===========================================================================
/*!
    Constructor of rtsClayStrip.

    \fn     rtsClayStrip::rtsClayStrip()
    \param  a_
*/
//===========================================================================
rtsClayStrip::rtsClayStrip(cWorld* a_world, rtsClayMesh* a_playdohMesh, bool a_isCurved, bool a_isPrevStrip, bool a_isFutureStrip, cVector3d a_p0, cVector3d a_p1, cVector3d a_p2, cVector3d a_p3)
{
	// set parent Clay world
	m_world = a_world;

	// set playdoh mesh
	m_playdohMesh = a_playdohMesh;

	// set default sphere spacing
	m_defaultRadiusToSpacingRatio = 5;

	// set default sphere radius
	m_defaultSphereRadius = 0.25;

	// set sphere spacing
	m_sphereSpacing = m_defaultSphereRadius / m_defaultRadiusToSpacingRatio;

	// set robot positions
	m_p0 = a_p0;
	m_p1 = a_p1;
	m_p2 = a_p2;
	m_p3 = a_p3;

	// set curvature
	m_isCurved = a_isCurved;

	// previous and future strips
	m_isPrevStrip = a_isPrevStrip;
	m_isFutureStrip = a_isFutureStrip;

	// create Clay strip connecting robots 1 and 2
	if(m_isCurved)
		createCurvedStrip();
	else createLineStrip();
};

void rtsClayStrip::setStiffness(double a_stiffness)
{
	rtsClayNode* temp;
	list<rtsClayNode*>::iterator iter;
	for (iter = m_spheres.begin(); iter != m_spheres.end(); iter++)
	{
		// dereference iterator
		temp = *iter;
		
		// set stiffness for the sphere
		temp->setStiffness(a_stiffness);
	}
}

void rtsClayStrip::createLineStrip()
{
	// vector from robot1 to robot2
	cVector3d diff12 = m_p2 - m_p1;
	cVector3d ndiff12 = cNormalize(diff12);

	// define number of spheres
	int numSpheres = (int)(diff12.length() / m_sphereSpacing);

	// add spheres between p1 and p2
	for (int i = 0; i < numSpheres; i++)
	{
		// create sphere and add it to the Clay's world
		rtsClayNode* sphere = new rtsClayNode();
		//m_world->addChild(sphere);

		//// set the first sphere to be at robot location
		//if(i == 0)
		//	sphere->setAtRobot(true);

		// set position of sphere along between robot points
		sphere->m_pos = m_p1 + (double)i / (double)numSpheres*diff12;

		//// set visibility
		//sphere->setShowEnabled(true);
		//sphere->setHapticEnabled(true);

		//// set color
		//cMaterial matSphere;
		//matSphere.m_ambient.set(0.0, 0.0, 1.0);
		//matSphere.m_diffuse.set(0.8, 0.4, 0.4);
		//matSphere.m_specular.set(1.0, 1.0, 1.0);
		//sphere->setMaterial(matSphere);

		//// set haptic effect on sphere
		//cEffectSurface* newEffect = new cEffectSurface(sphere);
		//sphere->addEffect(newEffect);

		// add this sphere to the list of spheres
		m_spheres.push_back(sphere);

		// add this sphere to the list of nodes
		m_playdohMesh->m_nodes.push_front(sphere);
	}
}

void rtsClayStrip::createLinks()
{
	rtsClayNode* current;
	rtsClayNode* next;
	list<rtsClayNode*>::iterator iter;
	list<rtsClayNode*>::iterator iterNext;
	for (iter = m_spheres.begin(); iter != m_spheres.end(); iter++)
	{
		// dereference iterator
		current = *iter;
		
		// get next element in list
		iterNext = iter;
		if ((iterNext != m_spheres.end()) && ((++iterNext) == m_spheres.end()))
		{
			// points at the last element
			std::cout << "last element" << std::endl;
		}
		else
		{
			//iterNext++;
			// dereference iterator
			next = *iterNext;

			// create link
			rtsClayLink* newLink = new rtsClayLink(current, next);
			m_playdohMesh->m_links.push_front(newLink);
		}
	}

}

void rtsClayStrip::setIsCurved(bool a_isCurved)
{
	m_isCurved = a_isCurved;
}

void rtsClayStrip::createCurvedStrip()
{

	// vector from robot1 to robot2
	cVector3d diff12 = m_p2 - m_p1;
	cVector3d norm12 = cNormalize(diff12);
	double distance = diff12.length();

	// normal vectors for curving
	cVector3d norm01 = m_p1 - m_p0;
	norm01.normalize();
	cVector3d norm32 = m_p2 - m_p3;
	norm32.normalize();

	// define number of spheres
	int numSpheres = (int)(diff12.length() / m_sphereSpacing);

	// add spheres between p1 and p2
	for (int i = 0; i < numSpheres; i++)
	{
		// create sphere and add it to the Clay's world
		rtsClayNode* sphere = new rtsClayNode();
		//m_world->addChild(sphere);

		//// set the first sphere to be at robot location
		//if(i == 0)
		//	sphere->setAtRobot(true);

		// set position of sphere along curved path between robot points
		cVector3d curveVec = norm12 + (double)(numSpheres - i) / (double)(numSpheres)*(norm01 + norm32);
		curveVec.normalize();
		//cVector3d curveVec2 = -1*norm12 + (double)(numSpheres - i) / (double)(numSpheres)*norm32;
		//curveVec2.normalize();

		cVector3d result = m_p1 + (double)i / (double)(numSpheres)*distance*curveVec;
		//cVector3d result2 = a_p2 + (double)i / (double)(numSpheres)*distance*curveVec2;
		sphere->m_pos = result;

		//// set visibility
		//sphere->setShowEnabled(true);
		//sphere->setHapticEnabled(true);

		//// set color
		//cMaterial matSphere;
		//matSphere.m_ambient.set(0.0, 0.0, 1.0);
		//if (i == 0)
		//{
		//	matSphere.m_ambient.set(1.0, 0.0, 0.0);
		//}
		//matSphere.m_diffuse.set(0.8, 0.4, 0.4);
		//matSphere.m_specular.set(1.0, 1.0, 1.0);
		//sphere->setMaterial(matSphere);

		//if (i == 0 || i == numSpheres - 1)
		//{
		//	matSphere.m_ambient.set(1.0, 0.0, 0.0);
		//}

		//// set haptic effect on sphere
		//cEffectSurface* newEffect = new cEffectSurface(sphere);
		//sphere->addEffect(newEffect);

		// add this sphere to the list of spheres
		m_spheres.push_back(sphere);

		// add this sphere to the list of nodes
		m_playdohMesh->m_nodes.push_front(sphere);
	}
}

void rtsClayStrip::updateClayStrip(cVector3d a_p0, cVector3d a_p1, cVector3d a_p2, cVector3d a_p3)
{
	// update robot positions
	m_p0 = a_p0;
	m_p1 = a_p1;
	m_p2 = a_p2;
	m_p3 = a_p3;

	// recalculate sphere positions

	// vector from robot1 to robot2
	cVector3d diff12 = m_p2 - m_p1;
	cVector3d norm12 = cNormalize(diff12);
	double distance = diff12.length();

	// normal vectors for curving
	cVector3d norm01 = m_p1 - m_p0;
	norm01.normalize();
	cVector3d norm32 = m_p2 - m_p3;
	norm32.normalize();

	// add spheres between p1 and p2
	int numSpheres = m_spheres.size();
	//for (int i = 0; i < numSpheres; i++)
	rtsClayNode* sphere;
	list<rtsClayNode*>::iterator iter;
	int i = 0;
	for (iter = m_spheres.begin(); iter != m_spheres.end(); iter++)
	{
		// derefernce iterator
		sphere = *iter;

		// set position of sphere along curved path between robot points
		cVector3d curveVec = norm12 + (double)(numSpheres - i) / (double)(numSpheres)*(norm01 + norm32);
		curveVec.normalize();

		cVector3d result = m_p1 + (double)i / (double)(numSpheres)*distance*curveVec;
		sphere->m_pos = result;	

		// increment
		i++;
	}
}

 void rtsClayStrip::addSphere(rtsClayNode* a_sphere1, rtsClayNode* a_sphere2, list<rtsClayNode*>::iterator iter)
{
	 // sphere2 might be redudant because it can be found using the iterator?


	// define new radius as an average of surrounding radii
	double radius1 = a_sphere1->m_radius;
	double radius2 = a_sphere2->m_radius;
	double newRadius = (radius1 + radius2) / 2;
	double anythingelse = a_sphere1->m_pos.length();

	// create new sphere to be added
	rtsClayNode* newSphere = new rtsClayNode();
	//m_world->addChild(newSphere);

	// define position of new sphere
	cVector3d pos1 = a_sphere1->m_pos;
	cVector3d pos2 = a_sphere2->m_pos;
	cVector3d mid12 = pos2 - pos1;
	newSphere->m_pos = pos1 + 0.5*mid12 + cVector3d(0.0, 0.0, 0.2);
	//newSphere->setPos(cVector3d(0.0, 0.0, 0.0));
	std::cout << newSphere->m_pos << std::endl;

	//// set color
	//cMaterial matSphere;
	//matSphere.m_ambient.set(0.0, 0.0, 0.0);
	//matSphere.m_diffuse.set(0.8, 0.4, 0.4);
	//matSphere.m_specular.set(1.0, 1.0, 1.0);
	//newSphere->setMaterial(matSphere);

	//// define new stiffness as an average of surrounding stiffnesses
	//double stiffness1 = a_sphere1->m_material.getStiffness();
	//double stiffness2 = a_sphere2->m_material.getStiffness();
	//double newStiffness = (stiffness1 + stiffness2) / 2;
	//newSphere->m_material.setStiffness(newStiffness);

	//// set haptic effect on sphere
	//cEffectSurface* newEffect = new cEffectSurface(newSphere);
	//newSphere->addEffect(newEffect);

	//// set visibility
	//newSphere->setShowEnabled(true);
	//newSphere->setHapticEnabled(true);

	// add sphere to list
	m_spheres.insert(iter, newSphere);
}

 list<rtsClayNode*>::iterator rtsClayStrip::removeSphere(list<rtsClayNode*>::iterator iter)
{
	// remove sphere from the world
	//m_world->deleteChild(*iter);

	return m_spheres.erase(iter);
}

 cVector3d rtsClayStrip::computeForce(const cVector3d& a_cursor, double a_cursorRadius)
 {
	 // init temp variable
	 cVector3d force;
	 force.zero();

	 rtsClayNode* current;
	 list<rtsClayNode*>::iterator iter;

	 for (iter = m_spheres.begin(); iter != m_spheres.end(); iter++)
	 {
		 // set current strip
		 current = *iter;

		 cVector3d nodePos = current->m_pos;
		 double nodeRadius = current->m_radius;
		 double nodeStiffness = current->m_stiffness;
		 cVector3d f = computeNodeForce(a_cursor, a_cursorRadius, nodePos, nodeRadius, nodeStiffness);
		 cVector3d tmpfrc = cNegate(f);
		 current->setExternalForce(tmpfrc);
		 force.add(f);

		 // check ground
		 if (nodePos.z() < 0.0)
			 current->m_pos.z(0);

	 }

	 return force;
 }

 cVector3d rtsClayStrip::computeNodeForce(const cVector3d& a_cursor,
	 double a_cursorRadius,
	 const cVector3d& a_spherePos,
	 double a_radius,
	 double a_stiffness)
 {

	 // compute the reaction forces between the tool and the ith sphere.
	 cVector3d force;
	 force.zero();
	 cVector3d vSphereCursor = a_cursor - a_spherePos;

	 // check if both objects are intersecting
	 if (vSphereCursor.length() < 0.0000001)
	 {
		 return (force);
	 }

	 if (vSphereCursor.length() > (a_cursorRadius + a_radius))
	 {
		 return (force);
	 }

	 // compute penetration distance between tool and surface of sphere
	 double penetrationDistance = (a_cursorRadius + a_radius) - vSphereCursor.length();
	 cVector3d forceDirection = cNormalize(vSphereCursor);
	 force = cMul(penetrationDistance * a_stiffness, forceDirection);

	 // return result
	 return (force);
 }