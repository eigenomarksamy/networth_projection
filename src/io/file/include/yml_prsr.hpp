#ifndef YML_PRSR
#define YML_PRSR

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class YamlParser {

public:
    YamlParser(const std::string& filename) : m_filename(filename) {}

    bool parseFile(std::vector<std::string>& data);

private:
    std::string m_filename;
};

void execYamlParserDemo();

#endif /* YML_PRSR */