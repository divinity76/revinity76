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

  	if (msgcontains(msg, 'hi') and (focus == 0)) and getDistanceToCreature(cid) < 4 then
  		selfSay('Hello ' .. creatureGetName(cid) .. '! I sell energy rings (1k), time rings (1k), sword rings (500gp), club rings (500gp), axe rings (500gp), dwarven rings (100gp), ring of healings (2k), life rings (1k), might rings (3k), stealth rings (2k), power rings (200gp) I am also selling Stone skin amulets (5k)')
  		focus = cid
  		talk_start = os.clock()

  	elseif msgcontains(msg, 'hi') and (focus ~= cid) and getDistanceToCreature(cid) < 4 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')

	elseif focus == cid then
		talk_start = os.clock()

		if msgcontains(msg, 'energy ring') then
			buy(cid,2167,1,1000)
		elseif msgcontains(msg, 'time ring') then
			buy(cid,2169,1,1000)
		elseif msgcontains(msg, 'sword ring') then
			buy(cid,2207,1,500)
		elseif msgcontains(msg, 'club ring') then
			buy(cid,2209,1,500)
		elseif msgcontains(msg, 'axe ring') then
			buy(cid,2208,1,500)
                elseif msgcontains(msg, 'dwarven ring') then
			buy(cid,2213,1,100)
		elseif msgcontains(msg, 'ring of healing') then
			buy(cid,2214,1,2000)
		elseif msgcontains(msg, 'life ring') then
			buy(cid,2168,1,1000)
		elseif msgcontains(msg, 'might ring') then
			buy(cid,2164,1,3000)
		elseif msgcontains(msg, 'stealth ring') then
			buy(cid,2165,1,2000)
                elseif msgcontains(msg, 'power ring') then
			buy(cid,2166,1,200)
		elseif msgcontains(msg, 'stone skin amulet') then
			buy(cid,2197,1,5000)

		elseif string.find(msg, '(%a*)bye(%a*)') and getDistanceToCreature(cid) < 4 then
			selfSay('Good bye, ' .. creatureGetName(cid) .. '!')
			focus = 0
			talk_start = 0
		end
	end
end


function onCreatureChangeOutfit(creature)

end


function onThink()
  	if (os.clock() - talk_start) > 15 then
  		if focus > 0 then
  			selfSay('Next Please...')
  		end
  			focus = 0
  	end
 	if focus ~= 0 then
 		if getDistanceToCreature(focus) > 3 then
 			selfSay('Good bye then.')
 			focus = 0
 		end
 	end
end
