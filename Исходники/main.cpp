#include "yEngine.h"
#include<locale>

using namespace std;
using namespace sf;
using namespace ye;

void begin0(ye::World &world);
void begin1(ye::World &world);
void begin2(ye::World &world);
void begin3(ye::World &world);

// ������ ���������
int main() {

	// ��������� ���������
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	
	//
	int NumberWorld = 0;
	
	// �������� �������
	font.push_back(Font());
	font.push_back(Font());
	font[0].loadFromFile("Fonts/font.woff");
	font[1].loadFromFile("Fonts/roboto.woff");
	
	// �������� �������
	textures.push_back(Texture());
	textures[0].loadFromFile("Engine/Textures/system.png");
	textures.push_back(Texture());
	textures[1].loadFromFile("Textures/billiardTable.png");
	textures.push_back(Texture());
	textures[2].loadFromFile("Textures/gamePanel.png");
	textures.push_back(Texture());
	textures[3].loadFromFile("Textures/ball.png");
	
	// �������� ������
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
	
	// ��������� �����
	vector<World> world;
	world.push_back(World());
	world.push_back(World());
	world.push_back(World());
	world.push_back(World());
	world[0].frame = world[1].frame = 60;
	
	// �������� �������� (��� 0) - �������� ����
	begin0(world[0]);
	
	// �������� �������� (��� 1) - ����
	begin1(world[1]);

	// �������� �������� (��� 2) - ���� ����������
	begin2(world[2]);
	
	// �������� �������� (��� 3) - ���� ��������
	begin3(world[3]);

	//======================================
	//         ��������� �������
	//======================================
	loadEngineShaders(); // �������� �������� �����
	ContextSettings settings;
    settings.antialiasingLevel = 8;

	// �������� ����
	RenderWindow window( VideoMode(800,600), "������� �������", sf::Style::Close, settings);
	window.setFramerateLimit(60);
	
	// ������� �������
	Input input(window);
	
	// ������ ����������
	int player = 1;              // ����� ������, �������� ����������� ���
	int idChoose;                // id ���������� ���� ��� �����
	int score[2] = {0,0};        // ���� �������
	int hitBall = 0;             // ���������� ������ ������ � ������ ����
	int hitBoard = 0;            // ���������� ����� � ���� � ������ ����
	int frameTryes = 0;          // ���������� ������, ������� ����� ���������� ��������� ��� ������ �����
	int dCoor = 100;             // ������������ ������ ������������ �����������
	bool canChoose = true;       // ����� �� ������� ���?
	bool canHit = true;          // ����� �� ������?
	bool isScore = false;        // ������� �� ���� ������� ������� � ������ ����?
	bool isComputer = true;      // 2-� ����� - ���������?
	bool demo = false;      	 // ��� ������ - ����������?
	bool ballReturn = false;     // ����� �� ���������� ���?
	
	vector<int> idHoles(6);      // id ����������� ����, ���������� �������
	for(int i = 0; i < 6; i++){
		idHoles[i] = world[1].getScene(12 + i)->getId(); // ����� ���� id � �� ������ � ������
	};
	
	vector<Vector2f> coorHoles(6);    // ���������� ��������� �����, ���� ������� ���� ����
    coorHoles[0] = {25,25};
    coorHoles[1] = {400,29}; 
    coorHoles[2] = {775,25}; 
    coorHoles[3] = {25,375};
    coorHoles[4] = {400,371}; 
    coorHoles[5] = {775,375};
	
	// ������������ ���������� �����
	soundFonMenu.play();
	
	//======================================
	//         �������� �������
	//======================================
	while(window.isOpen()){
		World *thisWorld = &world[NumberWorld];
		Event event;
		
		// ���������� ������� �������� ����
		while(window.pollEvent(event)){
	    	if(event.type == Event::Closed){
	    		window.close();
			}
		}
		
		// ������ �����
		input.checkInputs();
		
		// ������ ������ Update
		switch (NumberWorld){
			case 0:
			{
				// ���������� ������ ����� � ����
				if(thisWorld->getWidget(0)->getOnHover()) thisWorld->getWidget(0)->background = Color(255,192,0);
				else thisWorld->getWidget(0)->background = Color(224,168,0);
				if(thisWorld->getWidget(0)->getOnPressed()){
					NumberWorld = 1;
					soundClick.play();
					soundFonMenu.stop();
				}
				
				// ���������� ������ ����� � ����������
				if(thisWorld->getWidget(1)->getOnHover()) thisWorld->getWidget(1)->background = Color(255,192,0);
				else thisWorld->getWidget(1)->background = Color(224,168,0);
				if(thisWorld->getWidget(1)->getOnPressed()){
					NumberWorld = 2;
					soundClick.play();
				}
				
				// ���������� ������ ����� � ���������
				if(thisWorld->getWidget(2)->getOnHover()) thisWorld->getWidget(2)->background = Color(255,192,0);
				else thisWorld->getWidget(2)->background = Color(224,168,0);
				if(thisWorld->getWidget(2)->getOnPressed()){
					NumberWorld = 3;
					soundClick.play();
				}
				
				// ���������� ������ ������ �� ����
				if(thisWorld->getWidget(3)->getOnHover()) thisWorld->getWidget(3)->background = Color(255,192,0);
				else thisWorld->getWidget(3)->background = Color(224,168,0);
				if(thisWorld->getWidget(3)->getOnPressed()) window.close();
				
				break;
			}
			case 1:
			{
				// ���������� ������ ������ � ����
				if(thisWorld->getWidget(1)->getOnHover()) thisWorld->getWidget(1)->background = Color(255,192,0);
				else thisWorld->getWidget(1)->background = Color(224,168,0);
				if(thisWorld->getWidget(1)->getOnPressed()){
					thisWorld->clearWorld();
					NumberWorld = 0;
					
					// ����������� �������� � ���������� � ���������� ���������
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
				
				
				//////////// ������� ������� /////////////////
				
				// ����� ����
				if(canHit){
					
					// ���� �������
					if((player == 1 || !isComputer) && !demo){
						
						// ������� �������� ���
						if(canChoose){
							for (int i = 0; i < thisWorld->getAllActors().size() && canChoose; i++){
								if(thisWorld->getActor(i)->getOnHover()) thisWorld->getActor(i)->background = Color(192,192,192);
								else thisWorld->getActor(i)->background = Color(255,255,255);
								
								// ��������� ������� ����� ������ ����. ���� ������ - �������� ������ ���
								if(thisWorld->getActor(i)->getOnPressed()){
									idChoose = thisWorld->getActor(i)->getId();
									canChoose = false;
									
									// �������� ��������� �����
									for (int j = 5; j <= 9; j++){
										thisWorld->getWidget(j) -> background = Color (255,0,0,255);
									}
								}
							}
						}
						
						// ������� ������ ��� � �������� ���� � ����������� �����
						else{
							
							// ��������� ����������� � ���� ����� � ���������� ��� � ������ v
							Vector2f coorBall = thisWorld->getActorById(idChoose)->coor;
							Vector2f coorMouse = input.getMousePosition();
							Vector2f v = coorBall - coorMouse;
							if(vectorLength(v) > 200) v = normalized(v)*200;
							
							// ���������� ����������� �����
							for (int j = 5; j <= 9; j++){
								thisWorld->getWidget(j) -> coor = coorBall + (j - 4)*v/5;
							}
							
							// ��������� ������� ������ ������ ����. ���� ������ - �������� ����� ����
							if(input.rightButton->getOnReleased()){
								canChoose = true;
								
								// ��������� ��������� �����
								for (int j = 5; j <= 9; j++){
									thisWorld->getWidget(j) -> background = Color (255,255,0,0);
								}
							}
							
							// ��������� ������� ����� ������ ����. ���� ������ - �������
							else if(input.leftButton->getOnReleased()){
								canChoose = true;
								canHit = false;
								thisWorld->getWidget(4)->background = Color(255,0,0);
								thisWorld->getActorById(idChoose)->background = Color(255,255,255);
								thisWorld->getActorById(idChoose)->v = 2*v;
								
								// ��������� ��������� �����
								for (int j = 5; j <= 9; j++){
									thisWorld->getWidget(j) -> background = Color (255,255,0,0);
								}
							}
						}
					}
					
					// ���� ���������
					else{
						
						int howManyBalls = thisWorld->getAllActors().size();
						
						// ��������� �������� ����, ������� ����� ���� � ������� ���������� ������
						int iRand1 = rand()%howManyBalls; // �����
						int iRand2 = rand()%howManyBalls;
						
						// ���� ��� ���� � ��� �� ���, �� ��������� �����
						while(iRand1 == iRand2){
							iRand1 = rand()%howManyBalls;
							iRand2 = rand()%howManyBalls;
						}
						
						// ����� ���������� ����� � �����
						Vector2f coor10 = thisWorld -> getActor(iRand1) -> coor;
						Vector2f coor20 = thisWorld -> getActor(iRand2) -> coor;
						Vector2f coorHole = coorHoles[rand()%6];// �������� ���������� ��� �����, � ������� ����� ��������
						
						// ����������� ���������� ���������� 1-�� ����
						Vector2f coor1 = coor20 - 20*normalized(coorHole - coor20);
						
						// ����������� ����������� � �������� �����
						Vector2f v = (100 + rand()%100) * normalized(coor1 - coor10);
						
						bool goodAngle = false, otherBalls = true;
						int tryes = 0;
						
						while(tryes < 20 && !(goodAngle && !otherBalls)){
							
							otherBalls = false;
							
							// ������� �� ���� �����?
							goodAngle = v*(coorHole - coor20)/(vectorLength(v)*vectorLength(coorHole - coor20)) > 0.5;
							
							// ���� �� �� ���� ����� ������ ����?
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
							
							// ���� �� �� ���� ����������� ���� ������ ����?
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
						
						// ���� ���� �������, � ������ ����� �� ������ ������ ����, �� ��������� ����!
						if((goodAngle && !otherBalls) || frameTryes > 60){
							if(dCoor == 0) thisWorld->getActor(iRand1)->v = 2*v;
							else thisWorld->getActor(iRand1)->v = 2*v + Vector2f(-dCoor/2 + rand()%dCoor, -dCoor/2 + rand()%dCoor);
						
							canChoose = true;
							canHit = false;
							frameTryes = 0;
							thisWorld->getWidget(4)->background = Color(255,0,0);
						}
						
						// ����� �������� ������ ���� � ����� ��� �����, �� ��� � ��������� �����
					}
				}
				
				// ����� ���������� ���
				else if(ballReturn){
					
					// ������� ����������� ���� ������ � ��������
					Vector2f coorMouse = input.getMousePosition();
					thisWorld->getWidget(10)-> coor = coorMouse;
					
					////////////// ���� �� ����������� � ������� ������ � ��������� �� ����������� � ����? /////////////////
					bool canPut = true;
					
					// ��������� �� ����������� � ����?
					if(coorMouse.x >= 44 && coorMouse.x <= 756 && coorMouse.y >= 44 && coorMouse.y <= 356) canPut = true;
					else canPut = false;
					
					// ���� �� ����������� � ������� ������?
					int i = 0;
					while(i < thisWorld->getAllActors().size() && canPut){
						if(vectorLength(thisWorld->getActor(i)->coor - coorMouse) < 20) canPut = false;
						i++;
					}
					
					// ���� ��� �� ����� ���������, ������ � �������, � ��������� ������ - � �����:
					if(canPut) thisWorld->getWidget(10)->background = Color(255,255,255);
					else thisWorld->getWidget(10)->background = Color(255,0,0);
					
					// ��������� ������� ����� ������ ����
					if(input.leftButton->getOnReleased()){
						
						// ������, ���� ��� ���������
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
						
						// ����� ����������� ���� ������
						else{
							soundMistake.play();
						}
					}
					
				}
				
				// ���� �������
				else{
					
					// �������� ��� ����
					int BallInMove = 0;
					for (int i = 0; i < thisWorld->getAllActors().size(); i++){
						
						// ���������, c ��� ���������� ���
						vector<World::EventHit> eventHit = thisWorld->getActor(i)->getEventHit();
						bool inHole = false;
						for(int j = 0; j < eventHit.size() && !inHole; j++){
							
							// ��� �������� � ������ �����
							if(eventHit[j].typeName == "Actor"){
								hitBall++;
								soundImpactBalls.play();
							}
							
							// ��� �������� � ������
							else if(eventHit[j].typeName == "Scene"){
								hitBoard++;
								soundImpactBoard.play();
							}
							
							// ��������� �� ��� � �����?
							for(int k = 0; k < idHoles.size() && !inHole; k++){
								if(eventHit[j].id == idHoles[k]){
									inHole = true;
								}
								
							}
						}
						
						// ���� ��� � �����, ������� ���
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
						
						// ����� �������� ���
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
	
					// ���� ������������
					if(BallInMove == 0){
						
						// ���� �������� �������, ������������ ��� ��������� ������ ��������� ���
						if(hitBall == 0 && hitBoard < 3 && score[player - 1]){
							score[player - 1]--;
							thisWorld->getWidget(player + 1)->text = to_string(score[player - 1]);
							
							// ������� ���������� ����� (�������)
							int i = thisWorld->getAllActors().size();
							
							// ���� ������ ��������� (� ��������� �� ������ ��� � �����) ��� ������ ��� ��������, �� ������ ������������:
							if(((player == 2 && isComputer) || !isComputer) && !demo){
								ballReturn = true;
								thisWorld->getWidget(10)-> background = Color(255,255,255,255);
								thisWorld->getWidget(10)-> texture = &textures[3];
							}
							
							// ���� ������ ������������ (��� ��������� ������ ��� � �����), �� ������ ���������:
							else if((player == 1 && isComputer) || demo){
								
								Vector2f coorComputer;
								bool canPut = false;
								
								while(!canPut){
									canPut = true;
									
									// ��������� �������� ��������� ����� ��� ����, ���� ��������� ���
									coorComputer = {44 + rand()%712, 44 + rand()%312};
					
									// ���� �� ����������� � ������� ������?
									int i = 0;
									while(i < thisWorld->getAllActors().size() && canPut){
										if(vectorLength(thisWorld->getActor(i)->coor - coorComputer) < 20) canPut = false;
										i++;
									}
								}
						
								// ������, ���� ��� ���������
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
						
						// � ������ �� ������� 8 ��� ����� ����� => ������
						if(score[player - 1] >= 8){
							thisWorld->getWidget(11)->background = Color(224,168,0);
							thisWorld->getWidget(11)->text = "������� ����� " + to_string(player);
						}
						
						// ���� ���, �� ���� ������������
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
			
				// ���������� ������ ������ � ����
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
				// ���������� ������ ������ ���������
				if(thisWorld->getWidget(1)->getOnHover()) thisWorld->getWidget(1)->background = Color(255,192,0);
				else thisWorld->getWidget(1)->background = Color(224,168,0);
				if(thisWorld->getWidget(1)->getOnPressed()){
					if(!isComputer){
						isComputer = true;
						thisWorld->getWidget(1)->text = "C �����������";
					}
					else if(!demo){
						demo = true;
						thisWorld->getWidget(1)->text = "������������";
					}
					else{
						isComputer = demo = false;
						thisWorld->getWidget(1)->text = "C ���������";
					}

					soundClick.play();
				}
			
				// ���������� ������ ������ � ����
				if(thisWorld->getWidget(2)->getOnHover()) thisWorld->getWidget(2)->background = Color(255,192,0);
				else thisWorld->getWidget(2)->background = Color(224,168,0);
				if(thisWorld->getWidget(2)->getOnPressed()){
					NumberWorld = 0;
					soundClick.play();
				}
				
				// ���������� ������ ������ ���������
				if(thisWorld->getWidget(4)->getOnHover()) thisWorld->getWidget(4)->background = Color(255,192,0);
				else thisWorld->getWidget(4)->background = Color(224,168,0);
				if(thisWorld->getWidget(4)->getOnPressed()){
					switch(dCoor){
						case 100:
							dCoor = 10;
							thisWorld->getWidget(4)->text = "�������";
							break;
						case 10:
							dCoor = 0;
							thisWorld->getWidget(4)->text = "�������";
							break;
						case 0:
							dCoor = 100;
							thisWorld->getWidget(4)->text = "������";
							break;
					}
					soundClick.play();
				}
				
				break;
			}
		}
		
		// ���������� ������
		thisWorld->update(input);
		
		// ����������� ������
		redraw(window, *thisWorld);
	}
}

void begin0(ye::World &world){
	world.background = Color(0,128,0);	
	
	// ����� ������
	world.createWidget({400,82.5}, {0,0});	
	world.getWidget(0)->isCircle = false;
	world.getWidget(0)->setSize({700,125});
	world.getWidget(0)->textColor = Color(0,0,0);
	world.getWidget(0)->text = "����";
	world.createWidget({400,227.5}, {0,0});
	world.getWidget(1)->isCircle = false;
	world.getWidget(1)->setSize({700,125});
	world.getWidget(1)->textColor = Color(0,0,0);
	world.getWidget(1)->text = "����������";
	world.createWidget({400,372.5}, {0,0});
	world.getWidget(2)->isCircle = false;
	world.getWidget(2)->setSize({700,125});
	world.getWidget(2)->textColor = Color(0,0,0);
	world.getWidget(2)->text = "���������";
	world.createWidget({400,517.5}, {0,0});
	world.getWidget(3)->isCircle = false;
	world.getWidget(3)->setSize({700,125});
	world.getWidget(3)->textColor = Color(0,0,0);
	world.getWidget(3)->text = "�����";
}

void begin1(ye::World &world){
	world.n = 5;
	world.setSize({800,400});
	
	// ���������� ����	
	world.background = Color(0,128,0);
	//world.texture = *texture[0];
	
	// ����� ������
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
	
	// ���������� ������
	for(int i = 0; i < world.getAllScenes().size(); i++){
		world.getScene(i)->background = Color(128,64,0,0);
	}
	
	// ����� �����
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 3; j++){
		
			// ����� ����������� c ����������� ����������� ��� ����������� �����
			world.createScene({15 + j*385, 15 + i*370});
			world.getScene(6 + i*3 + j)->setRadius(15);
			world.getScene(6 + i*3 + j)->setEnableCollision(false);
			world.getScene(6 + i*3 + j)->background = Color(0,32,0,0);
			
		}
	}
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 3; j++){
		
			// ����� ����������� ����� ��� ����, ����� ��� ����� � ��� ��� ��������
			world.createScene({15 + j*385, 15 + i*370});
			world.getScene(12 + i*3 + j)->setRadius(8);
			world.getScene(12 + i*3 + j)->background = Color(0,0,0,0);
		}
	}
	
	// ����� ���� ����� - ���������� ���� � ����������� �����������
	world.createScene({400, 200});
	world.getScene(18)->isCircle = false;
	world.getScene(18)->setSize({800,400});
	world.getScene(18)->texture = &textures[1];
	world.getScene(18)->setEnableCollision(false);
	
	
	// ����� �����
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
	
	////////// ����� �������� /////////////
	// ������ ��� �����
	world.createWidget({400, 500},{0,0});
	world.getWidget(0)->isCircle = false;
	world.getWidget(0)->setSize({800,200});
	world.getWidget(0)->texture = &textures[2];
	
	// ����� � ������� ����
	world.createWidget({700, 500},{0,0});
	world.getWidget(1)->isCircle = false;
	world.getWidget(1)->setSize({180,180});
	world.getWidget(1)->background = Color(255,192,0);
	world.getWidget(1)->textColor = Color(0,0,0);
	world.getWidget(1)->text = "�����";
	
	// ���� ������� ������
	world.createWidget({300, 500},{0,0});
	world.getWidget(2)->isCircle = false;
	world.getWidget(2)->setSize({180,180});
	world.getWidget(2)->background = Color(0,0,0);
	world.getWidget(2)->textColor = Color(255,255,255);
	world.getWidget(2)->text = "0";
	world.getWidget(2)->fontSize = 50;
	
	// ���� ������� ������
	world.createWidget({500, 500},{0,0});
	world.getWidget(3)->isCircle = false;
	world.getWidget(3)->setSize({180,180});
	world.getWidget(3)->background = Color(0,0,0);
	world.getWidget(3)->textColor = Color(255,255,255);
	world.getWidget(3)->text = "0";
	world.getWidget(3)->fontSize = 50;
	
	// ��� �����
	world.createWidget({100, 500},{0,0});
	world.getWidget(4)->isCircle = false;
	world.getWidget(4)->setSize({180,180});
	world.getWidget(4)->background = Color(0,255,0);
	world.getWidget(4)->textColor = Color(0,0,0);
	world.getWidget(4)->text = "1";
	world.getWidget(4)->fontSize = 50;
	
	// ��������� �����
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
	
	// ��������� ������ ����
	world.createWidget({100, 500},{0,0});
	world.getWidget(10)->setRadius(10);
	world.getWidget(10)->background = Color(255,255,255,0);
	
	// ����� ������
	world.createWidget({400, 200},{0,0});
	world.getWidget(11)->isCircle = false;
	world.getWidget(11)->setSize({800,400});
	world.getWidget(11)->background = Color(255,255,255,0);
	
}

void begin2(ye::World &world){
	world.background = Color(0,128,0);
	
	// ����� ������
	string strInstr;
	strInstr = "-----------------------������� �������---------------------\n";
	strInstr += "���� ����� ������� � ����������� ��� � ���������\n";
	strInstr += "����� ����� ����� ������ ������� � �������\n";
	strInstr += "��� �����, �� � ���� ������ �� ������ ����\n";
	strInstr += "(���� �� ������� � �����������, ��� ����� - 1)\n";
	strInstr += "��� ����� �������� ��� �� ��� � ������� ����� ������\n";
	strInstr += "����, ����� ����, �� ������� ������, �� ������ ������\n";
	strInstr += "�����, ������� ����������� � ���� �����.\n";
	strInstr += "����� ����� �� ������ ������ ����� ������ ����, �����\n";
	strInstr += "��������� ���� ��� ������ ������ ������ ����, ����� �������\n";
	strInstr += "������ ���\n";
	strInstr += "����� ����� ��� ������ ������� ��������� � ������\n";
	strInstr += "��� ��� ������ � ����, � ��������� ������ � ��� ����������\n";
	strInstr += "���� ���\n";
	strInstr += "��� ��������� �������� ���� �������� ����� ����� �� ����,\n";
	strInstr += "����� ���� � ������� ����� ������� ����\n";
	strInstr += "��������� �����, ������ �������� 8 �����.\n";
	strInstr += "������ �����!!!\n";
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
	world.getWidget(1)->text = "� ����";
}

void begin3(ye::World &world){
	world.background = Color(0,128,0);
	
	// ����� ������
	world.createWidget({400,225}, {0,0});
	world.getWidget(0)->isCircle = false;
	world.getWidget(0)->setSize({700,50});
	world.getWidget(0)->background = Color(0,0,0,0);
	world.getWidget(0)->textColor = Color(255,255,255);
	world.getWidget(0)->fontSize = 24;
	world.getWidget(0)->text = "�������, ����� �������� ���������:";
	world.createWidget({400,320}, {0,0});
	world.getWidget(1)->isCircle = false;
	world.getWidget(1)->setSize({700,140});
	world.getWidget(1)->textColor = Color(0,0,0);
	world.getWidget(1)->text = "� �����������";
	world.createWidget({400,500}, {0,0});
	world.getWidget(2)->isCircle = false;
	world.getWidget(2)->setSize({700,150});
	world.getWidget(2)->textColor = Color(0,0,0);
	world.getWidget(2)->text = "� ����";
	world.createWidget({400,25}, {0,0});
	world.getWidget(3)->isCircle = false;
	world.getWidget(3)->setSize({700,50});
	world.getWidget(3)->background = Color(0,0,0,0);
	world.getWidget(3)->textColor = Color(255,255,255);
	world.getWidget(3)->fontSize = 24;
	world.getWidget(3)->text = "�������, ����� �������� ��������� ���� � �����������";
	world.createWidget({400,120}, {0,0});
	world.getWidget(4)->isCircle = false;
	world.getWidget(4)->setSize({700,140});
	world.getWidget(4)->textColor = Color(0,0,0);
	world.getWidget(4)->text = "������";
	
	
}
