/**
   @file MergeAdjacentText.hpp
   @author Goro Yabu
   @date 2018/12/04
   @date 2020/05/10 Change for ANLpy. Enabled to modify parameters.
   @version 1.0
**/

#include "MergeAdjacent.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <ANLmanager.hpp>
// using namespace anlcross;

#include <TDirectoryFile.h>
#include <TString.h>

// const int MergeAdjacent::materialid_si = 0;
// const int MergeAdjacent::materialid_cdte = 1;

MergeAdjacent::MergeAdjacent()
    : VANL_Module("MergeAdjacent", "1.0"),
      mDatabase(nullptr), m_histogram(nullptr)
{
    m_ndetector = 1;
    m_save_his = true;

    define_parameter<int>("enable_uniform_ethre_si", 0);
    define_parameter<int>("enable_uniform_ethre_cdte", 0);
    define_parameter<double>("uniform_ethre_si", 2.5);
    define_parameter<double>("uniform_ethre_cdte", 5.0);
}
int MergeAdjacent::mod_bgnrun()
{
    // using namespace evs;
    // status = ANL_OK;
    //std::cout << "MergeAdjacent::mod_init" << std::endl;
    m_histogram = new TH1D("hist_dist_epi","hist_dist_epi;pos;epi",100,-0.5,99.5);

    //auto manager = new anl::ANLmanager();
    auto manager = anl::ANLmanager::Instance();
    mDatabase = (DSDdatabase*)manager.get_module("DSDdatabase");
    if( !mDatabase || mDatabase->mod_name()!="DSDdatabase") {
	cout << "***Error*** DSDdatabase does not exist." << endl;
	return anl::ANL_NG;
    }
    m_detid_list = mDatabase->GetListOfDetids();

    static const int nbins_h2 = 128;
    static const double xmin_h2 = -0.5;
    static const double xmax_h2 = nbins_h2-0.5;
    
    for ( auto detid : m_detid_list ) {
	TString hname = Form("h2_multi_lv1_det%02d", detid);
	TString htitle = Form("Signals before Ecut on det %02d;N of X-signals;N of Y-signals", detid);
	auto h2 = new TH2D( hname, htitle, nbins_h2, xmin_h2, xmax_h2,
			    nbins_h2, xmin_h2, xmax_h2 );
	list_of_h2_multi_lv1[detid] = h2;
	nsignal_lv1_on_1det[detid] = std::make_pair(0,0);
    }    
    
    for ( auto detid : m_detid_list ) {
	TString hname = Form("h2_multi_lv2a_det%02d", detid);
	TString htitle = Form("Signals over Ethr on det %02d;N of X-signals;N of Y-signals", detid);
	auto h2 = new TH2D( hname, htitle, nbins_h2, xmin_h2, xmax_h2,
			    nbins_h2, xmin_h2, xmax_h2 );
	list_of_h2_multi_lv2a[detid] = h2;
    }    
    
    for ( auto detid : m_detid_list ) {
	TString hname = Form("h2_multi_lv2b_det%02d", detid);
	TString htitle = Form("Merged Signals on det %02d;N of merged X-signals;N of Y-signals", detid);
	auto h2 = new TH2D( hname, htitle, nbins_h2, xmin_h2, xmax_h2,
			    nbins_h2, xmin_h2, xmax_h2 );
	list_of_h2_multi_lv2b[detid] = h2;
    }
    
    auto status = this->bnkDefAll();
    if ( status!=anl::ANL_OK ) return status;

    evs::define("nsignal_x_lv2==1 && nsignal_y_lv2==1");
    evs::define("nsignal_x_lv2==1 && nsignal_y_lv2==2");
    evs::define("nsignal_x_lv2==2 && nsignal_y_lv2==1");
    evs::define("nsignal_x_lv2==2 && nsignal_y_lv2==2");
    evs::define("nsignal_x_lv2>=3 || nsignal_y_lv2>=3");
    evs::define("nsignal_x_lv2==1 && n_merged_strips_x_lv2==1");
    evs::define("nsignal_x_lv2==1 && n_merged_strips_x_lv2==2");
    evs::define("nsignal_y_lv2==1 && n_merged_strips_y_lv2==1");
    evs::define("nsignal_y_lv2==1 && n_merged_strips_y_lv2==2");

    //this->his();

    enable_uniform_ethre_si = get_parameter<int>("enable_uniform_ethre_si");
    enable_uniform_ethre_cdte = get_parameter<int>("enable_uniform_ethre_cdte");
    uniform_ethre_si = get_parameter<double>("uniform_ethre_si");
    uniform_ethre_cdte = get_parameter<double>("uniform_ethre_cdte");
    
    return anl::ANL_OK;
}
// int MergeAdjacent::his()
// {
//     // m_multipli_his = new TH2D("multipli_lv2",
//     // 			      "multiplicity lv2;nsignal_x_lv2;nsignal_y_lv2",
//     // 			      50, -0.5, 49.5, 50, -0.5, 49.5);
//     return anl::ANL_OK;
//     // status = ANL_OK;
// }
int MergeAdjacent::mod_ana()
{
    /* start ana */
    this->clearVectorAll();
    this->bnkGetAll();

    /* main analysis */
    auto status = this->extractSignalsOverThreshold();
    if ( status!=anl::ANL_OK ) return status;

    for(auto detid : m_detid_list)
	this->convertLv1toLv2(detid);
    
    /* end ana */
    this->bnkPutAll();
    return anl::ANL_OK;
}
int MergeAdjacent::mod_endrun()
{
    // m_multipli_his->Write();

    // if ( !gDirectory->IsWritable() )
    // 	return anl::ANL_OK;

    // auto dir = gDirectory->mkdir("his_MergeAdjacent");
    // if ( !dir ) return anl::ANL_OK;    
    
    // dir->cd();
    // for ( auto mp : list_of_h2_multi_lv1 ) {
    // 	mp.second->GetXaxis()->SetRangeUser(0, 10);
    // 	mp.second->GetYaxis()->SetRangeUser(0, 10);
    // 	mp.second->Write();
    // }
    // for ( auto mp : list_of_h2_multi_lv2a ) {
    // 	mp.second->GetXaxis()->SetRangeUser(0, 10);
    // 	mp.second->GetYaxis()->SetRangeUser(0, 10);	
    // 	mp.second->Write();
    // }
    // for ( auto mp : list_of_h2_multi_lv2b ) {
    // 	mp.second->GetXaxis()->SetRangeUser(0, 10);
    // 	mp.second->GetYaxis()->SetRangeUser(0, 10);	
    // 	mp.second->Write();
    // }

    // dir->cd("..");

    cout << "MergeAdjacent is closed." << endl;
    return anl::ANL_OK;
}
int MergeAdjacent::bnkDefAll()
{
    bnk::define<int>("nsignal_x_lv2", 1);
    bnk::define<int>("nsignal_y_lv2", 1);
    bnk::define<int>("detid_x_lv2", 128);
    bnk::define<int>("detid_y_lv2", 128);
    bnk::define<float>("epi_x_lv2", 128);
    bnk::define<float>("epi_y_lv2", 128);
    bnk::define<float>("pos_x_lv2", 128);
    bnk::define<float>("pos_y_lv2", 128);
    bnk::define<float>("width_x_lv2", 128);
    bnk::define<float>("width_y_lv2", 128);
    bnk::define<int>("n_merged_strips_x_lv2", 128);
    bnk::define<int>("n_merged_strips_y_lv2", 128);
    bnk::define<int>("n_lv1signal_x_lv2", 1);
    bnk::define<int>("n_lv1signal_y_lv2", 1);
    bnk::define<int>("lv1signal_id_x_lv2", 128);
    bnk::define<int>("lv1signal_id_y_lv2", 128);

    bnk::setkeytosize<int>("detid_x_lv2", "nsignal_x_lv2");
    bnk::setkeytosize<float>("epi_x_lv2", "nsignal_x_lv2");
    bnk::setkeytosize<float>("pos_x_lv2", "nsignal_x_lv2");
    bnk::setkeytosize<float>("width_x_lv2", "nsignal_x_lv2");
    bnk::setkeytosize<int>("n_merged_strips_x_lv2", "nsignal_x_lv2");

    bnk::setkeytosize<int>("detid_y_lv2", "nsignal_y_lv2");
    bnk::setkeytosize<float>("epi_y_lv2", "nsignal_y_lv2");
    bnk::setkeytosize<float>("pos_y_lv2", "nsignal_y_lv2");
    bnk::setkeytosize<float>("width_y_lv2", "nsignal_y_lv2");
    bnk::setkeytosize<int>("n_merged_strips_y_lv2", "nsignal_y_lv2");

    bnk::setkeytosize<int>("lv1signal_id_x_lv2", "n_lv1signal_x_lv2");
    bnk::setkeytosize<int>("lv1signal_id_y_lv2", "n_lv1signal_y_lv2");
    
    return anl::ANL_OK;
}
int MergeAdjacent::bnkGetAll()
{
    m_nsignal_x_lv1 = bnk::get<int>("nsignal_x_lv1");
    m_nsignal_y_lv1 = bnk::get<int>("nsignal_y_lv1");
    m_detid_x_lv1 = bnk::getv<int>  ("detid_x_lv1");
    m_detid_y_lv1 = bnk::getv<int>  ("detid_y_lv1");
    m_stripid_x_lv1 = bnk::getv<int>  ("stripid_x_lv1");
    m_stripid_y_lv1 = bnk::getv<int>  ("stripid_y_lv1");
    m_epi_x_lv1 = bnk::getv<float>("epi_x_lv1");
    m_epi_y_lv1 = bnk::getv<float>("epi_y_lv1");
    
    m_lv1data_x_list.clear();
    m_lv1data_y_list.clear();
    for(int i=0; i<m_nsignal_x_lv1; ++i){
	m_lv1data_x_list.emplace_back(lv1data(i, m_detid_x_lv1[i], m_stripid_x_lv1[i], m_epi_x_lv1[i]));
    }
    for(int i=0; i<m_nsignal_y_lv1; ++i){
	m_lv1data_y_list.emplace_back(lv1data(i, m_detid_y_lv1[i], m_stripid_y_lv1[i], m_epi_y_lv1[i]));
    }
    
    return anl::ANL_OK;
}
int MergeAdjacent::bnkPutAll()
{
    bnk::put<int>  ("nsignal_x_lv2", m_nsignal_x_lv2);
    bnk::put<int>  ("nsignal_y_lv2", m_nsignal_y_lv2);
    bnk::put<int>  ("detid_x_lv2",   m_detid_x_lv2, 0, m_nsignal_x_lv2);
    bnk::put<int>  ("detid_y_lv2",   m_detid_y_lv2, 0, m_nsignal_y_lv2);
    bnk::put<float>("epi_x_lv2",     m_epi_x_lv2,   0, m_nsignal_x_lv2);
    bnk::put<float>("epi_y_lv2",     m_epi_y_lv2,   0, m_nsignal_y_lv2);
    bnk::put<float>("pos_x_lv2",     m_pos_x_lv2,   0, m_nsignal_x_lv2);
    bnk::put<float>("pos_y_lv2",     m_pos_y_lv2,   0, m_nsignal_y_lv2);
    bnk::put<float>("width_x_lv2",   m_width_x_lv2, 0, m_nsignal_x_lv2);
    bnk::put<float>("width_y_lv2",   m_width_y_lv2, 0, m_nsignal_y_lv2);
    bnk::put<int>  ("n_merged_strips_x_lv2", m_n_merged_strips_x_lv2, 0, m_nsignal_x_lv2);
    bnk::put<int>  ("n_merged_strips_y_lv2", m_n_merged_strips_y_lv2, 0, m_nsignal_y_lv2);
    bnk::put<int>  ("n_lv1signal_x_lv2",     m_n_lv1signal_x_lv2);
    bnk::put<int>  ("n_lv1signal_y_lv2",     m_n_lv1signal_y_lv2);
    bnk::put<int>  ("lv1signal_id_x_lv2",    m_lv1signal_id_x_lv2,    0, m_n_lv1signal_x_lv2);
    bnk::put<int>  ("lv1signal_id_y_lv2",    m_lv1signal_id_y_lv2,    0, m_n_lv1signal_y_lv2);
    
    // m_multipli_his->Fill(m_nsignal_x_lv2, m_nsignal_y_lv2);
    
    if( m_nsignal_x_lv2 == 0 || m_nsignal_y_lv2 == 0 ) return anl::ANL_SKIP;
    if( m_nsignal_x_lv2 == 1 && m_nsignal_y_lv1 == 1 )
	evs::set("nsignal_x_lv2==1 && nsignal_y_lv2==1");
    else if( m_nsignal_x_lv2 == 1 && m_nsignal_y_lv1 == 2 )
	evs::set("nsignal_x_lv2==1 && nsignal_y_lv2==2");
    else if( m_nsignal_x_lv2 == 1 && m_nsignal_y_lv1 == 1 )
	evs::set("nsignal_x_lv2==2 && nsignal_y_lv2==1");
    else if( m_nsignal_x_lv2 == 2 && m_nsignal_y_lv1 == 2 )
	evs::set("nsignal_x_lv2==2 && nsignal_y_lv2==2");
    else
	evs::set("nsignal_x_lv2>=3 || nsignal_y_lv2>=3");

    if( m_nsignal_x_lv2 == 1 && m_n_merged_strips_x_lv2[0] == 1 )
	evs::set("nsignal_x_lv2==1 && n_merged_strips_x_lv2==1");
    else if( m_nsignal_x_lv2 == 1 && m_n_merged_strips_x_lv2[0] == 2 )
	evs::set("nsignal_x_lv2==1 && n_merged_strips_x_lv2==2");

    if( m_nsignal_y_lv2 == 1 && m_n_merged_strips_y_lv2[0] == 1 )
	evs::set("nsignal_y_lv2==1 && n_merged_strips_y_lv2==1");
    else if( m_nsignal_y_lv2 == 1 && m_n_merged_strips_y_lv2[0] == 2 )
	evs::set("nsignal_y_lv2==1 && n_merged_strips_y_lv2==2");    
    
    return anl::ANL_OK;
}
int MergeAdjacent::clearVectorAll()
{
    m_nsignal_x_lv2 = 0;
    m_nsignal_y_lv2 = 0;
    m_detid_x_lv2.clear();
    m_detid_y_lv2.clear();
    m_epi_x_lv2.clear();
    m_epi_y_lv2.clear();
    m_pos_x_lv2.clear();
    m_pos_y_lv2.clear();
    m_width_x_lv2.clear();
    m_width_y_lv2.clear();
    m_n_merged_strips_x_lv2.clear();
    m_n_merged_strips_y_lv2.clear();
    m_n_lv1signal_x_lv2 = 0;
    m_n_lv1signal_y_lv2 = 0;
    m_lv1signal_id_x_lv2.clear();
    m_lv1signal_id_y_lv2.clear();
    //m_lv1signal_id_x.clear();
    //m_lv1signal_id_y.clear();
    
    for ( auto mp : nsignal_lv1_on_1det ) {
    	//auto&& [ nx, ny ] = mp.second;
    	//nx = 0; ny = 0;
    	mp.second = std::make_pair(0,0);
    }
    return anl::ANL_OK;
}
void MergeAdjacent::extractOneDetector(int detid, std::vector<lv1data>& xdata, std::vector<lv1data>& ydata)
{
    xdata.clear();
    for(int i=0; i<m_nsignal_x_lv1; ++i){
        if( m_lv1data_x_list[i].detid != detid ) continue;
	xdata.emplace_back(m_lv1data_x_list[i]);	
    }
    ydata.clear();
    for(int i=0; i<m_nsignal_y_lv1; ++i){
        if( m_lv1data_y_list[i].detid != detid ) continue;
	ydata.emplace_back(m_lv1data_y_list[i]);	
    }
}
int MergeAdjacent::extractSignalsOverThreshold()
{
    int i = m_nsignal_x_lv1 - 1;
    for(; i>=0; --i){

	auto detid = mDatabase->GetDetid( m_lv1data_x_list[i].stripid );	
	nsignal_lv1_on_1det[detid].first++;
	
	//auto ethre = mDatabase->GetEthre( m_lv1data_x_list[i].stripid );
	auto mate = mDatabase->GetMaterial( m_lv1data_x_list[i].stripid );
	auto ethre = energy_threshold( mate, m_lv1data_x_list[i].stripid );
	// if ( m_lv1data_x_list[i].stripid>511 )
	//     cout << "mate,stripid,ethre=" << mate << " " << m_lv1data_x_list[i].stripid << " " << ethre << endl;

	if( m_lv1data_x_list[i].epi < ethre ){
	    m_lv1data_x_list.erase(m_lv1data_x_list.begin() + i);
	    --m_nsignal_x_lv1;
	}
    }

    i = m_nsignal_y_lv1 - 1;
    for(; i>=0; --i){

	auto detid = mDatabase->GetDetid( m_lv1data_y_list[i].stripid );
	nsignal_lv1_on_1det[detid].second++;
	
	//auto ethre = mDatabase->GetEthre( m_lv1data_y_list[i].stripid );
	auto mate = mDatabase->GetMaterial( m_lv1data_y_list[i].stripid );
	auto ethre = energy_threshold( mate, m_lv1data_y_list[i].stripid );
	if( m_lv1data_y_list[i].epi < ethre ){
	    m_lv1data_y_list.erase(m_lv1data_y_list.begin() + i);
	    --m_nsignal_y_lv1;
	}
    }
    
    for ( auto mp : nsignal_lv1_on_1det ) {
	auto [ detid, p ] = mp;
	list_of_h2_multi_lv1[ detid ]->Fill( p.first, p.second );
    }
    
    // if( m_nsignal_x_lv1 == 0 && m_nsignal_y_lv1 == 0 ) return anl::ANL_SKIP;
    return anl::ANL_OK;
}
int MergeAdjacent::convertLv1toLv2(int detid)
{
    std::vector<lv1data> xdata, ydata;
    this->extractOneDetector(detid, xdata, ydata);
    
    list_of_h2_multi_lv2a[detid]->Fill( xdata.size(), ydata.size() );
    
    const int maxloopx = (int)xdata.size();
    int nloopx = 0;
    while( (int)xdata.size() > 0 ){
	if( nloopx >= maxloopx ) break;
	this->fillHistogram(xdata);
	
        int nstrip = getNstrip( m_histogram );
        float epi = getEPI( m_histogram );
        float pos = getPos( m_histogram );
	float width = getWidth( m_histogram );
	m_detid_x_lv2.emplace_back(detid);
	m_epi_x_lv2.emplace_back(epi);
	m_pos_x_lv2.emplace_back(pos);
	m_width_x_lv2.emplace_back(width);
	m_n_merged_strips_x_lv2.emplace_back(nstrip);
	++m_nsignal_x_lv2;

	m_n_lv1signal_x_lv2 += nstrip;
	for( int lv1index : m_filled_index_list ) 
	    m_lv1signal_id_x_lv2.emplace_back(lv1index);

	m_histogram->Reset();
	m_stripid_list.clear();
	m_filled_index_list.clear();

	nloopx++;
    }
    
    const int maxloopy = (int)ydata.size();
    int nloopy = 0;
    while( (int)ydata.size() > 0 ){
	if( nloopy >=  maxloopy ) break;
	this->fillHistogram(ydata);

        int nstrip = getNstrip( m_histogram );
        float epi = getEPI( m_histogram );
        float pos = getPos( m_histogram );
	float width = getWidth( m_histogram );
	m_detid_y_lv2.emplace_back(detid);
	m_epi_y_lv2.emplace_back(epi);
        m_pos_y_lv2.emplace_back(pos);
        m_width_y_lv2.emplace_back(width);
        m_n_merged_strips_y_lv2.emplace_back(nstrip);
        ++m_nsignal_y_lv2;
        m_n_lv1signal_y_lv2 += nstrip;
	for( int lv1index : m_filled_index_list )
	    m_lv1signal_id_y_lv2.emplace_back(lv1index);

	m_histogram->Reset();
	m_stripid_list.clear();
	m_filled_index_list.clear();
	nloopy++;
    }

    list_of_h2_multi_lv2b[detid]->Fill( nloopx, nloopy );
    
    return anl::ANL_OK;
}
int MergeAdjacent::fillHistogram(std::vector<lv1data> &data_list)
{
    int nsignal = (int)data_list.size();
    int index, detid, stripid;
    float epi, pos, width;
    for(int i = nsignal-1; i>=0; --i){
	index = data_list[i].index;
	detid = data_list[i].detid;
	stripid = data_list[i].stripid;
	epi = data_list[i].epi;

	if( mDatabase->IsXside(stripid) ){
            pos = mDatabase->GetPosx( stripid );
            width = mDatabase->GetWidthx( stripid );
        }else{
            pos = mDatabase->GetPosy( stripid );
            width = mDatabase->GetWidthy( stripid );
        }
        if( m_histogram->GetEntries()==0 ){
            m_histogram->SetBins( 100, pos-(50.5*width), pos+(49.5*width) );
        }
        if( m_histogram->GetEntries()==0 || this->isAdjacent( stripid, m_stripid_list ) ){
            m_histogram->Fill(pos, epi);
            m_stripid_list.emplace_back(stripid);
            m_filled_index_list.emplace_back(index);
	    data_list.erase( data_list.begin()+i );
        }	
    }
    return anl::ANL_OK;
}
bool MergeAdjacent::isAdjacent(int stripid, const std::vector<int>& stripid_list)
{
    for(auto itr : stripid_list){
	if( abs( stripid - itr ) == 1 ) return true;
    }
    return false;
}
bool MergeAdjacent::isAdjacent(int stripid, const std::vector<lv1data>& data_list)
{
    for(auto data : data_list) if( abs( stripid - data.stripid ) == 1 ) {std::cout << "d"; return true;}
    return false;
}
