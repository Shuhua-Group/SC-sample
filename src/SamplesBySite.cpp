#include "SamplesBySite.h"
#include <fstream>
#include <stdio.h>
#include <string>
#include <iostream>
#include "string.h"

Site::Site(int sampleSize)
{
  nSampleSize = sampleSize;
  nId = -1;
  dPos = 0.0;
  nMut = 0;
  value = new char[sampleSize+1];       
                        
};        

Site::Site(int sampleSize, const char* str)
{
  nSampleSize = sampleSize;               
  nMut = 0;
  value = new char[sampleSize+1];
  sscanf(str,"SITE: %d %lf %s", &nId,&dPos,value);
  value[sampleSize] = '\0';
  calcMut();                                            
};    
Site::Site(const Site& site)
{
    nSampleSize = site.nSampleSize;
    nId = site.nId;
    dPos = site.dPos;
    nMut = site.nMut;
    simuParent = site.simuParent;         
    value = new char[nSampleSize+1];
    memcpy(value,site.value,nSampleSize);        
    value[nSampleSize] = '\0';
             
};
Site::~Site()
{
  if(value != NULL)
    delete []value;           
}; 
/*
Use the most common value as parent, different ones are mutation.
*/

void Site::calcMut()
{
  int nOne(0),nZero(0);
  for(int i=0;i<nSampleSize;i++)
  {
    if(value[i]== V_ONE)
      nOne++;
    else
      nZero++;                          
  };             
  if(nOne >= nZero){
    simuParent = V_ONE;
    nMut = nZero;
  }else{
    simuParent = V_ZERO;
    nMut = nOne;
  } 
               
};   

/* implementation which always regard '0' as parent, '1' as mutation.
void Site::calcMut()
{
  for(int i=0;i<nSampleSize;i++)
  {
    if(value[i]== V_ONE)
      nMut++;                          
  };    
  simuParent = V_ZERO;   
     
};
*/
                      
void Site::show()
{
  printf("id:%d  pos:%lf  value:%s mut:%d parent:%c\n", nId, dPos, value, nMut, simuParent);     
};    
void SamplesBySite::show()
{
  list<Site>::iterator it = sites.begin();
  list<Site>::iterator itEnd = sites.end();
  while(it != itEnd)
  {
     it->show();  
     it++;          
  }                              
     
};     
void SamplesBySite::add(const Site& site)
{
     sites.push_back(site);
};  
double SamplesBySite::getMutRate()
{
  list<Site>::iterator it = sites.begin();
  list<Site>::iterator itEnd = sites.end();
  int nTotalMut(0);
  int nTotalSite(0);
  while(it != itEnd)
  {
     nTotalMut += it->getMutNum();
     nTotalSite += it->getSampleSize();           
     it++;                
  }                         
  return (double)nTotalMut/nTotalSite;
};     
int SamplesBySite::getMutNum()
{
  return getMutNum(0.0,1.0);                 
       
};                        
int SamplesBySite::getMutNum(double posStart, double posEnd)
{
       
  list<Site>::iterator it = sites.begin();
  list<Site>::iterator itEnd = sites.end();
  
  int nTotalMut(0);

  while(it != itEnd)
  {
     double pos = it->getPos();
     if((pos >= posStart) && (pos <posEnd))
     {
          nTotalMut += it->getMutNum();
     };
     it++;                
  }                         
  return nTotalMut;                   
       
};  
int SamplesBySite::getMutNum(int sampID, double posStart, double posEnd)
{
       
  list<Site>::iterator it = sites.begin();
  list<Site>::iterator itEnd = sites.end();
  
  int nTotalMut(0);

  while(it != itEnd)
  {
     double pos = it->getPos();
     if((pos >= posStart) && (pos < posEnd))
     {
          if(it->isMuted(sampID)){
              ++nTotalMut;
          }
     };
     it++;                
  }                         
  return nTotalMut;                   
       
};     
bool SamplesBySite::buildFromFile(const char* file)
{
    string line;
    ifstream infile(file);
	if(infile == NULL)
	{
		printf("can not open %s\n", file);
		return false;
	}
    while(getline(infile,line))
    {
       //Site* site = new Site(nSampleSize,line.c_str());
       //macs format, and ignore useless lines.
       if(line.find("SITE:",0)==0)
       {
         Site site(nSampleSize,line.c_str());
         add(site);
       }else{
         cerr<<"ignore line: "<<line<<endl;
	   }
    }
	return true;
     
};     

Site& SamplesBySite::getSite(int ind)
{
  list<Site>::iterator it = sites.begin();
  list<Site>::iterator itEnd = sites.end();  
  for(int i=0;i<ind;i++)
    it++;
  return *it;
      
};
char SamplesBySite::getSiteValue(int siteIndex,int sampID)
{
  Site& site = getSite(siteIndex);
  return site.getValue(sampID);   
     
};
list<Site> SamplesBySite::getSites(double posStart, double posEnd)
{
  list<Site> res;
  list<Site>::iterator it = sites.begin();
  list<Site>::iterator itEnd = sites.end();  

  while(it != itEnd)
  {
     double pos = it->getPos();
     if((pos >= posStart) && (pos < posEnd))
     {
       res.push_back(*it);
     }else if(pos >= posEnd)
     {      
       break;
     }
     it++;                
  }                         
  return res;                      
}           

/*
get the 0-1 sequence for sampID between posStart and posEnd.

Return:
       result : the char sequence.
       int : size of the sequence.

*/
int SamplesBySite::getData(int sampID, double posStart, double posEnd, char* result)
{
  list<Site> sites = getSites(posStart, posEnd);
  list<Site>::iterator it = sites.begin();
  list<Site>::iterator itEnd = sites.end();
  int i=0;
  while(it != itEnd){
     result[i++] = it->getValue(sampID);   
     it++;                   
  }
  result[i] = '\0';
  return sites.size();    
};    

/*
Return gene string for all samples at same time, given one local tree,
to avoid calling getSites for all sample.

vector<string> SampleBySite::getData(double posStart, double posEnd)
{
  list<Site> sites = getSites(posStart, posEnd);
  list<Site>::iterator it = sites.begin();
  list<Site>::iterator itEnd = sites.end();
  
  vector<string> res;
  int i=0;
  while(it != itEnd){
           
     result[i++] = it->getValue(sampID);   
     it++;                   
  }
  
  result[i] = '\0';
  return sites.size();             
}               
*/
/*
Testing for Site.
*/
/*
class TestSite : public TestBase{
      
      public:
              void test();
      private:
              void testSite();
              void testSiteIsMuted();
      
};
void TestSite::test()
{
  testSite();     
  testSiteIsMuted();         
                
};

void TestSite::testSite()
{
  printf("============= %s =================\n", "testSite");
//  char* str = "SITE:	5	   0.0100770544	01000000100000000000";   
    char* str = "SITE:	5	   0.0100770544	01000000111111111111"; 
  Site* newSite = new Site(20,str);
  newSite->show();
  delete newSite;   
};
void TestSite::testSiteIsMuted()
{
  printf("============= %s =================\n", "testSiteIsMuted");
//  char* str = "SITE:	5	   0.0100770544	01000000100000000000";   
    char* str = "SITE:	5	   0.0100770544	01000000111111111111"; 
  Site* newSite = new Site(20,str);
  newSite->show();
  printf("parent: %c\n", newSite->getParent());
  printf("isMuted(0): %d\n",newSite->isMuted(0));
  printf("isMuted(1): %d\n",newSite->isMuted(1));
  printf("isMuted(19): %d\n",newSite->isMuted(19));
  delete newSite;   
};
//TestSite testsite;


void TestSamplesBySite::test()
{
  testSamplesBySite();    
  testBuildSamplesBySite();       
  testGetSites();   
  testGetData();
                
};
void TestSamplesBySite::testSamplesBySite()
{
  printf("============= %s =================\n", "testSamplesBySite");
  
  SamplesBySite samples(20);
  
  char* str = "SITE:	5	   0.0100770544	00000000100000000000";   
  Site* newSite = new Site(20,str);  
  samples.add(*newSite);
  
  char* str2 = "SITE:	6	   0.0200770544	01111111111111111110";   
  Site* newSite2 = new Site(20,str2);
  samples.add(*newSite2);
  
  char* str3 = "SITE:	7	   0.0300770544	00000000001111111111";   
  Site* newSite3 = new Site(20,str3);
  samples.add(*newSite3);
  
  samples.show();
  double mRate =  samples.getMutRate();
  printf("mut rate: %lf\n", mRate);  
  printf("mut num between 0.02 and 0.04 : %d\n", samples.getMutNum(0.02,0.04));
  
};
void TestSamplesBySite::testBuildSamplesBySite()
{
       printf("============= %s =================\n", "testBuildSamplesBySite");
       
     SamplesBySite* samples = new SamplesBySite(20);
     samples->buildFromFile("data/samples_20_10000_0.01_0.01");
//     samples->show();
     printf("Site num: %d\n", samples->getSiteNum());
     printf("Sample num: %d\n", samples->getSampleSize());
     printf("mut num: %d\n",samples->getMutNum());
     printf("mut rate: %lf\n",samples->getMutRate());
     delete samples;
     
};
void TestSamplesBySite::testGetSites()
{
       printf("============= %s =================\n", "testGetSites");
       
     SamplesBySite* samples = new SamplesBySite(20);
     samples->buildFromFile("data/samples_20_10000_0.01_0.01");
     list<Site> res = samples->getSites(0.1,0.4);
     printf("%d sites are found between 0.1 and 0.4:", res.size());
     list<Site>::iterator it = res.begin();
     list<Site>::iterator itEnd = res.end();
     while(it != itEnd){
       it->show();        
       it++; 
     }
     delete samples;
     
};
void TestSamplesBySite::testGetData()
{
       printf("============= %s =================\n", "testGetData");
       
     SamplesBySite* samples = new SamplesBySite(20);
     samples->buildFromFile("data/samples_20_10000_0.01_0.01");
     char data[256];
     int nSize = samples->getData(0,0.1,0.15,data);
     TestBase::assertEquals(17,nSize);
     printf("size : %d", nSize);
     TestBase::assertEquals(data,"11000000000111110",nSize);
     printf("data: %s\n", data);

     nSize = samples->getData(2,0.4,0.41,data);
     TestBase::assertEquals(3,nSize);
     printf("size : %d", nSize);
     TestBase::assertEquals("011",data,nSize);
     printf("data: %s\n", data);

     delete samples;
     
};*/
//TestSamplesBySite testSamplesBySite;

