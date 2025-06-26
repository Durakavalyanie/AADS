#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

enum class Colors {
  KWhite = 0,
  KGray,
  KBlack,
};

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
class AbstractDFSVisitor {
 public:
  virtual void DiscoverVertex(const Graph::VertexT& vertex) = 0;
  virtual void FinishVertex(const Graph::VertexT& vertex) = 0;
  virtual void FinishDFS() = 0;
  virtual Colors GetColor(const Graph::VertexT& vertex) const = 0;
  virtual ~AbstractDFSVisitor() = default;

 protected:
  std::unordered_map<typename Graph::VertexT, Colors> colors_;
};

template <class Graph>
class FindConnectedComponentsVisitor : public AbstractDFSVisitor<Graph> {
 public:
  void DiscoverVertex(const Graph::VertexT& vertex) {
    cur_component_.push_back(vertex);
    BaseClass::colors_[vertex] = Colors::KGray;
  }

  void FinishVertex(const Graph::VertexT& vertex) final {
    BaseClass::colors_[vertex] = Colors::KBlack;
  }

  Colors GetColor(const Graph::VertexT& vertex) const final {
    if (BaseClass::colors_.contains(vertex)) {
      return BaseClass::colors_.at(vertex);
    }
    return Colors::KWhite;
  }

  void FinishDFS() final {
    components_.push_back(cur_component_);
    cur_component_.clear();
  }

  const std::vector<std::vector<typename Graph::VertexT>>& GetComponents()
      const {
    return components_;
  }

 private:
  using BaseClass = AbstractDFSVisitor<Graph>;
  std::vector<typename Graph::VertexT> cur_component_;
  std::vector<std::vector<typename Graph::VertexT>> components_;
};

template <class Graph, class Visitor>
void DFS(Graph& graph, const typename Graph::VertexT& vertex,
         Visitor& visitor) {
  visitor.DiscoverVertex(vertex);
  for (const auto& outgoing_edge : graph.GetOutgoingEdges(vertex)) {
    const auto& neighbour = outgoing_edge.GetTarget();
    if (visitor.GetColor(neighbour) == Colors::KWhite) {
      DFS(graph, neighbour, visitor);
    }
  }
  visitor.FinishVertex(vertex);
}

void PrintAnswer(
    const std::vector<std::vector<typename Graph<int>::VertexT>>& arr) {
  std::cout << arr.size() << '\n';
  for (const auto& elem : arr) {
    std::cout << elem.size() << '\n';
    for (const auto kVertex : elem) {
      std::cout << kVertex << ' ';
    }
    std::cout << '\n';
  }
}

void Input(std::vector<Edge<int>>& edges) {
  int amount_of_edges;
  int amount_of_vertexes;
  std::cin >> amount_of_vertexes >> amount_of_edges;
  int start;
  int end;
  for (int count = 0; count < amount_of_edges; ++count) {
    std::cin >> start >> end;
    edges.push_back(Edge(start, end));
    edges.push_back(Edge(end, start));
  }
}

void Solver(FindConnectedComponentsVisitor<Graph<int>>& visitor,
            std::vector<Edge<int>>& edges) {
  Graph graph(edges);
  for (size_t count = 1; count <= graph.GetAmountOfEdges(); ++count) {
    if (visitor.GetColor(count) != Colors::KWhite) {
      continue;
    }
    DFS(graph, count, visitor);
    visitor.FinishDFS();
  }
}

int main() {
  std::vector<Edge<int>> edges;
  Input(edges);

  FindConnectedComponentsVisitor<Graph<int>> visitor;
  Solver(visitor, edges);

  PrintAnswer(visitor.GetComponents());
}
