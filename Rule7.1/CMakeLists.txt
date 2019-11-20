set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(Rule7.1
  octal.cpp
  )

target_link_libraries(Rule7.1
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
