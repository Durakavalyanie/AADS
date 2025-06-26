#include <algorithm>
#include <iostream>
#include <vector>

const u_int64_t kHashParam1 = 397;
const u_int64_t kHashParam2 = 42;
const u_int64_t kHashParam3 = 184469479;
const u_int64_t kLeaveHash = 197;

void DFS(int cur_vertex, const std::vector<std::vector<int>>& graph,
         std::vector<int>& subtree_size, std::vector<int>& centroid,
         std::vector<bool>& used) {
  subtree_size[cur_vertex] = 1;
  used[cur_vertex] = true;
  int size = graph.size();
  bool is_centroid = true;
  for (int neighbour : graph[cur_vertex]) {
    if (used[neighbour]) {
      continue;
    }
    DFS(neighbour, graph, subtree_size, centroid, used);
    subtree_size[cur_vertex] += subtree_size[neighbour];

    if (subtree_size[neighbour] > size / 2) {
      is_centroid = false;
    }
  }
  is_centroid = is_centroid && (size - subtree_size[cur_vertex] <= size / 2);

  if (is_centroid) {
    centroid.push_back(cur_vertex);
  }
}

std::vector<int> FindCentroid(int cur_vertex,
                              const std::vector<std::vector<int>>& graph,
                              std::vector<int>& subtree_size) {
  std::vector<int> centroids;
  std::vector<bool> used(graph.size(), false);
  DFS(cur_vertex, graph, subtree_size, centroids, used);
  return centroids;
}

u_int64_t HashTree(int cur_vertex, std::vector<std::vector<int>>& graph,
                   int parent = -1) {
  u_int64_t hash = 0;
  std::vector<std::pair<u_int64_t, int>> children;
  for (int neighbour : graph[cur_vertex]) {
    if (neighbour == parent) {
      continue;
    }
    u_int64_t neighbour_hash = HashTree(neighbour, graph, cur_vertex);
    children.push_back({neighbour_hash, neighbour});
  }

  std::sort(children.begin(), children.end());
  u_int64_t multiplicator = kHashParam1;
  graph[cur_vertex].clear();
  for (auto elem : children) {
    hash +=
        (elem.first * elem.first + elem.first * multiplicator + kHashParam2) %
        kHashParam3;
    multiplicator *= kHashParam1 % kHashParam3;
    graph[cur_vertex].push_back(elem.second);
  }

  if (parent != -1) {
    graph[cur_vertex].push_back(parent);
  }

  return (hash == 0) ? kLeaveHash : hash;
}

void FillNewNums(const std::vector<std::vector<int>>& graph1,
                 const std::vector<std::vector<int>>& graph2, int root1,
                 int root2, std::vector<int>& new_nums) {
  new_nums[root1] = root2;
  for (size_t i = 0; i < graph1[root1].size(); ++i) {
    if (new_nums[graph1[root1][i]] == -1) {
      FillNewNums(graph1, graph2, graph1[root1][i], graph2[root2][i], new_nums);
    }
  }
}

void Input(std::vector<std::vector<int>>& graph1,
           std::vector<std::vector<int>>& graph2) {
  int size;
  std::cin >> size;

  int first;
  int second;

  graph1 = std::vector<std::vector<int>>(size);
  graph2 = std::vector<std::vector<int>>(size);

  for (int i = 0; i < size - 1; ++i) {
    std::cin >> first >> second;
    --first;
    --second;
    graph1[first].push_back(second);
    graph1[second].push_back(first);
  }

  for (int i = 0; i < size - 1; ++i) {
    std::cin >> first >> second;
    --first;
    --second;
    graph2[first].push_back(second);
    graph2[second].push_back(first);
  }
}

int main() {
  std::vector<std::vector<int>> graph1;
  std::vector<std::vector<int>> graph2;
  Input(graph1, graph2);
  int size = graph1.size();

  std::vector<int> subtree_size(size, 0);
  std::vector<int> centroids1 = FindCentroid(0, graph1, subtree_size);

  std::fill(subtree_size.begin(), subtree_size.end(), 0);
  std::vector<int> centroids2 = FindCentroid(0, graph2, subtree_size);

  std::pair<u_int64_t, u_int64_t> hash1 = {HashTree(centroids1[0], graph1), 0};

  if (centroids1.size() == 2) {
    hash1.second = HashTree(centroids1[1], graph1);
    if (hash1.first > hash1.second) {
      std::swap(hash1.first, hash1.second);
      std::swap(centroids1[0], centroids1[1]);
    }
  }

  std::pair<u_int64_t, u_int64_t> hash2 = {HashTree(centroids2[0], graph2), 0};

  if (centroids2.size() == 2) {
    hash2.second = HashTree(centroids2[1], graph2);
    if (hash2.first > hash2.second) {
      std::swap(hash2.first, hash2.second);
      std::swap(centroids2[0], centroids2[1]);
    }
  }

  if (hash1 != hash2) {
    std::cout << -1;
  } else {
    std::vector<int> new_nums(size, -1);
    FillNewNums(graph1, graph2, centroids1[0], centroids2[0], new_nums);
    for (int elem : new_nums) {
      std::cout << elem + 1 << '\n';
    }
  }
}