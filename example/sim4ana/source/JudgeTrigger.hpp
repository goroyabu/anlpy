/**
   @file   JudgeTrigger.hpp
   @date   2020/08/24
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#ifndef JudgeTrigger_hpp
#define JudgeTrigger_hpp

#include <utility>

#include <TRandom3.h>

#include <VANL_Module.hpp>

class JudgeTrigger : public anl::VANL_Module
{
public:
    JudgeTrigger();
    ~JudgeTrigger();

    int mod_bgnrun() override;
    int mod_ana() override;
    int mod_endrun() override;

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
    inline double Pedestal() const
    {
	return gRandom->Gaus(0, pedestal_sigma);
    }	
    	
private:
	
    int nbinsx, nbinsy;
    double xmin, xmax;
    double ymin, ymax;
    double pedestal_sigma;

    bool is_enabled_add_pedestal;
    bool is_enabled_si_cdte_coincidence;
    int detid_max_si;
    int detid_min_cdte;

    // struct hit_data_res
    // {		
    // 	int detid;
    // 	int material;
    // 	int flag;
    // 	int strip_x;
    // 	int strip_y;
    // 	double edep_x;
    // 	double edep_y;
    // 	double pos_x;
    // 	double pos_y;	
    // };           
    
    struct strip_data_trg	
    {
	std::pair<int, int> det_strip_id;
	int material;
	double edep;
	double pos;
	
	static int index_in
	(const std::vector<strip_data_trg>& data, const int detid, const int stripid)
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
	int index_in(const std::vector<strip_data_trg>& data) const
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
	void merge_same_strip(std::vector<strip_data_trg>& data)
	{
	    auto index = this->index_in(data);
	    if ( index<0 ) 
		data.emplace_back( *this );
	    else
		data[index].edep += this->edep;
	}
    };

    inline bool is_coin_si_cd(const std::vector<int>& detid_list)
    {
	bool is_si = false;
	bool is_cdte = false;
	for ( auto detid : detid_list ) {
	    if ( detid<=detid_max_si ) is_si = true;
	    else if ( detid_min_cdte<=detid ) is_cdte = true;
	}
	return is_si && is_cdte;
    }
    
};

#endif

