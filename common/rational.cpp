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
 * $URL$
 * $Id$
 */

#include "common/rational.h"
#include "common/util.h"
#include "common/algorithm.h"

namespace Common {

Rational::Rational() {
	_num   = 1;
	_denom = 1;
}

Rational::Rational(int num) {
	_num   = num;
	_denom = 1;
}

Rational::Rational(int num, int denom) {
	assert(denom != 0);

	_num   = num;
	_denom = denom;

	normalize();
}

void Rational::cancel() {
	// Cancel the fraction by dividing both the num and the denom
	// by their greatest common denom.

	int gcd = Common::gcd(_num, _denom);

	_num   /= gcd;
	_denom /= gcd;
}

void Rational::normalize() {
	// Is the fraction negative?
	bool negative = !((!(_num < 0)) == (!(_denom < 0)));

	// Make both integers positive
	_num   = ABS(_num);
	_denom = ABS(_denom);

	// Cancel the fraction
	cancel();

	// If the fraction is supposed to be negative, make the num negative
	if (negative)
		_num = -_num;
}

Rational &Rational::operator=(const Rational &right) {
	_num   = right._num;
	_denom = right._denom;

	return *this;
}

Rational &Rational::operator=(int right) {
	_num   = right;
	_denom = 1;

	return *this;
}

Rational &Rational::operator+=(const Rational &right) {
	_num   = _num * right._denom + right._num * _denom;
	_denom = _denom * right._denom;

	normalize();

	return *this;
}

Rational &Rational::operator-=(const Rational &right) {
	_num   = _num * right._denom - right._num * _denom;
	_denom = _denom * right._denom;

	normalize();

	return *this;
}

Rational &Rational::operator*=(const Rational &right) {
	// Try to cross-cancel first, to avoid unnecessary overflow
	int gcd1 = Common::gcd(_num, right._denom);
	int gcd2 = Common::gcd(right._num, _denom);

	_num   = (_num    / gcd1) * (right._num    / gcd2);
	_denom = (_denom  / gcd2) * (right._denom  / gcd1);

	normalize();

	return *this;
}

Rational &Rational::operator/=(const Rational &right) {
	return *this *= Rational(right._denom, right._num);
}

Rational &Rational::operator+=(int right) {
	return *this += Rational(right);
}

Rational &Rational::operator-=(int right) {
	return *this -= Rational(right);
}

Rational &Rational::operator*=(int right) {
	return *this *= Rational(right);
}

Rational &Rational::operator/=(int right) {
	return *this /= Rational(right);
}

const Rational Rational::operator-() const {
	return Rational(-_num, _denom);
}

const Rational Rational::operator+(const Rational &right) const {
	Rational tmp = *this;

	tmp += right;

	return tmp;
}

const Rational Rational::operator-(const Rational &right) const {
	Rational tmp = *this;

	tmp -= right;

	return tmp;
}

const Rational Rational::operator*(const Rational &right) const {
	Rational tmp = *this;

	tmp *= right;

	return tmp;
}

const Rational Rational::operator/(const Rational &right) const {
	Rational tmp = *this;

	tmp /= right;

	return tmp;
}

const Rational Rational::operator+(int right) const {
	Rational tmp = *this;

	tmp += right;

	return tmp;
}

const Rational Rational::operator-(int right) const {
	Rational tmp = *this;

	tmp -= right;

	return tmp;
}

const Rational Rational::operator*(int right) const {
	Rational tmp = *this;

	tmp *= right;

	return tmp;
}

const Rational Rational::operator/(int right) const {
	Rational tmp = *this;

	tmp /= right;

	return tmp;
}

bool Rational::operator==(const Rational &right) const {
	return (_num == right._num) && (_denom == right._denom);
}

bool Rational::operator!=(const Rational &right) const {
	return (_num != right._num) || (_denom != right._denom);
}

bool Rational::operator>(const Rational &right) const {
	return (_num * right._denom) > (right._num * _denom);
}

bool Rational::operator<(const Rational &right) const {
	return (_num * right._denom) < (right._num * _denom);
}

bool Rational::operator>=(const Rational &right) const {
	return (_num * right._denom) >= (right._num * _denom);
}

bool Rational::operator<=(const Rational &right) const {
	return (_num * right._denom) <= (right._num * _denom);
}

bool Rational::operator==(int right) const {
	return (_denom == 1) && (_num == right);
}

bool Rational::operator!=(int right) const {
	return (_denom == 1) && (_num != right);
}

bool Rational::operator>(int right) const {
	return *this > Rational(right, 1);
}

bool Rational::operator<(int right) const {
	return *this < Rational(right, 1);
}

bool Rational::operator>=(int right) const {
	return *this >= Rational(right, 1);
}

bool Rational::operator<=(int right) const {
	return *this <= Rational(right, 1);
}

void Rational::invert() {
	assert(_num != 0);

	SWAP(_num, _denom);

	normalize();
}

Rational Rational::getInverse() const {
	Rational inverse = *this;

	inverse.invert();

	return inverse;
}

int Rational::toInt() const {
	assert(_denom != 0);

	return _num / _denom;
}

double Rational::toDouble() const {
	assert(_denom != 0);

	return ((double) _num) / ((double) _denom);
}

frac_t Rational::toFrac() const {
	return (_num * FRAC_ONE) / _denom;
}

Rational::operator int() const {
	return toInt();
}

Rational::operator double() const {
	return toDouble();
}

const Rational operator+(int left, const Rational &right) {
	Rational tmp = right;

	tmp += left;

	return tmp;
}

const Rational operator-(int left, const Rational &right) {
	Rational tmp = right;

	tmp -= left;

	return tmp;
}

const Rational operator*(int left, const Rational &right) {
	Rational tmp = right;

	tmp *= left;

	return tmp;
}

const Rational operator/(int left, const Rational &right) {
	Rational tmp = right;

	tmp /= left;

	return tmp;
}

bool operator==(int left, const Rational &right) {
	return right == left;
}

bool operator!=(int left, const Rational &right) {
	return right != left;
}

bool operator>(int left, const Rational &right) {
	return right < left;
}

bool operator<(int left, const Rational &right) {
	return right > left;
}

bool operator>=(int left, const Rational &right) {
	return right <= left;
}

bool operator<=(int left, const Rational &right) {
	return right >= left;
}

} // End of namespace Common
