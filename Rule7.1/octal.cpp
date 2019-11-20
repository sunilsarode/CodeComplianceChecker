#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Driver/Options.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"

#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"

#include "clang/Rewrite/Frontend/Rewriters.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"

#include <bits/stdc++.h>

using namespace clang;
using namespace clang::tooling;

class FindOctalVisitor : public RecursiveASTVisitor<FindOctalVisitor>
{
public:
  explicit FindOctalVisitor(ASTContext *Context)
      : Context(Context) {}

  bool VisitIntegerLiteral(IntegerLiteral *intLiteral)
  {
    // Source Location of current int literal
    SourceLocation loc = intLiteral->getLocation();
    // Source manager associated
    SourceManager &SM = Context->getSourceManager();
    // Get first char and second char of the integer literal (getCharacterData returns the whole code from the SourceLocation until the end of code)
    char first_char = SM.getCharacterData(loc, nullptr)[0];
    char second_char = SM.getCharacterData(loc, nullptr)[1];
    // If first character is 0
    if (first_char == '0')
    {
      // If second character is also a number
      if (second_char == '1' || second_char == '2' || second_char == '3' || second_char == '4' || second_char == '5' || second_char == '6' || second_char == '7' || second_char == '8' || second_char == '9')
      {
        
        FullSourceLoc FullLocation = Context->getFullLoc(intLiteral->getBeginLoc());
                             if (FullLocation.isValid()&&!FullLocation.isInSystemHeader())
                                  llvm::outs() << " Octal constants shall not be used  "
                                         << FullLocation.getSpellingLineNumber() << ":"
                                              << FullLocation.getSpellingColumnNumber() << "\n";
      }
    }
    return true;
  }

private:
  ASTContext *Context;
};

class FindNamedCallConsumer : public clang::ASTConsumer
{
public:
  explicit FindNamedCallConsumer(ASTContext *Context)
      : Visitor(Context) {}

  virtual void HandleTranslationUnit(clang::ASTContext &Context)
  {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  FindOctalVisitor Visitor;
};

class FindNamedCallAction : public clang::ASTFrontendAction
{
public:
  FindNamedCallAction() {}

  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
      clang::CompilerInstance &Compiler, llvm::StringRef InFile)
  {

    return std::unique_ptr<clang::ASTConsumer>(
        new FindNamedCallConsumer(&Compiler.getASTContext()));
  }
};

static llvm::cl::OptionCategory MyToolCategory("my-tool options");

int main(int argc, const char **argv)
{

  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  // run the Clang Tool, creating a new FrontendAction (explained below)
  int result = Tool.run(newFrontendActionFactory<FindNamedCallAction>().get());
  return result;
}
