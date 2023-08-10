#include "../include/joypad.h"
#include "../include/tools.h"

Joypad::Joypad(MemoryBus* bus) {
	_bus = bus;
}

void Joypad::Press(Buttons button) {
	_setButtonState(button, true);
	_bus->SetBit(MEM_IF, (uint8)InterFlags::Joypad, 1);
}

void Joypad::Release(Buttons button) {
	_setButtonState(button, false);
}

void Joypad::Write(uint16 address, uint8 value) {
	direction_switch = !checkBit(value, 4);
	button_switch = !checkBit(value, 5);
}

uint8 Joypad::Read(uint16 address) {
	uint8 buttons = 0xFF;

	if (direction_switch) {
		setBit(buttons, 0, !right);
		setBit(buttons, 1, !left);
		setBit(buttons, 2, !up);
		setBit(buttons, 3, !down);
	}

	if (button_switch) {
		setBit(buttons, 0, !a);
		setBit(buttons, 1, !b);
		setBit(buttons, 2, !select);
		setBit(buttons, 3, !start);
	}

	setBit(buttons, 4, !direction_switch);
	setBit(buttons, 5, !button_switch);

	return buttons;
}

void Joypad::_setButtonState(Buttons button, bool press) {
	switch (button)
	{
	case Buttons::Up:
		up = press;
		break;
	case Buttons::Down:
		down = press;
		break;
	case Buttons::Left:
		left = press;
		break;
	case Buttons::Right:
		right = press;
		break;
	case Buttons::A:
		a = press;
		break;
	case Buttons::B:
		b = press;
		break;
	case Buttons::Select:
		select = press;
		break;
	case Buttons::Start:
		start = press;
		break;
	}
}