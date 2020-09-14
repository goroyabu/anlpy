/**
   @file   DiffuseEnergyCharge.hpp
   @date   2020/08/24
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#ifndef DiffuseEnergyCharge_hpp
#define DiffuseEnergyCharge_hpp

#include <cmath>

#include <TH2D.h>
#include <TRandom3.h>

#include <VANL_Module.hpp>

class DiffuseEnergyCharge : public anl::VANL_Module
{
public:
    DiffuseEnergyCharge();
    ~DiffuseEnergyCharge();

    int mod_bgnrun() override;
    int mod_ana() override;
    int mod_endrun() override;

private:
    
    struct detector_pixel
    {
	int nbinsx;
	double xmin, xmax;

	int nbinsy;
	double ymin, ymax;

	int material;
	double pos_z;

	int nhits;
	
	TH2D* histo;
	std::map<int, double> content;
	
	// std::map<int, TH2D*> histo;
	// std::map<int, int> mate;
	// std::map<int, double> pos_z;
	// std::map<int, int> nhits;
	
	detector_pixel(int detid, int material, double pos_z,
		       int nbinsx, double xmin, double xmax,
		       int nbinsy, double ymin, double ymax)
	{
	    nhits = 0;
	    this->material = material;
	    this->pos_z = pos_z;
	    this->nbinsx = nbinsx;
	    this->xmin = xmin;
	    this->xmax = xmax;
	    this->nbinsy = nbinsy;
	    this->ymin = ymin;
	    this->ymax = ymax;
	    histo = new TH2D( Form("hist_detid%d", detid), "h",
			      nbinsx, xmin, xmax, nbinsy, ymin, ymax );	    
	}
	
	std::tuple<int,int> GetBinXYZ(int bin)
	{
	    auto xbin = bin%(this->nbinsx+2);
	    auto ybin = ( (bin-xbin)/(this->nbinsx+2) )
		%(this->nbinsy+2);
	    return std::make_tuple( xbin, ybin );
	}
	bool IsBinInRange(int xbin, int ybin)
	{
	    if ( xbin<=0 || this->nbinsx+1<xbin ) return false;
	    if ( ybin<=0 || this->nbinsy+1<ybin ) return false;
	    return true;
	}
	void AddContent(double x, double y, double e)
	{
	    auto bin = histo->FindBin(x,y);
	    if ( content.find(bin)!=content.end() )
		content[bin] += e;
	    else
		content[bin] = e;	    
	}
	void Reset()
	{
	    content.clear();
	    nhits = 0;
	}
	
    };

    struct detector_stack
    {
	int nbinsx;
	double xmin, xmax;

	int nbinsy;
	double ymin, ymax;

	std::map<int,detector_pixel*> detectors;
	
	bool Exist(int detid)
	{
	    return detectors.end()!= detectors.find(detid);
	}
	void AddDetector(int detid, int material, double pos_z)
	{
	    auto det = new detector_pixel
		(detid, material, pos_z, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
	    detectors[detid] = det;
	}
	void SetAxis
	(int nx, double x1, double x2, int ny, double y1, double y2)
	{
	    nbinsx = nx; xmin = x1; xmax = x2;
	    nbinsy = ny; ymin = y1; ymax = y2;
	}
	void Reset()
	{
	    for ( auto&& [ detid, det ] : detectors )
		det->Reset();
	}

    } detector_pixels;
    
    struct parameter_list
    {
	int diffusion_mode;
	double temperature;
	// double thickness;
	double thickness_of_si;
	double thickness_of_cdte;
	double spread_factor_anode;
	bool is_upsize_anode;
	double bias_voltage;
	
	int efield_mode;
	enum field_type { constant, linear };
	// double efield_mean;
	double efield_mean_of_si;
	double efield_mean_of_cdte;
	double efield_alpha;
	
    } parameter;

    struct hit
    {
	int material;
	int detid;
	// int strip_x;
	// int strip_y;
	double edep;
	double pos_x;
	double pos_y;
	double pos_z;
	double pixel_center_z;
    };

    double DiffusionSigmaAnode(double z, double pixel_center_z, int material);
    double Mutau(double z, double z_initial, int material);
    // int FillDiffusedEnergy(TH2D* detector_pixel, const hit& h);
    int FillDiffusedEnergy(detector_stack& det, const hit& h);
};

#endif

