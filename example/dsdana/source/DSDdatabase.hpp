/**
   @file   DSDdatabase.hpp
   @date   2020/04/11
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#ifndef DSDdatabase_hpp
#define DSDdatabase_hpp

#include <VANL_Module.hpp>

#include <vector>
#include <algorithm>
#include <cmath>

class DSDdatabase : public anl::VANL_Module
{
public:
    DSDdatabase();
    ~DSDdatabase();

    int mod_bgnrun() override;
    int mod_ana() override;
    int mod_endrun() override;

    class data_element
    {
    public:
        data_element() :
            asicid(0), asicch(0), detid(0), stripid(0),
            material(-1), posx(0), posy(0), posz(0),
            widthx(0), widthy(0), widthz(0), badch(0), ethre(0),
            is_xside(false), is_badch(false)
        {}

        data_element(const data_element& org) :
            asicid(org.asicid), asicch(org.asicch),
            detid(org.detid), stripid(org.stripid),
            material(org.material),
            posx(org.posx), posy(org.posy), posz(org.posz),
            widthx(org.widthx), widthy(org.widthy), widthz(org.widthz),
            badch(org.badch), ethre(org.ethre),
	    is_xside(org.is_xside), is_badch(org.is_badch)
        {
            for(auto p : org.calparam) calparam.emplace_back(p);
        }
        data_element &operator=(const data_element &org)
        {
            asicid = org.asicid; asicch = org.asicch;
            detid = org.detid; stripid = org.stripid;
            material = org.material;
            posx = org.posx; posy = org.posy; posz = org.posz;
            widthx = org.widthx; widthy = org.widthy; widthz = org.widthz;
            badch = org.badch; ethre = org.ethre;
	    is_xside = org.is_xside; is_badch = org.is_badch;
            for(auto p : org.calparam) calparam.emplace_back(p);
            return *this;
        }
	~data_element() {}

        int print();
	
	data_element* setx(const float pos, const float width)
        {
            posx = pos; widthx = width; return this;
        }
        data_element* sety(const float pos, const float width)
        {
            posy = pos; widthy = width; return this;
        }
        data_element* setz(const float pos, const float width)
        {
            posz = pos; widthz = width; return this;
        }
        data_element* set_status
	(const int badch, const float ethre, const bool is_xside)
        {
            this->badch = badch; this->ethre = ethre;
            this->is_xside = is_xside;
	    if ( badch == 0 ) this->is_badch = false;
	    else this->is_badch = true;
	    return this;
        }
	data_element* set_material(const int mate)
	{
	    this->material = mate;
	    return this;
	}
	    
	
	inline double epi(const float pha)
	{
	    double value = 0.0; int nparam = (int)this->calparam.size();
	    for(int i=0; i<nparam; ++i) value += calparam[i]*std::pow(pha, i);
	    return value;
	}

        int asicid, asicch, detid, stripid, material;
        float posx, posy, posz;
        float widthx, widthy, widthz;
        int badch;
        float ethre;
        bool is_xside, is_badch;
        std::vector<float> calparam;
    };
    
protected:

    std::vector<data_element*> database;
    data_element maxinfo;
    data_element mininfo;

    std::map< std::pair<int, int>, std::pair<int, int> > strip_map;
    std::map<int, int> stripid_to_index_map;

    std::vector<int> list_of_asicid;
    std::vector<int> list_of_detid;
    std::vector<int> list_of_stripid;
    
    template<typename T>
    bool exist(const std::vector<T>& vec, const T& elem)
    {
	auto itr = std::find(vec.begin(), vec.end(), elem);
	return itr != vec.end();
    }
    
public:
    
    std::vector<int> GetListOfAsicids();
    int GetNasics();
    bool ExistAsicid(const int asicid);

    std::vector<int> GetListOfDetids();
    int GetNdetectors();
    bool ExistDetid(const int detid);

    std::vector<int> GetListOfStrips();
    int GetNstrips();
    bool ExistStrip(int stripid);
    
    inline int GetNallch() { return GetNstrips(); }

    int GetNxside();
    inline int GetNyside() { return GetNstrips() - GetNxside(); }

    std::pair<int, int> FindStrip(int asicid, int asicch);
    
    int GetStripid(int asicid, int asicch)
    {
	return FindStrip(asicid, asicch).second;
    }
    
    /**
       @func GetIndex
       @return Index in the list of strips correponding to the 'stripid'.
       @return -1 if 'stripid' is not found.
    **/
    int GetIndex(const int stripid);

    data_element* GetDataElement(const int index);
    // {
    // 	return database[ index ];
    // }	
    
    int GetAsicid(const int stripid);
    int GetAsicch(const int stripid);
    int GetDetid(const int stripid);
    int GetMaterial(const int stripid);
    float GetPosx(const int stripid);
    float GetPosy(const int stripid);
    float GetPosz(const int stripid);
    float GetWidthx(const int stripid);
    float GetWidthy(const int stripid);
    float GetWidthz(const int stripid);
    float GetEthre(const int stripid);
    bool IsBadch(const int stripid);
    bool IsXside(const int stripid);

    virtual float GetEPI(const int stripid, const float pha);

    int GetDetectorPosz(const int detid);
    int GetDetectorWidthz(const int detid);    
        
    int GetAsicidMax() const { return maxinfo.asicid; }
    int GetDetidMax() const { return maxinfo.detid; }
    int GetStripidMax() const { return maxinfo.stripid; }
    int GetPosxMax() const { return maxinfo.posx; }
    int GetPosyMax() const { return maxinfo.posy; }
    int GetPoszMax() const { return maxinfo.posz; }
    int GetAsicidMin() const { return mininfo.asicid; }
    int GetDetidMin() const { return mininfo.detid; }
    int GetStripidMin() const { return mininfo.stripid; }
    int GetPosxMin() const { return mininfo.posx; }
    int GetPosyMin() const { return mininfo.posy; }
    int GetPoszMin() const { return mininfo.posz; }    
};

#endif

