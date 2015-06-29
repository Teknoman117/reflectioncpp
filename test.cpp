#include <iostream>
#include <string>
#include <map>

#include <boost/thread/shared_mutex.hpp>

#include "common.hpp"
#include "type.hpp"

using namespace reflectioncpp;

class MetaType
{
public:
	virtual std::string Name() const = 0;
	virtual size_t Size() const = 0;
	virtual TypeCode Identifier() const = 0;
};

template <typename T>
class MetaTypeImpl : public MetaType
{
public:
	size_t Size() const override
	{
		return sizeof(T);
	}

	std::string Name() const override
	{
		return TypeInfo<T>::GetName();
	}

	TypeCode Identifier() const override
	{
		return TypeInfo<T>::GetHashCode();
	}
};

class MetaTypeDatabase
{
	std::map<TypeCode, MetaType *> database;
	boost::shared_mutex databaseAccess;

	static MetaTypeDatabase& Instance()
	{
		static MetaTypeDatabase instance;
		return instance;
	}

public:
	static void RegisterType(MetaType *type)
	{
		boost::unique_lock<boost::shared_mutex> lock(Instance().databaseAccess);
		Instance().database[type->Identifier()] = type;
	}

	static MetaType* GetType(TypeCode typeId)
	{
		boost::shared_lock<boost::shared_mutex> lock(Instance().databaseAccess);
		return Instance().database[typeId];
	}

	template <typename T>
	static MetaType* Get()
	{
		boost::shared_lock<boost::shared_mutex> lock(Instance().databaseAccess);
		return Instance().database[TypeInfo<T>::GetHashCode()];
	}
};

struct Herp
{
	int a;
	char b;
	float c;
};
EXPORT_TYPEINFO(Herp, Herp)

// Constant strlen
constexpr size_t const_strlen(const char *str, const int idx = 0)
{
	return (str[idx] == '\0') ? idx : const_strlen(str, idx+1);
}
constexpr const char *aString = "I like pie";
constexpr size_t aStringLength = const_strlen(aString);


int main (int argc, char** argv)
{
	MetaTypeDatabase::RegisterType(new MetaTypeImpl<Herp>);
	MetaTypeDatabase::RegisterType(new MetaTypeImpl<int>);

	MetaType *metadata = MetaTypeDatabase::Get<Herp>();
	MetaType *integer = MetaTypeDatabase::Get<int>();

	std::cout << TypeInfo<int>::GetName() << ": " << TypeInfo<int>::GetHashCode() << std::endl;

	std::cout << integer->Name() << ": " << integer->Identifier() << std::endl;

	std::cout << metadata->Name() << ": " << metadata->Identifier() << std::endl;

	return 0;
}
