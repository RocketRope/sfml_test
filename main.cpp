#include <iostream>

#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "rnd.h"

class Game : private sf::RenderWindow
{
    private:

        int rows    = 16;
        int columns =  9;

        sf::RectangleShape backgroud;
        sf::RectangleShape grid;
        
        sf::Font font;
        sf::Text hold;
        sf::Text next;

        sf::CircleShape block;
        sf::CircleShape hold_block;
        sf::CircleShape next_block;

        std::vector<sf::CircleShape*> blocks;

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
            // Window / RenderTarget
            sf::ContextSettings settings;
            settings.antialiasingLevel = 8;
            create( sf::VideoMode(width, height),
                    "Block", 
                    sf::Style::Default, 
                    settings 
                  );

            setVerticalSyncEnabled(true);

            // Set Icon
            sf::Image icon;
            icon.loadFromFile("icon.png");
            setIcon( icon.getSize().x, 
                     icon.getSize().y,
                     icon.getPixelsPtr() 
                    );            

            // Load Font
            if ( !font.loadFromFile("Robotica.ttf") )
            {
                std::cout << "Error font!!!" << std::endl;
            }

            // Hold
            hold.setFont(font);
            hold.setString("HOLD");
            hold.setFillColor(sf::Color(0xBBBBBBFF));
            hold.setCharacterSize(100);
            hold.setScale(0.01f, 0.01f);
            auto bound = hold.getLocalBounds();
            hold.setOrigin(bound.width, 0);
            hold.setPosition(columns * (-0.1f), 0.0f);

            bound = hold.getGlobalBounds();

            hold_block.setFillColor( getRandomColor() );
            hold_block.setRadius( bound.height * 3.0f / 2.0f );
            hold_block.setPosition( bound.left * 0.85f, bound.height * 2.0f );

            // Next
            next.setFont(font);
            next.setString("NEXT");
            next.setFillColor(sf::Color(0xBBBBBBFF));
            next.setCharacterSize(100);
            next.setScale(0.01f, 0.01f);
            next.setPosition(columns * 1.1f, 0.0f);

            bound = next.getGlobalBounds();

            next_block.setFillColor( getRandomColor() );
            next_block.setRadius( bound.height * 3.0f / 2.0f );
            next_block.setPosition( bound.left * 1.05f , bound.height * 2.0f );

            // Grid
            grid.setSize( sf::Vector2f(columns, rows) );
            grid.setFillColor(sf::Color(0x00000000));
            grid.setOutlineColor(sf::Color(0x282C34FF));
            grid.setOutlineThickness(0.2f);

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

            float margin_x = 2.0f;
            float margin_y = 1.1f;

            float grid_ratio   = (columns * margin_x ) / ( rows * margin_y ) ;

            sf::Vector2f view_size;
            float screen_ratio = (float) getSize().x / (float) getSize().y;

            if ( screen_ratio > grid_ratio )
            {
                view_size.x = rows * screen_ratio * margin_y;
                view_size.y = rows * margin_y;
            }
            else
            {
                view_size.x = columns * margin_x;
                view_size.y = columns / screen_ratio * margin_x;
            }

            setView( sf::View( sf::Vector2f(columns / 2.0f, rows / 2.0f),
                               view_size
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

                        if ( key == sf::Keyboard::Key::Space )
                        {
                            sf::Color temp = hold_block.getFillColor();
                            hold_block.setFillColor( block.getFillColor() );
                            block.setFillColor( temp );
                        }

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

                clear( sf::Color(0x21252BFF) );

                draw(grid);
                draw(block);
                draw(hold);
                draw(hold_block);
                draw(next);
                draw(next_block);
    
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
                blocks[index] = new sf::CircleShape(block); 

            block.setRadius( 0.5f );

            do
            {
                block.setPosition( Random<int>(1, columns - 1) , Random<int>(1, rows - 1) );
            } while( collision() );

            block.setFillColor( next_block.getFillColor() );
            next_block.setFillColor( getRandomColor() );
        }

        sf::Color getRandomColor()
        {
            int c = Random<int>(0, sizeof(colors) / sizeof(sf::Color));
            return colors[c];
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
    game.run();

    return 0;
}