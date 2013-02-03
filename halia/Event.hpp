#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>

namespace Halia
{
	namespace Sync
	{
		//! Class to allow waiting and triggering
		/*!
			This class allows you to hold threads until the flag on the
			event is set.  Multiple-threads can be help waiting with a 
			call to Wait.  Once anything sets the event.  All waiting threads
			are released.
		*/
		class Event
		{
		public:
			//! Constructor
			inline Event( )
			{
				mEvent = CreateEvent( 0, 1, 0, 0 );
			};

			//! Destructor
			inline ~Event( )
			{
				CloseHandle( mEvent );
			};

			//! Trigger the event
			inline void Set( )
			{
				SetEvent( mEvent );
			};

			//! Reset the value of the event
			inline void Reset( )
			{
				ResetEvent( mEvent );
			};

			//! Hold the thread until the event is triggered
			inline void WaitFor( )
			{
				WaitForSingleObject( mEvent, INFINITE );
			};

		private:
			//! Internal pointer to the event
			HANDLE mEvent;
		};
	};
};