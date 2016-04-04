//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Implementation of tibia v7.6 protocoll
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


#include "definitions.h"
#include "ioplayer.h"
#include <string>
#include <iostream>
#include <sstream>
#include <time.h>
#include <list>

#include "networkmessage.h"
#include "protocol76.h"

#include "items.h"

#include "tile.h"
#include "creature.h"
#include "player.h"
#include "status.h"
#include "chat.h"

#include <stdio.h>

#include "luascript.h"

#include "otsystem.h"
#include "actions.h"
#include "game.h"


extern LuaScript g_config;
extern Actions actions;
Chat g_chat;

Protocol76::Protocol76(SOCKET s)
{
	OTSYS_THREAD_LOCKVARINIT(bufferLock);
	windowTextID = 0;
	readItem = NULL;
	this->s = s;
}


Protocol76::~Protocol76()
{
	OTSYS_THREAD_LOCKVARRELEASE(bufferLock);	
}

void Protocol76::reinitializeProtocol()
{
	windowTextID = 0;
	readItem = NULL;
	OutputBuffer.Reset();
	knownPlayers.clear();
}

#ifdef YUR_LOGIN_QUEUE
bool Protocol76::ConnectPlayer(int* placeInQueue)
{
	return game->placeCreature(player->pos, player, placeInQueue);
}
#else //YUR_LOGIN_QUEUE
bool Protocol76::ConnectPlayer()
{	
	Status* stat = Status::instance();
	if(!stat->hasSlot() && player->access == 0)
		return false;
	else                    
		return game->placeCreature(player->pos, player);
}
#endif //YUR_LOGIN_QUEUE

void Protocol76::ReceiveLoop()
{
	NetworkMessage msg;
	do{
		while(pendingLogout == false && msg.ReadFromSocket(s)){
			parsePacket(msg);
		}
		
		if(s){
			closesocket(s);
			s = 0;
		}
		// logout by disconnect?  -> kick
		if(pendingLogout == false){
			game->playerSetAttackedCreature(player, 0);
			//player->setAttackedCreature(0);
			while(player->inFightTicks >= 1000 && player->isRemoved == false && s == 0){
				OTSYS_SLEEP(250);
			}
			OTSYS_THREAD_LOCK(game->gameLock, "Protocol76::ReceiveLoop()")
			if(s == 0 && player->isRemoved == false){
				game->removeCreature(player);
			}
			OTSYS_THREAD_UNLOCK(game->gameLock, "Protocol76::ReceiveLoop()")
		}
	}while(s != 0 && player->isRemoved == false);
}


void Protocol76::parsePacket(NetworkMessage &msg)
{
	if(msg.getMessageLength() <= 0)
		return;
	
	uint8_t recvbyte = msg.GetByte();
	//a dead player can not performs actions
	if (player->isRemoved == true && recvbyte != 0x14) {
		OTSYS_SLEEP(10);
		return;
	}	
    
	switch(recvbyte)
	{
    case 0x14: // logout
		parseLogout(msg);
		break;
		
	case 0x64: // client moving with steps
		parseMoveByMouse(msg);
		break;
		
    case 0x65: // move north
		parseMoveNorth(msg);
		break;
		
    case 0x66: // move east
		parseMoveEast(msg);
		break;
		
    case 0x67: // move south
		parseMoveSouth(msg);
		break;
		
    case 0x68: // move west
		parseMoveWest(msg);
		break;
		
    case 0x6A:
		parseMoveNorthEast(msg);
		break;
		
    case 0x6B:
		parseMoveSouthEast(msg);
		break;
		
    case 0x6C:
		parseMoveSouthWest(msg);
		break;
		
    case 0x6D:
		parseMoveNorthWest(msg);
		break;
		
	case 0x6F: // turn north
		parseTurnNorth(msg);
		break;
		
	case 0x70: // turn east
		parseTurnEast(msg);
		break;
		
    case 0x71: // turn south
		parseTurnSouth(msg);
		break;
		
    case 0x72: // turn west
		parseTurnWest(msg);
		break;
		
	case 0x7D: // Request trade
		parseRequestTrade(msg);
		break;
		
	case 0x7E: // Look at an item in trade
		parseLookInTrade(msg);
		break;
		
	case 0x7F: // Accept trade
		parseAcceptTrade(msg);
		break;
		
	case 0x80: // Close/cancel trade
		parseCloseTrade();
		break;
		
    case 0x78: // throw item
		parseThrow(msg);
		break;
		
    case 0x82: // use item
		parseUseItem(msg);
		break;
		
    case 0x83: // use item
		parseUseItemEx(msg);
		break;
		
	case 0x84: // battle window
		parseBattleWindow(msg);
		break;
	
    case 0x85:	//rotate item
		parseRotateItem(msg);
		break;
		
    case 0x87: // close container
		parseCloseContainer(msg);
		break;
		
	case 0x88: //"up-arrow" - container
		parseUpArrowContainer(msg);	
		break;
		
	case 0x89:
		parseTextWindow(msg);
		break;

#ifdef BD_HOUSE_WINDOW
	case 0x8A: // save house text
		parseHouseWindow(msg);
		break;
#endif //BD_HOUSE_WINDOW
		
    case 0x8C: // throw item
		parseLookAt(msg);
		break;
		
    case 0x96:  // say something
		parseSay(msg);
		break;
		
    case 0xA1: // attack
		parseAttack(msg);
		break;

#ifdef BD_FOLLOW
case 0xA2: // follow
parseFollow(msg);
break;
#endif //BD_FOLLOW
#ifdef TLM_SKULLS_PARTY
	case 0xA3: // invite party
		parseInviteParty(msg);
		break;

	case 0xA5: // revoke party
		parseRevokeParty(msg);
		break;

	case 0xA7: // leave party
		game->LeaveParty(player);
		break;

	case 0xA4: // join party
		parseJoinParty(msg);
		break;

	case 0xA6: // pass leadership
		parsePassLeadership(msg);
		break;
#endif //TLM_SKULLS_PARTY
		
    case 0xD2: // request Outfit
		parseRequestOutfit(msg);
		break;
		
    case 0xD3: // set outfit
		parseSetOutfit(msg);
		break;
		
    case 0x97: // request Channels
		parseGetChannels(msg);
		break;
		
    case 0x98: // open Channel
		parseOpenChannel(msg);
		break;
		
    case 0x99: // close Channel
		parseCloseChannel(msg);
		break;
		
    case 0x9A: // open priv
		parseOpenPriv(msg);
		break;
		
    case 0xBE: // cancel move
		parseCancelMove(msg);
		break;
		
    case 0xA0: // set attack and follow mode
		parseModes(msg);
		break;
	
#ifdef ELEM_VIP_LIST
	case 0xDC: // Add vip
		elemParseAddVip(msg);
		break;

	case 0xDD: // Remove VIP
		elemParseRemoveVip(msg);
		break;
#else
	case 0xDC:
		parseAddVip(msg);
		break;
		
	case 0xDD:
		parseRemVip(msg);
		break;
#endif //ELEM_VIP_LIST
		
    case 0x69: // client quit without logout <- wrong
		if(game->stopEvent(player->eventAutoWalk)) {
			sendCancelWalk();
		}
		break;
		
    case 0x1E: // keep alive / ping response
		player->receivePing();
		break;
		
    case 0xC9: // change position
		// update position 
		break;
		
    default:
#ifdef __DEBUG__
		printf("unknown packet header: %x \n", recvbyte);
		parseDebug(msg);
#endif
		break;
	}
	game->flushSendBuffers();
}

void Protocol76::GetTileDescription(const Tile* tile, NetworkMessage &msg
#ifdef TRS_GM_INVISIBLE
									, Player* p
#endif //TRS_GM_INVISIBLE
									)
{
	if (tile)
	{
		int count = 0;
		if(tile->ground) {
			msg.AddItem(tile->ground);
			count++;
		}
		
		if (tile->splash)
		{
			msg.AddItem(tile->splash);
			count++;
		}
		
		ItemVector::const_iterator it;
		for (it = tile->topItems.begin(); ((it !=tile->topItems.end()) && (count < 10)); ++it)
		{		
			msg.AddItem(*it);
			count++;
		}
		
#ifdef TRS_GM_INVISIBLE
		CreatureVector::const_iterator itc;
		for (itc = tile->creatures.begin(); ((itc !=tile->creatures.end()) && (count < 10)); ++itc)
		{
			Player* player = dynamic_cast<Player*>((*itc));
			if(player)
			{
				if(!player->gmInvisible || player->gmInvisible && player == p || p->access >= player->access)
				{
					bool known;
					unsigned long removedKnown;
					checkCreatureAsKnown((*itc)->getID(), known, removedKnown);
					AddCreature(msg,*itc, known, removedKnown);
					count++;
				}
			}
			else
			{
				bool known;
				unsigned long removedKnown;
				checkCreatureAsKnown((*itc)->getID(), known, removedKnown);
				AddCreature(msg,*itc, known, removedKnown);
				count++;
			}
		}
#else //TRS_GM_INVISIBLE
		CreatureVector::const_iterator itc;
		for (itc = tile->creatures.begin(); ((itc !=tile->creatures.end()) && (count < 10)); ++itc)
		{
			bool known;
			unsigned long removedKnown;
			checkCreatureAsKnown((*itc)->getID(), known, removedKnown);
			AddCreature(msg,*itc, known, removedKnown);
			count++;
		}
#endif //TRS_GM_INVISIBLE	
		
		for (it = tile->downItems.begin(); ((it !=tile->downItems.end()) && (count < 10)); ++it)
		{			
			msg.AddItem(*it);
			count++;			
		}
	}
}

void Protocol76::GetMapDescription(unsigned short x, unsigned short y, unsigned char z,
                                   unsigned short width, unsigned short height,
                                   NetworkMessage &msg
#ifdef TRS_GM_INVISIBLE
								   , Player* p
#endif //TRS_GM_INVISIBLE
								   )
{
	Tile* tile;
	int skip = -1;
	int startz, endz, offset, zstep, cc = 0;
	if (z > 7) {
		startz = z - 2;
		endz = std::min(MAP_LAYER - 1, z + 2);
		zstep = 1;
	}
	else {
		startz = 7;
		endz = 0;
		
		zstep = -1;
	}
	
	for(int nz = startz; nz != endz + zstep; nz += zstep) {
		offset = z - nz;
		
		for (int nx = 0; nx < width; nx++)
			for (int ny = 0; ny < height; ny++) {
				tile = game->getTile(x + nx + offset, y + ny + offset, nz);
				if (tile) {
					if (skip >= 0) {
						msg.AddByte(skip);
						msg.AddByte(0xFF);
						cc +=skip;
					}   
					skip = 0;
					
#ifdef TRS_GM_INVISIBLE
					GetTileDescription(tile, msg, p);
#else
					GetTileDescription(tile, msg);
#endif //TRS_GM_INVISIBLE
					cc++;
					
				}
				else {
				/*
				if(skip == -1) 
				skip = 0;
					*/
					
					skip++;
					if (skip == 0xFF) {
						msg.AddByte(0xFF);
						msg.AddByte(0xFF);
						cc += skip;
						skip = -1;
					}
				}
			}
	}
	
	if (skip >= 0) {
		msg.AddByte(skip);
		msg.AddByte(0xFF);
		cc += skip;
	}
	
#ifdef __DEBUG__
	printf("tiles in total: %d \n", cc);
#endif
}


void Protocol76::checkCreatureAsKnown(unsigned long id, bool &known, unsigned long &removedKnown)
{
	// loop through the known player and check if the given player is in
	std::list<unsigned long>::iterator i;
	for(i = knownPlayers.begin(); i != knownPlayers.end(); ++i)
	{
		if((*i) == id)
		{
			// know... make the creature even more known...
			knownPlayers.erase(i);
			knownPlayers.push_back(id);
			
			known = true;
			return;
		}
	}
	
	// ok, he is unknown...
	known = false;
	
	// ... but not in future
	knownPlayers.push_back(id);
	
	// to many known players?
	if(knownPlayers.size() > 150)
	{
		// lets try to remove one from the end of the list
		for (int n = 0; n < 150; n++)
		{
			removedKnown = knownPlayers.front();
			
			Creature *c = game->getCreatureByID(removedKnown);
			if ((!c) || (!CanSee(c)))
				break;
			
			// this creature we can't remove, still in sight, so back to the end
			knownPlayers.pop_front();
			knownPlayers.push_back(removedKnown);
		}
		
		// hopefully we found someone to remove :S, we got only 150 tries
		// if not... lets kick some players with debug errors :)
		knownPlayers.pop_front();
	}
	else
	{
		// we can cache without problems :)
		removedKnown = 0;
	}
}


// Parse methods
void Protocol76::parseLogout(NetworkMessage &msg)
{
	if(player->inFightTicks >=1000 && player->isRemoved == false){
		sendCancel("You may not logout during or immediately after a fight!");
		return;
	}
	else{
		logout();
	}
}

void Protocol76::logout(){
	// we ask the game to remove us
	if(player->isRemoved == false){
		if(game->removeCreature(player))
			pendingLogout = true;
	}
	else{
		pendingLogout = true;
	}
}

void Protocol76::parseGetChannels(NetworkMessage &msg){
	sendChannelsDialog();
}

void Protocol76::parseOpenChannel(NetworkMessage &msg){
	unsigned short channelId = msg.GetU16();
	OTSYS_THREAD_LOCK_CLASS lockClass(game->gameLock, "Protocol76::parseOpenChannel()");
	if(g_chat.addUserToChannel(player, channelId))
		sendChannel(channelId, g_chat.getChannelName(player, channelId));
}

void Protocol76::parseCloseChannel(NetworkMessage &msg){
	unsigned short channelId = msg.GetU16();
	OTSYS_THREAD_LOCK_CLASS lockClass(game->gameLock, "Protocol76::parseCloseChannel()");
	g_chat.removeUserFromChannel(player, channelId);
}

void Protocol76::parseOpenPriv(NetworkMessage &msg){
	std::string receiver; 
	receiver = msg.GetString();
	OTSYS_THREAD_LOCK_CLASS lockClass(game->gameLock, "Protocol76::parseOpenPriv()");
	Player* player = game->getPlayerByName(receiver);
	if(player){
		sendOpenPriv(player->getName());
	}
}

void Protocol76::sendOpenPriv(const std::string &receiver){
	NetworkMessage newmsg; 
	newmsg.AddByte(0xAD); 
	newmsg.AddString(receiver);
	WriteBuffer(newmsg);
}     

void Protocol76::parseCancelMove(NetworkMessage &msg)
{
	game->playerSetAttackedCreature(player, 0);
	
	game->stopEvent(player->eventAutoWalk);
	player->sendCancelWalk();
}

void Protocol76::parseModes(NetworkMessage &msg)
{
#ifdef YUR_FIGHT_MODE
	player->setFightMode(msg.GetByte());
    player->followMode = msg.GetByte();
#endif //YUR_FIGHT_MODE
	//player->fightMode = msg.GetByte();
	
}

void Protocol76::parseDebug(NetworkMessage &msg)
{
	int dataLength = msg.getMessageLength()-3;
	if(dataLength!=0){
		printf("data: ");
		size_t data = msg.GetByte();
		while( dataLength > 0){
			printf("%d ", data);
			if(--dataLength >0)
				data = msg.GetByte();
		}
		printf("\n");
	}
}

void Protocol76::parseMoveByMouse(NetworkMessage &msg)
{
	// first we get all directions...
	std::list<Direction> path;
	size_t numdirs = msg.GetByte();
	for (size_t i = 0; i < numdirs; ++i) {
		unsigned char rawdir = msg.GetByte();
		Direction dir = SOUTH;
		
		switch(rawdir) {
		case 1: dir = EAST; break;
		case 2: dir = NORTHEAST; break;
		case 3: dir = NORTH; break;
		case 4: dir = NORTHWEST; break;
		case 5: dir = WEST; break;
		case 6: dir = SOUTHWEST; break;
		case 7: dir = SOUTH; break;
		case 8: dir = SOUTHEAST; break;
			
		default:
			continue;
		};
		
		/*
		#ifdef __DEBUG__
		std::cout << "Walk by mouse: Direction: " << dir << std::endl;
		#endif
		*/
		path.push_back(dir);
	}
	
	game->playerAutoWalk(player, path);
}

void Protocol76::parseMoveNorth(NetworkMessage &msg)
{
#ifdef TR_ANTI_AFK
	player->notAfk();
#endif //TR_ANTI_AFK

	if(game->stopEvent(player->eventAutoWalk)) {
		player->sendCancelWalk();
	}
	
	this->sleepTillMove();
	
	game->thingMove(player, player,
		player->pos.x, player->pos.y-1, player->pos.z, 1);
}

void Protocol76::parseMoveEast(NetworkMessage &msg)
{
#ifdef TR_ANTI_AFK
	player->notAfk();
#endif //TR_ANTI_AFK

	if(game->stopEvent(player->eventAutoWalk)) {
		player->sendCancelWalk();
	}
	
	this->sleepTillMove();
	
	game->thingMove(player, player,
		player->pos.x+1, player->pos.y, player->pos.z, 1);
}


void Protocol76::parseMoveSouth(NetworkMessage &msg)
{
#ifdef TR_ANTI_AFK
	player->notAfk();
#endif //TR_ANTI_AFK

	if(game->stopEvent(player->eventAutoWalk)) {
		player->sendCancelWalk();
	}
	
	this->sleepTillMove();
	
	game->thingMove(player, player,
		player->pos.x, player->pos.y+1, player->pos.z, 1);
}


void Protocol76::parseMoveWest(NetworkMessage &msg)
{
#ifdef TR_ANTI_AFK
	player->notAfk();
#endif //TR_ANTI_AFK

	if(game->stopEvent(player->eventAutoWalk)) {
		player->sendCancelWalk();
	}
	
	this->sleepTillMove();
	
	game->thingMove(player, player,
		player->pos.x-1, player->pos.y, player->pos.z, 1);
}

void Protocol76::parseMoveNorthEast(NetworkMessage &msg)
{
#ifdef TR_ANTI_AFK
	player->notAfk();
#endif //TR_ANTI_AFK

	if(game->stopEvent(player->eventAutoWalk)) {
		player->sendCancelWalk();
	}
	
	this->sleepTillMove();
	this->sleepTillMove();
	
	game->thingMove(player, player,
		(player->pos.x+1), (player->pos.y-1), player->pos.z, 1);
}

void Protocol76::parseMoveSouthEast(NetworkMessage &msg)
{
#ifdef TR_ANTI_AFK
	player->notAfk();
#endif //TR_ANTI_AFK

	if(game->stopEvent(player->eventAutoWalk)) {
		player->sendCancelWalk();
	}
	
	this->sleepTillMove();
	this->sleepTillMove();
	
	game->thingMove(player, player,
		(player->pos.x+1), (player->pos.y+1), player->pos.z, 1);
}

void Protocol76::parseMoveSouthWest(NetworkMessage &msg)
{
#ifdef TR_ANTI_AFK
	player->notAfk();
#endif //TR_ANTI_AFK

	if(game->stopEvent(player->eventAutoWalk)) {
		player->sendCancelWalk();
	}
	
	this->sleepTillMove();
	this->sleepTillMove();
	
	game->thingMove(player, player,
		(player->pos.x-1), (player->pos.y+1), player->pos.z, 1);
}

void Protocol76::parseMoveNorthWest(NetworkMessage &msg)
{
#ifdef TR_ANTI_AFK
	player->notAfk();
#endif //TR_ANTI_AFK

	if(game->stopEvent(player->eventAutoWalk)) {
		player->sendCancelWalk();
	}
	
	this->sleepTillMove();
	this->sleepTillMove();
	
	game->thingMove(player, player,
		(player->pos.x-1), (player->pos.y-1), player->pos.z, 1);   
}


void Protocol76::parseTurnNorth(NetworkMessage &msg)
{
#ifdef TR_ANTI_AFK
	player->notAfk();
#endif //TR_ANTI_AFK
	game->creatureTurn(player, NORTH);
}


void Protocol76::parseTurnEast(NetworkMessage &msg)
{
#ifdef TR_ANTI_AFK
	player->notAfk();
#endif //TR_ANTI_AFK
	game->creatureTurn(player, EAST);
}


void Protocol76::parseTurnSouth(NetworkMessage &msg)
{
#ifdef TR_ANTI_AFK
	player->notAfk();
#endif //TR_ANTI_AFK
	game->creatureTurn(player, SOUTH);
}


void Protocol76::parseTurnWest(NetworkMessage &msg)
{
#ifdef TR_ANTI_AFK
	player->notAfk();
#endif //TR_ANTI_AFK
	game->creatureTurn(player, WEST);
}

void Protocol76::parseRequestOutfit(NetworkMessage &msg)
{
	msg.Reset();
	
	msg.AddByte(0xC8);
	msg.AddByte(player->looktype);
	msg.AddByte(player->lookhead);
	msg.AddByte(player->lookbody);
	msg.AddByte(player->looklegs);
	msg.AddByte(player->lookfeet);

	switch (player->getSex()) 
	{
	case PLAYERSEX_FEMALE:
		msg.AddByte(PLAYER_FEMALE_1);
#ifdef YUR_PREMIUM_PROMOTION
		msg.AddByte(player->isPremium()? PLAYER_FEMALE_7 : PLAYER_FEMALE_4);
#else
		msg.AddByte(PLAYER_FEMALE_4);	
#endif //YUR_PREMIUM_PROMOTION
		break;

	case PLAYERSEX_MALE:
		msg.AddByte(PLAYER_MALE_1);
#ifdef YUR_PREMIUM_PROMOTION
		msg.AddByte(player->isPremium()? PLAYER_MALE_7 : PLAYER_MALE_4);
#else
		msg.AddByte(PLAYER_MALE_4);
#endif //YUR_PREMIUM_PROMOTION
		break;

	case PLAYERSEX_OLDMALE:
		msg.AddByte(160);
		msg.AddByte(160);
		break;

	default:
		msg.AddByte(PLAYER_MALE_1);
#ifdef YUR_PREMIUM_PROMOTION
		msg.AddByte(player->isPremium()? PLAYER_MALE_7 : PLAYER_MALE_4);
#else
		msg.AddByte(PLAYER_MALE_4);
#endif //YUR_PREMIUM_PROMOTION
	}
	
	WriteBuffer(msg);
}

void Protocol76::sendSetOutfit(const Creature* creature) {
	if (CanSee(creature)) {
		NetworkMessage newmsg;
		newmsg.AddByte(0x8E);
		newmsg.AddU32(creature->getID());

#ifdef BD_ITEM_LOOK
		if (creature->looktype > 1000)
		{
			newmsg.AddByte(0);
			newmsg.AddU16(Item::items[creature->looktype].clientId);
		}
		else
#endif //BD_ITEM_LOOK
		{
#ifdef YUR_INVISIBLE
			if (creature->isInvisible())
			{
				newmsg.AddByte(0);
				newmsg.AddU16(0);
			}
			else
#endif //YUR_INVISIBLE
			{
				newmsg.AddByte(creature->looktype);
				newmsg.AddByte(creature->lookhead);
				newmsg.AddByte(creature->lookbody);
				newmsg.AddByte(creature->looklegs);
				newmsg.AddByte(creature->lookfeet);
			}
		}

		WriteBuffer(newmsg);
	}
}

void Protocol76::sendInventory(unsigned char sl_id){	
	NetworkMessage msg;
	AddPlayerInventoryItem(msg,player,sl_id);
	WriteBuffer(msg);
}

void Protocol76::sendStats(){
	NetworkMessage msg;
	AddPlayerStats(msg,player);
	WriteBuffer(msg);
}

void Protocol76::sendTextMessage(MessageClasses mclass, const char* message){
	NetworkMessage msg;
	AddTextMessage(msg,mclass, message);
	WriteBuffer(msg);
}

void Protocol76::sendTextMessage(MessageClasses mclass, const char* message,const Position &pos, unsigned char type){
	NetworkMessage msg;
	AddMagicEffect(msg,pos,type);
	AddTextMessage(msg,mclass, message);
	WriteBuffer(msg);
}

void Protocol76::sendChannelsDialog(){
	NetworkMessage newmsg;
	ChannelList list;
	
	list = g_chat.getChannelList(player);
	
	newmsg.AddByte(0xAB);
	
	newmsg.AddByte((unsigned char)list.size()); //how many
	
	while(list.size())
	{
		ChatChannel *channel;
		channel = list.front();
		list.pop_front();
		
		newmsg.AddU16(channel->getId());
		newmsg.AddString(channel->getName());
	}
	WriteBuffer(newmsg);
}

void Protocol76::sendChannel(unsigned short channelId, std::string channelName){
	NetworkMessage newmsg;
	
	newmsg.AddByte(0xAC);
	newmsg.AddU16(channelId);
	newmsg.AddString(channelName);
	
	WriteBuffer(newmsg); 
}

void Protocol76::sendIcons(int icons){
	NetworkMessage newmsg;
	newmsg.AddByte(0xA2);
	newmsg.AddByte(icons);
	WriteBuffer(newmsg);
}

void Protocol76::parseSetOutfit(NetworkMessage &msg)
{
	int temp = msg.GetByte();
	if ((player->getSex() == PLAYERSEX_FEMALE && temp >= PLAYER_FEMALE_1 && 
#ifdef YUR_PREMIUM_PROMOTION
		temp <= (player->isPremium()? PLAYER_FEMALE_7 : PLAYER_FEMALE_4)) ||
#else
		temp <= PLAYER_FEMALE_7) ||
#endif //YUR_PREMIUM_PROMOTION

		(player->getSex() == PLAYERSEX_MALE && temp >= PLAYER_MALE_1 && 
#ifdef YUR_PREMIUM_PROMOTION
		temp <= (player->isPremium()? PLAYER_MALE_7 : PLAYER_MALE_4)))
#else
		temp <= PLAYER_MALE_7))
#endif //YUR_PREMIUM_PROMOTION
	{
		player->looktype= temp;
		player->lookmaster = player->looktype;
		player->lookhead=msg.GetByte();
		player->lookbody=msg.GetByte();
		player->looklegs=msg.GetByte();
		player->lookfeet=msg.GetByte();
		
		game->creatureChangeOutfit(player);
	}
}


void Protocol76::parseUseItemEx(NetworkMessage &msg)
{
	Position pos_from = msg.GetPosition();
	unsigned short itemid = msg.GetItemId();
	unsigned char from_stackpos = msg.GetByte();
	Position pos_to = msg.GetPosition();
	/*unsigned short tile_id = */msg.GetU16();
	unsigned char to_stackpos = msg.GetByte();
	
	game->playerUseItemEx(player,pos_from,from_stackpos, pos_to, to_stackpos, itemid);
	
}

void Protocol76::parseBattleWindow(NetworkMessage &msg)
{
	Position pos_from = msg.GetPosition();
	unsigned short itemid = msg.GetItemId();
	unsigned char from_stackpos = msg.GetByte();
	unsigned long creatureid = msg.GetU32();

	game->playerUseBattleWindow(player, pos_from, from_stackpos, itemid, creatureid);
}

void Protocol76::sendContainer(unsigned char index, Container *container)
{
	if(!container)
		return;
	
	NetworkMessage msg;
	
	player->addContainer(index, container);
	
	msg.AddByte(0x6E);
	msg.AddByte(index);
	
	//msg.AddU16(container->getID());
	msg.AddItemId(container);
	msg.AddString(container->getName());
	msg.AddByte(container->capacity());
	if(container->getParent() != NULL)
		msg.AddByte(0x01); // container up ID (can go up) 
	else
		msg.AddByte(0x00);
	
	msg.AddByte(container->size());
	
	ContainerList::const_iterator cit;
	for (cit = container->getItems(); cit != container->getEnd(); ++cit) {
		msg.AddItem(*cit);
	}
	WriteBuffer(msg);
}

void Protocol76::sendTradeItemRequest(const Player* player, const Item* item, bool ack)
{
	NetworkMessage msg;
	if(ack) {
		msg.AddByte(0x7D);
	}
	else {
		msg.AddByte(0x7E);
	}
	
	msg.AddString(player->getName());
	
	const Container *tradeContainer = dynamic_cast<const Container*>(item);
	if(tradeContainer) {
		
		std::list<const Container*> stack;
		stack.push_back(tradeContainer);
		
		std::list<const Item*> itemstack;
		itemstack.push_back(tradeContainer);
		
		ContainerList::const_iterator it;
		
		while(stack.size() > 0) {
			const Container *container = stack.front();
			stack.pop_front();
			
			for (it = container->getItems(); it != container->getEnd(); ++it) {
				Container *container = dynamic_cast<Container*>(*it);
				if(container) {
					stack.push_back(container);
				}
				
				itemstack.push_back(*it);
			}
		}
		
		msg.AddByte((unsigned char)itemstack.size());
		while(itemstack.size() > 0) {
			const Item* item = itemstack.front();
			itemstack.pop_front();
			msg.AddItem(item);
		}
	}
	else {
		msg.AddByte(1);
		msg.AddItem(item);
	}
	
	WriteBuffer(msg);
}

void Protocol76::sendCloseTrade()
{
	NetworkMessage msg;
	msg.AddByte(0x7F);
	
	WriteBuffer(msg);
}

void Protocol76::sendCloseContainer(unsigned char containerid)
{
	NetworkMessage msg;
	
	msg.AddByte(0x6F);
	msg.AddByte(containerid);
	WriteBuffer(msg);
}

void Protocol76::parseUseItem(NetworkMessage &msg)
{
	Position pos = msg.GetPosition();
	unsigned short itemid = msg.GetItemId();
	unsigned char stack = msg.GetByte();
	unsigned char index = msg.GetByte();
	
#ifdef __DEBUG__
	std::cout << "parseUseItem: " << "x: " << pos.x << ", y: " << (int)pos.y <<  ", z: " << (int)pos.z << ", item: " << (int)item << ", stack: " << (int)stack << ", index: " << (int)index << std::endl;
#endif
	
	game->playerUseItem(player, pos, stack, itemid, index);
}

void Protocol76::parseCloseContainer(NetworkMessage &msg)
{
	unsigned char containerid = msg.GetByte();
	player->closeContainer(containerid);
	sendCloseContainer(containerid);
}

void Protocol76::parseUpArrowContainer(NetworkMessage &msg)
{
	unsigned char containerid = msg.GetByte();
	OTSYS_THREAD_LOCK_CLASS lockClass(game->gameLock, "Protocol76::parseUpArrowContainer()");
	Container *container = player->getContainer(containerid);
	if(!container)
		return;
	
	Container *parentcontainer = container->getParent();
	if(parentcontainer) {
		sendContainer(containerid, parentcontainer);
	}
}

void Protocol76::parseThrow(NetworkMessage &msg)
{
	unsigned short from_x     = msg.GetU16();
	unsigned short from_y     = msg.GetU16(); 
	unsigned char  from_z     = msg.GetByte();
	unsigned short itemid     = msg.GetItemId();
	unsigned char  from_stack = msg.GetByte();
	unsigned short to_x       = msg.GetU16();
	unsigned short to_y       = msg.GetU16(); 
	unsigned char  to_z       = msg.GetByte();
	unsigned char count       = msg.GetByte();
	/*
	std::cout << "parseThrow: " << "from_x: " << (int)from_x << ", from_y: " << (int)from_y
	<<  ", from_z: " << (int)from_z << ", item: " << (int)itemid << ", from_stack: " 
	<< (int)from_stack << " to_x: " << (int)to_x << ", to_y: " << (int)to_y
	<<  ", to_z: " << (int)to_z  
	<< ", count: " << (int)count << std::endl;*/
	bool toInventory = false;
	bool fromInventory = false;
	
	if(from_x == to_x && from_y == to_y && from_z == to_z)
		return;
		if(to_x != 0xFFFF && (itemid == ITEM_PARCEL || itemid == ITEM_LETTER))
{
Tile* t = game->getTile(to_x, to_y, to_z);
if(t && (t->hasItem(ITEM_MAILBOX1) || t->hasItem(ITEM_MAILBOX2)))
{
Item* parcel = dynamic_cast<Item*>(game->getThing(Position(from_x, from_y, from_z), from_stack, player));
if(parcel)
{
bool canSend = true;
bool foundLabel = false;
std::string receiver;
unsigned int depotid;
Container* parcel2 = dynamic_cast<Container*>(parcel);
if(parcel2 && itemid == ITEM_PARCEL)
{
for(int x = 0; x < parcel2->size(); x++)
{
Item* label = dynamic_cast<Item*>(parcel2->getItem(x));
if(label && label->getID() == ITEM_LABEL)
{
foundLabel = true;
std::string text = label->getText();
std::istringstream iss(text, std::istringstream::in);
int i = 0;
std::string line[2];
while(std::getline(iss, text,'\n'))
{
line[i] = text;
i++;
if(i == 2)
break;
}
receiver = line[0];
std::transform(line[1].begin(), line[1].end(), line[1].begin(), (int(*)(int))tolower);
if(line[1] == "lakeshire")
depotid = 1;
else if(line[1] == "sanduska") // add as much as you want
depotid = 2;
else if(line[1] == "rivendale") // add as much as you want
depotid = 3;
else
canSend = false;
if(!game->isPlayer(receiver))
canSend = false;
if(canSend)
break;
}
}
}
else if(itemid == ITEM_LETTER)
{
foundLabel = true;
std::string text = parcel->getText();
std::istringstream iss(text, std::istringstream::in);
int i = 0;
std::string line[2];
while(std::getline(iss, text,'\n'))
{
line[i] = text;
i++;
if(i == 2)
break;
}
receiver = line[0];
std::transform(line[1].begin(), line[1].end(), line[1].begin(), (int(*)(int))tolower);
if(line[1] == "lakeshire")
depotid = 1;
else if(line[1] == "sanduska") // add as much as you want
depotid = 2;
else if(line[1] == "rivendale") // add as much as you want
depotid = 3;
else
canSend = false;
if(!game->isPlayer(receiver))
canSend = false;
}
if(canSend && foundLabel)
{
game->removeThing(player,Position(from_x, from_y, from_z), parcel, true);
Player* player = game->getPlayerByName(receiver);
if(player)
{
parcel->setID((itemid == ITEM_LETTER?ITEM_STLETTER:ITEM_STPARCEL));
Container* depot = player->getDepot(depotid);
if(depot)
depot->addItem(parcel);
std::vector<unsigned char> containerlist;
containerLayout::const_iterator cit;
for(cit = player->getContainers(); cit != player->getEndContainer(); ++cit)
{
Container* c = cit->second;
if(c == depot)
{
player->sendTextMessage(MSG_ADVANCE, "New mail has arrived.");
player->onItemAddContainer(depot, parcel);
break;
}
if(c->getParent() == depot)
{
player->sendTextMessage(MSG_ADVANCE, "New mail has arrived.");
break;
}
}
}
else
{
Protocol76* prot = new Protocol76(0);
player = new Player(receiver, prot);
parcel->setID((itemid == ITEM_LETTER?ITEM_STLETTER:ITEM_STPARCEL));
IOPlayer::instance()->loadPlayer(player, receiver);
Container* depot = player->getDepot(depotid);
if(depot)
depot->addItem(parcel);
player->lastlogin = player->lastLoginSaved;
IOPlayer::instance()->savePlayer(player);
delete player;
}
return;
}
}
}
}
	
	//container/inventory to container/inventory
	if(from_x == 0xFFFF && to_x == 0xFFFF) {
		unsigned char from_cid;
		unsigned char to_cid;
		
		if(from_y & 0x40) {
			from_cid = from_y & 0x0F;
		}
		else {
			fromInventory = true;
			from_cid = static_cast<unsigned char>(from_y);
		}
		
		if(to_y & 0x40) {
			to_cid = static_cast<unsigned char>(to_y & 0x0F);
		}
		else {
			toInventory = true;
			to_cid = static_cast<unsigned char>(to_y);
		}
		
		game->thingMove(player, from_cid, from_z, itemid,fromInventory, to_cid, to_z, toInventory, count);
	}
	//container/inventory to ground
	else if(from_x == 0xFFFF && to_x != 0xFFFF) {
		unsigned char from_cid;
		
		if(0x40 & from_y) {
			from_cid = static_cast<unsigned char>(from_y & 0x0F);
		}
		else {
			fromInventory = true;
			from_cid = static_cast<unsigned char>(from_y);
		}
		
		game->thingMove(player, from_cid, from_z, itemid, fromInventory, Position(to_x, to_y, to_z), count);
	}
	//ground to container/inventory
	else if(from_x != 0xFFFF && to_x == 0xFFFF) {
		unsigned char to_cid;
		
		if(0x40 & to_y) {
			to_cid = static_cast<unsigned char>(to_y & 0x0F);
		}
		else {
			toInventory = true;
			to_cid = static_cast<unsigned char>(to_y);
			
			if(to_cid > 11) {
				return;
			}
			
			if(to_cid == 0) {
				//Special condition, player drops an item to the "capacity window" when the inventory is minimized,
				//we should add this item to the most appropriate slot/container
				return;
			}
		}
		
		game->thingMove(player, Position(from_x, from_y, from_z), from_stack, itemid, to_cid, to_z, toInventory, count);
	}
	//ground to ground
	else {
		Tile *fromTile = game->getTile(from_x, from_y, from_z);
		if(!fromTile)
			return;
		Creature *movingCreature = dynamic_cast<Creature*>(fromTile->getThingByStackPos(from_stack));
		
		if(movingCreature) {
			Player *movingPlayer = dynamic_cast<Player*>(movingCreature);
			if(player == movingPlayer) {
				this->sleepTillMove();
			}
		}
		
		game->thingMove(player, from_x, from_y, from_z, from_stack, itemid, to_x, to_y, to_z, count);
	}
}


void Protocol76::parseLookAt(NetworkMessage &msg){
	Position LookPos = msg.GetPosition();
	unsigned short ItemNum = msg.GetU16();
	unsigned char stackpos = msg.GetByte();
	
#ifdef __DEBUG__
	std::cout << "look at: " << LookPos << std::endl;
	std::cout << "itemnum: " << ItemNum << " stackpos: " << (long)stackpos<< std::endl;
#endif
	
	NetworkMessage newmsg;
	std::stringstream ss;
	
	/*
	#ifdef __DEBUG__
	ss << "You look at " << LookPos << " and see Item # " << ItemNum << ".";
	AddTextMessage(newmsg,MSG_INFO, ss.str().c_str());
	#else
	*/
	Item *item = NULL;
	Creature *creature = NULL;
	OTSYS_THREAD_LOCK_CLASS lockClass(game->gameLock, "Protocol76::parseLookAt()");
	
	if(LookPos.x != 0xFFFF) {
		Tile* tile = game->getTile(LookPos.x, LookPos.y, LookPos.z);
		if(tile){
			item = dynamic_cast<Item*>(tile->getTopThing());
			creature = dynamic_cast<Creature*>(tile->getTopThing());
		}
	}
	else {
		//from container/inventory
		if(LookPos.y & 0x40) {
			unsigned char from_cid = LookPos.y & 0x0F;
			unsigned char slot = LookPos.z;
			
			Container *parentcontainer = player->getContainer(from_cid);
			if(!parentcontainer)
				return;
			
			item = parentcontainer->getItem(slot);
		}
		else {
			unsigned char from_cid = static_cast<unsigned char>(LookPos.y);
			item = player->getItem(from_cid);
		}
	}
	
	if(item) 
	{
             #ifdef TLM_BEDS    
       if(item->isBed2()){
    Position pos(LookPos.x, LookPos.y, LookPos.z);
    std::stringstream bedtext;        
    bedtext << "You see a bed"/*<< item->getName()*/
    << ".\n " << game->getBedSleeper(pos) << " is sleeping there.";
    AddTextMessage(newmsg, MSG_INFO,
    bedtext.str().c_str());
    sendNetworkMessage(&newmsg);
            return;
    }
#endif //TLM_BEDS 
#ifdef TLM_HOUSE_SYSTEM
		Tile* doorTile = game->getTile(LookPos);
		Thing* doorThing = doorTile? doorTile->getTopThing() : NULL;
		Item* doorItem = doorThing? dynamic_cast<Item*>(doorThing) : NULL;

		if (doorItem && Item::items[doorItem->getID()].isDoor && doorTile && doorTile->getHouse())
			AddTextMessage(newmsg, MSG_INFO, doorTile->getHouse()->getDescription().c_str());
		else
#endif //TLM_HOUSE_SYSTEM
		{
			bool fullDescription = false;
			if(LookPos.x == 0xFFFF) {
				fullDescription = true;
			}
			else if(std::abs(player->pos.x - LookPos.x) <= 1 && std::abs(player->pos.y - LookPos.y) <= 1 &&
				LookPos.z == player->pos.z) {
				fullDescription = true;
			}
			
			std::stringstream ss;
			ss << "You see " << item->getDescription(fullDescription);

#ifdef YUR_GM_LOOK
			if (player->access >= g_config.ACCESS_LOOK)
				ss << "\nId: " << Item::items.reverseLookUp(ItemNum)
				<< ". Pos: " << LookPos.x << ' ' << LookPos.y << ' ' << LookPos.z << '.';
#endif //YUR_GM_LOOK

			AddTextMessage(newmsg,MSG_INFO, ss.str().c_str());
		}
	}
	else if(creature) {
		if(player == creature) {
			std::stringstream ss;
			ss << "You see " << creature->getDescription(true);
			AddTextMessage(newmsg,MSG_INFO, ss.str().c_str());
		}
		else {
			std::stringstream ss;
			ss << "You see " << creature->getDescription().c_str();
			AddTextMessage(newmsg,MSG_INFO, ss.str().c_str());
		}
	}
	//#endif
	
	sendNetworkMessage(&newmsg);
}



void Protocol76::parseSay(NetworkMessage &msg)
{
	SpeakClasses type = (SpeakClasses)msg.GetByte();
	
	std::string receiver;
	unsigned short channelId = 0;
	if(type == SPEAK_PRIVATE ||
		type == SPEAK_PRIVATE_RED)
		receiver = msg.GetString();
	if(type == SPEAK_CHANNEL_Y ||
		type == SPEAK_CHANNEL_R1 ||
		type == SPEAK_CHANNEL_R2)
		channelId = msg.GetU16();
	std::string text = msg.GetString();
	
	if(game->creatureSaySpell(player, text))
		type = SPEAK_SAY;
	
	switch (type)
	{
	case SPEAK_SAY:
		game->creatureSay(player, type, text);
		break;
	case SPEAK_WHISPER:
		game->creatureWhisper(player, text);
		break;
	case SPEAK_YELL:
		game->creatureYell(player, text);
		break;
	case SPEAK_PRIVATE:
	case SPEAK_PRIVATE_RED:
		game->creatureSpeakTo(player, type, receiver, text);
		break;
	case SPEAK_CHANNEL_Y:
	case SPEAK_CHANNEL_R1:
	case SPEAK_CHANNEL_R2:
    case SPEAK_CHANNEL_O:
		game->creatureTalkToChannel(player, type, text, channelId);
		break;
	case SPEAK_BROADCAST:
		game->creatureBroadcastMessage(player, text);
		break;
	}
}
#ifdef BD_FOLLOW
void Protocol76::parseAttack(NetworkMessage &msg)
{
     if(game && player){
	unsigned long creatureid = msg.GetU32();
	game->playerSetAttackedCreature(player, creatureid);
	if(player->followMode != 0){
          game->playerSetFollowCreature(player, creatureid);
       }else
          game->playerSetFollowCreature(player, 0);
    }
}
#endif //BD_FOLLOW

#ifdef BD_FOLLOW
void Protocol76::parseFollow(NetworkMessage &msg)
{
unsigned long creatureid = msg.GetU32();
game->playerSetFollowCreature(player, creatureid);
}
#endif //BD_FOLLOW
void Protocol76::parseTextWindow(NetworkMessage &msg)
{
    unsigned long id = msg.GetU32(); 
    std::string new_text = msg.GetString(); 
    if(readItem && windowTextID == id) 
{ 
    readItem->setText(new_text); 
    readItem->releaseThing(); 
    readItem = NULL; 
} 
}

void Protocol76::parseRequestTrade(NetworkMessage &msg)
{
	Position pos = msg.GetPosition();
	unsigned short itemid = msg.GetItemId();
	unsigned char stack = msg.GetByte();
	unsigned long playerid = msg.GetU32();
	
	game->playerRequestTrade(player, pos, stack, itemid, playerid);
}

void Protocol76::parseAcceptTrade(NetworkMessage &msg)
{
	game->playerAcceptTrade(player);
}

void Protocol76::parseLookInTrade(NetworkMessage &msg)
{
	bool counterOffer = (msg.GetByte() == 0x01);
	int index = msg.GetByte();
	
	game->playerLookInTrade(player, counterOffer, index);
}

void Protocol76::parseCloseTrade()
{
	game->playerCloseTrade(player);
}

void Protocol76::parseAddVip(NetworkMessage &msg)
{
	std::string vip_name = msg.GetString();
	if(vip_name.size() > 32)
		return;
	game->requestAddVip(player, vip_name);
}

void Protocol76::parseRemVip(NetworkMessage &msg)
{
	unsigned long id = msg.GetU32();
	player->removeVIP(id);
}

void Protocol76::parseRotateItem(NetworkMessage &msg)
{
	Position pos = msg.GetPosition();
	unsigned short itemid = msg.GetItemId();
	unsigned char stackpos = msg.GetByte();
	
	game->playerRotateItem(player, pos, stackpos, itemid);
	
}

bool Protocol76::CanSee(int x, int y, int z) const
{
#ifdef __DEBUG__
	if(z < 0 || z >= MAP_LAYER) {
		std::cout << "WARNING! Protocol76::CanSee() Z-value is out of range!" << std::endl;
	}
#endif
	
	/*underground 8->15*/
	if(player->pos.z > 7 && z < 6 /*8 - 2*/) {
		return false;
	}
	/*ground level and above 7->0*/
	else if(player->pos.z <= 7 && z > 7){
		return false;
	}
	
	//negative offset means that the action taken place is on a lower floor than ourself
	int offsetz = player->pos.z - z;
	
	if ((x >= player->pos.x - 8 + offsetz) && (x <= player->pos.x + 9 + offsetz) &&
		(y >= player->pos.y - 6 + offsetz) && (y <= player->pos.y + 7 + offsetz))
		return true;
	
	return false;
}

bool Protocol76::CanSee(const Creature *c) const
{
	if(c->isRemoved == true)
		return false;
	
	return CanSee(c->pos.x, c->pos.y, c->pos.z);
}

void Protocol76::sendNetworkMessage(NetworkMessage *msg)
{
	WriteBuffer(*msg);
}

void Protocol76::AddTileUpdated(NetworkMessage &msg, const Position &pos)
{
#ifdef __DEBUG__
	std::cout << "Pop-up item from below..." << std::endl;
#endif

	if (CanSee(pos.x, pos.y, pos.z))
	{		
		msg.AddByte(0x69);
		msg.AddPosition(pos);
		
		Tile* tile = game->getTile(pos.x, pos.y, pos.z);
		if(tile) 
		{
#ifdef TRS_GM_INVISIBLE
			GetTileDescription(tile, msg, player);
#else
			GetTileDescription(tile, msg);
#endif //TRS_GM_INVISIBLE
			msg.AddByte(0);
			msg.AddByte(0xFF);
		}
		else {
			msg.AddByte(0x01);
			msg.AddByte(0xFF);
		}
	}
}

void Protocol76::sendTileUpdated(const Position &pos)
{
	NetworkMessage msg;
	
	AddTileUpdated(msg, pos);
	WriteBuffer(msg);
}

//container to container
void Protocol76::sendThingMove(const Creature *creature, const Container *fromContainer, unsigned char from_slotid,
							   const Item* fromItem, int oldFromCount, Container *toContainer, unsigned char to_slotid, const Item *toItem, int oldToCount, int count)
{
	if(player->NeedUpdateStats()) {
		player->sendStats();
	}
	
	NetworkMessage msg;
	
	//Auto-close container's
	const Container* moveContainer = dynamic_cast<const Container*>(fromItem);
	if(moveContainer){
		bool hasToContainerOpen = false;
		Position toMapPos = toContainer->getTopParent()->pos;
		
		if(toMapPos.x != 0xFFFF && std::abs(player->pos.x - toMapPos.x) <= 1 && std::abs(player->pos.y - toMapPos.y) <= 1) {
			for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {

				bool skipContainer = false;
				Container* tmpcontainer = cit->second;
				while(tmpcontainer != NULL) {
					if(tmpcontainer == moveContainer) {
						skipContainer = true;
						break;
					}

					tmpcontainer = tmpcontainer->getParent();
				}

				if(skipContainer)
					continue;

				if(cit->second == toContainer || cit->second->getTopParent()->isHoldingItem(toContainer)){
					hasToContainerOpen = true;
					break;
				}
			}
		}
		
		if(!hasToContainerOpen && !player->isHoldingContainer(toContainer)) {
			autoCloseContainers(moveContainer, msg);
		}
	}
	
	Item *container = NULL;
	for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {
		container  = cit->second;
		unsigned char cid = cit->first;
		
		if(container && container == fromContainer) {
			if(toContainer == fromContainer) {
				if(fromItem->isStackable()) {
					if(toItem && fromItem->getID() == toItem->getID() && toItem->getItemCountOrSubtype() != oldToCount) {
						//update count						
						TransformItemContainer(msg,cid,to_slotid,toItem);
						
						if(fromItem->getItemCountOrSubtype() > 0 && count != oldFromCount) {
							//update count							
							TransformItemContainer(msg,cid,from_slotid,fromItem);
						}
						else {
							//remove item							
							RemoveItemContainer(msg,cid,from_slotid);
						}
						
						//surplus items
						if(oldToCount + count > 100) {
							//add item														
							AddItemContainer(msg,cid,fromItem,oldToCount + count - 100);
						}
					}
					else {
						if(count == oldFromCount) {
							//remove item							
							RemoveItemContainer(msg,cid,from_slotid);
						}
						else {
							//update count							
							TransformItemContainer(msg,cid,from_slotid,fromItem);
						}
						
						//add item						
						AddItemContainer(msg,cid,fromItem,count);
					}
				}
				else {
					//remove item					
					RemoveItemContainer(msg,cid,from_slotid);
					
					//add item					
					AddItemContainer(msg,cid,fromItem);
				}
			}
			else {
				if(fromItem->isStackable() && fromItem->getItemCountOrSubtype() > 0 && count != oldFromCount) {
					//update count					
					TransformItemContainer(msg,cid,from_slotid,fromItem);
				}
				else {
					//remove item					
					RemoveItemContainer(msg,cid,from_slotid);
				}
			}
		}
		
		if(container && container == toContainer && toContainer != fromContainer) {
			if(fromItem->isStackable()) {
				if(toItem && fromItem->getID() == toItem->getID() && toItem->getItemCountOrSubtype() != oldToCount) {
					//update count					
					TransformItemContainer(msg,cid,to_slotid,toItem);
					
					//surplus items
					if(oldToCount + count > 100) {
						//add item						
						AddItemContainer(msg,cid,fromItem,oldToCount + count - 100);
					}
				}
				else {
					//add item					
					AddItemContainer(msg,cid,fromItem,count);
				}
			}
			else {
				//add item				
				AddItemContainer(msg,cid,fromItem);
			}
		}
	}
	
	WriteBuffer(msg);
}

//inventory to container
void Protocol76::sendThingMove(const Creature *creature, slots_t fromSlot, const Item* fromItem,
							   int oldFromCount, const Container *toContainer, unsigned char to_slotid, const Item *toItem, int oldToCount, int count)
{
	if(player->NeedUpdateStats()) {
		player->sendStats();
	}
	
	NetworkMessage msg;
	
	for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {
		if(cit->second == toContainer) {
			unsigned char cid = cit->first;
			
			if(fromItem->isStackable()) {
				if(toItem && fromItem->getID() == toItem->getID() && toItem->getItemCountOrSubtype() != oldToCount) {
					//update count					
					TransformItemContainer(msg,cid,to_slotid,toItem);
					
					//surplus items
					if(oldToCount + count > 100) {
						//add item						
						AddItemContainer(msg,cid,fromItem,oldToCount + count - 100);
					}
				}
				else {
					//add item					
					AddItemContainer(msg,cid,fromItem,count);
				}
			}
			else {
				//add item				
				AddItemContainer(msg,cid,fromItem);
			}
		}
	}
	
	if(creature == player) {
		AddPlayerInventoryItem(msg,player, fromSlot);

		//Update up-arrow
		const Container* moveContainer = dynamic_cast<const Container*>(fromItem);
		if(moveContainer) {
			for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {
				if(cit->second == moveContainer) {
					sendContainer(cit->first, cit->second);
				}
			}
		}
		//
	}
	
	WriteBuffer(msg);
}

//inventory to inventory
void Protocol76::sendThingMove(const Creature *creature, slots_t fromSlot, const Item* fromItem,
							   int oldFromCount, slots_t toSlot, const Item* toItem, int oldToCount, int count)
{
	NetworkMessage msg;
	if(creature == player) {
		if(player->NeedUpdateStats()) {
			player->sendStats();
		}
		
		AddPlayerInventoryItem(msg, player, fromSlot);
		AddPlayerInventoryItem(msg, player, toSlot);
	}
	
	WriteBuffer(msg);
}

//container to inventory
void Protocol76::sendThingMove(const Creature *creature, const Container *fromContainer,
							   unsigned char from_slotid, const Item* fromItem, int oldFromCount, slots_t toSlot, const Item *toItem, int oldToCount, int count)
{
	if(player->NeedUpdateStats()) {
		player->sendStats();
	}
	
	NetworkMessage msg;
	
	for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {
		if(cit->second == fromContainer) {
			unsigned char cid = cit->first;
			
			if(!fromItem->isStackable() || (oldFromCount == count && oldToCount + count <= 100)) {
				//remove item				
				RemoveItemContainer(msg,cid,from_slotid);
			}
			else {
				//update count				
				TransformItemContainer(msg,cid,from_slotid,fromItem);
			}
			
			if(toItem && toItem->getID() != fromItem->getID()) {
				//add item				
				AddItemContainer(msg,cid,toItem);
			}
		}
	}
	
	if(creature == player) {
		AddPlayerInventoryItem(msg,player, toSlot);

		//Update up-arrow
		const Container* moveContainer = dynamic_cast<const Container*>(fromItem);
		if(moveContainer) {
			for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {
				if(cit->second == moveContainer) {
					sendContainer(cit->first, cit->second);
				}
			}
		}
		//
	}
	else{
		const Container* moveContainer = dynamic_cast<const Container*>(fromItem);
		if(moveContainer){
			autoCloseContainers(moveContainer, msg);
		}
	}
		
	WriteBuffer(msg);
}

//container to ground
void Protocol76::sendThingMove(const Creature *creature, const Container *fromContainer, unsigned char from_slotid,
							   const Item* fromItem, int oldFromCount, const Position &toPos, const Item *toItem, int oldToCount, int count)
{
	if(player->NeedUpdateStats()) {
		player->sendStats();
	}
	
	NetworkMessage msg;

	//Auto-close container's
	const Container* moveContainer = dynamic_cast<const Container*>(fromItem);
	bool updateContainerArrow = false;

	if(moveContainer) {
		//Auto-close container's
		if(std::abs(player->pos.x - toPos.x) > 1 || std::abs(player->pos.y - toPos.y) > 1) {
			autoCloseContainers(moveContainer, msg);			
		}
		else
			updateContainerArrow = true;
	}

	if(CanSee(toPos.x, toPos.y, toPos.z)) {
		if(toItem && toItem->getID() == fromItem->getID() && fromItem->isStackable() && toItem->getItemCountOrSubtype() != oldToCount) {
			AddTileUpdated(msg, toItem->pos);
		}
		else {
			AddAppearThing(msg, toPos);
			if(fromItem->isStackable()) {
				msg.AddItem(fromItem->getID(), count);
			}
			else
				msg.AddItem(fromItem);
		}
	}
	
	for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {
		unsigned char cid = cit->first;
		if(cit->second == fromContainer) {
			
			if(!fromItem->isStackable() || fromItem->getItemCountOrSubtype() == 0 || count == oldFromCount) {
				//remove item				
				RemoveItemContainer(msg,cid,from_slotid);
			}
			else {
				//update count				
				TransformItemContainer(msg,cid,from_slotid,fromItem);
			}
		}
	}
	
	//Update up-arrow
	if(updateContainerArrow) {
		const Container* moveContainer = dynamic_cast<const Container*>(fromItem);
		if(moveContainer) {
			for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {
				if(cit->second == moveContainer) {
					sendContainer(cit->first, cit->second);
				}
			}
		}
	}
	//
	
	WriteBuffer(msg);
}

//inventory to ground
void Protocol76::sendThingMove(const Creature *creature, slots_t fromSlot,
							   const Item* fromItem, int oldFromCount, const Position &toPos, const Item *toItem, int oldToCount, int count)
{
	NetworkMessage msg;
	
	if(creature == player) {
		if(player->NeedUpdateStats()) {
			player->sendStats();
		}
		
		//Auto-closing containers
		const Container* moveContainer = dynamic_cast<const Container*>(fromItem);
		if(moveContainer) {
			//Auto-close container's
			if(std::abs(player->pos.x - toPos.x) > 1 || std::abs(player->pos.y - toPos.y) > 1) {
				autoCloseContainers(moveContainer, msg);			
			}
		}
	}
	
	if(CanSee(toPos.x, toPos.y, toPos.z)) {
		if(toItem && toItem->getID() == fromItem->getID() && fromItem->isStackable() && toItem->getItemCountOrSubtype() != oldToCount) {
			AddTileUpdated(msg, toItem->pos);
		}
		else {
			AddAppearThing(msg, toPos);
			if(fromItem->isStackable()) {
				msg.AddItem(fromItem->getID(), count);
			}
			else
				msg.AddItem(fromItem);
		}
	}
	
	if(creature == player) {
		AddPlayerInventoryItem(msg, player, fromSlot);
	}
	
	WriteBuffer(msg);
}

//ground to container
void Protocol76::sendThingMove(const Creature *creature, const Position &fromPos, int stackpos, const Item* fromItem,
							   int oldFromCount, const Container *toContainer, unsigned char to_slotid, const Item *toItem, int oldToCount, int count)
{
	if(player->NeedUpdateStats()) {
		player->sendStats();
	}
	
	NetworkMessage msg;
	
	//Auto-closing containers
	const Container* moveContainer = dynamic_cast<const Container*>(fromItem);
	bool updateContainerArrow = false;
	if(moveContainer) {
		bool hasToContainerOpen = false;
		Position toMapPos = toContainer->getTopParent()->pos;
		
		if(toMapPos.x != 0xFFFF && std::abs(player->pos.x - toMapPos.x) <= 1 && std::abs(player->pos.y - toMapPos.y) <= 1) {
			for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit){

				bool skipContainer = false;
				Container* tmpcontainer = cit->second;
				while(tmpcontainer != NULL) {
					if(tmpcontainer == moveContainer) {
						skipContainer = true;
						break;
					}

					tmpcontainer = tmpcontainer->getParent();
				}

				if(skipContainer)
					continue;

				if(cit->second == toContainer || cit->second->getTopParent()->isHoldingItem(toContainer)){
					hasToContainerOpen = true;
					break;
				}
			}
		}
		
		if(!hasToContainerOpen && !player->isHoldingContainer(toContainer)){
			autoCloseContainers(moveContainer, msg);
		}
		else
			updateContainerArrow = true;
	}
	//
	
	if(CanSee(fromPos.x, fromPos.y, fromPos.z)) {
		if(!fromItem->isStackable() || (oldFromCount == count && oldToCount + count <= 100)) {
			AddRemoveThing(msg, fromPos, stackpos);
		}
		else
			AddTileUpdated(msg, fromPos);
	}
	
	Container *container = NULL;
	for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {
		container = cit->second;
		if(container == toContainer) {
			unsigned char cid = cit->first;
			
			if(fromItem->isStackable()) {
				if(toItem && fromItem->getID() == toItem->getID() && toItem->getItemCountOrSubtype() != oldToCount) {
					//update count					
					TransformItemContainer(msg,cid,to_slotid,toItem);
					
					//surplus items
					if(oldToCount + count > 100) {
						//add item						
						AddItemContainer(msg,cid,fromItem,oldToCount + count - 100);
					}
				}
				else {
					//add item					
					AddItemContainer(msg,cid,fromItem,count);
				}
			}
			else {
				//add item				
				AddItemContainer(msg,cid,fromItem);
			}
		}
	}
	
	if(updateContainerArrow) {
		const Container* moveContainer = dynamic_cast<const Container*>(fromItem);
		if(moveContainer) {
			for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {
				
				if(cit->second == moveContainer) {
					sendContainer(cit->first, cit->second);
				}
			}
		}
	}
	
	WriteBuffer(msg);
}

//ground to inventory
void Protocol76::sendThingMove(const Creature *creature, const Position &fromPos, int stackpos, const Item* fromItem,
							   int oldFromCount, slots_t toSlot, const Item *toItem, int oldToCount, int count)
{
	if(player->NeedUpdateStats()) {
		player->sendStats();
	}
	
	NetworkMessage msg;
	
	if(creature == player) {
		AddPlayerInventoryItem(msg, player, toSlot);
	}
	else {
		//Auto-closing containers
		const Container* moveContainer = dynamic_cast<const Container*>(fromItem);
		if(moveContainer) {
			autoCloseContainers(moveContainer, msg);						
		}
	}
	
	if(CanSee(fromPos.x, fromPos.y, fromPos.z)) {
		if(!fromItem->isStackable() || (oldFromCount == count && oldToCount + count <= 100)) {
			AddRemoveThing(msg, fromPos, stackpos);
			
			if(toItem && (!toItem->isStackable() || toItem->getID() != fromItem->getID())) {
				AddAppearThing(msg, fromPos);
				msg.AddItem(toItem);
			}
		}
		else
			AddTileUpdated(msg, fromPos);
	}
	
	WriteBuffer(msg);
}

//ground to ground
void Protocol76::sendThingMove(const Creature *creature, const Thing *thing,
							   const Position *oldPos, unsigned char oldStackPos, unsigned char oldcount, unsigned char count, bool tele)
{
	NetworkMessage msg;
	
	const Creature* c = dynamic_cast<const Creature*>(thing);
	if(c && c->isRemoved)
		return;
	
	if (!tele && c && (CanSee(oldPos->x, oldPos->y, oldPos->z)) && (CanSee(thing->pos.x, thing->pos.y, thing->pos.z)))
	{
		msg.AddByte(0x6D);
		msg.AddPosition(*oldPos);
		msg.AddByte(oldStackPos);
		msg.AddPosition(thing->pos);
		
		Tile *fromTile = game->getTile(oldPos->x, oldPos->y, oldPos->z);
		if(fromTile && fromTile->getThingCount() > 8) {
			//We need to pop up this item
			Thing *newthing = fromTile->getThingByStackPos(9);
			
			if(newthing != NULL) {
				AddTileUpdated(msg, *oldPos);
			}
		}
	}
	else
	{
		if (!tele && CanSee(oldPos->x, oldPos->y, oldPos->z))
		{
			AddRemoveThing(msg,*oldPos,oldStackPos);
		}
		
		if (!(tele && thing == this->player) && CanSee(thing->pos.x, thing->pos.y, thing->pos.z))
		{
			AddAppearThing(msg,thing->pos);      
			if (c) {
				bool known;
				unsigned long removedKnown;
				checkCreatureAsKnown(((Creature*)thing)->getID(), known, removedKnown);
				AddCreature(msg,(Creature*)thing, known, removedKnown);
			}
			else {
				msg.AddItem((Item*)thing);
				
				//Auto-close container's
				const Container* moveContainer = dynamic_cast<const Container*>(thing);
				if(moveContainer){
					if(std::abs(player->pos.x - thing->pos.x) > 1 || std::abs(player->pos.y - thing->pos.y) > 1 || player->pos.z != thing->pos.z ){
						autoCloseContainers(moveContainer, msg);
					}
				}
			}
		}
	}
	
	if (thing == this->player) {
#ifdef TRS_GM_INVISIBLE
		if(tele){
			msg.AddByte(0x64);
			msg.AddPosition(player->pos);
			GetMapDescription(player->pos.x-8, player->pos.y-6, player->pos.z, 18, 14, msg, this->player);
		}
		else{
			if (oldPos->y > thing->pos.y) { // north, for old x
				msg.AddByte(0x65);
				GetMapDescription(oldPos->x - 8, thing->pos.y - 6, thing->pos.z, 18, 1, msg, this->player);
			} else if (oldPos->y < thing->pos.y) { // south, for old x
				msg.AddByte(0x67);
				GetMapDescription(oldPos->x - 8, thing->pos.y + 7, thing->pos.z, 18, 1, msg, this->player);
			}
			if (oldPos->x < thing->pos.x) { // east, [with new y]
				msg.AddByte(0x66);
				GetMapDescription(thing->pos.x + 9, thing->pos.y - 6, thing->pos.z, 1, 14, msg, this->player);
			} else if (oldPos->x > thing->pos.x) { // west, [with new y]
				msg.AddByte(0x68);
				GetMapDescription(thing->pos.x - 8, thing->pos.y - 6, thing->pos.z, 1, 14, msg, this->player);
			}
		}
#else //TRS_GM_INVISIBLE
		if(tele){
			msg.AddByte(0x64); 
			msg.AddPosition(player->pos); 
			GetMapDescription(player->pos.x-8, player->pos.y-6, player->pos.z, 18, 14, msg); 
		}
		else{               
			if (oldPos->y > thing->pos.y) { // north, for old x
				msg.AddByte(0x65);
				GetMapDescription(oldPos->x - 8, thing->pos.y - 6, thing->pos.z, 18, 1, msg);
			} else if (oldPos->y < thing->pos.y) { // south, for old x
				msg.AddByte(0x67);
				GetMapDescription(oldPos->x - 8, thing->pos.y + 7, thing->pos.z, 18, 1, msg);
			}
			if (oldPos->x < thing->pos.x) { // east, [with new y]
				msg.AddByte(0x66);
				GetMapDescription(thing->pos.x + 9, thing->pos.y - 6, thing->pos.z, 1, 14, msg);
			} else if (oldPos->x > thing->pos.x) { // west, [with new y]
				msg.AddByte(0x68);
				GetMapDescription(thing->pos.x - 8, thing->pos.y - 6, thing->pos.z, 1, 14, msg);
			}
		}
#endif //TRS_GM_INVISIBLE

		//Auto-close container's
		std::vector<Container *> containers;
		for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {
			Container *container = cit->second;
			
			while(container->getParent()) {
				container = container->getParent();
			}
			
			//Only add those we need to close
			if(container->pos.x != 0xFFFF) {				
				if(std::abs(player->pos.x - container->pos.x) > 1 || std::abs(player->pos.y - container->pos.y) > 1 || player->pos.z != container->pos.z) {
					containers.push_back(cit->second);
				}
			}
		}
		
		for(std::vector<Container *>::const_iterator it = containers.begin(); it != containers.end(); ++it) {
			autoCloseContainers(*it, msg);
		}
	}	
	
	WriteBuffer(msg);
}


//close container and its child containers
void Protocol76::autoCloseContainers(const Container *container, NetworkMessage &msg)
{
	std::vector<unsigned char> containerlist;
	for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {
		Container *tmpcontainer = cit->second;
		while(tmpcontainer != NULL) {
			if(tmpcontainer == container) {
				containerlist.push_back(cit->first);
				break;
			}
			
			tmpcontainer = tmpcontainer->getParent();
		}
	}
	
	for(std::vector<unsigned char>::iterator it = containerlist.begin(); it != containerlist.end(); ++it) {
		player->closeContainer(*it);
		msg.AddByte(0x6F);
		msg.AddByte(*it);
	}						
}

void Protocol76::sendCreatureTurn(const Creature *creature, unsigned char stackPos)
{
	if (CanSee(creature))
	{
		NetworkMessage msg;
		
		msg.AddByte(0x6B);
		msg.AddPosition(creature->pos);
		msg.AddByte(stackPos); 
		
		msg.AddByte(0x63);
		msg.AddByte(0x00);
		msg.AddU32(creature->getID());
		msg.AddByte(creature->getDirection());
		WriteBuffer(msg);
	}
}


void Protocol76::sendCreatureSay(const Creature *creature, SpeakClasses type, const std::string &text)
{
	NetworkMessage msg;
	AddCreatureSpeak(msg,creature, type, text, 0);
	WriteBuffer(msg);
}

void Protocol76::sendToChannel(const Creature * creature, SpeakClasses type, const std::string &text, unsigned short channelId){
	NetworkMessage msg;
	AddCreatureSpeak(msg,creature, type, text, channelId);
	WriteBuffer(msg);
}

void Protocol76::sendCancel(const char *msg)
{
	NetworkMessage netmsg;
	AddTextMessage(netmsg,MSG_SMALLINFO, msg);
	WriteBuffer(netmsg);
}

void Protocol76::sendCancelAttacking()
{
	NetworkMessage netmsg;
	netmsg.AddByte(0xa3);
	WriteBuffer(netmsg);
}

void Protocol76::sendChangeSpeed(const Creature *creature)
{
	NetworkMessage netmsg;
	netmsg.AddByte(0x8F);
	
	netmsg.AddU32(creature->getID());
	netmsg.AddU16(creature->getSpeed());
	WriteBuffer(netmsg);
}

void Protocol76::sendCancelWalk()
{
	NetworkMessage netmsg;
	netmsg.AddByte(0xB5);
	netmsg.AddByte(player->getDirection()); // direction
	WriteBuffer(netmsg);
}

void Protocol76::sendThingDisappear(const Thing *thing, unsigned char stackPos, bool tele)
{
	NetworkMessage msg;
	const Creature* creature = dynamic_cast<const Creature*>(thing);
	//Auto-close trade
	if(player->getTradeItem() && dynamic_cast<const Item*>(thing) == player->getTradeItem()) {
		game->playerCloseTrade(player);
	}
	
	if(creature && creature->isRemoved)
		return;
	
	if(!tele) {		
		if(creature && creature->health > 0){
			const Player* remove_player = dynamic_cast<const Player*>(creature);
			if(remove_player == player)
				return;
			
			if(CanSee(creature)){
				AddMagicEffect(msg,thing->pos, NM_ME_PUFF);
			}
		}
	}
	
	if(CanSee(thing->pos.x, thing->pos.y, thing->pos.z)) {
		AddRemoveThing(msg,thing->pos, stackPos);
		if(creature && stackPos > 9){
			AddCreatureHealth(msg,creature);
		}
		Tile *tile = game->getTile(thing->pos.x, thing->pos.y, thing->pos.z);
		if(tile && tile->getThingCount() > 8) {
			//We need to pop up this item
			Thing *newthing = tile->getThingByStackPos(9);
			if(newthing != NULL) {
				AddTileUpdated(msg, thing->pos);
			}
		}
	}
	
	WriteBuffer(msg);
}

void Protocol76::sendThingAppear(const Thing *thing){
	NetworkMessage msg;
	const Creature* creature = dynamic_cast<const Creature*>(thing);
	if(creature){
		const Player* add_player = dynamic_cast<const Player*>(creature);
		if(add_player == player){
			msg.AddByte(0x0A);
			msg.AddU32(player->getID());
			
			msg.AddByte(0x32);
			msg.AddByte(0x00);
			
			msg.AddByte(0x00); //can report bugs 0,1
			
			/*msg.AddByte(0x0B);//TODO?. GM actions
			  msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
			  msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
			  msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
			  msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
			  msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
			  msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
			  msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
			  msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
			*/
			
			msg.AddByte(0x64);
			msg.AddPosition(player->pos);
#ifdef TRS_GM_INVISIBLE
			GetMapDescription(player->pos.x-8, player->pos.y-6, player->pos.z, 18, 14, msg, player);
#else
			GetMapDescription(player->pos.x-8, player->pos.y-6, player->pos.z, 18, 14, msg);
#endif //TRS_GM_INVISIBLE
			AddMagicEffect(msg,player->pos, NM_ME_ENERGY_AREA);
			
			AddPlayerStats(msg,player);	
			
			msg.AddByte(0x82);
#ifdef CVS_DAY_CYCLE
			msg.AddByte(game->getLightLevel());
#else
			msg.AddByte(0x6F); //LIGHT LEVEL
#endif //CVS_DAY_CYCLE
			msg.AddByte(0xD7);//light? (seems constant)
			
			/*msg.AddByte(0x8d);//8d
			  msg.AddU32(player->getID());
			  msg.AddByte(0x03);//00
			  msg.AddByte(0xd7);//d7
			*/
			
			AddPlayerSkills(msg,player);
			
			AddPlayerInventoryItem(msg,player, 1);
			AddPlayerInventoryItem(msg,player, 2);
			AddPlayerInventoryItem(msg,player, 3);
			AddPlayerInventoryItem(msg,player, 4);
			AddPlayerInventoryItem(msg,player, 5);
			AddPlayerInventoryItem(msg,player, 6);
			AddPlayerInventoryItem(msg,player, 7);
			AddPlayerInventoryItem(msg,player, 8);
			AddPlayerInventoryItem(msg,player, 9);
			AddPlayerInventoryItem(msg,player, 10);
			
			AddTextMessage(msg,MSG_EVENT, g_config.getGlobalString("loginmsg", "Welcome.").c_str());
			std::string tempstring;
			tempstring = "Your last visit was on ";
			time_t lastlogin = player->getLastLoginSaved();
			tempstring += ctime(&lastlogin);
			tempstring.erase(tempstring.length() -1);
			tempstring += ".";

#ifdef YUR_CVS_MODS
			if (lastlogin == 0)
				AddTextMessage(msg,MSG_EVENT,"It is your first visit.");
			else
#endif //YUR_CVS_MODS
				AddTextMessage(msg,MSG_EVENT, tempstring.c_str());

			WriteBuffer(msg);
			
#ifdef ELEM_VIP_LIST
			for(int i = 0; i < MAX_VIPS; i++)
			{
				if(!player->vip[i].empty())
				{
					Creature* c = game->getCreatureByName(player->vip[i].c_str());
					Player* p = dynamic_cast<Player*>(c);

					NetworkMessage msgs;
					msgs.AddByte(0xD2);
					msgs.AddU32(i+1);
					msgs.AddString(player->vip[i]);
					if(p)
						msgs.AddByte(0x01);
					else
						msgs.AddByte(0x02);
					WriteBuffer(msgs);
				}
			}
#else //ELEM_VIP_LIST
			for(VIPListSet::iterator it = player->VIPList.begin(); it != player->VIPList.end(); it++){
				bool online;
				std::string vip_name;
				
				if(IOPlayer::instance()->getNameByGuid((*it), vip_name)){
					online = (game->getPlayerByName(vip_name) != NULL);
					sendVIP((*it), vip_name, online);
				}
			}
#endif //ELEM_VIP_LIST
			
			//force flush
			flushOutputBuffer();
			return;
		}
		else if(CanSee(creature)){
			bool known;
			unsigned long removedKnown;
			checkCreatureAsKnown(creature->getID(), known, removedKnown);
			AddAppearThing(msg,creature->pos);
			AddCreature(msg,creature, known, removedKnown);		
			// login bubble
			AddMagicEffect(msg,creature->pos, NM_ME_ENERGY_AREA);
		}
	}
	else if(CanSee(thing->pos.x, thing->pos.y, thing->pos.z))
	{
		const Item *item = dynamic_cast<const Item*>(thing);
		if(item){
			Tile *tile = game->getTile(item->pos.x,item->pos.y,item->pos.z);
			if(tile->getThingCount() > 8){
				AddTileUpdated(msg,item->pos);
			}
			else{
				AddAppearThing(msg,item->pos);
				msg.AddItem(item);
			}
		}
	}
	
	WriteBuffer(msg);
}

void Protocol76::sendThingTransform(const Thing* thing,int stackpos){
	if(CanSee(thing->pos.x, thing->pos.y, thing->pos.z)) {
		const Item *item = dynamic_cast<const Item*>(thing);
		if(item){
			NetworkMessage msg;
			if(stackpos == 0){
				AddTileUpdated(msg,thing->pos);
			}
			else if(stackpos < 10){
				msg.AddByte(0x6B);
				msg.AddPosition(thing->pos);
				msg.AddByte(stackpos);
				msg.AddItem(item);
			}
			else{
				return;
			}
			WriteBuffer(msg);
		}
		//update container icon
		if(dynamic_cast<const Container*>(item)){
			const Container *updateContainer = dynamic_cast<const Container*>(item);
			for(containerLayout::const_iterator cit = player->getContainers(); cit != player->getEndContainer(); ++cit) {
				Container *container = cit->second;
				if(container == updateContainer) {
					sendContainer(cit->first, container);
				}
			}
		}
	}
}

void Protocol76::sendSkills()
{
	NetworkMessage msg;
	AddPlayerSkills(msg,player);
	WriteBuffer(msg);
}

void Protocol76::sendPing()
{
	NetworkMessage msg;
	msg.AddByte(0x1E);
	WriteBuffer(msg);
}

void Protocol76::sendThingRemove(const Thing *thing){
	//Auto-close trade
	if(player->getTradeItem() && dynamic_cast<const Item*>(thing) == player->getTradeItem()) {
		game->playerCloseTrade(player);
	}
	
	NetworkMessage msg;

	//Auto-close container's
	const Container* moveContainer = dynamic_cast<const Container *>(thing);
	if(moveContainer) {
		autoCloseContainers(moveContainer, msg);		
	}

	WriteBuffer(msg);
}

void Protocol76::sendDistanceShoot(const Position &from, const Position &to, unsigned char type){
	NetworkMessage msg;
	AddDistanceShoot(msg,from, to,type);
	WriteBuffer(msg);
}
void Protocol76::sendMagicEffect(const Position &pos, unsigned char type){
	NetworkMessage msg;
	AddMagicEffect(msg,pos,type);
	WriteBuffer(msg);
}
void Protocol76::sendAnimatedText(const Position &pos, unsigned char color, std::string text){
	NetworkMessage msg;
	AddAnimatedText(msg,pos,color,text);
	WriteBuffer(msg);
}
void Protocol76::sendCreatureHealth(const Creature *creature){
	NetworkMessage msg;
	AddCreatureHealth(msg,creature);
	WriteBuffer(msg);
}

void Protocol76::sendItemAddContainer(const Container *container, const Item *item){
	NetworkMessage msg;
	unsigned char cid = player->getContainerID(container);
	if(cid != 0xFF){
		AddItemContainer(msg,cid,item);
		WriteBuffer(msg);
	}
}

void Protocol76::sendItemRemoveContainer(const Container *container, const unsigned char slot)   {
	NetworkMessage msg;
	unsigned char cid = player->getContainerID(container);
	if(cid != 0xFF){
		RemoveItemContainer(msg,cid,slot);
		WriteBuffer(msg);
	}
}

void Protocol76::sendItemUpdateContainer(const Container *container, const Item* item,const unsigned char slot){
	NetworkMessage msg;
	unsigned char cid = player->getContainerID(container);
	if(cid != 0xFF){
		TransformItemContainer(msg,cid,slot,item);
		WriteBuffer(msg);
	}
}

void Protocol76::sendTextWindow(Item* item,const unsigned short maxlen, const bool canWrite){
	NetworkMessage msg;
	msg.AddByte(0x96);
	windowTextID++;
	msg.AddU32(windowTextID);
	//msg.AddU16(item->getID());
	msg.AddItemId(item);
	if(canWrite){
		msg.AddU16(maxlen);		
		msg.AddString(item->getText());		
		item->useThing();
		readItem = item;
	}
	else{		
		msg.AddU16((unsigned short)item->getText().size());
		msg.AddString(item->getText());									
		readItem = NULL;		
	}
	msg.AddString("unknown");
	
	WriteBuffer(msg);
}

void Protocol76::sendVIPLogIn(unsigned long guid)
{
	NetworkMessage msg;
	msg.AddByte(0xD3);
	msg.AddU32(guid);
	WriteBuffer(msg);
}

void Protocol76::sendVIPLogOut(unsigned long guid)
{
	NetworkMessage msg;
	msg.AddByte(0xD4);
	msg.AddU32(guid);
	WriteBuffer(msg);
}

void Protocol76::sendVIP(unsigned long guid, const std::string &name, bool isOnline)
{
	NetworkMessage msg;
	
	msg.AddByte(0xD2);
	msg.AddU32(guid);
	msg.AddString(name);
	msg.AddByte(isOnline == true ? 1 : 0);
	
	WriteBuffer(msg);
}

////////////// Add common messages
void Protocol76::AddTextMessage(NetworkMessage &msg,MessageClasses mclass, const char* message)
{
	msg.AddByte(0xB4);
	msg.AddByte(mclass);
	msg.AddString(message);
}

void Protocol76::AddAnimatedText(NetworkMessage &msg,const Position &pos, unsigned char color, std::string text)
{
#ifdef __DEBUG__
	if(text.length() == 0) {
		std::cout << "Warning: 0-Length string in AddAnimatedText()" << std::endl;
	}
#endif
	
	msg.AddByte(0x84); 
	msg.AddPosition(pos);
	msg.AddByte(color);
	msg.AddString(text);
}


void Protocol76::AddMagicEffect(NetworkMessage &msg,const Position &pos, unsigned char type)
{
	msg.AddByte(0x83);
	msg.AddPosition(pos);
	msg.AddByte(type+1);
}


void Protocol76::AddDistanceShoot(NetworkMessage &msg,const Position &from, const Position &to, unsigned char type)
{
	msg.AddByte(0x85); 
	msg.AddPosition(from);
	msg.AddPosition(to);
	msg.AddByte(type+1);
}


void Protocol76::AddCreature(NetworkMessage &msg,const Creature *creature, bool known, unsigned int remove)
{
	if (known)
	{
		msg.AddByte(0x62);
		msg.AddByte(0x00);
		msg.AddU32(creature->getID());
	}
	else
	{
		msg.AddByte(0x61);
		msg.AddByte(0x00);
		//AddU32(0);
		msg.AddU32(remove);
		msg.AddU32(creature->getID());
		msg.AddString(creature->getName());
	}
	
	msg.AddByte(std::max(1, creature->health*100/creature->healthmax));
	
	msg.AddByte((unsigned char)creature->getDirection());
	
#ifdef BD_ITEM_LOOK
	if (creature->looktype > 1000)
	{
		msg.AddByte(0x00);
		msg.AddU16(Item::items[creature->looktype].clientId);
	}
	else
#endif //BD_ITEM_LOOK
	{
#ifdef YUR_INVISIBLE
		if (creature->isInvisible())
		{
			msg.AddByte(0);
			msg.AddU16(0);
		}
		else
#endif //YUR_INVISIBLE
		{
			msg.AddByte(creature->looktype);
			msg.AddByte(creature->lookhead);
			msg.AddByte(creature->lookbody);
			msg.AddByte(creature->looklegs);
			msg.AddByte(creature->lookfeet);
		}
	}
	
#ifdef CVS_DAY_CYCLE
	const Player* p = dynamic_cast<const Player*>(creature);
	if (p)
	{
		msg.AddByte(p->getLightLevel());
		msg.AddByte(p->getLightColor());
	}
	else
#endif //CVS_DAY_CYCLE
	{
		msg.AddByte(0x00); // light level
		msg.AddByte(0xD7); // light color
	}

	msg.AddU16(creature->getSpeed());
	
#ifdef TLM_SKULLS_PARTY
	Creature *ctarget = game->getCreatureByID(creature->getID());
	Player*target=dynamic_cast<Player*>(ctarget);
	std::vector<Player*>::iterator invited = std::find(player->invitedplayers.begin(), player->invitedplayers.end(), target);
	std::vector<Player*>::iterator inviter = std::find(player->inviterplayers.begin(), player->inviterplayers.end(), target);

	if(target && target->skullType == SKULL_NONE && target->party != 0 && player->party == target->party)
		msg.AddByte(2);
	else
		msg.AddByte(creature->skullType);

	if(target && target->party != 0 && player->party != 0 && target->party == player->party)
	{
		if(target->getID() == player->party)
			msg.AddByte(4);
		else
			msg.AddByte(3);
	}
	else if(invited != player->invitedplayers.end())
		msg.AddByte(2);
	else if(inviter != player->inviterplayers.end())
		msg.AddByte(1);
	else
		msg.AddByte(0x00);
#else
	msg.AddByte(0x00); // skull
	msg.AddByte(0x00); // shield
#endif //TLM_SKULLS_PARTY
}


void Protocol76::AddPlayerStats(NetworkMessage &msg,const Player *player)
{
	msg.AddByte(0xA0);
	msg.AddU16(player->getHealth());
	msg.AddU16(player->getPlayerInfo(PLAYERINFO_MAXHEALTH));
	msg.AddU16((unsigned short)std::floor(player->getFreeCapacity()));

#ifdef YUR_HIGH_LEVELS
	if (player->getPlayerInfo(PLAYERINFO_LEVEL) > 65535)
	{
		msg.AddU32(player->getPlayerInfo(PLAYERINFO_LEVEL));
		msg.AddU16(0);
	}
	else if (player->getExperience() > 2000000000L)
	{
		msg.AddU32(0);
		msg.AddU16(player->getPlayerInfo(PLAYERINFO_LEVEL));
	}
	else
	{
		msg.AddU32((unsigned long)player->getExperience());
		msg.AddU16(player->getPlayerInfo(PLAYERINFO_LEVEL));
	}
#else
	msg.AddU32(player->getExperience());
	msg.AddU16(player->getPlayerInfo(PLAYERINFO_LEVEL));
#endif //YUR_HIGH_LEVELS

	msg.AddByte(player->getPlayerInfo(PLAYERINFO_LEVELPERCENT));
	msg.AddU16(player->getMana());
	msg.AddU16(player->getPlayerInfo(PLAYERINFO_MAXMANA));
	msg.AddByte(player->getMagicLevel());
	msg.AddByte(player->getPlayerInfo(PLAYERINFO_MAGICLEVELPERCENT));
	msg.AddByte(player->getPlayerInfo(PLAYERINFO_SOUL));
}

void Protocol76::AddPlayerSkills(NetworkMessage &msg,const Player *player)
{
	msg.AddByte(0xA1);
	
	msg.AddByte(player->getSkill(SKILL_FIST,   SKILL_LEVEL));
	msg.AddByte(player->getSkill(SKILL_FIST,   SKILL_PERCENT));
	msg.AddByte(player->getSkill(SKILL_CLUB,   SKILL_LEVEL));
	msg.AddByte(player->getSkill(SKILL_CLUB,   SKILL_PERCENT));
	msg.AddByte(player->getSkill(SKILL_SWORD,  SKILL_LEVEL));
	msg.AddByte(player->getSkill(SKILL_SWORD,  SKILL_PERCENT));
	msg.AddByte(player->getSkill(SKILL_AXE,    SKILL_LEVEL));
	msg.AddByte(player->getSkill(SKILL_AXE,    SKILL_PERCENT));
	msg.AddByte(player->getSkill(SKILL_DIST,   SKILL_LEVEL));
	msg.AddByte(player->getSkill(SKILL_DIST,   SKILL_PERCENT));
	msg.AddByte(player->getSkill(SKILL_SHIELD, SKILL_LEVEL));
	msg.AddByte(player->getSkill(SKILL_SHIELD, SKILL_PERCENT));
	msg.AddByte(player->getSkill(SKILL_FISH,   SKILL_LEVEL));
	msg.AddByte(player->getSkill(SKILL_FISH,   SKILL_PERCENT));
}


void Protocol76::AddPlayerInventoryItem(NetworkMessage &msg,const Player *player, int item)
{
	if (player->getItem(item) == NULL)
	{
		msg.AddByte(0x79);
		msg.AddByte(item);
	}
	else
	{
		msg.AddByte(0x78);
		msg.AddByte(item);
		msg.AddItem(player->getItem(item));
	}
}


void Protocol76::AddCreatureSpeak(NetworkMessage &msg,const Creature *creature, SpeakClasses  type, std::string text, unsigned short channelId)
{
	msg.AddByte(0xAA);
	msg.AddString(creature->getName());
	msg.AddByte(type);
	switch(type){
	case SPEAK_SAY:
	case SPEAK_WHISPER:
	case SPEAK_YELL:
	case SPEAK_MONSTER1:
	case SPEAK_MONSTER2:
		msg.AddPosition(creature->pos);
		break;	
	case SPEAK_CHANNEL_Y:
	case SPEAK_CHANNEL_R1:
	case SPEAK_CHANNEL_R2:
    case SPEAK_CHANNEL_O:
		msg.AddU16(channelId);
		break;	
	}
	msg.AddString(text);
}

void Protocol76::AddCreatureHealth(NetworkMessage &msg,const Creature *creature)
{
	msg.AddByte(0x8C);
	msg.AddU32(creature->getID());
	msg.AddByte(std::max(1, creature->health*100/creature->healthmax));
}

void Protocol76::AddRemoveThing(NetworkMessage &msg, const Position &pos,unsigned char stackpos){
	if(stackpos < 10) {
		if(CanSee(pos.x, pos.y, pos.z)) {
			msg.AddByte(0x6C);
			msg.AddPosition(pos);
			msg.AddByte(stackpos);
		}
	}
	else {
		//This will cause some problem, we remove an item (example: a player gets removed due to death) from the map, but the client cant see it
		//(above the 9 limit), real tibia has the same problem so I don't think there is a way to fix this.
		//Problem: The client won't be informed that the player has been killed
		//and will show the player as alive (0 hp).
		//Solution: re-log.
	}
	
	Tile *fromTile = game->getTile(pos.x, pos.y, pos.z);
	if(fromTile && fromTile->getThingCount() > 8) {
		//We need to pop up this item
		Thing *newthing = fromTile->getThingByStackPos(9);
		
		if(newthing != NULL) {
			AddTileUpdated(msg, pos);
		}
	}
}

void Protocol76::AddAppearThing(NetworkMessage &msg, const Position &pos){
	if(CanSee(pos.x, pos.y, pos.z)) {		
		msg.AddByte(0x6A);
		msg.AddPosition(pos);	
	}
}

void Protocol76::AddItemContainer(NetworkMessage &msg,unsigned char cid, const Item *item){
	msg.AddByte(0x70);
	msg.AddByte(cid);
	msg.AddItem(item);
}

void Protocol76::AddItemContainer(NetworkMessage &msg,unsigned char cid, const Item *item,unsigned char count){
	msg.AddByte(0x70);
	msg.AddByte(cid);
	msg.AddItem(item->getID(), count);
}

void Protocol76::TransformItemContainer(NetworkMessage &msg,unsigned char cid,unsigned char slot,const  Item *item){
	msg.AddByte(0x71);
	msg.AddByte(cid);
	msg.AddByte(slot);
	msg.AddItem(item);
}

void Protocol76::RemoveItemContainer(NetworkMessage &msg,unsigned char cid,unsigned char slot){
	msg.AddByte(0x72);
	msg.AddByte(cid);
	msg.AddByte(slot);
}

//////////////////////////

void Protocol76::flushOutputBuffer(){
	OTSYS_THREAD_LOCK_CLASS lockClass(bufferLock, "Protocol76::flushOutputBuffer()");
	//force writetosocket	
	OutputBuffer.WriteToSocket(s);
	OutputBuffer.Reset();
	
	return;
}

void Protocol76::WriteBuffer(NetworkMessage &add){
	
	game->addPlayerBuffer(player);	
	
	OTSYS_THREAD_LOCK(bufferLock, "Protocol76::WriteBuffer")
		if(OutputBuffer.getMessageLength() + add.getMessageLength() >= NETWORKMESSAGE_MAXSIZE - 16){
			this->flushOutputBuffer();
		}	
		OutputBuffer.JoinMessages(add);	
		OTSYS_THREAD_UNLOCK(bufferLock, "Protocol76::WriteBuffer")	
			return;
}


#ifdef CVS_DAY_CYCLE
void Protocol76::sendWorldLightLevel(unsigned char lightlevel, unsigned char color)
{ 
	NetworkMessage msg; 
	msg.AddByte(0x82);
	msg.AddByte(lightlevel);		 // 6F - Light level
	msg.AddByte(color /*0xA7*/); // D7 - Light color
	WriteBuffer(msg);
}

void Protocol76::sendPlayerLightLevel(const Player* player)
{
	NetworkMessage msg;
	msg.AddByte(0x8D);
	msg.AddU32(player->getID());
	msg.AddByte(player->getLightLevel());
	msg.AddByte(player->getLightColor());		//
	WriteBuffer(msg);
}
#endif //CVS_DAY_CYCLE


#ifdef BD_HOUSE_WINDOW
void Protocol76::sendHouseWindow(std::string members)
{
	NetworkMessage msg;
	msg.AddByte(0x97);
	msg.AddByte(0x01);// window ID?
	msg.AddByte(0x4E);//  U16 Part 1     ItemId
	msg.AddByte(0xC5);//  U16 Part 2
	msg.AddU16((unsigned short)members.size());
	msg.AddString(members);
	sendNetworkMessage(&msg);
}

void Protocol76::parseHouseWindow(NetworkMessage& msg)
{
	unsigned long id     = msg.GetU32();
	unsigned short size  = msg.GetU16();
	msg.GetByte();
	std::string new_text = msg.GetRaw();
	player->receiveHouseWindow(new_text);
}
#endif //BD_HOUSE_WINDOW


#ifdef ELEM_VIP_LIST
void Protocol76::elemParseAddVip(NetworkMessage &msg)
{
	std::string name = msg.GetString();
	Creature* c = game->getCreatureByName(name.c_str());
	Player* p = dynamic_cast<Player*>(c);

	if (game->isPlayer(name.c_str()))
	{
		bool success = false;
		for (int i = 0; i < MAX_VIPS; i++)
		{
			if (player->vip[i].empty())
			{
				NetworkMessage msgs;
				msgs.AddByte(0xD2);
				msgs.AddU32(i+1);
				msgs.AddString(name);
				if(p)
					msgs.AddByte(0x01);
				else
					msgs.AddByte(0x02);

				msgs.WriteToSocket(s);
				player->vip[i] = name;
				success = true;
				break;
			}
		}
		if (!success)
			player->sendCancel("Your VIP list is currently full, please remove some of your VIP's before adding more.");
	}
	else
		player->sendCancel("A player with that name does not exist.");
}

void Protocol76::elemParseRemoveVip(NetworkMessage &msg)
{
	unsigned long id = msg.GetU32();
	if(id >= 1 && id <= MAX_VIPS)
		player->vip[id-1].clear();
}
#endif //ELEM_VIP_LIST


#ifdef TLM_SKULLS_PARTY
void Protocol76::sendSkull(const Creature *creature)
{
  NetworkMessage msg;
  msg.AddByte(0x90);
  msg.AddU32(creature->getID());
  msg.AddByte(creature->skullType);
  WriteBuffer(msg);
}

void Protocol76::sendPartyIcons(const Player *playa, int icontype, bool skull, bool removeskull)
{
  NetworkMessage msg;
  msg.AddByte(0x91);
  msg.AddU32(playa->getID());
  msg.AddByte(icontype);
  WriteBuffer(msg);
  msg.Reset();
  if(skull || removeskull){
  msg.AddByte(0x90);
  msg.AddU32(playa->getID());
  if(skull)
  msg.AddByte(2);
  if(removeskull)
  msg.AddByte(0);
  WriteBuffer(msg);
  }
}

void Protocol76::parseInviteParty(NetworkMessage &msg)
{
	unsigned long creatureid = msg.GetU32();
	Creature* creature = game->getCreatureByID(creatureid);
	Player* target = dynamic_cast<Player*>(creature);
	if (!target)
		return;

	if (target->party != 0)
	{
		std::stringstream bericht;
		bericht << target->getName() << " is already in a party.";
		player->sendTextMessage(MSG_INFO, bericht.str().c_str());
		return;
	}

	player->party = player->getID();
	target->inviterplayers.push_back(player);
	player->invitedplayers.push_back(target);

	std::stringstream bericht1;
	bericht1 << target->getName() << " has been invited.";
	player->sendTextMessage(MSG_INFO, bericht1.str().c_str());

	std::stringstream bericht2;
	if(player->getSex() == PLAYERSEX_MALE){
		bericht2 << player->getName() <<" invites you to his party.";
	}else{
		bericht2 << player->getName() <<" invites you to her party.";}

	target->sendTextMessage(MSG_INFO, bericht2.str().c_str());
	target->onPartyIcons(player, 2, false, false);
	sendPartyIcons(player, 1, true, false);
	sendPartyIcons(player, 4, true, false);
	sendPartyIcons(target, 2, false, false);
	target->onPartyIcons(player, 1, false, false);
}

void Protocol76::parseRevokeParty(NetworkMessage &msg)
{
	unsigned long creatureid = msg.GetU32();
	Creature* creature = game->getCreatureByID(creatureid);
	Player* target = dynamic_cast<Player*>(creature);
	if (!target)
		return;

	std::vector<Player*>::iterator invited = std::find(player->invitedplayers.begin(), player->invitedplayers.end(), target);
	if(invited != player->invitedplayers.end())
		player->invitedplayers.erase(invited);

	std::stringstream bericht1;
	bericht1 << "Invitation for " << target->getName() << " has been revoked.";
	player->sendTextMessage(MSG_INFO, bericht1.str().c_str());

	std::stringstream bericht2;
	bericht2 << player->getName() << " has revoked his invitation.";
	target->sendTextMessage(MSG_INFO, bericht2.str().c_str());

	target->onPartyIcons(player, 0, false, false);
	sendPartyIcons(target, 0, false, false);
}

void Protocol76::parseJoinParty(NetworkMessage &msg)
{
	unsigned long creatureid = msg.GetU32();
	Creature* creature = game->getCreatureByID(creatureid);
	Player* target = dynamic_cast<Player*>(creature);
	if (!target)
		return;

	player->party = target->party;
	std::vector<Player*>::iterator invited = std::find(target->invitedplayers.begin(), target->invitedplayers.end(), player);
	if(invited != target->invitedplayers.end())
		target->invitedplayers.erase(invited);

	player->inviterplayers.clear();
	std::stringstream bericht1;
	bericht1 <<  "You have joined "<< target->getName() << "'s party.";
	player->sendTextMessage(MSG_INFO, bericht1.str().c_str());

	std::stringstream bericht2;
	bericht2 << player->getName() << " has joined the party.";

	for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it)
	{
		if((*it).second->party == target->party)
		{
			if((*it).second->getID() != player->getID())
			{
				(*it).second->sendTextMessage(MSG_INFO, bericht2.str().c_str());
				(*it).second->onPartyIcons(player, 3, true, false);
			}
			player->onPartyIcons((*it).second, 3, true, false);
		}
		if((*it).second->getID() == player->party)
		{
			player->onPartyIcons((*it).second, 4, true, false);
		}
	}
}

void Protocol76::parsePassLeadership(NetworkMessage &msg)
{
	unsigned long creatureid = msg.GetU32();
	Creature* creature = game->getCreatureByID(creatureid);
	Player* target = dynamic_cast<Player*>(creature);
	if (!target)
		return;

	target->sendTextMessage(MSG_INFO, "You are now leader of your party.");
	std::stringstream bericht;
	bericht << target->getName() << " is now leader of your party.";
	int oldpartyid = player->getID();

	for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it)
	{
		if((*it).second->party == oldpartyid)
		{
			if((*it).second->getID() != target->getID())
				(*it).second->sendTextMessage(MSG_INFO, bericht.str().c_str());
			(*it).second->onPartyIcons(target, 4, true, false);
			(*it).second->onPartyIcons(player, 3, true, false);
			(*it).second->party = target->getID();
		}
	}
}
#endif //TLM_SKULLS_PARTY

void Protocol76::sendInfoBox(std::string text, int itemid)
{
     NetworkMessage msg;
  msg.AddByte(0x96);
  msg.AddU32(1);
  msg.AddU16(itemid);
  msg.AddU16(text.size());
  msg.AddString(text);
  msg.AddString("Arconia");
     WriteBuffer(msg);
}

void Protocol76::sendTextDialog(const std::string &text)
{
    NetworkMessage msg;
    msg.AddByte(0x15);
    msg.AddString(text.c_str());
    WriteBuffer(msg);
}

#ifdef KICK_PLAYER
void Protocol76::sendKick()
{
    closesocket(s);
    s = 0;
}
    #endif //KICK_PLAYER
