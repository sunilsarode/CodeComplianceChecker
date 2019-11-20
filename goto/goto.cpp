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
#include "clang/Sema/Scope.h"
#include <bits/stdc++.h>

using namespace clang;
using namespace clang::tooling;

class FindGotoVisitor : public RecursiveASTVisitor<FindGotoVisitor>
{
public:
  explicit FindGotoVisitor(ASTContext *Context)
      : Context(Context) {}

  //map to detect recursion in Goto statement
  std::map<std::string, unsigned int> keyAsName;
  bool VisitStmt(Stmt *s)
  {

    if (isa<GotoStmt>(s))
    {

      GotoStmt *gotoStmt = cast<GotoStmt>(s);

      FullSourceLoc FullLocation = Context->getFullLoc(gotoStmt->getBeginLoc());
      if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
        llvm::outs() << "Found Goto at "
                     << FullLocation.getSpellingLineNumber() << ":"
                     << FullLocation.getSpellingColumnNumber() << "\n";
    }

    if (s)
    {
      for (auto it = s->child_begin(); it != s->child_end(); ++it)
      {
        auto statement = *it;

        if (!statement) //if s is not null
        {
          continue;
        }

        if (isa<LabelStmt>(statement))
        {

          LabelStmt *labelStmt = cast<LabelStmt>(statement);

          std::string str(labelStmt->getName());

          FullSourceLoc FullLocation = Context->getFullLoc(labelStmt->getBeginLoc());
          if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
            keyAsName[str] = FullLocation.getLineNumber();
        }
        else if (isa<GotoStmt>(statement))
        {
          GotoStmt *gotoStmt = cast<GotoStmt>(statement);
          LabelDecl *lb = gotoStmt->getLabel();

          LabelStmt *glbstmt = lb->getStmt();

          std::string str = glbstmt->getName();

          if (keyAsName.find(str) != keyAsName.end())
          {

            unsigned int labelStmtLoc = keyAsName[str];
            FullSourceLoc FullLocation = Context->getFullLoc(gotoStmt->getBeginLoc());
            unsigned int gotoLoc = FullLocation.getSpellingLineNumber();
            if (gotoLoc > labelStmtLoc)
            {
              if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
                llvm::outs() << "The goto statement shall jump to a label declared later in the same function "
                             << FullLocation.getSpellingLineNumber() << ":"
                             << FullLocation.getSpellingColumnNumber() << "\n";
            }
          }
        }
      }
    }

    /*if (s)
    {
      for (auto it = s->child_begin(); it != s->child_end(); ++it)
      {
        
        if (!statement) //if s is not null
        {
          continue;
        }

        if (isa<GotoStmt>(statement))
        {
          std::cout<<"Hi"<<std::endl;
          GotoStmt *gotoStmt = cast<GotoStmt>(statement);
          LabelDecl *lb = gotoStmt->getLabel();
          
         std::cout<<lb->getOwningModuleID()<<std::endl;
          if(lb->isLocalExternDecl()){
             // std::cout<<m->getVisibilityID()<<std::endl;
             std::cout<<"Hello"<<std::endl;
          }
        } 

      }
    } */

    return true;
  }

  /*std::queue<Stmt *> q;
  bool VisitFunctionDecl(FunctionDecl *FD)
  {

    if (FD->getBody())
    {

      q.push(FD->getBody());
      while (!q.empty())
      {
        Stmt *stmt = q.front();

      
        q.pop();
        for (auto it = stmt->child_begin(); it != stmt->child_end(); ++it)
        {
          auto statement = *it;
          std::cout << statement->getStmtClassName() << std::endl;
          q.push(statement);
        }
      }
    }

    return true;
  }*/

  /*  bool TraverseType(QualType x) {
        // your logic here
      //  std::cout<<x.getAsString()<<std::endl;
        Qualifiers q=x.getQualifiers();

     
       
        if(q.hasRestrict()==1){
            std::cout<<q.getAsString()<<std::endl;
        }else{
            std::cout<<"No qualifier "<<std::endl;
        }
        RecursiveASTVisitor<FindGotoVisitor>::TraverseType(x);
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
  FindGotoVisitor Visitor;
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
