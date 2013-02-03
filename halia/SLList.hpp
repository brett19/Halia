#pragma once
#include "LogManager.hpp"

namespace Halia
{
	//! A list using the single linked-list paradaigm
	/*!
		A list that connects every node with the next node in the
		list.  Allows constant-time insertion at the beginning and
		end; as well as constant-time deletion from the front.
	*/
	template< typename T >
	class SLList
	{
	public:
		//! Defines a single linked-list entry.
		class Entry
		{
			friend SLList;

		protected:
			//! Constructor
			inline Entry( T val, Entry* nxt )
				: value( val ), next( nxt )
			{
			};

			//! The actual value
			T value;

			//! Pointer to the next node in the list
			Entry* next;
		};

		//! An iterator for passing over the single linked-list
		class Iter
		{
			friend SLList;

		protected:
			//! Constructor
			inline Iter( SLList* owner, Entry* start )
				: mOwner( owner ), mValue( start )
			{
			};

		public:
			//! Whether this iterator points to a valid value
			inline bool IsValid( ) const
			{
				return mValue != 0;
			};

			//! Move to the next element in the ArrList
			inline Iter operator++( )
			{
				Iter retval = *this;
				mValue = mValue->next;
				return retval;
			};

			//! Move to the next element in the ArrList
			inline Iter operator++( int )
			{
				mValue = mValue->next;
				return *this;
			};

			//! Returns the element from the array
			inline T& operator*( ) const
			{
				return mValue->value;
			};

			//! Returns a pointer to the element from the array
			inline T* operator->( ) const
			{
				return &mValue->value;
			};

		private:
			//! The owner of this iterator
			SLList* mOwner;

			//! A pointer to the current entry
			Entry* mValue;
		};

		//! Constructor
		inline SLList( )
			: mFirst( 0 ), mLast( 0 )
		{
		};

		//! Destructor
		inline ~SLList( )
		{
			Entry* cur = mFirst;
			Entry* next = cur;
			while( cur ) {
				next = cur->next;
				delete cur;
				cur = next;
			}
		};

		//! Returns an iterator to the start of the array
		inline Iter Begin( )
		{
			return Iter( this, mFirst );
		};

		//! Returns an iterator to the end of the array
		inline Iter End( )
		{
			return Iter( this, mEnd );
		};

		//! Add an entry to the end of the ArrList
		void PushBack( T value )
		{
			Entry* e = new Entry( value, 0 );

			if( !mFirst )
				mFirst = e;
			else
				mLast->next = e;
			mLast = e;
		};

		//! Add an entry to the beginning of the ArrList
		void PushFront( T value )
		{
			Entry* e = new Entry( value, mFirst );

			if( !mLast )
				mLast = e;
			mFirst = e;
		};

		//! Remove an entry from the end of the ArrList
		T PopFront( )
		{
			Entry* cur = mFirst;
			T retval = cur->value;
			mFirst = cur->next;
			delete cur;
			return retval;
		};

	private:
		//! Pointer to the first entry in the list
		Entry* mFirst;

		//! Pointer to the last entry in the list
		Entry* mLast;
	};
};