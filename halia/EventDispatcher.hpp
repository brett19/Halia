#pragma once
#include "SLList.hpp"
#include "Events.hpp"
#include "EventListener.hpp"

namespace Halia
{
	//! A base-class used for any object which can dispatch events
	class EventDispatcher
	{
	private:
		//! Internal class for holding listener data
		struct Listener
		{
			int mEventType;
			EventListener* mListener;
		};

	public:
		//! Constructor
		EventDispatcher( );

		//! Destructor
		~EventDispatcher( );

		//! Dispatch an event from this object
		void DispatchEvent( Event* evt );

		//! Add a new listener listening for an event
		void AddListener( EventType::EventType type, EventListener* owner );

	private:
		//! List of all current listeners
		SLList<Listener> mListeners;

	};
};