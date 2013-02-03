#pragma once
#include <winsock2.h>
#include "Socket.hpp"

namespace Halia
{
	//! Used for basic Network control
	namespace Net
	{

		//! Initialize the networking subsystem for the operating system
		/*!
			Calls WSAStartup for windows, and any other initialization procedures
			required on other systems.
		*/
		bool Init( );

		//! Shuts down any network subsystem started by Net::Init
		void Destroy( );
	};
};