luasql = require("luasql.mysql")
ltn12  = require("ltn12")
ftp    = require("socket.ftp")

local DbServerIp  = "10.3.18.27"
local DbUser 	  = "ptsf"
local DbPassword  = "123456"
local DbName 	  = "scenic"
local DbPort 	  = 3306

local rootPath    = "C:\\"
local ftpServerIp = "127.0.0.1"
local ftpUser     = "root"
local ftpPassword = "123456"

function main()
	env = assert (luasql.mysql())
	-- connect to data source
	mysql_con = assert (env:connect(DbName, DbUser, DbPassword, DbServerIp, DbPort))

	ent_tab = assert(mysql_con:execute([[select enterprise_id from sc_enterprise]]))
	ent_row = ent_tab:fetch ({}, "a") 
	local loc_ent_id = ent_row.enterprise_id
	ent_tab:close()

	local timeTab   = os.date("*t")
	local nowDate   = string.format("%s-%s-%s", timeTab.year, timeTab.month, timeTab.day)
	local startTime = string.format("%s-%s-%s %s", timeTab.year, timeTab.month, timeTab.day, "00:00:00")
	local endTime   = string.format("%s-%s-%s %s", timeTab.year, timeTab.month, timeTab.day, "12:23:59")
	local fileName  = loc_ent_id .. "_" .. nowDate .. ".txt"
	local filePath  = rootPath .. fileName
	startTime = "2013-6-8 00:00:00"
	endTime   = "2013-6-8 23:59:00"
	local selectSql = [[SELECT a.id, a.swip_type, a.swip_no, a.gate_no, a.scenic_id,
							   (SELECT enterprise_id FROM sc_enterprise a LIMIT 0,1) enterprise_id,
							   a.ticket_type, a.action, DATE_FORMAT(create_time,'%Y-%m-%d %H:%i:%s') ctime 
							   FROM sc_swiping a 
							   WHERE create_time >= DATE_FORMAT(']] .. startTime .. [[','%Y-%m-%d %H:%i:%s') AND 
									 create_time <= DATE_FORMAT(']] .. endTime   .. [[','%Y-%m-%d %H:%i:%s') 
							   ORDER BY a.id ASC ]]
							   --into outfile ']] .. [[C:\\ddd.txt]] .. [[']] 

	print("Searching Tab (" .. selectSql .. ")")			
	swiping_tab = assert(mysql_con:execute(selectSql))
	swiping_row = swiping_tab:fetch ({}, "a") 				
	print("////////////////////////////////////////////////")	
	print("<<<<Start Write File " .. filePath .. ">>>>")
	local outFileHandle = assert(io.open(filePath, "w+"))
	while swiping_row do	
		local lineStr = swiping_row.id .. ", " ..
		                swiping_row.swip_type .. ", " ..
		                swiping_row.swip_no .. ", " ..
		                swiping_row.gate_no .. ", " ..
		                swiping_row.scenic_id .. ", " ..
		                swiping_row.enterprise_id .. ", " ..
		                swiping_row.ticket_type .. ", " ..
		                swiping_row.action .. ", " ..
		                swiping_row.ctime  .. "\n"

		print(lineStr)
		outFileHandle:write(lineStr)
		print("////////////////////////////////////////////////")		  
		swiping_row = swiping_tab:fetch ({}, "a")	
	end
	assert(outFileHandle:close())

	print("Put File(" .. filePath .. ") To Ftp(" .. ftpServerIp .. ")")
	local res, err = ftp.put
	{
		host		= ftpServerIp, 
		user 		= ftpUser,
		password 	= ftpPassword,
		command 	= "STOR",
		argument 	= fileName,
		source		= ltn12.source.file(io.open(filePath, "r"))
	}

	if res == nil then 
		print(err)
	end

	swiping_tab:close()
	mysql_con:close()
	env:close()

	return 0;
end


local ret = main()
if ret == 0 then
	print("Done!")
else
	print("Error!")
end
