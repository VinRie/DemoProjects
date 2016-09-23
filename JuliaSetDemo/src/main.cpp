#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <complex>
#include <iomanip>
#include <ctime>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>

const int WIDTH = 1200;
const int HEIGHT = 800;


const double zoom_start = 0.002;

// f(x) = z^2 – 0.221 – 0.713 i
std::complex <double> c(-0.940, 0.254);

const int FUNCTION_THRESHOLD = 2;
const int MAX_ITERATION = 254;




enum {NO_UPDATE,UPDATE_AVAILABLE};

sf::Image image;


int calculateIterations( double &real, double &imag){
    std::complex< double> z(real,imag);
    for(int iteration = 0; iteration < MAX_ITERATION ;iteration++){
        z = std::pow(z,2) + c;

        if(norm(z) > FUNCTION_THRESHOLD){
            return iteration;

        }
    }
    return MAX_ITERATION;
}


void initColorPalette(sf::Color *color_palette, const int &size){

    int r=0, g=0, b=0;
    for(int i = 0; i < size; ++i){
        if(i < 255/3) {
            color_palette[i] = sf::Color(r, 0, 0);
            r+=3;
        }else if(i < 2*255/3){
            color_palette[i] = sf::Color(r, g, 0);
            r-=3;
            g+=3;

        }else if (i <= 255){
            color_palette[i] = sf::Color(0, g, b);
            g-=3;
            b+=3;
        }
    }
}

int main()
{

    const std::string fileBaseName = "Julia Set -";
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Julia Set Demo");
    window.setFramerateLimit(30);


    image.create(WIDTH, HEIGHT, sf::Color(0, 0, 0));
    sf::Texture texture;
    sf::Sprite sprite;

    double zoom = zoom_start;
    double x_offset = 0.0;
    double y_offset = 0.0;


    sf::Color palette[255];
    bool status = UPDATE_AVAILABLE;

    initColorPalette(palette, 255);

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
                    break;

                    /* Custom Input Handling */
                case sf::Event::KeyPressed:
                    status = UPDATE_AVAILABLE;
                    switch (event.key.code) {
                        case sf::Keyboard::Space:
                            zoom = zoom_start;
                            x_offset = 0.0;
                            y_offset = 0.0;
                            break;
                        case sf::Keyboard::Q:
                            zoom *= 2;
                            break;
                        case sf::Keyboard::E:
                            zoom /= 2;
                            break;
                        case sf::Keyboard::W:
                            y_offset -= 10 * zoom;
                            break;
                        case sf::Keyboard::S:
                            y_offset += 10 * zoom;
                            break;
                        case sf::Keyboard::A:
                            x_offset -= 10 * zoom;
                            break;
                        case sf::Keyboard::D:
                            x_offset += 10 * zoom;
                            break;
                        case sf::Keyboard::P: {
                            auto t = std::time(nullptr);
                            auto tm = *std::localtime(&t);
                            std::ostringstream oss;
                            oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
                            auto str = oss.str();
                            image.saveToFile(fileBaseName + str + ".png");
                            break;
                        }

                        default:
                            break;
                    }

                    /* End Custom Input Handling */
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left){
                        x_offset += (event.mouseButton.x - WIDTH / 2.0) * zoom ;
                        y_offset += (event.mouseButton.y - HEIGHT / 2.0) * zoom ;

                        status = UPDATE_AVAILABLE;

                    }
                    break;
                default:
                    break;
            }
        }

        /* Start Gameupdate*/

        if(status == UPDATE_AVAILABLE) {


            double real = (0 - WIDTH  / 2) * zoom + x_offset;
            double imag = (0 - HEIGHT / 2) * zoom + y_offset;
            double imagStart = imag;

            for (unsigned int x = 0; x < WIDTH; x++, real+=zoom) {
                imag = imagStart;
                for (unsigned int y = 0; y < HEIGHT; y++, imag+=zoom) {

                    int value = calculateIterations(real, imag);
                    image.setPixel(x, y, palette[ value % 255]); }
            }
            status = NO_UPDATE;
        }

        texture.loadFromImage(image);
        sprite.setTexture(texture);

        /* End of Gameupdate */
        window.clear(sf::Color::Black);
        /* start drawing */

        window.draw(sprite);

        /* end drawing   */

        window.display();
    }

    return 0;
}