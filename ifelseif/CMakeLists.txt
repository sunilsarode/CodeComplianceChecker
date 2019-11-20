set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(ifelseif
  ifelseif.cpp
  )

target_link_libraries(ifelseif
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
