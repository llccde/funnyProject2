#include<SFML/Graphics.hpp>
#include<vector>
#include<random>
#include<cmath>
#include<time.h>
#include<iostream>
const int maxBranchNum = 3;
const int maxLevel = 8;
const int maxY = 50;
const int mineY = 25;
const int maxAge = 300;
const int newBranchEnergyCost = 100;
const int addAgeEnergyCost = 20;
const float basicEnergyCostLevel = 0.1;//per age
const int energyCycleTime = 720;
const int maxDebtEnergy = -1500;

sf::RenderWindow window(sf::VideoMode(800, 800), "SFML Polyline Example", sf::Style::Default);
sf::Color interpolateBetweenTwoColors(sf::Color color1, sf::Color color2, float level){
    if(level < 0.f) level = 0.f;
    if(level > 1.f) level = 1.f;
    sf::Color result;
    result.r = color1.r * (1 - level) + color2.r * level;
    result.g = color1.g * (1 - level) + color2.g * level;
    result.b = color1.b * (1 - level) + color2.b * level;
    return result;
}
float curve_grow_with_level(int level){
	return (sqrt(level)/sqrt(maxLevel));
}
float curve_grow_with_age(int age){
	return(sqrt(age)/sqrt(maxAge));
}
float curve_decline_with_level(int level){
	return 2-1.5*(sqrt(level)/sqrt(maxLevel));
}


class branch{
	public:
	std::vector<branch> children;
	int age = 10;
	sf::Color color = sf::Color::Green;
	sf::Vector2f end;
	sf::Vector2f currentEnd;//grow with ages
	long long int saveEnergy;
	long long int level;

	branch(int level){
		this->level = level;
		float y = (rand()%(maxY-mineY))+mineY;
		float x = sqrt(maxY*maxY-y*y)*(((rand()%2)==1)?-1:1)*curve_grow_with_level(this->level)*2;
		this->end = sf::Vector2f{x,y}*curve_decline_with_level(level);
		this->currentEnd = end*curve_grow_with_age(age);
	};
	void totally(long long int &totalAge,long long int &positiveEnergy,long long int &negativeEnergy,bool isRoot = false) {
		totalAge += age;
		if (saveEnergy > 0){
			positiveEnergy += saveEnergy;
		}
		else if (saveEnergy < 0){
			negativeEnergy += saveEnergy;
		}
		for(auto &child : children){
			child.totally(totalAge, positiveEnergy, negativeEnergy);
		}
	}
	void draw(sf::Vector2f rootPoint){
		sf::Vertex p1(rootPoint);
		sf::Vertex p2(rootPoint+this->currentEnd);
		p1.color = this->color;
		p2.color = this->color;
		sf::Vertex l[] = {p1,p2};
		window.draw(l,2,sf::Lines);

		for (size_t i = 0; i < children.size(); i++)
		{
			children[i].draw(rootPoint+this->currentEnd);
		}
		
	}
	void shortEnergy(){

	
		if(this->saveEnergy<maxDebtEnergy){
			if(this->children.size()>0){
				this->saveEnergy+=this->destroyAllChild(true);
			}
			else{
				this->saveEnergy+=addAgeEnergyCost*0.66;
				this->age-=1;

				if(age<=1) age = 1;

				float ageLevel = curve_grow_with_age(age);
				this->currentEnd = end * ageLevel;
				this->color = interpolateBetweenTwoColors(sf::Color::Green, sf::Color::Red, ageLevel);
			}
		}
	}
	/**
	 * 返回营养
	*/
	long long int destroyAllChild(bool isRoot = false){

		long long int summary = this->age*addAgeEnergyCost*0.8+((this->saveEnergy>0)?this->saveEnergy*0.6:0);
		if(isRoot) summary = 0;

		for (auto i = children.begin(); i != children.end();) {
			summary +=i->destroyAllChild();
			i = children.erase(i);
		}

		return summary;
		
	}
	void addAge(long long int& energy) {
		if(energy<addAgeEnergyCost) return;
		if (this->age < maxAge) {
			float ageLevel = curve_grow_with_age(age);
			this->currentEnd = end * ageLevel;
			this->color = interpolateBetweenTwoColors(sf::Color::Green, sf::Color::Magenta, ageLevel);
			age += 1;
			energy-=addAgeEnergyCost;
		}
	}

	void newBranch(long long int& energy) {
		if(this->saveEnergy<newBranchEnergyCost) return;
		bool isGrow = rand()%20 == 0;
		if(isGrow && (this->level < maxLevel) && (children.size() < maxBranchNum)) {
			branch newBranch(this->level+1);
			children.push_back(newBranch);
			energy-=newBranchEnergyCost;
		}
	}

	void keepLife(long long int& energy){
		this->saveEnergy-=basicEnergyCostLevel*this->age;
		if(this->saveEnergy<0) this->shortEnergy();
	}
	void childGrow(long long int& energy){
		if(energy<0) {
			for (size_t i = 0; i < children.size(); i++) {
				children[i].grow(0,energy);
			}
			return;
		};

		for (size_t i = 0; i < children.size(); i++) {
			children[i].grow((energy/this->children.size())/1.5,energy);
			energy-=(energy/this->children.size())/1.5;
		}
	}
	void grow(long long int energy,long long int& fatherBranchEnergy) {
		this->saveEnergy+=energy;
		keepLife(this->saveEnergy);
		addAge(this->saveEnergy);

		childGrow(this->saveEnergy);
		newBranch(this->saveEnergy);

	}


};


int main() {
	srand(time(NULL));
	branch root(0);
	sf::View view = window.getDefaultView();
	view.setSize(view.getSize().x, -view.getSize().y); // 调整视图的大小，使其在 y 轴上反转
	window.setView(view);
	int frames = 0;
	std::vector<int> info;
	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // 对窗口上的关闭按钮点击事件进行响应
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

		root.draw({400,0});
		long long int a;
		int energy = 15000*((float)(frames%energyCycleTime)/(float)energyCycleTime)+200;
		//std::cout<<energy<<"\n";
		root.grow(energy,a);

		window.display();
		sf::sleep(sf::Time(sf::milliseconds(30)));
		frames++;
;
		if(frames%30==0){
			long long int a1[3] = {0,0,0};
			root.totally(a1[0],a1[1],a1[2],true);
			std::cout<<"\n total age"<<a1[0]<<"\n positive"<<a1[1]<<"\n negative"<<a1[2]<<"\n enegy"<<energy<<"\n";
		}
    }
    return 0;
	
}