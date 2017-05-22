#ifndef pnShapeEditorModel_h__
#define pnShapeEditorModel_h__

#include "pnShapeModel.h"

class pnShapeEditorModel :
	public pnShapeModel
{
public:
	pnShapeEditorModel(void);
	virtual ~pnShapeEditorModel(void);

	void SelectShape(shape_list& shapes, const pxPoint& leftup, const pxPoint& rightbottom);
	void GetShapesBoundRect(pxPoint& leftup, pxPoint& rightbottom, int enlarge);

	static void TranslateShapes(const shape_list& shapes, const pxPoint& offset);//Æ½ÒÆ
	static void RotateShapes(const shape_list& shapes, const pxPoint& refPoint, double angle);//Ðý×ª
	static void ScaleShapes(const shape_list& shapes, const pxPoint& rotate_ref, double rotation,
		const pxPoint& scale_ref, const pxPoint& scale); //Ëõ·Å
	static void GetShapesBoundRect(const shape_list& shapes, pxPoint& leftup, pxPoint& rightbottom, int enlarge);
};

#endif // pnShapeEditorModel_h__
