/**
   @date 2020/03/27
**/

#include "VANL_Module.hpp"
#include "ANLmanager.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

PYBIND11_MODULE(anlpy, m) {
    
    m.attr("__name__") = "anlpy";
    m.doc() = R"pbdoc(
        ANLpy Documentation
        -----------------------

        .. currentmodule:: anlpy

        .. autosummary::
           :toctree: _generate

    )pbdoc";
    
    pybind11::class_<anl::VANL_Module>(m, "VANL_Module")
	.def(pybind11::init<>())
	.def(pybind11::init<const std::string, const std::string>())
	.def(pybind11::init<const anl::VANL_Module&>())
	.def("SetParameter", &anl::VANL_Module::set_parameter<int>)
	.def("SetParameter", &anl::VANL_Module::set_parameter<double>)
	.def("SetParameter", &anl::VANL_Module::set_parameter<bool>)
	.def("SetParameter", &anl::VANL_Module::set_parameter<std::string>)
	.def("DefineParameter", &anl::VANL_Module::define_parameter<int>)
	.def("DefineParameter", &anl::VANL_Module::define_parameter<double>)
	.def("DefineParameter", &anl::VANL_Module::define_parameter<bool>)
	.def("DefineParameter", &anl::VANL_Module::define_parameter<std::string>)
	.def("ShowParameter", &anl::VANL_Module::show_parameters);

    pybind11::class_<anl::ANLmanager, anl::VANL_Module>(m, "ANLmanager")
	.def(pybind11::init<>())
	.def(pybind11::init<const anl::ANLmanager&>())
	.def("AddModule", &anl::ANLmanager::add_module)
	.def("ReadData", &anl::ANLmanager::read_data)
	.def("ShowAnalysis", &anl::ANLmanager::show_analysis)	
	.def("ShowStatus", &anl::ANLmanager::show_status)
	.def("Quit", &anl::ANLmanager::quit)
	.def("ResetStatus", &anl::ANLmanager::reset_status);
 
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
