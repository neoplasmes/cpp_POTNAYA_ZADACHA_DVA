#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

struct Point{
    public:
        double x, y;

        Point(){}

        Point(double x, double y){
            this->x = x;
            this->y = y;
        }

        bool operator ==(const Point& p) const {
            return (x == p.x && y == p.y);
        }
};

Point createPoint(){
    double x, y;
    cin >> x >> y;

    return Point(x, y);
}

typedef vector<Point> vPt;

class Triangle{
    private:
        vPt verts;

    public:   
    //для ввода с клавы
        Triangle(){
            setVerts();
        }

        //конструктор для прямого ввода
        Triangle(Point p1, Point p2, Point p3){
            verts.push_back(p1);
            verts.push_back(p2);
            verts.push_back(p3);
        }

        void setVerts(){
            verts.clear();

            verts.push_back(createPoint());
            verts.push_back(createPoint());
            verts.push_back(createPoint());
        }

        vPt getVerts(){
            return verts;
        }

        double area(){
            return 0.5 * abs((verts[1].x - verts[0].x) * (verts[2].y - verts[0].y) - (verts[2].x - verts[0].x) * (verts[1].y - verts[0].y));
        }
};

bool pointIsInside(Point p, Triangle t){
    //какие то лучи вправо теорема лучей
    bool res = false;
    vPt verts = t.getVerts();
    for(int i = 0; i <= 2; i++){
        Point l1 = verts[i];
        Point l2 = (i == 2) ? verts[0] : verts[i+1];
        //если параллельно чтобы не делить на ноль
        if (l1.y == l2.y) continue;

        bool conditionY = (p.y < l1.y) != (p.y < l2.y);
        //подобие треугольников
        bool conditionX = p.x < (l1.x + ((p.y - l1.y)/(l2.y - l1.y))*(l2.x - l1.x));
        if(conditionX && conditionY){
            res = !res;
        }
    }

    return res;
}


struct IntrResult{
    public:
        bool areIntersect;
        Point intersectionPoint;

        IntrResult(){}

        IntrResult(bool areIntersect, Point intersectionPoint) {
            this->areIntersect = areIntersect;
            this->intersectionPoint = intersectionPoint;
        }
};


IntrResult segmentIntersection(Point a, Point b, Point c, Point d){
    bool result = false;
    Point intersectionPoint;

    //(векторное произведение)
    double top1 = (d.x - c.x)*(c.y-a.y) - (d.y-c.y)*(c.x-a.x);

    double top2 = (b.x - a.x)*(c.y-a.y) - (b.y - a.y)*(c.x-a.x);
    double bottom = (d.x - c.x)*(b.y-a.y) - (d.y - c.y)*(b.x-a.x);

    if(bottom != 0){
        double alpha = top1 / bottom;
        double beta = top2 / bottom;
        //если не добавить альфа больше нуля то будет выкиывать пересечение за границей отрезков
        //если не добавить бета то работает странно
        if(alpha <= 1 && alpha > 0 && beta <= 1 && beta > 0){
            result = true;
            intersectionPoint.x = a.x + alpha * (b.x - a.x);
            intersectionPoint.y = a.y + alpha * (b.y - a.y);
        }
    }

    return IntrResult(result, intersectionPoint);    
}


vPt findIntersectionPoints(Triangle& trg1, Triangle& trg2){
    vPt intersectionPolygon;

    //костыль
    auto uniqueInsert = [&intersectionPolygon](Point p){
        bool check = true;
        for (Point i : intersectionPolygon){
            if(i == p){
                check = false;
                break;
            }
        }
        if (check) intersectionPolygon.push_back(p);
    };

    vPt verts1 = trg1.getVerts();
    vPt verts2 = trg2.getVerts();

    //точки внутри O(n) для каждой точки, итого - O(2*n^2);
    for (Point i : verts1){
        if (pointIsInside(i, trg2)){
            uniqueInsert(i);
        }
    }

    for (Point i : verts2){
        if (pointIsInside(i, trg1)){
            uniqueInsert(i);
        }
    }

    //точки на пересечении O(n^2)
    for (int i = 0; i <= 2; i++){
        Point a1 = verts1[i];
        Point a2 = (i == 2) ? verts1[0] : verts1[i+1];
        for (int j = 0; j <= 2; j++){
            Point b1 = verts2[j];
            Point b2 = (j == 2) ? verts2[0] : verts2[j+1];
            IntrResult res = segmentIntersection(a1,a2,b1,b2);
            if (res.areIntersect == true){
                uniqueInsert(res.intersectionPoint);
            }
        }
    }


    return intersectionPolygon;
}

double polygonArea(vPt polygon) {
    int size = polygon.size();
    if (size <= 2) return 0;
    if (size == 3) return Triangle(polygon[0], polygon[1], polygon[2]).area();
    //сортировка по часовой стрелке
    Point center(0, 0);//сработает только для выпуклых многоугольников или нет я не знаю я вообще не знаю что у меня происходит в программе
    for (Point i : polygon){
        center.x += i.x;
        center.y += i.y;
    }
    center.x /= size;
    center.y /= size;

    sort(polygon.begin(), polygon.end(), [&center](Point& a, Point& b){
        return atan2(a.x - center.x, a.y - center.y) < atan2(b.x - center.x, b.y - center.y);
    });
    //

    /*auto advancedIndex = [&polygon](int i){
        int size = polygon.size();
        if(i >= size){
            return i % size;
        }
        if(i < 0){
            return size + (i % size);
        }
        return i;
    }*/
    //триангуляция
    //упрощённый вариант для выпуклых многоуг
    int k = 0;
    double area = 0;
    while (polygon.size() > 3){
        area += Triangle(polygon[0], polygon[polygon.size()], polygon[1]).area();
        polygon.erase(polygon.begin());
    }
    area += Triangle(polygon[0], polygon[polygon.size()], polygon[1]).area();
    
    return area;
}
/*double rNum(){
    double n;
    cin >> n;
    return n;
}*/

int main(){
    Triangle trg1;
    Triangle trg2;

    cout << polygonArea(findIntersectionPoints(trg1, trg2));
    
}