set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(goto
  goto.cpp
  )

target_link_libraries(goto
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
