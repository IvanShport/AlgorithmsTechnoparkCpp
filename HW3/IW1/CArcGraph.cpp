//
// Created by ivanshport on 22.12.18.
//

#include "CArcGraph.h"

CArcGraph::CArcGraph(unsigned int node_count):
        node_count(node_count)
{}

CArcGraph::CArcGraph(const IGraph *igraph):
        node_count(igraph->VerticesCount())
{
    std::vector<int> nodes;
    for (int i = 0; i < node_count; i++) {
        nodes = igraph->GetNextVertices(i);
        for (auto &j : nodes) {
            AddEdge(i, j);
        }
    }
}

CArcGraph::Edge::Edge(int from, int to) :
from(from),
to(to)
{}

void CArcGraph::AddEdge(int from, int to) {
    for(auto & i : edges)
        if(i.from == from && i.to == to)
            return;

    Edge edge(from, to);
    edges.push_back(edge);
}

int CArcGraph::VerticesCount() const {
    return node_count;
}

std::vector<int> CArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;

    for(auto & i : edges)
        if(i.from == vertex)
            result.push_back(i.to);

    return result;
}

std::vector<int> CArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;

    for(auto & i : edges)
        if(i.to == vertex)
            result.push_back(i.from);

    return result;
}