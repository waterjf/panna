#ifndef flxPool_h__
#define flxPool_h__

#include "flxArray.h"

class flxPool : flatArray
{
public:
	flxPool()
	{
		memset((flatArray*)this, 0, sizeof(flatArray));
		m_FreeLink = 0;
		m_count = 0;
	};

	void Init(ulong element_size, ulong min_size = 100, ulong max_size = 0x1000000)
	{
		flatArrayInit(this, element_size, min_size, max_size);
	}

	void Clear()
	{
		flatArrayFree(this);
	}

	virtual ~flxPool(void)
	{
		Clear();
	}

	byte* AllocElement()
	{
		byte* ret = 0;
		m_count ++;

		if(m_FreeLink)
		{
			ret = m_FreeLink;
			m_FreeLink = *((byte**)m_FreeLink);
		}
		else
		{
			if(m_count > last_allocated)
			{
				flatArrayExpand(this, 0);
				last_allocated += records_in_block;
			}

			ret = (byte*)flatArrayGetAt(this, m_count - 1);
		}

		return ret;
	}

	void RecycleElement(byte* element)
	{
		m_count --;

		if(m_FreeLink)
		{
			*((byte**)element) = m_FreeLink;
		}
		else
		{
			*((byte**)element) = 0;
		}

		m_FreeLink = element;
	}

	void* operator[] (const uint pos);

protected:
	byte* m_FreeLink;
	ulong m_count;
};

template <class TBase>
class list_node
{
public:
	TBase instance;
	list_node* prev;
	list_node* next;
};

template <class TBase>
class iterator
{
public:
	typedef list_node<TBase> my_list_node;

	iterator()
	{
		ptr = 0;
	}

	iterator(my_list_node* p)
	{
		ptr = p;
	}

	iterator& operator++()
	{
		ptr = ptr->next;
		return *this;
	}

	iterator& operator++(int)
	{
		if(ptr->next)
			ptr = ptr->next;
		return *this;
	}

	iterator& operator-- ()
	{
		if(ptr->prev)
			ptr = ptr->prev;
		return *this;
	}

	iterator& operator--(int)
	{
		ptr = ptr->prev;
		return *this;
	}

	bool operator==(const iterator& _Right) const
	{
		return ptr == _Right.ptr;
	}

	bool operator!=(const iterator& _Right) const
	{
		return ptr != _Right.ptr;
	}

	TBase& operator*() const
	{
		return ptr->instance;
	}

	TBase* operator->() const
	{
		return &ptr->instance;
	}

	my_list_node* ptr;
};
 
template <class TBase>
class flxList
{
public:
	typedef list_node<TBase> my_list_node;
	typedef iterator<TBase> my_iterator;

	flxList()
	{
		m_node_count = 0;

		m_pool.Init(sizeof(my_list_node));

		//buy a fake node
		my_list_node* node = (my_list_node*)m_pool.AllocElement();
		memset(&(node->instance), 0, sizeof(node->instance));
		node->prev = 0;
		node->next = 0;

		m_pHead = node;
	}

	virtual ~flxList()
	{
		clear();
		m_pool.Clear();
	}

public:

	my_iterator begin() const
	{
		if(m_pHead->next)
			return my_iterator(m_pHead->next);

		return my_iterator(m_pHead);
	}

	my_iterator end() const
	{
		return my_iterator(m_pHead);
	}

	my_iterator tail() const
	{
		return --end();
	}

	my_iterator push_front(const TBase& instance)
	{
		my_list_node* node = (my_list_node*)m_pool.AllocElement();
		my_iterator itor = my_iterator(node);

		node->my_list_node::list_node();
		node->instance = instance;
		node->prev = 0;
		node->next = 0;

		link_before(itor, begin());

		m_node_count ++;

		return itor;
	}

	my_iterator push_back(const TBase& instance)
	{
		my_list_node* node = (my_list_node*)m_pool.AllocElement();
		my_iterator itor = my_iterator(node);

		node->my_list_node::list_node();
		node->instance = instance;
		node->prev = 0;
		node->next = 0;

		if(m_node_count > 0)
			link_after(itor, tail());
		else
			link_after(itor, end());

		m_node_count ++;

		return itor;
	}

	void clear()
	{
		my_iterator itor = begin();
		while(itor != end())
		{
			my_list_node* node = itor.ptr;
			itor++;

			node->~my_list_node();
			m_pool.RecycleElement((byte*)node);
		}

		m_pHead->prev = 0;
		m_pHead->next = 0;
		m_node_count = 0;
	}

	my_iterator find(const TBase& target)
	{
		my_iterator itor = begin();
		while(itor != end())
		{
			if(target == *itor)
				return itor;

			itor ++;
		}

		return end();
	}

	void move_up(my_iterator& itor, bool move_to_top = false)
	{
		if(itor != begin())
		{
			iterator prev = itor--;

			unlink(itor);

			if(move_to_top)
			{
				link_before(itor, begin());
			}
			else
			{
				link_before(itor, prev);
			}
		}
	}

	void move_down(my_iterator& itor, bool move_to_bottom = false)
	{
		my_iterator next = itor++;

		if(next != end())
		{
			unlink(itor);

			if(move_to_bottom)
			{
				link_after(itor, tail());
			}
			else
			{
				link_after(itor, next);
			}
		}
	}

	void remove(const my_iterator& itor)
	{
		unlink(itor);

		itor.ptr->~my_list_node();
		m_pool.RecycleElement((byte*)itor.ptr);

		m_node_count --;
	}
	
	void link_after(const my_iterator& itor, const my_iterator& after_which)
	{
		if(after_which == end())
		{//first element
			itor.ptr->prev = m_pHead;
			itor.ptr->next = m_pHead;
			m_pHead->prev = itor.ptr;
			m_pHead->next = itor.ptr;
		}
		else
		{
			unlink(itor);

			itor.ptr->prev = after_which.ptr;
			itor.ptr->next = after_which.ptr->next;

			if(after_which.ptr->next)
				after_which.ptr->next->prev = itor.ptr;
			after_which.ptr->next = itor.ptr;
		}
	}

	void link_before(const my_iterator& itor, const my_iterator& before_which)
	{
		if(before_which == end())
		{//first element
			itor.ptr->prev = m_pHead;
			itor.ptr->next = m_pHead;
			m_pHead->prev = itor.ptr;
			m_pHead->next = itor.ptr;
		}
		else
		{
			unlink(itor);

			itor.ptr->prev = before_which.ptr->prev;
			itor.ptr->next = before_which.ptr;

			if(before_which.ptr->prev)
				before_which.ptr->prev->next = itor.ptr;
			before_which.ptr->prev = itor.ptr;
		}
	}

	uint GetCount() const { return m_node_count; }
	bool IsEmpty() const { return m_node_count == 0; };

protected:
	void unlink(const my_iterator& itor)
	{
		if(itor.ptr->prev)
			itor.ptr->prev->next = itor.ptr->next;
		if(itor.ptr->next)
			itor.ptr->next->prev = itor.ptr->prev;
	}

protected:
	my_list_node* m_pHead;
	uint m_node_count;

	flxPool m_pool;
};

template<class KEY_TYPE, class VAL_TYPE>
class flxHash
{
public:
	flxHash();
	virtual ~flxHash();

	void init(uint max_records = 0x100000);


protected:
	struct DH_CELL
	{
		KEY_TYPE key;
		VAL_TYPE value;
		DH_CELL* next;
	};

	flxPool m_pSlots;

	unsigned int m_nSlots;
	unsigned int m_nActiveSlots;
	unsigned int m_nUsedSlots;
	int m_nRehashSlot;

protected:
	int GetSlotPos(const KEY_TYPE& key) const;
	void* InsertRepos(int repos, int free); //return free cell;
	VAL_TYPE& AddNewKey(const KEY_TYPE& key, DH_CELL* pFreeCell);

	int RemoveRepos(int repos, int free);
	void RemoveOldKey(const KEY_TYPE& key, int nFreePos);
	VAL_TYPE& SetAt(const KEY_TYPE& key/*in*/);
	void* Lookup(const KEY_TYPE& key/*in*/) const;

public:
	int Lookup(const KEY_TYPE& key/*in*/, VAL_TYPE& value/*out*/) const;
	void SetAt(const KEY_TYPE& key/*in*/, const VAL_TYPE& value/*out*/);
	VAL_TYPE& operator [](const KEY_TYPE& key);

	int RemoveKey(const KEY_TYPE& key);
	void RemoveAll();

	int GetCount() const;
	int IsEmpty();

	VAL_TYPE* GetVal(unsigned int pos);
	KEY_TYPE* GetKey(unsigned int pos);

	void* GetCell(unsigned int pos); //debug helper
	int CheckHash(const void* pSlotCell, void* expected_cell = 0); //debug helper
};

#define NULL_CELL (0)
#define HAVE_NEXT(cell) (((cell)->next) != 0)
#define NEXT_CELL(cell) ((cell)->next)

#define SET_NEXT(prev_cell, cell) ((prev_cell)->next = cell)
#define SET_NEXT_FROM_CELL(prev_cell, current_cell) SET_NEXT( (prev_cell), ((current_cell)->next) )
#define GET_CELL(pos) ((DH_CELL*)flatArrayGetAt((flatArray*)&m_pSlots, pos))

#define EOS (~0)
#define NO_REHASH EOS

template<class KEY_TYPE>
inline uint do_hash(const KEY_TYPE& key)
{
	return (uint)key;
}

inline uint do_hash(const char* sz)
{
	char* p = (char*)sz;
	uint nr = 1;
	uint nr2 = 4;

	while (*p)
	{
		nr ^= (((nr & 63) + nr2) * (*p)) + (nr << 8);
		nr2 += 3;

		p++;
	}

	return nr;
}

inline uint do_hash(const wxString str)
{
	return do_hash((const char*)str);
}

template<class KEY_TYPE, class VAL_TYPE>
void flxHash<KEY_TYPE, VAL_TYPE>::init(uint max_records)
{
	m_pSlots.Init(sizeof(struct DH_CELL), 100, max_records);
	RemoveAll();
}

template<class KEY_TYPE, class VAL_TYPE>
flxHash<KEY_TYPE, VAL_TYPE>::flxHash()
{
	init();
}

template<class KEY_TYPE, class VAL_TYPE>
flxHash<KEY_TYPE, VAL_TYPE>::~flxHash()
{
	RemoveAll();
}

template<class KEY_TYPE, class VAL_TYPE>
int flxHash<KEY_TYPE, VAL_TYPE>::GetSlotPos(const KEY_TYPE& key) const
{
	uint hash_val = do_hash(key);

	if ((hash_val & (m_nSlots - 1)) >= m_nActiveSlots)
		return (hash_val & ((m_nSlots >> 1) - 1));

	return (hash_val & (m_nSlots - 1));
}

template<class KEY_TYPE, class VAL_TYPE>
void* flxHash<KEY_TYPE, VAL_TYPE>::InsertRepos(int tobe_repos, int free_pos)
{
	DH_CELL* pOriginRehashCell = GET_CELL(tobe_repos);
	DH_CELL* pRehashCell = pOriginRehashCell;
	DH_CELL* pFreeCell = GET_CELL(free_pos);
	pFreeCell->DH_CELL::DH_CELL();

	DH_CELL* pLastNotMovedCell = 0;
	DH_CELL* pLastMovedCell = 0;

	//DH_CELL* pRehashHitCell = GET_CELL(nRehashHitPos);
	int nRehashHitPos = GetSlotPos(pRehashCell->key);

	if (nRehashHitPos == tobe_repos || nRehashHitPos == free_pos)
	{
		DH_CELL* current_cell = pRehashCell;

		while (pRehashCell)
		{//get through the chain begin at repos
			nRehashHitPos = GetSlotPos(pRehashCell->key);

			if (nRehashHitPos != tobe_repos)
			{
				if (!pLastMovedCell)
				{//find first need to be moved cell, move it to FreePos
					DH_CELL* pCell = pFreeCell;

					pCell->key = current_cell->key;
					pCell->value = current_cell->value;
					SET_NEXT(pCell, NULL_CELL);

					pLastMovedCell = pCell;
					pFreeCell = current_cell;
				}
				else
				{
					SET_NEXT(pLastMovedCell, current_cell);
					pLastMovedCell = pRehashCell;
				}
			}
			else
			{
				if (!pLastNotMovedCell)
				{//first not to be moved cell
					if (current_cell != pOriginRehashCell)
					{//repos cell moved, it must be free now
						pOriginRehashCell->key = current_cell->key;
						pOriginRehashCell->value = current_cell->value;

						pLastNotMovedCell = pOriginRehashCell;
						pFreeCell = current_cell;
					}
					else
					{
						pLastNotMovedCell = pRehashCell;
					}
				}
				else
				{
					SET_NEXT(pLastNotMovedCell, current_cell);
					pLastNotMovedCell = pRehashCell;
				}
			}

			current_cell = pRehashCell = NEXT_CELL(pRehashCell);
		}

		if (pLastMovedCell)
			SET_NEXT(pLastMovedCell, 0);

		if (pLastNotMovedCell)
			SET_NEXT(pLastNotMovedCell, 0);
	}

	return pFreeCell;
}

template<class KEY_TYPE, class VAL_TYPE>
VAL_TYPE& flxHash<KEY_TYPE, VAL_TYPE>::AddNewKey(const KEY_TYPE& key, DH_CELL* pFreeCell)
{
	int nKeyHitPos = GetSlotPos(key);
	DH_CELL* pKeyHitCell = GET_CELL(nKeyHitPos);

	if (!pFreeCell)
	{
		m_pSlots.AllocElement();

		pKeyHitCell = GET_CELL(nKeyHitPos);
		pFreeCell = (DH_CELL*)GET_CELL(m_nActiveSlots - 1);

		pKeyHitCell->DH_CELL::DH_CELL();//construct this cell
	}
	
	if (pKeyHitCell != pFreeCell)
	{
		int nHitCellSlot = GetSlotPos(pKeyHitCell->key);

		if (nHitCellSlot == nKeyHitPos)
		{//nKeyHitPos meets it's slot code, not linked from other slot.
			//get through chain for last link, and put new key on the tail.
			DH_CELL* pLastCell = pKeyHitCell;

			while (HAVE_NEXT(pLastCell))
				pLastCell = NEXT_CELL(pLastCell);

			//link new key up
			SET_NEXT(pLastCell, pFreeCell);
		}
		else
		{//thing in nKeyHitPos was linked from other slot.
			//we should move thing in nKeyHitPos to nFreePos, and leave 
			//nKeyHitPos for new key

			DH_CELL* pPrevLink = GET_CELL(nHitCellSlot);
			while (NEXT_CELL(pPrevLink) != pKeyHitCell)
				pPrevLink = NEXT_CELL(pPrevLink);

			SET_NEXT(pPrevLink, pFreeCell);

			pFreeCell->key = pKeyHitCell->key;
			pFreeCell->value = pKeyHitCell->value;
			SET_NEXT_FROM_CELL(pFreeCell, pKeyHitCell);

			pFreeCell = pKeyHitCell;
		}
	}

	pFreeCell->key = key;

	SET_NEXT(pFreeCell, 0);

	return pFreeCell->value;
}

template<class KEY_TYPE, class VAL_TYPE>
VAL_TYPE& flxHash<KEY_TYPE, VAL_TYPE>::operator [](const KEY_TYPE& key)
{
	return SetAt(key);
}

template<class KEY_TYPE, class VAL_TYPE>
VAL_TYPE& flxHash<KEY_TYPE, VAL_TYPE>::SetAt(const KEY_TYPE& key)
{
	DH_CELL* pCell = (DH_CELL*)Lookup(key);

	if (!pCell)
	{
		if (m_nUsedSlots == m_nSlots)
		{
			//append slots;
			m_nRehashSlot = 0;
			m_nSlots = m_nSlots << 1;
		}

		//rehash m_nRehashSlot;
		//m_nActiveSlots - 1 indicates current free slot;
		//and GetSlotPos() applies new m_nActiveSlots
		m_nActiveSlots++;

		DH_CELL* pFreeCell = 0;

		if (m_nRehashSlot != NO_REHASH) //first time inserting needs no rehash.
			pFreeCell = (DH_CELL*)InsertRepos(m_nRehashSlot, m_nActiveSlots - 1);

		VAL_TYPE& val = AddNewKey(key, pFreeCell);//use current free pos to do insert operation.

		m_nRehashSlot++; //count up rehash slot needed by next insert operation
		m_nUsedSlots++;

		return val;
	}

	//change key
	return pCell->value;
}

template<class KEY_TYPE, class VAL_TYPE>
void flxHash<KEY_TYPE, VAL_TYPE>::SetAt(const KEY_TYPE& key, const VAL_TYPE& value)
{
	SetAt(key) = value;
}

template<class KEY_TYPE, class VAL_TYPE>
int flxHash<KEY_TYPE, VAL_TYPE>::Lookup(const KEY_TYPE& key, VAL_TYPE& value) const
{
	DH_CELL* pCell = (DH_CELL*)Lookup(key);
	if (pCell)
	{
		value = pCell->value;
		return 1;
	}

	return 0;
}

template<class KEY_TYPE, class VAL_TYPE>
void* flxHash<KEY_TYPE, VAL_TYPE>::Lookup(const KEY_TYPE& key) const
{
	if (m_nUsedSlots)
	{
		int current_slot = GetSlotPos(key);
		DH_CELL* pCell = GET_CELL(current_slot);

		do
		{
			if (pCell->key == key)
			{
				return pCell;
			}

			pCell = NEXT_CELL(pCell);

		} while (pCell);
	}

	return 0;
}

template<class KEY_TYPE, class VAL_TYPE>
int flxHash<KEY_TYPE, VAL_TYPE>::RemoveKey(const KEY_TYPE& key)
{
	DH_CELL* pFreeCell = 0;

	if (m_nUsedSlots > 0)
	{
		int nKeyHitPos = GetSlotPos(key);
		DH_CELL* pKeyHitCell = GET_CELL(nKeyHitPos);
		DH_CELL* current_cell = pKeyHitCell;

		DH_CELL* pPrevCell = current_cell;

		do
		{
			if (current_cell->key == key)//key matched;
			{
				if (current_cell == pKeyHitCell) //means current node is at the head of the chain.
				{
					if (HAVE_NEXT(current_cell)) //if there is a link after head, it should ne moved to the head position;
					{
						DH_CELL* pPrevCell = current_cell;

						current_cell = NEXT_CELL(current_cell); //save the pos of the node to be moved; 
						//it would ne marked as free pos;
						*pPrevCell = *current_cell;
					}
				}
				else
				{
					//cut down link node
					SET_NEXT_FROM_CELL(pPrevCell, current_cell);
				}

				pFreeCell = current_cell;

				break;
			}

			pPrevCell = current_cell;
			current_cell = NEXT_CELL(current_cell);

		} while (current_cell);

		if (pFreeCell != 0)
		{
			m_nUsedSlots--;
			m_nActiveSlots--;

			DH_CELL* pRehashCell = GET_CELL(m_nActiveSlots);

			if (pFreeCell != pRehashCell)
			{
				//rehash cell's new pos;
				DH_CELL* pHitCell = GET_CELL(GetSlotPos(pRehashCell->key));

				if (pHitCell == pFreeCell)
				{
					*pFreeCell = *pRehashCell;
				}
				else
				{
					DH_CELL* pHitPosCell = GET_CELL(GetSlotPos(pHitCell->key));
					if (pHitPosCell == pHitCell) //HitCell is at a head pos
					{
						//Maybe nRehashPos is originally a link from nHitCellPos, maybe not.
						while (NEXT_CELL(pHitCell) != pRehashCell && HAVE_NEXT(pHitCell))
							pHitCell = NEXT_CELL(pHitCell);

						//Maybe nHitCellPos is a link from nRehashPos, let's check this out.
						DH_CELL* pPreCell = pRehashCell;
						DH_CELL* pCell = NEXT_CELL(pRehashCell);
						while (pCell)
						{
							if (pCell == pHitPosCell)
							{
								SET_NEXT(pPreCell, 0);
								break;
							}
							pPreCell = pCell;
							pCell = NEXT_CELL(pCell);
						}

						*pFreeCell = *pRehashCell;

						//add to last link
						SET_NEXT(pHitCell, pFreeCell);
					}
					else //HitCell is a link from other chain
					{
						DH_CELL* pCell = pHitPosCell;

						do
						{
							DH_CELL* pPrevCell = pCell;
							pCell = NEXT_CELL(pCell);

							if (pCell == pHitCell)
							{
								//find the link before nHitCellPos!
								//and these will occur with no doubt!
								SET_NEXT(pPrevCell, pFreeCell);
								*pFreeCell = *pHitCell;
								SET_NEXT_FROM_CELL(pFreeCell, pHitCell);

								*pHitCell = *pRehashCell;

								break;
							}

						} while (pCell);
					}
				}
			}

			if (m_nUsedSlots <= (m_nSlots >> 1))
			{
				//decrease slots;
				m_nSlots = m_nSlots >> 1;

				m_nRehashSlot = m_nSlots >> 1;

				//IMPORTANT!!
				if (m_nSlots == 0)
				{
					m_nSlots = 1;
				}

			}
			else
				m_nRehashSlot--;
		}
	}

	return (pFreeCell != 0);
}

template<class KEY_TYPE, class VAL_TYPE>
void flxHash<KEY_TYPE, VAL_TYPE>::RemoveAll()
{
	m_nSlots = 1;
	m_nActiveSlots = 0;
	m_nUsedSlots = 0;
	m_nRehashSlot = EOS;
	m_pSlots.Clear();
}

template<class KEY_TYPE, class VAL_TYPE>
int flxHash<KEY_TYPE, VAL_TYPE>::GetCount() const
{
	return m_nUsedSlots;
}

template<class KEY_TYPE, class VAL_TYPE>
int flxHash<KEY_TYPE, VAL_TYPE>::IsEmpty()
{
	return m_nUsedSlots > 0;
}
//
//template<class KEY_TYPE, class VAL_TYPE>
//int CLinearHash<KEY_TYPE, VAL_TYPE>::Enum(POS& pos, VAL_TYPE*& val)
//{
//	if(pos < m_nUsedSlots)
//	{
//		val = &m_pSlots[pos].value;
//		pos ++;
//
//		return 1;
//	}
//
//	return 0;
//}

template<class KEY_TYPE, class VAL_TYPE>
VAL_TYPE* flxHash<KEY_TYPE, VAL_TYPE>::GetVal(unsigned int pos)
{
	if (pos < m_nUsedSlots)
		return &(GET_CELL(pos)->value);

	return NULL;
}

template<class KEY_TYPE, class VAL_TYPE>
KEY_TYPE* flxHash<KEY_TYPE, VAL_TYPE>::GetKey(unsigned int pos)
{
	if (pos < m_nUsedSlots)
		return &(GET_CELL(pos)->key);

	return NULL;
}

template<class KEY_TYPE, class VAL_TYPE>
void* flxHash<KEY_TYPE, VAL_TYPE>::GetCell(unsigned int pos)
{
	return (void*)GET_CELL(pos);
}

template<class KEY_TYPE, class VAL_TYPE>
int flxHash<KEY_TYPE, VAL_TYPE>::CheckHash(const void* pSlotCell, void* expect)
{
	DH_CELL* pCell = (DH_CELL*)pSlotCell;

	int nSlotHitPos = GetSlotPos(pCell->key);
	DH_CELL* pSlotHitCell = GET_CELL(nSlotHitPos);

	int found = 0;
	if (!expect)
		found = 1;

	if (pSlotHitCell == pSlotCell)
	{
		//try find the chain
		while (pCell)
		{
			if (NEXT_CELL(pCell) == (DH_CELL*)expect)
				found = 1;

			if (nSlotHitPos != GetSlotPos(pCell->key))
				return 0;

			if (HAVE_NEXT(pCell))
				pCell = NEXT_CELL(pCell);
			else
				pCell = 0;
		}
	}
	else
	{
		if (expect)
			return 0;

		return CheckHash(pSlotHitCell, (void*)pSlotCell);
	}

	return found;
}

#endif // flxPool_h__
