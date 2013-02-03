#pragma once
#include "Overlay.hpp"
#include "OverlayTexture.hpp"

namespace Halia
{
	//! An overlay class for rendering an OverlayTexture
	/*!
		An overlay class used to allow a developer to easily render an
		OverlayTexture within the normal Overlay pipeline.
	*/
	class OverlayImage : public Overlay
	{
	public:
		//! Constructor ( w/ Texture )
		OverlayImage( Halia::OverlayTexture* tex );

		//! Constructor ( w/ Texture & Position )
		OverlayImage( Halia::OverlayTexture* tex, int x, int y );

		//! Constructor ( w/ Texture, Position & Size )
		OverlayImage( Halia::OverlayTexture* tex, int x, int y, int sx, int sy );

		//! Destructor
		~OverlayImage( );

		//! Returns the type of this overlay
		OverlayType GetType( ) const;

		//! Handles events
		virtual bool EventHandler( Event* evt );

		//! Updates this overlay
		virtual void Update( );

		//! Renders the attached image
		virtual void Render( );
		
	private:
		//! Reference to the OverlayTexture to render
		Halia::OverlayTexture* mTex;
	};
};