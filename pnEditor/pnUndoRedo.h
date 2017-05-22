#ifndef pnUndoRedo_h__
#define pnUndoRedo_h__

#include "pxShapePool.h"

enum ActionType
{
	ActionType_UnSelect = 1,
	ActionType_Modify,
	ActionType_Delete,
	ActionType_New,
	ActionType_ChangeOrder,
};

class pnUndoRedo
{
public:
	pnUndoRedo();
	~pnUndoRedo();

public:
	void Init(pxShapePool* pool, shape_list* pSelections);

	void RecordAction(ActionType at, const shape_list& releated_shapes, int ActionBlockNr = 0);
	void Undo();
	void Redo();
	
	bool CanUndo();
	bool CanRedo();

	void Clear();

protected:
	typedef flxList<shape_iterator> shape_ref_list;
	typedef iterator<shape_iterator> shape_ref_iterator;

	typedef struct
	{
		ActionType at;
		int ActionBlockNr; //几个动作构成一个组合动作
		int affecting_shape_nr;
		shape_iterator backup_itor;
		shape_iterator undo_pool_itor;
		shape_iterator redo_ref_itor;
	}Action;

	typedef flxList<Action> action_list;
	typedef iterator<Action> action_iterator;

	void RecordUnSelect(Action& action, const shape_list& releated_shapes);
	void UndoUnSelect(const Action& ac);
	void RedoUnSelect(const Action& ac);

	void RecordModify(Action& action, const shape_list& releated_shapes);
	void UndoModify(const Action& ac);
	void RedoModify(const Action& ac);

	void RecordNew(Action& action, const shape_list& releated_shapes);
	void UndoNew(const Action& ac);
	void RedoNew(const Action& ac);

	void RecordDelete(Action& action, const shape_list& releated_shapes);
	void UndoDelete(const Action& ac);
	void RedoDelete(const Action& ac);

	void RecordChangeOrder(Action& action, const shape_list& releated_shapes);
	void UndoChangeOrder(const Action& ac);
	void RedoChangeOrder(const Action& ac);

	void ClearRedo();
	void ClearRedo(const Action& ac);

	void ClearUndo();
	void ClearUndo(const Action& ac);

protected:
	pxShapePool* m_pShapePool;
	shape_list* m_pSelections;

	//undo list
	action_list m_undo_actions;	
	shape_list m_undo_shapes;
	shape_list m_undo_pool_shapes;

	//redo list
	action_list m_redo_actions;	
	shape_list m_redo_shapes;
	shape_list m_redo_ref_shapes; //用于redo的图元备份
};

#endif // pnUndoRedo_h__
