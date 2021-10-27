#include <iostream>
#include <TRandom.h>

#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>

#include "edm4hep/MCParticleData.h"


using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;
using RFieldBase = ROOT::Experimental::Detail::RFieldBase;


int main() {

    {
      auto model = RNTupleModel::Create();
      auto fldVpx = model->MakeField<std::vector<float>>("vpx");
      //auto part = model->MakeField<std::vector<edm4hep::MCParticleData>>("part");

       auto field = RFieldBase::Create("part",  "vector<edm4hep::MCParticleData>").Unwrap();
       auto vec = new std::vector<edm4hep::MCParticleData>();
       model->AddField(std::move(field));
       void *fieldDataPtr = model->GetDefaultEntry()->GetValue("part").GetRawPtr();
      auto ntuple = RNTupleWriter::Recreate(std::move(model), "F", "tmp.root");
      for (int i = 0; i < 100; ++i) { //event loop
        fldVpx->clear();
        vec->clear();
        for (int j = 0; j < 1000; ++j) {
          float px, py, pz;
          gRandom->Rannor(px, py);
          fldVpx->emplace_back(px);
          edm4hep::MCParticleData p;
          p.charge = 123;
          p.vertex.x = 234;
          vec->emplace_back(p);

          }

          std::cout << fieldDataPtr << "\t" << vec << "\t" << vec->size() << std::endl;

       *fieldDataPtr = *vec;
        ntuple->Fill();
        }
    }

    {

      auto model = RNTupleModel::Create();
      auto fldVpx = model->MakeField<std::vector<float>>("vpx");
      auto part = model->MakeField<std::vector<edm4hep::MCParticleData>>("part");
      auto ntuple = RNTupleReader::Open(std::move(model), "F", "tmp.root");
      ntuple->Show(41);
      for (auto entryId : *ntuple) {
        ntuple->LoadEntry(entryId);
        //std::cout << part->at(0).vertex.x << std::endl;
        std::cout << part->size() << std::endl;


      }


    }



  std::cout << "test" << std::endl;
  return 0;
}
