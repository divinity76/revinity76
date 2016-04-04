--Slot Machine by Mazero

function onUse(cid, item, frompos, item2, topos)

better = getPlayerPosition(cid)
place1 = {x=451, y=43, z=7}
place2 = {x=452, y=43, z=7}
place3 = {x=453, y=43, z=7}
piece1 = math.random(1,80)
piece2 = math.random(1,80)
piece3 = math.random(1,80)
brick1 = 2625
brick2 = 2626
brick3 = 2629
brick4 = 2628
brick5 = 2630
brick6 = 2631

  if item.itemid == 1946 then
    doTransformItem(item.uid,item.itemid-1)
  elseif item.itemid == 1945 then
    doTransformItem(item.uid,item.itemid+1)
	end

if doPlayerRemoveMoney(cid,100) == 1 then
doPlayerSendCancel(cid,"You bet 100 Gold Coins.")
else
doPlayerSendCancel(cid,"You need more money.")
return 1
end
if piece1 >= 1 and piece1 <= 40 then
doCreateItem(brick1,1,place1)
end
if piece2 >= 1 and piece2 <= 40 then
doCreateItem(brick1,1,place2)
end
if piece3 >= 1 and piece3 <= 40 then
doCreateItem(brick1,1,place3)
end
if piece1 >= 41 and piece1 <= 60 then
doCreateItem(brick2,1,place1)
end
if piece2 >= 41 and piece2 <= 60 then
doCreateItem(brick2,1,place2)
end
if piece3 >= 41 and piece3 <= 60 then
doCreateItem(brick2,1,place3)
end
if piece1 >= 61 and piece1 <= 70 then
doCreateItem(brick3,1,place1)
end
if piece2 >= 61 and piece2 <= 70 then
doCreateItem(brick3,1,place2)
end
if piece3 >= 61 and piece3 <= 70 then
doCreateItem(brick3,1,place3)
end
if piece1 >= 71 and piece1 <= 76 then
doCreateItem(brick4,1,place1)
end
if piece2 >= 71 and piece2 <= 76 then
doCreateItem(brick4,1,place2)
end
if piece3 >= 71 and piece3 <= 76 then
doCreateItem(brick4,1,place3)
end
if piece1 >= 77 and piece1 <= 79 then
doCreateItem(brick5,1,place1)
end
if piece2 >= 77 and piece2 <= 79 then
doCreateItem(brick5,1,place2)
end
if piece3 >= 77 and piece3 <= 79 then
doCreateItem(brick5,1,place3)
end
if piece1 == 80 then
doCreateItem(brick6,1,place1)
end
if piece2 == 80 then
doCreateItem(brick6,1,place2)
end
if piece3 == 80 then
doCreateItem(brick6,1,place3)
end
if piece1 >= 1 and piece1 <= 40 and piece2 >= 1 and piece2 <= 40 and piece3 >= 1 and piece3 <= 40 then
	doPlayerSendTextMessage(cid,22,"You won 4 Platinum Coins.")
	doPlayerAddItem(cid,2152,4)	--change prize here
	end
if piece1 >= 41 and piece1 <= 60 and piece2 >= 41 and piece2 <= 60 and piece3 >= 41 and piece3 <= 60 then
	doPlayerSendTextMessage(cid,22,"You won 8 Platinum Coins.")
	doPlayerAddItem(cid,2152,8)	--change prize here
	end
if piece1 >= 71 and piece1 <= 76 and piece2 >= 71 and piece2 <= 76 and piece3 >= 71 and piece3 <= 76 then
	doPlayerSendTextMessage(cid,22,"You won 32 Platinum Coins.")
	doPlayerAddItem(cid,2152,32) --change prize here
	end
if piece1 >= 77 and piece1 <= 79 and piece2 >= 77 and piece2 <= 79 and piece3 >= 77 and piece3 <= 79 then
	doPlayerSendTextMessage(cid,22,"You won 50 Platinum Coins!")
	doPlayerAddItem(cid,2152,50) --change prize here
	end
if piece1 == 80 and piece2 == 80 and piece3 == 80 then
	doPlayerSendTextMessage(cid,22,"JACKPOT! You won 2 Crystal Coins!")
	doPlayerAddItem(cid,2160,2) --change prize here
	end
return 0
end	