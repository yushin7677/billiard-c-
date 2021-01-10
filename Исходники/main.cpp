#include "yEngine.h"
#include<locale>

using namespace std;
using namespace sf;
using namespace ye;

void begin0(ye::World &world);
void begin1(ye::World &world);
void begin2(ye::World &world);
void begin3(ye::World &world);

// Запуск программы
int main() {

	// Системные настройки
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	
	//
	int NumberWorld = 0;
	
	// Загрузка шрифтов
	font.push_back(Font());
	font.push_back(Font());
	font[0].loadFromFile("Fonts/font.woff");
	font[1].loadFromFile("Fonts/roboto.woff");
	
	// Загрузка текстур
	textures.push_back(Texture());
	textures[0].loadFromFile("Engine/Textures/system.png");
	textures.push_back(Texture());
	textures[1].loadFromFile("Textures/billiardTable.png");
	textures.push_back(Texture());
	textures[2].loadFromFile("Textures/gamePanel.png");
	textures.push_back(Texture());
	textures[3].loadFromFile("Textures/ball.png");
	
	// Загрузка звуков
	vector<SoundBuffer> buffer(8);
	
	buffer[0].loadFromFile("Sounds/beginImpact.ogg");
	buffer[1].loadFromFile("Sounds/click.ogg");
	buffer[2].loadFromFile("Sounds/fonMenu.ogg");
	buffer[3].loadFromFile("Sounds/impactBalls.ogg");
	buffer[4].loadFromFile("Sounds/impactBoard.ogg");
	buffer[5].loadFromFile("Sounds/inPocket.ogg");
	buffer[6].loadFromFile("Sounds/mistake.ogg");
	buffer[7].loadFromFile("Sounds/putBall.ogg");
	
	Sound soundBeginImpact(buffer[0]);
	Sound soundClick(buffer[1]);
	Sound soundFonMenu(buffer[2]);
	Sound soundImpactBalls(buffer[3]);
	Sound soundImpactBoard(buffer[4]);
	Sound soundInPocket(buffer[5]);
	Sound soundMistake(buffer[6]);
	Sound soundPutBall(buffer[7]);
	
	// Генерация миров
	vector<World> world;
	world.push_back(World());
	world.push_back(World());
	world.push_back(World());
	world.push_back(World());
	world[0].frame = world[1].frame = 60;
	
	// Создание объектов (мир 0) - основное меню
	begin0(world[0]);
	
	// Создание объектов (мир 1) - игра
	begin1(world[1]);

	// Создание объектов (мир 2) - меню инструкции
	begin2(world[2]);
	
	// Создание объектов (мир 3) - меню настроек
	begin3(world[3]);

	//======================================
	//         Настройки графики
	//======================================
	loadEngineShaders(); // Загрузка шейдеров теней
	ContextSettings settings;
    settings.antialiasingLevel = 8;

	// Открытие окна
	RenderWindow window( VideoMode(800,600), "Русский бильярд", sf::Style::Close, settings);
	window.setFramerateLimit(60);
	
	// Задание инпутов
	Input input(window);
	
	// Всякие переменные
	int player = 1;              // номер игрока, которому принадлежит ход
	int idChoose;                // id выбранного шара для удара
	int score[2] = {0,0};        // очки игроков
	int hitBall = 0;             // количество ударов шарами в данном ходу
	int hitBoard = 0;            // количество шаров о борт в данном ходу
	int frameTryes = 0;          // количество кадров, которое может использует компьютер для выбора удара
	int dCoor = 100;             // Искуственная ошибка прицеливания компьютером
	bool canChoose = true;       // можно ли выбрать шар?
	bool canHit = true;          // можно ли ходить?
	bool isScore = false;        // набраны ли очки текущим игроком в данном ходе?
	bool isComputer = true;      // 2-й игрок - компьютер?
	bool demo = false;      	 // оба игрока - компьютеры?
	bool ballReturn = false;     // Нужно ли возвращать шар?
	
	vector<int> idHoles(6);      // id компонентов мира, являющихся лунками
	for(int i = 0; i < 6; i++){
		idHoles[i] = world[1].getScene(12 + i)->getId(); // поиск этих id и их запись в массив
	};
	
	vector<Vector2f> coorHoles(6);    // координаты наилучших точек, куда следует бить шары
    coorHoles[0] = {25,25};
    coorHoles[1] = {400,29}; 
    coorHoles[2] = {775,25}; 
    coorHoles[3] = {25,375};
    coorHoles[4] = {400,371}; 
    coorHoles[5] = {775,375};
	
	// Проигрывание стартового звука
	soundFonMenu.play();
	
	//======================================
	//         Основная функция
	//======================================
	while(window.isOpen()){
		World *thisWorld = &world[NumberWorld];
		Event event;
		
		// Обработчик событий закрытия окна
		while(window.pollEvent(event)){
	    	if(event.type == Event::Closed){
	    		window.close();
			}
		}
		
		// Модуль ввода
		input.checkInputs();
		
		// Модуль команд Update
		switch (NumberWorld){
			case 0:
			{
				// Обработчик кнопки входа в игру
				if(thisWorld->getWidget(0)->getOnHover()) thisWorld->getWidget(0)->background = Color(255,192,0);
				else thisWorld->getWidget(0)->background = Color(224,168,0);
				if(thisWorld->getWidget(0)->getOnPressed()){
					NumberWorld = 1;
					soundClick.play();
					soundFonMenu.stop();
				}
				
				// Обработчик кнопки входа в инструкцию
				if(thisWorld->getWidget(1)->getOnHover()) thisWorld->getWidget(1)->background = Color(255,192,0);
				else thisWorld->getWidget(1)->background = Color(224,168,0);
				if(thisWorld->getWidget(1)->getOnPressed()){
					NumberWorld = 2;
					soundClick.play();
				}
				
				// Обработчик кнопки входа в настройки
				if(thisWorld->getWidget(2)->getOnHover()) thisWorld->getWidget(2)->background = Color(255,192,0);
				else thisWorld->getWidget(2)->background = Color(224,168,0);
				if(thisWorld->getWidget(2)->getOnPressed()){
					NumberWorld = 3;
					soundClick.play();
				}
				
				// Обработчик кнопки выхода из игры
				if(thisWorld->getWidget(3)->getOnHover()) thisWorld->getWidget(3)->background = Color(255,192,0);
				else thisWorld->getWidget(3)->background = Color(224,168,0);
				if(thisWorld->getWidget(3)->getOnPressed()) window.close();
				
				break;
			}
			case 1:
			{
				// Обработчик кнопки выхода в меню
				if(thisWorld->getWidget(1)->getOnHover()) thisWorld->getWidget(1)->background = Color(255,192,0);
				else thisWorld->getWidget(1)->background = Color(224,168,0);
				if(thisWorld->getWidget(1)->getOnPressed()){
					thisWorld->clearWorld();
					NumberWorld = 0;
					
					// Возвращение объектов и переменных к начальному состоянию
					begin1(*thisWorld);
					player = 1, score[0] = score[1] = 0, hitBall = 0, hitBoard = 0;
					canChoose = true, canHit = true, isScore = false, ballReturn = false;	
					for(int i = 0; i < 6; i++){
						idHoles[i] = world[1].getScene(12 + i)->getId();
					};
					
					//
					soundClick.play();
					soundFonMenu.play();
				}
				
				
				//////////// Игровой процесс /////////////////
				
				// Можно бить
				if(canHit){
					
					// Бьет человек
					if((player == 1 || !isComputer) && !demo){
						
						// Человек выбирает шар
						if(canChoose){
							for (int i = 0; i < thisWorld->getAllActors().size() && canChoose; i++){
								if(thisWorld->getActor(i)->getOnHover()) thisWorld->getActor(i)->background = Color(192,192,192);
								else thisWorld->getActor(i)->background = Color(255,255,255);
								
								// Проверяем нажатие левой кнопки мыши. Есла нажата - выбираем данный шар
								if(thisWorld->getActor(i)->getOnPressed()){
									idChoose = thisWorld->getActor(i)->getId();
									canChoose = false;
									
									// Включаем указатели удара
									for (int j = 5; j <= 9; j++){
										thisWorld->getWidget(j) -> background = Color (255,0,0,255);
									}
								}
							}
						}
						
						// Человек выбрал шар и выбирает силу и направление удара
						else{
							
							// Вычисляем направление и силу удара и записываем его в вектор v
							Vector2f coorBall = thisWorld->getActorById(idChoose)->coor;
							Vector2f coorMouse = input.getMousePosition();
							Vector2f v = coorBall - coorMouse;
							if(vectorLength(v) > 200) v = normalized(v)*200;
							
							// Показываем направление удара
							for (int j = 5; j <= 9; j++){
								thisWorld->getWidget(j) -> coor = coorBall + (j - 4)*v/5;
							}
							
							// Проверяем нажатие правой кнопки мыши. Есла нажата - отменяем выбор шара
							if(input.rightButton->getOnReleased()){
								canChoose = true;
								
								// Отключаем указатели удара
								for (int j = 5; j <= 9; j++){
									thisWorld->getWidget(j) -> background = Color (255,255,0,0);
								}
							}
							
							// Проверяем отжатие левой кнопки мыши. Есла нажата - ударяем
							else if(input.leftButton->getOnReleased()){
								canChoose = true;
								canHit = false;
								thisWorld->getWidget(4)->background = Color(255,0,0);
								thisWorld->getActorById(idChoose)->background = Color(255,255,255);
								thisWorld->getActorById(idChoose)->v = 2*v;
								
								// Отключаем указатели удара
								for (int j = 5; j <= 9; j++){
									thisWorld->getWidget(j) -> background = Color (255,255,0,0);
								}
							}
						}
					}
					
					// Бьет компьютер
					else{
						
						int howManyBalls = thisWorld->getAllActors().size();
						
						// Компьютер выбирает шары, которым будет бить и который попытается забить
						int iRand1 = rand()%howManyBalls; // биток
						int iRand2 = rand()%howManyBalls;
						
						// Если это один и тот же шар, то повторяет выбор
						while(iRand1 == iRand2){
							iRand1 = rand()%howManyBalls;
							iRand2 = rand()%howManyBalls;
						}
						
						// Берет координаты шаров и лунки
						Vector2f coor10 = thisWorld -> getActor(iRand1) -> coor;
						Vector2f coor20 = thisWorld -> getActor(iRand2) -> coor;
						Vector2f coorHole = coorHoles[rand()%6];// Выбирает координаты для лунки, в который будет забивать
						
						// Высчитываем координату соударения 1-го шара
						Vector2f coor1 = coor20 - 20*normalized(coorHole - coor20);
						
						// Высчитывает направление и скорость удара
						Vector2f v = (100 + rand()%100) * normalized(coor1 - coor10);
						
						bool goodAngle = false, otherBalls = true;
						int tryes = 0;
						
						while(tryes < 20 && !(goodAngle && !otherBalls)){
							
							otherBalls = false;
							
							// Удобный ли угол удара?
							goodAngle = v*(coorHole - coor20)/(vectorLength(v)*vectorLength(coorHole - coor20)) > 0.5;
							
							// Есть ли на пути битка другие шары?
							for (int i = 0; i < howManyBalls && !otherBalls; i++){
								Vector2f coorOther = thisWorld -> getActor(i) -> coor;
								
								double cx = coorOther.x - coor10.x;
								double cy = coorOther.y - coor10.y;
								double vx = coor10.x - coor1.x;
								double vy = coor10.y - coor1.y; 
								double part;
								double D = -(cy*vx - cx*vy)*(cy*vx - cx*vy) + 400*(vx*vx + vy*vy); // 400 = (2*10)**2
								if (D < 0) part = 100;
								else part = ( - (cx*vx + cy*vy) - sqrt(D))/(vx*vx + vy*vy);
								
								if (part > 0 && part < 1){
									otherBalls = true;
								}
							}
							
							// Есть ли на пути забиваемого шара другие шары?
							for (int i = 0; i < howManyBalls && !otherBalls; i++){
								Vector2f coorOther = thisWorld -> getActor(i) -> coor;
								
								double cx = coorOther.x - coor20.x;
								double cy = coorOther.y - coor20.y;
								double vx = coor20.x - coorHole.x;
								double vy = coor20.y - coorHole.y; 
								double part;
								double D = -(cy*vx - cx*vy)*(cy*vx - cx*vy) + 400*(vx*vx + vy*vy); // 400 = (2*10)**2
								if (D < 0) part = 100;
								else part = ( - (cx*vx + cy*vy) - sqrt(D))/(vx*vx + vy*vy);
								
								if (part > 0 && part < 1){
									otherBalls = true;
								}
							}
						
							tryes++;
						}
						
						frameTryes++;
						
						// Если угол хороший, а другим шарам не мешают другие шары, то компьютер бъет!
						if((goodAngle && !otherBalls) || frameTryes > 60){
							if(dCoor == 0) thisWorld->getActor(iRand1)->v = 2*v;
							else thisWorld->getActor(iRand1)->v = 2*v + Vector2f(-dCoor/2 + rand()%dCoor, -dCoor/2 + rand()%dCoor);
						
							canChoose = true;
							canHit = false;
							frameTryes = 0;
							thisWorld->getWidget(4)->background = Color(255,0,0);
						}
						
						// Иначе выбирает другие шары и лунку для удара, но уже в следующем кадре
					}
				}
				
				// Нужно выставлять шар
				else if(ballReturn){
					
					// Двигаем изображение шара вместе с курсором
					Vector2f coorMouse = input.getMousePosition();
					thisWorld->getWidget(10)-> coor = coorMouse;
					
					////////////// Есть ли пересечения с другими шарами и находится ли изображение в поле? /////////////////
					bool canPut = true;
					
					// Находится ли изображение в поле?
					if(coorMouse.x >= 44 && coorMouse.x <= 756 && coorMouse.y >= 44 && coorMouse.y <= 356) canPut = true;
					else canPut = false;
					
					// Есть ли пересечения с другими шарами?
					int i = 0;
					while(i < thisWorld->getAllActors().size() && canPut){
						if(vectorLength(thisWorld->getActor(i)->coor - coorMouse) < 20) canPut = false;
						i++;
					}
					
					// Если шар не может ставиться, красим в красный, в противном случае - в белый:
					if(canPut) thisWorld->getWidget(10)->background = Color(255,255,255);
					else thisWorld->getWidget(10)->background = Color(255,0,0);
					
					// Проверяем нажатие левой кнопки мыши
					if(input.leftButton->getOnReleased()){
						
						// Ставим, если все нормально
						if(canPut){
							thisWorld->spawnActor(coorMouse,{0,0});
							thisWorld->getActor(i)-> background = Color(255,255,255);
							thisWorld->getActor(i)->texture = &textures[3];
							thisWorld->getActor(i)->haveShadow = true;
							thisWorld->getActor(i)->shadowOffset = {2,2};
							thisWorld->getActor(i)->shadowBlur = 4;
							thisWorld->getWidget(10)-> background = Color(255,255,255,0);
							ballReturn = false;
							canHit = true;
							soundPutBall.play();
						}
						
						// Иначе проигрываем звук ошибки
						else{
							soundMistake.play();
						}
					}
					
				}
				
				// Шары катятся
				else{
					
					// Тормозим все шары
					int BallInMove = 0;
					for (int i = 0; i < thisWorld->getAllActors().size(); i++){
						
						// Проверяем, c чем столкнулся шар
						vector<World::EventHit> eventHit = thisWorld->getActor(i)->getEventHit();
						bool inHole = false;
						for(int j = 0; j < eventHit.size() && !inHole; j++){
							
							// Шар ударился с другим шаром
							if(eventHit[j].typeName == "Actor"){
								hitBall++;
								soundImpactBalls.play();
							}
							
							// Шар ударился с бортом
							else if(eventHit[j].typeName == "Scene"){
								hitBoard++;
								soundImpactBoard.play();
							}
							
							// Закатился ли шар в лунку?
							for(int k = 0; k < idHoles.size() && !inHole; k++){
								if(eventHit[j].id == idHoles[k]){
									inHole = true;
								}
								
							}
						}
						
						// Если шар в лунке, удаляем его
						if(inHole){
							thisWorld->deleteActor(i);
							score[player - 1]++;
							if(hitBall > 0){
								isScore = true;
								thisWorld->getWidget(player + 1)->text = to_string(score[player - 1]);
								BallInMove++;
							}
							soundInPocket.play();
						}
						
						// Иначе тормозим его
						else{
							Vector2f v = thisWorld->getActor(i)->v;
							Vector2f dv = normalized(v)*0.4;
							if(vectorLength(v) > vectorLength(dv)){
								thisWorld->getActor(i)->v = v - dv;
								BallInMove++;
							}
							else if(vectorLength(v) > 0)  thisWorld->getActor(i)->v = {0,0};
						}
					}
	
					// Шары остановились
					if(BallInMove == 0){
						
						// Если нарушены правила, пользователь или компьютер должен поставить шар
						if(hitBall == 0 && hitBoard < 3 && score[player - 1]){
							score[player - 1]--;
							thisWorld->getWidget(player + 1)->text = to_string(score[player - 1]);
							
							// считаем количество шаров (акторов)
							int i = thisWorld->getAllActors().size();
							
							// Если ударил компьютер (и компьютер не играет сам с собой) или играют два человека, то ставит пользователь:
							if(((player == 2 && isComputer) || !isComputer) && !demo){
								ballReturn = true;
								thisWorld->getWidget(10)-> background = Color(255,255,255,255);
								thisWorld->getWidget(10)-> texture = &textures[3];
							}
							
							// Если ударил пользователь (или компьютер играет сам с собой), то ставит компьютер:
							else if((player == 1 && isComputer) || demo){
								
								Vector2f coorComputer;
								bool canPut = false;
								
								while(!canPut){
									canPut = true;
									
									// Компьютер выбирает случайное место для того, чтоб поставить шар
									coorComputer = {44 + rand()%712, 44 + rand()%312};
					
									// Есть ли пересечения с другими шарами?
									int i = 0;
									while(i < thisWorld->getAllActors().size() && canPut){
										if(vectorLength(thisWorld->getActor(i)->coor - coorComputer) < 20) canPut = false;
										i++;
									}
								}
						
								// Ставим, если все нормально
								thisWorld->spawnActor(coorComputer,{0,0});
								thisWorld->getActor(i)-> background = Color(255,255,255);
								thisWorld->getActor(i)->texture = &textures[3];
								thisWorld->getActor(i)->haveShadow = true;
								thisWorld->getActor(i)->shadowOffset = {2,2};
								thisWorld->getActor(i)->shadowBlur = 4;
								
								canHit = true;
								soundPutBall.play();
							}
						}
						else canHit = true;
						
						// У одного из игроков 8 или более очков => победа
						if(score[player - 1] >= 8){
							thisWorld->getWidget(11)->background = Color(224,168,0);
							thisWorld->getWidget(11)->text = "Победил игрок " + to_string(player);
						}
						
						// Если нет, то игра продолжается
						else{
							if(!isScore) player = 3 - player;
							isScore = false;
							thisWorld->getWidget(4)->background = Color(0,255,0);
							thisWorld->getWidget(4)->text = to_string(player);
							hitBall = hitBoard = 0;
						}
					}
				}
				
				break;
			}
			case 2:
			{	
			
				// Обработчик кнопки выхода в меню
				if(thisWorld->getWidget(1)->getOnHover()) thisWorld->getWidget(1)->background = Color(255,192,0);
				else thisWorld->getWidget(1)->background = Color(224,168,0);
				if(thisWorld->getWidget(1)->getOnPressed()){
					NumberWorld = 0;
					soundClick.play();
				}
				
				break;
			}
			case 3:
			{	
				// Обработчик кнопки выбора соперника
				if(thisWorld->getWidget(1)->getOnHover()) thisWorld->getWidget(1)->background = Color(255,192,0);
				else thisWorld->getWidget(1)->background = Color(224,168,0);
				if(thisWorld->getWidget(1)->getOnPressed()){
					if(!isComputer){
						isComputer = true;
						thisWorld->getWidget(1)->text = "C компьютером";
					}
					else if(!demo){
						demo = true;
						thisWorld->getWidget(1)->text = "Демонстрация";
					}
					else{
						isComputer = demo = false;
						thisWorld->getWidget(1)->text = "C человеком";
					}

					soundClick.play();
				}
			
				// Обработчик кнопки выхода в меню
				if(thisWorld->getWidget(2)->getOnHover()) thisWorld->getWidget(2)->background = Color(255,192,0);
				else thisWorld->getWidget(2)->background = Color(224,168,0);
				if(thisWorld->getWidget(2)->getOnPressed()){
					NumberWorld = 0;
					soundClick.play();
				}
				
				// Обработчик кнопки выбора сложности
				if(thisWorld->getWidget(4)->getOnHover()) thisWorld->getWidget(4)->background = Color(255,192,0);
				else thisWorld->getWidget(4)->background = Color(224,168,0);
				if(thisWorld->getWidget(4)->getOnPressed()){
					switch(dCoor){
						case 100:
							dCoor = 10;
							thisWorld->getWidget(4)->text = "Средний";
							break;
						case 10:
							dCoor = 0;
							thisWorld->getWidget(4)->text = "Трудный";
							break;
						case 0:
							dCoor = 100;
							thisWorld->getWidget(4)->text = "Легкий";
							break;
					}
					soundClick.play();
				}
				
				break;
			}
		}
		
		// Физический модуль
		thisWorld->update(input);
		
		// Графический модуль
		redraw(window, *thisWorld);
	}
}

void begin0(ye::World &world){
	world.background = Color(0,128,0);	
	
	// Спавн кнопок
	world.createWidget({400,82.5}, {0,0});	
	world.getWidget(0)->isCircle = false;
	world.getWidget(0)->setSize({700,125});
	world.getWidget(0)->textColor = Color(0,0,0);
	world.getWidget(0)->text = "Игра";
	world.createWidget({400,227.5}, {0,0});
	world.getWidget(1)->isCircle = false;
	world.getWidget(1)->setSize({700,125});
	world.getWidget(1)->textColor = Color(0,0,0);
	world.getWidget(1)->text = "Инструкция";
	world.createWidget({400,372.5}, {0,0});
	world.getWidget(2)->isCircle = false;
	world.getWidget(2)->setSize({700,125});
	world.getWidget(2)->textColor = Color(0,0,0);
	world.getWidget(2)->text = "Настройки";
	world.createWidget({400,517.5}, {0,0});
	world.getWidget(3)->isCircle = false;
	world.getWidget(3)->setSize({700,125});
	world.getWidget(3)->textColor = Color(0,0,0);
	world.getWidget(3)->text = "Выход";
}

void begin1(ye::World &world){
	world.n = 5;
	world.setSize({800,400});
	
	// Перекраска мира	
	world.background = Color(0,128,0);
	//world.texture = *texture[0];
	
	// Спавн бортов
	world.createScene({7,200});
	world.getScene(0)->isCircle = false;
	world.getScene(0)->setSize({14,320});
	world.getScene(0)->haveShadow = true;
	world.getScene(0)->shadowOffset = {2,2};
	world.getScene(0)->shadowBlur = 5;
	world.createScene({212.5,7});
	world.getScene(1)->isCircle = false;
	world.getScene(1)->setSize({335,14});
	world.getScene(1)->haveShadow = true;
	world.getScene(1)->shadowOffset = {2,2};
	world.getScene(1)->shadowBlur = 5;
	world.createScene({587.5,7});
	world.getScene(2)->isCircle = false;
	world.getScene(2)->setSize({335,14});
	world.getScene(2)->haveShadow = true;
	world.getScene(2)->shadowOffset = {2,2};
	world.getScene(2)->shadowBlur = 5;
	world.createScene({793,200});
	world.getScene(3)->isCircle = false;
	world.getScene(3)->setSize({14,320});
	world.getScene(3)->haveShadow = true;
	world.getScene(3)->shadowOffset = {2,2};
	world.getScene(3)->shadowBlur = 5;
	world.createScene({212.5,393});
	world.getScene(4)->isCircle = false;
	world.getScene(4)->setSize({335,14});
	world.getScene(4)->haveShadow = true;
	world.getScene(4)->shadowOffset = {2,2};
	world.getScene(4)->shadowBlur = 5;
	world.createScene({587.5,393});
	world.getScene(5)->isCircle = false;
	world.getScene(5)->setSize({335,14});
	world.getScene(5)->haveShadow = true;
	world.getScene(5)->shadowOffset = {2,2};
	world.getScene(5)->shadowBlur = 5;
	
	// Перекраска бортов
	for(int i = 0; i < world.getAllScenes().size(); i++){
		world.getScene(i)->background = Color(128,64,0,0);
	}
	
	// Спавн лунок
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 3; j++){
		
			// Спавн компонентов c отключенным соударением для обозначения лунок
			world.createScene({15 + j*385, 15 + i*370});
			world.getScene(6 + i*3 + j)->setRadius(15);
			world.getScene(6 + i*3 + j)->setEnableCollision(false);
			world.getScene(6 + i*3 + j)->background = Color(0,32,0,0);
			
		}
	}
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 3; j++){
		
			// Спавн компонентов сцены для того, чтобы при ударе о них шар удалялся
			world.createScene({15 + j*385, 15 + i*370});
			world.getScene(12 + i*3 + j)->setRadius(8);
			world.getScene(12 + i*3 + j)->background = Color(0,0,0,0);
		}
	}
	
	// Спавн фона стола - компонента мира с отключенным соударением
	world.createScene({400, 200});
	world.getScene(18)->isCircle = false;
	world.getScene(18)->setSize({800,400});
	world.getScene(18)->texture = &textures[1];
	world.getScene(18)->setEnableCollision(false);
	
	
	// Спавн шаров
	int k = 0;
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < (5 - i); j++){
			world.spawnActor({200 + i*25, 150 + i*12.5 + j*25},{0,0});
			world.getActor(k)->setRadius(10);
			world.getActor(k)->background = Color(255,255,255);
			world.getActor(k)->texture = &textures[3];
			world.getActor(k)->haveShadow = true;
			world.getActor(k)->shadowOffset = {2,2};
			world.getActor(k)->shadowBlur = 4;
			k++;
		}
	}
	
	world.spawnActor({600, 200},{0,0});
	world.getActor(15)->setRadius(10);
	world.getActor(15)->background = Color(255,255,255);
	world.getActor(15)->texture = &textures[3];
	world.getActor(15)->haveShadow = true;
	world.getActor(15)->shadowOffset = {2,2};
	world.getActor(15)->shadowBlur = 4;
	
	////////// Спавн виджетов /////////////
	// Задний фон табло
	world.createWidget({400, 500},{0,0});
	world.getWidget(0)->isCircle = false;
	world.getWidget(0)->setSize({800,200});
	world.getWidget(0)->texture = &textures[2];
	
	// Выход в главное меню
	world.createWidget({700, 500},{0,0});
	world.getWidget(1)->isCircle = false;
	world.getWidget(1)->setSize({180,180});
	world.getWidget(1)->background = Color(255,192,0);
	world.getWidget(1)->textColor = Color(0,0,0);
	world.getWidget(1)->text = "Выйти";
	
	// Очки первого игрока
	world.createWidget({300, 500},{0,0});
	world.getWidget(2)->isCircle = false;
	world.getWidget(2)->setSize({180,180});
	world.getWidget(2)->background = Color(0,0,0);
	world.getWidget(2)->textColor = Color(255,255,255);
	world.getWidget(2)->text = "0";
	world.getWidget(2)->fontSize = 50;
	
	// Очки второго игрока
	world.createWidget({500, 500},{0,0});
	world.getWidget(3)->isCircle = false;
	world.getWidget(3)->setSize({180,180});
	world.getWidget(3)->background = Color(0,0,0);
	world.getWidget(3)->textColor = Color(255,255,255);
	world.getWidget(3)->text = "0";
	world.getWidget(3)->fontSize = 50;
	
	// Кто ходит
	world.createWidget({100, 500},{0,0});
	world.getWidget(4)->isCircle = false;
	world.getWidget(4)->setSize({180,180});
	world.getWidget(4)->background = Color(0,255,0);
	world.getWidget(4)->textColor = Color(0,0,0);
	world.getWidget(4)->text = "1";
	world.getWidget(4)->fontSize = 50;
	
	// Указатели удара
	world.createWidget({100, 500},{0,0});
	world.getWidget(5)->setRadius(3);
	world.getWidget(5)->background = Color(255,255,0,0);
	world.createWidget({100, 500},{0,0});
	world.getWidget(6)->setRadius(3);
	world.getWidget(6)->background = Color(255,255,0,0);
	world.createWidget({100, 500},{0,0});
	world.getWidget(7)->setRadius(3);
	world.getWidget(7)->background = Color(255,255,0,0);
	world.createWidget({100, 500},{0,0});
	world.getWidget(8)->setRadius(3);
	world.getWidget(8)->background = Color(255,255,0,0);
	world.createWidget({100, 500},{0,0});
	world.getWidget(9)->setRadius(3);
	world.getWidget(9)->background = Color(255,255,0,0);
	
	// Указатель нового шара
	world.createWidget({100, 500},{0,0});
	world.getWidget(10)->setRadius(10);
	world.getWidget(10)->background = Color(255,255,255,0);
	
	// Табло победы
	world.createWidget({400, 200},{0,0});
	world.getWidget(11)->isCircle = false;
	world.getWidget(11)->setSize({800,400});
	world.getWidget(11)->background = Color(255,255,255,0);
	
}

void begin2(ye::World &world){
	world.background = Color(0,128,0);
	
	// Спавн кнопок
	string strInstr;
	strInstr = "-----------------------Русский бильярд---------------------\n";
	strInstr += "Игра может вестись с компьютером или с человеком\n";
	strInstr += "Когда левое табло станет зеленым и покажет\n";
	strInstr += "ваш номер, то в этот момент вы можете бить\n";
	strInstr += "(Если вы играете с компьютером, ваш номер - 1)\n";
	strInstr += "Для удара наведите мыш на шар и зажмите левую кнопку\n";
	strInstr += "мыши, после чего, не опуская кнопку, вы можете водить\n";
	strInstr += "мышью, выбирая направление и силу удара.\n";
	strInstr += "После этого вы можете отжать левую кнопку мыши, чтобы\n";
	strInstr += "совершить удар или нажать правую кнопку мыши, чтобы выбрать\n";
	strInstr += "другой шар\n";
	strInstr += "После удара шар должен однажды удариться о другой\n";
	strInstr += "шар или трижды о борт, в противном случае у вас вынимается\n";
	strInstr += "один шар\n";
	strInstr += "Для установки вынутого шара выберете любое место на поле,\n";
	strInstr += "кроме края и нажмите левой кнопкой мыши\n";
	strInstr += "Побеждает игрок, первый забивший 8 шаров.\n";
	strInstr += "Желаем удачи!!!\n";
	world.createWidget({400,350}, {0,0});
	world.getWidget(0)->isCircle = false;
	world.getWidget(0)->setSize({700,450});
	world.getWidget(0)->text = strInstr;
	world.getWidget(0)->textColor = Color(255,255,255);
	world.getWidget(0)->fontSize = 20;
	world.getWidget(0)->background = Color(0,0,0,0);
	world.createWidget({400,537.5}, {0,0});
	world.getWidget(1)->isCircle = false;
	world.getWidget(1)->setSize({700,85});
	world.getWidget(1)->textColor = Color(0,0,0);
	world.getWidget(1)->text = "В меню";
}

void begin3(ye::World &world){
	world.background = Color(0,128,0);
	
	// Спавн кнопок
	world.createWidget({400,225}, {0,0});
	world.getWidget(0)->isCircle = false;
	world.getWidget(0)->setSize({700,50});
	world.getWidget(0)->background = Color(0,0,0,0);
	world.getWidget(0)->textColor = Color(255,255,255);
	world.getWidget(0)->fontSize = 24;
	world.getWidget(0)->text = "Нажмите, чтобы изменить соперника:";
	world.createWidget({400,320}, {0,0});
	world.getWidget(1)->isCircle = false;
	world.getWidget(1)->setSize({700,140});
	world.getWidget(1)->textColor = Color(0,0,0);
	world.getWidget(1)->text = "С компьютером";
	world.createWidget({400,500}, {0,0});
	world.getWidget(2)->isCircle = false;
	world.getWidget(2)->setSize({700,150});
	world.getWidget(2)->textColor = Color(0,0,0);
	world.getWidget(2)->text = "В меню";
	world.createWidget({400,25}, {0,0});
	world.getWidget(3)->isCircle = false;
	world.getWidget(3)->setSize({700,50});
	world.getWidget(3)->background = Color(0,0,0,0);
	world.getWidget(3)->textColor = Color(255,255,255);
	world.getWidget(3)->fontSize = 24;
	world.getWidget(3)->text = "Нажмите, чтобы изменить сложность игры с компьютером";
	world.createWidget({400,120}, {0,0});
	world.getWidget(4)->isCircle = false;
	world.getWidget(4)->setSize({700,140});
	world.getWidget(4)->textColor = Color(0,0,0);
	world.getWidget(4)->text = "Легкий";
	
	
}
