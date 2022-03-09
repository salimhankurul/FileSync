/**
 * Freedom Project
 * Copyright (C) 2017 RangeMachine
 */

#pragma once

#define COMPILE_SEED 7231369381

template <int X>
struct EnsureCompileTime
{
	enum : int
	{
		Value = X
	};
};

constexpr int LinearCongruentGenerator(int rounds)
{
	return 1013904223 + 1664525 * ((rounds > 0) ? LinearCongruentGenerator(rounds - 1) : COMPILE_SEED & 0xFFFFFFFF);
}

#define Random() EnsureCompileTime<LinearCongruentGenerator(10)>::Value
#define RandomNumber(Min, Max) (Min + (Random() % (Max - Min + 1)))

const char g_xorKey = static_cast<char>(RandomNumber(0, 0xFF));

template <int... Pack> struct IndexList {};

template <typename IndexList, int Right> struct Append;

template <int... Left, int Right> struct Append<IndexList<Left...>, Right> 
{
	typedef IndexList<Left..., Right> Result;
};

template <int N> struct ConstructIndexList 
{
	typedef typename Append<typename ConstructIndexList<N - 1>::Result, N - 1>::Result Result;
};

template <> struct ConstructIndexList<0> 
{
	typedef IndexList<> Result;
};

constexpr char EncryptCharacter(const char character, int index) 
{
	return character ^ (g_xorKey + index);
}

constexpr wchar_t EncryptWCharacter(const wchar_t character, int index)
{
	return character ^ (g_xorKey + index);
}

template <typename IndexList> class MetaString;

template <int... Index> class MetaString<IndexList<Index...> > 
{
	public:
		__forceinline constexpr MetaString(const char* const value)
			: m_value { EncryptCharacter(value[Index], Index)... } 
		{
		}

		char* Decrypt() 
		{
			for (int t = 0; t < sizeof...(Index); t++) 
			{
				m_value[t] = m_value[t] ^ (g_xorKey + t);
			}

			m_value[sizeof...(Index)] = '\0';

			return m_value;
		}

	private:
		char m_value[sizeof...(Index) + 1];
};

template <typename IndexList> class MetaWString;

template <int... Index> class MetaWString<IndexList<Index...> > 
{
	public:
		__forceinline constexpr MetaWString(const wchar_t* const value)
			: m_value { EncryptWCharacter(value[Index], Index)... } 
		{
		}

		wchar_t* Decrypt() 
		{
			for (int t = 0; t < sizeof...(Index); t++) 
			{
				m_value[t] = m_value[t] ^ (g_xorKey + t);
			}

			m_value[sizeof...(Index)] = '\0';

			return m_value;
		}

	private:
		wchar_t m_value[sizeof...(Index) + 1];
};

#define META_STRING(x) (MetaString<ConstructIndexList<sizeof(x) - 1>::Result>(x).Decrypt()) 
#define META_WSTRING(x) (MetaWString<ConstructIndexList<sizeof(x) - 1>::Result>(x).Decrypt()) 