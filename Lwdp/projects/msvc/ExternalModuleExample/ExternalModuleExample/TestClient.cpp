
#include <Lwdp.h>
#include <LwApi.h>
using namespace NLwdp;
//#include <../UModule/Interface/Example/Ix_example.h>

#include <Interface/ConfigMgr/Ix_XmlMgr.h>
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include <Interface/LuaMgr/Ix_LuaMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>
#include <Interface/ZmqMgr/Ix_ZmqMgr.h>
#include <Interface/EventMgr/Ix_EventMgr.h>
#include <../UModule/Interface/TSFrontend/Ix_TSFrontend.h>

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

///////////////////////////////////////////////

#define NBR_CLIENTS 5
#define NBR_WORKERS 4

static unsigned int __stdcall
worker_task (void *args)
{
	GET_OBJECT_RET(ZmqMgr, iZmqMgr, 0);

	ContextHandle context = iZmqMgr->GetNewContext();
	SocketHandle responder = iZmqMgr->GetNewSocket(context, LWDP_REP);

	iZmqMgr->Connect(responder, "tcp://localhost:5560");

	while (1) {
		// Wait for next request from client
		std::string retdata = iZmqMgr->Recv(responder, 0);
		printf ("Received request: [%s]\n", retdata.c_str());

		// Do some 'work'
		Sleep (1);

		// Send reply back to client
		iZmqMgr->Send(responder, "World", 6, 0);
	}
	// We never get here but clean up anyhow

	iZmqMgr->CloseSocket(responder);
	iZmqMgr->CloseContext(context);
	
	return 0;
}

static unsigned int __stdcall
client_task (void *args)
{
	GET_OBJECT_RET(ZmqMgr, iZmqMgr, 0);
	
	ContextHandle context = iZmqMgr->GetNewContext();
	SocketHandle requester = iZmqMgr->GetNewSocket(context, LWDP_REQ);

	iZmqMgr->Connect(requester, "tcp://localhost:5559");


	int request_nbr;
	for (request_nbr = 0; request_nbr != 10; request_nbr++) 
	{
		iZmqMgr->Send(requester, "Hello", 6, 0);

		std::string retdata = iZmqMgr->Recv(requester, 0);
		
		printf ("Received reply %d [%s]\n", request_nbr, retdata.c_str());
	
	}

	iZmqMgr->CloseSocket(requester);
	iZmqMgr->CloseContext(context);
	return 0;
}


void io_callback(void *loop, void *w, int revents)
{
	printf("!!!!!Call Back!!!!\n");
}

int32_ main()
{
	LWRESULT stat = LWDP_ERROR;

	ConfigSrcImp csrc("../../../../Code/bin/xml/ConfigExternal.xml");
	stat = Fw_Init(&csrc, 1);
	if(stat != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "Fw_Init Error(0x%x)!", stat);
		system("pause");
		return -1;
		
	} 

	stat = Fw_Start();
	if(stat != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "Fw_Start Error(0x%x)!", stat);
		system("pause");
		return -1;
		
	}
 
	stat = Fw_Stop();
	if(stat != LWDP_OK)
	{
		lw_log_err(LWDP_MODULE_LOG, "Fw_Stop Error(0x%x)!", stat);
		system("pause");
		return -1;
		
	}

	//GET_OBJECT_RET(EventMgr, iEventMgr, 0);

	//RINOK(iEventMgr->InitLoop(0));

	GET_OBJECT_RET(TSFrontend, iTSFrontend, 0);

	LWRESULT ret = iTSFrontend->Init();

	ret = iTSFrontend->RunServer();
#if 0



    int client_nbr;
    for (client_nbr = 0; client_nbr < NBR_CLIENTS; client_nbr++) {
        HANDLE client;
        client = (HANDLE) _beginthreadex (NULL, 0,
        client_task, 0, 0 , NULL);
        if (client == 0) {
            printf ("error in _beginthreadex\n");
            return -1;
        }
    }
    int worker_nbr;
    for (worker_nbr = 0; worker_nbr < NBR_WORKERS; worker_nbr++) {
        HANDLE worker;
        worker = (HANDLE) _beginthreadex (NULL, 0,
        worker_task, 0, 0 , NULL);
        if (worker == 0) {
            printf ("error in _beginthreadex\n");
            return -1;
        }
    }



	GET_OBJECT_RET(ZmqMgr, iZmqMgr, 0);
	
	ContextHandle context = iZmqMgr->GetNewContext();
	SocketHandle frontend = iZmqMgr->GetNewSocket(context, LWDP_ROUTER);
	SocketHandle backend = iZmqMgr->GetNewSocket(context, LWDP_DEALER);

	iZmqMgr->Bind(frontend, "tcp://*:5559");
	iZmqMgr->Bind(backend, "tcp://*:5560");

	// Initialize poll set
	LWDP_POLLITEM_T items [] = {
	    { frontend, 0, LWDP_POLLIN, 0 },
	    { backend, 0, LWDP_POLLIN, 0 }
	};
	// Switch messages between sockets
	while (1) {
		
	    int more; // Multipart detection

	    iZmqMgr->Poll(items, 2, -1);
	    if (items [0].revents & LWDP_POLLIN) 
		{
	        while (1) 
			{
				GET_OBJECT_RET(ZMessage, iZMessage, 0);
	            // Process all parts of the message
	            iZMessage->InitZMessage();
	            iZmqMgr->Recv(frontend, iZMessage, 0);
	            
	            uint32_ more_size = sizeof (more);
				iZmqMgr->Getsockopt(frontend, LWDP_RCVMORE, &more, &more_size);
				iZmqMgr->Send(backend, iZMessage, more? LWDP_SNDMORE: 0);
                if (!more)
                    break; // Last message part
	         }
	    }
        if (items [1].revents & LWDP_POLLIN) 
		{
            while (1) 
			{
				GET_OBJECT_RET(ZMessage, iZMessage, 0);
                // Process all parts of the message
				iZMessage->InitZMessage();
	            iZmqMgr->Recv(backend, iZMessage, 0);
				
                size_t more_size = sizeof (more);
				iZmqMgr->Getsockopt(backend, LWDP_RCVMORE, &more, &more_size);
				iZmqMgr->Send(frontend, iZMessage, more? LWDP_SNDMORE: 0);
                if (!more)
                    break; // Last message part
            }
         }
	}
	                // We never get here but clean up anyhow
	iZmqMgr->CloseSocket(frontend);
	iZmqMgr->CloseSocket(backend);
	iZmqMgr->CloseContext(context);

#endif



	GET_OBJECT_RET(LuaMgr, iLuaMgr, 0);
	iLuaMgr->DoFile("../../../../code/bin/Lua/Test2.lua");

//	long_ val1 = 100;
//	GET_OBJECT_RET(Example, iExample, 0);
//	iExample->Foo(val1);

	system("pause");
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
