//
// Created by ivanshport on 22.12.18.
//

#include "CMatrixGraph.h"

CMatrixGraph::CMatrixGraph(unsigned int node_count):
node_count(node_count),
edges(node_count)
{
    for (auto &i : edges) {
        i.assign(node_count, false);
    }
}

CMatrixGraph::CMatrixGraph(const IGraph *igraph):
node_count(igraph->VerticesCount()),
edges(node_count)
{
    for (auto &i : edges) {
        i.assign(node_count, false);
    }
    std::vector<int> nodes;
    for (int i = 0; i < node_count; i++) {
        nodes = igraph->GetNextVertices(i);
        for (auto &j : nodes) {
            AddEdge(i, j);
        }
    }
}

void CMatrixGraph::AddEdge(int from, int to) {
    edges[from][to] = true;
}

int CMatrixGraph::VerticesCount() const {
    return node_count;
}

std::vector<int> CMatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;

    for (int i = 0; i < node_count; i++) {
        if (edges[vertex][i]) {
            result.push_back(i);
        }
    }

    return result;
}

std::vector<int> CMatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;

    for (int i = 0; i < node_count; i++) {
        if (edges[i][vertex]) {
            result.push_back(i);
        }
    }

    return result;
}