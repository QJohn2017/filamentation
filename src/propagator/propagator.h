//
// Created by vasilyev on 28.06.2019.
//

#ifndef FILAMENTATION_PROPAGATOR_H
#define FILAMENTATION_PROPAGATOR_H

#include <map>
#include <string>
#include <chrono>
#include <memory>

#include "pulsed_beam/base_pulsed_beam.h"
#include "medium/material/base_medium.h"

#include "manager/config_manager/config_manager.h"
#include "manager/dir_manager/dir_manager.h"
#include "postprocessor/postprocessor.h"
#include "saver/saver.h"
#include "logger/logger.h"

#include "term/wave_equation/linear/diffraction/diffraction.h"
#include "term/wave_equation/linear/dispersion/dispersion_full.h"
#include "term/wave_equation/linear/dispersion/dispersion_gvd.h"
#include "term/wave_equation/nonlinear/kerr/kerr.h"
#include "term/wave_equation/nonlinear/plasma/plasma.h"
#include "term/wave_equation/nonlinear/bremsstrahlung/bremsstrahlung.h"
#include "term/wave_equation/nonlinear/dissipation/dissipation.h"

#include "term/kinetic_equation/kinetic_equation.h"

#include "executor/linear_executor/linear_executor.h"
#include "executor/nonlinear_executor/nonlinear_executor.h"


class Propagator {
public:
    Propagator();
    explicit Propagator(std::shared_ptr<BasePulsedBeam>& _pulsed_beam,
                        ConfigManager& _config_manager,
                        DirManager& _dir_manager,
                        Postprocessor& _postprocessor,
                        std::shared_ptr<Logger>& _logger);
    ~Propagator();

    std::shared_ptr<BasePulsedBeam> pulsed_beam;

    ConfigManager config_manager;
    DirManager dir_manager;
    Postprocessor postprocessor;
    std::shared_ptr<Logger> logger;

    Saver saver;

    // executors
    std::shared_ptr<LinearExecutor> linear_executor;
    std::shared_ptr<NonlinearExecutor> nonlinear_executor;

    void propagate();

private:

};

#endif //FILAMENTATION_PROPAGATOR_H
