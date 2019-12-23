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

AIScriptLucy::AIScriptLucy(BladeRunnerEngine *vm) : AIScriptBase(vm) {
	_flag = 0;
}

void AIScriptLucy::Initialize() {
	_animationFrame = 0;
	_animationState = 0;
	_animationStateNext = 0;
	_animationNext = 0;

	_flag = 0;

	Actor_Set_Goal_Number(kActorLucy, kGoalLucyDefault);
}

bool AIScriptLucy::Update() {
	float x, y, z;

	if (Global_Variable_Query(kVariableChapter) == 3
	 && Actor_Query_Goal_Number(kActorLucy) < kGoalLucyMoveAround
	) {
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyMoveAround);
	}

	if (Actor_Query_Goal_Number(kActorLucy) == kGoalLucyHF04Start
	 && Player_Query_Current_Scene() == kSceneHF04
	) {
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF04Run1);
	}

	if (Global_Variable_Query(kVariableChapter) == 4
	 && Actor_Query_Goal_Number(kActorLucy) < kGoalLucyStartChapter4
	) {
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyStartChapter4);
	}

	if (Global_Variable_Query(kVariableChapter) == 4
	 && Actor_Query_Goal_Number(kActorLucy) == kGoalLucyGone
	 && Actor_Query_Which_Set_In(kActorLucy) != kSetFreeSlotI
	) {
		if (Actor_Query_Which_Set_In(kActorLucy) != Player_Query_Current_Set()) {
			Actor_Put_In_Set(kActorLucy, kSetFreeSlotI);
			Actor_Set_At_Waypoint(kActorLucy, 41, 0);
		}
	}

	if (Game_Flag_Query(kFlagMcCoyCapturedByHolloway)
	 && Actor_Query_Goal_Number(kActorLucy) == kGoalLucyWillReturnToHF03
	) {
		Actor_Put_In_Set(kActorLucy, kSetHF03);
		Actor_Set_At_Waypoint(kActorLucy, 371, 156);
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyReturnToHF03);
	}

	if ( Actor_Query_Goal_Number(kActorLucy) > 229
	 &&  Actor_Query_Goal_Number(kActorLucy) < 239
	 &&  Actor_Query_Goal_Number(kActorLucy) != kGoalLucyHF04TalkToMcCoy
	 &&  Player_Query_Current_Scene() == kSceneHF04
	 &&  Actor_Query_Which_Set_In(kActorLucy) == kSetHF04
	 && !Game_Flag_Query(kFlagMcCoyAttackedLucy)
	 &&  Actor_Query_Inch_Distance_From_Actor(kActorLucy, kActorMcCoy) < 84
	 && !Player_Query_Combat_Mode()
	 &&  Actor_Query_Friendliness_To_Other(kActorLucy, kActorMcCoy) > 40
	) {
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF04TalkToMcCoy); // how can this be triggered?
	}

	if (Actor_Query_Goal_Number(kActorLucy) == kGoalLucyHF04WaitForMcCoy1) {
		Actor_Query_XYZ(kActorMcCoy, &x, &y, &z);
		if (z < -875.0f) {
			Game_Flag_Set(kFlagHF04OpenDoors);
			Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF04Run3);
		}
	}

	if (Actor_Query_Goal_Number(kActorLucy) == kGoalLucyHF04WaitForMcCoy2) {
		Actor_Query_XYZ(kActorMcCoy, &x, &y, &z);
		if (x > 350.0f) {
			Game_Flag_Set(kFlagHF04CloseDoors);
			Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF04Run4);
		}
	}

	return false;
}

void AIScriptLucy::TimerExpired(int timer) {
	if (timer == kActorTimerAIScriptCustomTask0) { // rephrased this to be more expandable (if required)
		AI_Countdown_Timer_Reset(kActorLucy, kActorTimerAIScriptCustomTask0);
		if (Actor_Query_Goal_Number(kActorLucy) == kGoalLucyGoToHF03) {
			if (Player_Query_Current_Scene() == kSceneHF03) {
				AI_Countdown_Timer_Start(kActorLucy, kActorTimerAIScriptCustomTask0, 20);
			} else {
				Actor_Set_Goal_Number(kActorLucy, kGoalLucyMoveAround);
			}
		}
	}
}

void AIScriptLucy::CompletedMovementTrack() {
	switch (Actor_Query_Goal_Number(kActorLucy)) {
	case kGoalLucyGoToHF03:
		if (Game_Flag_Query(kFlagMcCoyCapturedByHolloway)
		 && Global_Variable_Query(kVariableHollowayArrest) == 3
		) {
			Actor_Set_Goal_Number(kActorLucy, kGoalLucyReturnToHF03);
			return; //true;
		}
		AI_Countdown_Timer_Reset(kActorLucy, kActorTimerAIScriptCustomTask0);
		AI_Countdown_Timer_Start(kActorLucy, kActorTimerAIScriptCustomTask0, 30);
		break;

	case kGoalLucyHF03RunOutPhase1:
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF03RunOutPhase2);
		break;

	case kGoalLucyHF03RunOutPhase2:
		Game_Flag_Set(kFlagLucyRanAway);
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF03RanAway);
		break;

	case kGoalLucyHF03RunToHF041:
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF03RunToHF042);
		break;

	case kGoalLucyHF03RunToHF042:
		if (Actor_Clue_Query(kActorLucy, kClueMcCoyHelpedLucy)
		 && Global_Variable_Query(kVariableHollowayArrest) != 3
		) {
			Game_Flag_Set(kFlagLucyRanAway);
		} else {
			Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF04Start);
			Game_Flag_Reset(kFlagHF04DoorsClosed);
		}
		break;

	case kGoalLucyHF03RunAwayWithHelp1:
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF03RunAwayWithHelp2);
		break;

	case kGoalLucyHF03RunAwayWithHelp2:
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyWillReturnToHF03);
		break;

	case kGoalLucyGoToFreeSlotGAG:
	case kGoalLucyGoToFreeSlotGAHJ:
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyMoveAround);
		break;

	case kGoalLucyHF04Run1:
		Game_Flag_Set(kFlagHF04CloseDoors);
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF04Run2);
		break;

	case kGoalLucyHF04Run2:
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF04WaitForMcCoy1);
		break;

	case kGoalLucyHF04Run3:
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF04WaitForMcCoy2);
		break;

	case kGoalLucyHF04Run4:
		Game_Flag_Set(kFlagLucyRanAway);
		Actor_Put_In_Set(kActorLucy, kSetFreeSlotA);
		Actor_Set_At_Waypoint(kActorLucy, 33, 0);
		Actor_Set_Health(kActorLucy, 30, 30);

		if (Global_Variable_Query(kVariableHollowayArrest) == 3) {
			Actor_Set_Goal_Number(kActorSteele, kGoalSteeleHF02ConfrontLucy);
		}
		break;

	case kGoalLucyHF04WalkAway:
		Game_Flag_Set(kFlagLucyRanAway);
		break;

	default:
		break;
	}
	return; //false;
}

void AIScriptLucy::ReceivedClue(int clueId, int fromActorId) {
	//return false;
}

void AIScriptLucy::ClickedByPlayer() {
	if (Actor_Query_Goal_Number(kActorLucy) == kGoalLucyGone) {
		Actor_Face_Actor(kActorMcCoy, kActorLucy, true);
		Actor_Says(kActorMcCoy, 8630, kAnimationModeTalk); // McCoy: What a waste.
	}
}

void AIScriptLucy::EnteredScene(int sceneId) {
	// return false;
}

void AIScriptLucy::OtherAgentEnteredThisScene(int otherActorId) {
	// return false;
}

void AIScriptLucy::OtherAgentExitedThisScene(int otherActorId) {
	// return false;
}

void AIScriptLucy::OtherAgentEnteredCombatMode(int otherActorId, int combatMode) {
	// return false;
}

void AIScriptLucy::ShotAtAndMissed() {
	checkCombat();
}

bool AIScriptLucy::ShotAtAndHit() {
	checkCombat();

	return false;
}

void AIScriptLucy::Retired(int byActorId) {
	if (byActorId == kActorMcCoy) {
		Actor_Modify_Friendliness_To_Other(kActorClovis, kActorMcCoy, -6);
	}

#if BLADERUNNER_ORIGINAL_BUGS
#else
	if (Actor_Query_In_Set(kActorLucy, kSetKP07)) {
		Global_Variable_Decrement(kVariableReplicantsSurvivorsAtMoonbus, 1);
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyGone);

		if (Global_Variable_Query(kVariableReplicantsSurvivorsAtMoonbus) == 0) {
			Player_Loses_Control();
			Delay(2000);
			Player_Set_Combat_Mode(false);
			Loop_Actor_Walk_To_XYZ(kActorMcCoy, -12.0f, -41.58f, 72.0f, 0, true, false, false);
			Ambient_Sounds_Remove_All_Non_Looping_Sounds(true);
			Ambient_Sounds_Remove_All_Looping_Sounds(1);
			Game_Flag_Set(kFlagKP07toKP06);
			Game_Flag_Reset(kFlagMcCoyIsHelpingReplicants);
			Set_Enter(kSetKP05_KP06, kSceneKP06);

			return; //true;
		}
	}
#endif // BLADERUNNER_ORIGINAL_BUGS

	if ((byActorId == kActorSteele
	  || byActorId == kActorMcCoy
	 )
	 && Actor_Query_In_Set(kActorSteele, kSetHF06)
	 && Actor_Query_In_Set(kActorMcCoy, kSetHF06)
	) {
		Non_Player_Actor_Combat_Mode_On(kActorSteele, kActorCombatStateUncover, true, kActorMcCoy, 15, kAnimationModeCombatIdle, kAnimationModeCombatWalk, kAnimationModeCombatRun, 0, 0, 100, 25, 300, false);
	}

	if (Query_Difficulty_Level() != kGameDifficultyEasy
	 && byActorId == kActorMcCoy
	 && Game_Flag_Query(kFlagLucyIsReplicant)
	) {
		Global_Variable_Increment(kVariableChinyen, 200);
	}

	Actor_Set_Goal_Number(kActorLucy, kGoalLucyGone);
}

int AIScriptLucy::GetFriendlinessModifierIfGetsClue(int otherActorId, int clueId) {
	return 0;
}

bool AIScriptLucy::GoalChanged(int currentGoalNumber, int newGoalNumber) {
	if (newGoalNumber == kGoalLucyDefault) {
		Actor_Put_In_Set(kActorLucy, kSetFreeSlotA);
		return false;
	}

	switch (newGoalNumber) {
	case kGoalLucyMoveAround:
		if (Global_Variable_Query(kVariableHollowayArrest) == 3) {
			if (Game_Flag_Query(kFlagDektoraRanAway)
			 && Game_Flag_Query(kFlagGordoRanAway)
			 && Player_Query_Current_Scene() != kSceneHF03
			) {
				Actor_Set_Goal_Number(kActorLucy, kGoalLucyGoToHF03);
			} else {
				if (Random_Query(0, 1) == 1) {
					Actor_Set_Goal_Number(kActorLucy, kGoalLucyGoToFreeSlotGAHJ);
				} else {
					Actor_Set_Goal_Number(kActorLucy, kGoalLucyGoToFreeSlotGAG);
				}
			}
		} else {
			int rnd = Random_Query(0, 3);

			if (rnd == 0) {
				Actor_Set_Goal_Number(kActorLucy, kGoalLucyGoToFreeSlotGAG);
			} else if (rnd == 1) {
				Actor_Set_Goal_Number(kActorLucy, kGoalLucyGoToFreeSlotGAHJ);
			} else if (Player_Query_Current_Scene() == kSceneHF03) {
				Actor_Set_Goal_Number(kActorLucy, kGoalLucyGoToFreeSlotGAG);
			} else {
				Actor_Set_Goal_Number(kActorLucy, kGoalLucyGoToHF03);
			}
		}
		break;

	case kGoalLucyGoToHF03:
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append_With_Facing(kActorLucy, 371, 0, 156);
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyHF03RunOutPhase1:
		Actor_Set_Immunity_To_Obstacles(kActorLucy, true);
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append_Run(kActorLucy, 377, 0);
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyHF03RunOutPhase2:
		Actor_Set_Immunity_To_Obstacles(kActorLucy, false);
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append_Run(kActorLucy, 372, 0);
		AI_Movement_Track_Append(kActorLucy, 33, 0);
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyHF03RunToHF041:
		Actor_Set_Immunity_To_Obstacles(kActorLucy, true);
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append_Run(kActorLucy, 378, 0);
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyHF03RunToHF042:
		Actor_Set_Immunity_To_Obstacles(kActorLucy, false);
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append_Run(kActorLucy, 373, 0);
		AI_Movement_Track_Append(kActorLucy, 33, 0);
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyHF03RunAwayWithHelp1:
		Actor_Says(kActorLucy, 320, 16); // Lucy: Help! Help! He tried to touch me.
		Actor_Set_Goal_Number(kActorHolloway, kGoalHollowayGoToHF03);
		Actor_Set_Immunity_To_Obstacles(kActorLucy, true);
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append(kActorLucy, 378, 0);
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyHF03RunAwayWithHelp2:
		Actor_Set_Immunity_To_Obstacles(kActorLucy, false);
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append_Run(kActorLucy, 373, 0);
		AI_Movement_Track_Append(kActorLucy, 33, 30);
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyGoToFreeSlotGAG:
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append(kActorLucy, 39, Random_Query(5, 10));
		AI_Movement_Track_Append(kActorLucy, 33, Random_Query(5, 10));
		AI_Movement_Track_Append(kActorLucy, 39, Random_Query(5, 10));
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyGoToFreeSlotGAHJ:
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append(kActorLucy, 39, Random_Query(5, 15));
		AI_Movement_Track_Append(kActorLucy, 33, Random_Query(10, 30));
		AI_Movement_Track_Append(kActorLucy, 40, Random_Query(15, 30));
		AI_Movement_Track_Append(kActorLucy, 42, Random_Query(10, 20));
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyHF04Start:
		AI_Movement_Track_Flush(kActorLucy);
		Actor_Put_In_Set(kActorLucy, kSetHF04);
		Actor_Set_At_Waypoint(kActorLucy, 518, 0);
		Actor_Set_Targetable(kActorLucy, true);
		Actor_Set_Health(kActorLucy, 5, 5);
		break;

	case kGoalLucyHF04TalkToMcCoy:
		Player_Loses_Control();
		Actor_Says(kActorMcCoy, 1700, 16); // McCoy: I put my gun away.
		AI_Movement_Track_Flush(kActorLucy);
		Actor_Face_Actor(kActorLucy, kActorMcCoy, true);
		Actor_Face_Actor(kActorMcCoy, kActorLucy, true);
		Actor_Says(kActorLucy,   350, 13); // Lucy: I saw you with that woman who smokes.
		Actor_Says(kActorMcCoy, 1705, 13); // McCoy: I have to deal with her just to keep up appearances. I can be more effective that way.
		Actor_Says(kActorLucy,   360, 13); // Lucy: She scares me.
		Actor_Says(kActorMcCoy, 1710, 13); // McCoy: She's being paid to kill your family.

		if (Global_Variable_Query(kVariableAffectionTowards) == kAffectionTowardsLucy) { // cut feature? if this is set lucy will not run into hf04
			Actor_Says(kActorLucy,   940, 13); // Lucy: You're a good man.
			Actor_Says(kActorMcCoy, 6780, 12); // McCoy: Don't jump to any conclusions.
			Actor_Says(kActorLucy,   950, 12); // Lucy: You're gentle. Father used to be like that too.
			Actor_Says(kActorLucy,   960, 14); // Lucy: He would read to me and tell me stories. Pretty stories so I would have pretty dreams.
			Actor_Says(kActorMcCoy, 6785, 13); // McCoy: He doesn't do that anymore?
			Actor_Says(kActorLucy,   970, 16); // Lucy: He's out with Sadik every night. And he and mother...
			Actor_Says(kActorLucy,   980, 13); // Lucy: They argue about what we're going to do.
			if (Game_Flag_Query(kFlagDektoraIsReplicant)) {
				Actor_Says(kActorLucy, 990, 15); // Lucy: He's worried about her. She hasn't been well lately.
			}
			Actor_Says(kActorMcCoy, 6790, 13); // McCoy: That must be tough on you.
			if (Game_Flag_Query(kFlagDektoraIsReplicant)
			 && Game_Flag_Query(kFlagLucyIsReplicant)
			) {
				Actor_Says(kActorLucy, 1000, 12); // Lucy: I've seen death before. But the other day...
			}
			Actor_Says(kActorLucy,  1010, 15); // Lucy: I was thinking; If a person can feel love, real love
			Actor_Says(kActorLucy,  1020, 12); // Lucy: for even just one day, I think it would all be worth it.
			Actor_Says(kActorMcCoy, 6795, 18); // McCoy: I agree.
			Actor_Says(kActorLucy,  1030, 15); // Lucy: Out in a place with trees and a lake and flowers. I've seen places like that in books.
			Actor_Says(kActorMcCoy, 6800, 16); // McCoy: You'll see them with your own eyes one day.
			Actor_Says(kActorLucy,  1040, 12); // Lucy: Will I?
			Actor_Says(kActorMcCoy, 6805, 15); // McCoy: I-- I promise you. But for now we gotta be careful. You should stay hidden for a while.
			Actor_Says(kActorLucy,  1050, 12); // Lucy: Oh, thank you. Thank you for everything.
		}
		Actor_Says(kActorLucy, 370, 14); // Lucy: I'll tell them. What you did. That you helped me.
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF04WalkAway);

		if (Global_Variable_Query(kVariableHollowayArrest) == 3) {
			Actor_Set_Goal_Number(kActorSteele, kGoalSteeleHF02LucyRanAway);
			Game_Flag_Set(kFlagLucyRanAway);
		}

		Player_Gains_Control();
		break;

	case kGoalLucyHF04Run1:
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append_Run(kActorLucy, 519, 0);
		AI_Movement_Track_Repeat(kActorLucy);
		Actor_Set_Health(kActorLucy, 5, 5);
		break;

	case kGoalLucyHF04Run2:
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append_Run(kActorLucy, 520, 0);
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyHF04Run3:
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append_Run(kActorLucy, 521, 0);
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyHF04Run4:
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append_Run(kActorLucy, 522, 0);
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyHF04WalkAway:
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append_Run(kActorLucy, 523, 0);
		AI_Movement_Track_Append(kActorLucy, 33, 0);
		AI_Movement_Track_Repeat(kActorLucy);
		Player_Gains_Control();
		Actor_Set_Health(kActorLucy, 30, 30);
		break;

	case 240: // not used anywhere
		if (Global_Variable_Query(kVariableHollowayArrest) == 3) {
			Actor_Set_Goal_Number(kActorLucy, kGoalLucyGone);
			Actor_Set_Goal_Number(kActorSteele, kGoalSteeleHF02ConfrontLucy);
		} else {
			Actor_Set_Goal_Number(kActorLucy, kGoalLucyHF03RanAway);
			Game_Flag_Set(kFlagLucyRanAway);
		}
		break;

	case kGoalLucyReturnToHF03:
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append(kActorLucy, 372, 0);
		AI_Movement_Track_Append_With_Facing(kActorLucy, 371, 0, 156);
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyStartChapter4:
		Actor_Put_In_Set(kActorLucy, kSetFreeSlotA);
		Actor_Set_At_Waypoint(kActorLucy, 33, 0);
		if (Global_Variable_Query(kVariableAffectionTowards) == kAffectionTowardsLucy) {
			Actor_Set_Goal_Number(kActorLucy, kGoalLucyUG01Wait);
		}
		break;

	case kGoalLucyUG01Wait:
		Actor_Put_In_Set(kActorLucy, kSetUG01);
		Actor_Set_At_Waypoint(kActorLucy, 544, 651);
		break;

	case kGoalLucyUG01VoightKampff:
		voightKampffTest();
		break;

	case kGoalLucyUG01RunAway:
		AI_Movement_Track_Flush(kActorLucy);
		AI_Movement_Track_Append_Run(kActorLucy, 545, 0);
		AI_Movement_Track_Append(kActorLucy, 33, 0); // kSetFreeSlotA
		AI_Movement_Track_Repeat(kActorLucy);
		break;

	case kGoalLucyGone:
		Game_Flag_Set(kFlagLucyRanAway);
		break;
	}

	return false;
}

bool AIScriptLucy::UpdateAnimation(int *animation, int *frame) {
	switch (_animationState) {
	case 0:
		*animation = 260;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(260)) {
			_animationFrame = 0;
		}
		break;

	case 1:
	case 2:
	case 3:
	case 4:
		if (_animationState == 1) {
			*animation = 253;
		}
		if (_animationState == 2) {
			*animation = 254;
		}
		if (_animationState == 3) {
			*animation = 255;
		}
		if (_animationState == 4) {
			*animation = 256;
		}
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
		}
		break;

	case 5:
	case 6:
		if (_animationState == 5) {
			*animation = 257;
		}
		if (_animationState == 6) {
			*animation = 258;
		}
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			*animation = 260;
			_animationFrame = 0;
			_animationState = 0;
			Actor_Change_Animation_Mode(kActorLucy, kAnimationModeIdle);
		}
		break;

	case 7:
		*animation = 259;
		if (_animationFrame < Slice_Animation_Query_Number_Of_Frames(259) - 1) {
			++_animationFrame;
		}
		break;

	case 8:
		if (!_animationFrame && _flag) {
			*animation = 260;
			_animationState = 0;
			_flag = 0;
		} else {
			*animation = 263;
			_animationFrame++;
			if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(263)) {
				_animationFrame = 0;
			}
		}
		break;

	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
		if (_animationState == 9) {
			*animation = 264;
		}
		if (_animationState == 10) {
			*animation = 265;
		}
		if (_animationState == 11) {
			*animation = 266;
		}
		if (_animationState == 12) {
			*animation = 267;
		}
		if (_animationState == 13) {
			*animation = 268;
		}
		if (_animationState == 14) {
			*animation = 269;
		}
		if (_animationState == 15) {
			*animation = 270;
		}
		if (_animationState == 16) {
			*animation = 271;
		}
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(*animation)) {
			_animationFrame = 0;
			_animationState = 8;
			*animation = 263;
		}
		break;

	case 17:
		*animation = 272;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(272)) {
			_animationFrame = 0;
		}
		break;

	case 18:
		if (!_animationFrame && _flag) {
			_animationState = 17;
			_flag = 0;
			*animation = 272;
		} else {
			*animation = 273;
			_animationFrame++;
			if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(273)) {
				_animationFrame = 0;
			}
		}
		break;

	case 19:
		*animation = 274;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(274)) {
			*animation = 260;
			_animationFrame = 0;
			_animationState = 0;
		}
		break;

	case 20:
		*animation = 275;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(275)) {
			_animationFrame = 0;
		}
		break;

	case 21:
		*animation = 276;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(276)) {
			_animationFrame = 0;
		}
		break;

	default:
		break;
	}
	*frame = _animationFrame;

	return true;
}

bool AIScriptLucy::ChangeAnimationMode(int mode) {
	switch (mode) {
		case 0:
	if (_animationState >= 8 && _animationState <= 16) {
			_flag = 1;
		} else {
			_animationState = 0;
			_animationFrame = 0;
		}
		break;

	case 1:
		_animationState = 1;
		_animationFrame = 0;
		break;

	case 2:
		_animationState = 2;
		_animationFrame = 0;
		break;

	case 3:
		_flag = 0;
		_animationState = 8;
		_animationFrame = 0;
		break;

	case 4:
		if (_animationState >= 8 && _animationState <= 16) {
			_flag = 1;
		} else {
			_animationState = 0;
			_animationFrame = 0;
		}
		break;

	case 7:
		_animationState = 1;
		_animationFrame = 0;
		break;

	case 8:
		_animationState = 2;
		_animationFrame = 0;
		break;

	case 12:
		_flag = 0;
		_animationState = 9;
		_animationFrame = 0;
		break;

	case 13:
		_flag = 0;
		_animationState = 10;
		_animationFrame = 0;
		break;

	case 14:
		_flag = 0;
		_animationState = 11;
		_animationFrame = 0;
		break;

	case 15:
		_flag = 0;
		_animationState = 12;
		_animationFrame = 0;
		break;

	case 16:
		_flag = 0;
		_animationState = 13;
		_animationFrame = 0;
		break;

	case 17:
		_flag = 0;
		_animationState = 14;
		_animationFrame = 0;
		break;

	case 18:
		_flag = 0;
		_animationState = 15;
		_animationFrame = 0;
		break;

	case 19:
		_flag = 0;
		_animationState = 16;
		_animationFrame = 0;
		break;

	case 21:
		if (Random_Query(1, 2) == 1) {
			_animationState = 5;
		} else {
			_animationState = 6;
		}
		_animationFrame = 0;
		break;

	case 22:
		if (Random_Query(1, 2) == 1) {
			_animationState = 5;
		} else {
			_animationState = 6;
		}
		_animationFrame = 0;
		break;

	case kAnimationModeDie:
		_animationState = 7;
		_animationFrame = 0;
		break;
	}

	return true;
}

void AIScriptLucy::QueryAnimationState(int *animationState, int *animationFrame, int *animationStateNext, int *animationNext) {
	*animationState     = _animationState;
	*animationFrame     = _animationFrame;
	*animationStateNext = _animationStateNext;
	*animationNext      = _animationNext;
}

void AIScriptLucy::SetAnimationState(int animationState, int animationFrame, int animationStateNext, int animationNext) {
	_animationState     = animationState;
	_animationFrame     = animationFrame;
	_animationStateNext = animationStateNext;
	_animationNext      = animationNext;
}

bool AIScriptLucy::ReachedMovementTrackWaypoint(int waypointId) {
	return true;
}

void AIScriptLucy::FledCombat() {
	if (Global_Variable_Query(kVariableChapter) == 5
	 && Actor_Query_Goal_Number(kActorLucy) == 450
	) {
		Actor_Put_In_Set(kActorLucy, kSetFreeSlotG);
		Actor_Set_At_Waypoint(kActorLucy, 39, 0);
		Actor_Set_Goal_Number(kActorLucy, kGoalLucyGone);
	}

	return; //true;
}

void AIScriptLucy::voightKampffTest() {
	Player_Loses_Control();
	Actor_Face_Actor(kActorMcCoy, kActorLucy, true);
	Actor_Says(kActorMcCoy, 6815, 11); // McCoy: I told you to stay hidden.
	Actor_Face_Actor(kActorLucy, kActorMcCoy, true);
	Actor_Says(kActorLucy, 1060, 16); // Lucy: I can't stay there anymore.
	Actor_Says(kActorLucy, 1070, 17); // Lucy: I don't want to be with father. Not when he's so angry. Besides...
	Delay(1000);
	Actor_Says(kActorLucy,  1080, 14); // Lucy: I need something.
	Actor_Says(kActorMcCoy, 6820, 16); // McCoy: We don't have time for an--
	Actor_Says(kActorLucy,  1090, 13); // Lucy: That thing you do to see if a person is human?
	if (!Game_Flag_Query(kFlagDirectorsCut)) {
		Actor_Says(kActorMcCoy, 6825, 13); // McCoy: The Voigt Kampff?
	}
	Actor_Says(kActorMcCoy, 6830, 12); // McCoy: You want it?
	Actor_Says(kActorLucy,  1100, 14); // Lucy: Is it always right?
	Actor_Says(kActorMcCoy, 6835, 14); // McCoy: Usually.
	Actor_Says(kActorLucy,  1110, 15); // Lucy: Give it to me. Father won't tell me anything anymore.
	Actor_Says(kActorMcCoy, 6840, 13); // McCoy: Is that why he was angry. Because you asked him--
	Delay(1000);
	Actor_Says(kActorMcCoy, 6845, 12); // McCoy: You deserve it.
	Delay(500);
	Actor_Says(kActorMcCoy, 6850, 12); // McCoy: You know how this works? I set up situations and you respond.
	Actor_Says(kActorLucy,  1120, 14); // Lucy: Okay. Is it hard? Like a test in school?
	Actor_Says(kActorMcCoy, 6855, 13); // McCoy: No. Sometimes it can be disturbing.
	Actor_Says(kActorMcCoy, 6860, 13); // McCoy: Are you ready?
	Actor_Says(kActorLucy,  1130, 14); // Lucy: Uh-huh.
	Music_Stop(2);
	Player_Gains_Control();

	Voight_Kampff_Activate(kActorLucy, 40);

	Player_Loses_Control();
	if (Actor_Clue_Query(kActorMcCoy, kClueVKLucyReplicant)) {
		Actor_Says(kActorMcCoy, 6865, 13); // McCoy: You're a Replicant.
		Actor_Says(kActorLucy,  1140, 14); // Lucy: Tell me.
		Actor_Says(kActorMcCoy, 6865, 14); // McCoy: You're a Replicant.
		Actor_Says(kActorLucy,  1150, 16); // Lucy: Then father was right. I shouldn't have taken the test.
		Actor_Says(kActorMcCoy, 6870, 14); // McCoy: No. But you had to know.
		Delay(500);
		Actor_Says(kActorMcCoy, 6875, 13); // McCoy: I gotta go.
		Actor_Says(kActorLucy,  1160, 16); // Lucy: I wanna stay with you.
	} else {
		Actor_Says(kActorMcCoy, 6880, 13); // McCoy: The test says you're human.
		Actor_Says(kActorLucy,  1170, 13); // Lucy: Really?
		Actor_Says(kActorLucy,  1180, 16); // Lucy: Why didn't father tell me?
		Actor_Says(kActorMcCoy, 6890, 15); // McCoy: Maybe he wanted to keep his... family together.
		Actor_Says(kActorLucy,  1190, 15); // Lucy: I'm not a little girl anymore.
		Actor_Says(kActorLucy,  1200, 17); // Lucy: It's not fair!
		Actor_Says(kActorMcCoy, 6885, 13); // McCoy: No. But it's understandable.
		Actor_Says(kActorLucy,  1210, 17); // Lucy: I'm gonna stay with you.
	}
	Actor_Says(kActorMcCoy, 6895, 15); // McCoy: Don't you understand? It's not safe.
	Actor_Says(kActorMcCoy, 6900, 11); // McCoy: There's a hunter out here. If she finds out where you are--
	Actor_Says(kActorLucy,  1220, 16); // Lucy: Can't you come back with me?
	Actor_Says(kActorMcCoy, 6905, 13); // McCoy: Now is not a good time.
	Actor_Says(kActorLucy,  1230, 17); // Lucy: It's never a good time!
	Actor_Says(kActorMcCoy, 6910, 13); // McCoy: Listen! You have to be patient, you have to trust me. It's not safe here.
	Delay(2000);
	Player_Gains_Control();
	Actor_Set_Goal_Number(kActorLucy, kGoalLucyUG01RunAway);
}

void AIScriptLucy::checkCombat() {
	Game_Flag_Set(kFlagMcCoyAttackedLucy);

	if (Actor_Query_In_Set(kActorLucy, kSetHF01)
	 && Global_Variable_Query(kVariableChapter) == 5
	 && Actor_Query_Goal_Number(kActorLucy) != 450
	) {
		if (Global_Variable_Query(kVariableAffectionTowards) == kAffectionTowardsLucy) {
			Global_Variable_Set(kVariableAffectionTowards, kAffectionTowardsNone);
		}
		Actor_Set_Goal_Number(kActorLucy, 450);
		Non_Player_Actor_Combat_Mode_On(kActorLucy, kActorCombatStateIdle, false, kActorMcCoy, 4, kAnimationModeIdle, kAnimationModeWalk, kAnimationModeRun, -1, 0, 0, 10, 300, false);
	}
}

} // End of namespace BladeRunner
