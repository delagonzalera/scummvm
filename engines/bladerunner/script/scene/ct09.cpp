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

void SceneScriptCT09::InitializeScene() {
	if (Game_Flag_Query(kFlagCT10toCT09)) {
		Setup_Scene_Information(160.0f,   349.0f, 587.0f, 490);
	} else if (Game_Flag_Query(kFlagCT08toCT09)) {
		Setup_Scene_Information(235.0f, 3348.52f, 599.0f, 800);
	} else {
		Setup_Scene_Information(107.0f,  348.52f, 927.0f, 200);
	}

	Scene_Exit_Add_2D_Exit(0, 321, 164, 345, 309, 1);
	Scene_Exit_Add_2D_Exit(1,   0,   0,  15, 479, 3);
	Scene_Exit_Add_2D_Exit(2, 198, 177, 263, 311, 0);

	Ambient_Sounds_Add_Looping_Sound(kSfxFLORBUZZ, 28, 0, 1);
	Ambient_Sounds_Add_Sound(kSfxTHNDER2, 6, 180, 33, 33, 0, 0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxTHNDER3, 5, 180, 33, 33, 0, 0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxTHNDER4, 5, 180, 33, 33, 0, 0, -101, -101, 0, 0);
}

void SceneScriptCT09::SceneLoaded() {
	Obstacle_Object("PHONE01", true);
	Unobstacle_Object("MAINBEAM01", true);
	Unobstacle_Object("MIDDLE WALL", true);
	Clickable_Object("BELL");
}

bool SceneScriptCT09::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptCT09::ClickedOn3DObject(const char *objectName, bool a2) {
	if (Object_Query_Click("BELL", objectName)) {
		if (Actor_Query_Which_Set_In(kActorLeon) != kSetCT09) {
			if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 229.0f, 348.52f, 851.0f, 36, true, false, false)) {
				Actor_Face_Object(kActorMcCoy, "BELL", true);
				Sound_Play(kSfxDESKBELL, 100, 0, 0, 50);
				if (!Actor_Query_Goal_Number(kActorDeskClerk)) {
					Actor_Says(kActorDeskClerk, 160, kAnimationModeTalk); // Desk Clerk: Hey, moron, I'm right here. You don't have to ring the damn bell.
				}
			}
		}
		return true;
	}
	return false;
}

bool SceneScriptCT09::ClickedOnActor(int actorId) {
	if (actorId == kActorDeskClerk) {
		if (Actor_Query_Goal_Number(kActorDeskClerk) == kGoalDeskClerkDefault
		 && Actor_Query_Which_Set_In(kActorLeon) != kSetCT09
		) {
			if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 270.0f, 348.52f, 846.0f, 12, true, false, false)) {
				Player_Loses_Control();
				Actor_Face_Actor(kActorMcCoy, kActorDeskClerk, true);
				if (Global_Variable_Query(kVariableChapter) < 3) { // it is impossible to get here before chapter 3
					Actor_Says(kActorMcCoy,     650, kAnimationModeTalk); // McCoy: You seen any suspicious types around here lately?
					Actor_Says(kActorDeskClerk, 250,                 12); // Desk Clerk: You're pretty suspicious.
					Actor_Says(kActorMcCoy,     665,                 18); // McCoy: Real funny, pal.
				} else if (!Game_Flag_Query(kFlagCT09DeskClerkTalk)) {
					Game_Flag_Set(kFlagCT09DeskClerkTalk);
					Actor_Says(kActorDeskClerk, 170,                 13); // Desk Clerk: Down the hall on your left.
					Actor_Says(kActorMcCoy,     630,                 12); // McCoy: What?
					Actor_Says(kActorDeskClerk, 180,                 14); // Desk Clerk: You're a cop, right? Your buddies have already been here. Room 27.
					Actor_Says(kActorMcCoy,     635, kAnimationModeTalk); // McCoy: What were they doing here?
					Actor_Says(kActorDeskClerk, 190,                 15); // Desk Clerk: I don't ask a lot of questions when a badge is shoved in my face.
					Actor_Says(kActorMcCoy,     640,                 12); // McCoy: You have a big guy with a strange-looking mustache staying here?
					Actor_Says(kActorMcCoy,     645, kAnimationModeTalk); // McCoy: He worked at Howie Lee's around the corner.
					Actor_Says(kActorDeskClerk, 200,                 13); // Desk Clerk: I don't break a lot of bread with the class of people that pass through here.
					Actor_Says(kActorDeskClerk, 210,                 14); // Desk Clerk: But if you want to look around, check out some rooms, be my guest.
				} else {
					Actor_Says(kActorMcCoy,     650, 18); // McCoy: You seen any suspicious types around here lately?
					Actor_Says(kActorDeskClerk, 220, 15); // Desk Clerk: Howley, that's all I do see. You think this is a Club Med or something?
				}
				Player_Gains_Control();
			}
		}
		return true;
	}
	return false;
}

bool SceneScriptCT09::ClickedOnItem(int itemId, bool a2) {
	return false;
}

bool SceneScriptCT09::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 206.0f, 348.52f, 599.0f, 0, true, false, false)) {
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, 235.0f, 348.52f, 599.0f, 0, false, false, false);
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Game_Flag_Set(kFlagCT09toCT08);
			Set_Enter(kSetCT08_CT51_UG12, kSceneCT08);
		}
		return true;
	}

	if (exitId == 1) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 107.0f, 348.52f, 927.0f, 0, true, false, false)) {
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Game_Flag_Set(kFlagCT09toCT11);
			Set_Enter(kSetCT11, kSceneCT11);
		}
		return true;
	}

	if (exitId == 2) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 159.0f, 349.0f, 570.0f, 0, true, false, false)) {
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Game_Flag_Set(kFlagCT09toCT10);
			Set_Enter(kSetCT10, kSceneCT10);
		}
		return true;
	}
	return false;
}

bool SceneScriptCT09::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptCT09::SceneFrameAdvanced(int frame) {
	if (frame == 6
	 || frame == 12
	 || frame == 19
	 || frame == 25
	 || frame == 46
	 || frame == 59
	) {
		Sound_Play(kSfxNEON7, Random_Query(47, 47), 70, 70, 50);
	}
}

void SceneScriptCT09::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptCT09::PlayerWalkedIn() {
	bool leonScene = false;

	if ( Global_Variable_Query(kVariableChapter) == 3
	 && !Game_Flag_Query(kFlagCT09Entered)
	) {
		Game_Flag_Set(kFlagCT09Entered);
		Actor_Set_Goal_Number(kActorLeon, kGoalLeonHoldingDeskClerk);
		leonScene = true;
		if (_vm->_cutContent) {
			// don't allow McCoy to leave until the situation is resolved
			// (the vanilla game allows him to leave)
			Scene_Exits_Disable();
		}
	}

	if (Game_Flag_Query(kFlagCT10toCT09)) {
		Game_Flag_Reset(kFlagCT10toCT09);
	} else if (Game_Flag_Query(kFlagCT08toCT09)) {
		if (leonScene) {
			Async_Actor_Walk_To_XYZ(kActorMcCoy, 206.0f, 348.52f, 599.0f, 0, false);
		} else {
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, 206.0f, 348.52f, 599.0f, 0, false, false, false);
		}
		Game_Flag_Reset(kFlagCT08toCT09);
	} else {
		if (leonScene) {
			Async_Actor_Walk_To_XYZ(kActorMcCoy, 124.0f, 348.52f, 886.0f, 0, false);
		} else {
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, 124.0f, 348.52f, 886.0f, 0, false, false, false);
		}
		Game_Flag_Reset(kFlagCT11toCT09);
	}

	if (Actor_Query_Goal_Number(kActorDeskClerk) == kGoalDeskClerkRecovered) {
		if (Game_Flag_Query(kFlagCT09LeonInterrupted)) {
			Actor_Says(kActorDeskClerk, 70, 13); // Desk Clerk: You know who that big guy was who was choking me?
			Actor_Face_Actor(kActorMcCoy, kActorDeskClerk, true);
			Actor_Says(kActorMcCoy,     600, 17); // McCoy: He looked familiar.
			Actor_Says(kActorDeskClerk,  80, 14); // Desk Clerk: I appreciate you getting him off of me.
			Actor_Says(kActorMcCoy,     605, 13); // McCoy: "To Protect and to Serve."
			Actor_Says(kActorDeskClerk,  90, 15); // Desk Clerk: People ain't done favors for me like that in a long time.
		} else {
			Actor_Says(kActorDeskClerk, 20, 12); // Desk Clerk: Hey, pal. You could have done something when that goon had me by the throat. Call the cops at least.
			Actor_Face_Actor(kActorMcCoy, kActorDeskClerk, true);
			Actor_Says(kActorMcCoy,     585, 18); // McCoy: You're still walking and breathing, aren't ya?
			Actor_Says(kActorDeskClerk,  40, 15); // Desk Clerk: That ain't the point.
			Actor_Says(kActorMcCoy,     590, 16); // McCoy: You scratch our back, we'll scratch yours.
			Actor_Says(kActorDeskClerk,  50, 14); // Desk Clerk: Nice frigging policy.
			Actor_Says(kActorMcCoy,     595, 14); // McCoy: You don't like it? Get a permit somewhere else.
			Actor_Says(kActorDeskClerk,  60, 13); // Desk Clerk: Lousy cops.
			Actor_Modify_Friendliness_To_Other(kActorDeskClerk, kActorMcCoy, -1);
		}
		Actor_Set_Goal_Number(kActorDeskClerk, kGoalDeskClerkDefault);
	}
}

void SceneScriptCT09::PlayerWalkedOut() {
}

void SceneScriptCT09::DialogueQueueFlushed(int a1) {
	Actor_Force_Stop_Walking(kActorMcCoy);

	if ( Actor_Query_Goal_Number(kActorLeon) == kGoalLeonHoldingDeskClerk
	 && !Game_Flag_Query(kFlagCT09LeonInterrupted)
	) {
		Player_Loses_Control();
		Actor_Set_Goal_Number(kActorLeon, kGoalLeonReleaseDeskClerk);
		//return true;
	}

	//return false;
}

} // End of namespace BladeRunner
