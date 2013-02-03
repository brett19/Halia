#pragma once
#include "RenderCached.hpp"

namespace Halia
{
	//! An interface for the index buffers
	class IndexBuffer : public RenderCached
	{
	public:
		//! Returns a pointer to the raw buffer
		virtual char* GetBuffer( ) = 0;

		//! Returns the size of the raw buffer
		virtual int GetBufferSize( ) = 0;

		//! Returns the amount of indices in the buffer
		virtual int GetLength( ) = 0;

		//! Returns the size of each index in the buffer
		virtual int GetStride( ) = 0;
	};
};