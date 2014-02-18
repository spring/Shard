
/*
	Copyright (c) 2008 Robin Vobruba <hoijui.quaero@gmail.com>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CppTestAI.h"


#include "ExternalAI/Interface/AISEvents.h"
#include "ExternalAI/Interface/AISCommands.h"

// generated by the C++ Wrapper scripts
#include "AI/Wrappers/Cpp/src-generated/OOAICallback.h"
#include "AI/Wrappers/Cpp/src-generated/SkirmishAI.h"
#include "AI/Wrappers/Cpp/src-generated/Unit.h"
#include "AI/Wrappers/Cpp/src-generated/UnitDef.h"
#include "AI/Wrappers/Cpp/src-generated/Engine.h"
#include "AI/Wrappers/Cpp/src-generated/WrappUnit.h"

#include <string>

cpptestai::CCppTestAI::CCppTestAI(springai::OOAICallback* callback):
		callback(callback),
		skirmishAIId(callback != NULL ? callback->GetSkirmishAIId() : -1),
		game(NULL)
		{
			game = new CSpringGame(callback);
		}

cpptestai::CCppTestAI::~CCppTestAI() {}

int cpptestai::CCppTestAI::HandleEvent(int topic, const void* data) {

	switch (topic) {
		case EVENT_INIT: {
			game->Me()->Init();
			break;
		}
		case EVENT_RELEASE: {
			game->Me()->GameEnd();
			break;
		}
		case EVENT_UPDATE: {
			game->Me()->Update();
			break;
		}
		case EVENT_UNIT_GIVEN: {
			struct SUnitGivenEvent* evt = (struct SUnitGivenEvent*) data;
			if(evt->unitId < 0){
				game->SendToConsole("shard-runtime warning: unitgiven evt->unit < 0");
				break;
			}
			CSpringUnit* u = game->GetUnitById(evt->unitId);
			if (!u) {
				u = game->CreateUnit(evt->unitId);
			}

			if (u) {
				// it might not have been given to us! Could have been given to another team
				if(game->GetTeamID() == u->Team()){
					game->Me()->UnitGiven(u);
				}
			} else {
				game->SendToConsole("shard-runtime warning: unitgiven unit NULL");
			}
			break;
		}
		case EVENT_UNIT_CREATED: {
			struct SUnitCreatedEvent* evt = (struct SUnitCreatedEvent*) data;
			if(evt->unit < 0){
				game->SendToConsole("shard-runtime warning: unitcreated evt->unit < 0");
				break;
			}
			CSpringUnit* u = game->CreateUnit(evt->unit);
			if (u) {
				game->Me()->UnitCreated(u);
			} else {
				game->SendToConsole("shard-runtime warning: unitcreated unit NULL");
			}
			break;
		}
		case EVENT_UNIT_FINISHED: {
			struct SUnitFinishedEvent* evt = (struct SUnitFinishedEvent*) data;
			if(evt->unit < 0){
				game->SendToConsole("shard-runtime warning: unitfinished evt->unit < 0");
				break;
			}
			CSpringUnit* u = game->GetUnitById(evt->unit);
			if(u == 0){
				u = game->CreateUnit(evt->unit);
			}
			if(u != 0){
				game->Me()->UnitBuilt(u);
			} else {
				game->SendToConsole("shard-runtime warning: unitfinished unit NULL");
			}
			break;
		}
		case EVENT_UNIT_DESTROYED: {
			struct SUnitDestroyedEvent* evt = (struct SUnitDestroyedEvent*) data;
			if(evt->unit < 0){
				game->SendToConsole("shard-runtime warning: unitdestroyed evt->unit < 0");
				break;
			}
			CSpringUnit* u = game->GetUnitById(evt->unit);
			if(!u) {
				u = game->CreateUnit(evt->unit);
			}
			if(u){
				game->Me()->UnitDead(u);
				game->DestroyUnit(evt->unit);
			} else {
				game->SendToConsole("shard-runtime warning: unitdestroyed unit NULL");
			}
			break;
		}
		case EVENT_ENEMY_DESTROYED: {
			struct SEnemyDestroyedEvent* evt = (struct SEnemyDestroyedEvent*) data;
			if(evt->enemy < 0){
				game->SendToConsole("shard-runtime warning: enemydestroyed evt->unit < 0");
				break;
			}
			CSpringUnit* u = game->GetUnitById(evt->enemy);
			if (!u) {
				u = game->CreateUnit(evt->enemy);
			}
			/*if(u){
				// @TODO: Add enemy dead event
				game->Me()->UnitDead(u);
			}*/
			game->DestroyUnit(evt->enemy);
			break;
		}
		case EVENT_ENEMY_CREATED: {
			struct SEnemyCreatedEvent* evt = (struct SEnemyCreatedEvent*) data;
			game->CreateUnit(evt->enemy);
			break;
		}
		case EVENT_ENEMY_FINISHED: {
			struct SEnemyFinishedEvent* evt = (struct SEnemyFinishedEvent*) data;
			game->CreateUnit(evt->enemy);
			break;
		}
		case EVENT_UNIT_DAMAGED: {
			struct SUnitDamagedEvent* evt = (struct SUnitDamagedEvent*) data;
			if(evt->unit < 0){
				game->SendToConsole("shard-runtime warning: unitdamaged evt->unit < 0");
				break;
			}
			CSpringUnit* u = game->GetUnitById(evt->unit);
			if (!u) {
				u = game->CreateUnit(evt->unit);
			}

			//attacker is allowed to be -1 if attacker cannot be seen or determined.
			CSpringUnit* a = NULL;
			if (evt->attacker >= 0) {
				a = game->GetUnitById(evt->attacker);
				if (!a) {
					game->CreateUnit(evt->attacker);
				}
			}
			if(u) {
				game->Me()->UnitDamaged(u,a);
			} else {
				if (!u)
					game->SendToConsole("shard-runtime warning: attacked unit not found.");
			}
			break;
		}
		case EVENT_UNIT_IDLE: {
			struct SUnitIdleEvent* evt = (struct SUnitIdleEvent*) data;
			if(evt->unit < 0){
				game->SendToConsole("shard-runtime warning: unitidle evt->unit < 0");
				break;
			}
			CSpringUnit* u = game->GetUnitById(evt->unit);
			if (!u) {
				game->CreateUnit(evt->unit);
			}
			if (u) {
				game->Me()->UnitIdle(u);
			} else {
				game->SendToConsole("shard-runtime warning: unitidle unit was NULL");
			}
			break;
		}
		case EVENT_UNIT_MOVE_FAILED: {
			struct SUnitMoveFailedEvent* evt = (struct SUnitMoveFailedEvent*) data;
			if(evt->unit < 0){
				game->SendToConsole("shard-runtime warning: SUnitMoveFailedEvent evt->unit < 0");
				break;
			}
			CSpringUnit* u = game->GetUnitById(evt->unit);
			if (!u) {
				u = game->CreateUnit(evt->unit);
			}
			if (u) {
				game->Me()->UnitMoveFailed(u);
			} else {
				game->SendToConsole("shard-runtime warning: Did not now about move failed unit");
			}
			break;
		}
		default: {
			break;
		}
	}

	// signal: everything went OK
	return 0;
}

CSpringUnit* cpptestai::CCppTestAI::getUnitByID( int unit_id ) {
	return game->GetUnitById(unit_id);
}

/*EVENT_NULL                         =  0,
	EVENT_INIT                         =  1,
	EVENT_RELEASE                      =  2,
	EVENT_UPDATE                       =  3,
	EVENT_MESSAGE                      =  4,
	EVENT_UNIT_CREATED                 =  5,
	EVENT_UNIT_FINISHED                =  6,
	EVENT_UNIT_IDLE                    =  7,
	EVENT_UNIT_MOVE_FAILED             =  8,
	EVENT_UNIT_DAMAGED                 =  9,
	EVENT_UNIT_DESTROYED               = 10,
	EVENT_UNIT_GIVEN                   = 11,
	EVENT_UNIT_CAPTURED                = 12,
	EVENT_ENEMY_ENTER_LOS              = 13,
	EVENT_ENEMY_LEAVE_LOS              = 14,
	EVENT_ENEMY_ENTER_RADAR            = 15,
	EVENT_ENEMY_LEAVE_RADAR            = 16,
	EVENT_ENEMY_DAMAGED                = 17,
	EVENT_ENEMY_DESTROYED              = 18,
	EVENT_WEAPON_FIRED                 = 19,
	EVENT_PLAYER_COMMAND               = 20,
	EVENT_SEISMIC_PING                 = 21,
	EVENT_COMMAND_FINISHED             = 22,
	EVENT_LOAD                         = 23,
	EVENT_SAVE                         = 24,
	EVENT_ENEMY_CREATED                = 25,
	EVENT_ENEMY_FINISHED               = 26,
	EVENT_LUA_MESSAGE                  = 27,

	*/
