#include "Halia.hpp"

namespace Halia
{
	void InitLogging( const std::string& path )
	{
		if( !LMInstance ) {
			LMInstance = new LogManager( );
			LMInstance->Open( path.c_str( ) );
			HLOGMSG( "Initialized Logger" );
		}
	};

	void InitFileSystem( )
	{
		if( !FSMInstance ) {
			FSMInstance = new FileSystemManager( );
			HLOGMSG( "Initialized File System Manager" );
		}
	};

	void InitResMgr( )
	{
		if( !RMInstance ) {
			RMInstance = new ResourceManager( );
			HLOGMSG( "Initialized Resource Manager" );
		}
	};

	void InitRenderer( RendererType::RendererType rtype, HWND hwnd, int width, int height, bool fullscreen )
	{
		if( !RSInstance ) {
			if( rtype == RendererType::DX9 )
				RSInstance = new DX9RenderSystem( );
			else if( rtype == RendererType::OpenGL )
				RSInstance = new DX9RenderSystem( );
			RSInstance->Init( hwnd, width, height, fullscreen );
			HLOGMSG( "Initialized Renderer System [RSType:%d,WIDTH:%d,HEIGHT:%d]", rtype, width, height );
		}
		if( !IMInstance ) {
			IMInstance = new InputManager( );
			IMInstance->Init( hwnd, width, height );
			HLOGMSG( "Initialized Input Manager" );
		}
	};

	void Destroy( )
	{
		if( IMInstance ) {
			delete IMInstance;
			IMInstance = 0;
		}
		if( RSInstance ) {
			delete RSInstance;
			RSInstance = 0;
		}
		if( RMInstance ) {
			delete RMInstance;
			RMInstance = 0;
		}
		if( FSMInstance ) {
			delete FSMInstance;
			FSMInstance = 0;
		}
		if( LMInstance ) {
			delete LMInstance;
			LMInstance = 0;
		}
	};
};