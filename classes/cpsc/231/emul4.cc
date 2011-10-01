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

// bittotal() counts the number of bits set to 1
int bittotal (bit *bitarray, int size) {
   	int x = 0, j;
        
        for(j = 0; j < size; j++) x +=bitarray[j];
        
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
	friend class alu_class;
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
	void dumpdec();

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
public:
	void and();
	void add();
	void lda();
	void cle();
	void cma();
	void cme();
	void cir();
	void cil();
	void sze();
	void dumpe();
	void inp();
private:
   	flag_class e;
	// Not yet implemented
};

// The memory class is for a single storage location in memory
struct mem_struct {
	bit value[DATA_SIZE];
	bit modified;
        int detail;
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

	int get_detail();
	void set_detail(int detail);

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

		outputnum = 0;
	}

	void push(bit *businput);
	bit *pop(bit *busoutput);
	void setsignal(bit s0, bit s1, bit s2);
	void checkinput();
	void outputchar();
	void dispoutputbuf();

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

	int outputbuf[2048];
	int outputnum;
};

// this class brings the entire cpu together into one structure
class cpu_class {
	friend class reg_class;
	friend class mem_class;
	friend class bus_class;
	friend class alu_class;
public:
	cpu_class(): ar(ADDR_SIZE), pc(ADDR_SIZE), dr(DATA_SIZE),
	             ac(DATA_SIZE), tr(DATA_SIZE), ir(DATA_SIZE),
	             inpr(IO_SIZE), outr(IO_SIZE), clock(DATA_SIZE) {

	    // Set up the signal lines from the bus to/from the registers
		register_set[1] = &ar;
		register_set[2] = &pc;
		register_set[3] = &dr;
		register_set[4] = &ac;
		register_set[5] = &ir;
		register_set[6] = &tr;
    }

	void execute_regref(bit busline[], int detail);
	void execute_memref(bit busline[], int detail);
	void execute_io(bit busline[], int detail);

	void init(char *file);
	void go();

private:
	bus_class bus;
	mem_class memory;
	alu_class alu;

	reg_class *register_set[7];

	reg_class ar, pc, dr, ac, tr, ir, inpr, outr, clock;

	flag_class i, s;

	flag_class d[3];

	flag_class ien, fgi, fgo;
};

// bad programming style perhaps, but cpu is global so that all functions can
// access it
cpu_class cpu;

int main(int argc, char *argv[]) {

    cout << "Loading data file..." << endl;
    cpu.init(argv[1]);
    cout << "Processing program..." << endl;
    cpu.go();

    return 0;
}

// dump dumps the contents of a register
void reg_class::dump() {
	int j, temp;

	cout << hex(value, size) << "\t\t" << bin(value, size) << endl;
}

// dumpdec dumps the decimal version of a register
void reg_class::dumpdec() {
	int temp;

	temp = bit2dec(value, size);
	cout << dec << temp << '\n';
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

// get_detail() gets the detail portion of the memory at ar
int mem_class::get_detail() {
   	int address;

   	// Get the address
        address = bit2dec(cpu.ar.value, cpu.ar.size);
        
        // Return the detail portion
        return(mem[address].detail);
}

// set_detail sets the detail portion of the memory at ar
void mem_class::set_detail(int detail) {
   	int address;

   	// Get the address
        address = bit2dec(cpu.ar.value, cpu.ar.size);
        
        // Set the detail portion
        mem[address].detail = detail;
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

void bus_class::checkinput() {
	int clockval, temp = 0;
	static int curintr = 0;

	clockval = bit2dec(cpu.clock.value, DATA_SIZE);
	if((curintr == 0) && (clockval >= 27)) {
		temp = 0xA6;
	} else if((curintr == 1) && (clockval >= 103)) {
		temp = 0x3F;
	} else if((curintr == 2) && (clockval >= 142)) {
		temp = 0x29;
	}
	if(temp) {
		dec2bit(temp, cpu.inpr.value, IO_SIZE);
		cpu.fgi.set();
		curintr++;
	}
}	

void bus_class::outputchar() {
	outputbuf[outputnum++] = bit2dec(cpu.outr.value, IO_SIZE);
}

void bus_class::dispoutputbuf() {
	int j;

	for(j = 0; j < outputnum; j++)
		cout << hex << outputbuf[j] << " ";
	cout << endl;
}

// The and() function ands ac and dr and puts the result in ac
void alu_class::and() {
	int j;

	for(j = 0; j < DATA_SIZE; j++)
		cpu.ac.value[j] = (bit)((int)cpu.ac.value[j] & (int)cpu.dr.value[j]);
}

// The add() function adds ac and dr, puts the result in ac, and puts the carry
// bit into e
void alu_class::add() {
	int j;

	e.clr();
	for(j = 0; j < DATA_SIZE; j++) {
		switch((int)cpu.ac.value[j] + (int)e.value) {
			case 2:
				e.set();
				cpu.ac.value[j] = off;
				break;
			case 1:
				e.clr();
				cpu.ac.value[j] = on;
				break;
			default:
				e.clr();
				cpu.ac.value[j] = off;
		}

		switch((int)cpu.ac.value[j] + (int)cpu.dr.value[j]) {
			case 2:
				e.set();
				cpu.ac.value[j] = off;
				break;
			case 1:
				cpu.ac.value[j] = on;
				break;
			default:
				cpu.ac.value[j] = off;
		}
	}
}

// The lda() function loads the contents of dr into ac
void alu_class::lda() {
	int j;

	for(j = 0; j < DATA_SIZE; j++)			// ac <-- dr
           	cpu.ac.value[j] = cpu.dr.value[j];
}

// The cle() function clears e
void alu_class::cle() {
   	e.clr();
}

// The cma() function complements ac
void alu_class::cma() {
	int j;

	for(j = 0; j < DATA_SIZE; j++) {		// ac <-- !ac
		if(cpu.ac.value[j]==0) {
			cpu.ac.value[j] = on;
		} else {
			cpu.ac.value[j] = off;
		}
	}
}

// The cme() function toggle (complements) e
void alu_class::cme() {
   	e.toggle();				// e <-- !e
}

void alu_class::cir() {
	int j;					// No way to do this
	bit tmp;				// without a temp var

	tmp = e.value;
	e.value = cpu.ac.value[0];		// e <-- ac[0]

	for(j = 0; j < DATA_SIZE - 1; j++)	// ac <-- shr ac
		cpu.ac.value[j] = cpu.ac.value[j + 1];

	cpu.ac.value[DATA_SIZE - 1] = tmp;	// ac[15] <-- e
}

void alu_class::cil() {
	int j;					// No way to do this
	bit tmp;				// without a temp var

	tmp = e.value;
	e.value = cpu.ac.value[DATA_SIZE - 1];	// e <-- ac[15]

	for(j = DATA_SIZE - 1; j > 0; j--)	// ac <-- shl ac
		cpu.ac.value[j] = cpu.ac.value[j - 1];
	
	cpu.ac.value[0] = tmp;			// ac[0] <-- e
}

// sze increments pc if e is 0
void alu_class::sze() {
	if(e.value==off)			// if e == 0 then
	cpu.pc.inr();				// pc <-- pc + 1
}

// dumpe dumps the contents of e
void alu_class::dumpe() {
   	e.dump();
}

// inp copies inpr into the lower 8 bits of ac
void alu_class::inp() {
	int j;

	for(j = 0; j < IO_SIZE; j++) cpu.ac.value[j] = cpu.inpr.value[j];
}

// init() initializes memory
void cpu_class::init(char *file) {
	long int temp=0, j;
	char tempstr[2048];
	bit busline[BUS_SIZE];
	ifstream input (file, ios::in);

	// Initialize pc
	input >> tempstr;
	input >> tempstr;
	temp = strtol(tempstr, NULL, 16);
	dec2bit(temp, busline, BUS_SIZE);
	bus.push(busline);
	pc.ld();

	// Load the data file
	while(!input.eof()) {

		input >> tempstr;	// Get input from the file
		temp = strtol(tempstr, NULL, 16);
		dec2bit(temp, busline, BUS_SIZE);
		cout << tempstr << "\t";

		bus.push(busline);	// Push file input onto the bus
		ar.ld();		// And copy it into ar

		input >> tempstr;	// Get input from the file
		temp = strtol(tempstr, NULL, 16);
		dec2bit(temp, busline, BUS_SIZE);
		cout << tempstr << "\t";

		bus.push(busline);	// Push file input onto the bus
		memory.write();		// And write it to memory
                
                input >> tempstr;	// Get input from the file
                temp = strtol(tempstr, NULL, 16);
                memory.set_detail(temp);
		cout << tempstr << "\t\n";
    }
    input.close();
}

// execute_regref() executes a register reference function
void cpu_class::execute_regref(bit busline[], int detail) {
   	if(busline[11] == on) {
           	if(detail) cout << "CLA";
                ac.clr();				// ac <-- 0
		clock.inr();
        } else if(busline[10] == on) {
           	if(detail) cout << "CLE";
                alu.cle();				// clear e
		clock.inr();
        } else if(busline[9] == on) {
           	if(detail) cout << "CMA";
                alu.cma();				// complement ac
		clock.inr();
	} else if(busline[8] == on) {
		if(detail) cout << "CME";
                alu.cme();				// complement e
		clock.inr();
	} else if(busline[7] == on) {
		if(detail) cout << "CIR";
                alu.cir();				// circulate right
		clock.inr();
	} else if(busline[6] == on) {
		if(detail) cout << "CIL";
                alu.cil();				// circulate left
		clock.inr();
	} else if(busline[5] == on) {
		if(detail) cout << "INC";
                ac.inr();				// ac <-- ac + 1
		clock.inr();
	} else if(busline[4] == on) {
		if(detail) cout << "SPA";
                bus.setsignal(off, off, on);		// 100 is for ac
                bus.pop(busline);
                if(busline[15]==off)			// if ac[15] == 0
                   	pc.inr();			// then pc <-- pc + 1
		clock.inr();
	} else if(busline[3] == on) {
		if(detail) cout << "SNA";
                bus.setsignal(off, off, on);		// 100 is for ac
                bus.pop(busline);
                if(busline[15]==on)			// if ac[15] == 1
                   	pc.inr();			// then pc <-- pc + 1
		clock.inr();
	} else if(busline[2] == on) {
		if(detail) cout << "SZA";
                bus.setsignal(off, off, on);		// 100 is for ac
                bus.pop(busline);
                if(bittotal(busline, BUS_SIZE)==0)	// if ac == 0
                   	pc.inr();			// then pc <-- pc + 1
		clock.inr();
	} else if(busline[1] == on) {
		if(detail) cout << "SZE";
                alu.sze();				// skip if e is 0
		clock.inr();
	} else if(busline[0] == on) {
		if(detail) cout << "HLT";
		s.clr();				// s <-- 0
		clock.inr();
	}
}

// execute_io() executes an io operation 
void cpu_class::execute_io(bit busline[], int detail) {
	if(busline[11] == on) {
		if(detail) cout << "INP";
		alu.inp();				// ac[0..7] <-- inpr
		fgi.clr();				// fgi <-- 0
		clock.inr();
	} else if(busline[10] == on) {
		if(detail) cout << "OUT";
		bus.setsignal(off, off, on);		// 100 is for ac
		outr.ld();				// outr <-- ac[0..7]
		fgo.clr();				// fgo <-- 0
		bus.outputchar();
		clock.inr();
	} else if(busline[9] == on) {
		if(detail) cout << "SKI";
		if(fgi.value) pc.inr();			// if(fgi=1) inr pc
		clock.inr();
	} else if(busline[8] == on) {
		if(detail) cout << "SKO";
		if(fgo.value) pc.inr();			// if(fgo=1) inr pc
		clock.inr();
	} else if(busline[7] == on) {
		if(detail) cout << "ION";
		ien.set();				// ien <-- 1
		clock.inr();
	} else if(busline[6] == on) {
		if(detail) cout << "IOF";
		ien.clr();				// ien <-- 0
		clock.inr();
	} else if(busline[0] == on) {
		if(detail) cout << "RTI";
		ar.clr();				// ar <-- 0
		clock.inr();
		ien.set();				// ien <-- 0
		memory.read();				// pc <-- m[ar]
		pc.ld();
		clock.inr();
	}		
}

// execute_memref() exectutes a memory-reference instruction
void cpu_class::execute_memref(bit busline[], int detail) {
	if(d[2].value == off) {
           	if(d[1].value == off) {
                   	if(d[0].value == off) {
                           	if(detail) cout << "AND";
				memory.read();
				dr.ld();		// dr <-- m[ar]
				clock.inr();
				alu.and();
				clock.inr();
                        } else {
                           	if(detail) cout << "ADD";
				memory.read();
				dr.ld();		// dr <-- m[ar]
				clock.inr();
				alu.add();
				clock.inr();
                        }
                } else {
                   	if(d[0].value == off) {
                           	if(detail) cout << "LDA";
                                memory.read();		// dr <-- m[ar]
                                dr.ld();
				clock.inr();
                                alu.lda();		// ac <-- dr
				clock.inr();
                        } else {
                           	if(detail) cout << "STA";
                                bus.setsignal(off, off, on);
							// 100 is for ac
				memory.write();
				clock.inr();
                        }
                }
        } else {
       	   	if(d[1].value == off) {
			if(d[0].value == off) {
				if(detail) cout << "BUN";
                                bus.setsignal(on, off, off);
							// 001 is for ar
                                pc.ld();		// pc <-- ar
				clock.inr();
			} else {
				if(detail) cout << "BSA";
                                bus.setsignal(off, on, off);
							// 010 is for pc
                                memory.write();		// m[ar] <-- pc
                                ar.inr();		// ar <-- ar + 1
				clock.inr();
                                bus.setsignal(on, off, off);
							// 001 is for ar
                                pc.ld();		// pc <-- ar
				clock.inr();
			}
		} else {
			if(detail) cout << "ISZ";
                        memory.read();
                        dr.ld();			// dr <-- m[ar]
			clock.inr();
                        dr.inr();			// dr <-- dr + 1
			clock.inr();
                        bus.setsignal(on, on, off);	// 011 is for dr
                        memory.write();			// m[ar] <-- dr
                        bus.setsignal(on, on, off);	// 011 is for dr
                        bus.pop(busline);
                        if(bittotal(busline, BUS_SIZE)==0)	// if dr == 0
                           	pc.inr();		// then pc <-- pc + 1
			clock.inr();
		}
        }
}

// The function go() is essentially the control unit
void cpu_class::go() {
	long int temp=0, j;
        int detail;
	bit busline[BUS_SIZE];

	// dump the contents of memory before beginning
	memory.dump();
	cout << endl;

	// Set the s flag to 1 so that we don't halt before we start
	s.set();

	while(s.value == on) {

		cout << "Clock cycle: ";
		clock.dumpdec();

		// *** instruction fetch ***
		bus.setsignal(off, on, off);		// 010 is for pc
		ar.ld();				// ar <-- pc
		clock.inr();

		memory.read();				// ir <-- m[ar]
		ir.ld();

        	detail = memory.get_detail();

		pc.inr();				// pc <-- pc + 1
		clock.inr();

		// *** decode ***

		bus.setsignal(on, off, on);		// 101 is for ir
		bus.pop(busline);

		for(j=0; j<3; j++)			// d[0]...d[2] <--
			d[j].value = busline[j+12];	// Decode ir(12-14)

		bus.setsignal(on, off, on);		// 101 is for ir
		ar.ld();				// ar <-- ir(0-11)

		i.value = busline[15];			// i <-- ir(15)
		clock.inr();

		if((d[0].value == on) && (d[1].value == on) && (d[2].value == on)) {
			if(i.value == off) {
                           	execute_regref(busline, detail);
				if(detail) cout << " (Class: Register reference operation)";
			} else {
                           	execute_io(busline, detail);
				if(detail) cout << " (Class: Interrupt or IO operation)";
			}
		} else {
			if(i.value == on) {
				memory.read();		// ar <-- m[ar]
				ar.ld();
				if(detail) cout << "Indirect ";
				clock.inr();
			}
			
                        execute_memref(busline, detail);
			if(detail) cout << " (Class: Memory-reference operation)";
		}
		if(detail) cout << endl;

		// *** register dump ***
		if(detail >= 2) {
                   	cout << "Item    \t" << "Hex val" << "\t\t"
                             << "Bin val" << endl;
			cout << "pc      \t"; pc.dump();
			cout << "ar      \t"; ar.dump();
                        cout << "m[ar]   \t"; memory.dumpcurrent();
			cout << "ac      \t"; ac.dump();
			cout << "dr      \t"; dr.dump();
                        cout << "ir      \t"; ir.dump();
                        cout << "i       \t"; i.dump();
                        cout << "s       \t"; s.dump();
                        cout << "e       \t"; alu.dumpe();
			cout << "inpr    \t"; inpr.dump();
			cout << "outr    \t"; outr.dump();
			cout << "fgi     \t"; fgi.dump();
			cout << "fgo     \t"; fgo.dump();
			cout << "ien     \t"; ien.dump();
                }
                
                if(detail >=3) memory.dump();

		cout << endl;

		// *** check for interrupts ***

		bus.checkinput();

		if((ien.value) & (fgi.value | fgo.value)) {
			cout << "Interrupt at clock cycle ";
			clock.dump();
			cout << endl;
			ar.clr();			// ar <-- 0
			bus.setsignal(off, on , off);	// 010 is for pc
			tr.ld();			// tr <-- pc
			clock.inr();
			bus.setsignal(off, on, on);	// 110 is for tr
			memory.write();			// m[ar] <-- tr
			pc.clr();			// pc <-- 0
			clock.inr();
			pc.inr();			// pc <-- pc + 1
			ien.clr();			// ien <-- 0
			clock.inr();
		}
	}

	cout << "Simulation complete." << endl;
	cout << "Total clock cycles: ";
	clock.dumpdec();
	cout << "Program output: ";
	bus.dispoutputbuf();
}
