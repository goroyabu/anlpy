/***
    @file   ProjectConeETCC.cpp
    @date
    @author User
    @detail Automatically generated by mkanlproject
***/

#include "ProjectConeETCC.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <bnk.hpp>
#include <evs.hpp>

#include <TROOT.h>
#include <TSeqCollection.h>
#include <TCanvas.h>

ProjectConeETCC::ProjectConeETCC()
    : anl::VANL_Module("ProjectConeETCC", "20210927"),
    input_file(nullptr),
    input_tree(nullptr),
    output_file(nullptr),
    output_tree(nullptr),
    image(nullptr),
    image_etcc(nullptr),
    h1_cone_filling_ratio(nullptr),
    etrack_calc_dedx(nullptr)
{
    define_parameter<std::string>("input_file", "input.root");
    define_parameter<std::string>("input_tree", "hittree");
    define_parameter<std::string>("output_file", "output.root");
    define_parameter<std::string>("output_tree", "resptree");
    define_parameter<double>("e_window_begin", 100.0);
    define_parameter<double>("e_window_end", 300.0);
    define_parameter<double>("cone_thick_deg", 0.5);
    define_parameter<double>("arc_length_deg", 10.0);
    define_parameter<double>("distance_index", 2.0);
    define_parameter<double>("e_threshold_si", 5.0);
    define_parameter<double>("e_threshold_cdte", 5.0);

    define_parameter<double>("source_posx", 0.0);
    define_parameter<double>("source_posy", 0.0);
    define_parameter<double>("source_posz", 0.0);

    define_parameter<int>("nbins_1axis", 100);
    define_parameter<double>("axis_minimum", -25.0);
    define_parameter<double>("axis_maximum", 25.0);

    define_parameter<int>("xaxis_nbins", 70);
    define_parameter<double>("xaxis_minimum", -35.0);
    define_parameter<double>("xaxis_maximum", 35.0);

    define_parameter<int>("yaxis_nbins", 70);
    define_parameter<double>("yaxis_minimum", -35.0);
    define_parameter<double>("yaxis_maximum", 35.0);

    define_parameter<int>("zaxis_nbins", 40);
    define_parameter<double>("zaxis_minimum", -10.0);
    define_parameter<double>("zaxis_maximum", 10.0);

    define_parameter<double>("theta_max_degree", 150.0);
    define_parameter<double>("detector_z_position", -41.0);
    define_parameter<int>("event_list_only", 0);
    define_parameter<double>("rotation_around_vertical_deg", 0.0);
    define_parameter<int>("enable_reject_fluor", 1);
    define_parameter<int>("enable_normalize_cone", 1);

    define_parameter<double>("eigen_ratio_threshold", 10.0);
    define_parameter<double>("pixel_ratio_threshold", 2.0);
}
ProjectConeETCC::~ProjectConeETCC()
{
}

int ProjectConeETCC::mod_bgnrun()
{
    cout << " --- ProjectConeETCC::mod_bgnrun" << endl;
    cout << endl;

    auto ifname = get_parameter<std::string>("input_file");
    this->input_file = new TFile( ifname.c_str() );
    if ( !this->input_file || this->input_file->IsZombie() ) {
        cout << "Opening " << ifname << " is failed." << endl;
        return anl::ANL_NG;
    }
    cout << ifname << " is opened." << endl;

    auto itname = get_parameter<std::string>("input_tree");
    this->input_tree = (TTree*)this->input_file->Get( itname.c_str() );
    if ( !this->input_tree ) {
        cout << itname << " is not found." << endl;
        return anl::ANL_NG;
    }
    cout << itname << " is loaded." << endl;

    this->event.SetBranchAddress(this->input_tree);
    evs::put("ANL_totalevents", this->event.GetEntries());

    auto ofname = get_parameter<std::string>("output_file");

    TSeqCollection * list = gROOT->GetListOfFiles();
    auto opened = list->FindObject( ofname.c_str() );

    if (
        !!opened
        && opened->ClassName()==(TString)"TFile"
        && ((TFile*)opened)->IsWritable()
    ) {
        this->output_file = (TFile*)opened;
        this->output_file->cd();
        cout << ofname << " is already opened." << endl;
    }
    else {
        this->output_file = new TFile( ofname.c_str(), "recreate" );
        if ( !this->output_file || this->output_file->IsZombie() ) {
            cout << "Creating " << ofname << " is failed." << endl;
            return anl::ANL_NG;
        }
        cout << ofname << " is created." << endl;
    }

    auto otname = get_parameter<std::string>("output_tree");
    // this->output_tree = new TTree( otname.c_str(), otname.c_str() );
    this->output_tree = this->input_tree->CloneTree(0);
    this->output_tree->SetNameTitle( otname.c_str(), otname.c_str() );

    auto copyid = get_parameter<std::string>("copyid");
    this->is_event_list_only = get_parameter<int>("event_list_only");

    auto x_nbins = get_parameter<int>("xaxis_nbins");
    auto x_min   = get_parameter<double>("xaxis_minimum");
    auto x_max   = get_parameter<double>("xaxis_maximum");

    auto y_nbins = get_parameter<int>("yaxis_nbins");
    auto y_min   = get_parameter<double>("yaxis_minimum");
    auto y_max   = get_parameter<double>("yaxis_maximum");

    auto z_nbins = get_parameter<int>("zaxis_nbins");
    auto z_min   = get_parameter<double>("zaxis_minimum");
    auto z_max   = get_parameter<double>("zaxis_maximum");

    if ( !is_event_list_only ) {
        this->image = new TH3F(
            (TString)"response"+copyid.c_str(),
            "response;X(mm);Y(mm);Z(mm)",
            x_nbins, x_min, x_max,
            y_nbins, y_min, y_max,
            z_nbins, z_min, z_max );

        this->image_etcc = (TH3F*)this->image->Clone();
        this->image_etcc->SetNameTitle(
            (TString)"response_etcc"+copyid.c_str(),
            "response_etcc;X(mm);Y(mm);Z(mm)"
        );

        this->h1_cone_filling_ratio = new TH1D(
            (TString)"cone_filling_ratio_"+copyid.c_str(),
            "cone_filling_ratio;Z(mm)", z_nbins, z_min, z_max );
    }

    const static double pixel_pitch = 0.02; //mm
    this->etrack_calc_dedx = new TH2D( "track_image", "etrack",
        110, -pixel_pitch*(10-0.5), pixel_pitch*(100-0.5),
        110, -pixel_pitch*(10-0.5), pixel_pitch*(100-0.5)
        );

    auto source_posx = this->get_parameter<double>("source_posx");
    auto source_posy = this->get_parameter<double>("source_posy");
    auto source_posz = this->get_parameter<double>("source_posz");
    this->source_position = TVector3( source_posx, source_posy, source_posz );

    this->h1_arm_distribution = new TH1D( "arm_dist", "ARM;(deg)",
        360, -180.5, 179.5
    );
    this->h1_spd_distribution = new TH1D( "spd_dist", "SPD;(deg)",
        360, -180.5, 179.5
    );
    this->h2_arm_vs_spd = new TH2D("arm_vs_spd",
        Form( "Src(X,Y,Z)=(%4.1f,%4.1f,%4.1f);ARM;SPD", source_posx, source_posy, source_posz ),
        360, -180.5, 179.5,
        360, -180.5, 179.5
    );
    this->h2_arm_distribition = new TH2D( "arm_dist_2d", ";Kine;Geom",
        360, -180.5, 179.5,
        360, -180.5, 179.5
    );
    this->h2_spd_distribition = new TH2D( "spd_dist_2d", ";Esti;Geom;",
        360, -180.5, 179.5,
        360, -180.5, 179.5
    );
    this->h2_ene_cmos_vs_cdte = new TH2D( "ene_cmos_vs_cdte", ";Si;CdTe",
        250, -2, 998,
        250, -2, 998
    );
    this->h1_ene_sum_cmos_cdte = new TH1D( "ene_sum_cmos_cdte", "Si+CdTe",
        1000, -0.5, 999.5
    );

    if ( DefineBranch( this->output_tree )!=anl::ANL_OK ) return anl::ANL_NG;

    evs::define("Si-CdTe 2hits event");
    evs::define("E-consistent Si-CMOS");
    evs::define("SC-2hits in E-window");
    evs::define("SC-2hits in Theta-range");
    evs::define("Direction_is_Reconstruted");
    evs::define("Eigen_Value_Is_Over_Threshold");
    evs::define("Pixel_Ratio_Is_Over_Threshold");

    cout << endl;

    e_window_begin = get_parameter<double>("e_window_begin");
    e_window_end   = get_parameter<double>("e_window_end");
    cout << " - Energy-window : " << e_window_begin << ", " << e_window_end << "keV" << endl;

    auto cone_thick_deg = get_parameter<double>("cone_thick_deg");
    cone_thick_rad = cone_thick_deg*TMath::Pi()/180.0;
    tangent_cone_thick = TMath::Tan( cone_thick_rad/2.35 );
    cout << " - Thickness of Cone : " << cone_thick_deg << "(deg)" << endl;

    auto arc_length_deg = get_parameter<double>("arc_length_deg");
    this->arc_length_rad = arc_length_deg*TMath::Pi()/180.0;
    this->arc_length_sigma = this->arc_length_rad/2.35;
    cout << " - Length of Arc : " << arc_length_deg << "(deg), ";
    cout << this->arc_length_sigma << "(rad)" << endl;

    distance_index_omega = -1.0*get_parameter<double>("distance_index");
    cout << " - Distance-factor Omega : " << distance_index_omega << endl;

    e_threshold_si = get_parameter<double>("e_threshold_si");
    e_threshold_cdte = get_parameter<double>("e_threshold_cdte");
    cout << " - Threshold for Si-DSD   : " << e_threshold_si << "keV" << endl;
    cout << " - Threshold for CdTe-DSD : " << e_threshold_cdte << "keV" << endl;

    this->theta_max_degree = get_parameter<double>("theta_max_degree");
    cout << " - Maximum scattering angle : " << this->theta_max_degree << "(deg)" << endl;

    this->detector_z_position = get_parameter<double>("detector_z_position");
    cout << " - Detector-Z : " << this->detector_z_position << "mm" << endl;

    auto deg = get_parameter<double>("rotation_around_vertical_deg");
    cout << " - Rotation around Y-axis : " << deg << "(deg)" << endl;

    if ( deg!=0.0 )
        this->rotation_around_vertical_rad = deg/180.0*TMath::Pi();
    else
        this->rotation_around_vertical_rad = 0.0;
    // cout << "Rotatation Angle around Y-axis is set to " << rotation_around_vertical_rad << " [rad]" << endl;

    this->enable_reject_fluor = get_parameter<int>("enable_reject_fluor");
    this->enable_normalize_cone = get_parameter<int>("enable_normalize_cone");

    this->eigen_ratio_threshold = get_parameter<double>("eigen_ratio_threshold");
    this->pixel_ratio_threshold = get_parameter<double>("pixel_ratio_threshold");
    cout << " - Threshold for Eigen value ratio : " << this->eigen_ratio_threshold << endl;
    cout << " - Threshold for Pixel value ratio : " << this->pixel_ratio_threshold << endl;

    int nbin_param2 = 32;
    double min_param2 = -0.2;
    double max_param2 = 0.1;
    const static double cdteposz_param2[32] = {
        -8.71105, -8.63401, -8.59368, -8.56371, -8.45148,
        -8.40101, -8.29345, -8.25342, -8.15028, -8.10914,
        -8.05395, -7.9456, -7.84244, -7.76908, -7.63907,
        -7.51551, -7.40043, -7.34394, -7.21731,  -7.14637,
        -7.04336, -6.95543, -6.93367, -6.91075, -6.87694,
        -6.75941, -6.75941, -6.75941, -6.75941, -6.75941,
        -6.75941, -6.75941
    };
    this->param2_cdtez = new TH1D("param2_cdtez","param2_cdtez", nbin_param2, min_param2, max_param2);
    for ( int iparam2_cdtez=0; iparam2_cdtez<nbin_param2; ++iparam2_cdtez )
    {
        this->param2_cdtez->SetBinContent(iparam2_cdtez+1, cdteposz_param2[iparam2_cdtez]);
    }

    this->si_ee_up = new TF1( "si_ee_up", "1.3743723 * x - 0.54678582", 0.0, 1000.0 );
    this->si_ee_lo = new TF1( "si_ee_lo", "1.1449777 * x - 0.88609000", 0.0, 1000.0 );

    this->hit1_detector = 0;
    this->hit2_detector = 10;
    this->hit3_detector = -1;
    this->hit3_energy = -1.0;
    this->hit3_posx = 0.0;
    this->hit3_posy = 0.0;
    this->hit3_posz = 0.0;

    this->theta_kine = 0.0;
    this->theta_geom = 0.0;
    this->theta_elec = 0.0;
    this->phi_esti = 0.0;
    this->phi_geom = 0.0;

    this->de_over_dx = 0.0;
    this->angle_inci = 0.0;

    cout << endl;
    cout << endl;
    return anl::ANL_OK;
}

int ProjectConeETCC::mod_ana()
{
    if ( !this->event.Next() ) return anl::ANL_LOOP;
    // return anl::ANL_OK;

    auto nhit_si = this->event.merged_si_nhit;
    auto nhit_ct = this->event.n_reconst;

    if ( nhit_si != 1 || nhit_ct != 1 )
        return anl::ANL_SKIP;
    else
        evs::set("Si-CdTe 2hits event");

    auto si = ProjectConeETCC::Hit(
            this->event.init_pos_cmos_detx,
            this->event.init_pos_cmos_dety,
            this->event.init_pos_cmos_detz,
            this->event.merged_epi_si[0],
            // this->event.phi_cmos_det - TMath::Pi()
            // -1.0 * this->event.phi_cmos_det
            this->event.phi_cmos_det
        );
    auto cdte = ProjectConeETCC::Hit(
            this->event.cdte_detx[0],
            this->event.cdte_dety[0],
            this->event.cdte_detz[0],
            this->event.reconst_epi[0],
            this->event.phi_cmos_det
        );

    if ( si.Energy()<=0.0 || cdte.Energy()<=0.0 )
        return anl::ANL_SKIP;
    // evs::set("Si-CdTe 2hits event");

    auto si_energy_on_va = this->event.epi_total;
    if ( si_energy_on_va < this->si_ee_lo->Eval( si.Energy() )
        || this->si_ee_up->Eval( si.Energy() ) < si_energy_on_va )
        return anl::ANL_SKIP;
    else
        evs::set("E-consistent Si-CMOS");

    this->h2_ene_cmos_vs_cdte->Fill( si.Energy(), cdte.Energy() );
    this->h1_ene_sum_cmos_cdte->Fill( si.Energy()+cdte.Energy() );

    if ( !this->IsInEnergyWindow( si.Energy()+cdte.Energy() ) )
        return anl::ANL_SKIP;
    else
        evs::set("SC-2hits in E-window");

    if ( !IsInThetaRange( si, cdte ) )
        return anl::ANL_SKIP;
    else
        evs::set("SC-2hits in Theta-range");

    if ( this->event.reconstructed == false )
        return anl::ANL_SKIP;
    else
        evs::set("Direction_is_Reconstruted");

    if ( this->event.eigen_ratio < this->eigen_ratio_threshold )
        return anl::ANL_SKIP;
    else
        evs::set("Eigen_Value_Is_Over_Threshold");

    if ( this->event.sum_pixel_value_around_end/this->event.sum_pixel_value_around_init
        < this->pixel_ratio_threshold )
        return anl::ANL_SKIP;
    else
        evs::set("Pixel_Ratio_Is_Over_Threshold");

    this->num_hits = nhit_si + nhit_ct;
    this->hit1_energy = si.Energy();
    this->hit1_posx = si.Postion().X();
    this->hit1_posy = si.Postion().Y();
    this->hit1_posz = si.Postion().Z();
    this->hit2_energy = cdte.Energy();
    this->hit2_posx = cdte.Postion().X();
    this->hit2_posy = cdte.Postion().Y();
    this->hit2_posz = cdte.Postion().Z();
    this->totalenergy = si.Energy() + cdte.Energy();

    // this->etrack_calc_dedx->Reset();
    // for ( auto i_pixel=0; i_pixel<this->event.n_pixel; ++i_pixel ) {
    //     // auto s = this->event.cmos_detx[i_pixel] - this->event.min_cmos_detx;
    //     this->etrack_calc_dedx->Fill(
    //         this->event.cmos_detx[i_pixel] - this->event.min_cmos_detx,
    //         this->event.cmos_dety[i_pixel] - this->event.min_cmos_dety,
    //         this->event.epi_pixel_value[i_pixel]
    //     );
    // }
    // double init_x = this->event.init_pos_cmos_detx - this->event.min_cmos_detx;
    // double init_y = this->event.init_pos_cmos_dety - this->event.min_cmos_dety;
    // double init_z = this->event.init_pos_cmos_detz;
    // double cos_phi = TMath::Cos(this->event.phi_cmos_det);
    // double sin_phi = TMath::Sin(this->event.phi_cmos_det);
    // double end_x = init_x + cos_phi * 0.1;
    // double end_y = init_y + sin_phi * 0.1;

    // const static auto ndiv_deltax = (int)(20);
    // const static auto deltax      = (double)(0.02 * 5);
    // const static auto div_deltax  = (double)(deltax / ndiv_deltax);
    // auto deltae = 0.0;
    // auto n_intrack = 0;
    // for ( auto i=0; i<ndiv_deltax+1; ++i ) {
    //     auto x = init_x + cos_phi * div_deltax * i;
    //     auto y = init_y + sin_phi * div_deltax * i;
    //     auto xbin = this->etrack_calc_dedx->GetXaxis()->FindBin(x);
    //     auto ybin = this->etrack_calc_dedx->GetYaxis()->FindBin(y);
    //     auto c = this->etrack_calc_dedx->GetBinContent( xbin, ybin );
    //     if ( c==0 ) break;
    //     ++n_intrack;
    //     deltae += c;
    // }
    // deltae /= (double)(n_intrack);
    // this->de_over_dx = deltae / deltax;
    // // this->etrack_calc_dedx->SetTitle(
    // //     Form("x=%4.2f,y=%4.2f,endx=%4.2f,endy=%4.2f,dedx=%4.2f",
    // //         init_x,init_y,end_x,end_y,de_over_dx)
    // // );

    // auto angle_theta_rad = this->ComptonTheta( si.Energy(), cdte.Energy() );
    // auto angle_phi_rad = TVector2::Phi_mpi_pi( si.Phi() + TMath::Pi()*0.5 );
    // this->theta_kine = angle_theta_rad;
    // this->phi_esti = angle_phi_rad;

    this->CalcComptonEvent( si, cdte );

    if ( is_event_list_only ) {
        this->output_tree->Fill();
        return anl::ANL_OK;
    }

    auto is_filled_voxels = this->Projection( si, cdte );
    if ( is_filled_voxels ) this->output_tree->Fill();

    return anl::ANL_OK;
}

int ProjectConeETCC::mod_endrun()
{
    cout << mod_name2() << "::mod_endrun" << endl;

    this->output_file->cd();

    this->output_tree->Write();

    this->h1_arm_distribution->Write();
    this->h1_spd_distribution->Write();
    this->h2_arm_vs_spd->Write();
    this->h2_arm_distribition->Write();
    this->h2_spd_distribition->Write();

    this->h2_ene_cmos_vs_cdte->Write();
    this->h1_ene_sum_cmos_cdte->Write();

    this->output_file->Close();
    cout << " - Closed : " << this->get_parameter<std::string>("output_file") << endl;

    this->input_file->Close();
    cout << " - Closed : " << this->get_parameter<std::string>("input_file") << endl;

    return anl::ANL_OK;
}

int ProjectConeETCC::DefineBranch(TTree* tree)
{
    // tree->Branch( "ti", &ti, "ti/i" );
    // tree->Branch( "livetime", &livetime, "livetime/i" );
    // tree->Branch( "unixtime", &unixtime, "unixtime/i" );
    // tree->Branch( "ext1pps", &ext1pps, "ext1pps/i" );
    // tree->Branch( "msec_counter", &msec_counter, "msec_counter/i" );

    tree->Branch( "num_hits", &num_hits, "num_hits/I" );
    tree->Branch( "externalCLK", &externalCLK, "externalCLK/i" );
    tree->Branch( "first_internalCLK", &first_internalCLK, "internalCLK/i" );
    tree->Branch( "hit1_detector", &hit1_detector, "hit1_detector/S" );
    tree->Branch( "hit1_energy", &hit1_energy, "hit1_energy/F" );
    tree->Branch( "hit1_posx", &hit1_posx, "hit1_posx/F" );
    tree->Branch( "hit1_posy", &hit1_posy, "hit1_posy/F" );
    tree->Branch( "hit1_posz", &hit1_posz, "hit1_posz/F" );
    tree->Branch( "hit2_detector", &hit2_detector, "hit2_detector/S" );
    tree->Branch( "hit2_energy", &hit2_energy, "hit2_energy/F" );
    tree->Branch( "hit2_posx", &hit2_posx, "hit2_posx/F" );
    tree->Branch( "hit2_posy", &hit2_posy, "hit2_posy/F" );
    tree->Branch( "hit2_posz", &hit2_posz, "hit2_posz/F" );
    tree->Branch( "hit3_detector", &hit3_detector, "hit3_detector/S" );
    tree->Branch( "hit3_energy", &hit3_energy, "hit3_energy/F" );
    tree->Branch( "hit3_posx", &hit3_posx, "hit3_posx/F" );
    tree->Branch( "hit3_posy", &hit3_posy, "hit3_posy/F" );
    tree->Branch( "hit3_posz", &hit3_posz, "hit3_posz/F" );
    tree->Branch( "totalenergy", &totalenergy, "totalenergy/F" );

    tree->Branch( "theta_kine", &theta_kine, "theta_kine/F" );
    tree->Branch( "theta_geom", &theta_geom, "theta_geom/F" );
    // tree->Branch( "theta_elec", &theta_elec, "theta_elec/F" );
    tree->Branch( "phi_esti", &phi_esti, "phi_esti/F" );
    tree->Branch( "phi_geom", &phi_geom, "phi_geom/F" );
    tree->Branch( "de_over_dx", &de_over_dx, "de_over_dx/F" );

    if ( !tree->Branch( "track_image", "TH2D", &(this->etrack_calc_dedx) ) ) {
        cout << "Creating a branch of TH2D is failed." << endl;
        return anl::ANL_NG;
    }

    tree->Branch( "angle_inci", &angle_inci, "angle_inci/F" );
    tree->Branch( "prod_inci_phi", &prod_inci_phi, "prod_inci/F" );
    tree->Branch( "sum_epi_around_init",
        &sum_epi_around_init, "sum_epi_around_init/F");
    tree->Branch( "sum_epi_forward_init",
        &sum_epi_forward_init, "sum_epi_forward_init/F");

    // if ( event.ExistBranch("coin_eventid") )
    //     output_tree->Branch( "coin_eventid", &event.coin_eventid, "coin_eventid/L" );
    //
    // if ( event.ExistBranch("coin_delta_t") )
    //     output_tree->Branch( "coin_delta_t", &event.coin_delta_t, "coin_delta_t/I" );

    if ( is_event_list_only ) return anl::ANL_OK;

    if ( !tree->Branch( "response", "TH3F", &image ) ) {
        cout << "Creating a branch of TH3F is failed." << endl;
        return anl::ANL_NG;
    }
    if ( !tree->Branch( "response_etcc", "TH3F", &image_etcc ) ) {
        cout << "Creating a branch of TH3F is failed." << endl;
        return anl::ANL_NG;
    }

    return anl::ANL_OK;
}
void ProjectConeETCC::CalcComptonEvent(const Hit& si, const Hit& cdte)
{
    this->etrack_calc_dedx->Reset();
    for ( auto i_pixel=0; i_pixel<this->event.n_pixel; ++i_pixel ) {
        // auto s = this->event.cmos_detx[i_pixel] - this->event.min_cmos_detx;
        this->etrack_calc_dedx->Fill(
            this->event.cmos_detx[i_pixel] - this->event.min_cmos_detx,
            this->event.cmos_dety[i_pixel] - this->event.min_cmos_dety,
            this->event.epi_pixel_value[i_pixel]
        );
    }
    double init_x = this->event.init_pos_cmos_detx - this->event.min_cmos_detx;
    double init_y = this->event.init_pos_cmos_dety - this->event.min_cmos_dety;
    double init_z = this->event.init_pos_cmos_detz;
    double cos_phi = TMath::Cos(this->event.phi_cmos_det);
    double sin_phi = TMath::Sin(this->event.phi_cmos_det);
    double end_x = init_x + cos_phi * 0.1;
    double end_y = init_y + sin_phi * 0.1;
    auto vec_phi_on_det_plane = TVector3( cos_phi, sin_phi, 0 );

    auto init_xbin = this->etrack_calc_dedx->GetXaxis()->FindBin( init_x );
    auto init_ybin = this->etrack_calc_dedx->GetYaxis()->FindBin( init_y );

    this->sum_epi_around_init = 0.0;
    this->sum_epi_forward_init = 0.0;
    for ( auto xbin=init_xbin-2; xbin<=init_xbin+2; ++xbin ) {
        for ( auto ybin=init_ybin-2; ybin<=init_ybin+2; ++ybin ) {
            auto gbin = this->etrack_calc_dedx->GetBin( xbin, ybin );
            if ( this->etrack_calc_dedx->IsBinOverflow( gbin ) ) continue;
            if ( this->etrack_calc_dedx->IsBinUnderflow( gbin ) ) continue;

            auto cont = this->etrack_calc_dedx->GetBinContent( xbin, ybin );
            this->sum_epi_around_init += cont;

            if ( 0 <= cos_phi )
                if ( xbin < init_xbin ) continue;
            else
                if ( init_xbin < xbin ) continue;

            if ( 0<= sin_phi )
                if ( ybin < init_ybin ) continue;
            else
                if ( init_ybin < ybin ) continue;

            this->sum_epi_forward_init += cont;
        }
    }

    const static auto ndiv_deltax = (int)(20);
    const static auto deltax      = (double)(0.02 * 5);
    const static auto div_deltax  = (double)(deltax / ndiv_deltax);
    auto deltae = 0.0;
    auto n_intrack = 0;
    for ( auto i=0; i<ndiv_deltax+1; ++i ) {
        auto x = init_x + cos_phi * div_deltax * i;
        auto y = init_y + sin_phi * div_deltax * i;
        auto xbin = this->etrack_calc_dedx->GetXaxis()->FindBin(x);
        auto ybin = this->etrack_calc_dedx->GetYaxis()->FindBin(y);
        auto c = this->etrack_calc_dedx->GetBinContent( xbin, ybin );
        if ( c==0 ) break;
        ++n_intrack;
        deltae += c;
    }
    deltae /= (double)(n_intrack);
    this->de_over_dx = deltae / deltax;
    // this->etrack_calc_dedx->SetTitle(
    //     Form("x=%4.2f,y=%4.2f,endx=%4.2f,endy=%4.2f,dedx=%4.2f",
    //         init_x,init_y,end_x,end_y,de_over_dx)
    // );

    auto vec_norm_vertical = TVector3( 0, 1, 0 );
    auto scat  = si.Postion();
    auto abso  = cdte.Postion();
    auto vec_cone_axis = scat - abso;
    auto angle_theta_rad = this->ComptonTheta( si.Energy(), cdte.Energy() );
    auto angle_phi_rad = TVector2::Phi_mpi_pi( si.Phi() + TMath::Pi()*0.5 );
    this->theta_kine = angle_theta_rad;
    this->phi_esti = angle_phi_rad;

    auto vec_norm_z = TVector3( 0, 0, -1 );
    auto source_to_scat = scat - this->source_position;
    auto vec_norm_inci = source_to_scat; vec_norm_inci.Unit();
    auto scat_to_abso = abso - scat;
    auto angle_theta_rad_geom = source_to_scat.Angle(scat_to_abso);
    auto arm = (angle_theta_rad - angle_theta_rad_geom)/TMath::Pi()*180.0;
    this->h1_arm_distribution->Fill( arm );
    this->h2_arm_distribition->Fill(
        angle_theta_rad/TMath::Pi()*180.0,
        angle_theta_rad_geom/TMath::Pi()*180.0
    );
    this->theta_geom = angle_theta_rad_geom;
    this->angle_inci = vec_norm_inci.Angle( vec_norm_z );

    auto vec_axis_to_src_plane = vec_cone_axis.Unit();
    vec_axis_to_src_plane *= this->source_position.Z() - scat.Z();
    auto pos_axis_on_src_plane = scat + vec_axis_to_src_plane;
    auto vec_phi_on_src_plane = this->source_position - pos_axis_on_src_plane;
    auto angle_phi_rad_geom = TVector2::Phi_mpi_pi( vec_phi_on_src_plane.DeltaPhi( vec_norm_vertical ) );
    auto spd = (angle_phi_rad - angle_phi_rad_geom)/TMath::Pi()*180;
    this->h1_spd_distribution->Fill( spd );
    this->h2_spd_distribition->Fill(
        angle_phi_rad/TMath::Pi()*180.0,
        angle_phi_rad_geom/TMath::Pi()*180.0
    );
    this->phi_geom = angle_phi_rad_geom;

    this->prod_inci_phi = source_to_scat.Dot( vec_phi_on_det_plane );
}

bool ProjectConeETCC::Projection(const Hit& si, const Hit& cdte)
{
    this->image->Reset();
    this->image_etcc->Reset();
    auto nvoxels = this->image->GetNcells();
    // cout << std::scientific << endl;
    // cout << "Phi=" << si.Phi() << endl;
    auto vec_norm_vertical = TVector3( 0, 1, 0 );

    auto scat  = si.Postion();
    auto abso  = cdte.Postion();
    auto vec_cone_axis = scat - abso;
    auto angle_theta_rad = this->ComptonTheta( si.Energy(), cdte.Energy() );
    auto angle_phi_rad = TVector2::Phi_mpi_pi( si.Phi() + TMath::Pi()*0.5 );
    // if ( TMath::Pi() < angle_phi_rad )
    //     angle_phi_rad -= 2*TMath::Pi();
    // else if ( angle_phi_rad < -1*TMath::Pi() )
    //     angle_phi_rad += 2*TMath::Pi();
    // this->theta_kine = angle_theta_rad;
    // this->phi_esti = angle_phi_rad;

    auto source_to_scat = scat - this->source_position;
    auto scat_to_abso = abso - scat;
    auto angle_theta_rad_geom = source_to_scat.Angle(scat_to_abso);
    auto arm = (angle_theta_rad - angle_theta_rad_geom)/TMath::Pi()*180.0;
    // this->h1_arm_distribution->Fill( arm );
    // this->h2_arm_distribition->Fill(
    //     angle_theta_rad/TMath::Pi()*180.0,
    //     angle_theta_rad_geom/TMath::Pi()*180.0
    // );
    // this->theta_geom = angle_theta_rad_geom;

    auto vec_axis_to_src_plane = vec_cone_axis.Unit();
    vec_axis_to_src_plane *= this->source_position.Z() - scat.Z();
    auto pos_axis_on_src_plane = scat + vec_axis_to_src_plane;
    auto vec_phi_on_src_plane = this->source_position - pos_axis_on_src_plane;
    auto angle_phi_rad_geom = TVector2::Phi_mpi_pi( vec_phi_on_src_plane.DeltaPhi( vec_norm_vertical ) );
    auto spd = (angle_phi_rad - angle_phi_rad_geom)/TMath::Pi()*180;
    // this->h1_spd_distribution->Fill( spd );
    // this->h2_spd_distribition->Fill(
    //     angle_phi_rad/TMath::Pi()*180.0,
    //     angle_phi_rad_geom/TMath::Pi()*180.0
    // );
    // this->phi_geom = angle_phi_rad_geom;

    int n_of_filled_voxels = 0;

    for ( int i=0; i<nvoxels; ++i ) {
        if ( image->IsBinOverflow(i) || image->IsBinUnderflow(i) ) continue;

        TVector3 voxel = VoxelCenter(image, i);

        auto vec_scat2vox = voxel - scat;
        auto fVecAxisRotateGeneratingline = vec_cone_axis.Cross(vec_scat2vox);
        auto vec_genline_nearest_to_vox = vec_cone_axis;

        if ( 0.001<=fVecAxisRotateGeneratingline.Mag() ){
            vec_genline_nearest_to_vox.Rotate(angle_theta_rad, fVecAxisRotateGeneratingline);
        }

        /* The event is rejected if the direction of the generating line
        is not oriented to the front of the camera. */
        auto scale = vec_scat2vox.Dot( vec_genline_nearest_to_vox.Unit() );
        if( scale<=0.0 ) continue;

        vec_genline_nearest_to_vox *= scale/vec_genline_nearest_to_vox.Mag();

        double mag_vec_nearest_to_vox = vec_genline_nearest_to_vox.Mag();
        double perpen_dist = ( vec_scat2vox - vec_genline_nearest_to_vox ).Mag();
        double sigma = mag_vec_nearest_to_vox*this->tangent_cone_thick;

        if( sigma*3<perpen_dist ) continue;

        auto weight = TMath::Power( mag_vec_nearest_to_vox, distance_index_omega )
            *TMath::Exp( -0.5* TMath::Power( perpen_dist/sigma, 2 ) );

        if ( weight<=0 ) continue;
        image->SetBinContent( i, image->GetBinContent(i)+weight );

        auto vec_axis_projected = TVector3( vec_cone_axis.Unit() );
        double mag_vec_axis_projected = vec_axis_projected.Dot( vec_genline_nearest_to_vox );
        vec_axis_projected *= mag_vec_axis_projected;

        auto vec_phi_axis_to_surface = vec_genline_nearest_to_vox - vec_axis_projected;
        auto angle_phi_voxel = TVector2::Phi_mpi_pi( vec_phi_axis_to_surface.DeltaPhi( vec_norm_vertical ) );

        auto delta_phi = angle_phi_rad - angle_phi_voxel;
        // cout << delta_phi << "," << si.Phi() << "," << angle_phi << "," << this->arc_length_sigma*3.0 << endl;
        if ( fabs(delta_phi)<this->arc_length_sigma*3 ) {
            // cout << delta_phi << "," << si.Phi() << "," << angle_phi << endl;
            weight *= TMath::Exp( -0.5* TMath::Power( delta_phi/this->arc_length_sigma, 2 ) );
            image_etcc->SetBinContent( i, image_etcc->GetBinContent(i)+weight );
            ++n_of_filled_voxels;
        }

    }

    const static int threshold_n_of_filled_voxels = 10;

    if ( n_of_filled_voxels<=threshold_n_of_filled_voxels ) return false;

    if ( this->enable_normalize_cone==false ) return true;

    auto filling_ratio = this->GetFillingRatio(image, scat, abso, angle_theta_rad);
    this->ScaleByFillingRatio( image, filling_ratio );

    filling_ratio = this->GetFillingRatio(image_etcc, scat, abso, angle_theta_rad);
    this->ScaleByFillingRatio( image_etcc, filling_ratio );

    return true;
}
TH1D* ProjectConeETCC::GetFillingRatio
(TH3F* image, const TVector3& scat, const TVector3& abso, double angle_theta_rad)
{
    auto vec_cone_axis = scat - abso;
    // auto angle_theta_rad = eval_theta( si.Energy(), cdte.Energy() );

    auto vec_genline_origin = vec_cone_axis;
    auto vertical = vec_cone_axis.Cross( TVector3(0,1,0) );
    vec_genline_origin.Rotate( angle_theta_rad, vertical );

    static const int nplot = 1000;
    double deltaphi = 2*TMath::Pi()/nplot;

    // auto zaxis = image->GetZaxis();
    this->h1_cone_filling_ratio->Reset();
    auto nbins = this->h1_cone_filling_ratio->GetXaxis()->GetNbins();

    for ( int iphi=0; iphi<nplot; ++iphi ) {

        auto phi = deltaphi*iphi;
        auto vec_genline = vec_genline_origin;
        vec_genline.Rotate( phi, vec_cone_axis );

        for ( int zbin=1; zbin<nbins+1; zbin++ ) {

            auto z = this->h1_cone_filling_ratio->GetXaxis()->GetBinCenter( zbin );
            auto deltaz_scat_to_xyplane = z - scat.Z();

            auto vec_genline_scat_to_xyplane = vec_genline;
            vec_genline_scat_to_xyplane *= deltaz_scat_to_xyplane/vec_genline.Z();

            auto pos_genline_on_xyplane = scat + vec_genline_scat_to_xyplane;

            if ( this->IsVectorInside(image, pos_genline_on_xyplane) )
                this->h1_cone_filling_ratio->Fill( z, (double)1/nplot );

        }
    }
    return this->h1_cone_filling_ratio;
}
double ProjectConeETCC::ScaleByFillingRatio(TH3F* th3, TH1D* ratio)
{
    auto nbinsx = th3->GetXaxis()->GetNbins();
    auto nbinsy = th3->GetYaxis()->GetNbins();
    auto nbinsz = th3->GetZaxis()->GetNbins();
    auto nbins_th1 = ratio->GetXaxis()->GetNbins();
    if ( nbinsz != nbins_th1 ) return -1;

    for ( int zbin=1; zbin<=nbinsz; ++zbin ) {

        auto z = th3->GetZaxis()->GetBinCenter(zbin);
        double integral = 0.0;

        for ( int xbin=1; xbin<=nbinsx; ++xbin )
            for ( int ybin=1; ybin<=nbinsy; ++ybin )
                integral += th3->GetBinContent( xbin, ybin, zbin );
        if ( integral == 0.0 ) continue;

        auto factor = 1/integral;
        auto rate = this->GetBinContent(ratio, z);
        if ( factor==0.0 || rate==0.0 ) continue;

        for ( int xbin=1; xbin<=nbinsx; ++xbin ) {
            for ( int ybin=1; ybin<=nbinsy; ++ybin ) {
                th3->SetBinContent( xbin, ybin, zbin,
                th3->GetBinContent( xbin, ybin, zbin )
                *factor*rate );
            }
        }
    }

    return 0;
}


int ComptreeEvent::SetBranchAddress(TTree* tree)
{
    auto branch_list = { "merged_si_nhit", "n_reconst", "epi_total", "reconstructed", "phi_cmos_det" };
    for ( auto branch : branch_list )
        if ( !this->ExistBranch(tree, branch) ) return anl::ANL_NG;

    static const int alloc_size = 100;
    this->merged_epi_si.resize(alloc_size);
    this->epi1.resize(alloc_size);
    this->epi2.resize(alloc_size);
    this->reconst_epi.resize(alloc_size);
    this->cdte_detx.resize(alloc_size);
    this->cdte_dety.resize(alloc_size);
    this->cdte_detz.resize(alloc_size);
    this->epi_pixel_value.resize(alloc_size);
    this->cmos_detx.resize(alloc_size);
    this->cmos_dety.resize(alloc_size);

    tree->SetBranchStatus( "*", 0 );
    tree->SetBranchStatus( "merged_si_nhit", 1 );
    tree->SetBranchStatus( "merged_epi_si", 1 );
    tree->SetBranchStatus( "n_reconst", 1 );
    tree->SetBranchStatus( "epi1", 1 );
    tree->SetBranchStatus( "epi2", 1 );
    tree->SetBranchStatus( "reconst_epi", 1 );
    tree->SetBranchStatus( "cdte_det*", 1 );
    tree->SetBranchStatus( "epi_total", 1 );
    tree->SetBranchStatus( "n_pixel", 1 );
    tree->SetBranchStatus( "epi_pixel_value", 1 );
    tree->SetBranchStatus( "cmos_detx*", 1 );
    tree->SetBranchStatus( "min_cmos_det*", 1 );
    tree->SetBranchStatus( "sum_pixel_value_around_*", 1 );
    tree->SetBranchStatus( "eigen_ratio", 1 );
    tree->SetBranchStatus( "reconstructed", 1 );
    tree->SetBranchStatus( "init_pos_cmos_det*", 1 );
    tree->SetBranchStatus( "phi_cmos_det", 1 );

    tree->SetBranchAddress( "merged_si_nhit", &(this->merged_si_nhit) );
    tree->SetBranchAddress( "merged_epi_si", this->merged_epi_si.data() );
    tree->SetBranchAddress( "n_reconst", &(this->n_reconst) );
    tree->SetBranchAddress( "epi1", this->epi1.data() );
    tree->SetBranchAddress( "epi2", this->epi2.data() );
    tree->SetBranchAddress( "reconst_epi", this->reconst_epi.data() );
    tree->SetBranchAddress( "cdte_detx", this->cdte_detx.data() );
    tree->SetBranchAddress( "cdte_dety", this->cdte_dety.data() );
    tree->SetBranchAddress( "cdte_detz", this->cdte_detz.data() );
    tree->SetBranchAddress( "epi_total", &(this->epi_total) );
    tree->SetBranchAddress( "n_pixel", &(this->n_pixel) );
    tree->SetBranchAddress( "epi_pixel_value", this->epi_pixel_value.data() );
    tree->SetBranchAddress( "cmos_detx", this->cmos_detx.data() );
    tree->SetBranchAddress( "cmos_dety", this->cmos_dety.data() );
    tree->SetBranchAddress( "min_cmos_detx", &(this->min_cmos_detx) );
    tree->SetBranchAddress( "min_cmos_dety", &(this->min_cmos_dety) );
    tree->SetBranchAddress( "sum_pixel_value_around_init", &(this->sum_pixel_value_around_init) );
    tree->SetBranchAddress( "sum_pixel_value_around_end", &(this->sum_pixel_value_around_end) );
    tree->SetBranchAddress( "eigen_ratio", &(this->eigen_ratio) );
    tree->SetBranchAddress( "reconstructed", &(this->reconstructed) );
    tree->SetBranchAddress( "init_pos_cmos_detx", &(this->init_pos_cmos_detx) );
    tree->SetBranchAddress( "init_pos_cmos_dety", &(this->init_pos_cmos_dety) );
    tree->SetBranchAddress( "init_pos_cmos_detz", &(this->init_pos_cmos_detz) );
    tree->SetBranchAddress( "phi_cmos_det", &(this->phi_cmos_det) );

    this->nentries = tree->GetEntries();
    this->current_entry = -1;
    this->tree = tree;

    return 0;
}
