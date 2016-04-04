function onUse(cid, item, frompos, item2, topos)

   	-- from main to island
   	if item.uid == 7004 then
   		queststatus = getPlayerStorageValue(cid,2392)

   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"Do the quest, fire sword.")
   		else
			gopos = {x=272, y=100, z=8}
   			doTeleportThing(cid, gopos)		end
   	-- from island to main
   	elseif item.uid == 7005 then
   		backpos = {x=118, y=36, z=6}
   		doTeleportThing(cid, backpos)
		doSendMagicEffect(cid,10)

	else
		return 0
   	end

   	return 1
end
