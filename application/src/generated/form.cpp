///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form.h"

///////////////////////////////////////////////////////////////////////////

GUIMainFrame::GUIMainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);

	m_panel8 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel8, wxAuiPaneInfo() .Center() .Caption( wxT("main") ).CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).CentrePane() );

	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );

	m_splitter1 = new wxSplitterWindow( m_panel8, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( GUIMainFrame::m_splitter1OnIdle ), NULL, this );

	m_panel1 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_genericDirCtrl2 = new wxGenericDirCtrl( m_panel1, ID_DIR_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRCTRL_3D_INTERNAL|wxDIRCTRL_DIR_ONLY|wxDIRCTRL_MULTIPLE|wxSUNKEN_BORDER, wxEmptyString, 0 );

	m_genericDirCtrl2->ShowHidden( false );
	bSizer2->Add( m_genericDirCtrl2, 1, wxEXPAND, 5 );


	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	m_panel2 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_button2 = new wxButton( m_panel2, ID_ADD_DIR_CTRL, wxT("→"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button2, 1, 0, 5 );

	m_button3 = new wxButton( m_panel2, ID_REMOVE_DIR_CTRL, wxT("←"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button3, 1, 0, 5 );


	bSizer4->Add( bSizer3, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer( wxVERTICAL );

	m_staticText7 = new wxStaticText( m_panel2, wxID_ANY, wxT("Directories for search"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer39->Add( m_staticText7, 0, wxALL, 5 );

	m_listBox1 = new wxListBox( m_panel2, ID_INCLUDE_DIR, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_MULTIPLE );
	bSizer39->Add( m_listBox1, 1, wxEXPAND, 5 );


	bSizer4->Add( bSizer39, 1, wxEXPAND, 5 );


	m_panel2->SetSizer( bSizer4 );
	m_panel2->Layout();
	bSizer4->Fit( m_panel2 );
	m_splitter1->SplitVertically( m_panel1, m_panel2, 0 );
	bSizer26->Add( m_splitter1, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1 = new wxStaticText( m_panel8, wxID_ANY, wxT("file externsion filter (delimiter: ' ')"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer8->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl2 = new wxTextCtrl( m_panel8, ID_EXT_FILTER_CTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_textCtrl2, 1, wxALIGN_CENTER_VERTICAL, 5 );


	bSizer26->Add( bSizer8, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer38;
	bSizer38 = new wxBoxSizer( wxHORIZONTAL );

	m_button1 = new wxButton( m_panel8, ID_START_CTRL, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer38->Add( m_button1, 1, wxALL, 5 );

	m_button28 = new wxButton( m_panel8, ID_STOP_CTRL, wxT("stop"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button28->Enable( false );

	bSizer38->Add( m_button28, 1, wxALL, 5 );


	bSizer26->Add( bSizer38, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxHORIZONTAL );

	m_gauge1 = new wxGauge( m_panel8, ID_PROGRESS, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	m_gauge1->SetValue( 0 );
	bSizer37->Add( m_gauge1, 1, wxALL, 5 );

	m_staticText6 = new wxStaticText( m_panel8, ID_PROGRESS_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	m_staticText6->SetMinSize( wxSize( 80,-1 ) );

	bSizer37->Add( m_staticText6, 0, wxALL, 5 );


	bSizer26->Add( bSizer37, 0, wxEXPAND, 5 );


	m_panel8->SetSizer( bSizer26 );
	m_panel8->Layout();
	bSizer26->Fit( m_panel8 );
	m_panel13 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_mgr.AddPane( m_panel13, wxAuiPaneInfo() .Center() .Caption( wxT("result") ).CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ) );

	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxVERTICAL );

	m_dataViewCtrl1 = new wxDataViewCtrl( m_panel13, ID_REPORT, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE );
	bSizer33->Add( m_dataViewCtrl1, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	m_button4 = new wxButton( m_panel13, ID_DEL_SEL_FILES, wxT("Delete selected files"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_button4, 1, wxALL, 5 );

	m_button27 = new wxButton( m_panel13, ID_MOVE_SEL_FILES_TO, wxT("move seleted files to ..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_button27, 1, wxALL, 5 );


	bSizer33->Add( bSizer7, 0, wxEXPAND, 5 );


	m_panel13->SetSizer( bSizer33 );
	m_panel13->Layout();
	bSizer33->Fit( m_panel13 );

	m_mgr.Update();
	this->Centre( wxBOTH );
}

GUIMainFrame::~GUIMainFrame()
{
	m_mgr.UnInit();

}
