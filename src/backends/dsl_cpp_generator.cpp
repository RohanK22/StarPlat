#include "dsl_cpp_generator.h"
#include<string.h>
#include<cassert>

<<<<<<< HEAD
int count = 0;
void dsl_cpp_generator::addIncludeToFile(char* includeName,dslCodePad file,bool isCppLib)
{  
    cout<<"inside addincludeto file"<<endl;
=======



void dsl_cpp_generator::addIncludeToFile(char* includeName,dslCodePad& file,bool isCppLib)
{  //cout<<"ENTERED TO THIS ADD INCLUDE FILE"<<"\n";
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224
    if(!isCppLib)
      file.push('"');
    else 
      file.push('<');  
     
     file.pushString(includeName);
     if(!isCppLib)
       file.push('"');
     else 
       file.push('>');
     file.NewLine();     
 }

void dsl_cpp_generator::generation_begin()
{ 
  char temp[1024];  
  header.pushString("#ifndef GENCPP_");
  header.pushUpper(fileName);
  header.pushstr_newL("_H");
  header.pushString("#define GENCPP_");
   header.pushUpper(fileName);
  header.pushstr_newL("_H");
<<<<<<< HEAD
  header.pushstr_newL("#include<iostream>");
  header.pushstr_newL("#include<cstdlib>");
  header.pushstr_newL("#include<cstdbool>");
  header.pushstr_newL("#include<climits>");
  header.pushstr_newL("#include<fstream>");
  header.pushstr_newL("#include <sys/time.h>");
  header.pushstr_newL("#include\"mpi.h\"");
  header.pushstr_newL("#include\"graph.hpp\"");
  //header.pushstr_newL("#include\"graph.hpp\"");
  header.pushstr_newL("using namespace std;");
  //addIncludeToFile("stdio.h",header,true);
  //addIncludeToFile("stdlib.h",header,true);
  //addIncludeToFile("mpi.h",header,true);
  //header.NewLine();
  sprintf(temp,"#include \"%s.h\" ",fileName);
  main.pushstr_newL(temp);
=======
  header.pushString("#include");
  addIncludeToFile("stdio.h",header,true);
  header.pushString("#include");
  addIncludeToFile("stdlib.h",header,true);
  header.pushString("#include");
  addIncludeToFile("limits.h",header,true);
  header.pushString("#include");
  addIncludeToFile("omp.h",header,true);
  header.pushString("#include");
  addIncludeToFile("graph.hpp",header,false);
  header.NewLine();
  main.pushString("#include");
  sprintf(temp,"%s.h",fileName);
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224
  addIncludeToFile(temp,main,false);
  //main.NewLine();

}

void add_InitialDeclarations(dslCodePad* main,iterateBFS* bfsAbstraction)
{
<<<<<<< HEAD
    if(count == 0)
    {
      count = 1;
      main.pushstr_newL("int my_rank,np,part_size,startv,endv;");
      main.pushstr_newL("int max_degree=g.max_degree();");
      //main.pushstr_newL("MPI_Init(&argc,&argv);");
      main.pushstr_newL("struct timeval start, end;");
      main.pushstr_newL("long seconds,micros;");
      main.pushstr_newL("MPI_Init(NULL,NULL);");
      main.pushstr_newL("MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);");
      main.pushstr_newL("MPI_Comm_size(MPI_COMM_WORLD, &np);");
      main.pushstr_newL("MPI_Request request;");
      main.pushstr_newL("part_size = g.num_nodes()/np;");
      main.pushstr_newL("startv = my_rank*part_size;");
      main.pushstr_newL("endv = startv + (part_size-1);");

    }
   
=======
   
  char strBuffer[1024];
  char* graphId=bfsAbstraction->getGraphCandidate()->getIdentifier();
  sprintf(strBuffer,"std::vector<std::vector<int>> %s(%s.%s()) ;","levelNodes",graphId,"num_nodes");
  main->pushstr_newL(strBuffer);
  sprintf(strBuffer,"std::vector<int>  %s(%s.%s()) ;","levelNodes_later",graphId,"num_nodes");
  main->pushstr_newL(strBuffer);
  sprintf(strBuffer,"std::vector<int>  %s(%s.%s()) ;","levelCount",graphId,"num_nodes");
  main->pushstr_newL(strBuffer);
  main->pushstr_newL("int phase = 0 ;");
  sprintf(strBuffer,"levelNodes[phase].push_back(%s) ;",bfsAbstraction->getRootNode()->getIdentifier());
  main->pushstr_newL(strBuffer);
  sprintf(strBuffer,"std::%s bfsCount = {%s} ;","atomic_int","1");
  main->pushstr_newL(strBuffer);
  main->pushstr_newL("levelCount[phase] = bfsCount;");



}

 void add_BFSIterationLoop(dslCodePad* main, iterateBFS* bfsAbstraction)
 {
   
    char strBuffer[1024];
    char* iterNode=bfsAbstraction->getIteratorNode()->getIdentifier();
    char* graphId=bfsAbstraction->getGraphCandidate()->getIdentifier();
    main->pushstr_newL("while ( bfsCount > 0 )");
    main->pushstr_newL("{");
    main->pushstr_newL(" int prev_count = bfsCount ;");
    main->pushstr_newL("bfsCount = 0 ;");
    main->pushstr_newL("#pragma omp for all");
    sprintf(strBuffer,"for( %s %s = %s; %s = prev_count ; %s++)","int","l","0","l","l");
    main->pushstr_newL(strBuffer);
    main->pushstr_newL("{");
    sprintf(strBuffer,"int %s = levelNodes[phase][%s] ;",iterNode,"l");
    main->pushstr_newL(strBuffer);
    sprintf(strBuffer,"for(%s %s = %s.%s[%s] ; %s < %s.%s[%s+1] ; %s++) ","int","edge",graphId,"indexofNodes",iterNode,"edge",graphId,"indexofNodes",iterNode,"edge");
     main->pushString(strBuffer);
     main->pushstr_newL("{");
     sprintf(strBuffer,"%s %s = %s.%s[%s] ;","int","nbr",graphId,"edgeList","edge");
     main->pushstr_newL(strBuffer);
     main->pushstr_newL("int dnbr ;");
     sprintf(strBuffer,"dnbr = %s(&dist[nbr],-1,dist[%s]+1);","__sync_val_compare_and_swap",iterNode);
     main->pushstr_newL(strBuffer);
     main->pushstr_newL("if (dnbr < 0)");
     main->pushstr_newL("{");
     sprintf(strBuffer,"int %s = bfsCount.fetch_add(%s,%s) ;","loc","1","std::memory_order_relaxed");
     main->pushstr_newL(strBuffer);
     sprintf(strBuffer," levelNodes_later[%s]=nbr ;","loc");
     main->pushstr_newL(strBuffer);
     main->pushstr_newL("}");
     main->pushstr_newL("}");
  
  }

  void add_RBFSIterationLoop(dslCodePad* main, iterateBFS* bfsAbstraction)
  {
   
    char strBuffer[1024];    
    main->pushstr_newL("while (phase > 0)") ;
    main->pushstr_newL("{");
    main->pushstr_newL("#pragma omp parallel for");
    sprintf(strBuffer,"for( %s %s = %s; %s = levelCount[phase] ; %s++)","int","i","0","l","l"); 
    main->pushstr_newL(strBuffer);
    main->pushstr_newL("{");
    sprintf(strBuffer,"int %s = levelCount[phase][i] ;",bfsAbstraction->getIteratorNode()->getIdentifier());
    main->pushstr_newL(strBuffer);



  }

 void dsl_cpp_generator::generateBFSAbstraction(iterateBFS* bfsAbstraction)
 {
   add_InitialDeclarations(&main,bfsAbstraction);
  //printf("BFS ON GRAPH %s",bfsAbstraction->getGraphCandidate()->getIdentifier());
   add_BFSIterationLoop(&main,bfsAbstraction);
   statement* body=bfsAbstraction->getBody();
   assert(body->getTypeofNode()==NODE_BLOCKSTMT);
   blockStatement* block=(blockStatement*)body;
   list<statement*> statementList=block->returnStatements();
   for(statement* stmt:statementList)
   {
       generateStatement(stmt);
   }
   main.pushstr_newL("}");

   main.pushstr_newL("phase = phase + 1 ;");
   main.pushstr_newL("levelCount[phase] = bfsCount ;");
   main.pushstr_newL(" levelNodes[phase].assign(levelNodes_later.begin(),levelNodes_later.begin()+bfsCount);");
   main.pushstr_newL("}");
   main.pushstr_newL("phase = phase -1 ;");
   add_RBFSIterationLoop(&main,bfsAbstraction);
   blockStatement* revBlock=(blockStatement*)bfsAbstraction->getRBFS()->getBody();
   list<statement*> revStmtList=revBlock->returnStatements();

    for(statement* stmt:revStmtList)
    {
       generateStatement(stmt);
    }
   
   main->pushstr_newL("}");
   main->pushstr_newL("}");


 }


void dsl_cpp_generator::generateStatement(statement* stmt)
{  
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224
   if(stmt->getTypeofNode()==NODE_BLOCKSTMT)
     {
       generateBlock((blockStatement*)stmt);

     }
   if(stmt->getTypeofNode()==NODE_DECL)
   {
     
      generateVariableDecl((declaration*)stmt);

   } 
   if(stmt->getTypeofNode()==NODE_ASSIGN)
     { 
       
       generateAssignmentStmt((assignment*)stmt);
     }
    
   if(stmt->getTypeofNode()==NODE_WHILESTMT) 
   {
    // generateWhileStmt((whileStmt*) stmt);
   }
   
   if(stmt->getTypeofNode()==NODE_IFSTMT)
   {
      generateIfStmt((ifStmt*)stmt);
   }

   if(stmt->getTypeofNode()==NODE_DOWHILESTMT)
   {
    //  generateBlock((blockStatement*) stmt);
   }

    if(stmt->getTypeofNode()==NODE_FORALLSTMT)
     {
      generateForAll((forallStmt*) stmt);
     }
  
    if(stmt->getTypeofNode()==NODE_FIXEDPTSTMT)
    {
      generateFixedPoint((fixedPointStmt*)stmt);
    }
    if(stmt->getTypeofNode()==NODE_REDUCTIONCALLSTMT)
<<<<<<< HEAD
    {
=======
    { cout<<"IS REDUCTION STMT HI"<<"\n";
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224
      generateReductionStmt((reductionCallStmt*) stmt);
    }
    if(stmt->getTypeofNode()==NODE_ITRBFS)
    {
      generateBFSAbstraction((iterateBFS*) stmt);
    }
    if(stmt->getTypeofNode()==NODE_PROCCALLSTMT)
    { 
      //cout<<"proc call expr"<<endl;
      generateProcCall((proc_callStmt*) stmt);
    }
    if(stmt->getTypeofNode()==NODE_PROPACCESS)
    { 
      cout<<"prOP ACCESS STATEMENT"<<endl;
      generatePropAccess((PropAccess*) stmt);
    }

}

void dsl_cpp_generator::generateReductionStmt(reductionCallStmt* stmt)
{ char strBuffer[1024];
  reductionCall* reduceCall=stmt->getReducCall();
  if(reduceCall->getReductionType()==REDUCE_MIN)
  {
    
    if(stmt->isListInvolved())
      {
        cout<<"INSIDE THIS OF LIST PRESENT"<<"\n";
        list<argument*> argList=reduceCall->getargList();
        list<ASTNode*>  leftList=stmt->getLeftList();
        int i=0;
      /*  ASTNode* a=leftList.front();
        PropAccess* p=(PropAccess*)a;
        cout<<" a's id 1"<<p->getIdentifier1()->getIdentifier()<<"\n";
        cout<<" a's id 2"<<p->getIdentifier2()->getIdentifier()<<"\n";
        for(ASTNode* l:leftList)
           {
             PropAccess* p=(PropAccess*)l;
             cout<<"ID 1 DSL"<<p->getIdentifier1()->getIdentifier()<<"\n";
             cout<<"ID 2 DSL"<<p->getIdentifier2()->getIdentifier()<<"\n";
           }*/
        list<ASTNode*> rightList=stmt->getRightList();
        printf("LEFT LIST SIZE %d \n",leftList.size());
      
            main.space();
            if(stmt->getAssignedId()->getSymbolInfo()->getType()->isPropType())
            { Type* type=stmt->getAssignedId()->getSymbolInfo()->getType();
              
              main.pushstr_space(convertToCppType(type->getInnerTargetType()));
            }
            cout<<"INSIDE ARG ID"<<stmt->getAssignedId()->getSymbolInfo()->getType()->gettypeId()<<"\n";

          //  
            sprintf(strBuffer,"%s_new",stmt->getAssignedId()->getIdentifier());
            main.pushString(strBuffer);
            list<argument*>::iterator argItr;
             argItr=argList.begin();
             argItr++; 
            main.pushString(" = ");
            generateExpr((*argItr)->getExpr());
            main.pushstr_newL(";");
            main.pushString("if (");
            if(stmt->isTargetId())
            generate_exprIdentifier(stmt->getTargetId());
            else
              generate_exprPropId(stmt->getTargetPropId());
            main.space();
            main.pushstr_space(">");
            main.pushString(strBuffer);
            main.pushstr_newL(")");
            main.pushstr_newL("{");
            list<ASTNode*>::iterator itr1;
            list<ASTNode*>::iterator itr2;
            itr2=rightList.begin();
            itr1=leftList.begin();
            itr1++;
            for( ;itr1!=leftList.end();itr1++)
            {   ASTNode* node=*itr1;
                ASTNode* node1=*itr2;
                
                if(node->getTypeofNode()==NODE_ID)
                    {
                      main.pushstr_space(convertToCppType(((Identifier*)node)->getSymbolInfo()->getType()));
                      sprintf(strBuffer,"%s_new",((Identifier*)node)->getIdentifier());
                      main.pushString(strBuffer);
                      main.pushString(" = ");
                      generateExpr((Expression*)node1);
                    } 
                    if(node->getTypeofNode()==NODE_PROPACCESS)
                    {
                      PropAccess* p=(PropAccess*)node;
                      Type* type=p->getIdentifier2()->getSymbolInfo()->getType();
                      if(type->isPropType())
                      {
                        main.pushstr_space(convertToCppType(type->getInnerTargetType()));
                      }
                      
                      sprintf(strBuffer,"%s_new",p->getIdentifier2()->getIdentifier());
                      main.pushString(strBuffer);
                      main.pushString(" = ");
                      Expression* expr=(Expression*)node1;
                      generateExpr((Expression*)node1);
                      main.pushstr_newL(";");
                    }
                    itr2++;
            }
          if(stmt->isTargetId())
          { 
             Identifier* p=stmt->getTargetId();
            sprintf(strBuffer,"omp_set_lock(&(lock[%s])) ;",stmt->getTargetId()->getIdentifier());
          }
          if(stmt->isTargetPropId())
          {
            sprintf(strBuffer,"omp_set_lock(&(lock[%s])) ;",stmt->getTargetPropId()->getIdentifier1()->getIdentifier());
            
          }
            main.pushstr_newL(strBuffer);

             main.pushString("if (");
            if(stmt->isTargetId())
            generate_exprIdentifier(stmt->getTargetId());
            else
              generate_exprPropId(stmt->getTargetPropId());
            main.space();
            main.pushstr_space(">");
            generate_exprIdentifier(stmt->getAssignedId());
            main.pushString("_new");
            main.pushstr_newL(")");
            main.pushstr_newL("{");
            
            itr1=leftList.begin();
            i=0;
            for( ;itr1!=leftList.end();itr1++)
            {   ASTNode* node=*itr1;

              if(node->getTypeofNode()==NODE_ID)
                    {
                        generate_exprIdentifier((Identifier*)node);
                    }
               if(node->getTypeofNode()==NODE_PROPACCESS)
                {
                  generate_exprPropId((PropAccess*)node);
                } 
                main.space();
                main.pushstr_space("=");
                if(node->getTypeofNode()==NODE_ID)
                    {
                        generate_exprIdentifier((Identifier*)node);
                    }
               if(node->getTypeofNode()==NODE_PROPACCESS)
                {
                  generate_exprIdentifier(((PropAccess*)node)->getIdentifier2());
                } 
                main.pushString("_new");
                main.pushstr_newL(";");    

            }
            main.pushstr_newL("}");
            if(stmt->isTargetId())
             { 
             Identifier* p=stmt->getTargetId();
             sprintf(strBuffer,"omp_unset_lock(&(lock[%s]));",stmt->getTargetId()->getIdentifier());
             }
            if(stmt->isTargetPropId())
             {
            sprintf(strBuffer,"omp_unset_lock(&(lock[%s]));",stmt->getTargetPropId()->getIdentifier1()->getIdentifier());
             }
              main.pushstr_newL(strBuffer);
              main.pushstr_newL("}");
          

      }
  }

}

void dsl_cpp_generator::generateIfStmt(ifStmt* ifstmt)
{ cout<<"INSIDE IF STMT"<<"\n";
  Expression* condition=ifstmt->getCondition();
  main.pushString("if (");
  cout<<"TYPE OF IFSTMT"<<condition->getTypeofExpr()<<"\n";
  generateExpr(condition);
  main.pushString(" )");
  generateStatement(ifstmt->getIfBody());
  if(ifstmt->getElseBody()==NULL)
     return;
  main.pushstr_newL("else");
  generateStatement(ifstmt->getElseBody());   
}

void dsl_cpp_generator::findTargetGraph(vector<Identifier*> graphTypes,Type* type)
{   
    if(graphTypes.size()>1)
    {
      cerr<<"TargetGraph can't match";
    }
    else
    { 
      
      Identifier* graphId=graphTypes[0];
     
      type->setTargetGraph(graphId);
    }
    
    
}

void dsl_cpp_generator::generateAssignmentStmt(assignment* asmt)
{  
   
   if(asmt->lhs_isIdentifier())
   { 
     Identifier* id=asmt->getId();
     main.pushString(id->getIdentifier());
   }
   else if(asmt->lhs_isProp())  //the check for node and edge property to be carried out.
   {
     PropAccess* propId=asmt->getPropId();
     if(asmt->getAtomicSignal())
      { 
        /*if(asmt->getParent()->getParent()->getParent()->getParent()->getTypeofNode()==NODE_ITRBFS)
           if(asmt->getExpr()->isArithmetic()||asmt->getExpr()->isLogical())*/
             main.pushstr_newL("#pragma omp atomic");
           
      }
     main.pushString(propId->getIdentifier2()->getIdentifier());
     main.push('[');
     main.pushString(propId->getIdentifier1()->getIdentifier());
     main.push(']');
     
     
   }

   main.pushString(" = ");
   generateExpr(asmt->getExpr());
   main.pushstr_newL(";");


}


void dsl_cpp_generator::generateProcCall(proc_callStmt* proc_callStmt)
{  cout<<"INSIDE PROCCALL OF GENERATION"<<"\n";
   proc_callExpr* procedure=proc_callStmt->getProcCallExpr();
  cout<<"FUNCTION NAME"<<procedure->getMethodId()->getIdentifier();
   string methodID(procedure->getMethodId()->getIdentifier());
   string IDCoded("attachNodeProperty");
   int x=methodID.compare(IDCoded);
   if(x==0)
       {  
         // cout<<"MADE IT TILL HERE";
          char strBuffer[1024];
          list<argument*> argList=procedure->getArgList();
          list<argument*>::iterator itr;
          //main.pushstr_newL("#pragma omp parallel for");
          sprintf(strBuffer,"for (%s %s = 0; %s < %s; %s ++) ","int","t","t",/*procedure->getId1()->getIdentifier(),*/"g.num_nodes()","t");
          main.pushstr_newL(strBuffer);
          main.pushstr_newL("{");
          for(itr=argList.begin();itr!=argList.end();itr++)
              { 
                assignment* assign=(*itr)->getAssignExpr();
                Identifier* lhsId=assign->getId();
                Expression* exprAssigned=assign->getExpr();
                sprintf(strBuffer,"%s[%s] = ",lhsId->getIdentifier(),"t");
                main.pushString(strBuffer);
                generateExpr(exprAssigned);

                main.pushstr_newL(";");
                
              }
             
        main.pushstr_newL("}");

       }
    
}

void dsl_cpp_generator::generatePropertyDefination(Type* type,char* Id)
{ 
  Type* targetType=type->getInnerTargetType();
  if(targetType->gettypeId()==TYPE_INT)
  {
     main.pushString("=");
     main.pushString(INTALLOCATION);
     main.pushString("[");
    // printf("%d SIZE OF VECTOR",)
    // findTargetGraph(graphId,type);
    
    if(graphId.size()>1)
    {
      cerr<<"TargetGraph can't match";
    }
    else
    { 
      
      Identifier* id=graphId[0];
     
      type->setTargetGraph(id);
    }
     char strBuffer[100];
     sprintf(strBuffer,"%s",/*type->getTargetGraph()->getIdentifier(),*/"g.num_nodes()");
     main.pushString(strBuffer);
     main.pushString("]");
     main.pushstr_newL(";");
  }
  if(targetType->gettypeId()==TYPE_BOOL)
  {
     main.pushString("=");
     main.pushString(BOOLALLOCATION);
     main.pushString("[");
     //findTargetGraph(graphId,type);
     if(graphId.size()>1)
    {
      cerr<<"TargetGraph can't match";
    }
    else
    { 
      
      Identifier* id=graphId[0];
     
      type->setTargetGraph(id);
    }
     char strBuffer[100];
     sprintf(strBuffer,"%s",/*type->getTargetGraph()->getIdentifier(),*/"g.num_nodes()");
     main.pushString(strBuffer);
     main.pushString("]");
     main.pushstr_newL(";");
  }

   if(targetType->gettypeId()==TYPE_FLOAT)
  {
     main.pushString("=");
     main.pushString(FLOATALLOCATION);
     main.pushString("[");
     //findTargetGraph(graphId,type);
     if(graphId.size()>1)
    {
      cerr<<"TargetGraph can't match";
    }
    else
    { 
      
      Identifier* id=graphId[0];
     
      type->setTargetGraph(id);
    }
     char strBuffer[100];
     sprintf(strBuffer,"%s.%s()",type->getTargetGraph()->getIdentifier(),"num_nodes");
     main.pushString(strBuffer);
     main.pushString("]");
     main.pushstr_newL(";");
  }


}

  void dsl_cpp_generator::getDefaultValueforTypes(int type)
  {
    switch(type)
    {
      case TYPE_INT:
      case TYPE_LONG:
          main.pushstr_space(" = 0");
          break;
      case TYPE_FLOAT:
      case TYPE_DOUBLE:
          main.pushstr_space(" = 0.0");
          break;
      case TYPE_BOOL:
          main.pushstr_space(" = false");
       default:
         assert(false);
         return;        
    }

  }
void dsl_cpp_generator::generatePropAccess(PropAccess* stmt)
{
  char strbuf[1024];
  Identifier* id1=stmt->getIdentifier1();
  Identifier* id2=stmt->getIdentifier2();
  cout<<id1->getIdentifier()<<endl;
  cout<<id2->getIdentifier()<<endl;
  sprintf(strbuf,"%s[%s] ",id2->getIdentifier(),id1->getIdentifier());
  main.pushstr_space(strbuf);
}

void dsl_cpp_generator::generateReductionStmt(reductionCallStmt* red)
{ 
  char strbuf[1024];
  cout<<"inside reduction gen"<<endl;
  list<ASTNode*> leftlist = red->getLeftList();
  list<ASTNode*> ::iterator itr;
  reductionCall* reduction=red->getReducCall();
  ASTNode* stmt;
  PropAccess* stmt1;
  Expression* expr = red->getExprVal();
  list<argument*> a = reduction->getargList();
  list<argument*> ::iterator itr1;
  itr=leftlist.begin();
  stmt=*itr;
  stmt1=(PropAccess*) stmt;
  sprintf(strbuf,"if (%s >= startv && %s <= endv)",stmt1->getIdentifier1()->getIdentifier(),stmt1->getIdentifier1()->getIdentifier());
  main.pushstr_newL(strbuf);
  main.pushstr_newL("{");
  main.insert_indent();

      string type="min";
      string red_type = getDefaultValueforReductionTypes(reduction->getReductionType());
      int t = type.compare(red_type);
      if(t==0)
      {
        //based on reduction type - if
        itr1=a.begin();
        //itr1++;
        argument* arg = *itr1;
        Expression *e= arg->getExpr();
        //sprintf(strbuf,"if (%s[%s] > ",stmt1->getIdentifier2()->getIdentifier(),stmt1->getIdentifier1()->getIdentifier());
        //main.pushString(strbuf);
        main.pushString("if (");
        generateExpr(e);
        main.pushString(">");
        itr1++;
        arg = *itr1;
        e= arg->getExpr();
        generateExpr(e);
        main.pushstr_newL(")");

        //statement 1
        main.pushstr_newL("{");
        main.insert_indent();
        //sprintf(strbuf,"%s[%s] = ",stmt1->getIdentifier2()->getIdentifier(),stmt1->getIdentifier1()->getIdentifier());
        //main.pushString(strbuf);
        generateStatement((statement*) stmt1);
        main.pushString("=");
        generateExpr(e);
        main.pushstr_newL(";");
        
        //statement 2
        advance(itr, 1);
        stmt=*itr;
        stmt1=(PropAccess*) stmt;
        //sprintf(strbuf," %s[%s] = ",stmt1->getIdentifier2()->getIdentifier(),stmt1->getIdentifier1()->getIdentifier());
        //main.pushString(strbuf);
        generateStatement((statement*) stmt1);
        main.pushString("=");
        generateExpr(expr);
        main.pushstr_newL(";");
        main.decrease_indent();
        main.pushstr_newL("}");
    main.decrease_indent();
    main.pushstr_newL("}");

        //else part
        main.pushstr_newL("else");
        main.pushstr_newL("{");
        main.insert_indent();
        main.pushstr_newL("dest_pro = nbr / part_size;");
        main.pushstr_newL("count[dest_pro]=count[dest_pro]+1;");
        main.pushstr_newL("int p=pos[dest_pro];");
        main.pushstr_newL("send_data[dest_pro*3*max_degree] = count[dest_pro];");
        main.pushstr_space("send_data[dest_pro*3*max_degree+p] = ");
        generateExpr(e->getLeft());
        main.pushstr_newL(";");
        main.pushstr_newL("send_data[dest_pro*3*max_degree+(p+1)] = nbr;");
        main.pushstr_space("//send_data[dest_pro*3*max_degree+(p+2)] = ");
        generateExpr(e->getLeft());
        main.pushstr_newL(";");
        main.pushstr_newL("send_data[dest_pro*3*max_degree+(p+2)] = wt;");
        main.pushstr_newL("pos[dest_pro] = pos[dest_pro]+3;");
        main.decrease_indent();
        main.pushstr_newL("}");
      }
  //main.decrease_indent();
  //main.pushstr_newL("}");
        
}

void dsl_cpp_generator::generateInnerReductionStmt(reductionCallStmt* red)
{ 
  char strbuf[1024];
  cout<<"inside reduction gen outside"<<endl;
  list<ASTNode*> leftlist = red->getLeftList();
  list<ASTNode*> ::iterator itr;
  reductionCall* reduction=red->getReducCall();
  ASTNode* stmt;
  PropAccess* stmt1;
  Expression* expr = red->getExprVal();
  list<argument*> a = reduction->getargList();
  list<argument*> ::iterator itr1;
  itr=leftlist.begin();
  stmt=*itr;
  stmt1=(PropAccess*) stmt;
  //sprintf(strbuf,"if (%s >= startv && %s <= endv)",stmt1->getIdentifier1()->getIdentifier(),stmt1->getIdentifier1()->getIdentifier());
  //main.pushstr_newL(strbuf);
  //main.pushstr_newL("{");
  //main.insert_indent();

      
      string type="min";
      string red_type = getDefaultValueforReductionTypes(reduction->getReductionType());
      int t = type.compare(red_type);
      if(t==0)
      {
        //based on reduction type - if
        itr1=a.begin();
        itr1++;
        argument* arg = *itr1;
        Expression *e= arg->getExpr();
        PropAccess* p =(PropAccess*) e->getLeft()->getPropId();
        //Initialization
        
        sprintf(strbuf,"int %s_%s = recv_data[(t*3*max_degree)+k],nbr = recv_data[(t*3*max_degree)+k+1],wt = recv_data[(t*3*max_degree)+k+2];",p->getIdentifier2()->getIdentifier(),p->getIdentifier1()->getIdentifier());
        main.pushstr_newL(strbuf);
        //sprintf(strbuf,"if (%s[%s] > ",stmt1->getIdentifier2()->getIdentifier(),stmt1->getIdentifier1()->getIdentifier());
        //main.pushString(strbuf);
        main.pushString("if (");
        //generateStatement((statement*) stmt1);
        itr1--;
        arg = *itr1;
        e= arg->getExpr();
        generateExpr(e);
        main.pushString(">");
        itr1++;
        arg = *itr1;
        e= arg->getExpr();
        p =(PropAccess*) e->getLeft()->getPropId();
        sprintf(strbuf,"%s_%s",p->getIdentifier2()->getIdentifier(),p->getIdentifier1()->getIdentifier());
        main.pushstr_space(strbuf);
        const char* operatorString=getOperatorString(e->getOperatorType());
        main.pushstr_space(operatorString);
        generateExpr(e->getRight());
        main.pushstr_newL(")");

        main.pushstr_newL("{");
        main.insert_indent();

        //Statement 1
       // sprintf(strbuf,"%s[%s] = ",stmt1->getIdentifier2()->getIdentifier(),stmt1->getIdentifier1()->getIdentifier());
       // main.pushString(strbuf);
        generateStatement((statement*) stmt1);
        main.pushString("=");
        p =(PropAccess*) e->getLeft()->getPropId();
        sprintf(strbuf,"%s_%s",p->getIdentifier2()->getIdentifier(),p->getIdentifier1()->getIdentifier());
        main.pushstr_space(strbuf);
        operatorString=getOperatorString(e->getOperatorType());
        main.pushstr_space(operatorString);
        generateExpr(e->getRight());
        main.pushstr_newL(";");
        
        //Statement 2
        advance(itr, 1);
        stmt=*itr;
        stmt1=(PropAccess*) stmt;
        //sprintf(strbuf," %s[%s] = ",stmt1->getIdentifier2()->getIdentifier(),stmt1->getIdentifier1()->getIdentifier());
        //main.pushString(strbuf);
        generateStatement((statement*) stmt1);
        main.pushString("=");
        generateExpr(expr);
        main.pushstr_newL(";");
        main.decrease_indent();
        main.pushstr_newL("}");
       
      }
  //main.decrease_indent();
  //main.pushstr_newL("}");
        
}


char* dsl_cpp_generator::getDefaultValueforReductionTypes(int type)
  {
    char strbuf[1024];
    switch(type)
    {
      case REDUCE_SUM:
        return "sum";
      case REDUCE_COUNT:
        return "count";
          //main.pushstr_space(" = 0");
          //break;
      case REDUCE_PRODUCT:
        return "product";
      case REDUCE_MAX:
        return "max";
          //main.pushstr_space(" = 0.0");
         // break;
      case REDUCE_MIN:
          return ("min");
          //break;
         // main.pushstr_space(" = false");
       default:
         assert(false);
         return "NA"; 
    
           
    }

  }

<<<<<<< HEAD
//............outer forall...........
void dsl_cpp_generator::generateForAll(forallStmt* forAll)
{
   Identifier* iterator;
   Identifier* source; 
   
   blockStatement* body = forAll->getBody();
  
   Expression *filter = forAll->getFilter();
   char strBuffer[1024];
   if(forAll->getIsForAll())
   {
     cout<<"forall"<<endl;
     iterator = forAll->getIdentifier();
     source = forAll -> getSource();
     //cout<<iterator<<end;
     //cout<<forAll->getSource()<<end;
      //main.pushString(iterator->getIdentifier());
      sprintf(strBuffer,"for (%s %s = startv; %s <= endv; %s ++) ","int",iterator->getIdentifier(),iterator->getIdentifier(),iterator->getIdentifier());
     // main.pushstr_newL("for (int i = startv; i < endv; i ++) ");
     main.pushstr_newL(strBuffer);
      main.pushstr_newL("{");
      main.insert_indent();
      main.pushstr_newL("int *count = new int[np];");
      main.pushstr_newL("int *pos = new int[np];");
      main.pushstr_newL("int *send_data = new int[np*3*max_degree];");
      main.pushstr_newL("int *recv_data = new int[np*3*max_degree];");
      main.pushstr_newL("int dest_pro;");
      main.pushstr_newL("for (int tem=0; tem<np; tem++)");
      main.pushstr_newL("{");
      main.insert_indent();
      main.pushstr_newL("count[tem]=0;");
      main.pushstr_newL("pos[tem]=1;"); 
      main.decrease_indent();
      main.pushstr_newL("}");
      main.pushstr_newL("for (int tem=0; tem<np*3*max_degree; tem++)");
      main.pushstr_newL("{");
      main.insert_indent();
      main.pushstr_newL("send_data[tem]=0;");
      main.pushstr_newL("recv_data[tem]=0;"); 
      main.decrease_indent();
      main.pushstr_newL("}");
      main.pushstr_newL("//MPI_Win window;");
      main.pushstr_newL("//MPI_Win_create(send_data, (sizeof(int)*4*max_degree*np), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &window);");
      if (filter!=NULL)
      {
        cout<<"filter"<<endl;
       // Expression* filter = forAll->getFilter();
        cout<<filter->getOperatorType()<<endl;
        cout<<filter->getTypeofExpr()<<endl;
        Expression* left = filter->getLeft();
        cout<<left->getTypeofExpr()<<endl;
        
        Expression* right = filter->getRight();
        cout<<right->getTypeofExpr()<<endl;
        
        //list <statement*> stmt = body->returnStatements();
              
     // }
        main.pushstr_space("if (");
        generate_exprRelational(filter);
        main.pushstr_newL(")");
        bool neg = false;
        if(filter -> getRight() -> isBoolConstant())
        {
          bool val = filter -> getRight()->getBooleanConstant();
          if(val == neg)
            neg = true;
        }
        list<statement*> stmtList=body->returnStatements();
        main.pushstr_newL("{");
        main.insert_indent();
        generateExpr(filter -> getLeft());
        sprintf(strBuffer,"= %d;",neg);
        main.pushstr_newL(strBuffer);
        // if(stmtList == NULL)
          //cout<<"The block contains no statement"<<endl;
          //else{
            list<statement*> ::iterator itr;
            for(itr=stmtList.begin();itr!=stmtList.end();itr++)
            {
              statement* stmt=*itr;
              cout<<"Gen block forall contains"<<stmt->getType()<<endl;
              if(stmt->getTypeofNode()==NODE_FORALLSTMT)
              {
                generateInnerForAll((forallStmt*) stmt);
              }
              else{
                generateStatement(stmt);
              }
            }
          main.decrease_indent();
          main.pushstr_newL("}");
      }
      else
      {
        
        list<statement*> stmtList=body->returnStatements();
        // if(stmtList == NULL)
          //cout<<"The block contains no statement"<<endl;
          //else{
            list<statement*> ::iterator itr;
                  for(itr=stmtList.begin();itr!=stmtList.end();itr++)
            {
              statement* stmt=*itr;
              cout<<"Gen block forall contains"<<stmt->getType()<<endl;
              if(stmt->getTypeofNode()==NODE_FORALLSTMT)
              {
                generateInnerForAll((forallStmt*) stmt);
              }
              else{
                generateStatement(stmt);
              }
            }
      }

    //Code generation for sending data
    main.pushstr_newL("/*");
    main.pushstr_newL("for (int temp=0;temp<np;temp++)");
    main.pushstr_newL("{");
    main.insert_indent();
    main.pushstr_newL("if(temp != my_rank)");
    main.insert_indent();
    main.pushstr_newL("MPI_Isend(send_data+temp*4*max_degree, 1, MPI_INT, temp, 0, MPI_COMM_WORLD, &request);");
    main.pushstr_newL("if(send_data[temp*4*max_degree] > 0)");    
       
        main.insert_indent();
          main.pushstr_newL("MPI_Isend(send_data+(temp*4*max_degree+1), send_data[temp*4*max_degree]*4, MPI_INT, temp, 1, MPI_COMM_WORLD, &request);");
        main.decrease_indent();
      main.decrease_indent();
    main.decrease_indent();
    main.pushstr_newL("}");
    main.pushstr_newL("MPI_Barrier(MPI_COMM_WORLD);");
    main.pushstr_newL("*/");
    main.pushstr_newL("MPI_Alltoall(send_data,3*max_degree,MPI_INT,recv_data,3*max_degree,MPI_INT,MPI_COMM_WORLD);");

    //Code generation for receiving data
    main.pushstr_newL("//MPI_Win_fence(0, window);");
    main.pushstr_newL("for(int t=0;t<np;t++)");
    main.pushstr_newL("{");
    main.insert_indent();
        main.pushstr_newL("if(t != my_rank)");
        main.pushstr_newL("{");
        main.insert_indent();
            main.pushstr_newL("int amount = recv_data[t*3*max_degree];");
            main.pushstr_newL("//MPI_Get(&amount, 1, MPI_INT, t, (my_rank*4*max_degree), 1, MPI_INT, window);");
            main.pushstr_newL("//MPI_Recv(&amount, 1, MPI_INT, t, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);");
            main.pushstr_newL("if(amount>0)");
            main.pushstr_newL("{");
            main.insert_indent();
              main.pushstr_newL("//int *buffer = new int [4*amount];");
              main.pushstr_newL(("//MPI_Recv(buffer, (amount*4), MPI_INT, t, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);"));
              main.pushstr_newL("//MPI_Get(buffer, (amount*4), MPI_INT, t, (my_rank*4*max_degree+1), (amount*4), MPI_INT, window);");
              main.pushstr_newL("for(int k=1;k<(amount*3);k+=3)");
              main.pushstr_newL("{");
                  main.insert_indent();
                 // main.pushstr_newL("int data = buffer[k],nbr = buffer[k+1],src = buffer[k+2],wt = buffer[k+3];");
                    //forAll->getBody()->returnStatements()->
                    list<statement*> stmtList=body->returnStatements();
                // if(stmtList == NULL)
                  //cout<<"The block contains no statement"<<endl;
                  //else{
                    list<statement*> ::iterator itr;
                    itr = stmtList.begin();
                    statement* stmt = *itr;
                    if(stmt->getTypeofNode()==NODE_FORALLSTMT)
                    {   
                        forallStmt* f = (forallStmt*)(stmt);
                        blockStatement* b = f->getBody(); 
                        list<statement*> sList=b->returnStatements();
                        list<statement*> ::iterator ptr;
                        for(ptr=sList.begin();ptr!=sList.end();ptr++)
                        {
                          statement* s1 = *ptr;
                          if(s1->getTypeofNode()==NODE_REDUCTIONCALLSTMT)
                          {
                            generateInnerReductionStmt((reductionCallStmt*) s1);
                          }
                        }
                    }
                 main.decrease_indent();
                main.pushstr_newL("}");
              main.decrease_indent();
              //main.pushstr_newL("delete [] buffer;");
              main.pushstr_newL("}");
          main.decrease_indent();
          main.pushstr_newL("}");
      main.decrease_indent();
      main.pushstr_newL("}");
      main.pushstr_newL("//MPI_Win_fence(0, window);");
      main.pushstr_newL("MPI_Barrier(MPI_COMM_WORLD);");
      main.pushstr_newL("delete [] send_data;");
      main.pushstr_newL("delete [] recv_data;");
      main.pushstr_newL("delete [] count;");
      main.pushstr_newL("delete [] pos;");
    main.decrease_indent();
    main.pushstr_newL("}"); 
     

   }
      

}  
//Inner for all
void dsl_cpp_generator::generateInnerForAll(forallStmt* forAll)
{   
  cout<<"inner fora ll"<<endl;
   Identifier* iterator;
   Identifier* source; 
   blockStatement* body = forAll->getBody();
   proc_callExpr* proc_call = forAll->getProcCallExpr();
   Expression *filter = forAll->getFilter();
   char strBuffer[1024];
   if(forAll->getIsForAll())
   {
     cout<<"forall"<<endl;
     iterator = forAll->getIdentifier();
     source = forAll -> getSource();
     list<argument*> arg = proc_call->getArgList();
     list<argument*> ::iterator itr;
     itr=arg.begin();
     argument* a = *itr;
     a->getExpr()->getId()->getIdentifier();
    // generateExpr(a->getExpr());
     // sprintf(strBuffer,"*for (int j = g.indexofNodes[v]; j<g.indexofNodes[v+1]; j ++) ");
      sprintf(strBuffer,"for (int j = g.indexofNodes[%s]; j<g.indexofNodes[%s+1]; j ++)",a->getExpr()->getId()->getIdentifier(),a->getExpr()->getId()->getIdentifier());
      main.pushstr_newL(strBuffer);
      main.pushstr_newL("{");
      main.insert_indent();
      sprintf(strBuffer,"int %s=g.edgeList[j];",iterator->getIdentifier()/*,a->getExpr()->getId()->getIdentifier()*/);
      main.pushstr_newL(strBuffer);
     // sprintf(strBuffer,"int wt = weight[%s]",iterator->getIdentifier());
     // main.pushstr_newL(strBuffer);
        //sprintf(strBuffer,"if (%s >= startv && %s <= endv)",iterator->getIdentifier(),iterator->getIdentifier());
        //main.pushstr_newL(strBuffer);
        //main.pushstr_newL("{");
        //main.insert_indent();
      //sprintf(strBuffer,"int %s=g.edgeList[%s]",iterator->getIdentifier(),a->getExpr()->getId()->getIdentifier());
      //main.pushstr_newL("#int wt = weight[j];//need based on getedge");
      if (filter!=NULL)
      {
        cout<<"filter"<<endl;
       // Expression* filter = forAll->getFilter();
        cout<<filter->getOperatorType()<<endl;
        cout<<filter->getTypeofExpr()<<endl;
        Expression* left = filter->getLeft();
        cout<<left->getTypeofExpr()<<endl;
        //cout<<left->
        Expression* right = filter->getRight();
        cout<<right->getTypeofExpr()<<endl;
        main.pushstr_newL("//modified[v] =0");
        //list <statement*> stmt = body->returnStatements();
              
     // }
        main.pushstr_space("if (");
        
        main.pushstr_newL(")");
        list<statement*> stmtList=body->returnStatements();
        // if(stmtList == NULL)
          //cout<<"The block contains no statement"<<endl;
          //else{
            list<statement*> ::iterator itr;
                  for(itr=stmtList.begin();itr!=stmtList.end();itr++)
            {
              statement* stmt=*itr;
              cout<<"Gen block forall contains"<<stmt->getType()<<endl;
              if(stmt->getTypeofNode()==NODE_FORALLSTMT)
              {
                generateInnerForAll((forallStmt*) stmt);
              }
              else{
                generateStatement(stmt);
              }
            }
            main.decrease_indent();
            main.pushstr_newL("}");
      }
      else
      {
        cout<< "no filter"<<endl;
        list<statement*> stmtList=body->returnStatements();
        // if(stmtList == NULL)
          //cout<<"The block contains no statement"<<endl;
          //else{
            list<statement*> ::iterator itr;
                  for(itr=stmtList.begin();itr!=stmtList.end();itr++)
            {
              statement* stmt=*itr;
              cout<<"Gen block forall contains"<<stmt->getType()<<endl;
              if(stmt->getTypeofNode()==NODE_FORALLSTMT)
              {
                generateInnerForAll((forallStmt*) stmt);
              }
              else{
                generateStatement(stmt);
              }
            }
            
      }
        //main.decrease_indent();
        //main.pushstr_newL("}");
      main.decrease_indent();
      main.pushstr_newL("}");
      

   }
      

}

=======
void dsl_cpp_generator::generateForAll_header()
{
  main.pushstr_newL("#pragma omp parallel for"); //This needs to be changed to checked for 'For' inside a parallel section.

}

bool dsl_cpp_generator::allGraphIteration(char* methodId)
{
   string methodString(methodId);
   
   return (methodString=="nodes"||methodString=="edges");


}

bool dsl_cpp_generator::neighbourIteration(char* methodId)
{
  string methodString(methodId);
   return (methodString=="neighbours");
}

void dsl_cpp_generator::generateForAllSignature(forallStmt* forAll)
{
   char strBuffer[1024];
  Identifier* iterator=forAll->getIterator();
  if(forAll->isSourceProcCall())
  {
    Identifier* sourceGraph=forAll->getSourceGraph();
    proc_callExpr* extractElemFunc=forAll->getExtractElementFunc();
    Identifier* iteratorMethodId=extractElemFunc->getMethodId();
    if(allGraphIteration(iteratorMethodId->getIdentifier()))
    {
      char* graphId=sourceGraph->getIdentifier();
      char* methodId=iteratorMethodId->getIdentifier();
      string s(methodId);
      if(s.compare("nodes")==0)
      {
        cout<<"INSIDE NODES VALUE"<<"\n";
      sprintf(strBuffer,"for (%s %s = 0; %s < %s.%s(); %s ++) ","int",iterator->getIdentifier(),iterator->getIdentifier(),graphId,"num_nodes",iterator->getIdentifier());
      }
      else
      sprintf(strBuffer,"for (%s %s = 0; %s < %s.%s(); %s ++) ","int",iterator->getIdentifier(),iterator->getIdentifier(),graphId,"num_edges",iterator->getIdentifier());

      main.pushstr_newL(strBuffer);

    }
    else if(neighbourIteration(iteratorMethodId->getIdentifier()))
    { 
      
       char* graphId=sourceGraph->getIdentifier();
       char* methodId=iteratorMethodId->getIdentifier();
       list<argument*>  argList=extractElemFunc->getArgList();
       assert(argList.size()==1);
       Identifier* nodeNbr=argList.front()->getExpr()->getId();
       sprintf(strBuffer,"for (%s %s = %s.%s[%s]; %s < %s.%s[%s+1]; %s ++) ","int","edge",graphId,"indexofNodes",nodeNbr->getIdentifier(),"edge",graphId,"indexofNodes",nodeNbr->getIdentifier(),"edge");
       main.pushstr_newL(strBuffer);
       main.pushString("{");
       sprintf(strBuffer,"%s %s = %s.%s[%s] ;","int",iterator->getIdentifier(),graphId,"edgeList","edge"); //needs to move the addition of
       main.pushstr_newL(strBuffer);                                                                                                    //statement to a different method.

    }
  }




}

blockStatement* dsl_cpp_generator::includeIfToBlock(forallStmt* forAll)
{ 
   
  Expression* filterExpr=forAll->getfilterExpr();
  statement* body=forAll->getBody();
  if(filterExpr->getExpressionFamily()==EXPR_RELATIONAL)
  {
    Expression* expr1=filterExpr->getLeft();
    Expression* expr2=filterExpr->getRight();
    if(expr1->isPropIdExpr()&&expr2->isBooleanLiteral()) //specific to sssp. Need to revisit again to change it.
    {   PropAccess* propId=expr1->getPropId();
        bool value=expr2->getBooleanConstant();
        Expression* exprBool=(Expression*)Util::createNodeForBval(!value);
        assignment* assign=(assignment*)Util::createAssignmentNode(propId,exprBool);
        if(body->getTypeofNode()==NODE_BLOCKSTMT)
        {
          blockStatement* newbody=(blockStatement*)body;
          newbody->addToFront(assign);
          body=newbody;
        }
    }

  }
  ifStmt* ifNode=(ifStmt*)Util::createNodeForIfStmt(filterExpr,forAll->getBody(),NULL);
  blockStatement* newBlock=new blockStatement();
  newBlock->setTypeofNode(NODE_BLOCKSTMT);
  newBlock->addStmtToBlock(ifNode);
  return newBlock;

  
}

void dsl_cpp_generator::generateForAll(forallStmt* forAll)
{ 
   proc_callExpr* extractElemFunc=forAll->getExtractElementFunc();
    Identifier* iteratorMethodId=extractElemFunc->getMethodId();
    statement* body=forAll->getBody();
  if(forAll->isForall())
  {
    generateForAll_header();
  }

  generateForAllSignature(forAll);

  if(forAll->hasFilterExpr())
  { 
    blockStatement* changedBody=includeIfToBlock(forAll);
   // cout<<"CHANGED BODY TYPE"<<(changedBody->getTypeofNode()==NODE_BLOCKSTMT);
    forAll->setBody(changedBody);
   // cout<<"FORALL BODY TYPE"<<(forAll->getBody()->getTypeofNode()==NODE_BLOCKSTMT);
  }
   
  if(neighbourIteration(iteratorMethodId->getIdentifier()))
  { 
   
    if(forAll->getParent()->getParent()->getTypeofNode()==NODE_ITRBFS)
     {   
        
         char strBuffer[1024];
         list<argument*>  argList=extractElemFunc->getArgList();
         assert(argList.size()==1);
         Identifier* nodeNbr=argList.front()->getExpr()->getId();
         sprintf(strBuffer,"if(bfsDist[%s]==bfsDist[%s]+1)",forAll->getIterator()->getIdentifier(),nodeNbr->getIdentifier());
         main.pushstr_newL(strBuffer);
         main.pushstr_newL("{");
       
     }

     /* This can be merged with above condition through or operator but separating 
        both now, for any possible individual construct updation.*/

      if(forAll->getParent()->getParent()->getTypeofNode()==NODE_ITRRBFS)
       {  

         char strBuffer[1024];
         list<argument*>  argList=extractElemFunc->getArgList();
         assert(argList.size()==1);
         Identifier* nodeNbr=argList.front()->getExpr()->getId();
         sprintf(strBuffer,"if(bfsDist[%s]==bfsDist[%s]+1)",forAll->getIterator()->getIdentifier(),nodeNbr->getIdentifier());
         main.pushstr_newL(strBuffer);
         main.pushstr_newL("{");

       }

     generateBlock((blockStatement*)body,false);
     if(forAll->getParent()->getParent()->getTypeofNode()==NODE_ITRBFS||forAll->getParent()->getParent()->getTypeofNode()==NODE_ITRRBFS)
       main.pushstr_newL("}");
    main.pushstr_newL("}");
  }
  else
  { 
    cout<<"ENTER INSIDE FOR NORMAL ITER FORALL"<<"\n";
    cout<<iteratorMethodId->getIdentifier()<<"\n";
    generateStatement(forAll->getBody());
  }

} 
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224

void dsl_cpp_generator:: generateVariableDecl(declaration* declStmt)
{
   Type* type=declStmt->getType();
   //bool value=type->gettypeId()==TYPE_BOOL;
   
   if(type->isPropType())
   {   
     if(type->getInnerTargetType()->isPrimitiveType())
     { 
       Type* innerType=type->getInnerTargetType();
       //printf("%s CPP VALUE",convertToCppType(innerType));
       main.pushString(convertToCppType(innerType)); //convertToCppType need to be modified.
       main.pushString("*");
       main.space();
       main.pushString(declStmt->getdeclId()->getIdentifier());
       generatePropertyDefination(type,declStmt->getdeclId()->getIdentifier());
     }
   }
   else if(type->isPrimitiveType())
   { 
     main.pushstr_space(convertToCppType(type));
     main.pushString(declStmt->getdeclId()->getIdentifier());
     if(declStmt->isInitialized())
     {
       main.pushString(" = ");
       generateExpr(declStmt->getExpressionAssigned());
       main.pushstr_newL(";");
     }
     else
     {
        main.pushString(" = ");
        getDefaultValueforTypes(type->gettypeId());
        main.pushstr_newL(";");
     }
     


   }
   else if(type->isEdgeType())
   {
     cout<<"edge type"<<endl;
     if(declStmt->isInitialized())
        generateExpr(declStmt->getExpressionAssigned());
   }
   else if(type->isNodeEdgeType())
        {
          main.pushstr_space(convertToCppType(type));
          main.pushString(declStmt->getdeclId()->getIdentifier());
          if(declStmt->isInitialized())
           {
              main.pushString(" = ");
             generateExpr(declStmt->getExpressionAssigned());
             main.pushstr_newL(";");
           }
        }



}

void dsl_cpp_generator::generate_exprLiteral(Expression* expr)
     {  
      
        char valBuffer[1024];
       
           int expr_valType=expr->getExpressionFamily();
          
           switch(expr_valType)
           { 
             case EXPR_INTCONSTANT:
                sprintf(valBuffer,"%ld",expr->getIntegerConstant());
                break;
            
             case EXPR_DOUBLECONSTANT:
                sprintf(valBuffer,"%lf",expr->getFloatConstant());
                break;
             case EXPR_BOOLCONSTANT:
                 sprintf(valBuffer,"%s",expr->getBooleanConstant()?"true":"false");
                 break;
             default:
              assert(false);
            
             // return valBuffer;       
           } 
           
           // printf("VALBUFFER %s",valBuffer);
            main.pushString(valBuffer); 
         
      

     }

 void dsl_cpp_generator::generate_exprInfinity(Expression* expr)
 {
              char valBuffer[1024];
              if(expr->getTypeofExpr()!=NULL)
                   {
                     int typeClass=expr->getTypeofExpr();
                     switch(typeClass)
                     {
                       case TYPE_INT:
                         sprintf(valBuffer,"%s",expr->isPositiveInfinity()?"INT_MAX":"INT_MIN");
                        break;
                       case TYPE_LONG:
                           sprintf(valBuffer,"%s",expr->isPositiveInfinity()?"LLONG_MAX":"LLONG_MIN");
                        break;
                       case TYPE_FLOAT:
                            sprintf(valBuffer,"%s",expr->isPositiveInfinity()?"FLT_MAX":"FLT_MIN");
                        break;  
                       case TYPE_DOUBLE:
                            sprintf(valBuffer,"%s",expr->isPositiveInfinity()?"DBL_MAX":"DBL_MIN");
                        break;
                        default:
                            sprintf(valBuffer,"%s",expr->isPositiveInfinity()?"INT_MAX":"INT_MIN");
                        break;


                      }
                          
                   }
                   else
                 
                   {
                 sprintf(valBuffer,"%s",expr->isPositiveInfinity()?"INT_MAX":"INT_MIN");
                   }    
                 
                 main.pushString(valBuffer);

    }    
  
  
  const char* dsl_cpp_generator::getOperatorString(int operatorId)
  {  
    switch(operatorId)
    {
      case OPERATOR_ADD:
       return "+";
      case OPERATOR_DIV:
       return "/";
      case OPERATOR_MUL:
       return "*";
      case OPERATOR_MOD:
       return "%";
      case OPERATOR_SUB:
       return "-";
      case OPERATOR_EQ:
       return "==";
      case OPERATOR_NE:
       return "!=";
      case OPERATOR_LT:
       return "<";
      case OPERATOR_LE:
       return "<=";
      case OPERATOR_GT:
      return ">";
      case OPERATOR_GE:
      return ">=";
      case OPERATOR_AND:
      return "&&";
      case OPERATOR_OR:
      return "||";
      case OPERATOR_COLON:
      return ":";
      default:
      return "NA";         
    }

    
  }

  void  dsl_cpp_generator::generate_exprRelational(Expression* expr)
  {
    cout<<"INSIDE RELATIONAL EXPR"<<"\n";
    
    generateExpr(expr->getLeft());
    
    main.space();
    const char* operatorString=getOperatorString(expr->getOperatorType());
    main.pushstr_space(operatorString);
    generateExpr(expr->getRight());
  }

<<<<<<< HEAD
  void  dsl_cpp_generator::generate_exprDependent(Expression* expr)
  {
    cout<<"INSIDE DEPENDENT EXPR"<<"\n";
    if(expr->getLeft()->isIdentifierExpr() && expr->getRight()->isIdentifierExpr() )
    {
      char buf[1024];
      sprintf(buf,"is_%s (startv,endv,%s)",expr->getLeft()->getId()->getIdentifier(),expr->getRight()->getId()->getIdentifier());
      main.pushString(buf);
      sprintf(buf,"bool is_%s (int startv,int endv,bool* %s)",expr->getLeft()->getId()->getIdentifier(),expr->getRight()->getId()->getIdentifier());
      header.pushstr_newL(buf);
      header.pushstr_newL("{");
      header.insert_indent();
      header.pushstr_newL("int sum,res=0;");
      header.pushstr_newL("for(int i=startv;i<=endv;i++)");
        header.pushstr_newL("{");
        header.insert_indent();
        sprintf(buf,"if (%s[i] == 1)",expr->getRight()->getId()->getIdentifier());
        header.pushstr_newL(buf);
        header.insert_indent();
        header.pushstr_newL("res=1;");
        header.decrease_indent();
        header.pushstr_newL("}");
      header.pushstr_newL("MPI_Allreduce(&res, &sum, 1, MPI_INT, MPI_SUM,MPI_COMM_WORLD);");
      header.pushstr_newL("if(sum ==0) return false;");
      header.pushstr_newL("else return true;");
      header.decrease_indent();
      header.pushstr_newL("}");

    }
    //generateExpr(expr->getLeft());
    //main.space();
    //const char* operatorString=getOperatorString(expr->getOperatorType());
    //main.pushstr_space(operatorString);
    //generateExpr(expr->getRight());
  }

void dsl_cpp_generator::generate_exprIdentifier(Expression* expr)
{
  main.pushString(expr->getId()->getIdentifier());
=======
void dsl_cpp_generator::generate_exprIdentifier(Identifier* id)
{
   main.pushString(id->getIdentifier());
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224
}

void dsl_cpp_generator::generate_exprProcCall(Expression* expr)
{
  if(expr->isProcCallExpr())
  {
 // cout<<"Methods id is"<<expr->getTypeofExpr()<<endl;
  proc_callExpr* p = (proc_callExpr*) expr;
  //cout<<p->getMethodId()->getIdentifier()<<endl; 
  //cout<<p->getId1()->getIdentifier()<<endl; 
  string methodID(p->getMethodId()->getIdentifier());
  string MethodCoded("get_edge");
  int x=methodID.compare(MethodCoded);
  list<argument*> arg = p->getArgList();
     list<argument*> ::iterator itr;
     itr=arg.begin();
     itr++;
     //advance(itr,1);
     argument* a = *itr;
     a->getExpr()->getId()->getIdentifier();
  if(x==0)
    {
      char buf[1024];
      //sprintf(buf,"int wt = weight[j];",/*a->getExpr()->getId()->getIdentifier()*/ );
      main.pushstr_newL("int wt = weight[j];");
    }
  
  //cout<<p->getId2()->getIdentifier()<<endl;
  }
}


void dsl_cpp_generator::generate_exprPropId(PropAccess* expr)
{
  // main.pushString(expr->getId()->getIdentifier());
  //Identifier* id1 = expr->getIdentifier1();
  //cout<< id1->getIdentifier();
  //cout<< expr->getIdentifier2()->getIdentifier()<<endl;
  char buf[1024];
  string com = "weight";
  string tocom = expr->getIdentifier2()->getIdentifier();
  int i = com.compare(tocom);
  if(i==0)
  {
    //sprintf(buf,"%s[%s]",expr->getIdentifier2()->getIdentifier(),expr->getIdentifier1()->getIdentifier());
  main.pushstr_space("wt");
  }
  else
  {
  sprintf(buf,"%s[%s]",expr->getIdentifier2()->getIdentifier(),expr->getIdentifier1()->getIdentifier());
  main.pushstr_space(buf);
  }
}
void dsl_cpp_generator::generateExpr(Expression* expr)
{ 
  cout<<"At expr gen"<<endl;
  if(expr->isLiteral())
     { 
       cout<<"INSIDE THIS FOR LITERAL"<<"\n";
       generate_exprLiteral(expr);
     }
     else if(expr->isInfinity())
       {
         generate_exprInfinity(expr);
       }

       else if(expr->isIdentifierExpr())
       {
<<<<<<< HEAD
         cout<<"identifier expr"<<endl;
         generate_exprIdentifier(expr);
       }
       else if(expr->isPropIdExpr())
       {
         cout<<"propidexpr"<<endl;
=======
         generate_exprIdentifier(expr->getId());
       }
       else if(expr->isPropIdExpr())
       {
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224
         generate_exprPropId(expr->getPropId());
       }
       else if(expr->isArithmetic()||expr->isLogical())
       {
<<<<<<< HEAD
         cout<<"logical"<<endl;
         generate_exprRelational(expr);
        // generate_exprArL(expr);
=======
         generate_exprArL(expr);
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224
       }
       else if(expr->isRelational())
       {
          cout<<"Relational expression"<<endl;
          generate_exprRelational(expr);
       }
       else if(expr->isProcCallExpr())
       {
<<<<<<< HEAD
         cout<<"proc call expr"<<endl;
         generate_exprProcCall(expr);
       }
       else if(expr->isDependent())
       {
         cout<<"dependent expr"<<endl;
         generate_exprDependent(expr);
=======
         generate_exprProcCall(expr);
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224
       }
       
       else 
       {
         cout<<"dont know type"<<endl;
         assert(false);
       }



}

void dsl_cpp_generator::generate_exprArL(Expression* expr)
{
  //cout<<"INSIDE RELATIONAL EXPR"<<"\n";
    
    generateExpr(expr->getLeft());
    
    main.space();
    const char* operatorString=getOperatorString(expr->getOperatorType());
    main.pushstr_space(operatorString);
    generateExpr(expr->getRight());

}


void dsl_cpp_generator::generate_exprProcCall(Expression* expr)
{
  proc_callExpr* proc=(proc_callExpr*)expr;
  string methodId(proc->getMethodId()->getIdentifier());
  if(methodId=="get_edge")
  {
    main.pushString("edge"); //To be changed..need to check for a neighbour iteration 
                             // and then replace by the iterator.
  }
}

void dsl_cpp_generator::generate_exprPropId(PropAccess* propId) //This needs to be made more generic.
{ char strBuffer[1024];
  //PropAccess* propId=(PropAccess*)expr->getPropId();
  Identifier* id1=propId->getIdentifier1();
  Identifier* id2=propId->getIdentifier2();
  sprintf(strBuffer,"%s[%s]",id2->getIdentifier(),id1->getIdentifier());
  main.pushString(strBuffer);



}

void dsl_cpp_generator::generateFixedPoint(fixedPointStmt* fixedPointConstruct)
<<<<<<< HEAD
{ 
  Expression* convergeExpr=fixedPointConstruct->getConvergeExpr();
  blockStatement* blockStmt=fixedPointConstruct->getBody();
  assert(convergeExpr->getExpressionFamily()==EXPR_RELATIONAL||convergeExpr->getExpressionFamily()==EXPR_LOGICAL||convergeExpr->getExpressionFamily()==EXPR_DEPENDENT);
  main.pushstr_newL("int* final_dist=new int[g.num_nodes()];");
  main.pushstr_newL("for (int t = 0; t < g.num_nodes(); t ++)");
  main.insert_indent();
  main.pushstr_newL("final_dist[t] = 0;");
  main.decrease_indent();
  main.pushstr_newL("gettimeofday(&start, NULL);");
  main.pushString("while (");
  generateExpr(convergeExpr);
  main.pushstr_newL(" )");
  //generateStatement(fixedPointConstruct->getBody());
  list<statement*> stmtList=fixedPointConstruct->getBody()->returnStatements();
   list<statement*> ::iterator itr;
        for(itr=stmtList.begin();itr!=stmtList.end();itr++)
   {
     statement* stmt=*itr;
     cout<<"Gen fixed point block  "<<stmt->getType()<<endl;
     //generateStatement(stmt);

   }
  generateBlock(fixedPointConstruct->getBody());
 }
=======
{ char strBuffer[1024];
  Expression* convergeExpr=fixedPointConstruct->getDependentProp();
  Identifier* fixedPointId=fixedPointConstruct->getFixedPointId();
  statement* blockStmt=fixedPointConstruct->getBody();
  assert(convergeExpr->getExpressionFamily()==EXPR_UNARY||convergeExpr->getExpressionFamily()==EXPR_ID);
  main.pushString("while ( ");
  main.push('!');
  main.pushString(fixedPointId->getIdentifier());
  main.pushstr_newL(" )");
  main.pushstr_newL("{");
  if(fixedPointConstruct->getBody()->getTypeofNode()!=NODE_BLOCKSTMT)
  generateStatement(fixedPointConstruct->getBody());
  else
    generateBlock((blockStatement*)fixedPointConstruct->getBody(),false);
  Identifier* dependentId=NULL;
  bool isNot=false;
  assert(convergeExpr->getExpressionFamily()==EXPR_UNARY||convergeExpr->getExpressionFamily()==EXPR_ID);
  if(convergeExpr->getExpressionFamily()==EXPR_UNARY)
  {
    if(convergeExpr->getUnaryExpr()->getExpressionFamily()==EXPR_ID)
    {
      dependentId=convergeExpr->getUnaryExpr()->getId();
      isNot=true;
    }
  }
  if(convergeExpr->getExpressionFamily()==EXPR_ID)
     dependentId=convergeExpr->getId();
     if(dependentId!=NULL)
     {
       if(dependentId->getSymbolInfo()->getType()->isPropType())
       {   
        sprintf(strBuffer,"bool %s_fp = false ;",dependentId->getIdentifier());
        main.pushstr_newL(strBuffer);
         if(dependentId->getSymbolInfo()->getType()->isPropNodeType())
         {  Type* type=dependentId->getSymbolInfo()->getType();
              main.pushString("#pragma omp parallel for");
           sprintf(strBuffer," reduction(||:%s_fp)",dependentId->getIdentifier());
           main.pushstr_newL(strBuffer);
           if(graphId.size()>1)
             {
               cerr<<"GRAPH AMBIGUILTY";
             }
             else
            sprintf(strBuffer,"for (%s %s = 0; %s < %s.%s(); %s ++) ","int","v","v",graphId[0]->getIdentifier(),"num_nodes","v");
           main.pushstr_newL(strBuffer);
           sprintf(strBuffer,"%s_fp = %s_fp || %s[%s] ;",dependentId->getIdentifier(),dependentId->getIdentifier(),dependentId->getIdentifier(),"v");
           main.pushstr_newL(strBuffer);
           if(isNot)
           {
            sprintf(strBuffer,"%s = !%s_fp ;",fixedPointId->getIdentifier(),dependentId->getIdentifier());
            main.pushstr_newL(strBuffer);
             }
             else
             {
               sprintf(strBuffer,"%s = %s_fp ;",fixedPointId->getIdentifier(),dependentId->getIdentifier());
               main.pushString(strBuffer);
             }

        }
      }
     }
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224

     main.pushstr_newL("}");
}



void dsl_cpp_generator::generateBlock(blockStatement* blockStmt,bool includeBrace)
{  cout<<"INSIDE BLOCK OF NORMAL ITER"<<"\n";
   list<statement*> stmtList=blockStmt->returnStatements();
   list<statement*> ::iterator itr;
   if(includeBrace)
   {
   main.pushstr_newL("{");
   }
   for(itr=stmtList.begin();itr!=stmtList.end();itr++)
   {
     statement* stmt=*itr;
<<<<<<< HEAD
     cout<<"Gen block "<<stmt->getType()<<endl;
=======
     //printf("CHECK IF INSIDE FOR ");//%d\n",stmt->getParent()->getParent()->getTypeofNode()==NODE_FORALLSTMT);
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224
     generateStatement(stmt);

   }
   if(includeBrace)
   {
   main.pushstr_newL("}");
   }
}
void dsl_cpp_generator::generateFunc(ASTNode* proc)
{  char strBuffer[1024];
   Function* func=(Function*)proc;
   generateFuncHeader(func,false);
   generateFuncHeader(func,true);
<<<<<<< HEAD
   //generateStatement(func->getBlockStatement());
   blockStatement* blockStmt = func->getBlockStatement();
   list<statement*> stmtList=blockStmt->returnStatements();
   list<statement*> ::iterator itr;
   //main.pushstr_newL("{");
   for(itr=stmtList.begin();itr!=stmtList.end();itr++)
   {
     statement* stmt=*itr;
     cout<<"Gen block "<<stmt->getType()<<endl;
     generateStatement(stmt);

   }
   main.pushstr_newL("gettimeofday(&end, NULL);");
   main.pushstr_newL("seconds = (end.tv_sec - start.tv_sec);");
   main.pushstr_newL("micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);");
   main.pushstr_newL("MPI_Gather(dist+my_rank*part_size,part_size,MPI_INT,final_dist,part_size,MPI_INT,0,MPI_COMM_WORLD);");
   main.pushstr_newL("if(my_rank==0)");
    main.pushstr_newL("{");
    main.insert_indent();
    char buff[1024];
    sprintf(buff,"cout<<\"The iteration time = \"<<micros<<\" micro secs.\"<<endl;");
    //sprintf(buff,"printf(\"The iteration time = \%ld micro secs.\n\",micros);");
    main.pushstr_newL(buff);
    main.pushstr_newL("for(int i=0;i< g.num_nodes();i++)");
    sprintf(buff,"cout<<\"dist[\"<<i<<\"] = \"<<final_dist[i] <<endl;");
    main.pushstr_newL(buff);
    main.decrease_indent();
    main.pushstr_newL("}");
   main.pushstr_newL("MPI_Finalize();");
   //main.pushstr_newL("return 0;");
   main.pushstr_newL("}");
   
   
=======
   //including locks before hand in the body of function.
    main.pushstr_newL("{");
   sprintf(strBuffer,"const int %s=%s.%s();","node_count",graphId[0]->getIdentifier(),"num_nodes");
   main.pushstr_newL(strBuffer);
   sprintf(strBuffer,"omp_lock_t lock[%s+1];","node_count");
   main.pushstr_newL(strBuffer);
   generateForAll_header();
   sprintf(strBuffer,"for(%s %s = %s; %s<%s.%s(); %s++)","int","v","0","v",graphId[0]->getIdentifier(),"num_nodes","v");
   main.pushstr_newL(strBuffer);
   sprintf(strBuffer,"omp_init_lock(&lock[%s]);","v");
   main.pushstr_newL(strBuffer);
   //including locks before hand in the body of function.
   generateBlock(func->getBlockStatement(),false);
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224
   main.NewLine();
   main.push('}');

   return;

} 

const char* dsl_cpp_generator:: convertToCppType(Type* type)
{
  if(type->isPrimitiveType())
  {
      int typeId=type->gettypeId();
      switch(typeId)
      {
        case TYPE_INT:
          return "int";
        case TYPE_BOOL:
          return "bool";
        case TYPE_LONG:
          return "long";
        case TYPE_FLOAT:
          return "float";
        case TYPE_DOUBLE:
          return "double";
        case TYPE_NODE:
          return "int";
        case TYPE_EDGE:
          return "int";   
        default:
         assert(false);          
      }
  }
  else if(type->isPropType())
  {
    Type* targetType=type->getInnerTargetType();
    if(targetType->isPrimitiveType())
    { 
      int typeId=targetType->gettypeId();
      cout<<"TYPEID IN CPP"<<typeId<<"\n";
      switch(typeId)
      {
        case TYPE_INT:
          return "int*";
        case TYPE_BOOL:
          return "bool*";
        case TYPE_LONG:
          return "long*";
        case TYPE_FLOAT:
          return "float*";
        case TYPE_DOUBLE:
          return "double*";
        default:
         assert(false);          
      }

    }
  }
  else if(type->isNodeEdgeType())
  {
     return "int"; //need to be modified.
      
  }
  else if(type->isGraphType())
  {
    return "graph";
  }

  return "NA";
}

void dsl_cpp_generator:: generateFuncHeader(Function* proc,bool isMainFile)
{ 
  char buff[1024];
  
 //cout<<"INSIDE THIS VIEW US"<<"\n";
  dslCodePad& targetFile=isMainFile?main:header;
  targetFile.pushstr_space("void");
  targetFile.pushString(proc->getIdentifier()->getIdentifier());
  targetFile.push('(');
  
  int maximum_arginline=4;
  int arg_currNo=0;
  int argumentTotal=proc->getParamList().size();
  list<formalParam*> paramList=proc->getParamList();
  list<formalParam*>::iterator itr;
  for(itr=paramList.begin();itr!=paramList.end();itr++)
  {
      arg_currNo++;
      argumentTotal--;

      Type* type=(*itr)->getType();
      targetFile.pushString(convertToCppType(type));
      /*if(type->isPropType())
      {
          targetFile.pushString("* ");
      }
      else 
      {*/
          targetFile.pushString(" ");
         // targetFile.space();
      //}   
      targetFile.pushString(/*createParamName(*/(*itr)->getIdentifier()->getIdentifier());
      if(argumentTotal>0)
         targetFile.pushString(",");

      if(arg_currNo==maximum_arginline)
      {
         
         targetFile.NewLine();          
         arg_currNo=0;  
      } 
     // if(argumentTotal==0)
<<<<<<< HEAD
=======
         
  }
>>>>>>> da5dbf845f87d4dee325ace2fe92861c865cd224

         
  }
    targetFile.pushString(",int* weight");
   targetFile.pushString(")");

   if(!isMainFile)
   {
       targetFile.pushString(";");
       targetFile.NewLine();
   }
    else
         //targetFile.NewLine();
      targetFile.pushstr_newL("{"); 
    //targetFile.NewLine();

   //my code
   /*
    if(isMainFile)
    {
      main.pushstr_newL("int main(int argc,char** argv)");
      main.pushstr_newL("{");
      list<formalParam*> paramList=proc->getParamList();
      list<formalParam*>::iterator itr;
      for(itr=paramList.begin();itr!=paramList.end();itr++)
      {
        Type* type=(*itr)->getType();
        if(type->isGraphType())
        {
          main.pushstr_newL("if (argc < 2)");
          main.pushstr_newL("{");
          main.insert_indent();
          main.pushstr_newL("cout << \"Usage: ./a.out input_file output_file\" << endl;");
          main.pushstr_newL("exit(0);");
          main.decrease_indent();
          main.pushstr_newL("}");

          main.pushstr_newL("string input_file = argv[1];");
          main.pushstr_newL("ifstream infile;");
          main.pushstr_newL("infile.open(input_file, ios::in);");
          main.pushstr_newL("if (!infile)");
          main.pushstr_newL("{");
          main.insert_indent();
          main.pushstr_newL("cout << \"Error reading file: \" << input_file << endl;");
          main.pushstr_newL("exit(0);");
          main.decrease_indent();
          main.pushstr_newL("}");

          main.pushstr_newL("int g.num_nodes(),num_edges,*g.indexofNodes,*g.edgeList,*weight;");
          main.pushstr_newL("infile >> g.num_nodes() >> num_edges;");
          main.pushstr_newL("g.indexofNodes = new int[g.num_nodes()+1];");
          main.pushstr_newL("g.edgeList = new int[num_edges];");
          main.pushstr_newL("weight = new int[num_edges];");

          main.pushstr_newL("for (int i=0; i<(g.num_nodes()+1); i++) {");
          main.pushstr_newL("infile >> g.indexofNodes[i];");
          main.pushstr_newL("}");

          main.pushstr_newL("for (int i=0; i<num_edges; i++) {");
          main.pushstr_newL("infile >> g.edgeList[i];");
          main.pushstr_newL("}");

          main.pushstr_newL("for (int i=0; i<num_edges; i++) {");
          main.pushstr_newL("infile >> weight[i];");
          main.pushstr_newL("}");
          //targetFile.pushString( (*itr)->getIdentifier()->getIdentifier());
        }
        if(type->isNodeType())
        {
          sprintf(buff,"int %s;",(*itr)->getIdentifier()->getIdentifier());
          main.pushstr_newL(buff);
          //sprintf(buff,"cout << \"Enter value of %s \"<<endl;",(*itr)->getIdentifier()->getIdentifier());
          //main.pushstr_newL(buff);
          sprintf(buff,"infile >> %s ;",(*itr)->getIdentifier()->getIdentifier());
          main.pushstr_newL(buff);
          //main.pushString((*itr)->getIdentifier()->getIdentifier());
        }
      }
    }*/
    return; 

        
}


bool dsl_cpp_generator::openFileforOutput()
{ 

  char temp[1024];
  printf("fileName %s\n",fileName);
  sprintf(temp,"%s/%s.h","../graphcode",fileName);
  headerFile=fopen(temp,"w");
  if(headerFile==NULL)
     return false;
  header.setOutputFile(headerFile);

  sprintf(temp,"%s/%s.cc","../graphcode",fileName);
  bodyFile=fopen(temp,"w"); 
  if(bodyFile==NULL)
     return false;
  main.setOutputFile(bodyFile);     
  
  return true;
}
void dsl_cpp_generator::generation_end()
  {
     header.NewLine();
     header.pushstr_newL("#endif");   
  }

 void dsl_cpp_generator::closeOutputFile()
 { 
   if(headerFile!=NULL)
   {
     header.outputToFile();
     fclose(headerFile);

   }
   headerFile=NULL;

   if(bodyFile!=NULL)
   {
     main.outputToFile();
     fclose(bodyFile);
   }

   bodyFile=NULL;

 } 

bool dsl_cpp_generator::generate()
{  

      
  // cout<<"FRONTEND VALUES"<<frontEndContext.getFuncList().front()->getBlockStatement()->returnStatements().size();    //openFileforOutput();
   if(!openFileforOutput())
      return false;
   generation_begin(); 
   
   list<Function*> funcList=frontEndContext.getFuncList();
   for(Function* func:funcList)
   {
       generateFunc(func);
   }
   

   generation_end();

   closeOutputFile();

   return true;

}


  void dsl_cpp_generator::setFileName(char* f) // to be changed to make it more universal.
  {

    char *token = strtok(f, "/");
	  char* prevtoken;
   
   
    while (token != NULL)
    {   
		prevtoken=token;
    token = strtok(NULL, "/");
    }
    fileName=prevtoken;

  }

