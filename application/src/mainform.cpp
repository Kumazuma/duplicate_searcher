#include "mainform.h"

MainForm::MainForm():
	GUIMainFrame{nullptr, wxID_ANY, wxT("")}
{
    wxDataViewColumn* const colCheckIconText = new wxDataViewColumn
    (
        L"\u2714 Path",
        new wxDataViewCheckIconTextRenderer(wxDATAVIEW_CELL_ACTIVATABLE),
        0
    );
    
	m_dataViewCtrl1->AppendColumn(colCheckIconText);
	m_dataViewCtrl1->AppendTextColumn(wxT("Size"), 1, wxDATAVIEW_CELL_ACTIVATABLE, -1, static_cast<wxAlignment>(wxALIGN_LEFT), wxDATAVIEW_COL_RESIZABLE);
}
