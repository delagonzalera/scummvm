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

#include "bladerunner/obstacles.h"

#include "bladerunner/bladerunner.h"

#include "bladerunner/view.h"

#include "common/debug.h"

#define WITHIN_TOLERANCE(a, b) (((a) - 0.009) < (b) && ((a) + 0.009) > (b))

namespace BladeRunner {

Obstacles::Obstacles(BladeRunnerEngine *vm) {
	_vm = vm;
	_polygons       = new Polygon[kPolygonCount];
	_polygonsBackup = new Polygon[kPolygonCount];
	_vertices       = new Vector2[kVertexCount];
	clear();
}

Obstacles::~Obstacles() {
	clear();

	delete[] _polygons;
	_polygons = nullptr;

	delete[] _polygonsBackup;
	_polygonsBackup = nullptr;

	delete[] _vertices;
	_vertices = nullptr;
}

void Obstacles::clear() {
	for (int i = 0; i < kPolygonCount; i++) {
		_polygons[i].isPresent = false;
		_polygons[i].verticeCount = 0;
		for (int j = 0; j < kPolygonVertexCount; j++) {
			_polygons[i].vertices[j].x = 0.0f;
			_polygons[i].vertices[j].y = 0.0f;
		}
	}
	_verticeCount = 0;
	_backup = false;
	_count = 0;
}

bool Obstacles::lineLineIntersection(LineSegment a, LineSegment b, Vector2 *intersectionPoint) {
	Vector2 s1(a.end.x - a.start.x, a.end.y - a.start.y);
	Vector2 s2(b.end.x - b.start.x, b.end.y - b.start.y);

	float s = (s1.x * (a.start.y - b.start.y) - s1.y * (a.start.x - b.start.x)) / (s1.x * s2.y - s2.x * s1.y);
	float t = (s2.x * (a.start.y - b.start.y) - s2.y * (a.start.x - b.start.x)) / (s1.x * s2.y - s2.x * s1.y);

	if (s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f) {
		intersectionPoint->x = a.start.x + (t * s1.x);
		intersectionPoint->y = a.start.y + (t * s1.y);
		return true;
	}

	return false; // No collision
}

bool Obstacles::linePolygonIntersection(LineSegment lineA, VertexType lineAType, Polygon *polyB, Vector2 *intersectionPoint, int *intersectionIndex) {
	bool hasIntersection = false;
	float nearestIntersectionDistance = 0.0f;

	for (int i = 0; i != polyB->verticeCount; ++i) {
		LineSegment lineB;
		lineB.start = polyB->vertices[i];
		lineB.end   = polyB->vertices[(i+1) % polyB->verticeCount];

		VertexType lineBType = polyB->vertexType[i];

		Vector2 newIntersectionPoint;

		if (lineLineIntersection(lineA, lineB, &newIntersectionPoint)) {
			if (lineAType == TOP_RIGHT    && lineBType == TOP_LEFT
			 || lineAType == BOTTOM_RIGHT && lineBType == TOP_RIGHT
			 || lineAType == BOTTOM_LEFT  && lineBType == BOTTOM_RIGHT
			 || lineAType == TOP_LEFT     && lineBType == BOTTOM_LEFT
			) {
				if (!WITHIN_TOLERANCE(lineB.end.x, intersectionPoint->x)
				 || !WITHIN_TOLERANCE(lineB.end.y, intersectionPoint->y)) {
					if (newIntersectionPoint != *intersectionPoint) {
						float newIntersectionDistance = getLength(intersectionPoint->x, intersectionPoint->y, newIntersectionPoint.x, newIntersectionPoint.y);
						if (!hasIntersection || newIntersectionDistance < nearestIntersectionDistance) {
							hasIntersection = true;
							nearestIntersectionDistance = newIntersectionDistance;
							*intersectionPoint = newIntersectionPoint;
							*intersectionIndex = i;
						}
					}
				}
			}
		}
	}

	return hasIntersection;
}

/*
 * Polygons vertices are defined in clock-wise order
 * starting at the top-most, right-most corner.
 *
 * When merging two polygons, we start at the top-most, right-most vertex.
 * The polygon with this vertex starts is the primary polygon.
 * We follow the edges until we find an intersection with the secondary polygon,
 * in which case we switch primary and secondary and continue following the new edges.
 *
 * Luckily the first two polygons added in RC01 (A, then B) are laid as as below,
 * making an ideal test case.
 *
 * Merge order: (B0,B1) (B1,B2) (B2,J) (J,A2) (A2,A3) (A3,A0) (A0,I) (I,B0)
 *
 *   0,0 ---> x
 *   |
 *   |                   primary
 *   |      B 0 ----- 1
 *   |        |       |
 *   |  A 0 --I-- 1   |
 *   |    |   |   |   |
 *   |    |   3 --J-- 2
 *   |    |       |
 *   |    3 ----- 2
 *   |               secondary
 *   v y
 */

bool Obstacles::mergePolygons(Polygon &polyA, Polygon &polyB) {
	bool flagDidMergePolygons = false;
	Polygon polyMerged;
	polyMerged.rect = merge(polyA.rect, polyB.rect);

	Polygon *polyPrimary, *polySecondary;
	if (polyA.rect.y0 < polyB.rect.y0 || (polyA.rect.y0 == polyB.rect.y0 && polyA.rect.x0 < polyB.rect.x0)) {
		polyPrimary = &polyA;
		polySecondary = &polyB;
	} else {
		polyPrimary = &polyB;
		polySecondary = &polyA;
	}

	Vector2 intersectionPoint;
	LineSegment polyLine;
	bool flagAddVertexToVertexList = true;
	bool flagDidFindIntersection = false;
	int vertIndex = 0;

	Polygon *startingPolygon = polyPrimary;
	int flagDone = false;
	while (!flagDone) {
		VertexType polyPrimaryType;

		polyLine.start  = flagDidFindIntersection ? intersectionPoint : polyPrimary->vertices[vertIndex];
		polyLine.end    = polyPrimary->vertices[(vertIndex + 1) % polyPrimary->verticeCount];

		// TODO(madmoose): How does this work when adding a new intersection point?
		polyPrimaryType = polyPrimary->vertexType[vertIndex];

		if (flagAddVertexToVertexList) {
			assert(polyMerged.verticeCount < kPolygonVertexCount);
			polyMerged.vertices[polyMerged.verticeCount] = polyLine.start;
			polyMerged.vertexType[polyMerged.verticeCount] = polyPrimaryType;
			polyMerged.verticeCount++;
		}

		flagAddVertexToVertexList = true;
		int polySecondaryIntersectionIndex = -1;

		if (linePolygonIntersection(polyLine, polyPrimaryType, polySecondary, &intersectionPoint, &polySecondaryIntersectionIndex)) {
			if (WITHIN_TOLERANCE(intersectionPoint.x, polyLine.start.x) && WITHIN_TOLERANCE(intersectionPoint.y, polyLine.start.y)) {
				flagAddVertexToVertexList = false;
				polyMerged.verticeCount--; // TODO(madmoose): How would this work?
			} else {
				// Obstacles::nop
			}
			vertIndex = polySecondaryIntersectionIndex;
			flagDidFindIntersection = true;

			// Swap secondary and primary polygon
			if (polyPrimary == &polyA) {
				polyPrimary = &polyB;
				polySecondary = &polyA;
			} else {
				polyPrimary = &polyA;
				polySecondary = &polyB;
			}

			flagDidMergePolygons = true;
		} else {
			vertIndex = (vertIndex + 1) % polyPrimary->verticeCount;
			flagDidFindIntersection = false;
		}
		if (startingPolygon->vertices[0] == polyPrimary->vertices[vertIndex]) {
			flagDone = true;
		}
	}

	if (flagDidMergePolygons) {
		*startingPolygon = polyMerged;
		startingPolygon->isPresent = true;
		if (startingPolygon == &polyA) {
			polyB.isPresent = false;
		} else {
			polyA.isPresent = false;
		}
	}

	return flagDidMergePolygons;
}

void Obstacles::add(Rect rect) {
	int polygonIndex = findEmptyPolygon();
	if (polygonIndex < 0) {
		return;
	}

	rect.expand(12.0f);
	rect.trunc_2_decimals();

	debug("\nObstacles::add { %.2f %.2f %.2f %.2f }", rect.x0, rect.y0, rect.x1, rect.y1);

	Polygon &poly = _polygons[polygonIndex];

	poly.rect = rect;

	poly.vertices[0] = Vector2(rect.x0, rect.y0);
	poly.vertexType[0] = TOP_LEFT;

	poly.vertices[1] = Vector2(rect.x1, rect.y0);
	poly.vertexType[1] = TOP_RIGHT;

	poly.vertices[2] = Vector2(rect.x1, rect.y1);
	poly.vertexType[2] = BOTTOM_RIGHT;

	poly.vertices[3] = Vector2(rect.x0, rect.y1);
	poly.vertexType[3] = BOTTOM_LEFT;

	poly.isPresent = true;
	poly.verticeCount = 4;

restart:
	for (int i = 0; i < kPolygonCount; ++i) {
		Polygon &polyA = _polygons[i];
		if (!polyA.isPresent) {
			continue;
		}

		for (int j = i+1; j < kPolygonCount; ++j) {
			Polygon &polyB = _polygons[j];
			if (!polyB.isPresent) {
				continue;
			}

			if (!overlaps(polyA.rect, polyB.rect)) {
				continue;
			}

			if (mergePolygons(polyA, polyB)) {
				goto restart;
			}
		}
	}

	for (int i = 0; i != kPolygonCount; ++i) {
		if (_polygons[i].isPresent) {
			debug("[%d] = {", i);
			debug("\trect:  { %.2f, %.2f, %.2f, %.2f }", rect.x0, rect.y0, rect.x1, rect.y1);

			Common::String s;
			s += "\tverts: {";
			for (int j = 0; j != _polygons[i].verticeCount; ++j) {
				s += Common::String::format(" %.2f,%.2f", _polygons[i].vertices[j].x, _polygons[i].vertices[j].y);
			}
			s += " }";
			debug("%s", s.c_str());

			s = "";
			s += "\ttypes: {";
			for (int j = 0; j != _polygons[i].verticeCount; ++j) {
				s += Common::String::format(" %d", _polygons[i].vertexType[j]);
			}
			s += " }";
			debug("%s", s.c_str());

			debug("}");
		}
	}
}

int Obstacles::findEmptyPolygon() const {
	for (int i = 0; i < kPolygonCount; i++) {
		if (!_polygons[i].isPresent) {
			return i;
		}
	}
	return -1;
}

float Obstacles::getLength(float x0, float z0, float x1, float z1) {
	if (x0 == x1) {
		return fabs(z1 - z0);
	}
	return fabs(x1 - x0);
}

bool Obstacles::find(const Vector3 &from, const Vector3 &to, Vector3 *next) const {
	//TODO
	*next = to;
	return true;
}

bool Obstacles::findIntersectionNearest(int polygonIndex, Vector2 from, Vector2 to,
                                        int *outVertexIndex, float *outDistance, Vector2 *out) const
{
	float   minDistance = 0.0f;
	Vector2 minIntersection;
	int     minVertexIndex = -1;

	bool hasDistance = false;

	for (int i = 0; i < _polygons[polygonIndex].verticeCount; ++i) {
		int nextIndex = (i + 1) % _polygons[polygonIndex].verticeCount;
		Vector2 *vertices = _polygons[polygonIndex].vertices;
		Vector2 intersection;
		bool intersects = lineIntersection(from, to, vertices[i], vertices[nextIndex], &intersection);
		if (intersects) {
			float distance2 = distance(from, intersection);
			if (!hasDistance || distance2 < minDistance) {
				minDistance = distance2;
				minIntersection = intersection;
				minVertexIndex = i;
				hasDistance = true;
			}
		}
	}

	*outDistance    = minDistance;
	*outVertexIndex = minVertexIndex;
	*out            = minIntersection;

	return minVertexIndex != -1;
}

bool Obstacles::findIntersectionFarthest(int polygonIndex, Vector2 from, Vector2 to,
                                         int *outVertexIndex, float *outDistance, Vector2 *out) const
{
	float   maxDistance = 0.0f;
	Vector2 maxIntersection;
	int     maxVertexIndex = -1;

	bool hasDistance = false;

	for (int i = 0; i < _polygons[polygonIndex].verticeCount; ++i) {
		int nextIndex = (i + 1) % _polygons[polygonIndex].verticeCount;
		Vector2 *vertices = _polygons[polygonIndex].vertices;
		Vector2 intersection;
		bool intersects = lineIntersection(from, to, vertices[i], vertices[nextIndex], &intersection);
		if (intersects) {
			float distance2 = distance(from, intersection);
			if (!hasDistance || distance2 > maxDistance) {
				maxDistance = distance2;
				maxIntersection = intersection;
				maxVertexIndex = i;
				hasDistance = true;
			}
		}
	}

	*outDistance    = maxDistance;
	*outVertexIndex = maxVertexIndex;
	*out            = maxIntersection;

	return maxVertexIndex != -1;
}

bool Obstacles::findPolygonVerticeByXZ(int *polygonIndex, int *verticeIndex, int *verticeCount, float x, float z) const {
	*polygonIndex = -1;
	*verticeIndex = -1;
	*verticeCount = -1;

	for (int i = 0; i != kPolygonCount; ++i) {
		if (!_polygons[i].isPresent || _polygons[i].verticeCount == 0) {
			continue;
		}

		for (int j = 0; j != kPolygonVertexCount; ++j) {
			if (_polygons[i].vertices[j].x == x && _polygons[i].vertices[j].y == z) {
				*polygonIndex = i;
				*verticeIndex = j;
				*verticeCount = _polygons[i].verticeCount;
				return true;
			}
		}
	}

	return false;
}

bool Obstacles::findPolygonVerticeByXZWithinTolerance(float x, float z, int *polygonIndex, int *verticeIndex) const {
	*polygonIndex = -1;
	*verticeIndex = -1;

	for (int i = 0; i != kPolygonCount; ++i) {
		if (!_polygons[i].isPresent || _polygons[i].verticeCount == 0) {
			continue;
		}

		for (int j = 0; j != kPolygonVertexCount; ++j) {
			if (WITHIN_TOLERANCE(_polygons[i].vertices[j].x, x)) {
				if (WITHIN_TOLERANCE(_polygons[i].vertices[j].y, z)) {
					*polygonIndex = i;
					*verticeIndex = j;
					return true;
				}
			}
		}
	}

	return false;
}

void Obstacles::clearVertices() {
	_verticeCount = 0;
}

void Obstacles::copyVerticesReverse() {

}

void Obstacles::copyVertices() {

}

void Obstacles::backup() {
	for (int i = 0; i != kPolygonCount; ++i) {
		_polygonsBackup[i].isPresent = false;
	}

	int count = 0;
	for (int i = 0; i != kPolygonCount; ++i) {
		if (_polygons[i].isPresent) {
			_polygonsBackup[count] = _polygons[i];
			++count;
		}
	}

	for (int i = 0; i != kPolygonCount; ++i) {
		_polygons[i] = _polygonsBackup[count];
	}

	_count = count;
	_backup = true;
}

void Obstacles::restore() {
	for (int i = 0; i != kPolygonCount; ++i) {
		_polygons[i].isPresent = false;
	}
	for (int i = 0; i != kPolygonCount; ++i) {
		_polygons[i] = _polygonsBackup[i];
	}
}

void Obstacles::draw() {
	for (int i = 0; i != kPolygonCount; ++i) {
		if (!_polygons[i].isPresent) {
			continue;
		}

		Vector3 p0 = _vm->_view->calculateScreenPosition(Vector3(
			_polygons[i].vertices[_polygons[i].verticeCount - 1].x,
			0,
			_polygons[i].vertices[_polygons[i].verticeCount - 1].y
		));

		for (int j = 0; j != _polygons[i].verticeCount; ++j) {
			Vector3 p1 = _vm->_view->calculateScreenPosition(Vector3(
				_polygons[i].vertices[j].x,
				0.0f,
				_polygons[i].vertices[j].y
			));

			_vm->_surfaceFront.drawLine(p0.x, p0.y, p1.x, p1.y, 0x7FE0);

			p0 = p1;
		}
	}
}

} // End of namespace BladeRunner
