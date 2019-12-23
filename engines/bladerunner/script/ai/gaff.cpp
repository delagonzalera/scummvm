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

AIScriptGaff::AIScriptGaff(BladeRunnerEngine *vm) : AIScriptBase(vm) {
}

void AIScriptGaff::Initialize() {
	_animationFrame = 0;
	_animationState = 0;
	_animationStateNext = 0;
	_animationNext = 0;
	Actor_Put_In_Set(kActorGaff, kSetFreeSlotC);
	Actor_Set_Goal_Number(kActorGaff, kGoalGaffDefault);
	//return false;
}

bool AIScriptGaff::Update() {
	if ( Global_Variable_Query(kVariableChapter) == 2
	 && !Game_Flag_Query(kFlagGaffChapter2Started)
	) {
		Actor_Set_Goal_Number(kActorGaff, kGoalGaffStartWalkingAround);
		Game_Flag_Set(kFlagGaffChapter2Started);
		return true;
	}

	if (Global_Variable_Query(kVariableChapter) == 4
	 && Actor_Query_Goal_Number(kActorGaff) < kGoalGaffStartChapter4
	) {
		Actor_Set_Goal_Number(kActorGaff, kGoalGaffStartChapter4);
	}

	return false;

}

void AIScriptGaff::TimerExpired(int timer) {
	if (timer == kActorTimerAIScriptCustomTask0) {
		AI_Countdown_Timer_Reset(kActorGaff, kActorTimerAIScriptCustomTask0);
		Actor_Set_Goal_Number(kActorGaff, kGoalGaffMA07TalkToMcCoy);
	}
	//return false;
}

void AIScriptGaff::CompletedMovementTrack() {
	if (Actor_Query_Goal_Number(kActorGaff) == kGoalGaffMA01ApproachMcCoy) {
		Actor_Face_Actor(kActorGaff, kActorMcCoy, true);
		Actor_Says(kActorGaff, 0, kAnimationModeTalk); // Gaff: Rumor has it you've been earning your stripes, McCoy.
		Loop_Actor_Walk_To_Actor(kActorMcCoy, kActorGaff, 36, false, true);
		Actor_Face_Actor(kActorMcCoy, kActorGaff, true);
		Actor_Says(kActorMcCoy, 670, kAnimationModeTalk); // McCoy: Working on it.
		Actor_Says(kActorGaff,   10, kAnimationModeTalk); // Gaff: Keep it up. You just might have a future in this business.
		Actor_Says(kActorMcCoy, 675, kAnimationModeTalk); // McCoy: I understand it's got a hell of a retirement plan.
		if (Game_Flag_Query(kFlagZubenRetired)) {
			Actor_Says(kActorGaff,   50, kAnimationModeTalk); // Gaff: You V-K the mark before retirement?
			Actor_Says(kActorMcCoy, 695, kAnimationModeTalk); // McCoy: Didn't have to.
			Actor_Says(kActorGaff,   60, kAnimationModeTalk); // Gaff: That's why they call it "the magic".
			Actor_Says(kActorMcCoy, 700, kAnimationModeTalk); // McCoy: I'm starting to understand.
			Actor_Says(kActorGaff,   70, kAnimationModeTalk); // Gaff: You ever retire human, your career is over. Remember that.
			Actor_Clue_Acquire(kActorGaff, kClueMcCoyRetiredZuben, true, -1);
		} else {
			Actor_Says(kActorGaff,   80, kAnimationModeTalk); // Gaff: Your target give you the slip?
			Actor_Says(kActorGaff,   90, kAnimationModeTalk); // Gaff: What happened, McCoy? You feel sorry for it?
			Actor_Says(kActorMcCoy, 705, kAnimationModeTalk); // McCoy: My game must have been off.
			Actor_Says(kActorGaff,  100, kAnimationModeTalk); // Gaff: Go home and get some rest. I'm sure you need it.
			Actor_Clue_Acquire(kActorGaff, kClueMcCoyLetZubenEscape, true, -1);
		}
		Actor_Says(kActorGaff,   20, kAnimationModeTalk); // Gaff: I checked with U.N.R. Looks your Reps swung that moonbus massacre last month.
		Actor_Says(kActorMcCoy, 680, kAnimationModeTalk); // McCoy: They ever find the moonbus?
		Actor_Says(kActorGaff,   30, kAnimationModeTalk); // Gaff: Disappeared into thin air. Bryant thinks it crashed out in the Kipple.
		Actor_Says(kActorMcCoy, 685, kAnimationModeTalk); // McCoy: How many Reps we're talking about?
		Actor_Says(kActorGaff,   40, kAnimationModeTalk); // Gaff: Enough. They're Nexus-6s, so don't take too much slack.
		Actor_Says(kActorMcCoy, 690, kAnimationModeTalk); // McCoy: Gotcha.
		Actor_Clue_Acquire(kActorMcCoy, kClueGaffsInformation, true, kActorGaff);
		CDB_Set_Crime(kClueZubenSquadPhoto, kCrimeMoonbusHijacking);
		Actor_Clue_Acquire(kActorGaff, kClueMcCoyRetiredZuben, true, -1);
		Game_Flag_Set(kFlagGaffApproachedMcCoyAboutZuben);
		Player_Gains_Control();
		Actor_Set_Goal_Number(kActorGaff, kGoalGaffMA01Leave);
	}

	if (Actor_Query_Goal_Number(kActorGaff) == kGoalGaffCT12GoToSpinner) {
		Actor_Set_Goal_Number(kActorGaff, kGoalGaffCT12FlyAway);
	}

	if (Actor_Query_Goal_Number(kActorGaff) == kGoalGaffStartWalkingAround) {
		Actor_Set_Goal_Number(kActorGaff, kGoalGaffWalkAround);
		return;// true;
	}

	if (Actor_Query_Goal_Number(kActorGaff) == kGoalGaffWalkAround) {
		Actor_Set_Goal_Number(kActorGaff, kGoalGaffRepeatWalkingAround);
		return;// true;
	}

	if (Actor_Query_Goal_Number(kActorGaff) == kGoalGaffRepeatWalkingAround) {
		Actor_Set_Goal_Number(kActorGaff, kGoalGaffWalkAround);
		return;// true;
	}
	// return false;
}

void AIScriptGaff::ReceivedClue(int clueId, int fromActorId) {
	//return false;
}

void AIScriptGaff::ClickedByPlayer() {
	if ((Global_Variable_Query(kVariableChapter) == 2
	  || Global_Variable_Query(kVariableChapter) == 3
	 )
	 && Game_Flag_Query(kFlagGaffChapter2Started)
	) {
		AI_Movement_Track_Pause(kActorGaff);
		Actor_Face_Actor(kActorMcCoy, kActorGaff, true);
		Actor_Face_Actor(kActorGaff, kActorMcCoy, true);
		if (Random_Query(1, 3) == 1) {
			Actor_Says(kActorMcCoy, 3970, 14); // McCoy: Hey.
			Actor_Says(kActorGaff,   100, 13); // Gaff: Go home and get some rest. I'm sure you need it.
		} else if (Random_Query(1, 3) == 2) {
			Actor_Says(kActorMcCoy, 3970, 14); // McCoy: Hey.
			Actor_Says(kActorGaff,   110, 13); // Gaff: Tough day, McCoy.
		} else {
			Actor_Says(kActorMcCoy, 3970, 14); // McCoy: Hey.
			Actor_Says(kActorGaff,   180, 13); // Gaff: You killed anyone yet?
		}
		AI_Movement_Track_Unpause(kActorGaff);
		// return true;
	}
	// return false;
}

void AIScriptGaff::EnteredScene(int sceneId) {
	// return false;
}

void AIScriptGaff::OtherAgentEnteredThisScene(int otherActorId) {
	// return false;
}

void AIScriptGaff::OtherAgentExitedThisScene(int otherActorId) {
	// return false;
}

void AIScriptGaff::OtherAgentEnteredCombatMode(int otherActorId, int combatMode) {
	// It is impossible to triger this as player has no control at this moment
	if (otherActorId == kActorMcCoy
	 && combatMode == 1
	 && Global_Variable_Query(kVariableChapter) == 4
	 && Actor_Query_In_Set(kActorMcCoy, kSetMA07)
	 && Actor_Query_Goal_Number(kActorGaff) == kGoalGaffMA07Wait
	) {
		AI_Countdown_Timer_Reset(kActorGaff, kActorTimerAIScriptCustomTask0);
		Actor_Set_Goal_Number(kActorGaff, kGoalGaffMA07ShootMcCoy);
	}
	// return false;
}

void AIScriptGaff::ShotAtAndMissed() {
	// return false;
}

bool AIScriptGaff::ShotAtAndHit() {
	return false;
}

void AIScriptGaff::Retired(int byActorId) {
	// return false;
}

int AIScriptGaff::GetFriendlinessModifierIfGetsClue(int otherActorId, int clueId) {
	return 0;
}

bool AIScriptGaff::GoalChanged(int currentGoalNumber, int newGoalNumber) {
	switch (newGoalNumber) {
	case kGoalGaffCT12WaitForMcCoy:
		Actor_Put_In_Set(kActorGaff, kSetCT01_CT12);
		Actor_Set_At_Waypoint(kActorGaff, 43, 0);
		return true;

	case kGoalGaffCT12GoToSpinner:
		AI_Movement_Track_Append(kActorGaff, 123, 1);
		AI_Movement_Track_Repeat(kActorGaff);
		return true;

	case kGoalGaffMA01ApproachMcCoy:
		Player_Loses_Control();
		AI_Movement_Track_Flush(kActorGaff);
		AI_Movement_Track_Append(kActorGaff, 124, 0);
		AI_Movement_Track_Append(kActorGaff, 126, 0);
		AI_Movement_Track_Repeat(kActorGaff);
		return true;

	case kGoalGaffMA01Leave:
		AI_Movement_Track_Flush(kActorGaff);
		AI_Movement_Track_Append(kActorGaff, 124, 0);
		AI_Movement_Track_Append(kActorGaff, 35, 0);
		AI_Movement_Track_Repeat(kActorGaff);
		return true;

	case kGoalGaffCT12Leave:
		AI_Movement_Track_Flush(kActorGaff);
		AI_Movement_Track_Append(kActorGaff, 35, 90);
		AI_Movement_Track_Repeat(kActorGaff);
		return true;

	case kGoalGaffCT12FlyAway:
		Game_Flag_Reset(kFlagCT12GaffSpinner);
		Scene_Loop_Set_Default(2);
		Scene_Loop_Start_Special(kSceneLoopModeOnce, 6, true);
		return true;

	case kGoalGaffStartWalkingAround:
		AI_Movement_Track_Flush(kActorGaff);
		AI_Movement_Track_Append(kActorGaff, 35, 300);
		AI_Movement_Track_Repeat(kActorGaff);
		return true;

	case kGoalGaffWalkAround:
		{
			AI_Movement_Track_Flush(kActorGaff);
			AI_Movement_Track_Append(kActorGaff, 82, 1);
			AI_Movement_Track_Append(kActorGaff, 76, 1);
			AI_Movement_Track_Append(kActorGaff, 78, 1);
			int rnd = Random_Query(1, 3);
			if (rnd == 1) {
				AI_Movement_Track_Append(kActorGaff, 271, 0);
				AI_Movement_Track_Append(kActorGaff, 272, 0);
				AI_Movement_Track_Append(kActorGaff, 273, 0);
			} else if (rnd == 2) {
				AI_Movement_Track_Append(kActorGaff, 273, 0);
				AI_Movement_Track_Append(kActorGaff, 272, 0);
				AI_Movement_Track_Append(kActorGaff, 271, 0);
			} else if (rnd == 3) {
				AI_Movement_Track_Append(kActorGaff, 272, 0);
				AI_Movement_Track_Append(kActorGaff, 273, 0);
			}
			AI_Movement_Track_Append(kActorGaff, 274, 0);
			AI_Movement_Track_Append(kActorGaff, 275, 0);
			AI_Movement_Track_Repeat(kActorGaff);
		}
		return true;

	case kGoalGaffRepeatWalkingAround:
		AI_Movement_Track_Flush(kActorGaff);
		AI_Movement_Track_Append(kActorGaff, 35, 90);
		AI_Movement_Track_Repeat(kActorGaff);
		return true;

	case 299:
		AI_Movement_Track_Flush(kActorGaff);
		Actor_Put_In_Set(kActorGaff, kSetFreeSlotC);
		Actor_Set_At_Waypoint(kActorGaff, 35, 0);
		return true;

	case kGoalGaffMA07Wait:
		Player_Loses_Control();
		Actor_Put_In_Set(kActorGaff, kSetMA07);
		Actor_Set_At_XYZ(kActorGaff, -102.54f, -172.43f, 463.18f, 1015);
		Actor_Set_Goal_Number(kActorGaff, kGoalGaffMA07TalkToMcCoy);
		return true;

	case kGoalGaffMA07TalkToMcCoy:
		Game_Flag_Set(kFlagMA07GaffTalk);
		Actor_Face_Actor(kActorGaff, kActorMcCoy, true);
		Actor_Says(kActorGaff, 110, 12); // Gaff: Tough day, McCoy.
		Actor_Face_Actor(kActorMcCoy, kActorGaff, true);
		Actor_Says(kActorMcCoy, 2945, 14); // McCoy: You could say that.
		Loop_Actor_Walk_To_Actor(kActorGaff, kActorMcCoy, 180, false, false);
		Actor_Says(kActorGaff,   120,                 16); // Gaff: Steele is looking for you. And Bryant.
		Actor_Says(kActorGaff,   130,                 13); // Gaff: You've been keeping the whole department hopping. You and Guzza.
		Actor_Says(kActorMcCoy, 2950, kAnimationModeTalk); // McCoy: I know.
		Actor_Says(kActorMcCoy, 2955,                 16); // McCoy: You're looking for me, too, Gaff?
		Actor_Says_With_Pause(kActorGaff, 140, 1.0f, 14);
		Loop_Actor_Walk_To_Actor(kActorMcCoy, kActorGaff, 36, false, false);
		Actor_Says(kActorGaff,   150, kAnimationModeTalk); // Gaff: You know you got some interesting neighbors, McCoy.
		Actor_Says(kActorMcCoy, 2960,                 12); // McCoy: Yeah, well, I've been too busy to visit lately.
		Actor_Says_With_Pause(kActorGaff, 160, 1.0f, 16);
		Actor_Says(kActorMcCoy, 2965, 13); // McCoy: I'm thinking about it.
		Actor_Says_With_Pause(kActorGaff, 170, 1.0f, kAnimationModeTalk);
		Actor_Says_With_Pause(kActorGaff, 180, 1.0f, 12);
		Actor_Says(kActorGaff, 190, 14); // Gaff: It's like I said before. You retire a human, your career is over.
		Actor_Says(kActorGaff, 200, 15); // Gaff: Your life too, maybe.
		Async_Actor_Walk_To_XYZ(kActorGaff, -388.44f, -162.8f, 165.08f, false, false);
		Delay(4000);
		Player_Gains_Control();
		Actor_Start_Speech_Sample(kActorGaff, 210);
		Actor_Set_Goal_Number(kActorGaff, kGoalGaffMA07Left);
		return true;

	case kGoalGaffMA07Left:
		return true;

	case kGoalGaffMA07ShootMcCoy:
		Actor_Face_Actor(kActorGaff, kActorMcCoy, true);
		Actor_Change_Animation_Mode(kActorGaff, kAnimationModeCombatAttack);
		Sound_Play(kSfxSMCAL3, 100, 0, 0, 50);
		Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeDie);
		Actor_Retired_Here(kActorMcCoy, 12, 12, true, -1);
		return true;

	case kGoalGaffGone:
		AI_Movement_Track_Flush(kActorGaff);
		Actor_Put_In_Set(kActorGaff, kSetKP05_KP06);
		Actor_Set_At_XYZ(kActorGaff, -782.15f, 8.26f, -263.64f, 52);
		return true;
	}
	return false;
}

bool AIScriptGaff::UpdateAnimation(int *animation, int *frame) {
	switch (_animationState) {
	case 0:
		*animation = 794;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		break;

	case 1:
		*animation = 788;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		break;

	case 2:
		*animation = 798;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		break;

	case 3:
		*animation = 799;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationState = 2;
			_animationFrame = 0;
			*animation = 798;
		}
		break;

	case 4:
		*animation = 800;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationState = 2;
			_animationFrame = 0;
			*animation = 798;
		}
		break;

	case 5:
		*animation = 801;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationState = 2;
			_animationFrame = 0;
			*animation = 798;
		}
		break;

	case 6:
		*animation = 800;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationState = 2;
			_animationFrame = 0;
			*animation = 798;
		}
		break;

	case 7:
		*animation = 801;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationState = 2;
			_animationFrame = 0;
			*animation = 798;
		}
		break;

	case 8:
		*animation = 802;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(35) - 1) { // Bug in the game?
			Actor_Set_Invisible(kActorGaff, true);
			*animation = 794;
			_animationFrame = 0;
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeIdle); // TODO: check this, it was set directly by calling actor script
			Actor_Set_Goal_Number(kActorGaff, kGoalGaffCT12Leave);
		}
		break;

	default:
		*animation = 399;
		break;
	}
	*frame = _animationFrame;
	return true;
}

bool AIScriptGaff::ChangeAnimationMode(int mode) {
	switch (mode) {
	case kAnimationModeIdle:
		_animationState = 0;
		_animationFrame = 0;
		break;

	case kAnimationModeWalk:
		if (_animationState != 1) {
			_animationState = 1;
			_animationFrame = 0;
		}
		break;

	case kAnimationModeTalk:
		_animationState = 2;
		_animationFrame = 0;
		break;

	case 12:
		_animationState = 3;
		_animationFrame = 0;
		break;

	case 13:
		_animationState = 4;
		_animationFrame = 0;
		break;

	case 14:
		_animationState = 5;
		_animationFrame = 0;
		break;

	case 15:
		_animationState = 6;
		_animationFrame = 0;
		break;

	case 16:
		_animationState = 7;
		_animationFrame = 0;
		break;

	case kAnimationModeSpinnerGetIn:
		_animationState = 8;
		_animationFrame = 0;
		break;
	}
	return true;
}

void AIScriptGaff::QueryAnimationState(int *animationState, int *animationFrame, int *animationStateNext, int *animationNext) {
	*animationState     = _animationState;
	*animationFrame     = _animationFrame;
	*animationStateNext = _animationStateNext;
	*animationNext      = _animationNext;
}

void AIScriptGaff::SetAnimationState(int animationState, int animationFrame, int animationStateNext, int animationNext) {
	_animationState     = animationState;
	_animationFrame     = animationFrame;
	_animationStateNext = animationStateNext;
	_animationNext      = animationNext;
}

bool AIScriptGaff::ReachedMovementTrackWaypoint(int waypointId) {
	return true;
}

void AIScriptGaff::FledCombat() {
	// return false;
}

} // End of namespace BladeRunner
