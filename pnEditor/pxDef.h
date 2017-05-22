#ifndef _PX_SHAPE_DEF_H_
#define _PX_SHAPE_DEF_H_

#ifndef TRUE
#define TRUE		1
#endif

#ifndef FALSE
#define FALSE		0
#endif

#ifndef NULL
#define NULL		0
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//data type definition
typedef char			sint8;
typedef char			uint8;
typedef short			sint16;
typedef sint16			int16;
typedef unsigned short	uint16;
typedef int				sint32;
typedef sint32			int32;
typedef unsigned int	uint32;
typedef long			slong;
typedef unsigned long	ulong;
typedef int				sint;
typedef unsigned int	uint;
typedef unsigned char	byte;
typedef double			float64;
typedef __int64			sint64;

#ifndef arr_size
#	define arr_size(a) (sizeof(a)/sizeof(a[0]))
#endif

#ifndef PI
#define PI 3.1415926535898
#endif

typedef double pxCoord;

typedef wxPoint2DDouble pxPoint;
typedef wxRect2DDouble pxRect;
typedef wxSize pxSize;

typedef struct 
{
	byte red;
	byte green;
	byte blue;
	byte alpha;
}pxColour;

#define PX_LINE_TERMINAL_RECT	0
#define PX_LINE_TERMINAL_CICLE	1

typedef struct
{
	byte style;
	byte width;
	byte terminal_style;
	
	byte start_style;
	byte end_style;
	byte start_size;
	byte end_size;
	
	pxColour color;
}pxLineStyle;

typedef struct  
{
	byte pattern;
	pxColour pattern_color;
	pxColour color;
}pxFillStyle;

#define MAX_FONT_FACE_NAME_LEN 63

typedef struct 
{
	char family;
	char font_style;
	char font_weight;
	char font_underline;
	char font_encoding;
	float font_size;
	char font_face_name[MAX_FONT_FACE_NAME_LEN+1];
}pxFontInfo;

typedef struct 
{
	pxFontInfo text_font;
	pxColour text_color;
}pxTextStyle;

typedef enum
{
	pxShapeType_Invalid = 0,
	pxShapeType_MIN,
	pxShapeType_LineSec = pxShapeType_MIN,
	pxShapeType_Rect,
	pxShapeType_Text,
	pxShapeType_Ellipse,
	pxShapeType_Arc,
	pxShapeType_BezierArc,
	pxShapeType_Img,
	pxShapeType_Animator,
	pxShapeType_ConnectedLines,
	pxShapeType_Group,
	pxShapeType_MAX = pxShapeType_Group,
	pxShapeType_COUNT,
}pxShapeType;

typedef enum
{
	pxTransformMark_None = 0,
	pxTransformMark_Move, //�ƶ�
	pxTransformMark_Rotate,
	pxTransformMark_NorthWest, //��������
	pxTransformMark_West, //��������
	pxTransformMark_SouthWest, //����������
	pxTransformMark_South, //��������
	pxTransformMark_SouthEast, //����������
	pxTransformMark_East, //��������
	pxTransformMark_NorthEast, //����������
	pxTransformMark_North, //��������
	pxTransformMark_MovePoint_0, //�ƶ������0
	pxTransformMark_MovePoint_1, //�ƶ������1
	pxTransformMark_MovePoint_2, //�ƶ������2
	pxTransformMark_MovePoint_3, //�ƶ������3
}pxTransformMark;

typedef enum
{
	pxAlignNone = 0,
	pxAlignLeft,
	pxAlignTop = pxAlignLeft,
	pxAlignHCenter,
	pxAlignVCenter = pxAlignHCenter,
	pxAlignRight,
	pxAlignBottom = pxAlignRight,
}pxAlignmentType;

#define MAX_SHAPE_POINT 0x10000 //һ��ͼԪ���65536����
#define COORD_EPSILON 1E-9 //��С����
#define COORD_RESOLUTION 1E-3 //��С�ֱ���
#define PX_TOLERANCE 5.0
#define PX_TOLERANCE_HALF (PX_TOLERANCE / 2)
#define PX_TOLERANCE_2 (PX_TOLERANCE * 1.414) //����2��

#endif
