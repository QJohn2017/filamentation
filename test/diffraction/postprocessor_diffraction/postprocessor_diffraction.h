//
// Created by vasilyev on 21.08.2019.
//

#ifndef FILAMENTATION_POSTPROCESSORDIFFRACTION_H
#define FILAMENTATION_POSTPROCESSORDIFFRACTION_H

#include "postprocessor/postprocessor.h"

class PostprocessorDiffraction : public Postprocessor {
public:
    PostprocessorDiffraction();

    explicit PostprocessorDiffraction(ConfigManager& _config_manager);

    ~PostprocessorDiffraction();

    void go(DirManager& _dir_manager,
            std::shared_ptr<Logger>& _logger) override;

};


#endif //FILAMENTATION_POSTPROCESSORDIFFRACTION_H
