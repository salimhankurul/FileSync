#include "Handler.h"
#include "Base64/base64.h"
#include "WinHttpClient/WinHttpClient.h"

std::string Handler::base_path("");
std::string Handler::username("");

using std::filesystem::directory_iterator;

#define HOST META_WSTRING(L"http://localhost:5000/")
#define UPLOAD_FILE_POST META_WSTRING(L"wAddFsedODVDTvDsqJdgGm")
#define CREATE_SERVER_ACCESS_AUTH META_WSTRING(L"Tm8dFLOyJMoMzsVETEbp")

std::string firstpath("");

namespace Decryption
{
	template<class T> T __ROL__(T value, int count) // there is _rotl WINAPI func, i pasted this from somewhere, cant remember atm 
	{
		const u32 nbits = sizeof(T) * 8;

		if (count > 0)
		{
			count %= nbits;
			T high = value >> (nbits - count);
			if (T(-1) < 0) // signed value
				high &= ~((T(-1) << count));
			value <<= count;
			value |= high;
		}
		else
		{
			count = -count % nbits;
			T low = value << (nbits - count);
			value >>= count;
			value |= low;
		}
		return value;
	}

	inline uint8_t  __ROR1__(uint8_t value, int count) { return  __ROL__((uint8_t)value, -count); }
	inline uint16_t __ROR2__(uint16_t value, int count) { return __ROL__((uint16_t)value, -count); }
	inline u32 __ROR4__(u32 value, int count) { return __ROL__((u32)value, -count); }
	inline u64 __ROR8__(u64 value, int count) { return __ROL__((u64)value, -count); }

	inline uint8_t  __ROL1__(uint8_t  value, int count) { return __ROL__((uint8_t)value, count); }
	inline uint16_t __ROL2__(uint16_t value, int count) { return __ROL__((uint16_t)value, count); }
	inline u32 __ROL4__(u32 value, int count) { return __ROL__((u32)value, count); }
	inline u64 __ROL8__(u64 value, int count) { return __ROL__((u64)value, count); }

}

// Simple rotating bits encryption, no need for fancy stuff atm.
std::wstring DecrptResponse(const u8* content, u32 Size)
{
	auto buffer = new BYTE[Size];
	memcpy(buffer, content, Size);

	for (size_t i = 0; i < Size; i++)
		buffer[i] = Decryption::__ROL1__(buffer[i], 4);

	std::string b((char*)buffer, Size);
	delete[] buffer;
	return std::wstring(b.begin(), b.end());
}


// server sends encrypted response
std::wstring GetAUTHKey() {
	try {
		auto URL = TextWFormat(META_WSTRING(L"%s%s"), HOST, CREATE_SERVER_ACCESS_AUTH);

		WinHttpClient GetShit(URL);

		if (!GetShit.SendHttpRequest())
			throw;

		auto response = DecrptResponse(GetShit.GetRawResponseContent(), GetShit.GetRawResponseContentLength());

		if (!response.compare(META_WSTRING(L"404")))
			throw;

		return response;
	}
	catch (...)
	{
		PrintWFormat(META_WSTRING(L"**** [Task Failed Successfully]: Something went wrong at operation #101 ****"));
		return L"";
	}
}



bool UploadFile(std::wstring path, std::string path_as_base64,std::string filename_as_base64) {
	try {

		std::ifstream is(path, std::ifstream::binary);

		if (!is)
			throw 0;

		// get length of file:
		is.seekg(0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);

		char* buffer = new char[length];
		is.read(buffer, length);
		is.close();

		for (size_t i = 0; i < length; i++)
		{
			buffer[i] = Decryption::__ROL1__(buffer[i], 4) /*^ 0x5C*/;
		}

		// Lets Get File Size First
		WinHttpClient client(TextWFormat(META_WSTRING(L"%s%s/?auth=%s&username=%hs&path=%hs&filename=%hs"), HOST, UPLOAD_FILE_POST,GetAUTHKey().c_str(), Handler::username.c_str(), path_as_base64.c_str(), filename_as_base64.c_str()));
		client.SetAdditionalDataToSend((BYTE*)buffer, length);

		wstring headers;
		headers += META_WSTRING(L"Connection: keep-alive");
		headers += META_WSTRING(L"\r\n");
		headers += META_WSTRING(L"Content-Length: ");
		headers += std::to_wstring(length);
		headers += META_WSTRING(L"\r\n");
		headers += META_WSTRING(L"Content-Type: application/octet-stream");
		headers += META_WSTRING(L"\r\n");

		client.SetAdditionalRequestHeaders(headers);
		client.SendHttpRequest(META_WSTRING(L"GET"));

		delete[] buffer;
		return true;

	}
	catch (...)
	{
		PrintWFormat(META_WSTRING(L"**** [Task Failed Successfully]: Something went wrong at operation #111 ****"));
	}
};

bool Handler::HandleFile(std::filesystem::path FilePath)
{
	auto path_as_string = FilePath.string();
	auto file_directory = GetFileDirectory(path_as_string);
	auto file_name = FilePath.filename().string();

	std::cout << "path_as_string : " << path_as_string << "\n";
	std::cout << "file_directory : " << file_directory << "\n";
	std::cout << "file_name : " << file_name << "\n";
	
	//  
	// Generate a new path based on target directory
	// 
	//THIS C:\Users\kurul\Desktop\Dropbox-Clone\Server\node_modules
	//TOTHIS Dropbox-Clone\Server\node_modules
	auto based_path = std::string(path_as_string.substr(base_path.size(), file_directory.size() - base_path.size()));
	std::cout << "based_path : " << based_path << "\n";

	auto based_path_as_base64 = based_path.size() > 0 ? macaron::Base64::Encode(based_path) : "";
	auto filename_as_base64 = macaron::Base64::Encode(file_name);

	std::cout << "based_path_as_base64 : " << based_path_as_base64 << "\n";
	std::cout << "filename_as_base64 : " << filename_as_base64 << "\n";

	UploadFile(FilePath.wstring(), based_path_as_base64, filename_as_base64);
}

bool Handler::HandlePath(std::filesystem::path Path)
{

	if (!std::filesystem::exists(Path))
	{
		std::cout << "Target Path does not exist! \n";
		return false;
	}

	for (const auto& file : directory_iterator(Path))
	{
		// Iterate trough subdirectories as well
		if (std::filesystem::is_directory(file.path()))
		{
			std::cout << "Directory : " << file.path() << "\n";
			HandlePath(file.path());
		}
		else
		{
			HandleFile(file.path());
		}
	}


	return true;
}