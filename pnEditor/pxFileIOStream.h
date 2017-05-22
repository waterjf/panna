#ifndef pxFileIOStream_h__
#define pxFileIOStream_h__

#include "pxShapeIOStream.h"

class wxFileStream;
class pxFileIOStream :
	public pxShapeIOStream
{
public:
	pxFileIOStream(wxFile* f);
	virtual ~pxFileIOStream(void);

public:
	virtual int Write( void* buff, size_t size );
	virtual int Read( void* buff, size_t size );
	virtual off_t Seek( off_t offset );

protected:
	wxFile* m_pFile;
};

#endif // pxFileIOStream_h__
