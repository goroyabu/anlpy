/**
   @file   pyfunc.cpp
   @date   2020/08/24
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/
#include "ReadTTree.hpp"
#include "WriteTTree.hpp"
#include "BeginG4TreeAnalysis.hpp"
#include "ApplyEnergyResponse.hpp"
#include "DiffuseEnergyCharge.hpp"
#include "JudgeTrigger.hpp"
#include "ConvertToLv1Data.hpp"
#include "CutReadoutThreshold.hpp"
#include "SelectHitPattern.hpp"
#include "CutAnalysisThreshold.hpp"
#include "MergeAdjacentStrip.hpp"
#include "CoupleHit.hpp"

#include <pybind11/pybind11.h>

PYBIND11_MODULE(sim4ana, m) {
    m.attr("__name__") = "sim4ana";
    m.doc() = R"pbdoc(
        sim4ana Documentation
        -----------------------
    )pbdoc";

    pybind11::class_<anl::VANL_Module>(m, "VANL_Module", pybind11::module_local());

    using anl::ReadTTree;
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

    using anl::WriteTTree;
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

    pybind11::class_<BeginG4TreeAnalysis, anl::VANL_Module>(m, "BeginG4TreeAnalysis")
	.def(pybind11::init<>())
	.def("SetParameter", &BeginG4TreeAnalysis::set_parameter<int>)
	.def("SetParameter", &BeginG4TreeAnalysis::set_parameter<double>)
	.def("SetParameter", &BeginG4TreeAnalysis::set_parameter<bool>)
	.def("SetParameter", &BeginG4TreeAnalysis::set_parameter<std::string>)
	.def("DefineParameter", &BeginG4TreeAnalysis::define_parameter<int>)
	.def("DefineParameter", &BeginG4TreeAnalysis::define_parameter<double>)
	.def("DefineParameter", &BeginG4TreeAnalysis::define_parameter<bool>)
	.def("DefineParameter", &BeginG4TreeAnalysis::define_parameter<std::string>)
	.def("ShowParameter", &BeginG4TreeAnalysis::show_parameters);
    
    pybind11::class_<ApplyEnergyResponse, anl::VANL_Module>(m, "ApplyEnergyResponse")
	.def(pybind11::init<>())
	.def("SetParameter", &ApplyEnergyResponse::set_parameter<int>)
	.def("SetParameter", &ApplyEnergyResponse::set_parameter<double>)
	.def("SetParameter", &ApplyEnergyResponse::set_parameter<bool>)
	.def("SetParameter", &ApplyEnergyResponse::set_parameter<std::string>)
	.def("DefineParameter", &ApplyEnergyResponse::define_parameter<int>)
	.def("DefineParameter", &ApplyEnergyResponse::define_parameter<double>)
	.def("DefineParameter", &ApplyEnergyResponse::define_parameter<bool>)
	.def("DefineParameter", &ApplyEnergyResponse::define_parameter<std::string>)
	.def("ShowParameter", &ApplyEnergyResponse::show_parameters);

    pybind11::class_<DiffuseEnergyCharge, anl::VANL_Module>(m, "DiffuseEnergyCharge")
	.def(pybind11::init<>())
	.def("SetParameter", &DiffuseEnergyCharge::set_parameter<int>)
	.def("SetParameter", &DiffuseEnergyCharge::set_parameter<double>)
	.def("SetParameter", &DiffuseEnergyCharge::set_parameter<bool>)
	.def("SetParameter", &DiffuseEnergyCharge::set_parameter<std::string>)
	.def("DefineParameter", &DiffuseEnergyCharge::define_parameter<int>)
	.def("DefineParameter", &DiffuseEnergyCharge::define_parameter<double>)
	.def("DefineParameter", &DiffuseEnergyCharge::define_parameter<bool>)
	.def("DefineParameter", &DiffuseEnergyCharge::define_parameter<std::string>)
	.def("ShowParameter", &DiffuseEnergyCharge::show_parameters);

    pybind11::class_<JudgeTrigger, anl::VANL_Module>(m, "JudgeTrigger")
	.def(pybind11::init<>())
	.def("SetParameter", &JudgeTrigger::set_parameter<int>)
	.def("SetParameter", &JudgeTrigger::set_parameter<double>)
	.def("SetParameter", &JudgeTrigger::set_parameter<bool>)
	.def("SetParameter", &JudgeTrigger::set_parameter<std::string>)
	.def("DefineParameter", &JudgeTrigger::define_parameter<int>)
	.def("DefineParameter", &JudgeTrigger::define_parameter<double>)
	.def("DefineParameter", &JudgeTrigger::define_parameter<bool>)
	.def("DefineParameter", &JudgeTrigger::define_parameter<std::string>)
	.def("ShowParameter", &JudgeTrigger::show_parameters);
    
    pybind11::class_<ConvertToLv1Data, anl::VANL_Module>(m, "ConvertToLv1Data")
	.def(pybind11::init<>())
	.def("SetParameter", &ConvertToLv1Data::set_parameter<int>)
	.def("SetParameter", &ConvertToLv1Data::set_parameter<double>)
	.def("SetParameter", &ConvertToLv1Data::set_parameter<bool>)
	.def("SetParameter", &ConvertToLv1Data::set_parameter<std::string>)
	.def("DefineParameter", &ConvertToLv1Data::define_parameter<int>)
	.def("DefineParameter", &ConvertToLv1Data::define_parameter<double>)
	.def("DefineParameter", &ConvertToLv1Data::define_parameter<bool>)
	.def("DefineParameter", &ConvertToLv1Data::define_parameter<std::string>)
	.def("ShowParameter", &ConvertToLv1Data::show_parameters);
    
    pybind11::class_<CutReadoutThreshold, anl::VANL_Module>(m, "CutReadoutThreshold")
	.def(pybind11::init<>())
	.def("SetParameter", &CutReadoutThreshold::set_parameter<int>)
	.def("SetParameter", &CutReadoutThreshold::set_parameter<double>)
	.def("SetParameter", &CutReadoutThreshold::set_parameter<bool>)
	.def("SetParameter", &CutReadoutThreshold::set_parameter<std::string>)
	.def("DefineParameter", &CutReadoutThreshold::define_parameter<int>)
	.def("DefineParameter", &CutReadoutThreshold::define_parameter<double>)
	.def("DefineParameter", &CutReadoutThreshold::define_parameter<bool>)
	.def("DefineParameter", &CutReadoutThreshold::define_parameter<std::string>)
	.def("ShowParameter", &CutReadoutThreshold::show_parameters);

    pybind11::class_<SelectHitPattern, anl::VANL_Module>(m, "SelectHitPattern")
	.def(pybind11::init<>())
	.def("SetParameter", &SelectHitPattern::set_parameter<int>)
	.def("SetParameter", &SelectHitPattern::set_parameter<double>)
	.def("SetParameter", &SelectHitPattern::set_parameter<bool>)
	.def("SetParameter", &SelectHitPattern::set_parameter<std::string>)
	.def("DefineParameter", &SelectHitPattern::define_parameter<int>)
	.def("DefineParameter", &SelectHitPattern::define_parameter<double>)
	.def("DefineParameter", &SelectHitPattern::define_parameter<bool>)
	.def("DefineParameter", &SelectHitPattern::define_parameter<std::string>)
	.def("ShowParameter", &SelectHitPattern::show_parameters);

    pybind11::class_<CutAnalysisThreshold, anl::VANL_Module>(m, "CutAnalysisThreshold")
	.def(pybind11::init<>())
	.def("SetParameter", &CutAnalysisThreshold::set_parameter<int>)
	.def("SetParameter", &CutAnalysisThreshold::set_parameter<double>)
	.def("SetParameter", &CutAnalysisThreshold::set_parameter<bool>)
	.def("SetParameter", &CutAnalysisThreshold::set_parameter<std::string>)
	.def("DefineParameter", &CutAnalysisThreshold::define_parameter<int>)
	.def("DefineParameter", &CutAnalysisThreshold::define_parameter<double>)
	.def("DefineParameter", &CutAnalysisThreshold::define_parameter<bool>)
	.def("DefineParameter", &CutAnalysisThreshold::define_parameter<std::string>)
	.def("ShowParameter", &CutAnalysisThreshold::show_parameters);

    pybind11::class_<MergeAdjacentStrip, anl::VANL_Module>(m, "MergeAdjacentStrip")
	.def(pybind11::init<>())
	.def("SetParameter", &MergeAdjacentStrip::set_parameter<int>)
	.def("SetParameter", &MergeAdjacentStrip::set_parameter<double>)
	.def("SetParameter", &MergeAdjacentStrip::set_parameter<bool>)
	.def("SetParameter", &MergeAdjacentStrip::set_parameter<std::string>)
	.def("DefineParameter", &MergeAdjacentStrip::define_parameter<int>)
	.def("DefineParameter", &MergeAdjacentStrip::define_parameter<double>)
	.def("DefineParameter", &MergeAdjacentStrip::define_parameter<bool>)
	.def("DefineParameter", &MergeAdjacentStrip::define_parameter<std::string>)
	.def("ShowParameter", &MergeAdjacentStrip::show_parameters);

    // pybind11::class_<CoupleHit, anl::VANL_Module>(m, "CoupleHit")
    // 	.def(pybind11::init<>())
    // 	.def("SetParameter", &CoupleHit::set_parameter<int>)
    // 	.def("SetParameter", &CoupleHit::set_parameter<double>)
    // 	.def("SetParameter", &CoupleHit::set_parameter<bool>)
    // 	.def("SetParameter", &CoupleHit::set_parameter<std::string>)
    // 	.def("DefineParameter", &CoupleHit::define_parameter<int>)
    // 	.def("DefineParameter", &CoupleHit::define_parameter<double>)
    // 	.def("DefineParameter", &CoupleHit::define_parameter<bool>)
    // 	.def("DefineParameter", &CoupleHit::define_parameter<std::string>)
    // 	.def("ShowParameter", &CoupleHit::show_parameters);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
