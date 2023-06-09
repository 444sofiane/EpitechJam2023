/*
** EPITECH PROJECT, 2023
** Jam
** File description:
** SettingsScene
*/

#ifndef SETTINGSSCENE_HPP_
    #define SETTINGSSCENE_HPP_

    #include "IScene.hpp"
    #include "Widget.hpp"
    #include "Curtains.hpp"

    namespace jam {

    class SettingsScene : public IScene {
        public:
            SettingsScene();
            virtual ~SettingsScene() override;
            virtual void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
            virtual void update(float dt) override;
            virtual void render(sf::RenderTarget& target) override;
            virtual void restart() override;
            virtual void stop() override;
        protected:
            std::unordered_map<std::string, std::shared_ptr<ui::Widget>> m_uiElements;
            sf::RectangleShape m_background;
            sf::RectangleShape m_poster;
            trans::Curtains m_transition;
    };

    }

#endif /* !SETTINGSSCENE_HPP_ */
