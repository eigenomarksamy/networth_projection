#include "utils.hpp"
#include "portfolio_cfg.hpp"

bool portfolio::resolveCfg(const std::string& confPath, PortfolioMgrYmlDirs& dirs) {
    YmlCfg dir_cfg(confPath);
    dirs.generation = YmlCfg::createConfigElm("portfolio-manager.generation");
    dir_cfg.addConfigElement(dirs.generation);
    dirs.database = YmlCfg::createConfigElm("portfolio-manager.database");
    dir_cfg.addConfigElement(dirs.database);
    dirs.logs = YmlCfg::createConfigElm("portfolio-manager.logs");
    dir_cfg.addConfigElement(dirs.logs);
    dirs.text = YmlCfg::createConfigElm("portfolio-manager.text");
    dir_cfg.addConfigElement(dirs.text);
    dirs.configuration = YmlCfg::createConfigElm("portfolio-manager.configuration");
    dir_cfg.addConfigElement(dirs.configuration);
    if (dir_cfg.readCfg(false, false)) {
        return true;
    }
    return false;
}

bool portfolio::resolveCfg(const std::string& confPath, PortfolioMgrYmlVals& vals) {
    YmlCfg val_cfg(confPath);
    vals.auto_load = YmlCfg::createConfigElm("portfolio-manager.auto-load");
    val_cfg.addConfigElement(vals.auto_load);
    vals.auto_log = YmlCfg::createConfigElm("portfolio-manager.auto-log");
    val_cfg.addConfigElement(vals.auto_log);
    vals.auto_save = YmlCfg::createConfigElm("portfolio-manager.auto-save");
    val_cfg.addConfigElement(vals.auto_save);
    vals.source = YmlCfg::createConfigElm("portfolio-manager.source");
    val_cfg.addConfigElement(vals.source);
    vals.table_name = YmlCfg::createConfigElm("portfolio-manager.table-name");
    val_cfg.addConfigElement(vals.table_name);
    if (val_cfg.readCfg(false, false)) {
        return true;
    }
    return false;
}

void portfolio::convertYmlData(PortfolioMgrCfg& inputs,
                               const PortfolioMgrYmlDirs& dirs) {
    PortfolioMgrYmlVals vals;
    auto conf_file = dirs.configuration->value;
    if (resolveCfg(conf_file, vals)) {
        inputs.db_dir = dirs.database->value;
        inputs.gen_dir = dirs.generation->value;
        inputs.portfolio_src = vals.source->value;
        inputs.table_name = vals.table_name->value;
        inputs.load_all_portfolios = convertStrToBool(vals.auto_load->value,
                                                      inputs.load_all_portfolios);
        inputs.auto_log = convertStrToBool(vals.auto_log->value, inputs.auto_log);
        inputs.auto_save = convertStrToBool(vals.auto_save->value, inputs.auto_save);
    }
}