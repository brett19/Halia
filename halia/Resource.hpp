#pragma once
#include "SmartPtr.hpp"

namespace Halia
{
	//! A base-class used by the resource manager to load and unload resources on the fly
	class Resource : public SmartPtr
	{
	public:
		//! Constructor
		Resource( );

		//! Destructor
		~Resource( );

		//! Returns whether this resource is loaded
		bool IsLoaded( ) const;

		//! Load this resource
		virtual bool Load( ) = 0;

		//! Unload this resource
		virtual void Unload( ) = 0;

	protected:
		//! Whether the resource is loaded yet
		bool volatile mLoaded;
	};
};