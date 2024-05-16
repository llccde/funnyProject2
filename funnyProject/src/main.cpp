#include<SFML/Graphics.hpp>
#include<vector>
#include<random>
#include<cmath>
#include<time.h>
#include<iostream>
#define GET_TWO_D_LIST_X_Y(LIMIT_X,LIMIT_Y,THE_X,THE_Y)\
for (int THE_X = 0; THE_X < LIMIT_X; THE_X++) {for (int THE_Y = 0; THE_Y < LIMIT_Y ; THE_Y++){

#define GET_TWO_D_LIST_PLUS(BEGIN_X,BEGIN_Y,LIMIT_X,LIMIT_Y,THE_X,THE_Y)\
for (int THE_X = BEGIN_X; THE_X < LIMIT_X; THE_X++) {for (int THE_Y = BEGIN_Y; THE_Y < LIMIT_Y ; THE_Y++){

#define END_GET_LIST }}

template <class T>
struct orderedPair
{
    T first;
    T second;
    public:
        orderedPair(T one,T two):first(one),second(two){};

        orderedPair<T> flip(){
            return {second,first};
        }

        bool same(orderedPair<T> pairTwo){
            return (this->first == pairTwo.first) && (this->second == pairTwo.second);
        }
};
template <class Q>
struct pair
{
    Q one,two;
    pair(Q X,Q Y):one(x),two(Y){};
};



sf::RenderWindow window(sf::VideoMode(800, 800), "SFML Polyline Example", sf::Style::Default);
int main() {
	srand(time(NULL));
    int frame = 0;
    
	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // 对窗口上的关闭按钮点击事件进行响应
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::sleep(sf::Time(sf::milliseconds(30)));
        
        frame++;

    }
    return 0;
	
}