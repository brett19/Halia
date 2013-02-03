#pragma once
#include <vector>
#include "Point.hpp"
#include "EventListener.hpp"
#include "EventDispatcher.hpp"

namespace Halia
{
	enum OverlayType
	{
		OverlayType_BASE,
		OverlayType_ROOT,
		OverlayType_LABEL,
		OverlayType_IMAGE,
		OverlayType_BUTTON,
		OverlayType_TEXTBOX,
		OverlayType_LISTBOX,
		OverlayType_CUSTOM = 0x100
	};

	//! A base for all Overlay System objects
	/*!
		A base for the Overlay System objects, that acts as a container.  This
		object is not abstract and can be used as a container for other objects.
	*/
	class Overlay : public EventListener, public EventDispatcher
	{
	public:
		//! Constructor
		Overlay( );

		//! Constructor ( w/ Position )
		Overlay( int x, int y );

		//! Constructor ( w/ Position & Size )
		Overlay( int x, int y, int sx, int sy );

		//! Destructor
		~Overlay( );

		//! Adds an overlay as a child to this
		void AddChild( Overlay* o );
		
		//! Removes a previously added child
		void RemoveChild( Overlay* o );

		//! Returns the type of this overlay
		virtual OverlayType GetType( ) const;

		//! Returns the current position of this overlay
		const Point& GetPosition( ) const;

		//! Returns the current size of this overlay
		const Point& GetSize( ) const;

		//! Sets the position of the overlay
		void SetPosition( int x, int y );

		//! Sets the size of the overlay
		void SetSize( int x, int y );

		//! Sets whether this overlay is visible or not
		void SetVisible( bool visible = true );

		//! Checks if a point touches this object ( in parent-space )
		virtual bool TestPoint( const Point& p ) const;
		
		//! Finds the highest level object under the given point
		Overlay* FindTarget( const Point& p );

		//! Centers the designated overlay within this overlay
		void CenterChild( Overlay* child ) const;
		
		//! Handles incoming Events from the Event System
		virtual bool EventHandler( Event* evt );

		//! Update this overlay ( Absolute Position and such )
		virtual void Update( );

		//! Render this overlay
		virtual void Render( );

	protected:
		//! The parent of this overlay
		Overlay* mParent;

		//! A list of the children attached to this overlay
		std::vector< Overlay* > mChildren;

		//! The position of this overlay
		Point mPosition;

		//! The size of this overlay
		Point mSize;

		//! The absolute ( screen-space ) position of this object
		Point mGPosition;

		//! Whether this overlay is visible or not
		bool mVisible;
	};
};