SRCFILES := $(wildcard src/*.c)
HFILES := $(wildcard src/*.h)

all: cachesim

cachesim: $(SRCFILES) $(HFILES)
	gcc -Wall -g -o cachesim $(SRCFILES) -lm

submission: cachesim
	./bin/makesubmission.sh

grade: cachesim
	./bin/run_grader.py

clean:
	rm -rfv test_results cachesim *-project2.tar.gz

.PHONY: all submission clean grade grade-full
