#include "Overlay.hpp"

namespace Halia
{
	Overlay::Overlay( )
		: mParent( 0 ), mPosition( 0, 0 ), mSize( 0, 0 ), mVisible( true )
	{
	};

	Overlay::Overlay( int x, int y )
		: mParent( 0 ), mPosition( x, y ), mSize( 0, 0 ), mVisible( true )
	{
	};

	Overlay::Overlay( int x, int y, int sx, int sy )
		: mParent( 0 ), mPosition( x, y ), mSize( sx, sy ), mVisible( true )
	{
	};

	Overlay::~Overlay( )
	{
	};

	void Overlay::AddChild( Overlay* o )
	{
		o->mParent = this;
		mChildren.push_back( o );
	};

	void Overlay::RemoveChild( Overlay* o )
	{
		for( std::vector< Overlay* >::iterator i = mChildren.begin( ); i != mChildren.end( ); ++i )
		{
			if( (*i) == o ) {
				mChildren.erase( i );
				break;
			}
		}
	};

	const Point& Overlay::GetPosition( ) const
	{
		return mPosition;
	};

	void Overlay::SetPosition( int x, int y )
	{
		mPosition = Point( x, y );
	};

	void Overlay::SetSize( int x, int y )
	{
		mSize = Point( x, y );
	};

	void Overlay::SetVisible( bool visible )
	{
		mVisible = visible;
	};

	const Point& Overlay::GetSize( ) const
	{
		return mSize;
	};

	OverlayType Overlay::GetType( ) const
	{
		return OverlayType_BASE;
	};

	void Overlay::CenterChild( Overlay* child ) const
	{
		child->SetPosition(
			( mSize.x / 2 ) - ( child->GetSize( ).x / 2 ),
			( mSize.y / 2 ) - ( child->GetSize( ).y / 2 )
		);
	};

	bool Overlay::EventHandler( Event* evt )
	{
		return true;
	};

	void Overlay::Update( )
	{
		if( mParent )
			mGPosition = mParent->mGPosition + mPosition;
		else
			mGPosition = mPosition;

		for( std::vector< Overlay* >::iterator i = mChildren.begin( ); i != mChildren.end( ); ++i )
		{
			if( (*i)->mVisible )
				(*i)->Update( );
		}
	};

	void Overlay::Render( )
	{
		for( std::vector< Overlay* >::iterator i = mChildren.begin( ); i != mChildren.end( ); ++i )
		{
			if( (*i)->mVisible )
				(*i)->Render( );
		}
	};

	bool Overlay::TestPoint( const Point& p ) const
	{
		if( p.x < mPosition.x || p.x >= mPosition.x + mSize.x ||
			p.y < mPosition.y || p.y >= mPosition.y + mSize.y )
			return false;
		return true;
	};

	Overlay* Overlay::FindTarget( const Point& p )
	{
		if( !TestPoint( p ) )
			return 0;
		
		for( std::vector< Overlay* >::iterator i = mChildren.begin( ); i != mChildren.end( ); ++i )
		{
			Overlay* target = (*i)->FindTarget( p - mPosition );
			if( target )
				return target;
		}

		return this;
	};
};