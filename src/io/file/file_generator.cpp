#include <algorithm>
#include <iterator>
#include <filesystem>
#include <iostream>
#include "file_generator.hpp"


bool FileGenerator::checkFileExtension(std::string expExt) {
    if (m_fileName.size() < 3) {
        return false;
    }
    std::string act_ext;
    std::copy_n(m_fileName.end() - 3, 3, std::back_inserter(act_ext));
    return (act_ext == expExt);
}

void FileGenerator::generateCsv(std::vector<std::string>& headers,
                              std::vector<std::vector<std::string>>& lines) {
    if (m_file.is_open() && checkFileExtension(std::string("csv"))) {
        for (const auto& header : headers) {
            m_file << header + ", ";
        }
        m_file << "\n";
        for (const auto& line : lines) {
            for (const auto& l : line) {
                m_file << l + ", ";
            }
            m_file << "\n";
        }
    }
}

void FileGenerator::generateTxt(std::vector<std::string>& lines) {
    if (m_file.is_open() && checkFileExtension(std::string("txt"))) {
        for (const auto& line : lines) {
            m_file << line + "\n";
        }
    }
}

std::vector<std::string> getFileNames(const std::string& directoryPath) {
    std::vector<std::string> fileNames;
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            fileNames.push_back(entry.path().filename().string());
        }
    }
    return fileNames;
}