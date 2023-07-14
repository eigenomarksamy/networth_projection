#include "conf_resolver.hpp"
#include "yml_prsr.hpp"
#include "utils.hpp"

void DirCfg::addConfigElement(const std::vector<std::shared_ptr<config_elm_t>>& elemnts) {
    for (const auto& elm : elemnts) {
        addConfigElement(elm);
    }
}

bool DirCfg::readCfg(const bool createMap, const bool useDefaults) {
    bool retVal;
    if (useDefaults) {
        for (auto& cfgElm : m_cfgElms) {
            (void)getValueFromYml(m_fileName, cfgElm->full_name, cfgElm->value, "");
        }
        retVal = true;
    }
    else {
        std::vector<std::string> names(m_cfgElms.size());
        std::vector<std::string> values(m_cfgElms.size());
        for (auto i = 0; i < m_cfgElms.size(); ++i) {
            names[i] = m_cfgElms[i]->full_name;
        }
        if (getValuesFromYml(m_fileName, names, values)) {
            for (auto i = 0; i < m_cfgElms.size(); ++i) {
                m_cfgElms[i]->value = values[i];
            }
            retVal = true;
        }
        else {
            retVal = false;
        }
    }
    if (createMap && retVal) {
        m_cfgMap.clear();
        for (const auto& cfgElm : m_cfgElms) {
            m_cfgMap.insert(std::make_pair(cfgElm->full_name, cfgElm->value));
        }
    }
    return retVal;
}

std::string DirCfg::getValue(const std::string& fullName) {
    std::string retVal = "";
    if (m_cfgMap.find(fullName) != m_cfgMap.end()) {
        retVal = m_cfgMap[fullName];
    }
    else {
        for (const auto cfgElm : m_cfgElms) {
            if (cfgElm->full_name == fullName) {
                retVal = cfgElm->value;
                break;
            }
        }
    }
    return retVal;
}

std::shared_ptr<config_elm_t> DirCfg::createConfigElm(const std::string& fullName) {
    std::vector<std::string> names;
    splitStr(fullName, '.', names);
    std::string firstName = names.back();
    names.pop_back();
    config_elm_t config(firstName, names);
    auto ptr = std::make_shared<config_elm_t>(firstName, names);
    return ptr;
}