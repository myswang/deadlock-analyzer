/**
 * Contains code adapted with assistance from LLMs (GPT-4o and Claude 3.7 Sonnet) during March 2025.
 *
 * These tools were used across this file to refine logic, improve structure, and support code refactoring.
 * All generated suggestions were carefully reviewed, tested, and modified to ensure full understanding.
 */

#include "dsa/Detector.h"
#include <sstream>

namespace dsa {
    // TODO: add DFS helper

    void Detector::addMutex(const Mutex& id) { mutexes_.insert(id); }

    void Detector::addMutexLock(Function& scope, Mutex& id, LineNum ln) {
        mutexEvents_[scope].push_back(MutexEvent{id, ln, true});
        if (scope == "main") mainMtxEndLn = max(mainMtxEndLn, ln);
    }

    void Detector::addMutexUnlock(Function& scope, Mutex& id, LineNum ln) {
        mutexEvents_[scope].push_back(MutexEvent{id, ln, false});
        if (scope == "main") mainMtxEndLn = max(mainMtxEndLn, ln);
    }

    void Detector::addThreadCreate(const Function& thread, const Function& entryFn, LineNum ln) {
        threadEvents_[thread] = ThreadEvent{entryFn, ln, 0, true, false};
        threadEvents_[entryFn] = ThreadEvent{entryFn, 0, 0, false, true};
        auto& mainCall = threadEvents_["main"];
        mainCall.entryFn = "main";
        mainCall.isThread = true;
        mainCall.isEntryFn = true;
        if (mainCall.create == 0 || ln < mainCall.create) mainCall.create = ln;
    }

    void Detector::addThreadJoin(const Function& caller, const Function& thread, LineNum ln) {
        threadEvents_[thread].join = ln;
        auto& call = threadEvents_[caller];
        call.entryFn = caller;
        call.isThread = true;
        call.isEntryFn = true;
        call.join = max(threadEvents_[caller].join, ln);
    }

    void Detector::addFnCall(Function& caller, const Function& callee) { callGraph_[caller].push_back(callee); }

    void Detector::detect() {
        buildCallGraph();
        analyzeDeadlocks();
        if (!deadlockCycles.empty()) {
            llvm::errs() << "WARNING: Potential deadlock cycles detected!\n";
            for (size_t i = 0; i < getDeadlockCycleStrs().size(); ++i) {
                llvm::errs() << "Cycle " << i + 1 << ": " << getDeadlockCycleStrs()[i] << "\n";
            }
        }
    }

    void Detector::setCFG(const Function& fn, unique_ptr<CFG> CFG) { CFG_[fn] = std::move(CFG); }

    vector<string> Detector::getDeadlockCycleStrs() {
        vector<string> result;
        for (auto& cycle : deadlockCycles) {
            ostringstream out;
            for (auto& mtx : cycle) out << mtx << " -> ";
            out << cycle[0];
            result.push_back(out.str());
        }
        return result;
    }

    void Detector::buildCallGraph() {
        if (!threadEvents_.contains("main")) threadEvents_["main"] = {"main", 0, 0, true, true};
        for (auto& [thread, call] : threadEvents_) {
            if (!call.isThread) continue;
            if (!threadEvents_.contains(call.entryFn)) threadEvents_[call.entryFn] = {call.entryFn, 0, 0, false, false};
            Function start = call.entryFn;
            set<Function> visited;
            function<void(const Function&)> dfs = [&](auto& fn) {
                if (visited.contains(fn)) return;
                visited.insert(fn);
                threadEvents_[fn].threads.insert(thread);
                for (auto& callee : callGraph_[fn]) dfs(callee);
            };
            dfs(start);
        }
    }

    void Detector::buildDependencies() {
        for (auto& [fn, events] : mutexEvents_) {
            set<Mutex> heldMutexes;
            Mutex lastLocked;
            for (auto& event : events) {
                if (event.isLock) {
                    if (heldMutexes.contains(event.mtx)) dependencies_[event.mtx][event.mtx].insert(fn);
                    if (lastLocked != event.mtx) dependencies_[lastLocked][event.mtx].insert(fn);
                    for (auto& mtx : heldMutexes) dependencies_[mtx][event.mtx].insert(fn);
                    lastLocked = event.mtx;
                    heldMutexes.insert(event.mtx);
                } else {
                    heldMutexes.erase(event.mtx);
                }
            }
        }
    }

    set<Detector::Function> Detector::getThreads(const Function& fn) {
        set<Function> result;
        if (threadEvents_.contains(fn)) {
            auto& call = threadEvents_.at(fn);
            result.insert(call.threads.begin(), call.threads.end());
        }
        return result;
    }

    void Detector::analyzeDeadlocks() {
        buildDependencies();
        set<set<Mutex>> seen;
        unsigned cycleId = 1;
        for (auto& start : dependencies_ | views::keys) {
            set<Mutex> visited;
            vector<Mutex> path;
            function<void(const Mutex&)> dfs = [&](auto& current) {
                visited.insert(current);
                path.push_back(current);
                for (auto& [next, _] : dependencies_[current]) {
                    auto cycleStart = find(path.begin(), path.end(), next);
                    if (cycleStart != path.end()) {
                        Cycle cycle(cycleStart, path.end());
                        if (set canonical(cycle.begin(), cycle.end());
                            seen.insert(canonical).second && canDeadlock(cycle)) {
                            deadlockCycles.insert(cycle);
                            processDeadlockCycle(cycle, cycleId++);
                        }
                    } else if (!visited.contains(next)) {
                        dfs(next);
                    }
                }
                path.pop_back();
            };
            dfs(start);
        }
    }

    void Detector::processDeadlockCycle(const Cycle& cycle, unsigned cycleId) {
        for (size_t i = 0; i < cycle.size(); ++i) {
            for (auto &from = cycle[i], &to = cycle[(i + 1) % cycle.size()]; auto& fn : dependencies_[from][to]) {
                auto threads = getThreads(fn);
                map<Function, unsigned> orderCounter;
                for (auto& thread : threads) {
                    for (auto& [mtx, ln, isLock] : mutexEvents_[fn]) {
                        if (!isLock || mtx != from) continue;
                        unsigned order = ++orderCounter[thread];
                        Function entryFn = threadEvents_[thread].entryFn;
                        deadlockEdges_.push_back(DeadlockEdge{thread, from, fn, ln, true, cycleId, order++, entryFn});
                        bool firstFound = false;
                        for (auto& [mtx2, ln2, isLock2] : mutexEvents_[fn]) {
                            if (isLock2 && mtx2 == to) {
                                auto edge = DeadlockEdge{thread, mtx2, fn, ln2, false, cycleId, order, entryFn};
                                if (from == to) {
                                    if (firstFound) {
                                        deadlockEdges_.push_back(edge);
                                        break;
                                    }
                                    firstFound = true;
                                } else {
                                    deadlockEdges_.push_back(edge);
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
    }

    bool Detector::canDeadlock(Cycle& cycle) {
        size_t n = cycle.size();
        if (n == 1) return true;
        for (size_t i = 0; i < n; ++i) {
            auto& setA = dependencies_[cycle[i]][cycle[(i + 1) % n]];
            for (size_t j = i + 1; j < n; ++j) {
                auto& setB = dependencies_[cycle[j]][cycle[(j + 1) % n]];
                for (auto& a : setA) {
                    for (auto& b : setB) {
                        if (canRunConcurrently(a, b)) return true;
                    }
                }
            }
        }
        return false;
    }

    bool Detector::canRunConcurrently(const Function& a, const Function& b) {
        if (a == b) return false;
        for (auto& tA : getThreads(a)) {
            for (auto& tB : getThreads(b)) {
                if (tA == tB) continue;
                auto &callA = threadEvents_.at(tA), &callB = threadEvents_.at(tB);
                if (mainMtxEndLn > 0) {
                    if (tA == "main" && mainMtxEndLn < callB.create || tB == "main" && mainMtxEndLn < callA.create) {
                        continue;
                    }
                }
                if (callA.join < callB.create || callB.join < callA.create) continue;
                return true;
            }
        }
        return false;
    }

    vector<string> Detector::stringifyBlock(const CFGBlock* block) {
        vector<string> result;
        for (auto& elem : *block) {
            if (auto cs = elem.getAs<CFGStmt>()) {
                ostringstream out;
                const Stmt* stmt = cs->getStmt();
                string code;
                llvm::raw_string_ostream rso(code);
                stmt->printPretty(rso, nullptr, PrintingPolicy(LangOptions()));
                rso.flush();
                if (code == "0" || code.empty() || code.find("__null") != string::npos) continue;
                out << code;
                result.push_back(out.str());
            }
        }
        return result;
    }

    string Detector::getBlockType(const CFGBlock* block) {
        const Stmt* term = block->getTerminatorStmt();
        if (!term) return "normal";
        if (isa<IfStmt>(term) || isa<SwitchStmt>(term)) return "branch";
        if (isa<ForStmt>(term) || isa<WhileStmt>(term) || isa<DoStmt>(term)) return "loop";
        if (isa<ReturnStmt>(term)) return "exit";
        return "normal";
    }

    bool Detector::shouldSkipFunction(const Function& fn) { return fn.empty() || fn.starts_with("_"); }

    json Detector::toCfgJson() {
        json j;
        // Add control flow edges
        for (auto& [func, cfg] : CFG_) {
            if (func.starts_with("_")) continue;
            for (auto* block : cfg->nodes()) {
                if (!block || block->getBlockID() == 0) continue;
                auto blockCode = stringifyBlock(block);
                if (blockCode.empty()) continue;
                string nodeId = func + "_B" + to_string(block->getBlockID());
                json codeArr = json::array();
                for (auto& code : blockCode) codeArr.push_back(code);
                j["nodes"].push_back(
                    {{"id", nodeId}, {"function", func}, {"code", codeArr}, {"type", getBlockType(block)}});
                int edgeIndex = 0;
                for (auto& next : block->succs()) {
                    if (!next || next->getBlockID() == 0) continue;
                    string targetId = func + "_B" + to_string(next->getBlockID());
                    json edge = {{"source", nodeId}, {"target", targetId}};
                    if (auto* term = block->getTerminatorStmt()) {
                        if (isa<IfStmt>(term) || isa<ForStmt>(term) || isa<WhileStmt>(term) || isa<DoStmt>(term)) {
                            edge["type"] = edgeIndex == 0 ? "true" : "false";
                        }
                    } else {
                        edge["type"] = "normal";
                    }
                    j["edges"].push_back(edge);
                    edgeIndex++;
                }
            }
        }
        // Add function call edges
        for (auto& [caller, callees] : callGraph_) {
            if (!CFG_.contains(caller)) continue;
            for (auto& callee : callees) {
                if (shouldSkipFunction(caller) || shouldSkipFunction(callee)) continue;
                if (!CFG_.contains(callee)) continue;
                CFG *callerCfg = CFG_[caller].get(), *calleeCfg = CFG_[callee].get();
                CFGBlock *callerLast = nullptr, *calleeFirst = nullptr;
                for (auto& block : *callerCfg) {
                    if (!stringifyBlock(block).empty()) {
                        callerLast = block;
                        break;
                    }
                }
                for (auto& block : *calleeCfg) {
                    if (!stringifyBlock(block).empty()) {
                        calleeFirst = block;
                        break;
                    }
                }
                if (callerLast && calleeFirst) {
                    string fromId = caller + "_B" + to_string(callerLast->getBlockID());
                    string toId = callee + "_B" + to_string(calleeFirst->getBlockID());
                    j["edges"].push_back({{"source", fromId}, {"target", toId}, {"type", "call"}});
                }
            }
        }
        return j;
    }

    json Detector::toJson(const string& filename, const string& source) {
        json j;
        bool hasDeadlocks = !deadlockCycles.empty();
        j["deadlocks"] = hasDeadlocks;
        j["nodes"] = json::array();
        j["edges"] = json::array();
        for (auto& mtx : mutexes_) j["nodes"].push_back({{"id", mtx}, {"type", "resource"}});
        for (auto& [fn, call] : threadEvents_) {
            if (call.isThread) j["nodes"].push_back({{"id", fn}, {"type", "process"}});
        }
        if (hasDeadlocks) {
            j["cycles"] = json::array();
            for (size_t i = 0; i < getDeadlockCycleStrs().size(); ++i) j["cycles"].push_back(getDeadlockCycleStrs()[i]);
            for (auto& [thread, mtx, fn, ln, isHold, cycleId, order, entryFn] : deadlockEdges_) {
                j["edges"].push_back({{"source", isHold ? mtx : thread},
                                      {"target", isHold ? thread : mtx},
                                      {"type", isHold ? "holds" : "waiting"},
                                      {"function", fn},
                                      {"line", ln},
                                      {"cycle", cycleId},
                                      {"order", order},
                                      {"entryPt", entryFn}});
            }
        } else {
            map<Function, unsigned> orderCounter;
            for (auto& [thread, call] : threadEvents_) {
                if (!call.isThread) continue;
                set<Function> visited;
                function<void(const Function&)> dfs = [&](auto& fn) {
                    if (visited.contains(fn)) return;
                    visited.insert(fn);
                    for (auto& [mtx, ln, isLock] : mutexEvents_[fn]) {
                        if (!isLock) continue;
                        j["edges"].push_back({{"source", mtx},
                                              {"target", thread},
                                              {"type", "holds"},
                                              {"function", fn},
                                              {"line", ln},
                                              {"order", ++orderCounter[thread]},
                                              {"entryPt", call.entryFn}});
                    }
                    for (auto& callee : callGraph_[fn]) dfs(callee);
                };
                dfs(call.entryFn);
            }
        }
        j["CFG"] = toCfgJson();
        j["source"] = source;
        j["filename"] = filename;
        return j;
    }
}
