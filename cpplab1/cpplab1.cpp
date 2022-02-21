#include "cpplab1.h"
#include <iostream>
#include <cmath>
#include <cassert>
//cd "c:\Users\Georgy\Desktop\CLABS\cpp_lab\cpp_lab1\" && g++ cpplab1.cpp -o cpplab1 -std=c++11 && cpplab1
const double pi = acos(-1);

Dot::Dot(double x, double y) : x{x}, y{y} {} //extended initializer lists
double Dot::getX() const {return x;}
double Dot::getY() const {return y;}
void Dot::setX(double x) {this->x = x;}
void Dot::setY(double y) {this->y = y;}
void Dot::setXY(double x, double y) {this->x = x; this->y = y;}
void Dot::setShift(double x, double y) {this->x += x; this->y += y;}
void Dot::setRotate(double alpha) {
    double oldx = this->x, oldy = this->y;
    this->x = oldx * cos(alpha) - oldy * sin(alpha);
    this->y = oldx * sin(alpha) + oldy * cos(alpha);
}
double Dot::getAbs() const {return sqrt(pow(this->x, 2) + pow(this->y, 2));}
double Dot::getInterval(const Dot& d) const {
    return sqrt(pow(this->x - d.x, 2) + pow(this->y - d.y, 2));
}


Line::Line(int number, Dot* source) {
    for (size_t i = 0; i < number; i++) {dots.push_back(source[i]);}
    this->quantity = number;
}

Dot Line::getDot(int index) const {
    if (index < quantity) return dots[index];
}
int Line::getQuantity() const {return quantity;}
double Line::getPerimeter() const {
    double value = 0.0;
    for (size_t i = 0; i < quantity - 1; i++) {value += dots[i].getInterval(dots[i + 1]);}
    return value;
}
void Line::setShiftLine(double x, double y) {
    for (int i = 0; i < getQuantity(); i++) {
        getDot(i).Dot::setShift(x, y);
    }
}
void Line::setRotateLine(double alpha) {
    for (int i = 0; i < getQuantity(); i++) {
        getDot(i).Dot::setRotate(alpha);
    }
}
void Line::doShift(const Dot &d) {
    setShiftLine(d.getX() - getDot(0).getX(), d.getY() - getDot(0).getY());
}
double CycleLine::getPerimeter() const {
    return getDot(getQuantity() - 1).getInterval(getDot(0)) + Line::getPerimeter();
}
bool CycleLine::isConvex() const {
    double x1 = dots[0].getX() - dots[getQuantity() - 1].getX(), x2 = dots[1].getX() - dots[0].getX(), y1 = dots[0].getX() - dots[getQuantity() - 1].getX(), y2 = dots[1].getX() - dots[0].getX();
    bool condition = x1 * y2 - x2 * y1 > 0;
    for (int i = 0; i < getQuantity() - 1; i++) {
        if (condition != (((dots[(getQuantity() + i + 1) % getQuantity()].getX() - dots[i].getX())*(dots[(getQuantity() + i + 2) % getQuantity()].getX() - dots[(getQuantity() + i + 1) % getQuantity()].getX())-(dots[(getQuantity() + i + 2) % getQuantity()].getX() - dots[(getQuantity() + i + 1) % getQuantity()].getX())*(dots[(getQuantity() + i + 1) % getQuantity()].getX() - dots[i].getX())) > 0)) {
            return false;
        }
    }
    return true;
}
double CycleLine::getSumAngles() const {
    if (getQuantity() < 3) return true;
    double current_angles = 0.0;
    double xa = dots[getQuantity() - 1].getX(), ya = dots[getQuantity() - 1].getY(), xb = dots[0].getX(), yb = dots[0].getY(), xc = dots[1].getX(), yc = dots[1].getY();
    double v1x = xb - xa, v1y = yb - ya, v2x = xc - xb, v2y = yc - yb;
    current_angles += acos(abs((v1x * v2x + v1y * v2y) / (sqrt(pow(v1x, 2) + pow(v1y, 2)) * sqrt(pow(v2x, 2) + pow(v2y, 2)))));
    xa = dots[getQuantity() - 2].getX(); 
    ya = dots[getQuantity() - 2].getY(); 
    xb = dots[getQuantity() - 1].getX(); 
    yb = dots[getQuantity() - 1].getY(); 
    xc = dots[0].getX(); 
    yc = dots[0].getY();
    v1x = xb - xa; 
    v1y = yb - ya; 
    v2x = xc - xb; 
    v2y = yc - yb;
    current_angles += acos(abs((v1x * v2x + v1y * v2y) / (sqrt(pow(v1x, 2) + pow(v1y, 2)) * sqrt(pow(v2x, 2) + pow(v2y, 2)))));
    for (int i = 0; i < getQuantity() - 2; i++) {
        xa = dots[i].getX();
        ya = dots[i].getY(); 
        xb = dots[i + 1].getX(); 
        yb = dots[i + 1].getY(); 
        xc = dots[i + 2].getX(); 
        yc = dots[i + 2].getY();
        v1x = xb - xa; 
        v1y = yb - ya; 
        v2x = xc - xb; 
        v2y = yc - yb;
        current_angles += acos(abs((v1x * v2x + v1y * v2y) / (sqrt(pow(v1x, 2) + pow(v1y, 2)) * sqrt(pow(v2x, 2) + pow(v2y, 2)))));
    }
    return current_angles;
}
double CycleLine::getComputation(const Dot& d1, const Dot& d2, const Dot& d3) const {
    double x1 = d2.getX() - d1.getX(), x2 = d3.getX() - d2.getX(), y1 = d2.getX() - d1.getX(), y2 = d3.getX() - d2.getX();
    return x1 * x2 + y1 * y2;
}


double Polygon::getArea() const {
    if (!isConvex()) {
        throw std::invalid_argument("invalid data source [not convex CycleLine]");
    }
    double area = 0.0;
    int n = getQuantity();
    for (int i = 0; i < n - 1; i++) {
        area += (getDot(i).getX() * getDot(i + 1).getY() - getDot(i + 1).getX() * getDot(i).getY());
    }
    if (n >= 3) {
        area += (getDot(n - 1).getX() * getDot(0).getY() - getDot(0).getX() * getDot(n - 1).getY());
    }
    return abs(area / 2);
}
Triangle::Triangle(size_t number, Dot* source) : Polygon(number, source) {
    if (getQuantity() != 3) {throw std::invalid_argument("not a triangle data");}
}
bool Triangle::isRight() const {
    return (((pow(getDot(0).getInterval(getDot(1)), 2) + pow(getDot(1).getInterval(getDot(2)), 2)) == (pow(getDot(0).getInterval(getDot(2)), 2)))
    || ((pow(getDot(1).getInterval(getDot(2)), 2) + pow(getDot(2).getInterval(getDot(0)), 2)) == (pow(getDot(0).getInterval(getDot(1)), 2)))
    || ((pow(getDot(1).getInterval(getDot(0)), 2) + pow(getDot(0).getInterval(getDot(2)), 2)) == (pow(getDot(1).getInterval(getDot(2)), 2))));
}
bool Triangle::isPerfect() const {
    double side1 = getDot(0).getInterval(getDot(1));
    double side2 = getDot(1).getInterval(getDot(2));
    double side3 = getDot(2).getInterval(getDot(0));
    return ((side1 == side2) && (side2 == side3));
}
bool Triangle::isIsoscles() const {
    double side1 = getDot(0).getInterval(getDot(1));
    double side2 = getDot(1).getInterval(getDot(2));
    double side3 = getDot(2).getInterval(getDot(0));
    return ((side1 == side2) || (side2 == side3) || (side1 == side3));
}

//////////////////////////FIXED///////////////////////////////////
Trapezoid::Trapezoid(size_t number, Dot* source) : Polygon(number, source) {
    if (getQuantity() != 4) {throw std::invalid_argument("not a trapezoid data");}
    double norm_x1 = abs(getDot(0).getX() - getDot(1).getX()) / getDot(0).getInterval(getDot(1)),
           norm_y1 = abs(getDot(0).getY() - getDot(1).getY()) / getDot(0).getInterval(getDot(1)),
           norm_x2 = abs(getDot(1).getX() - getDot(2).getX()) / getDot(1).getInterval(getDot(2)),
           norm_y2 = abs(getDot(1).getY() - getDot(2).getY()) / getDot(1).getInterval(getDot(2)),
           norm_x3 = abs(getDot(2).getX() - getDot(3).getX()) / getDot(2).getInterval(getDot(3)),
           norm_y3 = abs(getDot(2).getY() - getDot(3).getY()) / getDot(2).getInterval(getDot(3)),
           norm_x4 = abs(getDot(3).getX() - getDot(0).getX()) / getDot(3).getInterval(getDot(0)),
           norm_y4 = abs(getDot(3).getY() - getDot(0).getY()) / getDot(3).getInterval(getDot(0));
    if ((norm_x1 != norm_x3 && norm_y3 != norm_y1) && (norm_x2 != norm_x4 && norm_y2 != norm_y4)) {
        throw std::invalid_argument("not a trapezoid data");
    }
}
double Trapezoid::getHeight() const {
    double norm_x1 = abs(getDot(0).getX() - getDot(1).getX()) / getDot(0).getInterval(getDot(1)),
           norm_y1 = abs(getDot(0).getY() - getDot(1).getY()) / getDot(0).getInterval(getDot(1)),
           norm_x3 = abs(getDot(2).getX() - getDot(3).getX()) / getDot(2).getInterval(getDot(3)),
           norm_y3 = abs(getDot(2).getY() - getDot(3).getY()) / getDot(2).getInterval(getDot(3));
    if (norm_x1 == norm_x3 && norm_y3 == norm_y1) {
        return getArea() * 2 / (getDot(0).getInterval(getDot(1)) + getDot(2).getInterval(getDot(3)));
    }
    return getArea() * 2 / (getDot(1).getInterval(getDot(2)) + getDot(3).getInterval(getDot(0)));
}

PerfectPolygon::PerfectPolygon(size_t number, Dot* source) : Polygon(number, source) {
    if (getQuantity() < 3) {throw std::invalid_argument("not a perfectpolygon data");}
    double side = getDot(0).getInterval(getDot(1));
    for (int i = 1; i < getQuantity() - 1; i++) {
        if (getDot(i).getInterval(getDot(i + 1)) != side) {throw std::invalid_argument("not a perfect polygon data");}
    }
    if (side != getDot(getQuantity() - 1).getInterval(getDot(0))) {throw std::invalid_argument("not a perfect polygon data");}
}
/////////////////////////////////////////////////////////////////////////

double PerfectPolygon::getSideLength() const {
    return getDot(0).getInterval(getDot(1));
}
double PerfectPolygon::getArea() const {
    int n = getQuantity();
    if (n >= 3) {
        return n * (pow(getSideLength(), 2) / 4) * tan(((n - 2) * pi) / (2 * n));
    }
    return 0;
}
double PerfectPolygon::getPerimeter() const {
    return getQuantity() * getSideLength();
}

void testDot() {
    Dot d1(2,1), d2(5, 12);
    Dot d3 = d1, d4, d5(2, 3);
    std::cout << (d1.getX() == 0 && d1.getY() == 0) << " " << d1.getX() << "," << d1.getY() << std::endl;
    std::cout << (d2.getX() == 5 && d2.getY() == 12) << " " << d2.getX() << "," << d2.getY() << std::endl;
    std::cout << d1.getInterval(d2) << " " << d2.getInterval(d2) << std::endl;
    std::cout << d1.getAbs() << " " << d2.getAbs() << std::endl;
    d3.setX(4.5);
    std::cout << d3.getX() << " orig: " << d1.getX() << std::endl;
    std::cout << "(" << d4.getX() << ", " << d4.getY() << ")" << std::endl;
    std::cout << " orig d5: " << d5.getX() << d5.getY() << std::endl;
    d5.setShift(1, -1);
    std::cout << "shifted d5: " << d5.getX() << ", "  << d5.getY() << std::endl;
    d5.setRotate(pi / 6);
    std::cout << "rotated d5: " << d5.getX() << ", "  << d5.getY() << std::endl;
}
void testLine() {
    Dot *first = new Dot[4]{Dot(), Dot(5, 12), Dot(23, 12), Dot(30, -12)}, *second = new Dot[3]{Dot(2, 3), Dot(7, 23), Dot(45, 90)};
    const Line line1(4, first);
    const CycleLine cline(4, first);
    std::cout << "" << std::endl;
    std::cout << "line1 (4 dots): " << line1.getQuantity() << " dot3(23,12) x: " << line1.getDot(2).getX() << " perimeter(56): " << line1.getPerimeter() << std::endl;
    std::cout << "line dot0 (0,0): "<< line1.getDot(0).getX() << ", " << line1.getDot(0).getY() << std::endl;
    std::cout << "cycle perimeter (56+32.31): "<< cline.getPerimeter() << std::endl;
    Dot *forpolygon = new Dot[4]{Dot(), Dot(5, 12), Dot(23, 12), Dot(20, 11)};
    const CycleLine poly(4, forpolygon);
    std::cout << "convex(it is): " << poly.isConvex() << std::endl;
}
void testPolygon() {
    Dot *first = new Dot[4]{Dot(), Dot(5, 12), Dot(23, 12), Dot(30, -12)}, *second = new Dot[3]{Dot(), Dot(0, 4), Dot(3, 0)};
    const Polygon poly1(4, first), poly2(3, second);
    std::cout << "area (6): " << poly2.getArea() << std::endl;
}
void testOther() {
    Dot *first = new Dot[4]{Dot(), Dot(5, 12), Dot(23, 12), Dot(30, 0)}, *second = new Dot[3]{Dot(), Dot(0, 4), Dot(3, 0)},
    *third = new Dot[4]{Dot(), Dot(5, 12), Dot(23, 12), Dot(30, -12)}, *trapezoid_reversed = new Dot[4]{Dot(), Dot(0, 1), Dot(5, 10), Dot(5, 0)},
    *convex_trapezoid = new Dot[4]{Dot(), Dot(5, 10), Dot(0, 1), Dot(5, 0)}, *perfect = new Dot[4]{Dot(), Dot(1, 0), Dot(1, 1), Dot(0, 1)};
    //const Triangle wrong_triangle(4, first);
    const Triangle poly2(3, second);
    assert(poly2.getArea() == 6);
    const Trapezoid tr1(4, first);
    //const Trapezoid wrong_trapezoid(3, second);
    //const Trapezoid almost_trapezoid(4, third);
    const Trapezoid tr_reversed(4, trapezoid_reversed);
    assert(tr1.getArea() == 288 && tr1.getHeight() == 12);
    //const Trapezoid trr_false(4, convex_trapezoid);
    //const PerfectPolygon not_perfect(3, second);
    const PerfectPolygon perfect_poly(4, perfect);

}
int main(int argc, char* argv[]) {
    testDot();
    testLine();
    testPolygon();
    testOther();

    // struct Point {
    //     double x;
    //     double y;
    //     void reshift(double x, double y){
    //         this->x += x;
    //         this->y += y;
    //     } 
    // };
    // struct Line {
    //     Point p1, p2;
    //     double get_length(){
    //         double dx = p1.x - p2.x, dy = p1.y - p2.y;
    //         return sqrt(dx * dx + dy * dy);
    //     }        
    // };
    // Line line1 = {{0.4, 1.4} , {3.6, 6.5}};
    // cout << line1.p1.x << endl;
    // cout << line1.get_length() << endl;
    // line1.p1.reshift(100, 200);
    // cout << line1.p1.x << endl;
    return 0;
}
