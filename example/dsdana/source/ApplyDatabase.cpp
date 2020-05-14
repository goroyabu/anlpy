/**
   @file ApplyDatabase.cpp
   @author Goro Yabu
   @date 2018/11/27
   @date 2019/04/16 'ApplyDatabase' v1.1 Change this module name to ApplyDatabase
   @date 2019/06/20 'ApplyDatabase' v1.2 Fix bugs.
   @date 2019/07/23 'ApplyDatabase' v2.0 Change 'ReadDatabase' -> 'ReadDatabaseText'
   @date 2019/08/05 'ApplyDatabase' v2.1 Adapt to ReadDatabaseText v1.1
   @date 2020/03/23 'ApplyDatabase' v1.0 Divide 2 modules 'ApplyDatabase' and 'ApplyDatabase'
   @date 2020/05/10 Change for ANLpy. Enabled to modify parameters.
   @version 1.0
**/

#include "ApplyDatabase.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <bnk.hpp>
#include <evs.hpp>

#include <ANLmanager.hpp>

#include "TFile.h"

ApplyDatabase::ApplyDatabase()
    : VANL_Module("ApplyDatabase", "1.0"),
      mDatabase(nullptr), mRandom(nullptr)
      // m_histall(nullptr), m_spectall(nullptr),
      // m_multi_hist(nullptr)
{
    m_asic_bgn = 0; m_asic_end = 0;
}
int ApplyDatabase::mod_bgnrun()
{
    // using namespace bnk;
    // using namespace evs;

    // status = anl::ANL_OK;

    cout << "=============================" << endl;
    cout << "  ApplyDatabase::mod_bgnrun  " << endl;
    cout << "=============================" << endl;
    
    mRandom = new TRandom3();
    mRandom->SetSeed( time(NULL) );

    //auto manager = new anl::ANLmanager();
    auto manager = anl::ANLmanager::Instance();
    
    // manager->show_analysis();
    // cout << "showwww" << endl;
    // auto mm = manager->get_module("DSDdatabase");
    // if (!mm) cout << "errororor" << endl;
    // else cout << "name is " << mm->mod_name() << endl;
    
    mDatabase = (DSDdatabase*)manager.get_module("DSDdatabase");
    if ( !mDatabase || mDatabase->mod_name()!="DSDdatabase") {
	cout << "***Error*** DSDdatabase does not exist." << endl;
	return anl::ANL_NG;
    }
    // cout << mDatabase->mod_name() << endl;

    auto list_of_detid = mDatabase->GetListOfDetids();
    for ( auto detid : list_of_detid ) {
	TString hname = Form("h2_lv1_multi_%02d", detid);
	TString htitle = Form("Number of signals on %02d detector;X;Y", detid);
	auto h2 = new TH2D( hname, htitle,
			    100, -0.5, 99.5, 100, -0.5, 99.5 );
	list_of_h2_multi[detid] = h2;
    }
    
    m_nasic = mDatabase->GetNasics();
    m_asic_bgn = mDatabase->GetAsicidMin();
    m_asic_end = mDatabase->GetAsicidMax();
    mvec_hitnum.resize(m_nasic);
    mvec_cmn.resize(m_nasic);
    mvec_adc.resize(m_nasic);
    mvec_index.resize(m_nasic);

    int nxside = mDatabase->GetNxside();
    int nyside = mDatabase->GetNyside();
    m_detid_x_lv1.reserve(nxside);
    m_detid_y_lv1.reserve(nyside);
    m_stripid_x_lv1.reserve(nxside);
    m_stripid_y_lv1.reserve(nyside);
    m_epi_x_lv1.reserve(nxside);
    m_epi_y_lv1.reserve(nyside);

    bnk::define<int>("nsignal_x_lv1", 1);
    bnk::define<int>("nsignal_y_lv1", 1);
    bnk::define<int>("detid_x_lv1", nxside);    
    bnk::define<int>("detid_y_lv1", nyside);
    bnk::define<int>("stripid_x_lv1", nxside);
    bnk::define<int>("stripid_y_lv1", nyside);
    bnk::define<int>("adc_cmn_x_lv2", nxside);
    bnk::define<int>("adc_cmn_y_lv2", nyside);
    bnk::define<float>("epi_x_lv1", nxside);
    bnk::define<float>("epi_y_lv1", nyside);

    bnk::setkeytosize<int>("detid_x_lv1", "nsignal_x_lv1");
    bnk::setkeytosize<int>("stripid_x_lv1", "nsignal_x_lv1");
    bnk::setkeytosize<int>("adc_cmn_x_lv2", "nsignal_x_lv1");
    bnk::setkeytosize<float>("epi_x_lv1", "nsignal_x_lv1");

    bnk::setkeytosize<int>("detid_y_lv1", "nsignal_y_lv1");
    bnk::setkeytosize<int>("stripid_y_lv1", "nsignal_y_lv1");
    bnk::setkeytosize<int>("adc_cmn_y_lv2", "nsignal_y_lv1");
    bnk::setkeytosize<float>("epi_y_lv1", "nsignal_y_lv1");
    
    evs::define("nsignal_x_lv1==0 && nsignal_y_lv1==0");
    evs::define("nsignal_x_lv1==1 && nsignal_y_lv1==0");
    evs::define("nsignal_x_lv1==0 && nsignal_y_lv1==1");
    evs::define("nsignal_x_lv1==1 && nsignal_y_lv1==1");
    evs::define("nsignal_x_lv1==2 && nsignal_y_lv1==0");
    evs::define("nsignal_x_lv1==0 && nsignal_y_lv1==2");
    evs::define("nsignal_x_lv1==2 && nsignal_y_lv1==1");
    evs::define("nsignal_x_lv1==1 && nsignal_y_lv1==2");
    evs::define("nsignal_x_lv1==2 && nsignal_y_lv1==2");
    evs::define("nsignal_x_lv1>=3 || nsignal_y_lv1>=3");

    // status = anl::ANL_OK;

    //his();
    
    return anl::ANL_OK;
}
// int ApplyDatabase::his()
// {
//     m_histall = new TH2D("histall_lv1", "histall;stripid;pha",
// 			 mDatabase->GetNstrips(),
// 			 mDatabase->GetStripidMin()-0.5,
// 			 mDatabase->GetStripidMax()-0.5,
// 			 1000, -10.5, 989.5);
    
//     m_spectall = new TH2D("spectall_lv1", "spectall;stripid;epi",
// 			  mDatabase->GetNstrips(),
// 			  mDatabase->GetStripidMin()-0.5,
// 			  mDatabase->GetStripidMax()-0.5,
// 			  10000, -10, 990);
    
//     m_multi_hist = new TH2D("multipli_lv1",
// 			    "multiplicity lv1;nsignal_x_lv1;nsignal_y_lv1;",
// 			    50, -0.5, 49.5, 50, -0.5, 49.5);
    
//     // status = anlcross::ANL_OK;
//     return anl::ANL_OK;
// }
// int ApplyDatabase::mod_com()
// {
//     //random mode
//     // status = anlcross::ANL_OK;
//     return anl::
// }
int ApplyDatabase::mod_ana()
{
    // using namespace evs;
    
    // status = anlcross::ANL_OK;
    // if( status == anlcross::ANL_OK ){
    // cout << "ana" << endl;
    
    this->clearVectorAll();
    this->bnkGetAll();
    
    // return anl::ANL_OK;
    
    for ( int asicid = 0; asicid < m_nasic; asicid++ ) {
	// break;

	int hitnum = mvec_hitnum[asicid];
	
	for(int isignal = 0; isignal < hitnum; isignal++){
	    
	    if ( mvec_index[asicid].size()<=isignal ) continue;
	    
	    int asicch = mvec_index[asicid][isignal];
	    // float pha = mvec_adc[asicid][isignal] - mvec_cmn[asicid] + getRandom();
	    float pha = mvec_adc[asicid][isignal] - mvec_cmn[asicid];
	    
	    auto [detid, stripid] = mDatabase->FindStrip(asicid, asicch);

	    if( detid == -1 && stripid == -1 ) continue;		
	    if( mDatabase->IsBadch(stripid) ) continue;
	    
	    float epi = mDatabase->GetEPI(stripid, pha+getRandom());

	    // m_histall->Fill(stripid, pha);
	    // m_spectall->Fill(stripid, epi);
	    
	    if ( mDatabase->IsXside(stripid) ) {
		m_detid_x_lv1.emplace_back(detid);
		m_stripid_x_lv1.emplace_back(stripid);
		m_adc_cmn_x_lv1.emplace_back(pha);
		m_epi_x_lv1.emplace_back(epi);
		m_nsignal_x_lv1++;
	    }
	    else {
		m_detid_y_lv1.emplace_back(detid);
		m_stripid_y_lv1.emplace_back(stripid);
		m_adc_cmn_y_lv1.emplace_back(pha);
		m_epi_y_lv1.emplace_back(epi);
		m_nsignal_y_lv1++;
	    }
	    
	}
    }
    
    // m_multi_hist->Fill(m_nsignal_x_lv1, m_nsignal_y_lv1);
    
    if(m_nsignal_x_lv1==0 && m_nsignal_y_lv1==0)
    	evs::set("nsignal_x_lv1==0 && nsignal_y_lv1==0");
    else if(m_nsignal_x_lv1==1 && m_nsignal_y_lv1==0)
    	evs::set("nsignal_x_lv1==1 && nsignal_y_lv1==0");
    else if(m_nsignal_x_lv1==0 && m_nsignal_y_lv1==1)
    	evs::set("nsignal_x_lv1==0 && nsignal_y_lv1==1");
    else if(m_nsignal_x_lv1==1 && m_nsignal_y_lv1==1)
    	evs::set("nsignal_x_lv1==1 && nsignal_y_lv1==1");
    else if(m_nsignal_x_lv1==2 && m_nsignal_y_lv1==0)
    	evs::set("nsignal_x_lv1==2 && nsignal_y_lv1==0");
    else if(m_nsignal_x_lv1==0 && m_nsignal_y_lv1==2)
    	evs::set("nsignal_x_lv1==0 && nsignal_y_lv1==2");
    else if(m_nsignal_x_lv1==2 && m_nsignal_y_lv1==1)
    	evs::set("nsignal_x_lv1==2 && nsignal_y_lv1==1");
    else if(m_nsignal_x_lv1==1 && m_nsignal_y_lv1==2)
    	evs::set("nsignal_x_lv1==1 && nsignal_y_lv1==2");
    else if(m_nsignal_x_lv1==2 && m_nsignal_y_lv1==2)
    	evs::set("nsignal_x_lv1==2 && nsignal_y_lv1==2");
    else
    	evs::set("nsignal_x_lv1>=3 || nsignal_y_lv1>=3");

    // cout << "ana end" << endl;
    
    if( this->bnkPutAll()!=anl::ANL_OK )
	return anl::ANL_SKIP;
    // }
    return anl::ANL_OK;
}
int ApplyDatabase::mod_endrun()
{
    // if ( !gDirectory->InheritsFrom("TFile") )
    // 	return anl::ANL_OK;
    // m_histall->Write();
    // m_spectall->Write();
    // m_multi_hist->Write();
    // status = anlcross::ANL_OK;
    return anl::ANL_OK;
}
// void ApplyDatabase::mod_exit(int &status)
// {
//     status = anlcross::ANL_OK;
// }

// int ApplyDatabase::bnkDefAll()
// {
//     // using namespace bnk;
//     // using namespace std;

//     // m_nasic=0;
//     // while(true){
//     // 	if( bnk_is_def("hitnum"+to_string(m_nasic))==BNK_NG ) break;
//     // 	++m_nasic;
//     // }
//     // mvec_hitnum.resize(m_nasic);
//     // mvec_cmn.resize(m_nasic);
//     // mvec_adc.resize(m_nasic);
//     // mvec_index.resize(m_nasic);

//     // m_detid_x_lv1.reserve(m_nasic*64);
//     // m_detid_y_lv1.reserve(m_nasic*64);
//     // m_stripid_x_lv1.reserve(m_nasic*64);
//     // m_stripid_y_lv1.reserve(m_nasic*64);
//     // m_epi_x_lv1.reserve(m_nasic*64);
//     // m_epi_y_lv1.reserve(m_nasic*64);

//     // bnk_def<int>("nsignal_x_lv1", 1);
//     // bnk_def<int>("nsignal_y_lv1", 1);
//     // bnk_def<int>("detid_x_lv1", m_nasic*64);
//     // bnk_def<int>("detid_y_lv1", m_nasic*64);
//     // bnk_def<int>("stripid_x_lv1", m_nasic*64);
//     // bnk_def<int>("stripid_y_lv1", m_nasic*64);
//     // bnk_def<float>("epi_x_lv1", m_nasic*64);
//     // bnk_def<float>("epi_y_lv1", m_nasic*64);
    
//     return anlcross::ANL_OK;
// }

int ApplyDatabase::bnkGetAll()
{
    // using namespace bnk;

    for(int i=0; i<m_nasic; ++i){
	
	mvec_hitnum[i] = bnk::get<unsigned short>( "hitnum"+std::to_string(i) );
	mvec_cmn[i]    = bnk::get<unsigned short>( "cmn"+std::to_string(i) );
	
	// auto ad = bnk::get<unsigned short>( "adc"+std::to_string(i) );
	mvec_adc[i]    = bnk::getv<unsigned short>( "adc"+std::to_string(i) );
	mvec_index[i]  = bnk::getv<unsigned short>( "index"+std::to_string(i) );

	// cout << "hitnum=" << mvec_hitnum[i] << ", n_adc=" << mvec_adc[i].size();
	// cout << ", n_index=" << mvec_index[i].size() << "." << endl;
	// bnk::get<unsigned short>
	//     ("adc"+std::to_string(i), &mvec_adc[i], 0, mvec_hitnum[i]);
	// bnk::get<unsigned short>
	//     ("index"+std::to_string(i), &mvec_index[i], 0, mvec_hitnum[i]);
    }    

    return anl::ANL_OK;
}
int ApplyDatabase::bnkPutAll()
{
    // using namespace bnk;

    // return anl::ANL_OK;

    // const static int n = 100;
    // const static double v = 5.0;
    // m_nsignal_x_lv1 = n; m_nsignal_y_lv1 = n;
    // for ( int i=0; i<n; ++i ) {
    // 	m_detid_x_lv1.emplace_back(v);
    // 	m_detid_y_lv1.emplace_back(v);
    // 	m_stripid_x_lv1.emplace_back(v);
    // 	m_stripid_y_lv1.emplace_back(v);
    // 	m_epi_x_lv1.emplace_back(v);
    // 	m_epi_y_lv1.emplace_back(v);
    // }
   
    bnk::put<int>("nsignal_x_lv1", m_nsignal_x_lv1);
    bnk::put<int>("nsignal_y_lv1", m_nsignal_y_lv1);
    bnk::put<int>("detid_x_lv1", m_detid_x_lv1, 0, m_nsignal_x_lv1);
    bnk::put<int>("detid_y_lv1", m_detid_y_lv1, 0, m_nsignal_y_lv1);
    bnk::put<int>("stripid_x_lv1", m_stripid_x_lv1, 0, m_nsignal_x_lv1);
    bnk::put<int>("stripid_y_lv1", m_stripid_y_lv1, 0, m_nsignal_y_lv1);
    bnk::put<int>("adc_cmn_x_lv2", m_adc_cmn_x_lv1, 0, m_nsignal_x_lv1);
    bnk::put<int>("adc_cmn_y_lv2", m_adc_cmn_y_lv1, 0, m_nsignal_y_lv1);
    bnk::put<float>("epi_x_lv1", m_epi_x_lv1, 0, m_nsignal_x_lv1);
    bnk::put<float>("epi_y_lv1", m_epi_y_lv1, 0, m_nsignal_y_lv1);
    
    return anl::ANL_OK;
}
int ApplyDatabase::clearVectorAll()
{    
    mvec_hitnum.clear();  mvec_cmn.clear();
    
    for(int i=0; i<m_nasic; ++i){
	mvec_adc[i].clear();  mvec_index[i].clear();
    }
    
    m_nsignal_x_lv1 = 0;      m_detid_x_lv1.clear();
    m_stripid_x_lv1.clear();  m_epi_x_lv1.clear();
    m_adc_cmn_x_lv1.clear();
    
    m_nsignal_y_lv1 = 0;      m_detid_y_lv1.clear();
    m_stripid_y_lv1.clear();  m_epi_y_lv1.clear();
    m_adc_cmn_y_lv1.clear();
    
    return anl::ANL_OK;
}
int ApplyDatabase::isBadch(int asicid, int asicch)
{
    bool badch = mDatabase->IsBadch( mDatabase->GetStripid(asicid, asicch) );
    if( badch ) return anl::ANL_YES;
    return anl::ANL_NO;
}
float ApplyDatabase::getRandom(){
    return mRandom->Uniform(-0.5, 0.5);
}
