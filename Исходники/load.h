namespace ye{

	//=============================================================
	//    Добавим действия операторов и функции для векторов SFML
	//=============================================================

	// операторы
	double operator * (sf::Vector2f v2_1, sf::Vector2f v2_2){
		return (v2_1.x*v2_2.x + v2_1.y*v2_2.y);
	}
	
	sf::Vector2f operator * (sf::Vector2f v2, double f){
		v2.x = v2.x*f;
		v2.y = v2.y*f;
		return v2;
	}
	
	sf::Vector2f operator * (double f, sf::Vector2f v2){
		v2.x = v2.x*f;
		v2.y = v2.y*f;
		return v2;
	}
	
	sf::Vector2f operator / (sf::Vector2f v2, double f){
		v2.x = v2.x/f;
		v2.y = v2.y/f;
		return v2;
	}
	
	// Функции
	double vectorLength(sf::Vector2f v2){
		return sqrt(v2.x*v2.x + v2.y*v2.y);
	}
	
	sf::Vector2f normalized(sf::Vector2f v2){
		if(vectorLength(v2) > 0) return v2/sqrt(v2.x*v2.x + v2.y*v2.y);
		else return {0,1};
	}
	
	//=============================================================
	//                 Загрузки файлов из папок
	//=============================================================
	
	// Для шрифтов
	std::vector<sf::Font> font;
	
	// Для текстур
	std::vector<sf::Texture> textures;
	sf::Texture globalTexture;
}
