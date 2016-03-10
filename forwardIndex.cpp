#include <vector>
#include <string>
#include <algorithm>
#include "indri/QueryEnvironment.hpp"
#include "indri/SnippetBuilder.hpp"

using namespace indri::api;

bool sortTid(const int& a, const int& b){
  return (a < b);
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
   std::string indexPath = "/home/vgc/qi/CIKM2016/clueWeb09_B";
   indriEnvironment.addIndex(indexPath);

   indri::collection::Repository repository;
   repository.openRead(indexPath);
   indri::collection::Repository::index_state repIndexState = repository.indexes();
   indri::index::Index *thisIndex=(*repIndexState)[0];


   std::vector<lemur::api::DOCID_T> documentIDs;
   documentIDs.push_back(1);
   documentIDs.push_back(2);
   documentIDs.push_back(3);

   std::vector<ParsedDocument*> parsedDocs=indriEnvironment.documents(documentIDs);

   for (int i=0; i < documentIDs.size(); i++) {
     // get this document's parsed doc representation
     ParsedDocument* parsedDoc=parsedDocs[i];

     indri::utility::greedy_vector<indri::parse::MetadataPair>::iterator iter = std::find_if( parsedDocs[i]->metadata.begin(),
          parsedDocs[i]->metadata.end(),
          indri::parse::MetadataPair::key_equal( "docno" ) );

    string documentName;
    if( iter != parsedDocs[i]->metadata.end() )
          documentName = (char*) iter->value;

    const indri::index::TermList *termList=thisIndex->termList(documentIDs[i]);

     // print the document ID, document name and its content
     cout << documentIDs[i] << endl;
     cout << documentName << endl;
     vector <int> termIds;
     for(int j=0; j<termList->terms().size(); ++j){
      termIds.push_back(termList->terms()[j]);
     }
     for(int j=0; j<termList->terms().size(); ++j){
      cout << termList->terms()[j] << " ";
     }
     cout << endl;
     for(int j=0; j<termList->terms().size(); ++j){
      cout << thisIndex->term(termList->terms()[j]) << " ";
     }
     cout << endl;
     sort(termIds.begin(), termIds.end(), sortTid);

     int index = 0;
     for(int i = 0; i < termIds.size(); ++i){

      if( (termIds[index] == termIds[i]) || (termIds[i] == 0)){
        continue;
      }
      termIds[index] = termIds[i];
      index ++;
     }
     termIds.resize(index + 1);

     for(int j=0; j<termIds.size(); ++j){
      cout << termIds[j] << " ";
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


 //  #include <vector>
 //  #include "indri/Repository.hpp"

 //  // a global repository object for our index
 //  indri::collection::Repository repository;

 //  using namespace indri::api;

 //  void getFieldText(int documentID, std::string field) {
 //    // get the index from the repository
 //    // in our case it will be the first index
 //    indri::collection::Repository::index_state repIndexState = repository.indexes();
 //    indri::index::Index *thisIndex=(*repIndexState)[0];

 //    // get the field ID
 //    int fieldID=thisIndex->field(field);

 //    // is this a valid field? If not, exit
 //    if (fieldID < 1) { return; }

 //    // retrieve the document vector for this document
 //    const indri::index::TermList *termList=thisIndex->termList(documentID);

 //    // ensure we have a valid term list!
 //    if (!termList) { return; }

 //    // get the vector of fields
 //    indri::utility::greedy_vector< indri::index::FieldExtent > fieldVec=termList->fields();

 //    // go through the fields (create an iterator)
 //    indri::utility::greedy_vector< indri::index::FieldExtent >::iterator fIter=fieldVec.begin();
 //    while (fIter!=fieldVec.end()) {
 //      // does the ID of this field extent match our field ID?
 //      if ((*fIter).id==fieldID) {
 //        // yes! We can print out the text here
 //        int beginTerm=(*fIter).begin;
 //        int endTerm=(*fIter).end;

 //        // note that the text is inclusive of the beginning
 //        // but exclusive of the ending
 //        for (int t=beginTerm; t < endTerm; t++) {
 //          // get this term ID
 //          int thisTermID=termList->terms()[t];
 //          // convert the term ID to its string representation
 //          cout << thisIndex->term(thisTermID) << " ";
 //        }
 //        cout << endl;
 //      }

 //      // increment the iterator
 //      fIter++;
 //    }

 //    // destroy the term list object
 //    delete termList;
 //    termList=NULL;
 //  }

 // int main(int argc, char *argv[]) {
 //   // we assume the index path is the first argument and the query is second
 //   char *indexPath=argv[1];

 //   // open our repository object (read-only!!)
 //   repository.openRead(indexPath);

 //   // retrieve the title field for document 5
 //   getFieldText(5, "title");

 //   // retrieve any person fields for document 5
 //   getFieldText(5, "person");

 //   // close the repository...
 //   repository.close();

 //   return 0;
 // }