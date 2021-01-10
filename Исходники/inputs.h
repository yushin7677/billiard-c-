//=============================================================
//          Модуль обработки событий мыши и клавиатуры              
//=============================================================

namespace ye{
	class Input{
		
		class InputAction;
		class MouseAction;
		
		// Массивы объектов событий. в SFML доступна\
		лишь информация о том, удерживается ли кнопка или нет.\
		Объект InputAction расширяет функционал SFML\
		и содержит информацию о нажатии (onPressed), отжатии (onReleased)\
		удерживании (press) определенной кнопки клавиатуры, привязанной к объекту.\
		Объект MouseAction делает то же самое с кнопкой мыши.
		std::vector<InputAction> inputAction;
		std::vector<MouseAction> mouseAction;
		
		sf::RenderWindow *window;
		
	public:
		
		MouseAction *leftButton, *rightButton;
		
		// Конструктор
		Input(sf::RenderWindow &window_vvod){
			
			// Привязка событий к определенному окну (важно для мыши)
			window = &window_vvod;
			
			// Инициализация объектов для событий мыши
			mouseAction.push_back(MouseAction());
			mouseAction.push_back(MouseAction());
			leftButton = &mouseAction[0];
			rightButton = &mouseAction[1];
			leftButton->button = sf::Mouse::Left;
			rightButton->button = sf::Mouse::Right;
		}
		
		// Функция проверки введенных событий и изменения свойств в inputAction и mouseAction
		void checkInputs(){
			
			/////////// Проверка нажатия клавиш /////////////////
			for(int i = 0; i < inputAction.size(); i++){
				InputAction *a = &inputAction[i];
				
				// Запоминание предыдущего состояния
				a->lastKey = a->pressKey;
				
				// Проверка нового состояния
				a->pressKey = a->onPressed = a->onReleased = false;
				if(sf::Keyboard::isKeyPressed(a->key)) a->pressKey = true;
				if(a->lastKey == false && a->pressKey == true) a->onPressed = true;
				else if(a->lastKey == true && a->pressKey == false) inputAction[i].onReleased = true;	
			}
			
			/////////// Проверка нажатия мыши ///////////
			for(int i = 0; i < mouseAction.size(); i++){
				MouseAction *m = &mouseAction[i];
				
				// Запоминание предыдущего состояния
				m->lastButton = m->pressButton;
				
				// Проверка нового состояния
				m->pressButton = m->onPressed = m->onReleased = false;
				if(sf::Mouse::isButtonPressed(m->button)) m->pressButton = true;
				if(m->lastButton == false && m->pressButton == true) m->onPressed = true;
				else if(m->lastButton == true && m->pressButton == false) mouseAction[i].onReleased = true;	
			}
		}
		
		// Функция добавления объекта событий
		void addAction(sf::Keyboard::Key key){
			int i = inputAction.size();
			inputAction.push_back(InputAction());
			inputAction[i].key = key;
		}
		
		// Функция возврата указателя на объект событий
		InputAction *getAction(int i){
			return &inputAction[i];
		}
		
		// Функция взятия позиции мыши (можно это делать и через SFML, однако это может противоречить инкапсуляции)
		sf::Vector2f getMousePosition(){
			sf::Vector2i mPosi = sf::Mouse::getPosition(*window);
			sf::Vector2f mPosf(mPosi.x,mPosi.y);
			return mPosf;
		}
	
	private:
		
		// Класс объекта событий нажатия клавиши
		class InputAction{
			sf::Keyboard::Key key;
			bool pressKey = false;
			bool lastKey = false;
			bool onPressed = false;
			bool onReleased = false;
			friend Input;
			
		public:
			bool getPress(){
				return pressKey;
			}
			
			bool getOnPressed(){
				return onPressed;
			}
			
			bool getOnReleased(){
				return onReleased;
			}
		};
		
		// Класс объекта событий нажатия мыши
		class MouseAction{
			sf::Mouse::Button button;
			bool pressButton = false;
			bool lastButton = false;
			bool onPressed = false;
			bool onReleased = false;
			friend Input;
			
		public:
			bool getPress(){
				return pressButton;
			}
			
			bool getOnPressed(){
				return onPressed;
			}
			
			bool getOnReleased(){
				return onReleased;
			}
		};
	};
}
