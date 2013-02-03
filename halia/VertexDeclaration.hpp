#pragma once
#include <vector>
#include "SmartPtr.hpp"
#include "RenderCached.hpp"
#include "VertexElement.hpp"

namespace Halia
{
	//! A vertex declaration used by the render-system to define the format of a vertex buffer.
	class VertexDeclaration : public RenderCached, public SmartPtr
	{
	public:
		//! Constructor
		VertexDeclaration( );

		//! Destructor
		~VertexDeclaration( );

		//! Add an element to the declaration
		void AddElement( VertexElement elem_ );

		//! Return a list of elements in this declaration
		const VertexElement::List& GetElementList( );

		//! Checks if a specific semantic is supported on a stream
		bool IsSupported( int streamidx_, VDeclSemantic::VDeclSemantic semantic_ );

		//! Find the position of a semantic
		int GetElementOffset( VDeclSemantic::VDeclSemantic semantic_, int usageidx_ );

		//! Find the size of a single value in a stream
		int GetStride( int streamidx_ );

		//! List of elements in the declaration
		VertexElement::List mElements;
	};
};