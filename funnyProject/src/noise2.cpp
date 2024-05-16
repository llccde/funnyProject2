#include<SFML/Graphics.hpp>
#include<vector>
#include<random>
#include<cmath>
#include<time.h>
#include<iostream>
#include<cmath>
#include<perlin.h>
#define PI 3.14159265359
#define GET_TWO_D_LIST_X_Y(LIMIT_X,LIMIT_Y,THE_X,THE_Y)\
for (int THE_X = 0; THE_X < LIMIT_X; THE_X++) {for (int THE_Y = 0; THE_Y < LIMIT_Y ; THE_Y++){

#define GET_TWO_D_LIST_PLUS(BEGIN_X,BEGIN_Y,LIMIT_X,LIMIT_Y,THE_X,THE_Y)\
for (int THE_X = BEGIN_X; THE_X < LIMIT_X; THE_X++) {for (int THE_Y = BEGIN_Y; THE_Y < LIMIT_Y ; THE_Y++){

#define END_GET_LIST }}

#define DOT_PRODUCT(VEC1,VEC2) (double)VEC1.x*VEC2.x+(double)VEC1.y*VEC2.y

sf::RenderWindow window(sf::VideoMode(800, 800), "SFML Polyline Example", sf::Style::Default);
struct int2
{
    int x,y;
};

double getAverageInRange(int range, sf::Vector2i pos,int2 mapSize,std::vector<std::vector<short int>>& map) {
            long int sum = 0;
            long int count = 0;

            int xStart = std::max(0, pos.x - range);
            int xEnd = std::min(mapSize.x, pos.x + range);
            int yStart = std::max(0, pos.y - range);
            int yEnd = std::min(mapSize.y, pos.y + range);

            GET_TWO_D_LIST_PLUS(xStart,yStart,xEnd,yEnd,i,j)

                sum += map[i][j]*(2*range-(abs(i-pos.x)+abs(j-pos.y)));
                count +=(2*range-(abs(i-pos.x)+abs(j-pos.y)));
            END_GET_LIST

            return ((double)sum / count)+0.5;
        }

std::vector<std::vector<short int>> interation(int range,int2 mapSize,std::vector<std::vector<short int>>& map){
    std::vector<std::vector<short int>> tempMap(mapSize.x ,std::vector<short int>(mapSize.y));
    //std::vector<std::vector<short int>> temp(mapSize.x ,std::vector<short int>(mapSize.y));
    GET_TWO_D_LIST_X_Y(mapSize.x,mapSize.y,x,y)
        tempMap[x][y] = getAverageInRange(range,{x,y},mapSize,map);
    END_GET_LIST;
    return tempMap;
}
        
void fill(int size,float z,int2 mapSize){
    std::vector<std::vector<short int>> map(mapSize.x ,std::vector<short int>(mapSize.y));
    sf::Image image;
    image.create(mapSize.x,mapSize.y);
    
    GET_TWO_D_LIST_X_Y(mapSize.x,mapSize.y,x,y)
        float xf = (float)x/size;
        float yf = (float)y/size;
        float l = (perlin(xf,yf,z)+1)*0.5;
        //image.setPixel(x,y,sf::Color(l*255,l*255,l*255));
        short int high;
        if(l>=0&&l<=0.55) high = 0;
        else if (l>=0.55&&l<0.6) high = 1;
        else if (l>=0.6&&l<=0.8) high = 3;
        else high = 7;
        map[x][y] = high;
    END_GET_LIST
    
    for (int i = 2; i>0; i--)
    {
        map = interation(4,mapSize,map);
    }
    
    GET_TWO_D_LIST_X_Y(mapSize.x,mapSize.y,x,y)
        short int high = map[x][y];
        if(high==0) image.setPixel(x,y,sf::Color::Blue);
        else if (high == 1||high ==2) image.setPixel(x,y,sf::Color::Yellow);
        else if (high == 3 ||high ==4)image.setPixel(x,y,sf::Color::Green);
        else image.setPixel(x,y,sf::Color(165,42,42));
    END_GET_LIST
    
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    window.draw(sprite);
    window.display();
}
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
        fill(50,(float)frame+0.5,{800,800});
        sf::sleep(sf::Time(sf::milliseconds(30)));
        
        frame++;

    }
    return 0;
	
}