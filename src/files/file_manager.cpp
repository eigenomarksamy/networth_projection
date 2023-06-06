#include <stdexcept>
#include "file_manager.hpp"

void FileHandler::generateCsv(std::vector<std::string>& headers,
                              std::vector<std::vector<std::string>>& lines) {
    if (m_file.is_open()) {
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