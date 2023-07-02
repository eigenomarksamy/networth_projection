#include "yml_prsr.hpp"

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

void YamlParser::extractFieldNamesRecursive(const YAML::Node& node, const std::string& parentName) {
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
                m_dataMap[fieldName] = fieldValue; // Populate the attribute map
            }
        }
    }
}

YAML::Node YamlParser::findFieldNode(const YAML::Node& node, const std::string& fieldName) const {
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

bool YamlParser::getFieldScalarValue(const std::string& fieldName, std::string& fieldValue) const {
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

std::string YamlParser::findAttributeByName(const std::string& fieldName) const {
    std::string value;
    if (m_dataMap.find(fieldName) != m_dataMap.end()) {
        value = m_dataMap.at(fieldName);
    }
    return value;
}

void execYamlParserDemo() {
    YamlParser parser("conf/data.yaml");

    if (parser.parseYamlFile()) {
        parser.extractFieldNames();
        const std::vector<std::string>& fieldNames = parser.getFieldNames();

        std::cout << "Field names in the YAML file:" << std::endl;
        for (const std::string& fieldName : fieldNames) {
            std::cout << fieldName << std::endl;
            // if (!parser.hasNestedFields(fieldName)) {
            //     std::string fieldValue;
            //     if (parser.getFieldScalarValue(fieldName, fieldValue)) {
            //         std::cout << fieldName << ": " << fieldValue << std::endl;
            //     } else {
            //         std::cout << fieldName << ": (Not a scalar value or not found)" << std::endl;
            //     }
            // }

            // if (!fieldValue.IsNull()) {
            // } else {
            //     std::cout << "Value not found!" << std::endl;
            // }
        }

        // Access the attribute map
        const std::map<std::string, std::string>& attributeMap = parser.getDataMap();
        std::cout << "\nAttribute map:\n";
        for (const auto& entry : attributeMap) {
            std::cout << entry.first << ": " << entry.second << std::endl;
        }
        auto value = parser.findAttributeByName("person.name");
        std::cout << value << std::endl;
    }
}

void execYamlParserDemo2() {
    try {
        // Open the YAML file for reading
        std::ifstream file("conf/data.yaml");
        if (!file.is_open()) {
            std::cerr << "Error: Could not open the YAML file.\n";
            return;
        }

        // Parse the YAML file
        YAML::Node data = YAML::Load(file);

        // Access the nested data
        std::string name = data["person"]["name"].as<std::string>();
        int age = data["person"]["age"].as<int>();
        std::string email = data["person"]["contact"]["email"].as<std::string>();
        std::string phone = data["person"]["contact"]["phone"].as<std::string>();

        const YAML::Node& allFields = data;
        if (allFields.IsMap()) {
            for (const auto& field : allFields) {
                std::string fieldName = field.first.as<std::string>();
                std::cout << "Field name: " << fieldName << std::endl;
            }
        }
        std::cout << "---\n";

        // Extract field names from the top-level map (person in this case)
        const YAML::Node& personNode = data["animal"];
        if (personNode.IsMap()) {
            for (const auto& field : personNode) {
                std::string fieldName = field.first.as<std::string>();
                std::cout << "Field name: " << fieldName << std::endl;
            }
            try {
                const auto& animalContact = data["animal"]["type"];
                std::cout << animalContact.as<std::string>() << std::endl;
            } catch (const YAML::Exception& e) {
                std::cout << e.what() << std::endl;
            }
            try {
                const auto& animalContact = data["animal"];
                std::cout << animalContact.as<std::string>() << std::endl;
            } catch (const YAML::Exception& e) {
                std::cout << e.what() << std::endl;
            }
            try {
                const auto& animalContact = data["animal"]["type"]["email"];
                std::cout << animalContact.as<std::string>() << std::endl;
            } catch (const YAML::Exception& e) {
                std::cout << e.what() << std::endl;
            }
            try {
                const auto& animalContact = data["animal"]["contact"];
                std::cout << animalContact.as<std::string>() << std::endl;
            } catch (const YAML::Exception& e) {
                std::cout << e.what() << std::endl;
            }
        }

        // Output the parsed data
        std::cout << "Name: " << name << std::endl;
        std::cout << "Age: " << age << std::endl;
        std::cout << "Email: " << email << std::endl;
        std::cout << "Phone: " << phone << std::endl;

    } catch (const YAML::Exception& e) {
        std::cerr << "Error while parsing the YAML file: " << e.what() << std::endl;
        return;
    }
}