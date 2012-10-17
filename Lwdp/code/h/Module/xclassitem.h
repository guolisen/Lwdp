/*! \file   XClassItem.h
 *  \brief  Internal used definitions of class factory.
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
#ifndef X3_PLUGINIMPL_CLASSITEM_H_
#define X3_PLUGINIMPL_CLASSITEM_H_

#include <Interface/Ix_Object.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

//! class factory function.
typedef Ix_Object* (*PFNXObjectCreator)(X3IID, const MODID&);

//! object count of a class.
typedef long_ (*PFNXGetObjectCount)();

//! object (used by other modules) count of a class.
typedef long_ (*PFNXRefCountByOthers)();

#define MIN_SINGLETON_TYPE  10

//#pragma pack(push, 8)

/*! \ingroup _GROUP_PLUGIN_CORE2_
 *  \brief class factory registry.
 *  \see   x3GetClassEntryTable, XBEGIN_DEFINE_MODULE
 *  \internal
 */
class X3CLASSENTRY
{
public:
    BYTE                type;               //!< see MIN_SINGLETON_TYPE and XModuleMacro.h
    const tchar_*       className;          //!< implement class name
    LWCLSID             clsid;              //!< class id
    MODID               modid;              //!< module id of this class
    PFNXObjectCreator   pfnObjectCreator;   //!< class factory function
    PFNXGetObjectCount  pfnGetObjectCount;  //!< object count of this class
    PFNXRefCountByOthers    pfnRefCountByOthers;    //!< count of objects used by other modules

    //! Used by XDEFINE_CLASSMAP_ENTRY, XDEFINE_CLASSMAP_ENTRY_Singleton
    X3CLASSENTRY(BYTE    _type,
        const tchar_*           _className,
        const LWCLSID&          _clsid,
        const MODID&            _modid,
        PFNXObjectCreator       _pfnObjectCreator,
        PFNXGetObjectCount      _pfnGetObjectCount = NULL,
        PFNXRefCountByOthers    _pfnRefCountByOthers = NULL)

        : type(_type), className(_className), clsid(_clsid), modid(_modid)
        , pfnObjectCreator(_pfnObjectCreator)
        , pfnGetObjectCount(_pfnGetObjectCount)
        , pfnRefCountByOthers(_pfnRefCountByOthers)
    {
    }

    //! Used by XEND_DEFINE_MODULE
    X3CLASSENTRY()
        : type(0), className(__T("")), clsid(__T("")), modid(__T(""))
        , pfnObjectCreator(NULL), pfnGetObjectCount(NULL)
        , pfnRefCountByOthers(NULL)
    {
    }

    //! class factory registries. filled by XBEGIN_DEFINE_MODULE.
    //static const X3CLASSENTRY s_classes[];
};

/*! \ingroup _GROUP_PLUGIN_CORE2_
 *  \brief class factory registry.
 *  \see   x3GetClassEntryTable, XBEGIN_DEFINE_MODULE
 *  \internal
 */
struct X3MODULEINFO
{
    const tchar_*	className;          //!< implement class name
    TPLUGIN_CODE_DOMAIN   codeDomain; 
	TPLUGIN_LEVEL   pluginLevel;
	TPLUGIN_TYPE    pluginType;
    VER_TYPE		pluginVersion;      //!< Plugin Version
    int32_			unicodeTag;         //!< 1 UNICODE 0 ASCII
    MODULEID        moduleId;
    MODID			modId;              //!< module id

//	X3MODULEINFO() : className(NULL),codeDomain(UMODULE_CODE_DOMAIN),
//		pluginType(0), pluginVersion(iPLUGIN_VERSION), unicodeTag(UNICODE_TAG),
//		moduleId(-1),modId(NULL){}
};

//#pragma pack(pop)

EXTERN_C_END;
LWDP_NAMESPACE_END;


#endif // X3_PLUGINIMPL_CLASSITEM_H_
