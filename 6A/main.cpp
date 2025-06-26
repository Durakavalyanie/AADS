#include <iostream>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

namespace UpperBounds {
const int kInf = 2009000999;
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

template <class Graph>
class ShortestPathsVisitor : public AbstractVisitor<Graph> {
 public:
  using VertexT = Graph::VertexT;

  void Start(const Graph::VertexT& vertex) { dist_[vertex] = 0; }

  void EdgeRelaxed(const Graph::EdgeT& edge) {
    dist_[edge.GetTarget()] = dist_[edge.GetStart()] + edge.GetWeight(),
    dist_[edge.GetTarget()];
  }

  int GetDist(const Graph::VertexT& vertex) {
    if (dist_.contains(vertex)) {
      return dist_[vertex];
    }
    return UpperBounds::kInf;
  }

 private:
  std::unordered_map<VertexT, int> dist_;
};

template <class Graph, class Visitor>
void Dijkstra(Graph& graph, const typename Graph::VertexT& vertex,
              Visitor& visitor) {
  typename Graph::VertexT cur_vert;
  std::set<std::pair<int, typename Graph::VertexT>> used;
  visitor.Start(vertex);
  used.insert({0, vertex});
  while (!used.empty()) {
    cur_vert = used.begin()->second;
    used.erase(used.begin());
    for (const auto& neighbour : graph.GetOutgoingEdges(cur_vert)) {
      if (visitor.GetDist(neighbour.GetTarget()) == UpperBounds::kInf ||
          visitor.GetDist(neighbour.GetTarget()) >
              visitor.GetDist(neighbour.GetStart()) + neighbour.GetWeight()) {
        visitor.EdgeRelaxed(neighbour);
        used.insert(
            {visitor.GetDist(neighbour.GetTarget()), neighbour.GetTarget()});
      }
    }
  }
}

int Input(Graph<int>& graph) {
  int amount_of_edges;
  int amount_of_vertexes;
  std::cin >> amount_of_vertexes >> amount_of_edges;
  std::vector<Edge<int>> edges;
  int start;
  int end;
  int length;
  for (int count = 0; count < amount_of_edges; ++count) {
    std::cin >> start >> end >> length;
    edges.push_back(Edge(start, end, length));
    edges.push_back(Edge(end, start, length));
  }
  std::cin >> start;
  graph = Graph(edges);
  return start;
}

void Output(ShortestPathsVisitor<Graph<int>>& visitor, int amount_of_vertexes) {
  for (int i = 0; i < amount_of_vertexes; ++i) {
    std::cout << visitor.GetDist(i) << ' ';
  }
}

int main() {
  int amount_of_graphs;
  std::cin >> amount_of_graphs;

  for (int i = 0; i < amount_of_graphs; ++i) {
    Graph<int> graph;
    int start = Input(graph);

    ShortestPathsVisitor<Graph<int>> visitor;

    Dijkstra(graph, start, visitor);
    Output(visitor, graph.GetAmountOfVertexes());
  }
}