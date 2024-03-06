Алгоритм нахождения площади пересечения двух треугольников(может быть легко расширен до многоугольников)

1. Создаются реугольники: каждый 3м точкам.
2. Создаём массив(сначала он очевидно пустой) с точками, которые образуют область пересечения.
3. Добавляем точки из треуг1, которые находятся внутри(в этом алгритме не касаясь границы) треуг2.
4. Добавляем точки из треуг2, которые находятся внутри треуг1.

>Алгоритм определения того, что точка находится внутри полигона, работает по принципу лучей, направленных вдоль ОХ из исследуемой точки.
>ЕСЛИ луч пересекает многоугольник нечётное количество раз -> точка внутри ИНАЧЕ снаружи
>Сложность - n1 точек проходит проверку на пересечение n2 рёбер и наоборот, т.е. ~n2*n1 + n1*n2 = 2*n2*n1 т.е. O(n^2) что не очень хорошо но пофиг

5. Находим точки пересечения отрезков перебирая всевозможные пары и опять получая сложность O(n^2)

>Алгоритм нахождения точки пересечения основан на векторной алгебре, важную роль играет параметрический способ задания вектора.
>По сути точка пересечения это два вектора: один из вершины одного отрезка, другой из вершины другого. Оба вектора указывают на точку пересечения.
>Суть сводится к поиску этих параметров.

### __Отменяем__ все следующие шаги, если вершин получилось 2. Если 3, то просто выводим площадь, которая является функцией класс Треугольник

6. Собираем все эти точки и сортируем по часовой стрелке относительно центра.

> - Центр.х = среднее арифметическое абсцисс вершин полигона
> - Центр.у = среднее арифметическое ординат вершин полигона
>Сортировка выполняется по углу собственного ветора каждой вершины, за начало координат считает Центр.

7. Через простую триангуляцию, которая работает только для выпуклых треугольников, считаем площадь получившейся фигуры

> Смысл данной триангуляции такой, что мы просто берём вершину N, потом N-1, потом N+1, прибавляем площадь треугольника по этим трём вершинам
> к итоговому результату, а затем убираем из списка рассматриваемых нами вершин вершину с индексом N и начинаем заново. По сути можно просто
> делать всё, опираясь на 0-й или 1-й элемент массива, что я и сделал.
