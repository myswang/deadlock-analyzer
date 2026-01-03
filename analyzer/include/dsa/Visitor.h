#ifndef DSA_VISITOR_H
#define DSA_VISITOR_H

#include "dsa/Detector.h"
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>

namespace dsa {
    class Visitor : public RecursiveASTVisitor<Visitor> {
        Detector detector_;
        ASTContext& ctx_;
        string currentFn_;
        static optional<string> isPthreadMutexLock(CallExpr* call);
        static optional<string> isPthreadMutexUnlock(CallExpr* call);
        static optional<string> isPthreadCreate(CallExpr* call);
        static optional<string> isPthreadJoin(CallExpr* call);
        static unsigned getLineNum(SourceLocation loc, ASTContext& ctx);
        static string resolveExprName(Expr* expr);

    public:
        explicit Visitor(ASTContext* ctx) : ctx_(*ctx) {}

        void visitAST(const string& outputPath);
        void writeJson(const string& outputPath);
        void setCurrentFunction(const string& fn);
        bool VisitVarDecl(VarDecl* decl);
        bool VisitCallExpr(CallExpr* call);
    };
}

#endif
