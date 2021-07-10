#include<wx/wx.h>
#include"../searcher/searcher.h"
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
};

wxDECLARE_APP(App);