#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    // Реализуйте эту функцию

    string operation_code;
    cin >> operation_code;

    if (operation_code == "NEW_BUS"s) {
        q.type = QueryType::NewBus;
        is >> q.bus;
        
        int stop_count;
        is >> stop_count;

        q.stops.resize(stop_count); //или clear?
        for (string& stop : q.stops) {
            is >> stop;
            //q.stops.push_back(stop); 
        }
    } else if (operation_code == "BUSES_FOR_STOP"s) {
        q.type = QueryType::BusesForStop;
        is >> q.stop; 
    } else if (operation_code == "STOPS_FOR_BUS"s) {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else if (operation_code == "ALL_BUSES"s) {
        q.type = QueryType::AllBuses;        
    }    

    return is;
}

struct BusesForStopResponse {
    // Наполните полями эту структуру
    vector<string> buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    // Реализуйте эту функцию
            if (r.buses.size() == 0) {
                os << "No stop"s; // << endl;
            } else {
                bool is_first = true;
                for (const string& bus : r.buses) {
                    if (!is_first) {
                        os << " "s;
                    }
                    is_first = false;
                    os << bus;
                }
                //os << endl;
            }
    
    
    return os;
}

struct StopsForBusResponse {
    // Наполните полями эту структуру
    vector<pair<string, vector<string>>> stops;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию
            if (r.stops.size() == 0) {
                os << "No bus"s;// << endl;
            } else {
                
                bool first = true;
                
                for (const auto& stop : r.stops) {
                    if (first) {                       
                        first = false;
                    } else {
                        os << endl;                        
                    }
                    
                    os << "Stop "s << stop.first << ":"s;

                    if (stop.second.size() == 0) {
                        os << " no interchange"s;
                    } else {
                        for (const auto& other_bus : stop.second) {
                            //if (bus != other_bus) {
                                os << " "s << other_bus;
                            //}
                        }
                    }
//                    os << endl;
                }
            }
    return os;
}

struct AllBusesResponse {
    // Наполните полями эту структуру
    map<string, vector<string>> buses_to_stops;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    // Реализуйте эту функцию
     if (r.buses_to_stops.empty()) {
         os << "No buses"s;// << endl;
     } else {
         bool first = true;
         
         for (const auto& bus_item : r.buses_to_stops) {
         if (first) {
             first = false;
         } else {
             os << endl;
         }
             
             
             os << "Bus "s << bus_item.first << ":"s;
             for (const string& stop : bus_item.second) {
                 os << " "s << stop;
             }
             //os << endl;
        }
    }
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        // Реализуйте этот метод
        buses_to_stops[bus] = stops; //это надо?
        for (const string& stop : stops) {
            stops_to_buses[stop].push_back(bus);
        }        
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
       // cout << "GetBusesForStops"s << endl;
        
        // Реализуйте этот метод
        BusesForStopResponse result;
        //гарантия ненулевых данных?
        if (stops_to_buses.size() > 0) {
            for (const string& bus : stops_to_buses.at(stop)) { //не создавать stop
                result.buses.push_back(bus);
            }
        }        
                  
        return result;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        
  //      cout << "GetStopsForBus"s << endl;
        
        // Реализуйте этот метод
        StopsForBusResponse result;
        
    
            if (buses_to_stops.count(bus) != 0) {
                for (const string& stop : buses_to_stops.at(bus)) {
                    pair<string, vector<string>> stop_to_buses;
                    
                    stop_to_buses.first = stop;
                  
                    if (stops_to_buses.at(stop).size() > 1) {
                        for (const string& other_bus : stops_to_buses.at(stop)) {
                            if (bus != other_bus) {
                                stop_to_buses.second.push_back(other_bus);
                            }
                        }
                    }

                    result.stops.push_back(stop_to_buses);
                }
            }
    
        return result;
    }

    AllBusesResponse GetAllBuses() const {
    //    cout << "GetAllBuses"s << endl;
        // Реализуйте этот метод
        AllBusesResponse result;
        result.buses_to_stops = buses_to_stops;
        return result;    
    }
    
private:    
    map<string, vector<string>> buses_to_stops, stops_to_buses;
};

// Реализуйте функции и классы, объявленные выше, чтобы эта функция main
// решала задачу "Автобусные остановки"

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }

}