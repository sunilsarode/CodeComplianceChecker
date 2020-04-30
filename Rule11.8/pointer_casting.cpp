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

  void printMsg(Stmt *qst)
  {

    FullSourceLoc FullLocation = Context->getFullLoc(qst->getBeginLoc());
    if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
      llvm::outs() << "Rule 11.8 A cast shall not remove any const or volatile qualification from the type pointed to by a pointer "
                   << FullLocation.getSpellingLineNumber() << ":"
                   << FullLocation.getSpellingColumnNumber() << "\n";
  }

  int occurrence_string(std::string base_string,std::string search_string){

    int occurrences = 0;
    std::string::size_type start = 0;

    while ((start = base_string.find(search_string, start)) != std::string::npos) {
        ++occurrences;
        start += search_string.length();
    }
    return occurrences;
  }

  bool VisitCStyleCastExpr(CStyleCastExpr *s)
  {

    
    if(isa<ImplicitCastExpr>(s->getSubExpr())){

            std::string impcast=s->getSubExpr()->getType().getAsString();
            
            if(impcast.find("const")!=std::string ::npos){

              std::string cstyle=s->getType().getAsString();
              int impc_occurrences=occurrence_string(impcast,"const");
              int cstyle_occurrences=occurrence_string(cstyle,"const");

              if(cstyle_occurrences<impc_occurrences){
                  printMsg(s);
              }

            }
            if(impcast.find("volatile")!=std::string ::npos){

              std::string cstyle=s->getType().getAsString();
              int impc_occurrences=occurrence_string(impcast,"volatile");
              int cstyle_occurrences=occurrence_string(cstyle,"volatile");

              if(cstyle_occurrences<impc_occurrences){
                  printMsg(s);
              }

            }
            std::cout<<s->getSubExpr()->getType().isVolatileQualified()<<"\n";
            std::cout<<s->getType().isVolatileQualified()<<"\n";
            if(s->getSubExpr()->getType().isVolatileQualified()&&!(s->getType().isVolatileQualified())){
                  printMsg(s);
            }
            

            /*ImplicitCastExpr *impcastExpr=cast<ImplicitCastExpr>(s->getSubExpr());
            
            if(isa<DeclRefExpr>(impcastExpr->getSubExpr())){
                std::string cstyle=s->getType().getAsString();
                std::string declRef=impcastExpr->getSubExpr()->getType().getAsString();
                int cstyle_occurrences=occurrence_string(cstyle,"volatile");
            }*/

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
