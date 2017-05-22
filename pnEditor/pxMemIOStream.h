#ifndef pxMemIOStream_h__
#define pxMemIOStream_h__

#include "pxShapeIOStream.h"

class pxMemIOStream : public pxShapeIOStream
{
public:
	pxMemIOStream();
	virtual ~pxMemIOStream();

public:
	void Reset();
	int Alloc(int size);

	virtual int Write( void* buff, size_t size );
	virtual int Read( void* buff, size_t size );
	virtual off_t Seek( off_t offset );

	size_t Size() const { return m_size; }

protected:
	void Size(size_t val) { m_size = val; }

	void* m_pBuffer;
	size_t m_size;
	off_t m_offset;
};

#endif // pxMemIOStream_h__
