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

void SceneScriptPS09::InitializeScene() {
	if (Game_Flag_Query(kFlagMcCoyArrested)) {
		Setup_Scene_Information(-410.0f, 0.26f, -200.0f, 512);
	} else {
		Setup_Scene_Information(-559.0f, 0.0f, -85.06f, 250);
	}

	Scene_Exit_Add_2D_Exit(0, 0, 0, 30, 479, 3);

	Ambient_Sounds_Remove_All_Non_Looping_Sounds(false);
	Ambient_Sounds_Add_Looping_Sound(kSfxBRBED5X,  50, 0, 0);
	Ambient_Sounds_Add_Looping_Sound(kSfxPRISAMB1, 30, 0, 0);
	Ambient_Sounds_Add_Looping_Sound(kSfxPRISAMB3, 30, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxPRISSLM1, 15, 60, 7, 10, 100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxPRISSLM2, 25, 60, 7, 10, 100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxPRISSLM3, 25, 60, 7, 10, 100, 100, -101, -101, 0, 0);

	if (!Game_Flag_Query(kFlagGrigorianArrested)) {
		Actor_Put_In_Set(kActorGrigorian, kSetPS09);
		Actor_Set_At_XYZ(kActorGrigorian, -417.88f, 0.0f, -200.74f, 512);
		Game_Flag_Set(kFlagGrigorianArrested);
	}
	if (Game_Flag_Query(kFlagMcCoyArrested)) {
		Actor_Put_In_Set(kActorGrigorian, kSetFreeSlotD);
		Actor_Set_At_XYZ(kActorGrigorian, 0.0f, 0.0f, 0.0f, 512);
	}
	if (Game_Flag_Query(kFlagIzoArrested)) {
		Actor_Put_In_Set(kActorIzo, kSetPS09);
		Actor_Set_At_XYZ(kActorIzo, -476.0f, 0.2f, -225.0f, 518);
	}
	if (Game_Flag_Query(kFlagCrazylegsArrested)) { // cut feature? it is impossible to arrest crazylegs
		Actor_Put_In_Set(kActorCrazylegs, kSetPS09);
		Actor_Set_At_XYZ(kActorCrazylegs, -290.0f, 0.33f, -235.0f, 207);
	}
}

void SceneScriptPS09::SceneLoaded() {
	Obstacle_Object("OFFICE DOOR", true);
	Unobstacle_Object("OFFICE DOOR", true);
	Unclickable_Object("OFFICE DOOR");
}

bool SceneScriptPS09::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptPS09::ClickedOn3DObject(const char *objectName, bool a2) {
	return false;
}

bool SceneScriptPS09::ClickedOnActor(int actorId) {
	if (actorId == kActorGrigorian) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -381.11f, 0.0f, -135.55f, 0, true, false, false)) {
			Actor_Face_Actor(kActorMcCoy, kActorGrigorian, true);
			Actor_Face_Actor(kActorGrigorian, kActorMcCoy, true);

			if (!Game_Flag_Query(kFlagPS09GrigorianTalk1)) {
				Actor_Says(kActorGrigorian,    0, 12); // Grigorian: Are they letting me out?
				Actor_Says(kActorMcCoy,     4235, 18); // McCoy: I'm not the one who makes that decision.
				Actor_Says(kActorGrigorian,   10, 13); // Grigorian: I suppose it's my turn to bear the cross for the cause.
				Game_Flag_Set(kFlagPS09GrigorianTalk1);
				return true;
			}

			if ( Game_Flag_Query(kFlagPS09GrigorianTalk1)
			 && !Game_Flag_Query(kFlagPS09GrigorianTalk2)
			 && !Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewA)
			 && !Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewB1)
			 && !Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewB2)
			) {
				Actor_Says(kActorMcCoy,     4245, 14); // McCoy: Who are you anyway?
				Actor_Says(kActorGrigorian,   20, 14); // Grigorian: Please leave me alone. My migraine's acting up.
				Game_Flag_Set(kFlagPS09GrigorianTalk2);
				return true;
			}

			if (!Game_Flag_Query(kFlagPS09GrigorianDialogue)
			 &&  Game_Flag_Query(kFlagPS09GrigorianTalk1)
			 &&  (Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewA)
			  ||  Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewB1)
			  ||  Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewB2)
			  ||  Actor_Clue_Query(kActorMcCoy, kClueGrigoriansNote)
			 )
			) {
				Game_Flag_Set(kFlagPS09GrigorianDialogue);
				Actor_Says(kActorMcCoy,     4240, 13); // McCoy: You're Spencer Grigorian, right?
				Actor_Says(kActorGrigorian,  550, 15); // Grigorian: No.
				Actor_Says(kActorGrigorian,  480, 16); // Grigorian: I really think my lawyer should be here.
				dialogueWithGrigorian();
				return true;
			}

			if (Game_Flag_Query(kFlagGrigorianDislikeMcCoy)) {
				Actor_Says(kActorMcCoy,     4270, 18); // McCoy: I got some more questions for you.
				Actor_Says(kActorGrigorian,   30, 14); // Grigorian: Your associate, miss Steele, has already taken my statement.
				Actor_Says(kActorGrigorian,   40, 13); // Grigorian: I have nothing more to say, unless I have an attorney present.
				return true;
			}

			if (Game_Flag_Query(kFlagPS09GrigorianDialogue)
			 && Game_Flag_Query(kFlagPS09GrigorianTalk1)
			 && (Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewA)
			  || Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewB1)
			  || Actor_Clue_Query(kActorMcCoy, kClueGrigoriansNote)
			 )
			) {
				dialogueWithGrigorian();
				return true;
			}

			Actor_Says(kActorMcCoy,     4270, 18); // McCoy: I got some more questions for you.
			Actor_Says(kActorGrigorian,   30, 14); // Grigorian: Your associate, miss Steele, has already taken my statement.
			Actor_Says(kActorGrigorian,   40, 13); // Grigorian: I have nothing more to say, unless I have an attorney present.
			return true;
		}
	}

	if (actorId == kActorIzo) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -473.0f, 0.2f, -133.0f, 12, true, false, false)) {
			Actor_Face_Actor(kActorMcCoy, kActorIzo, true);
			Actor_Face_Actor(kActorIzo, kActorMcCoy, true);

			if (!Game_Flag_Query(kFlagPS09IzoTalk1)) {
				Actor_Says(kActorMcCoy, 4200,                 14); // McCoy: You look right at home in there, Izo.
				Actor_Says(kActorIzo,    570, kAnimationModeTalk); // Izo: I've been in much tighter spots than this, McCoy.
				Actor_Says(kActorMcCoy, 4205,                 18); // McCoy: I'll bet. But this is only the beginning.
				Game_Flag_Set(kFlagPS09IzoTalk1);
				return true;
			}

			if ( Game_Flag_Query(kFlagPS09IzoTalk1)
			 && !Game_Flag_Query(kFlagPS09IzoTalk2)
			) {
				Actor_Says(kActorMcCoy, 4210,                 18); // McCoy: You ready to supply me some answers?
				Actor_Says(kActorIzo,    580, kAnimationModeTalk); // Izo: I wish I had something to say.
				Actor_Says(kActorMcCoy, 4215,                 14); // McCoy: I know your hands are dirty, Izo.
				Actor_Says(kActorIzo,    590, kAnimationModeTalk); // Izo: Everybody in this world is trying to better himself. No matter what the terms are.
				Actor_Says(kActorIzo,    600, kAnimationModeTalk); // Izo: If that counts as dirty, so be it.
				Actor_Says(kActorMcCoy, 4220,                 18); // McCoy: The terms of your conviction won't be too clean.
				Actor_Says(kActorIzo,    610, kAnimationModeTalk); // Izo: I'll be through with this place long before you, McCoy.
				Actor_Says(kActorMcCoy, 4225,                 19); // McCoy: Yeah, I'm sure you've been saving up for a rainy day selling all those illegal weapons.
				Actor_Says(kActorIzo,    620, kAnimationModeTalk); // Izo: One never knows when fate will conspire. You must be prepared.
				Actor_Says(kActorMcCoy, 4230,                 14); // McCoy: You're a real trouper, Izo.
				Game_Flag_Set(kFlagPS09IzoTalk2);
				return true;
			}

			Actor_Says(kActorMcCoy, 4200, 13); // McCoy: You look right at home in there, Izo.
			return true;
		}
	}

	if (actorId == kActorCrazylegs) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -295.0f, 0.34f, -193.0f, 12, true, false, false)) {
			Actor_Face_Actor(kActorMcCoy, kActorCrazylegs, true);
			Actor_Face_Actor(kActorCrazylegs, kActorMcCoy, true);

			if (!Game_Flag_Query(kFlagPS09CrazylegsTalk1)) {
				Actor_Says(kActorMcCoy,     4415,                 18); // McCoy: Guess you can't sell a whole lot of cars from down here, Crazy.
				Actor_Says(kActorCrazylegs, 1090, kAnimationModeTalk); // Crazylegs: I'm being railroaded! All I did was conduct business as per usual.
				Actor_Says(kActorMcCoy,     4420,                 18); // McCoy: Contacting business with Reps is against the Law.
				Game_Flag_Set(kFlagPS09CrazylegsTalk1);
			}

			if ( Game_Flag_Query(kFlagPS09CrazylegsTalk1)
			 &&  Game_Flag_Query(kFlagGrigorianArrested)
			 && !Game_Flag_Query(kFlagPS09CrazylegsGrigorianTalk)
			) {
				Actor_Face_Actor(kActorGrigorian, kActorCrazylegs, true);
				Actor_Says(kActorGrigorian, 420, 14); // Grigorian: You don't have to be afraid of the truth, Larry.
				Actor_Face_Actor(kActorCrazylegs, kActorGrigorian, true);
				Actor_Says(kActorCrazylegs, 1120, kAnimationModeTalk); // Crazylegs: I don't know what you're talking about.
				Actor_Face_Actor(kActorMcCoy, kActorGrigorian, true);
				Actor_Says(kActorMcCoy,     4435,                 14); // McCoy: You got a bad case of diarrhea of the mouth, Spencer.
				Actor_Says(kActorGrigorian,  430,                 16); // Grigorian: In that case I'll zip my lips.
				Actor_Says(kActorCrazylegs, 1130, kAnimationModeTalk); // Crazylegs: Damn good idea!
				Game_Flag_Set(kFlagPS09CrazylegsGrigorianTalk);
				return true;
			}

			if ( Game_Flag_Query(kFlagPS09CrazylegsTalk1)
			 && !Game_Flag_Query(kFlagGrigorianArrested)
			 && !Game_Flag_Query(kFlagPS09CrazylegsTalk2)) {
				Actor_Says(kActorMcCoy,     4425,                 18); // McCoy: How about naming some names for me, Crazy.
				Actor_Says(kActorCrazylegs, 1100, kAnimationModeTalk); // Crazylegs: Whatever happened I was ignorant, McCoy.
				Actor_Says(kActorMcCoy,     4430,                 19); // McCoy: I figured you for a lot of things but ignorant wasn't one of them.
				Actor_Says(kActorCrazylegs, 1110, kAnimationModeTalk); // Crazylegs: You'd know what I mean.
				Game_Flag_Set(kFlagPS09CrazylegsTalk2);
				return true;
			}

			Actor_Says(kActorMcCoy,     4425,                 18); // McCoy: How about naming some names for me, Crazy.
			Actor_Says(kActorCrazylegs, 1160, kAnimationModeTalk); // Crazylegs: Take a ride, McCoy. I already told you everything.
			return true;
		}
	}
	return false;
}

bool SceneScriptPS09::ClickedOnItem(int itemId, bool a2) {
	return false;
}

bool SceneScriptPS09::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -559.15f, 0.0f, -85.06f, 0, true, false, false)) {
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Set_Enter(kSetPS02, kScenePS02);
			Game_Flag_Reset(kFlagPS09Entered);
		}
		return true;
	}
	return false;
}

bool SceneScriptPS09::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptPS09::SceneFrameAdvanced(int frame) {
	if (frame == 1
	 || frame == 15
	 || frame == 20
	 || frame == 31
	 || frame == 33
	 || frame == 35
	 || frame == 52
	 || frame == 54
	) {
		Sound_Play(kSfxNEON7, Random_Query(50, 33), 10, 10, 50);
	}
	//return true;
}

void SceneScriptPS09::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptPS09::PlayerWalkedIn() {
	if (Game_Flag_Query(kFlagMcCoyArrested)) {
		Player_Loses_Control();
		Delay(2000);
		Actor_Retired_Here(kActorMcCoy, 6, 6, 1, -1);
		//return true;
		return;
	}
	if (!Game_Flag_Query(kFlagPS09Entered)) {
		Player_Loses_Control();
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, -491.15f, 0.0f, -73.06f, 0, false, false, false);
		Player_Gains_Control();
		Game_Flag_Set(kFlagPS09Entered);
	}
	if (Game_Flag_Query(kFlagPS02toPS09)) {
		Game_Flag_Reset(kFlagPS02toPS09);
		//return true;
		return;
	}
	//return false;
}

void SceneScriptPS09::PlayerWalkedOut() {
}

void SceneScriptPS09::DialogueQueueFlushed(int a1) {
}

void SceneScriptPS09::dialogueWithGrigorian() {
	Dialogue_Menu_Clear_List();
	if (Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewA)
	 || Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewB1)
	 || Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewB2)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(170,  5, 5, 3); // PROTEST
		DM_Add_To_List_Never_Repeat_Once_Selected(180, -1, 5, 5); // CARS
		if ((_vm->_cutContent
		     && (!Game_Flag_Query(kFlagPS09GrigorianVKChosen)
		         && (!Actor_Clue_Query(kActorMcCoy, kClueVKGrigorianHuman) && !Actor_Clue_Query(kActorMcCoy, kClueVKGrigorianReplicant))))
		    || !_vm->_cutContent
		) {
			if (_vm->_cutContent) {
				Dialogue_Menu_Clear_Never_Repeat_Was_Selected_Flag(200);
			}
			DM_Add_To_List_Never_Repeat_Once_Selected(200, -1, 3, 6); // VOIGT-KAMPFF
		}
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueGrigoriansNote) // cut feature? it is impossible to obtain this clue
	 && (Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewA)
	  || Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewB1)
	  || Actor_Clue_Query(kActorMcCoy, kClueGrigorianInterviewB2)
	 )
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(190, 5, 6, -1); // NOTE
	}
#if BLADERUNNER_ORIGINAL_BUGS
	Dialogue_Menu_Add_To_List(210); // DONE // A bug? why not Dialogue_Menu_Add_DONE_To_List?
#else
	Dialogue_Menu_Add_DONE_To_List(210); // DONE
#endif // BLADERUNNER_ORIGINAL_BUGS

	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();

	switch (answer) {
	case 170: // PROTEST
		Actor_Says(kActorMcCoy,     4270, 13); // McCoy: I got some more questions for you.
		Actor_Says(kActorMcCoy,     4250, 18); // McCoy: Tell me about your protest at Tyrell's plant.
		Actor_Says(kActorGrigorian,   50, 13); // Grigorian: I already told that other detective everything. Under duress.
		Actor_Says(kActorMcCoy,     4275, 18); // McCoy: I want to know about the people who were there.
		Actor_Says(kActorMcCoy,     4280, 19); // McCoy: The Rastafarian and the Asian guy with the sunglasses.
		if (Game_Flag_Query(kFlagIzoIsReplicant)) {
			Actor_Says(kActorGrigorian,   60, 14); // Grigorian: I don't know them.
			Actor_Says(kActorMcCoy,     4285, 13); // McCoy: Were they together?
			Actor_Says(kActorGrigorian,   70, 12); // Grigorian: They seemed to be. I saw them talking.
			Actor_Says(kActorMcCoy,     4290, 13); // McCoy: Before or after the explosion?
			Actor_Says(kActorGrigorian,   80, 13); // Grigorian: Before. I didn't see them at all after.
			Actor_Says(kActorGrigorian,   90, 13); // Grigorian: That's all I know, I swear. I wouldn't even know where to find explosives. I-- I'm not a violent man.
			Actor_Says(kActorMcCoy,     4295, 18); // McCoy: Of course you aren't. You just want to let Replicants walk the streets.
			Actor_Says(kActorGrigorian,  110, 14); // Grigorian: They're not killers, detective. They're innocents. They just want to live a decent productive life like you or me.
			Actor_Says(kActorMcCoy,     4300, 17); // McCoy: It's that simple, huh?
			return;
		}
		if (!Game_Flag_Query(kFlagIzoIsReplicant)) {
			Actor_Says(kActorGrigorian,  130, 15); // Grigorian: I didn't know the Rastafarian.
			Actor_Says(kActorGrigorian,  140, 13); // Grigorian: The Asian fellow was one of our ex members. Izo was his name.
			Actor_Says(kActorMcCoy,     4305, 13); // McCoy: How long did you know him for?
			Actor_Says(kActorGrigorian,  150, 14); // Grigorian: Let's see... I met him about five years ago.
			Actor_Says(kActorGrigorian,  160, 12); // Grigorian: He seemed so smart and balanced. He really wanted to help the Replicant people.
			Actor_Says(kActorMcCoy,     4310, 13); // McCoy: Then what?
			Actor_Says(kActorGrigorian,  170, 15); // Grigorian: He started associating with the more militant factions.
			Actor_Says(kActorGrigorian,  180, 16); // Grigorian: I was astounded when I heard he was supplying weapons to some of them.
			Actor_Says(kActorMcCoy,     4315, 18); // McCoy: What kind of weapons?
			if (_vm->_cutContent) {
				Actor_Says(kActorGrigorian, 190, kAnimationModeTalk); // Grigorian: Everything and anything.
			}
			Actor_Says(kActorGrigorian, 200, 13); // Grigorian: Guns so new that even the police had hardly used them I heard.
			return;
		}
		break;

	case 180: // CARS
		Actor_Says(kActorMcCoy,     4270,                 18); // McCoy: I got some more questions for you.
		Actor_Says(kActorMcCoy,     4255, kAnimationModeTalk); // McCoy: Lot of people involved in your cause, Spencer?
		Actor_Says(kActorGrigorian,  210,                 12); // Grigorian: Yes sir. We're growing fast.
		Actor_Says(kActorGrigorian,  220,                 13); // Grigorian: As more and more people realize that Replicants have cognitive and emotional legitimacy.
		Actor_Says(kActorGrigorian,  230,                 14); // Grigorian: The "Citizens Against Replicant Slavery" will spearhead the movement towards--
		Actor_Says(kActorMcCoy,     4320,                 14); // McCoy: Save the pitch for someone who gives a shit.
		if (_vm->_cutContent) {
			Actor_Says(kActorMcCoy, 4325, kAnimationModeTalk); // McCoy: What else do you guys do besides wave signs and get arrested?
		}
		Actor_Says(kActorGrigorian,  240, 16); // Grigorian: We're peaceful people, detective.
		Actor_Says(kActorGrigorian,  250, 15); // Grigorian: We live by a code that men like you could never understand.
		Actor_Says(kActorMcCoy,     4330, 13); // McCoy: Try me.
		Actor_Says(kActorGrigorian,  260, 13); // Grigorian: Two centuries ago humans were held in slavery.
		Actor_Says(kActorGrigorian,  270, 12); // Grigorian: Brave souls formed an Underground Railroad.
		if (_vm->_cutContent) {
			Actor_Says(kActorGrigorian, 280, 12); // Grigorian: A way for the slaves to escape cruelty and find freedom.
		}
		Actor_Says(kActorMcCoy,     4335, 18); // McCoy: So you help Replicants escape the city.
		Actor_Says(kActorGrigorian,  290, 15); // Grigorian: I'm talking figuratively, detective.
		Actor_Says(kActorMcCoy,     4340, 13); // McCoy: Sure you are.
		Actor_Modify_Friendliness_To_Other(kActorGrigorian, kActorMcCoy, -5);
		if (Game_Flag_Query(kFlagCrazylegsArrested)) {
			Actor_Says(kActorGrigorian, 300, 12); // Grigorian: You support the cause, don't you Larry?
			Actor_Face_Actor(kActorCrazylegs, kActorGrigorian, true);
			Actor_Says(kActorCrazylegs, 1010, kAnimationModeTalk); // Crazylegs: Hey, now don't bring me into it.
			Actor_Face_Actor(kActorGrigorian, kActorCrazylegs, true);
			Actor_Says(kActorGrigorian, 310, 16); // Grigorian: You don't have to be ashamed of your feelings tow---
			Actor_Face_Actor(kActorMcCoy, kActorCrazylegs, true);
			Actor_Says(kActorMcCoy, 4345, 14); // McCoy: Are you supplying vehicles for this venture, Crazy?
			Actor_Face_Actor(kActorCrazylegs, kActorMcCoy, true);
			Actor_Says(kActorCrazylegs, 1020, kAnimationModeTalk); // Crazylegs: Well, for some kind of Underground Railroad why would I waste my inventory on that?
			Actor_Says(kActorMcCoy,     4350,                 18); // McCoy: You tell me.
			Actor_Says(kActorCrazylegs, 1030, kAnimationModeTalk); // Crazylegs: I don't care whether they're Reps or humans as long as the chinyen is real.
			Actor_Says(kActorMcCoy,     4355,                 19); // McCoy: So you were ready to do business with the Reps who dropped by your place.
			Actor_Says(kActorCrazylegs, 1040, kAnimationModeTalk); // Crazylegs: Hey, I didn't know what that broad was.
			Actor_Says(kActorMcCoy,     4360,                 16); // McCoy: Tell it straight or I'm gonna make sure you get the same as he gets. Full conspiracy, payable for 25.
			Actor_Says(kActorMcCoy, 4365, 14);
			Actor_Says(kActorCrazylegs, 1050, kAnimationModeTalk); // Crazylegs: Look. Now I believe in laissez-faire.
			Actor_Says(kActorCrazylegs, 1060, kAnimationModeTalk); // Crazylegs: I believe everyone can do what they gotta do. For a price, okay?
			Actor_Says(kActorMcCoy,     4370,                 14); // McCoy: You're a real humanitarian.
			Actor_Says(kActorCrazylegs, 1070, kAnimationModeTalk); // Crazylegs: Hey, McCoy, I've lost the use of my legs defending these goddamn people Off-World.
			Actor_Says(kActorCrazylegs, 1080, kAnimationModeTalk); // Crazylegs: I don't owe them anything.
		} else {
			Actor_Says(kActorGrigorian, 320, 13); // Grigorian: We're not children, detective. We have resources.
			if (_vm->_cutContent) {
				Actor_Says(kActorGrigorian, 330, kAnimationModeTalk); // Grigorian: Friends with access to vehicles, to escape routes.
			}
			Actor_Says(kActorGrigorian,  340, 14); // Grigorian: The world is changing.
			Actor_Says(kActorGrigorian,  350, 12); // Grigorian: People are waking up to the injustices perpetrated by a sick society that you have helped--
			Actor_Says(kActorMcCoy,     4375, 18); // McCoy: A lot of them will wake up dead, if Reps are allowed to run amok on Terra, jerk.
		}
		break;

	case 190: // NOTE
		Actor_Says(kActorMcCoy,     4270,                 18); // McCoy: I got some more questions for you.
		Actor_Says(kActorMcCoy,     4260, kAnimationModeTalk); // McCoy: You've been helping Reps, pal?
		Actor_Says(kActorGrigorian,  360,                 16); // Grigorian: Through peaceful protest only.
		Actor_Says(kActorMcCoy,     4380,                 19); // McCoy: So talk to me about the Reps with the black Sedan. I found your note in their car.
		Actor_Says(kActorMcCoy,     4385,                 19); // McCoy: Help these lost souls. Isn't that what it said?
		Actor_Says(kActorGrigorian,  370,                 13); // Grigorian: We still have freedom of speech in this country. I can write whatever I please.
		Actor_Says(kActorMcCoy,     4390,                 19); // McCoy: This little exercise of your right smells like criminal conspiracy to me, pal.
		Actor_Says(kActorMcCoy,     4395,                 18); // McCoy: How was Crazy-legs supposed to help them?
		Actor_Says(kActorGrigorian,  380,                 14); // Grigorian: You'll have to ask him yourself. I'm not ashamed of what I did. And I'm sure he's not either.
		Actor_Says(kActorGrigorian,  390,                 12); // Grigorian: (boop)
		Actor_Modify_Friendliness_To_Other(kActorGrigorian, kActorMcCoy, -5);
		break;

	case 200: // VOIGT-KAMPFF
		if (_vm->_cutContent) {
			Game_Flag_Set(kFlagPS09GrigorianVKChosen);
		}
		Actor_Says(kActorMcCoy,     4265, 14); // McCoy: I'm gonna give you a little test.
		Actor_Says(kActorGrigorian,  400, 13); // Grigorian: The other detective, she already tested me earlier today.
		Actor_Says(kActorMcCoy,     4400, 13); // McCoy: Aha, look, I gotta check out the equipment and you're the only stiff around.
		Actor_Says(kActorGrigorian,  410, 16); // Grigorian: I object to this treatment! If you'll just call my lawyer, he'll take care--
		Actor_Says(kActorMcCoy,     4405, 14); // McCoy: Your lawyer would tell you I got the authority to V-K the mayor, if I want.
		Actor_Says(kActorMcCoy,     4410, 15); // McCoy: And he'd have to smile and nod and kiss my butt until I cleared him. So sit down!
		Voight_Kampff_Activate(kActorGrigorian, 20);
		Actor_Modify_Friendliness_To_Other(kActorGrigorian, kActorMcCoy, -10);
		break;

	case 210: // DONE
		Actor_Says(kActorMcCoy,     8600, 18); // McCoy: You keeping busy, pal?
		Actor_Says(kActorGrigorian,   20, 15); // Grigorian: Please leave me alone. My migraine's acting up.
		break;
	}
}

} // End of namespace BladeRunner
