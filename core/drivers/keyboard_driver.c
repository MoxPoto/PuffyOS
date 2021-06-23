#include <memory_util.h>
#include <drivers/keyboard_driver.h>
#include <pic.h>
#include <early_terminal.h>
#include <ports.h>

#define KEY_INSTRUCTION(index, pressedd, scancoded, asciid) (keyboard_instructions + index)->ascii = asciid; (keyboard_instructions + index)->pressed = pressedd; (keyboard_instructions + index)->scancode = scancoded;
struct Key* keyboard_array;
struct KeyInstruction* keyboard_instructions;

void c_key_handle(void) {
	unsigned char scan_code = inportb(0x60);
	// fun fact: the keyboard doesnt continue sending any more interrupts until you read the scan
	// code)
	
    struct KeyInstruction* instruction = NULL;

    for (int i = 0; i < 128; i++) {
        struct KeyInstruction* instr = (keyboard_instructions + i);

        if (instr->scancode == scan_code) {
            instruction = instr;
            break;
        }
    }

    if (instruction != NULL) {
        for (int i = 0; i < 128; i++) {
            struct Key* thisKey = (keyboard_array + i);

            if (thisKey->ascii == instruction->ascii) {
                thisKey->pressed = instruction->pressed;
                break;
            }
        }
        
        terminal_writestring("Keyboard sent a: ");
        char output[3];
        output[0] = instruction->ascii;
        output[1] = '\n';
        output[2] = '\0';

        terminal_write(output, sizeof(output));
    }
    
	PIC_sendEOI(1);
	
	return;
}

void initialize_keyboard(void) {
    keyboard_array = malloc(sizeof(struct Key) * 128);
    keyboard_instructions = malloc(sizeof(struct KeyInstruction) * 128);

    for (int i = 0; i < 128; i++) {
        struct Key* thisKey = (keyboard_array + i);

        // Clear things out.. malloc doesn't do it for us
        thisKey->ascii = (unsigned char)i;
        thisKey->pressed = false;
    }

    // scan code mapping

    KEY_INSTRUCTION(0, true, 0x04, '3');   
    KEY_INSTRUCTION(1, true, 0x08, '7');
    KEY_INSTRUCTION(2, true, 0x0C, '-');
    KEY_INSTRUCTION(3, true, 0x10, 'Q');
 
}
