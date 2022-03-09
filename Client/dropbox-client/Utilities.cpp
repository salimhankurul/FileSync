/**
* Freedom Project
* Copyright (C) 2017 RangeMachine
*/

#pragma once

#include "Utilities.h"
#include "MetaString.h"

#include <ctime>
#include <random>
#include <iostream>
#include <codecvt>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 255);


std::wstring TurnAToW(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}


std::wstring RandomStringW(int lengtha, bool add)
{
	srand(std::time(0));

	std::wstring allowedChars = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::wstring result;

	int position;

	while (result.size() != lengtha)
	{
		position = dis(gen);

		if (position > allowedChars.size())
			continue;

		result += allowedChars.at(position);
	}

	if (add)
		result += L"_";

	return result;
}

std::string RandomStringA(int lengtha, bool add)
{
	srand(std::time(0));

	std::string allowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string result;

	int position;

	while (result.size() != lengtha)
	{
		position = dis(gen);

		if (position > allowedChars.size())
			continue;

		result += allowedChars.at(position);
	}

	if (add)
		result += "_";

	return result;
}

std::string RandomStringABigLetters(int lengtha)
{
	srand(std::time(0));

	std::string allowedChars = "ABCDEFGHKLMNOPRSTUVWXYZ";
	std::string result;

	int position;

	while (result.size() != lengtha)
	{
		position = dis(gen);

		if (position >= allowedChars.size())
			continue;

		result += allowedChars.at(position);
	}

	return result;
}

std::vector<std::wstring> splitW(const std::wstring& s, wchar_t seperator)
{
	std::vector<std::wstring> output;

	std::wstring::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::wstring::npos)
	{
		std::wstring substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}

std::vector<std::string> splitA(const std::string& s, const char* seperator)
{
	std::vector<std::string> output;

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}

std::wstring GetExecutableDirectory()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);

	std::wstring result = std::wstring(buffer);

	return result.substr(0, result.find_last_of(META_WSTRING(L"\\/")));
}

std::wstring GetFileDirectory(const std::wstring& fileName)
{
	std::wstring result = std::wstring(fileName);

	return result.substr(0, result.find_last_of(META_WSTRING(L"\\/")));
}

std::string GetFileDirectory(const std::string& fileName)
{
	std::string result = std::string(fileName);

	return result.substr(0, result.find_last_of(META_STRING("\\/")));
}

std::wstring GetRelativeFilePath(const std::wstring& directory, const std::wstring& fileName)
{
	return std::wstring(directory).append(META_WSTRING(L"\\")).append(fileName);
}

std::wstring GetRelativeFileShortPath(const std::wstring& directory, const std::wstring& fileName)
{
	wchar_t buffer[MAX_PATH];
	GetShortPathName(directory.c_str(), buffer, sizeof(buffer));

	return std::wstring(buffer).append(META_WSTRING(L"\\")).append(fileName);
}

void PrintFormat(const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);

	vprintf(format, argptr);

	va_end(argptr);
}

void PrintWFormat(const wchar_t* format, ...)
{
	va_list argptr;
	va_start(argptr, format);

	vwprintf(format, argptr);

	va_end(argptr);
}

std::string TextFormat(const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);

	char buffer[2048];
	vsprintf(buffer, format, argptr);

	va_end(argptr);

	return buffer;
}

std::wstring TextWFormat(const wchar_t* format, ...)
{
	va_list argptr;
	va_start(argptr, format);

	wchar_t buffer[2048];
	vswprintf(buffer, format, argptr);

	va_end(argptr);

	return buffer;
}

