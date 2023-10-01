#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <utility>
#include <vector>
#include <cassert>
#include <set>

using namespace std;
//namespace rng = std::ranges;

class Domain {
public:
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
    Domain() = default;

    template <typename Range>
    Domain(Range input_string) {
        std::reverse(input_string.begin(), input_string.end());
        domain_ = std::move(input_string);
        domain_.push_back('.');
    }

    // разработайте operator==
    friend bool operator==(const Domain& lhs, const Domain& rhs) {
        return lhs.domain_ == rhs.domain_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Domain& domain) {
        stream << domain.domain_;
        return stream;
    }

    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& rhs) const {
        string tmp = rhs.domain_.substr(0, domain_.size());
        bool is_domain = tmp == domain_;
        return is_domain;
    }

    [[nodiscard]] const string& GetDomain() const {
        return domain_;
    }
private:
    string domain_;
};

class DomainChecker {
    vector<Domain> prohibited_domains_;
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template<typename RandomIt>
    DomainChecker(RandomIt it_begin, RandomIt it_end) {
        while (it_begin != it_end) {
            prohibited_domains_.emplace_back(*it_begin++);
        }

        sort(prohibited_domains_.begin(), prohibited_domains_.end(),
             [&](const Domain& lhs, const Domain& rhs){
                 return lexicographical_compare(lhs.GetDomain().begin(), lhs.GetDomain().end(), rhs.GetDomain().begin(), rhs.GetDomain().end());
             });

        auto end_it = unique(prohibited_domains_.begin(), prohibited_domains_.end(),
                             [&](const Domain& lhs, const Domain& rhs){
                                 return lhs.IsSubdomain(rhs) || rhs.IsSubdomain(lhs);
                             });

        prohibited_domains_.erase(end_it, prohibited_domains_.end());
    }

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& domain) {
        auto upper_it = upper_bound(prohibited_domains_.begin(), prohibited_domains_.end(), domain, [](const Domain& lhs, const Domain& rhs){
            return lexicographical_compare(lhs.GetDomain().begin(), lhs.GetDomain().end(), rhs.GetDomain().begin(),rhs.GetDomain().end());
        });

        if (upper_it != prohibited_domains_.begin()) {
            return (--upper_it)->IsSubdomain(domain);
        }
        return false;
    }
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
template <typename Number>
std::vector<Domain> ReadDomains(istream& input, Number size_of_lines) {
    string input_string;
    vector<Domain> vector_of_domains;

    while (size_of_lines > 0) {
        getline(input, input_string);
        Domain domain(input_string);
        vector_of_domains.emplace_back(std::move(domain));
        --size_of_lines;
    }
    return vector_of_domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}


namespace Tests {
    bool TestEquility() {
        Domain d1("domain first"s);
        Domain d2("domain second"s);
        Domain d3("domain first"s);

        assert(d1 != d2);
        assert(d2 != d3);
        assert(d3 == d1);

        std::cout << "Equility Test status...\t"sv << "PASSED\n"s;
        return true;
    }

    bool TestSubDomain() {
        Domain d1("auto.ru"s);
        Domain d2("auto.gov"s);
        Domain d3("all.auto.ru"s);
        Domain d4("not.all.auto.ru"s);
        Domain d5("auto.ru.gov.ru"s);

        Domain d6("maps.me"s);
        Domain d7("m.maps.me"s);


        assert(!d1.IsSubdomain(d2));
        assert(!d2.IsSubdomain(d1));
        assert(!d3.IsSubdomain(d1));
        assert(d1.IsSubdomain(d3));
        assert(d1.IsSubdomain(d4));
        assert(!d2.IsSubdomain(d4));
        assert(!d2.IsSubdomain(d5));
        assert(d6.IsSubdomain(d7));

        std::cout << "Subdomain Test status...\t"sv << "PASSED\n"s;
        return true;
    }

    bool TestIsForbidden() {
        vector<Domain> prohibited_domains{"gdz.ru"s, "maps.me"s, "m.gdz.ru"s, "com"s, "gov"s, "gov.ru"s};
        vector<Domain> read_domains{
                "auto.ru"s,
                "maps.me"s,
                "not.all.auto.ru"s,
                "news.com"s,
                "news.ru"s,
                "new.gov"s,
                "world.news.gov"s,
                "all.auto.ru"s,
                "gdz.gov"s,
                "gdz.kz"s
        };
        DomainChecker checker(prohibited_domains.begin(), prohibited_domains.end());
        vector<bool> result{};
        vector<bool> correct_res{
            false,
            true,
            false,
            true,
            false,
            true,
            true,
            false,
            true,
            false
        };

        for (const auto& el : read_domains) {
            result.emplace_back(checker.IsForbidden(el));
        }

        assert(result == correct_res);
        std::cout << "ForbiddenDomains Test status...\t"sv << "PASSED\n"s;
        return true;
    }
}

void AllTests() {
    using namespace Tests;

    TestEquility();
    TestSubDomain();
    TestIsForbidden();
    std::cout << "\n\tAll Tests has been passed\n"s;
}

int main() {
    AllTests();
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));

    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
    return 0;
}