#pragma once
#include "RenderCached.hpp"

namespace Halia
{
	//! An interface for the vertex buffers
	class VertexBuffer : public RenderCached
	{
	public:
		//! Returns a pointer to the raw buffer
		virtual char* GetBuffer( ) = 0;

		//! Returns the size of the raw buffer
		virtual int GetBufferSize( ) = 0;

		//! Returns the amount of vertices in the buffer
		virtual int GetLength( ) = 0;

		//! Returns the size of each vertex in the buffer
		virtual int GetStride( ) = 0;
	};
};