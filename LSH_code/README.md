####class: 
#####Edge   //(docID, similarity)
#####edgeComparator  //return a.similarity > b.similarity
#####edgeComparatorargestFirst    //return a.similarity < b.similarity
#####Node   //vector<Edge> candidates,  priority-queue<Edge, vector<Edge>, edgeComparatorLargeFirst>
#####minHash    (docID, numTerms, vector minH)
#####minHashComparator    //return a.docId < b.docId
#####Graph    //vector<node> nodes  
#####SuperHash    //docID, superHash, minHash

Graph::FilterCandidates
