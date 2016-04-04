function MagicDamageObject(iattackType, ianimationEffect, ihitEffect, idamageEffect, ianimationColor, ioffensive, idrawblood, iminDmg, imaxDmg)
  local self = {
  attackType = iattackType,
  animationEffect = ianimationEffect,
  hitEffect = ihitEffect,
  damageEffect = idamageEffect,
  animationColor = ianimationColor,
  offensive = ioffensive,
  drawblood = idrawblood,
  minDmg = iminDmg,
  maxDmg = imaxDmg}

  function self:ordered()
    --print('ordered()')
    return {self.attackType, self.animationEffect, self.hitEffect, self.damageEffect, self.animationColor, self.offensive, self.drawblood, self.minDmg, self.maxDmg}
  end

return self
end

NM_ME_DRAW_BLOOD = 0
NM_ME_LOOSE_ENERGY = 1
NM_ME_PUFF = 2
NM_ME_BLOCKHIT = 3
NM_ME_EXPLOSION_AREA = 4
NM_ME_EXPLOSION_DAMAGE = 5
NM_ME_FIRE_AREA = 6
NM_ME_YELLOW_RINGS = 7
NM_ME_POISEN_RINGS = 8
NM_ME_HIT_AREA = 9
NM_ME_ENERGY_AREA = 10
NM_ME_ENERGY_DAMAGE = 11

-- only for internal use, dont send to client
NM_ME_NONE = 255

NM_ME_MAGIC_ENERGIE = 12
NM_ME_MAGIC_BLOOD = 13
NM_ME_MAGIC_POISEN = 14
NM_ME_HITBY_FIRE = 15
NM_ME_POISEN = 16
NM_ME_MORT_AREA = 17
NM_ME_SOUND = 18
NM_ME_SOUND_GREEN = 18
NM_ME_SOUND_RED = 19
NM_ME_POISONCLOUD = 20
NM_ME_SOUND_YELLOW = 21
NM_ME_SOUND_PURPLE = 22
NM_ME_SOUND_BLUE = 23
NM_ME_SOUND_WHITE = 24

NM_ANI_BOLT = 1
NM_ANI_ARROW = 2
NM_ANI_FIRE = 3
NM_ANI_ENERGY = 4
NM_ANI_POISONARROW = 5
NM_ANI_BURSTARROW = 6
NM_ANI_THROWINGSTAR = 7
NM_ANI_THROWINGKNIFE = 8
NM_ANI_SMALLSTONE = 9
NM_ANI_SUDDENDEATH = 10
NM_ANI_LARGEROCK = 11
NM_ANI_SNOWBALL = 12
NM_ANI_POWERBOLT = 13

-- only for internal use, dont send to client
NM_ANI_NONE = 0

GREEN	    = 30
RED       = 180
DARK_BLUE = 2
LIGHT_BLUE = 71
BLACK_WHITE = 86
ORANGE = 193
ENERGY    = 35
FIRE      = 198
GREY      = 86
BLUE      = -246
PURPLE    = 190
YELLOW    = 79
BLACK     = 255

ATTACK_NONE      = 0
ATTACK_ENERGY    = 1
ATTACK_BURST     = 2
ATTACK_FIRE      = 3
ATTACK_PHYSICAL  = 4
ATTACK_POISON    = 5
ATTACK_PARALYZE  = 6
ATTACK_DRUNKNESS = 7
ATTACK_LIFEDRAIN = 8
ATTACK_MANADRAIN = 9

outfits = {["Orc Warlord"] = 2, ["War Wolf"] = 3, ["Orc Ridder"] = 4, ["Orc"] = 5, ["Orc Shaman"] =6, ["Orc Warrior"] = 7, ["Orc Berserker"] = 8, ["Necromancer"] = 9, ["Black Sheep"] = 13, ["Sheep"] = 14, ["Troll"] = 15, ["Bear"] = 16, ["Beholder"] = 17, ["Ghoul"] = 18, ["Slime"] = 19, ["Rat"]= 21, ["Cyclops"] = 22, ["Minotaur Mage"] = 23, ["Minotaur Archer"] = 24, ["Minotaur"] = 25, ["Rotworm"] = 26, ["Wolf"] = 27, ["Snake"] = 28, ["Minotaur Guard"] =29, ["Spider"] = 30, ["Deer"] = 31, ["Dog"] = 32, ["Skeleton"] = 33, ["Dragon"] = 34, ["Poison Spider"] = 36, ["Demon Skeleton"] = 37, ["Giant Spider"] = 38, ["Dragon Lord"] = 39, ["Fire Devil"] = 40, ["Lion"] = 41, ["Polar Bear"] = 42, ["Scorpion"] = 43, ["Wasp"]= 44, ["Bug"] = 45, ["Ghost"] = 48, ["Fire Elemental"] = 49, ["Orc Spearman"] = 50, ["Djinn"] =51, ["Winter Wolf"] = 52, ["Frost Troll"]=53, ["Witch"] = 54, ["Behemoth"] =55, ["Cave Rat"] = 56, ["Monk"] = 57, ["Priestess"] = 58, ["Orc Leader"] = 59, ["Pig"] = 60, ["Goblin"] = 61, ["Elf"] = 62, ["Elf Arcanist"] = 63,  ["Elf Scout"] = 64, ["Mummy"] = 65, ["Dwarf Geomancer"] = 66, ["Stone Golem"] = 67, ["Vampire"] = 68,  ["Dwarf"] = 69, ["Dwarf Guard"] = 70, ["Dwarf Soldier"] = 71, ["Rabbit"] = 74, ["Swamp Troll"] = 76}
