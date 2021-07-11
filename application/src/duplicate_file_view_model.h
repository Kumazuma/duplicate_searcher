#pragma once
#include<wx/wx.h>
#include<wx/dataview.h>
#include<unordered_map>
#include<unordered_set>
#include<hashkey.h>
#include<variant>
#include<map>
class DuplicateFileViewModel : public wxDataViewModel
{
public:
	DuplicateFileViewModel(std::unordered_map<HashKey, std::vector<wxString>>& model, std::unordered_map<HashKey, std::unordered_set<wxString>>& selectedFilesTable);
	virtual wxString GetColumnType(unsigned int col)const override;
	virtual int Compare(const wxDataViewItem& item1, const wxDataViewItem& item2, unsigned int column, bool ascending) const override;
	virtual unsigned int GetColumnCount() const override;
	virtual void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const override;
	virtual bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col) override;
	virtual bool IsEnabled(const wxDataViewItem& item, unsigned int col)const override;
	virtual bool IsContainer(const wxDataViewItem& item) const override;
	virtual wxDataViewItem GetParent(const wxDataViewItem& item) const;
	virtual unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const override;
private:
	std::unordered_map<HashKey, std::vector<wxString>>& model;
	std::unordered_map<HashKey, std::unordered_set<wxString>>& selectedFilesTable;
	
	std::unordered_map<HashKey, std::variant<HashKey, wxString>> groupNodes;
	std::unordered_map<HashKey, std::vector<std::variant<HashKey, wxString>>> fileNodes;
	std::unordered_map<wxString, std::variant<HashKey, wxString>*> parents;
};