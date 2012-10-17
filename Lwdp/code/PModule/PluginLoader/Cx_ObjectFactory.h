// Copyright 2008-2011 Zhang Yun Gui, rhcad@hotmail.com
// http://sourceforge.net/projects/x3c/

// author: Zhang Yun Gui, Tao Jian Lin
// v2: 2011.01.05, ooyg: Change class-table to hash_map
// v3: 2011.02.04, ooyg: Add filename in MODULEINFO. Add module index in class map.
// v4: 2011.02.07, ooyg: Implement the delay-loaded feature.
// v5: 2011.02.16, ooyg: Avoid plugin loading when a plugin is unloading.

#ifndef _X3_CORE_OBJECTFACTORY_H
#define _X3_CORE_OBJECTFACTORY_H

#include <Module/XClassItem.h>
#include <Interface/Ix_Module.h>
#include <Interface/PluginLoader/Ix_ObjectFactory.h>

#if defined(_MSC_VER) && _MSC_VER > 1200    // not VC6
    #include <hash_map>
    using stdext::hash_map;
/*
#elif defined(__GNUC__)                     // gcc
    #include <ext/hash_map>
    using __gnu_cxx::hash_map;
    namespace __gnu_cxx {
        template<> struct hash<tstring>
        {
            size_t operator()(const tstring& s) const
            {
                return hash<tchar_*>()(s.c_str());
            }
        };
    } // of namespace __gnu_cxx
*/
#else                                       // VC6 or others
    #define hash_map std::map
#endif

LWDP_NAMESPACE_BEGIN;

class Cx_ObjectFactory : public Ix_ObjectFactory
{
    X3BEGIN_CLASS_DECLARE(Cx_ObjectFactory)
        X3DEFINE_INTERFACE_ENTRY(Ix_ObjectFactory)
    X3END_CLASS_DECLARE();
public:
    Cx_ObjectFactory();
    virtual ~Cx_ObjectFactory();

public:
    virtual bool IsCreatorRegister(const LWCLSID& clsid);
    virtual Ix_Object* CreateObject(const LWCLSID& clsid, X3IID iid, Ix_Object** ppv, const MODID& fromModule);

protected:
    typedef std::vector<LWCLSID>         	CLSIDS;
    typedef std::pair<X3CLASSENTRY, long_>  MAPITEM;    //!< entry+moduleIndex
    typedef hash_map<tstring, MAPITEM> 		CLSMAP;       //!< clsid+item

    struct MODULE                   //!< plugin module info
    {
    	MODID		modid;
        Ix_Module*	module;     //!< plugin module object
        CLSIDS		clsids;     //!< all classes of the plugin
        bool		owned;      //!< the DLL is loaded by this class or not
        bool		inited;     //!< InitializePlugins() has been called or not
        tchar_		filename[LWDP_MAX_PATH];   //!< plugin filename

        MODULE() : modid(NULL), module(NULL), owned(false), inited(false) {}
    };

    std::vector<MODULE*>    m_modules;  //!< all plugin modules
    CLSMAP                  m_clsmap;   //!< map from clsid to class factory
    long_                   m_unloading;    //!< positive if a a plugin is unloading

protected:
    int32_ 		FindModule(const MODID& modid);
    Ix_Module* 	GetModule(const MODID& modid);
    long_ 		RegisterClassEntryTable(int32_ moduleIndex);
    LWRESULT 	ReleaseModule(const MODID& modid);
    X3CLASSENTRY* FindEntry(const LWCLSID& clsid, int32_* moduleIndex = NULL);
    LWRESULT    ReferenceCheck(const MODID& target_modid, const MODID& srouce_modid);

private:
    Cx_ObjectFactory(const Cx_ObjectFactory&);
    void operator=(const Cx_ObjectFactory&);
    bool RegisterClass(int32_ moduleIndex, const X3CLASSENTRY& cls);
    virtual bool LoadDelayPlugin(const tchar_* filename) = 0;
};

LWDP_NAMESPACE_END;

#endif // _X3_CORE_OBJECTFACTORY_H
