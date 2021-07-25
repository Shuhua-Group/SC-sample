#include "MutSolver.h"
#include <list>
#include "simulator.h"

/*
void StdMutSolver::SolveOneSite(LocalTree& localTree, Site& site)
{
     NewickTree* nTree = localTree.tree;
     solveNode(nTree->getRoot(), site);
     
};     */

void StdMutSolver::fixUnknown(NodePtr pNode,bool bRootNode){
  GeneString* pGS = (GeneString*)pNode->getAssoData();
  char c = pGS->getValue(0);
  if(c == GeneString::VALUE_UNKNOWN){
    if(bRootNode){
      pGS->setValue(0,GeneString::VALUE_ZERO);  
    }else{
      EdgePtr edge0 = pNode->getTopEdgeByIndex(0);
      EdgePtr edge1 = pNode->getTopEdgeByIndex(1);
      int iter0=-1,iter1=-1;
      if (edge0!=NULL) iter0=edge0->iGraphIteration;
      if (edge1!=NULL) iter1=edge1->iGraphIteration;
      if (iter0==-1 && iter1==-1) throw "No top edge is in current tree!";
      EdgePtr & topEdge = (iter0>iter1)? edge0:edge1;
      NodePtr parentNode = topEdge->getTopNodeRef();
      GeneString* pParentGS = (GeneString*)parentNode->getAssoData();
      pGS->setValue(0,pParentGS->getValue(0));
    }
  }
 
  EdgePtr bottomEdge1 = pNode->getBottomEdgeByIndex(0);
  EdgePtr bottomEdge2 = pNode->getBottomEdgeByIndex(1);

  if(bottomEdge1 != NULL && bottomEdge1->iGraphIteration == pGB->iCurrentTreeId){
    NodePtr bottomNode = bottomEdge1->getBottomNodeRef();
    fixUnknown(bottomNode,false);
  }
 
  if(bottomEdge2 != NULL && bottomEdge2->iGraphIteration == pGB->iCurrentTreeId){
    NodePtr bottomNode = bottomEdge2->getBottomNodeRef();
    fixUnknown(bottomNode,false);
  }


}
/*
Associate a GeneString to each node , 
For sample node, GeneString build from 0-1 sequence from sample.
For non-sample node, GeneString build by algorithm.
memory allocated for GeneString in this function will be release in calcNodeMut .
*/
GeneString& StdMutSolver::visit(NodePtr pNode, SamplesBySite& samples, int nSiteIndex, bool bParentLevel)
{     
   //  cerr<<"entry of visit"<<endl; 
     if(pNode->getType() == Node::SAMPLE ){
   //    cerr<<"sample node before pNode.get()"<<endl;
       SampleNode *sNode = dynamic_cast<SampleNode*>(pNode.get());  
      // char geneStr[1024];
       char* pValue = new char[1];
       *pValue = samples.getSiteValue(nSiteIndex,sNode->iId);
//       cerr<<"value of node "<<sNode->iId<<" is :"<<pValue<<endl;
      // int len = samples.getData(sNode->getSamId(),dStart, dEnd, geneStr);
       GeneString *pGS = new GeneString(pValue,1);
       //if(DEBUG && pGS->getLen() > 0)
       //  printf("Genestring for node %d sampleid %d: %s\n", pNode->getNodeId(), sNode->getSamId(),pGS->c_str());
//       cerr<<"set gene for sample node at height "<<pNode->getHeight()<<" : "<<pGS->c_str()<<endl;
       pNode->setAssoData((void*)pGS);
       return *pGS; 
     }else{
       /*
       if(pNode->getType() == Node::XOVER){
         cerr<<"Nodetype is: XOVER"<<endl;
       }else{
         cerr<<"Nodetype is: "<<pNode->getType()<<endl;
       }*/
       EdgePtr bottomEdge1 = pNode->getBottomEdgeByIndex(0);
       EdgePtr bottomEdge2 = pNode->getBottomEdgeByIndex(1);    

       if(bottomEdge1 == NULL && bottomEdge2 == NULL) throw "bottomEdge1  and bottomEdge2 are both NULL!";
       //only have one bottom edge,XOVER node 
       if(bottomEdge1 == NULL || bottomEdge2 == NULL){
         EdgePtr curTreeEdge = bottomEdge1 == NULL ? bottomEdge2 : bottomEdge1;
         NodePtr bottomNode = curTreeEdge->getBottomNodeRef();
         GeneString& bottomNodeGS = visit(bottomNode,samples,nSiteIndex, false);
         GeneString *pGS = new GeneString(bottomNodeGS);
         pNode->setAssoData((void*)pGS);
         return *pGS;
       }
       //all bottom edge are new
       if(bottomEdge1->iGraphIteration == pGB->iCurrentTreeId && bottomEdge2->iGraphIteration ==pGB->iCurrentTreeId){
           GeneString& leftGS = visit(bottomEdge1->getBottomNodeRef(), samples, nSiteIndex, false);     
           GeneString& rightGS = visit(bottomEdge2->getBottomNodeRef(), samples, nSiteIndex, false);
           GeneString *pMergeGS = new GeneString(merge(leftGS, rightGS));
           /*known will be clean by fixUnkown()          
           //fix all '2' for root node.
           if(bParentLevel)
             pMergeGS->cleanUnknown();
           */
          // if(DEBUG && pMergeGS->getLen() > 0)
          //   printf("Genestring for node %d : %s\n", pNode->getNodeId(),pMergeGS->c_str());
//           cerr<<"set gene for current tree non-sample node at height "<<pNode->getHeight()<<" : "<<pMergeGS->c_str()<<endl;
           pNode->setAssoData((void*)pMergeGS);
           return *pMergeGS;           
       }else{
         EdgePtr curTreeEdge = bottomEdge1->iGraphIteration == pGB->iCurrentTreeId ? bottomEdge1 : bottomEdge2;    
         NodePtr bottomNode = curTreeEdge->getBottomNodeRef();
         GeneString& bottomNodeGS = visit(bottomNode,samples,nSiteIndex, false);
         GeneString *pGS = new GeneString(bottomNodeGS);
//         cerr<<"set gene for non-current tree non-sample node at height : "<<pNode->getHeight()<<" : "<<pGS->c_str()<<endl;
         pNode->setAssoData((void*)pGS);
         return *pGS;                 
       };
       
     };
     
};     
/*
- calculate mut num based on GeneString associated with Node.
- free memory of GeneString after calculating.
*/
/*
void StdMutSolver::calcNodeMut(Node* pNode)
{
     GeneString* pGS = (GeneString*)pNode->getAssoData();
     GeneString* pParentGS  = (GeneString*)pNode->getParent()->getAssoData();

     int nMut = pGS->diff(*pParentGS);
     pNode->setMutNum(nMut);     
     if(pNode->getLeftChild() != NULL)
       calcNodeMut(pNode->getLeftChild());
      
     if(pNode->getRightChild() != NULL)
       calcNodeMut(pNode->getRightChild());  
       
     
};     */
void StdMutSolver::calcNodeMut(NodePtr pNode, NodePtr pParentNode, int index, int* pRes)
{
     GeneString* pGS = (GeneString*)pNode->getAssoData();
     GeneString* pParentGS  = (GeneString*)pParentNode->getAssoData();

     int nMut = pGS->diff(*pParentGS, index);
     (*pRes) = (*pRes) + nMut;    
     
      EdgePtr bottomEdge1 = pNode->getBottomEdgeByIndex(0);
      EdgePtr bottomEdge2 = pNode->getBottomEdgeByIndex(1);  
//      if(bottomEdge1 == NULL) throw "bottomEdge1 is NULL!";
//      if(bottomEdge2 == NULL) throw "bottomEdge2 is NULL!";  
      if(bottomEdge1 != NULL && bottomEdge1->iGraphIteration == pGB->iCurrentTreeId){
         NodePtr bottomNode = bottomEdge1->getBottomNodeRef();                             
         calcNodeMut(bottomNode, pNode, index, pRes);                             
      }
      if(bottomEdge2 != NULL && bottomEdge2->iGraphIteration == pGB->iCurrentTreeId){
         NodePtr bottomNode = bottomEdge2->getBottomNodeRef();                             
         calcNodeMut(bottomNode, pNode, index, pRes);                             
      }
            
};
/*
 pnRes : site number of local tree whose mut = 1. It indicate the solve correct site of the local tree.
 */
bool StdMutSolver::validateMutResult(NodePtr pRoot)
{
     //cerr<<"entry of validateMutResult"<<endl;
    bool bRes(true);
    GeneString* pRootGene = (GeneString*)pRoot->getAssoData();
    int nLen = pRootGene->getLen();
    for(int i=0;i<nLen;i++)
    {
      int nMut = 0;
      EdgePtr bottomEdge1 = pRoot->getBottomEdgeByIndex(0);
      EdgePtr bottomEdge2 = pRoot->getBottomEdgeByIndex(1);    
      if(bottomEdge1 != NULL && bottomEdge1->iGraphIteration == pGB->iCurrentTreeId){
         NodePtr bottomNode = bottomEdge1->getBottomNodeRef();                             
         calcNodeMut(bottomNode, pRoot, i, &nMut);                             
      }
      if(bottomEdge2 != NULL && bottomEdge2->iGraphIteration == pGB->iCurrentTreeId){
         NodePtr bottomNode = bottomEdge2->getBottomNodeRef();                             
         calcNodeMut(bottomNode, pRoot, i, &nMut);                             
      }
      if(nMut > 1){
        bRes = false;
      };     
    }
    //cerr<<"end of validateMutResult "<<bRes<<endl;
    return bRes;
     
};
void StdMutSolver::releaseAssoData(NodePtr pNode)
{
     /*
     GeneString* pGS = (GeneString*)pNode->getAssoData();
     delete pGS;
     pGS = NULL;
     if(pNode->getLeftChild() != NULL)     
         releaseAssoData(pNode->getLeftChild());
     if(pNode->getRightChild() != NULL)     
         releaseAssoData(pNode->getRightChild());     
         */    
};     
bool StdMutSolver::SolveMutation(NodePtr pRoot, SamplesBySite& samples, int nSiteIndex)
{
     
     visit(pRoot, samples, nSiteIndex, true);
     fixUnknown(pRoot,true);
//     calcNodeMut(pRoot->getLeftChild());
//     calcNodeMut(pRoot->getRightChild());
     
     bool bValidateRes = validateMutResult(pRoot);
     //release memories.
     //releaseAssoData(pRoot);
     //return bValidate;
     return bValidateRes;
     
};    

double StdMutSolver::findNextSolveFailPos(NodePtr pRoot, SamplesBySite& samples, double dStartPos)
{
     int nSiteNum = samples.getSiteNum();
     for(int i=0;i<nSiteNum;i++)
     {
       Site& site = samples.getSite(i);
       if(site.getPos() < dStartPos)
         continue;      
       //found the site further than dStartPos;
       bool bSolveRes = SolveMutation(pRoot,samples,i);
       if(bSolveRes){
         continue;
       }else{
         //printf("findNextSolveFailPos from pos %.5lf: %.5lf\n",dStartPos, site.getPos());     
         return site.getPos();    
       }         
     }    
     return 1.0;                
};

//wy add
double StdMutSolver::PosbeforeNextSolveFail(NodePtr pRoot, SamplesBySite& samples, double dStartPos)
{
     int nSiteNum = samples.getSiteNum();
     for(int i=0;i<nSiteNum;i++)
     {
       Site& site = samples.getSite(i);
       if(site.getPos() < dStartPos)
         continue;      
       //found the site further than dStartPos;
       bool bSolveRes = SolveMutation(pRoot,samples,i);
       if(bSolveRes){
         continue;
       }else{
         i=i-1;
         Site& site = samples.getSite(i); 
         if(site.getPos()>dStartPos){return site.getPos();}
         else{return dStartPos;}    
       }         
     }    
     
     return 1.0;                
};

bool  StdMutSolver::SolveuntilNextFailPos(NodePtr pRoot, SamplesBySite& samples, double dStartPos, NodePtr Node1, NodePtr Node2)
{   
     int nSiteNum = samples.getSiteNum();
     bool Res(true);
     int i(0);
     for( i=0;i<nSiteNum;i++)
     {
       Site& site = samples.getSite(i);
       if(site.getPos() <= dStartPos)
         continue;      
       //found the site further than dStartPos;
       bool bSolveRes = SolveMutation(pRoot,samples,i);
       GeneString* pGS1 = (GeneString*)Node1->getAssoData();
       GeneString* pGS2 = (GeneString*)Node2->getAssoData();
       //cerr<<"GeneString of coalEdgeTopNode is:"<<pGS1->c_str()<<endl;
       //cerr<<"GeneString of xOverEdgeBtmNode is:"<<pGS2->c_str()<<endl;  
       if(bSolveRes){                                   
          if(pGS1->diff(*pGS2) == 0){continue;}
          else{Res = false; break;}
       }else{
          if(pGS1->diff(*pGS2) != 0)Res = false;  
          break;}
     }
     //for dtartPos > lastSite case
     //Solve mutation using last site to assign genesting to all node
     //if no this line, new coalescence node after dStartPos will have null string
     if(i==nSiteNum)
     {
       SolveMutation(pRoot,samples,nSiteNum-1);
     }
     return Res;        
};
//wy add end
bool  StdMutSolver::SolveNextSite(NodePtr pRoot, SamplesBySite& samples, double dStartPos)
{
      //cerr<<"entry of SolveNextSite"<<endl;
     int nSiteNum = samples.getSiteNum();
     bool bSolveRes(true);
     int i(0);
     for( i=0;i<nSiteNum;i++)
     {
       Site& site = samples.getSite(i);
       if(site.getPos() < dStartPos)
         continue;      
       //found the site further than dStartPos;
       bSolveRes = SolveMutation(pRoot,samples,i);
       break;
     }
     //for dtartPos > lastSite case
     //Solve mutation using last site to assign genesting to all node
     //if no this line, new coalescence node after dStartPos will have null string
     if(i==nSiteNum)
     {
       SolveMutation(pRoot,samples,nSiteNum-1);
     }

     //cerr<<"end of SolveNextSite "<<bSolveRes<<endl;
     return bSolveRes;        
};       
/*
  return : mut num of the local tree.
  pnCorrectSite : site num of the localtree which is solved correctly(mut=1).
 
 */
 /*
int StdMutSolver::SolveMutation(LocalTree& localTree, SamplesBySite& samples, int* pnCorrectSite)
{
     NewickTree* pTree = localTree.tree;
     Node* pRoot = pTree->getRoot();
     visit(pRoot, localTree.startPos, localTree.endPos, samples, true);
     calcNodeMut(pRoot->getLeftChild());
     calcNodeMut(pRoot->getRightChild());
     localTree.nGeneSite = ((GeneString*)pRoot->getAssoData())->getLen();
     
     bool bValidateRes = validateMutResult(localTree, pnCorrectSite);
     //release memories.
     releaseAssoData(pRoot);
     //return bValidate;
     return localTree.getMutNum();
     
};  */   
/*
 return : the total mut number of the arg.
 pnCorrectSite : total site number which is solved correctly(mut=1);
 pnCorrectLTree : total local tree number which is solved correctly( nCorrectSite == nGeneSite);
 */
 /*
int StdMutSolver::SolveARGMutation(ARG* pARG, SamplesBySite& samples, int* pnCorrectSite, int* pnCorrectLTree)
{  
    int nRes(0);     
    int nLocalTreeId(0);
	int nCorrectTree(0);
	int nCorrectSite(0);
      ARG::iterator itTree = pARG->begin();
      ARG::iterator itTreeEnd = pARG->end();
      while(itTree != itTreeEnd)
      {
        LocalTree* pLocalTree = (*itTree);
		int nTreeCorrectSite(0);
        int nLocalTreeMutNum = SolveMutation( *pLocalTree, samples, &nTreeCorrectSite);
        //printf("localtree %05d , nMut:%d, genesite:%d, nCorrectSite:%d\n", nLocalTreeId, nLocalTreeMutNum, pLocalTree->nGeneSite, nTreeCorrectSite );               
        if((nTreeCorrectSite == pLocalTree->nGeneSite) && nTreeCorrectSite > 0 )
			nCorrectTree++;
		itTree++;       
        nLocalTreeId++;
        nRes += nLocalTreeMutNum;
		nCorrectSite += nTreeCorrectSite;  
      }       
	(*pnCorrectSite) = nCorrectSite;
	(*pnCorrectLTree) = nCorrectTree;
     return nRes;
};     */
/*
void StdMutSolver::SolveARGMutation(vector<ARG*>& args, SamplesBySite& samples)
{
    vector<ARG*>::iterator it = args.begin();
    vector<ARG*>::iterator itEnd = args.end();
    int i(0);
    while(it != itEnd)
    {
      printf("solving mutation for arg %d...\n", i++);
      ARG* pARG = (*it);                     
      SolveARGMutation(pARG,samples);
      it++;     
    }        
      
};*/
