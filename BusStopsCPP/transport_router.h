#pragma once
#include "json_reader.h"
#include "graph.h"
#include "router.h"


namespace router {
	class TransportRouter
	{
	public:
		TransportRouter(const json::Node& data) {
			const json::Dict& input_data = data.AsMap();

			bus_velocity_ = input_data.at("bus_velocity").AsInt();
			bus_wait_time_ = input_data.at("bus_wait_time").AsInt();
		}

		graph::DirectedWeightedGraph<double> SetUpGraph() {

		}

		~TransportRouter() {}

	private:
		int bus_velocity_;
		int bus_wait_time_;
	};
}