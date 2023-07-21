//===- AAStat.h -- Program Expression Graph-----------------------------//

/*
 * AAStat.h
 *
 *  Created on: Aug 1, 2020
 *      Author: Yuxiang Lei
 */

#ifndef AASTAT_H
#define AASTAT_H

#include "SVF-LLVM/BasicTypes.h"
#include "SVFIR/SVFType.h"
#include <iostream>
#include <map>
#include <string>

namespace SVF
{

class AliasAnalysis;

class PEG;

/*!
 * Statistics of Andersen's analysis
 */
class AAStat
{
public:
    typedef llvm::DenseMap<const char*, u64_t> NUMStatMap;
    typedef llvm::DenseMap<const char*, double> TIMEStatMap;

    /// num counters
    u32_t numOfIteration;
    u32_t checks;
    u32_t numOfSumEdges;
    u32_t numOfSEdges;
    u32_t numOfNodes;
    u32_t numOfEdges;

    /// time counters
    double timeOfSolving;
    double startTime;
    double endTime;
    double sccTime;
    double gfTime;
    double interDyckTime;
    double gsTime;

private:
    AliasAnalysis* aa;

    NUMStatMap PTNumStatMap;
    TIMEStatMap timeStatMap;

    /// Memory usage, in KB
    u32_t _vmrssUsageBefore;
    u32_t _vmrssUsageAfter;
    u32_t _vmsizeUsageBefore;
    u32_t _vmsizeUsageAfter;

public:
    AAStat(AliasAnalysis* p) : aa(p),
                               numOfIteration(0),
                               checks(0),
                               numOfSEdges(0),
                               numOfSumEdges(0),
                               numOfNodes(0),
                               numOfEdges(0),
                               timeOfSolving(0)
    {
        startClk();
    };

    virtual ~AAStat()
    {}

    virtual inline void startClk()
    { startTime = CLOCK_IN_MS(); }

    virtual inline void endClk()
    { endTime = CLOCK_IN_MS(); }

    static inline double getClk()
    { return CLOCK_IN_MS(); }

    void setMemUsageBefore();
    void setMemUsageAfter();

    virtual void performStat();
    void pegStat();
    void printStat(std::string str = "");
};
}

#endif
