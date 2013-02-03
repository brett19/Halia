#pragma once
#include "RenderCache.hpp"

namespace Halia
{
	//! A base-class used to represent any object which will be cached by the render-system
	class RenderCached
	{
	public:
		RenderCached( );
		virtual ~RenderCached( );
		bool IsDirty( ) const;
		void SetDirty( bool dirty = true );

		RenderCache* mCache;

	private:
		bool mDirty;
	};
};