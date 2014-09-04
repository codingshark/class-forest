#ifndef _CLASS_FOREST_VISITOR_
#define _CLASS_FOREST_VISITOR_

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <string>

using namespace clang;

class ClassForestVisitor : public RecursiveASTVisitor<ClassForestVisitor>, public clang::ASTConsumer
{
	raw_ostream & Out;
	public:
		ClassForestVisitor(raw_ostream & an_out_stream):
			Out(an_out_stream){};
		virtual ~ClassForestVisitor(){};
		
		bool VisitCXXRecordDecl(CXXRecordDecl *D);
		virtual void HandleTranslationUnit(ASTContext &Context);
	
	protected:
		void PrintEdgeColor(AccessSpecifier & AS);
		void PrintEdgeProperty(CXXRecordDecl::base_class_iterator & Base);
		void PrintEndingVertex(CXXRecordDecl::base_class_iterator & Base);
		void PrintStartingVertex(CXXRecordDecl * D);
		void PrintVertex(CXXRecordDecl * D);
		void PrintCXXRecordDeclName(CXXRecordDecl * D);
		void PrintErrorMsg(const std::string & msg);
};

#endif
