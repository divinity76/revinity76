area = {
{0, 0, 0},
{0, 1, 0},
{0, 0, 0}
}
attackType = ATTACK_FIRE
needDirection = false
areaEffect = NM_ME_EXPLOSION_AREA
animationEffect = NM_ANI_FIRE

hitEffect = NM_ME_EXPLOSION_DAMAGE
damageEffect = NM_ME_DRAW_BLOOD
animationColor = RED
offensive = true
drawblood = true


HealFriendObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)

function onCast(cid, creaturePos, level, maglv, var)
centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z} targetpos = getPosition(var)
if (level * 2 + maglv * 3) >= 100 then
HealFriendObject.minDmg = (level * 2 + maglv * 3) * 0.906
HealFriendObject.maxDmg = (level * 2 + maglv * 3) * 1.267
elseif (level * 2 + maglv * 3) < 100 then
HealFriendObject.minDmg  = 100 * 0.906
HealFriendObject.maxDmg = 100 * 1.267
end

if targetpos.x ~= nil and targetpos.z ~= nil and targetpos.y ~= nil then
	if math.abs(targetpos.x - centerpos.x) < 8 and math.abs(targetpos.y - centerpos.y) < 6 and targetpos.z == centerpos.z then
		return doTargetMagic(cid, targetpos, HealFriendObject:ordered())
	end
end

return false
end  