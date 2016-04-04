focus = 0
  talk_start = 0
  target = 0
  following = false
  attacking = false
 
  function onThingMove(creature, thing, oldpos, oldstackpos)
 
  end
 
 
  function onCreatureAppear(creature)
 
  end
 
 
  function onCreatureDisappear(cid, pos)
  	if focus == cid then
          selfSay('Good bye then.')
          focus = 0
          talk_start = 0
  	end
  end
 
 
  function onCreatureTurn(creature)
 
  end
 
  function msgcontains(txt, str)
  	return (string.find(txt, str) and not string.find(txt, '(%w+)' .. str) and not string.find(txt, str .. '(%w+)'))
  end
 
  function onCreatureSay(cid, type, msg)
  	msg = string.lower(msg)
 
  	if ((string.find(msg, '(%a*)hi(%a*)')) and (focus == 0)) and getDistanceToCreature(cid) < 4 then
  		selfSay('Hello ' .. creatureGetName(cid) .. '! I sell weapons of many types, like axes, swords and clubs.')
  		focus = cid
  		talk_start = os.clock()
  	end
 
  	if string.find(msg, '(%a*)hi(%a*)') and (focus ~= cid) and getDistanceToCreature(cid) < 4 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')
  	end
 
  	if msgcontains(msg, 'axes') and focus == cid then
  		selfSay('I sell hatchet, battle axe, double axe and orcish axe.')
  		talk_start = os.clock()
  	end
 
  	if msgcontains(msg, 'hatchet') and focus == cid then
  		buy(cid,2388,1,50)
  		talk_start = os.clock()
  	end
 	if msgcontains(msg, 'battle axe') and focus == cid then
  		buy(cid,2378,1,100)
  		talk_start = os.clock()
  	end
  	if msgcontains(msg, 'double axe') and focus == cid then
  		buy(cid,2387,1,220)
  		talk_start = os.clock()
  	end
 
  	if msgcontains(msg, 'orcish axe') and focus == cid then
  		buy(cid,2428,1,130)
  		talk_start = os.clock()
  	end
 
  	if msgcontains(msg, 'swords') and focus == cid then
  		selfSay('I sell katana, broad sword, two-handed sword and scimitar.')
  		talk_start = os.clock()
  	end
 
  	if msgcontains(msg, 'katana') and focus == cid then
  		buy(cid,2412,1,50)
  		talk_start = os.clock()
  	end
 
  	if msgcontains(msg, 'broad sword') and focus == cid then
  		buy(cid,2413,1,100)
  		talk_start = os.clock()
  	end
 
  	if msgcontains(msg, 'two-handed sword') and focus == cid then
  		buy(cid,2377,1,200)
  		talk_start = os.clock()
  	end
 
  	if msgcontains(msg, 'scimitar') and focus == cid then
  		buy(cid,2419,1,130)
  		talk_start = os.clock()
  	end
 
  	if msgcontains(msg, 'clubs') and focus == cid then
  		selfSay('I sell mace, daramanian mace and battle hammer.')
  		talk_start = os.clock()
  	end
 
 
  	if msgcontains(msg, 'mace') and focus == cid then
  		buy(cid,2398,1,50)
  		talk_start = os.clock()
  	end
 
  	if msgcontains(msg, 'daramanian') and focus == cid then
  		buy(cid,2439,1,130)
  		talk_start = os.clock()
  	end
 
  	if msgcontains(msg, 'battle hammer') and focus == cid then
  		buy(cid,2417,1,240)
  		talk_start = os.clock()
  	end
 
  	if string.find(msg, '(%a*)bye(%a*)') and focus == cid and getDistanceToCreature(cid) < 4 then
  		selfSay('Good bye, ' .. creatureGetName(cid) .. '!')
  		focus = 0
  		talk_start = 0
  	end
  end
 
 
  function onCreatureChangeOutfit(creature)
 
  end
 
 
  function onThink()
  	if (os.clock() - talk_start) > 30 then
  		if focus > 0 then
  			selfSay('Next Please...')
  		end
  			focus = 0
  	end
 	if focus ~= 0 then
 		if getDistanceToCreature(focus) > 5 then
 			selfSay('Good bye then.')
 			focus = 0
 		end
 	end
  end
 
