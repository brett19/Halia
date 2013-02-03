#include "Network.hpp"

namespace Halia
{
	namespace Net
	{
		bool Init( )
		{
			WSADATA wsaData;
			return( !WSAStartup( 0x202, &wsaData ) );
		};

		void Destroy( )
		{
			WSACleanup( );
		};
	};
};