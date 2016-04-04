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

  	if (msgcontains(msg, 'hi') and focus == 0) and getDistanceToCreature(cid) < 4 then
  		selfSay('Hey there ' .. creatureGetName(cid) .. '! Beer and Wine is what we got.')
  		focus = cid
  		talk_start = os.clock()

  	elseif msgcontains(msg, 'hi') and (focus ~= cid) and getDistanceToCreature(cid) < 4 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! Go play slots ya drunk.')

  	elseif focus == cid then
		talk_start = os.clock()

		if msgcontains(msg, 'beer') then
			buy(cid,2006,3,10)
		elseif msgcontains(msg, 'wine') then
			buy(cid,2006,15,10)

		elseif msgcontains(msg, 'bye') and getDistanceToCreature(cid) < 4 then
			selfSay('Later on, ' .. creatureGetName(cid) .. '!')
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
selfSay('Dwarven brew is not for the weak...') 
end 
if randsay == 2 then 
selfSay('I just got in slot machines, why dont ya give em a yank?') 
end 
end 
end
