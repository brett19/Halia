#include "EventDispatcher.hpp"

namespace Halia
{
	EventDispatcher::EventDispatcher( )
	{
	};

	EventDispatcher::~EventDispatcher( )
	{
	};

	void EventDispatcher::DispatchEvent( Event* evt )
	{
		for( SLList<Listener>::Iter i = mListeners.Begin( ); i.IsValid( ); i++ )
		{
			if( (*i).mEventType == evt->mType || (*i).mEventType == EventType::ALL )
				(*i).mListener->EventHandler( evt );
		}
	};

	void EventDispatcher::AddListener( EventType::EventType type, EventListener* handler )
	{
		Listener lstnr = { type, handler };
		mListeners.PushBack( lstnr );
	};
};