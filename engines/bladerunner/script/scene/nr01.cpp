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

enum kNR01Loops {
	kNR01LoopInshot            = 0,
	kNR01LoopMainLoop          = 1,
	kNR01LoopOutshot           = 3,
	kNR01LoopMainLoopNoSpinner = 4
};

void SceneScriptNR01::InitializeScene() {
	if (Game_Flag_Query(kFlagSpinnerMissing)) {
		Setup_Scene_Information(-153.86f, 23.88f, -570.21f, 402);
	} else if (Game_Flag_Query(kFlagNR01DektoraFall)) {
		Setup_Scene_Information( -416.0f, 31.93f,  -841.0f, 200);
		Actor_Set_Invisible(kActorMcCoy, true);
		Preload(kModelAnimationDektoraInFlamesEndFalling);
	} else if (Game_Flag_Query(kFlagNR03toNR01)) {
		Setup_Scene_Information( -416.0f, 31.93f,  -841.0f, 200);
	} else if (Game_Flag_Query(kFlagUG06toNR01)) {
		Setup_Scene_Information( -270.0f,  4.93f, -1096.0f, 500);
	} else if (Game_Flag_Query(kFlagNR02toNR01)) {
		Setup_Scene_Information(  312.0f, 31.66f,  -901.0f, 700);
	} else if (Game_Flag_Query(kFlagNotUsed545)) {
		Setup_Scene_Information( -170.0f,  24.0f,  -574.0f, 768);
	} else {
		Setup_Scene_Information(   76.0f, 23.88f,  -109.0f, 966);
	}

	Scene_Exit_Add_2D_Exit(0, 31, 270, 97, 373, 3);
	if (Global_Variable_Query(kVariableChapter) > 3) {
		Scene_Exit_Add_2D_Exit(1, 201, 320, 276, 357, 2);
	}
	Scene_Exit_Add_2D_Exit(2, 583, 262, 639, 365, 1);
	if (Game_Flag_Query(kFlagSpinnerAtNR01)) {
		Scene_Exit_Add_2D_Exit(3, 320, 445, 639, 479, 2);
	}

	Ambient_Sounds_Add_Looping_Sound(kSfxCTRAIN1,  50,  0, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxMUSBLEED, 22, 55, 1);
	Ambient_Sounds_Add_Sound(kSfxSEXYAD2,  10, 10, 20, 20,  -70, -70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0480R,  5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0540R,  5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0560R,  5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0870R,  5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0900R,  5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0940R,  5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_1070R,  5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_1080R,  5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_1160R,  5, 70, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSPIN2B,   10, 80, 16, 25,    0,   0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSPIN3A,   10, 80, 16, 25,    0,   0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxTHNDER2,  10, 80, 33, 33,    0,   0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxTHNDER3,  10, 80, 33, 33,    0,   0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxTHNDER4,  10, 80, 33, 33,    0,   0, -101, -101, 0, 0);

	if (Game_Flag_Query(kFlagSteeleKnowsBulletBobIsDead)
	 && Actor_Query_Goal_Number(kActorSteele) == kGoalSteeleNR01WaitForMcCoy
	) {
		Game_Flag_Reset(kFlagSpinnerAtNR01);
		Game_Flag_Reset(kFlagSpinnerAtHF01);
	}

	if ( Game_Flag_Query(kFlagSpinnerAtNR01)
	 && !Game_Flag_Query(kFlagArrivedFromSpinner1)
	) {
		if ((!Game_Flag_Query(kFlagNR01VisitedFirstTimeWithSpinner) && Global_Variable_Query(kVariableChapter) == 3)
		     || Random_Query(1, 3) == 1)
		{
			// enhancement: don't always play after first visit
			Scene_Loop_Start_Special(kSceneLoopModeLoseControl, kNR01LoopInshot, false);
		}
		Scene_Loop_Set_Default(kNR01LoopMainLoop);
		Game_Flag_Set(kFlagArrivedFromSpinner1);
	} else if (Game_Flag_Query(kFlagSpinnerAtNR01)
	        && Game_Flag_Query(kFlagArrivedFromSpinner1)
	) {
		Scene_Loop_Set_Default(kNR01LoopMainLoop);
	} else {
		Scene_Loop_Set_Default(kNR01LoopMainLoopNoSpinner);
	}
}

void SceneScriptNR01::SceneLoaded() {
	Obstacle_Object("LAMPBASE01", true);
	Unclickable_Object("LAMPBASE01");
}

bool SceneScriptNR01::MouseClick(int x, int y) {
	if (Actor_Query_Goal_Number(kActorMcCoy) == kGoalMcCoyNR01LayDrugged) {
		Global_Variable_Increment(kVariableNR01GetUpCounter, 4);
		return true;
	}
	return false;
}

bool SceneScriptNR01::ClickedOn3DObject(const char *objectName, bool a2) {
	return false;
}

bool SceneScriptNR01::ClickedOnActor(int actorId) {
	return false;
}

bool SceneScriptNR01::ClickedOnItem(int itemId, bool a2) {
	return false;
}

bool SceneScriptNR01::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -380.0f, 31.93f, -841.0f, 0, true, false, false)) {
			if (Global_Variable_Query(kVariableChapter) > 3) {
				Actor_Says(kActorMcCoy, 8522, 12); // McCoy: Locked.
			} else {
				Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
				Ambient_Sounds_Remove_All_Looping_Sounds(1);
				Game_Flag_Set(kFlagNR01toNR03);
				Set_Enter(kSetNR03, kSceneNR03);
			}
		}
		return true;
	}

	if (exitId == 1) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -281.0f, 31.93f, -1061.0f, 0, true, false, false)) {
			Actor_Face_Heading(kActorMcCoy, 45, false);
			Loop_Actor_Travel_Stairs(kActorMcCoy, 3, false, kAnimationModeIdle);
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Game_Flag_Set(kFlagNR01toUG06);
			Set_Enter(kSetUG06, kSceneUG06);
		}
		return true;
	}

	if (exitId == 2) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 312.0f, 31.66f, -901.0f, 0, true, false, false)) {
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Game_Flag_Set(kFlagNR01toNR02);
			Set_Enter(kSetNR02, kSceneNR02);
		}
		return true;
	}

	if (exitId == 3) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 108.0f, 23.88f, -93.0f, 0, true, false, false)) {
			Game_Flag_Reset(kFlagMcCoyInChinaTown);
			Game_Flag_Reset(kFlagMcCoyInRunciters);
			Game_Flag_Reset(kFlagMcCoyInMcCoyApartment);
			Game_Flag_Reset(kFlagMcCoyInPoliceStation);
			Game_Flag_Reset(kFlagMcCoyInBradburyBuilding);
			Game_Flag_Reset(kFlagMcCoyInHysteriaHall);
			Game_Flag_Reset(kFlagMcCoyInTyrellBuilding);
			Game_Flag_Reset(kFlagMcCoyInNightclubRow);
			switch (Spinner_Interface_Choose_Dest(-1, true)) {
			case kSpinnerDestinationPoliceStation:
				Game_Flag_Set(kFlagMcCoyInPoliceStation);
				Game_Flag_Reset(kFlagSpinnerAtNR01);
				Game_Flag_Reset(kFlagArrivedFromSpinner1);
				Game_Flag_Set(kFlagSpinnerAtPS01);
				Set_Enter(kSetPS01, kScenePS01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kNR01LoopOutshot, true);
				break;
			case kSpinnerDestinationMcCoysApartment:
				Game_Flag_Set(kFlagMcCoyInMcCoyApartment);
				Game_Flag_Reset(kFlagSpinnerAtNR01);
				Game_Flag_Reset(kFlagArrivedFromSpinner1);
				Game_Flag_Set(kFlagSpinnerAtMA01);
				Set_Enter(kSetMA01, kSceneMA01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kNR01LoopOutshot, true);
				break;
			case kSpinnerDestinationRuncitersAnimals:
				Game_Flag_Set(kFlagMcCoyInRunciters);
				Game_Flag_Reset(kFlagSpinnerAtNR01);
				Game_Flag_Reset(kFlagArrivedFromSpinner1);
				Game_Flag_Set(kFlagSpinnerAtRC01);
				Set_Enter(kSetRC01, kSceneRC01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kNR01LoopOutshot, true);
				break;
			case kSpinnerDestinationChinatown:
				Game_Flag_Set(kFlagMcCoyInChinaTown);
				Game_Flag_Reset(kFlagSpinnerAtNR01);
				Game_Flag_Reset(kFlagArrivedFromSpinner1);
				Game_Flag_Set(kFlagSpinnerAtCT01);
				Set_Enter(kSetCT01_CT12, kSceneCT01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kNR01LoopOutshot, true);
				break;
			case kSpinnerDestinationAnimoidRow:
				Game_Flag_Set(kFlagMcCoyInAnimoidRow);
				Game_Flag_Reset(kFlagSpinnerAtNR01);
				Game_Flag_Reset(kFlagArrivedFromSpinner1);
				Game_Flag_Set(kFlagSpinnerAtAR01);
				Set_Enter(kSetAR01_AR02, kSceneAR01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kNR01LoopOutshot, true);
				break;
			case kSpinnerDestinationTyrellBuilding:
				Game_Flag_Set(kFlagMcCoyInTyrellBuilding);
				Game_Flag_Reset(kFlagSpinnerAtNR01);
				Game_Flag_Reset(kFlagArrivedFromSpinner1);
				Game_Flag_Set(kFlagSpinnerAtTB02);
				Set_Enter(kSetTB02_TB03, kSceneTB02);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kNR01LoopOutshot, true);
				break;
			case kSpinnerDestinationDNARow:
				Game_Flag_Set(kFlagMcCoyInDNARow);
				Game_Flag_Reset(kFlagSpinnerAtNR01);
				Game_Flag_Reset(kFlagArrivedFromSpinner1);
				Game_Flag_Set(kFlagSpinnerAtDR01);
				Set_Enter(kSetDR01_DR02_DR04, kSceneDR01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kNR01LoopOutshot, true);
				break;
			case kSpinnerDestinationBradburyBuilding:
				Game_Flag_Set(kFlagMcCoyInBradburyBuilding);
				Game_Flag_Reset(kFlagSpinnerAtNR01);
				Game_Flag_Reset(kFlagArrivedFromSpinner1);
				Game_Flag_Set(kFlagSpinnerAtBB01);
				Set_Enter(kSetBB01, kSceneBB01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kNR01LoopOutshot, true);
				break;
			case kSpinnerDestinationHysteriaHall:
				Game_Flag_Set(kFlagMcCoyInHysteriaHall);
				Game_Flag_Reset(kFlagSpinnerAtNR01);
				Game_Flag_Set(kFlagSpinnerAtHF01);
				Set_Enter(kSetHF01, kSceneHF01);
				Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kNR01LoopOutshot, true);
				break;
			default:
				Player_Loses_Control();
				Game_Flag_Set(kFlagMcCoyInNightclubRow);
				Game_Flag_Set(kFlagArrivedFromSpinner1);
				Player_Gains_Control();
				break;
			}
		}
		return true;
	}
	return false;
}

bool SceneScriptNR01::ClickedOn2DRegion(int region) {
	if (region == 0
	 && Player_Query_Combat_Mode()
	) {
		Sound_Play(kSfxGUNH1A, 100, 0, 0, 50);
		Actor_Set_Goal_Number(kActorSteele, kGoalSteeleNR01McCoyShotGun);
		Scene_2D_Region_Remove(0);
	}

	if (region == 1
	 && Player_Query_Combat_Mode()
	) {
		Sound_Play(kSfxGUNH1A, 100, 0, 0, 50);
		Actor_Set_Goal_Number(kActorGordo, kGoalGordoNR01Die);
		Actor_Set_Goal_Number(kActorSteele, kGoalSteeleNR01McCoyShotGordo);
		Scene_2D_Region_Remove(1);
		return true;
	}
	return false;

}

void SceneScriptNR01::SceneFrameAdvanced(int frame) {
	if (frame == 61) {
		Sound_Play(kSfxCARDOWN3, 40, 0, 0, 50);
	}

	if (frame == 184) {
		Sound_Play(kSfxCARUP3, 40, 80, 80, 50);
	}
	//return 0;
}

void SceneScriptNR01::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptNR01::PlayerWalkedIn() {
	if (Game_Flag_Query(kFlagSpinnerMissing)) {
		Actor_Set_Goal_Number(kActorSteele, kGoalSteeleTalkAboutMissingSpinner);
		Game_Flag_Reset(kFlagSpinnerMissing);
		//return true;
		return;
	}

	if (Actor_Query_Goal_Number(kActorSteele) == kGoalSteeleNR01ConfrontGordo) {
		Scene_Exits_Disable();
		ADQ_Flush();
		Actor_Set_Goal_Number(kActorSteele, kGoalSteeleNR01TalkToGordo);
		Scene_2D_Region_Add(0, 450, 316, 464, 333);
		Scene_2D_Region_Add(1, 233, 321, 240, 362);
		ADQ_Add(kActorGordo,    70,                 81); // Gordo: Now this is what I call a captive audience!
		ADQ_Add(kActorSteele,  990, kAnimationModeTalk); // Steele: You got nowhere to go, Gordo! You're a Replicant.
		ADQ_Add(kActorGordo,    80,                 82); // Gordo: Everyone's a critic.
		ADQ_Add(kActorGordo,    90,                 81); // Gordo: I've died on stage plenty of times. I'm used to it.
		ADQ_Add(kActorSteele, 1010, kAnimationModeTalk); // Steele: Why don't you let her go then?
		ADQ_Add(kActorGordo,   100,                 81); // Gordo: I didn't say I was ready.
		ADQ_Add(kActorSteele, 1020, kAnimationModeTalk); // Steele: This is only gonna end one way, Gordo. You really wanna take an innocent with you?
		ADQ_Add(kActorGordo,   110,                 82); // Gordo: Nobody's innocent down here, lady.
		ADQ_Add(kActorSteele, 1030, kAnimationModeTalk); // Steele: I'd rather lose her than the dozen I'd lose if I let you go.
		ADQ_Add(kActorSteele, 1040, kAnimationModeTalk); // Steele: You're a killer, pal. And you're not gonna change. That's how Tyrell build you.
		ADQ_Add(kActorGordo,   120,                 82); // Gordo: An entertainer. That's all I ever was. That's all I ever wanted to be.
	}

	if (Game_Flag_Query(kFlagNR03McCoyThrownOut)) {
		if (Game_Flag_Query(kFlagNR08TouchedDektora)) {
			ADQ_Add(kActorHanoi, 150, kAnimationModeTalk); // Hanoi: Look but don't touch!
			Game_Flag_Reset(kFlagNR08TouchedDektora);
		}
		Game_Flag_Reset(kFlagNR03McCoyThrownOut);
		Player_Gains_Control();
		//return true;
		return;
	}

	if (Game_Flag_Query(kFlagNR01DektoraFall)) {
		Delay(3500);
		Set_Enter(kSetNR11, kSceneNR11);
		//return true;
		return;
	}

	if (Game_Flag_Query(kFlagNR03toNR01)) {
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, -380.0f, 31.73f, -841.0f, 0, false, false, false);
		Game_Flag_Reset(kFlagNR03toNR01);
	} else if (Game_Flag_Query(kFlagUG06toNR01)) {
		Loop_Actor_Travel_Stairs(kActorMcCoy, 3, true, kAnimationModeIdle);
		Game_Flag_Reset(kFlagUG06toNR01);
		if (Actor_Query_Goal_Number(kActorSteele) == kGoalSteeleNR01WaitForMcCoy) {
			Actor_Face_Actor(kActorSteele, kActorMcCoy, true);
			Actor_Says(kActorSteele, 1440, 13); // Steele: What the hell took you so long?
			Loop_Actor_Walk_To_Actor(kActorMcCoy, kActorSteele, 48, false, true);
			Actor_Says(kActorMcCoy, 3145, 13); // McCoy: Good thing you made your entrance when you did.
			if (Global_Variable_Query(kVariableHollowayArrest) != 3) {
				Actor_Says(kActorSteele, 1450, 12); // Steele: Good thing one of my snitches saw them escort you out of the club.
				Actor_Says(kActorSteele, 1460, 13); // Steele: Otherwise you'd be three quarters in the bag right now.
			}
			Actor_Says(kActorMcCoy,  3150, 14); // McCoy: They must have been phony cops. Hired by the Reps for protection.
			Actor_Says(kActorSteele, 1470, 12); // Steele: Cops? Yes. Phony? No. I remembered Baker.
			Actor_Says(kActorSteele, 1480, 13); // Steele: He used to be an S.I.D. (Scientific Investigation Division) expert on Coercive Technique until he was fired from the force a couple of years ago.
			Actor_Says(kActorMcCoy,  3155, 15); // McCoy: A mercenary?
			Actor_Says(kActorSteele, 1500, 16); // Steele: Bingo.
			Actor_Says(kActorMcCoy,  3160, 12); // McCoy: He won't have to worry about getting paid no more.
			if (Game_Flag_Query(kFlagSteeleKnowsBulletBobIsDead)) {
				Actor_Says(kActorSteele, 1330, 12); // Steele: We got a problem, though. There's an APB out for you.
				Actor_Says(kActorSteele, 1340, 12); // Steele: You've been shooting civilians? Because that's what Guzza's saying.
				Actor_Says(kActorSteele, 1350, 12); // Steele: He wants to put you on the Machine.
				Actor_Says(kActorMcCoy,  3120, 15); // McCoy: You're gonna retire me, Steele?
				Actor_Says(kActorSteele, 1360, 12); // Steele: You're not on my list yet. That means no retirement swag.
				Actor_Says(kActorSteele, 1370, 12); // Steele: The uniforms know you're down here. They already towed your Spinner.
				Actor_Says(kActorMcCoy,  3125, 15); // McCoy: I was wondering where it went.
				Actor_Says(kActorSteele, 1380, 12); // Steele: Is it true, Slim? Did you kill somebody?
				Actor_Says(kActorMcCoy,  3130, 15); // McCoy: What do you think?
				Actor_Says(kActorSteele, 1390, 12); // Steele: I ain't sure yet. I like to be sure.
				Actor_Says(kActorSteele, 1400, 12); // Steele: Something ain't right. That setup underground? I didn't see a V-K Machine down there.
				Actor_Says(kActorSteele, 1410, 12); // Steele: Baker wasn't gonna take you downtown and he wasn't gonna test you.
				Actor_Says(kActorMcCoy,  3135, 15); // McCoy: No kidding.
				Actor_Says(kActorSteele, 1420, 12); // Steele: Go, if you're going. I'm gonna talk to Guzza before I do anything.
				Actor_Says(kActorMcCoy,  3140, 15); // McCoy: Thanks.
				Actor_Says(kActorSteele, 1430, 12); // Steele: Don't thank me yet. The next time you see me... (smacks lips twice)
				Actor_Set_Goal_Number(kActorSteele, kGoalSteeleImmediatelyStartChapter4);
			} else {
				int v0 = Global_Variable_Query(kVariableHollowayArrest);
				if (v0 == 1) { // Dektora called the fake cops
					Actor_Says(kActorSteele, 1510, 15); // Steele: That stripper you interviewed. She's one of them.
					Actor_Says(kActorSteele, 1520, 14); // Steele: Figure it out! She must have made the call to Baker and that other Bozo.
					Actor_Says(kActorSteele, 1530, 13); // Steele: Let's attend to some business.
					Actor_Says(kActorMcCoy,  3170, 13); // McCoy: I'm there.
					Actor_Set_Goal_Number(kActorSteele, kGoalSteeleNR01GoToNR08);
				} else if (v0 == 2) { // Gordo called the fake cops
					Actor_Says(kActorSteele, 1590, 15); // Steele: That little weasel who works for Taffy?
					Actor_Says(kActorMcCoy,  3195, 14); // McCoy: Gordo?
					Actor_Says(kActorSteele, 1600, 16); // Steele: He's one of them. You should have popped him right there on the stage.
					Actor_Says(kActorMcCoy,  3200, 13); // McCoy: Yeah. That would have been good publicity for the Department.
					Actor_Says(kActorSteele, 1610, 17); // Steele: That's your problem, McCoy. You worry too much about what people think.
					Actor_Says(kActorSteele, 1620, 15); // Steele: Now, excuse me while I go collect.
					Actor_Says(kActorSteele, 1630, 14); // Steele: You in or out?
					Actor_Says(kActorMcCoy,  3205, 12); // McCoy: In.
					Actor_Set_Goal_Number(kActorSteele, kGoalSteeleNR01GoToNR02);
				} else if (v0 == 3) { // Lucy called the fake cops
					Actor_Says(kActorSteele, 1540, 15); // Steele: That girl, the one from the arcade. She's one of them.
					Actor_Says(kActorMcCoy,  3175, 13); // McCoy: You sure? I didn't get a chance to put her on the Machine.
					Actor_Says(kActorSteele, 1550, 13); // Steele: I've been sure since I've heard about all those dead animals in Runciter's shop.
					Actor_Says(kActorSteele, 1560, 16); // Steele: You should have whacked her when you had the chance.
					Actor_Says(kActorMcCoy,  3180, 15); // McCoy: Yeah, right in front of all the other children in the arcade. That would have been great publicity.
					Actor_Says(kActorSteele, 1570, 12); // Steele: I bet she's still there, Slim.
					Actor_Says(kActorSteele, 1580, 14); // Steele: You coming or not?
					Actor_Says(kActorMcCoy,  3190, 12); // McCoy: Right behind you.
					Actor_Set_Goal_Number(kActorSteele, kGoalSteeleNR01GoToHF03);
				}
			}
		}
	} else if (Game_Flag_Query(kFlagNR02toNR01)) {
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, 239.0f, 31.66f, -901.0f, 0, false, false, false);
		Game_Flag_Reset(kFlagNR02toNR01);
		if (Actor_Query_Goal_Number(kActorGordo) == kGoalGordoNR01WaitAndAttack) {
			Scene_Exits_Disable();
			Actor_Set_Goal_Number(kActorGordo, kGoalGordoNR01Attack);
			Non_Player_Actor_Combat_Mode_On(kActorGordo, kActorCombatStateIdle, true, kActorMcCoy, 3, kAnimationModeCombatIdle, kAnimationModeCombatWalk, kAnimationModeCombatRun, -1, -1, -1, 20, 300, false);
		}
	} else if (Game_Flag_Query(kFlagNotUsed545)) {
		Game_Flag_Reset(kFlagNotUsed545);
		Actor_Put_In_Set(kActorHanoi, kSetNR01);
		Actor_Set_At_XYZ(kActorHanoi, -202.0f, 24.0f, -574.0f, 0);
		Actor_Face_Heading(kActorHanoi, 256, false);
		Actor_Set_Goal_Number(kActorHanoi, 204);
		Player_Gains_Control();
	} else {
		if (!Game_Flag_Query(kFlagNR01VisitedFirstTimeWithSpinner) && Game_Flag_Query(kFlagArrivedFromSpinner1)) {
			Game_Flag_Set(kFlagNR01VisitedFirstTimeWithSpinner);
		}
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, 48.0f, 23.88f, -189.0f, 0, false, false, false);
	}

	if (Game_Flag_Query(kFlagCrazylegsArrestedTalk)) {
		Game_Flag_Reset(kFlagCrazylegsArrestedTalk);
		Actor_Voice_Over(950, kActorVoiceOver); // Mainframe: Crazylegs was a small fish looking to make a few bucks in a big ocean.
		Actor_Voice_Over(960, kActorVoiceOver); // Mainframe: And he'd ended up swimming with the sharks.
		Actor_Voice_Over(970, kActorVoiceOver); // Mainframe: Still the last couple of hours hadn't been a total loss.
		Actor_Voice_Over(980, kActorVoiceOver); // Mainframe: I knew what the Reps were looking for. Some way to get out of the city.
	}

	if (Actor_Query_Goal_Number(kActorGordo) == kGoalGordoNR01WaitAndGiveUp) {
		Scene_Exits_Disable();
		Actor_Set_Goal_Number(kActorGordo, kGoalGordoNR01GiveUp);
		if (!Player_Query_Combat_Mode()) {
			Player_Set_Combat_Mode(true);
		}
	}
	//return false;
	return;
}

void SceneScriptNR01::PlayerWalkedOut() {
	Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
	Ambient_Sounds_Remove_All_Looping_Sounds(1);
	if (!Game_Flag_Query(kFlagNR01toUG06)
	 && !Game_Flag_Query(kFlagNR01toNR02)
	 && !Game_Flag_Query(kFlagNR01toNR03)
	 && !Game_Flag_Query(kFlagNR01DektoraFall)
	 && !Game_Flag_Query(kFlagChapter4Starting)
	) {
#if BLADERUNNER_ORIGINAL_BUGS
		Outtake_Play(kOuttakeAway1,   true, -1);
		Outtake_Play(kOuttakeInside2, true, -1);
#else
		if (!Game_Flag_Query(kFlagMcCoyInTyrellBuilding)) {
			// don't play an extra outtake when going to Tyrell Building
			// also switch order so inside2 outtake is not last
			Outtake_Play(kOuttakeInside2, true, -1);
			Outtake_Play(kOuttakeAway1,   true, -1);
		}
#endif // BLADERUNNER_ORIGINAL_BUGS
	}
	Game_Flag_Reset(kFlagChapter4Starting);
}

void SceneScriptNR01::DialogueQueueFlushed(int a1) {
	if (Actor_Query_Goal_Number(kActorSteele) == kGoalSteeleNR01TalkToGordo
	 && Actor_Query_Goal_Number(kActorGordo) != kGoalGordoNR01Die
	 && Actor_Query_Goal_Number(kActorGordo) != kGoalGordoNR01HostageShot
	 && Actor_Query_Goal_Number(kActorGordo) != kGoalGordoNR01HostageDie
	) {
		Actor_Set_Goal_Number(kActorSteele, kGoalSteeleNR01ShootGordo);
	}
}

} // End of namespace BladeRunner
