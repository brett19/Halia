#include "RenderCached.hpp"

namespace Halia
{
	RenderCached::RenderCached( )
		: mDirty( true ), mCache( 0 )
	{
	};

	RenderCached::~RenderCached( )
	{
		if( mCache )
			delete mCache;
	};

	bool RenderCached::IsDirty( ) const
	{
		return mDirty | !mCache;
	};

	void RenderCached::SetDirty( bool dirty )
	{
		mDirty = dirty;
	};
};