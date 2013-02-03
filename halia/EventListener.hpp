#pragma once
#include "Events.hpp"

namespace Halia
{
	//! A base class used for all objects that can process events from EventDispatcher's
	class EventListener
	{
	public:
		//! Constructor
		EventListener( );

		//! Destructor
		~EventListener( );

		//! Virtual function that handles incoming events
		virtual bool EventHandler( Event* evt ) = 0;

	};
};