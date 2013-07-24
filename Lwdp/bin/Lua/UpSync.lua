
luasql = require("luasql.mysql")
curl   = require("luacurl")
md5    = require("md5")
json   = require("dkjson")
--iconv  = require("luaiconv")

local DbServerIp  = "10.3.18.27"
local DbUser 	  = "ptsf"
local DbPassword  = "123456"
local DbName 	  = "scenic_0517"
local DbPort 	  = 3306

local REQ_ADDR = "http://10.3.18.27/cardc/1.0/sync/upload"


function get_html(url, post_value)
    local result = { }
   
    local c = curl.new()
    
    c:setopt(curl.OPT_URL, url)
	c:setopt(curl.OPT_POSTFIELDS, post_value)
    c:setopt(curl.OPT_WRITEDATA, result)
    c:setopt(curl.OPT_WRITEFUNCTION, 
    function(tab, buffer)     --call back函数，必须有
        table.insert(tab, buffer)                      --tab参数即为result，参考http://luacurl.luaforge.net/
        return #buffer
    end)
    local ok = c:perform()
    c:close()
    return ok, table.concat(result)             --此table非上一个table，作用域不同
end


function get_sign(srcStr, key)
	local entrys = {}
	for w in string.gfind(srcStr, "[^&=]+=[^&=]+") do
		table.insert(entrys, w)
	end
	
	table.sort(entrys)

	local i = 1
	local outStr = ""
	for i=1,#entrys do
		outStr = outStr .. entrys[i] .. "&"
	end
	outStr = outStr .. "key=" .. key
	local sg = md5.sumhexa (outStr)
	return sg
end

function check_one(to, from, text)
  --print("\n-- Testing conversion from " .. from .. " to " .. to)
  local cd = iconv.new(to .. "//TRANSLIT", from)
  assert(cd, "Failed to create a converter object.")
  local ostr, err = cd:iconv(text)

  if err == iconv.ERROR_INCOMPLETE then
    print("ERROR: Incomplete input.")
  elseif err == iconv.ERROR_INVALID then
    print("ERROR: Invalid input.")
  elseif err == iconv.ERROR_NO_MEMORY then
    print("ERROR: Failed to allocate memory.")
  elseif err == iconv.ERROR_UNKNOWN then
    print("ERROR: There was an unknown error.")
  end
  return ostr
end

function main()
	local retCode = 0;
	env = assert (luasql.mysql())
	-- connect to data source
	mysql_con = assert (env:connect(DbName, DbUser, DbPassword, DbServerIp, DbPort))

	res = assert(mysql_con:execute([[set names utf8]]))
	res = assert(mysql_con:execute([[SELECT DATE_FORMAT(NOW(),'%Y-%m-%d %H:%i:%s') dt ]]))
	ent_tab = assert(mysql_con:execute([[select enterprise_id ent_id,ekey from sc_enterprise]]))
	ent_row = ent_tab:fetch ({}, "a") 
	local loc_ent_id  = ent_row.ent_id
	local loc_ent_key = ent_row.ekey
	ent_tab:close()

	local timeTab = os.date("*t")
	local nowDate = string.format("%s-%s-%s %s:%s:%s", timeTab.year, timeTab.month, timeTab.day,
													   timeTab.hour, timeTab.min, timeTab.sec)
	print("Now Time: " .. nowDate)
				
	local sql = [[SELECT qsql, task_name, DATE_FORMAT(update_time,'%Y-%m-%d %H:%i:%s') begin_date FROM sc_sync WHERE DATE_FORMAT(']].. nowDate ..[[','%Y-%m-%d %H:%i:%s') > next_time and  act=1 and flag=1 ]]
	print("Searching SC Sync Tab(" .. sql .. ")")			
	sync_tab = mysql_con:execute(sql)
	if sync_tab == nil then
		print("Execute sync_tab Sql Error")
		return 1;
	end		

	print("////////////////////////////////////////////////")		
	sync_row = sync_tab:fetch ({}, "a")		
	while sync_row do	
		print("<<<<Start Sync Task " .. sync_row.task_name .. ">>>> \n")
		local reqSql = sync_row.qsql
		reqSql = string.gsub(reqSql, "#bdate#", tostring(sync_row.begin_date))
		reqSql = string.gsub(reqSql, "#edate#", tostring(nowDate))
		print("Get Tab SQL(" .. reqSql .. ")")
		req_tab = mysql_con:execute(reqSql)
		if req_tab == nil then
			print("Execute req_tab Sql Error")
			return 1;
		end	

		local sendSet = {}
		reqTab_row = req_tab:fetch ({}, "a")	
		
		while reqTab_row do	
			for key,value in pairs(reqTab_row) do
				--table.insert(sendSet, key .."="..  check_one("gbk", "utf-8", tostring(value)))
				table.insert(sendSet, key .."="..  tostring(value))
			end
			reqTab_row = req_tab:fetch ({}, "a")	
	    end
	    req_tab:close()

	    table.insert(sendSet, "_task=".. sync_row.task_name)
	    table.insert(sendSet, "_ent_id=".. loc_ent_id)
	    local sendStr = ""
	    sendStr = table.concat(sendSet, "&")
	
		local md5Key = get_sign(sendStr, loc_ent_key)
		sendStr = sendStr .. "&_sign=" .. md5Key

		--print(md5Key)
		print("Sending to CC (" .. sendStr .. ")\n")
		ok, html = get_html(REQ_ADDR, sendStr)
		if not ok or not html then
		    print ("Send Request To CC Error")   
			return 1
		end	

		print(html)
		local srcObj, tmp, err = json.decode (html, 1, nil)
		if err then
			print ("Json Parse CC_Src Error: ", err)
			return 1
		end

		if srcObj.code ~= "0000" then
			print("CC Execute Error Return Code(" .. srcObj.code .. ") msg(" .. srcObj.msg .. ")")
			retCode = 1
		end

		local updateTimeSql = [[update sc_sync set update_time=DATE_FORMAT(']] .. nowDate .. 
			                  [[','%Y-%m-%d %H:%i:%s'),next_time=DATE_ADD(DATE_FORMAT(']] .. nowDate .. 
							  [[','%Y-%m-%d %H:%i:%s'), INTERVAL (select val from sc_config where param=']] .. sync_row.task_name .. 
							  [[') MINUTE) where task_name=']] .. sync_row.task_name .. [[' ]]

		print("Update Time Sql (" .. updateTimeSql .. ")")
		--res = mysql_con:execute(updateTimeSql)
		if res == 0 then
			--print("Execute Sql Error")
			--retCode = 1
		end
		print("////////////////////////////////////////////////")		  
		sync_row = sync_tab:fetch ({}, "a")	
	end

	sync_tab:close()
	mysql_con:close()
	env:close()

	return retCode;
end


local ret = main()
if ret == 0 then
	print("Sync Upload Done!")
else
	print("Sync Upload Error!")
end

