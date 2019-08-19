//
// Created by vasilyev on 30.06.2019.
//

#ifndef FILAMENTATION_LOGGER_H
#define FILAMENTATION_LOGGER_H

#include <map>
#include <vector>

#include "pulsed_beam/gauss/gauss.h"
#include "pulsed_beam/ring/ring.h"
#include "pulsed_beam/vortex/vortex.h"
#include "manager/dir_manager/dir_manager.h"
#include "manager/config_manager/config_manager.h"
#include "logger/postprocessor/postprocessor.h"
#include "term/wave_equation/linear/base_linear_term.h"
#include "term/wave_equation/nonlinear/base_nonlinear_term.h"
#include "term/kinetic_equation/kinetic_equation.h"
#include "executor/linear_executor/linear_executor.h"
#include "executor/nonlinear_executor/nonlinear_executor.h"

#include "diffraction/processor_diffraction.h"
#include "dispersion/processor_dispersion.h"

template<typename PB, typename P> class Logger;

template<template<typename, typename...> class PulsedBeam, typename Medium, typename Postprocessor>
class Logger<PulsedBeam<Medium>, Postprocessor> {
public:
    Logger();
    explicit Logger(ConfigManager& _config_manager,
                    DirManager& _dir_manager,
                    Postprocessor& _postprocessor,
                    std::shared_ptr<PulsedBeam<Medium>> _pulsed_beam,
                    std::shared_ptr<LinearExecutor<PulsedBeam<Medium>>> _linear_executor,
                    std::shared_ptr<NonlinearExecutor<PulsedBeam<Medium>>> _nonlinear_executor);
    ~Logger();

    ConfigManager config_manager;
    DirManager dir_manager;
    Postprocessor postprocessor;

    std::shared_ptr<PulsedBeam<Medium>> pulsed_beam;

    std::shared_ptr<LinearExecutor<PulsedBeam<Medium>>> linear_executor;
    std::shared_ptr<NonlinearExecutor<PulsedBeam<Medium>>> nonlinear_executor;

    std::vector<std::string> states_columns;
    std::vector<std::vector<double>> states;

    void add_start(std::string& tex_file_data);
    void add_end(std::string& tex_file_data);
    void add_to_tex_file_data(std::string& tex_file_data, std::string& str, std::vector<size_t>& params);
    void add_to_tex_file_data(std::string& tex_file_data, std::string& str, std::vector<double>& params);
    void add_to_tex_file_data(std::string& tex_file_data, std::string& str, std::vector<std::string>& params);
    void add_to_yml_file_data(std::string& tex_file_data, std::string& str, std::vector<double>& params);
    void add_to_yml_file_data(std::string& tex_file_data, std::string& str, std::vector<std::string>& params);

    void save_initial_parameters_to_pdf(bool delete_tmp_files, bool delete_tex_file);
    void save_initial_parameters_to_yml();

    void print_current_state(size_t step, double z, double dz);
    void flush_current_state(size_t step, double z, double dz);
    void save_states_to_csv();
    void save_field(int step);
    void save_plasma(int step);

private:

};






#endif //FILAMENTATION_LOGGER_H


