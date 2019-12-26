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

class FindEnumVisitor : public RecursiveASTVisitor<FindEnumVisitor>
{
public:
  explicit FindEnumVisitor(ASTContext *Context)
      : Context(Context) {}

  bool VisitEnumDecl(EnumDecl *ED)
  {

    std::unordered_map<int, std::string> mp;
    for (auto it = ED->enumerator_begin(); it != ED->enumerator_end(); it++)
    {

      //std::cout << it->getNameAsString() << " " << it->getInitVal().getSExtValue() << std::endl;

      it->getInitExpr();

      Expr *exp = it->getInitExpr();
      if (exp != NULL)
      {
        /*for (auto it1 = exp->child_begin(); it1 != exp->child_end(); ++it1)
        {
          it1->dump();
        }*/
        if (mp.find(it->getInitVal().getExtValue()) != mp.end())
        {
          FullSourceLoc FullLocation = Context->getFullLoc(ED->getBeginLoc());
          if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
            llvm::outs() << " Within an enumerator list, the value of an implicitly-specified enumeration constant shall be unique "
                         << FullLocation.getSpellingLineNumber() << ":"
                         << FullLocation.getSpellingColumnNumber() << "\n";
        }
      }
      else
      {
        //implicit declaration
        mp[it->getInitVal().getSExtValue()] = it->getNameAsString();
      }
    }
    return true;
  }

  /*bool VisitEnumConstantDecl(EnumConstantDecl *ecd)
  {

    //std::cout <<ecd->getInitVal().getSExtValue()<<" "<<ecd->getNameAsString()<<" "<<ecd->dum<<std::endl;
    Expr *exp = ecd->getInitExpr();
    if (exp != NULL)
    {
      for (auto it = exp->child_begin(); it != exp->child_end(); ++it)
      {
        it->dump();
      }
    }
    else
    {
      //implicit declaration
    }

    //std::cout<<ecd->getNameAsString()<<" "<<ecd->isImplicit()<<" "<<ecd->getCanonicalDecl()->getInitVal().getSExtValue()<<std::endl;
    return true;
  }*/

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
  FindEnumVisitor Visitor;
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
