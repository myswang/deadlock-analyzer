/**
 * Contains code adapted with assistance from LLMs (GPT-4o and Claude 3.7 Sonnet) during March 2025.
 *
 * These tools were used across this file to refine logic, improve structure, and support code refactoring.
 * All generated suggestions were carefully reviewed, tested, and modified to ensure full understanding.
 */

#include "dsa/Visitor.h"
#include <fstream>

namespace dsa {
    void Visitor::visitAST(const string& outputPath) {
        for (auto* decl : ctx_.getTranslationUnitDecl()->decls()) {
            if (auto* var = dyn_cast<VarDecl>(decl)) VisitVarDecl(var);

            if (auto* fn = dyn_cast<FunctionDecl>(decl)) {
                if (fn->hasBody()) {
                    setCurrentFunction(fn->getNameAsString());
                    TraverseDecl(fn);
                    if (unique_ptr<CFG> cfg = CFG::buildCFG(fn, fn->getBody(), &ctx_, CFG::BuildOptions())) {
                        detector_.setCFG(fn->getNameAsString(), std::move(cfg));
                    }
                }
            }
        }
        detector_.detect();
        writeJson(outputPath);
    }

    void Visitor::writeJson(const string& outputPath) {
        const SourceManager& sm = ctx_.getSourceManager();
        string inputPath = sm.getFileEntryForID(sm.getMainFileID())->tryGetRealPathName().str();
        ifstream inFile(inputPath);
        ostringstream ss;
        ss << inFile.rdbuf();

        json result = detector_.toJson(llvm::sys::path::filename(inputPath).str(), ss.str());

        if (outputPath.empty()) {
            llvm::outs() << "Graph data:\n" << result.dump(4) << "\n";
            return;
        }

        ofstream outFile(outputPath);
        if (!outFile) {
            llvm::errs() << "Failed to open output file: " << filesystem::absolute(outputPath) << "\n";
            return;
        }

        outFile << result.dump(4);
        llvm::outs() << "Graph data written to " << filesystem::absolute(outputPath) << "\n";
    }

    void Visitor::setCurrentFunction(const string& fn) { currentFn_ = fn; }

    bool Visitor::VisitVarDecl(VarDecl* decl) {
        if (decl->getType().getAsString().find("pthread_mutex_t") != string::npos) {
            detector_.addMutex(decl->getNameAsString());
        }
        return true;
    }

    bool Visitor::VisitCallExpr(CallExpr* call) {
        if (auto mtxLocked = isPthreadMutexLock(call)) {
            detector_.addMutexLock(currentFn_, *mtxLocked, getLineNum(call->getExprLoc(), ctx_));
        } else if (auto mtxUnlocked = isPthreadMutexUnlock(call)) {
            detector_.addMutexUnlock(currentFn_, *mtxUnlocked, getLineNum(call->getExprLoc(), ctx_));
        } else if (auto entryFn = isPthreadCreate(call)) {
            detector_.addThreadCreate(resolveExprName(call->getArg(0)), *entryFn, getLineNum(call->getExprLoc(), ctx_));
        } else if (auto thread = isPthreadJoin(call))
            detector_.addThreadJoin(currentFn_, *thread, getLineNum(call->getExprLoc(), ctx_));
        if (auto* callee = call->getDirectCallee()) detector_.addFnCall(currentFn_, callee->getNameAsString());

        return true;
    }

    optional<string> Visitor::isPthreadMutexLock(CallExpr* call) {
        if (call->getDirectCallee()->getNameAsString() == "pthread_mutex_lock") {
            return resolveExprName(call->getArg(0));
        }
        return nullopt;
    }

    optional<string> Visitor::isPthreadMutexUnlock(CallExpr* call) {
        if (call->getDirectCallee()->getNameAsString() == "pthread_mutex_unlock") {
            return resolveExprName(call->getArg(0));
        }
        return nullopt;
    }

    optional<string> Visitor::isPthreadJoin(CallExpr* call) {
        if (call->getDirectCallee()->getNameAsString() != "pthread_join") return nullopt;
        if (auto* ref = dyn_cast<DeclRefExpr>(call->getArg(0)->IgnoreParenImpCasts())) {
            return ref->getNameInfo().getAsString();
        }

        return nullopt;
    }

    optional<string> Visitor::isPthreadCreate(CallExpr* call) {
        if (call->getDirectCallee()->getNameAsString() != "pthread_create") return nullopt;
        if (auto* ref = dyn_cast<DeclRefExpr>(call->getArg(2)->IgnoreParenImpCasts())) {
            if (auto* decl = dyn_cast<FunctionDecl>(ref->getDecl())) return decl->getNameAsString();
        }
        return nullopt;
    }

    string Visitor::resolveExprName(Expr* expr) {
        if (auto* ref = dyn_cast<DeclRefExpr>(expr)) return ref->getNameInfo().getName().getAsString();
        if (auto* unary = dyn_cast<UnaryOperator>(expr); unary->getOpcode() == UO_AddrOf) {
            return resolveExprName(unary->getSubExpr());
        }
        return expr->getStmtClassName();
    }

    unsigned Visitor::getLineNum(SourceLocation loc, ASTContext& ctx) {
        return ctx.getSourceManager().getSpellingLineNumber(loc);
    }
}
