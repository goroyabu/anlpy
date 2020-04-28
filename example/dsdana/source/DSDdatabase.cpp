/**
   @file   DSDdatabase.cpp
   @date   2020/04/11
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#include "DSDdatabase.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <algorithm>
using std::find;

#include <iomanip>
using std::setw;
using std::left;
using std::right;

#include <fstream>
#include <sstream>

#include <bnk.hpp>
#include <evs.hpp>

int DSDdatabase::data_element::print()
{
    std::cout << std::left;
    std::cout << "asicid = " << std::setw(3) << this->asicid;
    std::cout << ", asicch  = " << std::setw(3) << this->asicch << std::endl;
    std::cout << "detid  = " << std::setw(3) << this->detid;
    std::cout << ", stripid = " << std::setw(5) << this->stripid << std::endl;
    std::string side = "Y"; if(this->is_xside) side = "X";
    std::cout << "side = " << std::setw(3) << side << std::endl;
    std::cout << "material = " << std::setw(2) << this->material << std::endl;
    std::cout << std::right;
    std::cout << "pos   = (" << std::setw(7) << this->posx;
    std::cout << ", " << std::setw(7) << this->posy;
    std::cout << ", " << std::setw(7) << this->posz << ")" << std::endl;
    std::cout << "width = (" << std::setw(7) << this->widthx;
    std::cout << ", " << std::setw(7) << this->widthy;
    std::cout << ", " << std::setw(7) << this->widthz << ")" << std::endl;
    std::cout << std::left;
    std::string bad = "false"; if ( this->is_badch ) bad = "true";
    std::cout << "badch = " << std::setw(5) << bad << std::endl;
    std::cout << "ethre = " << std::setw(5) << this->ethre << std::endl;
    std::cout << "calparam : " << std::endl;
    for(int i=0; i<(int)calparam.size(); ++i) std::cout << "p" << i << " = " << calparam[i] << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    return 0;
}

DSDdatabase::DSDdatabase()
    : anl::VANL_Module("DSDdatabase", "0.1")
{
    /** Parameters can be modified via a method 'SetParameter' in Python **/
    define_parameter<std::string>("file_name", "database.txt");
}
DSDdatabase::~DSDdatabase()
{
}

int DSDdatabase::mod_bgnrun()
{
    auto file_name = get_parameter<std::string>("file_name");
    std::ifstream ifs( file_name );
    if( ifs.fail() ){
        std::cout << file_name << " is not found." << std::endl;
        return anl::ANL_NG;
    }

    int index = 0;
    std::string buf;
    while( std::getline(ifs, buf) ){

        std::istringstream ss(buf);
        int asicid, asicch, detid, stripid, material, badch;
        float posx, posy, posz, widthx, widthy, widthz;
        float ethre;
        std::vector<float> calparam;

        if( ss.str().substr(0, 1)=="#" ) continue;

        ss >> asicid >> asicch >> detid >> stripid >> material;
        ss >> posx >> posy >> posz >> widthx >> widthy >> widthz;
        ss >> badch >> ethre;

        double param;
        while( ss >> param ){
            calparam.emplace_back(param);
        }

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

        for ( auto p : calparam ) temp->calparam.emplace_back(p);

        database.emplace_back(temp);
        // temp->print();

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
    
    /** This function is called at the begging of the run **/
    // cout << mod_name2() << "::mod_bgnrun()";
    // cout << " -> Hello! :D" << endl;

    /** Example of operations described here **/

    /** Opening files **/
    // std::string input_file = get_parameter<std::string>( "input.txt" );
    // ifs = std::ifstream( input_file );

    /** Definition of data buffers (BNK) **/
    // bnk::define<std::string>( "time_stamp" );
    // bnk::define<double>( "energy_deposit" );

    /** Definition of event flags (EVS) **/
    // evs::define( "multiple_signals_event" );

    return anl::ANL_OK;
}

int DSDdatabase::mod_ana()
{
    /** This function is called at every events **/
    // cout << mod_name2() << "::mod_ana()" << endl;

    /** 1. Get values from data buffers (BNK) **/
    /**    Previous ANL modules often define buffers used here **/
    // int n_signals = bnk::define<int>( "n_signals" );
    // double energy_deposit = bnk::define<double>( "adc_value" ) * 0.25;

    /** 2. Use flags set by previous modules (EVS) **/
    // if ( evs::get("bad_signal_event") ) return ANL_SKIP;

    /** 3. Put values to data buffers (BNK) **/
    /**    This module often defines buffers used here **/
    // bnk::put<std::string>("time_stamp", "2020-04-03 21:12:15");
    // bnk::put<double>("energy_deposit", energy_deposit);

    /** 4. Set event flags in order for follwing modules to skip the same analysis **/
    // if ( n_signals > 1 || energy_deposit >= 5.0 ) evs::set( "multiple_signals_event" );

    return anl::ANL_OK;
}

int DSDdatabase::mod_endrun()
{
    /** This function is called at the end of the run **/
    cout << mod_name2() << "::mod_endrun()";
    cout << " -> Good Bye. :)" << endl;

    /** Example of operations described here **/

    /** Closing files **/
    // ifs.close(); // std::ifstream

    return anl::ANL_OK;
}

std::vector<int> DSDdatabase::GetListOfAsicids()
{
    std::vector<int> out;
    for ( auto data : database ) {
	auto elem = data->asicid;
	if ( exist( out, elem ) ) continue;
	out.emplace_back( elem );
    }
    return out;
}
int DSDdatabase::GetNasics()
{
    // return GetListOfAsicids().size();
    return list_of_asicid.size();
}
bool DSDdatabase::ExistAsicid(const int asicid)
{
    // return exist( GetListOfAsicids(), asicid );
    return exist( list_of_asicid, asicid );
}
std::vector<int> DSDdatabase::GetListOfDetids()
{
    std::vector<int> out;
    for ( auto data : database ) {
	auto elem = data->detid;
	if ( exist( out, elem ) ) continue;
	out.emplace_back( elem );
    }
    return out;
}
int DSDdatabase::GetNdetectors()
{
    // return GetListOfDetids().size();
    return list_of_detid.size();
}
bool DSDdatabase::ExistDetid(const int detid)
{
    // return exist( GetListOfDetids(), detid );
    return exist( list_of_detid, detid );
}
std::vector<int> DSDdatabase::GetListOfStrips()
{
    std::vector<int> out;
    for ( auto data : database ) {
	auto elem = data->stripid;
	if ( exist( out, elem ) ) continue;
	out.emplace_back( elem );
    }
    return out;
}
int DSDdatabase::GetNstrips()
{
    // return GetListOfStrips().size();
    return list_of_stripid.size();
}
bool DSDdatabase::ExistStrip(const int stripid)
{
    // return exist( GetListOfStrips(), stripid );
    return exist( list_of_stripid, stripid );
}
int DSDdatabase::GetNxside()
{
    int n = 0;
    for ( auto elem : database ) if ( elem->is_xside ) ++n;
    return n;
}

std::pair<int, int> DSDdatabase::FindStrip(const int asicid, const int asicch)
{
    auto pair = std::make_pair(asicid, asicch);
    if( strip_map.find(pair) == strip_map.end() ) return std::make_pair(-1, -1);
    return strip_map[pair];
}
int DSDdatabase::GetIndex(const int stripid)
{
    if ( !ExistStrip(stripid) ) return -1;
    return stripid_to_index_map[stripid];
}
DSDdatabase::data_element* DSDdatabase::GetDataElement(const int index)
{
    if ( index < 0 ) return new data_element();
    return database[ index ];
}

int DSDdatabase::GetAsicid(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->asicid;
}
int DSDdatabase::GetAsicch(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->asicch;
}
int DSDdatabase::GetDetid(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->detid;
}
int DSDdatabase::GetMaterial(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->material;
}
float DSDdatabase::GetPosx(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->posx;
}
float DSDdatabase::GetPosy(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->posy;
}
float DSDdatabase::GetPosz(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->posz;
}
float DSDdatabase::GetWidthx(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->widthx;
}
float DSDdatabase::GetWidthy(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->widthy;
}
float DSDdatabase::GetWidthz(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->widthz;
}
float DSDdatabase::GetEthre(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->ethre;
}
bool DSDdatabase::IsBadch(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->badch;
}
bool DSDdatabase::IsXside(const int stripid)
{
    return GetDataElement( GetIndex(stripid) )->is_xside;
}
float DSDdatabase::GetEPI(const int stripid, const float pha)
{
    return GetDataElement( GetIndex(stripid) )->epi( pha );
}
int DSDdatabase::GetDetectorPosz(const int detid)
{
    for ( auto data : database )
	if ( data->detid == detid ) return data->posz;	
    return 0.0;
}
int DSDdatabase::GetDetectorWidthz(const int detid)
{
    for ( auto data : database ) 
	if ( data->detid == detid ) return data->widthz;
    return 0.0;
}
