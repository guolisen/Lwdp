luasql = require("luasql.mysql")


local DbServerIp  = "10.3.18.27"
local DbUser 	  = "ptsf"
local DbPassword  = "123456"
local DbName 	  = "scenic"
local DbPort 	  = 3306

function main()
	env = assert (luasql.mysql())
	-- connect to data source
	mysql_con = assert (env:connect(DbName, DbUser, DbPassword, DbServerIp, DbPort))

	local timeTab   = os.date("*t")
	local nowDate   = string.format("%s-%s-%s", timeTab.year, timeTab.month, timeTab.day)
	local startTime = string.format("%s-%s-%s %s", timeTab.year, timeTab.month, timeTab.day, "00:00:00")
	local endTime   = string.format("%s-%s-%s %s", timeTab.year, timeTab.month, timeTab.day, "12:23:59")
	
	day_tab = assert(mysql_con:execute([[select count(1) line_count from cc_day_stat where stat_date=']] .. nowDate .. [[' ]]))
	day_row = day_tab:fetch ({}, "a") 
	if tonumber(day_row.line_count) >= 1 then
		day_tab:close()
		print("Have Executed!")
		return 0;
	end
	day_tab:close()

	--for sell
	local sellSql = [[SELECT IFNULL(SUM(actual_money+deposit),0) sell_total_money,
						 IFNULL(SUM(CASE WHEN ticket_type=1 THEN actual_money ELSE 0 END),0) sell_time_money,
						 IFNULL(SUM(CASE WHEN ticket_type=2 THEN actual_money ELSE 0 END),0) sell_package_money,
						 IFNULL(SUM(CASE WHEN ticket_type=3 THEN actual_money ELSE 0 END),0) sell_year_money,
						 IFNULL(SUM(deposit),0) sell_deposit,
						 COUNT(1) sell_total_ticket,
						 IFNULL(SUM(CASE WHEN ticket_type=1 THEN 1 ELSE 0 END),0) sell_time_ticket,
						 IFNULL(SUM(CASE WHEN ticket_type=2 THEN 1 ELSE 0 END),0) sell_package_ticket,
						 IFNULL(SUM(CASE WHEN ticket_type=3 THEN 1 ELSE 0 END),0) sell_year_ticket 
						 FROM cc_order a 
						 WHERE report_time >= DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') and 
						       report_time <= DATE_FORMAT(']] .. endTime   .. [[' ,'%Y-%m-%d %H:%i:%s')]]


	print("Searching Tab For Sell (" .. sellSql .. ")")			
	sell_count_tab = assert(mysql_con:execute(sellSql))
	sell_count_row = sell_count_tab:fetch ({}, "a") 				
	print("////////////////////////////////////////////////")	

  	local insertSql= [[insert into cc_day_stat(stat_date,sell_total_money,sell_time_money,sell_package_money,sell_year_money,deposit,
			  								   sell_total_ticket,sell_time_ticket,sell_package_ticket,sell_year_ticket) 
			  								   values(']] .. nowDate .. [[',]] ..
			  								   sell_count_row.sell_total_money .. ", " ..
			  								   sell_count_row.sell_time_money .. ", " ..
			  								   sell_count_row.sell_package_money .. ", " ..
			  								   sell_count_row.sell_year_money .. ", " ..
			  								   sell_count_row.sell_deposit .. ", " ..

			  								   sell_count_row.sell_total_ticket .. ", " ..
			  								   sell_count_row.sell_time_ticket .. ", " ..
			  								   sell_count_row.sell_package_ticket .. ", " ..
			  								   sell_count_row.sell_year_ticket .. ")"


	print("Insert Tab For cc_day_stat (" .. insertSql .. ")")		
	local res = assert(mysql_con:execute(insertSql))
	if res ~= 1 then 
		print("Insert sc_day_stat tab error")
		sell_count_tab:close()
		mysql_con:close()
		env:close()
		return 1
	end
	
	sell_count_tab:close()
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
