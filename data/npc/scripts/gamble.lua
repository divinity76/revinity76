focus = 0
next_focus = 0
talk_start = 0
target = 0
following = false
attacking = false
msgBye = 'Hey, you can\'t leave. Luck is smiling on you. I can feel it!'
maxBid = 5000
bid = 0
dice = 0
inConvo = 0
bidTradeItemID = 1761

function onThingMove(creature, thing, oldpos, oldstackpos)

end

function onCreatureAppear(creature)

end

function onCreatureDisappear(cid, pos)
     if (focus == cid) then
        selfSay(msgBye)
        if (next_focus > 0 and getDistanceToCreature(next_focus) < 4) then
            selfSay('Greetings, high roller. So you care for a game,  ' .. creatureGetName(next_focus) .. '?')
            talk_start = os.clock()
            focus = next_focus
            next_focus = 0
            inConvo = 1
        else
            focus = 0
            talk_start = 0
            inConvo = 0
        end
     end
end


function onCreatureTurn(creature)

end

function msgcontains(txt, str)
     return (string.find(txt, str) and not string.find(txt, '(%w+)' .. str) and not string.find(txt, str .. '(%w+)'))
end


function onCreatureSay(cid, type, msg)
    msg = string.lower(msg)
    
    if (type == 1) then

         if ((string.find(msg, '(%a*)hi(%a*)') or string.find(msg, '(%a*)hello(%a*)')) and (focus == 0) and getDistanceToCreature(cid) < 4) then
            selfSay('Greetings, high roller. So you care for a game,  ' .. creatureGetName(cid) .. '?')
            focus = cid
            talk_start = os.clock()
            inConvo = 1

        elseif ((string.find(msg, '(%a*)hi(%a*)') or string.find(msg, '(%a*)hello(%a*)')) and (focus ~= cid) and getDistanceToCreature(cid) < 4)  then
             if (next_focus > 0 and getDistanceToCreature(next_focus) < 4) then
                 selfSay('Take a minute to count your money, ' .. creatureGetName(cid) .. '. There are people ahead of you!')
             else
                 selfSay('Take a minute to count your money, ' .. creatureGetName(cid) .. '. I\'ll be here soon.')
                 next_focus = cid
             end

         elseif (focus == cid) then
            talk_start = os.clock()

            if (msgcontains(msg, 'dice') or msgcontains(msg, 'game') or msgcontains(msg, 'gamble')) then
                if (getPlayerLevel(creatureGetName(cid)) >= 18) then
                    selfSay('So you care for a civilized game of dice?')
                    inConvo = 1
                else
                    selfSay('I am sorry, but the law forbids me to let kids like you to gamble. Come back when you\'re older!')
                end

            elseif (msgcontains(msg, 'yes') and inConvo == 1) then
                selfSay('I will roll a dice. If it shows 6, you will get five times your bet. How much do you want to bet?')
                inConvo = 2

            elseif (msgcontains(msg, 'no') and inConvo == 1) then
                selfSay('Oh come on, don\'t be a child.')
                inConvo = 0

            elseif ((tonumber(msg) ~= nil) and inConvo == 2) then
                if (tonumber(msg) <= 0 or tonumber(msg) > maxBid) then
                    selfSay('I am sorry, but I accept only bets between 1 and 5,000 gold. I don\'t want to ruin you after all. How much do you want to bet?')
                else
                    bid = tonumber(msg)
                    if (pay(cid,bid)) then
                        dice = math.random(1,6)
                        if (dice == 6) then
                            buy(cid,bidTradeItemID,1,0)
                            sell(cid,bidTradeItemID,1,(bid*5))
                            selfSay('Ok, here we go ... ' .. dice .. '! You have won ' .. (bid * 5) .. ', congratulations. One more game?')
                            inConvo = 1
                        else
                            selfSay('Ok, here we go ... ' .. dice .. '! You have lost. Bad luck. One more game?')
                            inConvo = 1
                        end
                    else
                        selfSay('I am sorry, but you don\'t have so much money. How much do you want to bet?')
                        inConvo = 2
                    end
                end

            elseif (string.find(msg, '(%a*)venore(%a*)') and getDistanceToCreature(cid) < 4) then
                selfSay('Aaaah, Venore, Venore, what a wonderful town. Especially for someone with love for gambling like me.')
                inConvo = 0

            elseif (string.find(msg, '(%a*)job(%a*)') and getDistanceToCreature(cid) < 4) then
                selfSay('Oh, I am just sitting around here and gamble now and then.')
                inConvo = 0

             elseif (string.find(msg, '(%a*)bye(%a*)') and getDistanceToCreature(cid) < 4) then
                selfSay(msgBye)
                if (next_focus > 0 and getDistanceToCreature(next_focus) < 4) then
                    selfSay('Greetings, high roller. So you care for a game,  ' .. creatureGetName(next_focus) .. '?')
                    talk_start = os.clock()
                    focus = next_focus
                    next_focus = 0
                    inConvo = 1
                else
                    focus = 0
                    talk_start = 0
                    inConvo = 0
                end
             end
         end
     end
end


function onCreatureChangeOutfit(creature)

end


function onThink()
     if (os.clock() - talk_start) > 30 then
        if (focus > 0) then    
            selfSay(msgBye)
            if (next_focus > 0 and getDistanceToCreature(next_focus) < 4) then
                selfSay('Greetings, high roller. So you care for a game,  ' .. creatureGetName(next_focus) .. '?')
                talk_start = os.clock()
                focus = next_focus
                next_focus = 0
                inConvo = 1
            else
                focus = 0
                talk_start = 0
                inConvo = 0
            end
        end
     end
    if focus ~= 0 then
        if (getDistanceToCreature(focus) > 5) then
            selfSay(msgBye)
            if (next_focus > 0 and getDistanceToCreature(next_focus) < 4) then
                selfSay('Greetings, high roller. So you care for a game,  ' .. creatureGetName(next_focus) .. '?')
                talk_start = os.clock()
                focus = next_focus
                next_focus = 0
                inConvo = 1
            else
                focus = 0
                talk_start = 0
                inConvo = 0
            end
        end
    end
end