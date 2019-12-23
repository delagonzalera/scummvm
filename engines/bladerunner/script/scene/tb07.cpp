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

enum kTB07Loops {
	kTB07LoopMainLoop      = 0,
	kTB07LoopShadeDrop     = 2,
	kTB07LoopMainShadeDown = 3
};

void SceneScriptTB07::InitializeScene() {
	Setup_Scene_Information(68.0f, 12.0f, 288.0f, 0);

	Scene_Exit_Add_2D_Exit(0, 383, 445, 639, 479, 2);

	Ambient_Sounds_Add_Looping_Sound(kSfxBRBED3, 20, 0, 1);
	Ambient_Sounds_Add_Sound(kSfxSUNROOM1, 2, 55, 14, 14, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSUNROOM2, 2, 55, 14, 14, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSUNROOM3, 2, 55, 14, 14, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSUNROOM4, 2, 55, 14, 14, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxBELLY1,   1, 15, 20, 25, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxBELLY2,   1, 15, 20, 25, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxBELLY3,   1, 20, 20, 25, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxBELLY4,   1, 15, 20, 25, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxBELLY5,   1, 15, 20, 25, -100, 100, -101, -101, 0, 0);

	if ( Global_Variable_Query(kVariableChapter) == 4
	 && !Actor_Clue_Query(kActorMcCoy, kClueDNATyrell)
	) {
		Item_Add_To_World(kItemDNATyrell, kModelAnimationDNADataDisc, kSetTB07, 9.7f, 48.7f, -174.22f, 0, 12, 12, false, true, false, true);
	}
	if (Game_Flag_Query(kFlagTB07ShadeDown)) {
		Scene_Loop_Set_Default(kTB07LoopMainShadeDown);
	} else {
		Scene_Loop_Set_Default(kTB07LoopMainLoop);
	}
}

void SceneScriptTB07::SceneLoaded() {
	Obstacle_Object("EAGLE01", true);
	Clickable_Object("EAGLE01");
#if BLADERUNNER_ORIGINAL_BUGS
#else
	Unclickable_Object("EAGLE01");
#endif // BLADERUNNER_ORIGINAL_BUGS
}

bool SceneScriptTB07::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptTB07::ClickedOn3DObject(const char *objectName, bool a2) {
	return false;
}

bool SceneScriptTB07::ClickedOnActor(int actorId) {
	return false;
}

bool SceneScriptTB07::ClickedOnItem(int itemId, bool a2) {
	if (!Loop_Actor_Walk_To_Item(kActorMcCoy, itemId, 36, true, false)) {
		Actor_Face_Item(kActorMcCoy, itemId, true);
		if (itemId == kItemDNATyrell) {
			Item_Pickup_Spin_Effect(kModelAnimationDNADataDisc, 331, 296);
			Actor_Clue_Acquire(kActorMcCoy, kClueDNATyrell, false, -1);
		}
		Item_Remove_From_World(itemId);
	}
	return false;
}

bool SceneScriptTB07::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 68.0f, 12.0f, 288.0f, 0, true, false, false)) {
#if BLADERUNNER_ORIGINAL_BUGS
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
#endif // BLADERUNNER_ORIGINAL_BUGS
			if (Global_Variable_Query(kVariableChapter) == 4) {
				Game_Flag_Set(kFlagTB07toTB02);
				Set_Enter(kSetTB02_TB03, kSceneTB02);
			} else {
				Game_Flag_Reset(kFlagMcCoyInChinaTown);
				Game_Flag_Reset(kFlagMcCoyInRunciters);
				Game_Flag_Reset(kFlagMcCoyInMcCoyApartment);
				Game_Flag_Reset(kFlagMcCoyInPoliceStation);
				Game_Flag_Reset(kFlagMcCoyInBradburyBuilding);
				Game_Flag_Reset(kFlagMcCoyInHysteriaHall);
				Game_Flag_Reset(kFlagMcCoyInTyrellBuilding);
				Game_Flag_Reset(kFlagTB02ElevatorToTB05);
				switch (Spinner_Interface_Choose_Dest(-1, false)) {
				case kSpinnerDestinationPoliceStation:
					Game_Flag_Set(kFlagMcCoyInPoliceStation);
					Game_Flag_Reset(kFlagSpinnerAtTB02);
					Game_Flag_Set(kFlagSpinnerAtPS01);
					Set_Enter(kSetPS01, kScenePS01);
					break;
				case kSpinnerDestinationMcCoysApartment:
					Game_Flag_Set(kFlagMcCoyInMcCoyApartment);
					Game_Flag_Reset(kFlagSpinnerAtTB02);
					Game_Flag_Set(kFlagSpinnerAtMA01);
					Set_Enter(kSetMA01, kSceneMA01);
					break;
				case kSpinnerDestinationRuncitersAnimals:
					Game_Flag_Set(kFlagMcCoyInRunciters);
					Game_Flag_Reset(kFlagSpinnerAtTB02);
					Game_Flag_Set(kFlagSpinnerAtRC01);
					Set_Enter(kSetRC01, kSceneRC01);
					break;
				case kSpinnerDestinationChinatown:
					Game_Flag_Set(kFlagMcCoyInChinaTown);
					Game_Flag_Reset(kFlagSpinnerAtTB02);
					Game_Flag_Set(kFlagSpinnerAtCT01);
					Set_Enter(kSetCT01_CT12, kSceneCT01);
					break;
				case kSpinnerDestinationAnimoidRow:
					Game_Flag_Set(kFlagMcCoyInAnimoidRow);
					Game_Flag_Reset(kFlagSpinnerAtTB02);
					Game_Flag_Set(kFlagSpinnerAtAR01);
					Set_Enter(kSetAR01_AR02, kSceneAR01);
					break;
				case kSpinnerDestinationDNARow:
					Game_Flag_Set(kFlagMcCoyInDNARow);
					Game_Flag_Reset(kFlagSpinnerAtTB02);
					Game_Flag_Set(kFlagSpinnerAtDR01);
					Set_Enter(kSetDR01_DR02_DR04, kSceneDR01);
					break;
				case kSpinnerDestinationBradburyBuilding:
					Game_Flag_Set(kFlagMcCoyInBradburyBuilding);
					Game_Flag_Reset(kFlagSpinnerAtTB02);
					Game_Flag_Set(kFlagSpinnerAtBB01);
					Set_Enter(kSetBB01, kSceneBB01);
					break;
				case kSpinnerDestinationNightclubRow:
					Game_Flag_Set(kFlagMcCoyInNightclubRow);
					Game_Flag_Reset(kFlagSpinnerAtTB02);
					Game_Flag_Set(kFlagSpinnerAtNR01);
					Set_Enter(kSetNR01, kSceneNR01);
					break;
				case kSpinnerDestinationHysteriaHall:
					Game_Flag_Set(kFlagMcCoyInHysteriaHall);
					Game_Flag_Reset(kFlagSpinnerAtTB02);
					Game_Flag_Set(kFlagSpinnerAtHF01);
					Set_Enter(kSetHF01, kSceneHF01);
					break;
				default:
					Game_Flag_Set(kFlagMcCoyInTyrellBuilding);
					Loop_Actor_Walk_To_XYZ(kActorMcCoy, 44.0f, 12.0f, 176.0f, 0, false, false, false);
					break;
				}
			}
		}
		return true;
	}
	return false;
}

bool SceneScriptTB07::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptTB07::SceneFrameAdvanced(int frame) {
	if (frame == 66) {
		Ambient_Sounds_Play_Sound(kSfxLIGHTON, 20, 99, 0, 0);
	}
	//return false;
}

void SceneScriptTB07::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptTB07::PlayerWalkedIn() {
	int chapter = Global_Variable_Query(kVariableChapter);
	Loop_Actor_Walk_To_XYZ(kActorMcCoy, 44.0f, 12.0f, 176.0f, 0, false, false, false);
	if ((chapter == 2
	  || chapter == 3
	 )
	 && !Game_Flag_Query(kFlagTB07RachaelTalk)
	) {
		Player_Set_Combat_Mode(false);
		McCoyTalkWithRachaelAndTyrell();
	}
}

void SceneScriptTB07::PlayerWalkedOut() {
#if BLADERUNNER_ORIGINAL_BUGS
#else
	Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
	Ambient_Sounds_Remove_All_Looping_Sounds(1);
	if (Global_Variable_Query(kVariableChapter) < 4
	    && !Game_Flag_Query(kFlagMcCoyInTyrellBuilding)
	) {
		// Acts 2, 3 - use a spinner fly-through transition
		Outtake_Play(kOuttakeAway1,    true, -1);  // available in Acts 2, 3
	}
#endif // BLADERUNNER_ORIGINAL_BUGS

}

void SceneScriptTB07::DialogueQueueFlushed(int a1) {
}

void SceneScriptTB07::McCoyTalkWithRachaelAndTyrell() {
	Game_Flag_Set(kFlagTB07RachaelTalk);
	Delay(1500);
	Loop_Actor_Walk_To_XYZ(kActorMcCoy, 44.98f, 12.0f, 49.79f, 0, false, false, false);
	Actor_Face_Heading(kActorMcCoy, 178, true);
	Delay(3000);
	Actor_Put_In_Set(kActorRachael, kSetTB07);
	Actor_Set_At_XYZ(kActorRachael, -260.15f, 12.0f, -19.16f, 256);
	Actor_Change_Animation_Mode(kActorRachael, kAnimationModeIdle);
	Outtake_Play(kOuttakeRachel, true, -1);
	Loop_Actor_Walk_To_XYZ(kActorRachael, -146.15f, 12.0f, -5.84f, 0, false, false, false);
	Actor_Face_Actor(kActorRachael, kActorMcCoy, true);
	Actor_Says(kActorRachael, 480, 14); // Rachael: Beautiful isn't it?
	Actor_Face_Actor(kActorMcCoy, kActorRachael, true);
	Actor_Says(kActorMcCoy,   5315, 9); // McCoy: The owl or the view?
	Actor_Says(kActorRachael,  490, 3); // Rachael: Both.
	Actor_Face_Heading(kActorMcCoy, 178, true);
	Actor_Says(kActorMcCoy, 5320, 15); // McCoy: The view is amazing. The owl, (sigh) well...
	Actor_Says_With_Pause(kActorMcCoy, 5325, 1.0f, 19);
	Actor_Start_Speech_Sample(kActorRachael, 500);
	Loop_Actor_Walk_To_XYZ(kActorRachael, -60.15f, 12.0f, 60.84f, 0, false, false, false);
	Actor_Face_Actor(kActorRachael, kActorMcCoy, true);
	Actor_Face_Actor(kActorMcCoy, kActorRachael, true);
	Actor_Says(kActorMcCoy,   5330, 14); // McCoy: Owls are extinct. Who told you the model was real?
	Actor_Says(kActorRachael,  510, 12); // Rachael: Dr. Tyrell.
	Actor_Says(kActorMcCoy,   5335, 16); // McCoy: He's wrong or lying. Check your Sidney's.
	Actor_Says(kActorRachael,  520, 17); // Rachael: Eldon wouldn't make up something like that.
	Actor_Says(kActorMcCoy,   5340,  3); // McCoy: You're Tyrell's assistant?
	Actor_Start_Speech_Sample(kActorRachael, 530);
	Loop_Actor_Walk_To_XYZ(kActorRachael, -4.15f, 12.0f, 54.73f, 0, false, false, false);
	Actor_Says(kActorRachael,  540, 16); // Rachael: I'm fully familiar with all Tyrell operations.
	Actor_Says(kActorMcCoy,   5345, 18); // McCoy: He does know I'm a cop, right? Blade Runner unit?
	Actor_Says(kActorRachael,  550, 13); // Rachael: Dr. Tyrell has already met with one Blade Runner today and has important business to attend to.
	Actor_Says(kActorRachael,  570, 18); // Rachael: Your department should coordinate its efforts better.
	Actor_Says_With_Pause(kActorMcCoy, 5350, 0.0f, 18);
	Actor_Says(kActorRachael,  580, 16); // Rachael: You may speak to me or you may leave. I can't offer you any other choices.
	Actor_Says(kActorMcCoy,   5355, 16); // McCoy: Fair enough. I wanted to ask about Marcus Eisenduller.
	Actor_Says(kActorRachael,  590, 17); // Rachael: I can't tell you who killed him, if that's what you're asking.
	Actor_Says(kActorMcCoy,   5360, 17); // McCoy: We've already got a suspect.
	Actor_Says(kActorMcCoy,   5365, 13); // McCoy: What I want to know is why the killer would be interested in Replicant DNA information.
	Actor_Says_With_Pause(kActorRachael, 600, 1.0f, 12);
	Actor_Says(kActorMcCoy, 5370, 3); // McCoy: Oh, yeah? Why is that?
	Loop_Actor_Walk_To_XYZ(kActorRachael, -24.15f, 12.0f, -10.84f, 0, false, false, false);
	Actor_Says(kActorRachael, 610, 13); // Rachael: Your suspect is obviously a Replicant. A very dangerous one.
	Actor_Face_Actor(kActorMcCoy, kActorRachael, true);
	Actor_Says(kActorMcCoy, 5375, 18); // McCoy: I thought Replicants were safe! "More human than human".
	Actor_Says(kActorMcCoy, 5380, 19); // McCoy: Isn't Dr. Tyrell pushing to make it legal to use Rep labor to clean up the Kipple?
	Actor_Face_Actor(kActorRachael, kActorMcCoy, true);
	Actor_Says(kActorRachael, 620, 18); // Rachael: Are we here to discuss corporate policy or Marcus Eisenduller's murder?
	Actor_Says_With_Pause(kActorMcCoy, 5385, 2.0f, 12);
	Actor_Says_With_Pause(kActorMcCoy, 5390, 2.0f, 14);
	Actor_Says(kActorMcCoy, 5395, 15); // McCoy: Yeah, I guess you would have. 50 people dead.
	Actor_Says_With_Pause(kActorRachael, 630, 0.0f, 14);
	Actor_Says(kActorMcCoy,   5400, 18); // McCoy: I'm surprised you are not doing spin control.
	Actor_Says(kActorMcCoy,   5405,  3); // McCoy: The public gets nervous when your pets wander around the city without a leash.
	Actor_Says(kActorRachael,  640, 12); // Rachael: Replicants aren't pets.
	Actor_Says(kActorMcCoy,   5410, 16); // McCoy: Right. Pets live longer and don't go around killing people.
	Actor_Says(kActorRachael,  650, 15); // Rachael: They-- There are safeguards in the design.
	Actor_Says_With_Pause(kActorMcCoy, 5415, 1.0f, 17);
	Actor_Says(kActorMcCoy,   5420, 14); // McCoy: I'd probably get a little crazy too but I don't think I'd start killing people.
	Actor_Says(kActorRachael,  660, 15); // Rachael: Wouldn't you?
	Actor_Put_In_Set(kActorTyrell, kSetTB07);
	Actor_Set_At_XYZ(kActorTyrell, 68.0f, 12.0f, 288.0f, 0);
	Actor_Change_Animation_Mode(kActorTyrell, kAnimationModeIdle);
	Scene_Loop_Set_Default(kTB07LoopMainShadeDown);
	Scene_Loop_Start_Special(kSceneLoopModeOnce, kTB07LoopShadeDrop, false);
	Actor_Start_Speech_Sample(kActorTyrell, 0);
	Loop_Actor_Walk_To_XYZ(kActorTyrell, 44.0f, 12.0f, 176.0f, 0, false, false, false);
	Actor_Face_Actor(kActorTyrell, kActorMcCoy, true);
	Actor_Face_Actor(kActorMcCoy, kActorTyrell, true);
	Actor_Face_Actor(kActorRachael, kActorTyrell, true);
	Actor_Says(kActorTyrell,   10, 12); // Tyrell: I've given it the purity and innocence of childhood.
	Actor_Says(kActorTyrell,   20,  3); // Tyrell: And all the joy and despair that comes with a real existence.
	Actor_Says(kActorTyrell,   30, 12); // Tyrell: That's hardly a death sentence. That's life.
	Actor_Says(kActorMcCoy,  5425, 18); // McCoy: I thought you weren't meeting with any more Blade Runners today.
	Actor_Set_Goal_Number(kActorRachael, kGoalRachaelLeavesAfterTyrellMeeting);
	Actor_Says(kActorTyrell, 40, 15); // Tyrell: Well I decided to make an exception, Mr. eh... McCoy, isn't it?
	Actor_Start_Speech_Sample(kActorTyrell, 50);
	Loop_Actor_Walk_To_XYZ(kActorTyrell, -10.0f, 12.0f, 100.0f, 0, false, false, false);
	Actor_Face_Actor(kActorTyrell, kActorMcCoy, true);
	Actor_Face_Actor(kActorMcCoy, kActorTyrell, true);
	Actor_Says(kActorMcCoy,  5430, 17); // McCoy: Not really, no.
	Actor_Says(kActorMcCoy,  5435, 16); // McCoy: I do see why a Replicant who has an identity would want to steal Eisenduller's files.
	Actor_Says(kActorTyrell,   60, 14); // Tyrell: Please, enlighten me.
	Actor_Face_Actor(kActorMcCoy, kActorTyrell, true);
	Actor_Says(kActorMcCoy,  5440, 14); // McCoy: They want more life.
	Actor_Says(kActorTyrell,   70, 13); // Tyrell: They're wasting their time.
	Actor_Says(kActorMcCoy,  5445, 15); // McCoy: They're gonna come for you. Just like they came for Eisenduller.
	Actor_Says_With_Pause(kActorTyrell, 80, 1.0f, 12);
	Actor_Says(kActorTyrell, 90, 15); // Tyrell: Have I given you what you came here for?
	Actor_Says_With_Pause(kActorMcCoy, 5450, 1.0f, 15);
	Actor_Says(kActorMcCoy,  5455, 12); // McCoy: Yeah, maybe.
	Actor_Says(kActorTyrell,  100, 14); // Tyrell: Then if you'll excuse me I have a stockholder meeting to attend.
	Actor_Clue_Acquire(kActorMcCoy, kClueRachaelInterview, false, kActorRachael);
	Actor_Clue_Acquire(kActorMcCoy, kClueTyrellInterview, false, kActorTyrell);
	Loop_Actor_Walk_To_XYZ(kActorTyrell, -260.15f, 12.0f, -19.16f, 0, false, false, false);
}

} // End of namespace BladeRunner
