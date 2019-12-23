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

void SceneScriptHF03::InitializeScene() {
	if (Game_Flag_Query(kFlagHF02toHF03)) {
		Setup_Scene_Information( 479.0f, 47.76f,  -496.0f, 600);
	} else {
		Setup_Scene_Information(185.62f, 47.76f, -867.42f, 300);
	}

	Scene_Exit_Add_2D_Exit(0,   0,   0,  30, 479, 3);
	Scene_Exit_Add_2D_Exit(1, 589,   0, 639, 479, 1);
	Scene_Exit_Add_2D_Exit(2, 323, 110, 380, 166, 0);

	Ambient_Sounds_Add_Looping_Sound(kSfxARCBED1, 50, 0, 1);
	Ambient_Sounds_Add_Sound(kSfx67_0480R, 5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0540R, 5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0560R, 5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0870R, 5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0900R, 5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0940R, 5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_1070R, 5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_1080R, 5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_1160R, 5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
}

void SceneScriptHF03::SceneLoaded() {
	Obstacle_Object("MAIN", true);
	Unclickable_Object("MAIN");
}

bool SceneScriptHF03::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptHF03::ClickedOn3DObject(const char *objectName, bool a2) {
	if (Object_Query_Click(objectName, "MAIN")) {
		Actor_Says(kActorMcCoy, Random_Query(0, 3) + 8525, 18);
	}
	return false;
}

void SceneScriptHF03::dialogueWithLucy() {
	Dialogue_Menu_Clear_List();
	DM_Add_To_List_Never_Repeat_Once_Selected(840, -1, 3, 8); // VOIGT-KAMPFF
	DM_Add_To_List_Never_Repeat_Once_Selected(850, 6, 5, 2); // FATHER
	DM_Add_To_List_Never_Repeat_Once_Selected(860, 8, -1, -1); // CRYSTAL
	DM_Add_To_List_Never_Repeat_Once_Selected(870, 2, 8, 6); // RUNCITER
	Dialogue_Menu_Add_DONE_To_List(880); // DONE

	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();

	switch (answer) {
	case 840: // VOIGT-KAMPFF
		Actor_Says(kActorMcCoy, 1630, 15); // McCoy: Come with me. I've got a test I want you to take.
		if (Global_Variable_Query(kVariableHollowayArrest) == 3) {
			Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF03RunAwayWithHelp1);
		} else if (Game_Flag_Query(kFlagLucyIsReplicant)) {
			Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF03RunToHF041);
		} else {
			Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF03RunOutPhase1);
			Game_Flag_Set(kFlagLucyRanAway);
		}
		break;

	case 850: // FATHER
		Actor_Says(kActorMcCoy, 1635, 15); // McCoy: Can you take me to your father?
		Actor_Says(kActorLucy,   200, 13); // Lucy: I don't think he'd like that. I don't know where he is anyway.
		Actor_Modify_Friendliness_To_Other(kActorLucy, kActorMcCoy, 3);
		break;

	case 860: // CRYSTAL
		Actor_Says(kActorMcCoy, 1640, 12); // McCoy: There's another hunter after your family.
		if (Global_Variable_Query(kVariableHollowayArrest) == 3) {
			Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF03RunAwayWithHelp1);
		} else {
			Actor_Says(kActorLucy,   210, 13); // Lucy: We know. The woman who smokes.
			Actor_Says(kActorMcCoy, 1655, 15); // McCoy: You've got to get out of here.
			Actor_Modify_Friendliness_To_Other(kActorLucy, kActorMcCoy, Random_Query(9, 10));
			if (Actor_Query_Friendliness_To_Other(kActorLucy, kActorMcCoy) > 59
			 && Global_Variable_Query(kVariableAffectionTowards) == kAffectionTowardsNone
			) {
				Global_Variable_Set(kVariableAffectionTowards, kAffectionTowardsLucy);
				Actor_Says(kActorLucy,   940, 14); // Lucy: You're a good man.
				Actor_Says(kActorMcCoy, 6780, 11); // McCoy: Don't jump to any conclusions.
				Actor_Says(kActorLucy,   950, 12); // Lucy: You're gentle. Father used to be like that too.
				Actor_Says(kActorLucy,   960, 13); // Lucy: He would read to me and tell me stories. Pretty stories so I would have pretty dreams.
				Actor_Says(kActorMcCoy, 6785, 15); // McCoy: He doesn't do that anymore?
				Actor_Says(kActorLucy,   970, 16); // Lucy: He's out with Sadik every night. And he and mother...
				Actor_Says(kActorLucy,   980, 17); // Lucy: They argue about what we're going to do.
				if (Game_Flag_Query(kFlagDektoraIsReplicant)) {
					Actor_Says(kActorLucy, 990, 17); // Lucy: He's worried about her. She hasn't been well lately.
				}
				Actor_Says(kActorMcCoy, 6790, 15); // McCoy: That must be tough on you.
				Actor_Says(kActorLucy,  1000, 13); // Lucy: I've seen death before. But the other day...
				Actor_Says(kActorLucy,  1010, 17); // Lucy: I was thinking; If a person can feel love, real love
				Actor_Says(kActorLucy,  1020, 18); // Lucy: for even just one day, I think it would all be worth it.
				Actor_Says(kActorMcCoy, 6795, 14); // McCoy: I agree.
				Actor_Says(kActorLucy,  1030, 17); // Lucy: Out in a place with trees and a lake and flowers. I've seen places like that in books.
				Actor_Says(kActorMcCoy, 6800, 14); // McCoy: You'll see them with your own eyes one day.
			}
			Actor_Says(kActorLucy,   220, 13); // Lucy: You're not-- You're not the hunter anymore?
			Actor_Says(kActorMcCoy, 1660, 15); // McCoy: Go! Quickly.
			Actor_Says(kActorLucy,   230, 14); // Lucy: Thank you.
			Actor_Clue_Acquire(kActorLucy, kClueMcCoyHelpedLucy, true, kActorMcCoy);

			if (Game_Flag_Query(kFlagLucyIsReplicant)) {
				Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF03RunToHF041);
			} else {
				Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF03RunOutPhase1);
			}
		}
		break;

	case 870: // RUNCITER
		Actor_Says(kActorMcCoy, 1645, 18); // McCoy: Why'd they kill Runciter's animals?
		Actor_Says(kActorLucy,   240, 14); // Lucy: I didn't want them to. But father, he was so angry.
		Actor_Says(kActorLucy,   250, 12); // Lucy: And Sadik said Mr. Runciter deserved it for treating me that way.
		Actor_Says(kActorLucy,   260, 13); // Lucy: It's so funny. I'm worth less than those animals.
		Actor_Says(kActorLucy,   270, 19); // Lucy: I guess I should hate them. But I don't.
		Actor_Says(kActorMcCoy, 1665, 18); // McCoy: So, all the animals were real?
		Actor_Says(kActorLucy,   280, 13); // Lucy: Mr. Runciter also used to buy some of the... others.
		Actor_Says(kActorMcCoy, 1670, 12); // McCoy: Artificial?
		Actor_Says(kActorLucy,   290, 14); // Lucy: He used to tell me that he was protected.
		Actor_Says(kActorLucy,   300, 16); // Lucy: That they came from somebody close to the source.
		Actor_Says(kActorMcCoy, 1675, 12); // McCoy: Who else was there that night?
		Actor_Says(kActorLucy,   310, 13); // Lucy: Father and uncle Zuben.
		Actor_Clue_Acquire(kActorMcCoy, kClueLucyInterview, false, kActorLucy);
		break;

	case 880: // DONE
		Actor_Says(kActorMcCoy, 1650, 14); // McCoy: Take care of yourself, kid.
		break;
	}
}

bool SceneScriptHF03::ClickedOnActor(int actorId) {
	if (actorId == kActorLucy
	 && Actor_Query_Goal_Number(kActorLucy) == kGoalLucyGoToHF03
	 ) {
		if (Game_Flag_Query(kFlagLucyIsReplicant)
		 ? !Loop_Actor_Walk_To_Waypoint(kActorMcCoy, 377, 0, true, false)
		 : !Loop_Actor_Walk_To_Waypoint(kActorMcCoy, 378, 0, true, false)
		) {
			Actor_Face_Actor(kActorMcCoy, kActorLucy, true);
			if (!Game_Flag_Query(kFlagHF03LucyTalk)) {
				Game_Flag_Set(kFlagHF03LucyTalk);
				if (Game_Flag_Query(kFlagLucyIsReplicant)) {
					Actor_Says(kActorMcCoy, 1605, 15); // McCoy: You're sick, Lucy.
					Actor_Says(kActorLucy,   100, 12); // Lucy: No. I'm not.
					Actor_Says(kActorMcCoy, 1610, 14); // McCoy: Yes. You are.
				} else {
					Actor_Says(kActorMcCoy, 1615, 16); // McCoy: I've been looking for you.
					Actor_Says(kActorLucy,   110, 13); // Lucy: Please, leave me alone.
				}
				Actor_Says(kActorLucy,   120, 13); // Lucy: Father told me to watch out for you.
				Actor_Says(kActorMcCoy, 1620, 14); // McCoy: I'm not gonna hurt you.
				Actor_Says(kActorLucy,   130, 17); // Lucy: Have you always lived on Terra?
				Actor_Says(kActorMcCoy, 1625, 15); // McCoy: Yes.
				if (Game_Flag_Query(kFlagLucyIsReplicant)) {
					Actor_Says(kActorLucy, 140, 12); // Lucy: I always dreamed Terra would be so special and beautiful.
					Actor_Says(kActorLucy, 150, 13); // Lucy: Birds and flowers and trees and blue water. Like on the video.
					Actor_Says(kActorLucy, 160, 15); // Lucy: So different from everything on... uhm, where I was.
				} else {
					Actor_Says(kActorLucy, 170, 12); // Lucy: Father told me he thought Terra would be so special and beautiful.
					Actor_Says(kActorLucy, 180, 13); // Lucy: Birds and flowers and trees and blue water.
					Actor_Says(kActorLucy, 190, 15); // Lucy: But it's not like that at all. I don't remember it ever being like that.
				}
			}

			dialogueWithLucy();
		}
	}
	return false;
}

bool SceneScriptHF03::ClickedOnItem(int itemId, bool a2) {
	return false;
}

bool SceneScriptHF03::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 179.0f, 46.76f, -824.0f, 0, true, false, false)) {
			Game_Flag_Set(kFlagHF03toHF01);
			Set_Enter(kSetHF01, kSceneHF01);
		}
		return true;
	}

	if (exitId == 1) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 479.0f, 47.76f, -524.0f, 0, true, false, false)) {
			Game_Flag_Set(kFlagHF03toHF02);
			Set_Enter(kSetHF02, kSceneHF02);
		}
		return true;
	}

	if (exitId == 2) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 942.0f, 47.76f, -847.0f, 0, true, false, false)) {
			Game_Flag_Set(kFlagHF03toHF04);
			Set_Enter(kSetHF04, kSceneHF04);
		}
		return true;
	}
	return false;
}

bool SceneScriptHF03::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptHF03::SceneFrameAdvanced(int frame) {
}

void SceneScriptHF03::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptHF03::PlayerWalkedIn() {
	if (Game_Flag_Query(kFlagHF02toHF03)) {
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, 479.0f, 47.76f, -524.0f, 0, false, false, false);
		Game_Flag_Reset(kFlagHF02toHF03);
	} else {
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, 179.0f, 47.76f, -824.0f, 0, false, false, false);
		Game_Flag_Reset(kFlagHF01toHF03);
	}

	if (Actor_Query_Goal_Number(kActorLucy) == kGoalLucyReturnToHF03) {
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF03RunToHF041);
		Actor_Says(kActorSteele, 210, 13); // Steele: Just follow my lead, Slim.
		Actor_Face_Actor(kActorMcCoy, kActorSteele, true);
		Actor_Says(kActorMcCoy,  1680, 15); // McCoy: There's civilians all over the place.
		Actor_Says(kActorSteele,  220, 14); // Steele: Yeah, they move pretty fast, don't they?
		Actor_Says(kActorMcCoy,  1685, 13); // McCoy: I'll take care of it. You just cover the exit.
		Actor_Says(kActorSteele,  230, 16); // Steele: Who's giving the orders around here?
		Actor_Says(kActorMcCoy,  1690, 12); // McCoy: This one's mine, Steele. I tracked it, I get the kill.
		Actor_Says(kActorSteele,  240, 13); // Steele: About time you strapped on some balls, Slim. Go for it.
		Actor_Set_Goal_Number(kActorSteele, kGoalSteeleHF03McCoyChasingLucy);
	}
}

void SceneScriptHF03::PlayerWalkedOut() {
	Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
	Ambient_Sounds_Remove_All_Looping_Sounds(1);
}

void SceneScriptHF03::DialogueQueueFlushed(int a1) {
}

} // End of namespace BladeRunner
