#pragma once
#include <vector>
#include "IndexBuffer.hpp"

namespace Halia
{
	//! A class that allows per-index insertion, deletion and access to the index buffer
	template< typename T >
	class TypedIndexBuffer : public IndexBuffer
	{
	public:
		TypedIndexBuffer( )
		{
		};

		~TypedIndexBuffer( )
		{
		};

		void Resize( int size )
		{
			mIndices.resize( size );
		};

		void AddIndex( T idx )
		{
			mIndices.push_back( idx );
		};

		void SetIndex( int i, T idx )
		{
			mVerts[ i ] = idx;
		};

		template< typename U >
		U& GetIndex( int id )
		{
			return ((U*)&mIndices[0])[id];
		};

		T& GetIndex( int id )
		{
			return mIndices[ id ];
		};

		char* GetBuffer( )
		{
			return (char*)&mIndices[0];
		};

		int GetBufferSize( )
		{
			return mIndices.size( ) * sizeof( T );
		};
		
		int GetLength( )
		{
			return mIndices.size( );
		};

		int GetStride( )
		{
			return sizeof( T );
		};

		std::vector< T > mIndices;
	};
};