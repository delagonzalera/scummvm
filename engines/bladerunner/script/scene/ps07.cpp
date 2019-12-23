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

void SceneScriptPS07::InitializeScene() {
	Setup_Scene_Information(609.07f, 0.22f, -598.67f, 768);
	Scene_Exit_Add_2D_Exit(0, 610, 0, 639, 479, 1);
	Ambient_Sounds_Remove_All_Non_Looping_Sounds(false);
	Ambient_Sounds_Add_Looping_Sound(kSfxLABAMB3, 80, 0, 1);
	Ambient_Sounds_Add_Sound(kSfxTUBES1,   5, 20,  5, 10,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxLABMISC1, 5, 30,  5, 10,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxLABMISC2, 2, 20,  5, 10,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxLABMISC3, 2, 10, 10, 20,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxLABMISC4, 2, 10, 10, 20,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxLABMISC5, 2, 10, 10, 20,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxLABMISC6, 2, 10, 10, 20,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxLABMISC7, 2, 30, 10, 15,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxLABMISC8, 2, 20, 10, 15,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxLABMISC9, 5, 20, 10, 15,  -70,  70, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxTUBES4,   5, 30,  5,  8, -100, 100, -101, -101, 0, 0);
}

void SceneScriptPS07::SceneLoaded() {
	Obstacle_Object("RICE BOX01", true); // garbage can
	Unobstacle_Object("RICE BOX01", true);
	if (_vm->_cutContent) {
		Unobstacle_Object("L.MOUSE", true);
		Clickable_Object("L.MOUSE");
	}
}

bool SceneScriptPS07::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptPS07::ClickedOn3DObject(const char *objectName, bool a2) {
	if (Object_Query_Click("L.MOUSE", objectName)) { // this object is restored in the scene (buzzer button)
		Sound_Play(kSfxLABBUZZ1, 70, 0, 0, 50);
		if (Actor_Query_Goal_Number(kActorKlein) < kGoalKleinIsAnnoyedByMcCoyInit
		    && Actor_Query_Goal_Number(kActorKlein) > kGoalKleinDefault
		) {
			Actor_Face_Actor(kActorMcCoy, kActorKlein, true);
			Actor_Set_Goal_Number(kActorKlein, kGoalKleinGotoLabSpeaker);
			Actor_Modify_Friendliness_To_Other(kActorKlein, kActorMcCoy, -3);
		}
		return true;
	}
	return false;
}

bool SceneScriptPS07::ClickedOnActor(int actorId) {
	if (actorId == kActorKlein) {
		if (_vm->_cutContent
		    && (Actor_Query_Goal_Number(kActorKlein) >= kGoalKleinIsAnnoyedByMcCoyInit
		        || (Actor_Query_Goal_Number(kActorKlein) < kGoalKleinIsAnnoyedByMcCoyInit
		            && Actor_Query_Friendliness_To_Other(kActorKlein, kActorMcCoy) < 40))
		) {
			// Annoyed Dino ignores McCoy
			return true;
		} else {
			Actor_Face_Actor(kActorMcCoy, kActorKlein, true);
			Actor_Set_Goal_Number(kActorKlein, kGoalKleinGotoLabSpeaker);
			if (!Game_Flag_Query(kFlagPS07KleinTalkClues)) {
				Actor_Says(kActorMcCoy, 4115, 13); // McCoy: Got anything new for me, Dino?
			}

			if (!Game_Flag_Query(kFlagPS07KleinTalkClues)
			    && ((_vm->_cutContent && (Actor_Clue_Query(kActorKlein, kClueShellCasings)
			                              || Actor_Clue_Query(kActorKlein, kClueOfficersStatement)
			                              || Actor_Clue_Query(kActorKlein, kCluePaintTransfer)
			                              || Actor_Clue_Query(kActorKlein, kClueChromeDebris)))
			        || (!_vm->_cutContent && (Game_Flag_Query(kFlagMcCoyHasShellCasings)
			                                  || Game_Flag_Query(kFlagMcCoyHasOfficersStatement)
			                                  || Game_Flag_Query(kFlagMcCoyHasPaintTransfer)
			                                  || Game_Flag_Query(kFlagMcCoyHasChromeDebris))))
			) {
				Actor_Face_Actor(kActorKlein, kActorMcCoy, true);
				Actor_Says(kActorKlein, 30, 12); // Klein: I was wondering when you'd show your mug around here, McCoy.
				Game_Flag_Set(kFlagPS07KleinTalkClues);
			} else {
				if (Game_Flag_Query(kFlagPS07KleinTalkClues)) {
					Actor_Says(kActorMcCoy, 4130, 18); // McCoy: Anything else?
				}
			}

			// Restored cut content version uses Clues system. Klein gets the clues if McCoy has uploaded to the mainframe
			// The original version takes a shortcut here and uses flags for this (McCoy does not have to upload the clues to the mainframe)
			if (((_vm->_cutContent && Actor_Clue_Query(kActorKlein, kClueShellCasings))
			      || (!_vm->_cutContent && Game_Flag_Query(kFlagMcCoyHasShellCasings)))
			    && !Game_Flag_Query(kFlagPS07KleinTalkShellCasings)
			) {
				Game_Flag_Set(kFlagPS07KleinTalkShellCasings);
				Actor_Clue_Acquire(kActorMcCoy, kClueLabShellCasings, false, kActorKlein);
				Actor_Says(kActorKlein,   50, 16); // Klein: The shell casings from Runciter's should be in the Museum of military history.
				Actor_Says(kActorMcCoy, 4135, 13); // McCoy: How's that?
				Actor_Says(kActorKlein,   60, 15); // Klein: They're from an Endor rifle. It's standard-issue Off-World military hardware.
				Actor_Says(kActorKlein,   70, 12); // Klein: Heavy caliber. That puppy can punch right through a tank.
				Actor_Says(kActorMcCoy, 4140, 18); // McCoy: What can I conclude from that?
				Actor_Says(kActorKlein,   80, 14); // Klein: Whoever used this piece didn't just walk into their local sporting goods store and buy it.
				Actor_Says(kActorKlein,   90, 14); // Klein: Those babies are collectibles and they ain't cheap.
				Actor_Set_Goal_Number(kActorKlein, kGoalKleinMovingInLab01);
				return true;
			}

			if (((_vm->_cutContent && Actor_Clue_Query(kActorKlein, kClueOfficersStatement))
			      || (!_vm->_cutContent && Game_Flag_Query(kFlagMcCoyHasOfficersStatement)))
			    && !Game_Flag_Query(kFlagPS07KleinTalkOfficersStatement)
			) {
				Game_Flag_Set(kFlagPS07KleinTalkOfficersStatement);
				Actor_Clue_Acquire(kActorMcCoy, kClueLabCorpses, false, kActorKlein);
				Actor_Says(kActorKlein,  100, 13); // Klein: We got the animal corpses from Runciter's but there wasn't a whole lot to work with.
				Actor_Says(kActorMcCoy, 4145, 13); // McCoy: Bone marrow?
				Actor_Says(kActorKlein,  110, 12); // Klein: About 50% were real. The rest were fakes.
				Actor_Says(kActorMcCoy, 4150, 13); // McCoy: So, Runciter was ripping his customers off.
				Actor_Says(kActorKlein,  120, 14); // Klein: State-of-the-art fakes. Miles beyond the crap you see on Animoid Row.
				Actor_Says(kActorMcCoy, 4155, 17); // McCoy: The kind that comes from legit manufacturers?
				Actor_Says(kActorKlein,  130, 15); // Klein: Eh, they're the ones that usually produce the quality.
				Actor_Says(kActorMcCoy, 4160, 13); // McCoy: What about the wounds?
				Actor_Says(kActorKlein,  140, 16); // Klein: Most of them were shot point-blank but we're looking at some others... pteh, sloppy cuts with a knife. Like the perp was really pissed off at something.
				Actor_Says(kActorMcCoy, 4165, 18); // McCoy: Butcher knife?
				Actor_Says(kActorKlein,  160, 13); // Klein: Could be. But who cuts meat anymore?
				Actor_Says(kActorMcCoy, 4170, 19); // McCoy: Nice bunch of Reps I'm tailing.
				Actor_Says(kActorMcCoy, 4175, 19); // McCoy: Hope you're not pulling a meat cleaver out of my back next week.
				Actor_Modify_Friendliness_To_Other(kActorKlein, kActorMcCoy, 3);
				Actor_Set_Goal_Number(kActorKlein, kGoalKleinMovingInLab01);
				return true;
			}

			if (((_vm->_cutContent && Actor_Clue_Query(kActorKlein, kCluePaintTransfer))
			      || (!_vm->_cutContent && Game_Flag_Query(kFlagMcCoyHasPaintTransfer)))
			    && !Game_Flag_Query(kFlagPS07KleinTalkPaintTransfer)
			) {
				Game_Flag_Set(kFlagPS07KleinTalkPaintTransfer);
				Actor_Clue_Acquire(kActorMcCoy, kClueLabPaintTransfer, false, kActorKlein);
				Actor_Says(kActorKlein,  170, 14); // Klein: That black paint you found on the fire hydrant is from a ground car.
				Actor_Says(kActorMcCoy, 4180, 13); // McCoy: You sure?
				Actor_Says(kActorKlein,  180, 12); // Klein: The angle of impact was almost head-on.
				Actor_Says(kActorKlein,  190, 13); // Klein: And there ain't too many spinner drivers klutzy enough...
				Actor_Says(kActorKlein,  200, 16); // Klein: to smack a street-level hydrant in VTOL mode.
				Actor_Says(kActorMcCoy, 4185, 18); // McCoy: I heard your wife got her license back.
				Actor_Says(kActorKlein,  210, 12); // Klein: Ha, you're a real funny guy, McCoy.
				Actor_Modify_Friendliness_To_Other(kActorKlein, kActorMcCoy, -12);
				Actor_Set_Goal_Number(kActorKlein, kGoalKleinMovingInLab01);
				return true;
			}

			if (((_vm->_cutContent && Actor_Clue_Query(kActorKlein, kClueChromeDebris))
			      || (!_vm->_cutContent && Game_Flag_Query(kFlagMcCoyHasChromeDebris)))
			    && !Game_Flag_Query(kFlagPS07KleinTalkChromeDebris)
			) {
				Game_Flag_Set(kFlagPS07KleinTalkChromeDebris);
				Actor_Says(kActorKlein,  220, 12); // Klein: That piece of chrome you found outside of Runciter's is from a 1995 Pontiac.
				Actor_Says(kActorMcCoy, 4190, 13); // McCoy: Nice set of wheels.
				Actor_Says(kActorKlein,  230, 14); // Klein: It's okay. Not what I'd call a classic.
				if (_vm->_cutContent) {
					Actor_Says(kActorMcCoy, 4195, 13); // McCoy: Any way to find the car's owner?
					Actor_Says(kActorKlein,  240, 16); // Klein: Well, you'd need the license number or the VIN number from the inside of the door. You run either number through the Mainframe and it'll ID the car for you.
				}
				Actor_Set_Goal_Number(kActorKlein, kGoalKleinMovingInLab01);
				return true;
			}
			// If Dino has nothing to analyze
			if (_vm->_cutContent) {
				switch (Random_Query(1, 4)) {
				case 1:
					Actor_Says(kActorKlein, 0, 13); // Klein: Zip, man. Try me later.
					break;
				case 2:
					Actor_Says(kActorKlein, 40, 13); // Klein: Not right now. If anything else turns up, you'll be the first to know.
					break;
				case 3:
					Actor_Says(kActorKlein, 270, 13); // Klein: Nah, the place has been pretty quiet the last couple of days.
					break;
				case 4:
					Actor_Says(kActorKlein, 280, 13); // Klein: Come back at me when you got something worthwhile, McCoy.
					break;
				}
			} else {
				Actor_Says(kActorKlein, 0, 13); // Klein: Zip, man. Try me later.
			}
			Actor_Set_Goal_Number(kActorKlein, kGoalKleinMovingInLab01);
			return true;
		}
	}
	return false;

}

bool SceneScriptPS07::ClickedOnItem(int itemId, bool a2) {
	return false;
}

bool SceneScriptPS07::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 609.07f, 0.22f, -598.67f, 0, false, false, false)) {
			Set_Enter(kSetPS02, kScenePS02);
		}
		return true;
	}
	return false;
}

bool SceneScriptPS07::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptPS07::SceneFrameAdvanced(int frame) {
}

void SceneScriptPS07::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptPS07::PlayerWalkedIn() {
	Loop_Actor_Walk_To_XYZ(kActorMcCoy, 561.07f, 0.34f, -606.67f, 6, false, false, false);
	Game_Flag_Reset(kFlagPS02toPS07);
	//return false;
}

void SceneScriptPS07::PlayerWalkedOut() {
	if (!Game_Flag_Query(kFlagPS07KleinInsulted)
	    && ((_vm->_cutContent && Global_Variable_Query(kVariableChapter) < 4)
	        || (!_vm->_cutContent && Global_Variable_Query(kVariableChapter) == 1))
	){
		Actor_Set_Goal_Number(kActorKlein, kGoalKleinDefault);
	}
}

void SceneScriptPS07::DialogueQueueFlushed(int a1) {
}

} // End of namespace BladeRunner
