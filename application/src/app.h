#pragma once
#include<wx/wx.h>
#include<wx/dataview.h>
#include"searcher.h"
class App : public wxApp
{
public:
	virtual bool OnInit() override;
	virtual int OnExit() override;
protected:
	void OnClickStart(wxCommandEvent& event);
	void OnProcessSearch(wxThreadEvent& event);
	void OnProcessSuccess(wxThreadEvent& event);
private:
	wxFrame* frame;
	Searcher* searcher;
	wxDataViewModel* viewModel;
	std::unordered_map<HashKey, std::vector<wxString> > table;
};

wxDECLARE_APP(App);