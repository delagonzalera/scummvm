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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "tsage/blue_force/blueforce_ui.h"
#include "tsage/blue_force/blueforce_dialogs.h"
#include "tsage/blue_force/blueforce_logic.h"
#include "tsage/tsage.h"
#include "tsage/core.h"

namespace TsAGE {

namespace BlueForce {

void StripProxy::process(Event &event) {
	if (_action)
		_action->process(event);
}

/*--------------------------------------------------------------------------*/

void UIElement::synchronize(Serializer &s) {
	AltSceneObject::synchronize(s);
	s.syncAsSint16LE(_field88);
	s.syncAsSint16LE(_enabled);
	s.syncAsSint16LE(_frameNum);
}

void UIElement::setup(int visage, int stripNum, int frameNum, int posX, int posY, int priority) {
	_field88 = 0;
	_frameNum = frameNum;
	_enabled = true;

	SceneObject::setup(visage, stripNum, frameNum, posX, posY, priority);
}

void UIElement::setEnabled(bool flag) {
	if (_enabled != flag) {
		_enabled = flag;
		setFrame(_enabled ? _frameNum : _frameNum + 2);
	}
}

/*--------------------------------------------------------------------------*/

void UIQuestion::process(Event &event) {
	if (event.eventType == EVENT_BUTTON_DOWN) {
		int currentCursor = GLOBALS._events.getCursor();
		GLOBALS._events.hideCursor();
		showDescription(currentCursor);

		event.handled = true;
	}
}

void UIQuestion::showDescription(int lineNum) {
	if (lineNum == 8) {
		// Unknown object description
	} else {
		// Display object description
		SceneItem::display2(9001, lineNum);
	}
}

void UIQuestion::setEnabled(bool flag) {
	if (_enabled != flag) {
		UIElement::setEnabled(flag);
		BF_GLOBALS._uiElements.draw();
	}
}

/*--------------------------------------------------------------------------*/

void UIScore::postInit(SceneObjectList *OwnerList) {
	int xp = 266;
	_digit3.setup(1, 6, 1, xp, 180, 255);
	_digit3.reposition();
	xp += 7;
	_digit2.setup(1, 6, 1, xp, 180, 255);
	_digit2.reposition();
	xp += 7;
	_digit1.setup(1, 6, 1, xp, 180, 255);
	_digit1.reposition();
	xp += 7;
	_digit0.setup(1, 6, 1, xp, 180, 255);
	_digit0.reposition();
}

void UIScore::draw() {
	_digit3.draw();
	_digit2.draw();
	_digit1.draw();
	_digit0.draw();
}

void UIScore::updateScore() {
	int score = BF_GLOBALS._uiElements._scoreValue;
	
	_digit3.setFrame(score / 1000 + 1); score %= 1000;
	_digit2.setFrame(score / 100 + 1); score %= 100;
	_digit1.setFrame(score / 10 + 1); score %= 10;
	_digit0.setFrame(score + 1);
}

/*--------------------------------------------------------------------------*/

UIInventorySlot::UIInventorySlot(): UIElement() {
	_objIndex = 0;
	_object = NULL;
}

void UIInventorySlot::synchronize(Serializer &s) {
	UIElement::synchronize(s);
	s.syncAsSint16LE(_objIndex);
	SYNC_POINTER(_object);
}

void UIInventorySlot::process(Event &event) {
	if (event.eventType == EVENT_BUTTON_DOWN) {
		event.handled = true;

		if (_objIndex == INV_AMMO_BELT) {
			// Handle showing ammo belt
			showAmmoBelt();

		} else if (_objIndex != INV_NONE) {
			_object->setCursor();			
		}
	}
}

void UIInventorySlot::showAmmoBelt() {
	AmmoBeltDialog *dlg = new AmmoBeltDialog();
	dlg->execute();
	delete dlg;
}

/*--------------------------------------------------------------------------*/

UIInventoryScroll::UIInventoryScroll() {
	_isLeft = false;
}

void UIInventoryScroll::synchronize(Serializer &s) {
	UIElement::synchronize(s);
	s.syncAsSint16LE(_isLeft);
}

void UIInventoryScroll::process(Event &event) {
	if (event.eventType == EVENT_BUTTON_DOWN) {
		warning("TODO: UIInventoryScroll::process");
		event.handled = true;
	}
}

/*--------------------------------------------------------------------------*/

UICollection::UICollection(): EventHandler() {
	_clearScreen = false;
	_visible = false;
	_cursorChanged = false;
}

void UICollection::setup(const Common::Point &pt) {
	_position = pt;
	_bounds.left = _bounds.right = pt.x;
	_bounds.top = _bounds.bottom = pt.y;
}

void UICollection::hide() {
	erase();
	_visible = false;
}

void UICollection::show() {
	_visible = true;
	draw();
}

void UICollection::erase() {
	if (_clearScreen) {
		Rect tempRect(0, BF_INTERFACE_Y, SCREEN_WIDTH, SCREEN_HEIGHT);
		GLOBALS._screenSurface.fillRect(tempRect, 0);
		_clearScreen = false;
	}
}

void UICollection::resetClear() {
	_clearScreen = false;
}

void UICollection::draw() {
	if (_visible) {
		// Draw the elements
		for (uint idx = 0; idx < _objList.size(); ++idx)
			_objList[idx]->draw();

		_clearScreen = 1;
	}
}

/*--------------------------------------------------------------------------*/

UIElements::UIElements(): UICollection() {
	_cursorVisage.setVisage(1, 5);
}

void UIElements::process(Event &event) {
	if (_clearScreen && BF_GLOBALS._player._enabled && (BF_GLOBALS._sceneManager._sceneNumber != 50)) {
		if (_bounds.contains(event.mousePos)) {
			// Cursor inside UI area
			if (!_cursorChanged) {
				if (BF_GLOBALS._events.isInventoryIcon()) {
					// Inventory icon being displayed, so leave alone
				} else {
					// Change to the inventory use cursor
					GfxSurface surface = _cursorVisage.getFrame(6);
					BF_GLOBALS._events.setCursor(surface);
				}
				_cursorChanged = true;
			}

			// Pass event to any element that the cursor falls on
			for (int idx = (int)_objList.size() - 1; idx >= 0; --idx) {
				if (_objList[idx]->_bounds.contains(event.mousePos) && _objList[idx]->_enabled) {
					_objList[idx]->process(event);
					if (event.handled)
						break;
				}
			}

			if (event.eventType == EVENT_BUTTON_DOWN)
				event.handled = true;

		} else if (_cursorChanged) {
			// Cursor outside UI area, so reset as necessary
			BF_GLOBALS._events.setCursor(BF_GLOBALS._events.getCursor());
			_cursorChanged = false;

			SceneExt *scene = (SceneExt *)BF_GLOBALS._sceneManager._scene;
			if (scene->_eventHandler) {
				GfxSurface surface = _cursorVisage.getFrame(7);
				BF_GLOBALS._events.setCursor(surface);
			}
		}
	}
}

void UIElements::setup(const Common::Point &pt) {
	_slotStart = 0;
	_itemList.clear();
	_scoreValue = 0;
	_active = true;
	UICollection::setup(pt);
	hide();

	_object1.setup(1, 3, 1, 0, 0, 255);
	add(&_object1);

	// Set up the inventory slots
	int xp = 0;
	for (int idx = 0; idx < 4; ++idx) {
		UIElement *item = NULL;
		switch (idx) {
		case 0:
			item = &_slot1;
			break;
		case 1:
			item = &_slot2;
			break;
		case 2:
			item = &_slot3;
			break;
		case 3:
			item = &_slot4;
			break;
		}

		xp = idx * 63 + 2;
		item->setup(9, 1, idx, xp, 4, 255);
		add(item);
	}

	// Setup bottom-right hand buttons
	xp += 62;
	_question.setup(1, 4, 7, xp, 16, 255);
	_question.setEnabled(false);
	add(&_question);

	xp += 21;
	_scrollLeft.setup(1, 4, 1, xp, 16, 255);
	add(&_scrollLeft);
	_scrollLeft._isLeft = true;

	xp += 22;
	_scrollRight.setup(1, 4, 4, xp, 16, 255);
	add(&_scrollRight);
	_scrollRight._isLeft = false;

	// Set up the score
	_score.postInit();
	add(&_score);

	// Set interface area
	_bounds = Rect(0, BF_INTERFACE_Y - 1, SCREEN_WIDTH, SCREEN_HEIGHT);

	updateInventory();
}

void UIElements::add(UIElement *obj) {
	// Add object 
	assert(_objList.size() < 12);
	_objList.push_back(obj);

	obj->setPosition(Common::Point(_bounds.left + obj->_position.x, _bounds.top + obj->_position.y));
	obj->reposition();

	GfxSurface s = obj->getFrame();
	s.draw(obj->_position);
}

/**
 * Handles updating the visual inventory in the user interface
 */
void UIElements::updateInventory() {
	_score.updateScore();
	updateInvList();	

	// Enable scroll buttons if the player has more than four items
	if (_itemCount > 4) {
		_scrollLeft.setEnabled(true);
		_scrollRight.setEnabled(true);
	} else {
		_scrollLeft.setEnabled(false);
		_scrollRight.setEnabled(false);
	}

	// Handle cropping the slots start within inventory
	int last  = (_itemList.size() - 1) / 4 + 1;
	if (_slotStart < 0)
		_slotStart = last - 1;
	else if (_slotStart > (last - 1))
		_slotStart = 0;

	// Handle refreshing slot graphics
	UIInventorySlot *slotList[4] = { &_slot1, &_slot2, &_slot3, &_slot4 };

	// Loop through the inventory objects
	SynchronizedList<InvObject *>::iterator i;
	int objIndex = 0;
	for (i = BF_INVENTORY._itemList.begin(); i != BF_INVENTORY._itemList.end(); ++i, ++objIndex) {
		InvObject *obj = *i;

		// Check whether the object is in any of the four inventory slots
		for (int slotIndex = 0; slotIndex < 4; ++slotIndex) {
			int idx = _slotStart + slotIndex;
			int objectIdx = (idx < (int)_itemList.size()) ? _itemList[idx] : 0;

			if (objectIdx == objIndex) {
				UIInventorySlot *slot = slotList[slotIndex];

				slot->_objIndex = objIndex;
				slot->_object = obj;
				slot->setVisage(obj->_visage);
				slot->setStrip(obj->_strip);
				slot->setFrame(obj->_frame);
			}
		}
	}

	// Refresh the display if necessary
	if (_active)
		draw();
}

/**
 * Update the list of the indexes of items in the player's inventory
 */
void UIElements::updateInvList() {
	// Update the index list of items in the player's inventory
	_itemList.clear();

	SynchronizedList<InvObject *>::iterator i;
	int itemIndex = 0;
	for (i = BF_GLOBALS._inventory->_itemList.begin(); i != BF_GLOBALS._inventory->_itemList.end(); ++i, ++itemIndex) {
		InvObject *invObject = *i;
		if (invObject->inInventory())
			_itemList.push_back(itemIndex);
	}
}

/**
 * Set the game score
 */
void UIElements::addScore(int amount) {
	_scoreValue += amount;
	BF_GLOBALS._sound2.play(0);
	updateInventory();
}

} // End of namespace BlueForce

} // End of namespace TsAGE
