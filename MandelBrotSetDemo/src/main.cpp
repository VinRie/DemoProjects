#include <SFML/Graphics.hpp>
#include <iostream>

const int WIDTH = 1200;
const int HEIGHT = 800;
const int MAX_ITERATION = 500;
const double zoom_start = 0.004;


enum {NO_UPDATE,UPDATE_AVAILABLE};

sf::Image image;

int calculateMandelBrotSet(long double real, long double imag){

    long double start_real = real;
    long double start_imag = imag;

    for(int iteration = 0; iteration < MAX_ITERATION ;iteration++){
        long double r2 = real*real;
        long double i2 = imag*imag;
        if(i2+r2 > 4.0)
            return iteration;

        imag = 2 * real * imag + start_imag;
        real = r2 - i2 + start_real;

    }
    return MAX_ITERATION;
}


void initColorPalette(sf::Color *color_palette, const int &size){

    int r=120, g=90, b=90;
    for(int i = 0; i < size; ++i){
        if(i < 255/4) {
            color_palette[i] = sf::Color(r, 0, 0);
            r++;
        }else if(i < 2*255/4){
            color_palette[i] = sf::Color(r, g, 0);
            g++;
            r--;

        }else if (i < 3*255/4){
            color_palette[i] = sf::Color(0, g, b);
            b++;
            g--;
        }else{
            color_palette[i] = sf::Color(r,0, b);

        }
    }



}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set Demo");
    window.setFramerateLimit(30);


    image.create(WIDTH, HEIGHT, sf::Color(0, 0, 0));
    sf::Texture texture;
    sf::Sprite sprite;

    double zoom = 0.005;
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
                        case sf::Keyboard::P:
                            std::cout <<  "x_offset" << x_offset << ", y_offset:" << y_offset << std::endl;
                            break;

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
            long double real = (0 - WIDTH  / 2.0) * zoom + x_offset ;
            long double imag = (0 - HEIGHT / 2.0) * zoom + y_offset;
            long double imagStart = imag;
            for (unsigned int x = 0; x < WIDTH; x++, real+=zoom) {
                imag = imagStart;
                for (unsigned int y = 0; y < HEIGHT; y++, imag+=zoom) {
                    int value = calculateMandelBrotSet(real,imag);
                    image.setPixel(x, y, palette[ value % 255]);
                }
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