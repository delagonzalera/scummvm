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

#include "bladerunner/script/scene_script.h"

namespace BladeRunner {

void SceneScriptPS15::InitializeScene() {
	Setup_Scene_Information(-360.0f, -113.43f, 50.0f, 0);
	Scene_Exit_Add_2D_Exit(0, 0, 0, 20, 479, 3);
	Scene_Exit_Add_2D_Exit(1, 620, 0, 639, 479, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxRUMLOOP1, 20, 1, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxLABAMB3,  80, 0, 1);
	Ambient_Sounds_Add_Sound(kSfxPHONE1, 5, 50,  8,  8, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxPAGE1,  5, 20, 30, 30,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxPAGE2,  5, 20, 30, 30,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxPAGE3,  5, 20, 30, 30,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSWEEP3, 5, 20,  5,  9,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSWEEP4, 5, 20,  5,  9,  -70,  70, -101, -101, 0, 0);
	Actor_Put_In_Set(kActorSergeantWalls, kSetPS15);
	Actor_Set_At_XYZ(kActorSergeantWalls, -265.4f, -113.43f, -31.29f, 623);
}

void SceneScriptPS15::SceneLoaded() {
	Obstacle_Object("E.ARCH", true);
	if (Global_Variable_Query(kVariableChapter) == 2) {
		Item_Add_To_World(kItemWeaponsCrate, kModelAnimationWeaponsCrate, 101, -208.0f, -113.43f, 30.28f, 750, 16, 12, false, true, false, true);
	}
}

bool SceneScriptPS15::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptPS15::ClickedOn3DObject(const char *objectName, bool a2) {
	return false;
}

bool SceneScriptPS15::ClickedOnActor(int actorId) {
	if (actorId == kActorSergeantWalls) {
		if ((Actor_Clue_Query(kActorMcCoy, kClueWeaponsOrderForm)
		  || Actor_Clue_Query(kActorMcCoy, kCluePoliceIssueWeapons)
		 )
		 && !Actor_Clue_Query(kActorMcCoy, kClueShippingForm)
		) {
			if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -256.0f, -113.43f, 43.51f, 0, true, false, false)) {
				Actor_Face_Actor(kActorMcCoy, kActorSergeantWalls, true);
				Actor_Face_Actor(kActorSergeantWalls, kActorMcCoy, true);
				Actor_Says(kActorMcCoy,         4470, 17); // McCoy: That weapons' shipment just came in. You got the paperwork handy?
				Actor_Says(kActorSergeantWalls,  130, 12); // Sergeant Walls: Why? You got a pressing need to rummage through my private files?
				Actor_Says(kActorMcCoy,         4475, 18); // McCoy: Yeah. I forgot you were keeping your lacy underthings in there.
				Actor_Says(kActorMcCoy,         4480, 13); // McCoy: Look, Jack, I just want to see what they're charging for a crate of rifles these days.
				Actor_Says(kActorSergeantWalls,  140, 16); // Sergeant Walls: Too damn much if you ask me. Especially at the rate the assault teams are losing them.
				Item_Pickup_Spin_Effect(kModelAnimationWeaponsOrderForm, 211, 239);
				Actor_Says(kActorSergeantWalls, 150, 14); // Sergeant Walls: I guess there ain't no harm in it.
				Actor_Clue_Acquire(kActorMcCoy, kClueShippingForm, true, kActorSergeantWalls);
				if (!Game_Flag_Query(kFlagPS04WeaponsOrderForm)) {
					Item_Remove_From_World(kItemWeaponsOrderForm);
				}
			}
		} else {
			Actor_Face_Actor(kActorMcCoy, kActorSergeantWalls, true);
			Actor_Says(kActorMcCoy, 8600, 15); // McCoy: You keeping busy, pal?
			if (_vm->_cutContent) {
				switch (Random_Query(1, 2)) {
				case 1:
					Actor_Says(kActorSergeantWalls, 190, 12); // Sergeant Walls: Nah, the place has been pretty quiet the last couple of days.
					break;
				case 2:
					Actor_Says(kActorSergeantWalls, 200, 12); // Sergeant Walls: Come back at me when you got something worthwhile, McCoy.
					break;
				}
			} else {
				Actor_Says(kActorSergeantWalls, 190, 12); // Sergeant Walls: Nah, the place has been pretty quiet the last couple of days.
			}
		}
		return true;
	}
	return false;
}

bool SceneScriptPS15::ClickedOnItem(int itemId, bool a2) {
	if (itemId == kItemWeaponsCrate) {
		if (Actor_Clue_Query(kActorMcCoy, kClueWeaponsOrderForm)
		 && Actor_Clue_Query(kActorMcCoy, kCluePoliceIssueWeapons)
		) {
			Actor_Says(kActorMcCoy, 8570, 14); // McCoy: I've gotten all I can from that.
		} else {
			Actor_Face_Actor(kActorMcCoy, kActorSergeantWalls, true);
			Actor_Face_Actor(kActorSergeantWalls, kActorMcCoy, true);
			Actor_Says(kActorMcCoy,         4485, 17); // McCoy: We get a new weapons' shipment?
			Actor_Says(kActorSergeantWalls,  160, 14); // Sergeant Walls: Yeah, Guzza requisitioned a couple of cases for the assault teams.
			Actor_Says(kActorMcCoy,         4490, 12); // McCoy: Let me guess. He's planning on taking out a small city.
			Actor_Says(kActorSergeantWalls,  170, 13); // Sergeant Walls: Lieutenant's a big believer in overkill.
#if BLADERUNNER_ORIGINAL_BUGS
			Actor_Clue_Acquire(kActorMcCoy, kClueWeaponsOrderForm,   true, kActorMcCoy); // A bug? Shouldn't the last argument be -1 or kActorSergeantWalls here?
			Actor_Clue_Acquire(kActorMcCoy, kCluePoliceIssueWeapons, true, kActorMcCoy); // A bug? Shouldn't the last argument be -1 or kActorSergeantWalls here?
#else
			Actor_Clue_Acquire(kActorMcCoy, kClueWeaponsOrderForm,   true, kActorSergeantWalls);
			Actor_Clue_Acquire(kActorMcCoy, kCluePoliceIssueWeapons, true, kActorSergeantWalls);
#endif // BLADERUNNER_ORIGINAL_BUGS
		}
		return true;
	}
	return false;
}

bool SceneScriptPS15::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -360.0f, -113.43f, 50.0f, 0, true, false, false)) {
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Game_Flag_Set(kFlagPS15toPS05);
			Set_Enter(kSetPS05, kScenePS05);
		}
		return true;
	}

	if (exitId == 1) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -183.58f, -113.43f, 91.7f, 0, true, false, false)) {
			Actor_Says(kActorMcCoy,         4440, 18); // McCoy: Hit the lock, Jack.
			Actor_Says(kActorSergeantWalls,  150, 17); // Sergeant Walls: I guess there ain't no harm in it.
#if BLADERUNNER_ORIGINAL_BUGS
			// Sometimes the scene transition code (or the Ambient_Sounds_Remove_All_Non_Looping_Sounds)
			// would stop this from playing (rare occasions)
			// Solution: moved into PS10 code
			Sound_Play(kSfxLABBUZZ1, 90, 0, 0, 50);
#endif // BLADERUNNER_ORIGINAL_BUGS
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Set_Enter(kSetPS10_PS11_PS12_PS13, kScenePS10);
		}
		return true;
	}
	return false;
}

bool SceneScriptPS15::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptPS15::SceneFrameAdvanced(int frame) {
}

void SceneScriptPS15::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptPS15::PlayerWalkedIn() {
	Loop_Actor_Walk_To_XYZ(kActorMcCoy, -326.93f, -113.43f, 101.42f, 0, false, false, false);
	if (!Game_Flag_Query(kFlagPS15Entered)) {
		Actor_Face_Actor(kActorMcCoy, kActorSergeantWalls, true);
		Actor_Face_Actor(kActorSergeantWalls, kActorMcCoy, true);
		Actor_Says(kActorSergeantWalls,    0, 12); // Sergeant Walls: Well, if it ain't Ray McCoy. Bryant finally let you play with the big boys.
		Actor_Says(kActorMcCoy,         4445, 18); // McCoy: My clearance come through?
		Actor_Says(kActorSergeantWalls,   10, 12); // Sergeant Walls: You bet. You bring your piece?
		Actor_Says(kActorMcCoy,         4450, 18); // McCoy: Don't leave home without it.
		if (_vm->_cutContent && !Game_Flag_Query(kFlagSergeantWallsMazeInstructions)) {
			// TODO: These instructions don't make much sense... Is it better to not restore this at all?
			//       Could there have been a system with a bell-tone that was removed from the original version?
			Game_Flag_Set(kFlagSergeantWallsMazeInstructions);
			Actor_Says(kActorSergeantWalls,   20, 13); // Sergeant Walls: First time through is always the toughest. Listen for the bell tone.
			Actor_Says(kActorSergeantWalls,   30, 12); // Sergeant Walls: That'll let you know when you're too slow reacting to a hot target.
			Actor_Says(kActorMcCoy,         4455, 12); // McCoy: Then what?
			Actor_Says(kActorSergeantWalls,   40, 12); // Sergeant Walls: You can keep going but your score is gonna suffer.
			Actor_Says(kActorSergeantWalls,   50, 12); // Sergeant Walls: And take it easy in there.
		}
		Actor_Says(kActorSergeantWalls,   60, 13); // Sergeant Walls: Steele's been running the maze every other day for two weeks...
		Actor_Says(kActorSergeantWalls,   70, 12); // Sergeant Walls: and the targets are getting a little roughed up.
		Actor_Says(kActorMcCoy,         4460, 15); // McCoy: I thought Guzza was gonna requisition some new ones.
		Actor_Says(kActorSergeantWalls,   80, 13); // Sergeant Walls: I ain't seen him down here in a month. Guess he's been busy.
		Actor_Says(kActorMcCoy,         4465, 16); // McCoy: Poor guy. I bet he gets all tuckered out from pushing those papers around.
		Actor_Says(kActorSergeantWalls,   90, 13); // Sergeant Walls: Ah, don't sell him short, McCoy. Guzza can be one tough hombre.
		Actor_Says(kActorSergeantWalls,  100, 14); // Sergeant Walls: I remember when we worked Vice together...
		Actor_Says(kActorSergeantWalls,  110, 15); // Sergeant Walls: He poured acetone on two pimps that gave us some lip (laughs) and lit a match.
		Actor_Says(kActorSergeantWalls,  120, 15); // Sergeant Walls: (laughs) I'll tell, ya. It looked like the goddamn Fourth of July out there.
		Actor_Says(kActorMcCoy,         4555, 14); // McCoy: Peachy.
		Game_Flag_Set(kFlagPS15Entered);
		//return true;
		return;
	} else {
		//return false;
		return;
	}
}

void SceneScriptPS15::PlayerWalkedOut() {
}

void SceneScriptPS15::DialogueQueueFlushed(int a1) {
}

} // End of namespace BladeRunner
