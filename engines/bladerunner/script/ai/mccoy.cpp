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

#include "bladerunner/script/ai_script.h"

namespace BladeRunner {

AIScriptMcCoy::AIScriptMcCoy(BladeRunnerEngine *vm) : AIScriptBase(vm) {
	_animationLoopCounter = 0;
	_animationLoopLength = 0;
	_animationLoopDirection = 0;
	_animationLoopFrameMin = 0;
	_animationLoopFrameMax = 0;
	_animationStateNextSpecial = 0;
	_animationNextSpecial = 0;
	_nextSoundId = -1;
	_NR10SteeleShooting = false;
	_fallSpeed = 0.0f;
	_fallHeightCurrent = 0.0f;
	_fallHeightTarget = 0.0f;
}

void AIScriptMcCoy::Initialize() {
	_animationState = 0;
	_animationFrame = 0;
	_animationLoopCounter = 0;
	_animationLoopLength = 30;
	_animationLoopDirection = 1;
	_animationLoopFrameMin = 0;
	_animationLoopFrameMax = 3;
	_animationStateNextSpecial = 3;
	_animationNextSpecial = kModelAnimationMcCoyProtestingTalk;
	_nextSoundId = -1;
	_NR10SteeleShooting = false;
	_fallSpeed = 0;
	Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyDefault);
}

bool AIScriptMcCoy::Update() {
	if (_nextSoundId != -1) {
		Sound_Play(_nextSoundId, 100, 0, 0, 50);
		_nextSoundId = -1;
	}

	switch (Actor_Query_Goal_Number(kActorMcCoy)) {
	case kGoalMcCoyBB11PrepareToRunAway:
		Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyBB11RunAway);
		return true;

	case kGoalMcCoyBB11RunAway:
		if ( Actor_Query_Inch_Distance_From_Waypoint(kActorMcCoy, 316) < 36
		 && !Game_Flag_Query(kFlagBB11SadikPunchedMcCoy)
		) {
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeHit);
			Game_Flag_Set(kFlagBB11SadikPunchedMcCoy);
		}

		if (Actor_Query_Inch_Distance_From_Waypoint(kActorMcCoy, 316) < 4) {
			Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyBB11GetCaught);
		}
		return true;

	case kGoalMcCoyNRxxStandUp:
		Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyDefault);
		if (Player_Query_Current_Set() == kSetNR03) {
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, -166.0f, -70.19f, -501.0f, 0, false, false, false);
			Actor_Face_Heading(kActorMcCoy, 300, false);
		} else {
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, -461.0f, 0.0f, -373.0f, 0, false, false, false);
		}
		Player_Gains_Control();
		return true;

	case kGoalMcCoyNR01LayDrugged:
		if (Global_Variable_Query(kVariableNR01GetUpCounter) >= 13) {
			Global_Variable_Set(kVariableNR01GetUpCounter, 500);
		} if (Global_Variable_Query(kVariableNR01GetUpCounter) > 0) {
			Global_Variable_Decrement(kVariableNR01GetUpCounter, 1);
		}
		break;

	case kGoalMcCoyNR04PassOut:
		Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyDefault);
		Actor_Set_Goal_Number(kActorEarlyQ, kGoalEarlyQNR04TakeDisk);
		return true;

	case kGoalMcCoyNR10Fall:
		fallDown();
		break;

	case kGoalMcCoyUG15Fall:
		fallDown();
		break;

	case kGoalMcCoyUG15Die:
		Actor_Retired_Here(kActorMcCoy, 12, 48, true, -1);
		Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyGone);
		break;
	}
	return false;
}

void AIScriptMcCoy::TimerExpired(int timer) {
}

void AIScriptMcCoy::CompletedMovementTrack() {
}

void AIScriptMcCoy::ReceivedClue(int clueId, int fromActorId) {
	switch (clueId) {
	case kClueDispatchHitAndRun: // added case for cut content
		// fall through
	case kClueChopstickWrapper:
		// fall through
	case kClueSushiMenu:
		Spinner_Set_Selectable_Destination_Flag(kSpinnerDestinationChinatown, true);
		break;

	case kClueDragonflyEarring:
		// fall through
	case kClueBombingSuspect:
		Spinner_Set_Selectable_Destination_Flag(kSpinnerDestinationAnimoidRow, true);
		break;

	case kClueKingstonKitchenBox1:
		// fall through
	case kClueKingstonKitchenBox2:
		if (Query_Difficulty_Level() == kGameDifficultyEasy) {
			Spinner_Set_Selectable_Destination_Flag(kSpinnerDestinationAnimoidRow, true);
		}
		break;

	case kClueDragonflyCollection:
		Spinner_Set_Selectable_Destination_Flag(kSpinnerDestinationNightclubRow, true);
		break;

	case kClueHysteriaToken:
		// fall through
	case kClueCarRegistration1:
		// fall through
	case kClueCarRegistration2:
		// fall through
	case kClueCarRegistration3:
		// fall through
	case kClueLichenDogWrapper:
		Spinner_Set_Selectable_Destination_Flag(kSpinnerDestinationHysteriaHall, true);
		Spinner_Set_Selectable_Destination_Flag(kSpinnerDestinationNightclubRow, true);
		break;

	case kClueWeaponsCache:
		// fall through
	case kClueWeaponsOrderForm:
		// fall through
	case kClueShippingForm:
		// fall through
	case kCluePoliceIssueWeapons:
		Global_Variable_Increment(kVariableCorruptedGuzzaEvidence, 1);
		break;

	case kClueFolder:
		Global_Variable_Increment(kVariableCorruptedGuzzaEvidence, 5);
		break;

	case kClueHomelessManKid:
		// fall through
	case kClueOriginalRequisitionForm:
		Global_Variable_Increment(kVariableCorruptedGuzzaEvidence, 3);
		break;

	case kClueScaryChair:
		// fall through
	case kClueIzosStashRaided:
		Global_Variable_Increment(kVariableCorruptedGuzzaEvidence, 2);
		break;

	case kClueDNATyrell:
		// fall through
	case kClueDNASebastian:
		// fall through
	case kClueDNAChew:
		// fall through
	case kClueDNAMoraji:
		// fall through
	case kClueDNALutherLance:
		// fall through
	case kClueDNAMarcus:
		Global_Variable_Increment(kVariableDNAEvidence, 1);
		break;
	}

	if ( Global_Variable_Query(kVariableCorruptedGuzzaEvidence) > 6
	 &&  Global_Variable_Query(kVariableChapter) > 3
	 && !Actor_Clue_Query(kActorMcCoy, kClueGuzzaFramedMcCoy)
	) {
		Delay(500);
		Actor_Voice_Over(3320, kActorVoiceOver); // Mainframe: A little light started to flicker in my brain. Guzza was the one measuring me for the frame.
		switch (clueId) {
		case kClueWeaponsCache:
			// fall through
		case kClueWeaponsOrderForm:
			// fall through
		case kClueGuzzasCash:
			// fall through
		case kCluePoliceIssueWeapons:
			// fall through
		case kClueIzosStashRaided:
			// fall through
		case kClueOriginalRequisitionForm:
			Actor_Voice_Over(3340, kActorVoiceOver); // Mainframe: He set up a Black Market network...
			Actor_Voice_Over(3350, kActorVoiceOver); // Mainframe: Moving surplus LPD weapons through Izo and the Green Pawn.
			Actor_Voice_Over(3360, kActorVoiceOver); // Mainframe: Nothing wrong with a little graft...
			Actor_Voice_Over(3370, kActorVoiceOver); // Mainframe: but when I started stirring the pot...
			Actor_Voice_Over(3380, kActorVoiceOver); // Mainframe: the heat got too intense in the kitchen and the sweat started coming.
			break;

		case kClueHomelessManKid:
			Actor_Voice_Over(3330, kActorVoiceOver); // Mainframe: The homeless guy only confirmed my worse suspicions.
			break;

		case kClueScaryChair:
			Actor_Voice_Over(3390, kActorVoiceOver); // Mainframe: I used to think Guzza was a pretty smart guy...
			Actor_Voice_Over(3400, kActorVoiceOver); // Mainframe: but letting Baker and Holloway work their bogus interrogation deal with all that LPD equipment was a suicide move.
			Actor_Voice_Over(3420, kActorVoiceOver); // Mainframe: And those two morons weren't sharp enough to keep the stuff out of my sight.
			break;
		}

		Actor_Clue_Acquire(kActorMcCoy, kClueGuzzaFramedMcCoy, true, -1);

		if (clueId == kClueFolder) {
			// if McCoy just got the folder
			Actor_Voice_Over(2780, kActorVoiceOver); // Mainframe: When my stomach stopped churning, I realized I had everything I needed to force the bastard to play ball with me.
			Actor_Voice_Over(2800, kActorVoiceOver); // Mainframe: I knew that he would listen to reason, if applied with serious pressure.
			Actor_Voice_Over(2810, kActorVoiceOver); // Mainframe: All I needed to do was call him.
		} else if (Actor_Clue_Query(kActorMcCoy, kClueFolder)) {
			// if McCoy already had the folder
			Actor_Voice_Over(3430, kActorVoiceOver); // Mainframe: The info I'd gotten from Luther and Lance finally made some sense.
			Actor_Voice_Over(3440, kActorVoiceOver); // Mainframe: The Reps must had found out about Guzza's career's sidelines...
			Actor_Voice_Over(3450, kActorVoiceOver); // Mainframe: and pressured him to set me up in order to get me off their backs.
			Actor_Voice_Over(3460, kActorVoiceOver); // Mainframe: This nasty business he was only too willing to do.
			Actor_Voice_Over(3470, kActorVoiceOver); // Mainframe: But I wasn't gonna eat crow that easily.
			Actor_Voice_Over(3480, kActorVoiceOver); // Mainframe: Yeah, what a difference a day makes.
			Actor_Voice_Over(3490, kActorVoiceOver); // Mainframe: It was time to have my own little fun with Guzza.
			Actor_Voice_Over(3500, kActorVoiceOver); // Mainframe: All I had to do was place a call and make the worm squirm.
		} else {
			// if McCoy never got the folder
			Actor_Voice_Over(3510, kActorVoiceOver); // Mainframe: The picture was still a little blurry.
			Actor_Voice_Over(3520, kActorVoiceOver); // Mainframe: Guzza must have had some connection to Clovis and his group...
			Actor_Voice_Over(3530, kActorVoiceOver); // Mainframe: even though he always claimed to have hated Replicants.
			Actor_Voice_Over(3540, kActorVoiceOver); // Mainframe: He wouldn't have helped out any skin-jobs unless they had something really juicy to lord over him.
		}
	}
}

void AIScriptMcCoy::ClickedByPlayer() {
}

void AIScriptMcCoy::EnteredScene(int sceneId) {
}

void AIScriptMcCoy::OtherAgentEnteredThisScene(int otherActorId) {
}

void AIScriptMcCoy::OtherAgentExitedThisScene(int otherActorId) {
}

void AIScriptMcCoy::OtherAgentEnteredCombatMode(int otherActorId, int combatMode) {
}

void AIScriptMcCoy::ShotAtAndMissed() {
}

bool AIScriptMcCoy::ShotAtAndHit() {
	return false;
}

void AIScriptMcCoy::Retired(int byActorId) {
	if (byActorId == kActorSteele && Actor_Query_In_Set(kActorSteele, kSetHF06)) {
		if (Actor_Query_In_Set(kActorDektora, kSetHF06)
		 && Actor_Query_Goal_Number(kActorDektora) != kGoalDektoraGone) {
			Non_Player_Actor_Combat_Mode_On(kActorSteele, kActorCombatStateUncover, true, kActorDektora, 15, kAnimationModeCombatIdle, kAnimationModeCombatWalk, kAnimationModeCombatRun, 0, 0, 100, 25, 300, false);
		} else if (Actor_Query_In_Set(kActorLucy, kSetHF06)
		        && Actor_Query_Goal_Number(kActorLucy) != kGoalLucyGone
		) {
			Non_Player_Actor_Combat_Mode_On(kActorSteele, kActorCombatStateUncover, true, kActorLucy, 15, kAnimationModeCombatIdle, kAnimationModeCombatWalk, kAnimationModeCombatRun, 0, 0, 100, 25, 300, false);
		}
	}

	if (Actor_Query_In_Set(kActorMcCoy, kSetHF05)
	 && Actor_Query_In_Set(kActorOfficerLeary, kSetHF05)
	 && Actor_Query_In_Set(kActorDektora, kSetHF05)
	 && Actor_Query_Goal_Number(kActorDektora) != kGoalDektoraGone
	) {
		Non_Player_Actor_Combat_Mode_On(kActorOfficerLeary, kActorCombatStateUncover, true, kActorDektora, 4, kAnimationModeCombatIdle, kAnimationModeCombatWalk, kAnimationModeCombatRun, 0, 0, 100, 25, 300, false);
	}

	if (Actor_Query_In_Set(kActorMcCoy, kSetHF05)
	 && Actor_Query_In_Set(kActorOfficerGrayford, kSetHF05)
	 && Actor_Query_In_Set(kActorDektora, kSetHF05)
	 && Actor_Query_Goal_Number(kActorDektora) != kGoalDektoraGone
	) {
		Non_Player_Actor_Combat_Mode_On(kActorOfficerGrayford, kActorCombatStateUncover, true, kActorDektora, 4, kAnimationModeCombatIdle, kAnimationModeCombatWalk, kAnimationModeCombatRun, 0, 0, 100, 25, 300, false);
	}

	if (Actor_Query_In_Set(kActorMcCoy, kSetHF05)
	 && Actor_Query_In_Set(kActorOfficerLeary, kSetHF05)
	 && Actor_Query_In_Set(kActorLucy, kSetHF05)
	 && Actor_Query_Goal_Number(kActorLucy) != kGoalLucyGone
	) {
		Non_Player_Actor_Combat_Mode_On(kActorOfficerLeary, kActorCombatStateUncover, true, kActorLucy, 4, kAnimationModeCombatIdle, kAnimationModeCombatWalk, kAnimationModeCombatRun, 0, 0, 100, 25, 300, false);
	}

	if (Actor_Query_In_Set(kActorMcCoy, kSetHF05)
	 && Actor_Query_In_Set(kActorOfficerGrayford, kSetHF05)
	 && Actor_Query_In_Set(kActorLucy, kSetHF05)
	 && Actor_Query_Goal_Number(kActorLucy) != kGoalLucyGone
	) {
		Non_Player_Actor_Combat_Mode_On(kActorOfficerGrayford, kActorCombatStateUncover, true, kActorLucy, 4, kAnimationModeCombatIdle, kAnimationModeCombatWalk, kAnimationModeCombatRun, 0, 0, 100, 25, 300, false);
	}
}

int AIScriptMcCoy::GetFriendlinessModifierIfGetsClue(int otherActorId, int clueId) {
	return 0;
}

bool AIScriptMcCoy::GoalChanged(int currentGoalNumber, int newGoalNumber) {
	unsigned int affectionTowards;

	switch (newGoalNumber) {
	case kGoalMcCoyDefault:
		return true;

	case kGoalMcCoyDodge:
		dodge();
		return true;

	case 2:
		_animationFrame = 0;
		_animationState = 47;
		return true;

	case kGoalMcCoyBB11GetUp:
		Actor_Set_At_Waypoint(kActorMcCoy, 315, 263);
		_animationState = 53;
		_animationFrame = Slice_Animation_Query_Number_Of_Frames(18) - 1;
		Actor_Set_Invisible(kActorMcCoy, false);
		return true;

	case kGoalMcCoyBB11RunAway:
		Async_Actor_Walk_To_Waypoint(kActorMcCoy, 316, 0, true);
		return true;

	case kGoalMcCoyBB11GetCaught:
		Actor_Face_Actor(kActorMcCoy, kActorSadik, true);
		return true;

	case kGoalMcCoyNRxxSitAtTable:
		Player_Loses_Control();
		Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeIdle);
		return true;

	case kGoalMcCoyNR01ThrownOut:
		Actor_Put_In_Set(kActorMcCoy, kSetNR01);
		Actor_Set_At_XYZ(kActorMcCoy, -204.0f, 24.0f, -817.0f, 256);
		Actor_Set_Invisible(kActorMcCoy, false);
		if (Game_Flag_Query(kFlagNR01McCoyIsDrugged)) {
			Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyNR01LayDrugged);
		} else {
			_animationState = 53;
			_animationFrame = Slice_Animation_Query_Number_Of_Frames(18) - 1;
			Actor_Set_Invisible(kActorMcCoy, false);
		}
		return true;

	case kGoalMcCoyNR01GetUp:
		Actor_Face_Heading(kActorMcCoy, 512, false);
		Actor_Face_Heading(kActorMcCoy, 768, true);
		if (Random_Query(0, 1)) {
			Actor_Change_Animation_Mode(kActorMcCoy, 18);
		} else {
			Actor_Change_Animation_Mode(kActorMcCoy, 16);
		}
		Delay(150);
		Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeIdle);
		Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyDefault);
		Player_Gains_Control();
		return true;

	case kGoalMcCoyNR01LayDrugged:
		Global_Variable_Set(kVariableNR01GetUpCounter, 0);
		Player_Set_Combat_Mode_Access(false);
		Player_Gains_Control();
		Scene_Exits_Disable();
		_animationState = 68;
		_animationFrame = Slice_Animation_Query_Number_Of_Frames(18) - 1;
		return true;

	case kGoalMcCoyNR04Drink:
		Actor_Change_Animation_Mode(kActorMcCoy, 75);
		return true;

	case kGoalMcCoyNR11Shoot:
		_NR10SteeleShooting = Actor_Query_Goal_Number(kActorSteele) == kGoalSteeleNR11Shoot;
		Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeCombatAttack);
		return true;

	case kGoalMcCoyNR10Fall:
		Player_Set_Combat_Mode(false);
		Preload(kModelAnimationMcCoyFallsOnHisBack);
		Set_Enter(kSetNR10, kSceneNR10);
		Player_Loses_Control();
		Actor_Force_Stop_Walking(kActorMcCoy);
		Actor_Put_In_Set(kActorMcCoy, kSetNR10);
		Actor_Set_At_XYZ(kActorMcCoy, 14.0f, 110.84f, -300.0f, 926);
		Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeDie);
		_animationState = 27;
		_animationFrame = 0;
		_fallHeightTarget = 2.84f;
		_fallHeightCurrent = 110.84f;
		_fallSpeed = -6.0f;
		return true;

	case kGoalMcCoyUG07Caught:
		_animationState = 62;
		_animationFrame = 0;
		return true;

	case kGoalMcCoyUG07BrokenFinger:
		_animationState = 64;
		_animationFrame = 0;
		return true;

	case kGoalMcCoyUG07Released:
		_animationState = 65;
		_animationFrame = 0;
		return true;

	case kGoalMcCoyCallWithGuzza:
		Sound_Play(kSfxSPNBEEP9, 50, 0, 0, 50);
		Delay(1000);
		Sound_Play(kSfxVIDFONE1, 30, 0, 0, 50);
		Delay(1000);
		Sound_Play(kSfxSPNBEEP9, 50, 0, 0, 50);
		Actor_Says(kActorGuzza, 1380, kAnimationModeTalk); // Guzza: Yeah?
		Actor_Says(kActorMcCoy, 6610,                 13); // McCoy: Been messing with peoples' lives lately?
		Actor_Says(kActorGuzza, 1390, kAnimationModeTalk); // Guzza: McCoy! Where the hell have you been? We've been looking all over for you.
		Actor_Says(kActorMcCoy, 6615,                 18); // McCoy: Yeah? Am I wanted dead or alive?
		if (_vm->_cutContent) {
			Actor_Says(kActorGuzza, 1400, kAnimationModeTalk); // Guzza: I know there's been some confusion, kid.
			Actor_Says(kActorGuzza, 1410, kAnimationModeTalk); // Guzza: Come on downtown and we'll sit down like gentlemen and figure this out.
			Actor_Says(kActorMcCoy, 6620,                 15); // McCoy: If you're a gentleman, I'm a St. Bernard.
		}
		Actor_Says(kActorGuzza, 1420, kAnimationModeTalk); // Guzza: You don't know what you are, kid.
		Actor_Says(kActorMcCoy, 6625,                 11); // McCoy: You can sell that Replicant shit to everybody else but I ain't buying.
		Actor_Says(kActorGuzza, 1430, kAnimationModeTalk); // Guzza: If you're so clean, let's put you on the Machine. That will decide once and for all.
		Actor_Says(kActorMcCoy, 6630,                 12); // McCoy: I'm through listening, Guzza. Now it's your turn.
		Actor_Says(kActorMcCoy, 6635,                 17); // McCoy: I've been doing some investigating on my own. Came across some prime sources.
		Actor_Says(kActorMcCoy, 6640,                 13); // McCoy: You know what I'm talking about. Those little illegal weapons deals with Izo.
		Actor_Says(kActorMcCoy, 6645,                 19); // McCoy: You were raking in the chinyen selling LPD wares to scumbags and Reps.
		Actor_Says(kActorMcCoy, 6650,                 18); // McCoy: Oh, yeah.
		Actor_Says(kActorMcCoy, 6655,                 11); // McCoy: The department is gonna eat it up when they hear just how deep that corruption goes.
		Actor_Says(kActorGuzza, 1440, kAnimationModeTalk); // Guzza: That's a load of crap.
		Actor_Says(kActorMcCoy, 6660,                 17); // McCoy: Let's hope for your sake Bryant and the brass feel that way, when I lay this file on 'em.
		Actor_Says(kActorMcCoy, 6665,                 13); // McCoy: Otherwise, it's a one-way ticket to the Off-World penal colony, fat man.
		Delay(1000);
		Actor_Says(kActorGuzza, 1450, kAnimationModeTalk); // Guzza: What do you want, McCoy?
		Actor_Says(kActorMcCoy, 6670,                 14); // McCoy: Remove the frame, clear my name and get that guy out of my apartment!
		Actor_Says(kActorMcCoy, 6675,                 11); // McCoy: I want my life back.
		Actor_Says(kActorGuzza, 1460, kAnimationModeTalk); // Guzza: I can't do that.
		Actor_Says(kActorMcCoy, 6680,                 12); // McCoy: You better start finding a way 'cause I got another call to make.
		Actor_Says(kActorGuzza, 1470, kAnimationModeTalk); // Guzza: Okay, okay. How do we work this out? We can't talk over the damn phone.
		Actor_Says(kActorMcCoy, 6685,                 13); // McCoy: The sewers. I know you wanna be comfortable and I figured it's kinda like your second home down there.
		Delay(500);
		Actor_Says(kActorMcCoy, 6695,                 16); // McCoy: At the bottom of the old elevator there's a gate.
		Actor_Says(kActorMcCoy, 6700,                 17); // McCoy: Past it there's a chamber. It's got a round platform--
		Actor_Says(kActorGuzza, 1480, kAnimationModeTalk); // Guzza: Yeah, I know it.
		Actor_Says(kActorMcCoy, 6705,                 11); // McCoy: Be there! Alone. You'd better hold up your end or Bryant gets it all.
		Sound_Play(kSfxSPNBEEP9, 50, 0, 0, 50);
		return true;

	case kGoalMcCoyUG15Fall:
		Actor_Force_Stop_Walking(kActorMcCoy);
		Player_Loses_Control();
		_fallHeightCurrent = 48.07f;
		_fallSpeed = -4.0f;
		_fallHeightTarget = -20.0f;
		if (_animationState != 27
		 && _animationState != 50
		) {
			_animationState = 50;
			_animationFrame = Slice_Animation_Query_Number_Of_Frames(18) - 1;
		}
		return true;

	case 400:
		Actor_Set_Health(kActorMcCoy, 50, 50);
		Game_Flag_Set(kFlagKP02Available);
		affectionTowards = Global_Variable_Query(kVariableAffectionTowards);
		if (affectionTowards == kAffectionTowardsSteele) {
			Actor_Modify_Friendliness_To_Other(kActorSteele, kActorMcCoy, 3);
		} else if (affectionTowards == kAffectionTowardsDektora) {
			Actor_Modify_Friendliness_To_Other(kActorSteele, kActorMcCoy, -5);
			Actor_Modify_Friendliness_To_Other(kActorClovis, kActorMcCoy, 3);
		} else if (affectionTowards == kAffectionTowardsLucy) {
			Actor_Modify_Friendliness_To_Other(kActorSteele, kActorMcCoy, -5);
			Actor_Modify_Friendliness_To_Other(kActorClovis, kActorMcCoy, 5);
		}

		if (Game_Flag_Query(kFlagMcCoyFreedOfAccusations)) {
			Actor_Modify_Friendliness_To_Other(kActorSteele, kActorMcCoy, 3);
		}

		if (Actor_Query_Friendliness_To_Other(kActorSteele, kActorMcCoy) < Actor_Query_Friendliness_To_Other(kActorClovis, kActorMcCoy)) {
			Game_Flag_Set(kFlagMcCoyIsHelpingReplicants);
		}

		affectionTowards = Global_Variable_Query(kVariableAffectionTowards);
		if (affectionTowards == kAffectionTowardsSteele) {
			if (Game_Flag_Query(kFlagMcCoyIsHelpingReplicants)) {
				Global_Variable_Set(kVariableAffectionTowards, kAffectionTowardsNone);
			}
		} else if (affectionTowards == kAffectionTowardsDektora
		        || affectionTowards == kAffectionTowardsLucy
		) {
			if (!Game_Flag_Query(kFlagMcCoyIsHelpingReplicants)) {
				Global_Variable_Set(kVariableAffectionTowards, kAffectionTowardsNone);
			}
		}

		if (!Game_Flag_Query(kFlagMcCoyIsHelpingReplicants)) {
			Game_Flag_Set(kFlagMaggieHasBomb);
		}

		Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
		Ambient_Sounds_Remove_All_Looping_Sounds(1);
		Global_Variable_Set(kVariableChapter, 5);
		Outtake_Play(kOuttakeMovieD, false, -1);

		if (Game_Flag_Query(kFlagMcCoyFreedOfAccusations)) {
			Chapter_Enter(5, kSetMA07, kSceneMA07);
		} else {
			Game_Flag_Set(kFlagMA06ToMA02);
			Chapter_Enter(5, kSetMA02_MA04, kSceneMA02);
		}
		return true;

	case kGoalMcCoyArrested:
		Music_Stop(3);
		Player_Set_Combat_Mode(false);
		Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeIdle);
		_animationState = 0;
		_animationFrame = 0;
		Game_Flag_Set(kFlagMcCoyArrested);
		Set_Enter(kSetPS09, kScenePS09);
		return true;
	}
	return false;
}

bool AIScriptMcCoy::UpdateAnimation(int *animation, int *frame) {
	int v7;
	switch (_animationState) {
	case 0:
		*animation = kModelAnimationMcCoyIdle;
		if (_animationLoopCounter < _animationLoopLength) {
			_animationFrame += _animationLoopDirection;
			if (_animationFrame > _animationLoopFrameMax) {
				_animationFrame = _animationLoopFrameMax;
				_animationLoopDirection = -1;
			} else if (_animationFrame < _animationLoopFrameMin) {
				_animationFrame = _animationLoopFrameMin;
				_animationLoopDirection = 1;
			}
			_animationLoopCounter++;
		} else {
			_animationFrame += _animationLoopDirection;
			_animationLoopLength = 0;
			if (_animationFrame == 18 && Random_Query(0, 2)) {
				_animationLoopDirection = -1;
				_animationLoopCounter   = 0;
				_animationLoopFrameMin  = 14;
				_animationLoopFrameMax  = 18;
				_animationLoopLength    = Random_Query(0, 30);
			}
			if (_animationFrame == 26) {
				if (Random_Query(0, 2)) {
					_animationLoopDirection = -1;
					_animationLoopCounter   = 0;
					_animationLoopFrameMin  = 23;
					_animationLoopFrameMax  = 26;
					_animationLoopLength    = Random_Query(0, 30);
				}
			}
			if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
				_animationFrame = 0;
				if (Random_Query(0, 2)) {
					_animationLoopCounter  = 0;
					_animationLoopFrameMin = 0;
					_animationLoopFrameMax = 3;
					_animationLoopLength   = Random_Query(0, 45);
				}
			}
			if (_animationFrame < 0) {
				_animationFrame = Slice_Animation_Query_Number_Of_Frames(*animation) - 1;
			}
		}
		break;

	case 3:
	case 4:
		if (_animationFrame == 0 && !Game_Flag_Query(kFlagMcCoyAnimation1)) {
			_animationFrame = 1;
			_animationState = _animationStateNextSpecial;
			*animation = _animationNextSpecial;
			_animationStateNextSpecial = 4;
			_animationNextSpecial = kModelAnimationMcCoyProtestingTalk;
		} else if (_animationFrame <= 4 && Game_Flag_Query(kFlagMcCoyAnimation1)) {
			Game_Flag_Reset(kFlagMcCoyAnimation1);
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			_animationState = 0;
		} else {
			*animation = kModelAnimationMcCoyProtestingTalk;
			_animationFrame++;
			if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
				_animationFrame = 0;
			}
		}
		break;

	case 5:
		*animation = kModelAnimationMcCoyScratchHeadTalk;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 3;
			*animation = kModelAnimationMcCoyProtestingTalk;
		}
		break;

	case 6:
		*animation = kModelAnimationMcCoyScratchEarLongerTalk;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 3;
			*animation = kModelAnimationMcCoyProtestingTalk;
		}
		break;

	case 7:
		*animation = kModelAnimationMcCoyPointingTalk;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 3;
			*animation = kModelAnimationMcCoyProtestingTalk;
		}
		break;

	case 8:
		*animation = kModelAnimationMcCoyUpsetTalk;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 3;
			*animation = kModelAnimationMcCoyProtestingTalk;
		}
		break;

	case 9:
		*animation = kModelAnimationMcCoyDismissiveTalk;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 3;
			*animation = kModelAnimationMcCoyProtestingTalk;
		}
		break;

	case 10:
		*animation = kModelAnimationMcCoyScratchEarTalk;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 3;
			*animation = kModelAnimationMcCoyProtestingTalk;
		}
		break;

	case 11:
		*animation = kModelAnimationMcCoyHandsOnWaistTalk;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 3;
			*animation = kModelAnimationMcCoyProtestingTalk;
		}
		break;

	case 12:
		*animation = kModelAnimationMcCoyScratchEarLongerTalk;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 3;
			*animation = kModelAnimationMcCoyProtestingTalk;
		}
		break;

	case 13:
		*animation = kModelAnimationMcCoyIdle;
		if (_animationFrame < Slice_Animation_Query_Number_Of_Frames(*animation) / 2) {
			_animationFrame -= 3;
			if (_animationFrame <= 0) {
				_animationFrame = 0;
				*animation = _animationNext;
				_animationState = _animationStateNext;
			}
		} else {
			_animationFrame += 3;
			if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
				_animationFrame = 0;
				*animation = _animationNext;
				_animationState = _animationStateNext;
			}
		}
		break;

	case 14:
		*animation = kModelAnimationMcCoyWithGunIdle;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationState = 14;
			_animationFrame = 0;
		}
		break;

	case 15:
		*animation = kModelAnimationMcCoyWithGunUnholsterGun;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 14;
			*animation = kModelAnimationMcCoyWithGunIdle;
		}
		break;

	case 16:
		*animation = kModelAnimationMcCoyWithGunHolsterGun;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			_animationState = 0;
		}
		break;

	case 17:
		// this is just frame 0 always, McCoy doesn't animated shoot in this animation State
		// animation state 21 is for the full shooting animation
		*animation = kModelAnimationMcCoyWithGunShooting;
		_animationFrame = 0;
		// weird, but thats in game code
		if (Slice_Animation_Query_Number_Of_Frames(*animation) <= 0) {
			_animationFrame = 0;
			_animationState = 17;
		}
		break;

	case 18:
		*animation = kModelAnimationMcCoyWithGunAiming;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 17;
			*animation = kModelAnimationMcCoyWithGunShooting;
		}
		break;

	case 19:
		*animation = kModelAnimationMcCoyWithGunStopAimResumeIdle;
		_animationFrame++;
		if (_animationFrame >= 12) {
			_animationFrame = 0;
			_animationState = 14;
			*animation = kModelAnimationMcCoyWithGunIdle;
		}
		break;

	case 21:
		*animation = kModelAnimationMcCoyWithGunShooting;
		_animationFrame++;
		if (_animationFrame == 1
		 && Actor_Query_Goal_Number(kActorMcCoy) == kGoalMcCoyNR11Shoot
		 && _NR10SteeleShooting
		) {
			_nextSoundId = kSfxSMCAL3;
		}
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationState = 17;
			_animationFrame = 0;
			*animation = kModelAnimationMcCoyWithGunShooting;
#if BLADERUNNER_ORIGINAL_BUGS
#else
			// Resume combat idle position even when shot at a target -- if it's no longer a target (dead or moved)
			ChangeAnimationMode(kAnimationModeCombatIdle);
#endif // BLADERUNNER_ORIGINAL_BUGS

			if (Actor_Query_Goal_Number(kActorMcCoy) == kGoalMcCoyNR11Shoot) {
				_animationFrame = 0;
				_animationState = 21;
				_NR10SteeleShooting = true;
				*animation = kModelAnimationMcCoyWithGunShooting;
			}
		}
		break;

	case 22:
		_animationFrame = 0;
		_animationState = 17;
		*animation = kModelAnimationMcCoyWithGunShooting;
		break;

	case 23:
		*animation = kModelAnimationMcCoyWithGunGotHitRight;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 14;
			*animation = kModelAnimationMcCoyWithGunIdle;
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeCombatIdle);
		}
		break;

	case 24:
		*animation = kModelAnimationMcCoyWithGunGotHitRight;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 14;
			*animation = kModelAnimationMcCoyWithGunIdle;
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeCombatIdle);
		}
		break;

	case 25:
		*animation = kModelAnimationMcCoyGotHitRight;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			_animationState = 0;
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeIdle);
		}
		break;

	case 26:
		*animation = kModelAnimationMcCoyGotHitRight;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			_animationState = 0;
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeIdle);
		}
		break;

	case 27:
		*animation = kModelAnimationMcCoyFallsOnHisBack;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = Slice_Animation_Query_Number_Of_Frames(*animation) - 1;
			_animationState = 50;
			UG15fall();
			if (Actor_Query_Goal_Number(kActorMcCoy) == kGoalMcCoyNR04Drink) {
				Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyNR04PassOut);
			}
		}
		break;

	case 28:
		*animation = kModelAnimationMcCoyWithGunShotDead;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = Slice_Animation_Query_Number_Of_Frames(*animation) - 1;
			_animationState = 50;
			UG15fall();
		}
		break;

	case 29:
		*animation = kModelAnimationMcCoyFallsOnHisBack;
		_animationFrame += _animationLoopDirection;
		if (_animationFrame < 14) {
			_animationLoopDirection = 1;
		}
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeDie);
			*animation = kModelAnimationMcCoyFallsOnHisBack;
			_animationState = 27;
			_animationFrame = Slice_Animation_Query_Number_Of_Frames(*animation) - 1;
		}
		break;

	case 30:
		*animation = kModelAnimationMcCoyWalking;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		if (_animationFrame == 2) {
			Sound_Right_Footstep_Walk(kActorMcCoy);
		} else if (_animationFrame == 10) {
			Sound_Left_Footstep_Walk(kActorMcCoy);
		}
		break;

	case 31:
		*animation = kModelAnimationMcCoyRunning;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		if (_animationFrame == 5) {
			Sound_Left_Footstep_Run(kActorMcCoy);
		} else if (_animationFrame == 12) {
			Sound_Right_Footstep_Run(kActorMcCoy);
		}
		break;

	case 32:
		_animationFrame = 1;
		_animationState = 30;
		*animation = kModelAnimationMcCoyWalking;
		break;

	case 36:
		*animation = kModelAnimationMcCoyWithGunWalking;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		if (_animationFrame == 15) {
			Sound_Left_Footstep_Walk(kActorMcCoy);
		}
		if (_animationFrame == 6) {
			Sound_Right_Footstep_Walk(kActorMcCoy);
		}
		break;

	case 37:
		*animation = kModelAnimationMcCoyWithGunRunning;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		if (_animationFrame == 6) {
			Sound_Left_Footstep_Run(kActorMcCoy);
		}
		if (_animationFrame == 0) {
			Sound_Right_Footstep_Run(kActorMcCoy);
		}
		break;

	case 38:
		*animation = kModelAnimationMcCoyClimbStairsUp;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		if (_animationFrame == 9) {
			Sound_Left_Footstep_Walk(kActorMcCoy);
		} else if (_animationFrame == 4) {
			Sound_Right_Footstep_Walk(kActorMcCoy);
		}
		if (Game_Flag_Query(kFlagHF07toHF05)) {
			walkStairsLeft(0.5f);
		}
		break;

	case 39:
		*animation = kModelAnimationMcCoyClimbStairsDown;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		if (_animationFrame == 8) {
			Sound_Left_Footstep_Walk(kActorMcCoy);
		} else if (_animationFrame == 3) {
			Sound_Right_Footstep_Walk(kActorMcCoy);
		}
		if (Game_Flag_Query(kFlagHF05toHF07)) {
			walkStairsRight(-0.2f);
		}
		break;

	case 40:
		*animation = kModelAnimationMcCoyWithGunClimbStairsUp;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		if (_animationFrame == 8) {
			Sound_Left_Footstep_Walk(kActorMcCoy);
		} else if (_animationFrame == 3) {
			Sound_Right_Footstep_Walk(kActorMcCoy);
		}
		if (Game_Flag_Query(kFlagHF07toHF05)) {
			walkStairsLeft(0.5f);
		}
		break;

	case 41:
		*animation = kModelAnimationMcCoyWithGunClimbStairsDown;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		if (_animationFrame == 9) {
			Sound_Left_Footstep_Walk(kActorMcCoy);
		} else if (_animationFrame == 4) {
			Sound_Right_Footstep_Walk(kActorMcCoy);
		}
		if (Game_Flag_Query(kFlagHF05toHF07)) {
			walkStairsRight(-0.2f);
		}
		break;

	case 42:
		*animation = kModelAnimationMcCoyClimbsLadderUp;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation) - 1) { //why -1?
			_animationFrame = 0;
		}
		switch (_animationFrame) {
		case 9:
			Sound_Left_Footstep_Walk(kActorMcCoy);
			break;
		case 4:
			Sound_Right_Footstep_Walk(kActorMcCoy);
			break;
		case 1:
			Sound_Right_Footstep_Walk(kActorMcCoy);
			break;
		}
		break;

	case 43:
		*animation = kModelAnimationMcCoyClimbsLadderDown;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation) - 1) { //why -1?
			_animationFrame = 0;
		}
		switch (_animationFrame) {
		case 1:
			Sound_Right_Footstep_Walk(kActorMcCoy);
			break;
		case 4:
			Sound_Right_Footstep_Walk(kActorMcCoy);
			break;
		case 9:
			Sound_Left_Footstep_Walk(kActorMcCoy);
			break;
		}
		break;

	case 44:
		*animation = kModelAnimationMcCoyThrowsBeggarInTrash;
		_animationFrame++;
		if (_animationFrame == 127) {
			Game_Flag_Set(kFlagCT04BodyDumped);
		}
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			ChangeAnimationMode(kAnimationModeIdle);
			Actor_Set_At_XYZ(kActorMcCoy, -203.41f, -621.3f, 724.57f, 538);
			Player_Gains_Control();
		}
		break;

	case 45:
		*animation = kModelAnimationMcCoyEntersSpinner;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			Actor_Set_Invisible(kActorMcCoy, true);
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			ChangeAnimationMode(kAnimationModeIdle);
		}
		break;

	case 46:
		Actor_Set_Invisible(kActorMcCoy, false);
		*animation = kModelAnimationMcCoyExitsSpinner;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			Player_Gains_Control();
			ChangeAnimationMode(kAnimationModeIdle);
			Actor_Face_Heading(kActorMcCoy, (Actor_Query_Facing_1024(kActorMcCoy) + 512) & 1023, false);
		}
		break;

	case 47:
		*animation = kModelAnimationMcCoyLeaningOver;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 48;
			*animation = kModelAnimationMcCoyLeaningOverSearching;
		}
		break;

	case 48:
		*animation = kModelAnimationMcCoyLeaningOverSearching;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 49;
			*animation = kModelAnimationMcCoyLeaningOverResumeIdle;
		}
		break;

	case 49:
		*animation = kModelAnimationMcCoyLeaningOverResumeIdle;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyDefault);
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			ChangeAnimationMode(kAnimationModeIdle);
		}
		break;

	case 50:
		*animation = kModelAnimationMcCoyFallsOnHisBack;
		_animationFrame = Slice_Animation_Query_Number_Of_Frames(*animation) - 1;
		break;

	case 51:
		*animation = kModelAnimationMcCoyDodgeAndDrawGun;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			Player_Set_Combat_Mode(true);
			ChangeAnimationMode(kAnimationModeCombatIdle);
			Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyDefault);
			_animationFrame = 0;
			Player_Gains_Control();
		}
		break;

	case 52:
		*animation = kModelAnimationMcCoyDiesInAgony;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = Slice_Animation_Query_Number_Of_Frames(*animation) - 1;
			_animationState = 50;
		}
		break;

	case 53:
		*animation = kModelAnimationMcCoyFallsOnHisBack;
		_animationFrame--;
		if (_animationFrame <= 0) {
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeIdle);
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			_animationState = 0;
			if (Actor_Query_Goal_Number(kActorMcCoy) == kGoalMcCoyBB11GetUp) {
				Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyBB11PrepareToRunAway);
			}
			if (Actor_Query_Goal_Number(kActorMcCoy) == kGoalMcCoyNR01ThrownOut) {
				Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyNR01GetUp);
			}
		}
		break;

	case 55:
		*animation = kModelAnimationMcCoyGivesFromPocket;
		_animationFrame++;
		if (_animationFrame == 7) {
			Actor_Change_Animation_Mode(kActorMaggie, kAnimationModeFeeding);
		}
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			*animation = kModelAnimationMcCoyIdle;
			_animationState = 0;
		}
		break;

	case 56:
		*animation = kModelAnimationMcCoyStartled;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			if (Actor_Query_Which_Set_In(kActorMcCoy) == kSetUG15) {
				_animationState = 27;
			} else {
				*animation = kModelAnimationMcCoyIdle;
				_animationState = 0;
				Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeIdle);
			}
		}
		break;

	case 57:
		*animation = kModelAnimationMcCoyTiedInChairIdle;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		if (!Game_Flag_Query(kFlagMcCoyTiedDown)) {
			_animationFrame = 0;
			_animationState = 59;
			*animation = kModelAnimationMcCoyTiedInChairFreed;
		}
		break;

	case 58:
		*animation = kModelAnimationMcCoyTiedInChairMoving;
		_animationFrame++;
		if (_animationFrame == 6) {
			// Play one of kSfxCHARMTL7, kSfxCHARMTL8, kSfxCHARMTL9
			Ambient_Sounds_Play_Sound(Random_Query(kSfxCHARMTL7, kSfxCHARMTL9), 39, 0, 0, 99);
		}
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation) - 1) { //why -1? a bug?
			_animationFrame = 0;
		}
		break;

	case 59:
		*animation = kModelAnimationMcCoyTiedInChairFreed;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			_animationLoopLength = 0;
			_animationState = 0;
			Player_Gains_Control();
			Item_Add_To_World(kItemChair, kModelAnimationYukonHotelChair, kSetCT08_CT51_UG12, -110.0, 0.0, -192.0, 0, 48, 32, false, true, false, false);
		}
		break;

	case 60:
		*animation = kModelAnimationMcCoySittingToUseConsole;
		if (_animationFrame < Slice_Animation_Query_Number_Of_Frames(*animation) - 1) {
			_animationFrame++;
		}
		break;

	case 61:
		*animation = kModelAnimationMcCoySittingToUseConsole;
		_animationFrame--;
		if (_animationFrame <= 0) {
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			_animationState = 0;
			if (Actor_Query_Goal_Number(kActorMcCoy) == kGoalMcCoyNRxxSitAtTable) {
				Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyNRxxStandUp);
			}
		}
		break;

	case 62:
		*animation = kModelAnimationMcCoyWithGunGrabbedByArm0;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 63;
			*animation = kModelAnimationMcCoyWithGunGrabbedByArm1;
		}
		break;

	case 63:
		*animation = kModelAnimationMcCoyWithGunGrabbedByArm1;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		break;

	case 64:
		*animation = kModelAnimationMcCoyWithGunGrabbedByArmHurt;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 63;
			*animation = kModelAnimationMcCoyWithGunGrabbedByArm1;
		}
		break;

	case 65:
		*animation = kModelAnimationMcCoyWithGunGrabbedByArmFreed;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 14;
			*animation = kModelAnimationMcCoyWithGunIdle;
			Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyDefault);
		}
		break;

	case 66:
		*animation = kModelAnimationMcCoyGiveMovement;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeIdle);
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			_animationState = 0;
		}
		break;

	case 67:
		*animation = kModelAnimationMcCoyDrinkingBooze;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeIdle);
			*animation = kModelAnimationMcCoyIdle;
			_animationState = 0;
			_animationFrame = 0;
			if (Actor_Query_Goal_Number(kActorMcCoy) == kGoalMcCoyNR04Drink) {
				Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeDie);
			}
		}
		break;

	case 68:
		*animation = kModelAnimationMcCoyFallsOnHisBack;
		v7 = Slice_Animation_Query_Number_Of_Frames(*animation) - 1 - Global_Variable_Query(kVariableNR01GetUpCounter);
		if (_animationFrame < v7) {
			_animationFrame++;
		} else if (_animationFrame > v7) {
			_animationFrame--;
		}
		if (_animationFrame <= 0) {
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeIdle);
			*animation = kModelAnimationMcCoyIdle;
			_animationState = 0;
			_animationFrame = 0;
			Game_Flag_Reset(kFlagNR01McCoyIsDrugged);
			Scene_Exits_Enable();
			Player_Set_Combat_Mode_Access(true);
			Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyDefault);
		}
		break;

	case 69:
		*animation = kModelAnimationMcCoyCrouchingDown;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeSit);
			*animation = kModelAnimationMcCoyCrouchedIdle;
		}
		break;

	case 70:
		*animation = kModelAnimationMcCoyCrouchedIdle;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		break;

	case 71:
		*animation = kModelAnimationMcCoyCrouchedGetsUp;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeIdle);
			*animation = kModelAnimationMcCoyIdle;
			_animationFrame = 0;
			_animationState = 0;
			Player_Gains_Control();
		}
		break;
	}
	*frame = _animationFrame;
	return true;
}

bool AIScriptMcCoy::ChangeAnimationMode(int mode) {
	switch (mode) {
	case kAnimationModeIdle:
		if (Game_Flag_Query(kFlagMcCoyTiedDown)) {
			if (_animationFrame <= 6) {
				// Play one of kSfxCHARMTL7, kSfxCHARMTL8, kSfxCHARMTL9
				Ambient_Sounds_Play_Sound(Random_Query(kSfxCHARMTL7, kSfxCHARMTL9), 39, 0, 0, 99);
			}
			_animationState = 57;
			_animationFrame = 0;
			return true;
		}
		switch (_animationState) {
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
			Game_Flag_Set(kFlagMcCoyAnimation1);
			_animationLoopCounter = 0;
			_animationLoopLength = 30;
			_animationLoopFrameMin = 0;
			_animationLoopDirection = 1;
			_animationLoopFrameMax = 3;
			return true;
		case 14:
		case 17:
		case 20:
		case 21:
		case 36:
			_animationState = 16;
			_animationFrame = 0;
			return true;
		case 15:
			_animationState = 16;
			_animationFrame = 16 - ((16 * _animationFrame) / 12);
			return true;
		case 16:
		case 25:
		case 26:
		case 55:
			return true;
		case 60:
			_animationState = 61;
			return true;
		default:
			_animationState = 0;
			_animationFrame = 0;
			_animationLoopLength = 0;
			return true;
		}
		break;

	case kAnimationModeWalk:
		if (_animationState != 27
		 && _animationState != 50
		) {
			if (Game_Flag_Query(kFlagMcCoyTiedDown)) {
				_animationState = 58;
				_animationFrame = 0;
			} else {
				_animationState = 32;
				_animationFrame = 0;
			}
		}
		break;

	case kAnimationModeRun:
		if (_animationState != 27
		 && _animationState != 50
		) {
			if (Game_Flag_Query(kFlagMcCoyTiedDown)) {
				_animationState = 58;
				_animationFrame = 4;
			} else {
				_animationState = 31;
				_animationFrame = 0;
			}
		}
		break;

	case kAnimationModeTalk:
		if (_animationState < 3 || _animationState > 12) {
			_animationState = 13;
			_animationStateNext = 3;
			_animationNext = kModelAnimationMcCoyProtestingTalk;
		} else {
			Game_Flag_Reset(kFlagMcCoyAnimation1);
			_animationStateNextSpecial = 4;
			_animationNextSpecial = kModelAnimationMcCoyProtestingTalk;
		}
		break;

	case kAnimationModeCombatIdle:
		switch (_animationState) {
		case 14:
		case 15:
		case 19:
		case 21:
			break;
		case 16:
			_animationState = 15;
			_animationFrame = 12 - ((12 * _animationFrame) / 16);
			break;
		case 17:
		case 20:
			_animationState = 19;
			_animationFrame = 0;
			break;
		case 18:
			_animationState = 19;
			_animationFrame = 13 * ((8 - _animationFrame) / 8);
			break;
		case 22:
			_animationState = 19;
			_animationFrame = 41;
			break;
		case 36:
		case 37:
		case 40:
		case 41:
		case 51:
			_animationState = 14;
			_animationFrame = 0;
			break;
		default:
			_animationState = 15;
			_animationFrame = 0;
			break;
		}
		break;

	case kAnimationModeCombatAim:
		switch (_animationState) {
		case 17:
		case 18:
		case 21:
			break;
		case 19:
			_animationState = 18;
			_animationFrame = 8 * (13 - _animationFrame) / 13;
			break;
		case 22:
			_animationState = 22;
			_animationFrame = 0;
			break;
		default:
			_animationState = 18;
			_animationFrame = 0;
			break;
		}
		break;

	case kAnimationModeCombatAttack:
		_animationState = 21;
		_animationFrame = 0;
		break;

	case kAnimationModeCombatWalk:
		if (_animationState != 27 && _animationState != 50) {
			_animationState = 36;
			_animationFrame = 0;
		}
		break;

	case kAnimationModeCombatRun:
		if (_animationState != 27 && _animationState != 50) {
			_animationState = 37;
			_animationFrame = 0;
		}
		break;

	case 9:
	case 12:
		if (_animationState < 3 || _animationState > 12) {
			_animationState = 13;
			_animationStateNext = 5;
			_animationNext = kModelAnimationMcCoyScratchHeadTalk;
		} else {
			Game_Flag_Reset(kFlagMcCoyAnimation1);
			_animationStateNextSpecial = 5;
			_animationNextSpecial = kModelAnimationMcCoyScratchHeadTalk;
		}
		break;

	case 10:
	case 13:
		if (_animationState < 3 || _animationState > 12) {
			_animationState = 13;
			_animationStateNext = 6;
			_animationNext = kModelAnimationMcCoyScratchEarLongerTalk;
		} else {
			Game_Flag_Reset(kFlagMcCoyAnimation1);
			_animationStateNextSpecial = 6;
			_animationNextSpecial = kModelAnimationMcCoyScratchEarLongerTalk;
		}
		break;

	case 11:
	case 14:
		if (_animationState < 3 || _animationState > 12) {
			_animationState = 13;
			_animationStateNext = 7;
			_animationNext = kModelAnimationMcCoyPointingTalk;
		} else {
			Game_Flag_Reset(kFlagMcCoyAnimation1);
			_animationStateNextSpecial = 7;
			_animationNextSpecial = kModelAnimationMcCoyPointingTalk;
		}
		break;

	case 15:
		if (_animationState < 3 || _animationState > 12) {
			_animationState = 13;
			_animationStateNext = 8;
			_animationNext = kModelAnimationMcCoyUpsetTalk;
		} else {
			Game_Flag_Reset(kFlagMcCoyAnimation1);
			_animationStateNextSpecial = 8;
			_animationNextSpecial = kModelAnimationMcCoyUpsetTalk;
		}
		break;

	case 16:
		if (_animationState < 3 || _animationState > 12) {
			_animationState = 13;
			_animationStateNext = 9;
			_animationNext = kModelAnimationMcCoyDismissiveTalk;
		} else {
			Game_Flag_Reset(kFlagMcCoyAnimation1);
			_animationStateNextSpecial = 9;
			_animationNextSpecial = kModelAnimationMcCoyDismissiveTalk;
		}
		break;

	case 17:
		if (_animationState < 3 || _animationState > 12) {
			_animationState = 13;
			_animationStateNext = 10;
			_animationNext = kModelAnimationMcCoyScratchEarTalk;
		} else {
			Game_Flag_Reset(kFlagMcCoyAnimation1);
			_animationStateNextSpecial = 10;
			_animationNextSpecial = kModelAnimationMcCoyScratchEarTalk;
		}
		break;

	case 18:
		if (_animationState < 3 || _animationState > 12) {
			_animationState = 13;
			_animationStateNext = 11;
			_animationNext = kModelAnimationMcCoyHandsOnWaistTalk;
		} else {
			Game_Flag_Reset(kFlagMcCoyAnimation1);
			_animationStateNextSpecial = 11;
			_animationNextSpecial = kModelAnimationMcCoyHandsOnWaistTalk;
		}
		break;

	case 19:
		if (_animationState < 3 || _animationState > 12) {
			_animationState = 13;
			_animationStateNext = 12;
			_animationNext = kModelAnimationMcCoyScratchEarLongerTalk;
		} else {
			Game_Flag_Reset(kFlagMcCoyAnimation1);
			_animationStateNextSpecial = 12;
			_animationNextSpecial = kModelAnimationMcCoyScratchEarLongerTalk;
		}
		break;

	case kAnimationModeDodge:
		dodge();
		break;

	case kAnimationModeHit:
		switch (_animationState) {
		case 14:
		case 15:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
			if (Random_Query(0, 1) == 1) {
				_animationState = 24;
			} else {
				_animationState = 23;
			}
			_animationFrame = 0;
			break;
		default:
			if (Random_Query(0, 1) == 1) {
				_animationState = 26;
			} else {
				_animationState = 25;
			}
			_animationFrame = 0;
			break;
		}
		break;

	case kAnimationModeCombatHit:
		if (Random_Query(0, 1) == 1) {
			_animationState = 23;
		} else {
			_animationState = 24;
		}
		_animationFrame = 0;
		break;

	case 23:
		_animationState = 66;
		_animationFrame = 0;
		break;

	case 29:
		Player_Loses_Control();
		_animationFrame = 0;
		_animationState = 71;
		break;

	case 38:
		_animationState = 47;
		_animationFrame = 0;
		break;

	case 39:
		_animationFrame = 0;
		_animationState = 52;
		if (Player_Query_Current_Set() == kSetCT02) {
			_animationFrame = 23;
		}
		break;

	case 40:
		_animationState = 44;
		_animationFrame = 0;
		break;

	case kAnimationModeSpinnerGetIn:
		_animationState = 45;
		_animationFrame = 0;
		break;

	case kAnimationModeSpinnerGetOut:
		_animationState = 46;
		_animationFrame = 0;
		Player_Loses_Control();
		break;

	case kAnimationModeWalkUp:
		_animationState = 38;
		_animationFrame = 0;
		break;

	case kAnimationModeWalkDown:
		_animationState = 39;
		_animationFrame = 0;
		break;

	case kAnimationModeCombatWalkUp:
		_animationState = 40;
		_animationFrame = 0;
		break;

	case kAnimationModeCombatWalkDown:
		_animationState = 41;
		_animationFrame = 0;
		break;

	case kAnimationModeDie:
		switch (_animationState) {
		case 14:
		case 15:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
			_animationState = 28;
			_animationFrame = 0;
			break;
		case 27:
		case 28:
		case 50:
			break;
		default:
			_animationState = 27;
			_animationFrame = 0;
			break;
		}
		break;
	case kAnimationModeCombatDie:
		_animationState = 28;
		_animationFrame = 0;
		break;
	case 51:
		_animationState = 27;
		_animationFrame = 0;
		break;
	case kAnimationModeFeeding:
		_animationState = 55;
		_animationFrame = 0;
		break;
	case kAnimationModeSit:
		if (_animationState != 60 && (Player_Query_Current_Set() == kSetNR03 || Player_Query_Current_Set() == kSetNR05_NR08)) {
			_animationState = 60;
			_animationFrame = 0;
		} else {
			_animationFrame = 0;
			_animationState = 70;
		}
		break;
	case kAnimationModeClimbUp:
	case kAnimationModeCombatClimbUp:
		_animationState = 42;
		_animationFrame = 0;
		break;
	case kAnimationModeClimbDown:
	case kAnimationModeCombatClimbDown:
		_animationState = 43;
		_animationFrame = 0;
		break;
	case 68:
		_animationState = 29;
		_animationFrame = Slice_Animation_Query_Number_Of_Frames(18) - 1;
		_animationLoopDirection = -1;
		break;
	case 75:
		_animationState = 67;
		_animationFrame = 0;
		break;
	case 85:
		_animationFrame = 0;
		_animationState = 69;
		break;
	}
	return true;
}

void AIScriptMcCoy::QueryAnimationState(int *animationState, int *animationFrame, int *animationStateNext, int *animationNext) {
	*animationState     = _animationState;
	*animationFrame     = _animationFrame;
	*animationStateNext = _animationStateNext;
	*animationNext      = _animationNext;
}

void AIScriptMcCoy::SetAnimationState(int animationState, int animationFrame, int animationStateNext, int animationNext) {
	_animationState     = animationState;
	_animationFrame     = animationFrame;
	_animationStateNext = animationStateNext;
	_animationNext      = animationNext;
}

bool AIScriptMcCoy::ReachedMovementTrackWaypoint(int waypointId) {
	return true;
}

void AIScriptMcCoy::FledCombat() {}

void AIScriptMcCoy::fallDown() {
	float x, y, z;
	Actor_Query_XYZ(kActorMcCoy, &x, &y, &z);
	_fallHeightCurrent = _fallHeightCurrent + _fallSpeed;
	if (_fallHeightTarget < _fallHeightCurrent) {
		_fallSpeed = _fallSpeed - 0.2f;
	} else {
		_fallHeightCurrent = _fallHeightTarget;
		Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyDefault);
		Actor_Retired_Here(kActorMcCoy, 12, 48, true, -1);
	}
	return Actor_Set_At_XYZ(kActorMcCoy, x, _fallHeightCurrent, z, Actor_Query_Facing_1024(kActorMcCoy));
}

void AIScriptMcCoy::UG15fall() {
	if ( Actor_Query_Which_Set_In(kActorMcCoy) == kSetUG15
	 &&  Actor_Query_Goal_Number(kActorMcCoy) != kGoalMcCoyUG15Fall
	 && !Game_Flag_Query(kFlagUG15BridgeBroken)
	) {
		float x, y, z;
		Actor_Query_XYZ(kActorMcCoy, &x, &y, &z);
		if ( 220.0f > z
		 && -210.0f < x
		 &&  -70.0f > x
		) {
			Game_Flag_Set(kFlagUG15BridgeBroken);
			Scene_Loop_Set_Default(3); // kUG15LoopMainLoopBridgeBroken
			Scene_Loop_Start_Special(kSceneLoopModeOnce, 2, true); // kUG15LoopBridgeBreaks
			Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyUG15Fall);
			Actor_Query_XYZ(kActorFreeSlotA, &x, &y, &z);
			if (-200.0 < x
			 && -62.0f > x
			) {
				Actor_Set_Goal_Number(kActorFreeSlotA, kGoalFreeSlotAUG15Fall);
			}
		} else {
			Actor_Set_Goal_Number(kActorMcCoy, kGoalMcCoyUG15Die);
		}
	}
}

void AIScriptMcCoy::dodge() {
	int setId = Actor_Query_Which_Set_In(kActorMcCoy);
	if (setId == kSetCT02) {
		_animationState = 51;
		_animationFrame = 0;
		Player_Loses_Control();
		Game_Flag_Set(kFlagCT02ZubenFled);
	} else if (setId == kSetRC03 || setId == kSetUG15) {
		_animationState = 56;
		_animationFrame = 0;
	}
}

void AIScriptMcCoy::walkStairsLeft(float stepHeight) {
	float x, y, z;
	int currentAngle = Actor_Query_Facing_1024(kActorMcCoy);
	Actor_Query_XYZ(kActorMcCoy, &x, &y, &z);
	int angle = currentAngle - 12;
	if (angle < 0) {
		angle = currentAngle + 1012;
	}
	if (angle > 1023) {
		angle -= 1024;
	}
	Actor_Set_At_XYZ(kActorMcCoy, x, y + stepHeight, z, angle);
}

void AIScriptMcCoy::walkStairsRight(float stepHeight) {
	float x, y, z;
	int currentAngle = Actor_Query_Facing_1024(kActorMcCoy);
	Actor_Query_XYZ(kActorMcCoy, &x, &y, &z);
	int angle = currentAngle + 15;
	if (angle < 0) {
		angle = currentAngle + 1039;
	}
	if (angle > 1023) {
		angle -= 1024;
	}
	return Actor_Set_At_XYZ(kActorMcCoy, x, y + stepHeight, z, angle);
}

} // End of namespace BladeRunner
