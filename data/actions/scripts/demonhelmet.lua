function onUse(cid, item, frompos, item2, topos)

   	if item.uid == 6406 then
   		queststatus = getPlayerStorageValue(cid,6406)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Demon shield.")
   			doPlayerAddItem(cid,2520,1)
   			setPlayerStorageValue(cid,6406,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 6407 then
   		queststatus = getPlayerStorageValue(cid,6407)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Demon Helmet")
   			doPlayerAddItem(cid,2493,1)
   			setPlayerStorageValue(cid,6407,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 6408 then
   		queststatus = getPlayerStorageValue(cid,6408)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found Steel boots")
   			doPlayerAddItem(cid,2645,1)
   			setPlayerStorageValue(cid,6408,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   			end
return 1
	else
		return 0
	end
end