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

enum kRC02Exits {
	kRC02ExitRC01 = 0,
	kRC02ExitRC51 = 1
};

void SceneScriptRC02::InitializeScene() {
	if (Game_Flag_Query(kFlagRC01toRC02)) {
		Setup_Scene_Information(-103.0f, -1238.89f, 108603.04f, 1007);
	} else {
		Setup_Scene_Information(-20.2f, -1238.89f, 108100.73f, 539);
	}
	Scene_Exit_Add_2D_Exit(kRC02ExitRC01, 0, 460, 639, 479, 2);
	if (Game_Flag_Query(kFlagRC51Available)) {
		Scene_Exit_Add_2D_Exit(kRC02ExitRC51, 265, 58, 346, 154, 0);
	}
	if (_vm->_cutContent
	    && Global_Variable_Query(kVariableChapter) == 1
	    && !Game_Flag_Query(kFlagRC02McCoyCommentsOnVideoScreens)) {
		Scene_2D_Region_Add(0, 187, 104, 235, 150);// broken screen
	}
	Ambient_Sounds_Remove_All_Non_Looping_Sounds(false);
	Ambient_Sounds_Add_Looping_Sound(kSfxBRBED5,   50,   1, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxWINDLOP8, 75,   1, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxCTRUNOFF, 30, 100, 1);
	Ambient_Sounds_Add_Sound(kSfxSWEEP3,   5, 20, 10, 10, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxSWEEP4,   5, 20, 10, 10, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxPETDEAD1, 5, 40,  6,  6, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxPETDEAD3, 5, 40,  6,  6, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxPETDEAD4, 5, 40,  6,  6, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxPETDEAD5, 5, 40,  6,  6, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Speech_Sound(kActorOfficerLeary,    250, 10, 60, 5, 5, 100, 100, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorOfficerLeary,    330, 10, 60, 5, 5, 100, 100, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorOfficerGrayford, 380, 10, 60, 5, 5, 100, 100, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorOfficerGrayford, 510, 10, 60, 5, 5, 100, 100, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorDispatcher,       80, 10, 60, 5, 5, 100, 100, -101, -101, 1, 1);
	Ambient_Sounds_Add_Speech_Sound(kActorDispatcher,      160, 10, 60, 5, 5, 100, 100, -101, -101, 1, 1);
	Ambient_Sounds_Add_Sound(kSfxSIREN2,  20, 80, 10, 20,  100, 100, -101, -101, 0, 0);
}

void SceneScriptRC02::SceneLoaded() {
	Obstacle_Object("TABLETOP", true);
	Obstacle_Object("DRAPE01", true);
	Obstacle_Object("DRAPE03", true);
	Obstacle_Object("DRAPE04", true);
	Obstacle_Object("DRAPE05", true);
	Obstacle_Object("DRAPE06", true);
	Obstacle_Object("DRAPE07", true);
	Obstacle_Object("OUTR_DESK", true);
	Obstacle_Object("CAGE_BASE", true);
	Obstacle_Object("POLE_ROP01", true);
	Unobstacle_Object("LEGS", true);
	Unobstacle_Object("SLATS01", true);
	Unobstacle_Object("DRAPE07", true);
	if (_vm->_cutContent) {
		// improvement: broaden path for Runciter to his desk
		Unobstacle_Object("GRD ROPE04", true);
	}
	Clickable_Object("SCRTY CA03");
	Unclickable_Object("GRL_DSKLEG");
	Unclickable_Object("CURTAIN");
	if (_vm->_cutContent) {
		if (Global_Variable_Query(kVariableChapter) == 1
		    && !Game_Flag_Query(kFlagMcCoyCommentsOnMurderedAnimals)
		    && !Actor_Clue_Query(kActorMcCoy, kClueLabCorpses)
		) {
			Clickable_Object("DRAPE01");
			Clickable_Object("DRAPE02");
			Clickable_Object("DRAPE03");
			Clickable_Object("DRAPE04");
			Clickable_Object("DRAPE05");
			Clickable_Object("DRAPE06");
			Clickable_Object("DRAPE07");
		} else {
			Unclickable_Object("DRAPE01");
			Unclickable_Object("DRAPE02");
			Unclickable_Object("DRAPE03");
			Unclickable_Object("DRAPE04");
			Unclickable_Object("DRAPE05");
			Unclickable_Object("DRAPE06");
			Unclickable_Object("DRAPE07");
		}
	} else {
		// original code
		Unclickable_Object("DRAPE01");
		Unclickable_Object("DRAPE02");
		Unclickable_Object("DRAPE03");
		Unclickable_Object("DRAPE05");
		Unclickable_Object("DRAPE06");
		Unclickable_Object("DRAPE07");
	}

	if (Actor_Clue_Query(kActorMcCoy, kClueRuncitersVideo) || Global_Variable_Query(kVariableChapter) > 1) {
		Unclickable_Object("SCRTY CA03");
	}
	if (!Game_Flag_Query(kFlagRC02ShellCasingsTaken)) {
		Item_Add_To_World(kItemShellCasingA, kModelAnimationShellCasings, kSetRC02_RC51, -52.88f, -1238.89f, 108467.74f, 256, 6, 6, false, true, false, true);
		Item_Add_To_World(kItemShellCasingB, kModelAnimationShellCasings, kSetRC02_RC51, -37.16f, -1238.89f, 108456.59f, 512, 6, 6, false, true, false, true);
		Item_Add_To_World(kItemShellCasingC, kModelAnimationShellCasings, kSetRC02_RC51, -62.86f, -1238.89f, 108437.52f, 625, 6, 6, false, true, false, true);
	}
}

bool SceneScriptRC02::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptRC02::ClickedOn3DObject(const char *objectName, bool a2) {
	if ( Object_Query_Click("SCRTY CA03", objectName)
	 && !Actor_Clue_Query(kActorMcCoy, kClueRuncitersVideo)
	) {
		if (Actor_Clue_Query(kActorMcCoy, kClueRunciterInterviewA)
		 && Actor_Query_Is_In_Current_Set(kActorRunciter)
		) {
			AI_Movement_Track_Pause(kActorRunciter);
			Actor_Face_Actor(kActorMcCoy, kActorRunciter, true);
			Actor_Says(kActorMcCoy, 4545, 14); // McCoy: You got any discs from that camera?
			Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
			Actor_Says(kActorRunciter,    0, 14); // Runciter: It's irrelevant.
			Actor_Says(kActorRunciter,   10, 16); // Runciter: Those murderers also destroyed the camera. Thousands of chinyens' worth.
			Actor_Says(kActorMcCoy,    4550, 13); // McCoy: Maybe it photographed them, before they shot it out.
			Actor_Says(kActorRunciter,   20, 13); // Runciter: Hmm, I guess you've got a point at that.
			Loop_Actor_Walk_To_Waypoint(kActorRunciter, 89, 0, false, false);
			Actor_Face_Actor(kActorMcCoy, kActorRunciter, true);
			Loop_Actor_Walk_To_Waypoint(kActorRunciter, 102, 0, false, false);
			Actor_Face_Actor(kActorMcCoy, kActorRunciter, true);
			Actor_Face_Heading(kActorRunciter, 539, false);
			Delay(2000);
			Loop_Actor_Walk_To_Waypoint(kActorRunciter, 89, 0, false, false);
			Loop_Actor_Walk_To_Actor(kActorRunciter, kActorMcCoy, 24, 0, false);
			Item_Pickup_Spin_Effect(kModelAnimationVideoDisc, 357, 228);
			Actor_Face_Actor(kActorMcCoy, kActorRunciter, true);
			Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
			Actor_Says(kActorRunciter,   30, 23); // Runciter: I believe this is the disc from last night.
			Actor_Says(kActorMcCoy,    4555, 18); // McCoy: Peachy.
			Actor_Clue_Acquire(kActorMcCoy, kClueRuncitersVideo, true, kActorRunciter);
			Unclickable_Object("SCRTY CA03");
			AI_Movement_Track_Unpause(kActorRunciter);
			return true;
		} else {
			Actor_Face_Object(kActorMcCoy, "SCRTY CA03", true);
			Actor_Voice_Over(2000, kActorVoiceOver); // Mainframe: Security camera. It'd be a banner day, if it recorded anything before it was shot out.
			return true;
		}
	}

	if (_vm->_cutContent
	    && Global_Variable_Query(kVariableChapter) == 1
	    && !Game_Flag_Query(kFlagMcCoyCommentsOnMurderedAnimals)
	    && !Actor_Clue_Query(kActorMcCoy, kClueLabCorpses)
	    && (Object_Query_Click("DRAPE01", objectName)
	        || Object_Query_Click("DRAPE02", objectName)
	        || Object_Query_Click("DRAPE03", objectName)
	        || Object_Query_Click("DRAPE04", objectName)
	        || Object_Query_Click("DRAPE05", objectName)
	        || Object_Query_Click("DRAPE06", objectName)
	        || Object_Query_Click("DRAPE07", objectName))
	) {
		if (Player_Query_Agenda() == kPlayerAgendaSurly
		    || (Player_Query_Agenda() == kPlayerAgendaErratic && Random_Query(0, 1) == 1)
		) {
			Actor_Voice_Over(1940, kActorVoiceOver); // Mainframe: I'd never seen so many authentic animals under the same roof. It was beautiful... and horrendous.
		} else {
			Actor_Voice_Over(9010, kActorMcCoy); // McCoy: I'd never seen dead animals before. And I never wanted to again.
			Actor_Voice_Over(9015, kActorMcCoy); // McCoy: I didn't want to look too closely.
			Actor_Voice_Over(9020, kActorMcCoy); // McCoy: The boys back at the lab would tell me all I needed to know.
		}
		Game_Flag_Set(kFlagMcCoyCommentsOnMurderedAnimals);
		Unclickable_Object("DRAPE01");
		Unclickable_Object("DRAPE02");
		Unclickable_Object("DRAPE03");
		Unclickable_Object("DRAPE04");
		Unclickable_Object("DRAPE05");
		Unclickable_Object("DRAPE06");
		Unclickable_Object("DRAPE07");
		return true;
	}
	return false;
}

void SceneScriptRC02::dialogueWithRunciter() {
	Dialogue_Menu_Clear_List();
	DM_Add_To_List_Never_Repeat_Once_Selected( 0, 5, 6, 2);     // MOTIVES
	DM_Add_To_List_Never_Repeat_Once_Selected(10, 5, 4, 8);     // LUCY
	if (Actor_Clue_Query(kActorMcCoy, kClueRunciterInterviewB1)
	 || Actor_Clue_Query(kActorMcCoy, kClueRunciterInterviewB2)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(20, 6, 4, 5); // REFERENCE
	}
	if (_vm->_cutContent
	     && (!Game_Flag_Query(kFlagRC02RunciterVKChosen)
	         && (!Actor_Clue_Query(kActorMcCoy, kClueVKRunciterHuman) && !Actor_Clue_Query(kActorMcCoy, kClueVKRunciterReplicant)))
	){
		Dialogue_Menu_Clear_Never_Repeat_Was_Selected_Flag(200);
		DM_Add_To_List_Never_Repeat_Once_Selected(200, -1, 3, 6); // VOIGT-KAMPFF
	}
	Dialogue_Menu_Add_DONE_To_List(30); // DONE

	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();

	switch (answer) {
	case 0: // MOTIVES
		Actor_Says(kActorMcCoy, 4580, 13); // McCoy: Can you think of anything they might have been after?
		Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
		Actor_Says(kActorRunciter,  110, 18); // Runciter: First I thought they wanted money but I don't keep any in the shop.
		Actor_Says(kActorRunciter,  120, 17); // Runciter: Then I thought they wanted to steal my animals but they started...
		Actor_Says(kActorRunciter,  130, 19); // Runciter: executing them.
		Actor_Says(kActorMcCoy,    4605, 13); // McCoy: Could they have been Replicants?
		Actor_Says(kActorRunciter,  140, 16); // Runciter: Who else would be capable of such barbaric acts?
		Game_Flag_Set(kFlagRC02RunciterTalk1);
		break;

	case 10: // LUCY
		Actor_Says(kActorMcCoy, 4585, 13); // McCoy: Tell me about this Lucy.
		Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
		if (Game_Flag_Query(kFlagLucyIsReplicant)) {
			Actor_Says(kActorRunciter, 250, 13); // Runciter: There's not a lot I can tell you. She's only worked for me for about a month.
			Actor_Says(kActorRunciter, 270, 13); // Runciter: She's about fourteen years old with pink hair. A very attractive young thing.
			Actor_Clue_Acquire(kActorMcCoy, kClueRunciterInterviewB1, true, kActorRunciter);
		} else {
			Actor_Says(kActorRunciter, 260, 14); // Runciter: There's not a lot I can tell you. She's only worked for me for a short while.
			Actor_Says(kActorRunciter, 270, 13); // Runciter: She's about fourteen years old with pink hair. A very attractive young thing.
			Actor_Clue_Acquire(kActorMcCoy, kClueRunciterInterviewB2, true, kActorRunciter);
		}
		Actor_Says(kActorMcCoy,    4645, 13); // McCoy: How did you find her?
		Actor_Says(kActorRunciter,  280, 13); // Runciter: She came to me with a reference from Ogilvy's in the North West Zone.
		Actor_Says(kActorRunciter,  290, 13); // Runciter: And he's a very reputable wholesaler.
		Actor_Says(kActorMcCoy,    4650, 18); // McCoy: You treat her well?
		Actor_Says(kActorRunciter,  320, 13); // Runciter: As well as any young tart should be treated.
		Actor_Says(kActorMcCoy,    4665, 13); // McCoy: She got a desk or some place where she worked?
		Actor_Face_Object(kActorRunciter, "CURTAIN", true);
		Actor_Says(kActorRunciter, 350, 13); // Runciter: She used to eat over there.
		Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
		Scene_Exit_Add_2D_Exit(kRC02ExitRC51, 265, 58, 346, 154, 0);
		Game_Flag_Set(kFlagRC51Available);
		break;

	case 20: // REFERENCE
		Actor_Says(kActorMcCoy, 4590, 19); // McCoy: See if you can dig up that reference from Ogilvy's.
		Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
		Actor_Says(kActorRunciter, 360, 13); // Runciter: Yes, it's somewhere here. Just a moment.
		Loop_Actor_Walk_To_Waypoint(kActorRunciter, 89, 0, false, false);
		Loop_Actor_Walk_To_Waypoint(kActorRunciter, 102, 0, false, false);
		Actor_Face_Actor(kActorMcCoy, kActorRunciter, true);
		Actor_Face_Heading(kActorRunciter, 539, false);
		Delay(2000);
		Loop_Actor_Walk_To_Waypoint(kActorRunciter, 89, 0, false, false);
		Actor_Face_Actor(kActorMcCoy, kActorRunciter, true);
		Loop_Actor_Walk_To_Actor(kActorRunciter, kActorMcCoy, 24, false, false);
		Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
		Actor_Face_Actor(kActorMcCoy, kActorRunciter, true);
		Item_Pickup_Spin_Effect(kModelAnimationReferenceLetter, 357, 228);
		Actor_Says(kActorRunciter, 1700, 13); // Runciter: Here you are.
		Actor_Clue_Acquire(kActorMcCoy, kClueReferenceLetter, true, kActorRunciter);
		break;

	case 200:
		if (_vm->_cutContent) { // scene 16 79
			Game_Flag_Set(kFlagRC02RunciterVKChosen);
			Actor_Face_Actor(kActorMcCoy, kActorRunciter, true);
			Actor_Says(kActorMcCoy, 395, 14); // McCoy: There's a test I'd like you to take.
			Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
			Actor_Says(kActorRunciter, 1680, 13); // Runciter: No. I have a lot of cleaning up to do.
			Actor_Says(kActorMcCoy,     400, 14); // McCoy: It won't take too long.
			Voight_Kampff_Activate(kActorRunciter, 20);
			Actor_Modify_Friendliness_To_Other(kActorRunciter, kActorMcCoy, -10);
		}
		break;

	case 30: // DONE
		Actor_Says(kActorMcCoy, 4595, 14); // McCoy: Stick around. I may not be finished with you.
		break;
	}
}

bool SceneScriptRC02::ClickedOnActor(int actorId) {
	if (actorId == kActorRunciter) {
		if (Global_Variable_Query(kVariableChapter) == 4) {
			Actor_Face_Actor(kActorMcCoy, kActorRunciter, true);
			if (Actor_Query_Goal_Number(kActorRunciter) == kGoalRunciterDead) {
				if (_vm->_cutContent) {
					switch (Random_Query(1, 3)) {
					case 1:
						Actor_Says(kActorMcCoy, 8715, 17); // McCoy: Weirdo.
						break;
					case 2:
						Actor_Says(kActorMcCoy, 8720, 17); // McCoy: Freak.
						break;
					case 3:
						Actor_Says(kActorMcCoy, 8725, 17); // McCoy: Idiot.
						break;
					}
				} else {
					if (Random_Query(1, 2) == 1) {
						Actor_Says(kActorMcCoy, 8715, 17); // McCoy: Weirdo.
					} else {
						Actor_Says(kActorMcCoy, 8720, 17); // McCoy: Freak.
					}
				}
				return true;
			}

			if (!Game_Flag_Query(kFlagRC02RunciterTalkWithGun)
			 && !Game_Flag_Query(kFlagRC02RunciterTalk2)
			) {
				Actor_Says(kActorMcCoy, 4690, 11); // McCoy: I wanted to ask you about the Tyrell subcontractors again.
				Actor_Says(kActorMcCoy, 4695, 13); // McCoy: The ones down on DNA Row.
				Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
				Actor_Says(kActorRunciter, 1610, 14); // Runciter: Detective, if I knew something you can be sure I'd tell you.
				if (Actor_Clue_Query(kActorMcCoy, kClueEnvelope)) {
					Actor_Says(kActorMcCoy,    4700, 12); // McCoy: I found an envelope in an apartment down in the Row. From your shop.
					Actor_Says(kActorMcCoy,    4705, 13); // McCoy: Good chunk of chinyen inside. You didn't lose an envelope, did ya?
					Actor_Says(kActorRunciter, 1620, 12); // Runciter: Hmph! I don't think I want to talk to you anymore Mr. McCoy.
					Actor_Says(kActorMcCoy,    4710, 15); // McCoy: Yeah, it was a lot of money. Enough maybe for a... fake animal or two.
					Actor_Says(kActorMcCoy,    4715, 11); // McCoy: Something big even. Like a tiger.
					Delay(2000);
					Actor_Says(kActorMcCoy,    4720, 16); // McCoy: Bone marrow tests are getting cheaper every day.
					Actor_Says(kActorMcCoy,    4725, 17); // McCoy: I'm thinking it'll be worth a little tax payer money to check out those animals of yours.
					Actor_Says(kActorRunciter,  430, 16); // Runciter: Go away!
					Actor_Face_Heading(kActorRunciter, 1007, false);
				}
				Game_Flag_Set(kFlagRC02RunciterTalk2);
				return true;
			}

			Actor_Says(kActorMcCoy, 4805, 11); // McCoy: Listen up.
			Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
			if (Game_Flag_Query(kFlagRC02RunciterTalk2)) {
				Actor_Says(kActorRunciter, 720, 15); // Runciter: I have nothing more to say to you, detective.
			} else {
				Actor_Says(kActorRunciter, 730, 14); // Runciter: Please. Just leave me alone.
			}
			Actor_Face_Heading(kActorRunciter, 1007, false);
			return true;
		}

		AI_Movement_Track_Pause(kActorRunciter);
		Loop_Actor_Walk_To_Actor(kActorMcCoy, kActorRunciter, 48, true, false);
		Actor_Face_Actor(kActorMcCoy, kActorRunciter, true);

		if (!Game_Flag_Query(kFlagRC02RunciterInterview)) {
			Actor_Says(kActorMcCoy, 4560, 13); // McCoy: McCoy, LPD.
			Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
			Actor_Says(kActorRunciter,   40, 16); // Runciter: I'm absolutely certain Lucy Devlin was involved in this heinous act.
			Actor_Says(kActorRunciter,   50, 15); // Runciter: She's the young girl who works for me.
			Actor_Says(kActorMcCoy,    4565, 13); // McCoy: She's responsible for all this mayhem?
			Actor_Says(kActorRunciter,   60, 14); // Runciter: No, of course not! It was two men acting in concert with her, obviously.
			Actor_Says(kActorMcCoy,    4570, 18); // McCoy: Obviously. What did they look like?
			Actor_Says(kActorRunciter,   70, 13); // Runciter: Big and scary and absolutely malevolent.
			Game_Flag_Set(kFlagRC02RunciterInterview);
			Actor_Clue_Acquire(kActorMcCoy, kClueRunciterInterviewA, true, kActorRunciter);
			AI_Movement_Track_Unpause(kActorRunciter);
			return true;
		}

		if (Game_Flag_Query(kFlagRC02RunciterTalk1)) {
			if (Player_Query_Agenda() == kPlayerAgendaPolite) {
				Game_Flag_Reset(kFlagNotUsed0);
				dialogueWithRunciter();
				AI_Movement_Track_Unpause(kActorRunciter);
				return true;
			}

			Actor_Says(kActorMcCoy, 4610, 19); // McCoy: You ever do any business with the Replicant manufacturers? Tyrell Corporation?
			Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
			Actor_Says(kActorRunciter,  150, 15); // Runciter: I resent that accusation.
			Actor_Says(kActorMcCoy,    4615, 13); // McCoy: Just wondering.
			Actor_Says(kActorRunciter,  160, 14); // Runciter: Officer, last May I personally sold Governor Kolvig an exquisite Percheron colt.
			Actor_Says(kActorRunciter,  170, 15); // Runciter: I have been a guest at his house on three separate occasions.
			Actor_Says(kActorRunciter,  180, 13); // Runciter: Do I look like I need to carry artificial product?

			if (Player_Query_Agenda() == kPlayerAgendaSurly) {
				Actor_Says(kActorMcCoy,    4620, 19); // McCoy: So if we do the bone marrow on these animals, there won't be any surprises.
				Actor_Says(kActorRunciter,  190, 14); // Runciter: Don't you have anything better to do with the tax payers' money than perform superfluous autopsies?
				Actor_Says(kActorMcCoy,    4625, 13); // McCoy: Do you know anybody who works at Tyrell Corporation?
				Actor_Says(kActorRunciter,  210, 13); // Runciter: A couple of genetic designers are old friends of mine.
				Actor_Says(kActorMcCoy,    4630, 18); // McCoy: Such as?
				Actor_Says(kActorRunciter,  220, 14); // Runciter: Well, you know how tight security is there.
				Actor_Says(kActorRunciter,  230, 13); // Runciter: It would be a betrayal of trusts to mention names. And they have nothing to do with this.
				Actor_Says(kActorMcCoy,    4635, 19); // McCoy: I assume you're talking about some of those fruitcakes on DNA Row.
				Actor_Says(kActorRunciter,  240, 16); // Runciter: That's a horrible thing to say about people, detective.
				Actor_Says(kActorMcCoy,    4640, 17); // McCoy: Sorry.
			}
			Game_Flag_Reset(kFlagRC02RunciterTalk1);
			AI_Movement_Track_Unpause(kActorRunciter);
			return true;
		}
		dialogueWithRunciter();
		AI_Movement_Track_Unpause(kActorRunciter);
		return true;
	}
	return false;
}

bool SceneScriptRC02::ClickedOnItem(int itemId, bool a2) {
	if (itemId == kItemShellCasingA
	 || itemId == kItemShellCasingB
	 || itemId == kItemShellCasingC
	) {
		if (!Loop_Actor_Walk_To_Item(kActorMcCoy, kItemShellCasingA, 24, true, false)) {
			Actor_Face_Item(kActorMcCoy, kItemShellCasingA, true);
			Actor_Clue_Acquire(kActorMcCoy, kClueShellCasings, true, -1);
			Game_Flag_Set(kFlagRC02ShellCasingsTaken);
			Item_Remove_From_World(kItemShellCasingA);
			Item_Remove_From_World(kItemShellCasingB);
			Item_Remove_From_World(kItemShellCasingC);
			Item_Pickup_Spin_Effect(kModelAnimationShellCasings, 395, 352);
			Actor_Voice_Over(1960, kActorVoiceOver); // Mainframe: Big caliber. Possibly Off-World combat weaponry. Ballistics might give me a fix on it.
		}
		return true;
	}
	return false;
}

bool SceneScriptRC02::ClickedOnExit(int exitId) {
	if (exitId == kRC02ExitRC01) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -71.51f, -1238.89f, 108587.15f, 0, true, false, false)) {
			Game_Flag_Set(kFlagRC02toRC01);
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_Looping_Sound(kSfxBRBED5,   1);
			Ambient_Sounds_Remove_Looping_Sound(kSfxWINDLOP8, 1);
			Ambient_Sounds_Adjust_Looping_Sound(kSfxRCRAIN1, 100, -101, 1);
			Actor_Set_Goal_Number(kActorRunciter, kGoalRunciterDefault);
			Set_Enter(kSetRC01, kSceneRC01);
		}
		return true;
	}

	if (exitId == kRC02ExitRC51) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -20.2f, -1238.73f, 108152.73f, 0, true, false, false)) {
			Async_Actor_Walk_To_XYZ(kActorMcCoy, -8.87f, -1238.89f, 108076.27f, 0, false);
			Set_Enter(kSetRC02_RC51, kSceneRC51);
		}
		return true;
	}
	return false;
}

bool SceneScriptRC02::ClickedOn2DRegion(int region) {

	if (_vm->_cutContent
	    && Global_Variable_Query(kVariableChapter) == 1
	    && !Game_Flag_Query(kFlagRC02McCoyCommentsOnVideoScreens)
	    && region == 0
	) {
		Game_Flag_Set(kFlagRC02McCoyCommentsOnVideoScreens);
		Scene_2D_Region_Remove(0);
		Actor_Voice_Over(9025, kActorMcCoy); // McCoy: Video screens were everywhere in this damn city.
		Actor_Voice_Over(9030, kActorMcCoy); // McCoy: Sometimes I wondered if they were watching us.
		Actor_Voice_Over(9035, kActorMcCoy); // McCoy: I usually just change the channel but to each his own.
		return true;
	}
	return false;
}

void SceneScriptRC02::SceneFrameAdvanced(int frame) {
}

void SceneScriptRC02::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptRC02::PlayerWalkedIn() {
	Player_Set_Combat_Mode(false);
	if (Game_Flag_Query(kFlagRC01toRC02)) {
		Player_Loses_Control();
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, -72.2f, -1238.89f, 108496.73f, 0, false, false, false);
		Player_Gains_Control();
		Game_Flag_Reset(kFlagRC01toRC02);
		if (!Game_Flag_Query(kFlagRC02Entered)) {
			Actor_Voice_Over(1970, kActorVoiceOver); // Mainframe: Judging from the distance between the prints, I was looking for two very big men.
			Actor_Voice_Over(1980, kActorVoiceOver); // Mainframe: One dragged his right foot indicating a possible injury or lameness.
			Actor_Voice_Over(1990, kActorVoiceOver); // Mainframe: The other appeared a lot more controlled, graceful even. Quite a pair.
			Actor_Clue_Acquire(kActorMcCoy, kClueLimpingFootprints, true, -1);
			Actor_Clue_Acquire(kActorMcCoy, kClueGracefulFootprints, true, -1);
			Game_Flag_Set(kFlagRC02Entered);
		}
		if (Actor_Query_Which_Set_In(kActorRunciter) == kSetRC02_RC51
		 && Actor_Query_Goal_Number(kActorRunciter) < kGoalRunciterRC02Wait
		) {
			Actor_Set_Goal_Number(kActorRunciter, kGoalRunciterRC02WalkAround);
		}
		if ( Actor_Query_Goal_Number(kActorRunciter) == kGoalRunciterRC02Wait
		 && !Game_Flag_Query(kFlagRC02EnteredChapter4)
		) {
			Actor_Face_Actor(kActorRunciter, kActorMcCoy, true);
			Actor_Says(kActorRunciter, 370, 12); // Runciter: Oh, it's you.
			Actor_Says(kActorRunciter, 380, 14); // Runciter: Did you find the girl? Lucy.
			Actor_Face_Actor(kActorMcCoy, kActorRunciter, true);
			Actor_Says(kActorMcCoy,    4670, 15); // McCoy: I've met her, yes.
			Actor_Says(kActorRunciter,  390, 13); // Runciter: Really? Is she in custody? I'll be happy to come down to the station and identify her.
			Actor_Says(kActorMcCoy,    4675, 14); // McCoy: That won't be necessary.
			Actor_Face_Heading(kActorRunciter, 1007, false);
			Actor_Says(kActorRunciter, 400, 13); // Runciter: All right. I just wanted to ask her why. Why she would do such a thing.
			Actor_Says(kActorRunciter, 410, 12); // Runciter: My precious ones are gone. I cared for them. All of them.
			Game_Flag_Set(kFlagRC02EnteredChapter4);
		}
	} else {
		Player_Loses_Control();
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, -20.2f, -1238.89f, 108152.73f, 0, false, false, false);
		Player_Gains_Control();
	}
}

void SceneScriptRC02::PlayerWalkedOut() {
}

void SceneScriptRC02::DialogueQueueFlushed(int a1) {
}

} // End of namespace BladeRunner
