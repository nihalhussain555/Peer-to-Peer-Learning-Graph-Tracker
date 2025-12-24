#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <chrono>

using namespace std;

class PeerGraph {
private:
    // Adjacency list representation: peer -> list of connected peers
    map<string, list<string>> adjList;
    // Track node metadata (connection count, timestamp)
    map<string, pair<int, long long>> nodeMetadata;
    // Track edges with weights (frequency/quality)
    map<pair<string, string>, int> edgeWeights;
public:
  
    void addConnection(const string& peer1, const string& peer2, int weight = 1) {
        adjList[peer1].push_back(peer2);
        adjList[peer2].push_back(peer1);
        
        // Initialize nodes if they don't exist
        if (nodeMetadata.find(peer1) == nodeMetadata.end()) {
            nodeMetadata[peer1] = {0, getCurrentTimestamp()};
        }
        if (nodeMetadata.find(peer2) == nodeMetadata.end()) {
            nodeMetadata[peer2] = {0, getCurrentTimestamp()};
        }
        
        // Store edge weight
        edgeWeights[{peer1, peer2}] = weight;
        edgeWeights[{peer2, peer1}] = weight;
        
        cout << "✓ Connected: " << peer1 << " <-> " << peer2 << endl;
    }
    
    void removeConnection(const string& peer1, const string& peer2) {
        adjList[peer1].remove(peer2);
        adjList[peer2].remove(peer1);
        edgeWeights.erase({peer1, peer2});
        edgeWeights.erase({peer2, peer1});
        cout << "✓ Disconnected: " << peer1 << " <-> " << peer2 << endl;
    }
    
    void printGraph() const {
        cout << "\n" << string(60, '=') << endl;
        cout << "PEER-TO-PEER LEARNING GRAPH" << endl;
        cout << string(60, '=') << endl;
        
        if (adjList.empty()) {
            cout << "Graph is empty!" << endl;
            return;
        }
        
        for (const auto& node : adjList) {
            cout << "\n[" << node.first << "] → ";
            
            for (const auto& connectedPeer : node.second) {
                int weight = edgeWeights.at({node.first, connectedPeer});
                cout << connectedPeer << "(w:" << weight << ") ";
            }
            cout << endl;
        }
        cout << string(60, '=') << "\n" << endl;
    }
    
    vector<string> findSkillPath(const string& source, const string& target) {
        if (adjList.find(source) == adjList.end() || 
            adjList.find(target) == adjList.end()) {
            return {};
        }
        
        map<string, string> parent;
        queue<string> q;
        set<string> visited;
        
        q.push(source);
        visited.insert(source);
        parent[source] = "";
        
        while (!q.empty()) {
            string current = q.front();
            q.pop();
            
            if (current == target) {
                // Reconstruct path
                vector<string> path;
                string node = target;
                while (node != "") {
                    path.push_back(node);
                    node = parent[node];
                }
                reverse(path.begin(), path.end());
                return path;
            }
            
            for (const auto& neighbor : adjList[current]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }
        
        return {}; // No path found
    }
    void detectCommunities() {
        map<string, bool> visited;
        int communityNum = 0;
        
        cout << "\n📍 LEARNING COMMUNITIES DETECTED:\n" << endl;
        
        for (const auto& node : adjList) {
            if (!visited[node.first]) {
                communityNum++;
                vector<string> community = dfsHelper(node.first, visited);
                
                cout << "Community " << communityNum << ": ";
                for (const auto& peer : community) {
                    cout << peer << " ";
                }
                cout << "\n  Size: " << community.size() << " learners\n" << endl;
            }
        }
    }
    
    void calculateCentrality() {
        cout << "\n" << string(60, '=') << endl;
        cout << "INFLUENCE RANKING (Node Centrality)" << endl;
        cout << string(60, '=') << "\n" << endl;
        
        // Degree centrality
        vector<pair<string, int>> centrality;
        
        for (const auto& node : adjList) {
            centrality.push_back({node.first, node.second.size()});
        }
        
        // Sort by degree (descending)
        sort(centrality.begin(), centrality.end(), 
             [](const auto& a, const auto& b) {
                 return a.second > b.second;
             });
        
        cout << "🌟 KEY MENTORS & KNOWLEDGE HUBS:\n" << endl;
        for (int i = 0; i < centrality.size(); i++) {
            cout << fixed << setprecision(2);
            cout << (i+1) << ". " << centrality[i].first 
                 << " | Connections: " << centrality[i].second
                 << " | Influence Score: " << (100.0 * centrality[i].second / adjList.size()) << "%"
                 << endl;
        }
        cout << "\n" << string(60, '=') << "\n" << endl;
    }
    
    int simulatePropagation(const string& source, int hopsLimit = -1) {
        if (adjList.find(source) == adjList.end()) {
            return 0;
        }
        
        map<string, int> distance;
        queue<string> q;
        q.push(source);
        distance[source] = 0;
        int reached = 0;
        
        cout << "\n📡 KNOWLEDGE PROPAGATION FROM: " << source << "\n" << endl;
        
        while (!q.empty()) {
            string current = q.front();
            q.pop();
            int dist = distance[current];
            
            if (hopsLimit != -1 && dist >= hopsLimit) continue;
            
            cout << "  Hop " << dist << ": " << current << endl;
            reached++;
            
            for (const auto& neighbor : adjList[current]) {
                if (distance.find(neighbor) == distance.end()) {
                    distance[neighbor] = dist + 1;
                    q.push(neighbor);
                }
            }
        }
        
        cout << "\n  Total nodes reached: " << reached << " / " << adjList.size() << endl;
        cout << string(60, '=') << "\n" << endl;
        
        return reached;
    }
    
    void getNetworkStats() {
        cout << "\n" << string(60, '=') << endl;
        cout << "NETWORK STATISTICS" << endl;
        cout << string(60, '=') << endl;
        
        cout << "Total Nodes (Peers): " << adjList.size() << endl;
        cout << "Total Edges (Connections): " << edgeWeights.size() / 2 << endl;
        
        if (!adjList.empty()) {
            double avgDegree = 0;
            for (const auto& node : adjList) {
                avgDegree += node.second.size();
            }
            avgDegree /= adjList.size();
            
            cout << "Average Degree (Connections per Peer): " << fixed << setprecision(2) << avgDegree << endl;
            
            // Find density
            int maxEdges = (adjList.size() * (adjList.size() - 1)) / 2;
            double density = (double)(edgeWeights.size() / 2) / maxEdges;
            cout << "Graph Density: " << (density * 100) << "%" << endl;
        }
        
        cout << string(60, '=') << "\n" << endl;
    }
    
    vector<string> getAllPeers() const {
        vector<string> peers;
        for (const auto& node : adjList) {
            peers.push_back(node.first);
        }
        return peers;
    }

    vector<string> getConnections(const string& peer) const {
        if (adjList.find(peer) != adjList.end()) {
            return vector<string>(adjList.at(peer).begin(), adjList.at(peer).end());
        }
        return {};
    }

private:
  
    vector<string> dfsHelper(const string& node, map<string, bool>& visited) {
        vector<string> community;
        queue<string> q;
        q.push(node);
        visited[node] = true;
        
        while (!q.empty()) {
            string current = q.front();
            q.pop();
            community.push_back(current);
            
            for (const auto& neighbor : adjList[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        
        return community;
    }
    
    long long getCurrentTimestamp() const {
        return chrono::system_clock::now().time_since_epoch().count();
    }
};

int main() {
    cout << "\n╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║   PEER-TO-PEER LEARNING GRAPH TRACKER - C++ Implementation ║" << endl;
    cout << "║         Decentralized Knowledge Mapping System             ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝\n" << endl;
    
    PeerGraph network;
    cout << "Demo 1: Building Learning Network...\n" << endl;
    
    network.addConnection("Alice", "Bob", 3);
    network.addConnection("Alice", "Charlie", 2);
    network.addConnection("Bob", "Charlie", 4);
    network.addConnection("Charlie", "David", 2);
    network.addConnection("David", "Eve", 3);
    network.addConnection("Eve", "Frank", 2);
    network.addConnection("Bob", "Frank", 1);
    network.addConnection("Alice", "David", 2);
    
    network.printGraph();
    
    // Demo 2: Network Statistics 
    network.getNetworkStats();
    
    //  Demo 3: Skill Pathing (Shortest Path) 
    cout << "\nDemo 2: Finding Optimal Skill Path...\n" << endl;
    vector<string> path = network.findSkillPath("Alice", "Frank");
    
    if (!path.empty()) {
        cout << "🎯 Shortest learning path from Alice to Frank:\n   ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << " → ";
        }
        cout << "\n   Length: " << (path.size() - 1) << " steps\n" << endl;
    }
    
    //  Demo 3: Community Detection 
    cout << "Demo 3: Community Detection...\n" << endl;
    network.detectCommunities();
    
    // Demo 4: Influence Ranking 
    cout << "Demo 4: Calculate Influence Scores...\n" << endl;
    network.calculateCentrality();
    
    // Demo 5: Knowledge Propagation
    cout << "Demo 5: Simulate Knowledge Propagation...\n" << endl;
    network.simulatePropagation("Alice", 3);
    
    // Demo 6: Node Removal and Updates
    cout << "Demo 6: Dynamic Updates...\n" << endl;
    cout << "Adding new connection: Eve <-> Grace\n" << endl;
    network.addConnection("Eve", "Grace", 2);
    network.printGraph();
    
    cout << "\n╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                  Demo Complete!                            ║" << endl;
    cout << "║   All P2P Learning Graph operations executed successfully  ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝\n" << endl;
    
    return 0;
}
