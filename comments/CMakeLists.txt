set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(comments
  comments.cpp
  )

target_link_libraries(comments
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
