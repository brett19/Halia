#pragma once
#include <iostream>
#include <string>

namespace Halia
{
	namespace StringType
	{
		enum StringType
		{
			NULLTERM = -1,
			PRE7 = 0,
			PRE8 = 1,
			PRE16 = 2,
			PRE32 = 3
		};
	}		

	namespace SeekOrigin
	{
		enum SeekOrigin
		{
			SET = 0,
			CUR = 1,
			END = 2
		};
	};

	class FileSystem
	{
	public:
		class FileStream
		{
		public:
			virtual bool Open( const std::string& path ) = 0;
			virtual void Close( ) = 0;
			virtual int Read( void* buffer, int size ) = 0;
			virtual int Write( void* buffer, int size ) = 0;
			virtual void Seek( int offset, SeekOrigin::SeekOrigin origin ) = 0;
			virtual int Tell( ) const = 0;
			virtual bool Eof( ) const = 0;

			virtual int GetSize( );
			std::string ReadString( int len );
			std::string ReadString( StringType::StringType lsize = StringType::NULLTERM );
			
			template<class T>
			T Read( )
			{
				T tmp;
				Read( &tmp, sizeof( T ) );
				return tmp;
			};

		protected:

		};

		virtual bool FileExists( const std::string& path ) = 0;
		virtual FileStream* OpenFile( const std::string& path ) = 0;
		virtual void CloseFile( FileStream* fh );

	protected:
		
	};
};