//
// Created by ivanshport on 22.12.18.
//

#ifndef CMATRIXGRAPH_H
#define CMATRIXGRAPH_H

#include <iostream>
#include "IGraph.h"

class CMatrixGraph : public IGraph {
public:
    CMatrixGraph(unsigned int node_count);
    CMatrixGraph(const IGraph* igraph);

    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    size_t node_count;
    std::vector<std::vector<bool>> edges;

};

#endif //CMATRIXGRAPH_H
