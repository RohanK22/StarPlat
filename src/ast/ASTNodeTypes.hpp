#ifndef ASTNODETYPE_H
#define ASTNODETYPE_H

#include "ASTNode.hpp"
#include <string.h>
#include<list>
#include<iostream>
#include<vector>
#include<stack>
#include<map>
#include<set>
#include<queue>
#include "../maincontext/enum_def.hpp"



using namespace std;
/*class declaration for each node type. Incomplete at the moment.*/
class Expression;
class assignment;
class reductionCall;
class formalParam;
class Type;
class blockStatement;
class Identifier;
extern vector<map<int,vector<Identifier*>>> graphId; 
                                                      /*store the graph variables 
                                                       referenced in current function,
                                                       where the graphId vector is 
                                                       separated across different function type*/
class varTransferStmt;

class argument
{  
  private:
  Expression* expression;
  assignment* assignExpr;
  bool expressionflag;
  bool assign;

  public:
  argument()
  {
    expression=NULL;
    assignExpr=NULL;
    expressionflag=false;
    assign=false;
  }

  void setAssign(assignment* assign)
  {
    assignExpr=assign;
  }
  void setExpression(Expression* expr)
  {
    expression=expr;
  }
  void setAssignFlag()
  {
    assign=true;
  }
  void setExpressionFlag()
  {
    expressionflag=true;
  }

  assignment* getAssignExpr()
  {
    return assignExpr;
  }
  Expression* getExpr()
  {
    return expression;
  }

  bool isAssignExpr()
  {
    return assign;
  }

  bool isExpr()
  {
    return expressionflag;
  }





};
class tempNode
{
   public:
   reductionCall* reducCall=NULL;
   Expression* exprVal=NULL;



};

class paramList
{
  public:
  list<formalParam*> PList;
};

class argList
{
  public:
  list<argument*> AList;
};

class ASTNodeList
{
  public:
  list<ASTNode*> ASTNList;
};


class Identifier:public ASTNode
{
  private:

  int accessType;
  char* identifier;

  Expression* assignedExpr; /*added to store node/edge property initialized values.
                             - Used in SymbolTable phase for storing metadata. 
                             - This field is more of a code generation utility.  */
  bool redecl; /* added for fixedPoint.(node property parameter)
                - This field is populated during SymbolTable creation.
                - it checks if double buffering is required for a node/edge prop.*/
 
  bool fp_association; /*checks if the identifier as a node/edge property
                         is used as a dependent for fixedpoint.*/

  char* fpId;          /*If the identifier is associated with a fixedpoint,
                         the field stores the fixedpoint id name*/
  TableEntry* idInfo;
  Expression* dependentExpr; /*the expression in fixedPoint of which the current
                               identifier is a part of*/
  Identifier* updates_association; /* for update.source/destination, get the updates to
                                      which this update belongs to.*/                             
   
  public: 
 
  static Identifier* createIdNode(const char* id)
   {
   
     Identifier* idNode=new Identifier();
     idNode->identifier=new char[strlen(id)+1];
     strcpy(idNode->identifier,id);
     idNode->accessType=0;
     idNode->setTypeofNode(NODE_ID);
     idNode->redecl=false;
     idNode->fp_association = false;
     idNode->assignedExpr = NULL;
     idNode->dependentExpr = NULL;
   // std::cout<<"IDENTIFIER = "<<idNode->getIdentifier()<<" "<<strlen(idNode->getIdentifier());
     return idNode;

   }

   int getAccessType()
   {
     return accessType;
   }

    char* getIdentifier()
   {
     return identifier;
   }

   void setSymbolInfo(TableEntry* te)
   {
     idInfo=te;

   }
   TableEntry* getSymbolInfo()
   {
     return idInfo;
   }

   Identifier* copy()
   {
     Identifier* copyId;
     copyId = new Identifier();
     copyId->identifier=new char[strlen(identifier)+1];
     strcpy(copyId->identifier,identifier);
     copyId->accessType=0;
     copyId->setTypeofNode(NODE_ID);

     return copyId;
   }
   
   void set_redecl()
   {
     redecl=true;
   }

   bool require_redecl()
   {
     return redecl;
   }

   void set_fpassociation()
   {
     fp_association=true;
   }

   bool get_fp_association()
   {
     return fp_association;
   }

   void set_fpId(char* fp_sentId)
   {
     fpId=fp_sentId;
   }

   char* get_fpId()
   {
     return fpId;
   }

   void set_assignedExpr(Expression* assignExprSent)
   {
     assignedExpr = assignExprSent;
   }

   Expression* get_assignedExpr()
   {
     return assignedExpr;
   }

   void set_dependentExpr(Expression* dependExpr) //Expression in fixedPoint of which the identifier
                                                  // is part of
   {
     dependentExpr = dependExpr;
   }

   Expression* get_dependentExpr()
   {
     return dependentExpr;
   }

   void setUpdateAssociation(Identifier* updateId_sent)
    {
      updates_association = updateId_sent;
    }

   Identifier* getUpdateAssociation()
     {
       return updates_association;
     }

};

class PropAccess:public ASTNode
{
  private:
  Identifier* identifier1;
  Identifier* identifier2;
  int accessType;

  public:
  static PropAccess* createPropAccessNode(Identifier* id1, Identifier* id2)
   {
     PropAccess* propAccessNode=new PropAccess();
     propAccessNode->identifier1=id1;
     propAccessNode->identifier2=id2;
     propAccessNode->accessType=1;
     propAccessNode->setTypeofNode(NODE_PROPACCESS);
     return propAccessNode;

   }

   int getAccessType()
   {
     return accessType;
   }

   Identifier* getIdentifier1()
   {
     return identifier1;
   }
    Identifier* getIdentifier2()
   {
     return identifier2;
   }


};

class statement:public ASTNode
{
  protected:
  string statementType;

  public:

  statement()
  {
    statementType="";
  }


  statement(string statementTypeSent)
  {
     statementType=statementTypeSent;

  }

  string getType()
  {
    return statementType;
  }
  
  };

  class blockStatement:public statement
  {
     private:
     list<statement*> statements;

     public:
     blockStatement()
     {
        statementType="BlockStatement";
        createSymbTab();
     }

      static blockStatement* createnewBlock()
      {
        blockStatement* newBlock=new blockStatement();
        newBlock->setTypeofNode(NODE_BLOCKSTMT);
      //  newBlock->statementType="BlockStaement";
         return newBlock;
      }

      void addStmtToBlock(statement* stmt)
          {
            statements.push_back(stmt);
            stmt->setParent(this);
            //cout<<stmt->getInt();


          }
      void addToFront(statement* stmt)
      {
        statements.push_front(stmt);
      }

      void insertToBlock(statement* stmt,int pos)
      {

           list<statement*> newList;
           list<statement*>::iterator itrt;
           int count=0;
           for(itrt=statements.begin();itrt!=statements.end();itrt++)
           {
             if(count==pos-1)
             {
               //printf("Here\n");
               break;
             }

             newList.push_back(*itrt);
             count++;

           }
           newList.push_back(stmt);
           for( ;itrt!=statements.end();itrt++)
           {
             newList.push_back((*itrt));
           }

          statements.swap(newList);

      }

      list<statement*> returnStatements()
      {
        return statements;
      }

      void removeStatement(statement* stmt){
          statements.remove(stmt);
      }

      void clearStatements(){
        statements.clear();
      }
  };

class Function:public ASTNode
{
  private:

  Identifier* functionId;
  list<formalParam*> paramList;
  blockStatement* blockstmt;
  bool initialLockDecl; /* If omp_locks required in some part of function body,
                           set this to initialize the omp_locks in 1st part of func body*/
  bool hasReturn;                          
  int funcType ;


  public:

  Function()
  { 
    functionId=NULL;
    blockstmt=NULL;
    funcType = 0;
    initialLockDecl = false;
    createSymbTab();
  }

  static Function* createFunctionNode(Identifier* funcId,list<formalParam*> paramList)
  {
      Function* func=new Function();
      func->functionId=funcId;
      func->paramList=paramList;
      func->setTypeofNode(NODE_FUNC);
      func->setFuncType(GEN_FUNC);
      return func;

  }
  static Function*  createStaticFunctionNode(Identifier* funcId,list<formalParam*> paramList)
  {
   
    Function* staticFunc = new Function();
    staticFunc->functionId = funcId;
    staticFunc->paramList = paramList;
    staticFunc->setTypeofNode(NODE_FUNC);
    staticFunc->setFuncType(STATIC_FUNC);
    return staticFunc;
 

  }


  static Function*  createDynamicFunctionNode(Identifier* funcId,list<formalParam*> paramList)
  {
   
    Function* dynamicFunc = new Function();
    dynamicFunc->functionId = funcId;
    dynamicFunc->paramList = paramList;
    dynamicFunc->setTypeofNode(NODE_FUNC);
    dynamicFunc->setFuncType(DYNAMIC_FUNC);
    return dynamicFunc;
 

  }

  static Function*  createIncrementalNode(list<formalParam*> paramList)
  {
   
    Function* incrementalFunc = new Function();
    incrementalFunc->paramList = paramList;
    incrementalFunc->setTypeofNode(NODE_FUNC);
    incrementalFunc->setFuncType(INCREMENTAL_FUNC);

    return incrementalFunc;

  }

  static Function*  createDecrementalNode(list<formalParam*> paramList)
  {
   
    Function* decrementalFunc = new Function();
    decrementalFunc->paramList = paramList;
    decrementalFunc->setTypeofNode(NODE_FUNC);
    decrementalFunc->setFuncType(DECREMENTAL_FUNC);

    return decrementalFunc;

  }


  void setFuncType(FUNCTYPE type)
  {
    funcType = type;
  }
  
   void setBlockStatement(blockStatement* blockStmtSent)
   {
     blockstmt=blockStmtSent;
     blockStmtSent->setParent(this);
   }

   Identifier* getIdentifier()
   {
      return functionId;
   }
   list<formalParam*> getParamList()
   {
     return paramList;
   }

   blockStatement* getBlockStatement()
   {
     return blockstmt;
   }

   int getFuncType()
   {
     return funcType;
   }

   void setInitialLockDecl()
     { 
       //printf("INSIDE THIS FOR LOCKSET\n");
       initialLockDecl = true;
     }

   bool getInitialLockDecl()
      {
        return initialLockDecl ;
      } 

   void flagReturn()
     {

        hasReturn = true ;

     }    

  bool containsReturn()
     {

       return hasReturn ;
     }   


};

class Type:public ASTNode
{
  private:
  int typeId;
  int rootType;
  Identifier* TargetGraph;
  Type* innerTargetType;
  Identifier* sourceGraph;
  list<char*> graphPropList;

 public:
 Type()
 {
     typeId = TYPE_NONE;
     TargetGraph=NULL;
     innerTargetType=NULL;
     sourceGraph=NULL;
 }

  static Type* createForPrimitive(int typeIdSent,int rootTypeSent)
  {
     Type* type=new Type();
     type->typeId=typeIdSent;
     type->rootType=rootTypeSent;
     type->setTypeofNode(NODE_TYPE);
     return type;

  }

  static Type* createForGraphType(int typeIdSent,int rootTypeSent, Identifier* TargetGraphSent)
  {
       Type* type=new Type();
       type->typeId=typeIdSent;
       type->rootType=rootTypeSent;
       type->TargetGraph=TargetGraphSent;
       return type;
  }


  static Type* createForCollectionType(int typeIdSent,int rootTypeSent, Identifier* TargetGraphSent)
  {
       Type* type=new Type();
       type->typeId=typeIdSent;
       type->rootType=rootTypeSent;
       type->TargetGraph=TargetGraphSent;
       return type;
  }
  static Type* createForPropertyType(int typeIdSent,int rootTypeSent, Type* innerTargetTypeSent)
  {
       Type* type=new Type();
       type->typeId=typeIdSent;
       type->rootType=rootTypeSent;
       type->innerTargetType=innerTargetTypeSent;
       return type;
  }
  static Type* createForNodeEdgeType(int typeIdSent,int rootTypeSent)
  {
    Type* type=new Type();
    type->typeId=typeIdSent;
    type->rootType=rootTypeSent;
    return type;
  }

  Type* copy()
  {
     Type* copyType=new Type();
     copyType->typeId=typeId;
     copyType->TargetGraph=(this->TargetGraph==NULL)?NULL:this->TargetGraph->copy();
     copyType->sourceGraph=(this->sourceGraph==NULL)?NULL:this->sourceGraph->copy();
     copyType->innerTargetType=(this->innerTargetType==NULL)?NULL:this->innerTargetType->copy();
     return copyType;
  }
  int getRootType()
  {
    return rootType;
  }

  int gettypeId()
  {
    return typeId;
  }
  bool isNodeEdgeType()
    {
       return check_isNodeEdgeType(typeId);

    }

   bool isPropType()
   {
     return check_isPropType(typeId);
   }

   bool isCollectionType()
   {
     return check_isCollectionType(typeId);
   }

   bool isGraphType()
   {
     return check_isGraphType(typeId);
   }
   bool isPrimitiveType()
   {
     return check_isPrimitiveType(typeId);
   }

   bool isPropNodeType()
   {
     return check_isPropNodeType(typeId);
   }

   bool isPropEdgeType()
   {
     return check_isPropEdgeType(typeId);
   }

   bool isListCollection()
   {
     return check_isListCollectionType(typeId);
   }

   bool isSetCollection()
   {
     return check_isSetCollectionType(typeId);
   }

   bool isNodeType()
   {
     return check_isNodeType(typeId);
   }
   bool isEdgeType()
   {
     return check_isEdgeType(typeId);
   }

  void addSourceGraph(ASTNode* id)
  {
    sourceGraph=(Identifier*)id;
  }
  Type* getInnerTargetType()
  {
    return innerTargetType;
  }
  Identifier* getTargetGraph()
  {
    return TargetGraph;
  }
  Identifier* getSourceGraph()
  {
    return sourceGraph;
  }
  void setTargetGraph(Identifier* id)
  {
    TargetGraph=id;
  }

  void addToPropList(Identifier* id)
  {
    graphPropList.push_back(id->getIdentifier());
  }

  list<char*> getPropList()
  {
    return graphPropList;

  }

};
class formalParam:public ASTNode
{
  private:
  Type* type;
  Identifier* identifier;


  public:
  formalParam()
  {
      type=NULL;
      identifier=NULL;

  }

  static formalParam* createFormalParam(Type* typeSent,Identifier* identifierSent)
  {
      formalParam* formalPNode=new formalParam();
      formalPNode->type=typeSent;
      formalPNode->identifier=identifierSent;
      formalPNode->setTypeofNode(NODE_FORMALPARAM);

      return formalPNode;

  }

  Type* getType()
  {
      return type;
  }

  Identifier* getIdentifier()
  {
      return identifier;
  }



};


  class Expression:public ASTNode
  {
    private:
    Expression* left;
    Expression* right;
    Expression* unaryExpr;
    int overallType;
    long integerConstant;
    double floatConstant;
    bool booleanConstant;
    char charConstant;
    bool infinityType;
    int operatorType;
    int typeofExpr;
    Identifier* id;
    PropAccess* propId;
    bool enclosedBrackets;

    public:

    Expression()
    {
      left=NULL;
      right=NULL;
      id=NULL;
      propId=NULL;
      typeofNode=NODE_EXPR;
      overallType=-1;
      enclosedBrackets=false;
    }

    static Expression* nodeForArithmeticExpr(Expression* left,Expression* right,int arithmeticOperator)
    {
      Expression* arithmeticExpr=new Expression();
      arithmeticExpr->left=left;
      arithmeticExpr->right=right;
      arithmeticExpr->operatorType=arithmeticOperator;
      arithmeticExpr->typeofExpr=EXPR_ARITHMETIC;
      left->parent=arithmeticExpr;
      right->parent=arithmeticExpr;
       return arithmeticExpr;

    }

    static Expression* nodeForRelationalExpr(Expression* left,Expression* right,int relationalOperator)
    {
      Expression* relationalExpr=new Expression();
      relationalExpr->left=left;
      relationalExpr->right=right;
      relationalExpr->operatorType=relationalOperator;
      relationalExpr->typeofExpr=EXPR_RELATIONAL;
      relationalExpr->overallType=TYPE_BOOL;
      left->parent=relationalExpr;
      right->parent=relationalExpr;
      return relationalExpr;

    }

    static Expression* nodeForLogicalExpr(Expression* left,Expression* right,int logicalOperator)
    {
      Expression* logicalExpr=new Expression();
      logicalExpr->left=left;
      logicalExpr->right=right;
      logicalExpr->operatorType=logicalOperator;
      logicalExpr->typeofExpr=EXPR_LOGICAL;
      logicalExpr->overallType=TYPE_BOOL;
      left->parent=logicalExpr;
      right->parent=logicalExpr;


       return logicalExpr;

    }

    static Expression* nodeForUnaryExpr(Expression* expr,int operatorType)
    {
      Expression* unaryExpression=new Expression();
      unaryExpression->unaryExpr=expr;
      unaryExpression->operatorType=operatorType;
      unaryExpression->typeofExpr = EXPR_UNARY;
      expr->parent=unaryExpression;

      return unaryExpression;
    }


    /*static Expression* nodeForChar(char charVal)
      {
        Expression* charValExpr = new Expression();
        charValExpr->charConstant = charVal;
        charValExpr->typeofExpr = EXPR_CHARCONSTANT;
        return charValExpr;
      }*/

    static Expression* nodeForIntegerConstant(long integerValue)
    {
       Expression* integerConstantExpr=new Expression();
       integerConstantExpr->integerConstant=integerValue;
       integerConstantExpr->typeofExpr=EXPR_INTCONSTANT;
       return integerConstantExpr;

    }

    static Expression* nodeForDoubleConstant(double doubleValue)
    {
       Expression* doubleConstantExpr=new Expression();
       doubleConstantExpr->floatConstant=doubleValue;
       doubleConstantExpr->typeofExpr=EXPR_FLOATCONSTANT;
       return doubleConstantExpr;

    }
     static Expression* nodeForBooleanConstant(bool boolValue)
    {
       Expression* boolExpr = new Expression();
       boolExpr->booleanConstant=boolValue;
       boolExpr->typeofExpr = EXPR_BOOLCONSTANT;
       return boolExpr;

    }




     static Expression* nodeForInfinity(bool infinityValue)
    {
       Expression* infinityExpr=new Expression();
       infinityExpr->infinityType=infinityValue;
       infinityExpr->typeofExpr=EXPR_INFINITY;
       return infinityExpr;

    }

     static Expression* nodeForIdentifier(Identifier* id)
    {
       Expression* idExpr=new Expression();
       idExpr->id=id;
       idExpr->typeofExpr=EXPR_ID;
       return idExpr;

    }
      static Expression* nodeForPropAccess(PropAccess* propId)
    {
       Expression* propIdExpr=new Expression();
       propIdExpr->propId=propId;
       propId->setParent(propIdExpr);
       propIdExpr->typeofExpr=EXPR_PROPID;
       return propIdExpr;

    }
    void alterBoolValue()
    {
      booleanConstant=!booleanConstant;
    }
    bool isBooleanLiteral()
    {
      return (typeofExpr==EXPR_BOOLCONSTANT);
    }
    bool isArithmetic()
    {
      return (typeofExpr==EXPR_ARITHMETIC);
    }

    bool isRelational()
    {
      return (typeofExpr==EXPR_RELATIONAL);
    }
    bool isLogical()
    {
      return (typeofExpr==EXPR_LOGICAL);
    }
    bool isUnary()
    {
      return (typeofExpr==EXPR_UNARY);
    }

    bool isIdentifierExpr()
    {
      return (typeofExpr==EXPR_ID);
    }
    bool isPropIdExpr()
    {
      return (typeofExpr==EXPR_PROPID);
    }
    bool isLiteral()
    {
      return(typeofExpr==EXPR_BOOLCONSTANT||typeofExpr==EXPR_INTCONSTANT||
                    typeofExpr==EXPR_FLOATCONSTANT);
    }
    bool isInfinity()
    {
      return (typeofExpr==EXPR_INFINITY);
    }
    bool isProcCallExpr()
     {
       return (typeofExpr==EXPR_PROCCALL);
     }

     Expression* getUnaryExpr()
     {
       return unaryExpr;
     }

     Expression* getLeft()
     {
       return left;
     }

     Expression* getRight()
     {
       return right;
     }

     Identifier* getId()
     {
       return id;
     }

     PropAccess* getPropId()
     {
       return propId;
     }


     int getOperatorType()
     {
       return operatorType;
     }

     long getIntegerConstant()
     {
       return integerConstant;
     }

     bool getBooleanConstant()
     {
       return booleanConstant;
     }


     double getFloatConstant()
     {
       return floatConstant;
     }


     bool isPositiveInfinity()
     {

       return infinityType;

     }


     void setTypeofExpr(int type)
     {
        overallType=type;
     }

     int getTypeofExpr()
     {
       return overallType;
     }

     void setExpressionFamily(int exprFamily)
     {

       typeofExpr=exprFamily;
     }

     int getExpressionFamily()
     {
       return typeofExpr;
     }
     
     void setEnclosedBrackets()
     {
       enclosedBrackets=true;
     }

     bool hasEnclosedBrackets()
     {
       return enclosedBrackets;
     }

  };


  class returnStmt:public statement
   {
     private:
     Expression* returnExpression;

     public:
     returnStmt()
      {
        returnExpression = NULL;
      }
     
     static returnStmt* createNodeForReturnStmt(Expression* returnExpressionSent)
        {
            returnStmt* returnStmtNode = new returnStmt();
            returnStmtNode->returnExpression = returnExpressionSent;
            returnStmtNode->setTypeofNode(NODE_RETURN);
            returnExpressionSent->setParent(returnStmtNode);
            return returnStmtNode;
        }

     Expression* getReturnExpression()
       {
         return returnExpression;
       }   


   };




  

  class batchBlock:public statement
  {
    private:
    blockStatement* statements;
    Identifier* updateId; /* contains the update Id over which this batchblock operates on */
    Expression* batchSizeExpr;

    public:
    batchBlock()
      {
        statements = NULL;
        batchSizeExpr = NULL;
        updateId = NULL;
      }

    static batchBlock* createNodeForBatchBlock(Identifier* updatesIdSent, Expression* batchSizeExprSent, statement* statements_sent)
        {
            batchBlock* batchBlockNode = new batchBlock();
            batchBlockNode->statements = (blockStatement*)statements_sent;
            batchBlockNode->batchSizeExpr = batchSizeExprSent;
            batchBlockNode->updateId = updatesIdSent; 
            batchBlockNode->setTypeofNode(NODE_BATCHBLOCKSTMT);
            updatesIdSent->setParent(batchBlockNode);
            batchSizeExprSent->setParent(batchBlockNode);
            statements_sent->setParent(batchBlockNode);
            return batchBlockNode;
        }


    blockStatement* getStatements()
     {
       return statements;
     }

    Expression* getBatchSizeExpr()
     {
      
      return batchSizeExpr;

     }   
     
     /*void setUpdateId(Identifier* updateIdSent)
      {
        updateId = updateIdSent;
      }*/

      Identifier* getUpdateId()
       {
         return updateId;
       }


  
  } ;

  class declaration:public statement
  {
    private:
    Type* type;
    Identifier* identifier;
    Expression* exprAssigned;
    bool inGPU;

    public:
    declaration()
    {
        type=NULL;
        identifier=NULL;
        exprAssigned=NULL;
        statementType="declaration";
        inGPU = false;
       
    }

    static declaration* normal_Declaration(Type* typeSent,Identifier* identifierSent)
    {
          declaration* decl=new declaration();
          decl->type=typeSent;
          decl->identifier=identifierSent;
          decl->setTypeofNode(NODE_DECL);
          identifierSent->setParent(decl);
          typeSent->setParent(decl);
         // decl->statementType="declaration";
          return decl;
    }

    static declaration* assign_Declaration(Type* typeSent,Identifier* identifierSent,Expression* expression)
    {     declaration* decl=new declaration();
          decl->type=typeSent;
          decl->identifier=identifierSent;
          decl->setTypeofNode(NODE_DECL);
          expression->setTypeofExpr(typeSent->gettypeId());
          decl->exprAssigned=expression;
          typeSent->setParent(decl);
          identifierSent->setParent(decl);
          expression->setParent(decl);
          return decl;

    }
    Type* getType()
    {
      return type;
    }
    Identifier* getdeclId()
    {
      return identifier;
    }
    Expression* getExpressionAssigned()
    {
      return exprAssigned;
    }

    bool isInitialized()
    {
      return (exprAssigned!=NULL);
    }

    void setInGPU(bool inGPU){
      this->inGPU = inGPU;
    }

    bool getInGPU(){
      return inGPU;
    }
  };
  class assignment:public statement
  {
     private:
     Identifier* identifier;
     PropAccess* propId;
     Expression* exprAssigned;
     bool isPropCopy ;
     bool atomicSignal;
     bool deviceVariable;
     bool accumulateKernel;
     int lhsType;

     public:
     assignment()
    {
        identifier=NULL;
        propId=NULL;
        exprAssigned=NULL;
         statementType="assignment";
         atomicSignal=false;
         deviceVariable=false;
         accumulateKernel=false;
        isPropCopy = false;
    }

     static assignment* id_assignExpr(Identifier* identifierSent,Expression* expressionSent)
     {
            assignment* assign=new assignment();
            assign->identifier=identifierSent;
          //  cout<<"ID VALUES"<<identifierSent->getIdentifier()<<"\n";
            assign->exprAssigned=expressionSent;
            assign->lhsType=1;
            assign->setTypeofNode(NODE_ASSIGN);
            identifierSent->setParent(assign);
            expressionSent->setParent(assign);
          //  cout<<"TYPEASSIGN="<<assign->getType();
            return assign;


     }
      static assignment* prop_assignExpr(PropAccess* propId,Expression* expressionSent)
     {
            assignment* assign=new assignment();
            assign->propId=propId;
            assign->exprAssigned=expressionSent;
            assign->lhsType=2;
             assign->setTypeofNode(NODE_ASSIGN);
             propId->setParent(assign);
             expressionSent->setParent(assign);
            return assign;


     }

     bool lhs_isIdentifier()
     {
       return (lhsType==1);
     }

     bool lhs_isProp()
     {
       return (lhsType==2);
     }

     Identifier* getId()
     {
       return identifier;
     }

     PropAccess* getPropId()
     {
       return propId;
     }
     Expression* getExpr()
     {
       return exprAssigned;
     }
     int getLhsType()
     {
       return lhsType;
     }
     void addAtomicSignal()
     {
       if(exprAssigned->isArithmetic()||exprAssigned->isLogical())
          atomicSignal=true;
     }

     bool getAtomicSignal()
     {
       return atomicSignal;
     }


     void flagAsDeviceVariable(){
        //~ if(exprAssigned->isArithmetic()||exprAssigned->isLogical())
          deviceVariable=true;
     }
     bool isDeviceAssignment(){
      return deviceVariable;
     }
     void flagAccumulateKernel(){
        //~ if(exprAssigned->isArithmetic()||exprAssigned->isLogical())
          accumulateKernel=true;
     }
     bool isAccumulateKernel(){
      return accumulateKernel;
     }

      void setPropCopy(){
        isPropCopy = true;
      }

      bool hasPropCopy(){
        return isPropCopy;
      }
  };
class whileStmt:public statement
{
  private:
  Expression* iterCondition;
  blockStatement* body;

  public:

    whileStmt()
    {
      iterCondition=NULL;
      body=NULL;
      statementType="WhileStmt";
    }

    static whileStmt* create_whileStmt(Expression* iterConditionSent,blockStatement* bodySent)
    {
      whileStmt* new_whileStmt=new whileStmt();
      new_whileStmt->iterCondition=iterConditionSent;
      new_whileStmt->body=bodySent;
      new_whileStmt->setTypeofNode(NODE_WHILESTMT);
      iterConditionSent->setParent(new_whileStmt);
      bodySent->setParent(new_whileStmt);
      return new_whileStmt;
    }

    Expression* getCondition()
    {
      return iterCondition;
    }

    statement* getBody()
    {
      return body;
    }

    void setBody(blockStatement* bodySent)
    {
       body=bodySent;
    }
  }; 
  class dowhileStmt:public statement
{
  private:
  Expression* iterCondition;
  blockStatement* body;

  public:

    dowhileStmt()
    {
      iterCondition=NULL;
      body=NULL;
      statementType="WhileStmt";
    }

    static dowhileStmt* create_dowhileStmt(Expression* iterConditionSent,blockStatement* bodySent)
    {
      dowhileStmt* new_dowhileStmt=new dowhileStmt();
      new_dowhileStmt->iterCondition=iterConditionSent;
      new_dowhileStmt->body=bodySent;
      new_dowhileStmt->setTypeofNode(NODE_DOWHILESTMT);
      iterConditionSent->setParent(new_dowhileStmt);
      bodySent->setParent(new_dowhileStmt);
      return new_dowhileStmt;
    }

    Expression* getCondition()
    {
      return iterCondition;
    }

    statement* getBody()
    {
      return body;
    }

    void setBody(blockStatement* bodySent)
    {
       body=bodySent;
    }
  };



class fixedPointStmt:public statement
  {

    private:
    Expression* dependentProp;
    Identifier* fixedPointId;
    statement* body;

    public:
    fixedPointStmt()
    {
      dependentProp=NULL;
      fixedPointId=NULL;
      body=NULL;
      statementType="FixedPointStmt";


    }

    static fixedPointStmt* createforfixedPointStmt(Identifier* fixedPointIdSent,Expression* dependentPropSent,statement* body)
    {
      fixedPointStmt* new_fixedPointStmt=new fixedPointStmt();
      new_fixedPointStmt->fixedPointId=fixedPointIdSent;
      new_fixedPointStmt->dependentProp=dependentPropSent;
      new_fixedPointStmt->body=body;
      new_fixedPointStmt->setTypeofNode(NODE_FIXEDPTSTMT);
      fixedPointIdSent->setParent(new_fixedPointStmt);
      dependentPropSent->setParent(new_fixedPointStmt);
      body->setParent(new_fixedPointStmt);
      return new_fixedPointStmt;
    }

     Expression* getDependentProp()
     {
       return dependentProp;
     }

     Identifier* getFixedPointId()
     {
       return fixedPointId;
     }
     statement* getBody()
     {
       return body;
     }
    void setBody(statement* body)
    {
      this->body = body;
    }

};

  class ifStmt:public statement
 {
  private:
  Expression* condition;
  statement*  ifBody;
  statement*  thenBody;

  public:

    ifStmt()
    {
      condition=NULL;
      ifBody=NULL;
      thenBody=NULL;
      statementType="IfStmt";
    }

    static ifStmt* create_ifStmt(Expression* condition,statement* ifBodySent,statement* thenBodySent)
    {
      ifStmt* new_ifStmt=new ifStmt();
      new_ifStmt->condition=condition;
      new_ifStmt->ifBody=ifBodySent;
      new_ifStmt->thenBody=thenBodySent;
      new_ifStmt->setTypeofNode(NODE_IFSTMT);
      condition->setParent(new_ifStmt);
      ifBodySent->setParent(new_ifStmt);
      if(thenBodySent!=NULL)
      thenBodySent->setParent(new_ifStmt);

      return new_ifStmt;
    }

    Expression* getCondition()
    {
      return condition;
    }

    statement* getIfBody()
    {
      return ifBody;
    }
    statement* getElseBody()
    {
      return thenBody;
    }

  };

  class iterateReverseBFS:public ASTNode
  {
    private:
    Expression* booleanExpr;
    Expression* filterExpr;
    statement* body;
    //list<varTransferStmt*> revTransfer;

    list<Identifier*> usedVars;
    public: 
    iterateReverseBFS()
    {
      filterExpr=NULL;
      booleanExpr=NULL;
      body=NULL;
    }

    static iterateReverseBFS* nodeForRevBFS(Expression* booleanExpr,/*Expression* filterExpr,*/statement* body)
    {
      iterateReverseBFS* new_revBFS=new iterateReverseBFS();
      new_revBFS->booleanExpr=booleanExpr;
      //new_revBFS->filterExpr=filterExpr;
      new_revBFS->body=body;
      new_revBFS->setTypeofNode(NODE_ITRRBFS);
      if(booleanExpr!=NULL)
      booleanExpr->setParent(new_revBFS);
      /*if(filterExpr!=NULL)
      filterExpr->setParent(new_revBFS);*/
      body->setParent(new_revBFS);
      return new_revBFS;
    }

    statement* getBody()
    {
      return body;
    }

    Expression* getBFSFilter()
    {
      return booleanExpr;
    }
    bool hasFilter()
    {
      if(booleanExpr!=NULL)
        return true;
      else
        return false;
    }

    void initUsedVariable(list<Identifier*> usedVars){
      this->usedVars = usedVars;
    }
    
    /*void addVarTransfer(varTransferStmt* tstmt){
      this->revTransfer.push_back(tstmt);
    }*/
    void addAccumulateAssignment(){
      blockStatement* block=(blockStatement*)body;
       //~ int i=0;
        for(statement* stmt:block->returnStatements())
         {
             //~ std::cout<< "i" << i++ << '\n';
             if(stmt->getTypeofNode()==NODE_ASSIGN){
               std::cout<< "\t\tON ACC ASST STMT IN REV BFS" << '\n';
               assignment* assign=(assignment*)stmt;
               assign->flagAccumulateKernel();
              }
         }
    }

  };

  class proc_callExpr:public Expression
  {
    private:
    Identifier* id1;
    Identifier* id2;
    Identifier* methodId;
    list<argument*> argList;
    
    public:
    proc_callExpr()
    {
      id1=NULL;
      id2=NULL;
      methodId=NULL;
      typeofNode=NODE_PROCCALLEXPR;
    }

    
    static proc_callExpr* nodeForProc_Call(Identifier* id1,Identifier* id2,Identifier* methodId,list<argument*> argList)
    {
          proc_callExpr* procExpr=new proc_callExpr();
          procExpr->id1=id1;
          procExpr->id2=id2;
          procExpr->methodId=methodId;
          procExpr->argList=argList;
          procExpr->setExpressionFamily(EXPR_PROCCALL);
          return procExpr;


    }

    Identifier* getMethodId()
    {
      return methodId;
    }
    
    Identifier* getId1()
    {
      return id1;
    }
    
    Identifier* getId2()
    {
      return id2;
    }

    list<argument*> getArgList()
    {
      return argList;
    }

    void addToArgList(argument* arg)
    {
         argList.push_back(arg);
    }


  };


  
  class onDeleteBlock:public statement
  {

    private:
    Identifier* itertorId;
    Identifier* updateId;
    proc_callExpr* updateFunc;
    blockStatement* statements;

    public:
    onDeleteBlock()
      {

        itertorId = NULL;
        updateId = NULL;
        updateFunc = NULL;
        statements = NULL;
      }

    static onDeleteBlock* createNodeForOnDeleteBlock(Identifier* iteratorSent, Identifier* sourceId, proc_callExpr* sourceFunc,statement* statements_sent)
        {
            onDeleteBlock* onDeleteNode = new onDeleteBlock();
            onDeleteNode->itertorId = iteratorSent;
            onDeleteNode->updateId = sourceId;
            onDeleteNode->updateFunc = sourceFunc;
            onDeleteNode->statements = (blockStatement*)statements_sent;
            onDeleteNode->setTypeofNode(NODE_ONDELETEBLOCK);
            statements_sent->setParent(onDeleteNode);
            iteratorSent->setParent(onDeleteNode);
            sourceId->setParent(onDeleteNode);
            sourceFunc->setParent(onDeleteNode);
            return onDeleteNode;
        }

    blockStatement* getStatements()
     {
       return statements;
     }   

     Identifier* getIteratorId()
     {
       return itertorId;
     }  

     Identifier* getUpdateId()
     {
       return updateId;
     }
    
    proc_callExpr* getUpdateFunc()
    {
      return updateFunc;
    }
 

  };

  class onAddBlock:public statement
  {

    private:
    Identifier* itertorId;
    Identifier* updateId;
    proc_callExpr* updateFunc;
    blockStatement* statements;

    public:
    onAddBlock()
      {
        itertorId = NULL;
        updateId = NULL;
        updateFunc = NULL;
        statements = NULL;
      }

    static onAddBlock* createNodeForOnAddBlock(Identifier* iteratorSent, Identifier* sourceId, proc_callExpr* sourceFunc, statement* statements_sent)
        {
            onAddBlock* onAddNode = new onAddBlock();
            onAddNode->statements = (blockStatement*)statements_sent;
            onAddNode->itertorId = iteratorSent;
            onAddNode->updateId = sourceId;
            onAddNode->updateFunc = sourceFunc;
            onAddNode->setTypeofNode(NODE_ONADDBLOCK);
            statements_sent->setParent(onAddNode);
            iteratorSent->setParent(onAddNode);
            sourceId->setParent(onAddNode);
            sourceFunc->setParent(onAddNode);
            return onAddNode;
        }

    blockStatement* getStatements()
     {
       return statements;
     }  

     Identifier* getIteratorId()
     {
       return itertorId;
     }  

     Identifier* getUpdateId()
     {
       return updateId;
     }
    
    proc_callExpr* getUpdateFunc()
    {
      return updateFunc;
    }

  };
  
  class iterateBFS:public statement
  {   private:
      Identifier* iterator;
      Identifier* rootNode;
      Identifier* graphId;
      proc_callExpr* nodeCall;
      Expression* filterExpr;
      statement* body;
      iterateReverseBFS* revBFS;

      list<Identifier*> usedVars;
      public:

      iterateBFS()
      {
        iterator=NULL;
        graphId=NULL;
        nodeCall = NULL;
        rootNode=NULL;
        filterExpr=NULL;
        body=NULL;
        revBFS=NULL;
        statementType="IterateInBFS";
      }
    
      static iterateBFS* nodeForIterateBFS(Identifier* iterator,Identifier* graphId,proc_callExpr* nodeCall,Identifier* rootNode,Expression* filterExpr,statement* body,iterateReverseBFS* revBFS)
      {
        iterateBFS* new_iterBFS=new iterateBFS();
        new_iterBFS->iterator=iterator;
        new_iterBFS->graphId=graphId;
        new_iterBFS->rootNode=rootNode;
        new_iterBFS->nodeCall = nodeCall;
        new_iterBFS->filterExpr=filterExpr;
        new_iterBFS->body=body;
        new_iterBFS->revBFS=revBFS;
        new_iterBFS->setTypeofNode(NODE_ITRBFS);
        body->setParent(new_iterBFS);
        revBFS->setParent(new_iterBFS);
        return new_iterBFS;
      }

      Identifier* getRootNode()
      {
        return rootNode;
      }

      Identifier* getIteratorNode()
      {
        return iterator;
      }

      Identifier* getGraphCandidate()
      {
        return graphId;
      }

      statement* getBody()
      {
        return body;
      }

      iterateReverseBFS* getRBFS()
      {
        return revBFS;
      }

      proc_callExpr* getMethodCall()
      {
        return nodeCall;
      }

      void initUsedVariable(list<Identifier*> usedVars){
      this->usedVars = usedVars;
    }
  };
  
  class unary_stmt:public statement
  {
    private:
    Expression* unaryExpr;

    public:
    unary_stmt()
    {
      unaryExpr=NULL;
      statementType="UnaryStatement";
      typeofNode=NODE_UNARYSTMT;
    }
  
     static unary_stmt* nodeForUnaryStmt(Expression* unaryExpr)
    {
      unary_stmt* unary_stmtNode=new unary_stmt();
      unary_stmtNode->unaryExpr=unaryExpr;

      return unary_stmtNode;
    }

     Expression* getUnaryExpr()
    {
      return unaryExpr;
    }

  };

  
  class proc_callStmt:public statement
  {
    private:
    proc_callExpr* procCall;

    public:
    proc_callStmt()
    {
      procCall=NULL;
      statementType="ProcCallStatement";
      typeofNode=NODE_PROCCALLSTMT;

    }

    static proc_callStmt* nodeForCallStmt(Expression* procCall)
    {
      proc_callStmt* procCallStmtNode=new proc_callStmt();
      procCallStmtNode->procCall=(proc_callExpr*)procCall;

      return procCallStmtNode;
    }

    proc_callExpr* getProcCallExpr()
    {
      return procCall;
    }

};
  class forallStmt:public statement
  {

    private:
    Identifier* iterator;
    Identifier* sourceGraph;
    Identifier* source;
    PropAccess* sourceProp;
    proc_callExpr*  extractElemFunc;
    statement* body;
    Expression* filterExpr;
    bool isSourceId;
    bool isforall;
    map<int,list<Identifier*>> reduction_map;
    set<int> reduc_keys;
    bool filterExprAssoc;
    Expression* assocExpr;
    
    list<Identifier*> usedVars;
    public:
    forallStmt()
    {
      iterator=NULL;
      sourceGraph=NULL;
      extractElemFunc=NULL;
      body=NULL;
      filterExpr=NULL;
      statementType="ForAllStmt";
      typeofNode=NODE_FORALLSTMT;
      isforall=false;
      isSourceId=false;
      createSymbTab();
      filterExprAssoc = false; 
    }

    static forallStmt* createforallStmt(Identifier* iterator,Identifier* sourceGraph,proc_callExpr* extractElemFunc,statement* body,Expression* filterExpr,bool isforall)
    {
      forallStmt* new_forallStmt=new forallStmt();
      new_forallStmt->iterator=iterator;
      new_forallStmt->sourceGraph=sourceGraph;
      new_forallStmt->extractElemFunc=extractElemFunc;
      new_forallStmt->body=body;
      new_forallStmt->filterExpr=filterExpr;
    //  cout<<"FILTEREXPR TYPE IN FORALL"<<filterExpr->getTypeofExpr();
      new_forallStmt->isforall=isforall;
      body->setParent(new_forallStmt);
      return new_forallStmt;
    }
    static forallStmt* createforForStmt(Identifier* iterator,Identifier* source,statement* body,bool isforall)
    {
      forallStmt* new_forallStmt=new forallStmt();
      new_forallStmt->iterator=iterator;
      new_forallStmt->source=source;
      new_forallStmt->body=body;
      new_forallStmt->isforall=isforall;
      body->setParent(new_forallStmt);
      return new_forallStmt;
    }
     static forallStmt* id_createforForStmt(Identifier* iterator,Identifier* source,statement* body,bool isforall)
    {
      forallStmt* new_forallStmt=new forallStmt();
      new_forallStmt->iterator=iterator;
      new_forallStmt->source=source;
      new_forallStmt->body=body;
      new_forallStmt->isforall=isforall;
      new_forallStmt->isSourceId=true;
      body->setParent(new_forallStmt);
      return new_forallStmt;
    }

    static forallStmt* propId_createforForStmt(Identifier* iterator,PropAccess* source,statement* body,bool isforall)
    {
      forallStmt* new_forallStmt=new forallStmt();
      new_forallStmt->iterator=iterator;
      new_forallStmt->sourceProp=source;
      new_forallStmt->body=body;
      new_forallStmt->isforall=isforall;
      body->setParent(new_forallStmt);
      return new_forallStmt;
    }

    bool isForall()
    {
      return isforall;
    }
    
    /* disable parallelization of for
       for specific instances*/
    void disableForall() 
    {                     
      isforall=false;
    }

    proc_callExpr* getExtractElementFunc()
    {
       return extractElemFunc;
    }

    Identifier* getIterator()
    {
      return iterator;
    }

    PropAccess* getPropSource()
    {
      return sourceProp;
    }

    Identifier* getSource()
    {
      return source;
    }

    Identifier* getSourceGraph()
    {
      return sourceGraph;
    }

    bool isSourceField()
    {
      return (!isSourceId);
    }

    bool isSourceProcCall()
    {
      return (extractElemFunc!=NULL);
    }

    bool hasFilterExpr()
    {
      return (filterExpr!=NULL);
    }

    void setAssocExpr(Expression* filterExprSent)
     {
       assocExpr = filterExprSent;
     }

     Expression* getAssocExpr()
       {
          return assocExpr;
       }

     void setFilterExprAssoc()
     {
       filterExprAssoc = true;
     }
    
     bool hasFilterExprAssoc()
      {
        return filterExprAssoc;
      }
    statement* getBody()
    {
      return body;
    }
    void setBody(statement* bodySent)
    {
       body=bodySent;
    }
    Expression* getfilterExpr()
    {
      return filterExpr;
    }

    void push_reduction(int key,Identifier* val)
    {
        reduction_map[key].push_back(val);
        reduc_keys.insert(key);
    }

    set<int> get_reduceKeys()
    {
      return reduc_keys;
    } 
    
    list<Identifier*> get_reduceIds(int key)
    {
      return reduction_map[key];
    }
    
   

    void addAtomicSignalToStatements()
    {
        blockStatement* block=(blockStatement*)body;
        for(statement* stmt:block->returnStatements())
         {
           if(stmt->getTypeofNode()==NODE_ASSIGN)
             {
               std::cout<< "\t\tON: ATOMIC ASST" << '\n';
               assignment* assign=(assignment*)stmt;
               assign->addAtomicSignal();
             }
         }

    }

    void initUsedVariable(list<Identifier*> usedVars){
      this->usedVars = usedVars;
    }
    list<Identifier*> getUsedVariables(){
      return this->usedVars;
    }

    void addDeviceAssignment(){
      blockStatement* block=(blockStatement*)body;
        for(statement* stmt:block->returnStatements())
         {
           if(stmt->getTypeofNode()==NODE_ASSIGN)
             {
               std::cout<< "\t\tON: DEV ASST" << '\n';
               assignment* assign=(assignment*)stmt;
               assign->flagAsDeviceVariable();
             }

             //~ if(stmt->getTypeofNode()==NODE_ASSIGN){
               //~ std::cout<< "ON ACC ASST" << '\n';
               //~ assignment* assign=(assignment*)stmt;
               //~ assign->flagAccumulateKernel();
              //~ }
         }
    }

};
  class reductionCall:public ASTNode
  {
    int reductionType;
     list<argument*> argList;

     public:
     reductionCall()
     {
       reductionType=0;
       typeofNode=NODE_REDUCTIONCALL;
     }
     static reductionCall* nodeForReductionCall(int reduceType,list<argument*> argList)
     {
       reductionCall* reduceC=new reductionCall();
      // cout<<"REDUCTION OPERATION TYPE "<<(reduceType==REDUCE_MIN);
       reduceC->reductionType=reduceType;
       reduceC->argList=argList;
       return reduceC;
     }

     int getReductionType()
     {
       return reductionType;
     }

     list<argument*> getargList()
     {
       return argList;
     }

};

class reductionCallStmt:public statement
  {
     private:
     Identifier* id;
     PropAccess* propAccessId;
     Expression* rightSide;
     list<ASTNode*> leftList;
     list<ASTNode*> exprList;
     reductionCall* reducCall;
     int reduc_op;
     bool is_reduceCall;
<<<<<<< HEAD
=======
     int type;
>>>>>>> 4825f48 (code gen for tc)
    // Expression* exprVal;
     int lhsType;

     public:
     reductionCallStmt()
     {
       id=NULL;
       propAccessId=NULL;
       reducCall=NULL;
       rightSide=NULL;
      // exprVal=NULL;
       typeofNode=NODE_REDUCTIONCALLSTMT;
       type = 0;
     }

     static reductionCallStmt* id_reducCallStmt(Identifier* id,reductionCall* reducCall)
     {
       reductionCallStmt* reducCallStmtNode=new reductionCallStmt();
       reducCallStmtNode->id=id;
       reducCallStmtNode->reducCall=reducCall;
       reducCallStmtNode->lhsType=1;
       reducCallStmtNode->type = 1;
       return reducCallStmtNode;
     }

     static reductionCallStmt* propId_reducCallStmt(PropAccess* propId,reductionCall* reducCall)
     {
       reductionCallStmt* reducCallStmtNode=new reductionCallStmt();
       reducCallStmtNode->propAccessId=propId;
       reducCallStmtNode->reducCall=reducCall;
       reducCallStmtNode->lhsType=2;
       reducCallStmtNode->type = 2;
       return reducCallStmtNode;
     }

     static reductionCallStmt* id_reduc_opStmt(Identifier* id,int reduce_op,Expression* rightSide)
     {
         reductionCallStmt* reducCallStmtNode=new reductionCallStmt();
         reducCallStmtNode->id=id;
         rightSide->setParent(reducCallStmtNode);
         reducCallStmtNode->reduc_op=reduce_op;
         reducCallStmtNode->rightSide=rightSide;
<<<<<<< HEAD
=======
         reducCallStmtNode->type = 3;
>>>>>>> 4825f48 (code gen for tc)
         return reducCallStmtNode;

     }

     static reductionCallStmt* propId_reduc_opStmt(PropAccess* propId,int reduce_op,Expression* rightSide)
     {
         reductionCallStmt* reducCallStmtNode=new reductionCallStmt();
         reducCallStmtNode->propAccessId=propId;
         rightSide->setParent(reducCallStmtNode);
         reducCallStmtNode->reduc_op=reduce_op;
         reducCallStmtNode->rightSide=rightSide;
<<<<<<< HEAD
=======
         reducCallStmtNode->type = 4;
>>>>>>> 4825f48 (code gen for tc)
         return reducCallStmtNode;

     }
      
     static reductionCallStmt* leftList_reducCallStmt(list<ASTNode*> llist,reductionCall* reducCall,list<ASTNode*> exprListSent)
     {
      // cout<<"REDUC CALL TYPE "<<(reducCall->getReductionType()==REDUCE_MIN)<<"\n";
       reductionCallStmt* reducCallStmtNode=new reductionCallStmt();
       reducCallStmtNode->leftList=llist;
<<<<<<< HEAD
=======
       reducCallStmtNode->type = 5;
>>>>>>> 4825f48 (code gen for tc)
       for(ASTNode* node:llist)
        {
          node->setParent(reducCallStmtNode);
        }
       reducCallStmtNode->reducCall=reducCall;
       reducCallStmtNode->lhsType=3;
       reducCallStmtNode->exprList=exprListSent;
        for(ASTNode* node:exprListSent)
        {
          node->setParent(reducCallStmtNode);
        }
       return reducCallStmtNode;
     }

    int getLhsType()
      {
        return lhsType;

      }
    bool isLeftIdentifier()
    {
      return (id!=NULL);
    }

    reductionCall* getReducCall()
    {
      return reducCall;
    }

    Identifier* getLeftId()
    {
      return id;
    }

    PropAccess* getPropAccess()
    {
       return propAccessId;

    }
     bool isListInvolved()
     {
       return (exprList.size()!=0);
     }

    list<ASTNode*> getLeftList()
    {
      return leftList;
    }
    list<ASTNode*> getRightList()
    {
      return exprList;
    }

    Expression* getRightSide()
    {
      return rightSide;
    }

    int reduction_op()
    {
      return reduc_op;
    }

    bool isTargetId()
    {
      ASTNode* node=leftList.front();
      return (node->getTypeofNode()==NODE_ID);

    }
    bool isTargetPropId()
    {
      ASTNode* node=leftList.front();
      return (node->getTypeofNode()==NODE_PROPACCESS);
    }

    Identifier* getTargetId()
    {
       ASTNode* node=leftList.front();
       Identifier* targetId=(Identifier*)node;
       return targetId;
    }

    PropAccess* getTargetPropId()
    {
      ASTNode* node=leftList.front();
      PropAccess* targetPropId=(PropAccess*)node;
      return targetPropId;

    }
    Identifier* getAssignedId()
    {
      ASTNode* node=leftList.front();
      if(node->getTypeofNode()==NODE_ID)
          return (Identifier*)node;
      else
         return ((PropAccess*)node)->getIdentifier2();

    }

    bool is_reducCall()
    {
      if(reducCall!=NULL)
        return true;

      return false;  
    }
<<<<<<< HEAD


};

class varTransferStmt: public statement
{
  public:

  Identifier* transferVar;
  bool direction;
  /* 0 -> CPU to GPU
    1 -> GPU to CPU
  */

  varTransferStmt(Identifier* iden, bool dir)
  {
    this->transferVar = iden;
    this->direction = dir;
    setTypeofNode(NODE_TRANSFERSTMT);
  } 
};
=======
    int get_type()
    {
      return type;
    }


};
>>>>>>> 4825f48 (code gen for tc)
#endif
