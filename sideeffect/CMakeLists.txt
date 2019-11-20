set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(sideeffct
  sideeffect.cpp
  )

target_link_libraries(sideeffct
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
