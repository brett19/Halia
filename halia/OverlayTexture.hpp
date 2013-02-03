#pragma once
#include "RenderSystem.hpp"
#include "Texture.hpp"

namespace Halia
{
	//! A class for rendering part of a texture for an Overlay
	class OverlayTexture
	{
	public:
		//! Constructor
		OverlayTexture( Texture* tex, int x, int y, int sx, int sy )
			: mTexture( tex ), mOffset( x, y ), mSize( sx, sy )
		{
			mTexture->AddRef( );
		};

		//! Destructor
		~OverlayTexture( )
		{
			mTexture->DelRef( );
		};

		//! Render this texture to the device
		void Render( int x, int y, int sx, int sy, int color = 0xFFFFFFFF )
		{
			HRS->DrawSprite( x, y, sx, sy, mOffset.x, mOffset.y, mSize.x, mSize.y, mTexture, color );
		};

	private:
		//! Pointer to the texture used by this
		Texture* mTexture;

		//! Offset within the texture to start drawing from
		Point mOffset;

		//! Size of the part of the texture to draw
		Point mSize;
	};
};