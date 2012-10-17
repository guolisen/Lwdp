// TestClient.cpp : Defines the entry point for the console application.
//



#include <Lwdp.h>
#include <LwApi.h>
using namespace NLwdp;
#include <../UModule/Interface/Example/Ix_example.h>

#include <Interface/ConfigMgr/Ix_XmlMgr.h>
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/DctMgr/Ix_DctMgr.h>
 
#include <LwApiLib/LwApi/win32/posix/mqueue/mqueue.h>

#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <boost/shared_ptr.hpp>

struct Foo
{ 
  Foo( int _x ) : x(_x) {}
  ~Foo() { std::cout << "Destructing a Foo with x=" << x << "\n"; }
  int x;
  /* ... */
};

typedef boost::shared_ptr<Foo> FooPtr;

struct FooPtrOps
{
  bool operator()( const FooPtr & a, const FooPtr & b )
    { return a->x > b->x; }
  void operator()( const FooPtr & a )
    { std::cout << a->x << "\n"; }
};



class ConfigSrcImp : public Ix_ConfigSrc
{
public:
	ConfigSrcImp(){};
	ConfigSrcImp(const tstring& file_name):mFileName(file_name){};
	virtual ~ConfigSrcImp(){};


    virtual PC_DATA* LoadConfigData();
	virtual LWRESULT SaveConfigData(PC_DATA* buffer){return LWDP_OK;};
    virtual LWRESULT ReleseConfigData(){return LWDP_OK;};
	
    virtual int32_   GetConfigDataSize(){return 0;};
    virtual const tchar_*  GetConfigNameStr(){return "LocalFile";};

protected:
	tstring mFileName;

};




PC_DATA* ConfigSrcImp::LoadConfigData()
{
    int32_ file_len = 0;
    char *buf = NULL;
    FILE *fp = NULL;

    fp = fopen(mFileName.c_str(), "r");
    if(!fp)
    {
        printf( "FlashUtility::WriteBspFile----fopen(%s) error!\n", mFileName.c_str());
        return NULL;
    }
                     
    if(0 != fseek(fp, 0, SEEK_END))                  
    {                                                  
        printf("fseek() returns error!\n");        
        fclose(fp);                               
        return NULL;                              
    }                                                  
    else                                               
    {                                                  
        file_len = ftell(fp);                    
        printf("file %s  length is %d\n", mFileName.c_str(), file_len);
    }                                                  
                                                                               
    if(0 != fseek(fp, 0, SEEK_SET))                  
    {                                                  
        printf("fseek() returns error!\n");        
        fclose(fp);                               
        return NULL;                              
    }

	buf = new char[file_len];
    if ( NULL == buf )  //ÉêÇëÄÚ´æ
    {
        printf( "FlashUtility::WriteBspFile----malloc(%ld) error!\r\n", file_len);
        fclose(fp);
        return NULL;
    }
    memset(buf, 0, file_len);

	fread( buf, 1, file_len, fp);
#if 0
	int tt = 0;
    if (file_len != (tt =fread( buf, 1, file_len, fp)))
	{
        free(buf);
        fclose(fp);
        printf("FlashUtility::WriteBspFile----fread(%s, %ld) err!\r\n", mFileName.c_str(), file_len );
        return NULL;
    }
#endif
    fclose( fp );

	return (PC_DATA*)buf;
}


typedef std::list<int> INTLIST;

int gThreadInt = 0;
void *
thr1(void * arg)
{
	int i = 2 ;
	int j = 3 ;

  int result;


	GET_OBJECT(LogMgr, iLogMgr, 0);

	while(i--)
	{
		//iLogMgr->LogPrint("Test4", LWDP_LOG_MGR::DEBUG, __FILE__, __LINE__, "Thr Hello World3!!");
		gThreadInt++;
		while(j--)
			printf("TTTTTT %d\n", gThreadInt);

		j = 3;
		//Sleep(30);
	}



  return 0;
}

void *
thr2(void * arg)
{
	int i = 2 ;
int j = 5 ;

  int result;


	GET_OBJECT(LogMgr, iLogMgr, 0);

	while(i--)
	{
		//iLogMgr->LogPrint("Test4", LWDP_LOG_MGR::DEBUG, __FILE__, __LINE__, "Thr Hello World3!!");
		
		gThreadInt++;
		while(j--)
			printf("BBBBBB %d\n", gThreadInt);

		j = 5;
		//Sleep(5);
	}

  return 0;
}

mqd_t gMessageQ = NULL;

void *
thrMessage(void * arg)
{
	int stat = 0;
	while(1)
	{
		struct mq_attr attr;
		stat = mq_getattr(gMessageQ, &attr);
		if(stat) 
		  printf("mq_getattr Error!\n");

		void* buf = malloc(attr.mq_msgsize);
		printf("msg_size: %d\n", attr.mq_msgsize);
		uint32_ prio;
		stat = mq_receive(gMessageQ, (char *)buf, attr.mq_msgsize, &prio);
		if(stat<0) 
		{
		  printf("mq_receive Error!\n");
			continue;
		}

		printf("Recv: %s\n", buf);

		Sleep(10000);
	}
	stat = mq_close(gMessageQ);

}


int32_ main()
{
	LWRESULT stat = LWDP_ERROR;
	ConfigSrcImp csrc("D:/code/LwDp/LwDp/code/bin/xml/ConfigInternal.xml");
	stat = Fw_Init(&csrc, 1);
	if(stat != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "Fw_Init Error(0x%x)!", stat);
		getchar();
		return -1;
	}

	stat = Fw_Start();
	if(stat != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "Fw_Start Error(0x%x)!", stat);
		getchar();
		return -1;
	}

	stat = Fw_Stop();
	if(stat != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "Fw_Stop Error(0x%x)!", stat);
		getchar();
		return -1;
	}

	GET_OBJECT(LogMgr, iLogMgr, 0);
//	iLogMgr->RegisteAppender(MyAppenderFctory_Ptr(new TestAppenderFactory));
	//while(1)
	{
		iLogMgr->LogPrint("Test", LWDP_LOG_MGR::DEBUG, __FILE__, __LINE__, "DEBUG Hello World!!");
		iLogMgr->LogPrint("Test", LWDP_LOG_MGR::ERR, __FILE__, __LINE__, "ERR Hello World!!");
		
		iLogMgr->LogPrint("Test1", LWDP_LOG_MGR::DEBUG, __FILE__, __LINE__, "DEBUG Hello World1!!");
		iLogMgr->LogPrint("Test1", LWDP_LOG_MGR::ERR, __FILE__, __LINE__, "ERR Hello World1!!");

		iLogMgr->LogPrint("Test2", LWDP_LOG_MGR::DEBUG, __FILE__, __LINE__, "DEBUG Hello World2!!");
		iLogMgr->LogPrint("Test2", LWDP_LOG_MGR::ERR, __FILE__, __LINE__, "ERR Hello World2!!");

		iLogMgr->LogPrint("Test3", LWDP_LOG_MGR::DEBUG, __FILE__, __LINE__, "DEBUG Hello World3!!");
		iLogMgr->LogPrint("Test3", LWDP_LOG_MGR::ERR, __FILE__, __LINE__, "ERR Hello World3!!");

		iLogMgr->LogPrint("Test4", LWDP_LOG_MGR::DEBUG, __FILE__, __LINE__, "DEBUG Hello World3!!");
		iLogMgr->LogPrint("Test4", LWDP_LOG_MGR::ERR, __FILE__, __LINE__, "ERR Hello World3!!");


		Sleep(1000);
	}

	LWDP_LOG_PRINT("Test", LWDP_LOG_MGR::ERR, "ERR MACRO TEST(%d)", 1);
	LWDP_LOG_PRINT("Test", LWDP_LOG_MGR::WARNING, "ERR MACRO TEST(%d)", 1);
	
	//GET_OBJECT(DctMgr, iDctMgr, 0);
	//iDctMgr->Init();
	//while(1)
	{
		iLogMgr->LogPrint("Test4", LWDP_LOG_MGR::DEBUG, __FILE__, __LINE__, "DEBUG Hello World3!!");
		iLogMgr->LogPrint("Test4", LWDP_LOG_MGR::ERR, __FILE__, __LINE__, "ERR Hello World3!!");
		Sleep(1000);
	}


	
	
  pthread_t t;
  sem_t s;
  int result;

//  assert(pthread_create(&t, NULL, thr1, NULL) == 0);
 // assert(pthread_create(&t, NULL, thr2, NULL) == 0);

    //assert(pthread_create(&t, NULL, thr2, NULL) == 0);


//  assert(pthread_join(t, (void **)&result) == 0);
//  assert(result == 0);



  //mqd_t mqd;
  gMessageQ = mq_open("21hu.yy", O_CREAT | O_RDWR , 0666, 0);   //O_CREAT | O_RDWR  O_WRONLY
	int cont = 0;
	char st[100] = {0};
	assert(pthread_create(&t, NULL, thrMessage, NULL) == 0);
  while(1)
  {
	  
	sprintf(st, "%d", cont++);
	stat = mq_send(gMessageQ, st, strlen(st)+1, 30);
	if(stat) 
		printf("Send Error!\n");
	Sleep(1);
  }
	getchar();
	return 0;
}


#if 0
int32_ main()
{

	  std::vector<FooPtr>         foo_vector;
	  std::set<FooPtr,FooPtrOps>  foo_set; // NOT multiset!

	  FooPtr foo_ptr( new Foo( 2 ) );
	  foo_vector.push_back( foo_ptr );
	  foo_set.insert( foo_ptr );

	  foo_ptr.reset( new Foo( 1 ) );
	  foo_vector.push_back( foo_ptr );
	  foo_set.insert( foo_ptr );

	  foo_ptr.reset( new Foo( 3 ) );
	  foo_vector.push_back( foo_ptr );
	  foo_set.insert( foo_ptr );

	  foo_ptr.reset ( new Foo( 2 ) );
	  foo_vector.push_back( foo_ptr );
	  foo_set.insert( foo_ptr );

	  std::cout << "foo_vector:\n";
	  std::for_each( foo_vector.begin(), foo_vector.end(), FooPtrOps() );
	  
	  std::cout << "\nfoo_set:\n"; 
	  std::for_each( foo_set.begin(), foo_set.end(), FooPtrOps() );
	  std::cout << "\n";


	printf ("\033[44; The color of this line is red, %s! \033[0m \n", "ha ha");

	NX::Ele ele("Todo");
	NX::Ele ele2("Todo2");
	NX::Attr attr("val");


	NX::EXPR res = ele / ele2;
	NX::EXPR res2 =  ele / ele2.FilterExpr(attr == NX::V("123"));
	res2 =  ele / ele2.FilterExpr(attr == NX::V("1123"));
	res = res/res2;
 
	NX::EXPR res3 = ele / NX::pos(ele2.FilterExpr(attr == NX::V("123")), NX::N("3"));
	NX::EXPR res4 = NX::count(ele / NX::pos(ele2.FilterExpr(attr == NX::V("123")), NX::N("3")));
	NX::EXPR res5 = NX::concat(res3, NX::V(""), NX::V(""));

	ConfigSrcImp csrc("D:/code/LwDp/LwDp/code/bin/xml/Config.xml");
	//	ConfigSrcImp csrc2("D:/code/LwDp/code/bin/xml/Config2.xml");
 

	Fw_Init(&csrc, 1);

	//extern CPluginManager* gPluginLoader;
	//gPluginLoader->Unload();
    Cx_Interface<Ix_Example>   iExample(CLSID_Example);
    Cx_Interface<Ix_ConfigMgr> iConfigMgr(CLSID_ConfigMgr);


#if 1
	CPropertyForm tmpForm[] = {{"ModuleDir", NTagType::kSimple}};
	XPropertys prop;
	iConfigMgr->AddConfigSrc(&csrc);
//	iConfigMgr->AddConfigSrc(&csrc2);

	iConfigMgr->GetModulePropertys("ExampleTest", tmpForm, 1, prop);

	tstring str  = prop["ModuleDir"].propertyText;

	tstring str2 = prop["ModuleDir"].propertyText;

	tstring str3 = prop["Modr"].propertyText;
	tstring str4 = prop["ModuleDir"]["Test"];
	str4 = prop["ModuleDir"]["Test2"];
	str4 = prop["ModuleDir"]["Test3"];
	str4 = prop["dfgdfg"]["dfgs"];

	tstring str5 = prop[0]["Test"];
	tstring str6 = prop[0][0].AttribValue;
	str6 = prop[0][1].AttribValue;
	str6 = prop[0][2].AttribValue;
	str6 = prop[5][4].AttribValue;
	XPropertys prop2;
	iConfigMgr->GetModulePropEntry("ExampleTest", "Version", prop);
	iConfigMgr->GetModulePropEntry("ExampleTest", "ModuleDir", prop);

	str6 = prop[0][2].AttribValue;


	XPropertyTable pTable;
	iConfigMgr->GetModuleTable("ExampleTest", "TestTable", pTable);
	Script_Ptr pData; 
	iConfigMgr->GetModuleScript("ExampleTest", "ExampleTestInit", pData);

	prop.Clear();
#endif
	long_ test = 123;	
	iExample->Foo(test);
	lw_log_err(LWDP_API_LOG, __T("Main!!%d"), 23);

    Cx_Interface<Ix_XmlMgr>  iXmlMgr(CLSID_XmlMgr);

//	iXmlMgr->XmlParse("<Root/>");
//	tstring retbool;
//	iXmlMgr->XmlParseExprStr("4 * 2", retbool);

	iXmlMgr->XmlParse((XML_BUFFER*)csrc.LoadConfigData());

	XmlResults prop23;
//	typedef XmlResults   	XPropertys;
//	typedef XmlResultsSet   XPropertyTable;

	NX::Ele moduleTag("Module");
	NX::Ele propTag("Property");
	NX::Ele nameXPath  = moduleTag.FilterExpr(NX::Attr("Name") == NX::V("ExampleTest")) / propTag ;
	NX::Ele modidXPath = moduleTag.FilterExpr(NX::Attr("Modid") == NX::V("ExampleTest")) / propTag ;

	NX::Ele testTree =  moduleTag.FilterExpr(NX::Attr("Name") == NX::V("LWDP")); 

	TagTree_Ptr tmpTree(new TreeEntry);
	iXmlMgr->XmlGetElementTree(testTree, tmpTree);


	
	iXmlMgr->XmlGetElement(nameXPath, tmpForm, 1, prop23);
	str  = prop23["ModuleDir"].propertyText;

	str2 = prop23["ModuleDir"].propertyText;

	str3 = prop23["Modr"].propertyText;
	str4 = prop23["ModuleDir"]["Test"];
	str4 = prop23["ModuleDir"]["Test2"];
	str4 = prop23["ModuleDir"]["Test3"];
	str4 = prop23["dfgdfg"]["dfgs"];

	str5 = prop23[0]["Test"];
	str6 = prop23[0][0].AttribValue;
	str6 = prop23[0][1].AttribValue;
	str6 = prop23[0][2].AttribValue;
	str6 = prop23[5][4].AttribValue;


	NX::Ele tableSetTag("TableSet");
	NX::Ele tableTag(__T("PropertyTable"));
	nameXPath  = moduleTag.FilterExpr(NX::Attr("Name") == NX::V("ExampleTest")) / tableSetTag / tableTag.FilterExpr(NX::Attr("Name") == NX::V("TestTable")) ;
	modidXPath = moduleTag.FilterExpr(NX::Attr("Modid") == NX::V("86347b32-64e4-490c-b273-ec7e010f244e")) / tableSetTag / tableTag.FilterExpr(NX::Attr("Name") == NX::V("TestTable"));
	
	XmlResultsSet propSet;
	iXmlMgr->XmlGetElementSet(nameXPath, propSet);
	propSet.Clear();
	iXmlMgr->XmlGetElementSet(modidXPath, propSet);

	NX::Ele scriptTag(__T("Script"));
	nameXPath  = moduleTag.FilterExpr(NX::Attr("Name") == NX::V(__T("LWDP"))) / scriptTag.FilterExpr(NX::Attr("Name") == NX::V(__T("GlobalInit")));
	modidXPath = moduleTag.FilterExpr(NX::Attr("Modid") == NX::V(__T("LWDP"))) / scriptTag.FilterExpr(NX::Attr("Name") == NX::V(__T("GlobalInit")));

	Xml_Ptr code_buf;
	iXmlMgr->XmlGetElementBuf(nameXPath, code_buf);

    Cx_Interface<Ix_ScriptMgr>  iScriptMgr(CLSID_ScriptMgr);
	//iConfigMgr->
	
	LWRESULT stat = iScriptMgr->SyncScriptRun(code_buf);
//	printf("sdfsdfsdf\n");
	if(stat != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "SyncScriptRun ERROR(%x)!", stat);
	}
	getchar();
	return 0;
}

#endif
