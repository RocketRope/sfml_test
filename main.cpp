#include <iostream>

#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "rnd.h"

class Game : private sf::RenderWindow
{
    private:

        int rows    = 10;
        int columns = 10;

        sf::RectangleShape block;

        std::vector<sf::RectangleShape*> blocks;

        const sf::Color colors[8] = 
        {
            sf::Color(0x528bffff), // caret
            sf::Color(0xe06c75ff), // pink
            sf::Color(0x56b6c2ff), // cyan
            sf::Color(0x98c379ff), // lime
            sf::Color(0xc678ddff), // purple
            sf::Color(0xd19a66ff), // brown
            sf::Color(0xe5c07bff), // gold
            sf::Color(0x61afefff) // blue
        };


    public:

        Game(int width, int height)
        {
            create(sf::VideoMode(width, height), "Block");

            sf::Image icon;
            icon.loadFromFile("icon.png");
            setIcon( icon.getSize().x, 
                     icon.getSize().y,
                     icon.getPixelsPtr() 
                    );

            setVerticalSyncEnabled(true);

            blocks.resize(rows * columns, nullptr);

            createNewBlock();

            onResize();
        }

        ~Game()
        {
            for( auto b : blocks )
            {
                if ( b != nullptr )
                    delete b;
            }
        }

        void onResize()
        {
            std::cout << getSize().x << " : " << getSize().y << std::endl;

            setView( sf::View( sf::Vector2f(rows / 2, columns / 2),
                               sf::Vector2f(rows, columns) 
                             ) 
                    );
        }

        void run()
        {
            while ( isOpen() )
            {
                sf::Event event;
                
                while ( pollEvent(event) )
                {
                    if ( event.type == sf::Event::Closed )
                        close();

                    if ( event.type == sf::Event::KeyPressed )
                    {
                        auto key = event.key.code;

                        if ( key == sf::Keyboard::Key::Escape )
                            close();

                        auto old_pos = block.getPosition();
    
                        if ( key == sf::Keyboard::Key::W )
                             block.move(  0, -1);
                        if ( key == sf::Keyboard::Key::S )
                             block.move(  0,  1);
                        if ( key == sf::Keyboard::Key::A )
                             block.move( -1,  0);
                        if ( key == sf::Keyboard::Key::D )
                             block.move(  1,  0);

                        auto pos =  block.getPosition();

                        if ( collision() )
                        {
                            block.setPosition(old_pos);
                            createNewBlock();
                        }
                        else if (  pos.y == 0 || pos.y == rows - 1 )
                            createNewBlock();
                        else if (  pos.x == 0 || pos.x == columns - 1 ) 
                            createNewBlock();

                        
                            

                    }
                }

                clear( sf::Color(33 , 37 , 43) );
                
                draw(block);

                for( auto b : blocks )
                {
                    if( b != nullptr )
                        draw(*b);
                }
                display();
            }
        }

        private:

        void createNewBlock()
        {
            auto position = block.getPosition(); 
            int index = (position.y * columns) + position.x; 

            if ( blocks[index] == nullptr )
                blocks[index] = new sf::RectangleShape(block); 

            block.setSize( sf::Vector2f(1,1) );

            do
            {
                block.setPosition( Random<int>(1, columns - 1) , Random<int>(1, rows - 1) );
            } while( collision() );

            int c = Random<int>(0, sizeof(colors) / sizeof(sf::Color));
            block.setFillColor( colors[c] );
        }

        bool collision()
        {
            auto position = block.getPosition(); 

            int index = (position.y * columns) + position.x; 

            if( blocks[index] == nullptr )
                return false;

            return true;
        }

        
};

int main()
{

    Game game(800, 600);
    //game.setVerticalSyncEnabled(true);
    game.run();

    return 0;
}