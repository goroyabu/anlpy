/***
    @file   FilterComptree.hpp
    @date   2021/10/14
    @author Goro Yabu
***/

#ifndef FilterComptree_hpp
#define FilterComptree_hpp

#include <tuple>

#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <TMath.h>
#include <TVector3.h>
#include <TF1.h>

#include <VANL_Module.hpp>

#include "FilterComptree_inc.hpp"

class FilterComptree : public anl::VANL_Module
{
    public:
        FilterComptree();
        ~FilterComptree();

        int mod_bgnrun() override;
        int mod_ana() override;
        int mod_endrun() override;

    protected:
        ComptreeReader event;

        TFile * input_file;
        TTree * input_tree;
        TFile * output_file;
        TTree * output_tree;

        TH1D * param2_cdtez;
        TF1 * si_ee_lo;
        TF1 * si_ee_up;

        bool enable_reject_fluor;

        TVector3 source_position;
        double detector_z_position;

        double e_threshold_si;
        double e_threshold_cdte;
        double e_window_begin;
        double e_window_end;
        double theta_max_degree;
        double eigen_ratio_threshold;
        double pixel_ratio_threshold;

        int num_hits;
        short hit1_detector;
        float hit1_energy;
        float hit1_posx;
        float hit1_posy;
        float hit1_posz;
        short hit2_detector;
        float hit2_energy;
        float hit2_posx;
        float hit2_posy;
        float hit2_posz;
        float totalenergy;

        bool is_consis_si_energy;
        bool is_in_energy_range;
        bool is_in_theta_range;
        bool is_over_thre_eigen;
        bool is_over_thre_pixel_ratio;

        float theta_kine;
        float theta_geom;
        // float theta_elec;
        float phi_esti;
        float phi_geom;

        float angle_inci;
        float de_over_dx;
        float prod_inci_phi;
        float sum_epi_around_init;
        float sum_epi_forward_init;

        std::vector<float> vector_incident;
        std::vector<float> vector_photon;
        std::vector<float> vector_electron;
        float angle_electron_on_plane;
        float angle_electron_vertical;

        TH2D * beta_vs_alpha_angle_electron;
        TH2D * angle_distribution_electron;

    private:
        static TH1D* define_zparam_function();
        int define_branch(TTree* tree);
        int define_histogram();

    public:

        inline static double ComptonTheta(double scat, double abso)
        {
            static const double mass_of_electron = 511.0;
            auto costheta = 1 - mass_of_electron * ( 1/abso - 1/(scat+abso) );
            if ( costheta<-1.0 || 1.0<costheta ) return -1;
            return std::fabs( TMath::ACos(costheta) );
        }
        inline bool IsInEnergyWindow(const double total_energy) const
        {
            if ( this->e_window_begin<=total_energy && total_energy<=this->e_window_end )
                return true;
            return false;
        }
        inline bool IsInThetaRange(const double scat, const double abso) const
        {
            auto theta = ComptonTheta( scat, abso )/TMath::Pi()*180.0;
            if ( 0<theta && theta<=this->theta_max_degree )
                return true;
            return false;
        }
        inline static bool IsFluor(double energy)
        {
            return 21.0<energy && energy<=28.0;
        }
        inline static TVector3 ElectronVector(TVector3 norm_inci, TVector3 norm_photon, const double e_electron, const double e_photon)
        {
            static const double mass_of_electron = 511.0;
            double val = TMath::Sqrt( e_electron * (e_electron+2*mass_of_electron));
            norm_inci  *= (e_electron+e_photon)/val;
            norm_photon *= e_photon/val;
            return norm_inci + norm_photon;
        }
        inline static std::tuple<double,double> VectorToAlphaBeta(const TVector3& vec)
        {
            auto x = vec.X(); auto y = vec.Y(); auto z = vec.Z();

            auto alpha_norm = TMath::Sqrt( x*x + y*y );
            auto alpha_x = x/alpha_norm; auto alpha_y = y/alpha_norm;
            auto alpha = TVector2::Phi_mpi_pi( TMath::ACos(alpha_x) );
            if ( alpha_y < 0.0 ) alpha *= -1.0;
            alpha = TVector2::Phi_mpi_pi( alpha + TMath::Pi() * 0.5 );

            auto beta_norm = TMath::Sqrt( x*x + y*y + z*z );
            auto beta_z = z/beta_norm;
            auto beta = TMath::ASin(beta_z);
            if ( beta_z < 0.0 ) beta *= -1.0;

            return std::make_tuple(alpha, beta);
        }
};


#endif