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

  	if (msgcontains(msg, 'hi') and (focus == 0)) and getDistanceToCreature(cid) <  4 then
		if isPremium(cid) then
			selfSay('Hello ' .. creatureGetName(cid) .. ' , six platinum to get out of prison, say pay to get out.')
			focus = cid
			talk_start = os.clock()
		else
			selfSay('Only Premium players can get out of prison. Everyone else talks to Beebo.')
			focus = 0
			talk_start = 0
		end

  	elseif msgcontains(msg, 'hi') and (focus ~= cid) and getDistanceToCreature(cid) < 4 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I will talk to you in a minute.')

  	elseif focus == cid then
		talk_start = os.clock()

		if msgcontains(msg, 'pay') then
			if pay(cid,600) then
				selfSay('Let\'s go!')
				selfSay('/send ' .. creatureGetName(cid) .. ', 425 354 7')
				focus = 0
				talk_start = 0
			else
				selfSay('Ha, you dont have enough money, you filthy theif! Maybe Beebo will help you.')
			end

		elseif msgcontains(msg, 'out') then
			if pay(cid,600) then
				selfSay('Let\'s go!')
				selfSay('/send ' .. creatureGetName(cid) .. ', 425 354 7')
				focus = 0
				talk_start = 0
			else
				selfSay('Ha, you dont have enough money, you filthy theif! Talk to Beebo.')
			end

		elseif msgcontains(msg, 'bye') and getDistanceToCreature(cid) < 4 then
			selfSay('Good bye, ' .. creatureGetName(cid) .. ' Lets hope we dont see you again.')
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
 		if getDistanceToCreature(focus) > 15 then
 			selfSay('Good bye then.')
 			focus = 0
 		end
 	end
end