#if !defined(_PXSHAPEPROPERTY_BASE_H_)
#define _PXSHAPEPROPERTY_BASE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pxDef.h"
#include "flxPool.h"

typedef enum 
{
	SPIT_INVALID = 0,
	SPIT_TEXT,
	SPIT_DOUBLE,
	SPIT_BOOLEAN,
	SPIT_LONG,
	SPIT_CATEGORY,
}SPIT;

class pxShapePropertyItem;
class pxShapeProperyCategory;

class pxShapePropertyItem
{
public:
	pxShapePropertyItem();
	pxShapePropertyItem(wxString Name, SPIT type);
	~pxShapePropertyItem();

	void Clone(const pxShapePropertyItem& spi);

	wxString GetName() const { return m_Name; };
	SPIT GetType() const { return m_SPIT; }

	wxString GetText();
	void SetText(wxString text);

	pxColour GetColor();
	void SetColor(wxColour& color);

	pxFontInfo GetFont();
	void SetFont(wxFont& font);

	double GetDouble();
	void SetDouble(double dblval);

	bool GetBoolean();
	void SetBoolean(bool dblval);

	long GetLong();
	void SetLong(long val);

	pxShapeProperyCategory* GetCategory();
	void SetCategory(pxShapeProperyCategory* cateory);

	void* GetTargetPtr() const { return m_target_ptr; }
	void SetTargetPtr(void* val) { m_target_ptr = val; }
	int GetTargetLen() const { return m_target_len; }
	void SetTargetLen(int val) { m_target_len = val; }

	const wxVariant& Value() const { return m_Value; }
	void Value(const wxVariant& val) { m_Value = val; }

	void AttachTarget(void* target_ptr, int target_len);
	void FetchValue();
	void SendValue();

protected:
	wxString m_Name;
	SPIT m_SPIT;
	wxVariant m_Value;
	void* m_target_ptr;
	int m_target_len;
};

typedef flxList<pxShapePropertyItem> pxShapePropertyList;
class pxShapeProperyCategory
{
public:
	pxShapeProperyCategory();
	virtual ~pxShapeProperyCategory();

	void Clone(const pxShapeProperyCategory& spc);
	void FillFrom(const pxShapeProperyCategory& spc);

	pxShapeProperyCategory& AddCategory(const wxString& name);
	pxShapePropertyItem& AddItem(const wxString& name, SPIT type, void* ptr, int len);

	pxShapeProperyCategory* FindCategory(const wxString& name) const;
	pxShapePropertyItem* FindItem(const wxString& name) const;

	int GetPropertyItemCount();

	void FetchValue();
	void SendValue();

protected:
	static int GetPropertyItemCount(pxShapeProperyCategory* spc);
	const pxShapePropertyList& GetPropertyList() const { return m_PropertyCollection; };

	pxShapePropertyList m_PropertyCollection;

	flxHash<wxString, pxShapePropertyItem*> m_map;
};

#endif
