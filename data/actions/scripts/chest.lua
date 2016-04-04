function onUse(cid, item, frompos, item2, topos)
 
   	-- bright sword quest
   	if item.uid == 5001 then
   		queststatus = getPlayerStorageValue(cid,5001)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 20 then
   				doPlayerSendTextMessage(cid,22,"You have found Bright Sword.")
   				doPlayerAddItem(cid,2407,1)
   				setPlayerStorageValue(cid,5001,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 20 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
 
   	-- orc fortress quest
   	elseif item.uid == 5002 then
   		queststatus = getPlayerStorageValue(cid,5002)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 50 then
   				doPlayerSendTextMessage(cid,22,"You have found Golden Armor.")
   				doPlayerAddItem(cid,2466,1)
   				setPlayerStorageValue(cid,5002,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 50 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
 
   	-- pharaon quest
   	elseif item.uid == 5003 then
   		queststatus = getPlayerStorageValue(cid,5003)
   		if queststatus == -1 then
			if getPlayerLevel(cid) >= 50 then
   				doPlayerSendTextMessage(cid,22,"You have found Dragon Lance.")
   				doPlayerAddItem(cid,2414,1)
   				setPlayerStorageValue(cid,5003,1)
   			else
				doPlayerSendTextMessage(cid,22,"You need level 50 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5004 then
   		queststatus = getPlayerStorageValue(cid,5003)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 50 then
   				doPlayerSendTextMessage(cid,22,"You have found War Hammer.")
   				doPlayerAddItem(cid,2391,1)
   				setPlayerStorageValue(cid,5003,1)
   			else
				doPlayerSendTextMessage(cid,22,"You need level 50 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
	elseif item.uid == 5017 then
   		queststatus = getPlayerStorageValue(cid,5003)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 50 then
   				doPlayerSendTextMessage(cid,22,"You have found Giant Sword.")
   				doPlayerAddItem(cid,2393,1)
   				setPlayerStorageValue(cid,5003,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 50 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
 
   	-- demon quest
   	elseif item.uid == 5005 then
   		queststatus = getPlayerStorageValue(cid,5005)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 60 then
   				doPlayerSendTextMessage(cid,22,"You have found Steel Boots.")
   				doPlayerAddItem(cid,2645,1)
   				setPlayerStorageValue(cid,5005,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 60 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
 
   	-- annihilator
   	elseif item.uid == 5006 then
   		queststatus = getPlayerStorageValue(cid,5010)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Demon Armor.")
   			doPlayerAddItem(cid,2494,1)
   			setPlayerStorageValue(cid,5010,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5007 then
   		queststatus = getPlayerStorageValue(cid,5010)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Sword of Valor.")
   			doPlayerAddItem(cid,2400,1)
   			setPlayerStorageValue(cid,5010,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5008 then
   		queststatus = getPlayerStorageValue(cid,5010)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Stonecutter's Axe.")
   			doPlayerAddItem(cid,2431,1)
   			setPlayerStorageValue(cid,5010,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5009 then
   		queststatus = getPlayerStorageValue(cid,5010)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have a Thunder Hammer.")
   			doPlayerAddItem(cid,2421,1)
   			setPlayerStorageValue(cid,5010,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end

	-- elf camp quest
   	elseif item.uid == 5013 then
   		queststatus = getPlayerStorageValue(cid,5013)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 20 then
   				doPlayerSendTextMessage(cid,22,"You have found Dragon Hammer.")
   				doPlayerAddItem(cid,2434,1)
   				setPlayerStorageValue(cid,5013,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 20 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end

	-- banshee quest
   	elseif item.uid == 5014 then
   		queststatus = getPlayerStorageValue(cid,5014)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 20 then
   				doPlayerSendTextMessage(cid,22,"You have found Amazon Shield.")
   				doPlayerAddItem(cid,2537,1)
   				setPlayerStorageValue(cid,5014,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 20 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
 
   	-- slime quest
   	elseif item.uid == 5015 then
   		queststatus = getPlayerStorageValue(cid,5015)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 12 then
   				doPlayerSendTextMessage(cid,22,"You have found Brass Legs and Dark Shield.")
   				doPlayerAddItem(cid,2478,1)
   				doPlayerAddItem(cid,2521,1)
   				setPlayerStorageValue(cid,5015,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 12 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end

	-- necro quest
   	elseif item.uid == 5016 then
   		queststatus = getPlayerStorageValue(cid,5016)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 20 then
   				doPlayerSendTextMessage(cid,22,"You have found Knight Legs.")
   				doPlayerAddItem(cid,2477,1)
   				setPlayerStorageValue(cid,5016,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 20 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
 
   	-- ship quest
   	elseif item.uid == 5018 then
   		queststatus = getPlayerStorageValue(cid,5018)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 40 then
   				doPlayerSendTextMessage(cid,22,"You have found Helmet of the Ancients.")
   				doPlayerAddItem(cid,2342,1)
   				setPlayerStorageValue(cid,5018,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 40 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
 
   	-- steering patent
   	-- storage value 5019
 
   	-- omegilator
   	elseif item.uid == 5020 then
   		queststatus = getPlayerStorageValue(cid,5020)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 100 then
   				doPlayerSendTextMessage(cid,22,"You have found Helmet of the Ancients.")
   				doPlayerAddItem(cid,2343,1)
   				setPlayerStorageValue(cid,5020,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 100 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
 
 	-- hydra quest
 	elseif item.uid == 5022 then
   		queststatus = getPlayerStorageValue(cid,5022)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 40 then
   				doPlayerSendTextMessage(cid,22,"You have found Shield of Honour.")
   				doPlayerAddItem(cid,2517,1)
   				setPlayerStorageValue(cid,5022,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 40 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end

	-- yeti quest
 	elseif item.uid == 5023 then
   		queststatus = getPlayerStorageValue(cid,5023)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 20 then
   				doPlayerSendTextMessage(cid,22,"You have found Lady Helmet.")
   				doPlayerAddItem(cid,2499,1)
   				setPlayerStorageValue(cid,5023,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 20 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
 -- croc boots quest
 	elseif item.uid == 5024 then
   		queststatus = getPlayerStorageValue(cid,5024)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 25 then
   				doPlayerSendTextMessage(cid,22,"You have found Crocodile Boots.")
   				doPlayerAddItem(cid,3982,1)
   				setPlayerStorageValue(cid,5024,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 25 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end

   -- outlaw camp quest barrel one
 	elseif item.uid == 5025 then
   		queststatus = getPlayerStorageValue(cid,5025)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 25 then
   				doPlayerSendTextMessage(cid,22,"You have found stolen armor.")
   				doPlayerAddItem(cid,2486,1)
   				setPlayerStorageValue(cid,5025,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 25 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end

 -- outlaw camp quest barrel two
 	elseif item.uid == 5026 then
   		queststatus = getPlayerStorageValue(cid,5026)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 25 then
   				doPlayerSendTextMessage(cid,22,"You have found stolen jewells.")
   				doPlayerAddItem(cid,2144,2)
                                                                                doPlayerAddItem(cid,2133,1)
   				setPlayerStorageValue(cid,5026,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 25 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
     

 -- 10K Quest
 	elseif item.uid == 5027 then
   		queststatus = getPlayerStorageValue(cid,5027)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 20 then
   				doPlayerSendTextMessage(cid,22,"You have found a Magical Amulet and a 100 Platinum Coins!")
   				doPlayerAddItem(cid,2171,1)
   				doPlayerAddItem(cid,2152,100)
                                                                                
   				setPlayerStorageValue(cid,5027,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need level 20 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end

   	-- rook quest
   	elseif item.uid == 5028 then
   		queststatus = getPlayerStorageValue(cid,5028)
   		if queststatus == -1 then
   			if getPlayerLevel(cid) >= 5 then
   				doPlayerSendTextMessage(cid,22,"You have found treasure.")
   				doPlayerAddItem(cid,3969,1)
   				doPlayerAddItem(cid,2397,1)
   				doPlayerAddItem(cid,2143,2)
   				setPlayerStorageValue(cid,5028,1)
   			else
   				doPlayerSendTextMessage(cid,22,"You need to be level 5 to do this quest.")
   			end
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
 
end
return 1
end


     