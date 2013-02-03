#include "Halia/Halia.hpp"
#include "Halia/Overlay.hpp"
#include "Halia/OverlayRoot.hpp"
#include "Halia/OverlayImage.hpp"
#include "Halia/OverlayLabel.hpp"
#include "Halia/Font.hpp"

class HaliaTest : public Halia::HaliaApp
{
public:
	void Init( )
	{
		Halia::InitLogging( );
		Halia::InitFileSystem( );
		HFSM->AddFs( "master", new Halia::WindowsOSFileSystem( "C:\\RuffVFS\\" ) );
		//Halia::InitScripting( );
		HaliaApp::Init( Halia::RendererType::DX9, true, "Halia Engine", 1024, 768 );
		Halia::InitResMgr( );
		HRS->SetLightingEnabled( false );
		HLOGMSG( "-- Engine Init Completed!" );

		mTest2 = new Halia::Font( );
		mTest2->Init( "Arial", 14 );
		
		mTest1 = new Halia::FileTexture( "3DDATA\\CONTROL\\RES\\EXUI1.DDS" );
		mTest1->Load( );
		mOTex = new Halia::OverlayTexture( mTest1, 0, 0, 512, 256 );
		mTest1->DelRef( );

		mLoadImage = new Halia::OverlayImage( mOTex, 256, 256, 512, 256 );
		
		mORoot = new Halia::OverlayRoot( 1024, 768 );
		mORoot->AddChild( mLoadImage );

		HIM->AddListener( Halia::EventType::ALL, mORoot );
	};

	void PreFrame( )
	{
		mORoot->Update( );
	};

	void Frame( )
	{
		HRS->Begin2d( );
		mORoot->Render( );
		
		std::string str = "Ikarus Test Client v0.21\n" __DATE__ " - " __TIME__;
		Halia::Point d = mTest2->GetDimensions( str );
		mTest2->DrawString( 1024 - 10 - d.x, 768 - 10 - d.y, str, 0xFF000000 );
	};

	void Destroy( )
	{
		delete mORoot;
		delete mOTex;
		delete mLoadImage;
		delete mTest2;

		HaliaApp::Destroy( );
		Halia::Destroy( );
	};

	Halia::OverlayTexture* mOTex;
	Halia::FileTexture* mTest1;
	Halia::Font* mTest2;
	Halia::OverlayRoot* mORoot;
	Halia::OverlayImage* mLoadImage;
};

int main( int argc, char* argv[] )
{
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc( 241 );

	HaliaTest tstapp;
	tstapp.Init( );
	tstapp.StartRendering( );
	tstapp.Destroy( );

	return 1;
};