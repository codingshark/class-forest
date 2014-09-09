
#include <string>
#include <algorithm>

#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include "gtest/gtest.h"

#include "../ClassForestVisitor.h"

class ClassForestAction : public clang::ASTFrontendAction
{
	raw_ostream & Out;
	public:
		ClassForestAction(raw_ostream & an_out_stream):
			Out(an_out_stream){};
		virtual ~ClassForestAction(){};
		
		virtual clang::ASTConsumer *CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile)
		{
			return new ClassForestVisitor(Out);
		}
};

bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

class dbg_string: public std::string
{
	public:
	dbg_string(){};
	dbg_string(const char * str): std::string(str){};
	
	dbg_string & operator = (const std::string & a_str)
	{
		std::string::operator=(a_str);
		return *this;
	}
	
	void trim_spaces()
	{
		std::replace(begin(), end(), '\n', ' ');//ignore new line
		std::replace(begin(), end(), '\t', ' ');//ignore tab
		std::string::iterator new_end = std::unique(begin(), end(), BothAreSpaces);
		erase(new_end, end());
	}
	
};


class ClassForestTest : public ::testing::Test 
{
	protected:
		ClassForestTest():
			m_os(m_buf){};

		bool runToolOnCode(const std::string & str)
		{
			bool rev = clang::tooling::runToolOnCode(new ClassForestAction(m_os), str);
			m_out_str = m_os.str();
			m_out_str.trim_spaces();
			m_buf.clear();
			return rev;
		}
		const char * get_str()
		{
			return m_out_str.c_str();
		}
		
		dbg_string m_out_str;
		std::string m_buf;
		llvm::raw_string_ostream m_os;
};



TEST_F(ClassForestTest, BasicInheritance)
{
	ASSERT_TRUE(runToolOnCode("class X {};"));
	ASSERT_STRCASEEQ("digraph G { X [shape=box ] } ", get_str());
	
	ASSERT_TRUE(runToolOnCode("class A{};class B: public A {};"));
	ASSERT_STRCASEEQ("digraph G { A [shape=box ] B [shape=box ] B -> A [color=red] } ", get_str());
	
	ASSERT_TRUE(runToolOnCode("class A{};class B: public A {};class C: public B {};"));
	ASSERT_STRCASEEQ("digraph G { A [shape=box ] B [shape=box ] B -> A [color=red] C [shape=box ] C -> B [color=red] } ", get_str());
	
	ASSERT_TRUE(runToolOnCode("class D { class E {}; class F{}; };"));
	ASSERT_STRCASEEQ("digraph G { D [shape=box ] E [shape=box ] F [shape=box ] } ", get_str());
	
	ASSERT_TRUE(runToolOnCode("class G {void func(){class H{};};};"));
	ASSERT_STRCASEEQ("digraph G { G [shape=box ] H [shape=box , color=blue] } ", get_str());
	
	ASSERT_TRUE(runToolOnCode("template <typename T>class I {};class J: public I<double>{};"));
	ASSERT_STRCASEEQ("digraph G { I [shape=box ] J [shape=box ] J -> I [color=red] } ", get_str());
	
	ASSERT_TRUE(runToolOnCode("class A{};class B: public A {};class K: protected B, public virtual A{};"));
	ASSERT_STRCASEEQ("digraph G { A [shape=box ] B [shape=box ] B -> A [color=red] K [shape=box ] K -> B [color=green] K -> A [style=dotted, color=red] } ", get_str());
}



int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
