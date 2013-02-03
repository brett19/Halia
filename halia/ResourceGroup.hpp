#pragma once
#include <vector>
#include "Resource.hpp"

namespace Halia
{
	//! Defines a group of resources to be loaded and unloaded together
	class ResourceGroup : public Resource
	{
	public:
		//! Constructor
		ResourceGroup( );

		//! Destructor
		~ResourceGroup( );

		//! Add a resource to be loaded with the group
		void AddResource( Resource* res );

		//! Remove a resource from being loaded with the group
		bool DelResource( Resource*& res );

		//! Load all resources in this group
		virtual bool Load( );

		//! Unload all the resources in this group
		virtual void Unload( );

	protected:
		//! List of all resources in this group
		std::vector< Resource* > mResources;
	};
};