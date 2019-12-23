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

void SceneScriptRC04::InitializeScene() {
	Setup_Scene_Information(45.0f, 0.15f, 68.0f, 1018);
	Game_Flag_Reset(kFlagRC03toRC04);

	Scene_Exit_Add_2D_Exit(0, 225, 47, 359, 248, 0);

	if (!Game_Flag_Query(kFlagRC04McCoyShotBob)) {
		Actor_Put_In_Set(kActorBulletBob, kSetRC04);
		Actor_Set_At_XYZ(kActorBulletBob, -60.0f, -11.0f, 62.0f, 12);
	}
	if (Game_Flag_Query(kFlagRC04McCoyShotBob)) {
		Actor_Change_Animation_Mode(kActorBulletBob, 88);
	}

	Ambient_Sounds_Add_Looping_Sound(kSfxRAIN10, 100, 1, 1);
	Ambient_Sounds_Add_Sound(kSfxRCCARBY1, 5, 30, 10, 20, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxRCCARBY2, 5, 30, 10, 20, -100, 100, -101, -101, 0, 0);
	Ambient_Sounds_Add_Sound(kSfxRCCARBY3, 5, 30, 10, 20, -100, 100, -101, -101, 0, 0);
}

void SceneScriptRC04::SceneLoaded() {
	Obstacle_Object("Door New 01", true);
	Obstacle_Object("GRNDNEON05", true);
	Obstacle_Object("GRNDNEON06", true);
	Obstacle_Object("GRNDNEON07", true);
	Unobstacle_Object("DisplayTrim", true);
	Unobstacle_Object("Display01", true);
	Actor_Set_Goal_Number(kActorGenwalkerA, kGoalGenwalkerABulletBobsTrackGun);
}

bool SceneScriptRC04::MouseClick(int x, int y) {
	return false;
}

bool SceneScriptRC04::ClickedOn3DObject(const char *objectName, bool a2) {
	return false;
}

void SceneScriptRC04::dialogueWithBulletBob() {
	Dialogue_Menu_Clear_List();
	if ( Actor_Clue_Query(kActorMcCoy, kClueLabShellCasings)
	 && !Actor_Clue_Query(kActorMcCoy, kClueRadiationGoggles)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(580, -1, 4, 9); // WEAPONS
	}
	if ( Actor_Clue_Query(kActorMcCoy, kClueShellCasings)
	 && !Actor_Clue_Query(kActorMcCoy, kClueLabShellCasings)
	 && !Actor_Clue_Query(kActorMcCoy, kClueRadiationGoggles)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(590, 6, 5, 3); // SHELL CASINGS
	}
	if (Actor_Clue_Query(kActorMcCoy, kClueHasanInterview)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(600, -1, 3, 7); // HASAN
		DM_Add_To_List_Never_Repeat_Once_Selected(1310, -1, 2, 8); // VOIGT-KAMPFF
	}
	if ( Actor_Clue_Query(kActorMcCoy, kClueRadiationGoggles)
	 && !Actor_Clue_Query(kActorMcCoy, kClueGogglesReplicantIssue)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(610, 4, 5, 6); // GOGGLES
	}
	if (!Game_Flag_Query(kFlagRC04BobTalkAmmo)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(1280, 1, 2, 3); // AMMO
	}
	if ( Actor_Clue_Query(kActorMcCoy, kClueHoldensBadge)
	 && !Actor_Clue_Query(kActorBulletBob, kClueHoldensBadge)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(620, 1, -1, -1); // HOLDEN'S BADGE
	}
	Dialogue_Menu_Add_DONE_To_List(630); // DONE

	Dialogue_Menu_Appear(320, 240);
	int answer = Dialogue_Menu_Query_Input();
	Dialogue_Menu_Disappear();

	switch (answer) {
	case 580: // WEAPONS
		Actor_Says(kActorMcCoy,     4955, 30); // McCoy: You sell any Off-World stuff? Like Endor rifles?
		Actor_Says(kActorBulletBob,  210, 37); // Bullet Bob: No, I ain't seen anything like that in years.
		Actor_Says(kActorBulletBob,  220, 37); // Bullet Bob: Wish I had one, though. The Endor was a real workhorse.
		Actor_Says(kActorBulletBob,  230, 37); // Bullet Bob: Good stopping power, decent rate of fire.
		Actor_Says(kActorBulletBob,  240, 37); // Bullet Bob: You lookin'? Gonna cost you a bundle I'm afraid.
		Actor_Says(kActorMcCoy,     4990, 16); // McCoy: Not really.
		Actor_Says(kActorMcCoy,     4995, 11); // McCoy: Somebody shot up Runciter's shop with one.
		Actor_Says(kActorBulletBob,  270, 31); // Bullet Bob: Sucker had it coming. I'm sure of that.
		Actor_Says(kActorMcCoy,     5005, 16); // McCoy: Why is that?
		Actor_Says(kActorBulletBob,  280, 32); // Bullet Bob: Runciter is always putting on those airs like he's better than everybody else.
		Actor_Says(kActorBulletBob,  290, 30); // Bullet Bob: But he'll do anything for a buck. Sell anything to anybody.
		Actor_Says(kActorBulletBob,  300, 33); // Bullet Bob: That's where I draw the line. If it's suspicious
		Actor_Says(kActorBulletBob,  310, 31); // Bullet Bob: it's out of here. Got no place at Bullet Bob's.
		Actor_Says(kActorMcCoy,     5010, 11); // McCoy: You know anyone who might sell Endor rifles?
		Actor_Says(kActorBulletBob,  320, 30); // Bullet Bob: There's one other guy around here who dips into that stuff.
		Actor_Says(kActorBulletBob,  330, 33); // Bullet Bob: Got a pawn shop down the street. Goes by the name of Izo.
		Actor_Says(kActorBulletBob,  340, 37); // Bullet Bob: But who knows what the real title is.
		Actor_Says(kActorMcCoy,     5015, 11); // McCoy: Izo, huh. Sounds like you don't like him.
		if (Game_Flag_Query(kFlagIzoIsReplicant)) {
			Actor_Says(kActorBulletBob,  350, 32); // Bullet Bob: I've been around here a long time, Ray.
			Actor_Says(kActorBulletBob,  360, 33); // Bullet Bob: It always pisses me off when some dilettante
			Actor_Says(kActorBulletBob,  370, 30); // Bullet Bob: sets up shop and tries to horn in on my action.
			Actor_Says(kActorMcCoy,     5020, 16); // McCoy: How long has Izo been around?
			Actor_Says(kActorBulletBob,  380, 37); // Bullet Bob: He's brand-new. He's a cold son of a bitch, too.
			Actor_Says(kActorBulletBob,  390, 11); // Bullet Bob: I went over to introduce myself when he first moved in and he practically ignored me!
			Actor_Says(kActorBulletBob,  400, 37); // Bullet Bob: Said he was too busy to talk. Don't think I didn't consider kicking his ass, too.
#if BLADERUNNER_ORIGINAL_BUGS
			Actor_Clue_Acquire(kActorMcCoy, kClueBobInterview1, true, kActorMcCoy);  // A bug? Shouldn't the last argument be -1 or kActorBulletBob here?
#else
			Actor_Clue_Acquire(kActorMcCoy, kClueBobInterview1, true, kActorBulletBob);
#endif // BLADERUNNER_ORIGINAL_BUGS
		} else {
			Actor_Says(kActorBulletBob,  410, 11); // Bullet Bob: Izo is okay. He's run the Green Pawn for a while.
			Actor_Says(kActorBulletBob,  420, 37); // Bullet Bob: We're friendly competitors, I guess. But I don't appreciate his politics.
			Actor_Says(kActorMcCoy,     5025, 16); // McCoy: Yeah?
			Actor_Says(kActorBulletBob,  430, 30); // Bullet Bob: Rumors that's all they are.
			Actor_Says(kActorBulletBob,  440, 31); // Bullet Bob: I was out there, Ray. I did my time. Three and a half years worth.
			Actor_Says(kActorBulletBob,  450, 32); // Bullet Bob: And I can tell you: Reps got no business being down here on Earth, okay?
			Actor_Says(kActorMcCoy,     5030, 16); // McCoy: Yeah. Earth don't deserve them.
			Actor_Says(kActorBulletBob,  460, 37); // Bullet Bob: Tell me about it. We got standards to uphold down here.
#if BLADERUNNER_ORIGINAL_BUGS
			Actor_Clue_Acquire(kActorMcCoy, kClueBobInterview2, true, kActorMcCoy);  // A bug? Shouldn't the last argument be -1 or kActorBulletBob here?
#else
			Actor_Clue_Acquire(kActorMcCoy, kClueBobInterview2, true, kActorBulletBob);
#endif // BLADERUNNER_ORIGINAL_BUGS
		}
		return;

	case 590: // SHELL CASINGS
		Actor_Says(kActorMcCoy,     4960, 13); // McCoy: Ever see shell casings like these?
		Actor_Says(kActorBulletBob,  250, 30); // Bullet Bob: Oh, where did you find these? They're from an Endor.
		Actor_Says(kActorBulletBob,  260, 33); // Bullet Bob: I ain't seen one of these babies in, hell, maybe two-three years.
		Actor_Says(kActorMcCoy,     4995, 15); // McCoy: Somebody shot up Runciter's shop with one.
		Actor_Says(kActorBulletBob,  270, 32); // Bullet Bob: Sucker had it coming. I'm sure of that.
		Actor_Says(kActorMcCoy,     5005, 11); // McCoy: Why is that?
		Actor_Says(kActorBulletBob,  280, 33); // Bullet Bob: Runciter is always putting on those airs like he's better than everybody else.
		Actor_Says(kActorBulletBob,  290, 30); // Bullet Bob: But he'll do anything for a buck. Sell anything to anybody.
		Actor_Says(kActorBulletBob,  300, 32); // Bullet Bob: That's where I draw the line. If it's suspicious
		Actor_Says(kActorBulletBob,  310, 37); // Bullet Bob: it's out of here. Got no place at Bullet Bob's.
		Actor_Says(kActorMcCoy,     5010, 13); // McCoy: You know anyone who might sell Endor rifles?
		Actor_Says(kActorBulletBob,  320, 37); // Bullet Bob: There's one other guy around here who dips into that stuff.
		Actor_Says(kActorBulletBob,  330, 33); // Bullet Bob: Got a pawn shop down the street. Goes by the name of Izo.
		Actor_Says(kActorBulletBob,  340, 11); // Bullet Bob: But who knows what the real title is.
		Actor_Says(kActorMcCoy,     5015, 16); // McCoy: Izo, huh. Sounds like you don't like him.
		Actor_Modify_Friendliness_To_Other(kActorBulletBob, kActorMcCoy, 3);
		if (Game_Flag_Query(kFlagIzoIsReplicant)) {
			Actor_Says(kActorBulletBob,  350, 32); // Bullet Bob: I've been around here a long time, Ray.
			Actor_Says(kActorBulletBob,  360, 30); // Bullet Bob: It always pisses me off when some dilettante
			Actor_Says(kActorBulletBob,  370, 33); // Bullet Bob: sets up shop and tries to horn in on my action.
			Actor_Says(kActorMcCoy,     5020, 15); // McCoy: How long has Izo been around?
			Actor_Says(kActorBulletBob,  380, 33); // Bullet Bob: He's brand-new. He's a cold son of a bitch, too.
			Actor_Says(kActorBulletBob,  390, 37); // Bullet Bob: I went over to introduce myself when he first moved in and he practically ignored me!
			Actor_Says(kActorBulletBob,  400, 32); // Bullet Bob: Said he was too busy to talk. Don't think I didn't consider kicking his ass, too.
#if BLADERUNNER_ORIGINAL_BUGS
			Actor_Clue_Acquire(kActorMcCoy, kClueBobInterview1, true, kActorMcCoy);  // A bug? Shouldn't the last argument be -1 or kActorBulletBob here?
#else
			Actor_Clue_Acquire(kActorMcCoy, kClueBobInterview1, true, kActorBulletBob);
#endif // BLADERUNNER_ORIGINAL_BUGS
		} else {
			Actor_Says(kActorBulletBob,  410, 32); // Bullet Bob: Izo is okay. He's run the Green Pawn for a while.
			Actor_Says(kActorBulletBob,  420, 30); // Bullet Bob: We're friendly competitors, I guess. But I don't appreciate his politics.
			Actor_Says(kActorMcCoy,     5025, 13); // McCoy: Yeah?
			Actor_Says(kActorBulletBob,  430, 33); // Bullet Bob: Rumors that's all they are.
			Actor_Says(kActorBulletBob,  440, 32); // Bullet Bob: I was out there, Ray. I did my time. Three and a half years worth.
			Actor_Says(kActorBulletBob,  450, 37); // Bullet Bob: And I can tell you: Reps got no business being down here on Earth, okay?
			Actor_Says(kActorMcCoy,     5030, 16); // McCoy: Yeah. Earth don't deserve them.
			Actor_Says(kActorBulletBob,  460, 30); // Bullet Bob: Tell me about it. We got standards to uphold down here.
#if BLADERUNNER_ORIGINAL_BUGS
			Actor_Clue_Acquire(kActorMcCoy, kClueBobInterview2, true, kActorMcCoy);  // A bug? Shouldn't the last argument be -1 or kActorBulletBob here?
#else
			Actor_Clue_Acquire(kActorMcCoy, kClueBobInterview2, true, kActorBulletBob);
#endif // BLADERUNNER_ORIGINAL_BUGS
		}
		break;

	case 600: // HASAN
		Actor_Says(kActorMcCoy,     4965, 11); // McCoy: I hear you've been waving guns around on Animoid Row.
		Actor_Says(kActorBulletBob,  470, 11); // Bullet Bob: Who's been telling you that crap? That lying Egyptian snake oil salesman?
		Actor_Says(kActorMcCoy,     5035, 15); // McCoy: Hasan?
		Actor_Says(kActorBulletBob,  480, 30); // Bullet Bob: That's the one. I should have popped him when I had the chance.
		Actor_Says(kActorBulletBob,  490, 31); // Bullet Bob: You want to know what really happened?
		Actor_Says(kActorBulletBob,  500, 32); // Bullet Bob: I wanted to get my daughter a present for her birthday, right?
		Actor_Says(kActorBulletBob,  510, 33); // Bullet Bob: I bought an Iguana from that crook Hasan.
		Actor_Says(kActorBulletBob,  520, 34); // Bullet Bob: And the damn thing ran out of juice
		Actor_Says(kActorBulletBob,  530, 35); // Bullet Bob: just two days after I plucked down a shitload of chinyen.
		Actor_Says(kActorBulletBob,  540, 36); // Bullet Bob: Son of a bitch wouldn't give me a refund. So I threatened to burn down his shop.
		Actor_Says(kActorMcCoy,     5040, 16); // McCoy: Sometimes you just got to be philosophical about these things.
		Actor_Says(kActorBulletBob,  550, 11); // Bullet Bob: That's exactly what my wife says.
		Actor_Modify_Friendliness_To_Other(kActorBulletBob, kActorMcCoy, -6);
		if (_vm->_cutContent) {
			Actor_Modify_Friendliness_To_Other(kActorHasan, kActorMcCoy, 6);
		}
		Game_Flag_Set(kFlagMcCoyTalkedToBulletBobAboutHasan);
		break;

	case 610: // GOGGLES
		Actor_Says(kActorMcCoy, 4970, 16); // McCoy: You ever seen anything like these?
		if (Actor_Query_Friendliness_To_Other(kActorBulletBob, kActorMcCoy) < 50) {
			Actor_Says(kActorBulletBob,  700, 11); // Bullet Bob: Looks like goggles.
			Actor_Says(kActorMcCoy,     5070, 11); // McCoy: Would a Replicant need something like this?
			Actor_Says(kActorBulletBob,  710, 11); // Bullet Bob: You're asking me about what a skin-job would need?
			Actor_Says(kActorMcCoy,     5075, 15); // McCoy: Hey, pal.
			Actor_Says(kActorBulletBob,  720, 30); // Bullet Bob: Ask one of your friends.
			Actor_Says(kActorMcCoy,     5080, 11); // McCoy: Just answer the question.
			Actor_Says(kActorBulletBob,  730, 37); // Bullet Bob: I hear they give out retirement bonuses to civilians that bag Reps.
#if BLADERUNNER_ORIGINAL_BUGS
			Actor_Clue_Acquire(kActorMcCoy, kClueBobInterview1, true, kActorMcCoy);  // A bug? Shouldn't the last argument be -1 or kActorBulletBob here?
#else
			Actor_Clue_Acquire(kActorMcCoy, kClueBobInterview1, true, kActorBulletBob);
#endif // BLADERUNNER_ORIGINAL_BUGS
		} else {
			Actor_Says(kActorBulletBob,  560, 37); // Bullet Bob: Those are real beauties. Radiation goggles from the Off-World Wars. The 2017 model.
			Actor_Says(kActorMcCoy,     5070, 13); // McCoy: Would a Replicant need something like this?
			Actor_Says(kActorBulletBob,  570, 36); // Bullet Bob: Hell, the skin-jobs were the only ones who wore 'em.
			Actor_Says(kActorBulletBob,  580, 37); // Bullet Bob: The humans weren't allowed to get that close to the flash point.
			Actor_Says(kActorBulletBob,  590, 31); // Bullet Bob: U.N. Command figured it out, because the Rep skin held up...
			Actor_Says(kActorBulletBob,  600, 32); // Bullet Bob: but their eyes melted inside two kilometers from Ground Zero.
			Actor_Says(kActorBulletBob,  610, 30); // Bullet Bob: Where did you get them from?
			Actor_Says(kActorMcCoy,     5050, 16); // McCoy: Izo.
			Actor_Says(kActorBulletBob,  620, 35); // Bullet Bob: I knew it! I knew that son of a bitch was dirty as soon as he moved in.
			Actor_Says(kActorBulletBob,  630, 35); // Bullet Bob: He's got that cold Rep way about him. You know what I mean?
			Actor_Says(kActorMcCoy,     5055, 11); // McCoy: Don't jump to conclusions.
			Actor_Says(kActorBulletBob,  640, 36); // Bullet Bob: Sure, sure. Hey, I'd love to be there when you take him down.
			Actor_Says(kActorBulletBob,  650, 35); // Bullet Bob: I could get a photo of it. Put it up on my Wall of Fame.
			Actor_Says(kActorBulletBob,  660, 30); // Bullet Bob: Davey is up there. So is Gaff. Want to see?
			Actor_Says(kActorMcCoy,     5060, 13); // McCoy: Some other time.
#if BLADERUNNER_ORIGINAL_BUGS
			Actor_Clue_Acquire(kActorMcCoy, kClueGogglesReplicantIssue, true, kActorMcCoy);  // A bug? Shouldn't the last argument be -1 or kActorBulletBob here?
#else
			Actor_Clue_Acquire(kActorMcCoy, kClueGogglesReplicantIssue, true, kActorBulletBob);
#endif // BLADERUNNER_ORIGINAL_BUGS
		}
		break;

	case 620: // HOLDEN'S BADGE
		Actor_Says(kActorMcCoy,     4985, 11); // McCoy: How'd you like to be a hero?
		Actor_Says(kActorBulletBob,  850, 35); // Bullet Bob: Yeah? How's that?
		Actor_Says(kActorMcCoy,     5105, 13); // McCoy: Your pal Dave Holden lost his badge.
		Actor_Says(kActorMcCoy,     5110, 11); // McCoy: You turn it in, the department would owe you a favor.
		Actor_Says(kActorBulletBob,  860, 30); // Bullet Bob: And what's in it for you?
		Actor_Says(kActorMcCoy,     5115, 16); // McCoy: You're a smart guy, Bob. You'll think of something.
		Actor_Says(kActorBulletBob,  870, 31); // Bullet Bob: I could throw a few chinyen your way...
		Actor_Says(kActorMcCoy,     5120, 15); // McCoy: That'll work.
		Actor_Says(kActorBulletBob,  880, 34); // Bullet Bob: They really ought to pay you guys better.
		Actor_Clue_Acquire(kActorBulletBob, kClueHoldensBadge, true, kActorMcCoy);
		Actor_Modify_Friendliness_To_Other(kActorBulletBob, kActorMcCoy, 8);
		if (Query_Difficulty_Level() != kGameDifficultyEasy) {
			Global_Variable_Increment(kVariableChinyen, 60);
		}
		break;

	case 630: // DONE
		Actor_Says(kActorMcCoy, 1315, 12); // McCoy: Thanks for your time.
		break;

	case 1280: // AMMO
		Actor_Says(kActorMcCoy, 9040, 16); // McCoy: You got any decent ammo for my piece?
		if (!Game_Flag_Query(kFlagRC04BobTalkAmmo)) {
			Actor_Says(kActorBulletBob, 2080, 30); // Bullet Bob: You mean the good stuff.
			Actor_Says(kActorBulletBob, 2090, 37); // Bullet Bob: 'Cause I know them police-issue spitballs just don't cut it.
			Actor_Says(kActorMcCoy,     9045, 14); // McCoy: Not these days.
			Actor_Says(kActorBulletBob, 2100, 32); // Bullet Bob: Yeah, I know how it is.
			Actor_Says(kActorBulletBob, 2110, 37); // Bullet Bob: If you want to drop a skin-job, you need a little extra oomph!
			Game_Flag_Set(kFlagRC04BobTalkAmmo);
		}
		Actor_Says(kActorBulletBob, 2120, 31); // Bullet Bob: I got a few clips around here somewhere, if you got the chinyen for it.
		if (Global_Variable_Query(kVariableChinyen) > 40
		 || Query_Difficulty_Level() == kGameDifficultyEasy
		) {
			Actor_Says(kActorMcCoy, 4940, 13); // McCoy: Okay, let's have it.
			if (Query_Difficulty_Level() != kGameDifficultyEasy) {
				Global_Variable_Decrement(kVariableChinyen, 40);
			}
			Item_Pickup_Spin_Effect(kModelAnimationAmmoType01, 405, 192);
			Give_McCoy_Ammo(1, 24);
		} else {
			Actor_Says(kActorMcCoy, 125, 13); // McCoy: I'm a little strapped for cash right now.
			Actor_Modify_Friendliness_To_Other(kActorBulletBob, kActorMcCoy, -2);
		}
		break;

	case 1310: // VOIGT-KAMPFF
		Actor_Says(kActorMcCoy, 4980, 11); // McCoy: How'd you feel about taking a little test?
		if (Actor_Query_Friendliness_To_Other(kActorBulletBob, kActorMcCoy) > 49) {
			Actor_Says(kActorBulletBob,  740, 37); // Bullet Bob: Test? You mean the Voigt-Kampff?
			Actor_Says(kActorMcCoy,     5085, 16); // McCoy: Yeah, I don't think you're a Rep, but I want to--
			Actor_Says(kActorBulletBob,  750, 37); // Bullet Bob: Hey, hey, hey. You don't have to explain. I always wanted to see the machine in action.
			Actor_Says(kActorBulletBob,  760, 37); // Bullet Bob: Go on, set it up.
			Voight_Kampff_Activate(kActorBulletBob, 50);
			Actor_Modify_Friendliness_To_Other(kActorBulletBob, kActorMcCoy, 3);
			Actor_Says(kActorBulletBob,  810, 37); // Bullet Bob: So that's it?
			Actor_Says(kActorMcCoy,     5025, 13); // McCoy: Yeah?
			Actor_Says(kActorBulletBob,  820, 32); // Bullet Bob: Since you didn't shoot me, I must have registered okay.
			Actor_Says(kActorMcCoy,     5100, 11); // McCoy: Yeah, it was touch-and-go there for a while.
			Actor_Says(kActorBulletBob,  830, 31); // Bullet Bob: Well, you know, I think about skin-jobs all the time.
			Actor_Says(kActorBulletBob,  840, 35); // Bullet Bob: Try to get into their heads. Maybe that's what the deal is.
		} else {
			Actor_Says(kActorBulletBob,  770, 36); // Bullet Bob: You mean the Voigt-Kampff?
			Actor_Says(kActorBulletBob,  780, 36); // Bullet Bob: You come into my shop and you want to V-K me?
			Actor_Says(kActorMcCoy,     5090, 16); // McCoy: Relax, I just wa--
			Actor_Says(kActorBulletBob,  790, 36); // Bullet Bob: It's funny. You thinking I'm a Rep.
			Actor_Says(kActorBulletBob,  800, 35); // Bullet Bob: Set up the test. We'll see what's what around here.
			Voight_Kampff_Activate(kActorBulletBob, 50);
			Actor_Says(kActorBulletBob,  810, 30); // Bullet Bob: So that's it?
			Actor_Says(kActorMcCoy,     5025, 13); // McCoy: Yeah?
			Actor_Says(kActorBulletBob,  820, 31); // Bullet Bob: Since you didn't shoot me, I must have registered okay.
			Actor_Says(kActorMcCoy,     5100, 15); // McCoy: Yeah, it was touch-and-go there for a while.
			Actor_Says(kActorBulletBob,  830, 34); // Bullet Bob: Well, you know, I think about skin-jobs all the time.
			Actor_Says(kActorBulletBob,  840, 34); // Bullet Bob: Try to get into their heads. Maybe that's what the deal is.
		}
		break;
	}
}

bool SceneScriptRC04::ClickedOnActor(int actorId) {
	if (Player_Query_Combat_Mode()) {
		return false;
	}

	if (actorId == kActorBulletBob) {
		if ( Global_Variable_Query(kVariableChapter) == 2
		 && !Game_Flag_Query(kFlagRC04McCoyShotBob)
		) {
			Loop_Actor_Walk_To_Waypoint(kActorMcCoy, 104, 0, false, false);
			Actor_Face_Actor(kActorMcCoy, kActorBulletBob, true);
			if ( Game_Flag_Query(kFlagRC04Entered)
			 && !Game_Flag_Query(kFlagRC04BobTalk2)
			 &&  Actor_Query_Friendliness_To_Other(kActorBulletBob, kActorMcCoy) > 45
			) {
				Actor_Says(kActorBulletBob,   30, 30); // Bullet Bob: How come I never heard of you?
				Actor_Says(kActorMcCoy,     4875, 13); // McCoy: I just got assigned.
				Actor_Says(kActorBulletBob,   80, 31); // Bullet Bob: Yeah, I'm kind of buddies with Davey Holden. How's he doing?
				Actor_Says(kActorMcCoy,     4900, 15); // McCoy: Word is he's gonna be okay.
				Actor_Says(kActorBulletBob,   90, 33); // Bullet Bob: That skin-job must have been pretty damn quick getting by Davey like that.
				Actor_Says(kActorBulletBob,  100, 34); // Bullet Bob: He's the best in the business.
				Actor_Says(kActorMcCoy,     4905, 15); // McCoy: He'll be back on the job soon.
				Game_Flag_Set(kFlagRC04BobTalk2);
			} else if ( Game_Flag_Query(kFlagRC04Entered)
					&& !Game_Flag_Query(kFlagRC04BobTalk1)
					&& Actor_Query_Friendliness_To_Other(kActorBulletBob, kActorMcCoy) < 45
			) {
				Actor_Says(kActorBulletBob,   40, 30); // Bullet Bob: I heard of you.
				Actor_Says(kActorMcCoy,     4880, 13); // McCoy: Is that right?
				Actor_Says(kActorBulletBob,   50, 35); // Bullet Bob: You ain't done much.
				Actor_Says(kActorMcCoy,     4875, 16); // McCoy: I just got assigned.
				Actor_Says(kActorBulletBob,   60, 36); // Bullet Bob: Guys like you get hurt real easy.
				Actor_Says(kActorMcCoy,     4890, 13); // McCoy: Meaning what exactly?
				Actor_Says(kActorBulletBob,   70, 33); // Bullet Bob: It ain't like you're a real Blade Runner. Not like Steele or Gaff.
				Actor_Says(kActorMcCoy,     4895, 16); // McCoy: They get hurt sometimes too. It's a dangerous business.
				Actor_Modify_Friendliness_To_Other(kActorBulletBob, kActorMcCoy, -5);
				Game_Flag_Set(kFlagRC04BobTalk1);
			} else if ( Actor_Query_Friendliness_To_Other(kActorBulletBob, kActorMcCoy) > 51
					&& !Game_Flag_Query(kFlagRC04BobTalk3)
			) {
				Actor_Says(kActorBulletBob, 1870, 30); // Bullet Bob: Hey, check out the man's database.
				Actor_Says(kActorBulletBob, 1880, 30); // Bullet Bob: Is that the standard model?
				Actor_Says(kActorMcCoy,     8960, 13); // McCoy: What the KIA? Yeah, it's Department-issue.
				Actor_Says(kActorBulletBob, 1890, 36); // Bullet Bob: I could fix it up for you.
				Actor_Says(kActorBulletBob, 1900, 35); // Bullet Bob: Make it not quite so freaking standard, if you get my drift.
				Actor_Says(kActorMcCoy,     8965, 16); // McCoy: What for?
				Actor_Says(kActorBulletBob, 1920, 36); // Bullet Bob: Hey, Ray, relax! I'm your pal, ain't I?
				Actor_Says(kActorBulletBob, 1930, 33); // Bullet Bob: You don't have to step soft around me.
				Actor_Says(kActorBulletBob, 1940, 36); // Bullet Bob: The department's been infiltrated, right?
				Actor_Says(kActorBulletBob, 1950, 30); // Bullet Bob: That's the only way Davey could have been taken out like he was.
				Actor_Says(kActorMcCoy,     8970, 13); // McCoy: Holden is good. No question.
				Actor_Says(kActorBulletBob, 1960, 33); // Bullet Bob: When you hook into the mainframe you always give them everything.
				Actor_Says(kActorBulletBob, 1970, 30); // Bullet Bob: But you don't always want them to know what you know.
				Actor_Says(kActorBulletBob, 1980, 36); // Bullet Bob: 'Cause then they'd know, you know?
				Delay(1000);
				Actor_Says(kActorBulletBob, 2010, 35); // Bullet Bob: What do you say?
				if (Global_Variable_Query(kVariableChinyen) > 50
				 || Query_Difficulty_Level() == kGameDifficultyEasy
				) {
					Actor_Says(kActorMcCoy, 8975, 16); // McCoy: Sounds like a plan. Set it up.
					if (Query_Difficulty_Level() != kGameDifficultyEasy) {
						Global_Variable_Decrement(kVariableChinyen, 50);
					}
					Delay(3000);
					Item_Pickup_Spin_Effect(kModelAnimationDNADataDisc, 405, 192);
					Actor_Says(kActorBulletBob, 2030, 30); // Bullet Bob: There you go better than new.
					Game_Flag_Set(kFlagKIAPrivacyAddon);
				} else {
					Actor_Says(kActorMcCoy,     8980, 16); // McCoy: No, thanks, I'm saving for a rainy day.
					Actor_Says(kActorBulletBob, 2040, 30); // Bullet Bob: Suit yourself. Just remember: Big Mother's watching.
					Actor_Says(kActorMcCoy,     8985, 15); // McCoy: You mean Big Brother.
					Actor_Says(kActorBulletBob, 2050, 33); // Bullet Bob: Him, too.
				}
				Game_Flag_Set(kFlagRC04BobTalk3);
			} else if ( Actor_Clue_Query(kActorMcCoy, kClueLabShellCasings)
			        ||  Actor_Clue_Query(kActorMcCoy, kClueShellCasings)
			        ||  Actor_Clue_Query(kActorMcCoy, kClueRadiationGoggles)
			        ||  Actor_Clue_Query(kActorMcCoy, kClueHoldensBadge)
			        ||  Actor_Clue_Query(kActorMcCoy, kClueHasanInterview)
			        || !Game_Flag_Query(kFlagRC04BobTalkAmmo)
			) {
				dialogueWithBulletBob();
			} else {
				Actor_Says(kActorBulletBob, 1820, 30); // Bullet Bob: You want to make it as a Blade Runner, you ought to do your own investigations.
			}
			return true;
		}

		if (Game_Flag_Query(kFlagRC04McCoyShotBob)) {
			Actor_Face_Actor(kActorMcCoy, kActorBulletBob, true);
			if (Actor_Clue_Query(kActorMcCoy, kClueVKBobGorskyReplicant)) {
				Actor_Says(kActorMcCoy, 8590, -1); // McCoy: Not the talkative type.
			} else {
				Actor_Voice_Over(2100, kActorVoiceOver); // Mainframe: I'd crossed the line.
				Actor_Voice_Over(2110, kActorVoiceOver); // Mainframe: Bob was a psychopath but I was almost sure he wasn't a Replicant.
			}
			return true;
		}
	}
	return false;
}

bool SceneScriptRC04::ClickedOnItem(int itemId, bool a2) {
	return false;
}

bool SceneScriptRC04::ClickedOnExit(int exitId) {
	if (exitId == 0) {
		if (!Loop_Actor_Walk_To_XYZ(kActorMcCoy, 45.0f, 0.15f, 68.0f, 0, true, false, false)) {
			Game_Flag_Set(kFlagRC04toRC03);
			Set_Enter(kSetRC03, kSceneRC03);
		}
		return true;
	}
	return false;
}

bool SceneScriptRC04::ClickedOn2DRegion(int region) {
	return false;
}

void SceneScriptRC04::SceneFrameAdvanced(int frame) {
}

void SceneScriptRC04::ActorChangedGoal(int actorId, int newGoal, int oldGoal, bool currentSet) {
}

void SceneScriptRC04::PlayerWalkedIn() {
	Loop_Actor_Walk_To_Waypoint(kActorMcCoy, 103, 0, false, false);

	if ( Global_Variable_Query(kVariableChapter) == 2
	 && !Game_Flag_Query(kFlagRC04Entered)
	 && !Player_Query_Combat_Mode()
	) {
		Actor_Says(kActorBulletBob, 0, 31); // Bullet Bob: That a .45 blaster under your coat or you're just happy to be here?
		Loop_Actor_Walk_To_Waypoint(kActorMcCoy, 104, 0, false, false);
		Actor_Face_Actor(kActorMcCoy, kActorBulletBob, true);
		Actor_Says(kActorMcCoy,     4865, 13); // McCoy: You got good eyes.
		Actor_Says(kActorBulletBob,   10, 32); // Bullet Bob: They ought to be they're the only organs of mine that survived the Phobos wars intact.
		Actor_Says(kActorMcCoy,     4870, 16); // McCoy: Ray McCoy, Rep Detect.
		Actor_Says(kActorBulletBob,   20, 31); // Bullet Bob: Blade Runner, huh?
		Game_Flag_Set(kFlagRC04Entered);
		return; //true;
	}

	if ( Global_Variable_Query(kVariableChapter) == 4
	 && !Game_Flag_Query(kFlagRC04McCoyShotBob)
	 && !Game_Flag_Query(kFlagNotUsed306)
	) {
		Actor_Says(kActorDispatcher,  40,  3); // Dispatcher: Attention all units. Be on the lookout for Ray McCoy. Last seen in the Fourth Sector. Subject is armed and extremely dangerous. Repeat armed and extremely dangerous.
		Actor_Says(kActorBulletBob,  890, 37); // Bullet Bob: Scum sucking skin-jobs everywhere!
		Actor_Set_Goal_Number(kActorBulletBob, kGoalBulletBobShootMcCoy);
	}

	Game_Flag_Set(kFlagRC04Entered);

	//return false;
}

void SceneScriptRC04::PlayerWalkedOut() {
	Game_Flag_Reset(kFlagRC04McCoyWarned);
}

void SceneScriptRC04::DialogueQueueFlushed(int a1) {
}

} // End of namespace BladeRunner
