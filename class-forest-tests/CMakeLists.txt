set(LLVM_LINK_COMPONENTS
  support
  )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../
  )

add_extra_unittest(class-forest-tests
  class_forest_tests.cpp
  ../ClassForestVisitor.cpp
  )

target_link_libraries(class-forest-tests
  clangAST
  clangASTMatchers
  clangDynamicASTMatchers
  clangQuery
  clangTooling
  )
