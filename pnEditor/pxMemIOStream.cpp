#include "precomp.h"
#include "pxMemIOStream.h"

pxMemIOStream::pxMemIOStream()
{
	m_size = 0;
	m_pBuffer = 0;

	m_offset = 0;
}

pxMemIOStream::~pxMemIOStream()
{
	if(m_pBuffer)
		delete m_pBuffer;

	m_size = 0;
	m_pBuffer = 0;

	m_offset = 0;
}

int pxMemIOStream::Alloc( int size )
{
	if(m_pBuffer)
		delete m_pBuffer;

	m_pBuffer = 0;
	m_offset = 0;

	m_size = size;
	if(size > 0)
	{
		m_pBuffer = (void*) new char[size];
	}

	return m_pBuffer != 0;
}

int pxMemIOStream::Write( void* buff, size_t size )
{
	if(!m_pBuffer) //can be used to count size
		return size;

	if(m_offset + size > m_size)
		return -1; //out of space

	memcpy((char*)m_pBuffer + m_offset, buff, size);
	m_offset = m_offset + size;

	return size;
}

int pxMemIOStream::Read( void* buff, size_t size )
{
	size_t ret = m_size - m_offset;
	if(ret > size)
		ret = size;

	if(!m_pBuffer) //can be used to count size
		return size;	

	memcpy(buff, (char*)m_pBuffer + m_offset, ret);
	m_offset += ret;

	return (int)ret;
}

off_t pxMemIOStream::Seek( off_t offset )
{
	if(offset < 0)
		m_offset = 0;
	else if(offset < (off_t)m_size)
		m_offset = offset;

	return m_offset;
}

void pxMemIOStream::Reset()
{
	Alloc(0);
}
