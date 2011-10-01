#include <stdio.h>
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <strstream.h>
#include <stdlib.h>

// Define sizes of each data type
#define BUS_SIZE 16
#define IO_SIZE 8
#define DATA_SIZE 16
#define ADDR_SIZE 12
#define BIT_SIZE 1

// BUF_SIZE is the size of the base conversion buffers
#define BUF_SIZE 2048

// Integer exponential function
int ipow (int y, int x) {
    int j, ans = 1;

    for(j = 0; j < x; j++) ans *= y;

    return ans;
}

// Define what a bit is
enum bit {off=0, on=1};

// dec2bit converts an integer to an array of bits
int dec2bit (int x, bit *bitarray, int size) {
	int j;

	for(j = 0; j < size; j++) {
		bitarray[j] = (bit)(x & 1);
		x >>= 1;
	}

	return x;
}

// bit2dec converts an array of bits to an integer
int bit2dec (bit *bitarray, int size) {
	int j, x = 0;

	for(j = size - 1; j >= 0; j--) {
		 x <<= 1;
		 x |= bitarray[j];
	}

	return x;
}

// hex returns a pointer to a hex string of an integer
char *hex (bit *bitarray, int size) {
	static char buffer[BUF_SIZE];
	strstream bufstream;
	long int temp, j;

	for(j=0; j<4-(size/4); j++) bufstream << ' ';
	temp = bit2dec(bitarray, size);
	bufstream << setfill('0') << setw(size / 4) << hex << temp;
	bufstream.getline(buffer, BUF_SIZE);

	return buffer;
}

// bin returns a pointer to a binary string of a bit array
char *bin (bit *bitarray, int size) {
	static char buffer[BUF_SIZE];
	strstream bufstream;
	int j;

	for(j=0; j<20-size-((size+3)/4); j++) bufstream << ' ';
	for(j=size-1; j>=0; j--) {
		bufstream << bitarray[j];
		if(!(j%4) && (j!=0)) bufstream << ' ';
	}
	bufstream.getline(buffer, BUF_SIZE);

	return buffer;
}

// reg_class holds the data in the register
class reg_class {
	friend class bus_class;
	friend class mem_class;
public:

	void ld();
	void inr();
	void clr();

	reg_class(int initsize) {
		int j;

		value = new bit[initsize];
		for(j=0; j<initsize; j++) {
			value[j] = off;
		}

		size = initsize;
	}

    void dump();

private:
	bit *value;
	int size;

};

// flag_class is a class for flags
class flag_class {

public:
	void set();
	void clr();
	void toggle();
	void dump();

	flag_class() {
		value = off;
	}

	bit value;
};

// The alu (arithmetic and logic unit) class
class alu_class {
	// Not yet implemented
};

// The memory class is for a single storage location in memory
struct mem_struct {
	bit value[DATA_SIZE];
	bit modified;
};

class mem_class {
public:
	mem_class() {
		int i, j;

		mem = new mem_struct[ipow(2, ADDR_SIZE)];

		for(j = 0; j < ipow(2, ADDR_SIZE); j++) {
			for(i = 0; i < DATA_SIZE; i++) {
				mem[j].value[i] = off;
			}
			mem[j].modified = off;
		}
	}

	void dumpcurrent();
	void dump();

	void write();
	void read();

private:
	mem_struct *mem;
};

// The bus class transfers data to and from the bus
// The data on the bus is not preserved once it is read
class bus_class {
	friend class reg_class;
public:
	bus_class() {
		int j;

		for(j = 0; j < BUS_SIZE; j++) busdata[j] = off;
	}

	void push(bit *businput);
	bit *pop(bit *busoutput);
	void setsignal(bit s0, bit s1, bit s2);

private:
	bit busdata[BUS_SIZE];

	// the signal lines can be used as separate bits or a single value
	union {
		int value;
		struct {
			int s0: 1;
			int s1: 1;
			int s2: 1;
		} s;
	} signal;
};

// this class brings the entire cpu together into one structure
class cpu_class {
	friend class reg_class;
	friend class mem_class;
	friend class bus_class;
public:
	cpu_class(): ar(ADDR_SIZE), pc(ADDR_SIZE), dr(DATA_SIZE),
	             ac(DATA_SIZE), tr(DATA_SIZE), ir(DATA_SIZE),
	             inpr(IO_SIZE), outr(IO_SIZE) {

	    // Set up the signal lines from the bus to/from the registers
		register_set[1] = &ar;
		register_set[2] = &pc;
		register_set[3] = &dr;
		register_set[4] = &ac;
		register_set[5] = &ir;
		register_set[6] = &tr;
    }

    void init(char *file);
    void go();

private:
	bus_class bus;
	mem_class memory;

	reg_class *register_set[7];

	reg_class ar, pc, dr, ac, tr, ir, inpr, outr;

	flag_class i, s;

	flag_class d[3];
};

// bad programming style perhaps, but cpu is global so that all functions can
// access it
cpu_class cpu;

int main() {

    cout << "Loading data file..." << endl;
    cpu.init("temp.dat");
    cout << "Processing program..." << endl;
    cpu.go();

    return 0;
}

// dump dumps the contents of a register
void reg_class::dump() {
	int j, temp;

	cout << hex(value, size) << "\t\t" << bin(value, size) << endl;
}

// ld loads what is on the bus into a register
void reg_class::ld() {
	bit bus_input[BUS_SIZE];	// 16-bit input from the bus
	int j;

	// Get input from the bus
	cpu.bus.pop(bus_input);

	// Copy the bus input to the register
	for(j = 0; j < size; j++)
		value[j] = bus_input[j];
}

// inr increments a register
void reg_class::inr() {
	int j;
	bit x = on;

	for(j = 0; j < size; j++) {
		// Check the contents of the carry flag (x)
		if((value[j] == on) && (x == on)) {
			value[j] = off;
		} else if((value[j] == off) && (x == off)) {
			value[j] = off;
			x = off;
		} else {
			value[j] = on;
			x = off;
		}
	}
}

// clr clears the contents of a register
void reg_class::clr() {
	int j;

	for(j = 0; j < size; j++)
		value[j] = off;
}

// set turns a flag on
void flag_class::set() {
	value = on;
}

// clr turns a flag off
void flag_class::clr() {
	value = off;
}

// toggle toggles the value of a flag
void flag_class::toggle() {
	if(value == on) value = off; else value = on;
}

// dump dumps the value of a flag
void flag_class::dump() {
	cout << "   " << value << endl;
}

// dumpcurrent dumps information about the memory location pointed to by ar
void mem_class::dumpcurrent() {
	int ar_val, j, temp;

	ar_val = bit2dec(cpu.ar.value, cpu.ar.size);
	cout << hex(mem[ar_val].value, DATA_SIZE) << "\t\t" << bin(mem[ar_val].value, DATA_SIZE) << endl;
}

// dump dumps the contents of all modified memory locations
void mem_class::dump() {
	int i, j, temp;

	// Print the header for the memory dump
	cout << "Memory contents:" << endl
	     << "Address" << "\t\t" << "Hex val" << "\t\t" << "Bin val"
             << endl;
	for(j = 0; j < ipow(2, ADDR_SIZE); j++) {
		if(mem[j].modified==on) {
			cout << '[' << hex  << setfill('0') << setw(3) << j
			     << "]\t\t" 
			     << hex(mem[j].value, DATA_SIZE) << "\t\t"
			     << bin(mem[j].value, DATA_SIZE) << endl;
		}
	}
}

// write takes the bus contents and writes it to a memory location
void mem_class::write() {
	bit bus_input[BUS_SIZE];	// 16-bit input from the bus
	int j, address;

	// Get input from the bus
        cpu.bus.pop(bus_input);

	// Get the address
	address = bit2dec(cpu.ar.value, cpu.ar.size);

	// Copy the bus input to memory
	for(j = 0; j < DATA_SIZE; j++)
		mem[address].value[j] = bus_input[j];
	mem[address].modified = on;
}

// read pushes a memory location onto the bus
void mem_class::read() {
	int address;

	// Get the address
	address = bit2dec(cpu.ar.value, cpu.ar.size);

	// Put the value of the memory location on the bus
	cpu.bus.push(mem[address].value);
}

// push takes an array of bits and puts it onto the bus
void bus_class::push(bit *businput) {
	int j;

	for(j = 0; j < BUS_SIZE; j++)
		busdata[j] = businput[j];
	}

// pop pops data off the bus into an array of bits
bit *bus_class::pop(bit *busoutput) {
	int j;

	for(j = 0; j < BUS_SIZE; j++) {
		busoutput[j] = busdata[j];
		busdata[j] = off;		// the data on the bus is not
		}				// preserved

		return(busoutput);
}

// setsignal sets the signal for the bus and loads data from the appropriate
// register onto the bus
void bus_class::setsignal(bit s0, bit s1, bit s2) {
	int j;

	// Set the signal
	signal.s.s0 = s0;
	signal.s.s1 = s1;
	signal.s.s2 = s2;

	// Get a value from a register depending on which signal was set
	for(j = 0; j < cpu.register_set[signal.value]->size; j++)
		busdata[j] = cpu.register_set[signal.value]->value[j];
	for(; j < BUS_SIZE; j++)
		busdata[j] = off;

}

// init() initializes memory
void cpu_class::init(char *file) {
	long int temp=0, j;
	char tempstr[2048];
	bit busline[BUS_SIZE];
	ifstream input (file, ios::in);

	// Initialize ar to 0
	ar.clr();

	// Load the data file
	while(temp != -1) {

		input >> tempstr;	// Get input from the file
		temp = strtol(tempstr, NULL, 16);
		dec2bit(temp, busline, BUS_SIZE);

		bus.push(busline);	// Push file input onto the bus
		memory.write();		// And load it into memory

		ar.inr();		// Move ar to the next memory location
    }
    input.close();
}

void alu_class::and() {
}

void alu_class::add() {
}

// The lda() function loads the contents of dr into ac
void alu_class::lda() {
	int j;

	for(j = 0; j < DATA_SIZE; j++) cpu.ac.value[j] = cpu.dr.value[j];
}

// The cma() function complements ac
void alu_class::cma() {
	int j;

	for(j = 0; j < DATA_SIZE; j++) {
		if(cpu.ac.value[j]==0) {
			cpu.ac.value[j] = 1;
		} else {
			cpu.ac.value[j] = 0;
		}
	}
}

void alu_class::cir() {
}

void alu_class::cil() {
}

void cpu_class::execute() {
}

// The function go() is essentially the control unit
void cpu_class::go() {
	long int temp=0, j;
	bit busline[BUS_SIZE];

	// dump the contents of memory before beginning
	memory.dump();
	cout << endl;

	// Start the program at the beginning 
	pc.clr();

	// Set the s flag to 1 so that we don't halt before we start
	s.set();

	while(s.value == on) {

		// *** instruction fetch ***
		bus.setsignal(off, on, off);		// 010 is for pc
		ar.ld();				// ar <-- pc

		memory.read();				// ir <-- m[ar]
		ir.ld();

		pc.inr();				// pc <-- pc + 1

		// *** decode ***

		bus.setsignal(on, off, on);		// 101 is for ir
		bus.pop(busline);

		for(j=0; j<3; j++)			// d[0]...d[2] <--
			d[j].value = busline[j+12];	// Decode ir(12-14)

		bus.setsignal(on, off, on);		// 101 is for ir
		ar.ld();				// ar <-- ir(0-11)

		i.value = busline[15];			// i <-- ir(15)

		if((d[0].value == on) && (d[1].value == on) && (d[2].value == on)) {
			if(i.value == off) {
				if(busline[11] == on) {
					cout << "CLA";
				} else if(busline[10] == on) {
					cout << "CLE";
				} else if(busline[9] == on) {
					cout << "CMA";
				} else if(busline[8] == on) {
					cout << "CME";
				} else if(busline[7] == on) {
					cout << "CIR";
				} else if(busline[6] == on) {
					cout << "CIL";
				} else if(busline[5] == on) {
					cout << "INC";
				} else if(busline[4] == on) {
					cout << "SPA";
				} else if(busline[3] == on) {
					cout << "SNA";
				} else if(busline[2] == on) {
					cout << "SZA";
				} else if(busline[1] == on) {
					cout << "SZE";
				} else if(busline[0] == on) {
					cout << "HLT";
					s.clr();	// s <-- 0
				}
				cout << " (Class: Branch or accumulator operation)";
			} else {
				if(busline[11] == on) {
					cout << "INP";
				} else if(busline[10] == on) {
					cout << "OUT";
				} else if(busline[9] == on) {
					cout << "SKI";
				} else if(busline[8] == on) {
					cout << "SKO";
				} else if(busline[7] == on) {
					cout << "ION";
				} else if(busline[6] == on) {
					cout << "IOF";
				}
				cout << " (Class: Interrupt or IO operation)";
			}
		} else {
			if(i.value == on) {
				memory.read();		// ar <-- m[ar]
				ar.ld();
				cout << "Indirect ";
			}
			
			if(d[2].value == off) {
				if(d[1].value == off) {
					if(d[0].value == off) {
						cout << "AND";
					} else {
						cout << "ADD";
					}
				} else {
					if(d[0].value == off) {
						cout << "LDA";
					} else {
						cout << "STA";
					}
				}
			} else {
				if(d[1].value == off) {
					if(d[0].value == off) {
						cout << "BUN";
					} else {
						cout << "BSA";
					}
				} else {
					cout << "ISZ";
				}
			}
			cout << " (Class: ALU Operation)";
		}
		cout << endl;

		// *** register dump ***
		cout << "Item    " << '\t' << "Hex val" << "\t\t"
		     << "Bin val" << endl;
		cout << "ar      " << "\t"; ar.dump();
		cout << "m[ar]   " << "\t"; memory.dumpcurrent();
		cout << "pc      " << "\t"; pc.dump();
		cout << "ir      " << "\t"; ir.dump();
		cout << "i       " << "\t"; i.dump();
		cout << "s       " << "\t"; s.dump();
		cout << endl;
	}
}
