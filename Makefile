all:
	echo "All..."

object:
	gcc -c ./src/Image.c -I ./include -o ./obj/image.o -lm 

compile:
	gcc ./app/app_image.c ./obj/*.o -I ./include -o ./bin/app_image -lm

run:
	./bin/app_image

clean:
	rm -rf ./obj/*
	rm -rf ./bin/*