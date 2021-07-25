#ifndef _LLF_GENESTRING_H_
#define	_LLF_GENESTRING_H_

#include <stdlib.h>
//#include <mem.h>
#include <stdio.h>
#include <cstdlib>
#include <list>
#include <vector>
#include <iostream>
#include <string>

class GeneString{
  public:
         GeneString(unsigned int n);
         GeneString(const char* str, unsigned int n);
         GeneString(const GeneString& gs);
         virtual ~GeneString();
         void follow(const GeneString& right);
         bool containUnknown() const;
         inline int getUnknownCnt(){return nUnknown;};
         friend GeneString merge(GeneString& left, GeneString& right);  
         void cleanUnknown();       
         void print() const;
         void setValue(int i, char value);
	 inline int getLen(){return nLen;};
         int diff(const GeneString& gs);
	 int diff(const GeneString& gs, int ind);
         const inline char* c_str(){return data;};
         char getValue(int i){return data[i];};
         static const char VALUE_ZERO = '0';
         static const char VALUE_ONE = '1';
         static const char VALUE_UNKNOWN = '2';
  private:
          char* data;
          int nLen;          
          int nUnknown;         
      
};      
/*
//class TestGeneString : public TestBase{
class TestGeneString {
      
      public:
              void test();
      private:
              void testConstruct();
              void testMerge();
              void testFollow();
              void testContainUnknown();
              void testClearUnknown();
              void testDiff();
      
};
*/
#endif
