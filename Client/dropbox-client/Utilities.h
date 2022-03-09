/**
* Freedom Project
* Copyright (C) 2017 RangeMachine
*/

#pragma once

#include "Common.h"

std::string RandomStringABigLetters(int lengtha);
std::wstring TurnAToW(const std::string& str);
std::wstring RandomStringW(int lengtha, bool add);
std::string RandomStringA(int lengtha, bool add);
std::vector<std::wstring> splitW(const std::wstring& s, wchar_t seperator);
std::vector<std::string> splitA(const std::string& s, const char* seperator);
std::wstring GetExecutableDirectory();
std::wstring GetFileDirectory(const std::wstring& fileName);
std::string GetFileDirectory(const std::string& fileName);
std::wstring GetRelativeFilePath(const std::wstring& directory, const std::wstring& fileName);
std::wstring GetRelativeFileShortPath(const std::wstring& directory, const std::wstring& fileName);
void PrintFormat(const char* format, ...);
void PrintWFormat(const wchar_t* format, ...);
std::string TextFormat(const char* format, ...);
std::wstring TextWFormat(const wchar_t* format, ...);
