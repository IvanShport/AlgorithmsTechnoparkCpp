//
// Created by ivanshport on 22.12.18.
//

#include "CListGraph.h"

CListGraph::CListGraph(unsigned int node_count):
        node_count(node_count),
        in(node_count),
        out(node_count)
{}

CListGraph::CListGraph(const IGraph *igraph):
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

void CListGraph::AddEdge(int from, int to){
    if(in.size() < out.size()){
        for(int i : in[to])
            if(i == from)
                return;
    }
    else{
        for(int i : out[from])
            if(i == to)
                return;
    }

    in[to].push_back(from);
    out[from].push_back(to);
}

int CListGraph::VerticesCount() const {
    return node_count;
}

std::vector<int> CListGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;

    for (auto &i : out[vertex]) {
        result.push_back(i);
    }

    return result;
}

std::vector<int> CListGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;

    for (auto &i : in[vertex]) {
        result.push_back(i);
    }

    return result;
}