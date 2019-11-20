set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(Rule19.2
  union.cpp
  )

target_link_libraries(Rule19.2
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
