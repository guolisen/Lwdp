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

	day_tab = assert(mysql_con:execute([[select count(1) line_count from sc_day_stat where stat_date=']] .. nowDate .. [[' ]]))
	day_row = day_tab:fetch ({}, "a") 
	if tonumber(day_row.line_count) >= 1 then
		day_tab:close()
		print("Have Executed!")
		return 0;
	end
	day_tab:close()

	local result_tab = {}
	--for sell
	local sellSql = [[SELECT IFNULL(SUM(actual_money+deposit),0) sell_total_money,
						 IFNULL(SUM(CASE WHEN ticket_type=1 THEN actual_money ELSE 0 END),0) sell_time_money,
						 IFNULL(SUM(CASE WHEN ticket_type=2 THEN actual_money ELSE 0 END),0) sell_package_money,
						 IFNULL(SUM(CASE WHEN ticket_type=3 THEN actual_money ELSE 0 END),0) sell_year_money,
						 IFNULL(SUM(CASE WHEN ticket_type=2 THEN deposit ELSE 0 END),0) sell_year_deposit,
						 IFNULL(SUM(CASE WHEN ticket_type=3 THEN deposit ELSE 0 END),0) sell_package_deposit,
						 COUNT(1) sell_total_ticket,
						 IFNULL(SUM(CASE WHEN ticket_type=1 THEN 1 ELSE 0 END),0) sell_time_ticket,
						 IFNULL(SUM(CASE WHEN ticket_type=2 THEN 1 ELSE 0 END),0) sell_package_ticket,
						 IFNULL(SUM(CASE WHEN ticket_type=3 THEN 1 ELSE 0 END),0) sell_year_ticket
						 FROM sc_sale a
						 WHERE sale_time>=DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') and 
						 sale_time<=DATE_FORMAT(']] .. endTime .. [[' ,'%Y-%m-%d %H:%i:%s') and status=1]] 


	print("Searching Tab For Sell (" .. sellSql .. ")")			
	sell_count_tab = assert(mysql_con:execute(sellSql))
	sell_count_row = sell_count_tab:fetch ({}, "a") 		
	print("////////////////////////////////////////////////")	

	local fetchSql = [[SELECT COUNT(1) fetch_total_ticket, 
  							 IFNULL(SUM(CASE WHEN ticket_type=1 THEN 1 ELSE 0 END),0) fetch_time_ticket,
  							 IFNULL(SUM(CASE WHEN ticket_type=2 THEN 1 ELSE 0 END),0) fetch_package_ticket,
  							 IFNULL(SUM(CASE WHEN ticket_type=3 THEN 1 ELSE 0 END),0) fetch_year_ticket 
  							 FROM sc_fetch a
  							 WHERE a.create_time >= DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') AND 
  							       a.create_time <= DATE_FORMAT(']] .. endTime   .. [[' ,'%Y-%m-%d %H:%i:%s')]]
 
	print("Searching Tab For fetch (" .. fetchSql .. ")")			
	fetch_count_tab = assert(mysql_con:execute(fetchSql))
	fetch_count_row = fetch_count_tab:fetch ({}, "a") 		
	print("////////////////////////////////////////////////")	

  	local cardSql = [[SELECT IFNULL(SUM(CASE WHEN log_type=8 THEN 1 ELSE 0 END),0) loss_card,
  							 IFNULL(SUM(CASE WHEN log_type=9 THEN 1 ELSE 0 END),0) destory_card,
  							 IFNULL(SUM(CASE WHEN log_type=10 THEN 1 ELSE 0 END),0) patch_card 
  							 FROM sc_log 
  							 WHERE create_time>=DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') AND 
  								   create_time<=DATE_FORMAT(']] .. endTime   .. [[' ,'%Y-%m-%d %H:%i:%s') ]]

	print("Searching Tab For card (" .. cardSql .. ")")			
	card_count_tab = assert(mysql_con:execute(cardSql))
	card_count_row = card_count_tab:fetch ({}, "a") 		
	print("////////////////////////////////////////////////")	

  	local insertSql= [[insert into sc_day_stat(stat_date, sell_total_money, sell_time_money, sell_package_money, sell_year_money, annual_ticket_deposit,
			  								   package_ticket_deposit, sell_total_ticket, sell_time_ticket, sell_package_ticket, sell_year_ticket,
			  								   fetch_total_ticket, fetch_time_ticket, fetch_package_ticket, fetch_year_ticket,
			  								   loss_card, destory_card, loss_patch_card) 
			  								   values(']] .. nowDate .. [[',]] ..
			  								   sell_count_row.sell_total_money .. ", " ..
			  								   sell_count_row.sell_time_money .. ", " ..
			  								   sell_count_row.sell_package_money .. ", " ..
			  								   sell_count_row.sell_year_money .. ", " ..
			  								   sell_count_row.sell_year_deposit .. ", " ..
			  								   sell_count_row.sell_package_deposit .. ", " ..
			  								   sell_count_row.sell_total_ticket .. ", " ..
			  								   sell_count_row.sell_time_ticket .. ", " ..
			  								   sell_count_row.sell_package_ticket .. ", " ..
			  								   sell_count_row.sell_year_ticket .. ", " ..
			  								   fetch_count_row.fetch_total_ticket .. ", " ..
			  								   fetch_count_row.fetch_time_ticket .. ", " ..
			  								   fetch_count_row.fetch_package_ticket .. ", " ..
			  								   fetch_count_row.fetch_year_ticket .. ", " ..
			  								   card_count_row.loss_card .. ", " ..
			  								   card_count_row.destory_card .. ", " ..
			  								   card_count_row.patch_card .. ")"

	print("Insert Tab For sc_day_stat (" .. insertSql .. ")")		
	local res = assert(mysql_con:execute(insertSql))
	if res ~= 1 then 
		print("Insert sc_day_stat tab error")
		sell_count_tab:close()
		fetch_count_tab:close()
		card_count_tab:close()
		mysql_con:close()
		env:close()
		return 1
	end
	
	sell_count_tab:close()
	fetch_count_tab:close()
	card_count_tab:close()


------------------------------------------------------------------------------------------------
	print("//////////////////////// Get sc_day_stat Id ////////////////////////")	
	day_stat_tab = assert(mysql_con:execute([[select id from sc_day_stat where stat_date=']] .. nowDate .. [[' ]]))
	day_stat_row = day_stat_tab:fetch ({}, "a") 
	if tonumber(day_stat_row.id) == nil then
		day_stat_tab:close()
		print("Get sc_day_stat Id Error!")
		return 1;
	end
	local day_stat_id = day_stat_row.id
	day_stat_tab:close()

	print("<<<<<< Year Policy Sun... >>>>>>")
	local year_policy_result_sql = [[SELECT  ticketing_id, 
											IFNULL(SUM(CASE WHEN ticket_type=3 THEN actual_money ELSE 0 END),0) year_ticket_money,
											IFNULL(SUM(CASE WHEN ticket_type=3 THEN 1 ELSE 0 END),0) year_policy_ticket
											FROM sc_sale
							 				WHERE sale_time >= DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') and 
												  sale_time <= DATE_FORMAT(']] .. endTime   .. [[' ,'%Y-%m-%d %H:%i:%s') 
											GROUP BY ticketing_id]]

	print("Get Year Ticket Policy Sum (" .. year_policy_result_sql .. ")")										
	year_policy_result_tab = assert(mysql_con:execute(year_policy_result_sql))
	year_policy_result_row = year_policy_result_tab:fetch ({}, "a") 										
	while year_policy_result_row do
		print("------------------------------------------------------------")
		local day_stat_insert_sql = [[insert into sc_day_stat_ticket_count(day_stat_id, stat_date, ticket_type, ticketing_id, num, money) 
										  								   values(]] .. day_stat_id .. ", " ..
										  								   	[[']] .. nowDate .. [[',]] ..
										  								   	3 .. ", " ..
										  								   	year_policy_result_row.ticketing_id .. ", " ..
										  								   	year_policy_result_row.year_policy_ticket .. ", " ..
										  								   	year_policy_result_row.year_ticket_money .. ")"

		print("Year Ticket Policy Sum Insert To (" .. day_stat_insert_sql .. ")")								  		
		local res = assert(mysql_con:execute(day_stat_insert_sql))
		if res ~= 1 then 
			print("Insert sc_day_stat_ticket_count tab error")
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
	local package_policy_result_sql = [[SELECT  ticketing_id, 
												IFNULL(SUM(CASE WHEN ticket_type=2 THEN actual_money ELSE 0 END),0) package_ticket_money,
												IFNULL(SUM(CASE WHEN ticket_type=2 THEN 1 ELSE 0 END),0) package_policy_ticket
												FROM sc_sale
								 				WHERE sale_time >= DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') and 
													  sale_time <= DATE_FORMAT(']] .. endTime   .. [[' ,'%Y-%m-%d %H:%i:%s') 
												GROUP BY ticketing_id]]

	print("Get package Ticket Policy Sum (" .. package_policy_result_sql .. ")")										
	package_policy_result_tab = assert(mysql_con:execute(package_policy_result_sql))
	package_policy_result_row = package_policy_result_tab:fetch ({}, "a") 										
	while package_policy_result_row do
		print("------------------------------------------------------------")
		local day_stat_insert_sql = [[insert into sc_day_stat_ticket_count(day_stat_id, stat_date, ticket_type, ticketing_id, num, money) 
										  								   values(]] .. day_stat_id .. ", " ..
										  								   	[[']] .. nowDate .. [[',]] ..
										  								   	2 .. ", " ..
										  								   	package_policy_result_row.ticketing_id .. ", " ..
										  								   	package_policy_result_row.package_policy_ticket .. ", " ..
										  								   	package_policy_result_row.package_ticket_money .. ")"

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

	print("<<<<<< Times Policy Sun... >>>>>>")
	local Times_policy_result_sql = [[SELECT  ticketing_id, 
												IFNULL(SUM(CASE WHEN ticket_type=1 THEN actual_money ELSE 0 END),0) Times_ticket_money,
												IFNULL(SUM(CASE WHEN ticket_type=1 THEN 1 ELSE 0 END),0) Times_policy_ticket
												FROM sc_sale
								 				WHERE sale_time >= DATE_FORMAT(']] .. startTime .. [[' ,'%Y-%m-%d %H:%i:%s') and 
													  sale_time <= DATE_FORMAT(']] .. endTime   .. [[' ,'%Y-%m-%d %H:%i:%s') 
												GROUP BY ticketing_id]]

	print("Get Times Ticket Policy Sum (" .. Times_policy_result_sql .. ")")										
	Times_policy_result_tab = assert(mysql_con:execute(Times_policy_result_sql))
	Times_policy_result_row = Times_policy_result_tab:fetch ({}, "a") 										
	while Times_policy_result_row do
		print("------------------------------------------------------------")
		local day_stat_insert_sql = [[insert into sc_day_stat_ticket_count(day_stat_id, stat_date, ticket_type, ticketing_id, num, money) 
										  								   values(]] .. day_stat_id .. ", " ..
										  								   	[[']] .. nowDate .. [[',]] ..
										  								   	1 .. ", " ..
										  								   	Times_policy_result_row.ticketing_id .. ", " ..
										  								   	Times_policy_result_row.Times_policy_ticket .. ", " ..
										  								   	Times_policy_result_row.Times_ticket_money .. ")"

		print("Times Ticket Policy Sum Insert To (" .. day_stat_insert_sql .. ")")								  		
		local res = assert(mysql_con:execute(day_stat_insert_sql))
		if res ~= 1 then 
			print("Insert cc_day_stat_ticket_count tab error")
			Times_policy_result_tab:close()
			ent_tab:close()
			mysql_con:close()
			env:close()
			return 1
		end

		Times_policy_result_row = Times_policy_result_tab:fetch ({}, "a") 
	end
	Times_policy_result_tab:close()

	--print("//////////////////////// Sum Visitor ////////////////////////")	



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
