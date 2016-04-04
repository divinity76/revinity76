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
  		selfSay('hello ' .. creatureGetName(cid) .. ' If you want out you suck it and suck it good.. I wont tell nobody.')
  		focus = cid
  		talk_start = os.clock()

  	elseif msgcontains(msg, 'hi') and (focus ~= cid) and getDistanceToCreature(cid) < 4 then
  		selfSay('Wait a sec sweet cheeks, ' .. creatureGetName(cid) .. '! Your gonna get some.')

	elseif focus == cid then
		talk_start = os.clock()

		if msgcontains(msg, 'fag') then
                                          selfSay('Why I oughta....')
                           elseif msgcontains(msg, 'suck') then
                                          selfSay('/B ' .. creatureGetName(cid) .. '! Just sucked me off to be released from prison!')
                                          selfSay('/send ' .. creatureGetName(cid) .. ', 425 354 7')
                           elseif msgcontains(msg, 'bye') and getDistanceToCreature(cid) < 4 then
			selfSay('Whatever cock hole.')
			focus = 0
			talk_start = 0
		end
	end
end


function onCreatureChangeOutfit(creature)

end


function onThink()
  	if (os.clock() - talk_start) > 30 then
  		if focus > 0 then
  			selfSay('Anybody else.')
  		end
  			focus = 0
  	end
 	if focus ~= 0 then
 		if getDistanceToCreature(focus) > 5 then
 			selfSay('Later bitch.')
 			focus = 0
 		end
 	end
if focus == 0 then 
cx, cy, cz = selfGetPosition() 
randmove = math.random(1,25) 
if randmove == 1 then 
nx = cx + 1 
end 
if randmove == 2 then 
nx = cx - 1 
end 
if randmove == 3 then 
ny = cy + 1 
end 
if randmove == 4 then 
ny = cy - 1 
end 
if randmove >= 5 then 
nx = cx 
ny = cy 
end 
moveToPosition(nx, ny, cz) 
--summons = 30 
--summons2 = 30 
end 
if focus == 0 then 
randsay = math.random(1,75) 
if randsay == 1 then 
selfSay('Gonna break me a bitch...') 
end 
if randsay == 2 then 
selfSay('So many fish, so little time.') 
end 
end 
end