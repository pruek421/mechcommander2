/*************************************************************************************************\
PauseWindow.h			: Interface for the PauseWindow component.
//---------------------------------------------------------------------------//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//
\*************************************************************************************************/

//*************************************************************************************************

#pragma once

#ifndef PAUSEWINDOW_H
#define PAUSEWINDOW_H

//#include "utilities.h"
//#include <mechgui/afont.h>

/**************************************************************************************************
CLASS DESCRIPTION
PauseWindow:
**************************************************************************************************/

class ControlButton;
struct ButtonData;
class StaticInfo;
class FitIniFile;

struct MoveInfo
{
	float time; // in seconds
	float position; // x pixels
};


class PauseWindow
{
	public:
		
		PauseWindow();
		~PauseWindow();

		void update();
		void render();

		void init( FitIniFile& file );

		void handleClick( int ID );

		bool inRect( int mouseX, int mouseY );

		void begin(){ currentTime = 0.f; }
		void end();
		void begin(bool objectivesOn);

		enum PAUSEBUTTONS
		{
			OBJECTIVES = 0,
			SAVE = 1,
			LOAD = 2,
			OPTIONS = 3,
			RESTART = 4,
			EXIT = 5,
			RETURN = 6
		};

	private:

		ControlButton* buttons;
		ButtonData* buttonData;
		int32_t		buttonCount;
		StaticInfo* statics;
		int32_t		staticCount;
		RECT	backgrounds[2];

		float		finalReference;
		float		currentPos;
		float		currentTime;

		aFont	font;
		aFont	headerFont;

		bool wasDragging;
		bool objectivesAlreadyOn;

		static MoveInfo moveInfo[8];

		bool		bPromptToQuit;
		bool		bPromptToAbort;

};


//*************************************************************************************************
#endif  // end of file ( PauseWindow.h )
