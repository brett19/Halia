#pragma once
#include "IndexBuffer.hpp"

namespace Halia
{
	//! A class to allow generic access to index memory
	class GenericIndexBuffer : public IndexBuffer
	{
	public:
		GenericIndexBuffer( int length, int stride )
		{
			mLength = length;
			mStride = stride;
			mData = new char[ mLength * mStride ];
		};

		GenericIndexBuffer( IndexBuffer* original )
		{
			mLength = original->GetLength( );
			mStride = original->GetStride( );
			mData = new char[ mLength * mStride ];
		};

		~GenericIndexBuffer( )
		{
			delete[] mData;
		};

		virtual char* GetBuffer( ) const
		{
			return mData;
		};

		virtual int GetBufferSize( ) const
		{
			return mLength * mStride;
		};

		virtual int GetLength( ) const
		{
			return mLength;
		};

		virtual int GetStride( ) const
		{
			return mStride;
		};

	protected:
		char* mData;
		int mLength;
		int mStride;
	};
};