#include <cstdint>
#include <fstream>
#include <chrono>
#include <random>
#include <cstring>
#include <iostream>

#include "./platform.cpp"



#define VF_INDEX 15
#define VIDEO_HEIGHT 64
#define VIDEO_WIDTH 32
#define SPRITE_WIDTH 8
/* Specy bastard can be found here: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#memmap */

/* Instruction (opcode) Translations:
 * nnn = address
 * kk == hex value
 * x = register
 * y = register

 * Example:
 *  3xkk: x = register, kk = hex value
 *  5xy0: x = register, y = register, 0 probably means null.
 
 */

const unsigned int START_ADDRESS = 0x200; // 512
const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50; // 80


class Chip8
{
public:
	/* Interesting note:
	 * The memory for this thing is 4KB. It requires 2 bytes to even count to 4096.
	 */
	uint8_t memory[4096]{};
	/* Registers Start */
	uint8_t registers[16]{};       // General purpose Registers. V[0xF] is a special flag register
	uint16_t index{};              // Index (I) Register
	uint8_t sp{};                  // Stack Pointer
	uint16_t pc{};                 // Program Counter
	uint8_t delayTimer{};
	uint8_t soundTimer{};
	/* Registers End */

	uint8_t keypad[16]{};          // Can 15 keys be pressed at once?
	uint8_t op[16]{};              // Opt Codes
	uint16_t stack[16]{};
	/* ETYMOLOGY OF AN OPCODE
	 * $7522 == 0x7522
	 * 7 is the verb (e.g ADD)
	 * 5 is the register
	 * 22 is the hex value
	 * Interestingly the optcode also represents a location in memory (like a key)
	 */
	uint16_t opcode;                // AKA Current Instruction
	uint32_t video[VIDEO_HEIGHT * VIDEO_WIDTH]{};      // The screen (Og was 62 * 32)

	
	std::default_random_engine randGen;
	std::uniform_int_distribution<uint8_t> randByte;

	Chip8();

	void Cycle();
	void LoadROM(char const * filename);

	/* Opcodes */
	void OP_00E0();   // clear display
	void OP_00EE();   // return from subroutine
	void OP_1nnn();   // jump to location nnn
	void OP_2nnn();   // call subroutine at nnn
	void OP_3xkk();   // skip next instruction if Vx (register) == kk (hex number)
	void OP_4xkk();   // skip next instruction if Vx != kk
	void OP_5xy0();   // skip the next instruction if Vx = Vy
	void OP_6xkk();   // Set Vx = kk
	void OP_7xkk();   // Set Vx = Vx + kk
	void OP_8xy0();   // Set Vx = Vy
	void OP_8xy1();   // Set Vx = Vx OR Vy
	void OP_8xy2();   // Set Vx = Vx AND Vy
	void OP_8xy3();   // Set Vx = Vx XOR Vy
	void OP_8xy4();   // Set Vx = Vx +1, Set VF = carry
	void OP_8xy5();   // Set Vx = Vx - Vy, set VF = NOT borrow
	void OP_8xy6();   // Set Vx = Vx SHR 1
	void OP_8xy7();   // Set Vdx = Vy - Vx, set VF = NOT borrow

	void OP_8xyE();  //  Set Vx = Vx SHL 1
	void OP_9xy0();  // Skip next instruction if Vx != Vy
	void OP_Annn();  // Set I = nnn
	void OP_Bnnn();  // Jump to location nnn + V0
	void OP_Cxkk();  // Set Vx = random byte AND kk
	void OP_Dxyn();  // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
	void OP_Ex9E();  // Skip next instruction if key with the value of Vx is pressed
	void OP_ExA1();  // Skip next instruction if key with the value of Vx is not pressed
	void OP_Fx07();  // Set Vx = delay timer value
	void OP_Fx0A();  // Wait for a key press, store the value of the key in Vx
	void OP_Fx15();  // Set delay timer = Vx
	void OP_Fx18();  // Set sound timer = Vx
	void OP_Fx1E();  // Set I = I + Vx
	void OP_Fx29();  // Set I = location of sprite for digit Vx
	void OP_Fx33();  // SStore BCD representation of Vx in memory locations I, I+1, and I+2
	void OP_Fx55();  // Store registers V0 through Vx in memory starting at location I
	void OP_Fx65();  // Read rigisters V0 through Vx from memory starting at location I

	typedef void (Chip8::*Chip8Func)();
	Chip8Func table[0xF + 1];
	Chip8Func table0[0xE + 1];
	Chip8Func table8[0xE + 1];
	Chip8Func tableE[0xE + 1];
	Chip8Func tableF[0x65 + 1];

	void Table0();
	void Table8();
	void TableE();
	void TableF();
	void OP_NULL();
};



void Chip8::LoadROM(char const * filename)
{
	// Open the file as a stream of binary and move the file pointer to the end
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		// Get the size of the file
		std::streampos size = file.tellg();
		char * buffer = new char[size];

		// Go back to the beginning of the file and fill buffer
		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		// Load rom contents into chip8's memory starting at 0x200
		for (long i = 0; i < size; ++i)
			this->memory[START_ADDRESS + i] = buffer[i];

		// Free buffer
		delete[] buffer;
	}
	
}

/* http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#dispcoords */
uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8()
	// TODO: Ask Jacob what the hell this is doing (and also Ew this is nasty lol)
	:randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
	/* Loading fontset into memory */
	this->pc = START_ADDRESS;
	for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
	{
		this->memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	this->randByte = std::uniform_int_distribution<uint8_t>(0, 255U);



	table[0x0] = &Chip8::Table0;
	table[0x1] = &Chip8::OP_1nnn;
	table[0x2] = &Chip8::OP_2nnn;
	table[0x3] = &Chip8::OP_3xkk;
	table[0x4] = &Chip8::OP_4xkk;
	table[0x5] = &Chip8::OP_5xy0;
	table[0x6] = &Chip8::OP_6xkk;
	table[0x7] = &Chip8::OP_7xkk;
	table[0x8] = &Chip8::Table8;
	table[0x9] = &Chip8::OP_9xy0;
	table[0xA] = &Chip8::OP_Annn;
	table[0xB] = &Chip8::OP_Bnnn;
	table[0xC] = &Chip8::OP_Cxkk;
	table[0xD] = &Chip8::OP_Dxyn;
	table[0xE] = &Chip8::TableE;
	table[0xF] = &Chip8::TableF;

	for (size_t i = 0; i <= 0xE; i++)
	{
		table0[i] = &Chip8::OP_NULL;
		table8[i] = &Chip8::OP_NULL;
		tableE[i] = &Chip8::OP_NULL;
	}

	table0[0x0] = &Chip8::OP_00E0;
	table0[0xE] = &Chip8::OP_00EE;

	table8[0x0] = &Chip8::OP_8xy0;
	table8[0x1] = &Chip8::OP_8xy1;
	table8[0x2] = &Chip8::OP_8xy2;
	table8[0x3] = &Chip8::OP_8xy3;
	table8[0x4] = &Chip8::OP_8xy4;
	table8[0x5] = &Chip8::OP_8xy5;
	table8[0x6] = &Chip8::OP_8xy6;
	table8[0x7] = &Chip8::OP_8xy7;
	table8[0xE] = &Chip8::OP_8xyE;

	tableE[0x1] = &Chip8::OP_ExA1;
	tableE[0xE] = &Chip8::OP_Ex9E;


	for (size_t i = 0; i <= 0x65; i++)
	{
		tableF[0x07] = &Chip8::OP_Fx07;
		tableF[0x0A] = &Chip8::OP_Fx0A;
		tableF[0x15] = &Chip8::OP_Fx15;
		tableF[0x18] = &Chip8::OP_Fx18;
		tableF[0x1E] = &Chip8::OP_Fx1E;
		tableF[0x29] = &Chip8::OP_Fx29;
		tableF[0x33] = &Chip8::OP_Fx33;
		tableF[0x55] = &Chip8::OP_Fx55;
		tableF[0x65] = &Chip8::OP_Fx65;
	}

}

void Chip8::Cycle()
{
	// Fetch
	opcode = (this->memory[this->pc] << 8u) | this->memory[this->pc + 1];

	this->pc +=2;

	uint8_t command = opcode & 0xF000u >> 12u; // E.g. 7222 = 7 which might be the code to ADD
	// Decode and execute
	((*this).*(table[command]))();


	if (this->delayTimer > 0)
		--this->delayTimer;
	if (this->soundTimer > 0)
		--this->soundTimer;
}

/* CLEAR THE DISPLAY */
void Chip8::OP_00E0() {std::memset(this->video, 0, sizeof(video));}

/* RETURN FROM A SUBROUTINE
 * The interpreter sets the program counter to the address at the top of the stack,
 * then subtracts 1 from the stack pointer.
 */
void Chip8::OP_00EE()
{
	--this->sp;
	this->pc = this->stack[this->sp];
}

/* JUMP TO LOCATION NNN (JUMP TO ADDRESS)
 * The interpreter sets the program counter to nnn (address).
 */
void Chip8::OP_1nnn()
{
	/* I think the "u" is there just to specify that it will be unsigned. Not needed.
	 * TODO: I don't understand why opcode is used to find the address. Seems like opcode
	 *  would be the value of an address and not the address itself.
	 */
	uint16_t address = this->opcode & 0x0FFFu;
	this->pc = address;
}

/* CALL SUBROUTINE AT NNN
 * The interpreter increments the stack pointer, then puts the current PC on the top of the stack.
 * The PC is then set to nnn.
 */
void Chip8::OP_2nnn()
{
	uint16_t address = this->opcode & 0x0FFFu;

	/* in the stack (wherever that may be), store the value of
	 * the address of the next instruction
	 */
	this->stack[this->sp] = this->pc;
	/* increment the stack pointer */
        ++this->sp;
	/* set the new next instruction to be the current address */
	this->pc = address;
}

/* SKIP NEXT INSTRUCTION IF Vx = kk
 * Vx = any register
 * kk = any hex number
 * The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
 */
void Chip8::OP_3xkk()
{
	/* Basically sets all zero except for the register and moves it to the
	 * first position.
	 * 0x1761 & 0x0F00 -> 0x07 -> >> 8 -> 0x0007;
	 * NOTE: Vx is the upper register.
	 */
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu; // Returns the hex number (value)from the opcode
	if (registers[Vx] == byte)
		this->pc +=2;
}

/* SKIP NEXT INSTRUCTION IF Vx != kk.
 * The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
 */
void Chip8::OP_4xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x0FFu;

	if (registers[Vx] != byte)
		this->pc +=2;
}

/* SKIP NEXT INSTRUCTION IF Vx = Vy
 * The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
 */
void Chip8::OP_5xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u; 

	if (this->registers[Vx] == this->registers[Vy])
		this->pc+=2;
}

/* SET Vx = kk.
 * The interpreter puts the value kk into register Vx.
 */

void Chip8::OP_6xkk()
{
	
	uint8_t Vx   = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	this->registers[Vx] = byte;
}

/* Started to get the hang of it here lol */
void Chip8::OP_7xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	this->registers[Vx] = Vx + byte;
}

void Chip8::OP_8xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	this->registers[Vx] = this->registers[Vy];
}

void Chip8::OP_8xy1()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	this->registers[Vx] |= this->registers[Vy];
}

void Chip8::OP_8xy2()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	this->registers[Vx] &= this->registers[Vy];
}
void Chip8::OP_8xy3()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	this->registers[Vx] ^= this->registers[Vy];
}

void Chip8::OP_8xy4()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t VF = this->registers[0xF]; // that's why it's called the F register

	uint8_t total = this->registers[Vx] + this->registers[Vy];

	if (total > 0x00FFu) VF = 1;
	else VF = 0;

	uint8_t result = (total & 0x00FFu);
	this->registers[Vx] = result;
}

void Chip8::OP_8xy5()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8;
	uint8_t Vy = (opcode & 0x00F0u) >> 4;

	if (this->registers[Vx] > this->registers[Vy])
		this->registers[0xF] = 1;
	else
		this->registers[0xF] = 0;

	this->registers[Vx] -= this->registers[Vy];
}

void Chip8::OP_8xy6() // Could be wrong
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8;
	uint8_t lsb = this->registers[Vx] & 0x000Fu;

	if (lsb == 1)
		this->registers[0xF] = 1;
	else
		this->registers[0xF] = 0;

	this->registers[Vx] /= 2;
}

void Chip8::OP_8xy7()
{
	
	uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (this->opcode & 0x00F0u) >> 4u;

	if (this->registers[Vy] > this->registers[Vx])
		this->registers[0xF] = 1;
	else
		this->registers[0xF] = 0;
	
	this->registers[Vx] = this->registers[Vy] - this->registers[Vx];
}

void Chip8::OP_8xyE() // Could be wrong
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t msb = this->registers[Vx] & 0x0F00u >> 8u;

	if (msb == 1)
		this->registers[0xF] = 1;
	else
		this->registers[0xF] = 0;
	
	this->registers[Vx] *= 2;
}

void Chip8::OP_9xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (this->registers[Vx] == this->registers[Vy])
		return;

	this->pc +=2;
}

void Chip8::OP_Annn()
{
	this->index = (opcode & 0x0FFFu);
}

void Chip8::OP_Bnnn()
{
	uint8_t address = (opcode & 0x0FFFu);
	uint8_t V0Val = this->registers[0];
	this->pc = V0Val + address;
}

void Chip8::OP_Cxkk()
{
	int rand       =  this->randByte(randGen);
	uint8_t byte   = (opcode & 0x00FFu);
	uint8_t result = rand & byte;

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	this->registers[Vx] = result;
}



/* Black magic as far as I'm concerned
 * Will need to come back to this.
 */
void Chip8::OP_Dxyn()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8;
	uint8_t Vy = (opcode & 0x00F0u) >> 4;
	uint8_t height = opcode & 0x000Fu;     // Height == n. "Display n-byte sprite"

	uint8_t xPos = this->registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = this->registers[Vy] % VIDEO_HEIGHT;

	this->registers[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		/* Should start off at 0,0? */
		uint8_t spriteByte = this->memory[this->index + row];

		for (unsigned int col = 0; col < SPRITE_WIDTH; ++col)
		{
			uint8_t spritePixel    = spriteByte & (0x80u >> col); // Don't know why 0x80u
			uint32_t * screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			if (spritePixel)
			{
				if (*screenPixel == 0xFFFFFFFF)
					this->registers[0xF] =1;

				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void Chip8::OP_Ex9E()
{
	
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t key = this->registers[Vx];

	if (this->keypad[key])
		this->pc +=2;
	
}


void Chip8::OP_ExA1()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t key = this->registers[Vx];

	if (this->keypad[key] == false)
		this->pc +=2;
}

void Chip8::OP_Fx07()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	this->delayTimer = this->registers[Vx];
}

void Chip8::OP_Fx0A()
{
	while (true)
	{
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		if (this->keypad[0])
			this->registers[Vx] = 0;
		else if (this->keypad[1])
			this->registers[Vx] = 1;
		else if (this->keypad[2])
			this->registers[Vx] = 2;
		else if (this->keypad[3])
			this->registers[Vx] = 3;
		else if (this->keypad[4])
			this->registers[Vx] = 4;
		else if (this->keypad[5])
			this->registers[Vx] = 5;
		else if (this->keypad[6])
			this->registers[Vx] = 6;
		else if (this->keypad[7])
			this->registers[Vx] = 7;
		else if (this->keypad[8])
			this->registers[Vx] = 8;
		else if (this->keypad[9])
			this->registers[Vx] = 9;
		else if (this->keypad[10])
			this->registers[Vx] = 10;
		else if (this->keypad[11])
			this->registers[Vx] = 11;
		else if (this->keypad[12])
			this->registers[Vx] = 12;
		else if (this->keypad[13])
			this->registers[Vx] = 13;
		else if (this->keypad[14])
			this->registers[Vx] = 14;
		else if (this->keypad[15])
			this->registers[Vx] = 15;
		else
			this->pc -=2;
	}
}

void Chip8::OP_Fx15()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	this->delayTimer = this->registers[Vx];
}

void Chip8::OP_Fx18()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	this->soundTimer = this->registers[Vx];
}

void Chip8::OP_Fx1E()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	this->index += this->registers[Vx];
}

void Chip8::OP_Fx29()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = this->registers[Vx];
	this->index = FONTSET_START_ADDRESS + (5 * digit); // Why 5 * ?
}

void Chip8::OP_Fx33()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = this->registers[Vx];

	this->memory[this->index + 2] = value % 10;
	value /=10;

	this->memory[this->index + 1] = value %10;
	value /=10;
	
	this->memory[this->index] = value % 10;
}

void Chip8::OP_Fx55()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	
	for (unsigned int i = 0; i < Vx; i++)
		this->memory[this->index + i] = this->registers[i];
}

void Chip8::OP_Fx65()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	
	for (unsigned int i = 0; i < Vx; i++)
		this->registers[i] = this->memory[this->index + i];
}

void Chip8::Table0()
{
	((*this).*(table0[opcode & 0x000Fu]))();
	
}

void Chip8::Table8()
{
	((*this).*(table8[opcode & 0x000Fu]))();
	
}

void Chip8::TableE()
{
	((*this).*(tableE[opcode & 0x000Fu]))();
	
}

void Chip8::TableF()
{
	((*this).*(tableF[opcode & 0x000Fu]))();
}

void Chip8::OP_NULL()
{
	
}

int main(int argc, char ** argv)
{
if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

	int videoScale = std::stoi(argv[1]);
	int cycleDelay = std::stoi(argv[2]);
	char const* romFilename = argv[3];

	Platform platform("CHIP-8 Emulator", VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

	Chip8 chip8;
	chip8.LoadROM(romFilename);

	int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit)
	{
		quit = platform.ProcessInput(chip8.keypad);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (dt > cycleDelay)
		{
			lastCycleTime = currentTime;

			chip8.Cycle();

			platform.Update(chip8.video, videoPitch);
		}
	}
	return 0;
}    
	
