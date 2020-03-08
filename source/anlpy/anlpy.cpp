#include "ANLCross.hpp"
#include "ANLModuleBase.hpp"
#include "ANLStatus.hpp"

#include <pybind11/pybind11.h>

int add(int i, int j) {
    return i + j;
}

namespace py = pybind11;

PYBIND11_MODULE(anlpy, m) {
    m.attr("__name__") = "anlpy";
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: anlpy

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

    m.def("add_module", &anlcross::add_module, R"pbdoc(
        Add modules
    )pbdoc");
    
    m.def("anl_init", &anlcross::anl_init, R"pbdoc(
        Initialize ANL
    )pbdoc");

    m.def("anl_body", &anlcross::anl_body, R"pbdoc(
        Run ANL
    )pbdoc");

    pybind11::class_<anlcross::ANLModuleBase>(m, "ANLModuleBase")
        .def(pybind11::init<>())
        .def(pybind11::init<const std::string, const std::string>());

    

    
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
