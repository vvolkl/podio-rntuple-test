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
      auto fldVpx = model->MakeField<std::vector<float>>("vpx");
      //auto part = model->MakeField<std::vector<edm4hep::MCParticleData>>("part");

       auto field = RFieldBase::Create("part",  "vector<edm4hep::MCParticleData>").Unwrap();
       auto vec = new edm4hep::MCParticleCollection();
       auto buffers = vec->getBuffers();

       std::vector<edm4hep::MCParticleData> **v = new std::vector<edm4hep::MCParticleData> *();
       *v = buffers.dataAsVector<edm4hep::MCParticleData>();
       void* vv =  (void*) *static_cast<std::vector<float>**>(buffers.data);
       //model->GetDefaultEntry()->CaptureValue(field->CaptureValue(*v));
       model->GetDefaultEntry()->CaptureValue(field->CaptureValue(vv));
       model->GetFieldZero()->Attach(std::move(field));
      auto ntuple = RNTupleWriter::Recreate(std::move(model), "F", "tmp.root");
      for (int i = 0; i < 100; ++i) { //event loop
        fldVpx->clear();
        vec->clear();
        for (int j = 0; j < 1000; ++j) {
          float px, py, pz;
          gRandom->Rannor(px, py);
          fldVpx->emplace_back(px);
          edm4hep::MCParticle p;
          p.setCharge(123);
          vec->push_back(p);
          

          }

        vec->prepareForWrite();
        std::cout<< vec->size() << "\t" << vec->getBuffers().dataAsVector<edm4hep::MCParticleData>()->size() << std::endl;
        ntuple->Fill();
        }
    }

    {

      auto model = RNTupleModel::Create();
      //auto fldVpx = model->MakeField<std::vector<float>>("vpx");
      //auto part = model->MakeField<std::vector<edm4hep::MCParticleData>>("part");
      

       auto field = RFieldBase::Create("part",  "vector<edm4hep::MCParticleData>").Unwrap();
       

       auto rv = TClass::GetClass("vector<edm4hep::MCParticleData>");
       auto rvc = TClass::GetClass("edm4hep::MCParticleCollection");

       auto vec = static_cast<podio::CollectionBase*>(rvc->New()); // new edm4hep::MCParticleCollection();
       auto buffers = vec->getBuffers();
       //auto rvc = rv->New();
             auto collection = 
                     static_cast<podio::CollectionBase*>(rvc->New());
       auto collBuffers = collection->getBuffers();
       collBuffers.data = rv->New();

       //std::vector<edm4hep::MCParticleData> **v = new std::vector<edm4hep::MCParticleData> *();
       //*v = buffers.dataAsVector<edm4hep::MCParticleData>();
       void* vv =  (void*) *static_cast<std::vector<float>**>(buffers.data);
       model->GetDefaultEntry()->CaptureValue(field->CaptureValue(vv));
       model->GetFieldZero()->Attach(std::move(field));




      auto ntuple = RNTupleReader::Open(std::move(model), "F", "tmp.root");
      ntuple->Show(41);
      for (auto entryId : *ntuple) {
        ntuple->LoadEntry(entryId);
        //std::cout << part->at(0).vertex.x << std::endl;
        //std::cout << (*v)->size() << std::endl;


      }


    }



  std::cout << "test" << std::endl;
  return 0;
}
