#pragma once

#include "base.h"
#include <FreeImagePlus.h>

namespace interplot
{

class Image
{
public:
	enum class Flags
	{
		Default       = 0,
		JpegGreyscale = JPEG_GREYSCALE,
		JpegAccurate  = JPEG_ACCURATE,
	};

public:
	bool load( const char* file, EnumFlags<Flags> flags = Flags::Default );

	bool save(
			const char* file,
			EnumFlags<Flags> flags = Flags::Default ) const;

private:
	fipImage m_Image;
};

}
