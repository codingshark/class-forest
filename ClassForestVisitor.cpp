#include "ClassForestVisitor.h"

void ClassForestVisitor::HandleTranslationUnit(ASTContext &Context)
{
	Out<<"digraph G {\n";
	TraverseDecl(Context.getTranslationUnitDecl());
	Out<<"}\n";
}

bool ClassForestVisitor::VisitCXXRecordDecl(CXXRecordDecl *D)
{
	PrintVertex(D);
	if (D->isCompleteDefinition())
	{
		if (D->getNumBases())
		{
			for (CXXRecordDecl::base_class_iterator Base = D->bases_begin(),
				BaseEnd = D->bases_end(); Base != BaseEnd; ++Base)
			{
				PrintStartingVertex(D);
				Out<<" -> ";
				PrintEndingVertex(Base);
				PrintEdgeProperty(Base);
			}
		}
	}
	return true;
}

void ClassForestVisitor::PrintEdgeColor(AccessSpecifier & AS)
{
	switch(AS)
	{
		case AS_none:      llvm_unreachable("No access specifier!");
		case AS_public:    Out << "color=red"; break;
		case AS_protected: Out << "color=green"; break;
		case AS_private:   Out << "color=blue"; break;
	}
}

void ClassForestVisitor::PrintEdgeProperty(CXXRecordDecl::base_class_iterator & Base)
{
	Out<<"\t";
	
	AccessSpecifier AS = Base->getAccessSpecifierAsWritten();
	if (Base->isVirtual() || AS != AS_none)
	{
		Out<<"[";
		if (Base->isVirtual())
		{
			Out << "style=dotted";
			
			if (AS != AS_none)
			{
				Out << ", ";
				PrintEdgeColor(AS);
			}
		}else
		{
			if (AS != AS_none)
			{
				PrintEdgeColor(AS);
			}
		}
		Out<<"]";
	}
	Out<<"\n";
//	if (Base->isPackExpansion())
//		Out << "...";
}

void ClassForestVisitor::PrintEndingVertex(CXXRecordDecl::base_class_iterator & Base)
{
	SplitQualType Split = Base->getType().split();

	switch (Split.Ty->getTypeClass())
	{
		case Type::TemplateSpecialization:
		{
			TemplateDecl * tmp_decl = cast<TemplateSpecializationType>(Split.Ty)->getTemplateName().getAsTemplateDecl();
			if(tmp_decl != NULL)
			{
				IdentifierInfo * id_info = tmp_decl->getIdentifier();
				if(id_info != NULL)
				{
					Out<<id_info->getName();
				}else
				{
					PrintErrorMsg("Cannot get Identifier for CXXRecordDecl::base_class_iterator Base");
				}
			}else
			{
				PrintErrorMsg("Cannot get TemplateDecl for CXXRecordDecl::base_class_iterator Base");
			}
		}
		break;
		case Type::Record:
		{
			IdentifierInfo * id_info = cast<RecordType>(Split.Ty)->getDecl()->getIdentifier();
			if(id_info != NULL)
			{
				Out<<id_info->getName();
			}else
			{
				PrintErrorMsg("Cannot get Identifier for CXXRecordDecl::base_class_iterator Base");
			}
		}
		break;
		default:
			PrintErrorMsg("Not handled CXXRecordDecl::base_class_iterator Base");
		break;
	}
	//Out << Base->getType().getAsString(D->getASTContext().getPrintingPolicy());
}

void ClassForestVisitor::PrintStartingVertex(CXXRecordDecl * D)
{
	PrintCXXRecordDeclName(D);
}

void ClassForestVisitor::PrintVertex(CXXRecordDecl * D)
{
	PrintCXXRecordDeclName(D);
	
	Out<<"\t[shape=box ";
	FunctionDecl * funcDecl = D->isLocalClass();
	if(funcDecl != NULL)
	{
		Out<<", color=blue";
	}
	Out<<"]\n";
}

void ClassForestVisitor::PrintCXXRecordDeclName(CXXRecordDecl * D)
{
	if (D->getIdentifier() != NULL)
	{
		Out<<D->getIdentifier()->getName();
	}else
	{
		PrintErrorMsg("Cannot get Identifier for CXXRecordDecl");
		Out<<"CXXRecordDecl_"<<D;
	}
}

void ClassForestVisitor::PrintErrorMsg(const std::string & msg)
{
	Out<<"// ERROR "<<msg<<"\n";
}

