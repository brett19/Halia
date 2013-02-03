#pragma once
#include "Overlay.hpp"

namespace Halia
{
	class OverlayRoot : public Overlay
	{
	public:
		OverlayRoot( int sx, int sy );
		~OverlayRoot( );

		OverlayType GetType( ) const;

		virtual bool EventHandler( Event* evt );

		virtual void Update( );
		virtual void Render( );
		
	private:
		Overlay* mKeyTarget;
	};
};