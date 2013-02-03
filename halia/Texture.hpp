#pragma once
#include <string>
#include <freeimage.h>
#include "SmartPtr.hpp"
#include "FileSystemManager.hpp"
#include "RenderCached.hpp"
#include "Resource.hpp"

namespace Halia
{
	namespace TextureFormat
	{
		enum TextureFormat
		{
			UNKNOWN = 0,
			R8G8B8 = 20,
			A8R8G8B8 = 21,
			X8R8G8B8 = 22,
			R5G6B5 = 23,
			X1R5G5B5 = 24,
			A1R5G5B5 = 25,
			A4R4G4B4 = 26,
			R3G3B2 = 27,
			A8 = 28,
			A8R3G3B2 = 29,
			X4R4G4B4 = 30,
			A2B10G10R10 = 31,
			A8B8G8R8 = 32,
			X8B8G8R8 = 33,
			G16R16 = 34,
			A2R10G10B10 = 35,
			A16B16G16R16 = 36,
			A8L8 = 51
		};

		const int LENGTH[] = { 
			/*  TO 20 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			/*  20 ON */ 3, 4, 4, 2, 2, 2, 2, 1, 1, 2, 2, 4, 4, 4, 4, 4, 8, 0, 0, 0,
			/*  40 ON */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0,
			/*  60 ON */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};
	};

	//! A class to represent a single texture, used by the render system during rendering
	class Texture : public RenderCached, public SmartPtr
	{
	public:
		//! Constructor
		Texture( )
			: mWidth( 0 ), mHeight( 0 ), mData( 0 ), mFormat( TextureFormat::UNKNOWN )
		{
		};

		//! Constructor
		Texture( int width, int height, TextureFormat::TextureFormat format )
			: mData( 0 )
		{
			Create( width, height, format );
		};

		//! Destructor
		~Texture( )
		{
			if( mData )
				delete[] mData;
		};

		//! Create an image with the given parameters
		void Create( int width, int height, TextureFormat::TextureFormat format )
		{
			if( mData )
				delete[] mData;

			mWidth = width;
			mHeight = height;
			mFormat = format;
			mData = new char[ width * height * TextureFormat::LENGTH[ mFormat ] ];

			SetDirty( );
		};

		//! Returns the width of the image
		int GetWidth( ) const
		{
			return mWidth;
		};

		//! Returns the height of the image
		int GetHeight( ) const
		{
			return mHeight;
		};

		//! Returns the format of the image
		TextureFormat::TextureFormat GetFormat( ) const
		{
			return mFormat;
		};

		//! Returns a raw pointer to the memory of the image
		char* GetBuffer( )
		{
			return mData;
		};

		//! Returns the size of the internal buffer for the image
		int GetBufferSize( ) const
		{
			return mWidth * mHeight * TextureFormat::LENGTH[mFormat];
		};

	protected:
		//! Width of the texture
		int mWidth;

		//! Height of the texture
		int mHeight;

		//! Format of the texture
		TextureFormat::TextureFormat mFormat;

		//! Pointer to the texture data
		char* mData;
	};

	//! A class to simplify loading of a texture
	class FileTexture : public Texture
	{
	public:
		//! Constructor
		FileTexture( const std::string& path )
			: mPath( path )
		{
		};

		//! Constructor
		FileTexture( )
			: mPath( "" )
		{
		};

		//! Destructor
		~FileTexture( )
		{
		};

		//! Set the path of the image to load
		void SetPath( const std::string& path )
		{
			mPath = path;
		};

		//! Load the image into memory
		bool Load( )
		{
			//:: Check if its already loaded
			if( mData )
				return true;

			// Load File Into Memory
			Halia::FileSystem::FileStream* fh = Halia::FSMInstance->OpenFile( mPath );
			if( !fh ) {
				printf( "Failed to load Texture '%s'\n", mPath.c_str( ) );
				return false;
			}

			int filelen = fh->GetSize( );
			char* filedata = new char[ filelen ];
			fh->Read( filedata, filelen );

			Halia::FSMInstance->CloseFile( fh );

			// Create FreeImage Data
			FIMEMORY* imgmem = FreeImage_OpenMemory( (BYTE*)filedata, filelen );
			FREE_IMAGE_FORMAT imgf = FreeImage_GetFileTypeFromMemory( imgmem, 0 );
			FIBITMAP* img = FreeImage_LoadFromMemory( imgf, imgmem, 0 );

			// Read Data
			mWidth = FreeImage_GetWidth( img );
			mHeight = FreeImage_GetHeight( img );
			mFormat = TextureFormat::A8R8G8B8;
			mData = new char[ mWidth * mHeight * TextureFormat::LENGTH[mFormat] ];
			FreeImage_ConvertToRawBits( (BYTE*)mData, img, FreeImage_GetPitch( img ), 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE );

			// Close FreeImage Handle
			FreeImage_Unload( img );
			FreeImage_CloseMemory( imgmem );

			delete filedata;

			return true;
		};

	protected:
		//! Path to the image to load
		std::string mPath;
	};
};