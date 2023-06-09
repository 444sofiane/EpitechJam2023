/*
** EPITECH PROJECT, 2023
** Jam
** File description:
** Slider
*/

#include "Slider.hpp"
#include "ResourceManager.hpp"
#include "info.hpp"
#include "math.hpp"

namespace jam {

namespace ui {

Slider::Slider(const sf::Texture& wagonTexture, const sf::Texture& railTexture, Orientation orientation)
: m_animation(wagonTexture.getSize().x / 3, wagonTexture.getSize().y), m_orientation(orientation)
{
    m_wagon.setTexture(&wagonTexture);
    m_outerRail.setTexture(&railTexture);


    m_animation.addFrame(0, sf::seconds(0.5));
    m_animation.addFrame(1, sf::seconds(0.5));
    m_animation.addFrame(2, sf::seconds(0.5));
    m_wagon.setTextureRect(m_animation.getFrame(0));

    m_wagon.setSize(sf::Vector2f(wagonTexture.getSize().x / 3, wagonTexture.getSize().y));
    m_wagon.setOrigin(m_wagon.getSize() * 0.5f);

    m_outerRail.setSize(sf::Vector2f(railTexture.getSize()));
    m_outerRail.setOrigin(m_outerRail.getSize() * 0.5f);
    m_wagon.setPosition(m_outerRail.getPosition());

    if (m_orientation == VERTICAL) {
        m_wagon.setRotation(90);
        m_outerRail.setRotation(90);
    }

    m_internRail = m_outerRail;
    m_internRail.setTexture(nullptr);
    m_internRail.setFillColor(sf::Color::Transparent);
    m_internRail.setOutlineColor(sf::Color::Red);
    m_internRail.setOutlineThickness(1);
    m_internRail.setSize({m_internRail.getSize().x - 32, m_internRail.getSize().y});
    m_internRail.setOrigin(m_internRail.getSize() * 0.5f);

    fixWagonPosition();
}

void Slider::setScale(const sf::Vector2f& scale)
{
    m_wagon.setBaseScale(scale);
    m_outerRail.setBaseScale(scale);
    m_internRail.setBaseScale(scale);
}

void Slider::setScale(float x, float y)
{
    m_wagon.setBaseScale(x, y);
    m_outerRail.setBaseScale(x, y);
    m_internRail.setBaseScale(x, y);
}

float Slider::getProgress() const
{
    return m_progress;
}

void Slider::handleEvent(sf::Event e, const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = info::getMousePosition();
    sf::FloatRect railBound = m_internRail.getGlobalBounds();
    sf::FloatRect detectionBound = railBound;
    sf::Vector2f diff;
    bool mouseClicked = false;

    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
        mouseClicked = true;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        m_mouseDown = true;
    if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
        m_mouseDown = false;

    if (m_orientation == HORIZONTAL)
        detectionBound = math::multRect(railBound, {1.f, 10.f});
    else if (m_orientation == VERTICAL)
        detectionBound = math::multRect(railBound, {3.f, 1.f});

    if (!m_wagon.isHovered(window) && m_state != GRABBED) {
        m_state = IDLE;
        m_wagon.setScale(m_wagon.getBaseScale());
        return;
    }
    if (m_wagon.isHovered(window) && (!m_mouseDown || mouseClicked)) {
        m_wagon.setScale(m_wagon.getBaseScale() + sf::Vector2f(0.1f, 0.1f));
        if (mouseClicked && m_mouseDown && m_state != GRABBED) {
            m_lastPosition = mousePos;
            m_state = GRABBED;
        } else if (!mouseClicked && !m_mouseDown)
            m_state = HOVERED;
    } else if (!mouseClicked && !m_mouseDown) {
        m_wagon.setScale(m_wagon.getBaseScale());
        m_state = HOVERED;
    }
    if (m_state == GRABBED) {
        m_wagon.setScale(m_wagon.getBaseScale() - sf::Vector2f(0.1f, 0.1f));
        diff = mousePos - m_lastPosition;
        sf::FloatRect wagonBound = m_wagon.getGlobalBounds();
        if (detectionBound.contains(mousePos)) {
            if (isOnTrack() && m_orientation == HORIZONTAL) {
                m_wagon.move(diff.x, 0.f);
                m_progress = math::dist(sf::Vector2f(railBound.left,
                    m_internRail.getPosition().y), m_wagon.getPosition()) / railBound.width;
            }
            else if (isOnTrack() && m_orientation == VERTICAL) {
                m_wagon.move(0.f, diff.y);
                m_progress = math::dist(sf::Vector2f(m_internRail.getPosition().x,
                    railBound.top), m_wagon.getPosition()) / railBound.height;
            }
        } else {
            m_state = IDLE;
            m_wagon.setScale(m_wagon.getBaseScale());
        }
        fixWagonPosition();
        m_lastPosition = mousePos;
    }
}

void Slider::fixWagonPosition()
{
    sf::FloatRect wagonBound = m_wagon.getGlobalBounds();
    sf::FloatRect railBound = m_internRail.getGlobalBounds();

    if (m_orientation == HORIZONTAL && wagonBound.left <= railBound.left) {
        m_wagon.setPosition(railBound.left + (wagonBound.width * 0.5) + 1, m_wagon.getPosition().y);
        return;
    }
    if (m_orientation == HORIZONTAL && wagonBound.left + wagonBound.width >= railBound.left + railBound.width) {
        m_wagon.setPosition(railBound.left + railBound.width - (wagonBound.width * 0.5) - 1, m_wagon.getPosition().y);
        return;
    }
    if (m_orientation == VERTICAL && wagonBound.top <= railBound.top) {
        m_wagon.setPosition(m_wagon.getPosition().x, railBound.top + (wagonBound.height * 0.5) + 1);
        return;
    }
    if (m_orientation == VERTICAL && wagonBound.top + wagonBound.height >= railBound.top + railBound.height) {
        m_wagon.setPosition(m_wagon.getPosition().x, railBound.top + railBound.height - (wagonBound.height * 0.5) - 1);
        return;
    }

}

bool Slider::isOnTrack() const
{
    sf::FloatRect wagonBound = m_wagon.getGlobalBounds();
    sf::FloatRect railBound = m_internRail.getGlobalBounds();

    if (m_orientation == HORIZONTAL && wagonBound.left > railBound.left
        && wagonBound.left + wagonBound.width < railBound.left + railBound.width)
        return true;
    if (m_orientation == VERTICAL && wagonBound.top > railBound.top
        && wagonBound.top + wagonBound.height < railBound.top + railBound.height)
        return true;
    return false;
}

void Slider::setPosition(const sf::Vector2f& pos)
{
    m_outerRail.setPosition(pos);
    m_internRail.setPosition(pos);

    if (m_orientation == HORIZONTAL)
        m_wagon.setPosition(m_internRail.getGlobalBounds().left + (m_internRail.getGlobalBounds().width * m_progress), pos.y);
    else if (m_orientation == VERTICAL)
        m_wagon.setPosition(pos.x, m_internRail.getGlobalBounds().top + (m_internRail.getGlobalBounds().height * m_progress));

    fixWagonPosition();
}

sf::Vector2f Slider::getSize() const
{
    return sf::Vector2f(m_outerRail.getGlobalBounds().width, m_outerRail.getGlobalBounds().height);
}

void Slider::render(sf::RenderTarget& target)
{
    target.draw(m_outerRail);
    target.draw(m_wagon);
    //target.draw(m_internRail);
}

}
}