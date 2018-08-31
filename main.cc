# include "babymaker.h"

int main(int argc, char* argv[])
{
    std::string parentName(argv[1]);
    std::string babyPrefix(argv[2]);
    std::string index(argv[3]);

    std::string babyName = babyPrefix + index + ".root";
   
    babymaker b(babyName);
    TChain *ch = new TChain("Events");
    ch->Add(parentName.c_str());
    b.makeBabies(ch); 
}
