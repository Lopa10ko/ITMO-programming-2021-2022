#include <iostream>
#include <math.h>
#include <assert.h>
#include <sstream>
//cd "c:\Users\Georgy\Desktop\CLABS\cpp_lab\cpp_lab1\" && g++ cpplab1overload.cpp -o cpplab1overload -std=c++11 && cpplab1overload

class Polynom {
private:
    int min_, max_;
    int *coefficients_;
    std::size_t size_;
    double compute(double num_val) const {
        double cur_val = powf(num_val, min_) * coefficients_[0];
        double answer = cur_val;
        for (int i = 0; i < size_; i++) {
            cur_val *= num_val;
            answer += cur_val * coefficients_[i];
        }
        return answer;
    }
    void resizeCurrent(std::size_t new_size, std::size_t shift) {
        int *a = new int[new_size];
        for(std::size_t i = 0; i < new_size; i++) {
            if(i >= shift && (i - shift) < size_) {
                a[i] = coefficients_[i - shift];
            } else {a[i] = 0;}
        }
        delete [] coefficients_;
        coefficients_ = a;
        size_ = new_size;
        min_ -= shift;
        max_ = min_ + size_ - 1;
    }
    void refreshThis(const Polynom& poly) {
        if (poly.min_ < min_ || poly.max_ > max_) {
            resizeCurrent((std::size_t)(std::max(max_, poly.max_) - std::min(min_, poly.min_) + 1), min_ - poly.min_);
        }
    }
public:
    //generators
    Polynom() {
        min_ = max_ = size_ = 0;
        coefficients_ = new int[1]{0};
    }
    Polynom(int min, int max) : min_{min}, max_{max}, size_{(std::size_t)(max - min + 1)}, coefficients_{new int[size_]} {
        for (int i = 0; i < size_; i++) {coefficients_[i] = 0;}
    }
    Polynom(int min, int max, int *coefficients) 
        : min_{min}
        , max_{max}
        , size_{(std::size_t)(max - min + 1)}
        , coefficients_{new int[size_]}
    {for (int i = 0; i < size_; i++) {coefficients_[i] = coefficients[i];}}
    Polynom(const Polynom &other) 
        : min_{other.min_}
        , max_{other.max_}
        , size_{other.size_}
        , coefficients_{new int[size_]}
    {for (int i = 0; i < size_; i++) {coefficients_[i] = other.coefficients_[i];}}
/////////////////////////////////////////////////////////////////////////////////////////
    Polynom& operator=(const Polynom &other) {
        //bad_alloc
        if (&other == this) return *this;
        delete [] coefficients_;
        min_ = other.min_;
        max_ = other.max_;
        size_ = other.size_;
        for (int i = 0; i < size_; i++) {coefficients_[i] = other.coefficients_[i];}
        return *this;
    }
/////////////////////////////////////////////////////////////////////////////////////////
    //destructor
    ~Polynom() {delete [] coefficients_;}
    //getters and others
    int getMin() const {return min_;}
    int getMax() const {return max_;}
    int getCoefInPow(int index) const {
        if (index > max_ || index < min_) {
            throw std::invalid_argument("no data for that power_index");
        }
        return coefficients_[index];
    }
    Polynom& operator+=(const Polynom &other) {
        refreshThis(other);
        for (int i = other.min_; i <= other.max_; i++) {coefficients_[i - min_] += other.coefficients_[i - other.min_];}
        return *this;
    }
    Polynom& operator-=(const Polynom &other) {
        refreshThis(other);
        for (int i = other.min_; i <= other.max_; i++) {coefficients_[i - min_] -= other.coefficients_[i - other.min_];}
        return *this;
    }
    Polynom& operator*=(const Polynom &);
    Polynom& operator*=(int);
    Polynom& operator/=(int);
    //setters
    int setMin(int min) {
        int current_size = max_ - min + 1;
        if (size_ != (std::size_t)current_size || current_size != sizeof(coefficients_)) {
            throw std::invalid_argument("not a valid data for value of min_");
        }
        this->min_= min;
    }
    int setMax(int max) {
        int current_size = max - min_ + 1;
        if (size_ != (std::size_t)current_size || current_size != sizeof(coefficients_)) {
            throw std::invalid_argument("not a valid data for value of max_");
        }
        this->max_= max;
    }
    int operator[](int index) const;
    int& operator[](int);
    //operators ==, !=
    bool operator==(const Polynom &) const;
    bool operator!=(const Polynom &) const;
    Polynom operator+(const Polynom &other) const {
        Polynom temp{*this};
        return temp += other;
    }
    Polynom operator-(const Polynom &other) const {
        Polynom temp{*this};
        return temp -= other;
    }
    Polynom operator-(double value) const {
        Polynom temp{*this};
        for (int i = 0; i < temp.size_; i++) {temp.coefficients_[i] -= value;}
        return temp;
    }
    Polynom operator-() const {
        Polynom temp{*this};
        for (int i = 0; i < temp.size_; i++) {temp.coefficients_[i] *= -1;}
        return temp;
    }
    //friends
    friend Polynom operator*(const Polynom &, const Polynom &);
    friend Polynom operator*(const Polynom &, const Polynom &);
    friend std::ostream& operator<<(std::ostream &output_thread, const Polynom &poly) {
        for (int temp = 0; temp < poly.size_; temp++) {
            std::size_t i = poly.size_ - 1 - temp;
            int cur_pow = poly.max_ - temp;
            if (!poly.coefficients_[i]) continue;
            if (poly.coefficients_[i] > 0 && i != poly.size_ - 1) output_thread << '+';
            if (cur_pow != 0 && poly.coefficients_[i] == -1) output_thread << '-';
            else if (cur_pow == 0 || poly.coefficients_[i] !=1) output_thread << poly.coefficients_[i];
            if (cur_pow != 0) output_thread << 't';
            if (cur_pow != 0 && cur_pow != 1) output_thread << '^' << cur_pow;
        }
        return output_thread;
    }
    friend std::istream & operator>>(std::istream &src_thread, Polynom &poly) {
        std::size_t a, b;
        src_thread >> a >> b;
        poly = Polynom(a, b);
        for(std::size_t i = 0; i < poly.size_; i++) {src_thread >> poly.coefficients_[i];}
        return src_thread;
    }
    //taylor
    //diff
    //integrate
};



void testPolynom() {
    std::stringstream in;
    in << Polynom() << " ";
    in << Polynom(1, 3, new int[3]{1, 5 , 1}) << " ";
    in << Polynom(-1, 3, new int[5]{-1, 1, 0, -1, -1}) << " ";
    assert(in.str() == " t^3+5t^2+t -t^3-t^2+1-t^-1 ");
    const auto p4 = Polynom(0, 3, new int[4]{-1, 0, -1, -1}), p3 = Polynom(1, 3, new int[3]{1, 5 , 1});
    const Polynom p1, p2(2, 4);
    assert(p1.getMax() == 0 && p1.getMin() == 0 && p2.getCoefInPow(2) == 0 && p2.getMax() == 4 && p3.getMin() == 1);
}

int main(int argc, char* argv[]) {
    testPolynom();
    return 0;
}