//
// Created by vasilyev on 28.06.2019.
//

#include "Propagator.h"
#include "Manager.h"

Propagator::Propagator(PulsedBeam &_pulsed_beam, std::map<std::string, std::string>& args) :
        pulsed_beam(_pulsed_beam) {

    Manager manager(args);
    Logger logger;

    logger.save_initial_parameters(pulsed_beam, manager.current_results_dir, true, true);

}

Propagator::~Propagator() = default;
