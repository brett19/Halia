#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "LogManager.hpp"

namespace Halia
{
	//! A list using contiguous memory.
	/*!
		A list using a contiguous block of memory allowing for quick 
		insertion and deletion from the end of the list and fast 
		iteration.  Also allows for passing a pointer to the base memory
		to functions not under your control.
	*/
	template< typename T >
	class ArrList
	{
	public:
		//! An iterator for passing over the ArrList
		class Iter
		{
			friend ArrList;

		protected:
			//! Constructor
			inline Iter( ArrList* owner, int spos )
				: mOwner( owner ), mPos( spos )
			{
			};

		public:
			//! Whether this iterator points to a valid value
			inline bool IsValid( ) const
			{
				return mPos >= 0 && mPos < mOwner->mSize;
			};

			//! Move to the next element in the ArrList
			inline Iter operator++( )
			{
				Iter retval = *this;
				mPos++;
				return retval;
			};

			//! Move to the next element in the ArrList
			inline Iter operator++( int ) {
				mPos++;
				return *this;
			};

			//! Move to the previous element in the ArrList
			inline Iter operator--( )
			{
				Iter retval = *this;
				mPos--;
				return retval;
			};

			//! Move to the previous element in the ArrList
			inline Iter operator--( int ) {
				mPos--;
				return *this;
			};

			//! Returns the element from the array
			inline T& operator*( ) const
			{
				return mOwner->mList[ mPos ];
			};

			//! Returns a pointer to the element from the array
			inline T* operator->( ) const
			{
				return &mOwner->mList[ mPos ];
			};

		private:
			//! The owner of this iterator
			ArrList* mOwner;

			//! The current position within the ArrList
			int mPos;
		};

		//! Constructor
		inline ArrList( )
			: mList( 0 ), mRealSize( 0 ), mSize( 0 )
		{
			Reserve( );
		};

		//! Destructor
		inline ~ArrList( )
		{
			if( mList ) {
				delete[] mList;
				mList = 0;
			}
		};

		//! Returns an iterator to the start of the array
		inline Iter Begin( )
		{
			return Iter( this, 0 );
		};

		//! Returns an iterator to the end of the array
		inline Iter End( )
		{
			return Iter( this, mSize - 1 );
		};

		//! Double the reserved entry space allocation
		void Reserve( )
		{
			int size = mRealSize * 2;
			if( size == 0 )
				size = 100;

			T* newlist = new T[ size ];
			if( mSize > 0 )
				memcpy( newlist, mList, mRealSize * sizeof( T ) );
			delete[] mList;

			mList = newlist;
			mRealSize = size;
		};

		//! Increase the reserved entry space allocation
		void Reserve( int size )
		{
			if( mRealSize > size )
				return;

			T* newlist = new T[ size ];
			if( mSize > 0 )
				memcpy( newlist, mList, mRealSize * sizeof( T ) );
			delete[] mList;

			mList = newlist;
			mRealSize = size;
		};

		//! Clear the ArrList of all entries
		inline void Clear( )
		{
			mSize = 0;
		};

		//! Add an entry to the end of the ArrList
		inline void PushBack( T value )
		{
			if( mSize >= mRealSize )
				Reserve( );

			mList[ mSize++ ] = value;
		};

		//! Remove an entry from the end of the ArrList
		inline T PopBack( )
		{
			return mList[ --mSize ];
		};

		//! Returns the current entry count
		inline int Size( )
		{
			return mSize;
		};

	private:
		//! Pointer to the memory used internally
		T* mList;

		//! The current entry count
		int mSize;

		//! The reserved entry size
		int mRealSize;
	};
};