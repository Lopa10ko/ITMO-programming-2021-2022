#pragma once
#if !defined( CPPLAB1_H )
#define CPPLAB1_H
#include <vector>
#include <math.h>

class Dot {
//only for Dot class
private:
    double x, y;
//interface
public:
    //defaults constructor
    Dot(double x = 0.0, double y = 0.0);
    Dot(const Dot &d) : x(d.x), y(d.y) {}; //undefined reference
    //operator
    Dot& operator = (const Dot &d);
    bool operator == (const Dot &d) const;
    //getters
    double getX() const;
    double getY() const;
    //setters
    void setX(double x);
    void setY(double y);
    void setXY(double x, double y);
    //other
    double getInterval(const Dot& d) const; //length between 2 objects
    double getAbs() const; //absolute vector value
    void copyNaive(const Dot& d);
    //rotate setter
    virtual void setRotate(double alpha);
    //reshift
    virtual void setShift(double x, double y);
};

//not inherited from Dot (Line -- Set of Dots)
class Line {
//for childs too and Line.package...
protected:
    std::vector<Dot> dots;
    int quantity;
public:
    //defaults constructor
    Line() = default;
    Line(int number, Dot* source);
    Line(const Line& l) = default;
    //operator
    Line& operator = (const Line &l);
    bool operator==(const Line &l) const;
    //getters
    Dot getDot(int number) const;
    int getQuantity() const;
    //for overriding
    virtual double getPerimeter() const;
    //rotate all obj in dots
    void setRotateLine(double alpha);
    //reshift all
    void setShiftLine(double x, double y);
};

//inherited from Line (if dts[0] == dots[-1])
class CycleLine : public Line {
public:
    using Line::Line;
    //overriding
    double getPerimeter() const override;
    //check if the line crosses itself
    bool isConvex() const;
    double getSumAngles() const;
protected:
    double getComputation(const Dot&, const Dot&, const Dot&) const;
};

class Polygon : public CycleLine {
public:
    using CycleLine::CycleLine;
    virtual double getArea() const;
};

class Triangle : public Polygon {
public:
    Triangle() : Polygon() {}
    Triangle(size_t number, Dot* source);
    Triangle(const Triangle& t) = default;
    Triangle& operator=(const Triangle& t) = default;
private:
    bool isPerfect() const;
    bool isRight() const;
    bool isIsoscles() const;
};

class Trapezoid : public Polygon {
public:
    using Polygon::Polygon;
private:
    double getHeight() const;
};

class PerfectPolygon : public Polygon {
public:
    using Polygon::Polygon;
private:
    double getSideLength() const;
    double getPerimeter() const override;
    double getArea() const override;
};

#endif