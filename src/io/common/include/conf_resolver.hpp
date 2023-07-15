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
    ConfigElm(const std::string& name) : name(name), full_name(name) {}
} config_elm_t;

struct DirectoriesValues {
    std::shared_ptr<config_elm_t> mortg_calc_in;
    std::shared_ptr<config_elm_t> mortg_calc_out;
    std::shared_ptr<config_elm_t> netwo_calc_in;
    std::shared_ptr<config_elm_t> netwo_calc_out;
    std::shared_ptr<config_elm_t> porto_dirs_out;
    std::shared_ptr<config_elm_t> porto_overview;
};

class YmlCfg {
public:
    YmlCfg(const std::string& confFile) : m_fileName(confFile) {}
    void addConfigElement(const std::vector<std::shared_ptr<config_elm_t>>& elemnts);
    void addConfigElement(const std::shared_ptr<config_elm_t>& elemnt) { m_cfgElms.push_back(elemnt); }
    bool readCfg(const bool createMap, const bool useDefaults);
    std::string getValue(const std::string& fullName);
    static std::shared_ptr<config_elm_t> createConfigElm(const std::string& fullName);
private:
    std::string m_fileName;
    std::vector<std::shared_ptr<config_elm_t>> m_cfgElms;
    std::map<std::string, std::string> m_cfgMap;
};

#endif /* CONF_RESOLVER_HPP_ */