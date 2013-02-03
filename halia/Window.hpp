#pragma once
#include <windows.h>
#include <string>

namespace Halia
{
	//! A class to encapsulate creating and destroying a simple window.  Generally used for rendering.
	/*!
		This class creates a window with the given title,width,height and style and then begins
		processing messages for it, without picking out any in particular except WM_QUIT; which
		will cause Poll to return false so you know when to shut down.
	*/
	class Window
	{
	public:
		//! Constructor
		Window( );

		//! Destructor
		~Window( );

		//! Initialize the window
		/*!
			Create a window with the given properties, and set up the message loop to be processed
			by the Halia::Window::Poll call.
		*/
		bool Init( std::string title, int width, int height, unsigned int style = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_VISIBLE );

		//! A message handler for the window, processes the events coming from the message queue
		bool MessageHandler( UINT iMessage, WPARAM wParam, LPARAM lParam );

		//! Poll the message queue for any new events, returns false if the window is being/has closed.
		bool Poll( );

		//! Get the HWND of the underlying window.
		HWND& GetHwnd( );

	private:
		//! Static WndProc callback that redirects to Window::MessageHandler
		static LRESULT CALLBACK WndProc( HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam );

		//! The HWND of the underlieing window
		HWND mWindow;
	};
};