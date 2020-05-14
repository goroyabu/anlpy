/**
   @file   pyfunc.cpp
   @date   2020/04/04
   @author goroyabu
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#include "ReadTTree.hpp"
#include "WriteTTree.hpp"
#include "DSDdatabase.hpp"
#include "DSDdatabaseSpline.hpp"
#include "ApplyDatabase.hpp"
#include "MergeAdjacent.hpp"
#include "CoupleHit.hpp"

#include <pybind11/pybind11.h>

PYBIND11_MODULE(dsdana, m) {
    m.attr("__name__") = "dsdana";
    m.doc() = R"pbdoc(
        dsdana Documentation
        -----------------------
    )pbdoc";

    pybind11::class_<anl::VANL_Module>(m, "VANL_Module", pybind11::module_local());

    pybind11::class_<ReadTTree, anl::VANL_Module>(m, "ReadTTree")
	.def(pybind11::init<>())
	.def("SetParameter", &ReadTTree::set_parameter<int>)
	.def("SetParameter", &ReadTTree::set_parameter<double>)
	.def("SetParameter", &ReadTTree::set_parameter<bool>)
	.def("SetParameter", &ReadTTree::set_parameter<std::string>)
	.def("DefineParameter", &ReadTTree::define_parameter<int>)
	.def("DefineParameter", &ReadTTree::define_parameter<double>)
	.def("DefineParameter", &ReadTTree::define_parameter<bool>)
	.def("DefineParameter", &ReadTTree::define_parameter<std::string>)
	.def("ShowParameter", &ReadTTree::show_parameters);

    pybind11::class_<WriteTTree, anl::VANL_Module>(m, "WriteTTree")
	.def(pybind11::init<>())
	.def("SetParameter", &WriteTTree::set_parameter<int>)
	.def("SetParameter", &WriteTTree::set_parameter<double>)
	.def("SetParameter", &WriteTTree::set_parameter<bool>)
	.def("SetParameter", &WriteTTree::set_parameter<std::string>)
	.def("DefineParameter", &WriteTTree::define_parameter<int>)
	.def("DefineParameter", &WriteTTree::define_parameter<double>)
	.def("DefineParameter", &WriteTTree::define_parameter<bool>)
	.def("DefineParameter", &WriteTTree::define_parameter<std::string>)
	.def("ShowParameter", &WriteTTree::show_parameters);

    pybind11::class_<DSDdatabase, anl::VANL_Module>(m, "DSDdatabase")
	.def(pybind11::init<>())
	.def("SetParameter", &DSDdatabase::set_parameter<int>)
	.def("SetParameter", &DSDdatabase::set_parameter<double>)
	.def("SetParameter", &DSDdatabase::set_parameter<bool>)
	.def("SetParameter", &DSDdatabase::set_parameter<std::string>)
	.def("DefineParameter", &DSDdatabase::define_parameter<int>)
	.def("DefineParameter", &DSDdatabase::define_parameter<double>)
	.def("DefineParameter", &DSDdatabase::define_parameter<bool>)
	.def("DefineParameter", &DSDdatabase::define_parameter<std::string>)
	.def("ShowParameter", &DSDdatabase::show_parameters);

    pybind11::class_<DSDdatabaseSpline, DSDdatabase>(m, "DSDdatabaseSpline")
	.def(pybind11::init<>())
	.def("SetParameter", &DSDdatabaseSpline::set_parameter<int>)
	.def("SetParameter", &DSDdatabaseSpline::set_parameter<double>)
	.def("SetParameter", &DSDdatabaseSpline::set_parameter<bool>)
	.def("SetParameter", &DSDdatabaseSpline::set_parameter<std::string>)
	.def("DefineParameter", &DSDdatabaseSpline::define_parameter<int>)
	.def("DefineParameter", &DSDdatabaseSpline::define_parameter<double>)
	.def("DefineParameter", &DSDdatabaseSpline::define_parameter<bool>)
	.def("DefineParameter", &DSDdatabaseSpline::define_parameter<std::string>)
	.def("ShowParameter", &DSDdatabaseSpline::show_parameters);

    pybind11::class_<ApplyDatabase, anl::VANL_Module>(m, "ApplyDatabase")
	.def(pybind11::init<>())
	.def("SetParameter", &ApplyDatabase::set_parameter<int>)
	.def("SetParameter", &ApplyDatabase::set_parameter<double>)
	.def("SetParameter", &ApplyDatabase::set_parameter<bool>)
	.def("SetParameter", &ApplyDatabase::set_parameter<std::string>)
	.def("DefineParameter", &ApplyDatabase::define_parameter<int>)
	.def("DefineParameter", &ApplyDatabase::define_parameter<double>)
	.def("DefineParameter", &ApplyDatabase::define_parameter<bool>)
	.def("DefineParameter", &ApplyDatabase::define_parameter<std::string>)
	.def("ShowParameter", &ApplyDatabase::show_parameters);

    pybind11::class_<MergeAdjacent, anl::VANL_Module>(m, "MergeAdjacent")
	.def(pybind11::init<>())
	.def("SetParameter", &MergeAdjacent::set_parameter<int>)
	.def("SetParameter", &MergeAdjacent::set_parameter<double>)
	.def("SetParameter", &MergeAdjacent::set_parameter<bool>)
	.def("SetParameter", &MergeAdjacent::set_parameter<std::string>)
	.def("DefineParameter", &MergeAdjacent::define_parameter<int>)
	.def("DefineParameter", &MergeAdjacent::define_parameter<double>)
	.def("DefineParameter", &MergeAdjacent::define_parameter<bool>)
	.def("DefineParameter", &MergeAdjacent::define_parameter<std::string>)
	.def("ShowParameter", &MergeAdjacent::show_parameters);

    pybind11::class_<CoupleHit, anl::VANL_Module>(m, "CoupleHit")
	.def(pybind11::init<>())
	.def("SetParameter", &CoupleHit::set_parameter<int>)
	.def("SetParameter", &CoupleHit::set_parameter<double>)
	.def("SetParameter", &CoupleHit::set_parameter<bool>)
	.def("SetParameter", &CoupleHit::set_parameter<std::string>)
	.def("DefineParameter", &CoupleHit::define_parameter<int>)
	.def("DefineParameter", &CoupleHit::define_parameter<double>)
	.def("DefineParameter", &CoupleHit::define_parameter<bool>)
	.def("DefineParameter", &CoupleHit::define_parameter<std::string>)
	.def("ShowParameter", &CoupleHit::show_parameters);

    
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
