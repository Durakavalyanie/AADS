#include <iostream>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

namespace UpperBounds {
const int kInf = 1e9;
}

template <class Vertex>
struct Edge {
  Edge(const Vertex& from, const Vertex& vert_to, int weight)
      : vertices_(from, vert_to), weight_(weight) {}

  const Vertex& GetStart() const { return vertices_.first; }
  const Vertex& GetTarget() const { return vertices_.second; }
  int GetWeight() const { return weight_; }

 private:
  std::pair<Vertex, Vertex> vertices_;
  int weight_;
};

template <typename Vertex = int>
class Graph {
 public:
  using VertexT = Vertex;
  using EdgeT = Edge<VertexT>;

  Graph() = default;

  Graph(const std::vector<EdgeT>& edges) {
    for (const auto& edge : edges) {
      adjacent_[edge.GetStart()].push_back(edge);
    }
    amount_of_edjes_ = edges.size();
  }

  VertexT GetFirstVertex() const { return (adjacent_.begin())->first; }

  const std::vector<EdgeT>& GetOutgoingEdges(const Vertex& vertex) {
    return adjacent_[vertex];
  }

  size_t GetAmountOfVertexes() const { return adjacent_.size(); }

  size_t GetAmountOfEdges() const { return amount_of_edjes_; }

 private:
  std::unordered_map<VertexT, std::vector<EdgeT>> adjacent_;
  size_t amount_of_edjes_;
};

template <class Graph>
class AbstractVisitor {
 public:
  virtual void EdgeRelaxed(const Graph::EdgeT& edge) = 0;
  virtual ~AbstractVisitor() = default;
};

template <typename Vertex>
int FindMST(Graph<Vertex>& graph) {
  std::set<std::pair<int, Vertex>> unconnected;
  std::vector<int> dist(graph.GetAmountOfVertexes(), UpperBounds::kInf);
  dist[0] = 0;
  std::unordered_map<Vertex, bool> used;
  unconnected.insert({0, graph.GetFirstVertex()});
  Vertex cur_vert;
  int cur_weight;
  int res = 0;
  while (!unconnected.empty()) {
    cur_vert = unconnected.begin()->second;
    cur_weight = unconnected.begin()->first;
    unconnected.erase(unconnected.begin());
    if (used.contains(cur_vert)) {
      continue;
    }
    used[cur_vert] = true;
    res += cur_weight;
    for (auto edge : graph.GetOutgoingEdges(cur_vert)) {
      if (!used.contains(edge.GetTarget())) {
        unconnected.insert({edge.GetWeight(), edge.GetTarget()});
      }
    }
  }
  return res;
}

template <typename Vertex>
void Input(Graph<Vertex>& graph) {
  int amount_of_vertexes;
  int amount_of_edges;
  std::cin >> amount_of_vertexes >> amount_of_edges;
  std::vector<Edge<int>> edges;
  int first;
  int second;
  int weight;
  for (int i = 0; i < amount_of_vertexes; ++i) {
    std::cin >> first >> second >> weight;
    edges.push_back(Edge(first, second, weight));
    edges.push_back(Edge(second, first, weight));
  }
  graph = Graph(edges);
}

int main() {
  Graph<int> graph;
  Input(graph);
  std::cout << FindMST(graph);
}