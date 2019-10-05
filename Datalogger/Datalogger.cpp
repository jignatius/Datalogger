// Datalogger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Poco\JSON\Object.h>
#include <Poco\JSON\Parser.h>
#include <Poco\Dynamic\Var.h>
#include <Poco\JSON\Stringifier.h>
#include <map>
#include <type_traits>
#include <sstream>


class MapToJsonConvertor
{
public:
	MapToJsonConvertor() = default;
	~MapToJsonConvertor() = default;

	template<typename T, typename = typename std::enable_if<std::is_integral<T>::value ||
		std::is_enum<T>::value, T>::type>
	Poco::JSON::Object::Ptr Convert(const std::map<int, T> &inputMap)
	{
		//Create root object
		Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
		//Create array to put into root
		Poco::JSON::Array::Ptr array = new Poco::JSON::Array();

		//Iterate map and create JSON objects for each element
		if (!inputMap.empty())
		{
			for (auto& elem : inputMap)
			{
				Poco::JSON::Object::Ptr elemPtr = new Poco::JSON::Object();
				elemPtr->set("key", elem.first);
				elemPtr->set("value", static_cast<int>(elem.second));
				array->add(elemPtr);
			}
		}

		root->set("elements", array);
		return root;
	}

	template<typename T, typename = typename std::enable_if<std::is_integral<T>::value ||
		std::is_enum<T>::value, T>::type>
	std::map<int, T> Convert(const Poco::JSON::Object::Ptr& jsonObject)
	{
		std::map<int, T> outputMap;
		Poco::JSON::Array::Ptr arr = jsonObject->getArray("elements");

		//Iterate JSON object and create an entry in map for each array element
		if (arr)
		{
			for (Poco::JSON::Array::ConstIterator it = arr->begin(); it != arr->end(); ++it)
			{
				const Poco::JSON::Object::Ptr elem = it->extract<Poco::JSON::Object::Ptr>();

				if (elem)
				{
					int key(0);
					T value;

					Poco::Dynamic::Var keyVar = elem->get("key");
					if (!keyVar.isEmpty())
					{
						key = keyVar.convert<int>();
					}

					Poco::Dynamic::Var valVar = elem->get("value");
					if (!valVar.isEmpty())
					{
						value = static_cast<T>(valVar.convert<int>());
					}

					outputMap[key] = value;
				}
			}
		}

		return outputMap;
	}

};

enum TestE
{
	Online,
	Offline
};

enum class TestC
{
	Online,
	Offline
};

int main()
{
	std::map<int, TestC> t;
	t[1] = TestC::Online;
	t[2] = TestC::Offline;
	MapToJsonConvertor convertor;
	auto ptr = convertor.Convert(t);

	std::map<int, TestC> t2;
	t2 = convertor.Convert<TestC>(ptr);

	std::stringstream ss;
	Poco::JSON::Stringifier::stringify(ptr, ss);
	std::string message = ss.str();

    std::cout << message;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
