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

enum kHF01Loops {
	kHF01LoopInshot            = 0,
	kHF01LoopMainLoopSpinner   = 1,
	kHF01LoopDoorAnim          = 3,
	kHF01LoopOutshot           = 4,
	kHF01LoopMainLoopNoSpinner = 5
};

void SceneScriptHF01::InitializeScene() {
	if (Game_Flag_Query(kFlagSpinnerMissing)) {
		Setup_Scene_Information(243.94f, 8.0f, -341.9f, 342);
	} else if (Game_Flag_Query(kFlagHF05toHF01)) {
		Setup_Scene_Information(-202.0f, 0.0f, -619.0f, 407);
	} else if (Game_Flag_Query(kFlagHF03toHF01)) {
		Setup_Scene_Information( 124.0f, 8.0f, -880.0f, 455);
	} else if (Game_Flag_Query(kFlagHF02toHF01)) {
		Setup_Scene_Information( 406.0f, 8.0f, -813.0f, 455);
	} else {
		Setup_Scene_Information( 100.0f, 0.0f, -260.0f,   0);
	}

	Scene_Exit_Add_2D_Exit(0, 81, 226, 169, 321, 0);
	if (!Game_Flag_Query(kFlagHF05PoliceArrived)) {
		Scene_Exit_Add_2D_Exit(1, 304, 239, 492, 339, 0);
		Scene_Exit_Add_2D_Exit(2, 560, 231, 639, 360, 0);
		if (Game_Flag_Query(kFlagSpinnerAtHF01)) {
			Scene_Exit_Add_2D_Exit(3, 0, 311, 66, 417, 2);
		}
	}

	Ambient_Sounds_Add_Looping_Sound(kSfxCTRAIN1, 50,   0, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxARCBED1, 25,   0, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxRCAMBR1, 60, 100, 1);
	Ambient_Sounds_Add_Sound(kSfx67_0480R,  5,  70, 12,  12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0540R,  5,  70, 12,  12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0560R,  5,  70, 12,  12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0870R,  5,  70, 12,  12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0900R,  5,  70, 12,  12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0940R,  5,  70, 12,  12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_1070R,  5,  70, 12,  12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_1080R,  5,  70, 12,  12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_1160R,  5,  70, 12,  12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSPIN2B,   10, 180, 16,  25,    0,   0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSPIN3A,   10, 180, 16,  25,    0,   0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxTHNDER2,  10, 180, 50, 100,    0,   0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxTHNDER3,  10, 180, 50, 100,    0,   0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxTHNDER4,  10, 180, 50, 100,    0,   0, -101, -101, 0, 0);

	if (Game_Flag_Query(kFlagSpinnerAtHF01)) {
		if (!Game_Flag_Query(kFlagHF02toHF01)
		 && !Game_Flag_Query(kFlagHF03toHF01)
		 && !Game_Flag_Query(kFlagHF05toHF01)
		) {
			Scene_Loop_Start_Special(kSceneLoopModeLoseControl, kHF01LoopInshot, false);
		}
		Scene_Loop_Set_Default(kHF01LoopMainLoopSpinner);
	} else if (Game_Flag_Query(kFlagHF05PoliceArrived)) {
		Scene_Loop_Set_Default(kHF01LoopMainLoopSpinner);
	} else {
		Scene_Loop_Set_Default(kHF01LoopMainLoopNoSpinner);
	}
}

void SceneScriptHF01::SceneLoaded() {
	Obstacle_Object("LOFT41", true);
	if (!Game_Flag_Query(kFlagSpinnerAtHF01)) {
		Unobstacle_Object("OBSTACLE BOX15", true);
	}
}

bool SceneScriptHF01::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptHF01::ClickedOn3DObject(const char *objectName, bool a2) {
	return false;
}

bool SceneScriptHF01::ClickedOnActor(int actorId) {
	int affectionTowardsActor;
	if (Global_Variable_Query(kVariableAffectionTowards) == kAffectionTowardsDektora) {
		affectionTowardsActor = kActorDektora;
	} else if (Global_Variable_Query(kVariableAffectionTowards) == kAffectionTowardsLucy) {
		affectionTowardsActor = kActorLucy;
	} else {
		affectionTowardsActor = -1;
	}

	if (actorId == kActorMia
	 || actorId == kActorMurray
	) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 504.04f, 8.0f, -242.17f, 12, true, false, false)) {
			ADQ_Flush();
			Actor_Face_Actor(kActorMcCoy, kActorMurray, true);
			if (!Game_Flag_Query(kFlagHF01MurrayMiaTalk)) {
				Actor_Says(kActorMcCoy,  1455, 15); // McCoy: You guys still battling it out?
				Actor_Says(kActorMurray,   40, 13); // Murray: That's what keeps our marriage alive and kicking, Ray. What can we do you for?
				Actor_Says(kActorMcCoy,  1460, 13); // McCoy: The works.
				Actor_Says(kActorMurray,   50, 12); // Murray: Coming right up.
				Item_Pickup_Spin_Effect(kModelAnimationLichenDogWrapper, 396, 359);
				Actor_Face_Heading(kActorMurray, 271, false);
				Actor_Says(kActorMurray,   60, 12); // Murray: You know anything about politics, Ray? Famous quotes, things like that?
				Actor_Says(kActorMcCoy,  1465, 15); // McCoy: I try not to.
				Actor_Face_Actor(kActorMurray, kActorMcCoy, true);
				Actor_Says(kActorMurray,   70,                 13); // Murray: How about the Checkers speech?
				Actor_Says(kActorMcCoy,  1470,                 14); // McCoy: Never heard of it.
				Actor_Says(kActorMia,      30, kAnimationModeTalk); // Mia: Hoover's Checkers speech.
				Actor_Says(kActorMurray,   80,                 15); // Murray: Nixon's Checkers speech.
				Actor_Says(kActorMia,      50, kAnimationModeTalk); // Mia: (snickers) You're forgetting a lot in your old age.
				Actor_Says(kActorMurray,  110,                 14); // Murray: Right behind you, lady.
				Game_Flag_Set(kFlagHF01MurrayMiaTalk);
			} else {
				dialogueWithMiaAndMurray();
			}
			return true;
		}
		return false;
	}

	if (affectionTowardsActor != -1
	 && actorId == affectionTowardsActor
	) {
		if (!Loop_Actor_Walk_To_Actor(kActorMcCoy, actorId, 28, true, false)) {
			if (Actor_Query_Goal_Number(affectionTowardsActor) == 599) {
				Actor_Says(kActorMcCoy, 8630, 13); // McCoy: What a waste.
				return true;
			}

			if (Global_Variable_Query(kVariableChapter) == 5
			 && (Actor_Clue_Query(kActorMcCoy, kCluePhoneCallDektora1)
			  || Actor_Clue_Query(kActorMcCoy, kCluePhoneCallLucy1)
			 )
			 && !Game_Flag_Query(kFlagCrazylegsArrested)
			 &&  Actor_Query_Goal_Number(kActorCrazylegs) != kGoalCrazyLegsLeavesShowroom
			 &&  Game_Flag_Query(kFlagMcCoyIsHelpingReplicants)
			 && !Game_Flag_Query(kFlagHF01TalkToLovedOne)
			) {
				Game_Flag_Set(kFlagHF01TalkToLovedOne);
				Actor_Face_Actor(kActorMcCoy, affectionTowardsActor, true);
				Actor_Face_Actor(affectionTowardsActor, kActorMcCoy, true);
				if (affectionTowardsActor == kActorDektora) {
					talkWithDektora();
				} else if (affectionTowardsActor == kActorLucy) {
					talkWithLucy();
				}
				Async_Actor_Walk_To_XYZ(affectionTowardsActor, -175.0f, 8.0f, -617.0f, 0, false);
				Loop_Actor_Walk_To_XYZ(kActorMcCoy, -137.0f, 8.0f, -577.0f, 0, false, false, true);
				Game_Flag_Set(kFlagHF01toHF05);
				Set_Enter(kSetHF05, kSceneHF05);
			}
		}
	}
	return false;
}

bool SceneScriptHF01::ClickedOnItem(int itemId, bool a2) {
	return false;
}

bool SceneScriptHF01::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -202.0f, 8.0f, -619.0f, 0, true, false, false)) {
			Game_Flag_Set(kFlagHF01toHF05);
			Set_Enter(kSetHF05, kSceneHF05);
		}
		return true;
	}

	if (exitId == 1) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 124.0f, 8.0f, -724.0f, 0, true, false, false)) {
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, 124.0f, 8.0f, -880.0f, 0, false, false, false);
			Game_Flag_Set(kFlagHF01toHF03);
			Set_Enter(kSetHF03, kSceneHF03);
		}
		return true;
	}

	if (exitId == 2) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 406.0f, 8.0f, -717.0f, 0, true, false, false)) {
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, 406.0f, 8.0f, -813.0f, 0, false, false, false);
			Game_Flag_Set(kFlagHF01toHF02);
			Set_Enter(kSetHF02, kSceneHF02);
		}
		return true;
	}

	if (exitId == 3) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 100.0f, 0.0f, -260.0f, 0, true, false, false)) {
			Game_Flag_Reset(kFlagMcCoyInChinaTown);
			Game_Flag_Reset(kFlagMcCoyInRunciters);
			Game_Flag_Reset(kFlagMcCoyInMcCoyApartment);
			Game_Flag_Reset(kFlagMcCoyInPoliceStation);
			Game_Flag_Reset(kFlagMcCoyInBradburyBuilding);
			Game_Flag_Reset(kFlagMcCoyInHysteriaHall);
			int spinnerDest = Spinner_Interface_Choose_Dest(kHF01LoopDoorAnim, false);
			switch (spinnerDest) {
			case kSpinnerDestinationPoliceStation:
				Game_Flag_Set(kFlagMcCoyInPoliceStation);
				Game_Flag_Reset(kFlagSpinnerAtHF01);
				Game_Flag_Set(kFlagSpinnerAtPS01);
				Set_Enter(kSetPS01, kScenePS01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kHF01LoopOutshot, true);
				break;
			case kSpinnerDestinationMcCoysApartment:
				Game_Flag_Set(kFlagMcCoyInMcCoyApartment);
				Game_Flag_Reset(kFlagSpinnerAtHF01);
				Game_Flag_Set(kFlagSpinnerAtMA01);
				Set_Enter(kSetMA01, kSceneMA01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kHF01LoopOutshot, true);
				break;
			case kSpinnerDestinationRuncitersAnimals:
				Game_Flag_Set(kFlagMcCoyInRunciters);
				Game_Flag_Reset(kFlagSpinnerAtHF01);
				Game_Flag_Set(kFlagSpinnerAtRC01);
				Set_Enter(kSetRC01, kSceneRC01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kHF01LoopOutshot, true);
				break;
			case kSpinnerDestinationChinatown:
				Game_Flag_Set(kFlagMcCoyInChinaTown);
				Game_Flag_Reset(kFlagSpinnerAtHF01);
				Game_Flag_Set(kFlagSpinnerAtCT01);
				Set_Enter(kSetCT01_CT12, kSceneCT01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kHF01LoopOutshot, true);
				break;
			case kSpinnerDestinationTyrellBuilding:
				Game_Flag_Set(kFlagMcCoyInTyrellBuilding);
				Game_Flag_Reset(kFlagSpinnerAtHF01);
				Game_Flag_Set(kFlagSpinnerAtTB02);
				Set_Enter(kSetTB02_TB03, kSceneTB02);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kHF01LoopOutshot, true);
				break;
			case kSpinnerDestinationAnimoidRow:
				Game_Flag_Set(kFlagMcCoyInAnimoidRow);
				Game_Flag_Reset(kFlagSpinnerAtHF01);
				Game_Flag_Set(kFlagSpinnerAtAR01);
				Set_Enter(kSetAR01_AR02, kSceneAR01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kHF01LoopOutshot, true);
				break;
			case kSpinnerDestinationDNARow:
				Game_Flag_Set(kFlagMcCoyInDNARow);
				Game_Flag_Reset(kFlagSpinnerAtHF01);
				Game_Flag_Set(kFlagSpinnerAtDR01);
				Set_Enter(kSetDR01_DR02_DR04, kSceneDR01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kHF01LoopOutshot, true);
				break;
			case kSpinnerDestinationBradburyBuilding:
				Game_Flag_Set(kFlagMcCoyInBradburyBuilding);
				Game_Flag_Reset(kFlagSpinnerAtHF01);
				Game_Flag_Set(kFlagSpinnerAtBB01);
				Set_Enter(kSetBB01, kSceneBB01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kHF01LoopOutshot, true);
				break;
			case kSpinnerDestinationNightclubRow:
				Game_Flag_Set(kFlagMcCoyInNightclubRow);
				Game_Flag_Reset(kFlagSpinnerAtHF01);
				Game_Flag_Set(kFlagSpinnerAtNR01);
				Set_Enter(kSetNR01, kSceneNR01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kHF01LoopOutshot, true);
				break;
			default:
				Game_Flag_Set(kFlagMcCoyInHysteriaHall);
				Loop_Actor_Walk_To_XYZ(kActorMcCoy, 100.0f, 0.0f, -300.0f, 0, true, false, false);
				break;
			}
		}
		return true;
	}
	return false;
}

bool SceneScriptHF01::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptHF01::SceneFrameAdvanced(int frame) {
	if (frame == 10) {
		Sound_Play(kSfxCARDOWN3, 40, 0, 0, 50);
	}

	if (frame == 72
	 || frame == 193
	) {
		Sound_Play(kSfxSPINOPN4, 100, -50, -50, 50);
	}

	if (frame == 88
	 || frame == 214
	) {
		Sound_Play(kSfxSPINCLS1, 100, -50, -50, 50);
	}

	if (frame == 242) {
		Sound_Play(kSfxCARUP3,    40, -50, 80, 50);
	}

	if (Actor_Query_Goal_Number(kActorMurray) == 1) {
		Actor_Set_Goal_Number(kActorMurray, 0);
	}
}

void SceneScriptHF01::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptHF01::PlayerWalkedIn() {
	if (Game_Flag_Query(kFlagHF05PoliceArrived)) {
		ADQ_Flush();
		ADQ_Add(kActorOfficerGrayford, 280, kAnimationModeTalk); // Officer Grayford: It's McCoy! Get him!
		Actor_Put_In_Set(kActorOfficerLeary, kSetHF01);
		Actor_Set_At_XYZ(kActorOfficerLeary, 8.2f, 8.0f, -346.67f, 1021);
		Actor_Put_In_Set(kActorOfficerGrayford, kSetHF01);
		Actor_Set_At_XYZ(kActorOfficerGrayford, 51.21f, 8.0f, -540.78f, 796);
		Non_Player_Actor_Combat_Mode_On(kActorOfficerLeary, kActorCombatStateUncover, true, kActorMcCoy, 4, kAnimationModeCombatIdle, kAnimationModeCombatWalk, kAnimationModeCombatRun, 0, 0, 0, 100, 300, false);
		Non_Player_Actor_Combat_Mode_On(kActorOfficerGrayford, kActorCombatStateUncover, true, kActorMcCoy, 4, kAnimationModeCombatIdle, kAnimationModeCombatWalk, kAnimationModeCombatRun, 0, 0, 0, 100, 300, false);
	}

	if (!Game_Flag_Query(kFlagCrazylegsArrested)
	 &&  Actor_Query_Goal_Number(kActorCrazylegs) != kGoalCrazyLegsLeavesShowroom
	) {
		if (Actor_Clue_Query(kActorMcCoy, kCluePhoneCallLucy1)
		 && Global_Variable_Query(kVariableAffectionTowards) == kAffectionTowardsLucy
		 && Actor_Query_Goal_Number(kActorLucy) != kGoalLucyGone
		) {
			Actor_Put_In_Set(kActorLucy, kSetHF01);
			Actor_Set_At_XYZ(kActorLucy, -5.0f, 8.0f, -622.0f, 419);
			Actor_Set_Targetable(kActorLucy, true);
		} else if (Actor_Clue_Query(kActorMcCoy, kCluePhoneCallDektora1)
		        && Global_Variable_Query(kVariableAffectionTowards) == kAffectionTowardsDektora
		        && Actor_Query_Goal_Number(kActorDektora) != kGoalDektoraGone
		) {
			Actor_Put_In_Set(kActorDektora, kSetHF01);
			Actor_Set_At_XYZ(kActorDektora, -5.0f, 8.0f, -622.0f, 419);
			Actor_Set_Targetable(kActorDektora, true);
		}
	}

	if (Game_Flag_Query(kFlagSpinnerMissing)) {
		Actor_Set_Goal_Number(kActorSteele, kGoalSteeleTalkAboutMissingSpinner);
		Game_Flag_Reset(kFlagSpinnerMissing);
		//return true;
		return;
	}

	if (Game_Flag_Query(kFlagCrazylegsArrestedTalk)) {
		Game_Flag_Reset(kFlagCrazylegsArrestedTalk);
		Actor_Voice_Over(950, kActorVoiceOver); // Mainframe: Crazylegs was a small fish looking to make a few bucks in a big ocean.
		Actor_Voice_Over(960, kActorVoiceOver); // Mainframe: And he'd ended up swimming with the sharks.
		Actor_Voice_Over(970, kActorVoiceOver); // Mainframe: Still the last couple of hours hadn't been a total loss.
		Actor_Voice_Over(980, kActorVoiceOver); // Mainframe: I knew what the Reps were looking for. Some way to get out of the city.
	} else if (!Game_Flag_Query(kFlagHF01MurrayMiaIntro)
	        &&  Global_Variable_Query(kVariableChapter) < 4
	) {
		ADQ_Flush();
		ADQ_Add(kActorMurray,  0,                 14); // Murray: It was "The Trickster". He said: "Pat doesn't have a mink coat."
		ADQ_Add(kActorMurray, 10, kAnimationModeTalk); // Murray: "She's got a good Republican cloth coat. That's all."
		ADQ_Add(kActorMia,     0, kAnimationModeTalk); // Mia: You're wrong, sweetie. It was Herbert Hoover.
		Actor_Face_Actor(kActorMurray, kActorMia, true);
		ADQ_Add(kActorMurray, 20,                 13); // Murray: I'm sorry, but I was there.
		ADQ_Add(kActorMia,    10, kAnimationModeTalk); // Mia: Bah-- If anything you were all of three years old back then.
		ADQ_Add(kActorMurray, 30, kAnimationModeTalk); // Murray: I still remember it! Like it was yesterday.
		ADQ_Add(kActorMia,    20, kAnimationModeTalk); // Mia: And he said fur coat not mink. I saw it on the Discovery disc.
		Actor_Face_Heading(kActorMurray, 271, false);
		Game_Flag_Set(kFlagHF01MurrayMiaIntro);
	}

	if (Game_Flag_Query(kFlagHF03toHF01)) {
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, 124.0f, 8.0f, -724.0f, 0, true, false, false);
	} else if (Game_Flag_Query(kFlagHF02toHF01)) {
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, 406.0f, 8.0f, -717.0f, 0, true, false, false);
	} else if (!Game_Flag_Query(kFlagHF05toHF01)) {
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, 100.0f, 0.0f, -300.0f, 0, true, false, false);
	}

	Game_Flag_Reset(kFlagHF03toHF01);
	Game_Flag_Reset(kFlagHF02toHF01);
	Game_Flag_Reset(kFlagHF05toHF01);
	//return false;
}

void SceneScriptHF01::PlayerWalkedOut() {
	Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
	Ambient_Sounds_Remove_All_Looping_Sounds(1);

	if (!Game_Flag_Query(kFlagHF01toHF05)
	 && !Game_Flag_Query(kFlagHF01toHF02)
	 && !Game_Flag_Query(kFlagHF01toHF03)
	 && !Game_Flag_Query(kFlagChapter4Starting)
	) {
#if BLADERUNNER_ORIGINAL_BUGS
		Outtake_Play(kOuttakeInside2,  true, -1);
		Outtake_Play(kOuttakeTowards3, true, -1);
#else
		if (!Game_Flag_Query(kFlagMcCoyInTyrellBuilding)) {
			// don't play an extra outtake when going to Tyrell Building
			Outtake_Play(kOuttakeInside2,  true, -1);
			Outtake_Play(kOuttakeTowards3, true, -1);
		}
#endif // BLADERUNNER_ORIGINAL_BUGS
	}
	Game_Flag_Reset(kFlagChapter4Starting);

	if (Actor_Query_Goal_Number(kActorLucy) == 450) {
		Actor_Put_In_Set(kActorLucy, kSetFreeSlotG);
		Actor_Set_At_Waypoint(kActorLucy, 39, 0);
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyGone);
	}

	if (Actor_Query_Goal_Number(kActorDektora) == 450) {
		Actor_Put_In_Set(kActorDektora, kSetFreeSlotG);
		Actor_Set_At_Waypoint(kActorDektora, 39, 0);
		Actor_Set_Goal_Number(kActorDektora, kGoalDektoraGone);
	}
}

void SceneScriptHF01::DialogueQueueFlushed(int a1) {
}

void SceneScriptHF01::dialogueWithMiaAndMurray() {
	Dialogue_Menu_Clear_List();
	if (Actor_Clue_Query(kActorMcCoy, kClueLucy)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(440, 8, 6, 3); // LUCY PHOTO
	}
	if (!Actor_Clue_Query(kActorMcCoy, kClueLucy)
	 &&  Actor_Clue_Query(kActorMcCoy, kClueRunciterInterviewA)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(450, 7, 6, 3); // LUCY
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueCheese)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(460, 3, 5, 6); // CHEESE
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueLichenDogWrapper)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(470, -1, 3, 8); // LICHEN DOG WRAPPER
	}
	Dialogue_Menu_Add_DONE_To_List(480); // DONE

	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();

	switch (answer) {
	case 440: // LUCY PHOTO
		Actor_Says(kActorMcCoy,  1480,                 15); // McCoy: You ever seen this girl around here, maybe in the arcade?
		Actor_Says(kActorMia,      70, kAnimationModeTalk); // Mia: Oh, yes! I saw her come out of the arcade yesterday.
		Actor_Says(kActorMurray,  120, kAnimationModeTalk); // Murray: No, it was the day before.
		Actor_Says(kActorMcCoy,  1505,                 14); // McCoy: Maybe you saw her both days.
		Actor_Says(kActorMia,      80, kAnimationModeTalk); // Mia: No, I'm positive. It was yesterday.
		Actor_Says(kActorMurray,  130, kAnimationModeTalk); // Murray: You're positively wrong. It was two days ago, sure as God made little green apples.
		Actor_Says(kActorMia,      90, kAnimationModeTalk); // Mia: You're getting Alzheimer's, sweetie.
		Actor_Says(kActorMcCoy,  1510,                 12); // McCoy: Okay, okay. Just forget it.
		break;

	case 450: // LUCY
		Actor_Says(kActorMcCoy,  1485,                 16); // McCoy: You seen a girl around here? About fourteen with pink hair... Maybe in the arcade?
		Actor_Says(kActorMia,     100, kAnimationModeTalk); // Mia: I was fourteen with pink hair once. (snickers) A long time ago.
		Actor_Says(kActorMurray,  140,                 12); // Murray: Don't think he means you.
		Actor_Says(kActorMia,     110, kAnimationModeTalk); // Mia: Of course not, not now. We work outside Hysteria Hall, don't we?
		Actor_Says(kActorMia,     120, kAnimationModeTalk); // Mia: Well, we must see twenty, thirty girls a day with pink hair.
		Actor_Says(kActorMurray,  150,                 14); // Murray: And every other color in the rainbow.
		break;

	case 460: // CHEESE
		Actor_Says(kActorMcCoy,  1490,                 13); // McCoy: You know where I could score some cheese, like this stuff?
		Actor_Says(kActorMurray,  160,                 15); // Murray: Oy, don't flash that around here!
		Actor_Says(kActorMia,     130,                 13); // Mia: He's the police, sweetie.
		Actor_Says(kActorMurray,  170,                 12); // Murray: Besides, the kids today...
		Actor_Says(kActorMurray,  180,                 13); // Murray: just as soon twist a knife in your back side as to ask for chinyen for the tram home.
		Actor_Says(kActorMurray,  190,                 14); // Murray: You show that cheese around here, you become a marked man.
		Actor_Says(kActorMcCoy,  1515,                 15); // McCoy: Who sells it?
		Actor_Says(kActorMurray,  200, kAnimationModeTalk); // Murray: Now, we aren't asking for any trouble, Ray.
		Actor_Says(kActorMcCoy,  1520,                 15); // McCoy: I'm not working Bunco. I just want some information.
		Actor_Says(kActorMurray,  210,                 13); // Murray: That nutty broad who runs Kingston Kitchen.
		Actor_Says(kActorMurray,  220,                 13); // Murray: She's always bragging about her special ingredient.
		Actor_Says(kActorMia,     140,                 12); // Mia: That stew she makes, it always did taste a little gamey.
		Actor_Says(kActorMurray,  230,                 13); // Murray: And she charges an arm and leg for it, too.
		Actor_Clue_Acquire(kActorMcCoy, kClueSpecialIngredient, false, kActorMurray);
		break;

	case 470: // LICHEN DOG WRAPPER
		Actor_Says(kActorMcCoy, 1495, 14); // McCoy: I found this wrapper.
		Actor_Face_Actor(kActorMurray, kActorMcCoy, true);
		Actor_Says(kActorMurray, 240, 13); // Murray: Yeah, that's from one of our 'dogs.
		Actor_Face_Actor(kActorMurray, kActorMia, true);
		break;

	case 480: // DONE
		Actor_Says(kActorMcCoy, 1500, 16); // McCoy: Be good you two.
		break;
	}
}

void SceneScriptHF01::talkWithDektora() {
	Actor_Says(kActorDektora,    0, kAnimationModeTalk); // Dektora: You made it.
	Actor_Says(kActorMcCoy,   1400, kAnimationModeTalk); // McCoy: I promised you, didn't I?
	Actor_Says(kActorDektora,   10, kAnimationModeTalk); // Dektora: You're one of the few people I've known who actually keep their promises.
	Actor_Says(kActorMcCoy,   1405, kAnimationModeTalk); // McCoy: Where's Clovis?
	Actor_Says(kActorDektora,   20, kAnimationModeTalk); // Dektora: He's not coming with us.
	Actor_Says(kActorMcCoy,   1410, kAnimationModeTalk); // McCoy: You're gonna miss him?
	Actor_Says(kActorDektora,   30, kAnimationModeTalk); // Dektora: I've practically forgotten him already.
	Actor_Says(kActorMcCoy,   1415, kAnimationModeTalk); // McCoy: Some ways I wish I'd gotten to know him better.
	Actor_Says(kActorDektora,   40, kAnimationModeTalk); // Dektora: Then you know him better than you think.
	Actor_Says(kActorDektora,   50, kAnimationModeTalk); // Dektora: There's a strange bond that exists between the two of you.
	Actor_Says(kActorDektora,   60, kAnimationModeTalk); // Dektora: I felt it from the start.
	Actor_Says(kActorDektora,   70, kAnimationModeTalk); // Dektora: That's one of the reasons I was so drawn to you.
	Actor_Says(kActorMcCoy,   1420, kAnimationModeTalk); // McCoy: Are you ready?
	Actor_Says(kActorDektora,   80, kAnimationModeTalk); // Dektora: Like I'm beginning a whole new life.
}

void SceneScriptHF01::talkWithLucy() {
	Actor_Says(kActorLucy,     0, kAnimationModeTalk); // Lucy: I knew you'd come.
	Actor_Says(kActorMcCoy, 1425, kAnimationModeTalk); // McCoy: I promised you, didn't I?
	Actor_Says(kActorLucy,    10, kAnimationModeTalk); // Lucy: Father wanted me to stay with him.
	Actor_Says(kActorLucy,    20, kAnimationModeTalk); // Lucy: But when I told him I was going with you, he gave us his blessing.
	Actor_Says(kActorMcCoy, 1430, kAnimationModeTalk); // McCoy: He is an extraordinary guy.
	Actor_Says(kActorLucy,    30, kAnimationModeTalk); // Lucy: I know.
	Actor_Says(kActorMcCoy, 1435, kAnimationModeTalk); // McCoy: You're gonna miss him?
	Actor_Says(kActorLucy,    40, kAnimationModeTalk); // Lucy: Of course, but now that I have you
	Actor_Says(kActorLucy,    50, kAnimationModeTalk); // Lucy: everything will be all right, won't it?
	Actor_Says(kActorMcCoy, 1440, kAnimationModeTalk); // McCoy: Sure it will.
	Actor_Says(kActorLucy,    60, kAnimationModeTalk); // Lucy: Promise me that we're going to find that special place!
	Actor_Says(kActorLucy,    70, kAnimationModeTalk); // Lucy: The one with the trees and the flowers and the beautiful clear lake.
	Actor_Says(kActorMcCoy, 1445, kAnimationModeTalk); // McCoy: It exists somewhere.
	Actor_Says(kActorLucy,    80, kAnimationModeTalk); // Lucy: It's so much more than a dream, isn't it?
	Actor_Says(kActorLucy,  3030, kAnimationModeTalk); // Lucy: It's where we were always meant to be. And we'll be there, together.
}

} // End of namespace BladeRunner
