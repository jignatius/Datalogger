#include "MbusMappingParser.h"
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NodeFilter.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Query.h>
#include <Poco/JSON/Stringifier.h>
#include <fstream>
#include <sstream>
#include <unordered_map>

namespace
{
	std::unordered_map<std::string, DeviceNode> DeviceNodesMap = { {"manufacturer", DeviceNode::Manufacturer},
																   {"version", DeviceNode::Version},
																   {"xml_json_mappings", DeviceNode::MbusJsonMappings},
																   {"xml_path", DeviceNode::XmlPath},
																   {"json_path", DeviceNode::JsonPath},
																   {"json_key", DeviceNode::JsonKey} };

	const std::string ManufacturerPath = "/MBusData/SlaveInformation/Manufacturer";
	const std::string VersionPath = "/MBusData/SlaveInformation/Version";
}

MbusMappingParser::MbusMappingParser(const std::string& path): mFilePath(path)
{
}

void MbusMappingParser::Load()
{
	std::ifstream in(mFilePath);
	Poco::XML::InputSource src(in);
	Poco::XML::DOMParser parser;
	Poco::AutoPtr<Poco::XML::Document> pDoc = parser.parse(&src);

	Poco::XML::Element* pRootElem = pDoc->documentElement();
	//Iterate over every child node (non-recursively)
	Poco::AutoPtr<Poco::XML::NodeList> pElems = pRootElem->getElementsByTagName("device");

	for (int i = 0; i < pElems->length(); ++i)
	{
		MBusDevice device;
		Poco::XML::Node* pNode = pElems->item(i);

		auto List = pNode->childNodes();
		for (auto i = 0; i < List->length(); i++)
		{
			auto item = List->item(i);
			auto type = item->nodeType();

			if (type == Poco::XML::Node::ELEMENT_NODE)
			{
				auto name = item->nodeName();
				auto it = DeviceNodesMap.find(name);

				if (it != DeviceNodesMap.end())
				{
					switch (it->second)
					{
					case DeviceNode::Manufacturer:
						device.Manufacturer = item->innerText();
						break;
					case DeviceNode::Version:
						device.Version = item->innerText();
						break;
					case DeviceNode::MbusJsonMappings:
					{
						Poco::XML::Element* elem = dynamic_cast<Poco::XML::Element*>(item);
						if (elem)
						{
							auto mappings = elem->getElementsByTagName("mapping");
							ParseMappings(mappings, device);
						}
						break;
					}
					default:
						break;
					}
				}
			}
		}

		mDeviceMappings.push_back(device);
	}
}

void MbusMappingParser::ParseMappings(const Poco::AutoPtr<Poco::XML::NodeList> &pElems, MBusDevice& device)
{
	for (auto i = 0; i < pElems->length(); i++)
	{
		Poco::XML::Node* pNode = pElems->item(i);
		auto List = pNode->childNodes();
		MBusMapping mapping;

		for (auto i = 0; i < List->length(); i++)
		{
			auto item = List->item(i);
			auto type = item->nodeType();

			if (type == Poco::XML::Node::ELEMENT_NODE)
			{
				auto name = item->nodeName();
				auto it = DeviceNodesMap.find(name);

				if (it != DeviceNodesMap.end())
				{
					switch (it->second)
					{
					case DeviceNode::XmlPath:
						mapping.XmlPath = item->innerText();
						break;
					case DeviceNode::JsonPath:
						mapping.JsonPath = item->innerText();
						break;
					case DeviceNode::JsonKey:
						mapping.JsonKey = item->innerText();
						break;
					default:
						break;
					}
				}
			}
		}

		device.Mappings.push_back(mapping);
	}
}

std::string MbusMappingParser::Parse(const std::string& xml, const std::string& jsonTemplate)
{
	std::string json;

	try
	{
		//Load mbus xml
		Poco::XML::DOMParser parser;
		Poco::AutoPtr<Poco::XML::Document> pDoc = parser.parseString(xml);

		//Load json template
		Poco::JSON::Parser json_parser;
		Poco::Dynamic::Var json_object = json_parser.parse(jsonTemplate);

		auto deviceIt = GetDeviceInformation(pDoc);
		SetJsonValues(deviceIt, pDoc, json_object);

		std::stringstream sJson;
		Poco::JSON::Stringifier::stringify(json_object, sJson, true);
		json = sJson.str();
	}
	catch (Poco::Exception &e)
	{
		std::cerr << e.displayText() << std::endl;
	}

	return json;
}

std::vector<MBusDevice>::iterator MbusMappingParser::GetDeviceInformation(const Poco::AutoPtr<Poco::XML::Document>& pDoc)
{
	auto pManufacturerNode = pDoc->getNodeByPath(ManufacturerPath);
	auto pVersionNode = pDoc->getNodeByPath(VersionPath);

	std::string manufacturer;
	std::string version;

	if (pManufacturerNode)
	{
		manufacturer = pManufacturerNode->innerText();
	}

	if (pVersionNode)
	{
		version = pVersionNode->innerText();
	}

	auto it = std::find_if(mDeviceMappings.begin(), mDeviceMappings.end(),
		[&manufacturer, &version](const MBusDevice& device) {
			return device.Manufacturer == manufacturer && device.Version == version; });

	return it;
}

void MbusMappingParser::SetJsonValues(DeviceIter deviceIt, Poco::AutoPtr<Poco::XML::Document> &pDoc, Poco::Dynamic::Var &jsonObject)
{
	if (deviceIt != mDeviceMappings.end())
	{
		for (auto& mapping : deviceIt->Mappings)
		{
			auto pNode = pDoc->getNodeByPath(mapping.XmlPath);

			Poco::JSON::Query query(jsonObject);
			Poco::Dynamic::Var jsonNode = query.find(mapping.JsonPath);

			if (pNode && !jsonNode.isEmpty())
			{
				Poco::JSON::Object::Ptr object = jsonObject.extract<Poco::JSON::Object::Ptr>();
				object->set(mapping.JsonKey, pNode->innerText());
			}
		}
	}
}