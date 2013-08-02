luasql = require("luasql.mysql")


local DbServerIp  = "10.3.18.27"
local DbUser 	  = "ptsf"
local DbPassword  = "123456"
local DbName 	  = "scenic_0517"
local DbPort 	  = 3306

function main()
	env = assert (luasql.mysql())
	-- connect to data source
	mysql_con = assert (env:connect(DbName, DbUser, DbPassword, DbServerIp, DbPort))

	local timeTab   = os.date("*t")
	local nowDate   = string.format("%s-%s-%s", timeTab.year, timeTab.month, timeTab.day)
	local startTime = string.format("%s-%s-%s %s", timeTab.year, timeTab.month, timeTab.day, "00:00:00")
	local endTime   = string.format("%s-%s-%s %s", timeTab.year, timeTab.month, timeTab.day, "23:59:59")
	
	--startTime = "2000-07-25 10-22-00"
	--endTime   = "2100-07-25 10-22-00"

	day_tab = assert(mysql_con:execute([[select count(1) line_count from sc_day_stat_visitor_count where stat_date=']] .. nowDate .. [[' ]]))
	day_row = day_tab:fetch ({}, "a") 
	if tonumber(day_row.line_count) >= 1 then
		day_tab:close()
		print("Have Executed!")
		return 0;
	end
	day_tab:close()

	local result_tab = {}
	--for sell
	local countVisitorSql = [[SELECT IFNULL(SUM(CASE WHEN action=0 THEN 1 ELSE 0 END),0) visitor_in,
							 IFNULL(SUM(CASE WHEN action=1 THEN 1 ELSE 0 END),0) visitor_out
							 FROM sc_swiping
						     WHERE create_time >  DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') and 
						           create_time <= DATE_FORMAT(']] .. endTime .. [[' ,'%Y-%m-%d %H:%i:%s')]] 


	print("Count Swiping (" .. countVisitorSql .. ")")			
	visitor_count_tab = assert(mysql_con:execute(countVisitorSql))
	visitor_count_row = visitor_count_tab:fetch ({}, "a") 		

	local sellSql = [[SELECT COUNT(1) sell_total_ticket
						     FROM sc_sale 
						     WHERE sale_time>=DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') and 
						     sale_time<=DATE_FORMAT(']] .. endTime .. [[' ,'%Y-%m-%d %H:%i:%s')]] 

	print("Count Swiping (" .. sellSql .. ")")			
	sell_count_tab = assert(mysql_con:execute(sellSql))
	sell_count_row = sell_count_tab:fetch ({}, "a") 	


  	local insertSql= [[INSERT INTO sc_day_stat_visitor_count (stat_date,
															  sale_ticket_num,
															  visitor_in_num,
															  visitor_out_num
															) VALUE (']] .. nowDate .. [[',]] ..
															 sell_count_row.sell_total_ticket .. ", " ..
															 visitor_count_row.visitor_in .. ", " ..
															 visitor_count_row.visitor_out .. ")"


	print("Insert Tab For sc_day_stat_visitor_count (" .. insertSql .. ")")		
	local res = assert(mysql_con:execute(insertSql))
	if res ~= 1 then 
		print("Insert sc_day_stat_visitor_count tab error")
		sell_count_tab:close()
		visitor_count_tab:close()
		mysql_con:close()
		env:close()
		return 1
	end
	
	sell_count_tab:close()
	visitor_count_tab:close()
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
