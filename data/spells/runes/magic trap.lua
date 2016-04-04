area = {
{1, 1, 1},
{1, 0, 1},
{1, 1, 1}
}

attackType = ATTACK_NONE
needDirection = false
areaEffect = NM_ME_POISON_AREA
animationEffect = 14

hitEffect = NM_ME_POISEN
damageEffect = NM_ME_POISEN_RINGS
animationColor = 19
offensive = true
drawblood = false
minDmg = 5
maxDmg = 5

PoisonBombObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
SubPoisonBombObject1 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood,

minDmg, maxDmg)
SubPoisonBombObject2 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 4, 4)
SubPoisonBombObject3 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 3, 3)
SubPoisonBombObject4 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 2, 2)
SubPoisonBombObject5 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 1, 1)

function onCast(cid, creaturePos, level, maglv, var)
centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}

return doAreaGroundMagic(cid, centerpos, needDirection, areaEffect, area, PoisonBombObject:ordered(),
0, 1, SubPoisonBombObject1:ordered(),
2000, 2, SubPoisonBombObject2:ordered(),
2000, 3, SubPoisonBombObject3:ordered(),
2000, 4, SubPoisonBombObject4:ordered(),
2000, 5, SubPoisonBombObject5:ordered(),
5, 60000, 1497,
1000, 6, SubPoisonBombObject2:ordered(),
1, 60000, 1497,
0, 25000, 1497, 3)

end