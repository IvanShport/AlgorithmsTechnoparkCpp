//
// Created by ivanshport on 22.12.18.
//

#ifndef CSETGRAPH_H
#define CSETGRAPH_H

#include <iostream>
#include <unordered_set>
#include "IGraph.h"

class CSetGraph : public IGraph {
public:
    CSetGraph(unsigned int node_count);

    CSetGraph(const IGraph *igraph);

    virtual void AddEdge(int from, int to) override;

    virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;

    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    size_t node_count;
    std::vector<std::unordered_set<int>> in;

    std::vector<std::unordered_set<int>> out;

};

#endif //CSETGRAPH_H
