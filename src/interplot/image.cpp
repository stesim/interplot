#include "image.h"

namespace interplot
{

bool Image::load( const char* file, EnumFlags<Flags> flags )
{
	return m_Image.load( file, flags.raw() );
}

bool Image::save( const char* file, EnumFlags<Flags> flags ) const
{
	return m_Image.save( file, flags.raw() );
}

}
