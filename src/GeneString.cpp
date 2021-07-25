#include "GeneString.h"
#include "string.h"
using namespace std;

GeneString::GeneString(unsigned int n)
{
  data = new char[n+1];
  memset(data, '0', n);               
  data[n] = '\0';
  nLen = n;                
  nUnknown = 0;        
};    
GeneString::GeneString(const char* str, unsigned int n)
{
  data = new char[n+1];
  nUnknown = 0;        
  nLen = n;            
  for(int i=0;i<n;i++){
     data[i] = str[i];
     if(str[i] == VALUE_UNKNOWN)
       nUnknown++;
  }     
  data[n] = '\0';                      

};         
GeneString::GeneString(const GeneString& gs)               
{
  nLen = gs.nLen;
  data = new char[nLen+1];
  memcpy(data,gs.data,nLen); 
  data[nLen] = '\0';   
  nUnknown = gs.nUnknown;                                                    
};     
GeneString::~GeneString()
{
  if(data != NULL)
  {
     delete []data;
     data = NULL;
  };                       
};                         
void GeneString::print() const
{
  printf("nKnown: %d\t%s\n", nUnknown, data);  
}       
void GeneString::setValue(int i, char value)
{
  if(data[i] == GeneString::VALUE_UNKNOWN )
    nUnknown--;
  else
    printf("error: only value %c can be reset.", GeneString::VALUE_UNKNOWN);
  data[i] = value;
};                         
void GeneString::follow(const GeneString& gs)
{
  if(!containUnknown())
    return;
  for(int i=0;i<nLen;i++){
    if((data[i] == VALUE_UNKNOWN) && gs.data[i] != VALUE_UNKNOWN )
    {
      data[i] = gs.data[i];                  
      nUnknown--;  
    }
  };                       
};     
bool GeneString::containUnknown() const
{
  if(nUnknown > 0)
    return true;   
  return false;
};     
void GeneString::cleanUnknown()
{
  if(nUnknown <= 0)
    return; 
  for(int i=0;i<nLen;i++)
  {
    if(data[i] == VALUE_UNKNOWN){
      data[i] = VALUE_ZERO;
      nUnknown--;           
    }               
  };
  //TestBase::assertEquals(0, nUnknown);          
};     
int GeneString::diff(const GeneString& gs){
  if(nLen != gs.nLen){
    printf("error: size should be same to diff!\n");        
    return 0;
  }
  if(containUnknown() || gs.containUnknown()){
    printf("error: should not contain 2 before diff\n");      
    print();
    gs.print();    
    return 0;              
  }
  int nDiff(0);
  for(int i=0;i<nLen;i++){
    if(data[i] != gs.data[i])
      nDiff++;      
  }
  return nDiff;  
};    
int GeneString::diff(const GeneString& gs, int i){
	if(nLen != gs.nLen){
		printf("error: size should be same to diff!\n");        
		return 0;
	}
	if(containUnknown() || gs.containUnknown()){
		printf("error: should not contain 2 before diff\n");      
		print();
		gs.print();    
		return 0;              
	}
	int nDiff(0);
	if(data[i] != gs.data[i])
		nDiff++;      
	return nDiff;  
};    

GeneString merge(GeneString& left, GeneString& right)
{
  GeneString res(left.nLen);
  for(int i=0;i<left.nLen;i++)
  {
    char cLeft = left.data[i];
    char cRight = right.data[i];    
    unsigned int nLeft = (int)cLeft - GeneString::VALUE_ZERO;
    unsigned int nRight = (int)cRight - GeneString::VALUE_ZERO;
    if((nLeft == nRight) && (nLeft != 2)){
      res.data[i] = cLeft;        
    }else if((nLeft + nRight) == 1){
      res.data[i] = GeneString::VALUE_UNKNOWN;
      res.nUnknown++;    
    }else if((nLeft == 2) && (nRight ==2)){
      res.data[i] = GeneString::VALUE_UNKNOWN;    
      /* do not reset this case during visit, will do it after visit from top.
      res.data[i] = GeneString::VALUE_ZERO;    
      //also set left and right to 0;
      left.setValue(i,GeneString::VALUE_ZERO);
      right.setValue(i,GeneString::VALUE_ZERO);
      */
      
    }else if(nLeft == 2){ //left=2, right=0|1
      res.data[i] = cRight;    
      //also set left and right to 0;
      left.setValue(i,cRight);
    }else if(nRight == 2){ // right=2, left=0|1
      res.data[i] = cLeft;
      right.setValue(i,cLeft);    
    }                    
  }           
  return res;   
};           
/*
void TestGeneString::test(){
      printf("=========TestGeneString============\n");
       testConstruct();
       testMerge();
       testFollow();
       testContainUnknown();
       testClearUnknown();                      
       testDiff();       
};                       
void TestGeneString::testConstruct(){
      printf("=========testConstruct============\n");
      GeneString gs1(10);
      gs1.print();
      TestBase::assertEquals("0000000000",gs1.c_str());
      const char* str = "010010101";
      GeneString gs2(str,strlen(str));
      gs2.print();
      GeneString gs3(gs2);    
      gs3.print();              
	  printf("%s\n",gs3.c_str());
};                              
void TestGeneString::testMerge(){
      printf("=========testMerge============\n");
      GeneString gs1("0012",4);
      gs1.print();
      GeneString gs2("1012",4);
      gs2.print();
      GeneString mergeGs = merge(gs1,gs2);
      mergeGs.print();
      TestBase::assertEquals("2010",mergeGs.c_str());
      TestBase::assertEquals(1,mergeGs.getUnknownCnt());
      gs1.print();
      TestBase::assertEquals("0010",gs1.c_str());
      gs2.print();
      TestBase::assertEquals("1010",gs2.c_str());
};   
void TestGeneString::testFollow(){
      printf("=========testFollow============\n");
      GeneString gs1("012210",6);
      GeneString gs2("011020",6);
      gs1.follow(gs2);
      gs1.print();
      TestBase::assertEquals("011010",gs1.c_str());
};    
void TestGeneString::testContainUnknown(){
      printf("=========testContainUnknown============\n");
      GeneString gs1("012210",6);
      TestBase::assertTrue(gs1.containUnknown());
      GeneString gs2("011010",6);
      TestBase::assertFalse(gs2.containUnknown());
      
};    
void TestGeneString::testClearUnknown(){
      printf("=========testClearUnknown============\n");
      GeneString gs1("012210",6);      
      TestBase::assertTrue(gs1.containUnknown());
      gs1.cleanUnknown();
      TestBase::assertFalse(gs1.containUnknown());
      gs1.print();
}; 
void TestGeneString::testDiff(){
      printf("=========testDiff============\n");
      GeneString gs1("011010",6);      
      GeneString gs2("110110",6);   
      int nDiff = gs1.diff(gs2);
      TestBase::assertEquals(3,nDiff);
}; 
//TestGeneString testGeneString;
*/
