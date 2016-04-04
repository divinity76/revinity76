function onUse(cid, item, frompos, item2, topos)

   	if item.uid == 6403 then
   		queststatus = getPlayerStorageValue(cid,6403)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Great shield")
   			doPlayerAddItem(cid,2522,1)
   			setPlayerStorageValue(cid,6403,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 6404 then
   		queststatus = getPlayerStorageValue(cid,6404)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Ice Rapier")
   			doPlayerAddItem(cid,2396,1)
   			setPlayerStorageValue(cid,6404,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 6405 then
   		queststatus = getPlayerStorageValue(cid,6405)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found Crystal Arrow")
   			doPlayerAddItem(cid,2352,1)
   			setPlayerStorageValue(cid,6405,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   			end
return 1
	else
		return 0
	end
end