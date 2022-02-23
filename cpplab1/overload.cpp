#include <iostream>
#include <cmath>
#include <cassert>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <limits>
//string constructor methods
#include <algorithm>
#include <regex>
//cd "c:\Users\Georgy\Desktop\CLABS\cpp_lab\cpp_lab1\" && g++ overload.cpp -o overload -std=c++11 && overload

class Polynom {
private:
    std::map <long long, double> coefs_;
    long long min_, max_;
    std::size_t size_;
public:
    //basic constructors
    Polynom()
        : min_(0)
        , max_(0)
        , size_(0)
    {}
    // constructor from string (example:"4x^4 + 3x - 10x^3 - 2x + 1 - 4x^4" --> {{3, -10}, {1, 1}, {0, 1}})
    Polynom(std::string source) {
        if (source[0] != '+' && source[0] != '-') {source = '+' + source;}
        //partition to [+-coefs] [x^(power)]
        //https://regex101.com/ -- test yor regex
        std::regex example(R"(([+-])\s*([A-Xa-x]))");
        source = regex_replace(source, example, "$1 1&2");
        //std::cout << source << std::endl;
        //https://en.cppreference.com/w/cpp/algorithm/remove
        //https://www.cplusplus.com/reference/string/string/erase/ -- trim & split
        source.erase(remove_if(source.begin(), source.end(), ::isspace), source.end());
        //std::cout << source << std::endl;
        //tokenize
        std::regex full_example(R"([-+]?((\d*[A-Xa-x]\^\d+)|(\d*[A-Xa-x])|(\d+)))");
        //https://en.cppreference.com/w/cpp/regex/regex_token_iterator
        std::sregex_token_iterator first(source.begin(), source.end(), full_example), last;
        int min_power = (int)std::numeric_limits<double>::infinity(),
            max_power = -(int)std::numeric_limits<double>::infinity();
        while (first != last) {
            std::string pch {*first++};
            std::smatch smatch_;
            //partition of [+-coefs][x^(power)] and [+-coefs]
            if (std::regex_match(pch, smatch_, std::regex(R"(([-+]?\d+)([A-Xa-x](\^(\d+))?)?)"))) { 
                double coefs {stod(smatch_[1])}; //stod == (double) std::string
                int power {0};
                //power_value-like smatch_[4].str()
                //partition of [coefs][x^(power)], [+-coefs][x] and [+-coefs]
                power = (!smatch_[2].str().empty()) ? ((!smatch_[4].str().empty()) ? stoi(smatch_[4].str()) : 1) : 0;
                coefs_[power] += coefs;
                max_power = std::max((int)max_power, power);
                min_power = std::min((int)min_power, power);
                if (!coefs_[power]) {coefs_.erase(power);}
            }
        }
        min_ = min_power;
        max_ = max_power;
        size_ = (std::size_t)(max_ - min_ + 1);
    }
    Polynom(long long min, long long max, std::vector<double> const &coefs)
        : min_(min)
        , max_(max)
        , size_((std::size_t)(max_ - min_ + 1))
    {long long key = min_; for (const auto &value : coefs) {coefs_[key] = value; ++key;}}
    Polynom(const Polynom& other)
        : min_(other.min_)
        , max_(other.max_)
        , size_(other.size_)
        , coefs_(other.coefs_)
    {}
    // Polynom& operator=(const Polynom &other) {
    //     //bad_allocation fix
    //     if (this == &other) return *this;
    //     coefs_.clear();
    //     min_ = other.min_;
    //     max_ = other.max_;
    //     size_ = other.size_;
    //     long long key = min_;
    //     for (const auto &pch : other.coefs_) {coefs_[key].insert(pch); ++key;}
    // }
    // ~Polynom() {} = default;
    int getMin() const {return min_;}
    int getMax() const {return max_;}
    int getSize() const {return size_;}
    Polynom derivative(const Polynom &other) {
        Polynom temp;
        for (const auto &pch : other.coefs_) {
            temp.coefs_[pch.first + 1] += pch.second / (pch.first + 2);
            if (!temp.coefs_[pch.first + 1]) {temp.coefs_.erase(pch.first + 1);}
        }
        return temp;
    }
    bool operator==(const Polynom &other) const {
        bool null_condition = true, second_condition = true;
        if (other.coefs_.empty() && coefs_.empty()) {return true;}
        if (other.coefs_.empty() || coefs_.empty()) {return false;}
        //auto pred = [] (auto f, auto s) {return (int)(f.first - s.first) == 0 && (int)(f.second - s.second) == 0;};
        return other.coefs_.size() == coefs_.size()
            && std::equal(other.coefs_.begin(), other.coefs_.end(), coefs_.begin());
    }
    bool operator!=(const Polynom &other) const {return !(*this == other);}
    Polynom& operator/=(double value) {
        if ((int)value == 0) {
            throw std::invalid_argument("no 0 devision allowed!");
        }
        for (const auto &pch : coefs_) {
            coefs_[pch.first] /= value;
        }
        return *this;
    }
    friend Polynom operator*(const Polynom &other, const Polynom &one) {
        Polynom temp;
        for (const auto &other_pch : other.coefs_) {
            for (const auto &one_pch : one.coefs_) {
                temp.coefs_[other_pch.first + one_pch.first] += other_pch.second * one_pch.second;
                if (!temp.coefs_[other_pch.first + one_pch.first]) {temp.coefs_.erase(other_pch.first + one_pch.first);}
            }
        }
        return temp;
    }
    Polynom& operator*=(const Polynom &other) {
        return *this = *this * other;
    }
    Polynom& operator*(double value) {
        for (const auto &pch : coefs_) {
            coefs_[pch.first] *= value;
        }
    }
    Polynom& operator/(double value) const {
        Polynom temp{*this};
        return temp /= value;
    }
    Polynom& operator+=(const Polynom &other) {
        for (const auto &pch : other.coefs_) {
            coefs_[pch.first] += pch.second;
            if (!coefs_[pch.first]) {coefs_.erase(pch.first);}
        }
        return *this;
    }
    Polynom& operator+(const Polynom &other) const {
        Polynom temp{*this};
        return temp += other;
    }
    friend Polynom operator+(const Polynom &other, const Polynom &one) {
        Polynom temp = one;
        for (const auto pch : other.coefs_) {
            temp.coefs_[pch.first] += pch.second;
            if (!temp.coefs_[pch.first]) {temp.coefs_.erase(pch.first);}
        }
        return temp;
    }
    Polynom& operator+(double value) {
        for (const auto &pch : coefs_) {
            coefs_[pch.first] += value;
            if (!coefs_[pch.first]) {coefs_.erase(pch.first);}
        }
        return *this;
    }
    Polynom& operator-=(const Polynom &other) {
        for (const auto &pch : other.coefs_) {
            coefs_[pch.first] -= pch.second;
            if (!coefs_[pch.first]) {coefs_.erase(pch.first);}
        }
        return *this;
    }
    friend Polynom operator-(const Polynom &other, const Polynom &one) {
        Polynom temp = one;
        for (const auto pch : other.coefs_) {
            temp.coefs_[pch.first] -= pch.second;
            if (!temp.coefs_[pch.first]) {temp.coefs_.erase(pch.first);}
        }
        return temp;
    }
    Polynom& operator-(const Polynom &other) const {
        Polynom temp{*this};
        return temp -= other;
    }
    Polynom& operator-() {
        for (const auto &pch : coefs_) {
            coefs_[pch.first] *= -1;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& out_thread, const Polynom& poly){
        for(auto iterator = poly.coefs_.rbegin(); iterator != poly.coefs_.rend(); ++iterator) {
            //iterator <power, coef>
            //https://www.cplusplus.com/reference/ios/showpos/
            if(iterator->second != 1 && iterator->second != -1) {out_thread << std::showpos << iterator->second;}
            else if(iterator->first != 0) {out_thread << ((iterator->second > 0) ? '+' : '-');}
            else {out_thread << std::showpos << iterator->second;}
            if(iterator->first != 0) {
                out_thread << 't';
                if(iterator->first != 1) {out_thread << std::noshowpos << iterator->first;}
            }
        }
        return out_thread;
    }
    // friend std::istream & operator>>(std::istream &src_thread, Polynom &poly) {
    //     std::size_t a, b;
    //     src_thread >> a >> b;
    //     poly = Polynom(a, b);
    //     for(std::size_t i = 0; i < poly.size_; i++) {src_thread >> poly.coefficients_[i];}
    //     return src_thread;
    // }

    double& operator[](unsigned long index) {
        if (coefs_.find(index) == coefs_.end()) {
            throw std::invalid_argument("no data on that index in polynomial map");
        }
        return this->coefs_[index];
    }
    double operator[](unsigned long index) const {
        if (coefs_.find(index) == coefs_.end()) {
            throw std::invalid_argument("no data on that index in polynomial map");
        }
        return this->coefs_.at(index);
    }
};

void testPolynom() {
    Polynom q1("4a^4+ 3a- 10a^3 - 2a + 1-4a^4");
    //std::cout << q1 << std::endl;
    // std::cout << q1.coefs_.at(3);  -- map public testing (passed:OK)
    Polynom q2(0, 4, {10, -2, 1});
    assert(q2[0] == 10 && q2[1] == -2 && q2[2] == 1);
    std::cout << (q2 += q1) << std::endl;
    assert(q2[0] == 11 && q2[1] == -1 && q2[2] == 1 && q2[3] == -10);
    const auto copy_q1 = q1;
    assert((copy_q1 == q1) == true && (q1 != q2) == true && (q1 == q2) == false);
}

int main(int argc, char* argv[]) {
    testPolynom();
    return 0;
}