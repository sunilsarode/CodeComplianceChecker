set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(Rule16.2
  switch_compound.cpp
  )

target_link_libraries(Rule16.2
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
