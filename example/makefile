build_DIR=../build

common_SRC=../src/*.cpp
blinkOnOff_SRC=blinkOnOff.cpp
rainbow_SRC=rainbow.cpp
shuttle_SRC=shuttle.cpp
rainbowShuttle_SRC=rainbowShuttle.cpp
list:=$(shell ls | sed -n 's/\.cpp//p')

all:
	@for f in $(list); do\
		$(MAKE) $$f;\
	done

blinkOnOff: target=blinkOnOff
rainbow: target=rainbow
shuttle: target=shuttle
rainbowShuttle: target=rainbowShuttle

$(list): one;

one:
	g++ $($(target)_SRC) $(common_SRC) -o $(build_DIR)/$(target)

clean:
	rm ../build/*