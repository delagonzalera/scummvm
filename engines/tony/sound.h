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
 * This code is based on original Tony Tough source code
 *
 * Copyright (c) 1997-2003 Nayma Software
 */

#ifndef TONY_SOUND_H
#define TONY_SOUND_H

#include "audio/mixer.h"
#include "common/file.h"
#include "tony/gfxcore.h"
#include "tony/loc.h"
#include "tony/utils.h"

namespace Audio {
class RewindableAudioStream;	
}

namespace Tony {

class FPStream;
class FPSfx;

enum SoundCodecs {
	FPCODEC_RAW,
	FPCODEC_ADPCM
};


/****************************************************************************\
*****************************************************************************
*       class FPSound
*       -------------
* Description: Sound driver per Falling Pumpkins
*****************************************************************************
\****************************************************************************/

class FPSound {

private:

	bool _bSoundSupported;

	/****************************************************************************\
	*       Methods
	\****************************************************************************/

public:

	/****************************************************************************\
	*
	* Function:     FPSound::FPSound();
	*
	* Description:  Default constructor. Initializes the attributes
	*
	\****************************************************************************/

	FPSound();

	/****************************************************************************\
	*
	* Function:     FPSound::~FPSound();
	*
	* Description:  Deinitialize the object, free memory
	*
	\****************************************************************************/

	~FPSound();

	/****************************************************************************\
	*
	* Function:     bool FPSound::Init();
	*
	* Description:  Initializes the objects, and prepare everything required to 
	*               create streams and sound effects.
	*
	* Return:       True if everything is OK, False otherwise.
	*
	\****************************************************************************/

	bool init();

	/****************************************************************************\
	*
	* Function:     bool CreateStream(FPStream** lplpStream);
	*
	* Description:  Allocates an object of type FPStream, and return its 
	*               pointer after it has been initialized.
	*
	* Input:        FPStream** lplpStream   Will contain the pointer of the 
	*                                       object
	*
	* Return:       True is everything i OK, False otherwise
	*
	* Note:         The use of functions like CreateStream () and CreateSfx () 
	*               are due to the fact that the class constructors and 
	*               FPStream FPSfx require that DirectSound is already initialized. 
	*               In this way, you avoid the bugs that would be created if an 
	*               object type is declared FPStream FPSfx or global 
	*               (or anyway before initializing DirectSound).
	\****************************************************************************/

	bool createStream(FPStream **lplpStream);

	/****************************************************************************\
	*
	* Function:     bool CreateSfx(FPSfx** lplpSfx);
	*
	* Description:  Allocates an object of type FPSfx and returns a pointer 
	*               pointing to it
	*
	* Input:        FPSfx** lplpSfx         Will contain the pointer of the 
	*                                       object
	*
	* Return:       True is everything i OK, False otherwise
	*
	* Note:         See notes about CreateStream()
	*
	\****************************************************************************/

	bool createSfx(FPSfx **lplpSfx);

	/****************************************************************************\
	*
	* Function:     void SetMasterVolume(int dwVolume);
	*
	* Description:  Set main volume
	*
	* Input:        int dwVolume          Volume to be set (0-63)
	*
	\****************************************************************************/

	void setMasterVolume(int dwVolume);

	/****************************************************************************\
	*
	* Function:     void GetMasterVolume(LPINT lpdwVolume);
	*
	* Description:  Get main volume
	*
	* Input:        LPINT lpdwVolume        This variable will contain the 
	*                                       current volume (0-63)
	*
	\****************************************************************************/

	void getMasterVolume(int *lpdwVolume);
};

class FPSfx {

	/****************************************************************************\
	*       Attributes
	\****************************************************************************/

private:
	bool _bSoundSupported;                 // True if the sound is active
	bool _bFileLoaded;                     // True is a file is opened
	bool _bLoop;                           // True is sound effect should loop
	int  _lastVolume;

	bool _bIsVoice;
	bool _bPaused;

	Audio::AudioStream *_loopStream;
	Audio::RewindableAudioStream *_rewindableStream;
	Audio::SoundHandle _handle;

public:
	uint32 _hEndOfBuffer;

private:


	/****************************************************************************\
	*       Methods
	\****************************************************************************/

public:
	/**
	 * Check process for whether sounds have finished playing
	 */
	static void soundCheckProcess(CORO_PARAM, const void *param);

	/****************************************************************************\
	*
	* Function:     FPSfx(bool bSoundOn);
	*
	* Description:  Default constructor. *DO NOT* declare the object directly,
	*               create it though FPSound::CreateSfx() instead
	*
	\****************************************************************************/

	FPSfx(bool bSoundOn);

	/****************************************************************************\
	*
	* Function:     ~FPSfx();
	*
	* Description:  Default destructor. It also stops the sound effect that 
	*               may be running, and free the memory used.
	*
	\****************************************************************************/

	~FPSfx();

	/****************************************************************************\
	*
	* Function:     release();
	*
	* Description:  Releases the memory object. Must be called when the object 
	*               is no longer useful and **ONLY** when the object was created 
	*               with the FPSound :: CreateStream ().
	*
	* Note:         Any object pointers are no longer valid after this call.
	*
	\****************************************************************************/

	void release();

	/****************************************************************************\
	*
	* Function:     bool loadFile(char *lpszFileName, uint32 dwCodec=FPCODEC_RAW);
	*
	* Description:  Opens a file and load sound effect
	*
	* Input:        char *lpszFile          SFX filename
	*               uint32 dwCodec          CODEC to be used to decompress
	*                                       the sound samples
	*
	* Return:       True if everything is OK, False otherwise
	*
	\****************************************************************************/

	bool loadFile(const char *lpszFileName, uint32 dwCodec = FPCODEC_RAW);
	bool loadWave(Common::SeekableReadStream *stream);
	bool loadVoiceFromVDB(Common::File &vdbFP);

	/****************************************************************************\
	*
	* Function:     bool play();
	*
	* Description:  Play the loaded FX.
	*
	* Return:       True if everything is OK, False otherwise
	*
	\****************************************************************************/

	bool play();

	/****************************************************************************\
	*
	* Function:     bool stop();
	*
	* Description:  Stop a FX
	*
	* Return:       True if everything is OK, False otherwise
	*
	\****************************************************************************/

	bool stop();

	/****************************************************************************\
	*
	* Function:     void pause(bool bPause);
	*
	* Description:  Pause a FX
	*
	\****************************************************************************/

	void pause(bool bPause);

	/****************************************************************************\
	*
	* Function:     bool setLoop(bool bLoop);
	*
	* Description:  Enables or disables SFX loop
	*
	* Input:        bool bLoop              True to activate the loop, else False
	*
	* Note:         The loop must be activated before the SFX starts to play, 
	*               else the effect will only be noticable next time the SFX is 
	*               played
	*
	\****************************************************************************/

	void setLoop(bool bLoop);

	/****************************************************************************\
	*
	* Function:     void setVolume(int dwVolume);
	*
	* Description:  Set SFX Volume
	*
	* Input:        int dwVolume            Volume to set (0-63)
	*
	\****************************************************************************/

	void setVolume(int dwVolume);

	/****************************************************************************\
	*
	* Function:     void getVolume(int * lpdwVolume);
	*
	* Description:  Get SFX volume
	*
	* Input:        int * lpdwVolume        Will contain the current volume
	*
	\****************************************************************************/

	void getVolume(int *lpdwVolume);

	/**
	 * Returns true if the sound has finished playing
	 */
	bool endOfBuffer() const;
};

class FPStream {

	/****************************************************************************\
	*       Attributes
	\****************************************************************************/

private:

	/*
	    HWND hwnd;
	    LPDIRECTSOUND lpDS;
	    LPDIRECTSOUNDBUFFER lpDSBuffer;    // DirectSound circular buffer
	    LPDIRECTSOUNDNOTIFY lpDSNotify;    // Notify hotspots in the buffer
	*/
	byte  *_lpTempBuffer;                  // Temporary buffer use for decompression

	uint32 _dwBufferSize;                  // Buffer size (bytes)
	uint32 _dwSize;                        // Stream size (bytes)
	uint32 _dwCodec;                       // CODEC used

	HANDLE _hThreadEnd;                    // Event used to close thread
	Common::File _file;                    // File handle used for the stream
	HANDLE _hPlayThread;                   // Handle of the Play thread
	HANDLE _hHot1, _hHot2, _hHot3;         // Events set by DirectSoundNotify
	HANDLE _hPlayThreadPlayFast;
	HANDLE _hPlayThreadPlayNormal;

	bool _bSoundSupported;                 // True if the sound is active
	bool _bFileLoaded;                     // True if the file is open 
	bool _bLoop;                           // True if the stream should loop
	bool _bDoFadeOut;                      // True if fade out is required
	bool _bSyncExit;
	bool _bPaused;
	int  _lastVolume;
	FPStream *_syncToPlay;
//	DSBPOSITIONNOTIFY dspnHot[3];

	bool createBuffer(int nBufSize);

public:
	bool _bIsPlaying;                      // True if the stream is playing

private:

	static void playThread(FPStream *This);

	/****************************************************************************\
	*       Methods
	\****************************************************************************/

public:

	/****************************************************************************\
	*
	* Function:     FPStream(bool bSoundOn);
	*
	* Description:  Default contractor. *DO NOT* declare the object directly: use
	*                                   FPSound::CreateStream() indtead
	*
	\****************************************************************************/

	FPStream(bool bSoundOn);

	/****************************************************************************\
	*
	* Function:     ~FPStream();
	*
	* Description:  Destructor by default. Stops the playing stream (if any) and
	*                                      frees the memory used by them
	*
	\****************************************************************************/

	~FPStream();

	/****************************************************************************\
	*
	* Function:     release();
	*
	* Description:  Releases memory used by object. Must be used when the object 
	*               is no longer used. *ONLY*�for objects created by 
	*               FPSound::CreateStream().
	*
	* Note:         Object pointers are no longer valid after this call.
	*
	\****************************************************************************/

	void release();

	/****************************************************************************\
	*
	* Function:     bool loadFile(char *lpszFileName, uint32 dwCodec=FPCODEC_RAW);
	*
	* Description:  Open a file for a stream.
	*
	* Input:        char *lpszFile          Filename to be opened
	*               uint32 dwCodec          CODEC to be used to decompress
	*                                       sound samples
	*
	* Return:       True if everything is OK, False otherwise
	*
	\****************************************************************************/

	bool loadFile(const char *lpszFileName, uint32 dwCodec = FPCODEC_RAW, int nSync = 2000);

	/****************************************************************************\
	*
	* Function:     unloadFile();
	*
	* Description:  Close a file stream (if opened). This function must be
	*               called to free the memory used by the stream
	*
	* Return:       Just to be sure, the destructor of this class calls
	*               UnloadFile() if it has not been mentioned explicitly
	*
	\****************************************************************************/

	bool unloadFile();

	/****************************************************************************\
	*
	* Function:     bool play();
	*
	* Description:  Play the loaded stream.
	*
	* Return:       True if everything is OK, False otherwise
	*
	\****************************************************************************/

	bool play();
	void playFast(void);
	void prefetch(void);

	/****************************************************************************\
	*
	* Function:     bool stop();
	*
	* Description:  Stops the play of the stream.
	*
	* Return:       True if everything is OK, False otherwise
	*
	\****************************************************************************/

	bool stop(bool bSync = false);
	void waitForSync(FPStream *toplay);

	/****************************************************************************\
	*
	* Function:     void pause(bool bPause);
	*
	* Description:  Pause sound effect
	*
	\****************************************************************************/

	void pause(bool bPause);

	/****************************************************************************\
	*
	* Function:     bool setLoop(bool bLoop);
	*
	* Description:  Enable of disable stream loop
	*
	* Input:        bool bLoop              True to enable loop, false otherwise
	*
	* Note:         The loop must be activated BEFORE you play back the stream. 
	*               Any changes made during play will not have 'effect until
	*               the stream is not stopped, and then comes back into play.
	*
	\****************************************************************************/

	void setLoop(bool bLoop);

	/****************************************************************************\
	*
	* Function:     void setVolume(int dwVolume);
	*
	* Description:  Change stream colume
	*
	* Input:        int dwVolume            Volume to be set (0-63)
	*
	\****************************************************************************/

	void setVolume(int dwVolume);

	/****************************************************************************\
	*
	* Function:     void getVolume(LPINT lpdwVolume);
	*
	* Description:  Get stream volume
	*
	* Input:        LPINT lpdwVolume        Will contain the current stream volume
	*
	\****************************************************************************/

	void getVolume(int *lpdwVolume);
};

} // End of namespace Tony

#endif
