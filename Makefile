default: debug

Filename := main.cpp
Exec := $(basename $(Filename)).exe
debug: $(Exec)
	$^
	del $^

$(Exec): $(Filename)
	g++ -o $@ -D DEBUG $^

