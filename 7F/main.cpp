#include <cmath>
#include <iostream>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

template <class Vertex>
struct Edge {
  Edge(const Vertex& from, const Vertex& vert_to) : vertices_(from, vert_to) {}

  const Vertex& GetStart() const { return vertices_.first; }

  const Vertex& GetTarget() const { return vertices_.second; }

 private:
  std::pair<Vertex, Vertex> vertices_;
};

template <typename Vertex = int>
class Graph {
 public:
  using VertexT = Vertex;
  using EdgeT = Edge<VertexT>;

  Graph() = default;

  Graph(const std::vector<EdgeT>& edges) : amount_of_edjes_(edges.size()) {
    for (const auto& edge : edges) {
      adjacent_[edge.GetStart()].push_back(edge);
    }
  }

  VertexT GetFirstVertex() const { return (adjacent_.begin())->first; }

  const std::vector<EdgeT>& GetOutgoingEdges(const Vertex& vertex) {
    return adjacent_[vertex];
  }

  size_t GetAmountOfVertexes() const { return adjacent_.size(); }

  size_t GetAmountOfEdges() const { return amount_of_edjes_; }

 protected:
  std::unordered_map<VertexT, std::vector<EdgeT>> adjacent_;
  size_t amount_of_edjes_;
};

template <typename Vertex = int>
class RootedGraph : public Graph<Vertex> {
 public:
  RootedGraph() = default;

  RootedGraph(const std::vector<Edge<Vertex>>& edges) : Graph<Vertex>(edges) {}

  Vertex GetAncestor(const Vertex& vert) const { return ancestors_.at(vert); }

  int GetDepth(const Vertex& vert) const { return depth_.at(vert); }

  void SetRoot(const Vertex& root) {
    root_ = root;
    ancestors_[root_] = root_;
    depth_[root_] = 1;
    FillAncestors(root);
  }

 private:
  void FillAncestors(Vertex cur, int depth = 2) {
    for (auto edge : Graph<Vertex>::adjacent_[cur]) {
      if (!ancestors_.contains(edge.GetTarget())) {
        ancestors_[edge.GetTarget()] = cur;
        depth_[edge.GetTarget()] = depth;
        FillAncestors(cur, depth + 1);
      }
    }
  }

  Vertex root_;
  std::unordered_map<Vertex, Vertex> ancestors_;
  std::unordered_map<Vertex, int> depth_;
};

int LCA(RootedGraph<int>& graph, std::vector<std::vector<int>>& ups, int log,
        int first, int second) {
  if (graph.GetDepth(first) > graph.GetDepth(second)) {
    std::swap(first, second);
  }
  for (int i = log - 1; i >= 0; --i) {
    if (graph.GetDepth(ups[second][i]) - graph.GetDepth(first) >= 0) {
      second = ups[second][i];
    }
  }
  if (first == second) {
    return first;
  }
  for (int i = log - 1; i >= 0; --i) {
    if (ups[first][i] != ups[second][i]) {
      first = ups[first][i];
      second = ups[second][i];
    }
  }
  return graph.GetAncestor(first);
}

template <typename Vertex>
void Input(RootedGraph<Vertex>& graph) {
  int vertexes;
  std::cin >> vertexes;
  std::vector<Edge<Vertex>> edges;
  Vertex first;
  Vertex second;
  for (int i = 0; i < vertexes - 1; ++i) {
    std::cin >> first >> second;
    edges.push_back(Edge(first, second));
    edges.push_back(Edge(second, first));
  }
  graph = RootedGraph<Vertex>(edges);
  graph.SetRoot(graph.GetFirstVertex());
}

void BinaryUps(RootedGraph<int>& graph, std::vector<std::vector<int>>& ups) {
  for (size_t i = 0; i < graph.GetAmountOfVertexes(); ++i) {
    ups[i][0] = graph.GetAncestor(i);
  }
  for (size_t i = 1; i < ups.begin()->size(); ++i) {
    for (size_t j = 0; j < graph.GetAmountOfVertexes(); ++j) {
      ups[j][i] = ups[ups[j][i - 1]][i - 1];
    }
  }
}

int GetRes(RootedGraph<int>& graph, std::vector<std::vector<int>>& ups, int log,
           int first, int second) {
  int lca = LCA(graph, ups, log, first, second);
  return graph.GetDepth(first) + graph.GetDepth(second) -
         2 * graph.GetDepth(lca);
}

void Solve(RootedGraph<int>& graph, std::vector<int>& ans) {
  int log = log2(graph.GetAmountOfVertexes()) + 1;
  std::vector<std::vector<int>> ups(graph.GetAmountOfVertexes(),
                                    std::vector<int>(log, 0));
  BinaryUps(graph, ups);
  int requests;
  std::cin >> requests;
  int first;
  int second;
  for (; requests > 0; --requests) {
    std::cin >> first >> second;
    ans.push_back(GetRes(graph, ups, log, first - 1, second - 1));
  }
}

void Output(std::vector<int>& ans) {
  for (int elem : ans) {
    std::cout << elem;
  }
}

int main() {
  RootedGraph<int> graph;
  Input(graph);
  std::vector<int> ans;
  Solve(graph, ans);
  Output(ans);
}