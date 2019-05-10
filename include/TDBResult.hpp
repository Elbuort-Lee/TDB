#pragma once
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>

#ifdef _WIN32
#	pragma warning(disable: 4251)
#	pragma warning(disable: 4996)
#endif

namespace TModel
{
	using TDBFields = std::vector<std::string>;
	using TDBFixResult = TDBFields;

	class TMySQL;
	class TDBRowValue :
		public std::string
	{
	protected:
		bool _bEnable = false;
	public:
		TDBRowValue() {}
		TDBRowValue(const char* src) :std::string(src), _bEnable(true) {}
		int toInt() { return std::stoi(*this); }
		float toFloat() { return std::stof(*this); };
		double toDouble() { return std::stod(*this); }
		long toLong() { return std::stol (*this); }
		long long toLL() { return std::stoll(*this); }
		std::string toString() { return *this; }
		bool enbale() { return _bEnable; }
	};


	class TDBRow :
		public std::unordered_map<std::string, TDBRowValue>
	{
		friend class TMySQL;
	public:
		inline size_t getFieldCount() { return size(); }
		
		TDBFields getFields()
		{
			TDBFields fields;
			for (auto itor : *this)
				fields.push_back(itor.first);
			return std::move(fields);
		}
	};


	class TDBResult : 
		public std::vector<TDBRow>
	{
		friend class TMySQL;
	public:
		TDBRow& operator[](const size_t index) { return this->at(index); }
	};


	class TDBKValue : 
		public std::string
	{
	protected:
		bool _bEnable = false;
	public:
		TDBKValue() {}
		TDBKValue(const char* value) :std::string(value), _bEnable(true) {}
		TDBKValue(const std::string& value) :std::string(value), _bEnable(true) {}
		inline int toInt() { return std::stoi(*this); }
		inline float toFloat() { return std::stof(*this); };
		inline double toDouble() { return std::stod(*this); }
		inline long toLong() { return std::stol(*this); }
		inline long long toLL() { return std::stoll(*this); }
		inline bool enable() { return _bEnable; }
	};


	class TSQLiteKV;
	class TDBKVObject : 
		public std::map<std::string, TDBKValue>
	{
		friend class TSQLiteKV;
	protected:
		int _index = -1;
	public:
		TDBKVObject() {}
		TDBKVObject(const size_t index) : _index(index) {}
		inline int getIndex() { return _index; }
		inline bool enable() { return _index != -1; }
	};

	
	class TDBKVResult :
		public std::vector<TDBKVObject>
	{
	public:
		TDBKVObject& operator[](const size_t rowIndex) { 
			return rowIndex >= size() ? *end() : at(rowIndex);
		}

		TDBKVResult::iterator findObject(const int index)
		{
			return std::find_if(begin(), end(),
				[&](TDBKVObject& obj)->bool
			{
				return obj.getIndex() == index;
			});
		}
	};
}