
--[[
	Author: Guolisen
	Notice: 必须在0点前执行脚本，才能统计当天信息，因为0点后获取时间是第二天数据

]]
luasql = require("luasql.mysql")


local DbServerIp  = "10.3.18.27"
local DbUser 	  = "ptsf"
local DbPassword  = "123456"
local DbName 	  = "cardc_0517"
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

	day_tab = assert(mysql_con:execute([[select count(1) line_count from cc_day_stat where stat_date=']] .. nowDate .. [[' ]]))
	day_row = day_tab:fetch ({}, "a") 
	if tonumber(day_row.line_count) >= 1 then
		day_tab:close()
		print("Have Executed!")
		return 0;
	end
	day_tab:close()

	local result_tab = {}
	--for sell
	print("/////////////////////// Counting year /////////////////////////")	
	local yearSellSql = [[SELECT IFNULL(SUM(actual_money), 0) sell_year_money,
       						 IFNULL(SUM(deposit), 0) sell_year_deposit,
							 COUNT(1) sell_year_ticket
							 FROM cc_annual_ticket_order
							 WHERE report_time >= DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') and 
							       report_time <= DATE_FORMAT(']] .. endTime   .. [[' ,'%Y-%m-%d %H:%i:%s')]]


	print("Searching Tab For Sell (" .. yearSellSql .. ")")			
	year_result_tab = assert(mysql_con:execute(yearSellSql))
	year_result_row = year_result_tab:fetch ({}, "a") 		

	result_tab.sell_year_money   = year_result_row.sell_year_money	
	result_tab.sell_year_deposit = year_result_row.sell_year_deposit
	result_tab.sell_year_ticket  = year_result_row.sell_year_ticket

	print("/////////////////////// Counting Package /////////////////////////")	
	local packageSellSql = [[SELECT IFNULL(SUM(deposit), 0) sell_package_deposit,
								 COUNT(1) sell_package_ticket
								 FROM cc_package_ticket_order
								 WHERE report_time >= DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') and 
								       report_time <= DATE_FORMAT(']] .. endTime   .. [[' ,'%Y-%m-%d %H:%i:%s')]]

	print("Searching Tab For Sell (" .. packageSellSql .. ")")			
	package_result_tab = assert(mysql_con:execute(packageSellSql))
	package_result_row = package_result_tab:fetch ({}, "a") 	

	result_tab.sell_package_deposit = package_result_row.sell_package_deposit
	result_tab.sell_package_ticket  = package_result_row.sell_package_ticket	

	result_tab.sell_total_money = tonumber(result_tab.sell_year_money) + 
	                              tonumber(result_tab.sell_year_deposit) +
	                              tonumber(result_tab.sell_package_deposit) 	

	result_tab.sell_total_ticket = tonumber(result_tab.sell_package_ticket) +
								   tonumber(result_tab.sell_year_ticket) 


	print("//////////////////////// Insert cc_day_stat ////////////////////////")	
  	local insertSql = [[insert into cc_day_stat(stat_date, sell_total_money, sell_time_money, sell_package_money, sell_year_money, annual_ticket_deposit,
			  								   package_ticket_deposit, sell_total_ticket, sell_time_ticket, sell_package_ticket, sell_year_ticket) 
			  								   values(']] .. nowDate .. [[',]] ..
			  								   result_tab.sell_total_money .. ", " ..
			  								   0 .. ", " ..
			  								   0 .. ", " ..
			  								   result_tab.sell_year_money .. ", " ..
			  								   result_tab.sell_year_deposit .. ", " ..
			  								   result_tab.sell_package_deposit .. ", " ..
			  								   result_tab.sell_total_ticket .. ", " ..
			  								   0 .. ", " ..
			  								   result_tab.sell_package_ticket .. ", " ..
			  								   result_tab.sell_year_ticket .. ")"


	print("Insert Tab For cc_day_stat (" .. insertSql .. ")")		
	local res = assert(mysql_con:execute(insertSql))
	if res ~= 1 then 
		print("Insert sc_day_stat tab error")
		sell_count_tab:close()
		mysql_con:close()
		env:close()
		return 1
	end
	
	year_result_tab:close()
	package_result_tab:close()

	print("//////////////////////// Get cc_day_stat Id ////////////////////////")	
	day_stat_tab = assert(mysql_con:execute([[select id from cc_day_stat where stat_date=']] .. nowDate .. [[' ]]))
	day_stat_row = day_stat_tab:fetch ({}, "a") 
	if tonumber(day_stat_row.id) == nil then
		day_stat_tab:close()
		print("Get cc_day_stat Id Error!")
		return 1;
	end
	local day_stat_id = day_stat_row.id
	day_stat_tab:close()

	print("//////////////////////// Get Ent Id ////////////////////////")	
	ent_tab = assert(mysql_con:execute([[select enterprise_id from cc_enterprise]]))
	ent_row = ent_tab:fetch ({}, "a") 
	while ent_row do
		print("//////////////////////////// ent_id " .. ent_row.enterprise_id .. " ////////////////////////////////")
		print("<<<<<< Year Policy Sun... >>>>>>")
		local year_policy_result_sql = [[SELECT ticketing_id, report_time,
												IFNULL(SUM(actual_money), 0) year_policy_sum_money,
												COUNT(1) year_policy_ticket
												FROM cc_annual_ticket_order 
								 				WHERE report_time >= DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') and 
													  report_time <= DATE_FORMAT(']] .. endTime   .. [[' ,'%Y-%m-%d %H:%i:%s') and
													  ent_id = ]] .. ent_row.enterprise_id .. [[
												GROUP BY ticketing_id]]

		print("Get Year Ticket Policy Sum (" .. year_policy_result_sql .. ")")										
		year_policy_result_tab = assert(mysql_con:execute(year_policy_result_sql))
		year_policy_result_row = year_policy_result_tab:fetch ({}, "a") 										
		while year_policy_result_row do
			print("------------------------------------------------------------")
			local day_stat_insert_sql = [[insert into cc_day_stat_ticket_count(day_stat_id, ent_id, stat_date, ticket_type, ticketing_id, num, money) 
											  								   values(]] .. day_stat_id .. ", " ..
											  								   	ent_row.enterprise_id .. ", " ..
											  								   	[[']] .. nowDate .. [[',]] ..
											  								   	2 .. ", " ..
											  								   	year_policy_result_row.ticketing_id .. ", " ..
											  								   	year_policy_result_row.year_policy_ticket .. ", " ..
											  								   	year_policy_result_row.year_policy_sum_money .. ")"

			print("Year Ticket Policy Sum Insert To (" .. day_stat_insert_sql .. ")")								  		
			local res = assert(mysql_con:execute(day_stat_insert_sql))
			if res ~= 1 then 
				print("Insert cc_day_stat_ticket_count tab error")
				year_policy_result_tab:close()
				ent_tab:close()
				mysql_con:close()
				env:close()
				return 1
			end

			year_policy_result_row = year_policy_result_tab:fetch ({}, "a") 
		end
		year_policy_result_tab:close()

		print("<<<<<< Pakage Policy Sun... >>>>>>")
		local package_policy_result_sql = [[SELECT ticketing_id,
												COUNT(1) package_policy_ticket
												FROM cc_package_ticket_order_policy_map 
								 				WHERE report_time >= DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') and 
													  report_time <= DATE_FORMAT(']] .. endTime   .. [[' ,'%Y-%m-%d %H:%i:%s') and
													  granter_id = ]] .. ent_row.enterprise_id .. [[
												GROUP BY ticketing_id]]

		print("Get package Ticket Policy Sum (" .. package_policy_result_sql .. ")")										
		package_policy_result_tab = assert(mysql_con:execute(package_policy_result_sql))
		package_policy_result_row = package_policy_result_tab:fetch ({}, "a") 										
		while package_policy_result_row do
			print("------------------------------------------------------------")
			local day_stat_insert_sql = [[insert into cc_day_stat_ticket_count(day_stat_id, ent_id, stat_date, ticket_type, ticketing_id, num, money) 
											  								   values(]] .. day_stat_id .. ", " ..
											  								   	ent_row.enterprise_id .. ", " ..
											  								   	[[']] .. nowDate .. [[',]] ..
											  								   	3 .. ", " ..
											  								   	package_policy_result_row.ticketing_id .. ", " ..
											  								   	package_policy_result_row.package_policy_ticket .. ", " ..
											  								   	0 .. ")"

			print("Package Ticket Policy Sum Insert To (" .. day_stat_insert_sql .. ")")								  		
			local res = assert(mysql_con:execute(day_stat_insert_sql))
			if res ~= 1 then 
				print("Insert cc_day_stat_ticket_count tab error")
				package_policy_result_tab:close()
				ent_tab:close()
				mysql_con:close()
				env:close()
				return 1
			end

			package_policy_result_row = package_policy_result_tab:fetch ({}, "a") 
		end
		package_policy_result_tab:close()


		ent_row = ent_tab:fetch ({}, "a")	
	end
	ent_tab:close()


	if tonumber(day_stat_row.id) == nil then
		day_stat_tab:close()
		print("Get cc_day_stat Id Error!")
		return 1;
	end
	local day_stat_id = day_stat_row.id
	day_stat_tab:close()


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
