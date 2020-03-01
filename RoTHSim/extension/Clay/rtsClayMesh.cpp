//===========================================================================
/*
    This file is part of the GEL dynamics engine.
    Copyright (C) 2003-2009 by Francois Conti, Stanford University.
    All rights reserved.

    \author    <http://www.chai3d.org>
    \author    Francois Conti
    \version   2.0.0 $Rev: 248 $
*/
//===========================================================================

//---------------------------------------------------------------------------
#include "rtsClayMesh.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
    Initialise deformable mesh.

    \fn       void rtsClayMesh::initialise()
*/
//===========================================================================
void rtsClayMesh::initialise()
{
    m_showSkeletonModel = false;
    m_showMassParticleModel = false;
    m_useSkeletonModel = false;
    m_useMassParticleModel = false;
}


//===========================================================================
/*!
    Clear forces.

    \fn       void rtsClayMesh::clearForces()
*/
//===========================================================================
void rtsClayMesh::clearForces()
{
    if (m_useSkeletonModel)
    {
        list<rtsClayNode*>::iterator i;

        for(i = m_nodes.begin(); i != m_nodes.end(); ++i)
        {
            (*i)->clearForces();
        }
    }
    if (m_useMassParticleModel)
    {
        vector<rtsClayVertex>::iterator i;

        for(i = m_gelVertices.begin(); i != m_gelVertices.end(); ++i)
        {
            i->m_massParticle->clearForces();
        }
    }
}


//===========================================================================
/*!
    Clear external forces on nodes.

    \fn       void rtsClayMesh::clearExternalForces()
*/
//===========================================================================
void rtsClayMesh::clearExternalForces()
{
    if (m_useSkeletonModel)
    {
        list<rtsClayNode*>::iterator i;

        for(i = m_nodes.begin(); i != m_nodes.end(); ++i)
        {
            (*i)->clearExternalForces();
        }
    }
    if (m_useMassParticleModel)
    {
        vector<rtsClayVertex>::iterator i;

        for(i = m_gelVertices.begin(); i != m_gelVertices.end(); ++i)
        {
            i->m_massParticle->clearExternalForces();
        }
    }
}


//===========================================================================
/*!
    Compute all internal forces.

    \fn       void rtsClayMesh::computeForces()
*/
//===========================================================================
void rtsClayMesh::computeForces()
{
    if (m_useSkeletonModel)
    {
        list<rtsClayLink*>::iterator i;

        for(i = m_links.begin(); i != m_links.end(); ++i)
        {
            (*i)->computeForces();
        }
    }
    if (m_useMassParticleModel)
    {
        list<rtsClayLinearSpring*>::iterator i;

        for(i = m_linearSprings.begin(); i != m_linearSprings.end(); ++i)
        {
            (*i)->computeForces();
        }
    }
}

//===========================================================================
/*!
    Compute next pose of each node.

    \fn       void rtsClayMesh::computeNextPose(double a_timeInterval)
*/
//===========================================================================
void rtsClayMesh::computeNextPose(double a_timeInterval)
{
    if (m_useSkeletonModel)
    {
        list<rtsClayNode*>::iterator i;

        for(i = m_nodes.begin(); i != m_nodes.end(); ++i)
        {
            (*i)->computeNextPose(a_timeInterval);
        }
    }
    if (m_useMassParticleModel)
    {
        vector<rtsClayVertex>::iterator i;

        for(i = m_gelVertices.begin(); i != m_gelVertices.end(); ++i)
        {
            i->m_massParticle->computeNextPose(a_timeInterval);
        }
    }
}


//===========================================================================
/*!
    Apply the next pose of each node.

    \fn       void rtsClayMesh::applyNextPose()
*/
//===========================================================================
void rtsClayMesh::applyNextPose()
{
    if (m_useSkeletonModel)
    {
        list<rtsClayNode*>::iterator i;

        for(i = m_nodes.begin(); i != m_nodes.end(); ++i)
        {
            (*i)->applyNextPose();
        }
    }
    if (m_useMassParticleModel)
    {
        vector<rtsClayVertex>::iterator i;

        for(i = m_gelVertices.begin(); i != m_gelVertices.end(); ++i)
        {
            i->m_massParticle->applyNextPose();
        }
    }
}

void rtsClayMesh::updateAllDynamics(double a_timeInterval)
{
	// check skeleton mode and expected sizes of links and nodes
	if (m_useSkeletonModel && m_links.size() == m_nodes.size())
	{
		list<rtsClayNode*>::iterator i;
		list<rtsClayNode*>::iterator nextIter;
		list<rtsClayLink*>::iterator link;

		// set link iterators
		link = this->m_links.begin();

		for (i = m_nodes.begin(); i != m_nodes.end(); ++i)
		{
			// get next node
			rtsClayNode* next;
			nextIter = i;
			if ((nextIter != m_nodes.end()) && ((++nextIter) == m_nodes.end()))
			{
				next = *(m_nodes.begin());
			}
			else
			{
				next = *nextIter;
			}

			// if at the beginning of list
			if (i == m_nodes.begin())
			{ 
				(*i)->clearForces();
				next->clearForces();
				(*link)->computeForces();
			}
			else
			{
				next->clearForces();
				(*link)->computeForces();
				(*i)->computeNextPose(a_timeInterval);
				(*i)->applyNextPose();
			}

			// if at the end of the node list, update first node
			if (next == *(m_nodes.begin()))
			{
				next->computeNextPose(a_timeInterval);
				next->applyNextPose();
			}

			// iterate (should work)
			link++;
		}
	}
	if (m_useMassParticleModel)
	{
		vector<rtsClayVertex>::iterator i;

		for (i = m_gelVertices.begin(); i != m_gelVertices.end(); ++i)
		{
			i->m_massParticle->applyNextPose();
		}
	}

}
//===========================================================================
/*!
This method graphically renders the deformable mesh in OpenGL.

\param  a_options  Rendering options.
*/
//===========================================================================
void rtsClayMesh::render(cRenderOptions& a_options)
{
	// render mesh
	cMultiMesh::render(a_options);

	/////////////////////////////////////////////////////////////////////////
	// Render parts that are always opaque
	/////////////////////////////////////////////////////////////////////////
	if (SECTION_RENDER_OPAQUE_PARTS_ONLY(a_options))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (m_showSkeletonModel)
		{
			if (m_enableMutex)
			{
				m_mutex->lock();

				// create an exact copy of the list of pointers to nodes
				m_renderNodes = m_nodes;

				// create an exact copy of the list of pointers to links
				m_renderLinks = m_links;

				m_mutex->unlock();
				m_enableMutex = false; //toFix (place before unlock)
			}
			else
			{
				if (!m_renderNodes.empty())
				{
					list<rtsClayNode*>::iterator i;
					for (i = m_renderNodes.begin(); i != m_renderNodes.end(); ++i)
					{
						rtsClayNode* nextItem = *i;
						nextItem->render();
					}
				}

				//if (!m_renderLinks.empty())
				//{
				//	list<rtsClayLink*>::iterator j;
				//	for (j = m_renderLinks.begin(); j != m_renderLinks.end(); ++j)
				//	{
				//		rtsClayLink* nextItem = *j;
				//		nextItem->render();
				//	}
				//}

			}
		}

		if (m_showMassParticleModel)
		{
			glDisable(GL_LIGHTING);

			list<rtsClayLinearSpring*>::iterator j;
			for (j = m_linearSprings.begin(); j != m_linearSprings.end(); ++j)
			{
				(*j)->render();
			}

			vector<rtsClayVertex>::iterator i;
			for (i = m_gelVertices.begin(); i != m_gelVertices.end(); ++i)
			{
				i->m_massParticle->render();
			}

			glEnable(GL_LIGHTING);
		}
	}
}

//---------------------------------------------------------------------------

void rtsClayMesh::setMutex(std::mutex* a_mutex)
{
	// set mutex
	m_mutex = a_mutex;

	// initialize enable value
	m_enableMutex = false;
}

//===========================================================================
/*!
    Build dynamic vertices for deformable mesh

    \fn       void rtsClayMesh::buildVertices();
*/
//===========================================================================
void rtsClayMesh::buildVertices()
{
	// for all meshes, activate user data
	vector<cMesh*>::iterator it;
	for (it = m_meshes->begin(); it < m_meshes->end(); it++)
	{
		(*it)->m_vertices->allocateData(0, true, true, true, true, true, true);
	}

	// clear all deformable vertices
	m_gelVertices.clear();

	// get number of vertices
	int numVertices = getNumVertices();

	// create a table of deformable vertices based on the vertices of image
	for (int i = 0; i<numVertices; i++)
	{
		cMesh* mesh = NULL;
		unsigned int vertexIndex = 0;

		// create a new deformable vertex data
		if (getVertex(i, mesh, vertexIndex))
		{
			rtsClayVertex newDefVertex(mesh, vertexIndex);
			newDefVertex.m_link = NULL;
			newDefVertex.m_node = NULL;
			mesh->m_vertices->setUserData(vertexIndex, i);

			// add vertex to list
			m_gelVertices.push_back(newDefVertex);
		}
	}
}


//===========================================================================
/*!
    Connect each vertex to nearest node.

    \fn     void rtsClayMesh::connectVerticesToSkeleton(bool a_connectToNodesOnly)
    \param  a_connectToNodesOnly  if \b true, then skin is only connected to nodes.
                otherwise skin shall be connected to links too.
*/
//===========================================================================
void rtsClayMesh::connectVerticesToSkeleton(bool a_connectToNodesOnly)
{
    // get number of vertices
    int numVertices = m_gelVertices.size();

    // for each deformable vertex we search for the nearest sphere or link
    for (int i=0; i<numVertices; i++)
    {
        // get current deformable vertex
        rtsClayVertex* curVertex = &m_gelVertices[i];

        // get current vertex position
		cVector3d pos = curVertex->m_mesh->m_vertices->getLocalPos(curVertex->m_vertexIndex);

        // initialize constant
        double min_distance = 99999999999999999.0;
        rtsClayNode* nearest_node = NULL;
        rtsClayLink* nearest_link = NULL;

        // search for the nearest node
		list<rtsClayNode*>::iterator itr;
        for(itr = m_nodes.begin(); itr != m_nodes.end(); ++itr)
        {
            rtsClayNode* nextNode = *itr;
            double distance = cDistance(pos, nextNode->m_pos);
            if (distance < min_distance)
            {
                min_distance = distance;
                nearest_node = nextNode;
                nearest_link = NULL;
            }
        }

        // search for the nearest link if any
        if (!a_connectToNodesOnly)
        {
            list<rtsClayLink*>::iterator j;
            for(j = m_links.begin(); j != m_links.end(); ++j)
            {
                rtsClayLink* nextLink = *j;
                double angle0 = cAngle(nextLink->m_wLink01, cSub(pos, nextLink->m_node0->m_pos));
                double angle1 = cAngle(nextLink->m_wLink10, cSub(pos, nextLink->m_node1->m_pos));

                if ((angle0 < (C_PI / 2.0)) && (angle1 < (C_PI / 2.0)))
                {
                    cVector3d p = cProjectPointOnLine(pos,
                                                      nextLink->m_node0->m_pos,
                                                      nextLink->m_wLink01);

                    double distance = cDistance(pos, p);

                    if (distance < min_distance)
                    {
                        min_distance = distance;
                        nearest_node = NULL;
                        nearest_link = nextLink;
                    }
                }
            }
        }

        // attach vertex to nearest node if it exists
        if (nearest_node != NULL)
        {
            curVertex->m_node = nearest_node;
            curVertex->m_link = NULL;
            cVector3d posRel = cSub(pos, nearest_node->m_pos);
            curVertex->m_massParticle->m_pos = cMul(cTranspose(nearest_node->m_rot), posRel);
        }

        // attach vertex to nearest link if it exists
        else if (nearest_link != NULL)
        {
            curVertex->m_node = NULL;
            curVertex->m_link = nearest_link;

            cMatrix3d rot;
            rot.setCol( nearest_link->m_A0,
                        nearest_link->m_B0,
                        nearest_link->m_wLink01);
            cVector3d posRel = cSub(pos, nearest_link->m_node0->m_pos);
            curVertex->m_massParticle->m_pos = cMul(cInverse(rot), posRel);
        }
    }
}


//===========================================================================
/*!
    Update position of vertices connected to skeleton.

    \fn       void rtsClayMesh::updateVertexPosition()
*/
//===========================================================================
void rtsClayMesh::updateVertexPosition()
{
    if (m_useSkeletonModel)
    {
        // get number of vertices
        int numVertices = m_gelVertices.size();

        // for each deformable vertex, update its position
        for (int i=0; i<numVertices; i++)
        {
            // get current deformable vertex
            rtsClayVertex* curVertex = &m_gelVertices[i];

            // the vertex is attached to an node
            if (curVertex->m_node != NULL)
            {
                cVector3d newPos;
                curVertex->m_node->m_rot.mulr(curVertex->m_massParticle->m_pos, newPos);
                newPos.add(curVertex->m_node->m_pos);
				curVertex->m_mesh->m_vertices->setLocalPos(curVertex->m_vertexIndex, newPos);
			}

            else if (curVertex->m_link != NULL)
            {
                cVector3d newPos;
                curVertex->m_link->m_node0->m_pos.addr(curVertex->m_massParticle->m_pos.z() * curVertex->m_link->m_wLink01, newPos);
                newPos.add(curVertex->m_massParticle->m_pos.x() * curVertex->m_link->m_wA0);
                newPos.add(curVertex->m_massParticle->m_pos.y() * curVertex->m_link->m_wB0);


                //curVertex->node->m_rot.mulr(curVertex->pos, newPos);
                //newPos.add(curVertex->node->m_pos);
				curVertex->m_mesh->m_vertices->setLocalPos(curVertex->m_vertexIndex, newPos);
			}
        }
    }

    if (m_useMassParticleModel)
    {
        // get number of vertices
        int numVertices = m_gelVertices.size();

        // for each deformable vertex, update its position
        for (int i=0; i<numVertices; i++)
        {
            // get current deformable vertex
            rtsClayVertex* curVertex = &m_gelVertices[i];

            // the vertex is attached to an node
            if (curVertex->m_massParticle != NULL)
            {
				curVertex->m_mesh->m_vertices->setLocalPos(curVertex->m_vertexIndex, curVertex->m_massParticle->m_pos);
			}
        }
    }
}


