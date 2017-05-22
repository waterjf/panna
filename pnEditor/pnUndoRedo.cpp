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
	
		//ָ���һ���ڵ�
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

			//��¼redo��
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

			//�Ƴ�undo��
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

			//��¼undo��
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

			//�Ƴ�redo��
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

		//��newͼԪ����undo list
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

		//ȡ��NewͼԪ��ע��
		m_pShapePool->UnregisterShape(backup_shape);

		//��NewͼԪ����redo list
		m_redo_shapes.push_back(backup_shape);

		//��undo list���Ƴ���¼
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

		//��NewͼԪ��λ������ע�ᵽpool,
		m_pShapePool->RegisterShape(backup_shape);
		
		//��newͼԪ����undo list
		m_undo_shapes.push_back(backup_shape);

		//��redo lis�Ƴ���¼
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

		//ȡ��ͼԪע�ᣬ����¼���һ��ͼԪΪ�ο�ͼԪ
		pxShapeBase* undo_ref_shape = m_pShapePool->FindBefore(shape);

		//��deleteͼԪ����undo list�������ţ����ܱ�֤�ָ����ͼԪ����
		shape_iterator u = m_undo_shapes.push_back(shape);
		if(undo_itor != m_undo_shapes.end())
			m_undo_shapes.link_before(u, undo_itor);
		undo_itor = u;

		//�����ο�ͼԪ����undo ref,�Ա�ָ���ͬ��������
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
			//ͼԪ����shapepool�����У���Ҫ������ƶ�Z��˳��ᵼ�����������
			pxShapeBase* before_which = m_pShapePool->UnregisterShape(backup_shape);
			shape_iterator r = m_redo_ref_shapes.push_back(before_which);
			if(redo_ref_itor != m_redo_ref_shapes.end())
				m_redo_ref_shapes.link_before(r, redo_ref_itor);
			redo_ref_itor = r;
		}

		//��deleteͼԪ����ע��
		m_pShapePool->RegisterShape(backup_shape, *undo_ref_itor);

		//��¼redo list ��ΪRecordDelete�������ŵģ��������������
		shape_iterator r = m_redo_shapes.push_back(backup_shape);
		if(redo_itor != m_redo_shapes.end())
			m_redo_shapes.link_before(r, redo_itor);
		redo_itor = r;

		//��undo list��after which list���Ƴ���¼
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

		//ȡ��ͼԪע�ᣬ����¼���һ��ͼԪΪ�ο�ͼԪ
		pxShapeBase* undo_ref_shape = m_pShapePool->UnregisterShape(backup_shape);
		if(action.at == ActionType_ChangeOrder)
		{
			m_pShapePool->RegisterShape(backup_shape, *redo_ref_itor);

			m_redo_ref_shapes.remove(redo_ref_itor);
			redo_ref_itor ++;
		}

		//��deleteͼԪ����undo list�������ţ����ܱ�֤�ָ����ͼԪ����
		shape_iterator u = m_undo_shapes.push_back(backup_shape);
		if(undo_itor != m_undo_shapes.end())
			m_undo_shapes.link_before(u, undo_itor);
		undo_itor = u;

		//�����ο�ͼԪ����undo ref,�Ա�ָ���ͬ��������
		shape_iterator a = m_undo_pool_shapes.push_back(undo_ref_shape);
		if(undo_ref_itor != m_undo_pool_shapes.end())
			m_undo_pool_shapes.link_before(a, undo_ref_itor);
		undo_ref_itor = a;

		//��redo list���Ƴ���¼
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

		//��ͼԪ����redo list
		m_redo_shapes.push_back(backup_shape);

		//��undo list���Ƴ���¼
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

		//�޸ĺ��ͼԪ�ָ�����shape pool�У�������undo ref���Ա�undo
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
		//�޸�ǰ��ͼԪ��¡һ��,����undo list
		pxShapeBase* backup = m_pShapePool->CloneShape(shape);
		m_undo_shapes.push_back(backup);
		//���޸�ͼԪ��shape pool�е�λ�ô���before_which_shapes���Ա�undo
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

		//�޸ĺ��ͼԪ��¡һ�ݴ���redo list
		pxShapeBase* tmp_shape = m_pShapePool->CloneShape(pool_shape);
		m_redo_shapes.push_back(tmp_shape);

		//�޸�ǰ��ͼԪ�ָ�����shape pool�У�������redo ref���Ա�redo
		pool_shape->Clone(backup_shape);
		m_redo_ref_shapes.push_back(pool_shape);

		//ѡ�����ͼԪ
		m_pSelections->push_back(pool_shape);

		//��undo list���Ƴ���¼
		m_undo_shapes.remove(backup_itor);
		m_undo_pool_shapes.remove(undo_ref_itor);

		//�ͷ�backup shape
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

		//�޸�ǰ��ͼԪ��¡һ�ݴ���undo list
		pxShapeBase* tmp_shape = m_pShapePool->CloneShape(pool_shape);
		m_undo_shapes.push_back(tmp_shape);

		//�޸ĺ��ͼԪ�ָ�����shape pool�У�������undo ref���Ա�undo
		pool_shape->Clone(backup_shape);
		m_undo_pool_shapes.push_back(pool_shape);
		
		//ѡ��ͼԪ
		m_pSelections->push_back(pool_shape);

		//��redo list���Ƴ����ݼ�¼
		m_redo_shapes.remove(backup_itor);
		m_redo_ref_shapes.remove(redo_ref_itor);

		//�ͷ�backup shape
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

		//�ͷ�undo list�ﱸ�ݵ�ͼԪ
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

		//�ͷ�redo list�ﱸ�ݵ�ͼԪ
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
