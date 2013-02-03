#pragma once
#include <string>
#include "Halia.hpp"
#include "LogManager.hpp"
#include "FileSystemManager.hpp"
#include "Network.hpp"
#include "ResourceManager.hpp"
#include "InputManager.hpp"
#include "RenderSystem.hpp"
#include "Window.hpp"
#include "Math.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "DX9RenderSystem.hpp"

namespace Halia
{
	//! Sample application to aid in boosting initial developement
	class HaliaApp
	{
	public:
		//! Constructor
		HaliaApp( );

		//! Destructor
		~HaliaApp( );

		//! Initialize the application
		bool Init( RendererType::RendererType rtype = RendererType::DX9, bool wndauto = true, std::string wndtitle = "Halia Engine", int width = 1024, int height = 768 );
		
		//! Destroy the application, close all resources
		void Destroy( );

		//! Begin rendering
		void StartRendering( );

		//! Force the application to shut down
		void Quit( );

		//! Event called prior to frame rendering
		virtual void PreFrame( ) = 0;

		//! Event called after frame rendering
		virtual void Frame( ) = 0;

	private:
		//! The type of renderer used by this application
		RendererType::RendererType mRType;

		//! A window used for rendering
		Window* mWindow;

		//! The amount of frames rendered since the last FPS refresh
		int mFrameCount;

		//! The total batches ( Draw* calls ) in mFrameCount frames
		int mBatchCount;

		//! The total primitives renderered in mFrameCount frames
		int mPrimCount;

		//! The time the application started
		int mStartTime;

		//! Variables for cacheing FPS display information
		char mSFps[6][60];
	};
};