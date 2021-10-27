#include <iostream>
#include <TRandom.h>

#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>

#include "edm4hep/MCParticleData.h"


using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;
//


int main() {

    {
      auto model = RNTupleModel::Create();
      auto fldVpx = model->MakeField<std::vector<float>>("vpx");
      auto part = model->MakeField<std::vector<edm4hep::MCParticleData>>("part");

      auto ntuple = RNTupleWriter::Recreate(std::move(model), "F", "tmp.root");
      for (int i = 0; i < 100; ++i) { //event loop
        fldVpx->clear();
        for (int j = 0; j < 1000; ++j) {
          float px, py, pz;
          gRandom->Rannor(px, py);
          fldVpx->emplace_back(px);
          edm4hep::MCParticleData p;
          p.charge = 123;
          p.vertex.x = 234;
          part->emplace_back(p);
          }
        ntuple->Fill();
        }
    }

    {

      auto model = RNTupleModel::Create();
      auto fldVpx = model->MakeField<std::vector<float>>("vpx");
      auto part = model->MakeField<std::vector<edm4hep::MCParticleData>>("part");
      auto ntuple = RNTupleReader::Open(std::move(model), "F", "tmp.root");
      //ntuple->Show(41);
      for (auto entryId : *ntuple) {
        ntuple->LoadEntry(entryId);
        std::cout << part->at(0).vertex.x << std::endl;


      }


    }



  std::cout << "test" << std::endl;
  return 0;
}
