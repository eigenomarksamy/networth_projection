#ifndef CONF_RESOLVER_HPP_
#define CONF_RESOLVER_HPP_

#include <string>
#include <vector>
#include <map>
#include <memory>

typedef struct ConfigElm {
    std::string full_name;
    std::string name;
    std::string value;
    ConfigElm(const std::string& name,
              const std::vector<std::string>& family)
      : name(name) {
        full_name = "";
        for (const auto& fam : family) {
            full_name += fam + ".";
        }
        full_name += name;
    }
} config_elm_t;

class YmlCfg {
public:
    YmlCfg(const std::string& confFile) : m_fileName(confFile) {}
    void addConfigElement(const std::vector<std::shared_ptr<config_elm_t>>& elemnts);
    void addConfigElement(const std::shared_ptr<config_elm_t>& elemnt) { m_cfgElms.push_back(elemnt); }
    bool readCfg(const bool createMap, const bool useDefaults);
    std::string getValue(const std::string& fullName);
    std::map<std::string, std::string> getMap() const { return m_cfgMap; }
    static std::shared_ptr<config_elm_t> createConfigElm(const std::string& fullName);
    static std::vector<std::shared_ptr<config_elm_t>> createConfigElm(
                                                         const std::string& fullName,
                                                         const std::string& fileName);
private:
    std::string m_fileName;
    std::vector<std::shared_ptr<config_elm_t>> m_cfgElms;
    std::map<std::string, std::string> m_cfgMap;
};

#endif /* CONF_RESOLVER_HPP_ */