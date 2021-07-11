#include"duplicate_file_view_model.h"
#include<wx/filename.h>
DuplicateFileViewModel::DuplicateFileViewModel(std::unordered_map<HashKey, std::vector<wxString>>& model, std::unordered_map<HashKey, std::unordered_set<wxString>>& selectedFilesTable):
	model{model},
	selectedFilesTable{ selectedFilesTable }
{

	for (auto& pair : model)
	{
		auto it{ groupNodes.emplace(pair.first, pair.first).first };
		auto& nodes{ fileNodes[pair.first] };
		nodes.reserve(model[pair.first].size());
		selectedFilesTable.insert_or_assign(pair.first, std::unordered_set<wxString>{});
	}
	for (auto& pair : model)
	{
		auto& it{ groupNodes.at(pair.first) };
		auto& nodes{ fileNodes[pair.first] };
		for (auto& file : model[pair.first])
		{
			nodes.push_back(file);
			parents.emplace(file, &it);
		}
	}
}

wxString DuplicateFileViewModel::GetColumnType(unsigned int col) const
{
	switch (col)
	{
	case 0:
		return wxDataViewCheckIconTextRenderer::GetDefaultType();
	case 1:
		return wxT("string");
	case 2:
		return wxT("string");
	case 3:
		return wxT("string");
	}
	return wxT("");
}

int DuplicateFileViewModel::Compare(const wxDataViewItem& item1, const wxDataViewItem& item2, unsigned int column, bool ascending) const
{
	return 0;
}

unsigned int DuplicateFileViewModel::GetColumnCount() const
{
	return 3;
}
#ifdef WIN32
static std::list<std::wstring> g_DateFormats;
BOOL CALLBACK EnumDateFormatsProc(_In_ LPWSTR lpDateFormatString) {
	// Store each format in the global list of dateformats.
	g_DateFormats.push_back(lpDateFormatString);
	return TRUE;
}

std::wstring GetShortDatePattern() {
	if (g_DateFormats.size() == 0 &&
		// Enumerate all system default short dateformats; EnumDateFormatsProc is
		// called for each dateformat.
		!::EnumDateFormats(EnumDateFormatsProc,
			LOCALE_SYSTEM_DEFAULT,
			0)) {
		throw std::runtime_error("EnumDateFormatsW");
	}
	// There can be more than one short date format. Arbitrarily pick the first one:
	return g_DateFormats.front();
}

std::wstring GetShortTimePattern() {
	g_DateFormats.clear();
	if (g_DateFormats.size() == 0 &&
		// Enumerate all system default short dateformats; EnumDateFormatsProc is
		// called for each dateformat.
		!::EnumTimeFormatsW(EnumDateFormatsProc,
			LOCALE_SYSTEM_DEFAULT,
			0)) {
		throw std::runtime_error("EnumTimeFormatsW");
	}
	// There can be more than one short date format. Arbitrarily pick the first one:
	return g_DateFormats.front();
}
#endif


void DuplicateFileViewModel::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const
{
	auto& node{ *reinterpret_cast<std::variant<HashKey, wxString>*>(item.GetID()) };
	auto& hashkey{ *std::get_if<HashKey>(&node) };
	wxCheckBoxState state;
	
	if (&hashkey != nullptr)
	{
		bool select{ true };
		wxString text;
		auto& selectedFiles{ selectedFilesTable.at(hashkey) };
		auto& files{ model.at(hashkey) };
		switch (col)
		{
		case 0:
			for (auto const& it: files)
			{
				select = select && selectedFiles.find(it) != selectedFiles.end();
			}

			for (int i = 0; i < 8; ++i)
			{
				text += wxString::Format(wxT("%02X%02X%02X%02X-"),
					hashkey[i * 4 + 0], hashkey[i * 4 + 1], hashkey[i * 4 + 2], hashkey[i * 4 + 3]);
			}
			text.erase(text.length() - 1);
			state = select ? wxCHK_CHECKED : wxCHK_UNCHECKED;
			variant << wxDataViewCheckIconText(text, wxNullIcon, state);
			break;
		case 1:

			variant = text;
			break;
		}

	}
	if (&hashkey == nullptr)
	{
		auto& path{ *std::get_if<wxString>(&node) };
		wxFileName fileName{ path };
		uint64_t filesize;
		
		switch (col)
		{
		case 0:
		{
			auto& parent{ parents.at(path) };
			auto& hashkey{ std::get<HashKey>(*parent) };
			auto& selectedFiles{ selectedFilesTable.at(hashkey) };
			auto select{ selectedFiles.find(path) != selectedFiles.end() };
			state = select ? wxCHK_CHECKED : wxCHK_UNCHECKED;
			variant << wxDataViewCheckIconText(path, wxNullIcon, state);
		}
		break;
		case 1:

			filesize = fileName.GetSize().GetValue();
			if (filesize < 1024)
			{
				variant = wxString::Format(wxT("%d bytes"), (int)filesize);
			}
			else if (filesize < 1024 * 1024)
			{
				variant = wxString::Format(wxT("%f KiB"), filesize / 1024.0);
			}
			else if (filesize < 1024 * 1024 * 1024)
			{
				variant = wxString::Format(wxT("%f MiB"), filesize / (1024.0 * 1024.0));
			}
			else if (filesize < 1024ull * 1024ull * 1024ull * 1024ull)
			{
				variant = wxString::Format(wxT("%f GiB"), filesize / (1024.0 * 1024.0 * 1024.0));
			}
			break;
		case 2:
		{
			auto locale{ wxLocale::GetSystemLanguage() };
			wxDateTime creationDt;
			wxDateTime modifiedDt;
			wxDateTime accessDt;
			wxString str;
			fileName.GetTimes(&accessDt, &modifiedDt, &creationDt);
#ifdef WIN32
			SYSTEMTIME now;
			GetLocalTime(&now);

			TCHAR date[64];
			TCHAR time[64];
			TCHAR fmt[64];
			SYSTEMTIME systime;
			modifiedDt.GetAsMSWSysTime(&systime);
			GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &systime, NULL, date, 64);
			GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &systime, NULL, time, 64);

			str = date;
			str += wxT(" ");
			str += time;
#else
			str = modifiedDt.Format();
#endif
			variant = str;
		}
		break;
		}
	}
}

bool DuplicateFileViewModel::SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col)
{
	if(col != 0)
		return false;
	if (!item.IsOk())
		return false;

	auto& node{ *reinterpret_cast<std::variant<HashKey, wxString>*>(item.GetID()) };
	auto* hashkey{ std::get_if<HashKey>(&node) };
	wxDataViewCheckIconText value{  };
	value << variant;
	bool select{ value.GetCheckedState() != wxCHK_UNCHECKED };

	if (hashkey != nullptr)
	{
		auto& selects{ selectedFilesTable[*hashkey] };
		if (select)
		{
			auto& table{ model[*hashkey] };
			for (auto& it: table)
			{
				selects.insert(it);
			}
		}
		else
		{
			selects.clear();
		}

		wxDataViewItemArray arr;
		for (auto& it : fileNodes[*hashkey])
		{
			arr.push_back(wxDataViewItem{ &it });
		}
		ItemsChanged(arr);
		return true;
	}
	else
	{
		auto& file{ *std::get_if<wxString>(&node) };
		auto parent{ parents.at(file) };
		hashkey = &std::get<HashKey>(*parent);
		auto& selectedFiles{ selectedFilesTable[*hashkey] };
		if (select)
		{
			selectedFiles.insert(file);
		}
		else
		{
			selectedFiles.erase(file);
		}
		//ItemChanged(wxDataViewItem{ parent });
		ItemChanged(wxDataViewItem{ parent });
		return true;
	}
	return false;
}

bool DuplicateFileViewModel::IsEnabled(const wxDataViewItem& item, unsigned int col) const
{
	return true;
}

bool DuplicateFileViewModel::IsContainer(const wxDataViewItem& item) const
{
	if (!item.IsOk())
		return true;
	void* id = item.GetID();
	auto& node{ *reinterpret_cast<std::variant<HashKey, wxString>*>(id) };
	auto hashkey{ std::get_if<HashKey>(&node) };
	return hashkey != nullptr;
}

wxDataViewItem DuplicateFileViewModel::GetParent(const wxDataViewItem& item) const
{
	// the invisible root node has no parent
	if (!item.IsOk())
		return wxDataViewItem{ nullptr };

	auto& node{ *reinterpret_cast<std::variant<HashKey, wxString>*>(item.GetID()) };
	auto file{ std::get_if<wxString>(&node) };
	
	if (file == nullptr)
		return wxDataViewItem{ nullptr };

	return wxDataViewItem{ const_cast<std::variant<HashKey,wxString>*>(parents.at(*file)) };
}

unsigned int DuplicateFileViewModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
	void* id{ parent.GetID() };
	if (id == nullptr)
	{
		// Root items.
		for (auto& pair : groupNodes)
		{
			array.Add(wxDataViewItem{ const_cast<std::variant<HashKey,wxString>*>( &pair.second )});
		}
		return array.size();
	}
	auto& node{ *reinterpret_cast<std::variant<HashKey, wxString>*>(id) };
	auto hashkey{ std::get_if<HashKey>(&node) };
	if (hashkey == nullptr)
		return 0;

	auto& nodes{ fileNodes.at(*hashkey) };
	for (auto& file : nodes)
	{
		array.Add(wxDataViewItem{ const_cast<std::variant<HashKey,wxString>*>(&file) });
	}
	return array.size();
}
