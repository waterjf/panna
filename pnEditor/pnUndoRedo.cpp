#include "precomp.h"
#include "pnUndoRedo.h"
#include "pxShapeBase.h"

pnUndoRedo::pnUndoRedo()
{
}

pnUndoRedo::~pnUndoRedo(void)
{
	Clear();
}

void pnUndoRedo::RecordAction( ActionType at, const shape_list& releated_shapes, int ActionBlockNr)
{
	if(releated_shapes.GetCount() > 0)
	{
		//clear redo actions
		ClearRedo();
	
		//record for undo
		Action action;
		action.at = at;
		action.ActionBlockNr = ActionBlockNr;
		action.affecting_shape_nr = releated_shapes.GetCount();
		action.backup_itor = m_undo_shapes.tail();
		action.undo_pool_itor = m_undo_pool_shapes.tail();

		switch (at)
		{
		case ActionType_UnSelect:
			RecordUnSelect(action, releated_shapes);
			break;
		case ActionType_Modify:
			RecordModify(action, releated_shapes);
			break;
		case ActionType_Delete:
			RecordDelete(action, releated_shapes);
			break;
		case ActionType_New:
			RecordNew(action, releated_shapes);
			break;
		case ActionType_ChangeOrder:
			RecordChangeOrder(action, releated_shapes);
			break;
		default:
			break;
		}
	
		//指向第一个节点
		if(action.backup_itor == m_undo_shapes.end())
			action.backup_itor = m_undo_shapes.begin();
		else
			action.backup_itor ++;

		if(action.undo_pool_itor == m_undo_pool_shapes.end())
			action.undo_pool_itor = m_undo_pool_shapes.begin();
		else
			action.undo_pool_itor ++;

		m_undo_actions.push_front(action);
	}
}

void pnUndoRedo::Undo()
{
	if(CanUndo())
	{
		action_iterator itor = m_undo_actions.begin();
		int ActionBlockNr = (*itor).ActionBlockNr;
		int count = ActionBlockNr;

		do 
		{
			Action& action = *itor;

			shape_iterator backup_itor = m_redo_shapes.tail();
			shape_iterator redo_ref_itor = m_redo_ref_shapes.tail();

			switch(action.at)
			{
			case ActionType_UnSelect:
				UndoUnSelect(action);
				break;
			case ActionType_Modify:
				UndoModify(action);
				break;
			case ActionType_Delete:
				UndoDelete(action);
				break;
			case ActionType_New:
				UndoNew(action);
				break;
			case ActionType_ChangeOrder:
				UndoChangeOrder(action);
				break;
			default:
				break;
			}

			//记录redo点
			if(backup_itor == m_redo_shapes.end())
				backup_itor = m_redo_shapes.begin();
			else
				backup_itor ++;

			if(redo_ref_itor == m_redo_ref_shapes.end())
				redo_ref_itor = m_redo_ref_shapes.begin();
			else
				redo_ref_itor ++;

			action.backup_itor = backup_itor;
			action.redo_ref_itor = redo_ref_itor;
			action.ActionBlockNr = ActionBlockNr;

			m_redo_actions.push_front(action);

			//移除undo点
			m_undo_actions.remove(itor);

			count--;
			itor ++;
		}while(count > 0);
	}
}

void pnUndoRedo::Redo()
{
	if(CanRedo())
	{
		action_iterator itor = m_redo_actions.begin();
		int ActionBlockNr = (*itor).ActionBlockNr;
		int count = ActionBlockNr;

		do 
		{
			Action& action = *itor;
			shape_iterator backup_itor = m_undo_shapes.tail();
			shape_iterator undo_ref_itor = m_undo_pool_shapes.tail();

			switch(action.at)
			{
			case ActionType_UnSelect:
				RedoUnSelect(action);
				break;
			case ActionType_Modify:
				RedoModify(action);
				break;
			case ActionType_Delete:
				RedoDelete(action);
				break;
			case ActionType_New:
				RedoNew(action);
				break;
			case ActionType_ChangeOrder:
				RedoChangeOrder(action);
				break;
			default:
				break;
			}

			//记录undo点
			if(backup_itor == m_undo_shapes.end())
				backup_itor = m_undo_shapes.begin();
			else
				backup_itor ++;

			if(undo_ref_itor == m_undo_pool_shapes.end())
				undo_ref_itor = m_undo_pool_shapes.begin();
			else
				undo_ref_itor ++;

			action.backup_itor = backup_itor;
			action.undo_pool_itor = undo_ref_itor;
			action.ActionBlockNr = ActionBlockNr;
			m_undo_actions.push_front(action);

			//移除redo点
			m_redo_actions.remove(itor);

			count --;
			itor ++;
		}while(count > 0);
	}
}

void pnUndoRedo::RecordNew( Action& , const shape_list& releated_shapes )
{
	shape_iterator itor = releated_shapes.begin();
	while(itor != releated_shapes.end())
	{
		pxShapeBase* backup_shape = *itor;

		//将new图元存入undo list
		m_undo_shapes.push_back(backup_shape);

		itor ++;
	}
}

void pnUndoRedo::UndoNew(const Action& ac)
{
	int n;
	shape_iterator backup_itor = ac.backup_itor;

	for(n = 0; n < ac.affecting_shape_nr; n ++)
	{
		pxShapeBase* backup_shape = *backup_itor;

		//取消New图元的注册
		m_pShapePool->UnregisterShape(backup_shape);

		//将New图元存入redo list
		m_redo_shapes.push_back(backup_shape);

		//从undo list中移除记录
		m_undo_shapes.remove(backup_itor);

		backup_itor++;
	}
}

void pnUndoRedo::RedoNew( const Action& ac )
{
	int n;
	shape_iterator backup_itor = ac.backup_itor;

	for(n = 0; n < ac.affecting_shape_nr; n ++)
	{
		pxShapeBase* backup_shape = *backup_itor;

		//将New图元按位置重置注册到pool,
		m_pShapePool->RegisterShape(backup_shape);
		
		//将new图元存入undo list
		m_undo_shapes.push_back(backup_shape);

		//从redo lis移除记录
		m_redo_shapes.remove(backup_itor);

		backup_itor++;
	}
}

void pnUndoRedo::RecordDelete( Action& , const shape_list& releated_shapes )
{
	shape_iterator itor = releated_shapes.begin();

	shape_iterator undo_itor = m_undo_shapes.end();
	shape_iterator undo_ref_itor = m_undo_pool_shapes.end();

	while(itor != releated_shapes.end())
	{
		pxShapeBase* shape = *itor;

		//取消图元注册，并记录其后一个图元为参考图元
		pxShapeBase* undo_ref_shape = m_pShapePool->FindBefore(shape);

		//将delete图元存入undo list。逆序存放，才能保证恢复后的图元次序
		shape_iterator u = m_undo_shapes.push_back(shape);
		if(undo_itor != m_undo_shapes.end())
			m_undo_shapes.link_before(u, undo_itor);
		undo_itor = u;

		//并将参考图元存入undo ref,以便恢复。同样逆序存放
		shape_iterator a = m_undo_pool_shapes.push_back(undo_ref_shape);
		if(undo_ref_itor != m_undo_pool_shapes.end())
			m_undo_pool_shapes.link_before(a, undo_ref_itor);
		undo_ref_itor = a;

		itor ++;
	}
}

void pnUndoRedo::UndoDelete(const Action& action)
{
	int n;
	shape_iterator backup_itor = action.backup_itor;
	shape_iterator undo_ref_itor = action.undo_pool_itor;

	shape_iterator redo_ref_itor = m_redo_ref_shapes.end();
	shape_iterator redo_itor = m_redo_shapes.end();

	for(n = 0; n < action.affecting_shape_nr; n ++)
	{
		pxShapeBase* backup_shape = *backup_itor;

		if(action.at == ActionType_ChangeOrder)
		{
			//图元还在shapepool队列中，需要清除（移动Z轴顺序会导致这种情况）
			pxShapeBase* before_which = m_pShapePool->UnregisterShape(backup_shape);
			shape_iterator r = m_redo_ref_shapes.push_back(before_which);
			if(redo_ref_itor != m_redo_ref_shapes.end())
				m_redo_ref_shapes.link_before(r, redo_ref_itor);
			redo_ref_itor = r;
		}

		//将delete图元重新注册
		m_pShapePool->RegisterShape(backup_shape, *undo_ref_itor);

		//记录redo list 因为RecordDelete是逆序存放的，这里再逆序回来
		shape_iterator r = m_redo_shapes.push_back(backup_shape);
		if(redo_itor != m_redo_shapes.end())
			m_redo_shapes.link_before(r, redo_itor);
		redo_itor = r;

		//从undo list和after which list中移除记录
		m_undo_shapes.remove(backup_itor);
		m_undo_pool_shapes.remove(undo_ref_itor);

		backup_itor++;
		undo_ref_itor++;
	}
}

void pnUndoRedo::RedoDelete( const Action& action )
{
	int n;
	shape_iterator backup_itor = action.backup_itor;
	shape_iterator redo_ref_itor = action.redo_ref_itor;

	shape_iterator undo_itor = m_undo_shapes.end();
	shape_iterator undo_ref_itor = m_undo_pool_shapes.end();

	for(n = 0; n < action.affecting_shape_nr; n ++)
	{
		pxShapeBase* backup_shape = *backup_itor;

		//取消图元注册，并记录其后一个图元为参考图元
		pxShapeBase* undo_ref_shape = m_pShapePool->UnregisterShape(backup_shape);
		if(action.at == ActionType_ChangeOrder)
		{
			m_pShapePool->RegisterShape(backup_shape, *redo_ref_itor);

			m_redo_ref_shapes.remove(redo_ref_itor);
			redo_ref_itor ++;
		}

		//将delete图元存入undo list。逆序存放，才能保证恢复后的图元次序
		shape_iterator u = m_undo_shapes.push_back(backup_shape);
		if(undo_itor != m_undo_shapes.end())
			m_undo_shapes.link_before(u, undo_itor);
		undo_itor = u;

		//并将参考图元存入undo ref,以便恢复。同样逆序存放
		shape_iterator a = m_undo_pool_shapes.push_back(undo_ref_shape);
		if(undo_ref_itor != m_undo_pool_shapes.end())
			m_undo_pool_shapes.link_before(a, undo_ref_itor);
		undo_ref_itor = a;

		//从redo list中移除记录
		m_redo_shapes.remove(backup_itor);

		backup_itor++;
	}
}

void pnUndoRedo::RecordUnSelect( Action& action, const shape_list& releated_shapes )
{
	RecordNew(action, releated_shapes);
}

void pnUndoRedo::UndoUnSelect( const Action& ac )
{
	int n;
	shape_iterator backup_itor = ac.backup_itor;

	m_pSelections->clear();

	for(n = 0; n < ac.affecting_shape_nr; n ++)
	{
		pxShapeBase* backup_shape = *backup_itor;

		m_pSelections->push_back(backup_shape);

		//将图元存入redo list
		m_redo_shapes.push_back(backup_shape);

		//从undo list中移除记录
		m_undo_shapes.remove(backup_itor);

		backup_itor++;
	}
}

void pnUndoRedo::RedoUnSelect( const Action& ac)
{
	int n;
	shape_iterator backup_itor = ac.backup_itor;

	for(n = 0; n < ac.affecting_shape_nr; n ++)
	{
		pxShapeBase* backup_shape = *backup_itor;
	
		m_undo_shapes.push_back(backup_shape);

		//修改后的图元恢复到在shape pool中，并记入undo ref，以便undo
		m_redo_shapes.remove(backup_itor);
	
		backup_itor++;
	}

	m_pSelections->clear();
}

void pnUndoRedo::RecordModify( Action&, const shape_list& releated_shapes )
{
	shape_iterator itor = releated_shapes.begin();
	while(itor != releated_shapes.end())
	{
		pxShapeBase* shape = *itor;
		//修改前的图元克隆一份,存入undo list
		pxShapeBase* backup = m_pShapePool->CloneShape(shape);
		m_undo_shapes.push_back(backup);
		//被修改图元在shape pool中的位置存入before_which_shapes，以便undo
		m_undo_pool_shapes.push_back(shape);

		itor ++;
	}
}

void pnUndoRedo::UndoModify(const Action& ac)
{
	int n;
	shape_iterator backup_itor = ac.backup_itor;
	shape_iterator undo_ref_itor = ac.undo_pool_itor;

	m_pSelections->clear();

	for(n = 0; n < ac.affecting_shape_nr; n ++)
	{
		pxShapeBase* backup_shape = *backup_itor;
		pxShapeBase* pool_shape = *undo_ref_itor;

		//修改后的图元克隆一份存入redo list
		pxShapeBase* tmp_shape = m_pShapePool->CloneShape(pool_shape);
		m_redo_shapes.push_back(tmp_shape);

		//修改前的图元恢复到在shape pool中，并记入redo ref，以便redo
		pool_shape->Clone(backup_shape);
		m_redo_ref_shapes.push_back(pool_shape);

		//选中这个图元
		m_pSelections->push_back(pool_shape);

		//从undo list中移除记录
		m_undo_shapes.remove(backup_itor);
		m_undo_pool_shapes.remove(undo_ref_itor);

		//释放backup shape
		m_pShapePool->FreeShape(backup_shape);

		backup_itor++;
		undo_ref_itor++;
	}
}

void pnUndoRedo::RedoModify( const Action& ac )
{
	int n;
	shape_iterator backup_itor = ac.backup_itor;
	shape_iterator redo_ref_itor = ac.redo_ref_itor;
	
	m_pSelections->clear();

	for(n = 0; n < ac.affecting_shape_nr; n ++)
	{
		pxShapeBase* backup_shape = *backup_itor;
		pxShapeBase* pool_shape = *redo_ref_itor;

		//修改前的图元克隆一份存入undo list
		pxShapeBase* tmp_shape = m_pShapePool->CloneShape(pool_shape);
		m_undo_shapes.push_back(tmp_shape);

		//修改后的图元恢复到在shape pool中，并记入undo ref，以便undo
		pool_shape->Clone(backup_shape);
		m_undo_pool_shapes.push_back(pool_shape);
		
		//选中图元
		m_pSelections->push_back(pool_shape);

		//从redo list中移除备份记录
		m_redo_shapes.remove(backup_itor);
		m_redo_ref_shapes.remove(redo_ref_itor);

		//释放backup shape
		m_pShapePool->FreeShape(backup_shape);

		backup_itor++;
		redo_ref_itor++;
	}
}

void pnUndoRedo::RecordChangeOrder( Action& action, const shape_list& releated_shapes )
{
	RecordDelete(action, releated_shapes);
}

void pnUndoRedo::UndoChangeOrder( const Action& ac )
{
	UndoDelete(ac);
}

void pnUndoRedo::RedoChangeOrder( const Action& ac )
{
	RedoDelete(ac);
}

bool pnUndoRedo::CanUndo()
{
	return m_undo_actions.GetCount() > 0;
}

bool pnUndoRedo::CanRedo()
{
	return m_redo_actions.GetCount() > 0;
}

void pnUndoRedo::ClearUndo()
{
	action_iterator itor = m_undo_actions.begin();
	while(itor != m_undo_actions.end())
	{
		Action& action = *itor;

		ClearUndo(action);

		m_undo_actions.remove(itor);
		itor ++;
	}
}

void pnUndoRedo::ClearUndo( const Action& ac )
{
	int n; 
	shape_iterator backup_itor = ac.backup_itor;
	shape_iterator undo_ref_itor = ac.undo_pool_itor;

	for(n = 0; n < ac.affecting_shape_nr; n ++)
	{
		pxShapeBase* shape = *backup_itor;

		//释放undo list里备份的图元
		if(ac.at == ActionType_Modify || ac.at == ActionType_Delete) 
		{
			m_pShapePool->FreeShape(shape);
			m_undo_pool_shapes.remove(undo_ref_itor);
		}

		m_undo_shapes.remove(backup_itor);

		backup_itor ++;
		undo_ref_itor++;
	}
}

void pnUndoRedo::ClearRedo()
{
	action_iterator itor = m_redo_actions.begin();
	while(itor != m_redo_actions.end())
	{
		Action& action = *itor;

		ClearRedo(action);

		m_redo_actions.remove(itor);
		itor ++;
	}
}

void pnUndoRedo::ClearRedo( const Action& ac )
{
	int n; 
	shape_iterator backup_itor = ac.backup_itor;
	shape_iterator redo_ref_itor = ac.redo_ref_itor;

	for(n = 0; n < ac.affecting_shape_nr; n ++)
	{
		pxShapeBase* shape = *backup_itor;

		//释放redo list里备份的图元
		if(ac.at == ActionType_New || ac.at == ActionType_Modify)
			m_pShapePool->FreeShape(shape);

		m_redo_shapes.remove(backup_itor);

		if(ac.at == ActionType_Modify)
			m_redo_ref_shapes.remove(redo_ref_itor);

		backup_itor ++;
		redo_ref_itor++;
	}
}

void pnUndoRedo::Init( pxShapePool* pool, shape_list* pSeletions )
{
	m_pShapePool = pool;
	m_pSelections = pSeletions;
}

void pnUndoRedo::Clear()
{
	ClearRedo();
	ClearUndo();
}
