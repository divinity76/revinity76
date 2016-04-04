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
 
 	if ((string.find(msg, '(%a*)hi(%a*)')) and (focus == 0)) and getDistanceToCreature(cid) < 3 then
 		selfSay('Hey there! I am Mister Panda, how are you?')
 		focus = cid
 		talk_start = os.clock()
 	end
 
 	if string.find(msg, '(%a*)hi(%a*)') and (focus ~= cid) and getDistanceToCreature(cid) < 3 then
 		selfSay('Just a second fella...')
 	end
 
 	if msgcontains(msg, 'fuck') and focus == cid then
		selfSay('You should wash your mouth with soap!')
 		selfSay('dragon_firewave "' .. creatureGetName(cid) .. '')
 		talk_start = os.clock()
 	end
 	
 	 	if msgcontains(msg, 'gay') and focus == cid then
		selfSay('Gay means happy.')
 		selfSay('dragon_firewave "' .. creatureGetName(cid) .. '')
 		talk_start = os.clock()
 	end
 	
 	 	if msgcontains(msg, 'fag') and focus == cid then
		selfSay('Fag? No thanks I dont smoke.')
 		selfSay('dragon_firewave "' .. creatureGetName(cid) .. '')
 		talk_start = os.clock()
 	end
 	
 	 	if msgcontains(msg, 'ass') and focus == cid then
		selfSay('Ass? I dont see a donkey.')
 		selfSay('dragon_firewave "' .. creatureGetName(cid) .. '')
 		talk_start = os.clock()
 	end
 	
 	 	if msgcontains(msg, 'cock') and focus == cid then
		selfSay('COCK A DOODLE DOO')
 		selfSay('dragon_firewave "' .. creatureGetName(cid) .. '')
 		talk_start = os.clock()
 	end
 	
 	 	 	if msgcontains(msg, 'noob') and focus == cid then
		selfSay('And how long have YOU played?')
 		selfSay('dragon_firewave "' .. creatureGetName(cid) .. '')
 		talk_start = os.clock()
 	end
 	
 	 	 	

 			  if string.find(msg, '(%a*)bye(%a*)') and focus == cid and getDistanceToCreature(cid) < 3 then
 		selfSay('Have a great day.')
 		focus = 0
 		talk_start = 0
 	end
 end
 
 
 function onCreatureChangeOutfit(creature)
 
 end
 
 
 function onThink()
 	if (os.clock() - talk_start) > 30 then
 		if focus > 0 then
 			selfSay('I sure do love bamboo..')
 		end
 			focus = 0
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
selfSay('Someone always gets hurt when you say rude things.I promise.') 
end 
if randsay == 2 then 
selfSay('How do I talk? I am magic silly!') 
end 
end 
if focus > 0 then
 x, y, z = creatureGetPosition(focus)
 myx, myy, myz = selfGetPosition()
 if ((myy-y==0) and (myx-x<=0 and myx-x>=-4)) then
       selfTurn(0)
 end
 if ((myy-y==0) and (myx-x>=0 and myx-x<=4)) then
       selfTurn(2)
 end
       if ((myx-x==0) and (myy-y<=0 and myy-y>=-4)) then
       selfTurn(1)
 end
 if ((myx-x==0) and (myy-y>=0 and myy-y<=4)) then
       selfTurn(3)
 end
 if ((myy-y==-2) and (myx-x>=-1 and myx-x<=1)) then
       selfTurn(1)
 end
 if ((myy-y==2) and (myx-x>=-1 and myx-x<=1)) then
        selfTurn(3)
 end
 if ((myx-x==2) and (myy-y>=-1 and myy-y<=1)) then
        selfTurn(2)
 end
 if ((myx-x==-2) and (myy-y>=-1 and myy-y<=1)) then
        selfTurn(0)
 end
 if ((myy-y==-3) and (myx-x>=-2 and myx-x<=2)) then
        selfTurn(1)
 end
 if ((myy-y==3) and (myx-x>=-2 and myx-x<=2)) then
        selfTurn(3)
 end
 if ((myx-x==3) and (myy-y>=-2 and myy-y<=2)) then
        selfTurn(2)
 end
 if ((myx-x==-3) and (myy-y>=-2 and myy-y<=2)) then
        selfTurn(0)
 end
 if ((myy-y==-4) and (myx-x>=-3 and myx-x<=3)) then
        selfTurn(1)
 end
 if ((myy-y==4) and (myx-x>=-3 and myx-x<=3)) then
        selfTurn(3)
 end
 if ((myx-x==4) and (myy-y>=-3 and myy-y<=3)) then
        selfTurn(2)
 end
 if ((myx-x==-4) and (myy-y>=-3 and myy-y<=3)) then
        selfTurn(0)
 end
end
end
 