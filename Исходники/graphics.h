//=============================================================
//                    Графический модуль                
//=============================================================

namespace ye{
	
	// Объявление шейдерных переменных
	sf::Shader yShaderCirc;
	sf::Shader yShaderRect;
	
	// Объявление вспомогательных текстурных и спрайтовых переменных
	sf::Texture yTexture;
	sf::Sprite ySprite;
	
	// Функция загрузки шейдеров теней
	void loadEngineShaders(){
		yTexture.loadFromFile("Engine/Textures/system.png");
		ySprite.setTexture(yTexture);	  
		yShaderRect.loadFromFile("Engine/Shaders/ShadowRect.glsl", sf::Shader::Fragment);
		yShaderCirc.loadFromFile("Engine/Shaders/ShadowCirc.glsl", sf::Shader::Fragment);
	}
	
	// Функция перерисовки окна
	void redraw (sf::RenderWindow &window, World world){
		window.clear(world.background);
		
		/////////// Прорисовка компонентов мира ///////////
		for (int i = 0; i < world.getAllScenes().size(); i++){
			double x = world.getScene(i) -> coor.x;
			double y = world.getScene(i) -> coor.y;
			double r = world.getScene(i) -> getRadius();
			double w = world.getScene(i) -> getSize().x;
			double h = world.getScene(i) -> getSize().y;

			// Компонент мира круглый
			if(world.getScene(i) -> isCircle){
				sf::CircleShape circle;
				circle.setFillColor(world.getScene(i) -> background);
				circle.setPosition(x - r,y - r);
				circle.setRadius(r);
				circle.setTexture(world.getScene(i)->texture);
				
				// Рисуем круглую тень, если есть
				if(world.getScene(i)->haveShadow){
					ySprite.setPosition(world.getScene(i) -> coor - sf::Vector2f(r/2,r/2) + world.getScene(i) -> shadowOffset);
					ySprite.setTextureRect(sf::IntRect(0, 0, 2*r, 2*r));
					yShaderCirc.setParameter("r",r);
					yShaderCirc.setParameter("coor",world.getScene(i) -> coor + world.getScene(i) -> shadowOffset);
					yShaderCirc.setParameter("color",world.getScene(i) -> shadowColor);
					yShaderCirc.setParameter("blur",world.getScene(i) -> shadowBlur);
					window.draw(ySprite, &yShaderCirc);
				}
				
				window.draw(circle);
			}
			
			// Компонент мира прямоугольный
			else{
				sf::RectangleShape rect;
				rect.setFillColor(world.getScene(i) -> background);
				rect.setPosition(x - w/2,y - h/2);
				rect.setSize({w,h});
				rect.setTexture(world.getScene(i)->texture);
				
				// Рисуем прямоугольную тень, если есть
				if(world.getScene(i)->haveShadow){
					ySprite.setPosition(world.getScene(i) -> coor - world.getScene(i) -> getSize()/2 + world.getScene(i) -> shadowOffset);
					ySprite.setTextureRect(sf::IntRect(0, 0, w, h));
					yShaderRect.setParameter("size",{w,h});
					yShaderRect.setParameter("coor",world.getScene(i) -> coor + world.getScene(i) -> shadowOffset);
					yShaderRect.setParameter("color",world.getScene(i) -> shadowColor);
					yShaderRect.setParameter("blur",world.getScene(i) -> shadowBlur);
					window.draw(ySprite, &yShaderRect);
				}
				
				window.draw(rect);
			}
		};
		
		/////////// Прорисовка акторов ///////////
		for (int i = 0; i < world.getAllActors().size(); i++){
			double x = world.getActor(i) -> coor.x;
			double y = world.getActor(i) -> coor.y;
			double r = world.getActor(i) -> getRadius();
			double w = world.getActor(i) -> getSize().x;
			double h = world.getActor(i) -> getSize().y;
			
			// Актор круглый
			if(world.getActor(i) -> isCircle){
				sf::CircleShape circle;
				circle.setFillColor(world.getActor(i) -> background);
				circle.setPosition(x - r,y - r);
				circle.setRadius(r);
				circle.setTexture(world.getActor(i)->texture);
				
				// Рисуем круглую тень, если есть
				if(world.getActor(i)->haveShadow){
					ySprite.setPosition(world.getActor(i) -> coor - sf::Vector2f(r/2,r/2) + world.getActor(i) -> shadowOffset); 
					ySprite.setTextureRect(sf::IntRect(0, 0, 2*r, 2*r));
					yShaderCirc.setParameter("r",r);
					yShaderCirc.setParameter("coor",world.getActor(i) -> coor + world.getActor(i) -> shadowOffset);
					yShaderCirc.setParameter("color",world.getActor(i) -> shadowColor);
					yShaderCirc.setParameter("blur",world.getActor(i) -> shadowBlur);
					window.draw(ySprite, &yShaderCirc);
				}
				
				window.draw(circle);
			}
			
			// Актор прямоугольный
			else{
				sf::RectangleShape rect;
				rect.setFillColor(world.getActor(i) -> background);
				rect.setPosition(x - w/2,y - h/2);
				rect.setSize({w,h});
				rect.setTexture(world.getActor(i)->texture);
				
				// Рисуем прямоугольную тень, если есть
				if(world.getActor(i)->haveShadow){
					ySprite.setPosition(world.getActor(i) -> coor - world.getActor(i) -> getSize()/2 + world.getActor(i) -> shadowOffset);
					ySprite.setTextureRect(sf::IntRect(0, 0, w, h));
					yShaderRect.setParameter("size",{w,h});
					yShaderRect.setParameter("coor",world.getActor(i) -> coor + world.getActor(i) -> shadowOffset);
					yShaderRect.setParameter("color",world.getActor(i) -> shadowColor);
					yShaderRect.setParameter("blur",world.getActor(i) -> shadowBlur);
					window.draw(ySprite, &yShaderRect);
				}
				
				window.draw(rect);
			}
		};
		
		/////////// Прорисовка виджетов ///////////
		for (int i = 0; i < world.getAllWidgets().size(); i++){
			double x = world.getWidget(i) -> coor.x;
			double y = world.getWidget(i) -> coor.y;
			double r = world.getWidget(i) -> getRadius();
			double w = world.getWidget(i) -> getSize().x;
			double h = world.getWidget(i) -> getSize().y;
			
			// Виджет круглый
			if(world.getWidget(i) -> isCircle){
				sf::CircleShape circle;
				circle.setFillColor(world.getWidget(i) -> background);
				circle.setPosition(x - r,y - r);
				circle.setRadius(r);
				circle.setTexture(world.getWidget(i)->texture);
				
				// Рисуем круглую тень, если есть
				if(world.getWidget(i)->haveShadow){
					ySprite.setPosition(world.getWidget(i) -> coor - sf::Vector2f(r/2,r/2) + world.getWidget(i) -> shadowOffset);
					ySprite.setTextureRect(sf::IntRect(0, 0, 2*r, 2*r));
					yShaderCirc.setParameter("r",r);
					yShaderCirc.setParameter("coor",world.getWidget(i) -> coor + world.getWidget(i) -> shadowOffset);
					yShaderCirc.setParameter("color",world.getWidget(i) -> shadowColor);
					yShaderCirc.setParameter("blur",world.getWidget(i) -> shadowBlur);
					window.draw(ySprite, &yShaderCirc);
				}
				
				window.draw(circle);
			}
			
			// Виджет прямоугольный
			else{
				sf::RectangleShape rect;
				rect.setFillColor(world.getWidget(i) -> background);
				rect.setPosition(x - w/2,y - h/2);
				rect.setSize({w,h});
				rect.setTexture(world.getWidget(i)->texture);
				
				// Рисуем прямоугольную тень, если есть
				if(world.getWidget(i)->haveShadow){
					ySprite.setPosition(world.getWidget(i) -> coor - world.getWidget(i) -> getSize()/2 + world.getWidget(i) -> shadowOffset);
					ySprite.setTextureRect(sf::IntRect(0, 0, w, h));
					yShaderRect.setParameter("size",{w,h});
					yShaderRect.setParameter("coor",world.getWidget(i) -> coor + world.getWidget(i) -> shadowOffset);
					yShaderRect.setParameter("color",world.getWidget(i) -> shadowColor);
					yShaderRect.setParameter("blur",world.getWidget(i) -> shadowBlur);
					window.draw(ySprite, &yShaderRect);
				}
				
				window.draw(rect);
			}
			
			// Вывод текста на экран по центру виджета
			int fontSize = world.getWidget(i) -> fontSize;
			sf::Text text("",font[0],fontSize);
			text.setColor(world.getWidget(i) -> textColor);
			text.setString(world.getWidget(i) -> text);
			text.setPosition(x - text.getLocalBounds().width/2, y - text.getLocalBounds().height*0.75);
			window.draw(text);
		};
		
		window.display();
	}
}

