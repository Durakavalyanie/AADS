#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

namespace Const {
const int kSize = 9;
const int kInf = 1e9;
std::vector<int> k_goal = {1, 2, 3, 4, 4 + 1, 4 + 2, 4 + 3, 4 + 4, 0};
}  // namespace Const

template <class Vertex>
struct Edge {
  Edge(const Vertex& from, const Vertex& vert_to, char move)
      : vertices_(from, vert_to), move_(move) {}

  const Vertex& GetStart() const { return vertices_.first; }
  const Vertex& GetTarget() const { return vertices_.second; }
  int GetWeight() const { return weight_; }
  char GetMove() const { return move_; }

 private:
  std::pair<Vertex, Vertex> vertices_;
  int weight_ = 1;
  char move_;
};

template <class Vertex>
int GetZeroPlace(const Vertex& arr) {
  int res = -1;
  for (int i = 0; i < Const::kSize; ++i) {
    if (arr[i] == 0) {
      res = i;
      break;
    }
  }
  return res;
}

template <typename Vertex = int>
class Graph {
 public:
  using VertexT = Vertex;
  using EdgeT = Edge<VertexT>;

  std::vector<EdgeT> GetOutgoingEdges(const Vertex& vertex) {
    int zero_place = GetZeroPlace(vertex);
    int left_place = zero_place - 1;
    int right_place = zero_place + 1;
    int up_place = zero_place + 3;
    int down_place = zero_place - 3;
    Vertex new_vert = vertex;
    std::vector<EdgeT> res;
    if (zero_place % 3 != 0 && left_place >= 0) {
      std::swap(new_vert[zero_place], new_vert[left_place]);
      res.push_back({vertex, new_vert, 'L'});
      std::swap(new_vert[zero_place], new_vert[left_place]);
    }
    if (right_place < Const::kSize && zero_place % 3 != 2) {
      std::swap(new_vert[zero_place], new_vert[right_place]);
      res.push_back({vertex, new_vert, 'R'});
      std::swap(new_vert[zero_place], new_vert[right_place]);
    }
    if (down_place < Const::kSize && down_place >= 0) {
      std::swap(new_vert[zero_place], new_vert[down_place]);
      res.push_back({vertex, new_vert, 'D'});
      std::swap(new_vert[zero_place], new_vert[down_place]);
    }
    if (up_place < Const::kSize && up_place >= 0) {
      std::swap(new_vert[zero_place], new_vert[up_place]);
      res.push_back({vertex, new_vert, 'U'});
      std::swap(new_vert[zero_place], new_vert[up_place]);
    }
    return res;
  }
};

template <class Graph>
class AbstractVisitor {
 public:
  virtual void EdgeRelaxed(const Graph::EdgeT& edge) = 0;
  virtual ~AbstractVisitor() = default;
};

template <class Graph>
class GameVisitor : public AbstractVisitor<Graph> {
 public:
  using VertexT = Graph::VertexT;

  void Start(const Graph::VertexT& vertex) { dist_[vertex] = 0; }

  void EdgeRelaxed(const Graph::EdgeT& edge) {
    dist_[edge.GetTarget()] = dist_[edge.GetStart()] + edge.GetWeight(),
    dist_[edge.GetTarget()];
    prev_[edge.GetTarget()] = edge.GetMove();
  }

  int GetDist(const Graph::VertexT& vertex) const {
    if (dist_.contains(vertex)) {
      return dist_.at(vertex);
    }
    return Const::kInf;
  }

  void PrintAnswer(const Graph::VertexT& start) {
    VertexT end = Const::k_goal;
    std::string ans;
    int zero_place;
    int new_place;
    char cur;
    while (end != start) {
      zero_place = GetZeroPlace(end);
      cur = prev_[end];
      if (cur == 'L') {
        new_place = zero_place + 1;
        ans += cur;
      }
      if (cur == 'R') {
        new_place = zero_place - 1;
        ans += cur;
      }
      if (cur == 'D') {
        new_place = zero_place + 3;
        ans += "U";
      }
      if (cur == 'U') {
        new_place = zero_place - 3;
        ans += "D";
      }
      std::swap(end[zero_place], end[new_place]);
    }
    std::reverse(ans.begin(), ans.end());
    std::cout << ans;
  }

 private:
  std::map<VertexT, int> dist_;
  std::map<VertexT, char> prev_;
};

template <class Graph>
int Evristika(const typename Graph::VertexT& arr) {
  int res = 0;
  for (size_t i = 0; i < arr.size() - 1; ++i) {
    if (static_cast<size_t>(arr[i]) != i) {
      ++res;
    }
  }
  if (arr[Const::kSize - 1] != 0) {
    ++res;
  }
  return res;
}

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
      if (visitor.GetDist(neighbour.GetTarget()) == Const::kInf ||
          visitor.GetDist(neighbour.GetTarget()) >
              visitor.GetDist(neighbour.GetStart()) + neighbour.GetWeight()) {
        visitor.EdgeRelaxed(neighbour);
        used.insert({visitor.GetDist(neighbour.GetTarget()) +
                         Evristika<Graph>(neighbour.GetTarget()),
                     neighbour.GetTarget()});
      }
    }
  }
}

int Inversions(const std::vector<int>& arr) {
  int res = 0;
  for (size_t i = 0; i < arr.size(); ++i) {
    if (arr[i] == 0) {
      continue;
    }
    for (size_t j = i; j < arr.size(); ++j) {
      if (arr[j] == 0) {
        continue;
      }
      if (arr[i] > arr[j]) {
        ++res;
      }
    }
  }
  return res;
}

int main() {
  std::vector<int> start(Const::kSize);
  for (int i = 0; i < Const::kSize; ++i) {
    std::cin >> start[i];
  }
  if (Inversions(start) % 2 == 1) {
    std::cout << -1;
    return 0;
  }
  Graph<std::vector<int>> graph;
  GameVisitor<Graph<std::vector<int>>> visitor;
  Dijkstra(graph, start, visitor);
  visitor.PrintAnswer(start);
}