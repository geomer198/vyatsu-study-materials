#include <wx/wx.h>
#include <wx/valnum.h>

#include <map>
#include <memory>

#include "conductors.h"

using fpvalidator = wxFloatingPointValidator<double>;
using stodmap = std::map<wxString, double>;

class MetalSizer : public wxGridSizer {
private:
	BaseConductor* conductor;
	stodmap resistivitymp;
	wxComboBox* resistivityBox;
	wxTextCtrl* lengthEntry;
	wxTextCtrl* surfaceEntry;
	wxButton* compute;

	void OnComputeClick(wxCommandEvent& event) {
		double val;
		bool ok;
		ok = lengthEntry->GetValue().ToDouble(&val);
		conductor->setLength(ok ? val : 0);

		conductor->setResistivity(resistivitymp.at(resistivityBox->GetStringSelection()));

		ok = surfaceEntry->GetValue().ToDouble(&val);
		dynamic_cast<MetalConductor*>(conductor)->setSurface(ok ? val : 0);

		if (this->conductor->check()) {
			wxString res = wxString::FromDouble(conductor->resistance());
			wxMessageBox("������������� ����������: " + res + " ��", "���������");
		}
		else {
			wxMessageBox("������������ ������", "������", wxICON_ERROR);
		}
	}
public:
	MetalSizer(wxWindow* parent, const stodmap& mr) : wxGridSizer(4, 2, 0, 0), resistivitymp(mr), conductor(new MetalConductor()) {
		wxSizerFlags textFlags = wxSizerFlags().Left();
		wxSizerFlags boxFlags = wxSizerFlags().Top().Right();

		wxStaticText* resistivity = new wxStaticText(parent, wxID_ANY, "�������� �������������\n(��*��^2/�)");
		wxStaticText* length = new wxStaticText(parent, wxID_ANY, "����� (�)");
		wxStaticText* surface = new wxStaticText(parent, wxID_ANY, "������� ������� (��^2)");
		
		resistivity->SetExtraStyle(wxALIGN_LEFT);
		length->SetExtraStyle(wxALIGN_LEFT);
		surface->SetExtraStyle(wxALIGN_LEFT);

		resistivityBox = new wxComboBox(parent, wxID_ANY);
		lengthEntry = new wxTextCtrl(parent, wxID_ANY);
		surfaceEntry = new wxTextCtrl(parent, wxID_ANY);

		lengthEntry->SetValidator(fpvalidator(3, nullptr, wxNUM_VAL_NO_TRAILING_ZEROES));
		surfaceEntry->SetValidator(fpvalidator(3, nullptr, wxNUM_VAL_NO_TRAILING_ZEROES));

		resistivityBox->SetEditable(false);
		for (const auto& p : resistivitymp) {
			resistivityBox->AppendString(p.first);
		}
		resistivityBox->SetSelection(0);

		compute = new wxButton(parent, wxID_ANY, "���������");
		compute->Bind(wxEVT_BUTTON, &MetalSizer::OnComputeClick, this);

		this->Add(resistivity, textFlags);
		this->Add(resistivityBox, boxFlags);
		this->Add(length, textFlags);
		this->Add(lengthEntry, boxFlags);
		this->Add(surface, textFlags);
		this->Add(surfaceEntry, boxFlags);
		this->Add(compute, boxFlags);
	}
};

class LiquidSizer : public wxGridSizer {
private:
	BaseConductor* conductor;
	stodmap resistivitymp;
	wxComboBox* resistivityBox;
	wxTextCtrl* lengthEntry;
	wxTextCtrl* widthEntry;
	wxTextCtrl* heightEntry;
	
	void OnComputeClick(wxCommandEvent& event) {
		double val;
		bool ok;
		ok = lengthEntry->GetValue().ToDouble(&val);
		conductor->setLength(ok ? val : 0);

		ok = widthEntry->GetValue().ToDouble(&val);
		dynamic_cast<LiquidConductor*>(conductor)->setWidth(ok ? val : 0);

		ok = heightEntry->GetValue().ToDouble(&val);
		dynamic_cast<LiquidConductor*>(conductor)->setHeight(ok ? val : 0);

		conductor->setResistivity(resistivitymp.at(resistivityBox->GetStringSelection()));

		if (this->conductor->check()) {
			wxString res = wxString::FromDouble(conductor->resistance());
			wxMessageBox("������������� ����������: " + res + " ��", "���������");
		}
		else {
			wxMessageBox("������������ ������", "������", wxICON_ERROR);
		}
	}
public:
	LiquidSizer(wxWindow* parent, const stodmap& lr) : wxGridSizer(5, 2, 0, 0), resistivitymp(lr), conductor(new LiquidConductor()) {
		wxSizerFlags textFlags = wxSizerFlags().Left();
		wxSizerFlags boxFlags = wxSizerFlags().Top().Right();

		wxStaticText* resistivity = new wxStaticText(parent, wxID_ANY, "�������� �������������\n(��*��^2/�)");
		wxStaticText* length = new wxStaticText(parent, wxID_ANY, "����� (�)");
		wxStaticText* width = new wxStaticText(parent, wxID_ANY, "������ (�)");
		wxStaticText* height = new wxStaticText(parent, wxID_ANY, "������ (�)");
		for (wxStaticText* text : { resistivity, length, width, height }) {
			text->SetExtraStyle(wxALIGN_LEFT);
		}

		resistivityBox = new wxComboBox(parent, wxID_ANY);
		lengthEntry = new wxTextCtrl(parent, wxID_ANY);
		widthEntry = new wxTextCtrl(parent, wxID_ANY);
		heightEntry = new wxTextCtrl(parent, wxID_ANY);
		
		lengthEntry->SetValidator(fpvalidator(3, nullptr, wxNUM_VAL_NO_TRAILING_ZEROES));
		widthEntry->SetValidator(fpvalidator(3, nullptr, wxNUM_VAL_NO_TRAILING_ZEROES));
		heightEntry->SetValidator(fpvalidator(3, nullptr, wxNUM_VAL_NO_TRAILING_ZEROES));

		resistivityBox->SetEditable(false);
		for (const auto& p : resistivitymp) {
			resistivityBox->AppendString(p.first);
		}
		resistivityBox->SetSelection(0);

		wxButton* compute = new wxButton(parent, wxID_ANY, "���������");
		compute->Bind(wxEVT_BUTTON, &LiquidSizer::OnComputeClick, this);

		this->Add(resistivity, textFlags);
		this->Add(resistivityBox, boxFlags);
		this->Add(length, textFlags);
		this->Add(lengthEntry, boxFlags);
		this->Add(width, textFlags);
		this->Add(widthEntry, boxFlags);
		this->Add(height, textFlags);
		this->Add(heightEntry, boxFlags);
		this->Add(compute, boxFlags);
	}
};

class MainFrame : public wxFrame {
public:
	MainFrame() : wxFrame(NULL, wxID_ANY, "Lab6") {
		stodmap mr = {
			{ "��������", 0.028  },
			{ "������",   0.1    },
			{ "������",   0.1    },
			{ "������",   0.023  },
			{ "������",   0.025  },
			{ "����",     0.0175 },
			{ "�����",    0.12   },
			{ "�������",  0.107  },
			{ "������",   0.22   },
			{ "�������",  0.015  },
			{ "����",     0.054  },
			{ "�����",    0.94   },
			{ "��������", 0.05   }
		};
		stodmap lr = {
			{ "��������� ������ ", 0.032 },
			{ "������ �������",    0.315 },
			{ "������ �������",    0.025 },
			{ "������� �������",   0.016 },
			{ "������ ������",     0.083 }
		};

		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		wxSizerFlags flags = wxSizerFlags().Expand().Border();

		sizer->Add(new MetalSizer(this, mr), flags);
		sizer->Add(new LiquidSizer(this, lr), flags);

		this->SetSizer(sizer);
		this->SetSize(wxSize(600, 300));
		this->SetBackgroundColour(wxColor("White"));
	}
};

class Lab6App : public wxApp {
public:
	virtual bool OnInit() {
		MainFrame *frame = new MainFrame();
		frame->Show(true);
		return true;
	}
};

wxIMPLEMENT_APP(Lab6App);
