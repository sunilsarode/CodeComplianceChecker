set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(Rule16.3
  break.cpp
  )

target_link_libraries(Rule16.3
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
