#include "Font.hpp"
#include "LogManager.hpp"

namespace Halia
{
	Font::Font( )
		: mTex( 0 )
	{
	};

	Font::~Font( )
	{
		if( mTex ) {
			delete mTex;
			mTex = 0;
		}
	};

	bool Font::Init( std::string fontname, int fontsize, int fontweight, int charsetstart, int charsetend )
	{
		//:: Initialize stuff
		HDC fntdc = CreateCompatibleDC( GetDC( 0 ) );
		HFONT fntobj = CreateFont( -fontsize, 0, 0, 0, fontweight,
			FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY, DEFAULT_PITCH, fontname.c_str( ) );
		SelectObject( fntdc, fntobj );

		TEXTMETRIC metrics;
		GetTextMetrics( fntdc, &metrics );
		mLineHeight = metrics.tmHeight;

		for( int i = 0; i < 256; i++ )
		{
			if( i == ' ' ) {
				GetCharWidth32( fntdc, ' ', ' ', &mChars[i].width );
				mChars[i].flags = 2;
			} else if( i == '\t' ) {
				GetCharWidth32( fntdc, ' ', ' ', &mChars[i].width );
				mChars[i].width *= 4;
				mChars[i].flags = 2;
			} else if( i == '\n' ) {
				mChars[i].width = 0;
				mChars[i].flags = 2;
			} else if( ( i >= charsetstart && i <= charsetend ) || i == '?' ) {
				GetCharWidth32( fntdc, i, i, &mChars[i].width );
				mChars[i].flags = 1;
			} else {
				mChars[i].width = 0;
				mChars[i].flags = 0;
			}
		}

		int texwidth, texheight;
		for( int size = 64; size <= 2048; size *= 2 )
		{
			bool failed = false;
			for( int i = 0, ix = 0, iy = 0; i < 256; i++ )
			{
				if( mChars[i].flags == 1 ) {
					// Check if we overran the texture width
					if( ix + mChars[i].width > size ) {
						ix = 0;
						iy += mLineHeight;
					}
					// Check if we overran the texture height
					if( iy + mLineHeight > size ) {
						failed = true;
						break;
					}

					mChars[i].x = ix;
					mChars[i].y = iy;

					ix += mChars[i].width;
				}
			}
			if( failed )
				continue;

			texwidth = size;
			texheight = size;
			break;
		}

		HBITMAP fntbmp = CreateCompatibleBitmap( GetDC( 0 ), texwidth, texheight );
		SelectObject( fntdc, fntbmp );
		BitBlt( fntdc, 0, 0, texwidth, texheight, 0, 0, 0, WHITENESS );

		for( int i = 0; i < 256; i++ )
		{
			if( mChars[i].flags == 1 ) {
				RECT rect;
				rect.left = mChars[i].x; rect.right = rect.left + mChars[i].width;
				rect.top = mChars[i].y; rect.bottom = rect.top + mLineHeight;
				DrawText( fntdc, (char*)&i, 1, &rect, DT_LEFT | DT_NOPREFIX );
			}
		}

		//:: Create real texture!
		unsigned int picount = texwidth * texheight;
		unsigned int* data = new unsigned int[ picount ];
		int rval = GetBitmapBits( fntbmp, picount * 4, data );
		mTex = new Halia::Texture( texwidth, texheight, Halia::TextureFormat::A8L8 );
		unsigned short* texbuf = (unsigned short*)mTex->GetBuffer( );
		for( unsigned int i = 0; i < picount; i++ )
			*texbuf++ = ~( data[i] << 8 ) | 0x00FF;
		delete data;

		//:: Clean shit up!
		DeleteObject( fntobj );
		DeleteObject( fntbmp );
		DeleteObject( fntdc );

		return true;
	};

	void Font::DrawString( int x, int y, std::string text, unsigned int color ) const
	{
		int ix = x;
		int iy = y;

		for( std::string::iterator i = text.begin( ); i != text.end( ); i++ )
		{
			char c = *i;
			if( c != '\n' ) {
				// If the character doesn't exist, switch to the ? sign
				if( mChars[ c ].flags == 0 )
					c = '?';

				if( mChars[ c ].flags == 1 ) {
					HRS->DrawSprite(
						ix,
						iy,
						mChars[ c ].width,
						mLineHeight,
						mChars[ c ].x,
						mChars[ c ].y,
						mChars[ c ].width,
						mLineHeight,
						mTex,
						color
					);
				}

				ix += mChars[ c ].width;
			} else {
				ix = x;
				iy += mLineHeight;
			}
		}
	};

	Point Font::GetDimensions( std::string text ) const
	{
		int ix = 0;
		int iy = 0;
		for( std::string::iterator i = text.begin( ); i != text.end( ); i++ )
		{
			char c = *i;
			if( c != '\n' ) {
				// If the character doesn't exist, switch to the ? sign
				if( mChars[ c ].flags == 0 )
					c = '?';
				ix += mChars[ c ].width;
			} else {
				ix = 0;
				iy += mLineHeight;
			}
		}

		return Point( ix, iy + mLineHeight );
	};
};