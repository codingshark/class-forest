set(LLVM_LINK_COMPONENTS
	Support
)

add_clang_executable(class-forest
	main.cpp
	ClassForestVisitor.h
	ClassForestVisitor.cpp
)

target_link_libraries(class-forest
	clangAST
	clangASTMatchers
	clangBasic
	clangFrontend
	clangTooling
)

#add_subdirectory(class-forest-tests)
