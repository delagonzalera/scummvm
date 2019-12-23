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

AIScriptRachael::AIScriptRachael(BladeRunnerEngine *vm) : AIScriptBase(vm) {
	_flag = true;
}

void AIScriptRachael::Initialize() {
	_animationFrame = 0;
	_animationState = 0;
	_animationStateNext = 0;
	_animationNext = 0;

	_flag = true;

	Actor_Set_Goal_Number(kActorRachael, kGoalRachaelDefault);
}

bool AIScriptRachael::Update() {
	if (_vm->_cutContent) {
		if (Global_Variable_Query(kVariableChapter) == 3) {
			if (Actor_Query_Goal_Number(kActorRachael) == kGoalRachaelLeavesAfterTyrellMeeting) {
				// Decide 50-50 whether Rachael will be encountered outside or in the elevator in Act 3
				if (Random_Query(1,2 ) == 1) {
					Actor_Set_Goal_Number(kActorRachael, kGoalRachaelShouldBeOutsideMcCoysAct3);
				} else {
					Actor_Set_Goal_Number(kActorRachael, kGoalRachaelShouldBeInElevatorMcCoysAct3);
				}
				return true;
			}
		} else if (Global_Variable_Query(kVariableChapter) == 4) {
			if (Actor_Query_Goal_Number(kActorRachael) < 399) {
				Actor_Set_Goal_Number(kActorRachael, kGoalRachaelAtEndOfAct3IfNotMetWithMcCoy);
				return true;
			}
		} else if (Global_Variable_Query(kVariableChapter) > 4
		        && Actor_Query_Goal_Number(kActorRachael) != kGoalRachaelAtEndOfAct4) {
				Actor_Set_Goal_Number(kActorRachael, kGoalRachaelAtEndOfAct4);
				return true;
		}
	}
	return false;
}

void AIScriptRachael::TimerExpired(int timer) {
	//return false;
}

void AIScriptRachael::CompletedMovementTrack() {
	//return false;
}

void AIScriptRachael::ReceivedClue(int clueId, int fromActorId) {
	//return false;
}

void AIScriptRachael::ClickedByPlayer() {
	if (Actor_Query_Goal_Number(kActorRachael) == 305) {
		Actor_Face_Actor(kActorMcCoy, kActorRachael, true);
		Actor_Says(kActorMcCoy, 2730, 12); // McCoy: Rachael, right?
		AI_Movement_Track_Pause(kActorRachael);
		dialogue_start(); // "I remember you mr McCoy" till "I'm fine, thank you for asking."

		// the structure is simplified (maintaining the same logic flow)
		if ((Player_Query_Agenda() == kPlayerAgendaSurly || Player_Query_Agenda() == kPlayerAgendaErratic)
			|| (Player_Query_Agenda() != kPlayerAgendaPolite && Actor_Query_Friendliness_To_Other(kActorSteele, kActorMcCoy) > Actor_Query_Friendliness_To_Other(kActorClovis, kActorMcCoy))
		) {
			dialogue_agenda2();
		} else {
			dialogue_agenda1();
		}

		Actor_Set_Goal_Number(kActorRachael, kGoalRachaelIsOutResumesWalkToPoliceHQAct3);
		AI_Movement_Track_Unpause(kActorRachael);
	}
}

void AIScriptRachael::EnteredScene(int sceneId) {
	// return false;
}

void AIScriptRachael::OtherAgentEnteredThisScene(int otherActorId) {
	// return false;
}

void AIScriptRachael::OtherAgentExitedThisScene(int otherActorId) {
	// return false;
}

void AIScriptRachael::OtherAgentEnteredCombatMode(int otherActorId, int combatMode) {
	// return false;
}

void AIScriptRachael::ShotAtAndMissed() {
	// return false;
}

bool AIScriptRachael::ShotAtAndHit() {
	return false;
}

void AIScriptRachael::Retired(int byActorId) {
	// return false;
}

int AIScriptRachael::GetFriendlinessModifierIfGetsClue(int otherActorId, int clueId) {
	return 0;
}

bool AIScriptRachael::GoalChanged(int currentGoalNumber, int newGoalNumber) {
	switch (newGoalNumber) {
	case kGoalRachaelLeavesAfterTyrellMeeting:
		AI_Movement_Track_Flush(kActorRachael);
		AI_Movement_Track_Append(kActorRachael, 379, 0);
		AI_Movement_Track_Append(kActorRachael, 39, 0);
		AI_Movement_Track_Repeat(kActorRachael);
		break;

	case kGoalRachaelIsOutsideMcCoysBuildingAct4:
		// added goal for restored content
		// fall through
	case kGoalRachaelIsOutsideMcCoysBuildingAct3:
		// This puts Rachael outside McCoy's building
		Actor_Put_In_Set(kActorRachael, kSetMA07);
		Actor_Set_At_XYZ(kActorRachael, -8.09f, -162.8f, 135.33f, 544);
		break;

	case kGoalRachaelIsInsideElevatorStartTalkAct3:
		// added goal for restored content
		dialogue_start(); // "I remember you mr McCoy" till "I'm fine, thank you for asking."

		// the structure is simplified (maintaining the same logic flow)
		if ((Player_Query_Agenda() == kPlayerAgendaSurly || Player_Query_Agenda() == kPlayerAgendaErratic)
			|| (Player_Query_Agenda() != kPlayerAgendaPolite && Actor_Query_Friendliness_To_Other(kActorSteele, kActorMcCoy) > Actor_Query_Friendliness_To_Other(kActorClovis, kActorMcCoy))
		) {
			dialogue_agenda2();
		} else {
			dialogue_agenda1();
		}
		break;

	case kGoalRachaelIsInsideElevatorStartTalkAct4:
		dialogue_act4();
		break;

	case kGoalRachaelIsOutWalksToPoliceHQAct4:
		// added goal for restored content
		// fall through
	case kGoalRachaelIsOutWalksToPoliceHQAct3:
		// Rachael's goal is set to this when the player walks in the MA07 scene and if her goal is already kGoalRachaelIsOutsideMcCoysBuildingAct3
		AI_Movement_Track_Flush(kActorRachael);
		// This makes Rachael (who is right outside McCoy's building) head towards the Police Station (left)
		AI_Movement_Track_Append(kActorRachael, 468, 0);
		AI_Movement_Track_Append(kActorRachael, 39, 0);
		AI_Movement_Track_Repeat(kActorRachael);
		break;

	case kGoalRachaelAtEndOfAct3IfNotMetWithMcCoy:
		// added goal for restored content
		// fall through
	case kGoalRachaelAtEndOfAct4:
		// added goal for restored content
		// fall through
	case kGoalRachaelAtEndOfAct3IfMetWithMcCoy:
		Actor_Put_In_Set(kActorRachael, kSetFreeSlotG);
		Actor_Set_At_Waypoint(kActorRachael, 39, 0);
		break;

	default:
		return false;
	}

	return true;
}

bool AIScriptRachael::UpdateAnimation(int *animation, int *frame) {
	switch (_animationState) {
	case 0:
		*animation = kModelAnimationRachaelIdle;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(kModelAnimationRachaelIdle) - 1) {
			_animationFrame = 0;
		}
		break;

	case 1:
		*animation = kModelAnimationRachaelWalking;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(kModelAnimationRachaelWalking) - 1) {
			_animationFrame = 0;
		}
		break;

	case 2:
		if (!_animationFrame && _flag) {
			*animation = kModelAnimationRachaelIdle;
			_animationState = 0;
		} else {
			*animation = kModelAnimationRachaelTalkSoftNod;
			_animationFrame++;
			if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(kModelAnimationRachaelTalkSoftNod) - 1) {
				_animationFrame = 0;
			}
		}
		break;

	case 3:
		*animation = kModelAnimationRachaelTalkNodToLeft;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(kModelAnimationRachaelTalkNodToLeft) - 1) {
			_animationFrame = 0;
			_animationState = 2;
			*animation = kModelAnimationRachaelTalkSoftNod;
		}
		break;

	case 4:
		*animation = kModelAnimationRachaelTalkSuggestWithNodToLeft;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(kModelAnimationRachaelTalkSuggestWithNodToLeft) - 1) {
			_animationFrame = 0;
			_animationState = 2;
			*animation = kModelAnimationRachaelTalkSoftNod;
		}
		break;

	case 5:
		*animation = kModelAnimationRachaelTalkIndiffWithNodToLeft;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(kModelAnimationRachaelTalkIndiffWithNodToLeft) - 1) {
			_animationFrame = 0;
			_animationState = 2;
			*animation = kModelAnimationRachaelTalkSoftNod;
		}
		break;

	case 6:
		*animation = kModelAnimationRachaelTalkOfferPointing;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(kModelAnimationRachaelTalkOfferPointing) - 1) {
			_animationFrame = 0;
			_animationState = 2;
			*animation = kModelAnimationRachaelTalkSoftNod;
		}
		break;

	case 7:
		*animation = kModelAnimationRachaelTalkHaltMovement;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(kModelAnimationRachaelTalkHaltMovement) - 1) {
			_animationFrame = 0;
			_animationState = 2;
			*animation = kModelAnimationRachaelTalkSoftNod;
		}
		break;

	case 8:
		*animation = kModelAnimationRachaelTalkHandOnChest;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(kModelAnimationRachaelTalkHandOnChest) - 1) {
			_animationFrame = 0;
			_animationState = 2;
			*animation = kModelAnimationRachaelTalkSoftNod;
		}
		break;

	case 9:
		*animation = kModelAnimationRachaelTalkHandWaveToRight;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(kModelAnimationRachaelTalkHandWaveToRight) - 1) {
			_animationFrame = 0;
			_animationState = 2;
			*animation = kModelAnimationRachaelTalkSoftNod;
		}
		break;

	default:
		break;
	}
	*frame = _animationFrame;

	return true;
}

bool AIScriptRachael::ChangeAnimationMode(int mode) {
	switch (mode) {
	case kAnimationModeIdle:
		if (_animationState >= 2 && _animationState <= 9) { // talking states
			_flag = 1;
		} else {
			_animationState = 0; // idle state
			_animationFrame = 0;
		}
		break;
	case kAnimationModeWalk:
		_animationState = 1;
		_animationFrame = 0;
		break;
	case kAnimationModeTalk:
		_animationState = 2;
		_animationFrame = 0;
		_flag = 0;
		break;
	case 12:
		_animationState = 3;
		_animationFrame = 0;
		_flag = 0;
		break;
	case 13:
		_animationState = 4;
		_animationFrame = 0;
		_flag = 0;
		break;
	case 14:
		_animationState = 5;
		_animationFrame = 0;
		_flag = 0;
		break;
	case 15:
		_animationState = 6;
		_animationFrame = 0;
		_flag = 0;
		break;
	case 16:
		_animationState = 7;
		_animationFrame = 0;
		_flag = 0;
		break;
	case 17:
		_animationState = 8;
		_animationFrame = 0;
		_flag = 0;
		break;
	case 18:
		_animationState = 9;
		_animationFrame = 0;
		_flag = 0;
		break;
	default:
		break;
	}

	return true;
}

void AIScriptRachael::QueryAnimationState(int *animationState, int *animationFrame, int *animationStateNext, int *animationNext) {
	*animationState     = _animationState;
	*animationFrame     = _animationFrame;
	*animationStateNext = _animationStateNext;
	*animationNext      = _animationNext;
}

void AIScriptRachael::SetAnimationState(int animationState, int animationFrame, int animationStateNext, int animationNext) {
	_animationState     = animationState;
	_animationFrame     = animationFrame;
	_animationStateNext = animationStateNext;
	_animationNext      = animationNext;
}

bool AIScriptRachael::ReachedMovementTrackWaypoint(int waypointId) {
	return true;
}

void AIScriptRachael::FledCombat() {
	// return false;
}

void AIScriptRachael::dialogue_start() {
	if (_vm->_cutContent) {
			Actor_Face_Actor(kActorRachael, kActorMcCoy, true);
			Loop_Actor_Walk_To_Actor(kActorRachael, kActorMcCoy, 84, false, false);
			if (_vm->_cutContent) {
				Actor_Says(kActorMcCoy, 2735, 14); // McCoy: We met at your uncle's office.
			}
			Actor_Says(kActorRachael,    0, 15); // Rachael: I remember you Mr. McCoy.
			Actor_Says(kActorMcCoy,   2740, 13); // McCoy: You live here? I didn't know we were in the same building.
			Actor_Says(kActorRachael,   10, 14); // Rachael: No I... I'm visiting somebody.
			Actor_Says(kActorMcCoy,   2745, 13); // McCoy: Who? Maybe I know him.
			Actor_Says(kActorRachael,   20, 12); // Rachael: Perhaps it's none of your business.
			Actor_Says_With_Pause(kActorMcCoy, 2750, 1.5f, 3); // OkGetThePicture
			Actor_Says(kActorMcCoy,   2755,  3); // McCoy: You all right? You look a little pale.
			Actor_Says(kActorRachael,   30, 13); // Rachael: I've certainly had better days in my life, but I'm fine.
			Actor_Says(kActorRachael,   40, 15); // Rachael: Thank you for asking.
	} else {
		// original code -- un-triggered
		Loop_Actor_Walk_To_Actor(kActorRachael, kActorMcCoy, 84, false, false);
		Actor_Says(kActorRachael,    0, 15); // Rachael: I remember you Mr. McCoy.
		Actor_Says(kActorMcCoy,   2740, 13); // McCoy: You live here? I didn't know we were in the same building.
		Actor_Says(kActorRachael,   10, 14); // Rachael: No I... I'm visiting somebody.
		Actor_Says(kActorMcCoy,   2745, 13); // McCoy: Who? Maybe I know him.
		Actor_Says(kActorRachael,   20, 12); // Rachael: Perhaps it's none of your business.
		Actor_Says_With_Pause(kActorMcCoy, 2750, 1.5f, 3);  // ok I get the picture
		Actor_Says(kActorRachael, 30, 13); // Rachael: I've certainly had better days in my life, but I'm fine.
		Actor_Says(kActorRachael, 40, 15); // Rachael: Thank you for asking.
	}
}

void AIScriptRachael::dialogue_agenda1() {
	// polite or Rep friendly case
	if (_vm->_cutContent) {
		Actor_Says(kActorMcCoy,   2795, 13); // McCoy: You're upset.
		Actor_Says(kActorRachael,  140, 15); // Rachael: How observant.
		Actor_Says(kActorMcCoy,   2800, 12); // McCoy: What's the matter? You can tell me. I'm a good listener.
		Actor_Says(kActorRachael,  150, 16); // Rachael: I'm sure you are Mr. McCoy. I just don't much feel like talking at the moment.
		Actor_Says(kActorMcCoy,   2805, 18); // McCoy: Did your uncle say something to you?
		Actor_Says(kActorRachael,  160, 14); // Rachael: He can be so cruel.
		Actor_Says(kActorRachael,  170, 13); // Rachael: Here I am, pouring my heart out to a veritable killer. It's all so absurd.
		Actor_Says(kActorMcCoy,   2810, 14); // McCoy: That's not how I look at it.
		Actor_Says(kActorRachael,  180, 13); // Rachael: Sure. You've got to live with yourself somehow.
		Actor_Says(kActorMcCoy,   2815, 12); // McCoy: I'm not just some killing machine, Rachael.
		Actor_Says(kActorMcCoy,   2820, 13); // McCoy: I've thought about-- well, lately I've been reevaluating the job. I don't like it. I never have.
		Actor_Says(kActorRachael,  190, 15); // Rachael: Why don't you quit then?
		Actor_Says(kActorMcCoy,   2825, 12); // McCoy: Someone would just take my place.
		Actor_Says(kActorRachael,  200, 15); // Rachael: So much for integrity.
		Actor_Says(kActorMcCoy,   2830, 14); // McCoy: It's not that.
		Actor_Says(kActorRachael,  210, 15); // Rachael: It's the money, then. Isn't it always the money in this world?
		Actor_Says(kActorRachael,  220, 16); // Rachael: God, I'm so fed up with all of it.
		Actor_Says(kActorMcCoy,   2835, 13); // McCoy: I've been thinking about the treatment of Replicants in our world.
		Actor_Says(kActorRachael,  230, 14); // Rachael: Bravo. Should we call a press conference?
		Actor_Says(kActorMcCoy,   2840, 12); // McCoy: Maybe they've gotten a raw deal.
		Actor_Says(kActorRachael,  240, 13); // Rachael: Imagine somebody engineering your mind.
		Actor_Says(kActorRachael,  250, 15); // Rachael: Putting whatever thoughts and memories they wanted inside of it.
		Actor_Says(kActorRachael,  260, 16); // Rachael: Nothing. Nothing in this world would belong to you, would it?
		Actor_Says(kActorMcCoy,   2845, 13); // McCoy: No.
		Actor_Says(kActorRachael,  270, 13); // Rachael: Think about it Mr. McCoy.
		Actor_Says(kActorRachael,  280, 14); // Rachael: Maybe you aren't so different from those Replicants after all.
		Actor_Says(kActorMcCoy,   2850, 13); // McCoy: I suppose we're all Replicants in one way or another.
		Actor_Says(kActorMcCoy,   2855, 14); // McCoy: Programmed to do things, to think things.
		Actor_Says(kActorRachael,  290, 14); // Rachael: That's right.
		if (Actor_Query_In_Set(kActorRachael, kSetMA07)) {
			Actor_Says_With_Pause(kActorRachael, 300, 1.0f, 3); // GoodbyeMcCoy
			Actor_Says(kActorMcCoy, 2860, 14); // McCoy: You take care of yourself.
		}
	} else {
		// original code
		Actor_Says(kActorMcCoy,   2795, 13); // McCoy: You're upset.
		Actor_Says(kActorRachael,  140, 15); // Rachael: How observant.
		Actor_Says(kActorMcCoy,   2800, 12); // McCoy: What's the matter? You can tell me. I'm a good listener.
		Actor_Says(kActorRachael,  150, 16); // Rachael: I'm sure you are Mr. McCoy. I just don't much feel like talking at the moment.
		Actor_Says(kActorMcCoy,   2805, 18); // McCoy: Did your uncle say something to you?
		Actor_Says(kActorRachael,  160, 14); // Rachael: He can be so cruel.
		Actor_Says(kActorRachael,  170, 13); // Rachael: Here I am, pouring my heart out to a veritable killer. It's all so absurd.
		Actor_Says(kActorMcCoy,   2810, 14); // McCoy: That's not how I look at it.
		Actor_Says(kActorRachael,  180, 13); // Rachael: Sure. You've got to live with yourself somehow.
		Actor_Says(kActorMcCoy,   2815, 12); // McCoy: I'm not just some killing machine, Rachael.
		Actor_Says(kActorMcCoy,   2820, 13); // McCoy: I've thought about-- well, lately I've been reevaluating the job. I don't like it. I never have.
		Actor_Says(kActorRachael,  190, 15); // Rachael: Why don't you quit then?
		Actor_Says(kActorMcCoy,   2825, 12); // McCoy: Someone would just take my place.
		Actor_Says(kActorRachael,  200, 15); // Rachael: So much for integrity.
		Actor_Says(kActorMcCoy,   2830, 14); // McCoy: It's not that.
		Actor_Says(kActorRachael,  210, 15); // Rachael: It's the money, then. Isn't it always the money in this world?
		Actor_Says(kActorRachael,  220, 16); // Rachael: God, I'm so fed up with all of it.
		Actor_Says(kActorMcCoy,   2835, 13); // McCoy: I've been thinking about the treatment of Replicants in our world.
		Actor_Says(kActorRachael,  230, 14); // Rachael: Bravo. Should we call a press conference?
		Actor_Says(kActorMcCoy,   2840, 12); // McCoy: Maybe they've gotten a raw deal.
		Actor_Says(kActorRachael,  240, 13); // Rachael: Imagine somebody engineering your mind.
		Actor_Says(kActorRachael,  250, 15); // Rachael: Putting whatever thoughts and memories they wanted inside of it.
		Actor_Says(kActorRachael,  260, 16); // Rachael: Nothing. Nothing in this world would belong to you, would it?
		Actor_Says(kActorMcCoy,   2845, 13); // McCoy: No.
		Actor_Says(kActorRachael,  270, 13); // Rachael: Think about it Mr. McCoy.
		Actor_Says(kActorRachael,  280, 14); // Rachael: Maybe you aren't so different from those Replicants after all.
		Actor_Says(kActorMcCoy,   2850, 13); // McCoy: I suppose we're all Replicants in one way or another.
		Actor_Says(kActorRachael,  290, 14); // Rachael: That's right.
		Actor_Says_With_Pause(kActorRachael, 300, 1.0f, 3); // GoodbyeMcCoy
		Actor_Says(kActorMcCoy, 2860, 14); // McCoy: You take care of yourself.
	}
}

void AIScriptRachael::dialogue_agenda2() {
	// surly/erratic or human friendly case
	if (_vm->_cutContent) {
		Actor_Says(kActorMcCoy,   2760, 14); // McCoy: Interesting guy your uncle. Quite a character.
		Actor_Says(kActorRachael,   50, 15); // Rachael: I don't think he'd appreciate being called that.
		Actor_Says(kActorMcCoy,   2765, 16); // McCoy: Oh, I'm sure he's a brilliant man.
		Actor_Says(kActorMcCoy,   2770, 17); // McCoy: Anyone who could create the Nexus-6, the state of the art of Rep technology...
		Actor_Says(kActorRachael,   60, 14); // Rachael: He's keeping people like you employed, isn't he?
		Actor_Says(kActorMcCoy,   2775, 16); // McCoy: Hey, I'd just as soon not do this job.
		Actor_Says(kActorRachael,   70, 13); // Rachael: Do you really expect me to believe that Mr. McCoy?
		Actor_Says(kActorRachael,   80, 14); // Rachael: I saw that look in your eye.
		Actor_Says(kActorRachael,   90, 15); // Rachael: You'd like nothing better than to kill innocent people, isn't that right?
		Actor_Says(kActorMcCoy,   2780, 17); // McCoy: Replicants aren't people.
		Actor_Says(kActorRachael,  100, 16); // Rachael: But they certainly are more innocent than most of the people I've ever met.
		Actor_Says(kActorRachael,  110, 15); // Rachael: They didn't ask to be brought into this world.
		Actor_Says(kActorMcCoy,   2785, 17); // McCoy: Nobody does.
		Actor_Says(kActorRachael,  120, 13); // Rachael: That's right. And nobody has a license to hunt down and kill humans. Isn't that true?
		Actor_Says(kActorMcCoy,   2790, 18); // McCoy: Uh-huh.
		Actor_Says(kActorRachael,  130, 14); // Rachael: But Replicants, how humane.
		if (Actor_Query_In_Set(kActorRachael, kSetMA07)) {
			Actor_Says_With_Pause(kActorRachael, 300, 1.0f, 3); // GoodbyeMcCoy
			Actor_Says(kActorMcCoy, 2860, 14); // McCoy: You take care of yourself.
		}
	} else {
		// original code - missing some quotes or removing some quotes to make it fit better
		Actor_Says(kActorRachael,   50, 15); // Rachael: I don't think he'd appreciate being called that.
		Actor_Says(kActorMcCoy,   2765, 16); // McCoy: Oh, I'm sure he's a brilliant man.
		Actor_Says(kActorMcCoy,   2770, 17); // McCoy: Anyone who could create the Nexus-6, the state of the art of Rep technology...
		Actor_Says(kActorRachael,   60, 14); // Rachael: He's keeping people like you employed, isn't he?
		Actor_Says(kActorMcCoy,   2775, 16); // McCoy: Hey, I'd just as soon not do this job.
		Actor_Says(kActorRachael,   70, 13); // Rachael: Do you really expect me to believe that Mr. McCoy?
		Actor_Says(kActorRachael,   80, 14); // Rachael: I saw that look in your eye.
		Actor_Says(kActorRachael,   90, 15); // Rachael: You'd like nothing better than to kill innocent people, isn't that right?
		Actor_Says(kActorMcCoy,   2780, 17); // McCoy: Replicants aren't people.
		Actor_Says(kActorRachael,  100, 16); // Rachael: But they certainly are more innocent than most of the people I've ever met.
		Actor_Says(kActorRachael,  110, 15); // Rachael: They didn't ask to be brought into this world.
		Actor_Says(kActorMcCoy,   2785, 17); // McCoy: Nobody does.
		Actor_Says(kActorRachael,  120, 13); // Rachael: That's right. And nobody has a license to hunt down and kill humans. Isn't that true?
		Actor_Says(kActorMcCoy,   2790, 16); // McCoy: Uh-huh.
		Actor_Says(kActorRachael,  130, 14); // Rachael: But Replicants, how humane.
		Actor_Says_With_Pause(kActorRachael, 300, 1.0f, 3); // GoodbyeMcCoy
		Actor_Says(kActorMcCoy, 2860, 14); // McCoy: You take care of yourself.
	}
}

// Not used in the game
void AIScriptRachael::dialogue_act4() {
	Actor_Says(kActorMcCoy, 2865, 17); // McCoy: Lobby.
	if (_vm->_cutContent) {
		Game_Flag_Set(kFlagMA06toMA07); // to Ground Floor / Lobby
		Delay(500);
		AI_Movement_Track_Pause(kActorRachael);
		Actor_Face_Actor(kActorRachael, kActorMcCoy, true);
	}
	Actor_Says(kActorRachael, 320, 12); // Rachael: McCoy.
	Actor_Says(kActorRachael, 330, 17); // Rachael: Rachael. Remember me?
	if (_vm->_cutContent) {
		Actor_Face_Actor(kActorMcCoy, kActorRachael, true);
	}
	Actor_Says(kActorMcCoy,   2870, 13); // McCoy: Jesus. I don't know what I remember anymore.
	Actor_Says(kActorRachael,  340, 12); // Rachael: Now you look like you're in trouble.
	Actor_Says(kActorMcCoy,   2875, 15); // McCoy: You saw me here before, didn't you?
	Actor_Says(kActorRachael,  350,  3); // Rachael: Yes. What happened?
	Actor_Says(kActorMcCoy,   2880, 16); // McCoy: It's suddenly as if I didn't exist. As if everything I knew and accepted is gone.
	Actor_Says(kActorMcCoy,   2885, 12); // McCoy: And my animal, Maggie, my prized possession disappeared.
	Actor_Says(kActorRachael,  360,  3); // Rachael: I'm sorry.
	Actor_Says(kActorRachael,  370,  3); // Rachael: I know the feeling.
	Actor_Says(kActorMcCoy,   2890, 18); // McCoy: You do?
	Actor_Says(kActorRachael,  380, 18); // Rachael: Everything we believe, those things we call reality?
	Actor_Says(kActorRachael,  390, 12); // Rachael: Well, what is reality?
	Actor_Says(kActorRachael,  400, 13); // Rachael: Maybe it's all someone else's fantasy.
	Actor_Says(kActorMcCoy,   2895, 14); // McCoy: That would make us a fantasy.
	Actor_Says(kActorRachael,  410, 15); // Rachael: That's right. And in the blink of an eye it can all go away.
	Actor_Says(kActorMcCoy,   2900, 16); // McCoy: But just yesterday, I--
	Actor_Says(kActorRachael,  420, 14); // Rachael: Yesterday, two months ago, ten years ago. What's the difference?
	Actor_Says(kActorMcCoy,   2905, 13); // McCoy: But if we both remembered--
	Actor_Says(kActorRachael,  430, 16); // Rachael: Copies. Only copies.
	Actor_Says(kActorRachael,  440, 12); // Rachael: The only thing we can trust is right now. Not tomorrow and certainly not yesterday.
	Actor_Says(kActorMcCoy,   2910, 14); // McCoy: No, I'm not losing my mind. Someone is trying to mess with me. The police are saying I'm an escaped Replicant.
	Actor_Says(kActorMcCoy,   2920, 17); // McCoy: Someone must be setting me up for some reason. Using me.
	Actor_Says(kActorRachael,  450,  3); // Rachael: There's nothing wrong with accepting what you are.
	Actor_Says(kActorMcCoy,   2925, 15); // McCoy: I'm not a Replicant goddamn it.
	Actor_Says(kActorMcCoy,   2930, 14); // McCoy: Look, maybe you can help me. You're Tyrell's niece, you can vouch for me.
	Actor_Says(kActorRachael,  460, 13); // Rachael: Why don't you just take that Voigt Kampff test. That would settle it, wouldn't it?
	Actor_Says(kActorMcCoy,   2935, 19); // McCoy: Yeah, that's a good idea. Maybe I'll do that.
	Actor_Says(kActorRachael,  470, 18); // Rachael: I hope you get the answers you're looking for, McCoy.
}

} // End of namespace BladeRunner
