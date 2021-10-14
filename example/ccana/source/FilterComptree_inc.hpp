/***
    @file   FilterComptree_inc.hpp
    @date   2021/10/14
    @author Goro Yabu
***/

#ifndef FilterComptree_inc_hpp
#define FilterComptree_inc_hpp

#include <vector>
#include <iostream>

#include <TTree.h>

class ComptreeReader
{
    private:
        TTree* tree;
        long nentries;
        long current_entry;

    public:

        int merged_si_nhit;
        std::vector<double> merged_epi_si;

        int n_reconst;
        std::vector<double> epi1;
        std::vector<double> epi2;
        std::vector<double> reconst_epi;
        std::vector<double> cdte_detx;
        std::vector<double> cdte_dety;
        std::vector<double> cdte_detz;
        double epi_total;

        int n_pixel;
        std::vector<double> epi_pixel_value;
        std::vector<double> cmos_detx;
        std::vector<double> cmos_dety;
        double min_cmos_detx;
        double min_cmos_dety;
        double eigen_ratio;
        double sum_pixel_value_around_init;
        double sum_pixel_value_around_end;

        bool reconstructed;
        double init_pos_cmos_detx;
        double init_pos_cmos_dety;
        double init_pos_cmos_detz;
        double phi_cmos_det;

        ComptreeReader()
            : tree(nullptr)
            {}

        inline bool ExistBranch(TTree* tree, TString key)
        {
            if ( !tree->FindBranch(key) ) {
                std::cout << "TBranch " << key;
                std::cout << " is not found." << std::endl;
                return false;
            }
            return true;
        }
        inline bool Next()
        {
            ++current_entry;
            if ( current_entry>=nentries ) return false;
            tree->GetEntry(current_entry);
            return true;
        }
        inline long long int GetEntries() const
        {
            return this->nentries;
        }
        int SetBranchAddress(TTree* tree);

};

struct ComptonHit
{
    private :

        double x, y, z;
        double energy;
        double phi;

    public :
        ComptonHit(double x, double y, double z, double energy, double phi)
            : x(x), y(y), z(z), energy(energy), phi(phi)
        {}
        ComptonHit(const ComptonHit& other)
            : x(other.x), y(other.y), z(other.z),
            energy(other.energy), phi(other.phi)
        {}
        ComptonHit& operator=(const ComptonHit& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            energy = other.energy;
            phi = other.phi;
            return *this;
        }
        TVector3 Postion() const
        {
            return TVector3(x,y,z);
        }
        double Energy() const
        {
            return energy;
        }
        double Phi() const
        {
            return phi;
        }
};

#endif