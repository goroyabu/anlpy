/**
   @file   ProjectCone3D.hpp
   @date   2020/05/07
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#ifndef ProjectCone3D_hpp
#define ProjectCone3D_hpp

#include <tuple>

#include <TFile.h>
#include <TTree.h>
#include <TH3F.h>
#include <TMath.h>
#include <TVector3.h>

#include <VANL_Module.hpp>

class ProjectCone3D : public anl::VANL_Module
{

public:
    
    ProjectCone3D();
    ~ProjectCone3D();

    int mod_bgnrun() override;
    int mod_ana() override;
    int mod_endrun() override;
    
public:

    struct hit
    {
	int detid;
	double e,x,y,z;

	hit()
	    : detid(-1), e(0.0), x(0.0), y(0.0), z(0.0)
	{}
	hit(const hit& other)
	    : detid(other.detid), e(other.e),
	      x(other.x), y(other.y), z(other.z)
	{}
	hit& operator=(const hit& other)
	{
	    detid = other.detid;
	    e = other.e;
	    x = other.x; y = other.y; z = other.z;
	    return *this;
	}
	TVector3 Postion() const
	{
	    return TVector3(x,y,z);
	}
	double Energy() const
	{
	    return e;
	}
	
    };

    class hittree_event
    {
    private:
	TTree* tree;
	long nentries;
	long current_entry;

    public:
	unsigned int ti;
	unsigned int livetime;
	unsigned int unixtime;
	unsigned int ext1pps;
	unsigned int msec_counter;
	
	int nhit_lv3;
	std::vector<int> detid_lv3;
	std::vector<float> epi_lv3;
	std::vector<float> epi_x_lv3;
	std::vector<float> epi_y_lv3;
	std::vector<float> pos_x_lv3;
	std::vector<float> pos_y_lv3;
	std::vector<float> pos_z_lv3;

	long long coin_eventid;
	int coin_delta_t;
	
	hittree_event()
	    : tree(nullptr)
	{}	

	bool exist_branch(TTree* tree, TString key);
	bool exist_branch(TString key) {
	    return exist_branch( this->tree, key);
	}
	int set_branch_address(TTree* tree);
	bool next();
       	
    };

protected:

    TFile * input_file;
    TTree * input_tree;
    TFile * output_file;
    TTree * output_tree;
    TH3F * image;

    /** parameters */
    double cone_thick_rad;
    double tangent_cone_thick;
    double distance_index_omega;
    double e_threshold_si;
    double e_threshold_cdte;
    double e_window_begin;
    double e_window_end;
    double theta_max_degree;
    double detector_z_position;
    bool is_event_list_only;
    double rotation_around_vertical_deg;
    
    hittree_event event;
    
    /* branch */
    unsigned int ti;
    unsigned int livetime;
    unsigned int unixtime;
    unsigned int ext1pps;
    unsigned int msec_counter;
    
    int num_hits;
    unsigned int externalCLK;
    unsigned int first_internalCLK;
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
    short hit3_detector;
    float hit3_energy;
    float hit3_posx;
    float hit3_posy;
    float hit3_posz;
    float totalenergy;    

public:

    bool next() { return event.next(); }
    std::tuple<hit, hit> get_sc2hit_event();
    bool projection(TH3F* image, const hit& si, const hit& cdte);

    inline bool projection(const hit& si, const hit& cdte)
    {
	return projection(image, si, cdte);
    }
    inline int fill()
    {
	output_tree->Fill();
	return 0;
    }
    
    // inline bool has_flour(const hit& h)
    // {
    // 	//if ( 20.0<h.e && h.e<30.0 ) return true;
    // 	return false;
    // }
    inline bool is_in_energy_range(const double total_energy)
    {
	if ( e_window_begin<=total_energy && total_energy<=e_window_end )
	    return true;
	return false;
    }
    inline bool is_in_theta_range(const hit& scat, const hit& abso)
    {
	auto theta = eval_theta( scat.e, abso.e );
	if ( 0<theta && theta<=theta_max_degree ) return true;
	return false;
    }
    inline bool is_fluor(double energy)
    {
	// return false;
	return 21.0<energy && energy<=28.0;
    }
    
    inline bool is_si(int detid)
    {
	return detid==0 || detid==1;
    }
    inline bool is_cdte(int detid)
    {
	return detid==10 || detid==11 || detid==12;
    }
    
    inline double eval_theta(double scat, double abso)
    {
	static const double mass_of_electron = 511.0;
	auto costheta = 1 - mass_of_electron * ( 1/abso - 1/(scat+abso) );
	if ( costheta<-1.0 || 1.0<costheta ) return -1;
	return std::fabs( TMath::ACos(costheta) );
    }
    inline TVector3 voxel_center(TH3* h, int bin)
    {
	int xbin, ybin, zbin;
	h->GetBinXYZ( bin, xbin, ybin, zbin );
	auto x = h->GetXaxis()->GetBinCenter(xbin);
	auto y = h->GetYaxis()->GetBinCenter(ybin);
	auto z = h->GetZaxis()->GetBinCenter(zbin);
	return TVector3(x, y, z);				
    }
    inline std::tuple<long, int> coin_info()
    {
	return std::make_tuple( event.coin_eventid, event.coin_delta_t );
    }

private:
    int define_branch(TTree* tree);
   
    
};

#endif

