#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <vector>
#include <set>
#include <iostream>
#include "random.hpp"

#define INF 0xFFFFFFFF
#define RANGE 0xFFFFF

struct adjacencyMatrix;

template <class T>
struct VertexToNote : public std::pair<size_t, T>{
    VertexToNote(const size_t& vertex, const T& weight ) : std::pair<size_t, T>(vertex, weight){}
};

template <class T>
struct VertexToNoteLess : public std::less<VertexToNote<T>>{
    constexpr bool operator()(const VertexToNote<T>& a, const VertexToNote<T>& z){
        return a.second < z.second ? true : a.second > z.second ? false : a.first < z.first;
    }
};

typedef std::set<VertexToNote<double>, VertexToNoteLess<double>>  notesHeap;
typedef std::pair<std::pair<size_t, size_t>,double> verFromVerToDistance;

std::ostream& operator <<(std::ostream& out, verFromVerToDistance const& in);

struct Edge{
    size_t from;
    size_t to;
    double weight;

    Edge(size_t from, size_t to, double weight);
    bool operator<=(Edge& b) const;

    friend std::ostream& operator <<(std::ostream &out, const Edge& e);
};

struct Graph{
    Graph(const char* filename);
    Graph(double **m, size_t size);
    Graph();
    std::vector<Edge> getVectorEdges(bool oriented) const;
    size_t numberOfVertex() const;
    std::vector<std::pair<size_t, double> > const& getEdges(int from) const;
    void addEdge(size_t from, size_t to, double weight);
    const std::vector<std::vector<std::pair<size_t, double> > >& getEdgeLists() const;
    Graph(adjacencyMatrix const& am);
private:
    std::vector<std::vector<std::pair<size_t, double> > > edgeLists;
    friend std::ostream& operator <<(std::ostream &out, Graph g);
};

void dijkstra(Graph const& g, size_t from, double*& weights);
void bellman(Graph const& g, size_t from, double*& weights);
void dijkstraHeap(Graph const& g, size_t from, double*& weights);
void floyd(Graph const& g, verFromVerToDistance& res, double** m);
void naiveDijkstraForAll(Graph const& g, verFromVerToDistance& res);
void naiveDijkstraHeapForAll(Graph const& g, verFromVerToDistance& res);
void bellmanForAll(Graph const& g, verFromVerToDistance& res);


struct adjacencyMatrix{
    adjacencyMatrix(size_t size);
    adjacencyMatrix(size_t size, size_t dense);
    size_t getNumberOfEdges() const;
    double getDense() const;
    ~adjacencyMatrix();

    friend Graph;
private:
    size_t size;
    double **m;
    size_t numberOfEdges;
    friend std::ostream& operator <<(std::ostream &out, adjacencyMatrix const& am);
};
#endif