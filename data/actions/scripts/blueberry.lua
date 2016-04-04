function onUse(cid, item, frompos, item2, topos)

    foodtime = 12   -- Time in seconds that it generates health/mana
    fulltime = 400  -- When is a player full (time measured in seconds)
    
    if (getPlayerFood(cid) < fulltime) then
        doPlayerFeed(cid, foodtime)
        if (item.type > 1) then
            doChangeTypeItem(item.uid, item.type-1)
        else
            doRemoveItem(item.uid, item.type)
        end
    else
        doPlayerSendCancel(cid, "You are full.")
    end
end