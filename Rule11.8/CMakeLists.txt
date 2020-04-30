set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(Rule11.8
  pointer_casting.cpp
  )

target_link_libraries(Rule11.8
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
