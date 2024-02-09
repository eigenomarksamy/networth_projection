#ifndef FILE_GENERATOR_HPP_
#define FILE_GENERATOR_HPP_

#include <vector>
#include <string>
#include <fstream>

class FileGenerator {

    bool checkFileExtension(std::string expExt, uint8_t extSize = 3);

    std::string m_fileName;
    std::ofstream m_file;

public:

    FileGenerator(const std::string& filename) : m_fileName(filename) {
        m_file.open(filename);
    }

    ~FileGenerator() {
        if (m_file.is_open()) {
            m_file.close();
        }
    }

    void generateCsv(std::vector<std::string>& headers,
                     std::vector<std::vector<std::string>>& lines);

    void generateTxt(std::vector<std::string>& lines);

};

std::vector<std::string> getFileNames(const std::string& directoryPath);

#endif /* FILE_GENERATOR_HPP_ */