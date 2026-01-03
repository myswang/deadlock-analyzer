#include "dsa/Visitor.h"
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

using namespace clang;
using namespace clang::tooling;
using namespace llvm::cl;

OptionCategory toolCategory("Deadlock Static Analyzer (DSA) Options");
opt<std::string> outputFile("o", desc("JSON output file"), value_desc("filename"), cat(toolCategory));

class Consumer final : public ASTConsumer {
    std::string outputPath_;

    void HandleTranslationUnit(ASTContext& ctx) override {
        dsa::Visitor visitor(&ctx);
        visitor.visitAST(outputPath_);
    }

public:
    explicit Consumer(std::string& path) : outputPath_(path) {}
};

class DSAFrontendAction final : public ASTFrontendAction {
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance&, StringRef) override {
        return std::make_unique<Consumer>(outputFile);
    }
};

int main(int argc, const char** argv) {
    auto argsParser = CommonOptionsParser::create(argc, argv, toolCategory);
    if (!argsParser) {
        llvm::errs() << argsParser.takeError();
        return -1;
    }

    ClangTool tool(argsParser->getCompilations(), argsParser->getSourcePathList());
    return tool.run(newFrontendActionFactory<DSAFrontendAction>().get());
}
