luasql = require("luasql.mysql")
curl = require("luacurl")
md5  = require("md5")
json = require ("dkjson")

local DbServerIp  = "10.3.18.27"
local DbUser 	  = "ptsf"
local DbPassword  = "123456"
local DbName 	  = "scenic"
local DbPort 	  = 3306

local REQ_ADDR = "http://10.3.18.27/cardc/1.0/sync/down"


function get_html(url, post_value)
    local result = { }
   
    local c = curl.new()
    
    c:setopt(curl.OPT_URL, url)
	c:setopt(curl.OPT_POSTFIELDS, post_value)
    c:setopt(curl.OPT_WRITEDATA, result)
    c:setopt(curl.OPT_WRITEFUNCTION, function(tab, buffer)     --call back函数，必须有
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

local function crete_insert_sql(db, tabName, jsonObj, metaObj)
    assert(db)
    assert(tabName)
    assert(jsonObj)
    assert(metaObj)

	local colSet = {}
	local valSet = {}

	for col_name, col_tab in pairs(metaObj) do 	
		if col_tab.insert == "Y" then	
			local col_value = jsonObj[col_name]
			if col_tab.primary == "Y" then
				local pkLineSql = "SELECT COUNT(*) line_count FROM " .. tabName .. " WHERE " .. col_name .. " = " .. col_value
				print(pkLineSql)
				local res = assert(db:execute(pkLineSql))
				local count_row = res:fetch ({}, "a")	
				if tonumber(count_row.line_count) >= 1 then
					return nil
				end
			end

			table.insert(colSet, col_name)
			if col_tab.type == "V" then
				table.insert(valSet, "'" .. col_value .."'")
			elseif col_tab.type == "N" then
				table.insert(valSet,  col_value)
			elseif col_tab.type == "D" then
				table.insert(valSet,  "DATE_FORMAT('" .. col_value .. "','%Y-%m-%d')")
			elseif col_tab.type == "T" then
				table.insert(valSet,  "TIME_FORMAT('" .. col_value .. "','%H:%i:%s')")
			end
		end
	end

	if #colSet == 0 or #valSet == 0 then 
		return nil
	end

	local colListStr = ""
	local valListStr = ""
	colListStr = table.concat(colSet, ", ")
	valListStr = table.concat(valSet, ", ")

	local sql = "insert into " .. tabName .. "(" .. colListStr .. ")" .. " values(" .. valListStr .. ")"
	return sql
end

local function crete_update_sql(db, tabName, jsonObj, metaObj)
    assert(db)
    assert(tabName)
    assert(jsonObj)
    assert(metaObj)

	local colSet = {}
	local whereStr = ""

	for col_name, col_tab in pairs(metaObj) do 	
		if col_tab.update == "Y" then
			local col_value = jsonObj[col_name]
			if col_tab.primary == "Y" then
		        if col_tab.type == "N" then
		        	whereStr = whereStr ..  col_name .. " = " ..  col_value
		        else
		        	whereStr = whereStr ..  col_name .. " = '" ..  col_value .. "'"
		        end			
			else
				if col_tab.type == "V" then
					table.insert(colSet, col_name .. " = " .. "'" .. col_value .."'")
				elseif col_tab.type == "N" then
					table.insert(colSet,  col_name .. " = " .. col_value)
				elseif col_tab.type == "D" then
					table.insert(colSet,  col_name .. " = " .. "DATE_FORMAT('" .. col_value .. "','%Y-%m-%d')")
				elseif col_tab.type == "T" then
					table.insert(colSet,  col_name .. " = " .. "TIME_FORMAT('" .. col_value .. "','%H:%i:%s')")
				end
			end
		end
	end

	if #colSet == 0 or whereStr == "" then 
		return nil
	end

	local updateColSet = ""
	updateColSet = table.concat(colSet, ", ")

	local sql = "update " .. tabName .. " set " .. updateColSet .. " where " .. whereStr;
	return sql
end

function json_to_sql(db, tabName, jsonObj, meta)
    assert(db)
    assert(tabName)
    assert(jsonObj)

	local metaObj, tmp, err = json.decode (meta, 1, nil)
	if err then
		print ("Json Parse Meta Error:", err)
	end	

	if jsonObj._opt == "insert" then	
		return crete_insert_sql(db, tabName, jsonObj, metaObj)
	elseif jsonObj._opt == "update" then
		return crete_update_sql(db, tabName, jsonObj, metaObj)
	end

	return nil
end



function main()
	env = assert (luasql.mysql())
	-- connect to data source
	mysql_con = assert (env:connect(DbName, DbUser, DbPassword, DbServerIp, DbPort))

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
				
	local sql = [[SELECT meta,table_name,task_name,DATE_FORMAT(update_time,'%Y-%m-%d %H:%i:%s') begin_date FROM sc_sync WHERE DATE_FORMAT(']].. nowDate ..[[','%Y-%m-%d %H:%i:%s') > next_time and  act=0 and flag=1 ]]
	print("Searching SC Sync Tab(" .. sql .. ")")			
	sync_tab = assert(mysql_con:execute(sql))
			
	print("////////////////////////////////////////////////")		
	sync_row = sync_tab:fetch ({}, "a")		
	while sync_row do	
		print("<<<<Start Sync Task " .. sync_row.task_name .. ">>>> \n")
		local p = "_ent_id=" .. loc_ent_id .. 
		          "&bdate=" .. sync_row.begin_date ..
				  "&edate=" .. nowDate ..
				  "&_task=" .. sync_row.task_name
			  
		local md5Key = get_sign(p, loc_ent_key)
		p = p .. "&_sign=" .. md5Key
		print("Sending to CC (" .. p .. ")\n")

		ok, html = get_html(REQ_ADDR, p)
		if not ok or not html then
		    print ("Send Request To CC Error")   
			return 1
		end		  

		--print(html)
		local srcObj, tmp, err = json.decode (html, 1, nil)
		if err then
			print ("Json Parse CC_Src Error:", err)
			return 1
		end

		for i = 1,#srcObj.data do
			local update_sql = json_to_sql(mysql_con, sync_row.table_name, srcObj.data[i], sync_row.meta)
			if update_sql ~= nil then
				print(update_sql)
				res = mysql_con:execute(update_sql)
				if res == 0 then
					print("Execute Sql Error")
					return 1;
				end
			end
		end	  

		local updateTimeSql = [[update sc_sync set update_time=DATE_FORMAT(']] .. nowDate .. 
			                  [[','%Y-%m-%d %H:%i:%s'),next_time=DATE_ADD(DATE_FORMAT(']] .. nowDate .. 
							  [[','%Y-%m-%d %H:%i:%s'), INTERVAL (select val from sc_config where param=']] .. sync_row.task_name .. 
							  [[') MINUTE) where task_name=']] .. sync_row.task_name .. [[' ]]

		print("Update Time Sql (" .. updateTimeSql .. ")")
		res = assert(mysql_con:execute(updateTimeSql))

		print("////////////////////////////////////////////////")		  
		sync_row = sync_tab:fetch ({}, "a")	
	end

	sync_tab:close()
	mysql_con:close()
	env:close()

	return 0;
end


local ret = main()
if ret == 0 then
	print("Sync Done!")
else
	print("Sync Error!")
end

