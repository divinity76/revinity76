--Anstice's Incredible Teleport Rune
function onUse(cid, item, frompos, item2, topos)
doTeleportThing(cid,topos)
doSendMagicEffect(frompos)
doSendMagicEffect(topos)
if item2.itemid == 0 then
return 0
end
return 1
end