#include <vector>
#include <string>
#include <algorithm>
#include "indri/QueryEnvironment.hpp"
#include "indri/SnippetBuilder.hpp"

using namespace indri::api;

bool sortTid(const int& a, const int& b){
  return (a < b);
}



void removeDups(vector<int>& termIds){
     int index = 0;
     for(int j = 0; j < termIds.size(); ++j){

      if( termIds[index] == termIds[j]){
        continue;
      }else{
        termIds[++index] = termIds[j];
      }
     }
     termIds.resize(index + 1);
}

void getDeltaList(vector<int>& termIds, vector<int>& deltaList){
     deltaList.push_back(termIds[0]);
     for(int j = 0; j < termIds.size() - 1; ++j){
        deltaList.push_back( termIds[j+1] - termIds[j]);
     }
}

string getDocumentName(ParsedDocument* Pd){
    indri::utility::greedy_vector<indri::parse::MetadataPair>::iterator iter = std::find_if( Pd->metadata.begin(), Pd->metadata.end(),
          indri::parse::MetadataPair::key_equal( "docno" ) );
        if( iter != Pd->metadata.end() ){
          return (char*) iter->value;
        }else{
          cout << "can't find the document.." <<endl;
          exit(0);
        }
}

void getTermList(vector<int>& termIds, indri::index::Index *thisIndex, int did){
     const indri::index::TermList *termList=thisIndex->termList(did);
     for(int j=0; j<termList->terms().size(); ++j){
       cout << termList->terms()[j] << " ";
     }
     cout << endl;
     for(int j=0; j<termList->terms().size(); ++j){
      cout << thisIndex->term(termList->terms()[j]) << " ";
     }
     cout << endl;

     for(int j=0; j<termList->terms().size(); ++j){
      if(termList->terms()[j]!=0){
        termIds.push_back(termList->terms()[j]);
      }
     }
}

int main(int argc, char *argv[]) {
   // we assume the index path is the first argument and the query is second
   // char *indexPath=argv[1];
   // char *query=argv[2];

   // our builder object - false in the constructor means no HTML output.
   SnippetBuilder builder(true);

   // create a query environment
   QueryEnvironment indriEnvironment;

   // open the index
   std::string indexPath = "/home/qi/CIKM2016/indriIndex/smallIndex";
   indriEnvironment.addIndex(indexPath);

   indri::collection::Repository repository;
   repository.openRead(indexPath);
   indri::collection::Repository::index_state repIndexState = repository.indexes();
   indri::index::Index *thisIndex=(*repIndexState)[0];


   std::vector<lemur::api::DOCID_T> documentIDs;
   documentIDs.push_back(1);
   // documentIDs.push_back(2);
   // documentIDs.push_back(3);

   std::vector<ParsedDocument*> parsedDocs=indriEnvironment.documents(documentIDs);

   for (int i=0; i < documentIDs.size(); i++) {

    string documentName = getDocumentName(parsedDocs[i]);

     // print the document ID, document name and its content
     cout << documentIDs[i] << endl;
     cout << documentName << endl;

     vector <int> termIds;
     getTermList(termIds, thisIndex, documentIDs[i]);
     
     // sorting
     sort(termIds.begin(), termIds.end(), sortTid);

     for(int j=0; j<termIds.size(); ++j){
      cout << termIds[j] << " ";
     }
     cout << endl;
     // remove dups
     removeDups(termIds);

     // make delta list
     vector <int> deltaList;
     getDeltaList(termIds, deltaList);

     for(int j=0; j<termIds.size(); ++j){
      cout << termIds[j] << " ";
     }
     cout << endl;
     for(int j=0; j<deltaList.size(); ++j){
      cout << deltaList[j] << " ";
     }
     cout << endl;
     for(int j=0; j<termIds.size(); ++j){
      cout << thisIndex->term(termIds[j]) << " ";
     }
     cout << endl;
     // cout << parsedDoc->getContent()<<endl;
     // cout << endl;
   }

  // note that we do not need to explicitly delete the
  // QueryEnvironment object here to close the index. It will
  // automatically be removed when it goes out of scope.
  return 0;
 }