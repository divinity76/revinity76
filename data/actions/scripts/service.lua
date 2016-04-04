--Example remove money--
   
   
   function onUse(cid, item, frompos, item2, topos)
   
    	if item.uid == 2300 then
   
   	 if doPlayerRemoveMoney(cid, 20) == 1 then 
   		doPlayerAddItem(cid, 1998, 1)
                   doSendMagicEffect(topos,12)
   else
   		doPlayerSendCancel(cid, "You dont have enough money.")
   
   end
    	elseif item.uid == 2301 then
   
   	   if doPlayerRemoveMoney(cid, 20) == 1 then 
   		doPlayerAddItem(cid, 1999, 1)
                   doSendMagicEffect(topos,12)
   else
   		doPlayerSendCancel(cid, "You dont have enough money.")
   
   end
    	elseif item.uid == 2302 then
   
   	  if doPlayerRemoveMoney(cid, 20) == 1 then 
   		doPlayerAddItem(cid, 2000, 1)
                   doSendMagicEffect(topos,12)
   else
   		doPlayerSendCancel(cid, "You dont have enough money.")
   
   end
    	elseif item.uid == 2303 then
   
   	 if doPlayerRemoveMoney(cid, 20) == 1 then 
   		doPlayerAddItem(cid, 2001, 1)
                   doSendMagicEffect(topos,12)
   else
   		doPlayerSendCancel(cid, "You dont have enough money.")
   
   end
    	elseif item.uid == 2304 then
   
   	 if doPlayerRemoveMoney(cid, 20) == 1 then 
   		doPlayerAddItem(cid, 2002, 1)
                   doSendMagicEffect(topos,12)
   else
   		doPlayerSendCancel(cid, "You dont have enough money.")
   
   end
    	elseif item.uid == 2305 then
   
   	  if doPlayerRemoveMoney(cid, 20) == 1 then 
   		doPlayerAddItem(cid, 2003, 1)
                   doSendMagicEffect(topos,12)
   else
   		doPlayerSendCancel(cid, "You dont have enough money.")
   
   end
    	elseif item.uid == 2306 then
   
   	 if doPlayerRemoveMoney(cid, 20) == 1 then 
   		doPlayerAddItem(cid, 2004, 1)
                   doSendMagicEffect(topos,12)
   else
   		doPlayerSendCancel(cid, "You dont have enough money.")
   
   end
    	elseif item.uid == 2307 then
   
   	 if doPlayerRemoveMoney(cid, 20) == 1 then 
   		doPlayerAddItem(cid, 2260, 1)
                   doSendMagicEffect(topos,12)
   else
   		doPlayerSendCancel(cid, "You dont have enough money.")
   
   end
   
   end
   
   	
   	return 1
   end