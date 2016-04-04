function onUse(cid, item, frompos, item2, topos)

   	-- from main to island
   	if item.uid == 7002 then
   		queststatus = getPlayerStorageValue(cid,2392)

   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"Do the quest, fire sword.")
   		else
			gopos = {x=277, y=40, z=8}
   			doTeleportThing(cid, gopos)		end
   	-- from island to main
   	elseif item.uid == 7003 then
   		backpos = {x=118, y=36, z=6}
   		doTeleportThing(cid, backpos)
		doSendMagicEffect(cid,10)

	else
		return 0
   	end

   	return 1
end
