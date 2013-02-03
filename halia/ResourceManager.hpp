#pragma once
#include <queue>
#include "Event.hpp"
#include "Thread.hpp"
#include "CriticalSection.hpp"
#include "Resource.hpp"
#include "ResourceGroup.hpp"

namespace Halia
{
	//! A resource manager for handling loading of resources in the background
	class ResourceManager : public Thread
	{
	public:
		//! Constructor
		ResourceManager( );

		//! Destructor
		~ResourceManager( );

		//! Begin checking and loading resources
		int Execute( );

		//! Add a resource to be loaded
		void QueueResource( Resource* res );

	protected:
		//! List of queued resources to be loaded
		std::queue< Resource* > mQueue;

		//! Whether there is a resource to be loaded
		Sync::Event mHasQueued;

		//! Lock to control access to the queue
		Sync::CriticalSection mQueueLock;
	};

	//! Singleton Instance of the Resource Manager
	extern ResourceManager* RMInstance;
};