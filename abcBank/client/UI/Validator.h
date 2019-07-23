#pragma once

#include <wx/wx.h>


class Validator
{
public:
static bool validateNameAndTip(const wxString&);
static bool validatePasswordAndTip(const wxString&);
static bool validateIdAndTip(const wxString&);
static bool validateAccountIdAndTip(const wxString&);
static bool validateMoneyAndTip(const wxString&);
static bool validateDateAndTip(const wxString&);
static bool validateRepeatAndTip(const wxString&, const wxString&);


static bool validateName(const wxString&);
static bool validatePassword(const wxString&);
static bool validateId(const wxString&);
static bool validateAccountId(const wxString&);
static bool validateMoney(const wxString&);
static bool validateDate(const wxString&);
};
