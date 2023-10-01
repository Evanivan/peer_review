//ebook implementation
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <set>

using namespace std::literals;

class ReadingUsers {
    std::vector<std::int16_t> id_to_page;
    std::vector<std::uint32_t> users_to_page;
    std::vector<double> cheers;
    std::set<std::uint32_t> real_users;
    void PutRead(std::uint32_t id, std::int16_t current_page);
    double CalcShare(std::uint32_t index);
    void PutCheer(std::uint32_t id);
public:
    ReadingUsers()
        : id_to_page(100001), users_to_page(1001){}
    void InputRequests(std::istream& input_stream);
    void OutPut();
};

void ReadingUsers::OutPut() {
    std::cout << std::setprecision(6);
    for (double cheer : cheers) {
        std::cout << cheer << std::endl;
    }
}

double ReadingUsers::CalcShare(std::uint32_t index) {
    auto page = id_to_page.at(index);
    auto readers = users_to_page.at(page);
    auto all_users = real_users.size() - 1;
    auto portion_users = real_users.size() - readers;

    double fin_res = (double )(portion_users) / (double)all_users;
    return fin_res;
}

void ReadingUsers::PutRead(std::uint32_t id, std::int16_t current_page) {
    for (auto i = id_to_page[id] + 1; i <= current_page;){
        ++users_to_page[i];
        ++i;
    }
    id_to_page[id] = current_page;
    real_users.emplace(id);
}

void ReadingUsers::PutCheer(std::uint32_t id) {
    if (real_users.size() == 1 && id_to_page.at(id)) {
        cheers.emplace_back(1.0);
    } else if (id_to_page.at(id)) {
        cheers.emplace_back(CalcShare(id ));
    } else {
        cheers.emplace_back(0.0);
    }
}

void ReadingUsers::InputRequests(std::istream& input_stream) {
    std::uint32_t count = 0, id = 0;
    std::int16_t current_page;
    std::string request_type;

    input_stream >> count;
    for (int i = 0; i < count; ++i) {
        input_stream >> request_type;
        if (request_type == "READ"s) {
            input_stream >> id;
            input_stream >> current_page;
            PutRead(id, current_page);
        } else if (request_type == "CHEER"s) {
            input_stream >> id;
            PutCheer(id);
        }
    }
}

int main() {
    ReadingUsers users;
    users.InputRequests(std::cin);
    users.OutPut();
    return 0;
}