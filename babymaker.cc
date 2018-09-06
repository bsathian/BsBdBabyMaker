# include "babymaker.h"

babymaker::babymaker(std::string fileName)
{
    babyFile = new TFile(fileName.c_str(),"RECREATE");
    Events = new TTree("Events","Events");
    

    ll_muon_id_ = new std::vector<int>;
    Events->Branch("ll_id",&ll_muon_id_);
    lt_muon_id_ = new std::vector<int>;
    Events->Branch("lt_id",&lt_muon_id_);
    muon_mother_id_= new std::vector<int>;
    Events->Branch("muon_mother_id",&muon_mother_id_);
    
    ll_muon_p4_ = new std::vector<LorentzVector>;
    Events->Branch("ll_muon_p4",&ll_muon_p4_);
    lt_muon_p4_ = new std::vector<LorentzVector>;
    Events->Branch("lt_muon_p4",&lt_muon_p4_);
    pv_ = new LorentzVector;
    Events->Branch("PV",&pv_);
    lv_  = new std::vector<LorentzVector>;
    Events->Branch("lep_vtx",&lv_);
    pv_cov_ = new std::vector<float>;
    Events->Branch("PV_cov",&pv_cov_);
    lv_cov_ = new std::vector<std::vector<float>>;
    Events->Branch("lep_vtx_cov",&lv_cov_);

    gen_muon_p4_ = new std::vector<LorentzVector>;
    Events->Branch("gen_muon_p4",&gen_muon_p4_);
    gen_muon_v4_ = new std::vector<LorentzVector>;
    Events->Branch("gen_muon_v4",&gen_muon_v4_);
    gen_pv_ = new std::vector<LorentzVector>;
    //Events->Branch("gen_PV",&gen_pv_);
    
}

void babymaker::cleanVectors()
{
    ll_muon_p4_->clear();
    lt_muon_p4_->clear();
    ll_muon_id_->clear();
    lt_muon_id_->clear();
    lv_->clear();
    pv_cov_->clear();
    lv_cov_->clear();
    gen_muon_p4_->clear();
    gen_muon_v4_->clear();
    gen_pv_->clear();
    completedllIdx.clear();
    completedltIdx.clear();
    muon_mother_id_->clear();
}

int babymaker::fieldCopy(CMS3& cms3)
{
    *pv_ = cms3.vtxs_position().at(0);
    *pv_cov_ = cms3.vtxs_covMatrix().at(0);

    /*Sends status 1 if any valid vertex found*/
    int flag = 0;
    int gen_ll_index,gen_lt_index,mus_ll_index,mus_lt_index;
    
    //Massaging for lv_cov
    LorentzVector dilepP4;
    for(size_t i = 0; i < cms3.hyp_FVFit_status().size();i++)
    {
        if(cms3.hyp_FVFit_status().at(i) != 0)
            continue;
        if((cms3.hyp_ll_id().at(i) * cms3.hyp_lt_id().at(i)) != -169)
            continue;

        //muon eta cut
        if(!(abs(cms3.hyp_ll_p4().at(i).Eta()) < 1.4 && abs(cms3.hyp_lt_p4().at(i).Eta()) < 1.4))
            continue;
        //mass cut
        dilepP4 = cms3.hyp_ll_p4().at(i) + cms3.hyp_lt_p4().at(i);

        if(!(dilepP4.M() > 4.5 && dilepP4.M() < 6.5))
                continue;

        //MuonMaker level - positions in the mus_mc3idx vector
        mus_ll_index = cms3.hyp_ll_index().at(i); 
        mus_lt_index = cms3.hyp_lt_index().at(i);

        //Fail if lt and ll indices are the same, or lt/ll indices at -9999
        if(mus_lt_index < 0 || mus_ll_index < 0 || mus_ll_index == mus_lt_index)
            continue;

        //Generator level - positions in the genps vector
        gen_ll_index = cms3.mus_mc3idx().at(mus_ll_index);
        gen_lt_index = cms3.mus_mc3idx().at(mus_lt_index);

        if(gen_ll_index < 0 || gen_lt_index < 0 || gen_ll_index == gen_lt_index)
            continue;

        //Duplicate checking
        if(std::find(completedllIdx.begin(),completedllIdx.end(),gen_ll_index) != completedllIdx.end()) //already done!
            continue;
        if(std::find(completedltIdx.begin(),completedltIdx.end(),gen_lt_index) != completedltIdx.end()) //lt also already done!
            continue;
        


        //Checking if the muons came from B meson
        if(abs(cms3.genps_id_mother().at(gen_ll_index)) != 531 || abs(cms3.genps_id_mother().at(gen_lt_index)) != 531)
            continue;
        
        
        completedllIdx.push_back(gen_ll_index);
        completedltIdx.push_back(gen_lt_index);
        
        flag = 1;
        
        ll_muon_p4_->push_back(cms3.hyp_ll_p4().at(i));
        lt_muon_p4_->push_back(cms3.hyp_lt_p4().at(i));
        lv_->push_back(cms3.hyp_FVFit_v4().at(i));
        ll_muon_id_->push_back(cms3.hyp_ll_id().at(i));
        lt_muon_id_->push_back(cms3.hyp_lt_id().at(i));

        //push gen level stuff
        gen_muon_p4_->push_back(cms3.genps_p4().at(gen_ll_index));
        gen_muon_v4_->push_back(cms3.genps_prod_vtx().at(gen_ll_index));
        gen_muon_p4_->push_back(cms3.genps_p4().at(gen_lt_index));
        gen_muon_v4_->push_back(cms3.genps_prod_vtx().at(gen_lt_index));

        muon_mother_id_->push_back(cms3.genps_id_mother().at(gen_ll_index));
        muon_mother_id_->push_back(cms3.genps_id_mother().at(gen_lt_index));



        float temp[] = {cms3.hyp_FVFit_v4cxx().at(i), cms3.hyp_FVFit_v4cxy().at(i), cms3.hyp_FVFit_v4czx().at(i), cms3.hyp_FVFit_v4cxy().at(i), cms3.hyp_FVFit_v4cyy().at(i), cms3.hyp_FVFit_v4czy().at(i), cms3.hyp_FVFit_v4czx().at(i), cms3.hyp_FVFit_v4czy().at(i), cms3.hyp_FVFit_v4czz().at(i)};
        
        std::vector<float> tempVec(temp,temp + sizeof(temp)/sizeof(float));
        lv_cov_->push_back(tempVec);
    }
    

    

    return flag; 
}

void babymaker::makeBabies(TChain *ch)
{
    size_t nEventsTotal = 0;
    size_t nEventsChain = ch->GetEntries();
    cms3.Init(ch);

    for(auto event = 0; event < nEventsChain; event++)
    {
        cms3.GetEntry(event);
        nEventsTotal++;
        //clean all vectors to prevent pushing new data behind existing data
        cleanVectors(); 
        //start babymaking
        int flag = fieldCopy(cms3);
        if(flag == 1) //fills only if valid vertex found
            Events->Fill();
    }
}


babymaker::~babymaker()
{
    babyFile->Write();
}
