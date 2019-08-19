//
// Created by vasilyev on 24.07.2019.
//

#ifndef FILAMENTATION_PROCESSORDISPERSION_H
#define FILAMENTATION_PROCESSORDISPERSION_H

#include "logger/postprocessor/postprocessor.h"
#include "manager/config_manager/config_manager.h"

class ProcessorDispersion : public Postprocessor {
public:
    ProcessorDispersion();
    ProcessorDispersion(ConfigManager& _config_manager, DirManager& _dir_manager);

    std::string path_to_test_script;

    void go() override;

    ~ProcessorDispersion();
};


#endif //FILAMENTATION_PROCESSORDISPERSION_H
