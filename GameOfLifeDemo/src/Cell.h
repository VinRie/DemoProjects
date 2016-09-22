#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

/*
 *     *-*-*-*
 *     |8|1|2|
 *     *-*-*-*
 *     |7|x|3|
 *     *-*-*-*
 *     |6|5|4|
 *     *-*-*-*
 */

class Cell {

public:

    Cell(const int x_in, const int y_in, sf::Vector2f &scale_in):
    x(x_in),
    y(y_in),
    scale(scale_in)
    {


    }

    ~Cell(){

    }

    bool getCurrentState(){
        return state;
    }

    int getX(){
        return x;
    }

    int getY(){
        return y;
    }

    std::vector<std::shared_ptr<Cell>> getNeighbors(){
        return neighbors;
    }

    int getNeighborPopulation(){
        int population = 0;
        for(auto it=neighbors.begin(); it != neighbors.end(); it++){
            if((*it)->getCurrentState())
                population++;
        }
        return population;
    }

    void addNeighbors(std::shared_ptr<Cell>  cell){
        neighbors.push_back(cell);
    }

    void setNextState(bool nextState_in){
        nextState = nextState_in;
    }

    void setState(bool state_in){
        state = state_in;
    }
    void update(){
        state = nextState;
    }

    void draw(sf::RenderWindow &window){
        rectangle.setPosition(x*scale.x, y*scale.y);
        rectangle.setSize(sf::Vector2f(scale.x, scale.y));

        if(state) {
            rectangle.setFillColor(sf::Color::White);
            rectangle.setOutlineColor(sf::Color::Black);

        }else{
            rectangle.setFillColor(sf::Color::Black);
            rectangle.setOutlineColor(sf::Color::White);
        }



        window.draw(rectangle);
    }

    friend std::ostream &operator<<(std::ostream &stream, Cell &rhs){

        stream << "Cell  x:" << rhs.getX() << " y:" << rhs.getY() << " Currentstate: " << rhs.getCurrentState();

        return stream;
    }

private:
    //Debug
    sf::Text text;
    sf::Font font;


    sf::RectangleShape rectangle;
    // 0 = dead; 1 = alive.
    bool state = 0;
    bool nextState = 0;

    sf::Vector2f scale;


    // Position on the Grid. (0,0) is center. Grid is endless in positive x,y and negative x,y
    const float x, y;
    /*
     *     *-*-*-*
     *     |8|1|2|
     *     *-*-*-*
     *     |7|x|3|
     *     *-*-*-*
     *     |6|5|4|
     *     *-*-*-*
     */
    std::vector<std::shared_ptr<Cell>> neighbors;

};