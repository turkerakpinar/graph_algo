#include "Graph.h"

/*Do not add new libraries or files*/


Graph::Graph() {
    
}

Graph::Graph(const Graph& rhs) {
    adjList=rhs.adjList;
    

}

Graph& Graph::operator=(const Graph& rhs) {
   adjList=rhs.adjList;
   return *this;
    

}

Graph::~Graph() {


}


void Graph::addNode(const Node &node) {
    list<Edge> temp;
    adjList.Insert(node.getVid(),temp);

}

void Graph::addConnection(const Node& headNode, const Node& tailNode, int import) {
    adjList.Get(headNode.getVid()).emplace_back(tailNode,import);

}

list<Node> Graph::getAdjacentNodes(const Node& node) {
    list<Node> edlist;
    list<Edge> edge =adjList.Get(node.getVid());
     for(auto it = edge.begin(); it != edge.end(); ++it){
         edlist.push_back((*it).getTailNode());
         
         
     }
     return edlist;

}

long Graph::getTotalImports(const Node& node) {
     long total=0;
    list<Edge> edge =adjList.Get(node.getVid());
     for(auto it = edge.begin(); it != edge.end(); ++it){
         total +=(*it).getImport();
         
         
     }
     return total;

}


void Graph::deleteNode(const Node& node) {
    int keys[adjList.Size()];
    adjList.getKeys(keys);
     for(int i=0;i<adjList.Size();i++){
        list<Edge> &edge =adjList.Get(keys[i]);
        for(auto it = edge.begin(); it != edge.end(); ){
            if((*it).getTailNode().getVid()==node.getVid()){
                it=edge.erase(it);
            }
            else{
                it++;
            }
            
        }
     }
    adjList.Delete(node.getVid());
}


list<string> Graph::findLeastCostPath(const Node& srcNode, const Node& destNode) {
    HashTable<int, int> prevNode;
    HashTable<int, long> cost;
    HashTable <int,string> country;
    list<string> path;
    int k=adjList.Size();
    int keys[k];
    adjList.getKeys(keys);
    for(int i=0;i<k;i++){
        prevNode.Insert(keys[i],keys[i]);
        cost.Insert(keys[i],LONG_MAX);
    }
    cost.Insert(srcNode.getVid(),0);
    priority_queue<pair<long,int>, vector<pair<long,int>>, greater<pair<long,int>>> pq;
    pq.push(make_pair(0,srcNode.getVid()));
    country.Insert(srcNode.getVid(),srcNode.getCountry());
    while(!pq.empty()){
        pair<long,int> last=pq.top();
        pq.pop();
        list<Edge> edge =adjList.Get(last.second);
        for(auto it = edge.begin(); it != edge.end(); ++it){
            int current_distance=last.first+(*it).getImport();
            int key=((*it).getTailNode()).getVid();
            if(current_distance<cost.Get(key)){
                cost.Get(key)=current_distance;
                prevNode.Get(key)=last.second;
                pq.push(make_pair(current_distance,key));
                country.Insert(key,(*it).getTailNode().getCountry());
            }
        }
        
    }
    path.push_back(destNode.getCountry());
    long n=cost.Get(destNode.getVid());
    int prev=prevNode.Get(destNode.getVid());
    while(n!=0){
        path.push_front(country.Get(prev));
        prev=prevNode.Get(prev);
        n=cost.Get(prev);
    }
    path.push_front(srcNode.getCountry());
    return path;
}


bool Graph::isCyclic() {
     HashTable<int, bool> nodeVisited;
      HashTable<int, bool> nodeRecursion;
    int n=adjList.Size();
    int keys[n];
    adjList.getKeys(keys);
    for(int i=0;i<n;i++){
        nodeVisited.Insert(keys[i],false);
        nodeRecursion.Insert(keys[i],false);
    }
    for(int i = 0; i < n; i++) 
        if (helper_cycle(keys[i], nodeVisited, nodeRecursion)){
            return true;
        }
    return false; 

}

bool Graph::helper_cycle(int key,HashTable<int, bool> &nodeVisited,HashTable<int, bool> &nodeRecursion){
    if(!nodeVisited.Get(key)){
        nodeVisited.Get(key)=true;
        nodeRecursion.Get(key)=true;
        list<Edge> edge =adjList.Get(key);
        for(auto it = edge.begin(); it != edge.end(); ++it){
             if ( nodeVisited.Get(((*it).getTailNode()).getVid())==false&& helper_cycle(((*it).getTailNode()).getVid(), nodeVisited, nodeRecursion) ) {
                return true; 
             }
            else if (nodeRecursion.Get(((*it).getTailNode()).getVid())){ 
                return true;
            }
        }
    }
    nodeRecursion.Get(key)=false;
    return false;
}


list<string> Graph::getBFSPath(const Node& srcNode, const Node& destNode) {
    int keys[adjList.Size()];
    list<string> path;
    queue<Node> q;
    adjList.getKeys(keys);
    HashTable<int, bool> nodeMark;
    for(int i=0;i<adjList.Size();i++){
        nodeMark.Insert(keys[i],false);
    }
    nodeMark.Get(srcNode.getVid())=true;
    q.push(srcNode);
     while (!q.empty()){
         path.push_back((q.front()).getCountry());
         Node last=q.front();
         q.pop();
         if (last.getVid() == destNode.getVid()){ 
            return path;
         }
         list<Edge> edge =adjList.Get(last.getVid());
     for(auto it = edge.begin(); it != edge.end(); ++it){
         if(nodeMark.Get(((*it).getTailNode()).getVid())==false){
             nodeMark.Get(((*it).getTailNode()).getVid())=true;
             q.push((*it).getTailNode());
         }
     }
     }
}