set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(Rule16.5_16.6
  switch_clause.cpp
  )

target_link_libraries(Rule16.5_16.6
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
