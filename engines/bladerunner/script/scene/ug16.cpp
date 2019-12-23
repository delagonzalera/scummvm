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

void SceneScriptUG16::InitializeScene() {
	if (Game_Flag_Query(kFlagDR06toUG16)) {
		Setup_Scene_Information(-270.76f, -34.88f, -504.02f, 404);
		Game_Flag_Reset(kFlagDR06toUG16);
	} else if (Game_Flag_Query(kFlagUG15toUG16a)) {
		Setup_Scene_Information(-322.0f,   -34.0f,  -404.0f, 345);
		Game_Flag_Reset(kFlagUG15toUG16a);
	} else {
		Setup_Scene_Information(-318.0f,   -34.0f,  -216.0f, 340);
		Game_Flag_Reset(kFlagUG15toUG16b);
	}

	Scene_Exit_Add_2D_Exit(0, 242, 169, 282, 262, 3);
	Scene_Exit_Add_2D_Exit(1, 375, 166, 407, 251, 3);
	Scene_Exit_Add_2D_Exit(2, 461, 148, 523, 248, 0);

	Ambient_Sounds_Add_Looping_Sound(kSfxELECLAB1, 33, 81, 0);
	Ambient_Sounds_Add_Looping_Sound(kSfxUGBED1,   40,  0, 1);
	Ambient_Sounds_Add_Looping_Sound(kSfxUGBED2,   40,  0, 1);

	if (Game_Flag_Query(kFlagUG16ComputerOff)) {
		Scene_Loop_Set_Default(5);
	} else {
		Scene_Loop_Set_Default(0);
	}
}

void SceneScriptUG16::SceneLoaded() {
	Obstacle_Object("BED", true);
	Obstacle_Object("QUADPATCH07", true);
	Obstacle_Object("QUADPATCH05", true);
	Obstacle_Object("SCREEN 01", true);
	Obstacle_Object("BOX49", true);
	Obstacle_Object("CYLINDER07", true);
	Unobstacle_Object("SEAT 1", true);
	Unobstacle_Object("SEAT 2", true);
	Unclickable_Object("BED");
	Unclickable_Object("QUADPATCH07");
	Clickable_Object("QUADPATCH05");
	Clickable_Object("SCREEN 01");
	Unclickable_Object("BOX49");
	Unclickable_Object("CYLINDER07");
	Unobstacle_Object("BOX67", true);
	Footstep_Sounds_Set(0, 3);
	Footstep_Sounds_Set(1, 2);
	Footstep_Sounds_Set(2, 3);
	Footstep_Sounds_Set(6, 3);
}

bool SceneScriptUG16::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptUG16::ClickedOn3DObject(const char *objectName, bool a2) {
	if (Object_Query_Click("QUADPATCH05", objectName)) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 194.0f, -35.0f, 160.8f, 0, true, false, false)) {
			Actor_Face_Heading(kActorMcCoy, 870, false);
			if (!Game_Flag_Query(kFlagUG16FolderFound)
			 &&  Game_Flag_Query(kFlagLutherLanceAreDead)
			) {
				Game_Flag_Set(kFlagUG16FolderFound);
				Delay(1000);
				Actor_Voice_Over(3480, kActorVoiceOver); // Mainframe: Yeah, what a difference a day makes.
				Actor_Change_Animation_Mode(kActorMcCoy, 38);
				Sound_Play(kSfxDRAWER1, 100, 0, 0, 50);
				Delay(1000);
				Item_Pickup_Spin_Effect(kModelAnimationFolder, 460, 287);
				Actor_Voice_Over(2740, kActorVoiceOver); // Mainframe: Guzza had his fingers in as many rancid pies as possible.
				Actor_Voice_Over(2750, kActorVoiceOver); // Mainframe: He was insatiable. He sold police equipment, ripped off criminals. And he even killed.
				Actor_Voice_Over(2760, kActorVoiceOver); // Mainframe: And here were documents that proved it all. Depositions, photographs, videodiscs.
				Actor_Voice_Over(2770, kActorVoiceOver); // Mainframe: It was the complete file on Guzza, the dirty cop.
				Actor_Clue_Acquire(kActorMcCoy, kClueFolder, true, -1);
			} else {
				Actor_Says(kActorMcCoy, 8523, 12); // McCoy: (grunts)
				Actor_Says(kActorMcCoy, 8635, 12); // McCoy: That sucks.
			}
			return true;
		}
	}

	if (Object_Query_Click("SCREEN 01", objectName)) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 194.0f, -35.0f, 160.8f, 0, true, false, false)) {

			Actor_Face_Heading(kActorMcCoy, 870, false);

			if (( Game_Flag_Query(kFlagLutherLanceAreDead)
			  || !Actor_Query_Is_In_Current_Set(kActorLuther)
			 )
			 && !Actor_Clue_Query(kActorMcCoy, kClueDNALutherLance)
			 && !Game_Flag_Query(kFlagUG16ComputerOff)
			) {
				Delay(2000);
				Actor_Face_Heading(kActorMcCoy, 1016, false);
				Delay(2000);
				Actor_Says(kActorMcCoy, 5725, 14); // McCoy: Dump a copy of all the data in this directory to drive D.
				Delay(1000);
				Item_Pickup_Spin_Effect(kModelAnimationDNADataDisc, 418, 305);
				Actor_Clue_Acquire(kActorMcCoy, kClueDNALutherLance, true, -1);
				return true;
			}

			Actor_Says(kActorMcCoy, 8525, 12); // McCoy: Hmph.
			Actor_Says(kActorMcCoy, 8526, 12); // McCoy: Nothing.
		}
	}
	return false;
}

bool SceneScriptUG16::ClickedOnActor(int actorId) {
	if (Actor_Query_Goal_Number(kActorLuther) < 490) { // Luther & Lance are alive
		dialogueWithLuther();
		return true;
	}
	return false;
}

bool SceneScriptUG16::ClickedOnItem(int itemId, bool a2) {
	return false;
}

bool SceneScriptUG16::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -322.0f, -34.0f, -216.0f, 0, true, false, false)) {
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Game_Flag_Set(kFlagUG16toUG15b);
			Set_Enter(kSetUG15, kSceneUG15);
		}
		return true;
	}

	if (exitId == 1) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -322.0f, -34.0f, -404.0f, 0, true, false, false)) {
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Game_Flag_Set(kFlagUG16toUG15a);
			Set_Enter(kSetUG15, kSceneUG15);
		}
		return true;
	}

	if (exitId == 2) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, -316.78f, -34.88f, -533.27f, 0, true, false, false)) {
			Actor_Face_Heading(kActorMcCoy, 0, false);
			Loop_Actor_Travel_Stairs(kActorMcCoy, 13, true, kAnimationModeIdle);
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Game_Flag_Set(kFlagUG16toDR06);
			Set_Enter(kSetDR06, kSceneDR06);
		}
		return true;
	}
	return false;
}

bool SceneScriptUG16::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptUG16::SceneFrameAdvanced(int frame) {
	if (frame == 132) {
		Ambient_Sounds_Remove_Looping_Sound(kSfxELECLAB1, 1);
	}
}

void SceneScriptUG16::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptUG16::PlayerWalkedIn() {
	Game_Flag_Set(kFlagDR06UnlockedToUG16);

	if (!Game_Flag_Query(kFlagLutherLanceAreDead)) {
		Actor_Set_Goal_Number(kActorLuther, kGoalLutherStop);
	}

	if (!Game_Flag_Query(kFlagUG16LutherLanceTalk1)
	 &&  Actor_Query_Is_In_Current_Set(kActorLuther)
	) {
		Player_Loses_Control();
		Loop_Actor_Walk_To_XYZ(kActorMcCoy, 120.29f, -35.67f, 214.8f, 310, false, false, false);
		Actor_Face_Actor(kActorMcCoy, kActorLuther, true);
		Actor_Says(kActorLuther,  0,  6); // Luther: No, no, we tried that already. Look, we know that the Adenine-Thymine and Guanine-Cytosine sequences can be mutated in progress.
		Actor_Says(kActorLuther, 30, 13); // Luther: Let's move on.
		Actor_Change_Animation_Mode(kActorLuther, 17);
		Actor_Says(kActorLance,     0, 17); // Lance: Hey, it's about time you showed up.
		Actor_Says(kActorMcCoy,  5710, 14); // McCoy: You know who I am?
		Actor_Says(kActorLuther,   40, 13); // Luther: Detective Ray McCoy.
		Actor_Says(kActorLuther,   50, 15); // Luther: You're investigating Moraji's death.
		Actor_Says(kActorLance,    20, 12); // Lance: And Eisenduller's.
		Actor_Says(kActorLuther,   60, 23); // Luther: Having any luck?
		Actor_Says(kActorMcCoy,  5715, 14); // McCoy: You're Luther and Lance?
		Actor_Says(kActorLance,    30, 16); // Lance: Hell of a smart cookie there.
		Actor_Says(kActorLuther,   70,  6); // Luther: Take your guns and your violence elsewhere. We've got important work to do.
		Player_Gains_Control();
		Game_Flag_Set(kFlagUG16LutherLanceTalk1);
	}
}

void SceneScriptUG16::PlayerWalkedOut() {
	if (!Game_Flag_Query(kFlagLutherLanceAreDead)) {
		Actor_Set_Goal_Number(kActorLuther, kGoalLutherMoveAround);
		//return true;
	}
	//return false;
}

void SceneScriptUG16::DialogueQueueFlushed(int a1) {
}

void SceneScriptUG16::dialogueWithLuther() {
	Dialogue_Menu_Clear_List();
	DM_Add_To_List_Never_Repeat_Once_Selected(1400, 5, 6, 2); // REPLICANTS
	DM_Add_To_List_Never_Repeat_Once_Selected(1410, 5, 4, 8); // WORK
	if (Game_Flag_Query(kFlagUG16LutherLanceTalkReplicants1)
	 || Game_Flag_Query(kFlagUG16LutherLanceTalkReplicants2)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1420, 6, 4, 5); // LIFESPAN
		DM_Add_To_List_Never_Repeat_Once_Selected(1430, 6, 4, 5); // CLOVIS
		DM_Add_To_List_Never_Repeat_Once_Selected(1440, 6, 4, 5); // VOIGT-KAMPFF
	}
	if ( Global_Variable_Query(kVariableCorruptedGuzzaEvidence) > 1
	 && !Actor_Clue_Query(kActorMcCoy, kClueFolder)
	 ) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1450, 6, 4, 5); // GUZZA
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueEnvelope)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1460, 6, 4, 5); // RUNCITER
	}
	if ( Actor_Clue_Query(kActorMcCoy, kClueDNATyrell)
	 && !Actor_Clue_Query(kActorMcCoy, kClueFolder)
	 &&  Game_Flag_Query(kFlagUG15LanceLuthorTrade)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1470, 6, 4, 5); // TRADE
	}
	Dialogue_Menu_Add_DONE_To_List(1480); // DONE

	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();

	switch (answer) {
	case 1400: // REPLICANTS
		Actor_Says(kActorMcCoy, 5730, 13); // McCoy: The Reps that killed Marcus and Moraji. They'll be looking for you.
		Actor_Face_Actor(kActorMcCoy, kActorLuther, true);
		Actor_Says(kActorLuther,  100, 18); // Luther: You're a little late, detective. They've already found us.
		Actor_Says(kActorMcCoy,  5775, 13); // McCoy: So why are you still walking around?
		Actor_Says(kActorLance,    70, 17); // Lance: Cause we're all part of the same suit of cards, you know what I mean?
		Actor_Says(kActorLuther,  110, 16); // Luther: Eldon makes mistakes just like any other... human. The difference is...
		Actor_Says(kActorLance,    80,  6); // Lance: The difference is the old bastard doesn't like to admit it. No, he-- he covers them up, pretends they're human, gives them a job at his stinking corporation. And when they actually produce original work he boots them out on the street.
		Actor_Says(kActorMcCoy,  5780, 13); // McCoy: You're saying you are Replicants?
		Actor_Says(kActorLuther,  120, 16); // Luther: We finally figured it out.
		Actor_Says(kActorLance,   120, 13); // Lance: Clovis helped us see the light.
		Actor_Says(kActorMcCoy,  5785, 13); // McCoy: But that's ridiculous. Tyrell wouldn't design something like you. He wants perfection.
		Actor_Says(kActorLuther,  130,  6); // Luther: I told you we were a mistake.
		Actor_Says(kActorMcCoy,  5825, 13); // McCoy: Keep talking.
		Actor_Modify_Friendliness_To_Other(kActorLuther, kActorMcCoy, -5);
		if (Game_Flag_Query(kFlagLutherLanceIsReplicant)) {
			Actor_Says(kActorLuther,  140, 13); // Luther: Sometimes our memories are different from each other's.
			Actor_Says(kActorLuther,  150, 14); // Luther: The implant process isn't perfect. We know, we've seen the studies.
			Actor_Says(kActorLuther,  160, 13); // Luther: You can't always be sure exactly how the memories will play out in the subject's mind.
			Actor_Says(kActorLance,   140, 16); // Lance: We've got different recollections of our childhood. Explain that.
			Actor_Says(kActorMcCoy,  5790, 13); // McCoy: Maybe one of you just forgot.
			Actor_Says(kActorLuther,  170, 14); // Luther: If you can't trust your own memories, what's left that makes you human?
			Game_Flag_Set(kFlagUG16LutherLanceTalkReplicants1);
			Actor_Modify_Friendliness_To_Other(kActorLuther, kActorMcCoy, 5);
		} else {
			Actor_Says(kActorLuther,  180, 14); // Luther: Clovis showed us our incept photos.
			Actor_Says(kActorMcCoy,  5795, 13); // McCoy: So what?
			Actor_Says(kActorLance,   150, 17); // Lance: That's a pretty damn good argument, you gotta admit.
			Actor_Says(kActorMcCoy,  5800, 13); // McCoy: Photos can be doctored. It's not proof.
			Actor_Says(kActorLuther,  190, 15); // Luther: Clovis said you'd say that.
			Game_Flag_Set(kFlagUG16LutherLanceTalkReplicants2);
			Actor_Modify_Friendliness_To_Other(kActorLuther, kActorMcCoy, -10);
		}
		break;

	case 1410: // WORK
		Actor_Says(kActorMcCoy, 5735, 13); // McCoy: What are you doing down here?
		Actor_Face_Actor(kActorMcCoy, kActorLuther, true);
		Actor_Says(kActorLance,  160, 17); // Lance: Eldon shafted us. So, now we're returning the favor.
		Actor_Says(kActorLuther, 200, 14); // Luther: We promised Clovis we'd be finished with this by the time he got back, so if you don't mind...
		break;

	case 1420: // LIFESPAN
		Actor_Says(kActorMcCoy, 5740, 13); // McCoy: Tyrell says the four year lifespan can't be extended.
		Actor_Face_Actor(kActorMcCoy, kActorLuther, true);
		Actor_Says(kActorLance,   180, 15); // Lance: Of course he's gonna say that but he's wrong.
		Actor_Says(kActorLuther,  220, 13); // Luther: Very wrong.
		Actor_Says(kActorLance,   190, 17); // Lance: We got the research right here that will prove it.
		Actor_Says(kActorMcCoy,  5805, 13); // McCoy: Is it complete? Could you extend the life of a Replicant right now?
		Actor_Says(kActorLuther,  230, 14); // Luther: Well, no. The four year limit is an integral part of every element of the Replicant.
		Actor_Says(kActorLuther,  240, 13); // Luther: Tyrell's dictate was that every body part would fail. And only the designer's of each would know exactly how it worked.
		Actor_Says(kActorLance,   200, 17); // Lance: Clovis has already unlocked the secret of the skin. That was Moraji.
		Actor_Says(kActorLuther,  260, 13); // Luther: Sebastian and Chew, they know about the central nervous system and the eyes.
		Actor_Says(kActorLuther,  270, 15); // Luther: A lot of retrograde viruses were built into the eyes.
		Actor_Says(kActorLance,   210, 14); // Lance: And Tyrell, he's got the basic DNA pattern information but he won't just give it up.
		Actor_Says(kActorMcCoy,  5810, 13); // McCoy: Did you tell Clovis all this?
		Actor_Says(kActorLance,   220, 14); // Lance: He didn't ask, he already knew.
		Actor_Says(kActorLance,   230, 17); // Lance: He knows all about you too.
		Actor_Clue_Acquire(kActorMcCoy, kClueLutherLanceInterview, true, kActorLuther);
		break;

	case 1430: // CLOVIS
		Actor_Says(kActorMcCoy, 5745, 13); // McCoy: Where's Clovis now?
		Actor_Face_Actor(kActorMcCoy, kActorLuther, true);
		Actor_Says(kActorLance,   240, 15); // Lance: Gone to have a little heart to heart with Eldon.
		Actor_Says(kActorMcCoy,  5815, 13); // McCoy: He'll kill Tyrell, if he gets to him.
		Actor_Says(kActorLance,   250, 16); // Lance: Isn't that just too bad.
		Actor_Says(kActorLuther,  290, 15); // Luther: I won't shed any tears. Tyrell is killing us all. Four years isn't enough, is it?
		Actor_Says(kActorLance,   260, 15); // Lance: If Clovis snaps his neck, it will be merciful and quick. That's a hell of a lot more than we've been given.
		break;

	case 1440: // VOIGT-KAMPFF
		Actor_Says(kActorMcCoy, 5750, 13); // McCoy: I got a little test I want you to take.
		Actor_Face_Actor(kActorMcCoy, kActorLuther, true);
		Actor_Says(kActorLance,  280,  6); // Lance: (scoffs) Do you believe this clown? He wants to Voigt Kampff us.
		Actor_Says(kActorLuther, 300, 14); // Luther: Sorry, McCoy. Shoot us or leave us alone.
		Actor_Says(kActorLuther, 310, 15); // Luther: Let's get back to work.
		Actor_Modify_Friendliness_To_Other(kActorLuther, kActorMcCoy, -5);
		break;

	case 1450: // GUZZA
		Actor_Says(kActorMcCoy, 5755, 13); // McCoy: Clovis set me up, didn't he? And he used my boss to do it.
		Actor_Face_Actor(kActorMcCoy, kActorLuther, true);
		Actor_Says(kActorLance,   290, 17); // Lance: He's sharp this one.
		Actor_Says(kActorLuther,  320, 16); // Luther: No question.
		Actor_Says(kActorMcCoy,  5820, 13); // McCoy: Tell me what you know.
		Actor_Says(kActorLance,   300, 17); // Lance: How about we trade.
		Actor_Says(kActorLuther,  330, 14); // Luther: Our life for yours.
		Actor_Says(kActorMcCoy,  5825, 13); // McCoy: Keep talking.
		Actor_Says(kActorLuther,  340, 13); // Luther: Clovis has gone to see Eldon.
		Actor_Says(kActorLance,   310, 13); // Lance: But he might not make it.
		Actor_Says(kActorLuther,  350, 13); // Luther: Still the work will go on with or without Clovis.
		Actor_Says(kActorLuther,  360, 15); // Luther: Get us Tyrell's files. The rest of the DNA data. And we will help you.
		Actor_Says(kActorMcCoy,  5830, 13); // McCoy: You want me to break into the Tyrell Building for you? You're nuts.
		Actor_Says(kActorLance,   320, 16); // Lance: Defective, yes. Nuts, no.
		Actor_Says(kActorLance,   330, 15); // Lance: You need us, McCoy. We can serve up Guzza's carcass on a big oversize platter.
		Game_Flag_Set(kFlagUG15LanceLuthorTrade);
		break;

	case 1460: // RUNCITER
		Actor_Says(kActorMcCoy, 5760, 13); // McCoy: What kind of deal did you have going with Runciter?
		Actor_Face_Actor(kActorMcCoy, kActorLuther, true);
		Actor_Says(kActorLuther,  370, 15); // Luther: Don't talk to me about it. That was Lance's stupid deal.
		Actor_Says(kActorLance,   340, 14); // Lance: We just wanted to make some extra cash on the side.
		Actor_Says(kActorMcCoy,  5835, 13); // McCoy: That probably had something to do with you guys getting fired.
		Actor_Says(kActorLuther,  380, 15); // Luther: I'm sure he wasn't too pleased.
		Actor_Says(kActorLance,   370,  6); // Lance: Since Eldon is such a pain in the ass control freak...
		Actor_Says(kActorMcCoy,  5840, 13); // McCoy: Did you meet Clovis through Runciter?
		Actor_Says(kActorLance,   380, 13); // Lance: It was all just a coincidence.
		break;

	case 1470: // TRADE
		Actor_Says(kActorMcCoy, 5765, 13); // McCoy: I think I got what you wanted from Tyrell's office.
		Actor_Face_Actor(kActorMcCoy, kActorLuther, true);
		Actor_Says(kActorLance,   400, 15); // Lance: Check it out, bro. The little man with the big gun did good.
		Actor_Says(kActorMcCoy,  5845, 13); // McCoy: A bargain's a bargain. You said you'd give me Guzza.
		Actor_Says(kActorLuther,  390, 23); // Luther: It's all right here.
		Actor_Says(kActorLance,   410, 14); // Lance: Your boss is a real piece of work, let me tell you. Almost as good as Tyrell.
		Actor_Says(kActorLance,   420, 17); // Lance: Take a look.
		Actor_Says(kActorMcCoy,  5835, 13); // McCoy: That probably had something to do with you guys getting fired.
		Delay(1000);
		Item_Pickup_Spin_Effect(kModelAnimationFolder, 239, 454);
		Actor_Voice_Over(2740, kActorVoiceOver); // Mainframe: Guzza had his fingers in as many rancid pies as possible.
		Actor_Voice_Over(2750, kActorVoiceOver); // Mainframe: He was insatiable. He sold police equipment, ripped off criminals. And he even killed.
		Actor_Voice_Over(2760, kActorVoiceOver); // Mainframe: And here were documents that proved it all. Depositions, photographs, videodiscs.
		Actor_Voice_Over(2770, kActorVoiceOver); // Mainframe: It was the complete file on Guzza, the dirty cop.
		Actor_Says(kActorMcCoy,  5850, 13); // McCoy: Where did you get this?
		Actor_Says(kActorLuther,  400, 15); // Luther: From Clovis. He told us to hold it for him.
		Actor_Says(kActorLance,   430,  6); // Lance: Come to think of it I'm not sure you're supposed to see it. You wanna give it back now?
		Actor_Says(kActorMcCoy,  5855, 13); // McCoy: Not a chance.
		Actor_Says(kActorLuther,  410, 14); // Luther: Let him have it. Clovis said he was finished with the policemen anyway.
		Game_Flag_Set(kFlagUG16FolderFound);
		Actor_Clue_Acquire(kActorMcCoy, kClueFolder, true, kActorLuther);
		break;

	case 1480: // DONE
		Actor_Says(kActorMcCoy, 4595, 14); // McCoy: Stick around. I may not be finished with you.
		break;
	}
}

} // End of namespace BladeRunner
