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

#include "bladerunner/subtitles.h"

#include "bladerunner/font.h"
#include "bladerunner/text_resource.h"
#include "bladerunner/audio_speech.h"

#include "common/debug.h"

#include "graphics/font.h"
#include "graphics/fonts/ttf.h"

namespace BladeRunner {

/*
 * Optional support for subtitles
 *
 * CHECK what happens in VQA where the audio plays separately (are the finales such VQAs ?)
 * TODO? Use another escape sequence to progressively display text in a line (like in SCUMM games) <-- this could be very useful with very long lines
 *			- might also need an extra manual time or ticks parameter to determine when during the display of the first segment we should switch to the second.
 * TODO? A more advanced subtitles system:
 *          TODO: subtitles could be independent from sound playing (but probably should disappear when switching between UI screens)
 *          TODO?: Support for queuing subtitles when more than one subtitle should play for a spoken dialogue (due to a very long quote)
 *          TODO?: Predefine a minimum time for a subtitle to appear, before it is interrupted by the next one. (might need queuing)
 *          TODO?: If the subtitle is the last one then extend its duration to another predefined delay.
 *
 * DONE Removed support for internal font TAHOMA18 - this particular font is corrupted!
 * DONE Create and Support proper external FON for subtitles.
 * DONE split at new line character (priority over auto-split)
 * DONE auto-split a long line into two
 * DONE support the basic 2 line subtitles
 * DONE support a third line for subtitles (some quotes are too long for 2 lines). Are there quotes that are too long for 3 lines?
 * DONE handle missing subtitle files! Gracefully don't show subtitles for VQAs or in-game dialogue if the required respective files are missing!
 * DONE add subtitle files for the rest of VQAs that have spoken dialogue
 * DONE A system to auto-split a dialogue after some max total width of character glyphs per line.
 * DONE - OK - CHECK What happens with skipped dialogue (enter / skip dialogue key pressed)
 * DONE - OK - CHECK what happens in VQA when no corresponding TRE subs file?
 */

const char *Subtitles::SUBTITLES_FONT_FILENAME_EXTERNAL = "SUBTLS_E.FON";

const char *Subtitles::SUBTITLES_VERSION_TRENAME        = "SBTLVERS"; // addon resource file for Subtitles version info - can only be SBTLVERS.TRE
/*
* All entries need to have the language code appended (after a '_').
* And all entries should get the suffix extension ".TRx"; the last letter in extension "TR*" should also be the language code
* If/When adding new Text Resources here --> Update kMaxTextResourceEntries and also update method getIdxForSubsTreName()
*/
const char *Subtitles::SUBTITLES_FILENAME_PREFIXES[kMaxTextResourceEntries] = {
	"INGQUO",           // 0 // (in-game subtitles, not VQA subtitles)
	"WSTLGO",           // 1 // all game (language) versions have the English ('E') version of WSTLGO
	"BRLOGO",           // 2 // all game (language) versions have the English ('E') version of BRLOGO
	"INTRO",            // 3
	"MW_A",             // 4
	"MW_B01",           // 5
	"MW_B02",           // 6
	"MW_B03",           // 7
	"MW_B04",           // 8
	"MW_B05",           // 9
	"INTRGT",           // 10
	"MW_C01",           // 11
	"MW_C02",           // 12
	"MW_C03",           // 13
	"MW_D",             // 14
	"END04A",           // 15
	"END04B",           // 16
	"END04C",           // 17
	"END06",            // 18
	"END01A",           // 19
	"END01B",           // 20
	"END01C",           // 21
	"END01D",           // 22
	"END01E",           // 23
	"END01F",           // 24
	"END03"             // 25
};

/**
* Subtitles Constructor
*/
Subtitles::Subtitles(BladeRunnerEngine *vm) {
	_vm = vm;
	_subtitlesSystemActive = false;
	// Initializing and reseting Subtitles
	for (int i = 0; i < kMaxTextResourceEntries; i++) {
		_vqaSubsTextResourceEntries[i] = nullptr;
	}
	_font = nullptr;
	reset();
}

/**
* Subtitles Destructor
*/
Subtitles::~Subtitles() {
	reset();
}

//
// Init is kept separated from constructor to allow not loading up resources if subtitles system is disabled
//
void Subtitles::init(void) {
	// Loading subtitles versioning info if available
	TextResource *versionTxtResource = new TextResource(_vm);
	if ( versionTxtResource->open(SUBTITLES_VERSION_TRENAME, false)) {
		_subtitlesInfo.credits = versionTxtResource->getText((uint32)0);
		_subtitlesInfo.versionStr = versionTxtResource->getText((uint32)1);
		_subtitlesInfo.dateOfCompile = versionTxtResource->getText((uint32)2);
		_subtitlesInfo.languageMode = versionTxtResource->getText((uint32)3);
		Common::String fontType = versionTxtResource->getText((uint32)4);
		_subtitlesInfo.fontName = versionTxtResource->getText((uint32)5);

		if (fontType.equalsIgnoreCase("ttf")) {
			_subtitlesInfo.fontType = Subtitles::kSubtitlesFontTypeTTF;
		} else {
			_subtitlesInfo.fontType = Subtitles::kSubtitlesFontTypeInternal;
		}

		if ( _subtitlesInfo.fontName.empty()) {
			_subtitlesInfo.fontName = "SUBTLS_E.FON";
		}

		debug("Subtitles version info: v%s (%s) %s by: %s",
		       _subtitlesInfo.versionStr.c_str(),
		       _subtitlesInfo.dateOfCompile.c_str(),
		       _subtitlesInfo.languageMode.c_str(),
		       _subtitlesInfo.credits.c_str());

		delete versionTxtResource;
		versionTxtResource = nullptr;
	} else {
		debug("Subtitles version info: N/A");
	}

	//
	// Initializing/Loading Subtitles Fonts
	if (_subtitlesInfo.fontType == Subtitles::kSubtitlesFontTypeInternal) {
		// Use TAHOMA18.FON (is corrupted in places)
		// 10PT or TAHOMA24 or KIA6PT  have all caps glyphs (and also are too big or too small) so they are not appropriate.
		_font = Font::load(_vm, _subtitlesInfo.fontName, -1, true);
		_useUTF8 = false;
	} else if (_subtitlesInfo.fontType == Subtitles::kSubtitlesFontTypeTTF) {
#if defined(USE_FREETYPE2)
		Common::ScopedPtr<Common::SeekableReadStream> stream(_vm->getResourceStream(_subtitlesInfo.fontName));
		// Common::ScopedPtr<Common::SeekableReadStream> stream(SearchMan.createReadStreamForMember("NotoSansCJKsc-Medium.otf"));

		_font = Graphics::loadTTFFont(*stream, 18);
		_useUTF8 = true;
#else
		warning("Subtitles require a TTF font but this ScummVM build doesn't support it.");
		return;
#endif
	}

	if (_font) {
		debug("Subtitles font '%s' was loaded successfully.", _subtitlesInfo.fontName.c_str());
	} else {
		warning("Subtitles font '%s' could not be loaded.", _subtitlesInfo.fontName.c_str());
		return;
	}
	//Done - Initializing/Loading Subtitles Fonts
	//

	//
	// Loading text resources
	for (int i = 0; i < kMaxTextResourceEntries; i++) {
		_vqaSubsTextResourceEntries[i] = new TextResource(_vm);
		Common::String tmpConstructedFileName = "";
		bool localizedResource = true;
		if (!strcmp(SUBTITLES_FILENAME_PREFIXES[i], "WSTLGO") || !strcmp(SUBTITLES_FILENAME_PREFIXES[i], "BRLOGO")) {
			tmpConstructedFileName = Common::String(SUBTITLES_FILENAME_PREFIXES[i]) + "_E"; // Only English versions of these exist
			localizedResource = false;
		}
		else {
			tmpConstructedFileName = Common::String(SUBTITLES_FILENAME_PREFIXES[i]) + "_" + _vm->_languageCode;
		}

		if ( _vqaSubsTextResourceEntries[i]->open(tmpConstructedFileName, localizedResource)) {
			_gameSubsResourceEntriesFound[i] = true;
		}
	}
	// Done - Loading text resources
	//

	//
	// calculate the Screen Y position of the subtitle lines
	// getTextHeight("") returns the maxHeight of the font glyphs regardless of the actual text parameter
	//  debug("Max height %d", _subsFont->getTextHeight(""));
	for (int i = 0; i < kMaxNumOfSubtitlesLines; ++i) {
		_subtitleLineScreenY[i] = 479 - kSubtitlesBottomYOffsetPx - ((kMaxNumOfSubtitlesLines - i) * (_font->getFontHeight() + 1));
	}

	_subtitlesSystemActive = true;
}

Subtitles::SubtitlesInfo Subtitles::getSubtitlesInfo() const {
	return _subtitlesInfo;
}

/**
*
* Returns the index of the specified Text Resource filename in the SUBTITLES_FILENAME_PREFIXES table
*/
int Subtitles::getIdxForSubsTreName(const Common::String &treName) const {
	Common::String tmpConstructedFileName = "";
	for (int i = 0; i < kMaxTextResourceEntries; ++i) {
		if (!strcmp(SUBTITLES_FILENAME_PREFIXES[i], "WSTLGO") || !strcmp(SUBTITLES_FILENAME_PREFIXES[i], "BRLOGO")) {
			tmpConstructedFileName = Common::String(SUBTITLES_FILENAME_PREFIXES[i]) + "_E"; // Only English versions of these exist
		} else {
			tmpConstructedFileName = Common::String(SUBTITLES_FILENAME_PREFIXES[i]) + "_" + _vm->_languageCode;
		}
		if (tmpConstructedFileName == treName) {
			return i;
		}
	}
	// error case
	return -1;
}

/**
* Get the active subtitle text by searching with actor ID and speech ID
* Use this method for in-game dialogue - Not dialogue during a VQA cutscene
* Returns the dialogue quote, but also sets the private _currentSubtitleTextFull member
*/
const char *Subtitles::getInGameSubsText(int actorId, int speech_id)  {
	if (!_subtitlesSystemActive) {
		return "";
	}

	int32 id = 10000 * actorId + speech_id;
	if (!_gameSubsResourceEntriesFound[0]) {
		if (_currentSubtitleTextFull  != "") {
			_currentSubtitleTextFull = "";
			_subtitlesQuoteChanged = true;
		}
		return "";
	}
	// Search in the first TextResource of the _vqaSubsTextResourceEntries table, which is the TextResource for in-game dialogue (i.e. not VQA dialogue)
	const Common::String &text = _vqaSubsTextResourceEntries[0]->getText((uint32)id);
	_currentSubtitleTextFull = Common::String(text);
	_subtitlesQuoteChanged = true;
	return _currentSubtitleTextFull.c_str();
}

/**
* Use this method for dialogue during VQA cutscenes
* Returns the dialogue quote, but also sets the private _currentSubtitleTextFull member
*/
const char *Subtitles::getOuttakeSubsText(const Common::String &outtakesName, int frame) {
	if (!_subtitlesSystemActive) {
		return "";
	}

	int fileIdx = getIdxForSubsTreName(outtakesName);
	if (fileIdx == -1 || !_gameSubsResourceEntriesFound[fileIdx]) {
		if (_currentSubtitleTextFull != "") {
			_currentSubtitleTextFull = "";
			_subtitlesQuoteChanged = true;
		}
		return "";
	}
	// Search in the requested TextResource at the fileIdx index of the _vqaSubsTextResourceEntries table for a quote that corresponds to the specified video frame
	// debug("Number of resource quotes to search: %d, requested frame: %u", _vqaSubsTextResourceEntries[fileIdx]->getCount(), (uint32)frame );
	const Common::String &text = _vqaSubsTextResourceEntries[fileIdx]->getOuttakeTextByFrame((uint32)frame);
	//if (text != "") {
	//    debug("Text = %s", text.c_str());
	//}
	if (_currentSubtitleTextFull != Common::String(text)) {
		_currentSubtitleTextFull = Common::String(text);
		_subtitlesQuoteChanged = true;
	}
	return _currentSubtitleTextFull.c_str();
}

/**
* Explicitly set the active subtitle text to be displayed
* Used for debug purposes mainly.
*/
void Subtitles::setGameSubsText(Common::String dbgQuote, bool forceShowWhenNoSpeech) {
	if (_currentSubtitleTextFull != dbgQuote) {
		_currentSubtitleTextFull = dbgQuote;
		_subtitlesQuoteChanged = true;
		_forceShowWhenNoSpeech = forceShowWhenNoSpeech; // overrides not showing subtitles when no one is speaking
	}
}

/**
* Sets the _isVisible member var to true if it's not already set
* @return true if the member was set now, false if the member was already set
*/
bool Subtitles::show() {
	if (!_subtitlesSystemActive) {
		return false;
	}

	if (_isVisible) {
		return false;
	}
	_isVisible = true;
	return true;
}

/**
* Clears the _isVisible member var if not already clear.
* @return true if the member was cleared, false if it was already clear.
*/
bool Subtitles::hide() {
	if (!_subtitlesSystemActive) {
		return false;
	}

	if (!_isVisible) {
		return false;
	}

	_isVisible = false;
	return true;
}

/**
* Checks whether the subtitles should be visible or not
* @return the value of the _isVisible member boolean var
*/
bool Subtitles::isVisible() const {
	return !_subtitlesSystemActive || _isVisible;
}

/**
* Tick method specific for outtakes (VQA videos)
*/
void Subtitles::tickOuttakes(Graphics::Surface &s) {
	if (!_subtitlesSystemActive || !_vm->isSubtitlesEnabled()) {
		return;
	}

	if (_currentSubtitleTextFull.empty()) {
		_vm->_subtitles->hide();
	} else {
		_vm->_subtitles->show();
	}

	if (!_isVisible) { // keep it as a separate if
		return;
	}
	draw(s);
}

/**
* Tick method for in-game subtitles -- Not for outtake cutscenes (VQA videos)
*/
void Subtitles::tick(Graphics::Surface &s) {
	if (!_subtitlesSystemActive || !_vm->isSubtitlesEnabled()) {
		return;
	}

	if (!_vm->_audioSpeech->isPlaying() && !_forceShowWhenNoSpeech && _isVisible) {
		_vm->_subtitles->hide(); // TODO might need a better system. Don't call it always.
	}

	if (!_isVisible)  { // keep it as a separate if
		return;
	}
	draw(s);
}

/**
* Draw method for drawing the subtitles on the display surface
*/
void Subtitles::draw(Graphics::Surface &s) {
	if (!_subtitlesSystemActive || !_isVisible || _currentSubtitleTextFull.empty()) {
		return;
	}
	if (_subtitlesQuoteChanged) {
		calculatePosition(); // Don't always call calc position, only when quote has changed
		_subtitlesQuoteChanged = false;
	}

	// multi-line quotes appear from top to bottom
	// ie. _subtitleLineQuote[0] is the top-most line
	// The default available lines for drawing are:
	// 	(kMaxNumOfSubtitlesLines - kStartFromSubtitleLineFromTop)
	// And by default we prefer drawing starting from line: kStartFromSubtitleLineFromTop.
	// However, if we have to draw more lines than the default available
	// we should then override the default starting line and start from further up instead
	int startingLineFromTop = kStartFromSubtitleLineFromTop;
	if (_currentSubtitleLines > kMaxNumOfSubtitlesLines - kStartFromSubtitleLineFromTop) {
		startingLineFromTop = kMaxNumOfSubtitlesLines  - _currentSubtitleLines;
	}

	for (int i = 0, j = startingLineFromTop; i < _currentSubtitleLines; ++i, ++j) {
		Common::U32String text = _useUTF8 ? convertUtf8ToUtf32(_subtitleLineQuote[i]) : _subtitleLineQuote[i];
		switch (_subtitlesInfo.fontType) {
			case Subtitles::kSubtitlesFontTypeInternal:
				// shadow/outline is part of the font color data
				_font->drawString(&s, text, _subtitleLineScreenX[i], _subtitleLineScreenY[j], s.w, 0);
				break;
			case Subtitles::kSubtitlesFontTypeTTF:
				_font->drawString(&s, text, _subtitleLineScreenX[i] - 1, _subtitleLineScreenY[j] - 1, s.w, s.format.RGBToColor(  0,   0,   0));
				_font->drawString(&s, text, _subtitleLineScreenX[i] + 1, _subtitleLineScreenY[j] - 1, s.w, s.format.RGBToColor(  0,   0,   0));
				_font->drawString(&s, text, _subtitleLineScreenX[i] - 1, _subtitleLineScreenY[j] + 1, s.w, s.format.RGBToColor(  0,   0,   0));
				_font->drawString(&s, text, _subtitleLineScreenX[i] + 1, _subtitleLineScreenY[j] + 1, s.w, s.format.RGBToColor(  0,   0,   0));
				_font->drawString(&s, text, _subtitleLineScreenX[i]    , _subtitleLineScreenY[j]    , s.w, s.format.RGBToColor(255, 255, 255));
				break;
		}
	}
}

/**
* Calculate the position (X axis - horizontal) where the current active subtitle text should be displayed/drawn
* This also determines if more than one lines should be drawn and what text goes into each line; splitting into multiple lines is done here
*
* The code first prioritizes splitting on the "new line" character.
*   That is, if the string contains at least one new line character, then line splitting occurs on new line characters exclusively.
*   The idea is that new line characters are put in the string explicitly by someone who wants specific control over line splitting
*   and thus they assume the responsibility for the resulting line segment widths (the code won't bother with them in this case).
*
* If there are NO "new line" characters, then the code will split lines on a space character (auto-split case).
*   For this case we only split if the full original line width exceeds a preset width threshold.
*   If the threshold is exceeded, then we parse the line and calculate how many lines we can split it into (starting from 2 lines)
*   to get segments smaller than the width threshold and also while maintaining (close to) even width across the resulting line segments.
*   What's happening here is that we loop dividing the original quote's character total by an increasing target number of line segments,
*   in order to get an "ideal" length for each segment (for evenness). Then we seek for split points (space character)
*   past the characters of the "ideal" length points.
*
* For the second case (auto-split), we don't account for the special case of a single word larger than max line length
* (no spaces), as practically this won't ever happen.
*
* TODO: simplify this code
* TODO: maybe calculate auto-split points taking into account on quote pixel width per character and not simply the character count
* TODO: somehow merge with graphics/font.cpp -> wordWrapTextImpl ?
*/
void Subtitles::calculatePosition() {
	// wOrig is in pixels, origQuoteNumOfChars is num of chars in string
	int wOrig = _font->getStringWidth(_currentSubtitleTextFull) + 2; // +2 to account for left/ right shadow pixels (or for good measure)
	int origQuoteNumOfChars = _currentSubtitleTextFull.size();
	int tmpCharIndex = 0;
	bool drawSingleLineQuote = false;

	const uint8 *textCharacters = (const uint8 *)_currentSubtitleTextFull.c_str();
	int tmpLineWidth[kMaxNumOfSubtitlesLines];

	// initialization of aux variables
	_currentSubtitleLines = 1;
	for (int j = 0; j < kMaxNumOfSubtitlesLines; ++j) {
		_subtitleLineSplitAtCharIndex[j] = 0;
		_subtitleLineQuote[j] = "";
		_subtitleLineScreenX[j] = 0;
		tmpLineWidth[j] = 0;
	}

	while (*textCharacters != 0) {
		// check for new line explicit split case
		if (_currentSubtitleLines < kMaxNumOfSubtitlesLines
			&& *textCharacters == '\n'
			&& tmpCharIndex != 0
			&& _subtitleLineSplitAtCharIndex[_currentSubtitleLines - 1] == 0) {
			_subtitleLineSplitAtCharIndex[_currentSubtitleLines - 1] = tmpCharIndex;
			_currentSubtitleLines += 1;
		}
		tmpCharIndex += 1;
		textCharacters += 1;
	}
	_subtitleLineSplitAtCharIndex[_currentSubtitleLines - 1] = tmpCharIndex;
	if (_currentSubtitleLines > 1) { // This means that splitting on new line characters is possible
		//
		int j = 0;																// j iterates over the subtitle line segments
		textCharacters = (const uint8 *)_currentSubtitleTextFull.c_str();		// reset pointer to the start of the subtitle quote
		for (int i = 0; i < origQuoteNumOfChars ; ++i) {						// i iterates over characters in the quote
			if (j < _currentSubtitleLines && i < _subtitleLineSplitAtCharIndex[j]) {
				_subtitleLineQuote[j] += textCharacters[i];
			} else { 															// i is now at a split point of the quote
				_subtitleLineQuote[j] += '\0';
				j += 1;															// start next line
			}
		}
		_subtitleLineQuote[j] += '\0';											// the last line should also be NULL terminated
		//
		// Check widths and set starting X positions per line
		for (int k = 0; k < _currentSubtitleLines; ++k) {
			tmpLineWidth[k] = _font->getStringWidth(_subtitleLineQuote[k]) + 2;
			_subtitleLineScreenX[k] = (639 - tmpLineWidth[k]) / 2;
			_subtitleLineScreenX[k] = CLIP(_subtitleLineScreenX[k], 0, 639 - tmpLineWidth[k]);
		}
	} else {
		// Here we initially have _currentSubtitleLines == 1
		// We check quote for auto-splitting
		// Auto splitting requires space characters in the quote string (which should be ok for the typical cases)
		if (wOrig > kMaxWidthPerLineToAutoSplitThresholdPx) { // kMaxWidthPerLineToAutoSplitThresholdPx is a practical chosen width threshold for auto-splitting quotes purposes
			// Start by splitting in two lines. If the new parts are still too lengthy, re-try by splitting in three lines, etc.
			for (int linesToSplitInto = 2; linesToSplitInto <= kMaxNumOfSubtitlesLines; ++linesToSplitInto) {
				// find the first space after the middle
				_subtitleLineQuote[0] = "";
				_currentSubtitleLines = 1;

				textCharacters = (const uint8 *)_currentSubtitleTextFull.c_str();		// reset pointer to the start of subtitle quote
				textCharacters += (origQuoteNumOfChars / linesToSplitInto);
				_subtitleLineSplitAtCharIndex[0] = (origQuoteNumOfChars / linesToSplitInto);
				while (*textCharacters != 0 && !Common::isSpace(*textCharacters)) {		// seek for a space character
					_subtitleLineSplitAtCharIndex[0] += 1;
					textCharacters += 1;
				}
//                debug("space character at: %d", _subtitleLineSplitAtCharIndex[0]);
				if (Common::isSpace(*textCharacters)) { // if we found a space, we store the segment up to this point in the first _subtitleLineQuote entry
					textCharacters = (const uint8 *)_currentSubtitleTextFull.c_str();
					for (int i = 0; i < _subtitleLineSplitAtCharIndex[0] ; ++i) {
						_subtitleLineQuote[0] += textCharacters[i];
					}
					_subtitleLineQuote[0] += '\0';
//                    debug(" Line 0 quote %s", _subtitleLineQuote[0].c_str());
					tmpLineWidth[0] = _font->getStringWidth(_subtitleLineQuote[0]) + 2; // check the width of the first segment of the quote
					if (tmpLineWidth[0] > kMaxWidthPerLineToAutoSplitThresholdPx && linesToSplitInto < kMaxNumOfSubtitlesLines) {
						// we exceed max width so we reset process by trying to split into more lines
						continue; // re-try the For-loop with increased linesToSplitInto by 1
					} else {
						// keep current split, proceed with splitting the quote for the rest of the subtitle lines (linesToSplitInto)
						for (int j = 2; j <= linesToSplitInto; ++j) {
							textCharacters = (const uint8 *)_currentSubtitleTextFull.c_str();	// reset pointer to the start of subtitle quote
							textCharacters += ((j * origQuoteNumOfChars) / linesToSplitInto);	// move pointer to start of split-seek point for this line segment
							_subtitleLineSplitAtCharIndex[_currentSubtitleLines] = ((j * origQuoteNumOfChars) / linesToSplitInto);
							while (*textCharacters != 0 && !Common::isSpace(*textCharacters)) {
								_subtitleLineSplitAtCharIndex[_currentSubtitleLines] += 1;
								textCharacters += 1;
							}
							textCharacters = (const uint8 *)_currentSubtitleTextFull.c_str();	// reset pointer to the start of subtitle quote
							for (int i = _subtitleLineSplitAtCharIndex[_currentSubtitleLines - 1] + 1; i < _subtitleLineSplitAtCharIndex[_currentSubtitleLines]; ++i) {
								_subtitleLineQuote[_currentSubtitleLines] += textCharacters[i];
							}
							_subtitleLineQuote[_currentSubtitleLines] +=  '\0';
//                            debug(" Line %d, space blank at: %d, quote %s", _currentSubtitleLines, _subtitleLineSplitAtCharIndex[_currentSubtitleLines], _subtitleLineQuote[_currentSubtitleLines].c_str());
							_currentSubtitleLines += 1;
						}
						//
						// Check widths and set starting X positions per line
						for (int j = 0; j < _currentSubtitleLines; ++j) {
							tmpLineWidth[j] = _font->getStringWidth(_subtitleLineQuote[j]) + 2;
							_subtitleLineScreenX[j] = (639 - tmpLineWidth[j]) / 2;
							_subtitleLineScreenX[j] = CLIP(_subtitleLineScreenX[j], 0, 639 - tmpLineWidth[j]);
						}
						break; // end the for-loop on linesToSplitInto
					}
				} else {
					// the line exceeds max width but has no space characters
					// we treat it as single line quote (it will appear clipped). This won't happen practically though.
					drawSingleLineQuote = true;
					break;  // end the for-loop on linesToSplitInto
				}
			}
		} else { // the width of the line is smaller than the max width
			drawSingleLineQuote = true;
		}
		if (drawSingleLineQuote) {
			_subtitleLineQuote[0] = _currentSubtitleTextFull;
			_subtitleLineScreenX[0] = (639 - wOrig) / 2;
			_subtitleLineScreenX[0] = CLIP(_subtitleLineScreenX[0], 0, 639 - wOrig);
		}
	}
	//debug("calculatePosition: %d %d", w, _screenFirstLineX);
}

/**
* Initialize a few basic member vars
*/
void Subtitles::clear() {
	_isVisible = false;
	_forceShowWhenNoSpeech = false;
	_currentSubtitleTextFull = "";
	for (int i = 0; i < kMaxNumOfSubtitlesLines; ++i) {
		_subtitleLineQuote[i] = "";
		_subtitleLineScreenY[i] = 0;
		_subtitleLineScreenX[i] = 0;
		_subtitleLineSplitAtCharIndex[i] = 0;
	}
	_subtitlesQuoteChanged = true;
	_currentSubtitleLines = 0;
}

/**
* Initialize/ reset member vars, close open file descriptors and garbage collect subtitle fonts and text resource
*/
void Subtitles::reset() {
	clear();

	_subtitlesInfo.credits = "N/A";
	_subtitlesInfo.versionStr = "N/A";
	_subtitlesInfo.dateOfCompile = "N/A";
	_subtitlesInfo.languageMode = "N/A";

	for (int i = 0; i != kMaxTextResourceEntries; ++i) {
		if (_vqaSubsTextResourceEntries[i] != nullptr) {
			delete _vqaSubsTextResourceEntries[i];
			_vqaSubsTextResourceEntries[i] = nullptr;
		}
		_gameSubsResourceEntriesFound[i] = false;
	}

	if (_font != nullptr) {
		delete _font;
		_font = nullptr;
	}
	_useUTF8 = false;
}

} // End of namespace BladeRunner
