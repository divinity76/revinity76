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


#ifndef __OTSERV_ITEM_H
#define __OTSERV_ITEM_H

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <iostream>
#include <list>
#include <vector>

#include "thing.h"
#include "items.h"

class Creature;
class Player;

class Item : public Thing
{
protected:
	unsigned short id;  // the same id as in ItemType
	unsigned char count; // number of stacked items
	unsigned char chargecount; //number of charges on the item
	unsigned char fluid;
	unsigned short actionId;
	unsigned short uniqueId;
	std::string *specialDescription;
	std::string *text;	//text written

#ifdef YUR_RINGS_AMULETS
	int time;
	int charges;
#endif //YUR_RINGS_AMULETS
#ifdef YUR_READABLES
	std::string *readable;
#endif //YUR_READABLES

private:
	int useCount;

public:
	static Item* CreateItem(const unsigned short _type, unsigned short _count = 0); //Factory member to create item of right type based on type
	static Items items;

	unsigned short getID() const;    // ID as in ItemType
	void setID(unsigned short newid);

	WeaponType getWeaponType() const;
	amu_t	getAmuType() const;
	subfight_t getSubfightType() const;
	virtual double getWeight() const;
	int getAttack() const;
	int getArmor() const;
	int getDefense() const;
	int getSlotPosition() const;
	int getRWInfo() const;
	int getWorth() const;

	bool isBlocking() const;
	bool isStackable() const;
	bool isFluidContainer() const;
	//bool isMultiType() const;
	bool isAlwaysOnTop() const;
	bool isGroundTile() const;
	bool isSplash() const;
	bool isNotMoveable() const;
	bool isPickupable() const;
	bool isWeapon() const;
	bool isUseable() const;

	bool floorChangeDown() const;
	bool floorChangeNorth() const;
	bool floorChangeSouth() const;
	bool floorChangeEast() const;
	bool floorChangeWest() const;

#ifdef YUR_RINGS_AMULETS
	 int getCharges() const { return charges; }
	 int getTime() const { return time; }
	 void useCharge() { --charges; }
	 void useTime(int thinkTicks) { time -= thinkTicks; }
	 void setGlimmer();
	 void removeGlimmer();
#endif //YUR_RINGS_AMULETS
#ifdef YUR_READABLES
	 void setReadable(const std::string& text) { readable = new std::string(text); }
#endif //YUR_READABLES
#ifdef TP_TRASH_BINS
	 bool isDeleter() const { return items[id].isDeleter; }
#endif //TP_TRASH_BINS
#ifdef YUR_CLEAN_MAP
	 bool decoration;
#endif //YUR_CLEAN_MAP

	virtual std::string getDescription(bool fullDescription) const;
	std::string getName() const ;
	void setSpecialDescription(std::string desc);
	std::string getSpecialDescription();
	void clearSpecialDescription();
	void setText(std::string desc);
	void clearText();
	std::string getText();

	virtual int unserialize(xmlNodePtr p);
	virtual xmlNodePtr serialize();

  // get the number of items
	unsigned short getItemCountOrSubtype() const;
	void setItemCountOrSubtype(unsigned char n);

	unsigned char getItemCharge() const {return chargecount;};
	void setItemCharge(unsigned char n) {chargecount = n;};

	unsigned char getFluidType() const {return fluid;};
	void setFluidType(unsigned char n) {fluid = n;};

	void setActionId(unsigned short n);
	unsigned short getActionId() const;

	void setUniqueId(unsigned short n);
	unsigned short getUniqueId() const;

	virtual long getDecayTime();
	bool canDecay();

	/**
	 * Called when the item is about to decay/transform to the next step.
	 * \returns The item to decay to.
	 */
	virtual Item* decay();
	bool isDecaying;
#ifdef TLM_BEDS
    bool isBed();
    bool isBed2();
#endif //TLM_BEDS
	bool rotate();

  // Constructor for items
	Item(const unsigned short _type);
	Item(const unsigned short _type, unsigned short _count);
	Item();
	Item(const Item &i);

	virtual ~Item();
	virtual void useThing() {
		//std::cout << "Item: useThing() " << this << std::endl;
		useCount++;
	};

	virtual void releaseThing() {
		useCount--;
		//std::cout << "Item: releaseThing() " << this << std::endl;
		//if (useCount == 0)
		if (useCount <= 0)
			delete this;
	};

	virtual bool canMovedTo(const Tile *tile) const;
};

class Teleport : public Item
{
public:
	Teleport(const unsigned short _type);
	virtual ~Teleport();
	virtual void useThing() {
		//std::cout << "Teleport: useThing() " << this << std::endl;
		useCount++;
	};

	virtual void releaseThing() {
		useCount--;
		//std::cout << "Teleport: releaseThing() " << this << std::endl;
		//if (useCount == 0)
		if (useCount <= 0)
			delete this;
	};

	void setDestPos(const Position &pos) {destPos = pos;};
	const Position& getDestPos() const {return destPos;};
private:
	int useCount;
	virtual int unserialize(xmlNodePtr p);
	virtual xmlNodePtr serialize();
	Position destPos;
};

#endif
