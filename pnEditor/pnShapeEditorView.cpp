#include "precomp.h"
#include "pnShapeEditorView.h"
#include "pnShapeEditorModel.h"
#include "pxCanvasWx2DEx.h"
#include "pxShapeConnectedLines.h"

pnShapeEditorView::pnShapeEditorView(pnShapeEditorModel* model)
{
	m_pShapeInDrawing = 0;
	m_pShapeModel = model;
	m_UndoRedo.Init(model, &m_selected_shapes);
	m_bModified = false;
	m_paste_translate = 0;

	ResetView();
}

pnShapeEditorView::~pnShapeEditorView(void)
{
	m_selected_shapes.clear();
}

void pnShapeEditorView::DrawShapeSelectionMark( pxShapeBase* shape, pxCanvas* canvas )
{
	if(UsingLineTool())
		shape->DrawControlPointMark(canvas);
	else
		shape->DrawSelectedMark(canvas);
}

void pnShapeEditorView::DrawShapeMultiSelMark( pxShapeBase* shape, pxCanvas* canvas, bool bMainTarget )
{
	shape->DrawMultipleSelectedMark(canvas, bMainTarget);
}

void pnShapeEditorView::Paint( pxCanvas* pCanvas )
{
	wxBitmap bmp;
	wxSize size = pCanvas->GetSize();

	bmp.Create(size);
	wxMemoryDC* memdc = new wxMemoryDC(bmp);
	pxCanvasWx2DEx canvas(memdc);

	//clear bkground
	canvas.SetBrush(*wxWHITE_BRUSH);
	canvas.SetPen(*wxWHITE_PEN);
	canvas.DrawRectangle(0, 0, size.x, size.y);

	canvas.SetTransform(m_translate, m_scale);

	//paint shapes
	pnShapeView::Paint(&canvas);

	//paint selected marks
	DrawSelectMark(&canvas);

	//draw select rect
	if(m_bRectSelecting)
		DrawMouseMotionRect(&canvas, m_MLD_point, m_MLU_point);

	if(m_b2DTransforming && m_transform_mark == pxTransformMark_Move)
		PaintMovingShapes(&canvas);

	if(m_pShapeInDrawing)
	{
		m_pShapeInDrawing->Draw(&canvas);
		m_pShapeInDrawing->DrawSelectedMark(&canvas, false, false);
	}
	//swap buff
	pCanvas->Blit(0,0,size.GetWidth(),size.GetHeight(),&canvas,0,0,wxCOPY,false,wxDefaultCoord,wxDefaultCoord);

	memdc->SelectObject(wxNullBitmap);
}

void pnShapeEditorView::MouseLeftDown( const pxPoint& p )
{
	pxPoint point = p;
	TransformMouseCoord(point);

	m_bRectSelecting = false;
	m_b2DTransforming = false;
	ClearTranformingBackup();

	m_MLD_point = point;
	m_MLU_point = m_MLD_point;

	if(m_eCurrentShapeType > pxShapeType_Invalid)
	{
		if(m_transform_mark >= pxTransformMark_MovePoint_0)
		{//移动图元的点
			Begin2DTransform();
		}
		else
		{//绘制新图元
			BeginCreatingShape();
		}
	}
	else
	{
		pxTransformMark mark = TransformTestOnSelection(point);
		RememberMarkCursor(mark);

		if(mark > pxTransformMark_Move)
		{//图元2D变换
			Begin2DTransform();
		}
		else
		{
			BeginMovingShapes();

			if(!m_moving_shape)
			{//没有悬停目标，开始拉框
				BeginRectSelecting();
			}
			else
			{//开始点选目标

			}
		}
	}
}

void pnShapeEditorView::MouseMotion( const pxPoint& p )
{
	m_MMotion_Point = p;
	TransformMouseCoord(m_MMotion_Point);

	m_MLU_point = m_MMotion_Point;

	if(m_eCurrentShapeType > pxShapeType_Invalid)
	{
		if(m_pShapeInDrawing)
		{//绘制新图元
			ContinueCreatingShape();
		}
		else
		{
			if(m_b2DTransforming)
			{//线条图元移动点
				Continue2DTranform();
			}
			else
			{
				DoLinePointTransformTest();
			}
		}
	}
	else
	{
		if(m_bRectSelecting)
		{//拉框中。。。
			ContinueRectSelecting();
		}
		else
		{
			if(m_moving_shape)
			{//拖动目标中...
				ContinueMovingShapes();
			}

			if(m_b2DTransforming)
			{//2D变换：拉伸
				Continue2DTranform();
			}
			else
			{//测试鼠标形状
				DoTransformTest();
			}
		}
	}

	//store current mouse position
	m_MMotion_Point = p;
}

void pnShapeEditorView::MouseLeftUp( const pxPoint& p )
{
	m_MLU_point = p;
	TransformMouseCoord(m_MLU_point);

	if(m_eCurrentShapeType > pxShapeType_Invalid)
	{//绘制新图元
		if(m_pShapeInDrawing)
		{
			if(!DoneCreatingShape())
			{
				SelectByPoint(m_MLU_point);
			}				
		}
	}
	else
	{
		if(m_bRectSelecting)
		{//框选结果
			DoneRectSelecting();
		}
		else
		{
			if(m_b2DTransforming)
			{//2D变换生效
				if(m_transform_mark == pxTransformMark_Move)
					DoneMovingShapes();
				else
					Done2DTransform();
			}
			else
			{//点选结果
				SelectByPoint(m_MLU_point);
			}
		}
	}

	//清理状态
	m_moving_shape = 0;
	m_bRectSelecting = false;
	m_b2DTransforming = false;
	ClearTranformingBackup();
}

void pnShapeEditorView::OperationCancel(bool clear_selecetion)
{
	ResetView(clear_selecetion);
	RememberMarkCursor(pxTransformMark_None);
	RefreshClientArea();
}

void pnShapeEditorView::DrawMouseMotionRect( pxCanvas* canvas, const pxPoint& start, const pxPoint& end )
{ 
	pxPoint point[4];
	px2DGeometry::Init4PointRect(start, end, point);
	pxShapeBase::DrawSelectedMark(canvas, point, false, false);
}

void pnShapeEditorView::DrawSelectMark( pxCanvas* canvas )
{
	int selcount = m_selected_shapes.GetCount();
	shape_iterator itor = m_selected_shapes.begin();
	if(selcount == 1)
		DrawShapeSelectionMark(*itor, canvas);
	else if(selcount > 1)
	{
		bool bMainTarget = true;

		while( itor != m_selected_shapes.end() )
		{
			DrawShapeMultiSelMark(*itor, canvas, bMainTarget);
			bMainTarget = false;
			itor ++;
		}

		pxShapeBase::DrawSelectedMark(canvas, m_shapes_rect);
	}
}

void pnShapeEditorView::RememberMarkCursor( pxTransformMark mark )
{
	if(m_transform_mark != mark)
	{
		m_transform_mark = mark;
		if(m_transform_mark == pxTransformMark_None)
			SetToolCursor(m_eCurrentShapeType);
		else
			SetMarkCursor(m_transform_mark);
	}
}

void pnShapeEditorView::PaintMovingShapes( pxCanvas* canvas )
{
	pxPoint translate, scale;
	canvas->GetTransform(translate, scale);
	canvas->SetTransform(translate + (m_MLU_point - m_MLD_point), scale);

	pxShapeBase* shape = 0;
	shape_iterator itor = m_selected_shapes.begin();

	while(itor != m_selected_shapes.end())
	{
		shape = *itor;

		shape->Draw(canvas);

		itor ++;
	}

	if(m_selected_shapes.GetCount() == 1)
	{
		shape = *m_selected_shapes.begin();
		shape->DrawSelectedMark(canvas, false, false);
	}
	else
	{
		pxShapeBase::DrawSelectedMark(canvas, m_shapes_rect, false, false);
	}

	canvas->SetTransform(translate, scale);
}

void pnShapeEditorView::RefreshSelectedShapesBoundRect()
{
	pnShapeEditorModel::GetShapesBoundRect(m_selected_shapes, m_shapes_rect[0], m_shapes_rect[2], 4);
	px2DGeometry::Init4PointRect(m_shapes_rect[0], m_shapes_rect[2], m_shapes_rect);
}

void pnShapeEditorView::CreateTransformingBackup()
{
	ClearTranformingBackup();

	shape_iterator itor = m_selected_shapes.begin();
	while(itor != m_selected_shapes.end())
	{
		pxShapeBase* shape = *itor;

		pxShapeBase* backup = m_pShapeModel->CreateShape(shape->GetShapeType());
		backup->Clone(shape);
		m_transforming_backup.push_back(backup);

		itor ++;
	}
}

void pnShapeEditorView::RestoreTransformingBackup()
{
	shape_iterator itor = m_selected_shapes.begin();
	shape_iterator itor1 = m_transforming_backup.begin();

	while(itor != m_selected_shapes.end())
	{
		pxShapeBase* shape = *itor;
		pxShapeBase* backup = *itor1;
		if(	backup->GetShapeType() != shape->GetShapeType())
		{//不应该发生
			break;
		}

		pxShapeBase::DestructShape(shape);
		pxShapeBase::ConstructShape(shape, backup->GetShapeType(), m_pShapeModel);

		shape->Clone(backup);

		itor ++;
		itor1 ++;
	}

	RefreshSelectedShapesBoundRect();
}


void pnShapeEditorView::GetTranformScale( pxPoint& rotate_ref, double& rotation, 
										 pxPoint& scale_ref, pxPoint& scale )
{
	scale_ref = pxPoint(0,0);
	scale.m_x = 1.0;
	scale.m_y = 1.0;

	if(m_b2DTransforming)
	{
		rotation = 0;
		pxPoint lu,rb;
		pxPoint recPoint[4];

		if(m_selected_shapes.GetCount() == 1)
		{
			pxShapeBase* shape = *m_selected_shapes.begin();
			shape->GetBoundsRect(recPoint);
			rotation = shape->GetRotationAngle();

			lu = recPoint[0];
			rb = recPoint[2];
			px2DGeometry::RotatePoint(lu, -rotation, rb);//旋转为水平矩形
		}
		else
		{
			pnShapeEditorModel::GetShapesBoundRect(m_selected_shapes, lu, rb, 0);
		}

		rotate_ref = lu;

		//原始尺寸太小，给放大到1个像素
		double deltax = rb.m_x - lu.m_x;
		if(deltax < 1) 	deltax = 1;
		double deltay = rb.m_y - lu.m_y;
		if(deltay < 1)	deltay = 1;
		
		//计算坐标移动
		recPoint[0] = m_MLD_point;
		recPoint[1] = m_MLU_point;
		px2DGeometry::RotatePoint(lu, -rotation, recPoint[0]);
		px2DGeometry::RotatePoint(lu, -rotation, recPoint[1]);

		pxPoint offset = recPoint[1] - recPoint[0];

		double scalex = offset.m_x / deltax;
		double scaley = offset.m_y / deltay;

		switch (m_transform_mark)
		{
		case pxTransformMark_NorthWest: //向左上伸
			scale_ref = rb;
			scalex = -scalex;
			scaley = -scaley;
			scale.m_x = 1 + max(scalex, scaley);
			scale.m_y = scale.m_x;
			break;
		case pxTransformMark_West: //向左拉伸
			scale_ref = rb;
			scale.m_x = 1.0 - scalex;
			break;
		case pxTransformMark_SouthWest: //向左下拉伸
			scale_ref = pxPoint(rb.m_x, lu.m_y);
			scalex = -scalex;
			scale.m_x = 1 + max(scalex, scaley);
			scale.m_y = scale.m_x;
			break;
		case pxTransformMark_South: //向下拉伸
			scale_ref = lu;
			scale.m_y = 1.0 + scaley;
			break;
		case pxTransformMark_SouthEast: //向右下拉伸
			scale_ref = lu;
			scale.m_x = 1.0 + max(scalex, scaley);
			scale.m_y = scale.m_x;
			break;
		case pxTransformMark_East: //向右拉伸
			scale_ref = lu;
			scale.m_x = 1.0 + scalex;
			break;
		case pxTransformMark_NorthEast: //向右上拉伸
			scale_ref = pxPoint(lu.m_x, rb.m_y);
			scaley = -scaley;
			scale.m_x = 1 + max(scalex, scaley);
			scale.m_y = scale.m_x;
			break;
		case pxTransformMark_North: //向上拉伸
			scale_ref = rb;
			scale.m_y = 1.0 - scaley;
			break;
		default:
			break;
		}

		//保证不缩到1个像素以下
		deltax = 1 / deltax;
		deltay = 1 / deltay;

		if(scale.m_x < deltax)
			scale.m_x = deltax;
		if(scale.m_y < deltay)
			scale.m_y = deltay;
	}
}

void pnShapeEditorView::GetTranformRotation( pxPoint& refPoint, double& rotation )
{
	refPoint = pxPoint(0,0);
	rotation = 0;

	if(m_b2DTransforming)
	{
		pxPoint lu,rb;
		pnShapeEditorModel::GetShapesBoundRect(m_selected_shapes, lu, rb, 0);

		refPoint = (lu + rb) / 2;

		double angle0 = px2DGeometry::LineAngle(refPoint, m_MLD_point);
		double angle1 = px2DGeometry::LineAngle(refPoint, m_MLU_point);
		rotation = angle1 - angle0;
	}
}

void pnShapeEditorView::TransformSelectedShapes()
{
	if(m_transform_mark == pxTransformMark_Move)
	{//平移
		m_pShapeModel->TranslateShapes(m_selected_shapes, m_MLU_point - m_MLD_point);
	}
	else if(m_transform_mark == pxTransformMark_Rotate)
	{//旋转
		pxPoint refPoint;
		double rotation;
		GetTranformRotation(refPoint, rotation);
	
		m_pShapeModel->RotateShapes(m_selected_shapes, refPoint, rotation);
		
		int n;
		for(n = 0; n < 4; n ++)
		{
			px2DGeometry::RotatePoint(refPoint, rotation, m_shapes_rect[n]);
		}
	}
	else if(m_transform_mark >= pxTransformMark_MovePoint_0)
	{//设点
		pxShapeBase* shape = *m_selected_shapes.begin();
		shape->SetContrlPoint(m_transform_mark - pxTransformMark_MovePoint_0, m_MLU_point);
	}
	else
	{//缩放
		pxPoint rotate_ref;
		double rotation;
		pxPoint scale_ref;
		pxPoint scale;

		GetTranformScale(rotate_ref, rotation, scale_ref, scale);

		m_pShapeModel->ScaleShapes(m_selected_shapes, rotate_ref, rotation,
			scale_ref, scale);
		RefreshSelectedShapesBoundRect();
	}
}

pxTransformMark pnShapeEditorView::TransformTestOnSelection(const pxPoint& point)
{
	pxTransformMark mark = pxTransformMark_None;

	shape_iterator itor = m_selected_shapes.begin();
	pxShapeBase* shape = 0;

	int selcount = m_selected_shapes.GetCount();
	if(selcount == 1)
	{//单选
		shape = *itor;
		mark = shape->TransformMarkTest(point);
	}
	else if(selcount > 1)
	{//多选 鼠标落在多选方框哪个角
		mark = pxShapeBase::G_TransformMarkTest(point, m_shapes_rect);
		if(mark == pxTransformMark_None)
		{
			pxPoint pointAnchor, pointC;
			pxShapeBase::GetRotationControlPoints(m_shapes_rect, pointAnchor, pointC);

			if(px2DGeometry::PointNearPoint(point, pointAnchor))
				mark = pxTransformMark_Rotate;
		}
	}
	else
	{

	}

	return mark;
}

pxTransformMark pnShapeEditorView::LinePointTransformTest( const pxPoint& point )
{
	pxTransformMark mark = pxTransformMark_None;

	if(UsingLineTool() && m_selected_shapes.GetCount() == 1)
	{
		shape_iterator itor = m_selected_shapes.begin();
		pxShapeBase* shape = *itor;
		mark = shape->ControlPointTransformMarkTest(point);
		if(mark >= pxTransformMark_MovePoint_0)
		{
			pxPoint endPoints[2];
			if(shape->GetEndPoint(endPoints) && //有端点
				px2DGeometry::PointDistance(endPoints[0], endPoints[1]) > COORD_EPSILON) //端点不重合
			{
				if(px2DGeometry::PointNearPoint(point, endPoints[0]) ||
					px2DGeometry::PointNearPoint(point, endPoints[1]) )
				{//画线条是，端点不成为控制点，这样才能画出ConnnetedLines
					mark = pxTransformMark_None;
				}
			}
		}
	}

	return mark;
}

void pnShapeEditorView::TransformMouseCoord( pxPoint& point ) const
{
	point.m_x /= m_scale.m_x;
	point.m_y /= m_scale.m_y;

	point.m_x -= m_translate.m_x;
	point.m_y -= m_translate.m_y;
}

void pnShapeEditorView::UnTransformMouseCoord( pxPoint& point ) const
{
	point.m_x += m_translate.m_x;
	point.m_y += m_translate.m_y;

	point.m_x *= m_scale.m_x;
	point.m_y *= m_scale.m_y;
}

void pnShapeEditorView::ClearTranformingBackup()
{
	shape_iterator itor = m_transforming_backup.begin();
	while(itor != m_transforming_backup.end())
	{
		m_pShapeModel->FreeShape(*itor);
		itor++;
	}

	m_transforming_backup.clear();
}

void pnShapeEditorView::OperationDelete()
{
	if(m_selected_shapes.GetCount() > 0)
	{
		//记录undo
		m_UndoRedo.RecordAction(ActionType_UnSelect, m_selected_shapes);
		m_UndoRedo.RecordAction(ActionType_Delete, m_selected_shapes, 2);
		m_pShapeModel->UnregisterShapes(m_selected_shapes);

		SetModified(true);

		//不用真正删除图元，它们都在Undo记录里
		m_selected_shapes.clear();
		
		RefreshClientArea();

		MouseMotion(m_MMotion_Point);
	}
}

void pnShapeEditorView::OperationSelectAll()
{
	m_selected_shapes.clear();
	m_pShapeModel->SelectShape(m_selected_shapes, pxPoint(-DBL_MAX,-DBL_MAX), pxPoint(DBL_MAX,DBL_MAX));
	RefreshSelectedShapesBoundRect();
	RefreshClientArea();
}

pnShapeModel* pnShapeEditorView::GetShapeModel()
{
	return m_pShapeModel;
}

void pnShapeEditorView::ResetView(bool clear_selection)
{
	if(m_b2DTransforming)
	{
		if(m_transform_mark != pxTransformMark_Move)
		{
			RestoreTransformingBackup();
			ClearTranformingBackup();
		}
	}

	if(clear_selection)
		m_selected_shapes.clear();

	m_bRectSelecting = false;
	m_b2DTransforming = false;
	m_transform_mark = pxTransformMark_None;
	m_moving_shape = 0;

	if(m_pShapeInDrawing)
		pxShapeBase::FreeShape(m_pShapeInDrawing);
	m_pShapeInDrawing = 0;
}

bool pnShapeEditorView::SetCurrentTool( int id )
{
	if(id >= pxShapeType_Invalid && id < pxShapeType_COUNT)
	{
		SetToolCursor((pxShapeType)id);
		if(m_eCurrentShapeType != id)
		{
			m_eCurrentShapeType = (pxShapeType)id;
			RefreshClientArea();
		}
		return true;
	}

	return false;
}

bool pnShapeEditorView::CanUndo()
{
	return m_UndoRedo.CanUndo();
}

bool pnShapeEditorView::CanRedo()
{
	return m_UndoRedo.CanRedo();
}

void pnShapeEditorView::OperationUndo()
{
	m_UndoRedo.Undo();
	ResetView(false);
	RefreshClientArea();
}

void pnShapeEditorView::OperationRedo()
{
	m_UndoRedo.Redo();
	ResetView(false);
	RefreshClientArea();
}

void pnShapeEditorView::SelectByPoint( const pxPoint& point )
{
	pxShapeBase* shape = 0;
	if(m_selected_shapes.GetCount() == 1)
		shape = *m_selected_shapes.begin();

	pxShapeBase* selected_shape = m_pShapeModel->HitestShape(point, shape);

	if(selected_shape)
	{
		if(!wxGetKeyState(WXK_SHIFT))
		{
			m_selected_shapes.clear();
			m_selected_shapes.push_back(selected_shape);
		}
		else
		{
			//已经选中再点就取消
			shape_iterator found = m_selected_shapes.find(selected_shape);
			if(found != m_selected_shapes.end())
				m_selected_shapes.remove(found);
			else
				m_selected_shapes.push_front(selected_shape);
		}

		RefreshSelectedShapesBoundRect();
	}

	if(selected_shape != shape)
		RefreshClientArea();
}

void pnShapeEditorView::SelectByRect( const pxPoint& lu, const pxPoint& rb )
{
	pxPoint _lu = lu, _rb = rb;
	px2DGeometry::NormalizeRect(_lu, _rb);

	if(!wxGetKeyState(WXK_SHIFT))
		m_selected_shapes.clear();

	m_pShapeModel->SelectShape(m_selected_shapes, _lu, _rb);
	RefreshSelectedShapesBoundRect();

	RefreshClientArea();
}

bool pnShapeEditorView::UsingLineTool()
{
	return pxShapeBase::IsLineShape(m_eCurrentShapeType);
}

void pnShapeEditorView::BeginCreatingShape()
{
	m_pShapeInDrawing = pxShapeBase::NewShape(m_eCurrentShapeType);
	if(m_pShapeInDrawing)
	{
		pxRect rect;
		rect.SetLeftTop(m_MLD_point);
		rect.SetRightBottom(m_MLU_point);

		m_pShapeInDrawing->SetRect(rect);
		RefreshClientArea();
	}
}

void pnShapeEditorView::Begin2DTransform()
{
	m_b2DTransforming = true;

	//拉伸、旋转或移动点的图元直接变换，所以要backup，取消操作时好恢复
	if(m_transform_mark != pxTransformMark_Move)
		CreateTransformingBackup();
}

void pnShapeEditorView::ContinueCreatingShape()
{
	pxRect rect;
	rect.SetLeftTop(m_MLD_point);
	rect.SetRightBottom(m_MLU_point);
	m_pShapeInDrawing->SetRect(rect);

	RefreshClientArea();
}

void pnShapeEditorView::Continue2DTranform()
{
	if(m_transform_mark > pxTransformMark_Move)
	{//拉伸旋转时直接变换
		RestoreTransformingBackup();
		TransformSelectedShapes();
	}
	RefreshClientArea();
}

void pnShapeEditorView::ContinueMovingShapes()
{
	RememberMarkCursor(pxTransformMark_Move);
	if(!m_b2DTransforming)
	{//进入2D变换的"拖动"模式
		m_b2DTransforming = true;

		if(m_selected_shapes.find(m_moving_shape) == m_selected_shapes.end())
		{//拖动的不是当前目标
			m_selected_shapes.clear();
			m_selected_shapes.push_front(m_moving_shape);
			RefreshSelectedShapesBoundRect();
		}
	}
}

void pnShapeEditorView::BeginMovingShapes()
{
	if(m_transform_mark == pxTransformMark_Move)
		m_moving_shape = *m_selected_shapes.begin();
	else
		m_moving_shape = m_pShapeModel->HitestShape(m_MLD_point, 0);
}

void pnShapeEditorView::DoTransformTest()
{
	pxTransformMark cursor = TransformTestOnSelection(m_MMotion_Point);

	if(cursor == pxTransformMark_None)
	{
		if(m_pShapeModel->HitestShape(m_MMotion_Point, 0))
			cursor = pxTransformMark_Move;
	}

	RememberMarkCursor(cursor);
}

void pnShapeEditorView::DoLinePointTransformTest()
{
	//测试移动图元点
	pxTransformMark mark = LinePointTransformTest(m_MMotion_Point);
	RememberMarkCursor(mark);
}

void pnShapeEditorView::BeginRectSelecting()
{
	m_bRectSelecting = true;
}

void pnShapeEditorView::ContinueRectSelecting()
{
	RefreshClientArea();//使框选得到绘制
}

void pnShapeEditorView::DoneRectSelecting()
{
	SelectByRect(m_MLD_point, m_MLU_point);
}

void pnShapeEditorView::Done2DTransform()
{
	if(m_transform_mark > pxTransformMark_Move)
	{//拉伸旋转时直接变换
		RestoreTransformingBackup();

		//记录undo
		m_UndoRedo.RecordAction(ActionType_Modify, m_selected_shapes);
		SetModified(true);

		TransformSelectedShapes();
		RefreshClientArea();
	}
}

pxShapeBase* pnShapeEditorView::DoneCreatingShape()
{
	pxRect rect;
	rect.SetLeftTop(m_MLD_point);
	rect.SetRightBottom(m_MLU_point);
	m_pShapeInDrawing->SetRect(rect);

	pxPoint d = m_MLD_point;
	pxPoint u = m_MLU_point;
	UnTransformMouseCoord(d);
	UnTransformMouseCoord(u);

	pxShapeBase* selection = 0;

	if(px2DGeometry::PointDistance(d, u) > 4)
	{
		//生成真正的shape
		pxShapeBase* shape = m_pShapeModel->CreateShape(m_pShapeInDrawing->GetShapeType());
		shape->Clone(m_pShapeInDrawing);

		pxShapeConnectedLines* cl = 0;
		if(UsingLineTool())
		{//是否能接成ConnectedLines
			if(m_selected_shapes.GetCount() == 1)
			{
				pxShapeBase* cur_shape = *m_selected_shapes.begin();
				if(cur_shape->IsLineShape())
				{
					cl = TryCreatingConnectedLines(cur_shape, shape);
				}
			}
		}

		if(!cl)
		{
			m_pShapeModel->RegisterShape(shape);
			selection = shape;

			m_UndoRedo.RecordAction(ActionType_UnSelect, m_selected_shapes);

			shape_list list;
			list.push_back(shape);
			m_UndoRedo.RecordAction(ActionType_New, list, 2);
			SetModified(true);
		}
		else
			selection = cl;
	}

	m_selected_shapes.clear();
	if(selection)
		m_selected_shapes.push_back(selection);

	pxShapeBase::FreeShape(m_pShapeInDrawing);
	m_pShapeInDrawing = 0;

	RefreshClientArea();

	return selection;
}

void pnShapeEditorView::DoneMovingShapes()
{
	//记录undo
	m_UndoRedo.RecordAction(ActionType_Modify, m_selected_shapes);
	SetModified(true);

	m_pShapeModel->TranslateShapes(m_selected_shapes, m_MLU_point - m_MLD_point);
	RefreshSelectedShapesBoundRect();

	RefreshClientArea();
}

pxShapeConnectedLines* pnShapeEditorView::TryCreatingConnectedLines
	( pxShapeBase* cur_shape, pxShapeBase* new_shape )
{
	pxShapeConnectedLines* cl = 0;

	pxPoint curshape_endpoint[2];
	if(cur_shape->GetEndPoint(curshape_endpoint))
	{
		pxPoint newshape_endpoint[2];
		new_shape->GetEndPoint(newshape_endpoint);

		int n;
		for(n = 0; n < 2; n ++)
		{
			if(px2DGeometry::PointDistance(newshape_endpoint[0], curshape_endpoint[n]) < 3)
			{
				//端点重合
				new_shape->SetContrlPoint(0, curshape_endpoint[n]);

				cl = CreateConnectedLines(cur_shape, new_shape, n);
				break;
			}
		}
	}

	return cl;
}

pxShapeConnectedLines* pnShapeEditorView::CreateConnectedLines( pxShapeBase* cur_shape, pxShapeBase* new_shape, int end_point_index )
{
	pxShapeConnectedLines* cl = 0;

	if(end_point_index == 0) //调整为首尾相连
		new_shape->RevertEndPoints();

	if(cur_shape->GetShapeType() == pxShapeType_ConnectedLines)
	{
		m_UndoRedo.RecordAction(ActionType_Modify, m_selected_shapes);

		cl = (pxShapeConnectedLines*)cur_shape;
		cl->AddLine(new_shape, end_point_index);

		SetModified(true);
	}
	else
	{
		//记录undo 认为是删除了cur_shape
		m_UndoRedo.RecordAction(ActionType_UnSelect, m_selected_shapes);
		m_UndoRedo.RecordAction(ActionType_Delete, m_selected_shapes);
		m_pShapeModel->UnregisterShapes(m_selected_shapes);

		SetModified(true);

		//开始创建CL
		new_shape->CloneStyle(cur_shape);

		cl = (pxShapeConnectedLines*)m_pShapeModel->CreateShape(pxShapeType_ConnectedLines);
		m_pShapeModel->RegisterShape(cl);

		//重新克隆一个cur_shape
		pxShapeBase* old_shape = cur_shape;
		cur_shape = m_pShapeModel->CreateShape(old_shape->GetShapeType());
		cur_shape->Clone(old_shape);

		shape_list list;
		if(end_point_index == 1)
		{
			list.push_back(cur_shape);
			list.push_back(new_shape);
		}
		else
		{
			list.push_back(new_shape);
			list.push_back(cur_shape);
		}
		cl->Assemble(list);

		//记录undo
		shape_list shapes;
		shapes.push_back(cl);
		m_UndoRedo.RecordAction(ActionType_New, shapes, 3);
		SetModified(true);
	}

	return cl;
}

bool pnShapeEditorView::GetModified()
{
	return m_bModified;
}

void pnShapeEditorView::SetModified( bool bModified )
{
	m_bModified = bModified;
}

int pnShapeEditorView::GetSelectionCount()
{
	return m_selected_shapes.GetCount();
}

void pnShapeEditorView::OperationCut()
{
	OperationCopy();
	OperationDelete();
}

void pnShapeEditorView::OperationCopy()
{
	if(m_selected_shapes.GetCount() > 0)
	{
		m_clip_board.Reset();
		int len = m_pShapeModel->Write(&m_clip_board, m_selected_shapes);
		if(len > 0)
		{
			m_clip_board.Alloc(len);
			m_pShapeModel->Write(&m_clip_board, m_selected_shapes);

			m_paste_translate = 0;
		}
	}
}

void pnShapeEditorView::OperationPaste()
{
	if(CanPaste())
	{
		m_selected_shapes.clear();

		m_clip_board.Seek(0);
		m_pShapeModel->Read(&m_clip_board, m_selected_shapes);
		
		m_paste_translate += 10;

		m_pShapeModel->TranslateShapes(m_selected_shapes, pxPoint(m_paste_translate, m_paste_translate));
		
		shape_iterator itor = m_selected_shapes.begin();
		while(itor != m_selected_shapes.end())
		{
			m_pShapeModel->RegisterShape(*itor);
			itor++;
		}

		m_UndoRedo.RecordAction(ActionType_New, m_selected_shapes);

		RefreshClientArea();
	}
}

bool pnShapeEditorView::CanPaste()
{
	return m_clip_board.Size() > 0;
}

void pnShapeEditorView::ZoomIn()
{
	pxPoint scale = m_scale + pxPoint(ZOOM_STEP,ZOOM_STEP);

	pxPoint lu,rb;
	m_pShapeModel->GetShapesBoundRect(lu, rb, 0);
	SetViewSize(rb.m_x, rb.m_y, scale.m_x, scale.m_y);

	RefreshClientArea();
}

void pnShapeEditorView::ZoomOut()
{
	pxPoint scale;

	if(m_scale.m_x < 0.5|| m_scale.m_y < 0.5)
		scale = m_scale * 0.5;
	else
		scale = m_scale - pxPoint(ZOOM_STEP,ZOOM_STEP);

	if(scale.m_x < COORD_EPSILON)
		scale.m_x = COORD_EPSILON;
	if(scale.m_y < COORD_EPSILON)
		scale.m_y = COORD_EPSILON;

	pxPoint lu,rb;
	m_pShapeModel->GetShapesBoundRect(lu, rb, 0);
	SetViewSize(rb.m_x, rb.m_y, scale.m_x, scale.m_y);

	RefreshClientArea();
}

void pnShapeEditorView::Zoom100()
{
	pxPoint lu,rb;
	m_pShapeModel->GetShapesBoundRect(lu, rb, 0);
	SetViewSize(rb.m_x, rb.m_y, 1, 1);

	RefreshClientArea();
}

void pnShapeEditorView::ZoomFit()
{
	pxPoint lu,rb;
	m_pShapeModel->GetShapesBoundRect(lu, rb, 0);
	
	pxSize size;
	GetCanvasSize(size);

	double scale_x = size.GetWidth() / rb.m_x;
	double scale_y = size.GetHeight() / rb.m_y;
	double scale = min(scale_x,scale_y);

	SetViewSize(size.GetWidth(), size.GetHeight(), scale, scale);

	RefreshClientArea();
}

void pnShapeEditorView::OperationAlign( pxAlignmentType x, pxAlignmentType y )
{
	m_UndoRedo.RecordAction(ActionType_Modify, m_selected_shapes);

	pxShapeBase* first_shape = 0;
	pxPoint lu,rb;

	shape_iterator itor = m_selected_shapes.begin();
	while(itor != m_selected_shapes.end())
	{
		pxShapeBase* shape = *itor;
		if(!first_shape)
		{
			first_shape = shape;
			first_shape->GetBoundsRect(lu,rb);
		}
		else
		{
			pxPoint leftup,rightbottom;
			shape->GetBoundsRect(leftup, rightbottom);

			pxCoord xtrans = 0;
			if(x == pxAlignLeft)
				xtrans = lu.m_x - leftup.m_x;
			else if(x == pxAlignRight)
				xtrans = rb.m_x - rightbottom.m_x;
			else if(x == pxAlignHCenter)
				xtrans = ((rb.m_x + lu.m_x) - (rightbottom.m_x + leftup.m_x)) / 2;

			pxCoord ytrans = 0;
			if(y == pxAlignTop)
				ytrans = lu.m_y - leftup.m_y;
			else if(y == pxAlignBottom)
				ytrans = rb.m_y - rightbottom.m_y;
			else if(y == pxAlignVCenter)
				ytrans = ((rb.m_y + lu.m_y) - (rightbottom.m_y + leftup.m_y)) / 2;

			shape->Translate(xtrans, ytrans);
		}
		itor ++;
	}
	RefreshSelectedShapesBoundRect();
	RefreshClientArea();
}

void pnShapeEditorView::OperationAlignLeft()
{
	OperationAlign(pxAlignLeft, pxAlignNone);
}

void pnShapeEditorView::OperationAlignRight()
{
	OperationAlign(pxAlignRight, pxAlignNone);
}

void pnShapeEditorView::OperationAlignHCenter()
{
	OperationAlign(pxAlignHCenter, pxAlignNone);
}

void pnShapeEditorView::OperationAlignTop()
{
	OperationAlign(pxAlignNone, pxAlignTop);
}

void pnShapeEditorView::OperationAlignBottom()
{
	OperationAlign(pxAlignNone, pxAlignBottom);
}

void pnShapeEditorView::OperationAlignVCenter()
{
	OperationAlign(pxAlignNone, pxAlignVCenter);
}

void pnShapeEditorView::OperationBringSelection( int direction )
{
	m_UndoRedo.RecordAction(ActionType_ChangeOrder, m_selected_shapes);

	shape_iterator itor = m_selected_shapes.begin();
	while(itor != m_selected_shapes.end())
	{
		pxShapeBase* shape = *itor;

		if(direction < 0) //down move toward head
		{
			pxShapeBase* after_which = m_pShapeModel->FindAfter(shape);
			if(after_which)
			{
				if(direction == -1)
				{
					m_pShapeModel->UnregisterShape(shape);
					m_pShapeModel->RegisterShape(shape, after_which);
				}
				else
				{//bottom
					m_pShapeModel->UnregisterShape(shape);

					pxShapeBase* before_which = 0;
					shape_node node = m_pShapeModel->GetStartPostion();
					m_pShapeModel->GetNextShape(node, before_which);
					
					m_pShapeModel->RegisterShape(shape, before_which);
				}
			}
		}
		else //up move toward tail
		{
			if(direction == 1)
			{
				pxShapeBase* before_which = m_pShapeModel->UnregisterShape(shape);
				if(before_which)
					before_which = m_pShapeModel->FindBefore(before_which);

				m_pShapeModel->RegisterShape(shape, before_which);
			}
			else
			{//top
				m_pShapeModel->UnregisterShape(shape);
				m_pShapeModel->RegisterShape(shape, 0);
			}
		}
		itor ++;
	}

	RefreshClientArea();
}

void pnShapeEditorView::OperationMove( int xstep, int ystep )
{
	if(m_selected_shapes.GetCount() > 0)
	{
		double steplen = 1;
		if(wxGetKeyState(WXK_SHIFT))
		{
			steplen = px2DGeometry::GetTolerance() * 2;
		}
		double xdistance = xstep * steplen;
		double ydistance = ystep * steplen;

		m_UndoRedo.RecordAction(ActionType_Modify, m_selected_shapes);

		shape_iterator itor = m_selected_shapes.begin();
		while(itor != m_selected_shapes.end())
		{
			pxShapeBase* shape = *itor;

			shape->Translate(xdistance, ydistance);

			itor++;
		}

		RefreshClientArea();
	}
}

void pnShapeEditorView::OperationBringToTop()
{
	OperationBringSelection(2);
}

void pnShapeEditorView::OperationBringToBottom()
{
	OperationBringSelection(-2);
}

void pnShapeEditorView::OperationBringUp()
{
	OperationBringSelection(1);
}

void pnShapeEditorView::OperationBringDown()
{
	OperationBringSelection(-1);
}

void pnShapeEditorView::OperationAssemble()
{
	if(m_selected_shapes.GetCount() > 1)
	{
		pxShapeGroup* group = (pxShapeGroup*)m_pShapeModel->CreateShape(pxShapeType_Group);
		group->Assemble(m_selected_shapes);
		m_pShapeModel->UnregisterShapes(m_selected_shapes);
		m_pShapeModel->RegisterShape(group);

		m_selected_shapes.clear();
		m_selected_shapes.push_back(group);

		RefreshSelectedShapesBoundRect();
		RefreshClientArea();
	}
}

bool pnShapeEditorView::CanDissemble()
{
	return (m_selected_shapes.GetCount() == 1 && 
		(*m_selected_shapes.begin())->GetShapeType() == pxShapeType_Group );
}

void pnShapeEditorView::OperationDissemble()
{
	if(CanDissemble())
	{
		pxShapeBase* group = *m_selected_shapes.begin();

		m_selected_shapes.clear();
		((pxShapeGroup*)group)->Dissemble(m_selected_shapes);
		m_pShapeModel->RegisterShapes(m_selected_shapes);

		m_pShapeModel->UnregisterShape(group);
		m_pShapeModel->FreeShape(group);

		RefreshSelectedShapesBoundRect();
		RefreshClientArea();
	}
}

void pnShapeEditorView::OperationSameDistance( bool same_horz, bool same_vert )
{
	if(m_selected_shapes.GetCount() > 2)
	{
		m_UndoRedo.RecordAction(ActionType_Modify, m_selected_shapes);
		//图元中线等距
		pxPoint first,last;
		double xdistance, ydistance;

		pxPoint point[2];
		shape_iterator itor;
		pxShapeBase* shape;

		//第一个图元
		itor = m_selected_shapes.begin();
		shape = *itor;
		shape->GetBoundsRect(point[0], point[1]);
		first = (point[1] + point[0]) / 2;
		//最后一个图元
		itor = --m_selected_shapes.end();
		shape = *itor;
		shape->GetBoundsRect(point[0], point[1]);
		last = (point[1] + point[0]) / 2;
		//间距
		xdistance = (last.m_x - first.m_x) / (m_selected_shapes.GetCount() - 1);
		ydistance = (last.m_y - first.m_y) / (m_selected_shapes.GetCount() - 1);

		itor = m_selected_shapes.begin();
		while(itor != m_selected_shapes.end())
		{
			shape = *itor;

			shape->GetBoundsRect(point[0], point[1]);
			pxPoint center = (point[1] + point[0]) / 2;

			double xtrans = 0;
			double ytrans = 0;
			if(same_horz)
				xtrans = first.m_x - center.m_x;
			if(same_vert)
				ytrans = first.m_y - center.m_y;

			shape->Translate(xtrans, ytrans);

			first.m_x += xdistance;
			first.m_y += ydistance;
			itor++;
		}
	}
}

void pnShapeEditorView::OperationHSameDistance()
{
	OperationSameDistance(true, false);
	RefreshSelectedShapesBoundRect();
	RefreshClientArea();
}

void pnShapeEditorView::OperationVSameDistance()
{
	OperationSameDistance(false, true);
	RefreshSelectedShapesBoundRect();
	RefreshClientArea();	
}

void pnShapeEditorView::OperationSameSize( bool same_width, bool same_height )
{
	if(m_selected_shapes.GetCount() > 1)
	{
		m_UndoRedo.RecordAction(ActionType_Modify, m_selected_shapes);
		//图元中线等距
		double width,height;
	
		pxPoint point[4];
		shape_iterator itor;
		pxShapeBase* shape;

		//第一个图元
		itor = m_selected_shapes.begin();
		shape = *itor;
		shape->GetBoundsRect(point);
		
		px2DGeometry::RotatePoint(point[0], -shape->GetRotationAngle(), point[2]);

		width = point[2].m_x - point[0].m_x;
		height = point[2].m_y - point[0].m_y;

		itor = m_selected_shapes.begin();
		itor ++;

		while(itor != m_selected_shapes.end())
		{
			shape = *itor;

			shape->GetBoundsRect(point);
			px2DGeometry::RotatePoint(point[0], -shape->GetRotationAngle(), point[2]);
		
			double xscale = 1;
			double yscale = 1;
			if(same_width && (point[2].m_x - point[0].m_x) > COORD_EPSILON)
				xscale = width / (point[2].m_x - point[0].m_x);
			if(same_height && (point[2].m_y - point[0].m_y) > COORD_EPSILON)
				yscale = height / (point[2].m_y - point[0].m_y);

			shape->Scale(point[0], shape->GetRotationAngle(), point[0], xscale, yscale);

			itor++;
		}
	}
}

void pnShapeEditorView::OperationSameWidth()
{
	OperationSameSize(true, false);
	RefreshSelectedShapesBoundRect();
	RefreshClientArea();
}

void pnShapeEditorView::OperationSameHeight()
{
	OperationSameSize(false, true);
	RefreshSelectedShapesBoundRect();
	RefreshClientArea();
}

void pnShapeEditorView::OperationSameSize()
{
	OperationSameSize(true, true);
	RefreshSelectedShapesBoundRect();
	RefreshClientArea();
}

void pnShapeEditorView::OperationRotate( double angle )
{
	if(m_selected_shapes.GetCount() > 0)
	{
		m_UndoRedo.RecordAction(ActionType_Modify, m_selected_shapes);

		pxPoint lu,rb,center;
		m_pShapeModel->GetShapesBoundRect(m_selected_shapes, lu, rb, 0);
		center = (lu + rb ) / 2;

		m_pShapeModel->RotateShapes(m_selected_shapes, center, angle);

		int n;
		for(n = 0; n < 4; n ++)
		{
			px2DGeometry::RotatePoint(center, angle, m_shapes_rect[n]);
		}
	}
}

void pnShapeEditorView::OperationRotateN90()
{
	OperationRotate(-PI / 2);
	RefreshClientArea();
}

void pnShapeEditorView::OperationRotateP90()
{
	OperationRotate(PI / 2);
	RefreshClientArea();
}

void pnShapeEditorView::OperationFlip( bool flip_horz, bool flip_vert )
{
	if(m_selected_shapes.GetCount() > 0)
	{
		m_UndoRedo.RecordAction(ActionType_Modify, m_selected_shapes);
		
		pxPoint lu,rb,center;
		m_pShapeModel->GetShapesBoundRect(m_selected_shapes, lu, rb, 0);
		center = (lu + rb ) / 2;

		shape_iterator itor = m_selected_shapes.begin();
		pxShapeBase* shape;
		while(itor != m_selected_shapes.end())
		{
			shape = *itor;
			shape->Flip(center, flip_horz, flip_vert);
			itor++;
		}
	}
}

void pnShapeEditorView::OperationFlipVert()
{
	OperationFlip(false, true);
	RefreshSelectedShapesBoundRect();
	RefreshClientArea();
}

void pnShapeEditorView::OperationFlipHorz()
{
	OperationFlip(true, false);
	RefreshSelectedShapesBoundRect();
	RefreshClientArea();
}
