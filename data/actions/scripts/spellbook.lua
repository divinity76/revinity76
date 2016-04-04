function onUse(cid, item, frompos, item2, topos)

if getPlayerVocation(cid) == 1 then
doSetItemText(item.uid,"-Spell Book For Sorcerer:\nexura vita - mag 8 mana 80\nexura gran - mag 2 mana 40\nexura - mag 1 mana 25\nexevo gran mas vis - mag 40 mana 800\nexevo gran vis lux - mag 14 mana 200\nexevo mort hur - mag 20 mana 250\nutevo res ina - mag 10 mana 100\nutamo vita - mag 4 mana 50\nutani hur - mag 4 mana 60\nutani gran hur - mag 8 mana 100\nexevo gran mas pox - mag 28 mana 600\nexori flam - mag 3 mana 20\nexori mort - mag 2 mana 20\nexori vis - mag 3 mana 20\nexevo flam hur - mag 7 mana 80\nexevo pan - mag 0 mana 30\n-Runes:\nadori gran flam - mag 9 mana 120\nadori - mag 2 mana 40\nadevo grav flam - mag 3 mana 60\nadevo mas hur - mag 12 mana 180\nadori vita vis - mag 25 mana 220\nadevo mas flam - mag 9 mana 150\nadevo grav tera - mag 14 mana 250\nadevo res flam - mag 13 mana 150\nadevo grav pox - mag 1 mana 50\nadito grav - mag 3 mana 60\nadevo grav vis - mag 3 mana 80\nadori blank - mag 12 mana 200")

elseif getPlayerVocation(cid) == 2 then
doSetItemText(item.uid, "-Spell Book For Druid:\nexura vita - mag 8 mana 80\nexura gran - mag 2 mana 40\nexura - mag 1 mana 25\nexura gran mas res - mag 19 mana 150\nutevo res ina - mag 10 mana 100\nutamo vita - mag 4 mana 50\nexura sio - mag 7 mana 70\nutani hur - mag 4 mana 60\nutani gran hur - mag 8 mana 100\nexevo gran mas pox - mag 28 mana 600\nexori flam - mag 3 mana 20\nexori mort - mag 2 mana 20\nexori vis - mag 3 mana 20\nexevo pan - mag 0 mana 30\n-Runes:\nadura vita - mag 11 mana 100\nadura gran - mag 1 mana 60\nadori gran flam - mag 9 mana 120\nadori gran - mag 3 mana 70\nadori - mag 2 mana 40\nadevo grav flam - mag 3 mana 60\nadevo mas hur - mag 12 mana 180\nadevo mas flam - mag 9 mana 150\nadevo res flam - mag 13 mana 150\nadevo grav pox - mag 1 mana 50\nadito grav - mag 3 mana 60\nadevo grav vis - mag 3 mana 80\nadori blank - mag 12 mana 200\nadevo max pox - mag 8 mana 130")

elseif getPlayerVocation(cid) == 3 then
doSetItemText(item.uid,"-Spell Book For Paladin:\nexura vita - mag 8 mana 80\nexura gran - mag 2 mana 40\nexura - mag 1 mana 25\nutamo vita - mag 4 mana 50\nutani hur - mag 4 mana 60\nexevo con - mag 2 mana 40\nexevo con mort - mag 6 mana 70\nexevo con pox - mag 5 mana 70\nexevo con flam - mag 10 mana 120\nexevo con vis - mag 14 mana 200\nexevo pan - mag 0 mana 30\nadori gran - mag 3 mana 70\nadori - mag 2 mana 40\nadito grav - mag 3 mana 60")

elseif getPlayerVocation(cid) == 4 then
doSetItemText(item.uid,"-Spell Book For Knight:\nutevo lux - mag 0 mana 20\nexura - mag 1 mana 25\nutani hur - mag 4 mana 60\nexori - mag 5 mana 100\nexori gran - mag 7 mana 175\n")

end

doShowTextWindow(item.uid,0,0)
doSetItemText(item.uid,"")
return 1
end