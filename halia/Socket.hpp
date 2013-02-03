#pragma once
#include <string>
#include "Network.hpp"

namespace Halia
{
	//! An enum for errors returned by the network module
	enum NetworkErr
	{
		NetworkErr_SUCCESS = 0,
		NetworkErr_CREATE,
		NetworkErr_BIND,
		NetworkErr_LISTEN,
		NetworkErr_ACCEPT,
		NetworkErr_RESOLVE,
		NetworkErr_CONNECT,

		NetworkErr_NOTCONNECTED,
	};

	//! A class that encapsulates most basic socket Network IO
	/*!
		A class which encapsulates most of the functions needed for Network IO,
		in a cross-platform way.
	*/
	class Socket
	{
	public:
		//! Constructor
		Socket( )
			: mSock( INVALID_SOCKET )
		{
		};

		//! Destructor
		~Socket( )
		{
			Disconnect( );
		};

		//! Binds the socket to a specific IP and Port
		NetworkErr Bind( const std::string& ip, int port )
		{
			if( !Create( ) )
				return NetworkErr_CREATE;
			
			mSockInfo.sin_family = AF_INET;
			mSockInfo.sin_addr.s_addr = inet_addr( ip.c_str( ) );
			mSockInfo.sin_port = htons( port );
			
			if( bind( mSock, (SOCKADDR *)&mSockInfo, sizeof( mSockInfo ) ) == SOCKET_ERROR )
				return NetworkErr_BIND;

			return NetworkErr_SUCCESS;
		};

		//! Begins listening for connections on the socket
		NetworkErr Listen( int backlog = SOMAXCONN )
		{
			if( listen( mSock, backlog ) == SOCKET_ERROR )
				return NetworkErr_LISTEN;
			return NetworkErr_SUCCESS;
		};
		
		//! Accepts a single connection from the listen backlog
		NetworkErr Accept( Socket& sock )
		{
			int sisize = sizeof( sock.mSockInfo );
			sock.mSock = accept( mSock, (SOCKADDR *)&sock.mSockInfo, &sisize );
			if( sock.mSock == INVALID_SOCKET )
				return NetworkErr_ACCEPT;
			return NetworkErr_SUCCESS;
		};

		//! Connects to another host listening for TCP connections on the given port.
		NetworkErr Connect( const std::string& host, int port )
		{
			if( !Create( ) )
				return NetworkErr_CREATE;

			hostent* hostent;
			if( !(hostent = gethostbyname( host.c_str( ) )))
				return NetworkErr_RESOLVE;

			mSockInfo.sin_family		= AF_INET;
			mSockInfo.sin_addr.s_addr	= *((unsigned long*)hostent->h_addr_list[0]);
			mSockInfo.sin_port		= htons( port );

			if( connect( mSock, (SOCKADDR *)&mSockInfo, sizeof( mSockInfo ) ) == SOCKET_ERROR )
				return NetworkErr_CONNECT;

			return NetworkErr_SUCCESS;
		};

		//! Disconnect from whatever service the socket is connected to.
		void Disconnect( )
		{
			shutdown( mSock, SD_BOTH );
			Destroy( );
		};

		//! Send network data
		int Send( void* buffer, int len )
		{
			if( mSock == INVALID_SOCKET )
				return 0;

			int retval = send( mSock, (char*)buffer, len, 0 );
			if( retval <= 0 ) {
				Disconnect( );
				return 0;
			}
			return retval;
		};

		//! Receive some data from the network's queue
		int Recv( void* buffer, int len )
		{
			if( mSock == INVALID_SOCKET )
				return 0;

			int retval = recv( mSock, (char*)buffer, len, 0 );
			if( retval <= 0 ) {
				Disconnect( );
				return 0;
			}
			return retval;
		};

		//! Checks wether this connection is currently active
		bool IsConnected( )
		{
			return mSock != INVALID_SOCKET;
		};

		//! Returns the underlying socket of which this class is based on.
		const SOCKET& GetSocket( )
		{
			return mSock;
		};

	private:
		//! Internal function for creating the socket used
		bool Create( )
		{
			if( mSock != INVALID_SOCKET )
				return true;
			mSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
			if( mSock == INVALID_SOCKET )
				return false;
			return true;
		};

		//! Internal function for destroying the socket created with Socket::Create
		void Destroy( )
		{
			closesocket( mSock );
			mSock = INVALID_SOCKET;
		};

	protected:
		//! The underlying socket used by this class
		SOCKET mSock;

		//! Used to store information about the 2nd party in this conversation
		struct sockaddr_in mSockInfo;
	};
};