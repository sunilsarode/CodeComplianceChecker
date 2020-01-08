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

Below is the path of clang-tools-extra ,you can  igonre /home/sunil/  or it is your own path in your system.

/home/sunil/LLVM/llvm-project/clang-tools-extra 

Inside the clang-tools-extra you have to write your tool.

In order to run the tool 
/LLVM/build/bin$ ./Rule16.2 /home/sunil/LLVMPASSTEST/Rule16.2/test1.c

Note: whenever you change your code in your project, you have to build the llvm tools again using make -j4 command.



