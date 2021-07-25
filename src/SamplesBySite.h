#ifndef _LLF_SAMPLESBYSITE_H_
#define	_LLF_SAMPLESBYSITE_H_


#include <cstdlib>
#include <vector>
#include <list>


using namespace std;

class Site{
  public:
      Site(int sampleSize);
      Site(int sampleSize, const char* str);
      Site(const Site& site);
      virtual ~Site();
      void show();
      static const char V_ZERO = '0';
      static const char V_ONE = '1';
      static const char V_TWO = '2';      
      int getMutNum() const { return nMut;};
      int getSampleSize(){ return nSampleSize;};
      inline double getPos() { return dPos;};
      bool isMuted(int sampID){ return value[sampID] != simuParent;};  //if sampID is muted on this site.
      char getParent(){return simuParent;};
      char getValue(int sampID){ return value[sampID];};
      //int getIntValue(int sampID){ return atoi(value[sampID]);};
  private:
      void calcMut();   
      int nId;
      double dPos;
      char* value;      
      int nSampleSize;            
      int nMut;
      char simuParent;
};

class SamplesBySite
{
  public:
  //  SampleBySite(int nSample){nSampleSize = nSample};
    SamplesBySite(int sampleSize) { nSampleSize = sampleSize;};
    bool buildFromFile(const char* file);
    double getMutRate();
    void add(const Site& site);
    int getMutNum();
    int getMutNum(double posStart, double posEnd);
    int getMutNum(int sampID, double posStart, double posEnd);
    void show();
    int getSiteNum(){return sites.size();};
    list<Site> getSites(double posStart, double posEnd);
    int getData(int sampID, double posStart, double posEnd, char* result);
    int getSampleSize(){return nSampleSize;};
    Site& getSite(int ind);
    char getSiteValue(int siteIndex,int sampID);
  private:
      int nSampleSize;
      list<Site> sites;        
};

/*
Testing for TestSamplesBySite.
*/
/*
//class TestSamplesBySite : public TestBase{
class TestSamplesBySite{
      
      public:
              void test();
      private:
              void testSamplesBySite();
              void testBuildSamplesBySite();
              void testGetSites();
              void testGetData();
      
};*/
#endif
