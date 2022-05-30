LIBS=-lGL -lglfw -lGLEW
HEADERS=allmodels.h  constants.h  cube.h  lodepng.h  model.h  shaderprogram.h  sphere.h  teapot.h  torus.h tiny_obj_loader.h ObjLoader.hpp TextureLoader.hpp config.hpp
FILES=cube.cpp  lodepng.cpp  main_file.cpp  model.cpp  shaderprogram.cpp  sphere.cpp  teapot.cpp  torus.cpp ObjLoader.cpp TextureLoader.cpp

main_file: $(FILES) $(HEADERS)
	g++ -g -o main_file $(FILES) $(LIBS) -I.

.PHONY: clean

clean:
	rm -f main_file