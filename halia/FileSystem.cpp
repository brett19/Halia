#include "FileSystem.hpp"

namespace Halia
{
	int FileSystem::FileStream::GetSize( )
	{
		int curpos = Tell( );
		Seek( 0, SeekOrigin::END );
		int size = Tell( );
		Seek( curpos, SeekOrigin::SET );
		return size;
	};

	std::string FileSystem::FileStream::ReadString( int len )
	{
		char* tmp = new char[ len + 1 ];
		Read( tmp, len );
		tmp[ len ] = 0;
		std::string str( tmp );
		delete[] tmp;
		return str;
	};

	std::string FileSystem::FileStream::ReadString( StringType::StringType lsize )
	{
		if( lsize == StringType::NULLTERM ) {
			int tmps = 0;
			while( ++tmps && Read<char>( ) != 0 );
			Seek( -tmps, SeekOrigin::CUR );
			return ReadString( tmps );
		} else if( lsize == StringType::PRE7 ) {
			int tmpk = 0;
			char tmpc = Read<char>( );
			int tmps = tmpc & 0x7F;
			while( tmpc & 0x80 )
			{
				tmpc = Read<char>( );
				tmpk += 7;
				tmps |= ( tmpc & 0x7F ) << tmpk;
			}
			return ReadString( tmps );
		} else {
			int tmps = 0;
			Read( (char*)&tmps, lsize );
			return ReadString( tmps );
		}
	};

	void FileSystem::CloseFile( FileStream* fh )
	{
		fh->Close( );
		delete fh;
	};
};