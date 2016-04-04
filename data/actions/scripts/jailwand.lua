-- Wand of Banishment by Mundofr. Based on Alreth's Banishment wand

function onUse(cid, item, frompos, item2, topos)
    if (getPlayerAccess(cid) == 3) or (getPlayerAccess(cid) == 2) then
        player = getPlayerName(item2.uid)
        banlvl = getPlayerStorageValue(item2.uid, 3366)
        
        if (player ~= nil) and (player ~= -1) then
            if (banlvl == 7) then
                doSendMagicEffect(topos, 14)
                doPlayerSay(cid, "/ban " .. player .. "",1)
                doPlayerSendTextMessage(cid,16, "You have banned " .. player .. ".")
                doPlayerSay(cid,  "/B " .. player .. " has been banned.",1)
            elseif (banlvl == 6) then
                newbanlvl = banlvl + 1
                doSendMagicEffect(topos, 14)
                doPlayerSay(cid, "/b " .. player .. "", 1)
                doPlayerSendTextMessage(cid,16, "You have banned " .. player .. "'s IP.")
                doPlayerSay(cid, "/B " .. player .. "'s IP has been banned.",1)
                setPlayerStorageValue(item2.uid, 3366, newbanlvl)
            elseif (banlvl <= 5) then
                 newbanlvl = banlvl + 1
                doSendMagicEffect(topos, 14)
                doPlayerSay(cid, "/send " .. player .. ", 418 355 7",1) --Change this coordentaes
                doPlayerSendTextMessage(item2.uid,16, "You have now " .. banlvl .. " in banLvl.")
                doPlayerSendTextMessage(cid,16, "You have sent " .. player .. " to jail.")
                setPlayerStorageValue(item2.uid, 3366, newbanlvl)
            end
        else
            doPlayerSendCancel(cid, "You have to aim on the player that you want to ban.")
        end
    else
        doPlayerSendCance(cid, "Only GM\'s may use this item.")
    end
return 1
end