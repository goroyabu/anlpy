/**
   @file   IterateCone3D.hpp
   @date   2020/05/12
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#ifndef IterateCone3D_hpp
#define IterateCone3D_hpp

#include <VANL_Module.hpp>

#include <iostream>
#include <thread>

#include <TFile.h>
#include <TTree.h>
#include <TH3F.h>
#include <TH2D.h>

class IterateCone3D : public anl::VANL_Module
{
    
public:
    
    IterateCone3D();
    ~IterateCone3D();

    int mod_bgnrun() override;
    int mod_ana() override;
    int mod_endrun() override;

public:

    struct resptree_event
    {
	TTree * tree;
	long nentries;
	long current_entry;

	long first_entry;
	long last_entry;
	
	TH3F * response;
	resptree_event()
	    : tree(nullptr)
	{
	    response = new TH3F();
	    nentries = 0;
	    current_entry = -1;
	    set_entry_range(0,-1);
	}
	~resptree_event() {}

	long set_branch_address(TTree* tree)
	{
	    this->tree = tree;
	    tree->SetBranchAddress("response", &response);
	    nentries = tree->GetEntries();
	    if ( nentries>0 ) tree->GetEntry(0);
	    return nentries;
	}

	void set_entry_range(long first, long last)
	{
	    if ( last<0 || nentries<last ) last = nentries;
	    if ( 0<first ) first = 0;

	    if ( first < last ) {
		first_entry = first;
		last_entry = last;
		return;
	    }
	    
	    first_entry = 0;
	    last_entry = nentries;
	}
	
	bool next()
	{
	    ++current_entry;

	    if ( current_entry>=last_entry ) return false;
	    // if ( current_entry>=nentries ) return false;
	    tree->GetEntry(current_entry);

	    if ( current_entry%100==0 ) {
		std::cout << current_entry << "/" << nentries << std::endl;
	    }
	    
	    return true;
	}
	bool init_entry()	    
	{
	    //if ( nentries<=first ) first = 0;
	    current_entry = first_entry-1;
	    //if ( nentries>first ) tree->GetEntry(first);
	    tree->GetEntry(first_entry);
	    return true;
	}
    };

    class TH3Slicer
    {
    public:
	static std::vector<TH2D*> Slice(TH3F* th3);
    private:
	static TH2D* define_slice(TH3F* th3, int slice_axis=2);
    };
    
protected:
    
    TFile * input_file;
    TTree * input_tree;
    TFile * output_file;
    resptree_event event;
    TH3F * sbp_image;
    std::vector<TH3F*> mlem_image;
    
    std::vector<double> vector_integral_of_response;
    // std::vector<double> vector_integral_of_multiple;    
    
    /* Parameters */
    int n_of_iterations;
    double denominator_offset;
    int eventid;
    int current_entry;
    bool is_enabled_multiple_thread;
    int n_threads;
    
    TH3F* next_image(TH3F* previous_image);

    using vector1 = std::vector<float>;
    using vector2 = std::vector<vector1>;
    using vector3 = std::vector<vector2>;

    vector3 make_vector3(int x, int y, int z, double value);

    void h2v_get_elements(TH3F* th3, vector3* out);
    //{ get_elements(th3, out); }
    void h2v_add_elements(TH3F* th3, vector3* out);
    
    void v2h_add_elements(const vector3& in, TH3F* th3)
    { add_elements(in, th3); }
    void  v2h_set_elements(const vector3& in, TH3F* th3);

    void v2v_copy_elements(const vector3& in, vector3* out)
    { copy_elements(in, out); }
    void v2v_add_elements(const vector3& in, vector3* out);
    void v2v_multiply_elements(const vector3& in, vector3* out);
    //{ multiply_elements(in, out); }

    double get_integral(const vector3& in);       
    void scale_elements(double factor, vector3* out);
    
    // void multiply_elements(const vector3& in, vector3* out);
    void copy_elements(const vector3& in, vector3* out);
    // void get_elements(TH3F* th3, vector3* out);
    void add_elements(const vector3& in, TH3F* th3);
    
    static void h2v_get_elem_impl
    (TH3F* th3, vector3* out, int x1, int x2, int ny, int nz);
    static void v2v_multi_elem_impl
    (const vector3& in, vector3* out, int x1, int x2, int ny, int nz);
    static void v2v_add_elem_impl
    (const vector3& in, vector3* out, int x1, int x2, int ny, int nz);

    static void v2v_modify_element
    (const vector3& in, vector3* out, int x1, int x2, int ny, int nz,
     std::function<void(const float, float*)> func);

};

#endif

