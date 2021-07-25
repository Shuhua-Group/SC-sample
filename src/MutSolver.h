#ifndef _LLF_MUTSOLVER_H_
#define	_LLF_MUTSOLVER_H_

#include <stdlib.h>
//#include "Newick.h"
#include "SamplesBySite.h"
#include <vector>
#include "GeneString.h"
#include "simulator.h"
//class NodePtr;

using namespace std;

/*
class MutSolver{
      
   public:
         MutSolver(){};
         virtual ~MutSolver(){};
     //    virtual void SolveARGMutation(vector<ARG*>& args, SamplesBySite& samples) = 0;             
    //     virtual int SolveARGMutation(ARG* pArg, SamplesBySite& samples, int* pnCorrectSite,int* pnCorrectTree) = 0;  
    
      
};*/
/*
class EasyMutSolver : public MutSolver{
      
   public:
         EasyMutSolver(){printf("EasyMutSolver is constructed...\n");};
         virtual ~EasyMutSolver(){}; 
    //     void SolveARGMutation(vector<ARG*>& args, SamplesBySite& samples);   
         int SolveARGMutation(ARG* pArg, SamplesBySite& samples, int* pnCorrectSite,int* pnCorrectTree);  
   private:
         int findSolve(const vector<int>& values, const vector<int>& indexes);
         void SolveNodeMutation(Node* node, vector<int>& sampleMutations);
         int SolveMutation(LocalTree& localTree, SamplesBySite& samples);
         
};      */
//class StdMutSolver : public MutSolver{
class StdMutSolver{        
      
   public:
         StdMutSolver(GraphBuilder* pgb){pGB = pgb; printf("StdMutSolver is constructed...\n");};
         virtual ~StdMutSolver(){};
     //    void SolveARGMutation(vector<ARG*>& args, SamplesBySite& samples);      
      //   int SolveMutation(LocalTree& localTree, SamplesBySite& samples, int* pnCorrectSite);
      //   int SolveARGMutation(ARG* pArg, SamplesBySite& samples, int* pnCorrectSite,int* pnCorrectTree);  
         bool SolveMutation(NodePtr pRoot, SamplesBySite& samples, int nSiteIndex);
         double findNextSolveFailPos(NodePtr pRoot, SamplesBySite& samples, double dStartPos);
         //wy added
         double PosbeforeNextSolveFail(NodePtr pRoot, SamplesBySite& samples, double dStartPos);
         bool  SolveuntilNextFailPos(NodePtr pRoot, SamplesBySite& samples, double dStartPos, NodePtr Node1, NodePtr Node2);
         //wy added end
         bool SolveNextSite(NodePtr pRoot, SamplesBySite& samples, double dStartPos);
         void fixUnknown(NodePtr pRoot,bool bRootNode);
   private:
         StdMutSolver(){printf("StdMutSolver is constructed...\n");};
         //void SolveNodeMutation(Node* node, vector<int>& sampleMutations);
         void releaseAssoData(NodePtr pNode);
         void calcNodeMut(NodePtr pNode);
//         GeneString& visit(Node* pNode, double dStart, double dEnd, SamplesBySite& samples, bool bParentLevel);
         GeneString& visit(NodePtr pNode,  SamplesBySite& samples, int nSiteIndex, bool bParentLevel);
         
         bool validateMutResult(NodePtr pNode);
         void calcNodeMut(NodePtr pNode, NodePtr pParentNode, int index, int* pRes);
         GraphBuilder* pGB;
};
/*
typedef enum {
E_TYPE_EASY,
E_TYPE_STD
} SOLVER_TYPE;        

class MutSolverFactory{
  public:
         static MutSolver* getSolver(SOLVER_TYPE type){
                
              if(type == TYPE_STD){
                      return new StdMutSolver();
              }else if (type ==  TYPE_EASY){
                    return new  EasyMutSolver();
              }else{
                    return NULL;
              }
              return new EasyMutSolver();
                
         };     
         static const SOLVER_TYPE TYPE_STD =   E_TYPE_STD;      
         static const SOLVER_TYPE TYPE_EASY =   E_TYPE_EASY;         
};        */   
#endif
