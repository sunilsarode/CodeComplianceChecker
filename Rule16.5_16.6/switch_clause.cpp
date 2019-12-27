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

  void printMsg(Stmt *stmt)
  {

    FullSourceLoc FullLocation = Context->getFullLoc(stmt->getBeginLoc());
    if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
      llvm::outs() << "Every switch statement shall have at least two switch-clauses "
                   << FullLocation.getSpellingLineNumber() << ":"
                   << FullLocation.getSpellingColumnNumber() << "\n";
  }
  bool VisitStmt(Stmt *s)
  {

     /* if we have zero number of case statement or all case statements are blank then we report the warnings */
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

        SwitchStmt *swt = cast<SwitchStmt>(statement);
        Stmt *st = swt->getBody();

        bool caseStm = false;
        bool defaultStm = false;
        int blankCaseCount = 0;
        int caseStmCount = 0;
        //bool blankDefault = false;
        for (auto ini = st->child_begin(); ini != st->child_end(); ini++)
        {
          if (isa<CaseStmt>(*ini))
          {
            caseStm = true;
            caseStmCount++;
            CaseStmt *cst = cast<CaseStmt>(*ini);

            Stmt *ins = cst->getSubStmt();

            if (isa<BreakStmt>(ins))
            {
              blankCaseCount++;
            }
            else if (isa<DefaultStmt>(ins))
            {
              blankCaseCount++;
              ;
            }
            else if (isa<CaseStmt>(ins))
            {
              blankCaseCount++;
            }
          }
          else if (isa<DefaultStmt>(*ini))
          {
            defaultStm = true;

            //DefaultStmt *dft = cast<DefaultStmt>(*ini);

            //Stmt *ins = dft->getSubStmt();
          }
        }

        if (defaultStm == true && caseStm == false)
        { // no case statement
          printMsg(swt);
        }
        else if (caseStmCount == blankCaseCount)
        { //all cases are blank
          printMsg(swt);
        }
      }
    }

    //pass
    for (auto it = s->child_begin(); it != s->child_end(); ++it)
    {
      auto statement = *it;
      //if statement is null continue with next statement
      if (!statement)
      {
        continue;
      }
      /*
            Get all the child statements of statemnt and put it in all_switchlabel_queue. do this level by level for all statement that we push in queue 'q'.
            It is kind of BFS.
      */
      if (isa<SwitchStmt>(statement))
      {
        std::queue<Stmt *> q;
        std::queue<Stmt *> all_switchlabel_queue;
        SwitchStmt *swt = cast<SwitchStmt>(statement);
        Stmt *st = swt->getBody();

        if (st)
        {
          q.push(st);
        }

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
            all_switchlabel_queue.push(in_statement);
          }
        }
        /* while (all_switchlabel_queue.size())
        {
          std::cout << all_switchlabel_queue.front()->getStmtClassName() << std::endl;
          all_switchlabel_queue.pop();
        }*/

        /*
          Putting all the statements in vector_statement in the form of object 
          My_Data.
          struct My_Data
          {

            Stmt *stmt;
            unsigned int line;
            unsigned int col;
          };  
         */
        std::vector<My_Data> vector_statement;
        while (all_switchlabel_queue.size())
        {

          Stmt *qst = all_switchlabel_queue.front();
          all_switchlabel_queue.pop();
          if (!qst)
          {
            continue;
          }
          std::string type = qst->getStmtClassName();
          if (type == "DefaultStmt" || type == "CaseStmt")
          {
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
          }
        }

        /*
          sorting vector using line number, statement having line number less should appear 
          first , if both statement have same line number check for column number. 
          you can see the comparator.
        */

        std::sort(vector_statement.begin(), vector_statement.end(), cmp);

        /* 
           if front of the vector or back of the vector is not DefaultStmt , then search for default statement  
           and  if you found default statment report the warning.

         */

        if (vector_statement.size() > 0)
        {

          std::string frontstmt = vector_statement.front().stmt->getStmtClassName();
          std::string laststmt = vector_statement.back().stmt->getStmtClassName();
          //std::cout<<frontstmt<<" "<<laststmt<<std::endl;
          if (frontstmt != "DefaultStmt" && laststmt != "DefaultStmt")
          {
            while (vector_statement.size())
            {

              Stmt *qst = vector_statement.back().stmt;
              vector_statement.pop_back();
              std::string type = qst->getStmtClassName();
              //std::cout<<type<<std::endl;
              if (type == "DefaultStmt")
              {
                FullSourceLoc FullLocation = Context->getFullLoc(qst->getBeginLoc());
                if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
                  llvm::outs() << "A default label shall appear as either the first or the last switch label of a switch statement "
                               << FullLocation.getSpellingLineNumber() << ":"
                               << FullLocation.getSpellingColumnNumber() << "\n";
                break;
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
