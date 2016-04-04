--doTargetMagic
-- attackType: Type of attack.
-- cid: creature id.
-- Targetpos: Target position.
-- animationEffect: Projectile animation.
-- damageEffect: Effect to show when spell damage a creature.
-- hitEffect: Effect to show when spell hits a creature.
-- animationColor: Color of the text that is shown above the player when hit.
-- offensive: Indicates if the spell is a healing/attack spell.
-- drawblood: Determines if the spell causes blood splash.
-- minDmg: Minimal damage.
-- maxDmg: Maximum damage.
-- returns true if the spell was casted.

attackType = ATTACK_MANADRAIN
animationEffect = NM_ANI_SUDDENDEATH

hitEffect = NM_ME_MORT_AREA
damageEffect = NM_ME_LOOSE_ENERGY
animationColor = DARK_BLUE
offensive = true
drawblood = false

BeholderManaDrain = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)

function onCast(cid, creaturePos, level, maglv, var)
	centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}

	BeholderManaDrain.minDmg = 10
	BeholderManaDrain.maxDmg = 30

	return doTargetMagic(cid, centerpos, BeholderManaDrain:ordered())
end
