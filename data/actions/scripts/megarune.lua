 function onUse(cid, item, frompos, item2, topos)
mag = getPlayerMagLevel(cid)
if mag >= 5 then
 attackType = ATTACK_NONE
 animationEffect = NM_ANI_NONE
 
 hitEffect = NM_ME_NONE
 damageEffect = NM_ME_NONE
 animationColor = GREEN
 offensive = false
 drawblood = false
doSendMagicEffect(topos,1)
doPlayerSay(cid,"",16)
doPlayerAddMana(cid, 20000)
doPlayerAddHealth(cid, 20000)
if item.type > 1 then
doChangeTypeItem(item.uid,item.type-1)
else
doRemoveItem(item.uid,100)
end
else doSendMagicEffect(frompos,2)
doPlayerSendCancel(cid,"You don't have the required magic level to use that rune.")
end
return 1
end
