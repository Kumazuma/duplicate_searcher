///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/dirctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/splitter.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/gauge.h>
#include <wx/dataview.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>

///////////////////////////////////////////////////////////////////////////

#define ID_DIR_CTRL 1000
#define ID_EXT_FILTER_CTRL 1001
#define ID_START_CTRL 1002
#define ID_PROGRESS 1003
#define ID_REPORT 1004
#define ID_DEL_SEL_FILES 1005
#define ID_MOVE_SEL_FILES_TO 1006

///////////////////////////////////////////////////////////////////////////////
/// Class GUIMainFrame
///////////////////////////////////////////////////////////////////////////////
class GUIMainFrame : public wxFrame
{
	private:

	protected:
		wxPanel* m_panel8;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel1;
		wxGenericDirCtrl* m_genericDirCtrl2;
		wxPanel* m_panel2;
		wxButton* m_button2;
		wxButton* m_button3;
		wxListBox* m_listBox1;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrl2;
		wxButton* m_button1;
		wxGauge* m_gauge1;
		wxPanel* m_panel13;
		wxDataViewCtrl* m_dataViewCtrl1;
		wxButton* m_button4;
		wxButton* m_button27;

	public:

		GUIMainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		wxAuiManager m_mgr;

		~GUIMainFrame();

		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 0 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( GUIMainFrame::m_splitter1OnIdle ), NULL, this );
		}

};

