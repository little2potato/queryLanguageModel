#include "globals.h"

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

void getDeltaList(vector<int>& termIds, vector<unsigned>& deltaList){
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

int compressionVbytes(vector<unsigned> input, unsigned char* output){
   int outputSize = 0;
   for (int i = 0; i < input.size(); ++i){
          unsigned char byteArr[CONST::numBytesInInt];
          bool started = false;
          int x = 0;

          for (x = 0; x < CONST::numBytesInInt; x++) {
             byteArr[x] = (input[i]%128);
             input[i] /= 128;
          }

          for (x = CONST::numBytesInInt - 1; x > 0; x--) {
            if (byteArr[x] != 0 || started == true) {
              started = true;
              byteArr[x] |= 128;
              output[outputSize++] =  byteArr[x];
            }
          }
          output[outputSize++] =  byteArr[0];
   }
   return outputSize;
}

int decompressionVbytes(unsigned char* input, unsigned int* output, int size){
    unsigned char* curr_byte = input;
    unsigned int n;
    for (int i = 0; i < size; ++i) {
      unsigned char b = *curr_byte;
      n = b & 0x7F;
//        cout<<"The first byte: "<<n<<endl;
//        print_binary(n);
//        cout<<endl;

      while((b & 0x80) !=0){
        n = n << 7;
        ++curr_byte;
          b = *curr_byte;
          n |= (b & 0x7F);
//          cout<<"The following byte: "<<n<<endl;
//          print_binary(n);
//          cout<<endl;
      }
    ++curr_byte;
    output[i] = n;
  }

  int num_bytes_consumed = (curr_byte - input);
  return (num_bytes_consumed >> 2) + ((num_bytes_consumed & 3) != 0 ? 1 : 0);
}

// Print unsigned integer in binary format with spaces separating each byte.
void print_binary(unsigned int num) {
   int arr[32];
    int i = 0;
    while (i++ < 32 || num / 2 != 0) {
      arr[i - 1] = num % 2;
      num /= 2;
    }

    for (i = 31; i >= 0; i--) {
      printf("%d", arr[i]);
      if (i % 8 == 0)
        printf(" ");
    }
    printf("\n");
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
   indriEnvironment.addIndex(CONST::indexPath);

   indri::collection::Repository repository;
   repository.openRead(CONST::indexPath);
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
     vector <unsigned> deltaList;
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

     // unsigned value = 1937;
     // unsigned char* output = new unsigned char[100];
     // int size = encodeVarint(value, output); 
     // cout << size << endl;
     // unsigned result;
     // result = decodeVarint(output, size);
     // cout << result << endl;

     unsigned char * compressed_list = new unsigned char[deltaList.size()*4];
     unsigned * uncompressed_list = new unsigned[deltaList.size()];

     int compressedSize = compressionVbytes(deltaList, compressed_list);
     decompressionVbytes(compressed_list, uncompressed_list, deltaList.size());

     for(int j=0; j<deltaList.size(); ++j){
      cout << uncompressed_list[j] << " ";
     }
     cout << endl;
     cout << compressedSize << endl;
     cout << deltaList.size() * 4 << endl;
   }

  // note that we do not need to explicitly delete the
  // QueryEnvironment object here to close the index. It will
  // automatically be removed when it goes out of scope.
  return 0;
 }