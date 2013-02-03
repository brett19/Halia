#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "RenderSystem.hpp"
#include "Point.hpp"

#define DX9MAXFRAMECACHE 100000

namespace Halia
{
	// Defines the operations for the DirectX render system.
	class DX9RenderSystem : public RenderSystem
	{
#define DEFINERC( x, y ) struct x : public RenderCache { x( ) : mData( 0 ) { }; ~x( ) { if( mData ) mData->Release( ); }; y mData; };
		DEFINERC( TextureRC, LPDIRECT3DTEXTURE9 );
		DEFINERC( VertexDeclarationRC, LPDIRECT3DVERTEXDECLARATION9 );
		DEFINERC( VertexBufferRC, LPDIRECT3DVERTEXBUFFER9 );
		DEFINERC( IndexBufferRC, LPDIRECT3DINDEXBUFFER9 );
#undef DEFINERC

	public:
		DX9RenderSystem( );
		~DX9RenderSystem( );

		//:: Initialize crap
		bool Init( HWND hwnd, int width, int height, bool fullscreen = false, int msquality = -1 );

		//:: Scene functions
		bool Clear( const Color& color );
		bool BeginScene( );
		bool EndScene( );
		bool Present( );
		int GetBatchCount( );
		int GetPrimCount( );

		//:: Camera functions
		void SetProjectionMatrix( const Matrix4& mat );
		void SetViewMatrix( const Matrix4& mat );
		void SetWorldMatrix( const Matrix4& mat );
		
		//:: Render state stuff
		void SetFillMode( FillMode::FillMode fm );
		void SetCullMode( CullMode::CullMode cm );
		void SetAlphaBlendEnabled( bool enabled );
		void SetAlphaTestEnabled( bool enabled );
		void SetAlphaFunction( CompareMode::CompareMode func );
		void SetAlphaReference( unsigned int ref );
		void SetDepthWriteEnabled( bool enabled );
		void SetDepthBufferEnabled( bool enabled );
		void SetSpecularEnabled( bool enabled );
		void SetLightingEnabled( bool enabled );

		//:: Texture stuff
		void SetTextureMatrix( int stageid, const Matrix4& mat );
		void SetTextureStageState( int stageid, TextureState::TextureState state, int value );
		int SetTexture( int stageid, Texture* tex );

		//:: Master shit
		int SetMaterial( Material* mat );
		int SetVertexDeclaration( VertexDeclaration* vdecl );
		int SetStreamSource( int streamidx, VertexBuffer* buffer, int start );
		int SetIndexSource( IndexBuffer* buffer );

		//:: Drawing stuff
		bool DrawPrimitive( PrimitiveType::PrimitiveType type, int start, int count );
		bool DrawPrimitiveUP( PrimitiveType::PrimitiveType type, int count, void* data, int stride );
		bool DrawIndexedPrimitive( PrimitiveType::PrimitiveType type, int vstart, int istart, int vcount, int count );
		bool DrawIndexedPrimitiveUP( PrimitiveType::PrimitiveType type, int vstart, int istart, int vcount, int count, void* vdata, int vstride, void* idata, int istride );
		
		//:: 2D Drawing stuff
		bool Begin2d( );
		bool DrawSprite( int x, int y, int sx, int sy, int tx, int ty, int tsx, int tsy, Texture* tex, unsigned int color = 0xFFFFFFFF );

		//:: DirectX only
		void SetRenderState( D3DRENDERSTATETYPE state, unsigned int value );
		LPDIRECT3DVERTEXBUFFER9 GetVertexBufferCache( VertexBuffer* vb );
		LPDIRECT3DINDEXBUFFER9 GetIndexBufferCache( IndexBuffer* ib );
		LPDIRECT3DTEXTURE9 GetTextureCache( Texture* tex );
		LPDIRECT3DVERTEXDECLARATION9 GetVertexDeclCache( VertexDeclaration* decl );
		D3DXMATRIX* GetDXMatrix( const Matrix4& mat );
		void ResetStates( );

	private:
		// Core System Stuff
		LPDIRECT3D9 mD3d;
		LPDIRECT3DDEVICE9 mDevice;
		Point mSize;

		// Base DirectX States
		bool mRenderStateCached[ 256 ];
		unsigned int mRenderStates[ 256 ];
		bool mTextureStateCached[ 8 ][ 36 ];
		unsigned int mTextureStates[ 8 ][ 36 ];
		VertexDeclaration* mDecl;
		VertexBuffer* mVBuffer[ 8 ];
		IndexBuffer* mIBuffer;
		Texture* mTexture[ 8 ];
		unsigned int mBatchCount;
		unsigned int mPrimCount;
		unsigned int mCacheCount;
		Matrix4 mMatProj;
		Matrix4 mMatView;
		Matrix4 mMatWorld;

		// 2D Drawing Stuff
		VertexDeclaration* m2dDecl;
		Material* m2dMat;
	};
};