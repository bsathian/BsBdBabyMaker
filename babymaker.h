#ifndef BABYMAKER_H
#define BABYMAKER_H

//ROOT
# include "TBenchmark.h"
# include "TChain.h"
# include "TDirectory.h"
# include "TFile.h"
# include "TROOT.h"
# include "TTreeCache.h"
# include "TEfficiency.h"
# include "TH1.h"
# include "TH2.h"
# include "TH3.h"

//CMS3
# include "CMS3.h"
# include "MuonSelections.h"

# include <iostream>
# include <vector>
# include <set>
# include <tuple>
# include <utility>
# include <fstream>
# include <cmath>
# include <algorithm>

class babymaker
{
    TFile *babyFile;
    TTree *Events;
    //TBranch variables
    std::vector<LorentzVector>* ll_muon_p4_;
    std::vector<LorentzVector>* lt_muon_p4_;
    std::vector<int> *ll_muon_id_;
    std::vector<int> *lt_muon_id_;
    std::vector<int> *muon_mother_id_;

    LorentzVector *pv_;

    std::vector<LorentzVector>* lv_;
    std::vector<float>* pv_cov_;
    std::vector<std::vector<float>>* lv_cov_;

    //gen stuff
    std::vector<LorentzVector>* gen_muon_p4_;
    std::vector<LorentzVector>* gen_muon_v4_;
    std::vector<int> completedllIdx; //also check unordered map
    std::vector<int> completedltIdx;
    std::vector<LorentzVector>* gen_pv_;

    //hit stuff
    std::vector<int> *muon_hits_;
    std::vector<int> *muon_pixelHits_;

    void cleanVectors(); 
    public:
    babymaker(std::string fileName);
    int fieldCopy(CMS3& cms3);
    void makeBabies(TChain* ch);
    ~babymaker();

};

#endif
