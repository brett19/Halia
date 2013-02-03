#include "DX9RenderSystem.hpp"

namespace Halia
{
	DX9RenderSystem::DX9RenderSystem( )
		: mD3d( 0 ), mDevice( 0 ), mBatchCount( 0 ), mPrimCount( 0 ), mCacheCount( 0 ), m2dDecl( 0 ), m2dMat( 0 )
	{
	};

	DX9RenderSystem::~DX9RenderSystem( )
	{
		if( m2dMat ) {
			delete m2dMat;
			m2dMat = 0;
		}
		if( m2dDecl ) {
			delete m2dDecl;
			m2dDecl = 0;
		}
		if( mDevice ) {
			mDevice->Release( );
			mDevice = 0;
		}
		if( mD3d ) {
			mD3d->Release( );
			mD3d = 0;
		}
	};

	bool DX9RenderSystem::Init( HWND hwnd, int width, int height, bool fullscreen, int msquality )
	{
		mSize.x = width;
		mSize.y = height;

		mD3d = Direct3DCreate9( D3D_SDK_VERSION );

		D3DPRESENT_PARAMETERS pp = { 0 };
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pp.EnableAutoDepthStencil = TRUE;
		pp.AutoDepthStencilFormat = D3DFMT_D24X8;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		if( msquality >= 0 )
		{
			pp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
			pp.MultiSampleQuality = msquality;
		}

		if( fullscreen )
		{
			pp.Windowed = false;
			pp.BackBufferCount = 1;
			pp.BackBufferWidth = width;
			pp.BackBufferHeight = height;
			pp.BackBufferFormat = D3DFMT_X8R8G8B8;
		}
		else
		{
			D3DDISPLAYMODE currentmode;
			if( FAILED(mD3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &currentmode )) )
				return false;

			// Set up windowed formatting
			pp.Windowed = true;
			pp.BackBufferCount = 1;
			pp.BackBufferFormat = currentmode.Format;
		}

		UINT adapterid = D3DADAPTER_DEFAULT;	
		D3DDEVTYPE rendertype = D3DDEVTYPE_HAL;

		int adaptercount = (int)mD3d->GetAdapterCount( );
		for( int i = 0; i < adaptercount; i++ )
		{
			D3DADAPTER_IDENTIFIER9 info;
			mD3d->GetAdapterIdentifier( i, 0, &info );
			
			if( strstr( info.Description, "PerfHUD" ) )
			{
				adapterid = i;
				rendertype = D3DDEVTYPE_REF;
				printf( "PerfHUD found on adapter %d (%s)\n", i, info.Description );
			}
		}

		if( FAILED(mD3d->CreateDevice( adapterid, rendertype, hwnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &mDevice )) )
			return false;

		for( int i = 0; i < 8; i++ )
			mDevice->SetSamplerState( i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );

		// Initialize 2d Rendering Stuff
		m2dDecl = new VertexDeclaration( );
		m2dDecl->AddElement( VertexElement( 0, VDeclSemantic::POSITION, VDeclType::FLOAT3, 0 ) );
		m2dDecl->AddElement( VertexElement( 0, VDeclSemantic::TEXCOORD, VDeclType::FLOAT2, 0 ) );
		m2dDecl->AddElement( VertexElement( 0, VDeclSemantic::COLOUR, VDeclType::COLOUR, 0 ) );

		m2dMat = new Material( );
		m2dMat->mAlphaBlend = true;
		m2dMat->mAlphaTest = true;
		m2dMat->mDepthWrite = true;
		m2dMat->mDepthBuffer = false;
		m2dMat->AddTextureStage(
			TextureStage(
				0,
				BlendMode( BlendOp::SELECTARGUMENT1, BlendArg::TEXTURE, BlendArg::DIFFUSE ),
				BlendMode( BlendOp::SELECTARGUMENT1, BlendArg::TEXTURE, BlendArg::DIFFUSE )
			)
		);

		return true;
	};

	bool DX9RenderSystem::Clear( const Color& color )
	{
		if( FAILED(mDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
			color.ToDword( ), 1.0f, 0 )) )
		{
			printf( "Clear failed\n" );
			return false;
		}

		return true;
	};

	bool DX9RenderSystem::BeginScene( )
	{
		if( FAILED(mDevice->BeginScene( )) )
		{
			printf( "BeginScene failed\n" );
			return false;
		}

		ResetStates( );
		mBatchCount = 0;
		mPrimCount = 0;
		mCacheCount = 0;

		return true;
	};

	bool DX9RenderSystem::EndScene( )
	{
		if( FAILED(mDevice->EndScene( )) )
		{
			printf( "EndScene failed\n" );
			return false;
		}

		return true;
	};

	bool DX9RenderSystem::Present( )
	{
		HRESULT pres = mDevice->Present( NULL, NULL, NULL, NULL );
		if( pres == D3DERR_DEVICELOST ) {
			return false;
		} else if( FAILED(pres) ) {
			return false;
		}

		return true;
	};

	int DX9RenderSystem::GetBatchCount( )
	{
		return mBatchCount;
	};

	int DX9RenderSystem::GetPrimCount( )
	{
		return mPrimCount;
	};

	void DX9RenderSystem::SetProjectionMatrix( const Matrix4& mat )
	{
		mDevice->SetTransform( D3DTS_PROJECTION, GetDXMatrix( mat ) );
		mMatProj = mat;
	};

	void DX9RenderSystem::SetViewMatrix( const Matrix4& mat )
	{
		mDevice->SetTransform( D3DTS_VIEW, GetDXMatrix( mat ) );
		mMatView = mat;
	};

	void DX9RenderSystem::SetWorldMatrix( const Matrix4& mat )
	{
		mDevice->SetTransform( D3DTS_WORLD, GetDXMatrix( mat ) );
		mMatWorld = mat;
	};

	void DX9RenderSystem::SetTextureMatrix( int stageid, const Matrix4& mat )
	{
		mDevice->SetTransform( (D3DTRANSFORMSTATETYPE)( D3DTS_TEXTURE0 + stageid ), GetDXMatrix( mat ) );
	};

	void DX9RenderSystem::SetFillMode( FillMode::FillMode fm )
	{
		if( fm == FillMode::POINT )
			SetRenderState( D3DRS_FILLMODE, D3DFILL_POINT );
		else if( fm == FillMode::WIRE )
			SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		else
			SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	};

	void DX9RenderSystem::SetCullMode( CullMode::CullMode cm )
	{
		if( cm == CullMode::NONE )
			SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		else if( cm == CullMode::CW )
			SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
		else if( cm == CullMode::CCW )
			SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	};

	void DX9RenderSystem::SetAlphaBlendEnabled( bool enabled )
	{
		SetRenderState( D3DRS_ALPHABLENDENABLE, enabled );
		SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	};

	void DX9RenderSystem::SetAlphaTestEnabled( bool enabled )
	{
		SetRenderState( D3DRS_ALPHATESTENABLE, enabled );
		SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
		SetRenderState( D3DRS_ALPHAREF, 128 );
	};

	void DX9RenderSystem::SetAlphaFunction( CompareMode::CompareMode func )
	{
		SetRenderState( D3DRS_ALPHAFUNC, func );
	};

	void DX9RenderSystem::SetAlphaReference( unsigned int ref )
	{
		SetRenderState( D3DRS_ALPHAREF, ref );
	};

	void DX9RenderSystem::SetDepthWriteEnabled( bool enabled )
	{
		SetRenderState( D3DRS_ZWRITEENABLE, enabled );
	};

	void DX9RenderSystem::SetDepthBufferEnabled( bool enabled )
	{
		SetRenderState( D3DRS_ZENABLE, enabled );
	};

	void DX9RenderSystem::SetSpecularEnabled( bool enabled )
	{
		SetRenderState( D3DRS_SPECULARENABLE, enabled );
	};

	void DX9RenderSystem::SetLightingEnabled( bool enabled )
	{
		SetRenderState( D3DRS_LIGHTING, enabled );
	};

	void DX9RenderSystem::SetTextureStageState( int stageid, TextureState::TextureState state, int value )
	{
		if( mTextureStateCached[ stageid ][ state ] && mTextureStates[ stageid ][ state ] == value )
			return;
		mTextureStateCached[ stageid ][ state ] = true;
		mTextureStates[ stageid ][ state ] = value;
		mDevice->SetTextureStageState( stageid, (D3DTEXTURESTAGESTATETYPE)state, value );
	};

	int DX9RenderSystem::SetTexture( int stageid, Texture* tex )
	{
		if( !tex )
			return 1;

		if( !tex->IsDirty( ) && mTexture[ stageid ] == tex )
			return 0;

		LPDIRECT3DTEXTURE9 dxbuf = GetTextureCache( tex );
		
		if( dxbuf ) {
			if( !FAILED(mDevice->SetTexture( stageid, dxbuf )) )
			{
				mTexture[ stageid ] = tex;
				return 0;
			}
		} else if( mCacheCount >= DX9MAXFRAMECACHE ) {
			return 2;
		}
		printf( "Failed to set texture\n" );
		return 1;
	};

	int DX9RenderSystem::SetMaterial( Material* mat )
	{
		if( mat == 0 )
			return 1;

		int j = 0;
		for( std::vector< TextureStage >::iterator i = mat->mTStages.begin( ); i != mat->mTStages.end( ); i++, j++ )
		{
			if( SetTexture( j, (*i).texture ) == 2 )
				return 2;

			if( !(*i).uses_transform ) {
				SetTextureStageState( j, TextureState::TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
			} else {
				SetTextureStageState( j, TextureState::TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
				SetTextureMatrix( j, (*i).transform );
			}

			SetTextureStageState( j, TextureState::COLOROP, (*i).colormode.op );
			if( (*i).colormode.op != BlendOp::DISABLE ) {
				SetTextureStageState( j, TextureState::COLORARG1, (*i).colormode.arg1 );
				SetTextureStageState( j, TextureState::COLORARG2, (*i).colormode.arg2 );
			}
			SetTextureStageState( j, TextureState::ALPHAOP, (*i).alphamode.op );
			if( (*i).alphamode.op != BlendOp::DISABLE ) {
				SetTextureStageState( j, TextureState::ALPHAARG1, (*i).alphamode.arg1 );
				SetTextureStageState( j, TextureState::ALPHAARG2, (*i).alphamode.arg2 );
			}
		}
		SetTextureStageState( j, TextureState::COLOROP, D3DTOP_DISABLE );
		SetTextureStageState( j, TextureState::ALPHAOP, D3DTOP_DISABLE );

		SetAlphaBlendEnabled( mat->mAlphaBlend );
		SetAlphaTestEnabled( mat->mAlphaTest );
		SetDepthWriteEnabled( mat->mDepthWrite );
		SetDepthBufferEnabled( mat->mDepthBuffer );
		SetSpecularEnabled( mat->mSpecularEnabled );

		return 0;
	};

	int DX9RenderSystem::SetVertexDeclaration( VertexDeclaration* vdecl )
	{
		if( !vdecl->IsDirty( ) && mDecl == vdecl )
			return 0;

		LPDIRECT3DVERTEXDECLARATION9 decl = GetVertexDeclCache( vdecl );

		if( decl ) {
			if( !FAILED(mDevice->SetVertexDeclaration( decl )) )
			{
				mDecl = vdecl;
				return 0;
			}
		} else if( mCacheCount >= DX9MAXFRAMECACHE ) {
			return 2;
		}
		printf( "Failed to set Vertex Declaration\n" );
		return 1;
	};

	int DX9RenderSystem::SetStreamSource( int streamidx, VertexBuffer* buffer, int start )
	{
		if( !buffer->IsDirty( ) && mVBuffer[ streamidx ] == buffer )
			return 0;

		LPDIRECT3DVERTEXBUFFER9 dxbuf = GetVertexBufferCache( buffer );

		if( dxbuf ) {
			if( !FAILED(mDevice->SetStreamSource( streamidx, dxbuf, 0, buffer->GetStride( ) )) )
			{
				mVBuffer[ streamidx ] = buffer;
				return 0;
			}
		} else if( mCacheCount >= DX9MAXFRAMECACHE ) {
			return 2;
		}
		printf( "Failed to set stream source\n" );
		return 1;
	};

	int DX9RenderSystem::SetIndexSource( IndexBuffer* buffer )
	{
		if( !buffer->IsDirty( ) && mIBuffer == buffer )
			return 0;

		LPDIRECT3DINDEXBUFFER9 dxbuf = GetIndexBufferCache( buffer );

		if( dxbuf ) {
			if( !FAILED(mDevice->SetIndices( dxbuf )) )
			{
				mIBuffer = buffer;
				return 0;
			}
		} else if( mCacheCount >= DX9MAXFRAMECACHE ) {
			return 2;
		}
		printf( "Failed to set index source\n" );
		return 1;
	};

	bool DX9RenderSystem::DrawPrimitive( PrimitiveType::PrimitiveType type, int start, int count )
	{
		if( FAILED(mDevice->DrawPrimitive( (D3DPRIMITIVETYPE)type, start, count )) ) 
		{
			printf( "DrawPrimitive failed\n" );
			return false;
		}

		mBatchCount++;
		mPrimCount += count;

		return true;
	};

	bool DX9RenderSystem::DrawPrimitiveUP( PrimitiveType::PrimitiveType type, int count, void* data, int stride )
	{
		if( FAILED(mDevice->DrawPrimitiveUP( (D3DPRIMITIVETYPE)type, count, data, stride )) )
		{
			printf( "DrawPrimitiveUP failed\n" );
			return false;
		}

		mBatchCount++;
		mPrimCount += count;

		return true;
	};

	bool DX9RenderSystem::DrawIndexedPrimitive( PrimitiveType::PrimitiveType type, int vstart, int istart, int vcount, int count )
	{
		if( FAILED(mDevice->DrawIndexedPrimitive( (D3DPRIMITIVETYPE)type, vstart, istart, vcount, 0, count )) )
		{
			printf( "DrawIndexedPrimitive failed\n" );
			return false;
		}

		mBatchCount++;
		mPrimCount += count;

		return true;
	};

	bool DX9RenderSystem::DrawIndexedPrimitiveUP( PrimitiveType::PrimitiveType type, int vstart, int istart, int vcount, int count, void* vdata, int vstride, void* idata, int istride )
	{
		D3DFORMAT idf;
		if( istride == 2 )
			idf = D3DFMT_INDEX16;
		else if( istride == 4 )
			idf = D3DFMT_INDEX32;
		else
			return false;

		if( FAILED(mDevice->DrawIndexedPrimitiveUP( (D3DPRIMITIVETYPE)type, vstart, vcount, count, idata, idf, vdata, vstride )) )
		{
			printf( "DrawIndexedPrimitiveUP failed\n" );
			return false;
		}

		mBatchCount++;
		mPrimCount += count;

		return true;
	};

	bool DX9RenderSystem::Begin2d( )
	{
		SetCullMode( CullMode::NONE );
		SetFillMode( FillMode::SOLID );
		SetProjectionMatrix( Matrix4::CreateOrthoLH( (float)mSize.x, -(float)mSize.y, 0.0f, 1.0f ) );
		SetViewMatrix( Matrix4::IDENTITY );
		SetWorldMatrix( Matrix4::IDENTITY );
		SetVertexDeclaration( m2dDecl );

		SetTextureStageState( 0, TextureState::TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
		SetTextureStageState( 0, TextureState::COLOROP, BlendOp::MODULATE );
		SetTextureStageState( 0, TextureState::COLORARG1, BlendArg::TEXTURE );
		SetTextureStageState( 0, TextureState::COLORARG2, BlendArg::DIFFUSE );
		SetTextureStageState( 0, TextureState::ALPHAOP, BlendOp::MODULATE );
		SetTextureStageState( 0, TextureState::ALPHAARG1, BlendArg::TEXTURE );
		SetTextureStageState( 0, TextureState::ALPHAARG2, BlendArg::DIFFUSE );
		SetTextureStageState( 1, TextureState::COLOROP, BlendOp::DISABLE );
		SetTextureStageState( 1, TextureState::ALPHAOP, BlendOp::DISABLE );
		SetAlphaBlendEnabled( true );
		SetAlphaTestEnabled( false );
		SetDepthWriteEnabled( true );
		SetDepthBufferEnabled( false );
		SetSpecularEnabled( false );

		return true;
	};

	bool DX9RenderSystem::DrawSprite( int x, int y, int sx, int sy, int tx, int ty, int tsx, int tsy, Texture* tex, unsigned int color )
	{
		// Get the place on the screen
		float fx = (float)(x) - (float)mSize.x / 2 - 0.5f;
		float fy = (float)(y) - (float)mSize.y / 2 - 0.5f;

		// Get the width for the screen
		float fsx = (float)sx;// / (float)mSize.x;
		float fsy = (float)sy;// / (float)mSize.y;

		// Get the place for the TextureBase
		float ftx = (float)tx / (float)tex->GetWidth( );
		float fty = (float)ty / (float)tex->GetHeight( );

		// Get the width for the TextureBase
		float ftsx = (float)tsx / (float)tex->GetWidth( );
		float ftsy = (float)tsy / (float)tex->GetHeight( );

		struct VertDef { Vector3 pos; Vector2 uv; unsigned int color; } verts[] =
		{
			{ Vector3( fx, fy, 1 ), Vector2( ftx, fty ), color },
			{ Vector3( fx, fy+fsy, 1 ), Vector2( ftx, fty+ftsy ), color },
			{ Vector3( fx+fsx, fy, 1 ), Vector2( ftx+ftsx, fty ), color },
			{ Vector3( fx+fsx, fy+fsy, 1 ), Vector2( ftx+ftsx, fty+ftsy ), color }
		};

		SetTexture( 0, tex );
		DrawPrimitiveUP( PrimitiveType::TriangleStrip, 2, &verts, sizeof( VertDef ) );

		return true;
	};

	void DX9RenderSystem::SetRenderState( D3DRENDERSTATETYPE state, unsigned int value )
	{
		if( mRenderStateCached[ state ] && mRenderStates[ state ] == value )
			return;
		mRenderStateCached[ state ] = true;
		mRenderStates[ state ] = value;
		mDevice->SetRenderState( state, value );
	};

	LPDIRECT3DVERTEXBUFFER9 DX9RenderSystem::GetVertexBufferCache( VertexBuffer* vb )
	{
		VertexBufferRC*& rc = (VertexBufferRC*&)vb->mCache;
		if( !rc )
			rc = new VertexBufferRC( );

		if( vb->IsDirty( ) )
		{
			if( mCacheCount >= DX9MAXFRAMECACHE )
				return 0;
			mCacheCount += 1;

			if( FAILED(mDevice->CreateVertexBuffer( vb->GetBufferSize( ), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &rc->mData, 0 )) )
			{
				printf( "Failed to create Vertex Buffer Cache\n" );
				return 0;
			}

			if( rc->mData )
			{
				void* buf;
				if( FAILED(rc->mData->Lock( 0, 0, &buf, 0 )) )
					return 0;
				memcpy( buf, vb->GetBuffer( ), vb->GetBufferSize( ) );
				if( FAILED(rc->mData->Unlock( )) )
					return 0;
				vb->SetDirty( false );
			}
		}
		return rc->mData;
	};

	LPDIRECT3DINDEXBUFFER9 DX9RenderSystem::GetIndexBufferCache( IndexBuffer* ib )
	{
		IndexBufferRC*& rc = (IndexBufferRC*&)ib->mCache;
		if( !rc )
			rc = new IndexBufferRC( );

		if( ib->IsDirty( ) )
		{
			if( mCacheCount >= DX9MAXFRAMECACHE )
				return 0;
			mCacheCount += 1;

			if( FAILED(mDevice->CreateIndexBuffer( ib->GetBufferSize( ), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &rc->mData, 0 )) )
			{
				printf( "Failed to create Index Buffer Cache\n" );
				return 0;
			}

			if( rc->mData )
			{
				void* buf;
				if( FAILED(rc->mData->Lock( 0, 0, &buf, 0 )) )
					return 0;
				memcpy( buf, ib->GetBuffer( ), ib->GetBufferSize( ) );
				if( FAILED(rc->mData->Unlock( )) )
					return 0;
				ib->SetDirty( false );
			}
		}
		return rc->mData;
	};

	LPDIRECT3DTEXTURE9 DX9RenderSystem::GetTextureCache( Texture* tex )
	{
		TextureRC*& rc = (TextureRC*&)tex->mCache;
		if( !rc )
			rc = new TextureRC( );

		if( tex->IsDirty( ) )
		{
			if( mCacheCount >= DX9MAXFRAMECACHE )
				return 0;
			mCacheCount++;

			if( FAILED(D3DXCreateTexture( mDevice, tex->GetWidth( ), tex->GetHeight( ), 1, D3DUSAGE_DYNAMIC, (D3DFORMAT)tex->GetFormat( ), D3DPOOL_DEFAULT, &rc->mData )) )
			{
				printf( "Failed to create Texture Cache :: Couldn't create surface\n" );
				return 0;
			}

			LPDIRECT3DSURFACE9 surf;
			rc->mData->GetSurfaceLevel( 0, &surf );
			D3DLOCKED_RECT rect;
			surf->LockRect( &rect, 0, 0 );
			char* data = (char*)rect.pBits;
			memcpy( data, tex->GetBuffer( ), tex->GetBufferSize( ) );
			surf->UnlockRect( );

			D3DXSaveTextureToFile( "c:\\texdebug.png", D3DXIFF_PNG, rc->mData, 0 );

			tex->SetDirty( false );
		}
		return rc->mData;
	};

	LPDIRECT3DVERTEXDECLARATION9 DX9RenderSystem::GetVertexDeclCache( VertexDeclaration* decl )
	{
		VertexDeclarationRC*& rc = (VertexDeclarationRC*&)decl->mCache;
		if( !rc )
			rc = new VertexDeclarationRC( );

		if( decl->IsDirty( ) )
		{
			if( mCacheCount >= DX9MAXFRAMECACHE )
				return 0;
			mCacheCount += 1;

			// Generate Declaration
			std::vector< D3DVERTEXELEMENT9 > elems;
			int offset = 0;
			for( std::vector< VertexElement >::iterator i = decl->mElements.begin( ); i != decl->mElements.end( ); i++ )
			{
				D3DVERTEXELEMENT9 elem = { (*i).streamidx, offset, (*i).type, D3DDECLMETHOD_DEFAULT, (*i).semantic, (*i).usageidx };
				elems.push_back( elem );
				offset += VDeclType::LENGTH[ (*i).type ];
			}
			D3DVERTEXELEMENT9 lastelem = D3DDECL_END();
			elems.push_back( lastelem );

			if( FAILED(mDevice->CreateVertexDeclaration( &elems[0], &rc->mData )) )
			{
				printf( "Failed to create Vertex Declaration Cache\n" );
				return 0;
			}

			decl->SetDirty( false );
		}

		return rc->mData;
	};

	D3DXMATRIX* DX9RenderSystem::GetDXMatrix( const Matrix4& mat )
	{
		return (D3DXMATRIX*)&mat;
	};

	void DX9RenderSystem::ResetStates( )
	{
		for( int i = 0; i < 256; i++ )
			mRenderStateCached[i] = false;
		for( int i = 8; i < 8; i++ )
			for( int j = 0; j < 36; j++ )
				mTextureStateCached[i][j] = false;
		mDecl = 0;
		for( int i = 0; i < 8; i++ )
			mVBuffer[i] = 0;
		mIBuffer = 0;
		for( int i = 0; i < 8; i++ )
			mTexture[i] = 0;
	};
};