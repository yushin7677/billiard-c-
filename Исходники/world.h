//=============================================================
//                         ����� ����                  
//=============================================================

// ����� ����
namespace ye{
	class World{
		
	private:
		class Object;
	    class Actor;
	    class Scene;
	    class Widget;
	    
		// ������� �������� ����. ������ ������ ��� �������� � �����������\
		��������� ������ ����� �������. 
		std::vector<Actor> actor;
		std::vector<Scene> scene;
		std::vector<Widget> widget;
		
		// ������ ����
		sf::Vector2f size;
		
		// ������� id. ������ ������ ������ ����� ���������� id, ������� ����� �������� ������ ������ �������\
		idLast ������������� �� �������
		int idLast = 1;
		
	public:
		
		// ��������� ������ � ����������
		struct EventHit{
			int id;
			std::string typeName;
			std::string className;
		};
		
		// ����������
		double n = 1; // �������� ������� ��������
		int frame = 60; // ���� ���������� ������
		
		// ����
		sf::Color background = sf::Color(128,255,255);
		
		// ��������
		sf::Texture *texture = &globalTexture;
		
		// �����������
		World(){
			size.x = 800, size.y = 600;
		};
		
		////////////////// ������� /////////////////////////
		
		// ����� ������
		void spawnActor(sf::Vector2f loc_vvod, sf::Vector2f vel_vvod){
			int i = actor.size();
			
			// �������� ������ � ������ � ������ actor
			actor.push_back(Actor());
			actor[i].coor = loc_vvod;
			actor[i].v = vel_vvod;
			actor[i].id = idLast;
			actor[i].typeName = "Actor";
			idLast++;
		};
		
		// �������� ���������� ����
		void createScene(sf::Vector2f loc_vvod){
			int i = scene.size();
			
			// �������� ���������� ���� � ������ � ������ scene
			scene.push_back(Scene());
			scene[i].coor = loc_vvod;
			scene[i].id = idLast;
			scene[i].infinityMass = true;
			scene[i].typeName = "Scene";
			idLast++;
		};
		
		// �������� �������
		void createWidget(sf::Vector2f loc_vvod, sf::Vector2f vel_vvod){
			int i = widget.size();
			
			// �������� ������� ���� � ������ � ������ widget
			widget.push_back(Widget());
			widget[i].coor = loc_vvod;
			widget[i].v = vel_vvod;
			widget[i].id = idLast;
			widget[i].enableCollision = false;
			widget[i].typeName = "Widget";
			idLast++;
		};
		
		// �������� �������� ������ �������
		void deleteActor(int i){
			actor.erase(actor.begin() + i);
		};
		
		void deleteScene(int i){
			scene.erase(scene.begin() + i);
		};
		
		void deleteWidget(int i){
			widget.erase(widget.begin() + i);
		};
		
		// �������� ���� �������� � ����
		void clearWorld(){
			actor.clear();
			scene.clear();
			widget.clear();
		};
		
		// �������� ��������� ���� �������� � �������� ������� ����
		void update(ye::Input input){
			
			// ��������� eventHit
			for(int i = 0; i < actor.size(); i++){
				actor[i].eventHit.clear();
				actor[i].eventHit.shrink_to_fit();
			}
			
			// �������� ������������ ������� � �������� ��������� �� ������ ���������� ��������
			for(int k = 0; k < n; k++){
				for(int i = 0; i < actor.size(); i++){
					
					relationshipWorld(actor[i]);
					for(int j = 0; j < scene.size(); j++){
						relationshipObjects(actor[i],scene[j]);
					}
					for(int j = i + 1; j < actor.size() && !actor[i].collision; j++){
						relationshipObjects(actor[i],actor[j]);
					}
					actor[i].coor = actor[i].coor + actor[i].d;
				}
				
				// ����� ������� collision (��� ��� �����, ������� ����)
				for(int i = 0; i < actor.size(); i++){
					actor[i].collision = false;
				}
			}
			
			// �������� ������� �� ��������
			for(int i = 0; i < widget.size(); i++){
				Widget *w = &widget[i];
				sf::Vector2f mPos = input.getMousePosition();
				
				// ����������� ����������� ���������
				w->lastButton = w->pressButton;
				
				///////////// �������� ������ ��������� /////////////
				w->pressButton = w->onHover = w->onPressed = w->onReleased = false;
				
				if(w->isCircle){
					if(vectorLength(mPos - w->coor) <= w->r){
						w->onHover = true;
					}
				}
				else{
					if(fabs(mPos.x - w->coor.x) <= w->size.x/2 && fabs(mPos.y - w->coor.y) <= w->size.y/2){
						w->onHover = true;
					}
				}
				
				if(input.leftButton->getOnPressed() && w->onHover) w->pressButton = true;
				if(w->lastButton == false && w->pressButton == true) w->onPressed = true;
				else if(w->lastButton == true && w->pressButton == false) w->onReleased = true;	
			}
			
			// �������� ������� �� �������
			for(int i = 0; i < actor.size(); i++){
				Actor *a = &actor[i];
				sf::Vector2f mPos = input.getMousePosition();
				
				// ����������� ����������� ���������
				a->lastButton = a->pressButton;
				
				///////////// �������� ������ ��������� /////////////
				a->pressButton = a->onHover = a->onPressed = a->onReleased = false;
				
				if(a->isCircle){
					if(vectorLength(mPos - a->coor) <= a->r){
						a->onHover = true;
					}
				}
				else{
					if(fabs(mPos.x - a->coor.x) <= a->size.x/2 && fabs(mPos.y - a->coor.y) <= a->size.y/2){
						a->onHover = true;
					}
				}
				
				if(input.leftButton->getOnPressed() && a->onHover) a->pressButton = true;
				if(a->lastButton == false && a->pressButton == true) a->onPressed = true;
				else if(a->lastButton == true && a->pressButton == false) a->onReleased = true;	
			}
			
			// �������� ������� �� ����������� ����
			for(int i = 0; i < scene.size(); i++){
				Scene *s = &scene[i];
				sf::Vector2f mPos = input.getMousePosition();
				
				// ����������� ����������� ���������
				s->lastButton = s->pressButton;
				
				///////////// �������� ������ ��������� /////////////
				s->pressButton = s->onHover = s->onPressed = s->onReleased = false;
				
				if(s->isCircle){
					if(vectorLength(mPos - s->coor) <= s->r){
						s->onHover = true;
					}
				}
				else{
					if(fabs(mPos.x - s->coor.x) <= s->size.x/2 && fabs(mPos.y - s->coor.y) <= s->size.y/2){
						s->onHover = true;
					}
				}
				
				if(input.leftButton->getOnPressed() && s->onHover) s->pressButton = true;
				if(s->lastButton == false && s->pressButton == true) s->onPressed = true;
				else if(s->lastButton == true && s->pressButton == false) s->onReleased = true;	
			}
			
			// ����� ��������
			for(int i = 0; i < widget.size(); i++){
				Widget *w = &widget[i];
				w->coor = w->coor + w->v/frame;
			}
		}
	
		// ������ �������� �� ������������ � �������
		Actor *getActor(int i){
			return &actor[i];
		}
		
		Scene *getScene(int i){
			return &scene[i];
		}
		
		Widget *getWidget(int i){
			return &widget[i];
		}
		
		// ������ �������� �� id
		Actor *getActorById(int i){
			int j = 0;
			while(j < actor.size() && actor[j].id != i){
				j++;
			};
			return &actor[j];
		}
		
		Scene *getSceneById(int i){
			int j = 0;
			while(j < scene.size() && scene[j].id != i){
				j++;
			};
			return &scene[j];
		}
		
		Widget *getWidgetById(int i){
			int j = 0;
			while(j < widget.size() && widget[j].id != i){
				j++;
			};
			return &widget[j];
		}
		
		// ������ ����� ��������
		std::vector<Actor> getAllActors(){
			return actor;
		}
		
		std::vector<Scene> getAllScenes(){
			return scene;
		}
		
		std::vector<Widget> getAllWidgets(){
			return widget;
		}
		
		// ���� ������� ����
		void setSize(sf::Vector2f size_vvod){
			size = size_vvod;
		}
		
		// ����� ������� ����
		sf::Vector2f getSize(){
			return size;
		}
		
	private:
		
		// ����� ����� ��������
		class Object{
		protected:
			
			int id;
			
			std::string typeName;
			
			sf::Vector2f d;
			sf::Vector2f size;
			double r = 10, m = 1;
			
			// ����������, ������������ � �������� ����������\
			���������� collision ����������, ���� �� ��������� ���������� ������ � ������ ��������\
			������� �������� ������� ������ ���� ����� �� ������� ����������� ��������, ����� ����\
			���������� collision ���������� true, � ������ � ����� ��������� collision �� ���������\
			� ����������� ��������. ��� ����������� ���� �� ������������, ��� ��� ��� ��������� ��\
			���� ������������ �������� �������. ��� �������� �� �������� ����� ����, ��� ��� �� ��������\
			����������� ���������
			bool collision = false; // ������������ ������ ��� �������
			bool enableCollision = true; // ��� �������� ������ false
			bool infinityMass = false; // ������ ��� �����������
			
			// ���������� ��������� ��������� � ������� ���� �� ���
			bool pressButton = false;
			bool lastButton = false;
			bool onPressed = false;
			bool onReleased = false;
			bool onHover = false;
			
			// ������ ������ ����������
			std::vector<EventHit> eventHit;
		
		public:
			
			std::string className;
			
			sf::Vector2f coor;
			sf::Vector2f v;
			bool isCircle = true;// ��� false, �� ������ - �������������
			
			// ����
			sf::Color background = sf::Color(255,255,128);
			
			// ��������
			sf::Texture *texture = new sf::Texture();
			
			// ����
			sf::Color shadowColor = sf::Color(0,0,0);
			double shadowBlur = 0;
			sf::Vector2f shadowOffset = {0,0};
			bool haveShadow = false;
			
			// ������� ���������� (����� ������� ������������� ����� �������) - ���� �� ������������
			//int layer = 0;
			
			Object(){
				texture = &ye::globalTexture;
			}
		
			// ������� ������
			sf::Vector2f getSize(){
				return size;
			}
			double getRadius(){
				return r;
			}
			int getId(){
				return id;
			}
			
			bool getPress(){
				return pressButton;
			}
			
			bool getOnPressed(){
				return onPressed;
			}
			
			bool getOnReleased(){
				return onReleased;
			}
			bool getOnHover(){
				return onHover;
			}
			
			
			// ������� �����
			void setSize(sf::Vector2f v2){
				size = v2;
			}
			void setRadius(double r_vvod){
				if(r_vvod < 0.01) r_vvod = 0.01;
				r = r_vvod;
			}
			
			friend World;
		
		};
		
		// ����� ������
		class Actor : public Object{
		
		public:
		
			// ������� ������
			double getMass(){
				return m;
			}
			bool getInfinityMass(){
				return infinityMass;
			}
			std::vector<EventHit> getEventHit(){
				return eventHit;
			}
			bool getEnableCollision(bool ec_vvod){
				return enableCollision;
			}
			
		
			// ������� �����
			void setMass(double m_vvod){
				if(m_vvod < 0.01) m_vvod = 0.01;
				m = m_vvod;
			}
			void setInfinityMass(bool inf_vvod){
				infinityMass = inf_vvod;
			}
			void setEnableCollision(bool ec_vvod){
				enableCollision = ec_vvod;
			}
			
			friend World;
		
		};
		
		// ����� ���������� ����
		class Scene : public Object{
		
		public:
			
			bool getEnableCollision(bool ec_vvod){
				return enableCollision;
			}
			
			void setEnableCollision(bool ec_vvod){
				enableCollision = ec_vvod;
			}
			
			friend World;
		
		};
		
		// ����� �������
		class Widget : public Object{
		
		public:
			
			// ����� �������
			std::string text = "";
			sf::Color textColor = sf::Color(0,0,128);
			int fontSize = 30;
			
			friend World;
		
		};
		
		/////////// ������� �������� //////////
		
		// �������� ������ � �����
		void relationshipWorld(Actor &a){ // ���� ���������� ������������ � �����, �������� �������� ���������, ���� ���, �� �������� ������������� ��������
			
			double cx = a.coor.x;
			double cy = a.coor.y;
			double vx = a.v.x/(n*frame);
			double vy = a.v.y/(n*frame);
			double w,h;
			if(a.isCircle) w = h = 2*a.r;
			else w = a.size.x, h = a.size.y;
			
			// ������������ � ����� ���� �� x
			if (cx + vx < w/2){
				a.v.x= -a.v.x;
				a.d.x = -vx + w - 2*cx;
				
				a.collision = true;
				
				// ������ ������ � ������������
				int eventLength = a.eventHit.size();
				a.eventHit.push_back(EventHit());
				a.eventHit[eventLength].id = 0;
				a.eventHit[eventLength].typeName = "World";
				a.eventHit[eventLength].className = "World";
			}
			else if (cx + vx > size.x - w/2){
				a.v.x = -a.v.x;
				a.d.x = -vx + 2*(size.x - w/2) - 2*cx;
				
				a.collision = true;
				
				// ������ ������ � ������������
				int eventLength = a.eventHit.size();
				a.eventHit.push_back(EventHit());
				a.eventHit[eventLength].id = 0;
				a.eventHit[eventLength].typeName = "World";
				a.eventHit[eventLength].className = "World";
			}
			else{
				a.d.x = vx;
			};
				
			// ������������ � ����� ���� �� y
			if (cy + vy < h/2){
				a.v.y = -a.v.y;
				a.d.y = -vy + h - 2*cy;
				
				a.collision = true;
				
				// ������ ������ � ������������
				int eventLength = a.eventHit.size();
				a.eventHit.push_back(EventHit());
				a.eventHit[eventLength].id = 0;
				a.eventHit[eventLength].typeName = "World";
				a.eventHit[eventLength].className = "World";
			}
			else if (cy + vy > size.y - h/2){
				a.v.y = -a.v.y;
				a.d.y = -vy + 2*(size.y - h/2) - 2*cy;
				
				a.collision = true;
				
				// ������ ������ � ������������
				int eventLength = a.eventHit.size();
				a.eventHit.push_back(EventHit());
				a.eventHit[eventLength].id = 0;
				a.eventHit[eventLength].typeName = "World";
				a.eventHit[eventLength].className = "World";
			}
			else{
				a.d.y = vy;
			};
				
		}
	
		// �������� �������� ���� � ������
		void relationshipObjects(Object &a1, Object &a2){
			if (!a1.collision && (!a2.collision || a2.typeName == "Scene") && a1.enableCollision && a2.enableCollision){
				
				// ������������ 2 �����������
				if (a1.isCircle && a2.isCircle){
					double dr = a1.r + a2.r;
					double m1 = a1.m, m2 = a2.m;
				
					if (vectorLength( (a1.coor + a1.v/n/frame) - (a2.coor + a2.v/n/frame) ) < dr){
					
						// ��������� ��������
						sf::Vector2f v10 = a1.v/n/frame, v20 = a2.v/n/frame;
					
						// ������� ����� ����������(������,a - ����� ���� �� ����������???)
						double cx = a2.coor.x - a1.coor.x;
						double cy = a2.coor.y - a1.coor.y;
						double vx = v20.x - v10.x;
						double vy = v20.y - v10.y; 
						double part;
						if(vx != 0 || vy != 0){
							double D = -(cy*vx - cx*vy)*(cy*vx - cx*vy) + dr*dr*(vx*vx + vy*vy);
							if (D < 0) D = 0;
							part = ( - (cx*vx + cy*vy) - sqrt(D))/(vx*vx + vy*vy);
						}
						else{
							part = 0.99;
						};
					
						// ��������� ����� �����
						sf::Vector2f coor1 = a1.coor + v10*part;
						sf::Vector2f coor2 = a2.coor + v20*part;
					
						// ������� ���������� � ������������� ������������ ���������
						sf::Vector2f vOs = coor2 - coor1;
						sf::Vector2f vn1, vn2, vt1, vt2, v1, v2;
						if (vectorLength(v10) != 0){
							double cos1 = v10*vOs/( vectorLength(v10)*vectorLength(vOs) );
							vn1 = vOs * (vectorLength(v10) * cos1/vectorLength(vOs));
							vt1 = v10 - vn1;
						}
						else{
							vn1.x = vn1.y = vt1.x = vt1.y = 0;
						}
					
						if(vectorLength(v20) != 0){
							double cos2 = v20*vOs/( vectorLength(v20)*vectorLength(vOs) );
							vn2 = vOs * (vectorLength(v20) * cos2/vectorLength(vOs));
							vt2 = v20 - vn2;
						}
						else{
							vn2.x = vn2.y = vt2.x = vt2.y = 0;
						}
					
						// �������� ��������
						if(a1.infinityMass && a2.infinityMass && a2.typeName == "Actor"){
							v1 = vn2 + vt1;
							v2 = vn1 + vt2;
						}
						else if(a1.infinityMass && !a2.infinityMass){
							v1 = vn1 + vt1;
							v2 = vn1*2 - vn2 + vt2;
						}
						else if(!a1.infinityMass && a2.infinityMass){
							v1 = vn2*2 - vn1 + vt1;
							v2 = vn2 + vt2;
						}
						else if(!a1.infinityMass && !a2.infinityMass){
							v1 = (vn1*(m1/m2-1) + vn2*2)/(1+m1/m2) + vt1;
							v2 = (vn2*(m2/m1-1) + vn1*2)/(1+m2/m1) + vt2;
						}
						a1.v = v1*n*frame;
						a2.v = v2*n*frame;
					
						//
						a1.d = v10*part + v1*(1-part);
						a2.d = v20*part + v2*(1-part);
						a1.collision = true;
						a2.collision = true;
						
						// ������ ������ � ������������
						int eventLength1 = a1.eventHit.size(), eventLength2 = a2.eventHit.size();
						a1.eventHit.push_back(EventHit()), a2.eventHit.push_back(EventHit());
						a1.eventHit[eventLength1].id = a2.id, a2.eventHit[eventLength2].id = a1.id;
						a1.eventHit[eventLength1].typeName = a2.typeName;
						a2.eventHit[eventLength2].typeName = a1.typeName;
						a1.eventHit[eventLength1].className = a2.className, a2.eventHit[eventLength2].className = a1.className;
					}
					else{
						a1.d = a1.v/n/frame;
						a2.d = a2.v/n/frame;
					}
				}
					
				// ������������ ���������� � ��������������
				else if ((a1.isCircle && !a2.isCircle) || (!a1.isCircle && a2.isCircle)){
					
					// ���������, ��� ����, � ��� �������������
					Object *aCirc, *aRect;
					if(a1.isCircle) aCirc = &a1, aRect = &a2;
					else aCirc = &a2, aRect = &a1;
					                  
					double mCirc = aCirc->m, mRect = aRect->m;    // �����
					double w = aRect->size.x, h = aRect->size.y;  // ������� ��������������
					double r = aCirc->r;                          // ������� �����
					sf::Vector2f Cc0 = aCirc->coor;                   // ��������� ���������� �����
					sf::Vector2f Vc0 = aCirc->v/n/frame;              // ��������� �������� �����
					sf::Vector2f Cr0 = aRect->coor;                   // ��������� ���������� ��������������
					sf::Vector2f Vr0 = aRect->v/n/frame;              // ��������� �������� ��������������
					sf::Vector2f Vc, Vr;                              // �������� ��������
					double part, partX = 1, partY = 1;
					
					// ���������, ���� �� ����������
					if(fabs((Cc0.x + Vc0.x) - (Cr0.x + Vr0.x)) < (r + w/2) && fabs((Cc0.y + Vc0.y) - (Cr0.y + Vr0.y)) < (r + h/2)){
	
						/////// �� ����� ��� ����������� ������? ////////////
						// ����� ��� ������
						if( Cc0.x <= Cr0.x ){	
							partX = (r + w/2 + Cc0.x - Cr0.x)/(Vr0.x - Vc0.x);
						}
						else if( Cc0.x >= Cr0.x ){
							partX = (r + w/2 + Cr0.x - Cc0.x)/(Vc0.x - Vr0.x);
						}
						
						// ���� ������ ��� �����
						if( Cc0.y <= Cr0.y ){
							partY = (r + h/2 + Cc0.y - Cr0.y)/(Vr0.y - Vc0.y);
						}
						else if( Cc0.y >= Cr0.y ){
							partY = (r + h/2 + Cr0.y - Cc0.y)/(Vc0.y - Vr0.y);
						}
						
						// ��������� ��������� ����� ����������
						sf::Vector2f Ccx = Cc0 + Vc0*partX;
						sf::Vector2f Crx = Cr0 + Vr0*partX;
						
						sf::Vector2f Ccy = Cc0 + Vc0*partY;
						sf::Vector2f Cry = Cr0 + Vr0*partY;
						
						// ��� �� ��������� �����������?
						if (!((Ccx.y + r >= Crx.y - h/2) && (Ccx.y - r <= Crx.y + h/2))) partX = 2;
						if (!((Ccy.x + r >= Cry.x - w/2) && (Ccy.x - r <= Cry.x + w/2))) partY = 2;
						
						// ����������� ����� ��� ������!
						if(partX < partY){
							part = partX;
							
							// �������� �������� 
							if(aRect->infinityMass && aCirc->infinityMass && aRect->typeName == "Actor" && aCirc->typeName == "Actor"){
								Vr = {Vc0.x, Vr0.y};
								Vc = {Vr0.x, Vc0.y};
							}
							else if(aRect->infinityMass && !aCirc->infinityMass){
								Vr = Vr0;
								Vc = {2*Vr0.x - Vc0.x, Vc0.y};
							}
							else if(!aRect->infinityMass && aCirc->infinityMass){
								Vr = {2*Vc0.x - Vr0.x, Vc0.y};
								Vc = Vc0;
							}
							else if(!aRect->infinityMass && !aCirc->infinityMass){
								Vr = {(Vr0.x*(mRect/mCirc-1) + Vc0.x*2)/(1+mRect/mCirc), Vr0.y};
								Vc = {(Vc0.x*(mCirc/mRect-1) + Vr0.x*2)/(1+mCirc/mRect), Vc0.y};
							}
							aCirc->v = Vc*n*frame;
							aRect->v = Vr*n*frame;
								
							aCirc->d = Vc0*part + Vc*(1-part);
							aRect->d = Vr0*part + Vr*(1-part);
							aCirc->collision = true;
							aRect->collision = true;
							
							// ������ ������ � ������������
							int eventLength1 = a1.eventHit.size(), eventLength2 = a2.eventHit.size();
							a1.eventHit.push_back(EventHit()), a2.eventHit.push_back(EventHit());
							a1.eventHit[eventLength1].id = a2.id, a2.eventHit[eventLength2].id = a1.id;
							a1.eventHit[eventLength1].typeName = a2.typeName;
							a2.eventHit[eventLength2].typeName = a1.typeName;
							a1.eventHit[eventLength1].className = a2.className, a2.eventHit[eventLength2].className = a1.className;
						}
						
						// ����������� ������ ��� �����!
						else if (partX >= partY){ // �� ������ ����������� ����
							part = partY;
								
							// �������� ��������
							if(aRect->infinityMass && aCirc->infinityMass){
								Vr = {Vr0.x, Vc0.y};
								Vc = {Vc0.x, Vr0.y};
							}
							else if(aRect->infinityMass && !aCirc->infinityMass){
								Vr = Vr0;
								Vc = {Vc0.x, 2*Vr0.y - Vc0.y};
							}
							else if(!aRect->infinityMass && aCirc->infinityMass){
								Vr = {Vr0.x, 2*Vc0.y - Vr0.y};
								Vc = Vc0;
							}
							else{
								Vr = {Vr0.x, (Vr0.y*(mRect/mCirc-1) + Vc0.y*2)/(1+mRect/mCirc)};
								Vc = {Vc0.x, (Vc0.y*(mCirc/mRect-1) + Vr0.y*2)/(1+mCirc/mRect)};
							}
							aCirc->v = Vc*n*frame;
							aRect->v = Vr*n*frame;
								
							aCirc->d = Vc0*part + Vc*(1-part);
							aRect->d = Vr0*part + Vr*(1-part);
							aCirc->collision = true;
							aRect->collision = true;
							
							// ������ ������ � ������������
							int eventLength1 = a1.eventHit.size(), eventLength2 = a2.eventHit.size();
							a1.eventHit.push_back(EventHit()), a2.eventHit.push_back(EventHit());
							a1.eventHit[eventLength1].id = a2.id, a2.eventHit[eventLength2].id = a1.id;
							a1.eventHit[eventLength1].typeName = a2.typeName;
							a2.eventHit[eventLength2].typeName = a1.typeName;
							a1.eventHit[eventLength1].className = a2.className, a2.eventHit[eventLength2].className = a1.className;
						}
					}
					else{
						a1.d = a1.v/n/frame;
						a2.d = a2.v/n/frame;
					}
				}
				
				// ������������ 2 ���������������
				else if (!a1.isCircle && !a2.isCircle){
					                  
					double m1 = a1.m, m2 = a2.m;
					double w1 = a1.size.x, w2 = a2.size.x, h1 = a1.size.y, h2 = a2.size.y; 
					sf::Vector2f c10 = a1.coor, c20 = a2.coor;
					sf::Vector2f v10 = a1.v/n/frame, v20 = a2.v/n/frame;           
					sf::Vector2f v1, v2;  // �������� ��������
					double a, ax = 1, ay = 1;
					
					// ���������, ���� �� ����������
					if(fabs((c10.x + v10.x) - (c20.x + v20.x)) < (w1/2 + w2/2) && fabs((c10.y + v10.y) - (c20.y + v20.y)) < (h1/2 + h2/2)){
	
						/////// �� ����� ��� ����������� ������? ////////////
						// ����� ��� ������
						if( c10.x <= c20.x ){
							ax = (w1/2 + w2/2 + c10.x - c20.x)/(v20.x - v10.x);
						}
						else if( c10.x >= c20.x ){
							ax = (w1/2 + w2/2 + c20.x - c10.x)/(v10.x - v20.x);
						}
						
						// ���� ������ ��� �����
						if( c10.y <= c20.y ){
							ay = (h1/2 + h2/2 + c10.y - c20.y)/(v20.y - v10.y);
						}
						else if( c10.y >= c20.y ){
							ay = (h1/2 + h2/2 + c20.y - c10.y)/(v10.y - v20.y);
						}
						
						// ��������� ��������� ����� ����������
						sf::Vector2f c2x = c20 + v20*ax;
						sf::Vector2f c1x = c10 + v10*ax;
						
						sf::Vector2f c2y = c20 + v20*ay;
						sf::Vector2f c1y = c10 + v10*ay;
						
						// ��� �� ��������� �����������?
						if (!((c2x.y + h2/2 >= c1x.y - h1/2) && (c2x.y - h2/2 <= c1x.y + h1/2))) ax = 2;
						if (!((c2y.x + w2/2 >= c1y.x - w1/2) && (c2y.x - w2/2 <= c1y.x + w1/2))) ay = 2;
						
						// ����������� ����� ��� ������!
						if(ax < ay){
							a = ax;
							
							// �������� ��������
							if(a1.infinityMass && a2.infinityMass && a2.typeName == "Actor"){
								v1 = {v20.x, v10.y};
								v2 = {v10.x, v20.y};
							}
							else if(a1.infinityMass && !a2.infinityMass){
								v1 = v10;
								v2 = {2*v10.x - v20.x, v20.y};
							}
							else if(!a1.infinityMass && a2.infinityMass){
								v1 = {2*v20.x - v10.x, v10.y};
								v2 = v20;
							}
							else if(!a1.infinityMass && !a2.infinityMass){
								v1 = {(v10.x*(m1/m2-1) + v20.x*2)/(1+m1/m2), v10.y};
								v2 = {(v20.x*(m2/m1-1) + v10.x*2)/(1+m2/m1), v20.y};
							}
							a1.v = v1*n*frame;
							a2.v = v2*n*frame;
								
							a1.d = v10*a + v1*(1-a);
							a2.d = v20*a + v2*(1-a);
							a1.collision = true;
							a2.collision = true;
							
							// ������ ������ � ������������
							int eventLength1 = a1.eventHit.size(), eventLength2 = a2.eventHit.size();
							a1.eventHit.push_back(EventHit()), a2.eventHit.push_back(EventHit());
							a1.eventHit[eventLength1].id = a2.id, a2.eventHit[eventLength2].id = a1.id;
							a1.eventHit[eventLength1].typeName = a2.typeName;
							a2.eventHit[eventLength2].typeName = a1.typeName;
							a1.eventHit[eventLength1].className = a2.className, a2.eventHit[eventLength2].className = a1.className;
						}
						
						// ����������� ������ ��� �����!
						else if (ax >= ay){ // �� ������ ����������� ����
							a = ay;
								
							// �������� ��������
							if(a1.infinityMass && a2.infinityMass && a2.typeName == "Actor"){
								v1 = {v10.x, v20.y};
								v2 = {v20.x, v10.y};
							}
							else if(a1.infinityMass && !a2.infinityMass){
								v1 = v10;
								v2 = {v20.x, 2*v10.y - v20.y};
							}
							else if(!a1.infinityMass && a2.infinityMass){
								v1 = {v10.x, 2*v20.y - v10.y};
								v2 = v20;
							}
							else{
								v1 = {v10.x, (v10.y*(m1/m2-1) + v20.y*2)/(1+m1/m2)};
								v2 = {v20.x, (v20.y*(m2/m1-1) + v10.y*2)/(1+m2/m1)};
							}
							a1.v = v1*n*frame;
							a2.v = v2*n*frame;
								
							a1.d = v10*a + v1*(1-a);
							a2.d = v20*a + v2*(1-a);
							a1.collision = true;
							a2.collision = true;
							
							// ������ ������ � ������������
							int eventLength1 = a1.eventHit.size(), eventLength2 = a2.eventHit.size();
							a1.eventHit.push_back(EventHit()), a2.eventHit.push_back(EventHit());
							a1.eventHit[eventLength1].id = a2.id, a2.eventHit[eventLength2].id = a1.id;
							a1.eventHit[eventLength1].typeName = a2.typeName;
							a2.eventHit[eventLength2].typeName = a1.typeName;
							a1.eventHit[eventLength1].className = a2.className, a2.eventHit[eventLength2].className = a1.className;
						}
					}
					else{
						a1.d = a1.v/n/frame;
						a2.d = a2.v/n/frame;
					}
				}
			}
		}
	};
}
