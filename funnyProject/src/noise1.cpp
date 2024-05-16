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



sf::RenderWindow window(sf::VideoMode(800, 800), "SFML Polyline Example", sf::Style::Default);
struct size2
{
    int x;
    int y;
};

class randomMap{
    public:
        size2 mapSize;
        std::vector<std::vector<short int>> map;
        sf::Image mapImg;
        short int precision;
        randomMap(size2 size,int precision){
            this->precision = precision;
            map.resize(size.x,std::vector<short int>(size.y));
            mapImg.create(size.x,size.y);
            this->mapSize = size;
        }
        void randomFill(){

            GET_TWO_D_LIST_X_Y(this->mapSize.x,this->mapSize.y,i,j)
                map[i][j] = rand()%precision;
            END_GET_LIST
        }
        void draw(){

            GET_TWO_D_LIST_X_Y(this->mapSize.x,this->mapSize.y,i,j)
                sf::Color color = (map[i][j]>(precision/2)?sf::Color::White : sf::Color::Black);
                mapImg.setPixel(i,j,color);
            END_GET_LIST

            sf::Texture texture;
            texture.loadFromImage(mapImg);
            sf::Sprite sprite;
            sprite.setTexture(texture);
            sprite.setPosition({0,0});
            window.draw(sprite);
        }; 
        /**
         * 向左向右格式range距离；
        */
        double getAverageInRange(int range, sf::Vector2i pos) {
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

        void interation(int range){
            std::vector<std::vector<short int>> tempMap(mapSize.x ,std::vector<short int>(mapSize.y));
            //std::vector<std::vector<short int>> temp(mapSize.x ,std::vector<short int>(mapSize.y));

            GET_TWO_D_LIST_X_Y(this->mapSize.x,this->mapSize.y,x,y)
                tempMap[x][y] = getAverageInRange(range,{x,y});
            END_GET_LIST;

            this->map = tempMap;

        }
        
    };


int main() {
	srand(time(NULL));
    bool pause = true;
    randomMap map({800,800},1000+(rand()%1000));
    map.randomFill();
    int frame  = 0;
    int interationRange = 4;
    bool nextFrame = true;
    bool input = false;
    std::cout<<"range is "<<interationRange<<"\n";
	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // 对窗口上的关闭按钮点击事件进行响应
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::A){
                    map.randomFill();
                }
                if(event.key.code == sf::Keyboard::Space){
                    pause = !pause;
                }
                if(event.key.code == sf::Keyboard::F&&pause){
                    nextFrame = true;
                }
                if(event.key.code == sf::Keyboard::I&&pause){
                    input = true;
                }

            }
        }


        sf::sleep(sf::Time(sf::milliseconds(5)));
        if(!pause){
            window.clear(sf::Color::Black);
            map.interation(interationRange);
            map.draw();
            window.display();
        }
        else if(pause&&nextFrame){
            window.clear(sf::Color::Black);
            map.interation(interationRange);
            map.draw();
            window.display();
            nextFrame = false;
        }
        else if(pause&&input){
            input = false;
            std::cout<<"range is "<<interationRange<<"\n";
            std::cout<<"read \n";
            std::cin>>interationRange;
            std::cout<<"range change to "<<interationRange<<"\n"; 
        }


        frame++;

    }
    return 0;
	
}