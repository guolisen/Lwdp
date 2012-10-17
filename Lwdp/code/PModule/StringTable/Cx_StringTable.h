// Copyright 2008-2011 Zhang Yun Gui, rhcad@hotmail.com
// http://sourceforge.net/projects/x3c/

#ifndef _X3_CORE_STRINGTABLE_H
#define _X3_CORE_STRINGTABLE_H

#include <Xml/Ix_StringTable.h>

class Cx_StringTable : public Ix_StringTable
{
    X3BEGIN_CLASS_DECLARE(Cx_StringTable)
        X3DEFINE_INTERFACE_ENTRY(Ix_StringTable)
    X3END_CLASS_DECLARE()
protected:
    Cx_StringTable();
    ~Cx_StringTable();

private:
    bool GetValue(tstring& value, const tstring& name,
        tstring& module, tstring& id);
    bool GetValue(tstring& value,
        const tstring& module, const tstring& id);
    long_ LoadFiles(const tstring& path);
    long_ RegisterFile(const tstring& filename);
    tstring GetValue(const tstring& module,
        const tstring& id, bool* hasvalue = NULL);

private:
    struct ITEM
    {
        tstring    module;
        Cx_Ptr          group;  // Ix_ConfigSection
        Cx_Ptr          file;   // Ix_ConfigData
        ITEM() {}
    };
    typedef std::list<ITEM>::iterator IT_ITEM;

    IT_ITEM Find(const tstring& module)
    {
        IT_ITEM it = m_groups.begin();
        for (; it != m_groups.end(); ++it)
        {
            if (_wcsicmp(it->module.c_str(), module.c_str()) == 0)
            {
                break;
            }
        }

        return it;
    }

private:
    std::list<ITEM>     m_groups;
    bool                m_loaded;
};

#endif // _X3_CORE_STRINGTABLE_H
