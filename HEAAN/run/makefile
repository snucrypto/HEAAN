# This sample should be run after make libHEAAN.a file in lib folder

# All Target
all: clean HEAAN run

# Tool invocations
HEAAN:
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/usr/local/lib -o "HEAAN" ../lib/libHEAAN.a run.cpp -I/../src -lgmp -lntl -lm -O2 -std=c++11
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	rm -rf HEAAN

run:
	./HEAAN