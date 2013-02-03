#include "OverlayRoot.hpp"

namespace Halia
{
	OverlayRoot::OverlayRoot( int sx, int sy )
		: Overlay( 0, 0, sx, sy ), mKeyTarget( 0 )
	{
	};

	OverlayRoot::~OverlayRoot( )
	{
	};

	OverlayType OverlayRoot::GetType( ) const
	{
		return OverlayType_ROOT;
	};

	bool OverlayRoot::EventHandler( Event* evt )
	{
		if( evt->mType == EventType::MOUSEDOWN || evt->mType == EventType::MOUSEUP || evt->mType == EventType::MOUSEMOVE ) {
			PosEvent* tevt = (PosEvent*)evt;

			Overlay* ptarget = FindTarget( Point( (int)tevt->mPosition.x, (int)tevt->mPosition.y ) );
			if( ptarget && ptarget != this )
				ptarget->EventHandler( evt );
		} else if( evt->mType == EventType::KEYDOWN || evt->mType == EventType::KEYUP ) {
			if( mKeyTarget )
				mKeyTarget->EventHandler( evt );
		}

		return false;
	};

	void OverlayRoot::Update( )
	{
		return Overlay::Update( );
	};

	void OverlayRoot::Render( )
	{
		return Overlay::Render( );
	};
};