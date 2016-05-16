#include <vector>
#include <iostream>
#include <algorithm>
#include <CL/cl.h>
#include "ClInit.hpp"
#include "graph.hpp"
#include "tools.hpp"


adjacencyMatrix::adjacencyMatrix(size_t size) : size(size){
        m = new double*[size];
        for (int i = 0; i < size; ++i) {
            m[i] = new double[size];
        }
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                m[i][j] = 0;
            }
        }
        for (int j = 0; j < size-1; ++j) {
            m[j][j+1] = fastRand(RANGE);
            m[j+1][j] = fastRand(RANGE);
        }
    numberOfEdges = 2*size-1;
}

adjacencyMatrix::adjacencyMatrix(size_t size, size_t dense) : adjacencyMatrix(size){
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if((i!=j) && (i!=j+1) && (fastRand(0xF) < dense)) {
                m[i][j] = fastRand(RANGE);
                ++numberOfEdges;
            }
        }
    }
}

double adjacencyMatrix::getDense() const{
    return ((double)numberOfEdges)/((double)(size*(size-1)));
}
size_t adjacencyMatrix::getNumberOfEdges() const{
    return numberOfEdges;
}

adjacencyMatrix::~adjacencyMatrix(){
    for (int i = 0; i < size; ++i) {
        delete [] m[i];
    }
    delete [] m;
}

std::ostream& operator <<(std::ostream &out, adjacencyMatrix const& am){
    out << am.size << "\n";
    for (int i = 0; i < am.size; ++i) {
        for (int j = 0; j < am.size; ++j) {
            out << am.m[i][j] << " ";
        }
        out<<"\n";
    }
    out << "Edges = " << am.numberOfEdges << std::endl;
    out << "Vertex = " << am.size * am.size-1 << std::endl;
    out << "Dense = " << am.getDense() << std::endl;
}

Edge::Edge(size_t from, size_t to, double weight) : from(from), to(to), weight(weight) {}
bool Edge::operator<=(Edge& b) const {return weight<=b.weight;}

Graph::Graph(const char* filename){
    std::ifstream fin(filename);
    size_t vertexCount;
    fin >> vertexCount;
    edgeLists.reserve(vertexCount);
    for (int i = 0; i < vertexCount; i++)
        edgeLists.push_back({});
    for (int i = 0; i < vertexCount; i++)
        for (int j = 0; j < vertexCount; j++){
            double tmp;
            fin >> tmp;
            if (tmp > 0)
                addEdge(i, j, tmp);
        }
}

Graph::Graph(double **m, size_t size){
    size_t vertexCount = size;
    edgeLists.reserve(vertexCount);
    for (int i = 0; i < vertexCount; i++)
        edgeLists.push_back({});
    for (int i = 0; i < vertexCount; i++)
        for (int j = 0; j < vertexCount; j++){
            double tmp = m[i][j];
            if (tmp > 0)
                addEdge(i, j, tmp);
        }
}

Graph::Graph(adjacencyMatrix const& am) : Graph(am.m, am.size){}
Graph::Graph(){}

std::vector<Edge> Graph::getVectorEdges(bool oriented = false) const{
    size_t vertexCount = edgeLists.size();
    std::vector<Edge> out;
    if(oriented){
        for( size_t i=0; i<vertexCount; ++i){
            for (auto it = edgeLists[i].begin(); it != edgeLists[i].end(); ++it){
                if(i > it->first)
                    out.push_back( {i, it->first, it->second} );
            }
        }
    }
    else{
        for( size_t i=0; i<vertexCount; ++i){
            for (auto it = edgeLists[i].begin(); it != edgeLists[i].end(); ++it){
                out.push_back( {i, it->first, it->second} );
            }
        }
    }

    return out;
}

size_t Graph::numberOfVertex() const{
    return edgeLists.size();
}

std::vector<std::pair<size_t, double> > const& Graph::getEdges(int from) const{
    return edgeLists[from];
}

void Graph::addEdge(size_t from, size_t to, double weight){
    while(edgeLists.size()<from+1 || edgeLists.size()<to+1)
        edgeLists.push_back({});

    edgeLists[from].push_back(std::pair<size_t,double>( to,weight ));
}


std::ostream& operator<<(std::ostream &out, Graph g)
{
    size_t vertexCount = g.edgeLists.size();
    out<<vertexCount<< '\n';
    double *weights = new double[vertexCount];
    for (size_t i = 0; i < vertexCount; i++)
    {
        for (size_t j = 0; j < vertexCount; j++)
            weights[j] = -1;
        for (auto it = g.edgeLists[i].begin(); it != g.edgeLists[i].end(); ++it)
            weights[it->first] = it->second;
        for (size_t j = 0; j < vertexCount; j++)
            out << weights[j] << ' ';
        out << '\n';
    }
    delete [] weights;
    return out;
}

std::ostream& operator <<(std::ostream& out, const Edge& e){
    out << e.from << ' ' << e.to << ' ' << e.weight << std::endl;
    return out;
}

std::ostream& operator <<(std::ostream& out, verFromVerToDistance const& in){
    out << "Max distance between " << in.first.first << " " << in.first.second << " " << in.second << std::endl;
}

const std::vector<std::vector<std::pair<size_t, double> > >& Graph::getEdgeLists() const{
    return edgeLists;
};

void dijkstra(Graph const& g, size_t from, double*& weights){
    int numberOfVertex = g.numberOfVertex();

    bool *visits = new bool[numberOfVertex];

    for (int i =0; i<numberOfVertex;++i){
        weights[i] = INF;
        visits[i] = false;
    }

    weights[from] = 0;

    bool isFinished = false;

    while(!isFinished){

        int curVertex = -1;
        double minNotice = INF;

        for(int i = 0; i<numberOfVertex; ++i){
            if(!visits[i] && weights[i]<minNotice){
                curVertex = i;
                minNotice = weights[i];
            }
        }
        if(curVertex<0){
            isFinished=true;
        }
        if(!isFinished){
            auto edges = g.getEdges(curVertex);
            for(int i =0; i<edges.size(); ++i){
                if(weights[edges[i].first]>weights[curVertex]+edges[i].second)
                    weights[edges[i].first]=weights[curVertex]+edges[i].second;
            }
            visits[curVertex]=true;
        }
    }

    delete [] visits;
}

void bellman(Graph const& g, size_t from, double*& weights){
    const size_t numberOfVertex = g.numberOfVertex();
    for(int i = 0; i<numberOfVertex ; ++i)
        weights[i]=INF;
    weights[from] = 0;

    for(int i = 0; i < numberOfVertex - 2; ++i){
        for(int j=0; j<numberOfVertex; ++j){
            if(weights[j] != INF){
                for(int q=0; q<g.getEdgeLists()[j].size(); ++q) {
                    if(weights[g.getEdgeLists()[j][q].first] > weights[j] + g.getEdgeLists()[j][q].second)
                        weights[g.getEdgeLists()[j][q].first] = weights[j] + g.getEdgeLists()[j][q].second;
                }
            }
        }
    }
}

void dijkstraHeap(Graph const& g, size_t from, double*& weights) {
    int numberOfVertex = g.numberOfVertex();
    notesHeap notes;
    for (int i =0; i<numberOfVertex;++i){
        weights[i] = INF;
    }
    weights[from] = 0;
    notes.insert({from,0});

    while (notes.size()){
        auto current = notes.begin();
        size_t curVertex = current->first;
        notes.erase(notes.begin());
        auto edges = g.getEdges(curVertex);
        for(int i=0; i<edges.size(); ++i){
            if(weights[edges[i].first]>weights[curVertex]+edges[i].second) {
                weights[edges[i].first] = weights[curVertex] + edges[i].second;
                notes.insert({edges[i].first, weights[edges[i].first]});
            };
        }
    }
}

void fastDijkstraHeap(Graph const& g, size_t from, double*& weights) {
    size_t numberOfVertex = g.numberOfVertex();
    notesHeap notes;

    weights[from] = 0;
    notes.insert({from,0});
    while (notes.size()){
        auto current = notes.begin();
        size_t curVertex = current->first;
        notes.erase(notes.begin());
        auto edges = g.getEdges(curVertex);
        for(int i=0; i<edges.size(); ++i){
            if(weights[edges[i].first]>weights[curVertex]+edges[i].second) {
                weights[edges[i].first] = weights[curVertex] + edges[i].second;
                notes.insert({edges[i].first, weights[edges[i].first]});
            };
        }
    }
}
void floyd(Graph const& g, verFromVerToDistance& result, double** f){
    size_t numberOfVertex = g.numberOfVertex();

    double **m = new double*[g.numberOfVertex()];
    for (int i = 0; i < g.numberOfVertex(); ++i) {
        m[i] = new double[g.numberOfVertex()];
    }
    for (int i = 0; i < numberOfVertex; ++i) {
        for (int j = 0; j < numberOfVertex; ++j) {
            if(i==j)
                m[i][i]=0;
            else
                m[i][j]=INF;
        }
    }
    for (int i = 0; i < numberOfVertex; ++i) {
        for (int j = 0; j < numberOfVertex; ++j) {
            auto edges = g.getEdges(i);
            for (int k = 0; k < edges.size(); ++k) {
                m[i][edges[k].first] = edges[k].second;
            }
        }
    }
    for (int k = 0; k < numberOfVertex; ++k) {
        for (int i = 0; i < numberOfVertex; ++i) {
            for (int j = 0; j < numberOfVertex; ++j) {
                if(m[i][j] > m[i][k]+m[k][j])
                    m[i][j] = m[i][k]+m[k][j];
            }
        }
    }
    result.first.first = 0;
    result.first.second= 0;
    result.second = 0;
    for (size_t i = 0; i < numberOfVertex; ++i) {
        for (size_t j = 0; j < numberOfVertex; ++j) {
            if(m[i][j] > result.second){
                result.first.first = i;
                result.first.second= j;
                result.second = m[i][j];
            }
        }
    }
}

void naiveDijkstraForAll(Graph const& g, verFromVerToDistance& res){
    res.second = 0;
    double *weights = new double[g.numberOfVertex()];
    for (size_t i = 0; i < g.numberOfVertex(); ++i) {
        dijkstra(g, i, weights);
        auto cur = getMax(weights, g.numberOfVertex());
        if(cur.second > res.second){
            res.first.first = i;
            res.first.second= cur.first;
            res.second = cur.second;
        }
    }
}

void naiveDijkstraHeapForAll(Graph const& g, verFromVerToDistance& res){
    res.second = 0;
    double *weights = new double[g.numberOfVertex()];
    for (size_t i = 0; i < g.numberOfVertex(); ++i) {
        dijkstraHeap(g, i, weights);
        auto cur = getMax(weights, g.numberOfVertex());
        if(cur.second > res.second){
            res.first.first = i;
            res.first.second= cur.first;
            res.second = cur.second;
        }
    }
}

void bellmanForAll(Graph const& g, verFromVerToDistance& res){
    res.second = 0;
    double *weights = new double[g.numberOfVertex()];
    for (size_t i = 0; i < g.numberOfVertex(); ++i) {
        bellman(g, i, weights);
        auto cur = getMax(weights, g.numberOfVertex());
        if(cur.second > res.second){
            res.first.first = i;
            res.first.second= cur.first;
            res.second = cur.second;
        }
    }
}

void fastDijkstraForAll(Graph const& g, verFromVerToDistance& res){
    res.second = 0;
    double *weights = new double[g.numberOfVertex()];
    for (int i =0; i<g.numberOfVertex();++i){
        weights[i] = INF;
    }

    for (size_t i = 0; i < g.numberOfVertex(); ++i) {
        fastDijkstraHeap(g, i, weights);
        auto cur = getMax(weights, g.numberOfVertex());
        if(cur.second > res.second){
            res.first.first = i;
            res.first.second= cur.first;
            res.second = cur.second;
        }
    }
}

void fastesDijkstraForAll(Graph const& g, verFromVerToDistance& res){
    cl_device_id device;
    cl_context context;
    ClInit::init(&device, &context);
    cl_command_queue commandQueue =
            clCreateCommandQueueWithProperties(
                    context, device, NULL, NULL);
}