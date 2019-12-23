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

enum kHF05Loops {
	kHF05LoopMainLoopNoSpinner = 0,
	kHF05LoopMainLoopSpinner   = 2,
	kHF05LoopArmAnimation      = 4,
	kHF05LoopMainLoopHole      = 5
};

void SceneScriptHF05::InitializeScene() {
	if (Game_Flag_Query(kFlagHF06toHF05)) {
		Setup_Scene_Information(257.0f, 40.63f, 402.0f, 1000);
	} else if (Game_Flag_Query(kFlagHF07toHF05)) {
		Setup_Scene_Information(330.0f, 40.63f, -107.0f, 603);
	} else {
		Setup_Scene_Information(483.0f, 40.63f, -189.0f, 600);
	}

	Scene_Exit_Add_2D_Exit(0, 443, 270, 515, 350, 0);
	if (Global_Variable_Query(kVariableChapter) > 3) {
		Scene_Exit_Add_2D_Exit(1, 367, 298, 399, 349, 2);
	}
	Scene_Exit_Add_2D_Exit(2, 589,   0, 639, 479, 1);

	Ambient_Sounds_Add_Looping_Sound(kSfxRAINAWN1, 40, 1, 1);

	if (Game_Flag_Query(kFlagHF05Hole)) {
		Scene_Loop_Set_Default(kHF05LoopMainLoopHole);
		addAmbientSounds();
	} else if (Game_Flag_Query(kFlagHF06SteelInterruption)) {
		Scene_Loop_Set_Default(kHF05LoopMainLoopSpinner);
		addAmbientSounds();
	} else {
		Scene_Loop_Set_Default(kHF05LoopMainLoopNoSpinner);
	}
}

void SceneScriptHF05::SceneLoaded() {
	Obstacle_Object("MAINBASE", true);
	Unobstacle_Object("BTIRES02", true);
	Unobstacle_Object("LFTIRE02", true);
	if (Game_Flag_Query(kFlagHF05Hole)) {
		Unobstacle_Object("MONTE CARLO DRY", true);
	} else {
		Unobstacle_Object("OBSTACLE_HOLE", true);
	}
	Clickable_Object("TOP CON");
}

bool SceneScriptHF05::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptHF05::ClickedOn3DObject(const char *objectName, bool a2) {
	if (Object_Query_Click("TOP CON", objectName)) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 95.0f, 40.63f, 308.0f, 0, true, false, false)) {
			Actor_Face_Object(kActorMcCoy, "TOP CON", true);
			if (Actor_Query_In_Set(kActorCrazylegs, kSetHF05)
			 && Actor_Query_Goal_Number(kActorCrazylegs) != kGoalCrazyLegsShotAndHit
			 && Actor_Query_Goal_Number(kActorCrazylegs) != kGoalCrazyLegsLeavesShowroom
			) {
				Actor_Face_Actor(kActorCrazylegs, kActorMcCoy, true);
				Actor_Says(kActorCrazylegs, 480, 13); // Crazylegs: Hey, keep your paws off that, Ray!
				return true;
			}

			if (!Game_Flag_Query(kFlagHF01TalkToLovedOne)
			 ||  Game_Flag_Query(kFlagHF05Hole)
			) {
				Actor_Change_Animation_Mode(kActorMcCoy, 23);
				Sound_Play(kSfxELEBAD1, 100, 0, 0, 50);
				return true;
			}

			Player_Loses_Control();
			Actor_Set_Goal_Number(kActorOfficerLeary, kGoalOfficerLearyPoliceWait60SecondsToAttackHF05);
			Game_Flag_Set(kFlagHF05Hole);
			Game_Flag_Set(kFlagHF07Hole);
			Obstacle_Object("OBSTACLE_HOLE", true);
			Unobstacle_Object("MONTE CARLO DRY", true);

			if (getCompanionActor() != kActorMcCoy) {
				Loop_Actor_Walk_To_XYZ(getCompanionActor(), 181.54f, 40.63f, 388.09f, 0, false, true, false);
				Actor_Face_Actor(kActorMcCoy, getCompanionActor(), true);
				Actor_Face_Actor(getCompanionActor(), kActorMcCoy, true);
				Actor_Says(kActorMcCoy, 1785, kAnimationModeTalk); // McCoy: Let me try to get the floor open. Arm that thing, all right?
				Actor_Says(kActorMcCoy, 1790, kAnimationModeTalk); // McCoy: But wait for my signal before you drop it in.
			}

			Actor_Face_Heading(kActorMcCoy, 0, false);
			Actor_Change_Animation_Mode(kActorMcCoy, 23);
			Scene_Loop_Set_Default(kHF05LoopMainLoopHole);
			Scene_Loop_Start_Special(kSceneLoopModeOnce, kHF05LoopArmAnimation, true);


			if (getCompanionActor() == kActorMcCoy) {
				ADQ_Flush();
				ADQ_Add(kActorVoiceOver, 940, -1); // Mainframe: I had an idea. I didn't know if it was gonna work or not, but I'd run out of options.
				Ambient_Sounds_Play_Sound(kSfxLABMISC2, 50, 99, 0, 0);
				Delay(1500);
				Loop_Actor_Walk_To_XYZ(kActorMcCoy, 181.54f, 40.63f, 388.09f, 0, false, true, false);
				Actor_Face_Heading(kActorMcCoy, 0, false);
				Actor_Change_Animation_Mode(kActorMcCoy, 23);
				Actor_Clue_Lose(kActorMcCoy, kClueBomb);
			} else {
				if (getCompanionActor() == kActorDektora) {
					Actor_Face_Heading(kActorDektora, 0, false);
					Ambient_Sounds_Play_Sound(kSfxLABMISC2, 50, 99, 0, 0);
					Delay(3000);
					Actor_Face_Heading(kActorDektora, 0, false);
					Actor_Change_Animation_Mode(kActorDektora, 23);
				} else {
					Actor_Face_Heading(kActorLucy, 0, false);
					Ambient_Sounds_Play_Sound(kSfxLABMISC2, 50, 99, 0, 0);
					Delay(3000);
					Actor_Face_Heading(kActorLucy, 0, false);
					Actor_Change_Animation_Mode(kActorLucy, 13);
				}
				Actor_Face_Actor(kActorMcCoy, getCompanionActor(), true);
				Actor_Says(kActorMcCoy, 1805, kAnimationModeTalk); // McCoy: Now!
			}
			Player_Gains_Control();
		}
		return true;
	}
	return false;
}

bool SceneScriptHF05::ClickedOnActor(int actorId) {
	if (actorId == kActorCrazylegs) {
#if BLADERUNNER_ORIGINAL_BUGS
		if (!Loop_Actor_Walk_To_Actor(kActorMcCoy, kActorCrazylegs, 60, true, false)) {
			Actor_Face_Actor(kActorMcCoy, kActorCrazylegs, true);
			Actor_Face_Actor(kActorCrazylegs, kActorMcCoy, true);
			dialogueWithCrazylegs1();
		}
#else
		// Don't (re)start a dialogue with CrayLegs if he is leaving or insulted by McCoy drawing his gun
		if (Actor_Query_Goal_Number(kActorCrazylegs) != kGoalCrazyLegsLeavesShowroom
		    && Actor_Query_Goal_Number(kActorCrazylegs) != kGoalCrazyLegsMcCoyDrewHisGun) {
			if (!Loop_Actor_Walk_To_Actor(kActorMcCoy, kActorCrazylegs, 60, true, false)) {
				Actor_Face_Actor(kActorMcCoy, kActorCrazylegs, true);
				Actor_Face_Actor(kActorCrazylegs, kActorMcCoy, true);
				dialogueWithCrazylegs1();
			}
		} else {
			Actor_Face_Actor(kActorMcCoy, kActorCrazylegs, true);
			Actor_Says(kActorMcCoy, 5560, 15); // McCoy: Hey.
		}
#endif // BLADERUNNER_ORIGINAL_BUGS
	}
	return false;
}

bool SceneScriptHF05::ClickedOnItem(int itemId, bool a2) {
	return false;
}

bool SceneScriptHF05::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 483.0f, 40.63f, -189.0f, 0, true, false, false)) {
			if (!Game_Flag_Query(kFlagHF05PoliceAttacked)) {
				Game_Flag_Set(kFlagHF05toHF01);
				Set_Enter(kSetHF01, kSceneHF01);
			}
		}
		return true;
	}

	if (exitId == 1) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 330.0f, 40.63f, -85.0f, 0, true, false, false)) {
			if (!Game_Flag_Query(kFlagHF05PoliceAttacked)) {
				int affectionTowardsActor = getAffectionTowardsActor();
				if (Game_Flag_Query(kFlagHF05PoliceArrived)
				 && Game_Flag_Query(kFlagHF07Hole)
				 && affectionTowardsActor != -1
				) {
					Actor_Face_Actor(kActorMcCoy, affectionTowardsActor, true);
					Actor_Says(kActorMcCoy, 1810, 16); // McCoy: Come on, they'll be coming through the door any second!
				}
				Game_Flag_Set(kFlagHF05toHF07);
				Set_Enter(kSetHF07, kSceneHF07);
			}
		}
		return true;
	}

	if (exitId == 2) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 277.0f, 40.63f, 410.0f, 0, true, false, false)) {
			if (!Game_Flag_Query(kFlagHF05PoliceAttacked)) {
				Game_Flag_Set(kFlagHF05toHF06);
				Set_Enter(kSetHF06, kSceneHF06);
			}
		}
		return true;
	}
	return false;
}

bool SceneScriptHF05::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptHF05::SceneFrameAdvanced(int frame) {
	switch (frame) {
	case 126:
		Sound_Play(kSfxMAGMOVE1, 90, -20,  70, 50);
		break;

	case 152:
		Sound_Play(kSfxDORSLID2, 90,   0,   0, 50);
		break;

	case 156:
		Sound_Play(kSfxLOWERN1,  47, 100, 100, 50);
		break;

	case 161:
		Sound_Play(kSfxDORSLID1, 90,   0,   0, 50);
		break;

	case 176:
		Sound_Play(kSfxMAGCHNK1, 32, 100, 100, 50);
		break;

	case 178:
		Sound_Play(kSfxRAISEY1,  47, 100, 100, 50);
		break;

	case 179:
		Sound_Play(kSfxCAREXPL1, 90,   0,   0, 50);
		Music_Play(kMusicBatl226M, 50, 0, 2, -1, 0, 0);
		break;

	case 186:
		Sound_Play(kSfxCARCREK1, 32, 100, 100, 50);
		break;

	case 209:
		Sound_Play(kSfxMAGMOVE2, 90, 100, -20, 50);
		break;

	case 243:
		Sound_Play(kSfxLOWERY1,  40, -20, -20, 50);
		break;

	case 261:
		Sound_Play(kSfxCARLAND1, 47, -20, -20, 50);
		break;

	case 268:
		Sound_Play(kSfxMAGDROP1, 58, -20, -20, 50);
		break;

	case 269:
		Sound_Play(kSfxRAISEN1,  43, -20, -20, 50);
		break;
	}
	//return true;
}

void SceneScriptHF05::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
	if (actorId == kActorOfficerLeary
	 && newGoal == kGoalOfficerLearyPoliceAboutToAttackHF05
	) {
		Game_Flag_Set(kFlagHF05PoliceAttacked);
		policeAttack();
	}
	//return false;
}

void SceneScriptHF05::PlayerWalkedIn() {
	if (Game_Flag_Query(kFlagHF01TalkToLovedOne)) {
		int affectionTowardsActor = getAffectionTowardsActor();
		if (Game_Flag_Query(kFlagHF01TalkToLovedOne)
		 && affectionTowardsActor != -1
		) {
			Actor_Put_In_Set(affectionTowardsActor, kSetHF05);
			Actor_Force_Stop_Walking(affectionTowardsActor);
			if (Game_Flag_Query(kFlagHF01toHF05)) {
				Actor_Set_At_XYZ(affectionTowardsActor, 506.81f, 40.63f, -140.92f, 0);
				Async_Actor_Walk_To_Waypoint(affectionTowardsActor, 437, 36, false);
			} else if (Game_Flag_Query(kFlagHF06toHF05)) {
				Actor_Set_At_XYZ(affectionTowardsActor, 288.0f, 40.63f, 410.0f, 909);
			} else if (Game_Flag_Query(kFlagHF07toHF05)) {
				Actor_Set_At_XYZ(affectionTowardsActor, 298.0f, 40.63f, -107.0f, 512);
			} else {
				Actor_Set_At_XYZ(affectionTowardsActor, 284.0f, 40.63f, 286.0f, 0);
			}
		}

		if (Game_Flag_Query(kFlagHF05PoliceAttacked)) {
			policeAttack();
		} else if (Actor_Clue_Query(kActorMcCoy, kClueSpinnerKeys)
		        || Game_Flag_Query(kFlagHF06SteelInterruption)
		) {
			if ( Game_Flag_Query(kFlagHF06SteelInterruption)
			 && !Game_Flag_Query(kFlagHF05PoliceArrived)
			) {
				Game_Flag_Set(kFlagHF05PoliceArrived);
				Music_Play(kMusicBatl226M, 40, 0, 2, -1, 0, 0);
				Actor_Says(kActorOfficerGrayford, 200, kAnimationModeTalk); // Officer Grayford: It's all over, McCoy. You got nowhere to go and no hostages.
				Actor_Says(kActorOfficerGrayford, 210, kAnimationModeTalk); // Officer Grayford: You got two minutes. Then we're gonna clear you out!
				Actor_Set_Goal_Number(kActorOfficerLeary, kGoalOfficerLearyPoliceWait120SecondsToAttackHF05);
				if (getCompanionActor() ==  kActorDektora) {
					talkWithDektora();
				} else if (getCompanionActor() == kActorLucy) {
					talkWithLucy();
				}
			}
		} else {
			talkWithCrazylegs3(affectionTowardsActor);
		}
	} else if (Game_Flag_Query(kFlagHF01toHF05)) {
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, 399.0f, 40.63f, -85.0f, 0, false, false, false);
	} else if (Game_Flag_Query(kFlagHF07toHF05)) {
		Actor_Set_At_XYZ(kActorMcCoy, 346.0f, 4.63f, -151.0f, 603);
		Loop_Actor_Travel_Stairs(kActorMcCoy, 4, true, kAnimationModeIdle);
	}

	if (Actor_Query_In_Set(kActorCrazylegs, kSetHF05)) {
		if (!Game_Flag_Query(kFlagHF05CrazyLegsTalk1)) {
			talkWithCrazyLegs1();
			Game_Flag_Set(kFlagHF05CrazyLegsTalk1);
		} else if (!Game_Flag_Query(kFlagHF05CrazyLegsTalk2)
		        &&  Global_Variable_Query(kVariableChapter) == 3
		) {
			talkWithCrazyLegs2();
			Game_Flag_Set(kFlagHF05CrazyLegsTalk2);
		}
	}

	Game_Flag_Reset(kFlagHF01toHF05);
	Game_Flag_Reset(kFlagHF06toHF05);
	Game_Flag_Reset(kFlagHF07toHF05);

	//return false;
}

void SceneScriptHF05::PlayerWalkedOut() {
	if (Actor_Query_Goal_Number(kActorCrazylegs) == kGoalCrazyLegsMcCoyDrewHisGun) {
		Actor_Set_Goal_Number(kActorCrazylegs, kGoalCrazyLegsLeavesShowroom);
	}
	Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
	Ambient_Sounds_Remove_All_Looping_Sounds(1);
}

void SceneScriptHF05::DialogueQueueFlushed(int a1) {
}

void SceneScriptHF05::talkWithCrazyLegs2() {
	Loop_Actor_Walk_To_Actor(kActorCrazylegs, kActorMcCoy, 72, false, false);
	Actor_Face_Actor(kActorCrazylegs, kActorMcCoy, true);
	Actor_Face_Actor(kActorMcCoy, kActorCrazylegs, true);
	Actor_Says(kActorCrazylegs,  370, kAnimationModeTalk); // Crazylegs: Hey, Ray, how you're doing?
	Actor_Says(kActorMcCoy,     1855, kAnimationModeTalk); // McCoy: Just fine, Crazy.
	Actor_Says(kActorCrazylegs,  380,                 12); // Crazylegs: You've been thinking about the Caddy, haven't you?
	Actor_Says(kActorCrazylegs,  390,                 14); // Crazylegs: I don't blame you. She keeps me up at night too.
	Actor_Says(kActorCrazylegs,  400,                 15); // Crazylegs: And I ain't ever even lip wrestled in the back seat.
	Actor_Says(kActorCrazylegs,  410,                 16); // Crazylegs: Which is definitely what's in the cards for a stud of your caliber.
	Actor_Says(kActorMcCoy,     1860, kAnimationModeTalk); // McCoy: Throw in a babe and maybe you've got a deal.
	Actor_Says(kActorCrazylegs,  420, kAnimationModeTalk); // Crazylegs: No sweat. I'll have one over in a jiffy.
	Actor_Says(kActorMcCoy,     1865, kAnimationModeTalk); // McCoy: Nah, just kidding.
}

void SceneScriptHF05::dialogueWithCrazylegs1() {
	Dialogue_Menu_Clear_List();
	if (Actor_Clue_Query(kActorMcCoy, kClueGrigoriansNote) // cut feature? there is no way how to obtain this clue
	 && Global_Variable_Query(kVariableChapter) == 3
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1180, 3, 6, 7); // ADVERTISEMENT
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueCrazylegsInterview1)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1190, 2, 7, 4); // WOMAN
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueDektorasDressingRoom)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1200, 5, 5, 3); // WOMAN'S PHOTO
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueLucy)
	 && Actor_Query_Goal_Number(kActorLucy) != kGoalLucyGone
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1210, 4, 6, 2); // LUCY'S PHOTO
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueGrigoriansResources) // cut feature? there is no way how to obtain this clue
	 || (Actor_Clue_Query(kActorMcCoy, kClueGrigoriansNote) // cut feature? there is no way how to obtain this clue either
	  && Global_Variable_Query(kVariableChapter) == 3
	 )
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1220, -1, 2, 8); // GRIGORIAN
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueCarRegistration1)
	 || Actor_Clue_Query(kActorMcCoy, kClueCarRegistration3)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1230, 4, 7, -1); // CAR REGISTRATION
	}

	if (!Dialogue_Menu_Query_List_Size()) {
		Actor_Says(kActorMcCoy,     1880,                 15); // McCoy: I hope you got paperwork for all these vehicles.
		Actor_Says(kActorCrazylegs,  490, kAnimationModeTalk); // Crazylegs: Are we kids or what, Ray? You're talking to Crazylegs Larry Hirsch here.
		Actor_Says(kActorMcCoy,     1885, kAnimationModeTalk); // McCoy: I'll take that as a yes.
		Actor_Says(kActorCrazylegs,  500,                 16); // Crazylegs: You can take it anyway you want. Just don't take it for a test-drive in the Fourth Sector.
		return;
	}

	Dialogue_Menu_Add_DONE_To_List(1240); // DONE

	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();

	switch (answer) {
	case 1180: // ADVERTISEMENT
		Actor_Says(kActorMcCoy,     1890,                 23); // McCoy: You recognize this?
		Actor_Says(kActorCrazylegs,  510, kAnimationModeTalk); // Crazylegs: Sure I do. I get them printed out by the thousand.
		Actor_Says(kActorMcCoy,     1920,                 23); // McCoy: Okay, you recognize this?
		Actor_Says(kActorMcCoy,     1925, kAnimationModeTalk); // McCoy: This your handwriting, Crazy?
		Actor_Says(kActorCrazylegs,  530,                 12); // Crazylegs: A few customers I give the royal treatment to, you know. I'm a businessman.
		Actor_Says(kActorMcCoy,     1930,                 18); // McCoy: I found this in the possession of a Replicant.
		Actor_Says(kActorCrazylegs,  540,                 14); // Crazylegs: Huh, what--?
		Actor_Says(kActorMcCoy,     1935,                 14); // McCoy: A Replicant.
		Actor_Says(kActorCrazylegs,  550,                 16); // Crazylegs: That's impossible.
		Actor_Says(kActorMcCoy,     1940,                 15); // McCoy: You know it's illegal to sell anything to a Replicant.
		Actor_Says(kActorMcCoy,     1945,                 -1); // McCoy: Or to help a Replicant in any way.
		Actor_Says(kActorCrazylegs,  560,                 15); // Crazylegs: I didn't know! I mean it. I was just trying to make a sale--
		Actor_Says(kActorCrazylegs,  570,                 16); // Crazylegs: Hey, they came to me first. I didn't go to them.
		Actor_Says(kActorMcCoy,     1950,                 17); // McCoy: Unscrupulous businessman, eh?
		dialogueWithCrazylegs2();
		break;

	case 1190: // WOMAN
		Actor_Says(kActorMcCoy,     1895, kAnimationModeIdle); // McCoy: I gotta know more about the woman, Crazy.
		Actor_Says(kActorCrazylegs,  620, kAnimationModeTalk); // Crazylegs: Like I said, she was tall and blonde. Real woo bait.
		Actor_Says(kActorCrazylegs,  630,                 12); // Crazylegs: She was tooling around with a short guy in a bow tie.
		Actor_Says(kActorMcCoy,     2000,                 13); // McCoy: And she wanted a car.
		Actor_Says(kActorCrazylegs,  640,                 14); // Crazylegs: At first, she was looking for a very hard to find item.
		Actor_Says(kActorCrazylegs,  650,                 15); // Crazylegs: Something that doesn't show up on the lot too often.
		Actor_Says(kActorCrazylegs,  660,                 16); // Crazylegs: A... police-issue vehicle.
		Actor_Says(kActorMcCoy,     2005, kAnimationModeIdle); // McCoy: A Spinner?
		Actor_Says(kActorMcCoy,     2010, kAnimationModeTalk); // McCoy: But you didn't have one.
		Actor_Says(kActorCrazylegs,  670, kAnimationModeTalk); // Crazylegs: Well, not one that would actually fly.
		Actor_Says(kActorCrazylegs,  680,                 12); // Crazylegs: I told them to check back in a couple of days.
		Actor_Says(kActorCrazylegs,  690,                 14); // Crazylegs: So, then they got looking at that Sedan.
		Actor_Says(kActorMcCoy,     2015,                 14); // McCoy: You know how many years mandatory you'd get for selling a police Spinner?
		Actor_Says(kActorCrazylegs,  700,                 15); // Crazylegs: I was gonna report them, if they showed up again.
		Actor_Says(kActorMcCoy,     2020,                 18); // McCoy: Sure you were.
		break;

	case 1200: // WOMAN'S PHOTO
		Actor_Says(kActorMcCoy,     1900,                 23); // McCoy: This the woman that was in here?
		Actor_Says(kActorCrazylegs,  710,                 16); // Crazylegs: It doesn't do her any justice, let me tell ya.
		Actor_Says(kActorMcCoy,     2025, kAnimationModeIdle); // McCoy: Bet she's the one.
		Actor_Says(kActorCrazylegs,  720, kAnimationModeTalk); // Crazylegs: Uh-huh.
		Actor_Says(kActorCrazylegs,  730,                 12); // Crazylegs: I'd never forget her.
		break;

	case 1210: // LUCY'S PHOTO
		Actor_Says(kActorMcCoy,     1905,                 23); // McCoy: How about this girl. She look familiar?
		Actor_Says(kActorCrazylegs,  740,                 14); // Crazylegs: You kidding? She ain't old enough to drive.
		Actor_Says(kActorMcCoy,     2030,                 13); // McCoy: You've never seen her?
		Actor_Says(kActorCrazylegs,  750,                 15); // Crazylegs: She looks kinda familiar.
		Actor_Says(kActorMcCoy,     2035,                 18); // McCoy: Think real hard.
		Actor_Says(kActorCrazylegs,  760,                 16); // Crazylegs: Maybe you should try that arcade next door.
		Actor_Says(kActorCrazylegs,  770, kAnimationModeTalk); // Crazylegs: She looks like the kind of girl you see there all the time.
		Actor_Says(kActorMcCoy,     2040, kAnimationModeIdle); // McCoy: I'll do that.
		break;

	case 1220: // GRIGORIAN
		Actor_Says(kActorMcCoy,     1910, kAnimationModeTalk); // McCoy: You know a man named Spencer Grigorian?
		Actor_Says(kActorCrazylegs,  780,                 12); // Crazylegs: No, I never heard of him.
		Actor_Says(kActorMcCoy,     2045,                 17); // McCoy: Spencer Grigorian. The Citizens Against Replicant Slavery. CARS!
		Actor_Says(kActorMcCoy,     2050, kAnimationModeTalk); // McCoy: I know you know about CARS.
		Actor_Says(kActorCrazylegs,  790,                 14); // Crazylegs: Gotta be a thousand dealers in the city and you're picking on me.
		Actor_Says(kActorMcCoy,     2055,                 19); // McCoy: Grigorian was talking about friends with resources.
		Actor_Says(kActorMcCoy,     2060,                 -1); // McCoy: People who could set them up with vehicles.
		Actor_Says(kActorCrazylegs,  800,                 15); // Crazylegs: Give me a break, will ya? I'm just trying to make an honest buck.
		Actor_Says(kActorMcCoy,     2065,                 18); // McCoy: If you're honest, Crazy, I'm a doughnut.
		Actor_Says(kActorMcCoy,     2070,                 14); // McCoy: And any transactions with Replicants are illegal on Terra.
		Actor_Says(kActorCrazylegs,  810,                 16); // Crazylegs: I know, I know!
		dialogueWithCrazylegs2();
		break;

	case 1230: // CAR REGISTRATION
		Actor_Says(kActorMcCoy, 1915, 12); // McCoy: You sell a black Sedan a few days ago?
		if (Actor_Clue_Query(kActorMcCoy, kClueCarRegistration1)) {
			Actor_Says(kActorCrazylegs,  820, kAnimationModeTalk); // Crazylegs: Black Sedan? Hey, I got standards. If it ain't Sporty, I got no room for it.
			Actor_Says(kActorMcCoy,     2075,                 13); // McCoy: I've seen the registration. I know a woman bought it from you.
			Actor_Says(kActorCrazylegs,  830,                 12); // Crazylegs: Oh-- Ooh, that Sedan.
			Actor_Says(kActorCrazylegs,  840,                 14); // Crazylegs: Hey, look. A sexy blonde wants to pay top dollar for the ugliest car in the place...
			Actor_Says(kActorCrazylegs,  850,                 15); // Crazylegs: who am I to say no?
			Actor_Says(kActorMcCoy,     2080, kAnimationModeTalk); // McCoy: Did you know who she was?
			Actor_Says(kActorCrazylegs,  860,                 16); // Crazylegs: No, but I'd like to know her in the biblical sense, if you catch what I'm saying.
			Actor_Says(kActorCrazylegs,  870, kAnimationModeTalk); // Crazylegs: Look, Ray. It ain't a crime for me to make a living.
		} else if (Actor_Clue_Query(kActorMcCoy, kClueCarRegistration3)) {
			Actor_Says(kActorCrazylegs,  880,                 12); // Crazylegs: Family type cars ain't my specialty.
			Actor_Says(kActorCrazylegs,  890,                 14); // Crazylegs: I can hook you up though, if that's really what you want.
			Actor_Says(kActorMcCoy,     2085, kAnimationModeTalk); // McCoy: You sold this to a Blake Williams. I've seen the registration.
			Actor_Says(kActorCrazylegs,  900,                 15); // Crazylegs: Ooh-- Oh, that hurt Sedan.
			Actor_Says(kActorMcCoy,     2090,                 19); // McCoy: Blake Williams is a fake name.
			Actor_Says(kActorCrazylegs,  910,                 16); // Crazylegs: You just never know about some people.
			Actor_Says(kActorMcCoy,     2095,                 14); // McCoy: Did you forget to run a credit check?
			Actor_Says(kActorCrazylegs,  920, kAnimationModeTalk); // Crazylegs: He paid cash.
			Actor_Says(kActorMcCoy,     2100,                 15); // McCoy: And people come in here and buy cars with cash every day!
			Actor_Says(kActorCrazylegs,  930,                 12); // Crazylegs: In this neighborhood ever hours more like it.
			Actor_Says(kActorCrazylegs,  940,                 14); // Crazylegs: But then again I do have the best selection in town.
			Actor_Says(kActorMcCoy,     2105, kAnimationModeTalk); // McCoy: What did this guy look like?
			Actor_Says(kActorCrazylegs,  950,                 15); // Crazylegs: Nothing special. Beard, dark hair. Had these eyes, kinda...
			Actor_Says(kActorMcCoy,     2110, kAnimationModeIdle); // McCoy: Piercing?
			Actor_Says(kActorCrazylegs,  960,                 16); // Crazylegs: Yeah. Pretty scary. But, heck, I never turn away a customer.
		}
		break;
	}
}

void SceneScriptHF05::dialogueWithCrazylegs2() { // cut feature? it is impossible to trigger this dialog
	Dialogue_Menu_Clear_List();
	DM_Add_To_List_Never_Repeat_Once_Selected(1250, -1, -1, 10); // ARREST
	DM_Add_To_List_Never_Repeat_Once_Selected(1260, 10,  5, -1); // WARNING

	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();

	if (answer == 1250) { // ARREST
		Actor_Says(kActorMcCoy, 1955, 17); // McCoy: We're taking a little drive downtown.
		Actor_Says(kActorMcCoy, 1960, 23); // McCoy: Give me your wheelchair's ignition key and put your hands on top of your head.
		Item_Pickup_Spin_Effect(kModelAnimationSpinnerKeys, 315, 327);
		Delay(2000);
		Actor_Says(kActorMcCoy,     1980,                 23); // McCoy: Give me your phone.
		Actor_Says(kActorMcCoy,     1985, kAnimationModeTalk); // McCoy: I'll try to clear it so you aren't stuck in a cell with fifty mutant speed loaders
		Actor_Says(kActorCrazylegs,  580, kAnimationModeTalk); // Crazylegs: Look, I know we can work out some kind of deal here.
		Actor_Says(kActorCrazylegs,  590,                 15); // Crazylegs: You like that Bishy Caddy, right? I'll give it to you at the absolute lowest price.
		Actor_Says(kActorMcCoy,     1990,                 17); // McCoy: Sounds like a bribe to me.
		Actor_Says(kActorCrazylegs,  600,                 16); // Crazylegs: Not a bribe. Just a gift. You know, out of friendship.
		Actor_Says(kActorMcCoy,     1995, kAnimationModeTalk); // McCoy: We ain't friends, Crazy.
		Game_Flag_Set(kFlagCrazylegsArrested);
		Actor_Put_In_Set(kActorCrazylegs, kSetPS09);
		Actor_Set_At_XYZ(kActorCrazylegs, -315.15f, 0.0f, 241.06f, 583);
		Actor_Set_Goal_Number(kActorCrazylegs, kGoalCrazyLegsIsArrested);
		Game_Flag_Set(kFlagCrazylegsArrestedTalk);
		if (Game_Flag_Query(kFlagSpinnerAtNR01)) {
			Set_Enter(kSetNR01, kSceneNR01);
		} else {
			Game_Flag_Set(kFlagHF05toHF01);
			Set_Enter(kSetHF01, kSceneHF01);
		}
	} else if (answer == 1260) { // WARNING
		Actor_Says(kActorMcCoy,     1965,                 12); // McCoy: Let me give you some advice, Crazy.
		Actor_Says(kActorMcCoy,     1970, kAnimationModeTalk); // McCoy: You should start thinking about the company you keep.
		Actor_Says(kActorMcCoy,     1975, kAnimationModeTalk); // McCoy: Rather than the bucks you're trying to make.
		Actor_Says(kActorCrazylegs,  610,                 16); // Crazylegs: I gotta get my priorities straight. You're right.
	}
}

int SceneScriptHF05::getAffectionTowardsActor() {
	if (Global_Variable_Query(kVariableAffectionTowards) == kAffectionTowardsDektora
	 && Actor_Query_Goal_Number(kActorDektora) != kGoalDektoraGone
	) {
		return kActorDektora;
	}
	if (Global_Variable_Query(kVariableAffectionTowards) == kAffectionTowardsLucy
	 && Actor_Query_Goal_Number(kActorLucy) != kGoalLucyGone
	) {
		return kActorLucy;
	}
	return -1;
}

void SceneScriptHF05::policeAttack() {
	Actor_Force_Stop_Walking(kActorMcCoy);
	Actor_Put_In_Set(kActorOfficerLeary, kSetHF05);
	Actor_Set_At_XYZ(kActorOfficerLeary, 430.4f, 40.63f, -258.18f, 300);
	Actor_Put_In_Set(kActorOfficerGrayford, kSetHF05);
	Actor_Set_At_XYZ(kActorOfficerGrayford, 526.4f, 37.18f, -138.18f, 300);
	ADQ_Flush();
	ADQ_Add(kActorOfficerGrayford, 260, -1); // Officer Grayford: We're gonna nail your ass, McCoy!
	Player_Loses_Control();
	Non_Player_Actor_Combat_Mode_On(kActorOfficerLeary, kActorCombatStateUncover, true, kActorMcCoy, 4, kAnimationModeCombatIdle, kAnimationModeCombatWalk, kAnimationModeCombatRun, 0, 0, 100, 100, 1200, true);
	Non_Player_Actor_Combat_Mode_On(kActorOfficerGrayford, kActorCombatStateUncover, true, kActorMcCoy, 4, kAnimationModeCombatIdle, kAnimationModeCombatWalk, kAnimationModeCombatRun, 0, 0, 100, 100, 300, true);
}

void SceneScriptHF05::talkWithDektora() {
	Actor_Face_Actor(kActorMcCoy, kActorDektora, true);
	Actor_Face_Actor(kActorDektora, kActorMcCoy, true);
	Actor_Says(kActorDektora, 2660,                 12); // Dektora: I'll be right back.
	Actor_Says(kActorMcCoy,   8990, kAnimationModeTalk); // McCoy: What have you got there?
	Actor_Says(kActorDektora, 2670,                 13); // Dektora: One of Sadik's bombs.
	Actor_Says(kActorDektora, 2680,                 17); // Dektora: I stole it. If they want a fight--
	Actor_Says(kActorMcCoy,   8995,                 14); // McCoy: No.
	Actor_Says(kActorDektora, 2690,                 15); // Dektora: They're little people with small minds.
	Actor_Says_With_Pause(kActorMcCoy, 9000, 1.0f, 16);
	Actor_Says_With_Pause(kActorMcCoy, 9005, 1.0f, 19);
	Actor_Says(kActorMcCoy,   1765,                 17); // McCoy: Wait a minute. We're right on top of the old subway system!
	Actor_Says(kActorDektora,  160,                 12); // Dektora: But they'll follow us.
	Actor_Says(kActorMcCoy,   1770,                 15); // McCoy: Not if we can get a ground car down there.
	Actor_Says(kActorMcCoy,   1775, kAnimationModeTalk); // McCoy: Hell, we can drive right through that tunnel and disappear.
	Actor_Says(kActorDektora,  170, kAnimationModeTalk); // Dektora: What if it's collapsed?
	Actor_Says_With_Pause(kActorMcCoy, 1780, 1.0f, 18);
}

void SceneScriptHF05::talkWithLucy() {
	Actor_Face_Actor(kActorMcCoy, kActorLucy, true);
	Actor_Face_Actor(kActorLucy, kActorMcCoy, true);
	Actor_Says(kActorLucy,   400, 16); // Lucy: I won't let them shoot us down like that!
	Actor_Says(kActorMcCoy, 1750, 14); // McCoy: Where did you get that bomb?!
	Actor_Says(kActorLucy,   410, 12); // Lucy: I stole it from Sadik.
	Actor_Says(kActorLucy,   420, 14); // Lucy: We can die together! The only thing we'll feel is the love we have for each other.
	Actor_Says(kActorMcCoy, 1755, 16); // McCoy: I refuse to go out that way!
	Actor_Says(kActorLucy,   430, 18); // Lucy: We have no other choice!
	Actor_Says_With_Pause(kActorMcCoy, 1760, 1.0f, 15);
	Actor_Says(kActorMcCoy, 1765,                 17); // McCoy: Wait a minute. We're right on top of the old subway system!
	Actor_Says(kActorLucy,   440, kAnimationModeTalk); // Lucy: But they'll follow us!
	Actor_Says(kActorMcCoy, 1770,                 15); // McCoy: Not if we can get a ground car down there.
	Actor_Says(kActorMcCoy, 1775, kAnimationModeTalk); // McCoy: Hell, we can drive right through that tunnel and disappear.
	Actor_Says(kActorLucy,   450,                 17); // Lucy: What is it's collapsed?
	Actor_Says_With_Pause(kActorMcCoy, 1780, 1.0f, 18);
}

void SceneScriptHF05::talkWithCrazylegs3(int affectionTowardsActor) {
	if (affectionTowardsActor != -1
	 && Actor_Query_In_Set(kActorCrazylegs, kSetHF05)
	) {
		Async_Actor_Walk_To_Waypoint(affectionTowardsActor, 437, 36, false);
		Loop_Actor_Walk_To_Waypoint(kActorMcCoy, 437, 0, false, false);
		Actor_Face_Actor(kActorCrazylegs, kActorMcCoy, true);
		Actor_Face_Actor(kActorMcCoy, kActorCrazylegs, true);
		Actor_Face_Actor(affectionTowardsActor, kActorCrazylegs, true);
		Actor_Says(kActorCrazylegs,    0, kAnimationModeTalk); // Crazylegs: How's it going, Ray?
		Actor_Says(kActorCrazylegs,   10,                 12); // Crazylegs: Wait a minute, I thought you was going to arrest her.
		Actor_Says(kActorMcCoy,     1715,                 19); // McCoy: You don't get paid the big bucks to think, Crazy.
		Actor_Says(kActorMcCoy,     1720,                 -1); // McCoy: Any of your Spinners up and running?
		Actor_Says(kActorCrazylegs,   20,                 14); // Crazylegs: Uh... Sure, got one up on the roof.
		Actor_Says(kActorCrazylegs,   30,                 15); // Crazylegs: A real beaut. Ain't a cheap ride, though, I'll tell you right now.
		Actor_Says(kActorMcCoy,     1725, kAnimationModeTalk); // McCoy: I gotta take it for a test drive.
		Actor_Says(kActorCrazylegs,   40,                 16); // Crazylegs: Ray, I-- I always liked you...
		Actor_Says(kActorCrazylegs,   50, kAnimationModeTalk); // Crazylegs: True, I hardly know you. You seem like a stand up guy.
		Actor_Says(kActorCrazylegs,   60,                 12); // Crazylegs: Eventually, when this fiasco is all over and done with I--
		Actor_Says(kActorCrazylegs,   70,                 13); // Crazylegs: I know you'll get me on the com, right?
		Actor_Says(kActorMcCoy,     1730, kAnimationModeTalk); // McCoy: You're a stand up guy, Crazy.
		Loop_Actor_Walk_To_Actor(kActorCrazylegs, kActorMcCoy, 28, false, false);
		Item_Pickup_Spin_Effect(kModelAnimationSpinnerKeys, 315, 327);
		Actor_Says(kActorCrazylegs, 80, 23); // Crazylegs: That I am.
		Actor_Clue_Acquire(kActorMcCoy, kClueSpinnerKeys, true, kActorCrazylegs);
		Actor_Says(kActorCrazylegs,   90,                 15); // Crazylegs: You want me to tell your friend where you're going?
		Actor_Says(kActorMcCoy,     1735,                 17); // McCoy: Friend?
		Actor_Says(kActorCrazylegs,  100,                 16); // Crazylegs: You know. The doll with the cigarette.
		Actor_Says(kActorCrazylegs,  110, kAnimationModeTalk); // Crazylegs: She told me you were in trouble and she wanted to help. What should I tell her?
		Actor_Face_Actor(affectionTowardsActor, kActorMcCoy, true);
		if (affectionTowardsActor == kActorDektora) {
			Actor_Says(kActorDektora, 90, kAnimationModeTalk); // Dektora: Ray!?
		} else {
			Actor_Says(kActorLucy, 380, kAnimationModeTalk); // Lucy: Ray.
		}
		Actor_Says(kActorMcCoy,     1740, 14); // McCoy: You tell her we're headed South.
		Actor_Says(kActorCrazylegs,  120, 12); // Crazylegs: Ten Four.
		Actor_Set_Goal_Number(kActorCrazylegs, kGoalCrazyLegsLeavesShowroom);
		if (affectionTowardsActor == kActorDektora) {
			Actor_Says(kActorDektora, 100, kAnimationModeTalk); // Dektora: I'm afraid, Ray.
		} else {
			Actor_Says(kActorLucy, 390, kAnimationModeTalk); // Lucy: I'm scared, Ray.
		}
		Actor_Face_Actor(kActorMcCoy, affectionTowardsActor, true);
		Actor_Says(kActorMcCoy, 1745, kAnimationModeTalk); // McCoy: Crystal ain't gonna let up until we're both six feet under.
		Async_Actor_Walk_To_XYZ(affectionTowardsActor, 309.0f, 40.63f, 402.0f, 0, false);
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, 277.0f, 40.63f, 410.0f, 0, false, false, false);
		Game_Flag_Set(kFlagHF05toHF06);
		Set_Enter(kSetHF06, kSceneHF06);
	}
}

void SceneScriptHF05::talkWithCrazyLegs1() {
	Player_Loses_Control();
	if (Global_Variable_Query(kVariableChapter) == 3) {
		ADQ_Flush();
		ADQ_Add(kActorCrazylegs, 130, 18); // Crazylegs: It's a killer, I kid you not. Only thirty five thou...
		ADQ_Add(kActorCrazylegs, 140, 18); // Crazylegs: Yeah, red, orange, blue, puke green. Any color you want.
		ADQ_Add(kActorCrazylegs, 150, 18); // Crazylegs: Just give me 24 to hose the graffiti off--
		ADQ_Add(kActorCrazylegs, 160, 17); // Crazylegs: Just kidding. Ask for the man; Crazylegs Larry, toot-a-loo.
	}
	Loop_Actor_Walk_To_XYZ(kActorMcCoy, 307.0f, 40.63f, 184.0f, 0, false, false, false);
	Loop_Actor_Walk_To_Actor(kActorCrazylegs, kActorMcCoy, 72, false, false);
	Ambient_Sounds_Play_Sound(kSfxLABMISC4, 99, 99, 0, 0);
	Actor_Face_Actor(kActorCrazylegs, kActorMcCoy, true);
	Actor_Face_Actor(kActorMcCoy, kActorCrazylegs, true);
#if BLADERUNNER_ORIGINAL_BUGS
#else
	// There is a chance here that Crazylegs will "interrupt himself"
	// and thus sometimes skip the last sentence of the above queued dialogue in chapter 3.
	// So we explicitly wait for the queue to be emptied before proceeding to his next line
	ADQ_Wait_For_All_Queued_Dialogue();
#endif // BLADERUNNER_ORIGINAL_BUGS
	Actor_Says(kActorCrazylegs,  170, kAnimationModeTalk); // Crazylegs: Admiring that Bishy Caddy, ain't ya?
	Actor_Says(kActorCrazylegs,  180,                 12); // Crazylegs: I don't blame you. It's a classic ride in a deluxe sport package.
	Actor_Says(kActorCrazylegs,  190,                 14); // Crazylegs: It will push 125 without so much as a shake.
	Actor_Says(kActorCrazylegs,  200,                 15); // Crazylegs: Of course you gotta find the road for it.
	Actor_Says(kActorMcCoy,     1815,                 12); // McCoy: That's always the problem.
	Actor_Says(kActorCrazylegs,  210,                 16); // Crazylegs: Crazylegs Larry Hirsch. Pleased to meet ya.
	Actor_Says(kActorMcCoy,     1820,                 -1); // McCoy: Ray McCoy.
	Actor_Says(kActorCrazylegs,  220, kAnimationModeTalk); // Crazylegs: You can call me Crazy, you can call me Larry...
	Actor_Says(kActorCrazylegs,  230,                 12); // Crazylegs: You can even call me Crazylegs Larry.
	Actor_Says(kActorCrazylegs,  240,                 14); // Crazylegs: But don't call me, if you don't think a hot set of wheels ain't necessary.
	Actor_Says(kActorMcCoy,     1825, kAnimationModeIdle); // McCoy: Okay.
	Actor_Says(kActorCrazylegs,  250,                 15); // Crazylegs: The Bishy is exceptional. Mechanical is Pacific Rim Flawless as usual.
	Actor_Face_Object(kActorCrazylegs, "MONTE CARLO DRY", true);
	Actor_Says(kActorCrazylegs, 260, 16); // Crazylegs: Yeah, but they also save some of the classic styling of the old GM days.
	Actor_Face_Object(kActorMcCoy, "MONTE CARLO DRY", true);
	Actor_Says(kActorMcCoy, 1830, kAnimationModeIdle); // McCoy: Looks like a piece of work.
	Actor_Face_Actor(kActorCrazylegs, kActorMcCoy, true);
	Actor_Face_Actor(kActorMcCoy, kActorCrazylegs, true);
	Actor_Says(kActorCrazylegs, 270, kAnimationModeTalk); // Crazylegs: Forget those Fibro Plast Filipino heaps everyone is driving around.
	Actor_Says(kActorCrazylegs, 280,                 12); // Crazylegs: This is real. Just feast your eyes on this.
	Async_Actor_Walk_To_XYZ(kActorCrazylegs, 276.0f, 40.63f, 182.0f, 12, false);
	Loop_Actor_Walk_To_XYZ(kActorMcCoy, 335.0f, 40.63f, 131.0f, 12, false, false, false);
	Actor_Face_Object(kActorCrazylegs, "MONTE CARLO DRY", true);
	Actor_Face_Object(kActorMcCoy, "MONTE CARLO DRY", true);
	Actor_Says(kActorCrazylegs,  290, 14); // Crazylegs: Ya see it? Ya see it, that's actual chrome, the real stuff.
	Actor_Says(kActorCrazylegs,  300, 15); // Crazylegs: They saved a couple of hundred sheets of Tijuana Chrome from before the war.
	Actor_Says(kActorCrazylegs,  310, 16); // Crazylegs: It's completely legal and everything. Beautiful ain't it?
	Actor_Says(kActorMcCoy,     1835, 12); // McCoy: I don't think I've ever seen it before.
	Actor_Face_Actor(kActorCrazylegs, kActorMcCoy, true);
	Actor_Says(kActorCrazylegs, 320, kAnimationModeTalk); // Crazylegs: And you probably won't ever see it again unless you steal this puppy.
	Actor_Face_Actor(kActorMcCoy, kActorCrazylegs, true);
	Actor_Says(kActorCrazylegs,  330,                 12); // Crazylegs: And, honestly... No, frankly, I gotta tell you it's got your name written all over it: Ray McCovey.
	Actor_Says(kActorMcCoy,     1840, kAnimationModeTalk); // McCoy: McCoy.
	Actor_Says(kActorCrazylegs,  340,                 14); // Crazylegs: Uh, give or take a couple of letters.
	Actor_Says(kActorMcCoy,     1845, kAnimationModeTalk); // McCoy: I'll have to think about it.
	Actor_Says(kActorCrazylegs,  350,                 15); // Crazylegs: Don't think too long or too hard about it, Roy.
	Actor_Says(kActorCrazylegs,  360,                 16); // Crazylegs: It's liable to be gonezo by the time you come back.
	Actor_Says(kActorMcCoy,     1850, kAnimationModeTalk); // McCoy: Gotcha.
	Player_Gains_Control();
}

void SceneScriptHF05::addAmbientSounds() {
	Ambient_Sounds_Add_Sound(kSfxSIREN2, 20, 80, 20, 100, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Speech_Sound(kActorOfficerLeary,    250, 5, 70, 7, 10, -50, 50, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorOfficerLeary,    330, 5, 70, 7, 10, -50, 50, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorOfficerLeary,    340, 5, 90, 7, 10, -50, 50, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorOfficerLeary,    360, 5, 70, 7, 10, -50, 50, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorOfficerGrayford, 380, 5, 70, 7, 10, -50, 50, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorOfficerGrayford, 510, 5, 70, 7, 10, -50, 50, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorDispatcher,       80, 5, 70, 7, 10, -50, 50, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorDispatcher,      160, 5, 70, 7, 10, -50, 50, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorDispatcher,      280, 5, 70, 7, 10, -50, 50, -101, -101, 1, 1);
}

int SceneScriptHF05::getCompanionActor() {
	if (Actor_Query_In_Set(kActorDektora, kSetHF05)
	 && Actor_Query_Goal_Number(kActorDektora) != kGoalDektoraGone
	) {
		return kActorDektora;
	}

	if (Actor_Query_In_Set(kActorLucy, kSetHF05)
	 && Actor_Query_Goal_Number(kActorLucy) != kGoalLucyGone
	) {
		return kActorLucy;
	}

	return kActorMcCoy;
}

} // End of namespace BladeRunner
