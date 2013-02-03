#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace Halia
{
	namespace Sync
	{
		//! A class to allow singular access to a code-section
		/*!
			This class can be used to allow multiple threads to access shared
			resources in a controlled manner to prevent multi-threading issues.
			Only one thread may process a locked section at a time.
		*/
		class CriticalSection
		{
		public:
			//! Constructor
			inline CriticalSection( )
			{
				InitializeCriticalSection( &mCs );
			};

			//! Destructor
			inline ~CriticalSection( )
			{
				DeleteCriticalSection( &mCs );
			};

			//! Attempt to enter the Critical Section.
			inline bool TryEnter( )
			{
				return TryEnterCriticalSection( &mCs ) == 0;
			};

			//! Attempt to enter the Critical Section, wait until success
			inline void Enter( )
			{
				EnterCriticalSection( &mCs );
			};

			//! Exit the Critical Section
			inline void Leave( )
			{
				LeaveCriticalSection( &mCs );
			};

		private:
			//! Internal pointer to the Critical Section
			CRITICAL_SECTION mCs;
		};
	};
};