//
// Created by ivanshport on 22.12.18.
//
#include <vector>

#ifndef IGRAPH_H
#define IGRAPH_H

struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};


#endif //IGRAPH_H