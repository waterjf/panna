#include "precomp.h"
#include "pxFileIOStream.h"
#include "wx/file.h"

pxFileIOStream::pxFileIOStream(wxFile* f)
{
	m_pFile = f;
}

pxFileIOStream::~pxFileIOStream(void)
{

}

int pxFileIOStream::Write( void* buff, size_t size )
{
	int ret = -1;
	if(m_pFile)
	{
		ret = m_pFile->Write(buff, size);
	}

	return ret;
}

int pxFileIOStream::Read( void* buff, size_t size )
{
	int ret = -1;
	if(m_pFile)
	{
		ret = m_pFile->Read(buff, size);
	}

	return ret;
}

off_t pxFileIOStream::Seek( off_t offset )
{
	return m_pFile->Seek(offset);
}
