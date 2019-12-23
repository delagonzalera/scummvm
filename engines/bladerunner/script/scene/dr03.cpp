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

void SceneScriptDR03::InitializeScene() {
	if (Game_Flag_Query(kFlagDR02toDR03)) {
		Game_Flag_Reset(kFlagDR02toDR03);
		Setup_Scene_Information(330.31f, 4.27f, -910.91f, 297);
	}

	Setup_Scene_Information(330.31f, 4.27f, -910.91f, 297);

	Scene_Exit_Add_2D_Exit(0, 377, 122, 445, 266, 0);

	if (Global_Variable_Query(kVariableChapter) == 3) {
		Actor_Put_In_Set(kActorChew, kSetDR03);
		Actor_Set_At_XYZ(kActorChew, 431.21f, 4.27f, -776.26f, 0);
	} else {
		Actor_Put_In_Set(kActorChew, kSetDR03);
		Actor_Set_At_XYZ(kActorChew, 360.77f, 4.4f, -806.67f, 126);
	}

	Ambient_Sounds_Remove_All_Looping_Sounds(1);
	Ambient_Sounds_Add_Looping_Sound(kSfxRESPRTR1,  7,  0, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxBRBED3,   50,  0, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxBOILPOT2, 20, 70, 1);
}

void SceneScriptDR03::SceneLoaded() {
	Obstacle_Object("W2-CENTCASE02", true);
	Obstacle_Object("W2-CARTTOP", true);
	Obstacle_Object("W2-TANKAFLUID01", true);
}

bool SceneScriptDR03::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptDR03::ClickedOn3DObject(const char *objectName, bool a2) {
	return false;
}

bool SceneScriptDR03::ClickedOnActor(int actorId) {
	if (actorId == kActorChew) {
		Actor_Face_Actor(kActorMcCoy, kActorChew, true);
		Actor_Face_Actor(kActorChew, kActorMcCoy, true);

		if (!Game_Flag_Query(kFlagDR03ChewTalk1)) {
			if (_vm->_cutContent && Random_Query(1, 2) == 1) {
				Actor_Says(kActorMcCoy, 750, 18); // McCoy: Damn, you keep it cold in here.
				Actor_Says(kActorChew,    0, 14); // Chew: Hmph, of course cold. What do you expect, heat? Eyes go bad at room temperature. You buy winter coat. Like this, see, like this. Very comfy.
			} else {
				Actor_Says(kActorMcCoy, 755, 18); // McCoy: Excuse me.
				Actor_Says(kActorChew,   10, 14); // Chew: Deliveries to rear!
			}
			Actor_Says(kActorMcCoy, 760, 18); // McCoy: LPD. I got a couple of questions.
			Actor_Says(kActorChew,   20, 14); // Chew: Hmm. Talk, talk, talk, always talk. You wait. You no take up time. Busy, busy.
			Actor_Says(kActorMcCoy, 765, 18); // McCoy: Marcus Eisenduller is dead.
			Actor_Says(kActorChew,   30, 14); // Chew: Eh-- Okay, we talk.  Quick, quick now. What you want?
			Game_Flag_Set(kFlagDR03ChewTalk1);
			return true;
		}

		if (!Actor_Clue_Query(kActorMcCoy, kClueChewInterview)) {
			Actor_Says(kActorMcCoy, 770, 12); // McCoy: You were close to Eisenduller?
			Actor_Says(kActorChew,  110, 12); // Chew: Eisenduller? Ha, ha, no, no. No time for him. No go to lab.
			Actor_Says(kActorChew,  120, 13); // Chew: Eisenduller test Off-World gravity. Chew work important organs. Visual Center, you see?
			Actor_Says(kActorMcCoy, 835, 13); // McCoy: Marcus is dead.
			Actor_Says(kActorChew,  130, 14); // Chew: Chew work hard all night, all day. No stop, no break. No OT (paid overtime).
			Actor_Says(kActorMcCoy, 840, 16); // McCoy: Did you hear me?
			Actor_Says(kActorChew,  140, 15); // Chew: Of course I hear you! You-- you think I'm deaf? (mumbles) Hmph, your time more important, man?
			if (!Game_Flag_Query(kFlagDR03ChewTalkExplosion)) {
				Actor_Says(kActorChew,  150, 13); // Chew: Eisenduller dead. Moraji maybe dead. You out chasing skirts?
				Actor_Says(kActorMcCoy, 845, 17); // McCoy: Moraji?
				Actor_Says(kActorChew,  170, 18); // Chew: Chew eat supper with Moraji every night, no fail. Sometimes Chinese, sometimes Indian, sometimes home cook.
				Actor_Says(kActorChew,  180, 16); // Chew: Sometimes we go up and eat with twins.
				Actor_Says(kActorMcCoy, 850, 15); // McCoy: Moraji didn't show? Did you call him?
				Actor_Says(kActorChew,  190, 14); // Chew: I call. I knock. Ten minutes ago. Door locked, no answer.
				Actor_Says(kActorChew,  200, 13); // Chew: Where you come from, huh?
				Actor_Says(kActorMcCoy, 855, 18); // McCoy: What?
				Actor_Says(kActorChew,  210, 12); // Chew: Nothing. Nothing.
			}
			Actor_Clue_Acquire(kActorMcCoy, kClueChewInterview, true, kActorChew);
			return true;
		}

		if (Game_Flag_Query(kFlagDR05JustExploded)
		 && Game_Flag_Query(kFlagDR03ChewTalk1)
		) {
			Actor_Says(kActorMcCoy, 815, 18); // McCoy: Listen to me!
			Actor_Says(kActorChew,   60, 14); // Chew: Busy, busy. You not delivery man. You go away!
			Actor_Says(kActorChew,   70, 14); // Chew: (mumbles) Distraction all day long. Never stop.
			Actor_Says(kActorChew,   80, 14); // Chew: Loud noises on the street. Beep, beep, bonk, gonk, gonk. (sigh) Always make me drop eyes.
			Actor_Says(kActorMcCoy, 820, 18); // McCoy: That'd be Dermo Design burning to the ground.
			Actor_Says(kActorChew,   90, 14); // Chew: Oh, what-- what of Moraji?
			Actor_Says(kActorMcCoy, 825, 18); // McCoy: He didn't make it.
			Actor_Says(kActorChew,  100, 14); // Chew: You leave now, okay? Very busy. Must work. Must work, ah.
			Game_Flag_Reset(kFlagDR05JustExploded);
			Game_Flag_Set(kFlagDR03ChewTalkExplosion);
			return true;
		}

		if ((Actor_Clue_Query(kActorMcCoy, kClueDNATyrell)
		  || Actor_Clue_Query(kActorMcCoy, kClueAnsweringMachineMessage)
		  || Actor_Clue_Query(kActorMcCoy, kClueEnvelope)
		  || Actor_Clue_Query(kActorMcCoy, kClueChewInterview)
		 )
		 && Game_Flag_Query(kFlagDR03ChewTalkExplosion)
		) {
			dialogueWithChew();
		} else {
			Actor_Says(kActorMcCoy, 810, 18); // McCoy: Tell me this.
			Actor_Says(kActorChew,   40, 14); // Chew: Hey, LPD, you deaf or something, huh, LPD?
			Actor_Says(kActorChew,   50, 14); // Chew: I very busy. Eyes will wait for nothing.
		}
		return true;
	}

	return false;
}

bool SceneScriptDR03::ClickedOnItem(int itemId, bool a2) {
	return false;
}

bool SceneScriptDR03::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 330.31f, 4.27f, -910.91f, 24, true, false, false)) {
			Game_Flag_Set(kFlagDR03toDR02);
			Set_Enter(kSetDR01_DR02_DR04, kSceneDR02);
		}
		return true;
	}
	return false;
}

bool SceneScriptDR03::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptDR03::SceneFrameAdvanced(int frame) {
	if (frame == 1
	 || frame == 4
	 || frame == 8
	 || frame == 10
	 || frame == 19
	 || frame == 21
	 || frame == 22
	 || frame == 23
	 || frame == 30
	 || frame == 31
	 || frame == 32
	 || frame == 33
	 || frame == 46
	 || frame == 49
	) {
		if (Random_Query(0, 1)) {
			Sound_Play(kSfxNEON7, Random_Query(20, 33), 80, 80, 50);
		} else {
			Sound_Play(kSfxNEON5, Random_Query(5, 6), 80, 80, 50);
		}
	}
}

void SceneScriptDR03::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptDR03::PlayerWalkedIn() {
	if (!Game_Flag_Query(kFlagDR02toDR03)) {
		if (Random_Query(1, 2) == 1) {
			Actor_Says(kActorChew, 660, 14); // Chew: (yells) Ah!
			Actor_Says(kActorChew, 680, 14); // Chew: What you want? I busy.
		} else if (Random_Query(1, 2) == 2) {
			Actor_Says(kActorChew, 670, 14); // Chew: Hmph. (mumbles in Chinese)
			Actor_Says(kActorChew, 620, 14); // Chew: (yells) What you do, huh?
		} else {
			Actor_Says(kActorChew, 690, 14); // Chew: Not good time now, come back later.
			Actor_Says(kActorChew, 710, 14); // Chew: (Mumbles in Chinese)
		}
	}
}

void SceneScriptDR03::PlayerWalkedOut() {
}

void SceneScriptDR03::DialogueQueueFlushed(int a1) {
}

void SceneScriptDR03::dialogueWithChew() {
	Dialogue_Menu_Clear_List();
	if (Actor_Clue_Query(kActorMcCoy, kClueChewInterview)
	 || Actor_Clue_Query(kActorMcCoy, kClueAnsweringMachineMessage)
	 || Actor_Clue_Query(kActorMcCoy, kClueMorajiInterview)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(650, 5, 5, 5); // TWINS
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueChewInterview)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(660, 5, 5, 5); // MORAJI
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueTyrellInterview)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(670, 6, 5, 2); // TYRELL
	}
	if (Game_Flag_Query(kFlagDR03ChewTalkExplosion)) {
		if (Actor_Clue_Query(kActorMcCoy, kClueAnsweringMachineMessage)) {
			DM_Add_To_List_Never_Repeat_Once_Selected(680, 8, 8, 8); // SEBASTIAN
		}
		if (Actor_Clue_Query(kActorMcCoy, kClueEnvelope)) {
			DM_Add_To_List_Never_Repeat_Once_Selected(1270, 2, 5, 7); // LANCE'S ENVELOPE
		}
	}
	Dialogue_Menu_Add_DONE_To_List(690); // DONE

	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();

	switch (answer) {
	case 640: // EISENDULLER
		Actor_Says(kActorMcCoy, 770, 12); // McCoy: You were close to Eisenduller?
		Actor_Says(kActorChew,  110, 12); // Chew: Eisenduller? Ha, ha, no, no. No time for him. No go to lab.
		Actor_Says(kActorChew,  120, 13); // Chew: Eisenduller test Off-World gravity. Chew work important organs. Visual Center, you see?
		Actor_Says(kActorMcCoy, 835, 13); // McCoy: Marcus is dead.
		Actor_Says(kActorChew,  130, 14); // Chew: Chew work hard all night, all day. No stop, no break. No OT (paid overtime).
		Actor_Says(kActorMcCoy, 840, 16); // McCoy: Did you hear me?
		Actor_Says(kActorChew,  140, 15); // Chew: Of course I hear you! You-- you think I'm deaf? (mumbles) Hmph, your time more important, man?
		if (!Game_Flag_Query(kFlagDR03ChewTalkExplosion)) {
			Actor_Says(kActorChew,  150, 13); // Chew: Eisenduller dead. Moraji maybe dead. You out chasing skirts?
			Actor_Says(kActorMcCoy, 845, 17); // McCoy: Moraji?
			Actor_Says(kActorChew,  170, 18); // Chew: Chew eat supper with Moraji every night, no fail. Sometimes Chinese, sometimes Indian, sometimes home cook.
			Actor_Says(kActorChew,  180, 16); // Chew: Sometimes we go up and eat with twins.
			Actor_Says(kActorMcCoy, 850, 15); // McCoy: Moraji didn't show? Did you call him?
			Actor_Says(kActorChew,  190, 14); // Chew: I call. I knock. Ten minutes ago. Door locked, no answer.
			Actor_Says(kActorChew,  200, 13); // Chew: Where you come from, huh?
			Actor_Says(kActorMcCoy, 855, 18); // McCoy: What?
			Actor_Says(kActorChew,  210, 12); // Chew: Nothing. Nothing.
		}
		Actor_Clue_Acquire(kActorMcCoy, kClueChewInterview, true, kActorChew);
		break;

	case 650: // TWINS
		Actor_Says(kActorMcCoy, 775, 11); // McCoy: You mentioned twins.
		Actor_Says(kActorChew,  220, 14); // Chew: Yes, yes. Luther and Lance. Siamese twins. Live above Moraji.
		Actor_Says(kActorMcCoy, 860, 11); // McCoy: They're genetic designers?
		Actor_Says(kActorChew,  230, 14); // Chew: Were. Till last month. Got the boot. Shitcan.
		Actor_Says(kActorMcCoy, 865, 11); // McCoy: From who?
		Actor_Says(kActorChew,  240, 14); // Chew: Big boss, who else? [ speaks Chinese ] You go ask them.
		Actor_Says(kActorChew,  250, 14); // Chew: I waste no more time. You think I nose around everybody's business, huh?
		break;

	case 660: // MORAJI
		Actor_Says(kActorMcCoy, 780, 13); // McCoy: Moraji was some kind of DNA designer?
		if (Game_Flag_Query(kFlagDR03ChewTalkExplosion)) {
			Actor_Says(kActorChew, 260, 14); // Chew: Yeah, subcontractor like me.
			Actor_Says(kActorChew, 270, 13); // Chew: Heh. He funny, though. Like old movies. Moraji Master Chef.
			Actor_Says(kActorChew, 280, 12); // Chew: Twins go crazy when he cooks. (snickers)
		} else {
			Actor_Says(kActorChew,  260, 14); // Chew: Yeah, subcontractor like me.
			Actor_Says(kActorChew,  270, 13); // Chew: Heh. He funny, though. Like old movies. Moraji Master Chef.
			Actor_Says(kActorChew,  280, 12); // Chew: Twins go crazy when he cooks. (snickers)
			Actor_Says(kActorMcCoy, 870, 18); // McCoy: What does he work on?
			Actor_Says(kActorChew,  290, 15); // Chew: Skin. Did skin for Nexus-6. Beautiful work. You go see.
			if (!Game_Flag_Query(kFlagDR05JustExploded)) {
				Actor_Says(kActorChew, 300, 12); // Chew: He at "Dermo Design" across street. You find him, you tell him I wait.
			}
		}
		Actor_Clue_Acquire(kActorMcCoy, kClueChewInterview, true, kActorChew);
		break;

	case 670: // TYRELL
		Actor_Says(kActorMcCoy, 765, 12); // McCoy: Marcus Eisenduller is dead.
		if (_vm->_cutContent) {
			Actor_Says_With_Pause(kActorMcCoy, 785, 0.80f, 18);
		}
		Actor_Says(kActorMcCoy, 790, 13); // McCoy: You might consider knocking off work early.
		Actor_Says(kActorChew,  310, 12); // Chew: What Nexus-6 want with me, huh? I just do eyes.
		Actor_Says(kActorChew,  320,  3); // Chew: Busy, busy. Must make deadline. (sighs)
		break;

	case 680: // SEBASTIAN
		Actor_Says(kActorMcCoy, 795, 3); // McCoy: What's Sebastian been up to lately?
		if (Game_Flag_Query(kFlagDR03ChewTalkExplosion)) {
			Actor_Says(kActorChew,  330, 12); // Chew: Sebastian? He do what always do. Nervous System.
			Actor_Says(kActorChew,  340, 15); // Chew: Very important to Dr. Tyrell.
			Actor_Says(kActorMcCoy, 875, 16); // McCoy: Where do I find him?
			Actor_Says(kActorChew,  350, 12); // Chew: He at old building down alley. Er...
			Actor_Says(kActorChew,  360, 15); // Chew: Bradbury Hotel, I think. You walk down alley, you find it.
			Game_Flag_Set(kFlagBB01Available);
		} else {
			Actor_Says(kActorChew, 320, 13); // Chew: Busy, busy. Must make deadline. (sighs)
			Actor_Says(kActorChew, 150, 14); // Chew: Eisenduller dead. Moraji maybe dead. You out chasing skirts?
			Game_Flag_Set(kFlagBB01Available);
		}
		break;

	case 690: // DONE
		Actor_Says(kActorMcCoy, 805, 3); // McCoy: Sorry to bother you.
		break;

	case 1270: // LANCE'S ENVELOPE
		Actor_Says(kActorMcCoy, 800, 16); // McCoy: What about this Lance? What kind of guy is he?
		Actor_Says(kActorChew,  370,  3); // Chew: Why, he like Luther. Only different. Don't know which which.
		Actor_Says(kActorMcCoy, 880, 15); // McCoy: That so?
		Actor_Says(kActorChew,  380, 13); // Chew: One long hair, talk a lot, tah tah tah tah tah tah, always make deal.
		Actor_Says(kActorChew,  390, 12); // Chew: Other more quiet. Work much harder.
		Actor_Says(kActorMcCoy, 885, 14); // McCoy: What kind of deals?
		Actor_Says(kActorChew,  400, 13); // Chew: Sell this, sell that. No matter what weather.
		Actor_Says(kActorChew,  410, 15); // Chew: Like salesmen. Ha. Very hard to take. No trust, no way.
		Actor_Says(kActorMcCoy, 890, 18); // McCoy: Did Lance sell something to Runciter?
		Actor_Says(kActorChew,  420, 13); // Chew: Who? Never heard of Runciter.
		Actor_Says(kActorChew,  430, 12); // Chew: No time for charades. Sound like this, sound like that. Look like this. (mumbles)
		break;
	}
}

} // End of namespace BladeRunner
