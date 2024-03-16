#include"Game.h"

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	//TODO: read the configration of the file
	//
	 
	//set up the window
	m_window.create(sf::VideoMode(1280, 720), "2D Game Engine");
	m_window.setFramerateLimit(60);

	spawnPlayer();
}

void Game::run()
{
	while (m_running)
	{
		//TODO: Implement pause functionality

		m_entites.update();

		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sRender();
		sLifeSpan();

		m_currentFrame++;
	}
}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}

void Game::spawnPlayer()
{
	//TODO: Finish adding all the proprties of the player using config values

	//Creating an entity
	auto entity = m_entites.addEntity("Player");

	entity->cTransform =	std::make_shared<CTransform>(Vec2(200, 200), Vec2(0, 0));
	entity->cShape =		std::make_shared<CShape>(32, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4);
	entity->cInput =		std::make_shared<CInput>();

	m_player = entity;
}

void Game::sEnemySpawner()
{
	if (m_currentFrame - m_lastEnemySpawnTime > 100)
	{
		spawnEnemy();
	}

}

void Game::sMovement()
{

	m_player->cTransform->velocity = { 0,0 };

	
	if(m_player->cInput->up)
	{
		m_player->cTransform->velocity.y = -5;
	}
	
	if (m_player->cInput->left)
	{
		m_player->cTransform->velocity.x = -5;
	}

	if (m_player->cInput->right)
	{
		m_player->cTransform->velocity.x = 5;
	}

	if (m_player->cInput->down)
	{
		m_player->cTransform->velocity.y = 5;
	}
	

	//Update the player position every frame
	//m_player->cTransform->position.x += m_player->cTransform->velocity.x;
	//m_player->cTransform->position.y += m_player->cTransform->velocity.y;

	//only enemy movement
	for (auto entity : m_entites.getEntities())
	{
		entity->cTransform->position.x += entity->cTransform->velocity.x;
		entity->cTransform->position.y += entity->cTransform->velocity.y;
	}

}

void Game::sCollision()
{
	for (auto b : m_entites.getEntities("Bullet"))
	{
		for (auto e : m_entites.getEntities("Enemy"))
		{
			Vec2 diffVec(b->cTransform->position - e->cTransform->position);
			float distSqr = diffVec.x * diffVec.x + diffVec.y * diffVec.y;
			float radiusSum = (b->cCollision->radius + e->cCollision->radius) * (b->cCollision->radius + e->cCollision->radius);

			if (distSqr < radiusSum)
			{
				b->desrtoy();
				e->desrtoy();
			}
		}
	}
}

void Game::sLifeSpan()
{
	for (auto b : m_entites.getEntities("Bullet"))
	{
		if (b->cLifeSpan->remaining != 0)
		{
			b->cLifeSpan->remaining--;
		}
		else
		{
			b->desrtoy();
		}
	}
}

void Game::sRender()
{

	m_window.clear();

	

	for (auto e : m_entites.getEntities())
	{
		e->cShape->circle.setPosition(e->cTransform->position.x, e->cTransform->position.y);
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);

		m_window.draw(e->cShape->circle);
	}

	m_window.display();

}

void Game::sUserInput()
{

	sf::Event event;

	while (m_window.pollEvent(event))
	{
		//triggered when closing window
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		//triggered when key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{

			case sf::Keyboard::W:
				std::cout << "W is pressed\n";
				m_player->cInput->up = true;
				break;

			case sf::Keyboard::A:
				std::cout << "A is pressed\n";
				m_player->cInput->left = true;
				break;

			case sf::Keyboard::D:
				std::cout << "D is pressed\n";
				m_player->cInput->right = true;
				break;

			case sf::Keyboard::S:
				std::cout << "S is pressed\n";
				m_player->cInput->down = true;
				break;

			case sf::Keyboard::Space:
				std::cout << "Space is pressed\n";
				m_player->cInput->shoot = true;
				break;

			default:
				std::cout << "Input is not specified\n";
				break;
			}
		}

		//triggers when mouse is clicked
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << "Left mouse clicked\n";
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{

			}

		}

		//triggeres when key is released
		if(event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W is released\n";
				m_player->cInput->up = false;
				break;

			case sf::Keyboard::A:
				std::cout << "A is released\n";
				m_player->cInput->left = false;
				break;

			case sf::Keyboard::D:
				std::cout << "D is released\n";
				m_player->cInput->right = false;
				break;

			case sf::Keyboard::S:
				std::cout << "S is released\n";
				m_player->cInput->down = false;
				break;

			case sf::Keyboard::Space:
				std::cout << "Space is released\n";
				m_player->cInput->shoot = false;
				break;

			default:
				break;
			}
		}
	}

}

void Game::spawnEnemy()
{

	auto entity = m_entites.addEntity("Enemy");

	float ex = rand() % m_window.getSize().x;
	float ey = rand() % m_window.getSize().y;

	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(1, 1));
	entity->cShape = std::make_shared<CShape>(32, 3, sf::Color(0, 0, 255), sf::Color(255, 0, 0), 4);
	entity->cCollision = std::make_shared<CCollision>(40);

	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{

}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	float speed = 10;
	auto bullet = m_entites.addEntity("Bullet");

	Vec2 difVec(mousePos - entity->cTransform->position);
	difVec.Normalize();
	difVec *= speed;

	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->position, difVec, 0);
	bullet->cShape = std::make_shared<CShape>(10, 8, sf::Color(255, 255, 255), sf::Color(255, 0, 0), 3);
	bullet->cCollision = std::make_shared<CCollision>(11);
	bullet->cLifeSpan = std::make_shared<CLifeSpan>(50);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{

}