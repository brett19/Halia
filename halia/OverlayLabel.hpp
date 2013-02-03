#pragma once
#include <string>
#include "Overlay.hpp"

namespace Halia
{
	class OverlayLabel : public Overlay
	{
	public:
		OverlayLabel( std::string text );
		OverlayLabel( std::string text, int x, int y );
		OverlayLabel( std::string text, int x, int y, int sx, int sy );
		~OverlayLabel( );

		OverlayType GetType( ) const;

		virtual void Update( );
		virtual void Render( );
		
	private:
		std::string mText;
	};
};