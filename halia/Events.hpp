#pragma once
#include <string>
#include "Vector3.hpp"
#include "SmartPtr.hpp"
#include "InputConstants.hpp"

namespace Halia
{
	class EventDispatcher;

	namespace EventType
	{
		enum EventType
		{
			KEYDOWN,		// [KeyEvent] Overlay
			KEYUP,			// [KeyEvent] Overlay
			MOUSEDOWN,		// [PosEvent] Overlay
			MOUSEMOVE,		// [PosEvent] Overlay
			MOUSEUP,		// [PosEvent] Overlay
			MOUSEOVER,		// [Event] Overlay
			MOUSEOUT,		// [Event] Overlay

			CLICK,			// [PosEvent] Overlay, 3DObject
			GETFOCUS,		// [Event] Overlay
			LOSTFOCUS,		// [Event] Overlay
			SELECTED,		// [Event] OverlayComboItem, OverlayListItem
			DESELECTED,		// [Event] OverlayComboItem, OverlayListItem
			VALUECHANGED,	// [Event] OverlayTextBox, OverlayComboBox, OverlayListItem
			GOTDATA,		// [Event] Networking
			CONNECTED,		// [Event] Networking
			DISCONNECTED,	// [Event] Networking
			ERR,			// [ErrorEvent] *

			ALL = 0xFF,
			CUSTOM = 0x100	// [??] *
		};
	};

	//! Defines the basic properties for all events
	struct Event : public SmartPtr
	{
		//! Constructor
		Event( EventType::EventType type, EventDispatcher* target )
			: mType( type ), mTarget( target )
		{
		};

		//! The type of event
		EventType::EventType mType;

		//! The target of the event
		EventDispatcher *mTarget;
	};

	//! Defines an event which inclues a keycode
	struct KeyEvent : public Event
	{
		//! Constructor
		KeyEvent( EventType::EventType type, EventDispatcher* target, KeyCode::KeyCode key )
			: Event( type, target ), mKey( key )
		{
		};

		//! Keycode of the events key
		KeyCode::KeyCode mKey;
	};

	//! Defines an event which includes a position
	struct PosEvent : public Event
	{
		//! Constructor
		PosEvent( EventType::EventType type, EventDispatcher* target, Vector3 position )
			: Event( type, target ), mPosition( position )
		{
		};

		//! Position of the event
		Vector3 mPosition;
	};

	//! Defines an event which includes an error code and text
	struct ErrorEvent : public Event
	{
		//! Constructor
		ErrorEvent( EventType::EventType type, EventDispatcher* target, int code, const std::string& text )
			: Event( type, target ), mCode( code ), mText( text )
		{
		};

		//! Constructor
		ErrorEvent( EventType::EventType type, EventDispatcher* target, int code )
			: Event( type, target ), mCode( code ), mText( "[None Specified]" )
		{
		};

		//! Error code of the event
		int mCode;

		//! Optional error text of the event
		std::string mText;
	};
};