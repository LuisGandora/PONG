#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/Graphics/Text.hpp>
#include<random>
#include<cstdlib>
#include <chrono>
#include <thread>
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

int HEIGHT = 1920;
int WIDTH = 1080;
int x = 0;
int y = 0;
int vx;
int vy;
sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "PONG");
sf::RectangleShape shape(sf::Vector2f(10, 10));
sf::RectangleShape pong1(sf::Vector2f(10, 100));
int playery;
sf::RectangleShape pong2(sf::Vector2f(10, 100));
int playerScore =0;
int AIscore = 0;
sf::Font font;
sf::Font finalFont;
sf::Text playerHeader(std::to_string(playerScore), font);
sf::Text aiHeader(std::to_string(AIscore), font);
bool needReset = true;
sf::RectangleShape redshape(sf::Vector2f(10, 10));
int redspawntime =0;
bool MadeRed = false;
sf::RectangleShape blueshape(sf::Vector2f(10, 10));
int bluespawntime =0;
bool MadeBlue = false;
sf::RectangleShape greenshape(sf::Vector2f(10, 10));
int greenspawntime = 0;
bool MadeGreen = false;


int getRandomSpawn(int min, int max) {
    // Create a random device and a random engine
    std::random_device rd;  // Non-deterministic random number generator
    std::mt19937 gen(rd()); // Mersenne Twister random engine

    // Define the range of the random number
    std::uniform_int_distribution<> dis(min, max);

    // Generate and return the random number
    return dis(gen);
}

void setStartVelocity(){
    srand(time(0));
    vx = -((rand() % 2)+2); //srand(time(0)) when necessary
    vy = ((rand() % 2)+2);
}

void time_based_counter() {
    // This function will run in a separate thread
    while (true) {
        // Wait for 4 seconds (this is non-blocking for the main thread)
        std::this_thread::sleep_for(4s);
        
        // Code to be run every 4 seconds
        std::cout << "4 seconds have passed!" << std::endl;
    }
}

void MakeRedCube(){
    redshape.setPosition(getRandomSpawn(0,WIDTH), getRandomSpawn(0, HEIGHT));
    redshape.setFillColor(sf::Color::Red);
    window.draw(redshape);
    MadeRed = true;
}

void MakeBlueCube(){
    blueshape.setPosition(getRandomSpawn(0,WIDTH), getRandomSpawn(0, HEIGHT));
    blueshape.setFillColor(sf::Color::Blue);
    window.draw(blueshape);
    MadeBlue = true;
}

void MakeGreenCube(){
    greenshape.setPosition(getRandomSpawn(0,WIDTH), getRandomSpawn(0, HEIGHT));
    greenshape.setFillColor(sf::Color::Green);
    window.draw(greenshape);
    MadeGreen = true;
}

void startGame(){
    setStartVelocity();
    window.draw(shape);
    window.draw(pong1);
    window.draw(pong2);
    window.draw(playerHeader);
    window.draw(aiHeader);
    x = WIDTH/2;
    y = HEIGHT/2;
    shape.setPosition(x, y);
    pong1.setPosition(x/2, y);
    playery = y;
    pong2.setPosition(x+(x/2), y);
    window.display();
    sleep_for(1s);
    needReset = false;
}

void setScore(){

    playerHeader.setString(std::to_string(playerScore));
    playerHeader.setFont(font);  
    playerHeader.setPosition(pong1.getPosition().x, 0);
    playerHeader.setCharacterSize(25);
    playerHeader.setFillColor(sf::Color::White);
    playerHeader.setStyle(sf::Text::Bold);
    

    aiHeader.setString(std::to_string(AIscore));
    aiHeader.setFont(font); 
    aiHeader.setPosition(pong2.getPosition().x, 0);
    aiHeader.setCharacterSize(25);
    aiHeader.setFillColor(sf::Color::White);
    aiHeader.setStyle(sf::Text::Bold);
}

bool Collision(sf::RectangleShape &targetPong, sf::RectangleShape &ball){
    
    sf::FloatRect j = targetPong.getGlobalBounds();
    sf::FloatRect k = ball.getGlobalBounds();

    return j.intersects(k);
}

void runBall(){ 
    x=x+vx;
    y=y+vy;
    if (y<0 or y>HEIGHT)
        vy=-vy;
        y=y+vy;
    if(Collision(pong1, shape))
        vx = -vx;
        x = x + vx+ 50;
        shape.setPosition(x, y);
    if(Collision(pong2, shape))
        vx = -vx;
        x = x + vx-50;
        shape.setPosition(x, y);
    if (x<0 or x>(WIDTH)){
        if(x < 0){
            window.clear();
            AIscore += 1;
            setScore();
        }  
        else if(x>(WIDTH)){
            window.clear();
            playerScore += 1;
            setScore();
        }
        startGame();
    }
    shape.setPosition(x, y);
    window.draw(shape);
    window.draw(pong1);
    window.draw(pong2);
    sleep_for(10ns);
    sleep_until(system_clock::now() + 0.05s);
    
     ///change to pixels
  
}



//use g++ Pong.cpp -lsfml-graphics -lsfml-window -lsfml-system to compile
int main(){
    setStartVelocity();
    font.loadFromFile("pong-score.ttf"); //something is wrong with the font and the call to the variables
    finalFont.loadFromFile("ARIAL.TTF");
    setScore();
    
    while (window.isOpen()) {
        sf::Event event;
        
        
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();
        window.draw(playerHeader);
        window.draw(aiHeader);
        if(needReset){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            startGame();
            setScore();
        }
        else if(playerScore >= 10){
            window.clear();
            sf::Text FinalScore("You Win", finalFont);
            window.draw(FinalScore);
            FinalScore.setCharacterSize(200);
            FinalScore.setFillColor(sf::Color::White);
            FinalScore.setStyle(sf::Text::Bold);
            sf::FloatRect textBounds = FinalScore.getLocalBounds();
            FinalScore.setPosition(
                WIDTH / 2 - textBounds.width / 2,
                HEIGHT / 2 - textBounds.height / 2
            );
            
        }
        else if(AIscore >= 10){
            window.clear();
            sf::Text FinalScore("You Lost", finalFont);
            window.draw(FinalScore);
            FinalScore.setCharacterSize(200);
            FinalScore.setFillColor(sf::Color::White);
            FinalScore.setStyle(sf::Text::Bold);
            sf::FloatRect textBounds = FinalScore.getLocalBounds();
            FinalScore.setPosition(
                WIDTH / 2 - textBounds.width / 2,
                HEIGHT / 2 - textBounds.height / 2
            );
            
        }
        else{
            runBall();
        }
        int tempy;
        if(!MadeRed){
            redspawntime++;
            if(redspawntime > 20)
                MakeRedCube();
                redspawntime = 0;
        }
        else{

            if(MadeRed){
                if(Collision(shape, redshape)){
                    float tempvarx =  round(shape.getSize().x *1.25);
                    float tempvary = round(shape.getSize().y * 1.25);
                    shape.setSize(sf::Vector2f(tempvarx, tempvary));
                    MadeRed = false;
                }
                window.draw(redshape);
            }
        }
        if(!MadeBlue){
            bluespawntime++;
            if(bluespawntime > 20)
                MakeBlueCube();
                bluespawntime = 0;
        }
        else{
            if(MadeBlue){
                if(Collision(shape, blueshape)){
                    
                    vx++;
                    MadeBlue= false;
                }
                window.draw(blueshape);
            }
        }
        if(!MadeGreen){
            greenspawntime++;
            if(greenspawntime > 20)
                MakeGreenCube();
                greenspawntime = 0;
        }
        else{
            if(MadeGreen){
                if(Collision(shape, greenshape)){
                    
                    float tempvarx =  round(shape.getSize().x *0.75);
                    float tempvary = round(shape.getSize().y * 0.75);
                    shape.setSize(sf::Vector2f(tempvarx, tempvary));
                    MadeGreen = false;
                }
                window.draw(greenshape);
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            tempy = playery - 10;
            if(tempy > 0){
                pong1.setPosition(pong1.getPosition().x, tempy);
                playery = tempy;
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            tempy = playery + 10;
            if(tempy < HEIGHT){
                pong1.setPosition(pong1.getPosition().x, tempy);
                playery = tempy;
            }
        }
        if(pong2.getPosition().y < y) 
            pong2.setPosition(pong2.getPosition().x, (pong2.getPosition().y + 20));
        if(pong2.getPosition().y > y)
            pong2.setPosition(pong2.getPosition().x, (pong2.getPosition().y - 20));
        
         // Wait for a brief moment before resetting
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        window.display();
    }

    
    
    

    return 0;
}
