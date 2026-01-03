#ifndef DSA_DETECTOR_H
#define DSA_DETECTOR_H

#include "json.hpp"
#include <clang/Analysis/CFG.h>
#include <set>

namespace dsa {
    using namespace std;
    using namespace clang;
    using nlohmann::json;

    class Detector {
        using Mutex = string;
        using LineNum = unsigned;
        using Function = string;
        using Cycle = vector<Mutex>;

        struct MutexEvent {
            Mutex mtx;
            LineNum ln;
            bool isLock;
        };

        struct ThreadEvent {
            Function entryFn;
            LineNum create;
            LineNum join;
            bool isThread;
            bool isEntryFn;
            set<Function> threads;
        };

        struct DeadlockEdge {
            Function thread;
            Mutex mtx;
            Function fn;
            LineNum ln;
            bool isHold;
            unsigned cycleId;
            unsigned order;
            Function entryPt;
        };

        set<Mutex> mutexes_;
        map<Function, vector<MutexEvent>> mutexEvents_;
        map<Mutex, map<Mutex, set<Function>>> dependencies_;
        map<Function, ThreadEvent> threadEvents_;
        set<Cycle> deadlockCycles;
        vector<DeadlockEdge> deadlockEdges_;
        LineNum mainMtxEndLn = 0;
        map<Function, unique_ptr<CFG>> CFG_;
        map<Function, vector<Function>> callGraph_;

        void buildCallGraph();
        void analyzeDeadlocks();
        void buildDependencies();
        bool canDeadlock(Cycle& cycle);
        bool canRunConcurrently(const Function& a, const Function& b);
        void processDeadlockCycle(const Cycle& cycle, unsigned cycleId);
        set<Function> getThreads(const Function& fn);
        vector<string> getDeadlockCycleStrs();
        static bool shouldSkipFunction(const Function& fn);
        static vector<string> stringifyBlock(const CFGBlock* block);
        static string getBlockType(const CFGBlock* block);
        json toCfgJson();

    public:
        void addMutex(const Mutex& id);
        void addMutexLock(Function& scope, Mutex& id, LineNum ln);
        void addMutexUnlock(Function& scope, Mutex& id, LineNum ln);
        void addFnCall(Function& caller, const Function& callee);
        void addThreadCreate(const Function& thread, const Function& entryFn, LineNum ln);
        void addThreadJoin(const Function& caller, const Function& thread, LineNum ln);
        void detect();
        json toJson(const string& filename, const string& source);
        void setCFG(const Function& fn, unique_ptr<CFG> CFG);
    };
}

#endif
