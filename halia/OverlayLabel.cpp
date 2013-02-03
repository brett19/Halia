#include "OverlayLabel.hpp"

namespace Halia
{
	OverlayLabel::OverlayLabel( std::string text )
		: mText( text )
	{
	};

	OverlayLabel::OverlayLabel( std::string text, int x, int y )
		: mText( text ), Overlay( x, y )
	{
	};

	OverlayLabel::OverlayLabel( std::string text, int x, int y, int sx, int sy )
		: mText( text ), Overlay( x, y, sx, sy )
	{
	};

	OverlayLabel::~OverlayLabel( )
	{
	};

	OverlayType OverlayLabel::GetType( ) const
	{
		return OverlayType_LABEL;
	};

	void OverlayLabel::Update( )
	{
		return Overlay::Update( );
	};

	void OverlayLabel::Render( )
	{
		return Overlay::Render( );
	};
};