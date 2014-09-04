set(LLVM_LINK_COMPONENTS
  Support
  )

add_extra_unittest(class-forest-tests
  class_forest_tests.cpp
  )

target_link_libraries(class-forest-tests
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  )
