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

AIScriptSebastian::AIScriptSebastian(BladeRunnerEngine *vm) : AIScriptBase(vm) {
	_flag = false;
}

void AIScriptSebastian::Initialize() {
	_animationFrame = 0;
	_animationState = 0;
	_animationStateNext = 0;
	_animationNext = 0;

	_flag = false;

	Actor_Set_Goal_Number(kActorSebastian, 0);
}

bool AIScriptSebastian::Update() {
	if (Actor_Query_Goal_Number(kActorSebastian) < 200
	 && Global_Variable_Query(kVariableChapter) == 3
	) {
		Actor_Set_Goal_Number(kActorSebastian, 200);
	}

	return false;
}

void AIScriptSebastian::TimerExpired(int timer) {
	//return false;
}

void AIScriptSebastian::CompletedMovementTrack() {
	//return false;
}

void AIScriptSebastian::ReceivedClue(int clueId, int fromActorId) {
	//return false;
}

void AIScriptSebastian::ClickedByPlayer() {
	if (Actor_Query_Goal_Number(kActorSebastian) == 205) {
		AI_Movement_Track_Pause(kActorSebastian);
		Actor_Face_Actor(kActorSebastian, kActorMcCoy, true);
		Actor_Face_Actor(kActorMcCoy, kActorSebastian, true);
		if (Actor_Clue_Query(kActorSebastian, kClueMcCoyIsABladeRunner)) {
			Actor_Says(kActorMcCoy,     6985, 16); // McCoy: Got the straight scoop for me or what?
			Actor_Says(kActorSebastian,  610, 14); // Sebastian: I think you should leave now, Mr. McCoy.
		} else {
			dialogue();
		}

		AI_Movement_Track_Unpause(kActorSebastian);
		return; //true;
	}
	return; //false;
}

void AIScriptSebastian::EnteredScene(int sceneId) {
	// return false;
}

void AIScriptSebastian::OtherAgentEnteredThisScene(int otherActorId) {
	// return false;
}

void AIScriptSebastian::OtherAgentExitedThisScene(int otherActorId) {
	// return false;
}

void AIScriptSebastian::OtherAgentEnteredCombatMode(int otherActorId, int combatMode) {
	if (otherActorId == kActorMcCoy
	 && combatMode
	) {
		Global_Variable_Increment(kVariableGunPulledInFrontOfSebastian, 1);
		Actor_Modify_Friendliness_To_Other(kActorSebastian, kActorMcCoy, -5);
		AI_Movement_Track_Pause(kActorSebastian);
		Actor_Face_Actor(kActorSebastian, kActorMcCoy, true);

		if (Global_Variable_Query(kVariableGunPulledInFrontOfSebastian) == 1) {
			Actor_Says(kActorSebastian, 680, 12); // Sebastian: Hey, you don't need to do that.
			Actor_Face_Actor(kActorMcCoy, kActorSebastian, true);
			Actor_Says_With_Pause(kActorMcCoy, 7265, 0.0f, kAnimationModeCombatIdle);
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeCombatIdle);
			Delay(500);
			Actor_Says(kActorSebastian, 690, 16); // Sebastian: There's nothing in here. People don't even like to come inside my building.
		} else {
			Actor_Says(kActorSebastian, 700, 15); // Sebastian: Please! You don't have to pull your gun in here.
			Actor_Says_With_Pause(kActorMcCoy, 7270, 0.0f, kAnimationModeCombatIdle);
			Actor_Change_Animation_Mode(kActorMcCoy, kAnimationModeCombatIdle);
		}
		AI_Movement_Track_Unpause(kActorSebastian);

		return; //true;
	}
}

void AIScriptSebastian::ShotAtAndMissed() {
	// return false;
}

bool AIScriptSebastian::ShotAtAndHit() {
	return false;
}

void AIScriptSebastian::Retired(int byActorId) {
	// return false;
}

int AIScriptSebastian::GetFriendlinessModifierIfGetsClue(int otherActorId, int clueId) {
	return 0;
}

bool AIScriptSebastian::GoalChanged(int currentGoalNumber, int newGoalNumber) {
	if (newGoalNumber == 200) {
		Actor_Put_In_Set(kActorSebastian, kSetBB05);
		Actor_Set_At_XYZ(kActorSebastian, -13.08f, -60.31f, 100.88f, 470);
	}

	return false;
}

bool AIScriptSebastian::UpdateAnimation(int *animation, int *frame) {
	switch (_animationState) {
	case 0:
		*animation = 811;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(811) - 1) {
			_animationFrame = 0;
		}
		break;

	case 1:
		*animation = 809;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(809) - 1) {
			_animationFrame = 0;
		}
		break;

	case 2:
		*animation = 810;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(810) - 1) {
			Actor_Change_Animation_Mode(kActorSebastian, kAnimationModeIdle);
			*animation = 811;
			_animationFrame = 0;
			_animationState = 0;
		}
		break;

	case 3:
		*animation = 821;
		if (_animationFrame < Slice_Animation_Query_Number_Of_Frames(821) - 1) {
			_animationFrame++;
		}
		break;

	case 4:
		if (!_animationFrame && _flag) {
			Actor_Change_Animation_Mode(kActorSebastian, kAnimationModeIdle);
			*animation = 811;
			_animationState = 0;
			_animationFrame = 0;
		} else {
			*animation = 813;
			_animationFrame++;
			if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(813) - 1) {
				_animationFrame = 0;
			}
		}
		break;

	case 5:
		*animation = 814;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(814) - 1) {
			_animationFrame = 0;
			_animationState = 4;
			*animation = 813;
		}
		break;

	case 6:
		*animation = 815;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(815) - 1) {
			_animationFrame = 0;
			_animationState = 4;
			*animation = 813;
		}
		break;

	case 7:
		*animation = 816;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(816) - 1) {
			_animationFrame = 0;
			_animationState = 4;
			*animation = 813;
		}
		break;

	case 8:
		*animation = 817;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(817) - 1) {
			_animationFrame = 0;
			_animationState = 4;
			*animation = 813;
		}
		break;

	case 9:
		*animation = 818;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(818) - 1) {
			_animationFrame = 0;
			_animationState = 4;
			*animation = 813;
		}
		break;

	case 10:
		*animation = 819;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(819) - 1) {
			_animationFrame = 0;
			_animationState = 4;
			*animation = 813;
		}
		break;

	case 11:
		*animation = 820;
		_animationFrame++;
		if (_animationFrame > Slice_Animation_Query_Number_Of_Frames(820) - 1) {
			_animationFrame = 0;
			_animationState = 4;
			*animation = 813;
		}
		break;

	default:
		break;
	}
	*frame = _animationFrame;

	return true;
}

bool AIScriptSebastian::ChangeAnimationMode(int mode) {
	switch (mode) {
	case kAnimationModeIdle:
		if (_animationState > 11) {
			_animationState = 0;
			_animationFrame = 0;
		} else {
			_flag = true;
		}
		break;

	case kAnimationModeWalk:
		_animationState = 1;
		_animationFrame = 0;
		break;

	case kAnimationModeTalk:
		_animationState = 4;
		_animationFrame = 0;
		_flag = 0;
		break;

	case 12:
		_animationState = 5;
		_animationFrame = 0;
		_flag = 0;
		break;

	case 13:
		_animationState = 6;
		_animationFrame = 0;
		_flag = 0;
		break;

	case 14:
		_animationState = 7;
		_animationFrame = 0;
		_flag = 0;
		break;

	case 15:
		_animationState = 8;
		_animationFrame = 0;
		_flag = 0;
		break;

	case 16:
		_animationState = 9;
		_animationFrame = 0;
		_flag = 0;
		break;

	case 17:
		_animationState = 10;
		_animationFrame = 0;
		_flag = 0;
		break;

	case 18:
		_animationState = 11;
		_animationFrame = 0;
		_flag = 0;
		break;

	case 20:
		_animationState = 2;
		_animationFrame = 0;
		break;

	case kAnimationModeDie:
		_animationState = 3;
		_animationFrame = 0;
		break;
	}

	return true;
}

void AIScriptSebastian::QueryAnimationState(int *animationState, int *animationFrame, int *animationStateNext, int *animationNext) {
	*animationState     = _animationState;
	*animationFrame     = _animationFrame;
	*animationStateNext = _animationStateNext;
	*animationNext      = _animationNext;
}

void AIScriptSebastian::SetAnimationState(int animationState, int animationFrame, int animationStateNext, int animationNext) {
	_animationState     = animationState;
	_animationFrame     = animationFrame;
	_animationStateNext = animationStateNext;
	_animationNext      = animationNext;
}

bool AIScriptSebastian::ReachedMovementTrackWaypoint(int waypointId) {
	return true;
}

void AIScriptSebastian::FledCombat() {
	// return false;
}

void AIScriptSebastian::dialogue() {
	Dialogue_Menu_Clear_List();

	if (Actor_Query_Friendliness_To_Other(kActorSebastian, kActorMcCoy) >= 45) {
		DM_Add_To_List_Never_Repeat_Once_Selected(930, 5, 5, 5); // MORAJI AND CHEW
		DM_Add_To_List_Never_Repeat_Once_Selected(940, -1, 5, 6); // EISENDULLER
		DM_Add_To_List_Never_Repeat_Once_Selected(950, 5, 5, 5); // TYRELL
	}

	if (Actor_Clue_Query(kActorMcCoy, kClueAnsweringMachineMessage)) {
		DM_Add_To_List_Never_Repeat_Once_Selected(960, 3, -1, 5); // TWINS
	}

	if (Actor_Clue_Query(kActorMcCoy, kClueAnsweringMachineMessage)
	 && Actor_Clue_Query(kActorMcCoy, kClueEnvelope)
	) {
		DM_Add_To_List_Never_Repeat_Once_Selected(970, -1, 4, -1); // RUNCITER
	}

	DM_Add_To_List_Never_Repeat_Once_Selected(980, -1, -1, 7); // ROBBERS
	DM_Add_To_List_Never_Repeat_Once_Selected(990, 7, 3, -1); // NEXUS-6

	if (Dialogue_Menu_Query_List_Size()) {
		// This condition clause for non-empty dialogue menu options before adding the DONE option
		// only occurs in Sebastian's AI script.
		// Probably because, selecting "DONE" here, McCoy has nothing to say
		// so there's no point to add it as a "auto-selected" last option
		// if no other options exist in the list
		Dialogue_Menu_Add_DONE_To_List(1000); // DONE
		Dialogue_Menu_Appear(320, 240);
		int answer = Dialogue_Menu_Query_Input();
		Dialogue_Menu_Disappear();

		switch (answer) {
		case 930: // MORAJI AND CHEW
			Actor_Says(kActorMcCoy,     7075, 13); // McCoy: You know the design sub-cons down on the Row? Moraji and Chew?
			Actor_Says(kActorSebastian,  290, 12); // Sebastian: I've heard of them, but I've never seen them.
			Actor_Says(kActorSebastian,  300, 13); // Sebastian: I don't go in for the parties and stuff, and I never go down to the Row.
			break;

		case 940: // EISENDULLER
			Actor_Says(kActorMcCoy,     7080,                 15); // McCoy: You ever work with a Dr. Eisenduller?
			Actor_Says(kActorSebastian,  310,                 13); // Sebastian: We're in different fields.
			Actor_Says(kActorSebastian,  320,                 16); // Sebastian: I'm in basic bio-genetic design and he works on the practical application of Off-World physics.
			Actor_Says(kActorSebastian,  340,                 12); // Sebastian: He seems to be a very nice man.
			Actor_Says(kActorMcCoy,     7120,                 14); // McCoy: Who happens to be dead.
			Actor_Says(kActorSebastian,  350, kAnimationModeTalk); // Sebastian: Dead?
			Actor_Says(kActorMcCoy,     7125,                 13); // McCoy: He was murdered. Probably by a Replicant.
			Actor_Says(kActorSebastian,  360,                 17); // Sebastian: But Replicants aren't allowed on Terra.
			Actor_Says_With_Pause(kActorMcCoy, 7130, 1.0f, kAnimationModeTalk);
			Actor_Says(kActorMcCoy, 7135, 18); // McCoy: And who do you think they'll start looking for next?
			break;

		case 950: // TYRELL
			Actor_Says(kActorMcCoy, 7085, 15); // McCoy: How well do you know Dr. Tyrell?
			Actor_Says_With_Pause(kActorSebastian, 370, 0.30f, 13);
			Actor_Says_With_Pause(kActorSebastian, 380, 0.70f, 17);
			Actor_Says(kActorSebastian, 390, 14); // Sebastian: Dr. Tyrell and I play chess every week. I've only beaten him once.
			if (Actor_Clue_Query(kActorMcCoy, kClueChessTable)) {
				Actor_Says(kActorMcCoy,     7140, kAnimationModeTalk); // McCoy: You're in the middle of a game right now?
				Actor_Says(kActorSebastian,  400,                 12); // Sebastian: Uh-huh. Do you play?
				Actor_Says(kActorMcCoy,     7145,                 16); // McCoy: I don't have the patience for chess.
				Actor_Says(kActorSebastian,  410,                 13); // Sebastian: You play with Dr. Tyrell, you learn something new every time.
				Actor_Says(kActorMcCoy,     7150,                 17); // McCoy: Such as?
				Actor_Says(kActorSebastian,  420,                 13); // Sebastian: Every single move must be taken very seriously.
				Actor_Says(kActorSebastian,  430,                 14); // Sebastian: Otherwise, you've lost before you've even begun.
			}
			break;

		case 960: // TWINS
			Actor_Says(kActorMcCoy,     7090, 17); // McCoy: Talk to me about the twins.
			Actor_Says(kActorSebastian,  440, 14); // Sebastian: You mean Luther and Lance? They were Dr. Tyrell's favorites until he let them go,
			Actor_Says(kActorSebastian,  450, 13); // Sebastian: Nobody really knows why either.
			Actor_Says(kActorMcCoy,     7155, 13); // McCoy: You think they'd hold a grudge against Tyrell?
			Actor_Says(kActorSebastian,  460, 17); // Sebastian: Maybe. But they're decent fellows Mr. McCoy.
			Actor_Says(kActorSebastian,  470, 12); // Sebastian: I don't think they'd ever do a mean thing to anybody.
			Actor_Says(kActorSebastian,  480, 13); // Sebastian: They're real smart, too. Almost as smart as Dr. Tyrell.
			Actor_Says(kActorMcCoy,     7160, 18); // McCoy: Maybe they're too smart to keep around. Too much competition for the old man.
			Actor_Says(kActorSebastian,  490, 14); // Sebastian: Oh, they could never take Dr. Tyrell's place.
			Actor_Says(kActorMcCoy,     7165, 14); // McCoy: How about this. Do you think they're the type who might help out renegade Replicants?
			setMcCoyIsABladeRunner();
			break;

		case 970: // RUNCITER
			Actor_Says(kActorMcCoy,     7095, 13); // McCoy: What do you think the deal was with Runciter and the twins?
			Actor_Says(kActorSebastian,  500, 15); // Sebastian: Deal? I don't think I understand.
			Actor_Says(kActorMcCoy,     7170, 17); // McCoy: Runciter was paying the twins for something. I saw the cash hidden inside a statue.
			Actor_Says(kActorSebastian,  510, 12); // Sebastian: How would I know?
			Actor_Says(kActorMcCoy,     7175, 18); // McCoy: Come on, Sebastian, I heard you on the machine. You guys are buddies, right?
			Actor_Says(kActorSebastian,  520, 14); // Sebastian: There were rumors that the twins were selling some of the animal prototypes to him.
			Actor_Says(kActorMcCoy,     7180, 12); // McCoy: Do you think that had something to do with their being fired?
			Actor_Says(kActorSebastian,  530, 13); // Sebastian: Dr. Tyrell doesn't like anyone going around behind his back.
			Actor_Says(kActorMcCoy,     7185, 12); // McCoy: So, Tyrell would really be pissed, if he knew Luther and Lance were helping renegade Replicants.
			setMcCoyIsABladeRunner();
			break;

		case 980: // ROBBERS
			Actor_Says(kActorMcCoy,     7100, 12); // McCoy: You're sure you don't have much valuable stuff around here?
			Actor_Says(kActorSebastian,  540, 16); // Sebastian: Anything worthwhile I probably would have sold a long time ago. All this is personal stuff. Wouldn't mean a whole lot to anyone but me.
			Actor_Says(kActorMcCoy,     7195, 18); // McCoy: Maybe they were looking for you.
			Actor_Says(kActorSebastian,  720, 12); // Sebastian: Oh, no. I'm nobody.
			break;

		case 990: // NEXUS-6
			Actor_Says(kActorMcCoy, 7105, 18); // McCoy: How different are the Sixes from the other Nexus models?
			setMcCoyIsABladeRunner();
			break;

		default:
			break;
		}
	} else if (Actor_Query_Friendliness_To_Other(kActorSebastian, kActorMcCoy) >= 45) {
		Actor_Says(kActorMcCoy,     7115, 13); // McCoy: You're sure there isn't anything missing?
		Actor_Says(kActorSebastian,  280, 14); // Sebastian: Yes. I mean... No, sir. All my things are here.
	} else {
		Actor_Says(kActorMcCoy,     7110, 15); // McCoy: Maybe you can clear something up for me...
		Actor_Says(kActorSebastian,  270, 16); // Sebastian: You saw for yourself nothing's here. I wish you'd leave me alone.
	}
}

void AIScriptSebastian::setMcCoyIsABladeRunner() {
	Actor_Clue_Acquire(kActorSebastian, kClueMcCoyIsABladeRunner, true, kActorMcCoy);
	Actor_Modify_Friendliness_To_Other(kActorSebastian, kActorMcCoy, -5);
	Actor_Says(kActorSebastian,  560, 15); // Sebastian: You-- You're a Blade Runner, aren't you?
	Actor_Says(kActorMcCoy,     7200, 14); // McCoy: Bingo.
	Actor_Says(kActorSebastian,  570, 16); // Sebastian: But you said you were here about the burglary.
	Actor_Says(kActorMcCoy,     7205, 17); // McCoy: The guys who broke in might be Replicants.
	Actor_Says(kActorSebastian,  580, 13); // Sebastian: What in the world would a Replicant want from me?
	Actor_Says_With_Pause(kActorMcCoy, 7210, 1.0f, 16);
	Actor_Says(kActorSebastian,  590, 12); // Sebastian: Just a little bit.
	Actor_Says(kActorMcCoy,     7215, 19); // McCoy: Would you say that the Sixes are smarter than the old Fours and Fives?
	Actor_Says(kActorSebastian,  600, 14); // Sebastian: They're supposed to be. Stronger and more agile, too.
	Actor_Says(kActorMcCoy,     7220, 13); // McCoy: So, maybe they want to learn more about themselves.
	Actor_Says_With_Pause(kActorMcCoy, 7225, 0.80f, 14);
	Actor_Says(kActorSebastian, 610, 15); // Sebastian: I think you should leave now, Mr. McCoy.
}

} // End of namespace BladeRunner
