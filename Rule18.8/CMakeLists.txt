set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(Rule18.8
  VLA.cpp
  )

target_link_libraries(Rule18.8
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
