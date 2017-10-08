#include <stdafx.h>

#include "Pathfinding.h"
#include "NavMesh.h"
#include "NavMeshManager.h"
#include "Engine/Engine.h"
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>
#include "Scripts/ScriptManager.h"

CPathfinding * CPathfinding::m_pathfinding = nullptr;

CPathfinding &CPathfinding::Instance(Vect3f startPos, Vect3f endPos, std::string navMeshFile)
{
    if (!m_pathfinding)
        m_pathfinding = new CPathfinding(startPos, endPos, navMeshFile);
    return *m_pathfinding;
}

CPathfinding::CPathfinding()
{
    m_ObjectiveFound = false;
}

CPathfinding::CPathfinding(Vect3f startPos, Vect3f endPos, std::string navMeshFile)
{
    CNavMeshManager& l_NavMeshManager = CEngine::GetInstance().GetNavMeshManager();
    navmeshFilename = navMeshFile;
    m_navmesh = l_NavMeshManager(navmeshFilename);

    m_pathfinding = this;
    m_ObjectiveFound = false;
    //fill m_nodes
    uint16_t numTriangles = m_navmesh->GetNumTriangles();
    float cost = 0;
    for (uint16_t index = 0; index < numTriangles; ++index)
    {
        CPathNode node(m_navmesh->GetTriangle(index), cost, 0.f);
        node.SetCentrePos(m_navmesh->GetCenter(m_navmesh->GetTriangle(index)));
        m_nodes.push_back(node);
    }

    InitStartPosition(startPos);
    InitEndPosition(endPos);

}

CPathfinding::~CPathfinding()
{

}

const std::vector<Vect3f> &CPathfinding::GetPath() const
{
    return m_edgesPath;
}

CPathNode * CPathfinding::NearestNode(Vect3f &point)
{
    float currDist;
    //    float minDist = (m_nodes.begin()->GetPos() - point).Length(); //vertices
    float minDist = (m_navmesh->GetCenter(m_nodes.begin()->GetTriangle()) - point).Length(); //centro


    CPathNode * nearest = &(*m_nodes.begin());
    for (std::vector<CPathNode>::iterator itr = m_nodes.begin(); itr != m_nodes.end(); ++itr)
    {
        //currDist = (itr->GetPos() - point).Length(); //vertices
        currDist = (m_navmesh->GetCenter(itr->GetTriangle()) - point).Length();//centro
        if (currDist <= minDist)
        {
            minDist = currDist;
            nearest = &(*itr);
        }
    }
    return nearest;
}

void CPathfinding::UpdatePath()
{
    //need to set all parents to null before recalculating the path.
    //if not, there's an infinite loop in BuildPath()
    for (std::vector<CPathNode>::iterator itr = m_nodes.begin(); itr != m_nodes.end(); ++itr)
        itr->SetParent(nullptr);

    //set StartNode and EndNode
    m_startNode = NearestNode(m_StartPosition);
    m_endNode = NearestNode(m_EndPosition);

    //recalculate all estimated costs
    int16_t cost = 0;
    CPathNode * node;
    for (std::vector<CPathNode>::iterator itr = m_nodes.begin(); itr != m_nodes.end(); ++itr)
    {
        node = &(*itr);
        if (node->GetCost() != -1)
        {
            //estimated based on pixels distance
            float cost = node->GetCost();
            //float estimated = (sqrt(pow((m_endNode->GetPos().x - node->GetPos().x), 2) + pow(m_endNode->GetPos().z - node->GetPos().z, 2))); //vertices
            float estimated = (sqrt(pow((m_navmesh->GetCenter(m_endNode->GetTriangle()).x - node->GetPos().x), 2) + pow(m_navmesh->GetCenter(m_endNode->GetTriangle()).z - node->GetPos().z, 2))); //centro
            node->SetEstimatedCost(estimated);
            node->SetCost(cost + estimated);
            node->SetTotalCost(cost + estimated);
        }
    }

    //A*
    m_openNodes.clear();
    m_closedNodes.clear();
    m_startNode->SetCost(0);
    m_startNode->SetTotalCost(0);
    m_openNodes.push_back(m_startNode);
    m_ObjectiveFound = false;
    CNavMeshManager& l_NavMeshManager = CEngine::GetInstance().GetNavMeshManager();
    m_navmesh = l_NavMeshManager(navmeshFilename);

    while (!m_openNodes.empty() && !m_ObjectiveFound)
    {
        CPathNode * node = *(m_openNodes.begin());
        m_openNodes.erase(m_openNodes.begin());



        if (m_navmesh->GetCenter(node->GetTriangle()) == m_navmesh->GetCenter(m_endNode->GetTriangle())) //centro
            //if (node->GetPos() == m_endNode->GetPos()) //vertices
            BuildPath(node);
        else
        {
            std::vector<CNavMesh::Triangle> lneighbours = m_navmesh->GetNeighbours(node->GetTriangle());

            for (size_t v = 0; v < lneighbours.size(); ++v)
            {
                //printf("Triangle: %d - %d\n", node->GetTriangle()->id, lneighbours[v].id);
                CPathNode * nextNode = nullptr;
                for (std::vector<CPathNode>::iterator pathNodeItr = m_nodes.begin(); pathNodeItr != m_nodes.end(); ++pathNodeItr)
                {
                    if (pathNodeItr->GetTriangle()->id == lneighbours[v].id)
                    {
                        nextNode = &(*pathNodeItr);

                        if (nextNode == m_endNode)
                        {
                            nextNode->SetParent(node);
                            nextNode->SetTotalCost(node->GetTotalCost() + nextNode->GetCost());
                            BuildPath(nextNode);
                            m_ObjectiveFound = true;
                            break;
                        }
                        if (nextNode->GetCost() != -1)
                        {
                            if (node == nextNode)
                                continue;
                            else if (find(m_closedNodes.begin(), m_closedNodes.end(), nextNode) != m_closedNodes.end())
                                continue;
                            else if (find(m_openNodes.begin(), m_openNodes.end(), nextNode) != m_openNodes.end())
                            {
                                if (nextNode->GetTotalCost() >= 0 && nextNode->GetTotalCost() > node->GetTotalCost() + nextNode->GetCost())
                                {
                                    nextNode->SetTotalCost(node->GetTotalCost() + nextNode->GetCost());
                                    nextNode->SetParent(node);
                                }
                            }
                            else
                            {
                                nextNode->SetParent(node);
                                m_openNodes.push_back(nextNode);
                            }
                        }
                    }
                }
            }
            std::vector<CPathNode *>::iterator el = find(m_openNodes.begin(), m_openNodes.end(), node);
            if (el != m_openNodes.end())
                m_openNodes.erase(el);
            m_closedNodes.push_back(node);
        }
    }

    depurarCercanos();
}

Vect3f CPathfinding::NormVector(Vect3f vect)
{
    float length = vect.Length();

    vect.x = vect.x / length;
    vect.z = vect.z / length;

    return vect;
}

void CPathfinding::BuildPath(CPathNode * lastNode)
{
    CNavMeshManager& l_NavMeshManager = CEngine::GetInstance().GetNavMeshManager();
    m_navmesh = l_NavMeshManager(navmeshFilename);

    m_finalPath.clear();
    m_edgesPath.clear();
    //iterate over all node parents to get the full path
    CPathNode * node = lastNode;
    Vect3f edgeDir;
    Vect3f edgePos;
    float edgeLength;
    while (node->GetParent())
    {
        m_finalPath.push_back(node);
        std::vector<CNavMesh::Triangle> lneighbours = m_navmesh->GetNeighbours(node->GetTriangle());

        for (size_t v = 0; v < lneighbours.size(); ++v)
        {
            if (m_navmesh->GetTriangle(lneighbours[v].id) == node->GetParent()->GetTriangle())
            {
                //edgeDir = m_navmesh->GetVertex(node->GetTriangle()->idx2)->position - m_navmesh->GetVertex(node->GetTriangle()->idx1)->position; //vertices
                edgeDir = m_navmesh->GetCenter(node->GetParent()->GetTriangle()) - m_navmesh->GetCenter(node->GetTriangle());
                edgeLength = edgeDir.Length();

                //edgePos = m_navmesh->GetVertex(node->GetTriangle()->idx1)->position + (NormVector(edgeDir) * (edgeLength / 2)); //Vertices
                edgePos = m_navmesh->GetCenter(node->GetTriangle()) + (NormVector(edgeDir) * (edgeLength / 2)); //Centro
                m_edgesPath.push_back(edgePos);
            }
        }
        node = node->GetParent();
    }
}

void CPathfinding::depurarCercanos()
{

    if (m_edgesPath.size()>3)
    {

        std::vector<Vect3f> m_pathnew;
        m_pathnew = m_edgesPath;
        float l_distance = 0.0f;
        for (size_t i = 0; i < m_pathnew.size(); ++i)
        {
            for (size_t j = 0; j < m_pathnew.size(); ++j)
            {
                if (i != j)
                {
                    l_distance = m_pathnew[i].Distance(m_pathnew[j]);
                    if ((l_distance < m_radioSelected) && (m_pathnew[j] != Vect3f(0.0f, -99.0f, 0.0f)))
                        m_pathnew[j] = Vect3f(0.0f, -99.0f, 0.0f);
                }
            }
        }

        m_edgesPathAux.clear();
        for (size_t i = 0; i < m_pathnew.size(); ++i)
        {
            if (m_pathnew[i] != Vect3f(0.0f, -99.0f, 0.0f))
                m_edgesPathAux.push_back(m_pathnew[i]);
        }
        m_edgesPath = m_edgesPathAux;
    }
}


void CPathfinding::DrawDebug()
{
    //draw Polygons path
    /*for (std::vector<CPathNode *>::iterator pathItr = m_finalPath.begin(); pathItr != m_finalPath.end(); ++pathItr)
    {
    //printf("Triangle: %d - %d\n", (*pathItr)->GetTriangle()->id, m_navmesh->GetVertex((*pathItr)->GetTriangle()->id));
    /*        gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.1f);
    MOAIDraw::DrawPolygonFilled((*pathItr)->GetPolygon()->m_vertices);*/
    /*}

    //draw Edges path
    /*std::vector<Vect3f>::iterator edgeItr = m_edgesPath.begin();
    uint16_t edgeIt = 0;
    while (edgeIt < m_edgesPath.size() - 1)
    {
    /*        gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.1f);
    MOAIDraw::DrawLine(m_edgesPath.at(edgeIt), m_edgesPath.at(edgeIt++));*/
    //}
}

void CPathfinding::InitStartPosition(Vect3f position)
{
    m_StartPosition = position;
}

void CPathfinding::InitEndPosition(Vect3f position)
{
    m_EndPosition = position;
    UpdatePath();
}

void CPathfinding::SetStartPosition(Vect3f position)
{
    m_StartPosition = position;
    UpdatePath();
}
void CPathfinding::SetEndPosition(Vect3f position)
{
    m_EndPosition = position;
    UpdatePath();
}

bool CPathfinding::PathfindStep()
{
    // returns true if pathfinding process finished
    return m_ObjectiveFound;
}




