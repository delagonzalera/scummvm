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

void SceneScriptHC04::InitializeScene() {
	if (Game_Flag_Query(kFlagRC03toHC04)) {
		Setup_Scene_Information(-112.0f, 0.14f, -655.0f, 460);
		Game_Flag_Reset(kFlagRC03toHC04);
	} else {
		Setup_Scene_Information(-88.0f, 0.14f, -463.0f, 1013);
	}
	Music_Play(kMusicOneTime, 14, -90, 1, -1, 1, 2);

	Actor_Put_In_Set(kActorIsabella, kSetHC01_HC02_HC03_HC04);
	Actor_Set_At_XYZ(kActorIsabella, -210.0f, 0.0f, -445.0f, 250);

	Scene_Exit_Add_2D_Exit(0, 539,  51, 639, 309, 0);
	Scene_Exit_Add_2D_Exit(1,   0, 456, 639, 479, 2);

	Ambient_Sounds_Add_Looping_Sound(kSfxRAINAWN1, 50, 50, 0);
	Ambient_Sounds_Add_Looping_Sound(kSfxMAMASNG1, 16, 16, 0);
	Ambient_Sounds_Add_Looping_Sound(kSfxROTIS2,   40, 40, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0480R, 5, 70, 14, 16, -100,  100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0540R, 5, 70, 14, 16, -100,  100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0560R, 5, 70, 14, 16, -100,  100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0870R, 5, 70, 14, 16, -100,  100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0900R, 5, 70, 14, 16, -100,  100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_0940R, 5, 70, 14, 16, -100,  100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_1070R, 5, 70, 14, 16, -100,  100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_1080R, 5, 70, 14, 16, -100,  100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfx67_1160R, 5, 70, 14, 16, -100,  100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxBARSFX1,  3, 60, 16, 16, -100, -100, -101, -101, 0, 0);
#if BLADERUNNER_ORIGINAL_BUGS
	Ambient_Sounds_Add_Sound(kSfxBARSFX1,  3, 60, 16, 16, -100, -100, -101, -101, 0, 0);
#endif // BLADERUNNER_ORIGINAL_BUGS
	Ambient_Sounds_Add_Sound(kSfxBARSFX3,  3, 60, 16, 16, -100, -100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxBARSFX4,  3, 60, 16, 16, -100, -100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxBARSFX5,  3, 60, 16, 16, -100, -100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxBARSFX6,  3, 60, 16, 16, -100, -100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxBARSFX7,  3, 60, 16, 16, -100, -100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxCLINK1,   3, 60, 16, 16, -100, -100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxCLINK2,   3, 60, 16, 16, -100, -100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxCLINK3,   3, 60, 16, 16, -100, -100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxCLINK4,   3, 60, 16, 16, -100, -100, -101, -101, 0, 0);

	if (Game_Flag_Query(kFlagHC02toHC04)) {
		Scene_Loop_Start_Special(kSceneLoopModeLoseControl, 0, false);
		Scene_Loop_Set_Default(1);
		Game_Flag_Reset(kFlagHC02toHC04);
	} else {
		Scene_Loop_Set_Default(1);
	}
}

void SceneScriptHC04::SceneLoaded() {
	Obstacle_Object("CAN FIRE", true);
	Unobstacle_Object("ASIANMALE01", true);
	Clickable_Object("CAN FIRE");
#if BLADERUNNER_ORIGINAL_BUGS
#else
	Unclickable_Object("CAN FIRE");
#endif // BLADERUNNER_ORIGINAL_BUGS
}

bool SceneScriptHC04::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptHC04::ClickedOn3DObject(const char *objectName, bool a2) {
	return false;
}

bool SceneScriptHC04::ClickedOnActor(int actorId) {
	if (actorId == kActorIsabella) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -155.0f, 0.0f, -475.0f, 12, true, false, false)) {
			Actor_Face_Actor(kActorMcCoy, kActorIsabella, true);
			Actor_Face_Actor(kActorIsabella, kActorMcCoy, true);
			if (!Game_Flag_Query(kFlagHC04IsabellaTalk)) {
				Actor_Says(kActorIsabella,    0, kAnimationModeTalk); // Isabella: Step up. Step up. Mama Isabella cook you something special. Put a glow in your cheek.
				Actor_Says(kActorMcCoy,    1280, kAnimationModeTalk); // McCoy: McCoy, LPD. Mind if I ask you a couple of questions?
				Actor_Says(kActorIsabella,   20, kAnimationModeTalk); // Isabella: Questions? They always need answers.
				Game_Flag_Set(kFlagHC04IsabellaTalk);
				return true;
			} else {
				dialogueWithIsabella();
				return true;
			}
		}
	}
	return false;
}

bool SceneScriptHC04::ClickedOnItem(int itemId, bool a2) {
	return false;
}

bool SceneScriptHC04::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -108.0f, 0.14f, -639.0f, 0, true, false, false)) {
			Music_Stop(2);
			Game_Flag_Set(kFlagHC04toRC03);
			Game_Flag_Reset(kFlagMcCoyInHawkersCircle);
			Game_Flag_Set(kFlagMcCoyInRunciters);
			Set_Enter(kSetRC03, kSceneRC03);
		}
		return true;
	}

	if (exitId == 1) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -72.0f, 0.14f, -399.0f, 0, true, false, false)) {
			Game_Flag_Set(kFlagHC04toHC02);
			Set_Enter(kSetHC01_HC02_HC03_HC04, kSceneHC02);
		}
		return true;
	}
	return false;
}

bool SceneScriptHC04::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptHC04::SceneFrameAdvanced(int frame) {
}

void SceneScriptHC04::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptHC04::PlayerWalkedIn() {
}

void SceneScriptHC04::PlayerWalkedOut() {
	Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
	Ambient_Sounds_Remove_All_Looping_Sounds(1);
}

void SceneScriptHC04::DialogueQueueFlushed(int a1) {
}

void SceneScriptHC04::dialogueWithIsabella() {
	Dialogue_Menu_Clear_List();
	if (Actor_Clue_Query(kActorMcCoy, kClueKingstonKitchenBox2)
	 || Actor_Clue_Query(kActorMcCoy, kClueKingstonKitchenBox1)
	) {
		if (Actor_Clue_Query(kActorMcCoy, kClueBombingSuspect)) {
			DM_Add_To_List_Never_Repeat_Once_Selected(340, 5, 6, 5); // SADIK PHOTO
		} else if (Actor_Clue_Query(kActorMcCoy, kClueTyrellSecurityPhoto)) {
			DM_Add_To_List_Never_Repeat_Once_Selected(350, 5, 6, 5); // DELIVERYMEN
		}
		DM_Add_To_List_Never_Repeat_Once_Selected(360, 6, 4, 3); // MARCUS EISENDULLER
	}
	if ( Actor_Clue_Query(kActorMcCoy, kClueCheese)
	 && !Actor_Clue_Query(kActorMcCoy, kClueSpecialIngredient)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(370, 3, 4, 7); // CHEESE
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueSpecialIngredient)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(380, -1, 5, 8); // MIA AND MURRAY INFO
	}
	DM_Add_To_List_Never_Repeat_Once_Selected(390, 7, 5, -1); // BUY STEW
	Dialogue_Menu_Add_DONE_To_List(400); // DONE

	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();

	switch (answer) {
	case 340: // SADIK PHOTO
		Actor_Says(kActorMcCoy,    1285, kAnimationModeTalk); // McCoy: This guy work for you?
		Actor_Says(kActorIsabella,   50, kAnimationModeTalk); // Isabella: No, but I wish he do. He be one fine-looking man.
		Actor_Says(kActorMcCoy,    1330, kAnimationModeTalk); // McCoy: You sure? Never seen him before?
		Actor_Says(kActorIsabella,   60, kAnimationModeTalk); // Isabella: I'd remember him for sure.
		break;

	case 350: // DELIVERYMEN
		Actor_Says(kActorMcCoy,    1290, kAnimationModeTalk); // McCoy: How many delivery men you got working here?
		Actor_Says(kActorIsabella,   70, kAnimationModeTalk); // Isabella: Two. Sometimes three on the busy night.
		Actor_Says(kActorMcCoy,    1335, kAnimationModeTalk); // McCoy: Any Rastafarians?
		Actor_Says(kActorIsabella,   80, kAnimationModeTalk); // Isabella: Usual two riders. They Chinese. Third guy part timer. He Moroccan I think.
		Actor_Says(kActorMcCoy,    1340, kAnimationModeTalk); // McCoy: A big guy with dreadlocks?
		Actor_Says(kActorIsabella,   90, kAnimationModeTalk); // Isabella: Oh no. Shaved head. Real short.
		Actor_Says(kActorMcCoy,    1345, kAnimationModeTalk); // McCoy: Thanks.
		break;

	case 360: // MARCUS EISENDULLER
		Actor_Says(kActorMcCoy,    1295, kAnimationModeTalk); // McCoy: You send a lot of deliveries over to Marcus Eisenduller at the Tyrell building?
		Actor_Says(kActorIsabella,  100, kAnimationModeTalk); // Isabella: Eisenduller? Oh, Eisenduller, ya! Let me tell you that man can eat!
		Actor_Says(kActorMcCoy,    1350, kAnimationModeTalk); // McCoy: Send anything over lately?
		Actor_Says(kActorIsabella,  110, kAnimationModeTalk); // Isabella: No sir. No deliveries to him no more. Not since last month. He be way behind on his bill.
		Actor_Says(kActorMcCoy,    1355, kAnimationModeTalk); // McCoy: Not for a month, eh?
		Actor_Says(kActorIsabella,  130, kAnimationModeTalk); // Isabella: At least. No more till he pay up.
		Actor_Says(kActorMcCoy,    1360, kAnimationModeTalk); // McCoy: You may have a little trouble collecting. He's dead.
		break;

	case 370: // CHEESE
		Actor_Says(kActorMcCoy,    1300, kAnimationModeTalk); // McCoy: Hey, you know where I can get some cheese like this?
		Actor_Says(kActorIsabella,  140, kAnimationModeTalk); // Isabella: Don't know nothing about cheese. That stuff illegal. It bad news.
		Actor_Says(kActorMcCoy,    1365, kAnimationModeTalk); // McCoy: I'm not looking to bust the dealer. I just want to know who bought it.
		Actor_Says(kActorIsabella,  150, kAnimationModeTalk); // Isabella: This is a clean place man. Mama Isabella law-abiding soul.
		break;

	case 380: // MIA AND MURRAY INFO
		Actor_Says(kActorMcCoy, 1305, kAnimationModeTalk); // McCoy: Mia and Murray tell me you're using cheese in your stew. Real cheese.
		Actor_Modify_Friendliness_To_Other(kActorIsabella, kActorMcCoy, -2);
		Actor_Says(kActorIsabella,  160, kAnimationModeTalk); // Isabella: Them old buzzards. What they know?
		Actor_Says(kActorMcCoy,    1370, kAnimationModeTalk); // McCoy: I could take a sample of that stew you're cooking back to the Lab and have it tested.
		Actor_Says(kActorIsabella,  170, kAnimationModeTalk); // Isabella: Why you pick on Mama Isabella? I never do you no harm.
		Actor_Says(kActorMcCoy,    1375, kAnimationModeTalk); // McCoy: Where did you get it?
		Actor_Says(kActorIsabella,  180, kAnimationModeTalk); // Isabella: My sister. She got connections.
		Actor_Says(kActorMcCoy,    1380, kAnimationModeTalk); // McCoy: Where can I find her?
		Actor_Says(kActorIsabella,  190, kAnimationModeTalk); // Isabella: She back in Jamaica, mon. Far away from here. She send me cheese once maybe twice a month.
		Actor_Says(kActorIsabella,  210, kAnimationModeTalk); // Isabella: You know, funny thing you asking about the cheese. Last week this ugly little man, he ran by. Grab whole box of empty takeout cartons right off the counter.
		Actor_Says(kActorIsabella,  240, kAnimationModeTalk); // Isabella: Police next dawn they just laugh. Say the guy not be so happy when he opens them cartons. (scoffs)
		Actor_Says(kActorMcCoy,    1385, kAnimationModeTalk); // McCoy: What's that have to do with the cheese?
		Actor_Says(kActorIsabella,  260, kAnimationModeTalk); // Isabella: Cheese hidden in that box, mon. Keeping it safe till I cook up a new batch. So how can I tell police to bring back box. They find cheese, they shut me down.
		Actor_Says(kActorMcCoy,    1390, kAnimationModeTalk); // McCoy: This guy who robbed you. What did he look like?
		Actor_Says(kActorIsabella,  300, kAnimationModeTalk); // Isabella: Short, real short. Bad clothes, colors way too bright. Funny bow tie.
		Actor_Says(kActorIsabella,  310, kAnimationModeTalk); // Isabella: Maybe you look for this ugly little thief, ya?
		Actor_Says(kActorIsabella,  320, kAnimationModeTalk); // Isabella: I'm thinking you not really care about the cheese.
		Actor_Says(kActorMcCoy,    1395, kAnimationModeTalk); // McCoy: You're right. But I still think you ought to get yourself a different secret ingredient.
		Actor_Says(kActorIsabella,  330, kAnimationModeTalk); // Isabella: You bet, mon. That cheese's been nothing but trouble for Mama Isabella.
		Actor_Clue_Acquire(kActorMcCoy, kClueStolenCheese, false, kActorIsabella);
		break;

	case 390: // BUY STEW
		Actor_Says(kActorMcCoy, 1310, kAnimationModeTalk); // McCoy: That stew is looking tasty.
		Actor_Modify_Friendliness_To_Other(kActorIsabella, kActorMcCoy, 2);
		Actor_Says(kActorIsabella, 340, kAnimationModeTalk); // Isabella: Fix you right up. Only 30 chinyen. It put a spring in your step, mon. The ladies they be loving you.
		break;

	case 400: // DONE
		Actor_Says(kActorMcCoy, 1315, kAnimationModeTalk); // McCoy: Thanks for your time.
		break;

	default: // never used?
		Actor_Says(kActorMcCoy,    1320, kAnimationModeTalk); // McCoy: Smells good. What is it?
		Actor_Says(kActorIsabella,   30, kAnimationModeTalk); // Isabella: All in special recipe. (laughs) But if I be telling you what's in it, you might be thinking twice about eating it.
		Actor_Says(kActorMcCoy,    1325, kAnimationModeTalk); // McCoy: Uh... I'm not that hungry anyway.
		Actor_Says(kActorMcCoy,    1345, kAnimationModeTalk); // McCoy: Thanks.
		break;
	}
}

} // End of namespace BladeRunner
