-- Fencedoor Open/Close

function onUse(cid, item, frompos, item2, topos)
	if item.itemid == 1541 then
		doTransformItem(item.uid,1542)
		  doMovePlayer(getplayer.uid,1)
	elseif item.itemid == 1542 then
		doTransformItem(item.uid,1541)
	elseif item.itemid == 1539 then
		doTransformItem(item.uid,1540)
	elseif item.itemid == 1540 then
		doTransformItem(item.uid,1539)
	elseif getplayer.itemid > 0 then
  		doMovePlayer(getplayer.uid,1)
	end
end