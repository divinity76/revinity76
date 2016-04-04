function onUse(cid, item, frompos, item2, topos)

   	if item.uid == 6400 then
   		queststatus = getPlayerStorageValue(cid,6400)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Demon Armor.")
   			doPlayerAddItem(cid,2494,1)
   			setPlayerStorageValue(cid,6400,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 6401 then
   		queststatus = getPlayerStorageValue(cid,6401)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Demon Helmet")
   			doPlayerAddItem(cid,2493,1)
   			setPlayerStorageValue(cid,6401,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 6402 then
   		queststatus = getPlayerStorageValue(cid,6402)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found Demon Legs")
   			doPlayerAddItem(cid,2495,1)
   			setPlayerStorageValue(cid,6402,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   			end
return 1
	else
		return 0
	end
end