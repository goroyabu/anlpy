/**
   @file DSDdatabaseSpline.cpp
   @date 2020/05/06 
   @author Goro Yabu
**/

#include "DSDdatabaseSpline.hpp"

#include <TFile.h>

#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
#include <sstream>

#include <bnk.hpp>

DSDdatabaseSpline::DSDdatabaseSpline()
//: anl::VANL_Module("DSDdatabaseSpline", "1.0")
    :DSDdatabase()
{
    // define_parameter<std::string>("file_name", "database.txt");

    //set_parameter<std::string>("module_name", "DSDdatabaseSpline");
    set_parameter<std::string>("module_version", "Spline1.0");
    set_parameter<std::string>("copyid", "Spline");
    define_parameter<std::string>("root_file", "db_spline.root");
    define_parameter<int>("verbose_level", 0);
}

DSDdatabaseSpline::~DSDdatabaseSpline()
{
}

int DSDdatabaseSpline::mod_bgnrun()
{
    auto file_name = get_parameter<std::string>("file_name");
    std::ifstream ifs( file_name );
    if( ifs.fail() ){
        std::cout << file_name << " is not found." << std::endl;
        return anl::ANL_NG;
    }
    cout << file_name << " is opened." << endl;
    
    auto root_file_name = get_parameter<std::string>("root_file");
    auto root_file = new TFile( root_file_name.c_str() );
    if ( !root_file || root_file->IsZombie() ) {
	cout << "Opening " << file_name << " is failed." << std::endl;
        return anl::ANL_NG;
    }
    cout << root_file_name << " is opened." << endl;
    
    int index = 0;
    std::string buf;
    while( std::getline(ifs, buf) ){

        std::istringstream ss(buf);
        int asicid, asicch, detid, stripid, material, badch;
        float posx, posy, posz, widthx, widthy, widthz;
        float ethre;
        //std::vector<float> calparam;
	std::string calname;

        if( ss.str().substr(0, 1)=="#" ) continue;

        ss >> asicid >> asicch >> detid >> stripid >> material;
        ss >> posx >> posy >> posz >> widthx >> widthy >> widthz;
        ss >> badch >> ethre;

	ss >> calname;
        // double param;
        // while( ss >> param ){
        //     calparam.emplace_back(param);
        // }
	
        if( ExistStrip(stripid) ){
            std::cout << "Error : StripID " << stripid << " is duplicated.";
            return anl::ANL_NG;
        }
        // m_stripid_list.emplace_back(stripid);
	
        strip_map[ std::make_pair(asicid, asicch) ] = std::make_pair(detid, stripid);
	detid_stripid_to_index_map[ std::make_pair(detid, stripid) ] = index;
	// cout << index << " " << detid << "," << stripid << endl;

	// if( !ExistAsicid(asicid) ) m_asicid_list.emplace_back(asicid);
	// if( !ExistDetid(detid) ) {
        //     mDetIDList.emplace_back(detid);
        //     m_detector_posz_list.emplace_back(posz);
        //     m_detector_widthz_list.emplace_back(widthz);
        // }

	stripid_to_index_map[stripid] = index;
	
        data_element* temp = new data_element();
        temp->asicid = asicid;
        temp->asicch = asicch;
        temp->detid = detid;
        temp->stripid = stripid;
	temp->material = material;
        temp->posx = posx;
        temp->posy = posy;
        temp->posz = posz;
        temp->widthx = widthx;
        temp->widthy = widthy;
        temp->widthz = widthz;
        temp->badch = badch;
        temp->ethre = ethre;
	if ( badch==1 ) temp->is_badch = true;
	else temp->is_badch = false;
        if ( widthy<0 ) temp->is_xside = true;
        else temp->is_xside = false;

        //for ( auto p : calparam ) temp->calparam.emplace_back(p);
	auto spl = (TSpline3*)root_file->Get( calname.c_str() );
	if ( !spl || (TString)spl->ClassName()!="TSpline3" ) {
	    cout << "TSpline3 " << calname << " is not found." << endl;
	    return anl::ANL_NG;
	}
	gain_curve_spline3[ stripid ] = spl;

        database.emplace_back(temp);
        if ( get_parameter<int>("verbose_level")>0 ) temp->print();

	if( asicid>maxinfo.asicid ) maxinfo.asicid = asicid;
	if( detid>maxinfo.detid ) maxinfo.detid = detid;
        if( stripid>maxinfo.stripid ) maxinfo.stripid = stripid;
        if( posx>maxinfo.posx ) maxinfo.posx = posx;
        if( posy>maxinfo.posy ) maxinfo.posy = posy;
        if( posz>maxinfo.posz ) maxinfo.posz = posz;
	if( widthx>maxinfo.widthx ) maxinfo.widthx = widthx;
	if( widthy>maxinfo.widthy ) maxinfo.widthy = widthy;
        if( asicid<mininfo.asicid ) mininfo.asicid = asicid;
        if( detid<mininfo.detid ) mininfo.detid = detid;
        if( stripid<mininfo.stripid ) mininfo.stripid = stripid;
        if( posx<mininfo.posx ) mininfo.posx = posx;
        if( posy<mininfo.posy ) mininfo.posy = posy;
        if( posz<mininfo.posz ) mininfo.posz = posz;

        ++index;
    }

    list_of_asicid  = this->GetListOfAsicids();
    list_of_detid   = this->GetListOfDetids();
    list_of_stripid = this->GetListOfStrips();
    
    auto nstrips_1side = (int)( (int)list_of_stripid.size()/(int)list_of_detid.size()/2 );
    auto posx_lower_end = mininfo.posx - maxinfo.widthx*0.5;
    auto posx_upper_end = maxinfo.posx + maxinfo.widthx*0.5;
    auto posy_lower_end = mininfo.posy - maxinfo.widthy*0.5;
    auto posy_upper_end = maxinfo.posy + maxinfo.widthy*0.5;

    bnk::define<int>( "DSDinfo_nstrips_x" );
    bnk::define<int>( "DSDinfo_nstrips_y" );
    bnk::define<double>( "DSDinfo_xmin" );
    bnk::define<double>( "DSDinfo_xmax" );
    bnk::define<double>( "DSDinfo_ymin" );
    bnk::define<double>( "DSDinfo_ymax" );

    bnk::put<int>( "DSDinfo_nstrips_x", nstrips_1side );
    bnk::put<int>( "DSDinfo_nstrips_y", nstrips_1side );
    bnk::put<double>( "DSDinfo_xmin", posx_lower_end );
    bnk::put<double>( "DSDinfo_xmax", posx_upper_end );
    bnk::put<double>( "DSDinfo_ymin", posy_lower_end );
    bnk::put<double>( "DSDinfo_ymax", posy_upper_end );

    std::cout << "DSDinfo_nstrips_x : " << nstrips_1side << std::endl;	
    std::cout << "DSDinfo_nstrips_y : " << nstrips_1side << std::endl;	
    std::cout << "DSDinfo_xmin      : " << posx_lower_end << std::endl;
    std::cout << "DSDinfo_xmax      : " << posx_upper_end << std::endl;
    std::cout << "DSDinfo_ymin      : " << posy_lower_end << std::endl;
    std::cout << "DSDinfo_ymax      : " << posy_upper_end << std::endl;
    
    root_file->Close();
    ifs.close();

    return anl::ANL_OK;
}

float DSDdatabaseSpline::GetEPI(const int stripid, const float pha)
{
    if ( !ExistStrip(stripid) ) return 0.0;
    return gain_curve_spline3[stripid]->Eval(pha);
}
