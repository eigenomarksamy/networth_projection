#include <iostream>
#include "yml_prsr.hpp"
#include "utils.hpp"

bool YamlParser::parseYamlFile() {
    try {
        std::ifstream file(m_filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open the YAML file.\n";
            return false;
        }
        m_data = YAML::Load(file);
        return true;
    } catch (const YAML::Exception& e) {
        std::cerr << "Error while parsing the YAML file: " << e.what() << std::endl;
        return false;
    }
}

void YamlParser::extractFieldNames() {
    m_fieldNames.clear();
    m_dataMap.clear();
    extractFieldNamesRecursive(m_data, "");
}

YAML::Node YamlParser::getFieldValue(const std::string& fieldName) const {
    YAML::Node node = findFieldNode(m_data, fieldName);
    return node;
}

void YamlParser::extractFieldNamesRecursive(const YAML::Node& node,
                                            const std::string& parentName) {
    if (node.IsMap()) {
        for (const auto& it : node) {
            if (it.second.IsMap()) {
                std::string nestedName = parentName + it.first.as<std::string>() + ".";
                extractFieldNamesRecursive(it.second, nestedName);
            }
            else {
                std::string fieldName = parentName + it.first.as<std::string>();
                std::string fieldValue = it.second.as<std::string>();
                m_fieldNames.push_back(fieldName);
                m_dataMap[fieldName] = fieldValue;
            }
        }
    }
}

YAML::Node YamlParser::findFieldNode(const YAML::Node& node,
                                     const std::string& fieldName) const {
    YAML::Node result;

    if (node.IsMap()) {
        for (const auto& field : node) {
            std::string key = field.first.as<std::string>();
            if (key == fieldName) {
                return field.second;
            } else {
                result = findFieldNode(field.second, fieldName);
                if (!result.IsNull()) {
                    return result;
                }
            }
        }
    }

    return result;
}

bool YamlParser::getFieldScalarValue(const std::string& fieldName,
                                     std::string& fieldValue) const {
    YAML::Node node = findFieldNode(m_data, fieldName);
    std::cout << node.Type() << std::endl;
    if (node.IsScalar()) {
        fieldValue = node.as<std::string>();
        return true;
    } else {
        return false;
    }
}

bool YamlParser::hasNestedFields(const std::string& fieldName) const {
    YAML::Node node = findFieldNode(m_data, fieldName);
    return node.IsMap();
}

bool YamlParser::findAttributeByName(const std::string& fieldName,
                                     std::string& fieldValue) const {
    if (m_dataMap.find(fieldName) != m_dataMap.end()) {
        fieldValue = m_dataMap.at(fieldName);
        return true;
    }
    return false;
}

std::map<std::string, std::string> YamlParser::getChildrenFields(const std::string& fullFieldName) const {
    std::map<std::string, std::string> childrenFields;
    YAML::Node parentNode = findFieldNode(m_data, fullFieldName);
    if (!parentNode.IsNull() && parentNode.IsMap()) {
        for (const auto& field : parentNode) {
            std::string childFieldName = fullFieldName + "." + field.first.as<std::string>();
            std::string fieldValue;
            if (field.second.IsScalar()) {
                fieldValue = field.second.as<std::string>();
                childrenFields[childFieldName] = fieldValue;
            }
        }
    }
    return childrenFields;
}

void YmlUtils::displayAllFields(const std::string& fileName) {
    YamlParser parser(fileName);
    if (parser.parseYamlFile()) {
        parser.extractFieldNames();
        std::cout << parser.m_dataMap;
    }
}

bool getValueFromYml(const std::string& fileName,
                     const std::string& fieldName,
                     std::string& fieldValue,
                     const std::string& defaultValue) {
    YamlParser parser(fileName);
    if (parser.parseYamlFile()) {
        parser.extractFieldNames();
        auto ret = parser.findAttributeByName(fieldName, fieldValue);
        if (!ret) {
            fieldValue = defaultValue;
        }
        return true;
    }
    return false;
}

bool getValuesFromYml(const std::string& fileName,
                      const std::vector<std::string>& fieldNames,
                      std::vector<std::string>& fieldValues) {
    YamlParser parser(fileName);
    bool ret = false;
    if (parser.parseYamlFile() && (fieldNames.size() == fieldValues.size())) {
        ret = true;
        parser.extractFieldNames();
        for (size_t i = 0; i < fieldNames.size(); ++i) {
            ret &= parser.findAttributeByName(fieldNames[i], fieldValues[i]);
        }
    }
    return ret;
}

bool getChildrenValuesFromYml(const std::string& fileName,
                              const std::string& fieldName,
                              std::map<std::string, std::string>& fieldChildren) {
    YamlParser parser(fileName);
    size_t init_size = fieldChildren.size();
    if (parser.parseYamlFile()) {
        fieldChildren = parser.getChildrenFields(fieldName);
        if (init_size < fieldChildren.size()) {
            return true;
        }
    }
    return false;
}

bool getChildrenNamesFromYml(const std::string& fileName,
                             const std::string& fieldName,
                             std::vector<std::string>& childrenNames) {
    std::map<std::string, std::string> tmp;
    if (getChildrenValuesFromYml(fileName, fieldName, tmp)) {
        for (auto& t : tmp) {
            childrenNames.push_back(t.first);
        }
        return true;
    }
    return false;
}

bool hasChildren(const std::string& fileName,
                 const std::string& fieldName) {
    std::map<std::string, std::string> tmp;
    return getChildrenValuesFromYml(fileName, fieldName, tmp);
}