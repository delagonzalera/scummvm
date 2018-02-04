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

#ifndef BLADERUNNER_RECT_H
#define BLADERUNNER_RECT_H

#include "common/debug.h"
#include "common/types.h"
#include "common/util.h"

namespace BladeRunner {

struct Rect;
bool overlaps(const Rect &a, const Rect &b);

struct Rect {
	float top;
	float left;
	float bottom;
	float right;

	Rect()
		: top(0.0f), left(0.0f), bottom(0.0f), right(0.0f)
	{}
	Rect(float top, float left, float bottom, float right)
		: top(top), left(left), bottom(bottom), right(right)
	{}

	void expand(float d) {
		top    -= d;
		left   -= d;
		bottom += d;
		right  += d;
	}

	void trunc_2_decimals() {
		top    = truncf(top    * 100.0f) / 100.0f;
		left   = truncf(left   * 100.0f) / 100.0f;
		bottom = truncf(bottom * 100.0f) / 100.0f;
		right  = truncf(right  * 100.0f) / 100.0f;
	}
};

inline bool overlaps(const Rect &a, const Rect &b) {
	return a.left < b.right && a.right > b.left && a.top > b.bottom && a.bottom < b.top;
}

inline Rect merge(const Rect &a, const Rect &b) {
	Rect c;
	c.top    = MIN(a.top,    b.top);
	c.left   = MIN(a.left,   b.left);
	c.bottom = MAX(a.bottom, b.bottom);
	c.right  = MAX(a.right,  b.right);
	return c;
}

} // End of namespace BladeRunner

#endif
