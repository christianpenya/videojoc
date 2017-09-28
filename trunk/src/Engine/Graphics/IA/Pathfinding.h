#ifndef __PATHFINDING_H__
#define __PATHFINDING_H__

#include "NavMesh.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

class CNavMesh;
class CPathNode
{
public:
    CPathNode(CNavMesh::Triangle * pol, float cost, float estimCost, CPathNode * parent = nullptr) :
        m_triangle(pol), m_totalCost(cost + estimCost), m_cost(cost + estimCost),
        m_estimatedCost(estimCost), m_parentNode(parent) {}

    Vect3f GetPos() const
    {
        return m_centre;
    }

    void SetCentrePos(Vect3f &pos)
    {
        m_centre = pos;
    }

    float GetTotalCost() const
    {
        return m_totalCost;
    }

    float GetCost() const
    {
        return m_cost;
    }

    float GetEstimated() const
    {
        return m_estimatedCost;
    }

    CPathNode * GetParent() const
    {
        return m_parentNode;
    }

    CNavMesh::Triangle * GetTriangle() const
    {
        return m_triangle;
    }

    void SetTotalCost(float newCost)
    {
        m_totalCost = newCost;
    }
    void SetCost(float newCost)
    {
        m_cost = newCost;
    }
    void SetEstimatedCost(float newCost)
    {
        m_estimatedCost = newCost;
    }
    void SetParent(CPathNode * newParent)
    {
        m_parentNode = newParent;
    }
private:
    CNavMesh::Triangle * m_triangle;
    Vect3f m_centre;

    //if any cost is -1 -> it's an obstacle
    float m_totalCost; //accumulated cost of path
    float m_cost; //own cost, independent of path
    float m_estimatedCost;
    CPathNode * m_parentNode;
};

class CPathfinding
{
public:
    static CPathfinding &Instance(Vect3f startPos, Vect3f endPos, std::string navMeshFile);
    CPathfinding();
    CPathfinding(Vect3f startPos, Vect3f endPos, std::string navMeshFile);
    ~CPathfinding();

    virtual void DrawDebug();
    const std::vector<Vect3f> &GetPath() const;
    bool PathfindStep();
    lua_State * GetLuaState() const
    {
        return mLS;
    }
    void RegisterLUAFunctions();

private:
    lua_State *mLS;
    void UpdatePath();
    void BuildPath(CPathNode * lastNode);

    static CPathfinding * m_pathfinding;
    CPathNode * NearestNode(Vect3f &point);

    Vect3f NormVector(Vect3f vect);
    void InitStartPosition(Vect3f position);
    void InitEndPosition(Vect3f position);
    void SetStartPosition(Vect3f position);
    void SetEndPosition(Vect3f position);
    Vect3f m_StartPosition;
    Vect3f m_EndPosition;

    CNavMesh * m_navmesh;
    CPathNode * m_startNode;
    CPathNode * m_endNode;
    std::string navmeshFilename;
    std::vector<CPathNode> m_nodes;
    std::vector<CPathNode*> m_openNodes;
    std::vector<CPathNode*> m_closedNodes;
    std::vector<CPathNode*> m_finalPath;
    std::vector<Vect3f> m_edgesPath; //edges points to calc pathfollowing
    bool m_ObjectiveFound = false;
    float m_radioSelected = 2.0f;
    std::vector<Vect3f> m_edgesPathAux; //edges points to calc pathfollowing
    void depurarCercanos();

};


#endif


