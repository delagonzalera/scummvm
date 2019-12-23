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

enum kPS04Loops {
	kPS04LoopPanToPS04                 = 0, //   0 -  29
	kPS04LoopMainLoop                  = 1  //  30 -  90 (actually 31-90)
};

void SceneScriptPS04::InitializeScene() {
	AI_Movement_Track_Pause(kActorGuzza);
	if (Game_Flag_Query(kFlagPS03toPS04)) {
		Game_Flag_Reset(kFlagPS03toPS04);
	}
	Setup_Scene_Information(-668.0f, -354.0f, 974.0f, 475);
	if (Global_Variable_Query(kVariableChapter) == 1) {
		Actor_Put_In_Set(kActorGuzza, kSetPS04);
		Actor_Set_At_XYZ(kActorGuzza, -728.0f, -354.0f, 1090.0f, 150);
		Actor_Change_Animation_Mode(kActorGuzza, 53);
	}
	Scene_Exit_Add_2D_Exit(0, 347, 113, 469, 302, 0);
	Ambient_Sounds_Remove_All_Non_Looping_Sounds(false);
	Ambient_Sounds_Add_Looping_Sound(kSfxPSAMB6, 16, 1, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxRTONE3, 50, 1, 1);
	Ambient_Sounds_Add_Sound(kSfxSCANNER1, 9, 40, 20, 20, 0, 0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSCANNER2, 9, 40, 20, 20, 0, 0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSCANNER3, 9, 40, 20, 20, 0, 0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSCANNER4, 9, 40, 20, 20, 0, 0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSCANNER5, 9, 40, 20, 20, 0, 0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSCANNER6, 9, 40, 20, 20, 0, 0, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSCANNER7, 9, 40, 20, 20, 0, 0, -101, -101, 0, 0);

	Scene_Loop_Start_Special(kSceneLoopModeLoseControl, kPS04LoopPanToPS04, false);
	Scene_Loop_Set_Default(kPS04LoopMainLoop);
}

void SceneScriptPS04::SceneLoaded() {
	Obstacle_Object("CHAIR07", true);
	Unobstacle_Object("GOOD B.WALL", true);
	Unobstacle_Object("B.DOOR", true);
	Unobstacle_Object("B.CHAIR01", true);
	Unclickable_Object("CHAIR07");
#if BLADERUNNER_ORIGINAL_BUGS
#else
	Unclickable_Object("FLOOR");
#endif // BLADERUNNER_ORIGINAL_BUGS

	if ( Global_Variable_Query(kVariableChapter) == 2
	 && !Actor_Clue_Query(kActorMcCoy, kClueWeaponsOrderForm)
	 && !Game_Flag_Query(kFlagPS04WeaponsOrderForm)
	) {
		Item_Add_To_World(kItemWeaponsOrderForm, kModelAnimationOriginalRequisitionForm, kSetPS04, -643.5f, -318.82f, 1148.87f, 525, 16, 12, false, true, false, true);
		Game_Flag_Set(kFlagPS04WeaponsOrderForm);
	}

	if (Actor_Query_Is_In_Current_Set(kActorGuzza)) {
		Actor_Change_Animation_Mode(kActorGuzza, 53);
	}
}

bool SceneScriptPS04::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptPS04::ClickedOn3DObject(const char *objectName, bool a2) {
	return false;
}

bool SceneScriptPS04::ClickedOnActor(int actorId) {
	if (actorId == kActorGuzza) {
		if (!Loop_Actor_Walk_To_Actor(kActorMcCoy, kActorGuzza, 36, true, false)) {
			Actor_Face_Actor(kActorMcCoy, kActorGuzza, true);
			Actor_Face_Actor(kActorGuzza, kActorMcCoy, true);
			dialogueWithGuzza();
			return true;
		}
	}
	return false;
}

bool SceneScriptPS04::ClickedOnItem(int itemId, bool a2) {
	if (itemId == kItemWeaponsOrderForm
	 && Actor_Query_Is_In_Current_Set(kActorGuzza)
	) {
		Actor_Says(kActorGuzza, 560, 30); // Guzza: Get outta here before I throw my desk at you.
	} else if (!Actor_Clue_Query(kActorMcCoy, kClueWeaponsOrderForm)) {
		Item_Remove_From_World(kItemWeaponsOrderForm);
		Item_Pickup_Spin_Effect(kModelAnimationOriginalRequisitionForm, 464, 362);
		Actor_Says(kActorMcCoy, 4485, kAnimationModeTalk); // McCoy: We get a new weapons' shipment?
#if BLADERUNNER_ORIGINAL_BUGS
		Actor_Clue_Acquire(kActorMcCoy, kClueWeaponsOrderForm, true, kActorMcCoy); // A bug? Shouldn't the last argument be -1 here?
#else
		Actor_Clue_Acquire(kActorMcCoy, kClueWeaponsOrderForm, true, -1);
#endif // BLADERUNNER_ORIGINAL_BUGS
	}
	return false;
}

bool SceneScriptPS04::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -668.0f, -350.85f, 962.0f, 0, true, false, false)) {
			Game_Flag_Set(kFlagPS04toPS03);
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Set_Enter(kSetPS03, kScenePS03);
		}
		return true;
	}
	return false;
}

bool SceneScriptPS04::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptPS04::SceneFrameAdvanced(int frame) {
}

void SceneScriptPS04::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptPS04::PlayerWalkedIn() {
	if (Actor_Query_Which_Set_In(kActorGuzza) == kSetPS04) {
		Actor_Face_Actor(kActorMcCoy, kActorGuzza, true);
	}
	//return false;
}

void SceneScriptPS04::PlayerWalkedOut() {
	AI_Movement_Track_Unpause(kActorGuzza);
}

void SceneScriptPS04::DialogueQueueFlushed(int a1) {
}

void SceneScriptPS04::dialogueWithGuzza() {
	Dialogue_Menu_Clear_List();
	if (Global_Variable_Query(kVariableChapter) > 1) {
		if (Actor_Clue_Query(kActorMcCoy, kClueAttemptedFileAccess)) {
			DM_Add_To_List_Never_Repeat_Once_Selected(110, 5, 7, 4); // REQUEST TYRELL MEETING
		}
		DM_Add_To_List_Never_Repeat_Once_Selected(120, 1, -1, -1); // MONEY
		if (Actor_Clue_Query(kActorMcCoy, kClueHoldensBadge)) {
			DM_Add_To_List_Never_Repeat_Once_Selected(150, 7, 6, 5); // HOLDEN'S BADGE
		}
	}
	if (Game_Flag_Query(kFlagCT04HomelessKilledByMcCoy)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(140, 3, -1, -1); // CONFESS TO SHOOTING
	}
	DM_Add_To_List(130, 1, 1, 1); // REPORT IN
	Dialogue_Menu_Add_DONE_To_List(160); // DONE

	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();

	switch (answer) {
	case 110: // REQUEST TYRELL MEETING
		if (_vm->_cutContent) {
			Actor_Says(kActorGuzza, 420, 31); // Guzza: Keeping out of trouble, kid?
		}
		Actor_Says(kActorMcCoy, 3990, 19); // McCoy: The goons over at the Tyrell Building wouldn't let me talk to their boss.
		Actor_Says(kActorMcCoy, 3995, 17); // McCoy: Maybe you can set up a meeting.
		if (_vm->_cutContent) {
			Actor_Says(kActorGuzza,  430, 33); // Guzza: Tyrell? What the hell for?
			Actor_Says(kActorMcCoy, 4025, 18); // McCoy: One of his employees got his brains splattered inside the pyramid.
			Actor_Says(kActorMcCoy, 4030, 12); // McCoy: I thought maybe Tyrell could fill in some blanks.
		}
		Actor_Says(kActorGuzza,  440, 31); // Guzza: Anyone else in the whole company who knows anything?
		Actor_Says(kActorMcCoy, 4035, 13); // McCoy: You and I both know it's a one-man show over there.
		Actor_Says(kActorGuzza,  450, 34); // Guzza: Yeah, I'll give it a try, kid. And I'll have to pull some strings, so don't go in there half-assed.
		Actor_Says(kActorGuzza,  460, 33); // Guzza: Make sure you got some legit questions for the resident genius.
		Actor_Says(kActorMcCoy, 4040, 17); // McCoy: Appreciate it, Lieutenant.
		Game_Flag_Set(kFlagTB07TyrellMeeting);
		break;

	case 120: // MONEY
		Actor_Says(kActorMcCoy, 4000, 18); // McCoy: Can the department float me some chinyen? This investigation's getting expensive.
		if (_vm->_cutContent) {
			// Using cut content we have two cases:
			// 1. Guzza can accept the loan (as in ORIGINAL)
			// 2. Guzza can refuse the loan (CUT)
			// Basically, if McCoy hasn't retired Zuben or if he drunk away his money at the bar
			// then he'll have a small amount of chinyen and Guzza should accept the loan
			if (Global_Variable_Query(kVariableChinyen) <= 100) {
				Actor_Clue_Acquire(kActorMcCoy, kClueGuzzasCash, true, kActorGuzza);
				Actor_Says(kActorGuzza,  520, 33); // Guzza: Look, this ain't on the department tab, okay? This is a personal loan.
				Actor_Says(kActorMcCoy, 4055, 13); // McCoy: Thanks, Lieutenant.
				Actor_Says(kActorGuzza,  530, 31); // Guzza: This goddamn city doesn't know how to manage funds. They've been flush for years.
				Actor_Says(kActorMcCoy, 4060, 13); // McCoy: I thought we were under budget.
				Actor_Says(kActorGuzza,  540, 31); // Guzza: Trust me. Ain't nobody getting rich unless they're sneaking some on the side.
				Actor_Says(kActorGuzza,  550, 32); // Guzza: Hell, I can barely pay for the fuel for my car.
				Actor_Says(kActorMcCoy, 4065, 18); // McCoy: Ever hear of public transportation?
				Actor_Says(kActorGuzza,  560, 34); // Guzza: Get outta here before I throw my desk at you.
				if (Query_Difficulty_Level() != kGameDifficultyEasy) {
					Global_Variable_Increment(kVariableChinyen, 100);
				}
			} else {
				// McCoy has plenty cash already - Guzza denies the loan
				Actor_Says(kActorGuzza,  470, 33); // Guzza: Hey, I'd love to be your own personal ATM but the department's strapped right now.
				Actor_Says(kActorGuzza,  480, 31); // Guzza: We're still paying Holden even though he's in the hospital.
				Actor_Says(kActorGuzza,  490, 31); // Guzza: And one of the old-timers is freelancing a contract job.
				Actor_Says(kActorGuzza,  500, 32); // Guzza: The city can't be paying for your cocktails kid.
				Actor_Says(kActorMcCoy, 4045, 16); // McCoy: Okay, okay. Sorry I asked!
				Actor_Says(kActorGuzza,  510, 31); // Guzza: Hey, you track down a Rep, you get an advance.
				Actor_Says(kActorMcCoy, 4050, 18); // McCoy: Gee, wish I'd thought of that.
			}
		} else {
			Actor_Clue_Acquire(kActorMcCoy, kClueGuzzasCash, true, kActorGuzza);
			Actor_Says(kActorGuzza,  520, 33); // Guzza: Look, this ain't on the department tab, okay? This is a personal loan.
			Actor_Says(kActorMcCoy, 4055, 13); // McCoy: Thanks, Lieutenant.
			Actor_Says(kActorGuzza,  530, 31); // Guzza: This goddamn city doesn't know how to manage funds. They've been flush for years.
			Actor_Says(kActorMcCoy, 4060, 13); // McCoy: I thought we were under budget.
			Actor_Says(kActorGuzza,  540, 31); // Guzza: Trust me. Ain't nobody getting rich unless they're sneaking some on the side.
			Actor_Says(kActorGuzza,  550, 32); // Guzza: Hell, I can barely pay for the fuel for my car.
			Actor_Says(kActorMcCoy, 4065, 18); // McCoy: Ever hear of public transportation?
			Actor_Says(kActorGuzza,  560, 34); // Guzza: Get outta here before I throw my desk at you.
			if (Query_Difficulty_Level() != kGameDifficultyEasy) {
				Global_Variable_Increment(kVariableChinyen, 100);
			}
		}
		break;

	case 130: // REPORT IN
		if ( Game_Flag_Query(kFlagZubenRetired)
		 && !Game_Flag_Query(kFlagPS04GuzzaTalkZubenRetired)
		) {
			Actor_Says(kActorMcCoy, 3920, 13); // McCoy: Bryant go on permanent leave and you get full use of the office, Lieutenant?
			Actor_Says(kActorGuzza,  140, 30); // Guzza: You'd be amazed at how accommodating a man can be after his hemorrhoids get lanced.
			Actor_Face_Current_Camera(kActorGuzza, true);
			Actor_Says(kActorGuzza,  150, 31); // Guzza: You really been surprising the troops around here kid.
			Actor_Says(kActorGuzza,  160, 32); // Guzza: Gaff says you worked that retirement like a champ.
			Actor_Says(kActorMcCoy, 3925, 18); // McCoy: Just doing the job.
			Actor_Face_Actor(kActorGuzza, kActorMcCoy, true);
			Actor_Says(kActorGuzza, 170, 33); // Guzza: Yeah, well don't get too cocky. Those Sixes can be damn unpredictable.
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, -716.0f, -354.85f, 1042.0f, 0, false, false, false);
			Actor_Face_Actor(kActorMcCoy, kActorGuzza, true);
			Actor_Says(kActorMcCoy, 3930, 13); // McCoy: That I know.
			Actor_Face_Actor(kActorGuzza, kActorMcCoy, true);
#if BLADERUNNER_ORIGINAL_BUGS
			Actor_Says(kActorGuzza,  180, 34); // Guzza: But I'm proud of you McCoy. Why don't you take the rest of the day off?
			Actor_Says(kActorMcCoy, 3935, 13); // McCoy: Thanks.
#else
			if (Global_Variable_Query(kVariableChapter) == 1) { // only play this dialogue (about day off) on day one. It doesn't fit in the next days
				Actor_Says(kActorGuzza,  180, 34); // Guzza: But I'm proud of you McCoy. Why don't you take the rest of the day off?
				Actor_Says(kActorMcCoy, 3935, 13); // McCoy: Thanks.
			}
#endif // BLADERUNNER_ORIGINAL_BUGS
			Actor_Says(kActorGuzza,  190, 30); // Guzza: Payments is wiring the advance to your account as we speak.
			Actor_Says(kActorMcCoy, 3940, 16); // McCoy: How much?
			Actor_Says(kActorGuzza,  200, 31); // Guzza: Couple hundred chinyen. Enough to buy a sandwich or two.
			Actor_Says(kActorGuzza,  210, 33); // Guzza: We got to hold back the rest until the lab does the autopsy.
			Actor_Says(kActorGuzza,  220, 34); // Guzza: Paperwork, you know. Shouldn't take more than a couple of days.
			Actor_Says(kActorMcCoy, 3945, 17); // McCoy: So the check's in the mail.
			Actor_Says(kActorGuzza,  230, 32); // Guzza: Hey, don't worry. The boys upstairs want to keep our best and brightest happy.
			Actor_Says(kActorGuzza,  240, 31); // Guzza: Any idea what you want to spend it on?
			Actor_Says(kActorMcCoy, 3950, 13); // McCoy: I was thinking maybe a companion for my animal at home.
			Actor_Says(kActorGuzza,  250, 34); // Guzza: Forget that. Buy yourself something. Something nice.
			Actor_Says(kActorGuzza,  260, 33); // Guzza: Once you start carving up that juicy bacon kid, the taste never goes away.
			Actor_Says(kActorGuzza,  270, 32); // Guzza: The great things in life...? Believe me you'll just be wanting more and more.
			Game_Flag_Set(kFlagPS04GuzzaTalkZubenRetired);
#if BLADERUNNER_ORIGINAL_BUGS
			if (Query_Difficulty_Level() != kGameDifficultyEasy) {
				Global_Variable_Increment(kVariableChinyen, 200);
			}
			Game_Flag_Set(kFlagZubenBountyPaid);
#else
			if (!Game_Flag_Query(kFlagZubenBountyPaid)) { // get retirement money only if haven't been auto-paid at end of Day 1 (sleep trigger)
				if (Query_Difficulty_Level() != kGameDifficultyEasy) {
					Global_Variable_Increment(kVariableChinyen, 200);
				}
				Game_Flag_Set(kFlagZubenBountyPaid); // not a proper bug, but was missing from original code, so the flag would remain in non-consistent state in this case
			}
#endif // BLADERUNNER_ORIGINAL_BUGS
		} else if ( Game_Flag_Query(kFlagZubenSpared)
		        && !Game_Flag_Query(kFlagPS04GuzzaTalkZubenEscaped)
		) {
			Actor_Says(kActorMcCoy, 3955, 13); // McCoy: Bryant on permanent vacation or what, Lieutenant?
			Actor_Says(kActorGuzza,  280, 30); // Guzza: He sends his regards from the hemorrhoid hospital (soft chuckle). What's the good news?
			Actor_Says(kActorMcCoy, 3960, 18); // McCoy: Fresh out but I'm following up some leads.
#if BLADERUNNER_ORIGINAL_BUGS
			Actor_Says(kActorGuzza,  290, 32); // Guzza: Don't push it kid. You look like you're beat anyway.
			Actor_Says(kActorGuzza,  300, 31); // Guzza: Why don't you rest them dogs the rest of the day.
			Actor_Says(kActorMcCoy, 3965, 13); // McCoy: I still got plenty energy.
			Actor_Says(kActorGuzza,  310, 33); // Guzza: That's an order McCoy.
			Actor_Says(kActorGuzza,  320, 34); // Guzza: I'm ordering you to relax.
#else
			if (Global_Variable_Query(kVariableChapter) == 1) { // only play this dialogue (about day off) on day one. It doesn't fit in the next days
				Actor_Says(kActorGuzza,  290, 32); // Guzza: Don't push it kid. You look like you're beat anyway.
				Actor_Says(kActorGuzza,  300, 31); // Guzza: Why don't you rest them dogs the rest of the day.
				Actor_Says(kActorMcCoy, 3965, 13); // McCoy: I still got plenty energy.
				Actor_Says(kActorGuzza,  310, 33); // Guzza: That's an order McCoy.
				Actor_Says(kActorGuzza,  320, 34); // Guzza: I'm ordering you to relax.
			}
#endif // BLADERUNNER_ORIGINAL_BUGS
			Game_Flag_Set(kFlagPS04GuzzaTalkZubenEscaped);
		} else if (
		 (   Actor_Clue_Query(kActorMcCoy, kClueChopstickWrapper)
		  || Actor_Clue_Query(kActorMcCoy, kClueSushiMenu)
		 )
		 &&  Actor_Clue_Query(kActorMcCoy, kClueRunciterInterviewA)
		 &&  Actor_Query_Friendliness_To_Other(kActorGuzza, kActorMcCoy) < 50
		 && !Game_Flag_Query(kFlagPS04GuzzaTalk1)
		) {
			Actor_Says(kActorMcCoy, 3970, 18); // McCoy: Hey.
			Actor_Says(kActorGuzza,  330, 30); // Guzza: What the hell you've been doing out there, McCoy? Rustling up toe jam?
			Actor_Says(kActorGuzza,  340, 32); // Guzza: I need results and fast.
			Actor_Says(kActorMcCoy, 3975, 13); // McCoy: I'm working on it.
			Actor_Says(kActorGuzza,  350, 31); // Guzza: That's not good enough. Bryant's been crawling up my ass all morning.
			Actor_Says(kActorGuzza,  360, 34); // Guzza: If you can't cut it just tell me and I'll reassign the Gaffster or Steele.
			Actor_Says(kActorMcCoy, 3980, 13); // McCoy: I can handle it.
			Actor_Says(kActorGuzza,  370, 33); // Guzza: I don't know if you can, McCoy. I really got my doubts.
			Actor_Says(kActorGuzza,  380, 32); // Guzza: You blow chunks on your first assignment, you can damn near shit-can your future here.
			Actor_Says(kActorGuzza,  390, 31); // Guzza: Not to mention how it makes me look.
			Actor_Says(kActorMcCoy, 3985, 18); // McCoy: I hear ya.
			Actor_Says(kActorGuzza,  400, 34); // Guzza: Get the hell out of here! And show me the promoting you wasn't a fool move.
			Actor_Says(kActorGuzza,  410, 31); // Guzza: Jesus.
			Game_Flag_Set(kFlagPS04GuzzaTalk1);
		} else if (
		 (   Actor_Clue_Query(kActorMcCoy, kClueChopstickWrapper)
		  || Actor_Clue_Query(kActorMcCoy, kClueSushiMenu)
		 )
		 &&  Actor_Clue_Query(kActorMcCoy, kClueRunciterInterviewA)
		 && !Game_Flag_Query(kFlagPS04GuzzaTalk2)
		) {
			Actor_Says(kActorMcCoy, 3920, 13); // McCoy: Bryant go on permanent leave and you get full use of the office, Lieutenant?
			Actor_Says(kActorGuzza,  570, 32); // Guzza: You know the procedure. Dump it all in the mainframe upstairs and I'll read it later.
			Actor_Says(kActorMcCoy, 4070, 13); // McCoy: You got it.
			Game_Flag_Set(kFlagPS04GuzzaTalk2);
		} else if (Actor_Query_Friendliness_To_Other(kActorGuzza, kActorMcCoy) >= 50) {
			Actor_Says(kActorMcCoy, 4020, 13); // McCoy: Sorry to bother you, Lieutenant. I was just checking in.
			Actor_Says(kActorGuzza,  580, 34); // Guzza: Well, get back out there. Those Reps ain't retiring themselves.
			Actor_Says(kActorMcCoy, 4075, 16); // McCoy: Yeah, suicidal targets would make my job a lot easier.
			Actor_Says(kActorGuzza,  590, 33); // Guzza: Everybody's got a job, kid.
		} else {
			Actor_Says(kActorMcCoy, 4020, 18); // McCoy: Sorry to bother you, Lieutenant. I was just checking in.
			Actor_Says(kActorGuzza,  130, 30); // Guzza: Don't got time for you now, McCoy. Hit me later.
			Actor_Face_Current_Camera(kActorGuzza, true);
			Actor_Says(kActorMcCoy, 3915, 13); // McCoy: Right.
		}
		break;

	case 140: // CONFESS TO SHOOTING
		Actor_Says(kActorMcCoy, 4010, 12); // McCoy: Lieutenant, I-- I shot a guy. Down in Chinatown.
		Actor_Says(kActorGuzza,  600, 31); // Guzza: You what? Damn it, wha-- what are you telling me? I don't need this. Not today.
		Actor_Says(kActorMcCoy, 4080, 18); // McCoy: Hey, I'm trying to do the right thing here. I could have left the guy to rot.
		Actor_Says(kActorGuzza,  610, 33); // Guzza: You're gonna owe me on this one, McCoy. Big time.
		Actor_Face_Heading(kActorGuzza, 400, false);
		Actor_Says(kActorGuzza, 620, 32); // Guzza: I'm gonna have to call in some favors.
		if (_vm->_cutContent) {
			// add a fade-out here while Guzza calls-in for favors
			Scene_Loop_Start_Special(kSceneLoopModeOnce, kPS04LoopPanToPS04, true);
			Scene_Loop_Set_Default(kPS04LoopMainLoop);
			Delay(1000);
			Actor_Face_Actor(kActorGuzza, kActorMcCoy, true);
			Delay(1000);
			// if McCoy confesses before the body is dumped, then the body should be found (even if in dumpster)
			if (!Game_Flag_Query(kFlagCT04HomelessBodyThrownAway)) {
				Game_Flag_Set(kFlagCT04HomelessBodyFound);
				// return false;
			}
		}
		Actor_Says(kActorGuzza,  700, 34); // Guzza: So they're calling it a justified shooting.
		Actor_Says(kActorMcCoy, 4100, 13); // McCoy: I kind of thought they might.
		Actor_Says(kActorGuzza,  710, 31); // Guzza: That doesn't mean your ass is out of the fire with me, McCoy.
		Actor_Says(kActorGuzza,  720, 34); // Guzza: You may have put one over on internal affairs but you and I both know the real skinny.
		Actor_Says(kActorMcCoy, 4105, 18); // McCoy: Is that right?
#if BLADERUNNER_ORIGINAL_BUGS
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, -668.0f, -350.85f, 962.0f, 0, false, false, false);
#else
		// enforce stop running (if was running) - McCoy running in Guzza's office in this scene looks bad
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, -668.0f, -350.85f, 962.0f, 0, false, false, true);
#endif // BLADERUNNER_ORIGINAL_BUGS
		Actor_Says(kActorGuzza, 730, 32); // Guzza: You can't just go around waxing civilians.
		Actor_Face_Actor(kActorMcCoy, kActorGuzza, true);
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, -716.0f, -354.85f, 1042.0f, 0, false, false, false);
		Actor_Face_Actor(kActorGuzza, kActorMcCoy, true);
		Actor_Says(kActorGuzza, 740, 31); // Guzza: Look, McCoy. I know how it is on the street.
		Actor_Says(kActorGuzza, 750, 32); // Guzza: It's easy for a Blade Runner to step over the line.
		Actor_Says(kActorGuzza, 760, 33); // Guzza: You do this job right, you got a long career ahead of you. But you screw up again...
		Actor_Face_Actor(kActorMcCoy, kActorGuzza, true);
		Actor_Says(kActorMcCoy, 4110, 13); // McCoy: I can handle it.
		Actor_Says(kActorGuzza,  770, 32); // Guzza: You'd better.
		Actor_Says(kActorGuzza,  780, 31); // Guzza: Get lost we both got work to do.
		break;

	case 150: // HOLDEN'S BADGE
		Actor_Says(kActorMcCoy, 4015, 16); // McCoy: I found Holden's badge.
		Actor_Says(kActorGuzza,  630, 34); // Guzza: Damn. Let me guess, you found it in that hotel down in Chinatown
		Actor_Says(kActorMcCoy, 4085, 19); // McCoy: Yeah, someone from the department had already been through the room.
		Actor_Says(kActorMcCoy, 4090, 18); // McCoy: I guess they just missed it.
		Actor_Says(kActorGuzza,  640, 31); // Guzza: That's the old-timer's case. The one we brought back on contract.
		Actor_Says(kActorGuzza,  650, 32); // Guzza: Bryant's got faith in this guy but I gotta wonder about any son of a bitch who can miss a badge in a room that small.
		Actor_Says(kActorGuzza,  670, 34); // Guzza: This Deckard, he feels too much, you know? He's too far along the curve.
		Actor_Says(kActorMcCoy, 4095, 17); // McCoy: Did he retire the Rep who shot Holden?
		Actor_Says(kActorGuzza,  680, 32); // Guzza: Bryant said he did, yeah. I guess he's getting the job done.
		Actor_Says(kActorGuzza,  690, 31); // Guzza: Holden will be glad to get this back, kid. You've done good.
		break;

	case 160: // DONE
		// nothing here
		break;
	}
}

} // End of namespace BladeRunner
