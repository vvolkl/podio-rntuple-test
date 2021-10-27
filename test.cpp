#include <iostream>
#include <TRandom.h>
#include <TClass.h>

#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>

#include "edm4hep/MCParticleData.h"
#include "edm4hep/MCParticleCollection.h"


using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;
using RFieldBase = ROOT::Experimental::Detail::RFieldBase;


int main() {

    {
      auto model = RNTupleModel::Create();
      // start with a std vector as per examples
      auto fldVpx = model->MakeField<std::vector<float>>("vpx");
      // an example podio collection to be persistified
      auto vec = new edm4hep::MCParticleCollection();
      auto buffers = vec->getBuffers();
      // create field for a podio collection
      // in the same conditions as in the event store: only based on a string with the typename and the pointer to the buffer
      auto field = RFieldBase::Create("part",  "vector<edm4hep::MCParticleData>").Unwrap();
      // what I cast buffers.data to here does not seme to matter, but it seems I have to cast it to something?
      void* vv =  (void*) *static_cast<std::vector<float>**>(buffers.data); 
      model->GetDefaultEntry()->CaptureValue(field->CaptureValue(vv));
      model->GetFieldZero()->Attach(std::move(field));
      
      auto ntuple = RNTupleWriter::Recreate(std::move(model), "F", "tmp.root");

      for (int i = 0; i < 100; ++i) { //event loop
        fldVpx->clear();
        vec->clear();
        for (int j = 0; j < 1000; ++j) {
          // set some dummy values for later checks
          float px, py, pz;
          gRandom->Rannor(px, py);
          fldVpx->emplace_back(px);
          edm4hep::MCParticle p;
          p.setCharge(123);
          p.setMomentum({px, py, 234.});
          vec->push_back(p);
        }
        vec->prepareForWrite();
        std::cout<< vec->size() << "\t" << vec->getBuffers().dataAsVector<edm4hep::MCParticleData>()->size() << std::endl;
        ntuple->Fill();
        std::cout << "end of event " << i << std::endl;
        }
    }

    // part two: read back the collection from file
    {
      auto model = RNTupleModel::Create();
      // as per open lhcb example
      auto field = RFieldBase::Create("part",  "vector<edm4hep::MCParticleData>").Unwrap();
      model->AddField(std::move(field));
      void *fieldDataPtr = model->GetDefaultEntry()->GetValue("part").GetRawPtr();
      auto ntuple = RNTupleReader::Open(std::move(model), "F", "tmp.root");
      // can now use fielDataPtr to set the buffers of a new collection, tbd.
      // debug outputs to check correctness
      // this amounts to user code, so we can cast explicitly
      ntuple->Show(41);
      for (auto entryId : *ntuple) {
        ntuple->LoadEntry(entryId);
        std::cout << (static_cast<std::vector<edm4hep::MCParticleData>*>(fieldDataPtr))->size() << std::endl;
      }


    }





  std::cout << "test" << std::endl;
  return 0;
}
