CXXFLAGS := -std=c++11 -g -Wall -Wextra -Wpedantic
# -Wl,-subsystem,windows for no cmd prompt
INCLUDES := -Iinclude

LIBS := -lmingw32 -lSDL2main -lSDL2 -llua54 -lfreetype
OBJECTS := glad.o\
	Input.o\
	Random.o\
	Vector2.o\
	TextureManager.o\
	Renderer.o\
	Sprite.o\
	Player.o\
	EnemyBehaviours.o\
	ConcreteEnemyStates.o\
	Enemy.o\
	EnemyManager.o\
	Map.o\
	Game.o\
	main.o

# -ldl flag needed for linux i think
# -lmingw32 -lSDL2main -lSDL2 -llua54 -lfreetype needed for windows
# just -lSDL2 -llua for mac i think

2dgame: $(OBJECTS)
	g++ $^ -o test $(LIBS)

glad.o: src/glad.c
	gcc $(INCLUDES) -c $^ -o $@

%.o: src/%.cpp
	g++ $(INCLUDES) $(CXXFLAGS) -c $^ -o $@ $(LIBS)

.PHONY: clean
clean:
	-rm -f test.exe $(OBJECTS)