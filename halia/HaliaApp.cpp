#include "HaliaApp.hpp"

namespace Halia
{
	HaliaApp::HaliaApp( )
		: mWindow( 0 )
	{
		strcpy( mSFps[0], "FPS: [CALCULATING]" );
		strcpy( mSFps[1], "Average Prim Count: [CALCULATING]" );
		strcpy( mSFps[2], "Average Batch Count: [CALCULATING]" );
	};

	HaliaApp::~HaliaApp( )
	{
	};

	bool HaliaApp::Init( RendererType::RendererType rtype, bool wndauto, std::string wndtitle, int width, int height )
	{
		//:: Initialize render window module
		mWindow = new Window( );
		mWindow->Init( wndtitle, width, height );
		HLOGMSG( "Render Window created" );

		Halia::InitRenderer( rtype, mWindow->GetHwnd( ), width, height, false );

		// * Good loadin boyz
		return true;
	};

	void HaliaApp::Destroy( )
	{
		delete mWindow;
	};

	void HaliaApp::StartRendering( )
	{
		//:: Initialize frame rate counters
		mFrameCount = 0;
		mBatchCount = 0;
		mPrimCount = 0;
		mStartTime = GetTickCount( );

		//:: Start main render-loop
		while( 1 )
		{
			//:: Process window events
			if( !mWindow->Poll( ) )
				break;
			IMInstance->Poll( );

			//:: Call preframe listener
			PreFrame( );

			//:: Clear the scene
			RSInstance->Clear( Color( 0xFFFFFFFF ) );
			RSInstance->BeginScene( );

			//:: Call frame listener
			Frame( );

			//:: Render the overlays
			//ORInstance->DrawString( 10, 10, mSFps[0], 0xFF888888 );
			//ORInstance->DrawString( 10, 28, mSFps[1], 0xFF888888 );
			//ORInstance->DrawString( 10, 46, mSFps[2], 0xFF888888 );
			//ORInstance->Render( );

			//:: Finish rendering
			RSInstance->EndScene( );
			RSInstance->Present( );

			//:: Frame rate stuff
			mFrameCount++;
			mBatchCount += RSInstance->GetBatchCount( );
			mPrimCount += RSInstance->GetPrimCount( );
			if( GetTickCount( ) - mStartTime >= 2000 ) {
				float fps = (float)mFrameCount / ( GetTickCount( ) - mStartTime ) * 1000.0f;
				float abc = (float)mBatchCount / mFrameCount;
				float apc = (float)mPrimCount / mFrameCount;

				//sprintf( mSFps[0], "FPS: %4.2f", fps );
				//sprintf( mSFps[1], "Average Prim Count: %4.0f", apc );
				//sprintf( mSFps[2], "Average Batch Count: %4.0f", abc );
				//printf( "FPS: %4.2f\n", fps );

				mFrameCount = 0;
				mPrimCount = 0;
				mBatchCount = 0;
				mStartTime = GetTickCount( );
			}
		};
	};

	void HaliaApp::Quit( )
	{
		PostQuitMessage( 0 );
	};
};