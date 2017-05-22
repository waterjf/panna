#ifndef pnShapeEditorView_h__
#define pnShapeEditorView_h__

#include "pnShapeView.h"
#include "pxShapePool.h"
#include "pnUndoRedo.h"
#include "pxMemIOStream.h"

#define ZOOM_STEP 0.1

class pnShapeEditorModel;
class pxShapeConnectedLines;
class pnShapeEditorView :
	public pnShapeView
{
public:
	pnShapeEditorView(pnShapeEditorModel* model);
	virtual ~pnShapeEditorView(void);

public:
	void InitEditorView(pnShapeEditorModel* model);

	virtual void SetMarkCursor(pxTransformMark cursor_type) = 0;
	virtual void SetToolCursor(pxShapeType cursor_type) = 0;

	virtual void ResetView(bool clear_selection = true);

	virtual void MouseLeftDown(const pxPoint& point);

	virtual void MouseLeftUp(const pxPoint& point);
	virtual void MouseMotion(const pxPoint& point);

	virtual void Paint( pxCanvas* canvas );

	virtual pnShapeModel* GetShapeModel();

public:
	bool SetCurrentTool( int id );

	bool GetModified();
	bool CanPaste();
	void SetModified(bool bModified);
	void OperationCancel(bool clear_selecetion);
	void OperationDelete();
	void OperationMove(int xstep, int ystep);
	void OperationUndo();
	bool CanUndo();
	void OperationRedo();
	bool CanRedo();
	void OperationCut();
	void OperationCopy();
	void OperationPaste();
	void OperationSelectAll();
	//
	void ZoomIn();
	void ZoomOut();
	void Zoom100();
	void ZoomFit();
	//
	void OperationBringSelection(int direction); //负方向表示下沉，正方向表示上升
	void OperationBringToTop();
	void OperationBringToBottom();
	void OperationBringUp();
	void OperationBringDown();

	void OperationAssemble();
	bool CanDissemble();
	void OperationDissemble();
	void OperationAlign(pxAlignmentType x, pxAlignmentType y);
	void OperationAlignLeft();
	void OperationAlignRight();
	void OperationAlignHCenter();
	void OperationAlignTop();
	void OperationAlignBottom();
	void OperationAlignVCenter();
	void OperationSameDistance(bool same_horz, bool same_vert);
	void OperationHSameDistance();
	void OperationVSameDistance();
	void OperationSameSize(bool same_width, bool same_height);
	void OperationSameWidth();
	void OperationSameHeight();
	void OperationSameSize();
	void OperationRotate(double angle);
	void OperationRotateN90();
	void OperationRotateP90();
	void OperationFlip(bool flip_horz, bool flip_vert);
	void OperationFlipVert();
	void OperationFlipHorz();

	//
	int GetSelectionCount();

protected: //for painting
	//画单选图元标记
	void DrawShapeSelectionMark( pxShapeBase* shape, pxCanvas* canvas);
	//多选图元时，逐个画图元选择框
	void DrawShapeMultiSelMark( pxShapeBase* shape, pxCanvas* canvas, bool bMainTarget = false );
	//画选择框
	void DrawMouseMotionRect(pxCanvas* canvas, const pxPoint& start, const pxPoint& end);

	void DrawSelectMark( pxCanvas* canvas );
	void PaintMovingShapes( pxCanvas* canvas );

	bool UsingLineTool();

protected: //for selecting
	void BeginRectSelecting();
	void ContinueRectSelecting();
	void DoneRectSelecting();

	void SelectByPoint(const pxPoint& point);
	void SelectByRect(const pxPoint& lu, const pxPoint& rb);

	shape_list m_selected_shapes;
	pxShapeBase* m_moving_shape; //鼠标悬停图元

	bool m_bRectSelecting; //框选进行标记

	pxPoint m_MMotion_Point; //鼠标当前位置
	pxPoint m_MLD_point; //鼠标左键按下时的坐标
	pxPoint m_MLU_point; //鼠标左键弹起时的坐标
	
protected: // for bounding
	//计算多选图元的外边框m_multisel_leftup,m_multisel_rightbottom
	void RefreshSelectedShapesBoundRect();

	//多选图元的外接边框
	pxPoint m_shapes_rect[4];

protected:// for transforming
	void Begin2DTransform();
	void Continue2DTranform();
	void Done2DTransform();

	void BeginMovingShapes();
	void ContinueMovingShapes();
	void DoneMovingShapes();

	void GetTranformScale(pxPoint& rotate_ref, double& rotation, 
		pxPoint& scale_ref, pxPoint& scale);
	void GetTranformRotation(pxPoint& refPoint, double& rotation);

	void TransformSelectedShapes();

	void DoTransformTest();
	pxTransformMark TransformTestOnSelection(const pxPoint& point);
	void DoLinePointTransformTest();
	pxTransformMark LinePointTransformTest(const pxPoint& point);

	void TransformMouseCoord(pxPoint& point) const;
	void UnTransformMouseCoord(pxPoint& point) const;
	void RememberMarkCursor(pxTransformMark mark);

	void ClearTranformingBackup();
	void CreateTransformingBackup();
	void RestoreTransformingBackup();

	bool m_b2DTransforming; //2D变换进行标记
	pxTransformMark m_transform_mark; //2D变换类型

	shape_list m_transforming_backup;

protected: //for shape creating
	void BeginCreatingShape();
	void ContinueCreatingShape();
	pxShapeBase* DoneCreatingShape();
	pxShapeConnectedLines* TryCreatingConnectedLines(pxShapeBase* cur_shape, pxShapeBase* new_shape);
	pxShapeConnectedLines* CreateConnectedLines(pxShapeBase* cur_shape, pxShapeBase* new_shape, int end_point_index);

	pxShapeType m_eCurrentShapeType; 
	pxShapeBase* m_pShapeInDrawing;

protected: //for undo redo
	pnUndoRedo m_UndoRedo;

protected: //for copy paste
	pxMemIOStream m_clip_board;
	int m_paste_translate;

protected:
	pnShapeEditorModel* m_pShapeModel;
	bool m_bModified;
};

#endif // pnShapeEditorView_h__
