// JpegFileIO.h
//
// need libjpeg - http://www.ijg.org/

#ifndef	JPEG_FILE_IO_H
#define	JPEG_FILE_IO_H

#include	<string>
#include	<vector>

class	JpegFileIO {
protected:
	static const int	JPEG_DEFAULT_QUALITY = 80;

	int	origin;
	
public:
	enum {
		ORIGIN_LEFT_TOP = 0,
		ORIGIN_LEFT_BOTTOM = 1
	};
	
	JpegFileIO( const int n_origin = JpegFileIO::ORIGIN_LEFT_TOP );
	virtual	~JpegFileIO();

	virtual int	Read( const std::string& filename,
					  int& width, int& height, 
					  std::vector<unsigned char>& image );
	virtual int	ReadRGB( const std::string& filename,
						 int& width, int& height, 
						 std::vector<unsigned char>& image );
	virtual int	ReadRGBA( const std::string& filename,
						  int& width, int& height, 
						  std::vector<unsigned char>& image );

	virtual unsigned char	*Read( const std::string& filename,
								   int& width, int& height );
	virtual unsigned char	*ReadRGB( const std::string& filename,
									  int& width, int& height );
	virtual unsigned char	*ReadRGBA( const std::string& filename,
									   int& width, int& height );

	virtual	int Write( const std::string& filename,
					   const int width, const int height,
					   const std::vector<unsigned char>& image,
					   const int quality = JPEG_DEFAULT_QUALITY );
	virtual int	WriteRGB( const std::string& filename,
						  const int width, const int height,
						  const std::vector<unsigned char>& image,
						  const int quality = JPEG_DEFAULT_QUALITY );
	virtual int	WriteRGBA( const std::string& filename,
						   const int width, const int height,
						   const std::vector<unsigned char>& image,
						   const int quality = JPEG_DEFAULT_QUALITY );

	virtual	int Write( const std::string& filename,
					   const int width, const int height,
					   const unsigned char * const image,
					   const int quality = JPEG_DEFAULT_QUALITY );
	virtual int	WriteRGB( const std::string& filename,
						  const int width, const int height,
						  const unsigned char * const image,
						  const int quality = JPEG_DEFAULT_QUALITY );
	virtual int	WriteRGBA( const std::string& filename,
						   const int width, const int height,
						   const unsigned char * const image,
						   const int quality = JPEG_DEFAULT_QUALITY );
};

#endif	// JPEG_FILE_IO_H
