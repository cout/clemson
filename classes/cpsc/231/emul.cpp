#include <iostream.h>
#include <fstream.h>

// Define sizes of each data type
#define BUS_SIZE 16
#define IO_SIZE 8
#define DATA_SIZE 16
#define ADDR_SIZE 12

// Define what a bit is
enum bit {off=0, on=1};

int dec2bit (int x, bit *bitarray, int size) {
	int j;

	for(j = 0; j < size; j++) {
		bitarray[j] = (bit)(x & 1);
		x >>= 2;
	}

	return x;
}

int bit2dec (bit *bitarray, int size) {
	int j, x = 0;

	for(j = 0; j < size; j++) x &= bitarray[j];

	return x;
}

// reg_class holds the data in the register
class reg_class {
public:
	init(int s) {
		int j;

		value = new bit[s];

		size = s;
		for(j = 0; j < size; j++) value[j] = off;
	}

	bit *value;
	int size;
};

// ld_class defines the ld (load) control signal
class ld_class : private virtual reg_class {
public:
	void ld() {
		bit bus_input[BUS_SIZE];		// 16-bit input from the bus
		int j;

		// Get input from the bus
		bus.pop(bus_input);

		// Copy the bus input to the register
		for(j = 0; j < size; j++)
			value[j] = bus_input[j];
	}
};

// inr_class defines the inr (increment) control signal
class inr_class : private virtual reg_class {
public:
	void inr() {
		int j;
		bit x = on;

		for(j = 0; j < size; j++) {
			if((value[j] == on) && (x == on)) {
				value[j] = off;
			} else if((value[j] == off) && (x == off)) {
				value[j] = off;
				x = off;
			} else {
				value[j] = on;
			}
		}
	}
};

// clr_class defines the clr (clear) control signal
class clr_class : private virtual reg_class {
public:
	void clr() {
		int j;

		for(j = 0; j < size; j++)
			value[j] = off;
	}
};

// Data registers include dr (data register) and tr (temporary register)
class data_reg : public virtual reg_class, public ld_class, inr_class, clr_class {
public:
	data_reg() {
		init(DATA_SIZE);
	}
};

// Address registers include ar (address register) and pc (program counter)
class addr_reg : public virtual reg_class, public ld_class, inr_class, clr_class {
public:
	addr_reg() {
		init(ADDR_SIZE);
	}
};

// Define a class for the output register (outr)
// Has only a load control signal
class output_reg : public ld_class, virtual reg_class {
public:
	output_reg() {
		init(IO_SIZE);
	}
};

// Define a class for the input register (inpr)
// Has no control signals
class input_reg : virtual reg_class {
friend class alu_class;
public:
	input_reg() {
		init(IO_SIZE);
	}
};

// Define a class for the instruction register (ir)
class instr_reg : public virtual reg_class, public ld_class {
public:
	instr_reg() {
		init(IO_SIZE);
	}
};

struct registers {
	addr_reg ar, pc;
	data_reg dr, ac, tr;
	input_reg inpr;
	output_reg outr;
	instr_reg ir;
};

// The alu (arithmetic and logic unit) class
class alu_class {
	// Not yet implemented
};

// The bus class transfers data to and from the bus
// The data on the bus is not preserved once it is read
class bus_class {
public:

	bus_class(registers &regs) {
		// Set up the signal lines from the bus to/from the registers
		register_set[1] = &regs.ar;
		register_set[2] = &regs.pc;
		register_set[3] = &regs.dr;
		register_set[4] = &regs.ac;
		register_set[5] = &regs.ir;
		register_set[6] = &regs.tr;
	}

	void push(bit *businput) {
		int j;

		for(j = 0; j < BUS_SIZE; j++)
			busdata[j] = businput[j];
	}

	bit *pop(bit *busoutput) {
		int j;

		for(j = 0; j < BUS_SIZE; j++) {
			busoutput[j] = busvalue[j];
			busdata[j] = off;				// the data on the bus is not
		}									// preserved
	}

	void setsignal(bit s0, bit s1, bit s2) {
		int j;

		// Set the signal
		signal.s.s0 = s0;
		signal.s.s1 = s1;
		signal.s.s2 = s2;

		// Get a value from a register depending on which signal was set
		for(j = 0; j < register_set[j]->size; j++)
			busvalue[j] = register_set[j]->value[j];
		for(; j < BUS_SIZE; j++)
			busvalue[j] = 0;
	}

private:
	bit busdata[BUS_SIZE];

	reg_class *register_set[6];

	union {
		int value;
		struct {
			int s0: 1;
			int s1: 2;
			int s2: 3;
		} s;
	} signal;
};

// The memory class is for a single storage location in memory
struct mem_struct {
	bit value[DATA_SIZE];
	bit modified;
};

class mem_class {
public:
	mem_class (addr_reg& real_ar, bus_class& real_bus) {
		ar = real_ar;
		bus = real_bus;
	};

	void write() {
		bit bus_input[BUS_SIZE];		// 16-bit input from the bus
		int j, address;

		// Get input from the bus
		bus.pop(bus_input);

		// Get the address
		address = bit2dec(ar.value, ar.size);

		// Copy the bus input to memory
		for(j = 0; j < size; j++)
			mem[address].value[j] = bus_input[j];
	}

	void read() {
		int address;

		// Get the address
		address = bit2dec(ar.value, ar.size);

		// Put the value of the memory location on the bus
		bus.push(mem[address].value);
	}

private:
	mem_struct mem[2^ADDR_SIZE];
	addr_reg &ar;						// Reference to the real ar
	bus_class &bus;						// Reference to the real bus
};

int main() {
	registers regs;
	bus_class bus(regs);
	mem_class memory(regs.ar, bus);;
	int j, temp;
	bit businput[BUS_SIZE];

	ifstream input ("temp.dat", ios::in);

	while(temp != -1) {

		input >> temp;

		dec2bit(temp, businput, BUS_SIZE);

		bus.push(businput);					// Push file input onto the bus
		memory.write();						// And load it into memory

		regs.ar.inr();
	}
	input.close();

	return 0;
}
