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
    //generators - not explicit
    Polynom() {
        min_ = max_ = 0;
        coefficients_ = new int[1]{0};
        size_ = 0;
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
    int getCoefInIndex(int index) const {
        if (index > max_ || index < min_) {
            throw std::invalid_argument("no data for that coeff_index");
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
    Polynom& operator*=(const Polynom &other) {
        int min = other.min_ + min_, max = other.max_ + max_;
        std::size_t size = (std::size_t)(max - min + 1);
        size_ = size;
        int *coefficients = new int[size]{0};
        //lame, but working :)
        for (int i = min_; i <= max_; i++) {
            for (int j = other.min_; j <= other.max_; j++) {
                coefficients[i + j - min] += coefficients_[i - min_] * other.coefficients_[j - other.min_];
            }
        }
        delete [] coefficients_;
        coefficients_ = coefficients;
        max_ = max;
        min_ = min;
        return *this;
    }
    Polynom& operator*=(int value) {
        for (int i = 0; i < size_; i++) {coefficients_[i] *= value;}
    }
    Polynom& operator/=(int value) {
        if ((int)value == 0) {
            throw std::invalid_argument("no 0 devision allowed!");
        }
        for (int i = 0; i < size_; i++) {coefficients_[i] /= value;}
    }
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
    int operator[](int index) const {
        if (index > max_ || index < min_) {
            throw std::invalid_argument("no data for that power_index");
        }
        return coefficients_[index - min_];
    }
    int& operator[](int index) {
        if (index >= min_ && index <= max_) {return coefficients_[index - min_];}
        resizeCurrent((std::size_t)(std::max(max_, index) - std::min(min_, index) + 1), min_ - std::min(min_, index));
        return coefficients_[index - min_];
    }
    //operators ==, !=
    bool operator==(const Polynom &other) const {
        bool null_condition = true, second_condition = true;
        for (int i = 0; i < size_; i++) {
            if (coefficients_[i]) {
                null_condition = false;
                break;
            }
        }
        if (null_condition) {
            for (int i = 0; i < other.size_; i++) {
                if (other.coefficients_[i]) {
                    second_condition = false;
                    break;
                }
            }
            if(second_condition) return true;
        }
        bool result = 0;
        if (min_ == other.min_ && max_ == other.max_) {
            for (int i = 0; i < size_; i++) {
                if (abs(other.coefficients_[i] - coefficients_[i])) {result = 0; break;}
                result = 1;
            }
        }
        return ((result)? true : false);
    }
    bool operator!=(const Polynom &other) const {return !(*this == other);}
    Polynom operator+(const Polynom &other) const {
        Polynom temp{*this};
        return temp += other;
    }
    Polynom operator+(int value) const {
        Polynom temp{*this};
        for (int i = 0; i < temp.size_; i++) {temp.coefficients_[i] += value;}
        return temp;
    }
    Polynom operator-(const Polynom &other) const {
        Polynom temp{*this};
        return temp -= other;
    }
    Polynom operator-(int value) const {
        Polynom temp{*this};
        for (int i = 0; i < temp.size_; i++) {temp.coefficients_[i] -= value;}
        return temp;
    }
    Polynom operator-() const {
        Polynom temp{*this};
        for (int i = 0; i < temp.size_; i++) {temp.coefficients_[i] *= -1;}
        return temp;
    }
    Polynom operator*(const Polynom &other) const{
        Polynom temp{*this};
        return temp *= other;
    }
    Polynom operator*(int value) const {
        Polynom temp{*this};
        return temp *= value;
    }
    friend Polynom operator*(int value, const Polynom &other) {
        return other * value;
    }
    Polynom operator/(int value) const {
        Polynom temp{*this};
        return temp /= value;
    }
    //friends
    friend std::ostream& operator<<(std::ostream &output_thread, const Polynom &poly) {
        output_thread << "deg(P)[" << poly.min_ << ", " << poly.max_ << "] -" << "coefs: ";;
        for (int i = 0; i < poly.size_; i++) {output_thread << "|" << poly.coefficients_[i] << "|";}
        output_thread << ";\n";
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
    const auto p4 = Polynom(0, 3, new int[4]{-1, 0, -1, -1}), p3 = Polynom(1, 3, new int[3]{1, 5 , 1});
    const Polynom p1, p2(2, 4);
    const auto p5 = p4, p6 = p1;
    assert((p5 == p4) == true && p5.getCoefInIndex(0) == -1 && (p1 != p4) == true && (p3 != p4) == true);
    assert(p1.getMax() == 0 && p1.getMin() == 0 && p2.getCoefInIndex(2) == 0 && p2.getMax() == 4 && p3.getMin() == 1);
    //////////////////////////////////////
    auto truepr1 = Polynom(0, 6, new int[7]{0, -1, -5, -2, -6, -6, -1});
    Polynom pr1 = p4 * p3, pr2 = p3 * p4;
    std::cout << pr1[0] << pr1[1] << pr1[2] << pr1[3] << pr1[4] << pr1[5] << pr1[6] << std::endl;
    std::cout << pr2[0] << pr2[1] << pr2[2] << pr2[3] << pr2[4] << pr2[5] << pr2[6] << std::endl;
    assert((pr1 == truepr1) == true);
    assert((pr1 == pr2) == true);
    pr1 *= pr2;
    Polynom truepr2_square = pr2 * pr2;
    assert((pr1 == truepr2_square) == true);
    Polynom pr2_mult = pr2 + pr2;
    pr1 += pr2;
    auto result_truepr2_square = Polynom(0, 12, new int[13]{0, -1, -4, 8, 23, 26, 75, 86, 70, 76, 48, 12, 1});
    std::cout << pr2_mult[0] << pr2_mult[1] << pr2_mult[2] << pr2_mult[3] << pr2_mult[4] << pr2_mult[5] << pr2_mult[6]<< std::endl;
    std::cout << "\nall test passed!\n\n";
}

void testThread() {
    std::stringstream in;
    in << Polynom();
    in << Polynom(1, 3, new int[3]{1, 5 , 1});
    in << Polynom(-1, 3, new int[5]{-1, 1, 0, -1, -1});
    std::cout << in.str();
}

int main(int argc, char* argv[]) {
    testPolynom();
    testThread();
    return 0;
}