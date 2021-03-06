//
// Created by vasilyev on 02.07.2019.
//

#ifndef FILAMENTATION_LIF_H
#define FILAMENTATION_LIF_H

#include "medium/material/base_medium.h"

class LiF : public BaseMedium {
public:
    LiF();
    explicit LiF(ConfigManager& _config_manager, std::shared_ptr<Logger>& _logger);
    ~LiF() override;

    void initialize_kerr_parameters() override;
    void initialize_ionization_parameters() override;

private:


};

#endif //FILAMENTATION_LIF_H
