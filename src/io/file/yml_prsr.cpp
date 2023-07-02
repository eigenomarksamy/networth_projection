#include "yml_prsr.hpp"
#include "yaml-cpp/yaml.h"

void execYamlParserDemo() {
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