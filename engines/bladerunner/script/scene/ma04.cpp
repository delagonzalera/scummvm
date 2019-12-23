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

enum kMA04Loops {
	kMA04LoopInshot   = 0,
	kMA04LoopMainLoop = 1,
	kMA04LoopSleep    = 3,
	kMA04LoopWakeup   = 4
};

enum kMA04Exits {
	kMA04ExitMA02     = 0,
	kMA04ExitMA05     = 1,
	kMA04ExitKitchen  = 2
};

enum kMA04Regions {
	kMA04RegionPhone = 0,
	kMA04RegionBed   = 1
};

void SceneScriptMA04::InitializeScene() {
	if (Game_Flag_Query(kFlagMA05toMA04)) {
		Setup_Scene_Information(-7199.0f, 953.97f, 1579.0f, 502);
		if (Global_Variable_Query(kVariableChapter) != 2 && Global_Variable_Query(kVariableChapter) != 3) {
			Scene_Loop_Start_Special(kSceneLoopModeLoseControl, kMA04LoopInshot, false);
		}
	} else if (Game_Flag_Query(kFlagMA02ToMA04)) {
		Setup_Scene_Information(-7099.0f, 954.0f, 1866.0f, 502);
	} else if (Game_Flag_Query(kFlagMA04McCoySleeping)) {
		Setup_Scene_Information(-7107.0f, 954.0f, 1742.0f, 502);
		Scene_Loop_Start_Special(kSceneLoopModeLoseControl, kMA04LoopWakeup, false);
	} else {
		Setup_Scene_Information(-7143.0f, 954.0f, 1868.0f, 733);
	}
	Scene_Exit_Add_2D_Exit(kMA04ExitMA02,    496,  0, 639, 354, 1);
	Scene_Exit_Add_2D_Exit(kMA04ExitMA05,     33, 63, 113, 258, 0);
	Scene_Exit_Add_2D_Exit(kMA04ExitKitchen, 248, 98, 314, 284, 1);
	Scene_2D_Region_Add(kMA04RegionPhone, 343,  97, 353, 190);
	Scene_2D_Region_Add(kMA04RegionBed,     0, 340, 116, 479);
	Ambient_Sounds_Add_Looping_Sound(kSfxAPRTFAN1, 30,   0, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxRAINAWN1, 30, -80, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxAPRTAMB5, 12,   0, 1);
	Ambient_Sounds_Add_Sound(kSfxSWEEP2,   5,  30, 11, 11, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSWEEP3,   5,  30, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSWEEP4,   5,  30, 12, 12, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxTHNDER2, 10,  60, 20, 20,    0,   0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxTHNDER3, 10,  60, 20, 20,    0,   0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSIREN2,  10,  60, 16, 16, -100,   0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSPIN2B,  60, 180, 16, 25,    0,   0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSPIN3A,  60, 180, 16, 25,    0,   0, -101, -101, 0, 0);
	if (isPhoneRinging()) {
		Ambient_Sounds_Add_Sound(kSfxVIDFONE1, 3, 3, 100, 100, 0, 0, 0, 0, 99, 0);
	}
	Scene_Loop_Set_Default(kMA04LoopMainLoop);
}

void SceneScriptMA04::SceneLoaded() {
	Obstacle_Object("BED-DOG DISH", true);
	Unobstacle_Object("BEDDog BONE", true);
	Unobstacle_Object("BED-BOOK1", true);
	Clickable_Object("BED-SHEETS");
	if (Game_Flag_Query(kFlagMA04WatchedTV)) {
		Unclickable_Object("BED-TV-1");
		Unclickable_Object("BED-TV-2");
	} else {
		Clickable_Object("BED-TV-1");
		Clickable_Object("BED-TV-2");
	}
}

bool SceneScriptMA04::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptMA04::ClickedOn3DObject(const char *objectName, bool a2) {
	if (Object_Query_Click("BED-SHEETS", objectName)) {
		sleep();
		return false;
	}
	if (Object_Query_Click("BED-TV-1", objectName) || Object_Query_Click("BED-TV-2", objectName)) {
		if (!Loop_Actor_Walk_To_Scene_Object(kActorMcCoy, "BED-TV-2", 24, true, false)) {
			Game_Flag_Set(kFlagMA04WatchedTV);
			Unclickable_Object("BED-TV-1");
			Unclickable_Object("BED-TV-2");
			Sound_Play(kSfxSPNBEEP4, 100, 0, 0, 50);
			turnOnTV();
			return false;
		}
		return true;
	}
	return false;
}

bool SceneScriptMA04::ClickedOnActor(int actorId) {
	return false;
}

bool SceneScriptMA04::ClickedOnItem(int itemId, bool a2) {
	return false;
}

bool SceneScriptMA04::ClickedOnExit(int exitId) {
	if (exitId == kMA04ExitMA02) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -7099.0f, 954.0f, 1866.0f, 0, true, false, false)) {
			Game_Flag_Set(kFlagMA04ToMA02);
			Set_Enter(kSetMA02_MA04, kSceneMA02);
		}
		return true;
	}
	if (exitId == kMA04ExitMA05) {
		float x, y, z;
		Actor_Query_XYZ(kActorMcCoy, &x, &y, &z);
		if (z <= 1677.0f || !Loop_Actor_Walk_To_XYZ(kActorMcCoy, -7199.0f, 955.0f, 1675.0f, 0, true, false, false)) {
			if (isPhoneMessageWaiting() || isPhoneRinging()) {
				Overlay_Remove("MA04OVER");
			}
#if BLADERUNNER_ORIGINAL_BUGS
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, -7199.0f, 955.0f, 1675.0f, 0, false, false, true);
#else
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, -7199.0f, 955.0f, 1627.0f, 0, false, false, true);
#endif // BLADERUNNER_ORIGINAL_BUGS
			Game_Flag_Set(kFlagMA04toMA05);
			if (Global_Variable_Query(kVariableChapter) != 2 && Global_Variable_Query(kVariableChapter) != 3) {
				Async_Actor_Walk_To_XYZ(kActorMcCoy, -7199.0f, 956.17f, 1568.0f, 0, false);
			}
			Set_Enter(kSetMA05, kSceneMA05);
		}
		return true;
	}
	if (exitId == kMA04ExitKitchen) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -7115.0f, 954.0f, 1742.0f, 0, true, false, false)) {
			int sounds[] = {kSfxBARSFX1, kSfxTOILET1, kSfxDRAIN1X, kSfxWASH1, kSfxGARGLE1};
			Ambient_Sounds_Play_Sound(sounds[Random_Query(0, 4)], 50, 0, 0, 0);
			Delay(3000);
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, -7139.0f, 954.0f, 1746.0f, 0, true, false, true);
		}
	}
	return false;
}

bool SceneScriptMA04::ClickedOn2DRegion(int region) {
	if (Player_Query_Combat_Mode()) {
		return false;
	}
	if (region == kMA04RegionBed) {
		sleep();
		return true;
	}
	if (region == kMA04RegionPhone) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -7176.0f, 954.0f, 1806.0f, 0, true, false, false)) {
			Actor_Face_Heading(kActorMcCoy, 256, false);
			if (isPhoneRinging()) {
				Actor_Says(kActorMcCoy, 2680, 0); // McCoy: Pick up.
				Ambient_Sounds_Remove_Sound(kSfxVIDFONE1, true);
				Sound_Play(kSfxSPNBEEP9, 100, 0, 0, 50);
				Overlay_Remove("MA04OVER");
				Delay(500);
				if (Game_Flag_Query(kFlagMcCoyIsHelpingReplicants)) {
					if (Global_Variable_Query(kVariableAffectionTowards) == kAffectionTowardsDektora) {
						phoneCallWithDektora();
					} else if (Global_Variable_Query(kVariableAffectionTowards) == kAffectionTowardsLucy) {
						phoneCallWithLucy();
					} else {
						phoneCallWithClovis();
					}
				} else {
					phoneCallWithSteele();
				}
				Music_Play(kMusicBRBlues, 52, 0, 3, -1, 0, 0);
				return false;
			}
			if (Actor_Clue_Query(kActorClovis, kClueMcCoyRetiredZuben) && !Game_Flag_Query(kFlagMA04PhoneMessageFromClovis)) {
				Sound_Play(kSfxSPNBEEP9, 100, 0, 0, 50);
				Overlay_Remove("MA04OVER");
				Delay(500);
				Actor_Says(kActorClovis, 310, 3); // Clovis: I was angry with my friend. I told my wrath, my wrath did end...
				Actor_Says(kActorClovis, 320, 3); // Clovis: I was angry with my foe. I told my wrath, my wrath did grow!
				if (!Game_Flag_Query(kFlagDirectorsCut) && Global_Variable_Query(kVariableChapter) < 3) {
					Actor_Voice_Over(1300, kActorVoiceOver); // Mainframe: Crank calls were a dime a dozen in this city and I'd had my share.
					Actor_Voice_Over(1310, kActorVoiceOver); // Mainframe: Still, this guy sounded more like an English teacher than the average scumbag.
					Actor_Voice_Over(1320, kActorVoiceOver); // Mainframe: If I'd been more awake, the message might have spooked me.
				}
				Actor_Says(kActorMcCoy, 2445, 13); // McCoy: Erase and reset.
				Sound_Play(kSfxSPNBEEP9, 100, 0, 0, 50);
				Game_Flag_Set(kFlagMA04PhoneMessageFromClovis);
				return true;
			}
			if (Actor_Clue_Query(kActorLucy, kClueMcCoyLetZubenEscape) && !Game_Flag_Query(kFlagMA04PhoneMessageFromLucy)) {
				Sound_Play(kSfxSPNBEEP9, 100, 0, 0, 50);
				Overlay_Remove("MA04OVER");
				Delay(500);
				Actor_Says(kActorLucy, 500, 3); // Lucy: Hi, Mr. McCoy, I'm sure you remember me.
				Actor_Says(kActorLucy, 510, 3); // Lucy: I heard about what you did for my friend and I just wanted to say thanks.
				if (!Game_Flag_Query(kFlagDirectorsCut) && Global_Variable_Query(kVariableChapter) < 3) {
					Actor_Voice_Over(1330, kActorVoiceOver); // Mainframe: The message could have come from any of a dozen people.
					Actor_Voice_Over(1340, kActorVoiceOver); // Mainframe: I'd done my share of favors when I was in uniform.
					Actor_Voice_Over(1350, kActorVoiceOver); // Mainframe: Something about the voice was familiar but I was too tired to place it for sure.
				}
				Actor_Says(kActorMcCoy, 2445, 13); // McCoy: Erase and reset.
				Sound_Play(kSfxSPNBEEP9, 100, 0, 0, 50);
				Game_Flag_Set(kFlagMA04PhoneMessageFromLucy);
				return true;
			}
			Actor_Says(kActorMcCoy, 2670, 13); // McCoy: No messages.
			if (!Game_Flag_Query(kFlagDirectorsCut)) {
				Actor_Says(kActorMcCoy, 2675, 17); // McCoy: I'm in demand.
			}
		}
		return true;
	}
	return false;
}

void SceneScriptMA04::SceneFrameAdvanced(int frame) {
	Set_Fade_Color(0, 0, 0);
	if (frame >= 91 && frame < 121) {
		Set_Fade_Density((frame - 91) / 30.0f);
	} else if (frame >= 121 && frame < 151) {
		Set_Fade_Density((151 - frame) / 30.0f);
	} else {
		Set_Fade_Density(0.0f);
	}
	if (frame == 121 && (Game_Flag_Query(kFlagZubenRetired) || Game_Flag_Query(kFlagZubenSpared)) && !Game_Flag_Query(kFlagPS04GuzzaTalkZubenRetired)) {
		Sound_Play(kSfxVIDFONE1, 50, 0, 0, 50);
	}
}

void SceneScriptMA04::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptMA04::PlayerWalkedIn() {
	if (Game_Flag_Query(kFlagMA04McCoySleeping)) {
		Player_Gains_Control();
	}
	if (isPhoneMessageWaiting() || isPhoneRinging()) {
		Overlay_Play("MA04OVER", 0, true, false, 0);
	}
	if (Game_Flag_Query(kFlagMA04McCoySleeping)) {
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, -7139.0f, 954.0f, 1746.0f, 0, true, false, false);
	} else if (Game_Flag_Query(kFlagMA02ToMA04)) {
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, -7143.0f, 954.0f, 1868.0f, 0, true, false, false);
	}
	Game_Flag_Reset(kFlagMA02ToMA04);
	Game_Flag_Reset(kFlagMA05toMA04);
	Game_Flag_Reset(kFlagMA04McCoySleeping);
	if (Game_Flag_Query(kFlagChapter1Ended)) {
		if (Global_Variable_Query(kVariableChapter) == 2 && !Actor_Clue_Query(kActorMcCoy, kCluePhoneCallGuzza)) {
			Sound_Play(kSfxVIDFONE1, 100, 0, 0, 50);
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, -7176.0f, 954.0f, 1806.0f, 0, false, false, false);
			Actor_Face_Heading(kActorMcCoy, 256, true);
			Actor_Says(kActorMcCoy, 2680, kAnimationModeIdle); // McCoy: Pick up.
			Sound_Play(kSfxSPNBEEP9, 100, 0, 0, 50);
			Delay(500);
			Actor_Says(kActorGuzza,    0,  3); // Guzza: Hope you aren't camped out there with a bottle of tequila.
			Actor_Says(kActorMcCoy, 2685, 13); // McCoy: Hmph. Very funny.
			Actor_Says(kActorGuzza,   10,  3); // Guzza: Early-bird slays the worm McCoy.
			Actor_Says(kActorMcCoy, 2690, 17); // McCoy: Don't worry. I'm up, Lieutenant. What's the buzz?
			Actor_Says(kActorGuzza,   30,  3); // Guzza: One of Tyrell's employees got his fat face splattered all over the marble interior of the Tyrell Building.
			Actor_Says(kActorMcCoy, 2695, 12); // McCoy: Inside?
			Actor_Says(kActorGuzza,   40,  3); // Guzza: That's right. But we also got a sweet little break at it.
			Actor_Says(kActorGuzza,   50,  3); // Guzza: Tyrell's surveillance system recorded the whole shebang.
			Actor_Says(kActorMcCoy, 2700,  3); // McCoy: Beautiful! How many perps?
			Actor_Says(kActorGuzza,   60,  3); // Guzza: Just one. I don't know if it's related to the Runciter deal or not.
			Actor_Says(kActorGuzza,   70,  3); // Guzza: Tyrell security will have the disk for you when you get there. You be good, killer.
			Sound_Play(kSfxSPNBEEP9, 100, 0, 0, 50);
			Actor_Clue_Acquire(kActorMcCoy, kCluePhoneCallGuzza, false, kActorGuzza);
			Spinner_Set_Selectable_Destination_Flag(kSpinnerDestinationTyrellBuilding, true);
			Game_Flag_Set(kFlagRC01PoliceDone);
			if (!Game_Flag_Query(kFlagRC01ChromeDebrisTaken)) {
				Game_Flag_Set(kFlagRC01ChromeDebrisTaken);
				Item_Remove_From_World(kItemChromeDebris);
			}
			Actor_Set_Goal_Number(kActorOfficerLeary, kGoalOfficerLearyEndOfAct1);
			Actor_Put_In_Set(kActorOfficerLeary, kSetFreeSlotC);
			Actor_Set_At_Waypoint(kActorOfficerLeary, 35, 0); // kSetFreeSlotC
			Autosave_Game(0);
		}
		//return false;
		return;
	}
	if ((Game_Flag_Query(kFlagZubenRetired) || Game_Flag_Query(kFlagZubenSpared)) && !Game_Flag_Query(kFlagChapter1Ending)) {
		Music_Play(kMusicBRBlues, 52, 0, 2, -1, 0, 0);
		Player_Loses_Control();
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, -7199.0f, 955.0f, 1677.0f, 0, true, false, false);
		if (isPhoneMessageWaiting() || isPhoneRinging()) {
			Overlay_Remove("MA04OVER");
		}
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, -7199.0f, 955.0f, 1675.0f, 0, true, false, false);
		Game_Flag_Set(kFlagChapter1Ending);

		Async_Actor_Walk_To_XYZ(kActorMcCoy, -7204.0f, 956.17f, 1568.0f, 0, false);
		Set_Enter(kSetMA05, kSceneMA05);
	}
}

void SceneScriptMA04::PlayerWalkedOut() {
	Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
	Ambient_Sounds_Remove_All_Looping_Sounds(1);
	if (Game_Flag_Query(kFlagChapter2Intro)) {
		Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
		Ambient_Sounds_Remove_All_Looping_Sounds(1);
		Outtake_Play(kOuttakeMovieA, false, -1);
		Game_Flag_Reset(kFlagChapter2Intro);
	}
}

void SceneScriptMA04::DialogueQueueFlushed(int a1) {
	Overlay_Remove("MA04OVR2");
}

bool SceneScriptMA04::isPhoneRinging() {
	return Global_Variable_Query(kVariableChapter) == 5
	   && !Actor_Clue_Query(kActorMcCoy, kCluePhoneCallClovis)
	   && !Actor_Clue_Query(kActorMcCoy, kCluePhoneCallCrystal)
	   && !Actor_Clue_Query(kActorMcCoy, kCluePhoneCallDektora1)
	   && !Actor_Clue_Query(kActorMcCoy, kCluePhoneCallDektora2)
	   && !Actor_Clue_Query(kActorMcCoy, kCluePhoneCallLucy1)
	   && !Actor_Clue_Query(kActorMcCoy, kCluePhoneCallLucy2);
}

bool SceneScriptMA04::isPhoneMessageWaiting() {
	return (Actor_Clue_Query(kActorClovis, kClueMcCoyRetiredZuben) && !Game_Flag_Query(kFlagMA04PhoneMessageFromClovis))
	    || (Actor_Clue_Query(kActorLucy, kClueMcCoyLetZubenEscape) && !Game_Flag_Query(kFlagMA04PhoneMessageFromLucy));
}

void SceneScriptMA04::phoneCallWithDektora() {
	int answer;
	Actor_Says(kActorDektora,  220, 3); // Dektora: I've been trying to reach you for an hour. Are you all right?
	Actor_Says(kActorMcCoy,   2460, 0); // McCoy: Yeah.
	Actor_Says(kActorDektora,  230, 3); // Dektora: Look, I think I got Clovis and Sadik to let you alone.
	Actor_Says(kActorDektora,  240, 3); // Dektora: I told them you weren't hunting them anymore.
	Actor_Says(kActorMcCoy,   2465, 0); // McCoy: Bravo.
	Actor_Says(kActorDektora,  250, 3); // Dektora: What's the matter?
	Actor_Says_With_Pause(kActorMcCoy, 2470, 1.5f, 17);
	Actor_Says(kActorDektora,  260,  3); // Dektora: I'm so sorry. You were close to it, weren't you?
	Actor_Says(kActorMcCoy,   2475, 15); // McCoy: Not it. Her! Maggie.
	Actor_Says(kActorDektora,  270,  3); // Dektora: Clovis didn't do it. I'm sure of that.
	Actor_Says(kActorMcCoy,   2480,  0); // McCoy: No kidding.
	Actor_Says(kActorDektora,  280,  3); // Dektora: I also told him about us, Ray. He took it very well.
	Actor_Says(kActorDektora,  290,  3); // Dektora: He and I are finished for good this time.
	Actor_Says(kActorMcCoy,   2485, 19); // McCoy: I've a hard time believing that.
	Actor_Says(kActorDektora,  300,  3); // Dektora: He's been very philosophical. He's aware that his time is running out.
	Actor_Says(kActorDektora,  310,  3); // Dektora: That's why he wants to help us.
	Actor_Says(kActorMcCoy,   2490,  0); // McCoy: No hard feelings, huh?
	Actor_Says(kActorDektora,  330,  3); // Dektora: None at all.
	Actor_Says(kActorMcCoy,   2495,  0); // McCoy: Okay.
	Actor_Says(kActorDektora,  340,  3); // Dektora: But there's another way. Why don't you and I just go away from here?
	Actor_Says(kActorDektora,  350,  3); // Dektora: Leave this place and everybody else behind.
	if (Game_Flag_Query(kFlagCrazylegsArrested)
	 || Actor_Query_Goal_Number(kActorCrazylegs) == kGoalCrazyLegsLeavesShowroom
	) {
		answer = 1170; // CLOVIS
	} else {
		Dialogue_Menu_Clear_List();
		DM_Add_To_List_Never_Repeat_Once_Selected(1160, 1, 1, 2); // OK
		DM_Add_To_List_Never_Repeat_Once_Selected(1170, 2, 1, 1); // CLOVIS
		Dialogue_Menu_Appear(320, 240);
		answer = Dialogue_Menu_Query_Input();
		Dialogue_Menu_Disappear();
	}
	if (answer == 1160) { // OK
		Actor_Says(kActorMcCoy,   2500, 19); // McCoy: I got nothing to keep me here anymore. That's for sure. You got a plan?
		Actor_Says(kActorDektora,  360,  3); // Dektora: There's a used car place in the Fourth Sector near the arcade.
		Actor_Says(kActorMcCoy,   2510,  0); // McCoy: I know it.
		Actor_Says(kActorDektora,  370,  3); // Dektora: Gordo and I went there before looking for a vehicle.
		Actor_Says(kActorDektora,  380,  3); // Dektora: The owner was nice to us. He'll probably help us, if we pay him.
		Actor_Says(kActorMcCoy,   2515, 12); // McCoy: We'd need a Spinner to be able to get anywhere.
		Actor_Says(kActorDektora,  390,  3); // Dektora: He's got a couple for sale.
		Actor_Says(kActorMcCoy,   2520, 13); // McCoy: It's real risky. And illegal.
		Actor_Says(kActorDektora,  400,  3); // Dektora: Well, it's better than sitting out there in the Kipple waiting to die.
		Actor_Says(kActorDektora,  410,  3); // Dektora: Watching everybody else die. If you won't do it, I'll go get the car myself.
		Actor_Says(kActorMcCoy,   2525, 15); // McCoy: I didn't say I wasn't gonna do it.
		Actor_Says(kActorDektora,  420,  3); // Dektora: Then meet me there. I'll be there within the hour.
		Sound_Play(kSfxSPNBEEP9, 100, 0, 0, 50);
		Actor_Clue_Acquire(kActorMcCoy, kCluePhoneCallDektora1, true, -1);
	} else {
		Actor_Says_With_Pause(kActorMcCoy, 2505, 0.5f, 19);
		Actor_Says(kActorDektora,  430,  3); // Dektora: He's been struggling out in the Kipple. Trying to get the moonbus fixed.
		Actor_Says(kActorDektora,  440,  3); // Dektora: If you could help repair it, maybe we can all leave together.
		Actor_Says(kActorMcCoy,   2530,  0); // McCoy: How far out are they?
		Actor_Says(kActorDektora,  450,  3); // Dektora: A few miles North of the city.
		Actor_Says(kActorMcCoy,   2535, 12); // McCoy: There's bound to be tons of radioactive waste between here and there.
		Actor_Says(kActorDektora,  460,  3); // Dektora: There's a tunnel in the sewers. That's how Clovis and Sadik go back and forth.
		Actor_Says_With_Pause(kActorDektora, 470, 1.0f, 3);
		Actor_Says(kActorDektora, 480, 3); // Dektora: I need you, Ray. I can't watch my friends die. Not alone.
		Actor_Says(kActorDektora, 490, 3); // Dektora: And if you can't come, adieu my love.
		Sound_Play(kSfxSPNBEEP9, 100, 0, 0, 50);
		Actor_Says(kActorMcCoy, 2540, 15); // McCoy: Dektora, wait! Damn it.
		Actor_Clue_Acquire(kActorMcCoy, kCluePhoneCallDektora2, true, -1);
	}
}

void SceneScriptMA04::phoneCallWithLucy() {
	Actor_Says(kActorLucy,   530,  3); // Lucy: I-- I didn't think you're going to answer
	Actor_Says(kActorMcCoy, 2545, 19); // McCoy: Sorry, but I-- I was just... sleeping. What's going on?
	Actor_Says(kActorLucy,   540,  3); // Lucy: Father wants to meet you. He said he's sorry about everything he did.
	Actor_Says(kActorLucy,   550,  3); // Lucy: But he was afraid you'd lead the other hunter to them
	Actor_Says(kActorMcCoy, 2550, 13); // McCoy: Where is he now?
	Actor_Says(kActorLucy,   560,  3); // Lucy: Out in the Kipple, by the moonbus. Way out where the tunnel ends.
	Actor_Says(kActorMcCoy, 2555, 19); // McCoy: Tell him to stay where he is, until I can find a way for all of us to escape.
	Actor_Says(kActorLucy,   570,  3); // Lucy: But-- I thought we were going away together. Just you and me.
	Actor_Says(kActorMcCoy, 2560, 17); // McCoy: Maybe we'll do it after I talk to him.
	Actor_Says(kActorLucy,   580,  3); // Lucy: You promise?
	if (Game_Flag_Query(kFlagCrazylegsArrested)
	 || Actor_Query_Goal_Number(kActorCrazylegs) == kGoalCrazyLegsLeavesShowroom
	) {
		Actor_Says(kActorLucy, 630, 3); // Lucy: I'll meet you there, okay? At the place where he sells the cars.
		Actor_Says_With_Pause(kActorMcCoy, 2575, 0.0f, 15);
		if (!Game_Flag_Query(kFlagDirectorsCut)) {
			Actor_Says(kActorLucy, 640, 3); // Lucy: Oh, I love you so much Ray.
		}
		Actor_Clue_Acquire(kActorMcCoy, kCluePhoneCallLucy2, true, -1);
	} else {
		Actor_Says(kActorLucy,   590,  3); // Lucy: We could buy a car. From that place next to the arcade.
		Actor_Says(kActorMcCoy, 2565, 12); // McCoy: A ground car wouldn't get us too far.
		Actor_Says(kActorLucy,   600,  3); // Lucy: One of those flying cars would though.
		Actor_Says(kActorLucy,   610,  3); // Lucy: It could take us all the way out to that beautiful lake.
		Actor_Says(kActorLucy,   620,  3); // Lucy: You know, the one with the trees and the flowers.
		Actor_Says(kActorMcCoy, 2570, 13); // McCoy: Lucy, there's a good chance--
		Actor_Says_With_Pause(kActorLucy, 630, 0.0f, 3);
		Actor_Says_With_Pause(kActorMcCoy, 2575, 0.0f, 15);
		if (!Game_Flag_Query(kFlagDirectorsCut)) {
			Actor_Says(kActorLucy, 640, 3); // Lucy: Oh, I love you so much Ray.
		}
		Actor_Clue_Acquire(kActorMcCoy, kCluePhoneCallLucy1, true, -1);
	}
	Sound_Play(kSfxSPNBEEP9, 100, 0, 0, 50);
}

void SceneScriptMA04::phoneCallWithSteele() {
	Actor_Says(kActorSteele,  680,  3); // Steele: Where you've been, Slim? The boys told me you were kicked hours ago.
	Actor_Says(kActorMcCoy,  2630, 17); // McCoy: My dog is still missing.
	Actor_Says(kActorSteele,  690,  3); // Steele: Cut the crap about the dog, okay? I've got something that will really rev your engine.
	Actor_Says(kActorMcCoy,  2635, 18); // McCoy: I'm all ears.
	Actor_Says(kActorSteele,  700,  3); // Steele: I got a line on where the skin-jobs are holing up.
	Actor_Says(kActorMcCoy,  2640, 14); // McCoy: Not the Yukon?
	Actor_Says(kActorSteele,  710,  3); // Steele: Yesterdays news. Remember Gaff was talking about that moonbus massacre?
	Actor_Says(kActorSteele,  720,  3); // Steele: I found out where they crash-landed it. And five will get you ten they'll be licking their wounds there.
	Actor_Says(kActorMcCoy,  2645, 13); // McCoy: Excellent. Where do we go?
	Actor_Says(kActorSteele,  740,  3); // Steele: We? How about moi? Here's a hint, Slim.
	Actor_Says(kActorSteele,  750,  3); // Steele: It's way the shit out there in the Kipple. Way, way out.
	Actor_Says(kActorMcCoy,  2650, 12); // McCoy: How are they getting into the city?
	Actor_Says(kActorSteele,  760,  3); // Steele: They've been accessing through an old sewer tunnel.
	Actor_Says(kActorMcCoy,  2665, 13); // McCoy: We're gonna air out the Reps together or what?
	Actor_Says(kActorSteele,  810,  3); // Steele: Mm. I'm liking your style more and more.
	Actor_Says(kActorSteele,  820,  3); // Steele: But first come, first serve. I share my bonuses with nobody. See you on the other side, Slim.
	Sound_Play(kSfxSPNBEEP9, 100, 0, 0, 50);
	Actor_Clue_Acquire(kActorMcCoy, kCluePhoneCallCrystal, true, -1);
}

void SceneScriptMA04::phoneCallWithClovis() {
	Actor_Says(kActorClovis,  330,  3); // Clovis: I see you survived.
	Actor_Says(kActorMcCoy,  2580, 14); // McCoy: No thanks to you.
	Actor_Says(kActorClovis,  340,  3); // Clovis: Don't you think you'd already be dead, if that's what I so desired?
	Actor_Says(kActorMcCoy,  2585, 19); // McCoy: Talking like a god, Clovis. Isn't that a little over the top? Even for you?
	Actor_Says(kActorClovis,  350,  3); // Clovis: Prometheus was chained to a rock and vultures pecked at his liver.
	Actor_Says(kActorClovis,  360,  3); // Clovis: I haven't quite had to suffer such ordeals literally speaking of course.
	Actor_Says(kActorMcCoy,  2590, 18); // McCoy: Well, there's still time.
	Actor_Says(kActorClovis,  370,  3); // Clovis: (laughs) I'm calling to apologize, brother. I should have never recruited Guzza against you. I just had to make sure that you weren't going to betray us.
	Actor_Says(kActorMcCoy,  2595, 15); // McCoy: Guzza probably thought that he recruited you.
	Actor_Says(kActorClovis,  390,  3); // Clovis: To be honest we had a sort of eh... business relationship
	Actor_Says(kActorClovis,  400,  3); // Clovis: where our different interests converge.
	Actor_Says(kActorClovis,  410,  3); // Clovis: But he was never to be trusted. Unlike you.
	Actor_Says(kActorMcCoy,  2600, 15); // McCoy: I appreciate that.
	Actor_Says_With_Pause(kActorClovis, 420, 1.5f, 3);
	Actor_Says(kActorMcCoy,  2605, 17); // McCoy: My dog's been murdered.
	Actor_Says(kActorClovis,  430,  3); // Clovis: And does that not wet your thirst for vengeance?
	Actor_Says(kActorClovis,  440,  3); // Clovis: Come back home to us. We'll demand justice and receive it!
	Actor_Says(kActorMcCoy,  2610,  3); // McCoy: How do I know who I really am?
	Actor_Says(kActorClovis,  450,  3); // Clovis: Guzza must have programmed you as an experiment.
	Actor_Says(kActorClovis,  460,  3); // Clovis: To see if you could really act as one of them.
	Actor_Says(kActorClovis,  470,  3); // Clovis: He wiped out all your memories of our time together.
	Actor_Says(kActorClovis,  480,  3); // Clovis: As soldiers during the Phobos wars.
	Actor_Says(kActorClovis,  490,  3); // Clovis: The battle of the Gemini. Firefights on the top of the Olympus Mountains!
	Actor_Says(kActorMcCoy,  2615, 17); // McCoy: I don't remember.
	Actor_Says(kActorClovis,  500,  3); // Clovis: But if you dig real deep and feel, you'll know what's real.
	Actor_Says(kActorClovis,  530,  3); // Clovis: It's a passage to freedom, McCoy. To your destiny.
	Actor_Says(kActorClovis,  540,  3); // Clovis: An underground railroad to carry you from bondage. We'll be waiting.
	Sound_Play(kSfxSPNBEEP9, 100, 0, 0, 50);
	Actor_Clue_Acquire(kActorMcCoy, kCluePhoneCallClovis, true, -1);
}

void SceneScriptMA04::turnOnTV() {
	Overlay_Play("MA04OVR2", 0, true, false, 0);
	switch (Global_Variable_Query(kVariableNextTvNews)) {
	case 0:
		ADQ_Add(kActorNewscaster,   0, kAnimationModeTalk); // Newscaster: The Tyrell Corporation today recalled the last four functioning entertainment model
		ADQ_Add(kActorNewscaster,  10, kAnimationModeTalk); // Newscaster: Nexus-Five Replicants from Off-World service.
		ADQ_Add(kActorNewscaster,  20, kAnimationModeTalk); // Newscaster: Dr Eldon Tyrell speaking at a share holders meeting
		ADQ_Add(kActorNewscaster,  30, kAnimationModeTalk); // Newscaster: denied allegations that the model was defective and possibly dangerous.
		ADQ_Add(kActorTyrell,     430, kAnimationModeTalk); // Tyrell: The simple fact is that the model wasn't performing its designated function.
		ADQ_Add(kActorTyrell,     440, kAnimationModeTalk); // Tyrell: There was one isolated incident of violence a couple of years ago...
		ADQ_Add(kActorTyrell,     450, kAnimationModeTalk); // Tyrell: but the individual unit in question was immediately retired from service.
		ADQ_Add(kActorTyrell,     460, kAnimationModeTalk); // Tyrell: No, the problem is far from anything sinister. The entertainment model just isn't funny.
		break;
	case 1:
		ADQ_Add(kActorNewscaster, 40, kAnimationModeTalk); // Newscaster: Violent street crime in the city rose again last year by a whopping 26 percent
		ADQ_Add(kActorNewscaster, 50, kAnimationModeTalk); // Newscaster: according to the latest figures from the Department of Justice.
		ADQ_Add(kActorNewscaster, 60, kAnimationModeTalk); // Newscaster: Governor Kolvig attributed the change to the dramatic increase in the number of Specials
		ADQ_Add(kActorNewscaster, 70, kAnimationModeTalk); // Newscaster: registered as city residents and promised a tough new set of standards...
		ADQ_Add(kActorNewscaster, 80, kAnimationModeTalk); // Newscaster: for the City Immigration Bureau by next month.
		break;
	case 2:
		if (Actor_Query_Friendliness_To_Other(kActorClovis, kActorMcCoy) <= Actor_Query_Friendliness_To_Other(kActorSteele, kActorMcCoy)) {
			ADQ_Add(kActorNewscaster,   90, kAnimationModeTalk); // Newscaster: Another retirement in the Fourth Sector today
			ADQ_Add(kActorNewscaster,  100, kAnimationModeTalk); // Newscaster: as a Blade Runner shot an alleged Replicant in the middle of a crowded street.
			ADQ_Add(kActorNewscaster,  110, kAnimationModeTalk); // Newscaster: Lieutenant Edison Guzza, supervisor of the LPD's Rep Detect Unit, had this to say...
			ADQ_Add(kActorGuzza,      1540, kAnimationModeTalk); // Guzza: It was a good clean take-down.
			ADQ_Add(kActorGuzza,      1550, kAnimationModeTalk); // Guzza: The public should thank us for retiring these skin-jobs before they get into the real trouble.
			ADQ_Add(kActorGuzza,      1560, kAnimationModeTalk); // Guzza: You want to get that thing out of my face before it ends up shooting film where the sun don't shine?
		} else {
			ADQ_Add(kActorNewscaster,  120, kAnimationModeTalk); // Newscaster: A Blade Runner foiled a rogue Replicant's escape today
			ADQ_Add(kActorNewscaster,  130, kAnimationModeTalk); // Newscaster: Retiring it in the Fourth Sector before it could hurt anyone.
			ADQ_Add(kActorNewscaster,  140, kAnimationModeTalk); // Newscaster: Our man on the scene got this exclusive reaction from Edison Guzza
			ADQ_Add(kActorNewscaster,  150, kAnimationModeTalk); // Newscaster: supervisor of the LPD Rep Detect unit.
			ADQ_Add(kActorGuzza,      1570, kAnimationModeTalk); // Guzza: The detective on the case handled it by the book.
			ADQ_Add(kActorGuzza,      1580, kAnimationModeTalk); // Guzza: He investigated a lead at Taffy's, tested the alleged Replicant
			ADQ_Add(kActorGuzza,      1590, kAnimationModeTalk); // Guzza: and retired it exactly the way he is supposed to.
		}
		break;
	case 3:
		ADQ_Add(kActorNewscaster,     170, kAnimationModeTalk); // Newscaster: During his last election campaign, Governor Kolvig promised
		ADQ_Add(kActorNewscaster,     180, kAnimationModeTalk); // Newscaster: a bold new plan of action
		ADQ_Add(kActorNewscaster,     190, kAnimationModeTalk); // Newscaster: to clean up the worst regions of the highly toxic debris that surrounds our city.
		ADQ_Add(kActorNewscaster,     200, kAnimationModeTalk); // Newscaster: The so called Kipple.
		ADQ_Add(kActorNewscaster,     210, kAnimationModeTalk); // Newscaster: Just how much progress has been made since then?
		ADQ_Add(kActorNewscaster,     220, kAnimationModeTalk); // Newscaster: We spoke to the governor just before his weekly meeting with the City Council.
		ADQ_Add(kActorGovernorKolvig,  80, kAnimationModeTalk); // Governor Kolvig: Our studies have shown that the Kipple's effect upon Los Angeles is minor.
		ADQ_Add(kActorGovernorKolvig,  90, kAnimationModeTalk); // Governor Kolvig: The radiation and toxic waste is contained in relatively small pockets...
		ADQ_Add(kActorGovernorKolvig, 100, kAnimationModeTalk); // Governor Kolvig: all miles distant from the city center.
		ADQ_Add(kActorGovernorKolvig, 110, kAnimationModeTalk); // Governor Kolvig: Nevertheless, cleaning things up out there is a worthwhile goal.
		ADQ_Add(kActorGovernorKolvig, 120, kAnimationModeTalk); // Governor Kolvig: Especially, with the thousands of specials living on the city fringes.
		ADQ_Add(kActorGovernorKolvig, 130, kAnimationModeTalk); // Governor Kolvig: For now, all I can say is that we're looking at several options and I'm sure everyone will be pleasantly surprised by our final proposal.
		break;
	case 4:
		ADQ_Add(kActorNewscaster, 230, kAnimationModeTalk); // Newscaster: A homeless man was discovered in a Chinatown dumpster this morning apparently the victim of foul play.
		ADQ_Add(kActorNewscaster, 240, kAnimationModeTalk); // Newscaster: Chief Bryant promised a full investigation and dismissed allegations that the LPD has been lax in following up on crimes in the 4rth Sector.
		break;
	}
}

void SceneScriptMA04::sleep() {
	if (!Loop_Actor_Walk_To_Scene_Object(kActorMcCoy, "BED-SHEETS", 12, true, false)) {
		Actor_Says(kActorMcCoy, 8530, 12); // McCoy: Portrait of the sleep deprived.
		Music_Stop(4);
		if (isPhoneMessageWaiting() || isPhoneRinging()) {
			Overlay_Remove("MA04OVER");
		}
		Player_Loses_Control();
		Game_Flag_Set(kFlagMA04McCoySleeping);
		if ((Game_Flag_Query(kFlagZubenRetired) || Game_Flag_Query(kFlagZubenSpared)) && Global_Variable_Query(kVariableChapter) == 1) {
			if (Actor_Query_Goal_Number(kActorZuben) == kGoalZubenGone) {
				Actor_Put_In_Set(kActorZuben, kSetFreeSlotA);
				Actor_Set_At_Waypoint(kActorZuben, 33, 0);
			}
			Game_Flag_Set(kFlagChapter2Intro);
			Global_Variable_Set(kVariableChapter, 2);
			Chapter_Enter(2, kSetMA02_MA04, kSceneMA04);
#if BLADERUNNER_ORIGINAL_BUGS
			if (Query_Difficulty_Level() != kGameDifficultyEasy) {
				if (!Game_Flag_Query(kFlagZubenBountyPaid)) {
					Global_Variable_Increment(kVariableChinyen, 200);
				}
			}
#else // ensure valid kFlagZubenBountyPaid flag state
			// NOTE If not for the "Report Im" to Guzza, it would be simpler to have McCoy get the money as soon as he retires Zuben
			//		so that would be in a single place in the code
			if (!Game_Flag_Query(kFlagZubenBountyPaid) && Game_Flag_Query(kFlagZubenRetired)) { // get retirement money at end of day 1 only if Zuben was retired.
				if (Query_Difficulty_Level() != kGameDifficultyEasy) {
					Global_Variable_Increment(kVariableChinyen, 200);
				}
				Game_Flag_Set(kFlagZubenBountyPaid); // not a proper bug, but was missing from original code, so the flag would remain in non-consistent state in this case
			}
#endif // BLADERUNNER_ORIGINAL_BUGS
		} else {
			Set_Enter(kSetMA02_MA04, kSceneMA04);
		}
		Scene_Loop_Start_Special(kSceneLoopModeChangeSet, kMA04LoopSleep, false);
	}
}

} // End of namespace BladeRunner
