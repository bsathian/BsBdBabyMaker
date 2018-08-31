# include "TBenchmark.h"
# include "TChain.h"
# include "TDirectory.h"
# include "TFile.h"
# include "TROOT.h"
# include "TTreeCache.h"
# include "TEfficiency.h"

# include <iostream>
# include <vector>
# include <string>

class event
{
    private:
        const char* eventId;
        float eventParams;
    
    public:
        void assign(std::string,float);

};

void event::assign(std::string eventId, float eventParams)
{
    this->eventId = eventId.c_str();
    this->eventParams = eventParams;
}

int main(int argc, char* argv[])
{
    TFile *hfile = new TFile("AFile.root","RECREATE");
    
    TTree *tree = new TTree("Events","Events");

    auto *Ev = new event;
    std::cout<<sizeof(*Ev)<<std::endl;
    TBranch *br = tree->Branch("parameters",&Ev,"name/C:id/F");


    for(int i=0;i<10;i++)
    {
        Ev->assign("abc",100);
        br->Fill();
    }
    
    hfile->Write();


}
