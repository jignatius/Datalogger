#pragma once

#include <string>
#include <vector>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/NodeList.h>
#include <Poco/AutoPtr.h>
#include <Poco/JSON/Object.h>

enum class DeviceNode
{
	Manufacturer,
	Version,
	MbusJsonMappings,
	XmlPath,
	JsonPath,
	JsonKey
};

struct MBusMapping
{
	std::string XmlPath;
	std::string JsonPath;
	std::string JsonKey;
};

struct MBusDevice
{
	std::string Manufacturer;
	std::string Version;
	std::vector<MBusMapping> Mappings;
};

class MbusMappingParser
{
private:
	typedef std::vector<MBusDevice>::iterator DeviceIter;
	typedef std::vector<MBusMapping>::iterator MappingIter;

public:
	MbusMappingParser(const std::string &path);
	~MbusMappingParser() = default;

	void Load();
	std::string Parse(const std::string& xml, const std::string& jsonTemplate);

private:
	void ParseMappings(const Poco::AutoPtr<Poco::XML::NodeList> &pElems, MBusDevice& device);
	DeviceIter GetDeviceInformation(const Poco::AutoPtr<Poco::XML::Document>& pDoc);
	void SetJsonValues(DeviceIter deviceIt, Poco::AutoPtr<Poco::XML::Document> &pDoc, Poco::Dynamic::Var &jsonObject);

private:
	std::string mFilePath;
	std::vector<MBusDevice> mDeviceMappings;
};

