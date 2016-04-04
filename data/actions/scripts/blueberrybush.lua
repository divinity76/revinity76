function onUse(cid, item, frompos, item2, topos)

    bluebushid = 2785   -- Itemid of the blueberry bush
     bushid =  2786      -- Itemid of the empty bush
    blueberryid = 2677  -- Itemid of blueberries
    
    doTransformItem(item.uid, bushid)
    doCreateItem(blueberryid, 3, frompos)
    doDecayItem(item.uid)
end