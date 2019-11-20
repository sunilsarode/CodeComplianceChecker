set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(static_arr_size
  static_arr_size.cpp
  )

target_link_libraries(static_arr_size
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangTooling
  clangToolingRefactor
  )
