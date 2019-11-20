# CodeComplianceChecker
MISRA C RULES
 
To get LLVM and Clang  follow the insrtuctions.

1. make folder by your choice name . example LLVM
2. cd LLVM 
3. git clone https://github.com/llvm/llvm-project.git .
4 go to llvm-project folder and do git checkout release/8.x
5. beside llvm-project folder ,create build folder and cd build ,
6 fire cmake as follows
cmake -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_ENABLE_PROJECTS='clang;clang-tools-extra' ../llvm-project/llvm/
7 fire make -j4

for lib tooling
cmake -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_ENABLE_PROJECTS='clang;clang-tools-extra' -DCMAKE_EXPORT_COMPILE_COMMANDS=True ../llvm-project/llvm
