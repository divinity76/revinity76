-- Stones of Citizenship by Talaturen
function onUse(cid, item, frompos, item2, topos)
valantispos = {x=422, y=728, z=8} 	-- temple respawn 1st city
thakinepos = {x=94, y=501, z=8} 	-- temple respawn 2nd city
ztrahelpos = {x=405, y=881, z=7}   	-- temple respawn 3rd city
camelotpos = {x=724, y=588, z=7} 	-- temple respawn 4th city
playerpos = getPlayerPosition(cid)
if item.uid == 8000 then				-- 1st city fountain uniqueid
doSendMagicEffect(playerpos,12)
doPlayerSetMasterPos(cid,valantispos)
doPlayerSendTextMessage(cid,22,"You have been blessed and now you are a citizen of Valantis.")
end
if item.uid == 8001 then				-- 2nd city fountain uniqueid
doSendMagicEffect(playerpos,12)
doPlayerSetMasterPos(cid,thakinepos)
doPlayerSendTextMessage(cid,22,"You have been blessed and now you are a citizen of Thakine.")
end
if item.uid == 8002 then				-- 3rd city fountain uniqueid
doSendMagicEffect(playerpos,12)
doPlayerSetMasterPos(cid,ztrahelpos)
doPlayerSendTextMessage(cid,22,"You have been blessed and now you are a citizen of Ztrahel.")
end
if item.uid == 8003 then				-- 4th city fountain uniqueid
doSendMagicEffect(playerpos,12)
doPlayerSetMasterPos(cid,camelotpos)
doPlayerSendTextMessage(cid,22,"You have been blessed and now you are a citizen of Camelot.")
end
return 1
end