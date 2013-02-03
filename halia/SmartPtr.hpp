#pragma once

namespace Halia
{

	//! A base-class for built-in handling of reference counting
	/*!
		This class adds another variable to any class that inherits it, as well as
		some functions.  This allows for you to AddRef and DelRef it.  Once the reference
		count reaches 0 ( initially one upon instantiation ), the object is destructed.
	*/
	class SmartPtr
	{
	public:
		//! Constructor
		inline SmartPtr( )
		{
			mRefCount = 1;
		};

		//! Destructor
		virtual inline ~SmartPtr( )
		{
		};

		//! Add one to the reference count
		inline int AddRef( )
		{
			return mRefCount++;
		};

		//! Remove one from the reference count, and delete if the count is 0
		inline int DelRef( )
		{
			int rc = --mRefCount;
			if( rc == 0 )
				delete this;
			return rc;
		};

	private:
		//! Variable used to count references to this object.
		int volatile mRefCount;
	};
};