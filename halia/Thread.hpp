#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>

namespace Halia
{
	//! A class for created a classful thread
	/*!
		A class that allows you to inherit this base-class and override Execute.
		Upon calling Resume, the thread will start and process the overwritten
		Execute asynchronously.
	*/
	class Thread
	{
	public:
		//! Constructor
		Thread( bool autoexec = false )
		{
			mRunning = true;
			mActive = false;

			mThread = (HANDLE)_beginthreadex( 0, 0, &_Execute, this, CREATE_SUSPENDED, 0 );

			if( autoexec )
				Resume( );
		};

		//! Destructor
		~Thread( )
		{
			if( mRunning || mActive )
				printf( "Thread deleted prior to ending" );

			CloseHandle( mThread );
		};

		//! Resume execution of the thread
		void Resume( )
		{
			if( mActive )
				return;
			mActive = true;
			ResumeThread( mThread );
		};

		//! Suspend execution of the thread
		void Suspend( )
		{
			if( !mActive )
				return;
			mActive = false;
			SuspendThread( mThread );
		};

		//! Attempt to shut-down the thread
		void Terminate( )
		{
			mRunning = false;
			WaitForSingleObject( mThread, INFINITE );
		};

		//! Returns whether the thread is currently running
		inline bool IsRunning( ) const
		{
			return mRunning;
		};

	private:
		//! Function called from the thread
		virtual int Execute( )
		{
			return 0;
		};

		//! Internal passthru function to call the thread function
		static unsigned int __stdcall _Execute( void* a )
		{
			return ((Thread*)a)->Execute( );
		};

		//! Whether the thread is running or not
		bool mRunning;

		//! Whether the thread is created or destroyed
		bool mActive;

		//! Pointer to the underlying OS thread
		HANDLE mThread;
	};
};