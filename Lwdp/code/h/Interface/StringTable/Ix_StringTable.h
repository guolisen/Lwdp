/*! \file Ix_StringTable.h
 *  \brief Define the localization string table interface: Ix_StringTable
 *  \author Zhang Yun Gui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
#ifndef X3_XML_ISTRINGTABLE_H_
#define X3_XML_ISTRINGTABLE_H_

#include "Ix_Object.h"

CLSID_DEFINE(CLSID_StringTable, "b8c36b29-59c3-4db2-be43-fd4982e6e71d");

//! The localization string table interface.
/*! The string table's xml file is saved under the path: exepath/Translations/Strings/ ,
    just same as x3::FileNameRelToAbs(__T"Translations/Strings", false).
    \interface Ix_StringTable
    \ingroup _GROUP_PLUGIN_XML_
    \see x3::CLSID_StringTable
*/
class Ix_StringTable : public Ix_Object
{
public:
    DEFINE_IID(Ix_StringTable)

    //! Get the string value of the specified module and id name.
    /*!
        \param[out] value the actual value if exists.
        \param[in] name a string contains module and id name, the first tchar_ is '@' and leading as 'Module:StrID'.
        \param[out] module fills the module name.
        \param[out] id fills the id name, eg: "IDS_XXX", "GROUP_XXX".
        \return true if the result is not empty.
    */
    virtual bool GetValue(tstring& value, const tstring& name, 
        tstring& module, tstring& id) = 0;

    //! Get the string value of the specified module and id name.
    /*!
        \param[out] value the actual value if exists.
        \param[in] module the specified module name.
        \param[in] id the specified id name, eg: "IDS_XXX", "GROUP_XXX".
        \return true if the result is not empty.
    */
    virtual bool GetValue(tstring& value, 
        const tstring& module, const tstring& id) = 0;

    //! Load string table's xml files in the specified path.
    /*!
        \param path the specified path contains xml files,
            empty string is represented as the default path: exepath/Translations/Strings/ ,
            just same as x3::FileNameRelToAbs(__T"Translations/Strings", false).
        \return the count of files loaded.
    */
    virtual long_ LoadFiles(const tstring& path) = 0;

    //! Get the string value of the specified module and id name.
    /*!
        \param[in] module the specified module name.
        \param[in] id the specified id name, eg: "IDS_XXX", "GROUP_XXX".
        \param[out] hasvalue fills the result has value or not if hasvalue is not null.
        \return the actual value or empty string.
    */
    virtual tstring GetValue(const tstring& module, 
        const tstring& id, bool* hasvalue = NULL) = 0;
};

#ifdef X3_CORE_XCOMPTR_H_
namespace x3 {
//! Get the string value of the specified module and id name.
/*!
    \param[in] module the specified module name.
    \param[in] id the specified id name, eg: "IDS_XXX", "GROUP_XXX".
    \param[out] hasvalue fills the result has value or not if hasvalue is not null.
    \return the actual value or empty string.
*/
inline tstring GetStringValue(const tstring& module, 
    const tstring& id, bool* hasvalue = NULL)
{
    Cx_Interface<Ix_StringTable> pIFTable(x3::CLSID_StringTable);
    return pIFTable ? pIFTable->GetValue(module, id, hasvalue) : tstring();
}
} // x3
#endif

#endif // X3_XML_ISTRINGTABLE_H_
