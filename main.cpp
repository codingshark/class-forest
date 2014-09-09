#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"

#include "ClassForestVisitor.h"

using namespace clang::tooling;
using namespace llvm;
using namespace clang;


class ClassForestAction : public clang::ASTFrontendAction
{
	public:
		virtual clang::ASTConsumer *CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile)
		{
			return new ClassForestVisitor(llvm::outs());
		}
};

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory ClassForestCategory("class-forest options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nExample:\n\t$class-forest file.cpp --\n");

int main(int argc, const char **argv)
{
	CommonOptionsParser OptionsParser(argc, argv, ClassForestCategory);
	ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
	return Tool.run(newFrontendActionFactory<ClassForestAction>().get());
}

