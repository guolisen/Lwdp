print("Lua script test2 print!!!!!!!!!!!!")

LogMgr.LuaLogPrint("Test2", 0, "FileName", 13, "Hello!!!!!")
LogMgr.LuaLogPrint("Test2", 1, "FileName1", 13, "Hello!!!!!1")
LogMgr.LuaLogPrint("Test2", 2, "FileName2", 13, "Hello!!!!!2")
LogMgr.LuaLogPrint("Test2", 3, "FileName3", 13, "Hello!!!!!3")
LogMgr.LuaLogPrint("Test2", 4, "FileName4", 13, "Hello!!!!!4")
LogMgr.LuaLogPrint("Test2", 5, "FileName5", 13, "Hello!!!!!5")
LogMgr.LuaLogPrint("Test2", 6, "FileName6", 13, "Hello!!!!!6")
LogMgr.LuaLogPrint("Test2", 7, "FileName7", 13, "Hello!!!!!7")


local ret = TestAdd(23, 100)

print(ret)
