#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "LogManager.hpp"
#include "FileSystemManager.hpp"
#include "DX9RenderSystem.hpp"
#include "ResourceManager.hpp"
#include "InputManager.hpp"
#include "HaliaApp.hpp"

//! The primary namespace used by all Halia objects
namespace Halia
{
	//! Initializes the logging module of the engine
	void InitLogging( const std::string& path = "halia.log" );

	//! Initializes the file-system manager for the engine
	void InitFileSystem( );

	//! Initializes and starts the resource manager
	void InitResMgr( );

	//! Initializes the 2d and 3d rendering engine, as requested
	void InitRenderer( RendererType::RendererType rtype, HWND hwnd, int width, int height, bool fullscreen = false );

	//! Shuts down all modules of the engine and prepares for application shut-down
	void Destroy( );
};