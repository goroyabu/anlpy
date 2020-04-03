/**
   @date 2020/03/27
**/

#include "TimeGetter.hpp"
#include "LoggerText.hpp"

// #include "VANL_Module.hpp"
// #include "ANLStatus.hpp"
// #include "ANL.hpp"

#include <pybind11/pybind11.h>

PYBIND11_MODULE(anltest, m) {
    m.attr("__name__") = "anltest";
    m.doc() = R"pbdoc(
        ANLtest Documentation
        -----------------------
    )pbdoc";

    pybind11::class_<anl::VANL_Module>(m, "VANL_Module", pybind11::module_local());
    
    pybind11::class_<TimeGetter, anl::VANL_Module>(m, "TimeGetter")
	.def(pybind11::init<>())
	.def("SetParameter", &TimeGetter::set_parameter<int>)
	.def("SetParameter", &TimeGetter::set_parameter<double>)
	.def("SetParameter", &TimeGetter::set_parameter<bool>)
	.def("SetParameter", &TimeGetter::set_parameter<std::string>)
	.def("DefineParameter", &TimeGetter::define_parameter<int>)
	.def("DefineParameter", &TimeGetter::define_parameter<double>)
	.def("DefineParameter", &TimeGetter::define_parameter<bool>)
	.def("DefineParameter", &TimeGetter::define_parameter<std::string>)
	.def("ShowParameter", &TimeGetter::show_parameters);
    
    pybind11::class_<LoggerText, anl::VANL_Module>(m, "LoggerText")
	.def(pybind11::init<>())
	.def("SetParameter", &LoggerText::set_parameter<int>)
	.def("SetParameter", &LoggerText::set_parameter<double>)
	.def("SetParameter", &LoggerText::set_parameter<bool>)
	.def("SetParameter", &LoggerText::set_parameter<std::string>)
	.def("DefineParameter", &LoggerText::define_parameter<int>)
	.def("DefineParameter", &LoggerText::define_parameter<double>)
	.def("DefineParameter", &LoggerText::define_parameter<bool>)
	.def("DefineParameter", &LoggerText::define_parameter<std::string>)
	.def("ShowParameter", &LoggerText::show_parameters);
 
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
