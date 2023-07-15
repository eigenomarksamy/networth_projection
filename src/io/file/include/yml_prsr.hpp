#ifndef YML_PRSR
#define YML_PRSR

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "yaml-cpp/yaml.h"

class YamlParser {

public:
    explicit YamlParser(const std::string& filename) : m_filename(filename) {}

    bool parseYamlFile();

    void extractFieldNames();

    const std::vector<std::string>& getFieldNames() const { return m_fieldNames; }

    const std::map<std::string, std::string>& getDataMap() const { return m_dataMap; }

    YAML::Node getFieldValue(const std::string& fieldName) const;

    bool hasNestedFields(const std::string& fieldName) const;

    bool getFieldScalarValue(const std::string& fieldName, std::string& fieldValue) const;

    bool findAttributeByName(const std::string& fieldName,
                             std::string& fieldValue) const;

    std::map<std::string, std::string> getChildrenFields(const std::string& fullFieldName) const;

    friend class YmlUtils;

private:
    std::string m_filename;
    std::vector<std::string> m_fieldNames;
    YAML::Node m_data;
    std::map<std::string, std::string> m_dataMap;

    void extractFieldNamesRecursive(const YAML::Node& node, const std::string& parentKey);
    YAML::Node findFieldNode(const YAML::Node& node, const std::string& fieldName) const;
};

class YmlUtils {
public:
    void displayAllFields(const std::string& fileName);
};

bool getValueFromYml(const std::string& fileName,
                     const std::string& fieldName,
                     std::string& fieldValue,
                     const std::string& defaultValue="");

bool getValuesFromYml(const std::string& fileName,
                      const std::vector<std::string>& fieldNames,
                      std::vector<std::string>& fieldValues);

bool getChildrenValuesFromYml(const std::string& fileName,
                              const std::string& fieldName,
                              std::map<std::string, std::string>& fieldChildren);

bool getChildrenNamesFromYml(const std::string& fileName,
                             const std::string& fieldName,
                             std::vector<std::string>& childrenNames);

bool hasChildren(const std::string& fileName,
                 const std::string& fieldName);

#endif /* YML_PRSR */