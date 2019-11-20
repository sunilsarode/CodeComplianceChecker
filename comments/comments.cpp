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

class MyClass : public RecursiveASTVisitor<MyClass>
{
public:
  explicit MyClass(ASTContext *Context)
      : Context(Context) {}

private:
  ASTContext *Context;
};

class FindNamedCallConsumer : public clang::ASTConsumer
{
public:
  ASTContext *ctx = NULL;
  explicit FindNamedCallConsumer(ASTContext *context)
      : Visitor(context)
  {
    ctx = context;
  }

  virtual void HandleTranslationUnit(clang::ASTContext &Context)
  {

    auto comments = Context.getRawCommentList().getComments();
    int c = 0;
    std::cout << comments.size() << std::endl;

   

    for (auto comment : comments)
    {
      // std::cout << comment->getRawText(Context.getSourceManager()).str() << std::endl;
      // std::cout<<"--------------------------------------------"<<std::endl;
      std::string comment_str = comment->getRawText(Context.getSourceManager()).str();
      c++;

      if (comment_str[0] == '/' && comment_str[1] == '/')
      {
        
//-- -fparse-all-comments
        if (comment_str.find("/*") != std::string::npos && comment_str.find("*/") != std::string::npos)
        {

          size_t indexOfBlockComments=comment_str.find("/*");

         
          size_t indexOfLineComment=comment_str.find("//",indexOfBlockComments+1);

          //std::cout<<indexOfLineComment<<" "<<indexOfBlockComments<<" "<<std::endl;
           /*
              Basically line comment must be within block comment , to call it nested comment  
           */
          if (indexOfLineComment != std::string::npos&&indexOfLineComment>indexOfBlockComments)
          {
            //nestted comment found
            FullSourceLoc FullLocation = ctx->getFullLoc(comment->getBeginLoc());
            if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
              llvm::outs() << "Found Nested Comment for Comment at "
                           << FullLocation.getSpellingLineNumber() << ":"
                           << FullLocation.getSpellingColumnNumber() << "\n";
          }
        }
      }
      else if (comment_str.find("/*") != std::string::npos && comment_str.find("*/") != std::string::npos)
      {

        if (comment_str.find("//") != std::string::npos)
        {
          //nestted comment found
          FullSourceLoc FullLocation = ctx->getFullLoc(comment->getBeginLoc());
          if (FullLocation.isValid() && !FullLocation.isInSystemHeader())
            llvm::outs() << "Found Nested Comment for Comment at "
                         << FullLocation.getSpellingLineNumber() << ":"
                         << FullLocation.getSpellingColumnNumber() << "\n";
        }
      }
    }
    std::cout << c << std::endl;
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  MyClass Visitor;
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
