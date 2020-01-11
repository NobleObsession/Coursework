#pragma once

#include "Dijkstra.h"

struct Route {
	size_t from;
	size_t to;

	bool operator== (const Route& other) const{
		return ((from == other.from) && (to == other.to));
	}
};

class Cache{

private:
	struct RouteHasher {
		size_t operator()(const Route& route) const{
			size_t r1 = size_t_hash(route.from);
			size_t r2 = size_t_hash(route.to);
			size_t x = 101;
			return (r1*x + r2);
		}
		std::hash<unsigned long long> size_t_hash;
	};

	std::unordered_map<Route, UnordMapNode, RouteHasher> route_cache;
	std::unordered_map<size_t, CachedGraph> graph_cache;

public:

	std::pair<bool, UnordMapNode> FindRoute(const Route& route){
		auto found_route = route_cache.find(route);
		if(found_route != route_cache.end()){
			return std::make_pair(true, found_route->second);
		}else{
			return std::make_pair(false, UnordMapNode());
		}
	}

	std::pair<bool, CachedGraph> FindGraph(size_t from){
		auto found_graph = graph_cache.find(from);
		if(found_graph != graph_cache.end()){
			return std::make_pair(true, found_graph->second);
		}else{
			return std::make_pair(false, CachedGraph());
		}
	}

	auto InsertCachedGraph (size_t from, const CachedGraph& graph){
		return graph_cache.insert({from, graph});
	}

	auto InsertOutputNode (const Route& route, const UnordMapNode& node){
		return route_cache.insert({route, node});
	}

	std::unordered_map<Route, UnordMapNode, RouteHasher> GetRouteCache() const{
		return route_cache;
	}

	std::unordered_map<size_t, CachedGraph>& GetGraphCache(){
		return graph_cache;
	}

	std::unordered_map<size_t, CachedGraph> GetGraphCache() const{
		return graph_cache;
	}

};

