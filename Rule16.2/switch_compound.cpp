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

  bool VisitStmt(Stmt *s)
  {

    //pass
    for (auto it = s->child_begin(); it != s->child_end(); ++it)
    {
      auto statement = *it;

      if (!statement)
      {
        continue;
      }

      if (isa<SwitchStmt>(statement))
      {

        std::queue<Stmt *> q;
        std::queue<Stmt *> all_statement_queue;
        SwitchStmt *swt = cast<SwitchStmt>(statement);
        Stmt *st = swt->getBody();
        //if statement is not null then only push in queue 
        if (st)
        {
          q.push(st);
        }
        /*
            get all the child statements of statemnt and put it in all_statement_queue. do this level by level for all statement that we push in queue 'q'.
            It is kind of BFS.
        */
        while (!q.empty())
        {
          Stmt *stmt = q.front();

          q.pop();
          for (auto it = stmt->child_begin(); it != stmt->child_end(); ++it)
          {
            auto in_statement = *it;
            if (!in_statement)
            {
              continue;
            }
            //std::cout << in_statement->getStmtClassName() << std::endl;
            
            std::string type = in_statement->getStmtClassName();

            /*I am skipping nested switch statement cause , it will be consider in next iteration */
            if (type == "SwitchStmt")
            {
              continue;
            }
            q.push(in_statement);
            all_statement_queue.push(in_statement);
          }
        }
        //std::cout << all_statement_queue.size() << std::endl;

        /* 
          Iterate over the all_statement_queue and get all statement other than CaseStmt,BreakStmt and DefaultStmt . If those statement contain 
          CaseStmt and DefaultStmt then display the warnings 
        */
        while (all_statement_queue.size())
        {

          Stmt *qst = all_statement_queue.front();
          all_statement_queue.pop();
          std::string type = qst->getStmtClassName();

          if (type != "CaseStmt" && type != "BreakStmt" && type != "DefaultStmt")
          {
            for (auto it = qst->child_begin(); it != qst->child_end(); ++it)
            {
              auto in_statement = *it;
              //if in_statement is not null then only go ahead
              if (!in_statement)
              {
                continue;
              }
              std::string type = in_statement->getStmtClassName();
              
              if (type == "CaseStmt" || type == "DefaultStmt")
              {
                FullSourceLoc FullLocation = Context->getFullLoc(in_statement->getBeginLoc());
                if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
                  llvm::outs() << "A switch label shall only be used when the most closely-enclosing compound statement is the body of a switch statement "
                               << FullLocation.getSpellingLineNumber() << ":"
                               << FullLocation.getSpellingColumnNumber() << "\n";
              }
            }
          }
        }
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
