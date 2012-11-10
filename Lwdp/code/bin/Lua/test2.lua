print("Lua script test2 print!!!!!!!!!!!!")


--  Log Manager --

print("///////////////////////// <<< Log Manager >>> /////////////////////////")
LogMgr.LuaLogPrint("Test", DEBUG, "FileName", 13, "Hello!!!!!")
LogMgr.LuaLogPrint("Test", INFO, "FileName1", 13, "Hello!!!!!1")
LogMgr.LuaLogPrint("Test", NOTICE, "FileName2", 13, "Hello!!!!!2")
LogMgr.LuaLogPrint("Test", WARNING, "FileName3", 13, "Hello!!!!!3")
LogMgr.LuaLogPrint("Test", ERR, "FileName4", 13, "Hello!!!!!4")
LogMgr.LuaLogPrint("Test", CRIT, "FileName5", 13, "Hello!!!!!5")
LogMgr.LuaLogPrint("Test", ALERT, "FileName6", 13, "Hello!!!!!6")
LogMgr.LuaLogPrint("Test", EMERG, "FileName7", 13, "Hello!!!!!7")
print("///////////////////////// <<< Log Manager >>> /////////////////////////\n")


--  Config Manager --

print("///////////////////////// <<< Config Manager >>> /////////////////////////")
local a = Script_Ptr.new()
ConfigMgr.GetScript("LWDP", "GlobalInit", a)
print(Script_Ptr.get(a))

local prop = XmlResults:new_local();
ConfigMgr.GetModulePropEntry("ExampleTest", "Version", prop)
print(XmlResultsMod.get(prop[0].propertyText))

local propTab = XmlResultsSet:new_local();
local ret = ConfigMgr.GetModuleTable("ExampleTest", "TestTable", propTab)
print("!!!RET: " .. ret)


i = 0;
while propTab[i].ThereIs do
	print("-------------")
	print(propTab[i]:GetAttrByName("ts"))
	print(propTab[i][1].AttribValue)
	print("-------------")
	i=i+1;
end

print("//////////////////////// <<< Config Manager >>> /////////////////////////\n")


--  Test Fun --

local ret = TestAdd(23, 100)

print(ret)
