set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(Rule8.12
  enum_pass.cpp
  )

target_link_libraries(Rule8.12
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
