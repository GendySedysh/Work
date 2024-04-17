#pragma once

#include <sstream>
#include <string>
#include <iostream>
#include "transport_catalogue.h"

namespace catalogue_usage {

	class Controller
	{
	public:
		Controller(catalogue::TransportCatalogue catalogue);
		~Controller();

		void LoadData(std::istream& input);
		void ExecuteRequests(std::istream& input, std::ostream& out);

	private:
		catalogue::TransportCatalogue catalogue_;

		catalogue::Stop ParseStop_(std::string& str);
		catalogue::Bus ParseBus_(std::string& str);
		std::string ParseSubstring(std::string& str, const char& delim, const size_t& n);
		std::vector<catalogue::Stop*> ParseStops(std::string& str, const char& delim);

		std::string FindBus(std::string& str);
		std::string FindStop(std::string& str);
	};
}