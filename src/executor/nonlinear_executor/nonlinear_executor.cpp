//
// Created by vasilyev on 27.07.2019.
//

#include "nonlinear_executor.h"

#define base BaseExecutor
#define pb BaseExecutor::pulsed_beam
#define dt BaseExecutor::pulsed_beam->dt


NonlinearExecutor::NonlinearExecutor() = default;


NonlinearExecutor::NonlinearExecutor(std::shared_ptr<BasePulsedBeam>& _pulsed_beam,
                                     ConfigManager& _config_manager,
                                     std::shared_ptr<Logger>& _logger,
                                     double& _dz)
: BaseExecutor(_pulsed_beam, _logger)
, config_manager(_config_manager) {

    dz = &_dz;

    logger->add_propagation_event(std::string("....creating nonlinear executor"));

    kerr = std::make_shared<Kerr>(base::pulsed_beam, config_manager.kerr_info, config_manager.T.at("kerr"));
    plasma = std::make_shared<Plasma>(base::pulsed_beam, config_manager.T.at("plasma"));
    bremsstrahlung = std::make_shared<Bremsstrahlung>(base::pulsed_beam, config_manager.T.at("bremsstrahlung"));
    dissipation = std::make_shared<Dissipation>(base::pulsed_beam);

    kinetic_equation = std::make_shared<KineticEquation>(
            base::pulsed_beam,
            base::pulsed_beam->medium->v_i_const,
            base::pulsed_beam->medium->beta);


    terms_pool.insert(std::pair<std::string, std::shared_ptr<BaseNonlinearTerm>>(kerr->name, kerr));
    terms_pool.insert(std::pair<std::string, std::shared_ptr<BaseNonlinearTerm>>(plasma->name, plasma));
    terms_pool.insert(std::pair<std::string, std::shared_ptr<BaseNonlinearTerm>>(bremsstrahlung->name, bremsstrahlung));
    terms_pool.insert(std::pair<std::string, std::shared_ptr<BaseNonlinearTerm>>(dissipation->name, dissipation));

}


NonlinearExecutor::~NonlinearExecutor() = default;


void NonlinearExecutor::update_dz(double min_phase_increment, double& dz) {
    double d = 0.1;

    if (min_phase_increment < config_manager.phase_increment_min) {
        dz *= (1.0 - d) * std::abs(config_manager.phase_increment_min) / std::abs(min_phase_increment);
    }

    if (min_phase_increment > config_manager.phase_increment_max) {
        dz *= (1.0 + d);
    }
}

void NonlinearExecutor::execute(double& dz) {

    if (!config_manager.active_nonlinear_terms.empty()) {
        auto t_start = TIME::now();

        double min_phase_increment = 0.0;


#pragma omp parallel shared(min_phase_increment)
        {

        double min_phase_increment_thread = 0.0;

#pragma omp for
            for(size_t k = 0; k < pb->n_r; ++k) {
                for(size_t s = 1; s < pb->n_t - 1; ++s) {

                    // intensity
                    double I = norm(pb->field[k][s]) * pb->I_0;
                    double I_prev = norm(pb->field[k][s - 1]) * pb->I_0;
                    double dI = I - I_prev;

                    // plasma
                    double Ne = pb->plasma[k][s];
                    double Ne_prev = pb->plasma[k][s - 1];
                    double dNe = Ne - Ne_prev;

                    double R = 0.0;

                    if (config_manager.with_plasma) {
                        // ionization rate
                        R = pb->medium->ionization->R(I);

                        // dissipation
                        double Ne_increase_field = kinetic_equation->calculate_plasma_increase_field(Ne, R);
                        dissipation->update_R_dissipation(I, Ne_increase_field);
                    }

                    std::complex<double> increment = std::complex<double>(0.0, 0.0);
                    for(auto& nonlinear_term_name : config_manager.active_nonlinear_terms) {

                        increment +=       terms_pool[nonlinear_term_name]->R_kerr_instant * I
                                         + terms_pool[nonlinear_term_name]->R_kerr_instant_T * dI / dt
                                         + terms_pool[nonlinear_term_name]->R_plasma * Ne
                                         + terms_pool[nonlinear_term_name]->R_plasma_T * dNe / dt
                                         + terms_pool[nonlinear_term_name]->R_bremsstrahlung * Ne
                                         + terms_pool[nonlinear_term_name]->R_bremsstrahlung_T * dNe / dt
                                         + terms_pool[nonlinear_term_name]->R_dissipation;
                    }

                    pb->field[k][s] *= exp(increment * dz);


                    if (config_manager.with_plasma) {
                        // plasma increase
                        double Ne_increase_full = kinetic_equation->calculate_plasma_increase_full(I, Ne, R);
                        pb->plasma[k][s + 1] = Ne + Ne_increase_full;
                    }

                    min_phase_increment_thread = std::min(increment.imag() * dz, min_phase_increment_thread);
                    //std::cout << "max_phase_increment_thread = " << max_phase_increment_thread << std::endl;
                }
            }

#pragma omp critical
        {
            min_phase_increment = std::min(min_phase_increment_thread, min_phase_increment);
        };


        };

        auto t_end = TIME::now();
        logger->term_times["nonlinearities"] += logger->duration(t_start, t_end);

    update_dz(min_phase_increment, dz);

    }
}

