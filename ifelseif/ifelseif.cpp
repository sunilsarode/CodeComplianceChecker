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

class FindCaseVisitor : public RecursiveASTVisitor<FindCaseVisitor>
{
public:
  explicit FindCaseVisitor(ASTContext *Context)
      : Context(Context) {}

  std::map<IfStmt *, bool> mp;

  bool VisitStmt(Stmt *s)
  {

     
    // Search for -Statements

    /*if (isa<IfStmt>(s))
    {
      IfStmt *ifst = cast<IfStmt>(s);
      std::cout << ifst->hasElseStorage() << std::endl;
      //Stmt *ele=ifst->getElse();
      //std::cout<<ele->getStmtClassName()<<std::endl;

      //fun(ifst);
    }*/

    for (auto it = s->child_begin(); it != s->child_end(); ++it)
    {
      auto statement = *it;

      if (!statement)
      {
        continue;
      }

      if (isa<IfStmt>(statement))
      {
        IfStmt *ifst = cast<IfStmt>(statement);
        //Stmt *ele=ifst->getElse();
        //std::cout<<ele->getStmtClassName()<<std::endl;
        fun(ifst);
      }
    }

    return true;
  }

  void fun(IfStmt *ifst)
  {

    // ifst->hasElseStorage is 0 do nothing  --> it mean simply  single if statement with no else

    //ifst-> hasElseStorage is 1 and ifst->getElse() is simply else  then it is simple if else statement

    //otherwises if have else if , so get that ifst and if that ifst->hasElseStorage()
    // is 0 --->generate warning

    if (ifst->hasElseStorage() == 0)
    {
      return;
    }
    while (ifst->getElse() != NULL)
    {
      Stmt *ele = ifst->getElse();
      //std::cout << ele->getStmtClassName() << std::endl;
      if (isa<IfStmt>(ele))
      {
        ifst = cast<IfStmt>(ele);

        if (!mp[ifst])
        {
          mp[ifst] = 1;
          if (ifst->hasElseStorage() == 0)
          {
            //generate warning
            FullSourceLoc FullLocation = Context->getFullLoc(ifst->getBeginLoc());
            if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
              llvm::outs() << " You can have else statement at "
                           << FullLocation.getSpellingLineNumber() << ":"
                           << FullLocation.getSpellingColumnNumber() << "\n";
            break;
          }
        }else{//it mean we have visited this ifstm previously 
            break;
        }
      }
      else
      { //break when it is compound statement ,which is last else statement
        break;
      }
    }
    
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
  FindCaseVisitor Visitor;
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
