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

    
    struct detector_pixel
    {
	int nbinsx;
	double xmin, xmax;

	int nbinsy;
	double ymin, ymax;

	int material;
	double pos_z;
	double pixel_center_z;
	
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
	void AddDetector(hit& h)
	{
	    auto det = new detector_pixel
		( h.detid, h.material, h.pos_z,
		  nbinsx, xmin, xmax, nbinsy, ymin, ymax);
	    det->pixel_center_z = h.pixel_center_z;
	    detectors[h.detid] = det;
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

	double charge_factor_max;
	double charge_factor_min;
	// bool is_enabled_cce_si_xside;
	// bool is_enabled_cce_si_yside;
	bool is_enabled_cce_cdte_xside;
	// bool is_enabled_cce_cdte_yside;
	
    } parameter;


    struct strip_data
    {
        std::pair<int, int> det_strip_id;
        int material;
        double edep;
        double pos;
	double z, pixel_center_z;
	
        static int index_in
        (const std::vector<strip_data>& data, const int detid, const int stripid)
        {
            int index = 0;
            for ( auto itr : data ) {
                if ( itr.det_strip_id.first==detid &&
                     itr.det_strip_id.second==stripid )
                    return index;
                ++index;
            }
            return -1;
        }
        int index_in(const std::vector<strip_data>& data) const
        {
            int index = 0;
            for ( auto itr : data ) {

                if ( itr.det_strip_id.first==this->det_strip_id.first &&
                     itr.det_strip_id.second==this->det_strip_id.second )
                    return index;
                ++index;

            }
            return -1;
        }
        void merge_same_strip(std::vector<strip_data>& data)
        {
            auto index = this->index_in(data);
            if ( index<0 )
                data.emplace_back( *this );
            else {
		// if ( this->edep<data[index].edep ) {		    
		// }
                data[index].edep += this->edep;		
	    }
        }
    };
    
    int nbinsx, nbinsy;
    double xmin, xmax;
    double ymin, ymax;    
    
    double DiffusionSigmaAnode(double z, double pixel_center_z, int material);
    double Mutau(double z, double z_initial, int material);
    // int FillDiffusedEnergy(TH2D* detector_pixel, const hit& h);
    int FillDiffusedEnergy(detector_stack& det, const hit& h);
    
    inline int NstripX() const { return nbinsx; }
    inline int NstripY() const { return nbinsy; }
    inline double Xmin() const { return xmin; }
    inline double Xmax() const { return xmax; }
    inline double Ymin() const { return ymin; }
    inline double Ymax() const { return ymax; }
    inline double Xposition(int stripid_x) const
    {
        return (xmax-xmin)/nbinsx*(stripid_x-0.5) + xmin;
    }
    inline double Yposition(int stripid_y) const
    {
        return (ymax-ymin)/nbinsy*(stripid_y-0.5) + xmin;
    }
    inline double ChargeCollectionFactorX(strip_data& data)
    {
	return ChargeCollectionFactor(data, true);
    }
    inline double ChargeCollectionFactorY(strip_data& data)
    {
	return ChargeCollectionFactor(data, false);
    }
    inline double ChargeCollectionFactor(strip_data& data, bool is_hole_side)
    {
	// static const double factor_max = 1.00;
	// static const double factor_min = 0.75;	
	static const double factor_max = parameter.charge_factor_max;
	static const double factor_min = parameter.charge_factor_min;
	static const double factor_range_width = factor_max - factor_min;
	
	if ( data.material==0 ) return 1.0;
	if ( is_hole_side==false ) return 1.0;
	
	if ( parameter.is_enabled_cce_cdte_xside==false ) return 1.0;
	
	auto thickness = parameter.thickness_of_cdte;
	auto position  = data.z;
	auto center    = data.pixel_center_z;
	auto surface   = center - thickness*0.5;
	
	static const double dist_max = thickness;
	static const double dist_min = 0.0;
	static const double dist_range_width = dist_max - dist_min;
	static const double linear_coefficient = factor_range_width/dist_range_width;
	
	auto dist_from_surf = std::fabs( position - surface );
	if ( dist_max<dist_from_surf ) dist_from_surf = thickness;
	if ( dist_from_surf<dist_min ) dist_from_surf = 0.0;
	
	// auto factor = 1 - dist_from_surf/3;
	auto factor = 1 -dist_from_surf*linear_coefficient;
	
	// std::cout << "pos=" << position;
	// std::cout << ",cent=" << center;
	// std::cout << ",surf=" << surface;
	// std::cout << ",dist=" << dist_from_surf;
	// std::cout << ",factor=" << factor << std::endl;	
	
	return factor;
    }
    inline int get_stripid_x(double pos)
    {
	if ( pos<this->xaxis_min ) return -1;	
	for ( int id=0; id<this->nstrips_x; ++id ) {
	    auto pos_rel = pos - this->xaxis_min;
	    if ( id*strip_pitch_x<=pos_rel && pos_rel<(id+1)*strip_pitch_x )
		return id;
	}
	return -1;
    }
    inline int get_stripid_y(double pos)
    {
	if ( pos<this->yaxis_min ) return -1;	
	for ( int id=0; id<this->nstrips_y; ++id ) {
	    auto pos_rel = pos - this->yaxis_min;
	    if ( id*strip_pitch_y<=pos_rel && pos_rel<(id+1)*strip_pitch_y )
		return id;
	}
	return -1;
    }
    
    inline std::vector<int>convert_stripid_x
    (int nhits, const std::vector<int>& detid, const std::vector<double>& pos)
    {
	std::vector<int> new_array;
	for ( int ihit=0; ihit<nhits; ++ihit) {
	    auto stripid = get_stripid_x( pos[ihit] );
	    auto new_stripid = detid[ihit] * (nstrips_x+nstrips_y)
		+ nstrips_x - stripid -1;
	    new_array.emplace_back( new_stripid );
	}
	return new_array;
    }
    inline std::vector<int>convert_stripid_y
    (int nhits, const std::vector<int>& detid, const std::vector<double>& pos)
    {
	std::vector<int> new_array;
	for ( int ihit=0; ihit<nhits; ++ihit) {
	    auto stripid = get_stripid_y( pos[ihit] );
	    auto new_stripid = detid[ihit] * (nstrips_x+nstrips_y)
		+ nstrips_x + nstrips_y - stripid -1;
	    new_array.emplace_back( new_stripid );
	}
	return new_array;
    }
    int nstrips_x, nstrips_y;
    double xaxis_min, xaxis_max;
    double yaxis_min, yaxis_max;
    double strip_pitch_x, strip_pitch_y;
    

};

#endif

