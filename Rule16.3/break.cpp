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

struct My_Data
{

  Stmt *stmt;
  unsigned int line;
  unsigned int col;
};

bool cmp(My_Data d1, My_Data d2)
{

  if (d1.line == d2.line)
  {
    return d1.col < d2.col;
  }
  else
  {
    return d1.line < d2.line;
  }
}

class FindCaseVisitor : public RecursiveASTVisitor<FindCaseVisitor>
{
public:
  explicit FindCaseVisitor(ASTContext *Context)
      : Context(Context) {}

  void printMsg(Stmt *qst)
  {

    FullSourceLoc FullLocation = Context->getFullLoc(qst->getBeginLoc());
    if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
      llvm::outs() << " An unconditional break statement shall terminate every switch-clause "
                   << FullLocation.getSpellingLineNumber() << ":"
                   << FullLocation.getSpellingColumnNumber() << "\n";
  }

  bool VisitStmt(Stmt *s)
  {

    std::queue<Stmt *> q;
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
        all_statement_queue.push(st);
        while (!all_statement_queue.empty())
        {
          Stmt *stmt = all_statement_queue.front();

          all_statement_queue.pop();
          for (auto it = stmt->child_begin(); it != stmt->child_end(); ++it)
          {
            auto in_statement = *it;

            std::string type = in_statement->getStmtClassName();

            /*
              I am skipping nested switch statement cause , it will be consider in next iteration 
              I am also skipping the SwitchStmt, WhileStmt , IfStmt, DoStmt, ForStmt it is not required ,cause we can have break in statement,
              and that break is creating problem while matching case-break

              We also can have case or default statement in SwitchStmt, WhileStmt , IfStmt, DoStmt, ForStmt, I am not conisidering that ,cause pass 16.2
              will say such case or default statemnt is non complient 
              
            */
            if (type == "SwitchStmt" || type == "WhileStmt" || type == "IfStmt" || type == "DoStmt" || type == "ForStmt")
            {

              continue;
            }
            if (type == "BreakStmt" || type == "DefaultStmt" || type == "CaseStmt")
            {
              q.push(in_statement);
            }

            all_statement_queue.push(in_statement);
          }
        }

        std::vector<My_Data> vector_statement;

        //std::cout << q.size() << std::endl;

        while (q.size())
        {

          Stmt *qst = q.front();
          q.pop();

          My_Data my_data;
          my_data.stmt = qst;

          unsigned int line_s1;
          unsigned int col_s1;
          FullSourceLoc FullLocation = Context->getFullLoc(qst->getBeginLoc());
          if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
          {
            line_s1 = FullLocation.getSpellingLineNumber();
            col_s1 = FullLocation.getSpellingColumnNumber();
          }

          my_data.line = line_s1;
          my_data.col = col_s1;

          vector_statement.push_back(my_data);
          //std::string type = qst->getStmtClassName();
          //std::cout << type << std::endl;
        }

        std::sort(vector_statement.begin(), vector_statement.end(), cmp);

        std::queue<Stmt *> final_queue;

        for (std::size_t i = 0; i < vector_statement.size(); i++)
        {
          final_queue.push(vector_statement[i].stmt);
        }

        while (final_queue.size())
        {

          Stmt *qst = final_queue.front();
          final_queue.pop();
          std::string type = qst->getStmtClassName();
          //std::cout << type << std::endl;

          if (type == "CaseStmt") //does this if require
          {
            if (final_queue.size())
            {
              Stmt *inqst = final_queue.front();

              std::string intype = inqst->getStmtClassName();
              if (intype == "CaseStmt" || intype == "DefaultStmt")
              {
                printMsg(qst);
              }
              else
              {
                final_queue.pop(); //remove break from queue
              }
              //std::cout << type << std::endl;
            }
            else
            { //i think only one case with no break
              printMsg(qst);
            }
          }
          else if (type == "DefaultStmt")
          {
            if (final_queue.size())
            {
              Stmt *inqst = final_queue.front();

              std::string intype = inqst->getStmtClassName();
              if (intype == "CaseStmt")
              {
                printMsg(qst);
              }
              else
              {
                final_queue.pop(); //remove break from queue
              }
            }
            else
            { //no break for default statement
              printMsg(qst);
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
