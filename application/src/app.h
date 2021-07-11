#pragma once
#include<wx/wx.h>
#include<wx/dataview.h>
#include<unordered_set>
#include"searcher.h"
class App : public wxApp
{
public:
	virtual bool OnInit() override;
	virtual int OnExit() override;
protected:
	void OnClickStart(wxCommandEvent& event);
	void OnClickDelectSelctedFiles(wxCommandEvent& event);
	void OnClickMoveFilesTo(wxCommandEvent& event);
	void OnProcessSearch(wxThreadEvent& event);
	void OnProcessSuccess(wxThreadEvent& event);
	void UpdateViewModel();
private:
	wxFrame* frame;
	Searcher* searcher;
	wxDataViewModel* viewModel;
	std::unordered_map<HashKey, std::vector<wxString> > table;
	std::unordered_map<HashKey, std::unordered_set<wxString>> selectedFilesTable;
};

wxDECLARE_APP(App);