function onUse(cid, item, frompos, item2, topos)

    storefishtime	 = 301 -- Storage value where last fishing time is saved
    storeroduse		 = 302 -- Storage value where amount of rod uses is saved
    rodbreak		 = 1000 -- Amount of times player can use rod before it break
    fishingexhaust	 = 1 -- Time in seconds for exhausted
    fishid		 = 2667       -- Enter the itemid of a fish
    fishingrodid	 = 2580 -- Enter the itemid of a fishing rod

    theTime = os.time()
    oldTime = getPlayerStorageValue(cid, storefishtime)
    if (oldTime == nil or oldTime < 1 or os.difftime(theTime, oldTime) < 0) then
        oldTime = 1
    end
    
    diffTime = os.difftime(theTime, oldTime)
    
    if (fishingexhaust == nil or fishingexhaust < 0) then
        fishingexhaust = 0
    end
    
    if (diffTime >= fishingexhaust) then
        setPlayerStorageValue(cid, storefishtime, theTime) 
        rodUses = getPlayerStorageValue(cid, storeroduse)
        rodUses = rodUses + 1
        
        if (rodUses >= rodbreak) then
            doRemoveItem(item.uid, item.type)
            doPlayerSendTextMessage(cid, 20, "Your rod broke due to over use.")
            rodUses = 0
            doSendMagicEffect(getPlayerPosition(cid), 2)
        end
        setPlayerStorageValue(cid, storeroduse, rodUses)
        
        if ((item2.itemid >= 4608 and item2.itemid <= 4625)) then
            setPlayerStorageValue(cid, storefishtime, theTime)
            skill_level = getPlayerSkill(cid, 6)
            random_number = math.random(1, (100+skill_level/10))
            doSendMagicEffect(topos, 1)
            
            if random_number <= skill_level then
                catchtype = math.random(1, 4)

                if (catchtype == 1) then
                    doPlayerAddItem(cid, 2667, 1)
                end
            end
            doPlayerAddSkillTry(cid, 6, 1)
            
        else
            doPlayerSendCancel(cid, "You can not use this object.")
        end
    else
        doPlayerSendCancel(cid, "You are exhausted.")
    end
    return 1
end