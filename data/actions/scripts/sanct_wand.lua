function onUse(cid, item, frompos, item2, topos)
--Settings
sMana = 200 --How much mana does the player need to use it

--DO NOT EDIT THIS
temple = getPlayerMasterPos(cid)
if getPlayerMana(cid) > 200 then
	doSendMagicEffect(frompos, 2)
	doTeleportThing(cid, temple)
	doPlayerAddMana(cid, -sMana)
else
	doSendMagicEffect(frompos, 2)
	doPlayerSendCancel(cid, 'You dont have enough mana')
end
return 1
end