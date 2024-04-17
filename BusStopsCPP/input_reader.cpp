#include "input_reader.h"

namespace catalogue_usage {

	Controller::Controller(catalogue::TransportCatalogue catalogue) : catalogue_(catalogue) {

	}

	Controller::~Controller() {

	}

	void Controller::ExecuteRequests(std::istream& input, std::ostream& out) {
		std::string tmp;
		// выполнение запросов
		std::getline(input, tmp);
		int n = std::stoi(tmp);
		while (n > 0) {
			std::getline(input, tmp);
			if (tmp[0] == 'B') {
				out << FindBus(tmp);
			}
			else if (tmp[0] == 'S') {
				out << FindStop(tmp);
			}
			--n;
		}
	}

	void Controller::LoadData(std::istream& input) {
		// заполнение БД
		std::string tmp;
		std::vector<std::string> bus_strings;

		std::getline(input, tmp);
		int n = std::stoi(tmp);
		while (n > 0) {
			std::getline(input, tmp);
			if (tmp[0] == 'S') {
				catalogue_.AddStop(ParseStop_(tmp));
			}
			else if (tmp[0] == 'B') {
				bus_strings.push_back(tmp);
			}
			--n;
		}

		for (auto& str : bus_strings) {
			catalogue_.AddBus(ParseBus_(str));
		}
	}

	catalogue::Stop Controller::ParseStop_(std::string& str) {
		std::unordered_map<std::string, int> stopname_to_dist;

		std::string name = ParseSubstring(str, ':', 1);
		std::string coord_1 = ParseSubstring(str, ',', 1);
		std::string coord_2;
		if (str.find(',') == std::string::npos)
		{
			coord_2 = ParseSubstring(str, '\n', 1);
		}
		else
		{
			coord_2 = ParseSubstring(str, ',', 1);

			while (str.find(',') != std::string::npos)
			{
				std::string dist = ParseSubstring(str, 'm', 3);
				std::string stopname = ParseSubstring(str, ',', 1);
				stopname_to_dist[stopname] = std::stoi(dist);
			}
			std::string dist = ParseSubstring(str, 'm', 4);

			std::string stopname = ParseSubstring(str, '\n', 1);
			stopname_to_dist[stopname] = std::stoi(dist);
		}

		return { name, {std::stod(coord_1), std::stod(coord_2)}, stopname_to_dist };
	}

	std::vector<catalogue::Stop*> Controller::ParseStops(std::string& str, const char& delim) {
		size_t start;
		size_t finish;
		std::vector<catalogue::Stop*> stop_names;

		while (str.find_first_of(delim) != std::string::npos)
		{
			start = str.find_first_of(' ') + 1;
			finish = str.find_first_of(delim);
			stop_names.push_back(std::move(catalogue_.GetStopPointer(str.substr(start, finish - start - 1))));
			str.erase(0, finish + 1);
		}
		start = str.find_first_of(' ') + 1;
		finish = str.length();
		stop_names.push_back(std::move(catalogue_.GetStopPointer(str.substr(start, finish - start))));
		return stop_names;
	}

	catalogue::Bus Controller::ParseBus_(std::string& str) {
		std::string name = ParseSubstring(str, ':', 1);
		std::vector<catalogue::Stop*> stop_names;

		if (str.find('>') != std::string::npos) {
			stop_names = std::move(ParseStops(str, '>'));
		}
		else {
			stop_names = std::move(ParseStops(str, '-'));
			auto add_names = stop_names;

			auto it = add_names.rbegin() + 1;
			for (; it < add_names.rend(); ++it) {
				stop_names.push_back(*it);
			}
		}

		return { name, stop_names };
	}

	std::string Controller::ParseSubstring(std::string& str, const char& delim, const size_t& n) {
		size_t start = str.find_first_of(' ') + 1;
		size_t finish;

		if (delim == '\n') {
			finish = str.length();
		}
		else {
			finish = str.find_first_of(delim);
		}

		std::string ret = str.substr(start, finish - start);
		if (delim != '\n') {
			str.erase(0, finish + n);
		}

		return ret;
	}

	std::string Controller::FindBus(std::string& str) {
		std::string name = ParseSubstring(str, '\n', 1);

		std::string ret_str;
		auto bus_data = catalogue_.GetBusData(catalogue_.GetBusPointer(name));
		if (bus_data.is_null) {
			ret_str = "Bus " + name + ": not found\n";
		}
		else {
			std::stringstream ss;
			ss << std::fixed << std::setprecision(5) << bus_data.path_data.curvature;
			std::string curvature = ss.str();

			ret_str = "Bus " + name + ": " +
				std::to_string(bus_data.stops_on_route) + " stops on route, " +
				std::to_string(bus_data.unique_stops) + " unique stops, " +
				std::to_string(bus_data.path_data.real) + " route length, " +
				curvature + " curvature\n";
		}

		return ret_str;
	}

	std::string Controller::FindStop(std::string& str) {
		std::string name = ParseSubstring(str, '\n', 1);

		auto stop_data = catalogue_.GetStopData(catalogue_.GetStopPointer(name));

		std::string ret_str;

		if (stop_data.is_null) {
			ret_str = "Stop " + name + ": not found\n";
		}
		else {
			if (stop_data.num_of_buses == 0) {
				ret_str = "Stop " + name + ": no buses\n";
			}
			else {
				ret_str = "Stop " + name + ": buses";
				for (auto& bus : stop_data.buses) {
					ret_str += " ";
					ret_str += bus;
				}
				ret_str += '\n';
			}
		}
		return ret_str;
	}
}