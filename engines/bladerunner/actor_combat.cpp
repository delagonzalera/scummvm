/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "bladerunner/actor_combat.h"

#include "bladerunner/savefile.h"

namespace BladeRunner {

ActorCombat::ActorCombat(BladeRunnerEngine *vm) {
	_vm = vm;
}

ActorCombat::~ActorCombat() {
}

void ActorCombat::save(SaveFile &f) {
	// TODO
	f.write(0); // _actorId
	f.write(0); // _combatOn
	f.write(0); // _field2
	f.write(0); // _field3
	f.write(0); // _otherActorId
	f.write(0); // _field5
	f.write(0); // _field6
	f.write(0); // _field7
	f.write(0); // _field8
	f.write(0); // _field9
	f.write(0); // _field10
	f.write(0); // _field11
	f.write(0); // _field12
	f.write(0); // _actorHp
	f.write(0); // _field14
	f.write(0); // _field15
	f.write(0); // _actorPosition
	f.write(0); // _otherActorPosition
	f.write(0); // _availableCoversCount
	f.write(0); // _availableFleeWaypointsCount
	f.write(0); // _field24
}

void ActorCombat::hitAttempt() {
}

void ActorCombat::combatOn(int actorId, int a3, int a4, int otherActorId, int a6, int a7, int a8, int a9, int ammoDamage, int a11, int a12) {
}

void ActorCombat::combatOff() {
}

void ActorCombat::setup() {
}

} // End of namespace BladeRunner
