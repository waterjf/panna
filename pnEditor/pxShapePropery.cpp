#include "precomp.h"
#include "pxShapePropery.h"

pxShapePropertyItem::pxShapePropertyItem(wxString Name, SPIT type)
{
	m_Name = Name;
	m_SPIT = type;

	m_target_ptr = 0;
	m_target_len = 0;
}

pxShapePropertyItem::pxShapePropertyItem()
{
	m_SPIT = SPIT_INVALID;
	m_target_ptr = 0;
	m_target_len = 0;
}

pxShapePropertyItem::~pxShapePropertyItem()
{
	if (m_SPIT == SPIT_CATEGORY)
	{
		pxShapeProperyCategory* spc = (pxShapeProperyCategory*)m_Value.GetVoidPtr();
		if (spc)
			delete spc;

		m_Value = (void*)0;
	}
}

pxShapeProperyCategory* pxShapePropertyItem::GetCategory()
{
	pxShapeProperyCategory* spc = (pxShapeProperyCategory*)m_Value.GetVoidPtr();

	return spc;
}

void pxShapePropertyItem::SetCategory(pxShapeProperyCategory* cateory)
{
	m_Value = (void*)cateory;
}

void pxShapePropertyItem::Clone(const pxShapePropertyItem& spi)
{
	m_Name = spi.GetName();
	m_SPIT = spi.GetType();
	SetTargetPtr(spi.GetTargetPtr());
	SetTargetLen(spi.GetTargetLen());
	
	if (m_SPIT != SPIT_CATEGORY)
		Value(spi.Value());
}

void pxShapePropertyItem::AttachTarget(void* target_ptr, int target_len)
{
	m_target_ptr = target_ptr;
	m_target_len = target_len;
}

void pxShapePropertyItem::FetchValue()
{
	m_Value.Clear();

	if (!m_target_ptr)
		return;
	
	switch (m_SPIT)
	{
	case SPIT_TEXT:
		if (((char*)m_target_ptr)[m_target_len] == 0)
			m_Value = (const char*)m_target_ptr;
		break;
	case SPIT_DOUBLE:
		m_Value = *(double*)m_target_ptr;
		break;
	case SPIT_BOOLEAN:
		m_Value = *(bool*)m_target_ptr;
		break;
	case SPIT_LONG:
		m_Value = *(long*)m_target_ptr;
		break;
	default:
		break;
	}
}

void pxShapePropertyItem::SendValue()
{
	if (!m_target_ptr)
		return;

	switch (m_SPIT)
	{
	case SPIT_TEXT:
		if (((char*)m_target_ptr)[m_target_len] == 0)
			m_Value = (const char*)m_target_ptr;
		break;
	case SPIT_DOUBLE:
		m_Value = *(double*)m_target_ptr;
		break;
	case SPIT_BOOLEAN:
		m_Value = *(bool*)m_target_ptr;
		break;
	case SPIT_LONG:
		m_Value = *(long*)m_target_ptr;
		break;
	default:
		break;
	}
}

pxShapeProperyCategory::pxShapeProperyCategory()
{
	m_map.init();
}

pxShapeProperyCategory::~pxShapeProperyCategory()
{

}

pxShapeProperyCategory& pxShapeProperyCategory::AddCategory(const wxString& name)
{
	pxShapePropertyItem* spi = new pxShapePropertyItem(name, SPIT_CATEGORY);
	pxShapeProperyCategory* spc = new pxShapeProperyCategory();
	spi->SetCategory(spc);

	m_PropertyCollection.push_back(*spi);

	return *spc;
}

pxShapePropertyItem& pxShapeProperyCategory::AddItem(const wxString& name, SPIT type, void* ptr, int len)
{
	pxShapePropertyItem* spi = new pxShapePropertyItem(name, type);

	spi->SetTargetPtr(ptr);
	spi->SetTargetLen(len);

	pxShapePropertyList::my_iterator itor = m_PropertyCollection.push_back(*spi);
	pxShapePropertyItem& ret = *itor;

	delete spi;

	m_map.SetAt(name, &ret);

	return ret;
}

int pxShapeProperyCategory::GetPropertyItemCount()
{
	return GetPropertyItemCount(this);
}

int pxShapeProperyCategory::GetPropertyItemCount(pxShapeProperyCategory* spc)
{
	int count = 0;
	const pxShapePropertyList& property_list = spc->GetPropertyList();

	pxShapePropertyList::my_iterator itor = property_list.begin();
	while (itor != property_list.end())
	{
		pxShapePropertyItem& spi = *itor;
		if (spi.GetType() == SPIT_CATEGORY)
			count = count + GetPropertyItemCount(spi.GetCategory());
		else
			count++;

		itor++;
	}

	return count;
}

void pxShapeProperyCategory::Clone(const pxShapeProperyCategory& spc)
{
	const pxShapePropertyList& spc_list = spc.GetPropertyList();

	pxShapePropertyList::my_iterator itor = spc_list.begin();
	while (itor != spc_list.end())
	{
		pxShapePropertyItem& spi = *itor;
		if (spi.GetType() == SPIT_CATEGORY)
		{
			pxShapeProperyCategory* subspc = spi.GetCategory();
			AddCategory(spi.GetName()).Clone(*subspc);
		}
		else
		{
			AddItem(spi.GetName(), spi.GetType(), spi.GetTargetPtr(), spi.GetTargetLen()).Clone(spi);
		}

		itor++;
	}
}

void pxShapeProperyCategory::FillFrom(const pxShapeProperyCategory& source_spc)
{
	pxShapePropertyList::my_iterator itor = m_PropertyCollection.begin();
	while (itor != m_PropertyCollection.end())
	{
		pxShapePropertyItem& spi = *itor;
		if (spi.GetType() == SPIT_CATEGORY)
		{
			pxShapeProperyCategory* subspc = spi.GetCategory();
			pxShapeProperyCategory* sub_source_spc = source_spc.FindCategory(spi.GetName());

			subspc->FillFrom(*sub_source_spc);
		}
		else
		{
			spi.Clone(*source_spc.FindItem(spi.GetName()));
		}

		itor++;
	}
}

pxShapeProperyCategory* pxShapeProperyCategory::FindCategory(const wxString& name) const
{
	pxShapeProperyCategory* ret = 0;
	pxShapePropertyList::my_iterator itor = m_PropertyCollection.begin();
	while (itor != m_PropertyCollection.end())
	{
		pxShapePropertyItem& spi = *itor;
		if (spi.GetType() == SPIT_CATEGORY && spi.GetName() == name)
		{
			ret = spi.GetCategory();
			break;
		}
	
		itor++;
	}

	return ret;
}

pxShapePropertyItem* pxShapeProperyCategory::FindItem(const wxString& name) const
{
	pxShapePropertyItem* ret = 0;

	if (m_map.Lookup(name, ret) == 0)
		ret = 0;

	return ret;
}

void pxShapeProperyCategory::FetchValue()
{
	pxShapePropertyList::my_iterator itor = m_PropertyCollection.begin();
	while (itor != m_PropertyCollection.end())
	{
		pxShapePropertyItem& spi = *itor;
		if (spi.GetType() == SPIT_CATEGORY)
		{
			spi.GetCategory()->FetchValue();
		}
		else
			spi.FetchValue();

		itor++;
	}
}

void pxShapeProperyCategory::SendValue()
{
	pxShapePropertyList::my_iterator itor = m_PropertyCollection.begin();
	while (itor != m_PropertyCollection.end())
	{
		pxShapePropertyItem& spi = *itor;
		if (spi.GetType() == SPIT_CATEGORY)
		{
			spi.GetCategory()->SendValue();
		}
		else
			spi.SendValue();

		itor++;
	}
}
