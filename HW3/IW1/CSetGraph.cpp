//
// Created by ivanshport on 22.12.18.
//

#include "CSetGraph.h"

CSetGraph::CSetGraph(unsigned int node_count):
        node_count(node_count),
        in(node_count),
        out(node_count)
{}

CSetGraph::CSetGraph(const IGraph *igraph):
        node_count(igraph->VerticesCount()),
        in(node_count),
        out(node_count)
{
    std::vector<int> nodes;
    for (int i = 0; i < node_count; i++) {
        nodes = igraph->GetNextVertices(i);
        for (auto &j : nodes) {
            AddEdge(i, j);
        }
    }
}

void CSetGraph::AddEdge(int from, int to){
    in[to].insert(from);
    out[from].insert(to);
}

int CSetGraph::VerticesCount() const {
    return node_count;
}

std::vector<int> CSetGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;

    for (auto &i : out[vertex]) {
        result.push_back(i);
    }

    return result;
}

std::vector<int> CSetGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;

    for (auto &i : in[vertex]) {
        result.push_back(i);
    }

    return result;
}