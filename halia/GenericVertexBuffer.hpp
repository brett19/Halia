#pragma once
#include "VertexBuffer.hpp"

namespace Halia
{
	//! A class to allow generic access to vertex memory
	class GenericVertexBuffer : public VertexBuffer
	{
	public:
		GenericVertexBuffer( int length, int stride )
		{
			mLength = length;
			mStride = stride;
			mData = new char[ mLength * mStride ];
		};

		GenericVertexBuffer( VertexBuffer* original )
		{
			mLength = original->GetLength( );
			mStride = original->GetStride( );
			mData = new char[ mLength * mStride ];
		};

		~GenericVertexBuffer( )
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