#pragma once

namespace Halia
{
	class Quarternion
	{
	public:
		Quarternion( );
		Quarternion( float x_, float y_, float z_, float w_ );
		Quarternion( float s );

		float x, y, z, w;
	};
};