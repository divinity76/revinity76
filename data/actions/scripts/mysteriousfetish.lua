--Key system made by Orzech--
 function onUse(cid, item, frompos, item2, topos)
 if item.actionid == item2.actionid then
 if item2.itemid == 1225 then
 doTransformItem(item2.uid,item2.itemid+1)
 elseif item2.itemid == 1226 then
 doTransformItem(item2.uid,item2.itemid-1)
 
 
 end
 else
 return 0
 end
 return 1
 end
