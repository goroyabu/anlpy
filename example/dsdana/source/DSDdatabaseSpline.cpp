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

DSDdatabaseSpline::DSDdatabaseSpline()
//: anl::VANL_Module("DSDdatabaseSpline", "1.0")
    :DSDdatabase()
{
    // define_parameter<std::string>("file_name", "database.txt");

    //set_parameter<std::string>("module_name", "DSDdatabaseSpline");
    set_parameter<std::string>("module_version", "Spline1.0");
    set_parameter<std::string>("copyid", "Spline");
    define_parameter<std::string>("root_file", "db_spline.root");
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
        temp->print();

	if( asicid>maxinfo.asicid ) maxinfo.asicid = asicid;
	if( detid>maxinfo.detid ) maxinfo.detid = detid;
        if( stripid>maxinfo.stripid ) maxinfo.stripid = stripid;
        if( posx>maxinfo.posx ) maxinfo.posx = posx;
        if( posy>maxinfo.posy ) maxinfo.posy = posy;
        if( posz>maxinfo.posz ) maxinfo.posz = posz;
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
    
    root_file->Close();
    ifs.close();

    return anl::ANL_OK;
}

float DSDdatabaseSpline::GetEPI(const int stripid, const float pha)
{
    if ( !ExistStrip(stripid) ) return 0.0;
    return gain_curve_spline3[stripid]->Eval(pha);
}
