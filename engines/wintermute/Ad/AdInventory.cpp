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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * This file is based on WME Lite.
 * http://dead-code.org/redir.php?target=wmelite
 * Copyright (c) 2011 Jan Nedoma
 */

#include "engines/wintermute/Ad/AdInventory.h"
#include "engines/wintermute/Ad/AdGame.h"
#include "engines/wintermute/Ad/AdItem.h"
#include "engines/wintermute/PlatformSDL.h"
#include "common/str.h"

namespace WinterMute {

IMPLEMENT_PERSISTENT(CAdInventory, false)

//////////////////////////////////////////////////////////////////////////
CAdInventory::CAdInventory(CBGame *inGame): CBObject(inGame) {
	_scrollOffset = 0;
}


//////////////////////////////////////////////////////////////////////////
CAdInventory::~CAdInventory() {
	_takenItems.RemoveAll(); // ref only
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdInventory::insertItem(const char *name, const char *insertAfter) {
	if (name == NULL) return E_FAIL;

	CAdItem *item = ((CAdGame *)Game)->getItemByName(name);
	if (item == NULL) return E_FAIL;

	int insertIndex = -1;
	for (int i = 0; i < _takenItems.GetSize(); i++) {
		if (scumm_stricmp(_takenItems[i]->_name, name) == 0) {
			_takenItems.RemoveAt(i);
			i--;
			continue;
		}
		if (insertAfter && scumm_stricmp(_takenItems[i]->_name, insertAfter) == 0) insertIndex = i + 1;
	}


	if (insertIndex == -1) _takenItems.Add(item);
	else _takenItems.InsertAt(insertIndex, item);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdInventory::removeItem(const char *name) {
	if (name == NULL) return E_FAIL;

	for (int i = 0; i < _takenItems.GetSize(); i++) {
		if (scumm_stricmp(_takenItems[i]->_name, name) == 0) {
			if (((CAdGame *)Game)->_selectedItem == _takenItems[i])((CAdGame *)Game)->_selectedItem = NULL;
			_takenItems.RemoveAt(i);
			return S_OK;
		}
	}

	return E_FAIL;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CAdInventory::removeItem(CAdItem *item) {
	if (item == NULL) return E_FAIL;

	for (int i = 0; i < _takenItems.GetSize(); i++) {
		if (_takenItems[i] == item) {
			if (((CAdGame *)Game)->_selectedItem == _takenItems[i])((CAdGame *)Game)->_selectedItem = NULL;
			_takenItems.RemoveAt(i);
			return S_OK;
		}
	}

	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdInventory::persist(CBPersistMgr *persistMgr) {

	CBObject::persist(persistMgr);

	_takenItems.persist(persistMgr);
	persistMgr->transfer(TMEMBER(_scrollOffset));

	return S_OK;
}

} // end of namespace WinterMute
