//=============================================================
//          ������ ��������� ������� ���� � ����������              
//=============================================================

namespace ye{
	class Input{
		
		class InputAction;
		class MouseAction;
		
		// ������� �������� �������. � SFML ��������\
		���� ���������� � ���, ������������ �� ������ ��� ���.\
		������ InputAction ��������� ���������� SFML\
		� �������� ���������� � ������� (onPressed), ������� (onReleased)\
		����������� (press) ������������ ������ ����������, ����������� � �������.\
		������ MouseAction ������ �� �� ����� � ������� ����.
		std::vector<InputAction> inputAction;
		std::vector<MouseAction> mouseAction;
		
		sf::RenderWindow *window;
		
	public:
		
		MouseAction *leftButton, *rightButton;
		
		// �����������
		Input(sf::RenderWindow &window_vvod){
			
			// �������� ������� � ������������� ���� (����� ��� ����)
			window = &window_vvod;
			
			// ������������� �������� ��� ������� ����
			mouseAction.push_back(MouseAction());
			mouseAction.push_back(MouseAction());
			leftButton = &mouseAction[0];
			rightButton = &mouseAction[1];
			leftButton->button = sf::Mouse::Left;
			rightButton->button = sf::Mouse::Right;
		}
		
		// ������� �������� ��������� ������� � ��������� ������� � inputAction � mouseAction
		void checkInputs(){
			
			/////////// �������� ������� ������ /////////////////
			for(int i = 0; i < inputAction.size(); i++){
				InputAction *a = &inputAction[i];
				
				// ����������� ����������� ���������
				a->lastKey = a->pressKey;
				
				// �������� ������ ���������
				a->pressKey = a->onPressed = a->onReleased = false;
				if(sf::Keyboard::isKeyPressed(a->key)) a->pressKey = true;
				if(a->lastKey == false && a->pressKey == true) a->onPressed = true;
				else if(a->lastKey == true && a->pressKey == false) inputAction[i].onReleased = true;	
			}
			
			/////////// �������� ������� ���� ///////////
			for(int i = 0; i < mouseAction.size(); i++){
				MouseAction *m = &mouseAction[i];
				
				// ����������� ����������� ���������
				m->lastButton = m->pressButton;
				
				// �������� ������ ���������
				m->pressButton = m->onPressed = m->onReleased = false;
				if(sf::Mouse::isButtonPressed(m->button)) m->pressButton = true;
				if(m->lastButton == false && m->pressButton == true) m->onPressed = true;
				else if(m->lastButton == true && m->pressButton == false) mouseAction[i].onReleased = true;	
			}
		}
		
		// ������� ���������� ������� �������
		void addAction(sf::Keyboard::Key key){
			int i = inputAction.size();
			inputAction.push_back(InputAction());
			inputAction[i].key = key;
		}
		
		// ������� �������� ��������� �� ������ �������
		InputAction *getAction(int i){
			return &inputAction[i];
		}
		
		// ������� ������ ������� ���� (����� ��� ������ � ����� SFML, ������ ��� ����� ������������� ������������)
		sf::Vector2f getMousePosition(){
			sf::Vector2i mPosi = sf::Mouse::getPosition(*window);
			sf::Vector2f mPosf(mPosi.x,mPosi.y);
			return mPosf;
		}
	
	private:
		
		// ����� ������� ������� ������� �������
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
		
		// ����� ������� ������� ������� ����
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
