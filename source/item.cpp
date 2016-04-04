//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Item represents an existing item.
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

// include header file

#include "definitions.h"
#include "container.h"
#include "magic.h"
#include "player.h"
#include "tile.h"
#include "actions.h"

#include <iostream>
#include <sstream>
#include <iomanip>


Item* Item::CreateItem(const unsigned short _type, unsigned short _count /*= 0*/)
{
	Item *newItem;
	if(items[_type].isContainer()){
		newItem = new Container(_type);
	}
	else if(items[_type].isTeleport()){
		newItem = new Teleport(_type);
	}
	else if(items[_type].isMagicField()){
		newItem =  new Item(_type, _count);
	}
	else{
		newItem =  new Item(_type, _count);
	}
	newItem->isRemoved = false;
	newItem->useThing();
	return newItem;
}

//////////////////////////////////////////////////
// returns the ID of this item's ItemType
unsigned short Item::getID() const {
	return id;
}

//////////////////////////////////////////////////
// sets the ID of this item's ItemType
void Item::setID(unsigned short newid) {
	id = newid;
}

//////////////////////////////////////////////////
// return how many items are stacked or 0 if non stackable
unsigned short Item::getItemCountOrSubtype() const {
	if(isStackable()) {
		return count;
	}
	else if(isFluidContainer() || isSplash())
		return fluid;
	//else if(chargecount != 0)
	else if(items[id].runeMagLevel != -1)
		return chargecount;
	else
		return 0;
}

void Item::setItemCountOrSubtype(unsigned char n)
{
	if(isStackable()){
		/*if(n == 0){
			count = 1;
		}*/
		if(n > 100){
			count = 100;
		}
		else{
			count = n;
		}
	}
	else if(isFluidContainer() || isSplash())
		fluid = n;
	else if(items[id].runeMagLevel != -1)
		chargecount = n;
};

void Item::setActionId(unsigned short n){
	 if(n < 100)
	 	n = 100;
	actionId = n;
}

unsigned short Item::getActionId() const{
	return actionId;
}

void Item::setUniqueId(unsigned short n){
	//uniqueId only can be set 1 time
	if(uniqueId != 0)
		return;
	 if(n < 1000)
	 	n = 1000;
	uniqueId = n;
	ActionScript::AddThingToMapUnique(this);
}

unsigned short Item::getUniqueId() const{
	return uniqueId;
}

Item::Item(const unsigned short _type) {
	//std::cout << "Item constructor1 " << this << std::endl;
	id = _type;
	count = 0;
	chargecount = 0;
	fluid = 0;
	actionId = 0;
	uniqueId = 0;
	throwRange = 6;
	useCount = 0;
	isDecaying  = 0;
	specialDescription = NULL;
	text = NULL;

#ifdef YUR_RINGS_AMULETS
	const ItemType& it = items[id];
	time = it.newTime;
	charges = it.newCharges;
#endif //YUR_RINGS_AMULETS
#ifdef YUR_READABLES
	readable = NULL;
#endif //YUR_READABLES
#ifdef YUR_CLEAN_MAP
	decoration = false;
#endif //YUR_CLEAN_MAP
}

Item::Item(const Item &i){
	//std::cout << "Item copy constructor " << this << std::endl;
	id = i.id;
	count = i.count;
	chargecount = i.chargecount;
	throwRange = i.throwRange;
	useCount = 0;
	isDecaying  = 0;
	actionId = i.actionId;
	uniqueId = i.uniqueId;
	if(i.specialDescription != NULL){
		specialDescription = new std::string(*(i.specialDescription));
	}
	else{
		specialDescription = NULL;
	}
	if(i.text != NULL){
		text = new std::string(*(i.text));
	}
	else{
		text = NULL;
	}

#ifdef YUR_RINGS_AMULETS
	time = i.time;
	charges = i.charges;
#endif //YUR_RINGS_AMULETS
#ifdef YUR_READABLES
	if (i.readable)
		readable = new std::string(*(i.readable));
	else
		readable = NULL;
#endif //YUR_READABLES
#ifdef YUR_CLEAN_MAP
	decoration = i.decoration;
#endif //YUR_CLEAN_MAP
}

Item* Item::decay()
{
	unsigned short decayTo   = Item::items[getID()].decayTo;

	if(decayTo == 0) {
		return NULL;
	}

	if(dynamic_cast<Container*>(this)){
		if(items[decayTo].isContainer()){
			//container -> container
			setID(decayTo);
			return this;
		}
		else{
			//container -> no container
			Item *item = Item::CreateItem(decayTo,getItemCountOrSubtype());
			item->pos = this->pos;
			return item;
		}
	}
	else{
		if(items[decayTo].isContainer()){
			//no container -> container
			Item *item = Item::CreateItem(decayTo,getItemCountOrSubtype());
			item->pos = this->pos;
			return item;
		}
		else{
			//no contaier -> no container
			setID(decayTo);
			return this;
		}
	}
}

long Item::getDecayTime(){
	return items[id].decayTime*1000;
}

bool Item::rotate()
{
	if(items[id].rotable && items[id].rotateTo){
		id = items[id].rotateTo;
		return true;
	}
	return false;
}

Item::Item(const unsigned short _type, unsigned short _count) {
	//std::cout << "Item constructor2 " << this << std::endl;
	id = _type;
	count = 0;
	chargecount = 0;
	fluid = 0;
	actionId = 0;
	uniqueId = 0;
	useCount = 0;
	isDecaying  = 0;
	specialDescription = NULL;
	text = NULL;
	setItemCountOrSubtype((unsigned char)_count);
	/*
	if(isStackable()){
		if(_count == 0){
			count = 1;
		}
		else if(_count > 100){
			count = 100;
		}
		else{
			count = _count;
		}
	}
	else if(isFluidContainer() || isMultiType() )
		fluid = _count;
	else
		chargecount = _count;
	*/
	throwRange = 6;

#ifdef YUR_RINGS_AMULETS
	const ItemType& it = items[id];
	time = it.newTime;
	charges = it.newCharges;
#endif //YUR_RINGS_AMULETS
#ifdef YUR_READABLES
	readable = NULL;
#endif //YUR_READABLES
#ifdef YUR_CLEAN_MAP
	decoration = false;
#endif //YUR_CLEAN_MAP
}

Item::Item()
{
	//std::cout << "Item constructor3 " << this << std::endl;
	id = 0;
	count = 0;
	chargecount = 0;
	throwRange = 6;
	useCount = 0;
	isDecaying  = 0;
	actionId = 0;
	uniqueId = 0;
	specialDescription = NULL;
	text = NULL;

#ifdef YUR_RINGS_AMULETS
	time = 0;
	charges = 0;
#endif //YUR_RINGS_AMULETS
#ifdef YUR_READABLES
	readable = NULL;
#endif //YUR_READABLES
#ifdef YUR_CLEAN_MAP
	decoration = false;
#endif //YUR_CLEAN_MAP
}

Item::~Item()
{
	//std::cout << "Item destructor " << this << std::endl;
	if(specialDescription)
		delete specialDescription;
	if(text)
		delete text;

#ifdef YUR_READABLES
	if (readable)
		delete readable;
#endif //YUR_READABLES
}

bool Item::canMovedTo(const Tile *tile) const
{
	if(tile) {
		int objectstate = 0;

		if(isPickupable() || !isNotMoveable()) {
			objectstate |= BLOCK_PICKUPABLE;
		}

		if(isBlocking()) {
			objectstate |= BLOCK_SOLID;
		}

		return (tile->isBlocking(objectstate) == RET_NOERROR);
	}

	return false;
}

int Item::unserialize(xmlNodePtr p){
	char *tmp;
	tmp = (char*)xmlGetProp(p, (const xmlChar *) "id");
	if(tmp){
		id = atoi(tmp);
		xmlFreeOTSERV(tmp);
	}

	tmp = (char*)xmlGetProp(p, (const xmlChar *) "special_description");
	if(tmp){
		specialDescription = new std::string(tmp);
		xmlFreeOTSERV(tmp);
	}

	tmp = (char*)xmlGetProp(p, (const xmlChar *) "text");
	if(tmp){
		text = new std::string(tmp);
		xmlFreeOTSERV(tmp);
	}

	tmp = (char*)xmlGetProp(p, (const xmlChar *) "count");
	if(tmp){
		setItemCountOrSubtype(atoi(tmp));
		xmlFreeOTSERV(tmp);
	}

	tmp = (char*)xmlGetProp(p, (const xmlChar *) "actionId");
	if(tmp){
		setActionId(atoi(tmp));
		xmlFreeOTSERV(tmp);
	}

	tmp = (char*)xmlGetProp(p, (const xmlChar *) "uniqueId");
	if(tmp){
		setUniqueId(atoi(tmp));
		xmlFreeOTSERV(tmp);
	}

#ifdef YUR_RINGS_AMULETS
	tmp = (char*)xmlGetProp(p, (const xmlChar *) "charges");
	if(tmp){
		charges = atoi(tmp);
		xmlFreeOTSERV(tmp);
	}

	tmp = (char*)xmlGetProp(p, (const xmlChar *) "time");
	if(tmp){
		time = atoi(tmp);
		xmlFreeOTSERV(tmp);
	}
#endif //YUR_RINGS_AMULETS

	return 0;
}

xmlNodePtr Item::serialize(){
	std::stringstream s;
	xmlNodePtr ret;
	ret = xmlNewNode(NULL,(const xmlChar*)"item");
	s.str(""); //empty the stringstream
	s << getID();
	xmlSetProp(ret, (const xmlChar*)"id", (const xmlChar*)s.str().c_str());

	if(specialDescription){
		s.str(""); //empty the stringstream
		s << *specialDescription;
		xmlSetProp(ret, (const xmlChar*)"special_description", (const xmlChar*)s.str().c_str());
	}

	if(text){
		s.str(""); //empty the stringstream
		s << *text;
		xmlSetProp(ret, (const xmlChar*)"text", (const xmlChar*)s.str().c_str());
	}

	s.str(""); //empty the stringstream
	if(getItemCountOrSubtype() != 0){
		s << getItemCountOrSubtype();
		xmlSetProp(ret, (const xmlChar*)"count", (const xmlChar*)s.str().c_str());
	}

	s.str("");
	if(actionId != 0){
		s << actionId;
		xmlSetProp(ret, (const xmlChar*)"actionId", (const xmlChar*)s.str().c_str());
	}

	s.str("");
	if(uniqueId != 0){
		s << uniqueId;
		xmlSetProp(ret, (const xmlChar*)"uniqueId", (const xmlChar*)s.str().c_str());
	}

#ifdef YUR_RINGS_AMULETS
	s.str("");
	if(charges != 0){
		s << charges;
		xmlSetProp(ret, (const xmlChar*)"charges", (const xmlChar*)s.str().c_str());
	}

	s.str("");
	if (time != 0){
		s << time;
		xmlSetProp(ret, (const xmlChar*)"time", (const xmlChar*)s.str().c_str());
	}
#endif //YUR_RINGS_AMULETS

	return ret;
}

bool Item::isBlocking() const {
	const ItemType& it = items[id];
	return it.blockSolid;
}

bool Item::isStackable() const {
	return items[id].stackable;
}

/*
bool Item::isMultiType() const {
	//return items[id].multitype;
	return (items[id].group == ITEM_GROUP_SPLASH);
}
*/

bool Item::isFluidContainer() const {
	return (items[id].isFluidContainer());
}

bool Item::isAlwaysOnTop() const {
	return items[id].alwaysOnTop;
}

bool Item::isNotMoveable() const {
	return !items[id].moveable;
}

bool Item::isGroundTile() const {
	return items[id].isGroundTile();
}

bool Item::isSplash() const{
	return items[id].isSplash();
}

bool Item::isPickupable() const {
	return items[id].pickupable;
}

bool Item::isUseable() const{
	return items[id].useable;
}

bool Item::floorChangeDown() const {
	return items[id].floorChangeDown;
}

bool Item::floorChangeNorth() const {
	return items[id].floorChangeNorth;
}
bool Item::floorChangeSouth() const {
	return items[id].floorChangeSouth;
}
bool Item::floorChangeEast() const {
	return items[id].floorChangeEast;
}
bool Item::floorChangeWest() const {
	return items[id].floorChangeWest;
}

bool Item::isWeapon() const
{
  //now also returns true on SHIELDS!!! Check back with getWeponType!
  //old: return (items[id].weaponType != NONE && items[id].weaponType != SHIELD && items[id].weaponType != AMO);
  return (items[id].weaponType != NONE && items[id].weaponType != AMO);
}

WeaponType Item::getWeaponType() const {
	  return items[id].weaponType;
}

amu_t Item::getAmuType() const{
	 return items[id].amuType;
}

subfight_t Item::getSubfightType() const {
	return items[id].shootType;
}

int Item::getAttack() const {
	  return items[id].attack;
}

int Item::getArmor() const {
	  return items[id].armor;
}

int Item::getDefense() const {
	  return items[id].defence;
}

int Item::getSlotPosition() const {
	return items[id].slot_position;
}

double Item::getWeight() const {
	if(isStackable()){
		return items[id].weight * std::max(1, (int)count);
	}

	return items[id].weight;
}

std::string Item::getDescription(bool fullDescription) const
{
	std::stringstream s;
	std::string str;
	const Container* container;
	const ItemType& it = items[id];

	if(specialDescription){
		s << (*specialDescription) << ".";

		if(fullDescription) {
			if(it.weight > 0)
				s << std::endl << "It weighs " << std::fixed << std::setprecision(1) << it.weight << " oz.";
		}
	}
	else if (it.name.length()) {
		if(isStackable() && count > 1) {
			s << (int)count << " " << it.name << "s.";

			if(fullDescription) {
				s << std::endl << "They weight " << std::fixed << std::setprecision(1) << ((double) count * it.weight) << " oz.";
			}
		}
		else {
			if(items[id].runeMagLevel != -1)
			{
				s << "a spell rune for level " << it.runeMagLevel << "." << std::endl;

				s << "It's an \"" << it.name << "\" spell (";
				if(getItemCharge())
					s << (int)getItemCharge();
				else
					s << "1";
				s << "x)";
			}
			else if(isWeapon() && (getAttack() || getDefense()))
			{
				s << article(it.name) << " (Atk:" << (int)getAttack() << " Def:" << (int)getDefense() << ")";
			}
			else if(getArmor())
			{
				s << article(it.name) << " (Arm:"<< (int)getArmor() << ")";
			}
			else if(isFluidContainer()){
				s << article(it.name);
				if(fluid == 0){
					s << ". It is empty";
				}
				else{
					s << " of " << items[fluid].name;
				}
			}
			else if(isSplash()){
				s << article(it.name) << " of ";
				if(fluid == 0){
					s << items[1].name;
				}
				else{
					s << items[fluid].name;
				}
			}
			else if(it.isKey()){
				s << article(it.name) << " (Key:" << actionId << ")";
			}
			else if(it.isGroundTile()) //groundtile
			{
				s << it.name;
			}
#ifdef YUR_RINGS_AMULETS
			else if (charges)
			{
				s << article(it.name) << ". ";
				if (charges == 1)
					s << "\nIt has 1 charge left";
				else
					s << "\nIt has " << charges << " charges left";
			}
			else if (time)
			{
				s << article(it.name) << ". ";
				if (time < 60*1000)
					s << "\nIt has less than a minute left";
				else if (time == items[id].newTime)
					s << "\nIt is brand new";
				else
					s << "\nIt has " << (int)ceil(time/(60.0*1000.0)) << " minutes left";
			}
#endif //YUR_RINGS_AMULETS

			else if(it.isContainer() && (container = dynamic_cast<const Container*>(this))) {
				s << article(it.name) << " (Vol:" << container->capacity() << ")";
			}
			else {
				s << article(it.name);

#ifdef YUR_READABLES
				if (readable)
				{
					if (readable->empty())
						s << "\nNothing is written on it";
					else
						s << "\nYou read: " << *readable;
				}
#endif //YUR_READABLES
			}
			s << ".";
			if(fullDescription) {
				double weight = getWeight();
				if(weight > 0)
					s << std::endl << "It weighs " << std::fixed << std::setprecision(1) << weight << " oz.";

				if(items[id].description.length())
				{
					s << std::endl << items[id].description;
				}
			}
		}
	}
	else
		s<<"an item of type " << id <<".";

	str = s.str();
	return str;
}

std::string Item::getName() const
{
	return items[id].name;
}

void Item::setSpecialDescription(std::string desc){
	if(specialDescription){
		delete specialDescription;
		specialDescription = NULL;
	}
	if(desc.length() > 1)
		specialDescription = new std::string(desc);
}

std::string Item::getSpecialDescription()
{
	if(!specialDescription)
		return std::string("");
	return *specialDescription;
}

void Item::clearSpecialDescription(){
	if(specialDescription)
		delete specialDescription;
	specialDescription = NULL;
}

void Item::setText(std::string desc){
	if(text){
		delete text;
		text = NULL;
	}
	if(desc.length() > 1){
		text = new std::string(desc);
		if(items[id].readOnlyId != 0){//write 1 time
			id = items[id].readOnlyId;
		}
	}
}

void Item::clearText(){
	if(text)
		delete text;
	text = NULL;
}

std::string Item::getText()
{
	if(!text)
		return std::string("");
	return *text;
}

int Item::getRWInfo() const {
	return items[id].RWInfo;
}

bool Item::canDecay(){
	if(isRemoved)
		return false;
	return items[id].canDecay;
}
//Teleport class
Teleport::Teleport(const unsigned short _type) : Item(_type)
{
	useCount = 0;
	destPos.x = 0;
	destPos.y = 0;
	destPos.z = 0;
}

Teleport::~Teleport()
{
}

int Teleport::unserialize(xmlNodePtr p)
{
	Item::unserialize(p);
	char *tmp = (char*)xmlGetProp(p, (const xmlChar *) "destx");
	if(tmp){
		destPos.x = atoi(tmp);
		xmlFreeOTSERV(tmp);
	}
	tmp = (char*)xmlGetProp(p, (const xmlChar *) "desty");
	if(tmp){
		destPos.y = atoi(tmp);
		xmlFreeOTSERV(tmp);
	}
	tmp = (char*)xmlGetProp(p, (const xmlChar *) "destz");
	if(tmp){
		destPos.z = atoi(tmp);
		xmlFreeOTSERV(tmp);
	}


	return 0;
}

xmlNodePtr Teleport::serialize()
{
	xmlNodePtr xmlptr = Item::serialize();

	std::stringstream s;

	s.str(""); //empty the stringstream
	s << (int) destPos.x;
	xmlSetProp(xmlptr, (const xmlChar*)"destx", (const xmlChar*)s.str().c_str());

	s.str(""); //empty the stringstream
	s << (int) destPos.y;
	xmlSetProp(xmlptr, (const xmlChar*)"desty", (const xmlChar*)s.str().c_str());

	s.str(""); //empty the stringstream
	s << (int)destPos.z;
	xmlSetProp(xmlptr, (const xmlChar*)"destz", (const xmlChar*)s.str().c_str());

	return xmlptr;
}

int Item::getWorth() const
{
	switch(getID()){
	case ITEM_COINS_GOLD:
		return getItemCountOrSubtype();
	case ITEM_COINS_PLATINUM:
		return getItemCountOrSubtype() * 100;
	case ITEM_COINS_CRYSTAL:
		return getItemCountOrSubtype() * 10000;
	default:
		return 0;
	}
}

#ifdef YUR_RINGS_AMULETS
void Item::setGlimmer()
{
	switch (getID())
	{
		case ITEM_TIME_RING: setID(ITEM_TIME_RING_IN_USE); break;
		case ITEM_RING_OF_HEALING: setID(ITEM_RING_OF_HEALING_IN_USE); break; //Ring of healing   
        case ITEM_LIFE_RING: setID(ITEM_LIFE_RING_IN_USE); break; //Life Ring 
		case ITEM_SWORD_RING: setID(ITEM_SWORD_RING_IN_USE); break;
		case ITEM_AXE_RING: setID(ITEM_AXE_RING_IN_USE); break;
		case ITEM_CLUB_RING: setID(ITEM_CLUB_RING_IN_USE); break;
		case ITEM_POWER_RING: setID(ITEM_POWER_RING_IN_USE); break;
		case ITEM_ENERGY_RING: setID(ITEM_ENERGY_RING_IN_USE); break;
		case ITEM_STEALTH_RING: setID(ITEM_STEALTH_RING_IN_USE); break;
	}
}

void Item::removeGlimmer()
{
	switch (getID())
	{
		case ITEM_TIME_RING_IN_USE: setID(ITEM_TIME_RING); break;
		case ITEM_SWORD_RING_IN_USE: setID(ITEM_SWORD_RING); break;
		case ITEM_AXE_RING_IN_USE: setID(ITEM_AXE_RING); break;
		case ITEM_CLUB_RING_IN_USE: setID(ITEM_CLUB_RING); break;
		case ITEM_POWER_RING_IN_USE: setID(ITEM_POWER_RING); break;
		case ITEM_ENERGY_RING_IN_USE: setID(ITEM_ENERGY_RING); break;
		case ITEM_STEALTH_RING_IN_USE: setID(ITEM_STEALTH_RING); break;
		case ITEM_RING_OF_HEALING_IN_USE: setID(ITEM_RING_OF_HEALING); break; //Ring of healing    
        case ITEM_LIFE_RING_IN_USE: setID(ITEM_LIFE_RING); break; //Life Ring
	}
}
#endif //YUR_RINGS_AMULETS]

#ifdef TLM_BEDS
bool Item::isBed() 
{ 
if(id == 1754 || id == 1755 || id == 1756 || id == 1757 || id == 1758 || id == 1759 || id == 1760 || id == 1761 || id == 1762 || id == 1763 || id == 1764 || id == 1765 || id == 1766 || id == 1767 || id == 1768 || id == 1769) 
return true; 
else return false; 
}
bool Item::isBed2() 
{ 
if(id == 1762 || id == 1763 || id == 1764 || id == 1765 || id == 1766 || id == 1767 || id == 1768 || id == 1769) 
return true; 
else return false; 
}
#endif //TLM_BEDS
