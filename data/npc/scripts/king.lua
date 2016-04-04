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

end function msgcontains(txt, str)
 	return (string.find(txt, str) and not string.find(txt, '(%w+)' .. str) and not string.find(txt, str .. '(%w+)'))
end


function onCreatureSay(cid, type, msg)
 	msg = string.lower(msg)

 	if ((string.find(msg, '(%a*)hi(%a*)')) and (focus == 0)) and getDistanceToCreature(cid) < 4 then
		selfSay('Hello, my friend' .. creatureGetName(cid) .. '! How are we today?.')
		focus = cid
		talk_start = os.clock()

	elseif string.find(msg, '(%a*)hi(%a*)') and (focus ~= cid) and getDistanceToCreature(cid) < 4 then
 		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')  	elseif focus == cid then
		talk_start = os.clock()

		if msgcontains(msg, 'promotion') then
			if isPromoted(cid) then
				selfSay('Sorry, you are already promoted.')
				focus = 0
			else
				if getPlayerLevel(creatureGetName(cid)) >= 20 then
					if pay(cid,20000) then
						promote(cid)
						selfSay('You are now a noble warrior!')
					else
						selfSay('Sorry, you do not have enough money.')
					end
				else
					selfSay('Sorry, you need level 20 to buy promotion.')
				end
			end

 		elseif string.find(msg, '(%a*)bye(%a*)')  and getDistanceToCreature(cid) < 4 then
 			selfSay('Good bye, ' .. creatureGetName(cid) .. '!')
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
selfSay('Farwell...') 
end 
focus = 0 
end 
if focus ~= 0 then 
if getDistanceToCreature(focus) > 3 then 
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
selfSay('Hmmhmm...') 
end 
if randsay == 2 then 
selfSay('What to do about the orcs...') 
end 
end 
end