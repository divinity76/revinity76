//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// class representing the gamestate
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


#ifndef __OTSERV_GAME_H
#define __OTSERV_GAME_H


#include <queue>
#include <vector>
#include <set>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "spawn.h"
#include "position.h"
#include "item.h"
#include "container.h"
#include "magic.h"
#include "map.h"
#include "templates.h"
#include "loginqueue.h"

class Creature;   // see creature.h
class Player;
class Monster;
class Npc;
class Commands;
class SchedulerTask;
class lessSchedTask;

#define MAP_WIDTH    512
#define MAP_HEIGHT   512
#define MAP_LAYER     16

#ifdef CVS_DAY_CYCLE
enum eLightState{
	LIGHT_STATE_DAY,
	LIGHT_STATE_NIGHT,
	LIGHT_STATE_SUNSET,
	LIGHT_STATE_SUNRISE,
};
#endif //CVS_DAY_CYCLE

/** State of a creature at a given time
  * Keeps track of all the changes to be able to send to the client
	*/

class CreatureState {
public:
	CreatureState() {};
	~CreatureState() {};

	int damage;
	int manaDamage;
	bool drawBlood;
	std::vector<Creature*> attackerlist;
};

typedef std::vector< std::pair<Creature*, CreatureState> > CreatureStateVec;
typedef std::map<Tile*, CreatureStateVec> CreatureStates;

/** State of the game at a given time
  * Keeps track of all the changes to be able to send to the client
	*/

class Game;

class GameState {
public:
	GameState(Game *game, const Range &range);
	~GameState() {};

	void onAttack(Creature* attacker, const Position& pos, const MagicEffectClass* me);
	void onAttack(Creature* attacker, const Position& pos, Creature* attackedCreature);
	const CreatureStateVec& getCreatureStateList(Tile* tile) {return creaturestates[tile];};
	const SpectatorVec& getSpectators() {return spectatorlist;}

protected:
	void addCreatureState(Tile* tile, Creature* attackedCreature, int damage, int manaDamage, bool drawBlood);
	void onAttackedCreature(Tile* tile, Creature* attacker, Creature* attackedCreature, int damage, bool drawBlood);
	Game *game;

#ifdef YUR_PVP_ARENA
	bool isPvpArena(Creature* c);
#endif //YUR_PVP_ARENA
#ifdef YUR_RINGS_AMULETS
	int applyAmulets(Player* player, int damage, attacktype_t atype);
#endif //YUR_RINGS_AMULETS

	SpectatorVec spectatorlist;
	CreatureStates creaturestates;
};

enum enum_world_type{
	WORLD_TYPE_NO_PVP,
	WORLD_TYPE_PVP,
	WORLD_TYPE_PVP_ENFORCED
};

enum enum_game_state{
	GAME_STATE_NORMAL,
	GAME_STATE_CLOSED,
	GAME_STATE_SHUTDOWN
};

/**
  * Main Game class.
  * This class is responsible to controll everything that happens
  */

class Game {
public:
	Game();
  ~Game();
bool setSpellbookText(Player* player, Item* item);
#ifdef TLM_BEDS
	    std::string getBedSleeper(const Position pos);
          unsigned int getBedID(const Position pos);
          Position getBedPos(std::string name);
  
          bool changeBed(const Position pos, unsigned int oldid, std::string sleepname);
          bool loadBeds(std::string file);
#endif //TLM BEDS
void sendMagicEffect(const Position pos, unsigned char type);
	/**
	  * Load a map.
	  * \param filename Mapfile to load
	  * \param filekind Kind of the map, BIN SQL or TXT
	  * \returns Int 0 built-in spawns, 1 needs xml spawns, 2 needs sql spawns, -1 if got error
	  */
    int loadMap(std::string filename, std::string filekind);
	
	/**
	  * Get the map size - info purpose only
	  * \param a the referenced witdh var
	  * \param b the referenced height var
	  */
	void getMapDimensions(int& a, int& b) {
     a = map->mapwidth;  
     b = map->mapheight;  
     return;
  }
  
	void setWorldType(enum_world_type type);
  enum_world_type getWorldType() const {return worldType;}
	const std::string& getSpawnFile() {return map->spawnfile;}

	/**
	  * Get a single tile of the map.
	  * \returns A Pointer to the tile */
	Tile* getTile(unsigned short _x, unsigned short _y, unsigned char _z);
	Tile* getTile(const Position& pos);

	/**
	  * Set a Tile to a specific ground id
	  * \param groundId ID of the ground to set
	  */
	void setTile(unsigned short _x, unsigned short _y, unsigned char _z, unsigned short groundId);

	/**
	  * Place Creature on the map.
	  * Adds the Creature to playersOnline and to the map
	  * \param c Creature to add
	  */
#ifdef YUR_LOGIN_QUEUE
	bool placeCreature(Position &pos, Creature* c, int* placeInQueue = NULL);
#else
	bool placeCreature(Position &pos, Creature* c);
#endif //YUR_LOGIN_QUEUE

	/**
		* Remove Creature from the map.
		* Removes the Creature the map
		* \param c Creature to remove
		*/
	bool removeCreature(Creature* c);

	uint32_t getPlayersOnline();
	uint32_t getMonstersOnline();
	uint32_t getNpcsOnline();
	uint32_t getCreaturesOnline();

	void thingMove(Creature *creature, Thing *thing,
			unsigned short to_x, unsigned short to_y, unsigned char to_z, unsigned char count);

	//container/inventory to container/inventory
	void thingMove(Player *player,
			unsigned char from_cid, unsigned char from_slotid, unsigned short itemid,bool fromInventory,
			unsigned char to_cid, unsigned char to_slotid, bool toInventory,
			unsigned char count);

	//container/inventory to ground
	void thingMove(Player *player,
			unsigned char from_cid, unsigned char from_slotid, unsigned short itemid, bool fromInventory,
			const Position& toPos, unsigned char count);

	//ground to container/inventory
	void thingMove(Player *player,
			const Position& fromPos, unsigned char stackPos, unsigned short itemid,
			unsigned char to_cid, unsigned char to_slotid,
			bool isInventory, unsigned char count);
	
	//ground to ground
	void thingMove(Creature *creature,
			unsigned short from_x, unsigned short from_y, unsigned char from_z,
			unsigned char stackPos,unsigned short itemid,
			unsigned short to_x, unsigned short to_y, unsigned char to_z, unsigned char count);

	/**
		* Creature wants to turn.
		* \param creature Creature pointer
		* \param dir Direction to turn to
		*/
	void creatureTurn(Creature *creature, Direction dir);
	

	/**
	  * Creature wants to say something.
	  * \param creature Creature pointer
	  * \param type Type of message
	  * \todo document types
	  * \param text The text to say
	  */
	void creatureSay(Creature *creature, SpeakClasses type, const std::string &text);

	void creatureWhisper(Creature *creature, const std::string &text);
	void creatureYell(Creature *creature, std::string &text);
  	void creatureSpeakTo(Creature *creature, SpeakClasses type, const std::string &receiver, const std::string &text);
	void creatureBroadcastMessage(Creature *creature, const std::string &text);
  	void creatureTalkToChannel(Player *player, SpeakClasses type, std::string &text, unsigned short channelId);
	void creatureMonsterYell(Monster* monster, const std::string& text);
	void creatureChangeOutfit(Creature *creature);

	bool creatureThrowRune(Creature *creature, const Position& centerpos, const MagicEffectClass& me);
	bool creatureCastSpell(Creature *creature, const Position& centerpos, const MagicEffectClass& me);
	bool creatureSaySpell(Creature *creature, const std::string &text);

	void playerAutoWalk(Player* player, std::list<Direction>& path);
	bool playerUseItemEx(Player *player, const Position& posFrom,const unsigned char  stack_from,
		const Position &posTo,const unsigned char stack_to, const unsigned short itemid);
	bool playerUseItem(Player *player, const Position& pos, const unsigned char stackpos, const unsigned short itemid, const unsigned char index);
	bool playerUseBattleWindow(Player *player, Position &posFrom, unsigned char stackpos, unsigned short itemid, unsigned long creatureid);
	bool playerRotateItem(Player *player, const Position& pos, const unsigned char stackpos, const unsigned short itemid);

	void playerRequestTrade(Player *player, const Position& pos,
		const unsigned char stackpos, const unsigned short itemid, unsigned long playerid);
	void playerAcceptTrade(Player* player);
	void playerLookInTrade(Player* player, bool lookAtCounterOffer, int index);
	void playerCloseTrade(Player* player);
	void autoCloseTrade(const Item* item, bool itemMoved = false);
  void autoCloseAttack(Player* player, Creature* target);
	
	void playerSetAttackedCreature(Player* player, unsigned long creatureid);

  void changeOutfitAfter(unsigned long id, int looktype, long time);
  void changeSpeed(unsigned long id, unsigned short speed);
	unsigned long addEvent(SchedulerTask*);
	bool stopEvent(unsigned long eventid);

	//void creatureBroadcastTileUpdated(const Position& pos);
	void teleport(Thing *thing, const Position& newPos);
      
      #ifdef LOOSK_RECORD
	    int record;
    bool saveRecord();
    void checkRecord();
    bool loadRecord();
	#endif //LOOSK_RECORD
	
  std::vector<Player*> BufferedPlayers;   
  void flushSendBuffers();
  void addPlayerBuffer(Player* p);
  
  std::vector<Thing*> ToReleaseThings;   
  void FreeThing(Thing* thing);

  Thing* getThing(const Position &pos,unsigned char stack,Player* player = NULL);
  void addThing(Player* player,const Position &pos,Thing* thing);
  bool removeThing(Player* player,const Position &pos,Thing* thing, bool setRemoved = true);
  Position getThingMapPos(Player *player, const Position &pos);
  
  void sendAddThing(Player* player,const Position &pos,const Thing* thing);
  void sendRemoveThing(Player* player,const Position &pos,const Thing* thing,const unsigned char stackpos = 1,const bool autoclose = false);
  void sendUpdateThing(Player* player,const Position &pos,const Thing* thing,const unsigned char stackpos = 1);
		
	Creature* getCreatureByID(unsigned long id);
	Player* getPlayerByID(unsigned long id);
    Creature* getCreatureByPosition(int x, int y, int z);  
	Creature* getCreatureByName(const std::string &s);
	Player* getPlayerByName(const std::string &s);

	std::list<Position> getPathTo(Creature *creature, Position start, Position to, bool creaturesBlock=true);
	
	enum_game_state getGameState();
	void setGameState(enum_game_state newstate){game_state = newstate;}
	
	bool requestAddVip(Player* player, const std::string &vip_name);

	/** Lockvar for Game. */
  OTSYS_THREAD_LOCKVAR gameLock; 

#ifdef CVS_DAY_CYCLE
	void creatureChangeLight(Player* player, int time, unsigned char lightlevel, unsigned char lightcolor);
	unsigned char getLightLevel();
#endif //CVS_DAY_CYCLE

#ifdef WOLV_LOAD_NPC
	bool loadNpcs();
#endif //WOLV_LOAD_NPC

#ifdef TLM_SERVER_SAVE
 void serverSave();
 void autoServerSave();
 bool playerSave();
 bool houseSave();
 bool guildSave();
 void autoPlayerSave();
 void autoHouseSave();
 void autoguildSave();
#endif //TLM_SERVER_SAVE

#ifdef ELEM_VIP_LIST
	void vipLogin(Player* player);
	void vipLogout(std::string vipname);
	bool isPlayer(std::string name);
#endif //ELEM_VIP_LIST
#ifdef BD_FOLLOW
   void checkCreatureFollow(unsigned long id);
   void playerFollow(Player* player, Creature *followCreature);
   void playerSetFollowCreature(Player* player, unsigned long creatureid);
#endif //BD_FOLLOW
#ifdef YUR_LOGIN_QUEUE
	LoginQueue loginQueue;
#endif //YUR_LOGIN_QUEUE

#ifdef TR_SUMMONS
	bool placeSummon(Player* p, const std::string& name);
#endif //TR_SUMMONS

#ifdef TRS_GM_INVISIBLE
	void creatureBroadcastTileUpdated(const Position& pos);
#endif //TRS_GM_INVISIBLE

#ifdef TLM_SKULLS_PARTY
	void onPvP(Creature* creature, Creature* attacked, bool murder = false);
	void Skull(Player* player);
	void disbandParty(unsigned long partyID);
	void LeaveParty(Player *player);
#endif //TLM_SKULLS_PARTY

#ifdef SD_BURST_ARROW
	void burstArrow(Creature* c, const Position& pos);
	static double BURST_DMG_LVL, BURST_DMG_MLVL, BURST_DMG_LO, BURST_DMG_HI;
#endif //SD_BURST_ARROW

#ifdef YUR_SHUTDOWN
	void sheduleShutdown(int minutes);
	void checkShutdown(int minutes);
#endif //YUR_SHUTDOWN

#ifdef YUR_CMD_EXT
	void setMaxPlayers(uint32_t newmax);
#endif //YUR_CMD_EXT

#ifdef YUR_CLEAN_MAP
	long cleanMap();
#endif //YUR_CLEAN_MAP

#ifdef JD_WANDS
	void useWand(Creature *creature, Creature *attackedCreature, int wandid);
#endif //JD_WANDS

	void checkSpell(Player* player, SpeakClasses type, std::string text);

protected:
	std::map<Item*, unsigned long> tradeItems; //list of items that are in trading state, mapped to the player
	
	AutoList<Creature> listCreature;

	/*ground -> ground*/
	bool onPrepareMoveThing(Creature *player, const Thing* thing,
		const Position& fromPos, const Position& toPos, int count);

	/*ground -> ground*/
	bool onPrepareMoveThing(Creature *creature, const Thing* thing,
		const Tile *fromTile, const Tile *toTile, int count);

	/*inventory -> container*/
	bool onPrepareMoveThing(Player *player, const Item* fromItem, slots_t fromSlot,
		const Container *toContainer, const Item *toItem, int count);

	/*container -> container*/
	bool onPrepareMoveThing(Player* player, const Position& fromPos, const Container* fromContainer,
		const Item* fromItem, const Position& toPos, const Container* toContainer, const Item* toItem,
		int count);

	/*ground -> ground*/
	bool onPrepareMoveCreature(Creature *creature, const Creature* creatureMoving,
		const Tile *fromTile, const Tile *toTile);

	/*ground -> inventory*/
	bool onPrepareMoveThing(Player *player, const Position& fromPos, const Item *item,
		slots_t toSlot, int count);

	/*inventory -> inventory*/
	bool onPrepareMoveThing(Player *player, slots_t fromSlot, const Item *fromItem,
		slots_t toSlot, const Item *toItem, int count);

	/*container -> inventory*/
	bool onPrepareMoveThing(Player *player, const Container *fromContainer, const Item *fromItem,
		slots_t toSlot, const Item *toItem, int count);

	/*->inventory*/
	bool onPrepareMoveThing(Player *player, const Item *item, slots_t toSlot, int count);

	//container/inventory to container/inventory
	void thingMoveInternal(Player *player,
			unsigned char from_cid, unsigned char from_slotid, unsigned short itemid, 
			bool fromInventory,unsigned char to_cid, unsigned char to_slotid, 
			bool toInventory,unsigned char count);

	//container/inventory to ground
	void thingMoveInternal(Player *player,
			unsigned char from_cid, unsigned char from_slotid, unsigned short itemid,
			bool fromInventory,const Position& toPos, unsigned char count);

	//ground to container/inventory
	void thingMoveInternal(Player *player,
			const Position& fromPos, unsigned char stackPos,unsigned short itemid,
			unsigned char to_cid, unsigned char to_slotid,
			bool toInventory, unsigned char count);

	// use this internal function to move things around to avoid the need of
  // recursive locks
  void thingMoveInternal(Creature *player,
      unsigned short from_x, unsigned short from_y, unsigned char from_z,
      unsigned char stackPos,unsigned short itemid,
			unsigned short to_x, unsigned short to_y, unsigned char to_z, unsigned char count);

	void changeOutfit(unsigned long id, int looktype);
	bool creatureOnPrepareAttack(Creature *creature, Position pos);
	void creatureMakeDamage(Creature *creature, Creature *attackedCreature, fight_t damagetype);

	bool creatureMakeMagic(Creature *creature, const Position& centerpos, const MagicEffectClass* me);
	bool creatureOnPrepareMagicAttack(Creature *creature, Position pos, const MagicEffectClass* me);

	/**
		* Change the players hitpoints
		* Return: the mana damage and the actual hitpoint loss
		*/
	void creatureApplyDamage(Creature *creature, int damage, int &outDamage, int &outManaDamage
#ifdef YUR_PVP_ARENA
		, CreatureVector*
#endif //YUR_PVP_ARENA
		);

	void CreateDamageUpdate(Creature* player, Creature* attackCreature, int damage);
	void CreateManaDamageUpdate(Creature* player, Creature* attackCreature, int damage);
	void getSpectators(const Range& range, SpectatorVec& list);

	OTSYS_THREAD_LOCKVAR eventLock;
	OTSYS_THREAD_SIGNALVAR eventSignal;

	static OTSYS_THREAD_RETURN eventThread(void *p);

#ifdef __DEBUG_CRITICALSECTION__
	static OTSYS_THREAD_RETURN monitorThread(void *p);
#endif

	struct GameEvent
	{
		__int64  tick;
		int      type;
		void*    data;
	};

	void checkPlayerWalk(unsigned long id);
	void checkCreature(unsigned long id);
	void checkCreatureAttacking(unsigned long id);
	void checkDecay(int t);
	
	#define DECAY_INTERVAL  10000
	void startDecay(Item* item);
	struct decayBlock{
		long decayTime;
		std::list<Item*> decayItems;
	};
	std::list<decayBlock*> decayVector;
	
#ifdef CVS_DAY_CYCLE
	static const unsigned char LIGHT_LEVEL_DAY = 220;
	static const unsigned char LIGHT_LEVEL_NIGHT = 25;
	static const int SUNSET = 1305;
	static const int SUNRISE = 430;
	unsigned char lightlevel;
	eLightState light_state;
	int light_hour;
	int light_hour_delta;
	//int lightdelta;
	void checkLight(int delta);
#endif //CVS_DAY_CYCLE

	void checkSpawns(int t);
	std::priority_queue<SchedulerTask*, std::vector<SchedulerTask*>, lessSchedTask > eventList;
	std::map<unsigned long, SchedulerTask*> eventIdMap;
	unsigned long eventIdCount;

	uint32_t max_players;
	enum_world_type worldType;

	Map* map;
	
	std::vector<std::string> commandTags;
	void addCommandTag(std::string tag);
	void resetCommandTag();
	
	enum_game_state game_state;

	friend class Commands;
	friend class Monster;
	friend class GameState;
	friend class Spawn;
	friend class SpawnManager;
	friend class ActionScript;
	friend class Actions;
};

template<class ArgType>
class TCallList : public SchedulerTask {
public:
	TCallList(boost::function<int(Game*, ArgType)> f1, boost::function<bool(Game*)> f2, std::list<ArgType>& call_list, __int64 interval) :
	_f1(f1), _f2(f2), _list(call_list), _interval(interval) {
	}
	
	void operator()(Game* arg) {
		if(_eventid != 0 && !_f2(arg)) {
			int ret = _f1(arg, _list.front());
			_list.pop_front();
			if (ret && !_list.empty()) {
				SchedulerTask* newTask = new TCallList(_f1, _f2, _list, _interval);
				newTask->setTicks(_interval);
				newTask->setEventId(this->getEventId());
				arg->addEvent(newTask);
			}
		}

		return;
	}

private:
	boost::function<int(Game*, ArgType)> _f1;
	boost::function<bool(Game*)>_f2;
	std::list<ArgType> _list;
	__int64 _interval;
};

template<class ArgType>
SchedulerTask* makeTask(__int64 ticks, boost::function<int(Game*, ArgType)> f1, std::list<ArgType>& call_list, __int64 interval, boost::function<bool(Game*)> f2) {
	TCallList<ArgType> *t = new TCallList<ArgType>(f1, f2, call_list, interval);
	t->setTicks(ticks);
	return t;
}

#endif
