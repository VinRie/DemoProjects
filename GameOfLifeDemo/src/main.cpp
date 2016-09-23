#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Cell.h"
#include <math.h>
#include <memory>

sf::Clock cellUpdateClock; // starts the clock
sf::RenderWindow window(sf::VideoMode(1024,800, sf::Style::Fullscreen), "GameOfLife");


// Grid of Cells
const int grid_width = 40;
const int grid_height = 40;
std::vector<std::shared_ptr<Cell>> grid;


sf::Vector2f scale;

enum {EDIT, PLAY};

bool gameMode = EDIT;


void initWindow(){
    scale.x = window.getSize().x / grid_width;
    scale.y = window.getSize().y / grid_height;
}


std::shared_ptr<Cell> getCell(int x, int y){
    if(y > grid_height || y < 0 || x > grid_width || x < 0)
        throw std::out_of_range ("X,Y are not valid");
    return grid[(y*grid_height) + x];
}

std::shared_ptr<Cell> getCell(sf::Vector2i pos){
   return getCell(pos.x, pos.y);
}


sf::Vector2i getIndexFromPos(sf::Vector2<float> pos){

    sf::Vector2i result;

    // x * scale.x = window.getSize().x / grid_width * x;
    result.x = round(pos.x) / scale.x;
    result.y = round(pos.y) / scale.y;

   // std::cout << "Get Index From Pos called  pos.x:" << pos.x << " pos.y:"  << pos.y << " scale.x: " << scale.x << "scale.y: " << scale.y << "result.x: "<< result.x << "result.x: " << result.y << std::endl;

    return result;

}

/**
 * Initializes the Grid
 */
void initGrid(){

    for(int y = 0; y < grid_height; ++y) {
        for (int x = 0; x < grid_width; ++x) {
            grid.push_back(std::make_shared<Cell>(Cell(x,y, scale)));
            //std::cout << "created: x " << x << "created y: " << y << std::endl;
        }
    }

    // Set the Neigbors
    for(auto it=grid.begin(); it!=grid.end(); ++it)
        for(int rel_y = -1; rel_y <= 1; rel_y++){
            for(int rel_x = -1; rel_x <=  1; rel_x++){
                if(rel_y == 0 && rel_x == 0) {
                    continue;
                }
                else{
                    // Nachbarn sind innerhalb des Grids (X-Achse)
                    if(((*it)->getX() + rel_x) >= 0  && ((*it)->getX() + rel_x) < grid_width){
                        // Nachbarn sind innerhalb des Grids (Y-Achse)
                        if(((*it)->getY() + rel_y) >= 0  && ((*it)->getY() + rel_y) < grid_height) {

                            (*it)->addNeighbors(getCell((*it)->getX() + rel_x, (*it)->getY() + rel_y));

                        }
                    }

                }
            }
        }

}


/**
 * calculates the new cell states.
 *

 */
void calculateNextGridState(){

    for(auto it=grid.begin(); it != grid.end(); ++it){

        bool currentState = (*it)->getCurrentState();
        int currentNieghborPopulation = (*it)->getNeighborPopulation();

        //Any live cell with two or three live neighbours lives on to the next generation.
        if(currentState == 1 && (currentNieghborPopulation == 2 || currentNieghborPopulation == 3)) {
            (*it)->setNextState(1);
            //std::cout << *it << " - " << (*it)->getNeighborPopulation() << " <<-- stayin alive" << std::endl;
            continue;
        }
        //Any live cell with fewer than two live neighbours dies, as if caused by under-population.
        else if(currentState == 1 && currentNieghborPopulation < 2) {
            (*it)->setNextState(0);
            //std::cout << *it << " died caused by under-population " << std::endl;
        }
        //Any live cell with more than three live neighbours dies, as if by over-population.
        else if(currentState == 1 && currentNieghborPopulation> 3){
            (*it)->setNextState(0);
            //std::cout << *it << " died caused by over-population " << std::endl;

        }
        //Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
        else if(currentState == 0 && currentNieghborPopulation  == 3 ){
            (*it)->setNextState(1);
            //std::cout << *it << " birth " << std::endl;

        }
    }
}

void updateGrid(){
    if(cellUpdateClock.getElapsedTime().asSeconds() >= 0.5) {
        calculateNextGridState();
        for (auto it = grid.begin(); it != grid.end(); ++it)
            (*it)->update();
        cellUpdateClock.restart();
    }
}

void drawGrid(sf::RenderWindow &window){
    for(auto it=grid.begin(); it != grid.end(); ++it)
        (*it)->draw(window);

}

int main()
{

    /* Initialize GameObjects*/

    initWindow();
    initGrid();


    //sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    //window.create(sf::VideoMode(1024, 768, desktop.bitsPerPixel), "SFML window");

    cellUpdateClock.restart();

    /* end of init */

    while(window.isOpen()) {
        sf::Event event;

        while(window.pollEvent(event)){
            switch (event.type)
            {
                // window closed
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    initWindow();
                    break;

                /* Custom Input Handling */
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::E && gameMode == PLAY){
                        gameMode = EDIT;
                        break;
                    }
                    if (event.key.code == sf::Keyboard::P && gameMode == EDIT){
                        gameMode = PLAY;
                        break;
                    }
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        sf::Vector2i clickedIndex = getIndexFromPos(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                        std::shared_ptr<Cell> clickedCell = getCell(clickedIndex);
                       // std::cout << "Clicked Cell Coordinates - x: "  << clickedIndex.x << " y: " << clickedIndex.y  << " neighbors: " << (*clickedCell).getNeighborPopulation() << std::endl;
                        (*clickedCell).setState(!(*clickedCell).getCurrentState());

                    }
                    break;
                /* End Custom Input Handling */
                default:
                    break;
            }
        }

        /* Gameupdate */
        if(gameMode==PLAY) {
            updateGrid();
        }

        /* End of Gameupdate */
        window.clear(sf::Color::Black);

        /* start drawing */
        drawGrid(window);
        /* end drawing   */

        window.display();

    }

    return 0;
}