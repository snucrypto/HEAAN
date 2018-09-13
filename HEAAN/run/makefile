# This sample should be run after make libHEAAN.a file in lib folder

# All Target
all: clean HEAAN run

# Tool invocations
HEAAN:
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/usr/local/lib -o "HEAAN" ../lib/libHEAAN.a -lntl -lgmp -lm -std=c++11 -O2 run.cpp -I/../src -pthread
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	rm -rf HEAAN

run:
	./HEAAN