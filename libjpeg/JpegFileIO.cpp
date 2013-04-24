// JpegFileIO.cpp

#include	<cstdio>
#include	<iostream>
#include	<string>
#include	<vector>

#include	<jpeglib.h>

#include	"JpegFileIO.h"

using namespace std;



// constructor and destructor

JpegFileIO::JpegFileIO( const int n_origin )
	: origin( n_origin )
{
}

JpegFileIO::~JpegFileIO()
{
}



// read

int
JpegFileIO::Read( const string& filename,
				  int& width, int& height, vector<unsigned char>& image )
{
	return ReadRGB( filename, width, height, image );
}

int
JpegFileIO::ReadRGB( const string& filename,
					 int& width, int& height, vector<unsigned char>& image )
{
	struct	jpeg_decompress_struct	cinfo;
	struct	jpeg_error_mgr	jerr;

	cinfo.err = jpeg_std_error( &jerr );
	jpeg_create_decompress( &cinfo );
	
	FILE	*infile;
	if (( infile = fopen( filename.c_str(), "rb" )) == NULL ) {
		cerr << "JpegFileIO::Read(): cannot open " << filename << endl;
		return -1;
	}
	jpeg_stdio_src( &cinfo, infile );

	int	row_stride;
	JSAMPARRAY	row_buffer;

	jpeg_read_header( &cinfo, TRUE );
	jpeg_start_decompress( &cinfo );
	row_stride = cinfo.output_width * cinfo.output_components;
	row_buffer = ( *cinfo.mem->alloc_sarray )
		(( j_common_ptr ) &cinfo, JPOOL_IMAGE, row_stride, 1 );

	width = cinfo.output_width;
	height = cinfo.output_height;
	image.resize( width * height * 3 );

	int	y, y_step;

	if ( origin == ORIGIN_LEFT_BOTTOM ) {
		y = height - 1;
		y_step = -1;
	} else {
		y = 0;
		y_step = 1;
	}
	
	while ( cinfo.output_scanline < cinfo.output_height ) {
		jpeg_read_scanlines( &cinfo, row_buffer, 1 );
		unsigned char	*src, *dst;
		src = row_buffer[ 0 ];
		dst = &image[ y * width * 3 ];
		for ( int i = 0; i < width * 3; i++ ) {
			*dst++ = *src++;
		}
		y += y_step;
	}

	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress( &cinfo );
	fclose( infile );

	return 0;
}

int
JpegFileIO::ReadRGBA( const string& filename,
					  int& width, int& height, vector<unsigned char>& image )
{
	int	r;

	r = ReadRGB( filename, width, height, image );

	if ( !r ) {
		image.resize( width * height * 4 );
	
		unsigned char	*src = &image[ width * height * 3 - 1 ];
		unsigned char	*dst = &image[ width * height * 4 - 1 ];

		src++;
		dst++;

		for ( int i = 0; i < width * height; i++ ) {
			*(--dst) = 255;
			*(--dst) = *(--src);
			*(--dst) = *(--src);
			*(--dst) = *(--src);
		}
	}

	return r;
}

unsigned char *
JpegFileIO::Read( const string& filename, int& width, int& height )
{
	return ReadRGB( filename, width, height );
}

unsigned char *
JpegFileIO::ReadRGB( const string& filename, int& width, int& height )
{
	vector<unsigned char>	image;

	if ( ReadRGB( filename, width, height, image )) {
		return 0;
	}

	unsigned char	*r = new unsigned char[ width * height * 3 ];

	unsigned char	*src = &image[ 0 ];
	unsigned char	*dst = r;

	for ( int i = 0; i < width * height * 3; i++ ) {
		*dst++ = *src++;
	}

	return r;
}

unsigned char *
JpegFileIO::ReadRGBA( const string& filename, int& width, int& height )
{
	vector<unsigned char>	image;

	if ( ReadRGB( filename, width, height, image )) {
		return 0;
	}

	unsigned char	*r = new unsigned char[ width * height * 4 ];

	unsigned char	*src = &image[ 0 ];
	unsigned char	*dst = r;

	for ( int i = 0; i < width * height * 4; i++ ) {
		*dst++ = *src++;
	}

	return r;
}



// write

int
JpegFileIO::Write( const string& filename,
				   const int width, const int height,
				   const vector<unsigned char>& image,
				   const int quality )
{
	return Write( filename, width, height, &image[ 0 ], quality );
}

int
JpegFileIO::WriteRGB( const string& filename,
					  const int width, const int height,
					  const vector<unsigned char>& image,
					  const int quality )
{
	return WriteRGB( filename, width, height, &image[ 0 ], quality );
}

int
JpegFileIO::WriteRGBA( const string& filename,
					   const int width, const int height,
					   const vector<unsigned char>& image,
					   const int quality )
{
	return WriteRGBA( filename, width, height, &image[ 0 ], quality );
}

int
JpegFileIO::Write( const string& filename,
				   const int width, const int height,
				   const unsigned char * const image,
				   const int quality )
{
	return WriteRGB( filename, width, height, image, quality );
}

int
JpegFileIO::WriteRGB( const string& filename,
					  const int width, const int height,
					  const unsigned char * const buf,
					  const int quality )
{
	struct	jpeg_compress_struct	cinfo;
	struct	jpeg_error_mgr	jerr;

	cinfo.err = jpeg_std_error( &jerr );
	jpeg_create_compress( &cinfo );
	
	FILE	*outfile;
	if (( outfile = fopen( filename.c_str(), "wb" )) == NULL ) {
		cerr << "JpegFileIO::Write(): cannot open " << filename << endl;
		return -1;
	}
	jpeg_stdio_dest( &cinfo, outfile );

	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults( &cinfo );
	jpeg_set_quality( &cinfo, quality, TRUE );

	int	row_stride;
	JSAMPROW	row_pointer[ 1 ];

	jpeg_start_compress( &cinfo, TRUE );
	row_stride = width * 3;

	int	y, y_step;

	if ( origin == ORIGIN_LEFT_BOTTOM ) {
		y = height - 1;
		y_step = -1;
	} else {
		y = 0;
		y_step = 1;
	}
	
	while ( cinfo.next_scanline < static_cast<unsigned int>( height )) {
		row_pointer[ 0 ] = reinterpret_cast<JSAMPLE *>
			( const_cast<unsigned char *>( buf + y * width * 3 ));
		jpeg_write_scanlines( &cinfo, row_pointer, 1 );
		y += y_step;
	}

	jpeg_finish_compress( &cinfo );
	jpeg_destroy_compress( &cinfo );
	fclose( outfile );

	return 0;
}

int
JpegFileIO::WriteRGBA( const string& filename,
					   const int width, const int height,
					   const unsigned char * const image,
					   const int quality )
{
	vector<unsigned char>	tmp( width * height * 3 );

	const unsigned char	*src = image;
	unsigned char	*dst = &tmp[ 0 ];
	for ( int i = 0; i < width * height; i++ ) {
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		src++;
	}

	int	r = WriteRGB( filename, width, height, tmp, quality );

	return r;
}


#include	<iomanip>

int
main( int argc, char **argv )
{
	JpegFileIO	j( JpegFileIO::ORIGIN_LEFT_BOTTOM );
	int	w, h;
	vector<unsigned char>	image;

	j.ReadRGBA( argv[ 1 ], w, h, image );

	cout << "buffer size: " << image.size() << endl;
	cout << "width: " << w << endl;
	cout << "height: " << h << endl;
	cout << "image: ";

	ios::fmtflags	f = cout.flags();
	for ( int i = 0; i < 16; i++ ) {
		cout << hex << setw( 2 ) << setfill( '0' )
			 << static_cast<int>( image[ i ] ) << ",";
	}
	cout.setf( f );
	cout << "..." << endl;

    for(int i = 0; i < image.size(); i++) {
//        if (!(i % 3))
//            continue;
        image[i] = 255 - image[i];
    }
	
	j.WriteRGBA( argv[ 2 ], w, h, image, 80 );

	return 0;
}

