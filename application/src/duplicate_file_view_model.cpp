#include"duplicate_file_view_model.h"
#include<wx/filename.h>
DuplicateFileViewModel::DuplicateFileViewModel(std::unordered_map<HashKey, std::vector<wxString>>& model):
	model{model}
{

	for (auto& pair : model)
	{
		auto it{ groupNodes.emplace(pair.first, pair.first).first };
		auto& selectList{ this->selectList[pair.first] };
		selectList.reserve(model[pair.first].size());
		auto& nodes{ fileNodes[pair.first] };
		nodes.reserve(model[pair.first].size());
	}
	for (auto& pair : model)
	{
		auto& it{ groupNodes.at(pair.first) };
		auto& selectList{ this->selectList[pair.first] };
		auto& nodes{ fileNodes[pair.first] };
		for (auto& file : model[pair.first])
		{
			nodes.push_back(file);
			size_t index{ selectList.size() };
			selectList.push_back(false);
			indices[file] = index;
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
	return 2;
}

void DuplicateFileViewModel::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const
{
	auto& node{ *reinterpret_cast<std::variant<HashKey, wxString>*>(item.GetID()) };
	auto& hashkey{ *std::get_if<HashKey>(&node) };
	wxCheckBoxState state;
	
	if (&hashkey != nullptr)
	{
		bool select{ true };
		wxString text;
		switch (col)
		{
		case 0:
			for (auto const& it: selectList[hashkey])
			{
				select = select && it;
			}

			for (int i = 0; i < 8; ++i)
			{
				text += wxString::Format(wxT("%02X%02X%02X%02X-"),
					hashkey[i * 4 + 0], hashkey[i * 4 + 1], hashkey[i * 4 + 2], hashkey[i * 4 + 3]);
			}
			state = select ? wxCHK_CHECKED : wxCHK_UNCHECKED;
			text.erase(text.length() - 1);
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
		uint64_t filesize;
		
		switch (col)
		{
		case 0:
		{
			auto& parent{ parents[path] };
			auto& index{ indices[path] };
			state = selectList[std::get<HashKey>(*parent)][index] ? wxCHK_CHECKED : wxCHK_UNCHECKED;
			variant << wxDataViewCheckIconText(path, wxNullIcon, state);
		}
			break;
		case 1:
			filesize = wxFileName::GetSize(path).GetValue();
			if (filesize < 1024)
			{
				variant = wxString::Format(wxT("%d bytes"), (int)filesize);
			}
			else if(filesize < 1024 * 1024)
			{
				variant = wxString::Format(wxT("%f kib"), filesize /1024.0);
			}
			else if (filesize < 1024 * 1024 * 1024)
			{
				variant = wxString::Format(wxT("%f mib"), filesize / (1024.0 * 1024.0));
			}
			else if (filesize < 1024ull * 1024ull * 1024ull * 1024ull)
			{
				variant = wxString::Format(wxT("%f gib"), filesize / (1024.0 * 1024.0 * 1024.0));
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

		auto& selects{ selectList[*hashkey] };
		for (auto & it : selects)
		{
			it = select;
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
		hashkey = std::get_if<HashKey>(parent);
		size_t index{ indices.at(file) };
		selectList[*hashkey][index] = select;
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

	return wxDataViewItem{ parents[*file] };
}

unsigned int DuplicateFileViewModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
	void* id{ parent.GetID() };
	if (id == nullptr)
	{
		// Root items.
		for (auto& pair : groupNodes)
		{
			array.Add(wxDataViewItem{ &pair.second });
		}
		return array.size();
	}
	auto& node{ *reinterpret_cast<std::variant<HashKey, wxString>*>(id) };
	auto hashkey{ std::get_if<HashKey>(&node) };
	if (hashkey == nullptr)
		return 0;

	auto& nodes{ fileNodes[*hashkey] };
	for (auto& file : nodes)
	{
		array.Add(wxDataViewItem{ &file });
	}
	return array.size();
}
