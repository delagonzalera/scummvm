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

#ifndef TITANIC_ARM_H
#define TITANIC_ARM_H

#include "titanic/carry/carry.h"
#include "titanic/messages/messages.h"
#include "titanic/messages/mouse_messages.h"
#include "titanic/messages/pet_messages.h"

namespace Titanic {

class CArm : public CCarry {
	DECLARE_MESSAGE_MAP;
	bool PuzzleSolvedMsg(CPuzzleSolvedMsg *msg);
	bool TranslateObjectMsg(CTranslateObjectMsg *msg);
	bool UseWithOtherMsg(CUseWithOtherMsg *msg);
	bool MouseDragStartMsg(CMouseDragStartMsg *msg);
	bool MaitreDHappyMsg(CMaitreDHappyMsg *msg);
	bool PETGainedObjectMsg(CPETGainedObjectMsg *msg);
	bool MouseDragMoveMsg(CMouseDragMoveMsg *msg);
private:
	CString _string6;
	int _field138;
	Rect _hookedRect;
	CString _string7;
	int _field158;
	Rect _armRect;
	int _field16C;
	int _field170;
public:
	CLASSDEF;
	CArm();

	/**
	 * Save the data for the class to file
	 */
	virtual void save(SimpleFile *file, int indent);

	/**
	 * Load the data for the class from file
	 */
	virtual void load(SimpleFile *file);
};

} // End of namespace Titanic

#endif /* TITANIC_ARM_H */
