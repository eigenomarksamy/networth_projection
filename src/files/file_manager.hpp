#ifndef FILE_MANAGER_HPP_
#define FILE_MANAGER_HPP_

#include <vector>
#include <string>
#include <fstream>

class FileHandler {

    bool checkFileExtension(std::string expExt);

    std::string m_fileName;
    std::ofstream m_file;

public:

    FileHandler(std::string& filename) : m_fileName(filename) {
        m_file.open(filename);
    }

    ~FileHandler() {
        if (m_file.is_open()) {
            m_file.close();
        }
    }

    void generateCsv(std::vector<std::string>& headers,
                     std::vector<std::vector<std::string>>& lines);

    void generateTxt(std::vector<std::string>& lines);

};

#endif /* FILE_MANAGER_HPP_ */