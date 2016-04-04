function onUse(cid, item, frompos, item2, topos)


if item2.itemid == 4608 or item2.itemid == 4644 or item2.itemid == 4645 or item2.itemid == 4646 or item2.itemid == 4647 or item2.itemid == 4653 or item2.itemid == 4649 or item2.itemid == 4609 or item2.itemid == 4610 or item2.itemid == 4611 or item2.itemid == 4612 or item2.itemid == 4613 or item2.itemid == 4614 or item2.itemid == 4615 or item2.itemid == 4616 or item2.itemid == 4617 or item2.itemid == 4618 or item2.itemid == 4619 or item2.itemid == 4620 or item2.itemid == 4621 or item2.itemid == 4622 or item2.itemid == 4623 or item2.itemid == 4624 or item2.itemid == 4625 or item2.itemid == 4648 or item2.itemid == 4652 or item2.itemid == 4756 or item2.itemid == 103 or item2.itemid == 724 or item2.itemid == 4542 or item2.itemid == 4543 or item2.itemid == 4544 or item2.itemid == 4545 or item2.itemid == 110 or item2.itemid == 351 or item2.itemid == 352 or item2.itemid == 353 or item2.itemid == 354 or item2.itemid == 355 or item2.itemid == 368 or item2.itemid == 412 or item2.itemid == 231 or item2.itemid == 232 or item2.itemid == 233 or item2.itemid == 234 or item2.itemid == 235 or item2.itemid == 236 or item2.itemid == 237 or item2.itemid == 238 or item2.itemid == 239 or item2.itemid == 240 or item2.itemid == 4632 or item2.itemid == 4633 or item2.itemid == 4634 or item2.itemid == 4635 or item2.itemid == 4636 or item2.itemid == 4637 or item2.itemid == 4638 or item2.itemid == 4639 or item2.itemid == 4640 or item2.itemid == 4641 or item2.itemid == 4642 or item2.itemid == 4643 or item2.itemid == 4650 or item2.itemid == 4649 or item2.itemid == 4651 or item2.itemid == 102 or item2.itemid == 3139 or item2.itemid == 3259 or item2.itemid == 3260 or item2.itemid == 3261 or item2.itemid == 3263 or item2.itemid == 3264 or item2.itemid == 3265 or item2.itemid == 3266 or item2.itemid == 3267 or item2.itemid == 3268 or item2.itemid == 3270 or item2.itemid == 3271 or item2.itemid == 3272 or item2.itemid == 3273 or item2.itemid == 3274 or item2.itemid == 3282 or item2.itemid == 3283 or item2.itemid == 3284 or item2.itemid == 3285 or item2.itemid == 3286 or item2.itemid == 3287 or item2.itemid == 3288 or item2.itemid == 3289 or item2.itemid == 3290 or item2.itemid == 3291 or item2.itemid == 3292 or item2.itemid == 3293 or item2.itemid == 3294 or item2.itemid == 3295 or item2.itemid == 3296 or item2.itemid == 3297 or item2.itemid == 3553 or item2.itemid == 3554 or item2.itemid == 3556 or item2.itemid == 3557 or item2.itemid == 424 or item2.itemid == 431 or item2.itemid == 457 or item2.itemid == 458 or item2.itemid == 407 or item2.itemid == 104 or item2.itemid == 106 or item2.itemid == 107 then

doTeleportThing(cid,topos)
doSendMagicEffect(topos,1)
doSendMagicEffect(frompos,1)
doPlayerSendTextMessage(cid,24,"Using rune of Water.")

else
doSendMagicEffect(frompos,2)
doPlayerSendCancel(cid,"You cannot use the rune here.")
end
return 1
end