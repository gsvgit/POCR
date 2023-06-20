/* -------------------- ECG.h -- Edge Critical Graph ------------------ */
//
// Created by kisslune on 4/27/23.
//

#ifndef POCR_SVF_ECG_H
#define POCR_SVF_ECG_H

#include "CFLData.h"

namespace SVF
{
class ECG
{
public:
    enum ECGEdgeTy
    {
        Forth = 0,
        Back        // backward edges for tracking cycles
    };

    /// calculators
    u32_t checks;

    struct ECGNode
    {
        NodeID id;
        std::unordered_map<ECGNode*, ECGEdgeTy> successors;
        std::unordered_map<ECGNode*, ECGEdgeTy> predecessors;

        ECGNode(NodeID i) : id(i)
        {}

        inline bool operator==(const ECGNode& rhs) const
        {
            return id == rhs.id;
        }

        inline bool operator<(const ECGNode& rhs) const
        {
            return id < rhs.id;
        }
    };

    typedef std::pair<ECGNode*, ECGNode*> ECGEdge;   // first: src, second: dst

protected:
    std::unordered_map<NodeID, NodeID> nodeToRepMap;
    std::unordered_map<NodeID, ECGNode*> idToNodeMap;
    std::unordered_map<NodeID, NodeBS> succMap;
    std::unordered_map<NodeID, NodeBS> newEdgeMap;

    ECGNode* _backSrc;
    ECGNode* _backDst;
    std::stack<ECGEdge> redBackEdges;

public:
    /// constructor
    ECG() : _backSrc(nullptr),
            _backDst(nullptr),
            checks(0)
    {};

    /// node methods
    //@{
    inline void addNode(NodeID id)
    {
        idToNodeMap[id] = new ECGNode(id);
        setReachable(id, id);
    }

    inline NodeID repNodeID(NodeID id) const
    {
        auto it = nodeToRepMap.find(id);
        if (it == nodeToRepMap.end())
            return id;
        return it->second;
    }

    inline ECGNode* backSrc() const
    { return _backSrc; }

    inline ECGNode* backDst() const
    { return _backDst; }

    inline void resetBackSrc(ECGNode* v)
    { _backSrc = v; }

    inline void resetBackDst(ECGNode* v)
    { _backDst = v; }
    //@}

    /// edge methods
    //@{
    inline ECGNode* getNode(NodeID id)
    {
        auto it = idToNodeMap.find(repNodeID(id));
        assert(it != idToNodeMap.end() && "Node not found!");
        return it->second;
    }

    inline bool hasEdge(NodeID src, NodeID dst)
    {
        ECGNode* srcNode = getNode(src);
        return srcNode->successors.find(getNode(dst)) != srcNode->successors.end();
    }

    void addEdge(NodeID src, NodeID dst, ECGEdgeTy ty)
    {
        ECGNode* srcNode = getNode(src);
        ECGNode* dstNode = getNode(dst);
        addEdge(srcNode, dstNode, ty);
    }

    inline static void addEdge(ECGNode* src, ECGNode* dst, ECGEdgeTy ty)
    {
        src->successors[dst] = ty;
        dst->predecessors[src] = ty;
    }

    void removeEdge(NodeID src, NodeID dst)
    {
        ECGNode* vSrc = getNode(src);
        ECGNode* vDst = getNode(dst);
        removeEdge(vSrc, vDst);
    }

    static void removeEdge(ECGEdge edge)
    {
        ECGNode* vSrc = edge.first;
        ECGNode* vDst = edge.second;
        removeEdge(vSrc, vDst);
    }

    inline static void removeEdge(ECGNode* src, ECGNode* dst)
    {
        src->successors.erase(dst);
        dst->predecessors.erase(src);
    }
    //@}

    /// adjacency list methods
    //@{
    inline bool isReachable(NodeID n, NodeID tgt)
    { return succMap[n].test(tgt); }

    inline void setReachable(NodeID n, NodeID tgt)
    { succMap[n].set(tgt); }

    inline void recordNewEdge(NodeID n, NodeID tgt)
    { newEdgeMap[n].set(tgt); }
    //@}

    /// graph methods
    std::unordered_map<NodeID, NodeBS>& insertForthEdge(NodeID i, NodeID j);
    std::unordered_map<NodeID, NodeBS>& insertBackEdge(NodeID i, NodeID j);
    void searchForward(ECGNode* vi, ECGNode* vj);
    void searchBackward(ECGNode* vi, ECGNode* vj);
    void searchBackwardInCycle(ECGNode* vi, ECGNode* vj);   // no use vj

    /// calculator
    u32_t countReachablePairs();
    void countECGEdges();
};

}

#endif //POCR_SVF_ECG_H
