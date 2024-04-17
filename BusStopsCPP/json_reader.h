#pragma once
#include <sstream>
#include <string>
#include <iostream>

#include "transport_catalogue.h"
#include "transport_router.h"
#include "json.h"
#include "domain.h"
#include "map_renderer.h"
#include "json_builder.h"

namespace json {
	using namespace domain;

	class JsonHandler
	{
		using Parsed = std::variant<Stop, AddBus>;

	public:
		JsonHandler() = default;
		JsonHandler(catalogue::TransportCatalogue* catalogue);
		~JsonHandler() {}

		void LoadJSON(std::istream& input);
		void ExecuteJSON(std::ostream& out);

	private:
		catalogue::TransportCatalogue* catalogue_;
		json::Node requests_;
		renderer::MapRenderer renderer_;
		router::TransportRouter router_;

		Parsed PareseNode(const json::Node& data);

		Bus ParseBus(AddBus& data);
		AddBus ParseBus(const json::Node& data);

		Stop ParseStop(const json::Node& data);

		json::Node ExecuteStop(const json::Node& data);
		json::Node ExecuteBus(const json::Node& data);
		json::Node ExecuteMap(const json::Node& data);
	};
}