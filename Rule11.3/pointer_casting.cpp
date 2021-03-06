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
      llvm::outs() << " Rule 11.3 A cast shall not be performed between a pointer to object type and a pointer to a different object type "
                   << FullLocation.getSpellingLineNumber() << ":"
                   << FullLocation.getSpellingColumnNumber() << "\n";
  }

  void remove_sub_string(std::string &base_str,std::string remove_str){
      std::string::size_type i = base_str.find(remove_str);
      while (i != std::string::npos) {
        base_str.erase(i, remove_str.length());
        i = base_str.find(remove_str, i);
      }

  } 

  bool VisitCStyleCastExpr(CStyleCastExpr *s)
  {

    //std::cout<<s->getType().getAsString()<<"\n";
    //std::cout<<s->getSubExpr()->getType().getAsString()<<"\n";
    std::string cstyle=s->getType().getAsString();

    if(cstyle.find("const")!=std::string::npos){
         size_t pos=cstyle.find("const");
         if(pos==0){

            std::string sub1=cstyle.substr(0,6);
            std::string sub=cstyle.substr(6);
            remove_sub_string(sub,"const ");
            cstyle=sub1+sub;
         }
    }
    std::cout<<cstyle<<"\n";
    remove_sub_string(cstyle,"volatile ");

    std::cout<<cstyle<<"\n";

    if(isa<ImplicitCastExpr>(s->getSubExpr())){

                  std::string impcast=s->getSubExpr()->getType().getAsString();

                  if(impcast.find("const")!=std::string::npos){
                    size_t pos=impcast.find("const");
                    if(pos==0){

                        std::string sub1=impcast.substr(0,6);
                        std::string sub=impcast.substr(6);
                        remove_sub_string(sub,"const ");
                        impcast=sub1+sub;
                    }
                  }
                  std::cout<<impcast<<"\n";
                  remove_sub_string(impcast,"volatile ");
                  std::cout<<impcast<<"\n";
                  if(impcast.find("char")==std::string::npos){
                  
                    if(cstyle.compare(impcast)!=0&&cstyle.find("char")==std::string::npos){
                        printMsg(s);
                    }
                  }else{//going from char to other type ,non-complaint 
                        printMsg(s);
                  }

    }else if(isa<UnaryOperator>(s->getSubExpr())){
        UnaryOperator* uop=cast<UnaryOperator>(s->getSubExpr());
        std::string str=uop->getType().getAsString();
        std::cout<<str<<"\n";
        std::cout<<cstyle<<"\n";
                if(cstyle.compare(str)!=0){
                     printMsg(s);
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
