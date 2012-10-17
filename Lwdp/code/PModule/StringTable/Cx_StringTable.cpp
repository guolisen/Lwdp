// Copyright 2008-2011 Zhang Yun Gui, rhcad@hotmail.com
// http://sourceforge.net/projects/x3c/

#include <UtilFunc/PluginInc.h>
#include "Cx_StringTable.h"
#include <UtilFunc/ScanFiles.h>
#include <Xml/Ix_ConfigXml.h>
#include <Xml/Cx_ConfigSection.h>
#include <UtilFunc/RelToAbs.h>

Cx_StringTable::Cx_StringTable()
    : m_loaded(false)
{
}

Cx_StringTable::~Cx_StringTable()
{
}

tstring Cx_StringTable::GetValue(const tstring& module, 
                                      const tstring& id, bool* hasvalue)
{
    tstring value;

    GetValue(value, module, id);
    if (hasvalue)
    {
        *hasvalue = !value.empty();
    }
    if (value.empty())
    {
        X3LOG_WARNING2(__T"@StringTable:IDS_NO_STRVALUE", module +__T":" + id);
    }

    return value;
}

bool Cx_StringTable::GetValue(tstring& value, const tstring& name, 
    tstring& module, tstring& id)
{
    if (name.size() > 1 && L'@' == name[0] &&
        wcschr(name.c_str(), L':') != NULL)
    {
        const tchar_* p = wcschr(name.c_str(), L':');

        module = tstring(name.c_str() + 1, (size_t)(p - name.c_str() - 1));
        id = p + 1;

        return GetValue(value, module, id);
    }

    return false;
}

static void ReplaceLf(tstring& text)
{
    for (int32_ i = static_cast<int32_>(text.size()) - 1; i > 0; --i)
    {
        tchar_ c = text[i];
        if (L'\\' == text[i - 1] && (L'n' == c || L'r' == c || L't' == c))
        {
            switch (c)
            {
                case L'n': text[i - 1] = L'\n'; break;
                case L'r': text[i - 1] = L'\r'; break;
                case L't': text[i - 1] = L'\t'; break;
            }
            text.erase(i, 1);
        }
    }
}

bool Cx_StringTable::GetValue(tstring& value, 
    const tstring& module, const tstring& id)
{
    if (!m_loaded)
    {
        LoadFiles(x3::RelToAbsWithPlugin(__T"../Translations/Strings", false));
    }

    IT_ITEM it = Find(module);

    value.resize(0);
    if (it != m_groups.end())
    {
        Cx_ConfigSection group(it->group);
        ASSERT(group.IsNotNull());

        Cx_ConfigSection sec(group.GetSection(__T"string",__T"id", id.c_str(), false));
        value = sec->GetString(__T"value");
        ReplaceLf(value);
    }

    return !value.empty();
}

long_ Cx_StringTable::LoadFiles(const tstring& path)
{
    if (path.empty())
    {
        return 0;
    }

    long_ count = 0;
    std::vector<tstring> files;
    x3::CScanFilesByExtension scaner(&files,__T".xml");

    x3::ScanFiles(&scaner, path.c_str(), false);
    m_loaded = true;

    std::vector<tstring>::const_iterator it = files.begin();
    for (; it != files.end(); ++it)
    {
        if (RegisterFile(*it) > 0)
        {
            count++;
        }
    }
    
    if (0 == count)
    {
        X3LOG_INFO2(__T"@StringTable:IDS_NO_STRFILE", path);
    }

    return count;
}

long_ Cx_StringTable::RegisterFile(const tstring& filename)
{
    Cx_Interface<Ix_ConfigXml> pIFFile(x3::CLSID_ConfigXmlFile);
    if (pIFFile.IsNull())
    {
        return 0;
    }

    pIFFile->SetFileName(filename.c_str());
    X3LOG_DEBUG2(__T"@StringTable:IDS_LOAD_STRFILE", Api_PathFindFileName(filename.c_str()));

    long_ count = 0;
    for (int32_ i = 0; i < 99; i++)
    {
        Cx_ConfigSection sec (pIFFile->GetData()->GetSectionByIndex(NULL,__T"module", i));
        ITEM item;

        item.file = pIFFile->GetData();
        item.group = sec;
        item.module = sec->GetString(__T"name");
        ASSERT(item.group && item.file);

        if (item.module.empty())
            break;

        if (Find(item.module) == m_groups.end())
        {
            m_groups.push_back(item);
            count++;
        }
        else
        {
            X3LOG_WARNING2(__T"@StringTable:IDS_IGNORE_STRGROUP", item.module);
        }
    }

    return count;
}
