#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include<SFML/audio.hpp>
#include <iostream>
#include <ctime>
#include <string>
#include "Windows.h"

struct stDate {
    short year;
    short month;
    short day;
    short hour;
    short minute;
    short second;
};

stDate GetSystemDateTime(void)
{
    time_t  t_now = time(NULL); // curent time

    // tm* now = localtime(&t); // unsafe (using static memory)

    struct tm now;  // a buffer to store the curent time
    localtime_s(&now, &t_now);

    stDate date;

    date.day = now.tm_mday;
    date.month = now.tm_mon + 1;
    date.year = now.tm_year + 1900;
    date.hour = now.tm_hour;
    date.minute = now.tm_min;
    date.second = now.tm_sec;

    return date;
}

std::string GetDateTimeToString(stDate const& date, std::string sep = ":")
{
    std::string dateTime = "";

    dateTime = std::string(date.hour < 10 ? "0" : "") + std::to_string(date.hour) + sep +
        std::string(date.minute < 10 ? "0" : "") + std::to_string(date.minute) + sep + 
        std::string(date.second < 10 ? "0" : "") + std::to_string(date.second);

    return dateTime;
}

void UpdateStates(stDate const &systemTime , sf::Sprite &sprHour, sf::Sprite &sprMinute, sf::Sprite &sprSecond, sf::Text &digitalClockString , sf::Vector2f const &center)
{
    //std::cout << "Time : " << systemTime.hour << ":" << systemTime.minute << ":" << systemTime.second << std::endl;
    
    short DegreeStep = 6; // 6*15min = 90degré
    float angMinute = 0, angHour = 0, angSecond = 0;
        
    angMinute = DegreeStep * systemTime.minute;
    angSecond = DegreeStep * systemTime.second;
    angHour = systemTime.hour <= 12 ? 30*systemTime.hour + (angMinute/360)*30 : 30 * (systemTime.hour - 12) + (angMinute/360) * 30;

    //std::cout << "angles : " << angHour << ":" << angMinute << ":" << angSecond<< std::endl;

    // set rotations
    sprHour.setRotation(angHour);
    sprMinute.setRotation(angMinute);
    sprSecond.setRotation(angSecond);

    // digital clock string
    digitalClockString.setString( GetDateTimeToString(systemTime , "-"));
    digitalClockString.setPosition(sf::Vector2f(center.x - digitalClockString.getGlobalBounds().width / 2, 20));
 
}

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode({ 680, 680 }), "Raouf's Analog Clock");
    window.setPosition(sf::Vector2i(0, 0));

    // pour la transparence----------------------------
    // Récupérer le handle de la fenêtre pour interagir avec l'API Windows
    // HWND hwnd = window.getSystemHandle();

    // Appliquer la transparence à la fenêtre via l'API Windows
    //SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
    //SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY); // Rendre le fond transparent
    //--------------------------------------------------

    //centre
    sf::Vector2f center(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

    //variables
    float scale = 0.5f;
    float clockScale = 0.6f;
    stDate systemTime;

    //Text & font
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/digital-7.ttf"))
    {
        std::cerr << "enable to load font file !";
        return -1;
    }
    sf::Text digitalText;
    digitalText.setFont(font);
    digitalText.setFillColor(sf::Color::White);
    digitalText.setOutlineColor(sf::Color::Black);
    digitalText.setCharacterSize(36);
    digitalText.setOutlineThickness(1.5f);

    //Textures
    sf::Texture txtrMinute , txtrHour , txtrSecond, txtrClock , txtrRedCircle;
    if (!txtrMinute.loadFromFile("assets/images/minutes3.png") || 
        !txtrHour.loadFromFile("assets/images/hours2.png") ||
        !txtrSecond.loadFromFile("assets/images/secondes2.png") ||
        !txtrRedCircle.loadFromFile("assets/images/redCircle.png") ||
        !txtrClock.loadFromFile("assets/images/clock.png")   )
    {
        std::cerr << "enable to load image file !";
        return -1;
    }
    txtrMinute.setSmooth(true);
    txtrHour.setSmooth(true);
    txtrSecond.setSmooth(true); 
    txtrClock.setSmooth(true);
    txtrRedCircle.setSmooth(true);

    //sprites
    sf::Sprite sprMinute, sprHour, sprSecond, sprClock, sprRedCircle;
    sprMinute.setTexture(txtrMinute);
    sprHour.setTexture(txtrHour);
    sprSecond.setTexture(txtrSecond);
    sprClock.setTexture(txtrClock);
    sprRedCircle.setTexture(txtrRedCircle);

    sprMinute.setOrigin(sf::Vector2f(73, 453));
    sprHour.setOrigin(sf::Vector2f(89, 384));
    sprSecond.setOrigin(sf::Vector2f(98, 866));
    sprClock.setOrigin(sf::Vector2f(sprClock.getLocalBounds().width/2, sprClock.getLocalBounds().height/ 2));// clock1 (sf::Vector2f(447, 447));
    sprRedCircle.setOrigin(sf::Vector2f(100,100));

    sprMinute.setScale(sf::Vector2f(scale, scale));
    sprHour.setScale(sf::Vector2f(scale, scale));
    sprSecond.setScale(sf::Vector2f(scale/2, scale/2));
    sprClock.setScale(sf::Vector2f(clockScale, clockScale));  // (sf::Vector2f((8/3)*scale, (8/3)*scale));
    sprRedCircle.setScale(sf::Vector2f(0.17f, 0.17f));

    sprMinute.setPosition(center);
    sprHour.setPosition(center);
    sprSecond.setPosition(center);
    sprClock.setPosition(center);
    sprRedCircle.setPosition(center);

    //cercle blanc
    sf::CircleShape circle(sprClock.getLocalBounds().width / 2);
    circle.setFillColor(sf::Color::White);
    circle.setOrigin(sprClock.getLocalBounds().width / 2, sprClock.getLocalBounds().width / 2);
    circle.setScale(sf::Vector2f(scale+0.2f, scale + 0.2f));
    circle.setPosition(center);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            /*
            // min
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) 
            {
                angMinute += 6;
                sprMinute.setRotation(angMinute);
                std::cout << "Angle minute: " << angMinute << std::endl;
            }

            // sec
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) 
            {
                angSecond += 6;
                sprSecond.setRotation(angSecond);
                //std::cout << "angSecond: " << angSecond << std::endl;
            }

            // hour
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) 
            {
                angHour += 6;
                sprHour.setRotation(angHour);
                //std::cout << "Angle hour : " << angHour << std::endl;
            }
            */

            if (event.type == sf::Event::Closed)
                window.close();
        }


        //update states
        systemTime = GetSystemDateTime();
        UpdateStates(systemTime, sprHour , sprMinute, sprSecond , digitalText, center);

        // clear the window 
        window.clear(sf::Color::White);

        // draw everything here...
        window.draw(circle);
        window.draw(sprClock);
        window.draw(sprMinute);
        window.draw(sprHour);
        window.draw(sprSecond);
        window.draw(sprRedCircle);
        window.draw(digitalText);
        

        // end the current frame
        window.display();
    }

    return 0;
}
