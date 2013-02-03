#include "OverlayImage.hpp"

namespace Halia
{
	OverlayImage::OverlayImage( Halia::OverlayTexture* tex )
		: mTex( tex )
	{
	};

	OverlayImage::OverlayImage( Halia::OverlayTexture* tex, int x, int y )
		: mTex( tex ), Overlay( x, y )
	{
	};

	OverlayImage::OverlayImage( Halia::OverlayTexture* tex, int x, int y, int sx, int sy )
		: mTex( tex ), Overlay( x, y, sx, sy )
	{
	};

	OverlayImage::~OverlayImage( )
	{
	};

	OverlayType OverlayImage::GetType( ) const
	{
		return OverlayType_IMAGE;
	};

	bool OverlayImage::EventHandler( Event* evt )
	{
		return false;
	};

	void OverlayImage::Update( )
	{
		return Overlay::Update( );
	};

	void OverlayImage::Render( )
	{
		mTex->Render( mGPosition.x, mGPosition.y, mSize.x, mSize.y );
		return Overlay::Render( );
	};
};