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
      llvm::outs() << "Rule 11.6 A cast shall not be performed between pointer to void and an arithmetic type "
                   << FullLocation.getSpellingLineNumber() << ":"
                   << FullLocation.getSpellingColumnNumber() << "\n";
  }

  // bool VisitDeclRefExpr(DeclRefExpr *declRef){
  //     ValueDecl* valDecl=declRef->getDecl();

  //     //valDecl->dump();
  //     if(isa<VarDecl>(valDecl)){
  //          VarDecl* varDecl=cast<VarDecl>(valDecl);
  //          std::cout<<varDecl->getEvaluatedValue()<<std::endl;
  //     }
  //     return true;
  // }

  // bool VisitIntegerLiteral(IntegerLiteral* intLit){
  //     //std::cout<<intLit->getIntValue()->getSExtValue()<<std::endl;
  //     std::cout<<intLit->getValue().getSExtValue()<<"\n";
  //     //std::cout<<ge<<std::endl;
  //     return true;
  // }

bool IsOperand(char C) 
{
	if(C >= '0' && C <= '9') return true;
	if(C >= 'a' && C <= 'z') return true;
	if(C >= 'A' && C <= 'Z') return true;
	return false;
}

// Function to verify whether a character is operator symbol or not. 
bool IsOperator(char C)
{
	if(C == '+' || C == '-' || C == '*' || C == '/' || C== '^')
		return true;

	return false;
}

// Function to verify whether an operator is right associative or not. 
int IsRightAssociative(char op)
{
	if(op == '^') return true;
	return false;
}

// Function to get weight of an operator. An operator with higher weight will have higher precedence. 
int GetOperatorWeight(char op)
{
	int weight = -1; 
	switch(op)
	{
		case '+':
		    weight = 1;
	    	    break;
		case '-':
			weight = 1;
	    		break;
		case '*':
	    		weight = 2;
	    		break;
		case '/':
			weight = 2;
	    		break;
		case '^':
			weight = 3;
	}
	return weight;
}

// Function to perform an operation and return output. 
int HasHigherPrecedence(char op1, char op2)
{
	int op1Weight = GetOperatorWeight(op1);
	int op2Weight = GetOperatorWeight(op2);

	// If operators have equal precedence, return true if they are left associative. 
	// return false, if right associative. 
	// if operator is left-associative, left one should be given priority. 
	if(op1Weight == op2Weight)
	{
		if(IsRightAssociative(op1)) return false;
		else return true;
	}
	return op1Weight > op2Weight ?  true: false;
}

// Function to evaluate Postfix expression and return output
std::vector<std::string> InfixToPostfix(std::string expression)
{
	//((23*34)+(56*48)-99)")
	//(10 +(6*-6))
	// Declaring a Stack from Standard template library in C++. 
	std::stack<char> S;
	std::vector<std::string> arr;
        bool isOperator=false;
	std::string postfix = ""; // Initialize postfix as empty string.
	for(size_t i = 0;i< expression.length();i++) {

		// Scanning each character from left. 
		// If character is a delimitter, move on. 
                //std::cout<<i<<" "<<expression[i]<<"\n";
		if(expression[i] == ' ' || expression[i] == ','){
			
		 	continue; 
		}
		// If character is operator, pop two elements from stack, perform operation and push the result back. 
		else if(IsOperator(expression[i])) 
		{
			if(isOperator){
                                
				postfix+=expression[i];
				
                                continue;		
			}
			isOperator=true;
			if(postfix!=""){
			   arr.push_back(postfix);
                           postfix="";
			}
			while(!S.empty() && S.top() != '(' && HasHigherPrecedence(S.top(),expression[i]))
			{
				postfix+= S.top();
				S.pop();
				arr.push_back(postfix);
				postfix="";
			}
			S.push(expression[i]);
		}
		// Else if character is an operand
		else if(IsOperand(expression[i]))
		{	isOperator=false;
                        
			postfix +=expression[i];
			if(i==expression.size()-1){
			    arr.push_back(postfix);
				postfix="";
			}
		}

		else if (expression[i] == '(') 
		{	isOperator=false;
			S.push(expression[i]);
		}

		else if(expression[i] == ')') 
		{
			isOperator=false;
			if(postfix!=""){
			   arr.push_back(postfix);
                           postfix="";
			}
	

			while(!S.empty() && S.top() !=  '(') {
				postfix += S.top();
				S.pop();
                arr.push_back(postfix);
				postfix="";
			}
			S.pop();
		}
	}

	while(!S.empty()) {
		postfix += S.top();
		S.pop();
		arr.push_back(postfix);
		postfix="";
	}

	return arr;
}


// Function to verify whether a character is numeric digit. 
bool IsNumericDigit(char C) 
{
	if(C >= '0' && C <= '9') return true;
	return false;
}

    double evalRPN(std::vector<std::string>& tokens) {
       
        std::stack<double> st;
        int n=tokens.size();
        double a,b;
        for(int i=0;i<n;i++){
            
            if(tokens[i]=="+"){
                a=st.top();
                st.pop();
                b=st.top();
                st.pop();
                st.push(a+b);
            }else if(tokens[i]=="-"){
                a=st.top();
                st.pop();
                b=st.top();
                st.pop();
                
                st.push(b-a);
            }else if(tokens[i]=="*"){
                a=st.top();
                st.pop();
                b=st.top();
                st.pop();
                st.push(a*b);
                
            }else if(tokens[i]=="/"){
                a=st.top();
                st.pop();
                b=st.top();
                st.pop();
                st.push(b/a);
                
                
            }else if(tokens[i]=="^"){
	    		a=st.top();
                st.pop();
                b=st.top();
                st.pop();
                st.push(pow(b,a));
            }
	    else{
                st.push(stoi(tokens[i]));
            }
        }
        
        return st.top();
    }

  std::string exprtoString(clang::Expr *expr, SourceManager &sm)
  {

    std::string text = Lexer::getSourceText(CharSourceRange::getTokenRange(expr->getSourceRange()), sm, LangOptions(), 0);
    if (text.size() > 0 && (text.at(text.size() - 1) == ',')) //the text can be ""
      return Lexer::getSourceText(CharSourceRange::getCharRange(expr->getSourceRange()), sm, LangOptions(), 0);
    return text;
  }
  bool VisitCStyleCastExpr(CStyleCastExpr *s)
  {

    //std::cout<<s->getType().getAsString()<<"\n";
    //std::cout<<s->getSubExpr()->getType().getAsString()<<"\n";
    
    
    s->getSourceRange();
    std::string cstyle=s->getType().getAsString();
    if(isa<ImplicitCastExpr>(s->getSubExpr())){

          if(cstyle!="void *"){//casting void to non void
              ImplicitCastExpr* impCast=cast<ImplicitCastExpr>(s->getSubExpr());
              if(impCast->getType().getAsString()=="void *"){
                    printMsg(s);
              }
          }else if(cstyle=="void *"){//casting non-void to void and non-void is not zero 
              
              ImplicitCastExpr* impCast=cast<ImplicitCastExpr>(s->getSubExpr());
              if(impCast->getType().getAsString()=="int"){
                    if(isa<DeclRefExpr>(impCast->getSubExpr())){
                        DeclRefExpr* declRef=cast<DeclRefExpr>(impCast->getSubExpr());
                        ValueDecl* valDecl=declRef->getDecl();
                       
                        if(isa<VarDecl>(valDecl)){
                              VarDecl* varDecl=cast<VarDecl>(valDecl);
                              
                              if(isa<IntegerLiteral>(varDecl->getInit())){
                                  IntegerLiteral* intLit=cast<IntegerLiteral>(varDecl->getInit());
                                  if(intLit->getValue().getSExtValue()!=0){
                                    printMsg(s);
                                  }
                              }
                        }
                    }
              }
          }
    }else if(isa<IntegerLiteral>(s->getSubExpr())){
         if(cstyle=="void *"){
            IntegerLiteral* intLit=cast<IntegerLiteral>(s->getSubExpr());
            if(intLit->getValue().getSExtValue()!=0){
                   printMsg(s);
            }
         }
    }else if(isa<ParenExpr>(s->getSubExpr())){
          if(cstyle=="void *"){
              	clang::SourceManager &sm= Context->getSourceManager();
              	std::string text=exprtoString(s,sm);
		        		int pos=text.find("void");  
              	text=text.substr(pos);
				        size_t pos1=text.find("("); 
                if(pos1!=std::string::npos){
                  text=text.substr(pos1);
                  //std::cout<<text<<"\n";
                  std::vector<std::string> arr=InfixToPostfix(text);
                  double val=evalRPN(arr);
//                   if (abs(a-b) < 1e-9) {
// // a and b are equal
// }--->will think  about it ,A better way to compare floating point numbers is to assume that two
// numbers are equal if the difference between them is less than ε, where ε is a
// small number.
// In practice, the numbers can be compared as follows (ε = 10−9
// ):
                  if(val!=0){
                    printMsg(s);
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
