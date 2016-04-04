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
  		selfSay('Hello ' .. creatureGetName(cid) .. '! I sell Decco for your house. say: diamonds, flowers, other')
  		focus = cid
  		talk_start = os.clock()

  	elseif msgcontains(msg, 'hi') and (focus ~= cid) and getDistanceToCreature(cid) < 4 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')

	elseif focus == cid then
		talk_start = os.clock()

		if msgcontains(msg, 'diamonds') then
			selfSay('There is: black pearls, White pearls, small diamonds, violet gem, yellow gem. blue gem. big emerald. small sapphirs. small rubys. small emeralds. small amethysts (20k)')
		elseif msgcontains(msg, 'flowers') then
			selfSay('There is: red rose. blue rose. yellow rose. (2k)')
		elseif msgcontains(msg, 'other') then
			selfSay('There is: Talons. Gold nuggets. scarab coins. Blood herbs. (15k)')

		elseif msgcontains(msg, 'talons') then
			buy(cid,2151,10,20000)
		elseif msgcontains(msg, 'black pearls') then
			buy(cid,2144,10,20000)
		elseif msgcontains(msg, 'white pearls') then
			buy(cid,2143,10,20000)
		elseif msgcontains(msg, 'small diamonds') then
			buy(cid,2145,10,20000)
		elseif msgcontains(msg, 'violet gem') then
			buy(cid,2153,1,20000)

		elseif msgcontains(msg, 'yellow gem') then
			buy(cid,2154,1,20000)
		elseif msgcontains(msg, 'big emerald') then
			buy(cid,2155,1,20000)
		elseif msgcontains(msg, 'big ruby') then
			buy(cid,2156,1,20000)
		elseif msgcontains(msg, 'gold nuggets') then
			buy(cid,2157,5,15000)
		elseif msgcontains(msg, 'blue gem') then
			buy(cid,2158,1,20000)

		elseif msgcontains(msg, 'scarab coins') then
			buy(cid,2159,50,15000)
		elseif msgcontains(msg, 'blood herbs') then
			buy(cid,2798,3,15000)
		elseif msgcontains(msg, 'small sapphirs') then
			buy(cid,2311,10,20000)
		elseif msgcontains(msg, 'small rubys') then
			buy(cid,2147,10,20000)
		elseif msgcontains(msg, 'small emeralds') then
			buy(cid,2149,10,20000)
		elseif msgcontains(msg, 'small amethysts') then
			buy(cid,2150,10,20000)
		elseif msgcontains(msg, 'red rose') then
			buy(cid,2744,1,2000)
		elseif msgcontains(msg, 'blue rose') then
			buy(cid,2745,1,2000)
		elseif msgcontains(msg, 'yellow rose') then
			buy(cid,2746,1,2000)


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
