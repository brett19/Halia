#pragma once
#include <windows.h>
#include <OISInputManager.h>
#include <OISException.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISJoyStick.h>
#include <OISEvents.h>
#include <sstream>
#include "InputConstants.hpp"
#include "Events.hpp"
#include "EventDispatcher.hpp"
#include "EventListener.hpp"
#include "Point.hpp"

namespace Halia
{
	//! A class that allows simple processing of mouse and keyboard input
	class InputManager : public EventDispatcher, public OIS::KeyListener, public OIS::MouseListener
	{
	public:
		//! Constructor
		InputManager( );

		//! Destructor
		~InputManager( );

		//! Initialize the input manager
		bool Init( HWND hwnd, int width, int height );

		//! Check for new events
		bool Poll( );

		//! Returns whether a key is down
		bool GetKeyDown( int keycode );

		//! Returns the current position of the mouse
		Halia::Point GetAbsMousePos( );

		//! Returns the movement of the mouse since the last update
		Halia::Point GetRelMousePos( );

		//! Returns if the left mouse-button is down
		bool GetMouseDownLeft( );

		//! Returns if the right mouse-button is down
		bool GetMouseDownRight( );

	private:
		void handleNonBufferedKeys( );
		void handleNonBufferedMouse( );
		bool keyPressed( const OIS::KeyEvent& arg );
		bool keyReleased( const OIS::KeyEvent& arg );
		bool mouseMoved( const OIS::MouseEvent& arg );
		bool mousePressed( const OIS::MouseEvent& arg, OIS::MouseButtonID id );
		bool mouseReleased( const OIS::MouseEvent& arg, OIS::MouseButtonID id );

		OIS::InputManager* mInputManager;
		OIS::Keyboard* mKeyboard;
		OIS::Mouse* mMouse;
		unsigned int mModifiers;
	};

	//! Singleton Instance of the Input Manager
	extern InputManager* IMInstance;
};
#define HIM Halia::IMInstance