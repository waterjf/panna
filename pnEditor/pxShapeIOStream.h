// pxShapeIOStream.h: interface for the pxShapeIOStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PXSHAPESIOSTREAM_H__4C9F05E0_CB3B_4E61_B3FE_53C9EF0B482B__INCLUDED_)
#define AFX_PXSHAPESIOSTREAM_H__4C9F05E0_CB3B_4E61_B3FE_53C9EF0B482B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CA_RETURN(func, accum, ret) \
	do{ \
	ret = (func); \
	if(ret < 0) \
	return ret; \
		else \
		accum = accum + ret; \
	}while(0)

#define CA_WRITE_BYTES_RETURN(stream, target, target_len, accum, ret) \
	do{ \
	ret = (stream)->Write((void*)&(target), target_len); \
	if(ret < 0) \
	return ret; \
	else \
	accum = accum + ret; \
	}while(0)

#define CA_WRITE_RETURN(stream, target, accum, ret) \
	CA_WRITE_BYTES_RETURN(stream, target, sizeof(target), accum, ret)

#define CA_READ_BYTES_RETURN(stream, target, target_len, accum, ret) \
	do{ \
	ret = (stream)->Read(&(target), target_len); \
	if(ret < 0) \
	return ret; \
	else \
	accum = accum + ret; \
	}while(0)

#define CA_READ_RETURN(stream, target, accum, ret) \
	CA_READ_BYTES_RETURN(stream, target, sizeof(target), accum, ret)

class pxShapeIOStream  
{
public:
	pxShapeIOStream();
	virtual ~pxShapeIOStream();

public:
	virtual int Write( void* buff, size_t size ) = 0;
	virtual int Read( void* buff, size_t size ) = 0;
	virtual off_t Seek( off_t offset ) = 0;
};

#endif // !defined(AFX_PXSHAPESIOSTREAM_H__4C9F05E0_CB3B_4E61_B3FE_53C9EF0B482B__INCLUDED_)
