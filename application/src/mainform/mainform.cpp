#include "mainform.h"

MainForm::MainForm():
	GUIMainFrame{nullptr, wxID_ANY, wxT("")}
{
    wxListItem col;
    col.SetId(0);
    col.SetText(wxT("file name"));
	reportCtrl->InsertColumn(0, col);

    col.SetId(1);
    col.SetText(wxT("group"));
    reportCtrl->InsertColumn(1, col);

    col.SetId(2);
    col.SetText(wxT("file size"));
    reportCtrl->InsertColumn(2, col);
}
