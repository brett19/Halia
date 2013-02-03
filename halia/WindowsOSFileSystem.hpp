#pragma once
#include <iostream>
#include "FileSystem.hpp"

namespace Halia
{
	class WindowsOSFileSystem : public FileSystem
	{
	public:
		class FileStream : FileSystem::FileStream
		{
		public:
			FileStream( );

			virtual bool Open( const std::string& path );
			virtual void Close( );
			virtual int Read( void* buffer, int size );
			virtual int Write( void* buffer, int size );
			virtual void Seek( int offset, SeekOrigin::SeekOrigin origin );
			virtual int Tell( ) const;
			virtual bool Eof( ) const;

		protected:
			FILE* mFh;
		};

		WindowsOSFileSystem( const std::string& basepath );

		virtual FileSystem::FileStream* OpenFile( const std::string& path );
		virtual bool FileExists( const std::string& path );

	private:
		std::string mBasePath;
	};
};