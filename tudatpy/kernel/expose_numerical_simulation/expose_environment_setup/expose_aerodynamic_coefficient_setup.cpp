/*    Copyright (c) 2010-2019, Delft University of Technology
 *    All rights reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#include "expose_aerodynamic_coefficient_setup.h"
#include <tudat/basics/deprecationWarnings.h>

#include "tudatpy/docstrings.h"
#include <tudat/simulation/environment_setup.h>
#include <tudat/astro/reference_frames/referenceFrameTransformations.h>

//#include <pybind11/chrono.h>
#include <pybind11/eigen.h>
#include <pybind11/functional.h>
//#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>

namespace py = pybind11;
namespace tss = tudat::simulation_setup;
namespace ti = tudat::interpolators;
namespace ta = tudat::aerodynamics;

namespace tudat
{
namespace simulation_setup
{
//! @get_docstring(customAerodynamicCoefficientSettings)
inline std::shared_ptr< AerodynamicCoefficientSettings > customAerodynamicCoefficientSettingsDeprecated(
        const std::function< Eigen::Vector3d( const std::vector< double >& ) > forceCoefficientFunction,
        const double referenceArea,
        const std::vector< aerodynamics::AerodynamicCoefficientsIndependentVariables >
        independentVariableNames,
        const bool areCoefficientsInAerodynamicFrame = true,
        const bool areCoefficientsInNegativeAxisDirection = true )
{
    static bool isWarningPrinted = false;
    if( isWarningPrinted == false )
    {
        tudat::utilities::printDeprecationWarning( "tudatpy.numerical_simulation.environment_setup.aerodynamic_coefficients.custom",
                             "tudatpy.numerical_simulation.environment_setup.aerodynamic_coefficients.custom_aerodynamic_force_coefficients");
        isWarningPrinted = true;
    }

    return customAerodynamicCoefficientSettingsDeprecated(
                forceCoefficientFunction, referenceArea, independentVariableNames,
                areCoefficientsInAerodynamicFrame,
                areCoefficientsInNegativeAxisDirection );

}
}
}

namespace tudatpy {
namespace numerical_simulation {
namespace environment_setup {
namespace aerodynamic_coefficients {

    void expose_aerodynamic_coefficient_setup(py::module &m) {

        /////////////////////////////////////////////////////////////////////////////
        // createAerodynamicCoefficientInterface.h
        /////////////////////////////////////////////////////////////////////////////
        py::class_<tss::AerodynamicCoefficientSettings,
                std::shared_ptr<tss::AerodynamicCoefficientSettings>>(
                    m, "AerodynamicCoefficientSettings",
                                                get_docstring("AerodynamicCoefficientSettings").c_str());

        py::class_<tss::ConstantAerodynamicCoefficientSettings,
                std::shared_ptr<tss::ConstantAerodynamicCoefficientSettings>,
                tss::AerodynamicCoefficientSettings>(
                        m, "ConstantAerodynamicCoefficientSettings",
                        get_docstring("ConstantAerodynamicCoefficientSettings").c_str());
//            .def(py::init<const double, const double, const double,
//                 const Eigen::Vector3d &, const Eigen::Vector3d &,
//                 const Eigen::Vector3d &, const bool, const bool,
//                 const std::shared_ptr<ti::InterpolatorSettings>>(),
//                 py::arg("reference_length"), py::arg("reference_area"),
//                 py::arg("lateral_reference_length"),
//                 py::arg("moment_reference_point"),
//                 py::arg("constant_force_coefficient"),
//                 py::arg("constant_moment_coefficient") = Eigen::Vector3d::Zero(),
//                 py::arg("are_coefficients_in_aerodynamic_frame") = true,
//                 py::arg("are_coefficients_in_negative_axis_direction") = true,
//                 py::arg("interpolator_settings") = nullptr)
//            .def(py::init<const double, const Eigen::Vector3d &, const bool,
//                 const bool>(),
//                 py::arg("reference_area"),
//                 py::arg("constant_force_coefficient"),
//                 py::arg("are_coefficients_in_aerodynamic_frame") = true,
//                 py::arg("are_coefficients_in_negative_axis_direction") = true);

        m.def("constant",
              py::overload_cast<const double, const Eigen::Vector3d &, const bool,
                      const bool>(
                      &tss::constantAerodynamicCoefficientSettings),
              py::arg("reference_area"),
              py::arg("constant_force_coefficient"),
              py::arg("are_coefficients_in_aerodynamic_frame") = true,
              py::arg("are_coefficients_in_negative_axis_direction") = true,
              get_docstring("constant").c_str());

        m.def("custom_aerodynamic_force_coefficients",
              py::overload_cast<
                      const std::function<Eigen::Vector3d(const std::vector<double> &)>,
                      const double, const std::vector<ta::AerodynamicCoefficientsIndependentVariables>,
                      const bool, const bool>(&tss::customAerodynamicCoefficientSettings),
              py::arg("force_coefficient_function"),
              py::arg("reference_area"),
              py::arg("independent_variable_names"),
              py::arg("are_coefficients_in_aerodynamic_frame") = true,
              py::arg("are_coefficients_in_negative_axis_direction") = true,
              get_docstring("custom_aerodynamic_force_coefficients").c_str());

        m.def("custom_aerodynamic_force_and_moment_coefficients",
              py::overload_cast<
              const std::function< Eigen::Vector3d( const std::vector< double >& ) >,
              const std::function< Eigen::Vector3d( const std::vector< double >& ) >,
              const double,
              const double,
              const Eigen::Vector3d&,
              const std::vector< ta::AerodynamicCoefficientsIndependentVariables >,
              const bool,
              const bool >(&tss::customAerodynamicCoefficientSettings),
              py::arg("force_coefficient_function"),
              py::arg("moment_coefficient_function"),
              py::arg("reference_length"),
              py::arg("reference_area"),
              py::arg("moment_reference_point"),
              py::arg("independent_variable_names"),
              py::arg("are_coefficients_in_aerodynamic_frame") = true,
              py::arg("are_coefficients_in_negative_axis_direction") = true,
              get_docstring("custom_aerodynamic_force_and_moment_coefficients").c_str());

        m.def("custom",
              &tss::customAerodynamicCoefficientSettingsDeprecated,
              py::arg("force_coefficient_function"),
              py::arg("reference_area"),
              py::arg("independent_variable_names"),
              py::arg("are_coefficients_in_aerodynamic_frame") = true,
              py::arg("are_coefficients_in_negative_axis_direction") = true );

        m.def("tabulated",
              py::overload_cast<
                      const std::vector<double>,
                      const std::vector<Eigen::Vector3d>,
                      const std::vector<Eigen::Vector3d>,
                      const double,
                      const double,
                      const double,
                      const Eigen::Vector3d &,
                      const ta::AerodynamicCoefficientsIndependentVariables,
                      const bool,
                      const bool,
                      const std::shared_ptr<ti::InterpolatorSettings>>
                      (&tss::oneDimensionalTabulatedAerodynamicCoefficientSettings),
              py::arg("independent_variables"),
              py::arg("force_coefficients"),
              py::arg("moment_coefficients"),
              py::arg("reference_length"),
              py::arg("reference_area"),
              py::arg("lateral_reference_length"),
              py::arg("moment_reference_point"),
              py::arg("independent_variable_name"),
              py::arg("are_coefficients_in_aerodynamic_frame") = true,
              py::arg("are_coefficients_in_negative_axis_direction") = true,
              py::arg("interpolator_settings") = nullptr,
              get_docstring("tabulated").c_str());

        m.def("tabulated_force_only",
              py::overload_cast<
                      const std::vector<double>,
                      const std::vector<Eigen::Vector3d>,
                      const double,
                      const ta::AerodynamicCoefficientsIndependentVariables,
                      const bool,
                      const bool,
                      const std::shared_ptr<ti::InterpolatorSettings>>
                      (&tss::oneDimensionalTabulatedAerodynamicCoefficientSettings),
              py::arg("independent_variables"),
              py::arg("force_coefficients"),
              py::arg("reference_area"),
              py::arg("independent_variable_name"),
              py::arg("are_coefficients_in_aerodynamic_frame") = true,
              py::arg("are_coefficients_in_negative_axis_direction") = true,
              py::arg("interpolator_settings") = nullptr,
              get_docstring("tabulated_force_only").c_str());

        m.def("tabulated_force_only_from_files",
              py::overload_cast<
              const std::map< int, std::string >,
              const double,
              const std::vector< ta::AerodynamicCoefficientsIndependentVariables >,
              const bool,
              const bool,
              const std::shared_ptr< ti::InterpolatorSettings > >
                      (&tss::readTabulatedAerodynamicCoefficientsFromFiles),
              py::arg("force_coefficient_files"),
              py::arg("reference_area"),
              py::arg("independent_variable_names"),
              py::arg("are_coefficients_in_aerodynamic_frame") = true,
              py::arg("are_coefficients_in_negative_axis_direction") = true,
              py::arg("interpolator_settings") = nullptr,
              get_docstring("tabulated_force_only_from_files").c_str());

        m.def("tabulated_from_files",
              py::overload_cast<
              const std::map< int, std::string >,
              const std::map< int, std::string >,
              const double,
              const double,
              const double,
              const Eigen::Vector3d &,
              const std::vector< ta::AerodynamicCoefficientsIndependentVariables >,
              const bool,
              const bool,
              const std::shared_ptr< ti::InterpolatorSettings > >
                      (&tss::readTabulatedAerodynamicCoefficientsFromFiles),
              py::arg("force_coefficient_files"),
              py::arg("moment_coefficient_files"),
              py::arg("reference_length"),
              py::arg("reference_area"),
              py::arg("lateral_reference_length"),
              py::arg("moment_reference_point"),
              py::arg("independent_variable_names"),
              py::arg("are_coefficients_in_aerodynamic_frame") = true,
              py::arg("are_coefficients_in_negative_axis_direction") = true,
              py::arg("interpolator_settings") = nullptr,
              get_docstring("tabulated_from_files").c_str());

        m.def("scaled_by_constant",
              py::overload_cast<
                      const std::shared_ptr<tss::AerodynamicCoefficientSettings>,
                      const double, const double, const bool>
                      (&tss::scaledAerodynamicCoefficientSettings),
              py::arg("unscaled_coefficient_settings"),
              py::arg("force_scaling_constant"),
              py::arg("moment_scaling_constant"),
              py::arg("is_scaling_absolute") = false,
              get_docstring("scaled_by_constant").c_str());

        m.def("scaled_by_vector",
              py::overload_cast<
                      const std::shared_ptr<tss::AerodynamicCoefficientSettings>,
                      const Eigen::Vector3d, const Eigen::Vector3d, const bool>
                      (&tss::scaledAerodynamicCoefficientSettings),
              py::arg("unscaled_coefficient_settings"),
              py::arg("force_scaling_vector"),
              py::arg("moment_scaling_vector"),
              py::arg("is_scaling_absolute") = false,
              get_docstring("scaled_by_vector").c_str());

        m.def("scaled_by_vector_function",
              py::overload_cast<
                      const std::shared_ptr<tss::AerodynamicCoefficientSettings>,
                      const std::function<Eigen::Vector3d(const double)>,
                      const std::function<Eigen::Vector3d(const double)>, const bool>
                      (&tss::scaledAerodynamicCoefficientSettings),
              py::arg("unscaled_coefficient_settings"),
              py::arg("force_scaling_vector_function"),
              py::arg("moment_scaling_vector_function"),
              py::arg("is_scaling_absolute") = false,
              get_docstring("scaled_by_vector_function").c_str());

    }

}// namespace aerodynamic_coefficients
}// namespace environment_setup
}// namespace numerical_simulation
}// namespace tudatpy
