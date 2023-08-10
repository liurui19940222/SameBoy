#pragma once

#include "memory.h"

enum class Buttons {
	Up,
	Down,
	Left,
	Right,
	A,
	B,
	Select,
	Start
};

class Joypad : public IODevice
{
public:
	virtual ~Joypad(){}

	Joypad(MemoryBus* bus);

	void Press(Buttons button);

	void Release(Buttons button);

	void Write(uint16 address, uint8 value) override;

	uint8 Read(uint16 address) override;

private:
	MemoryBus* _bus;

	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool a = false;
	bool b = false;
	bool select = false;
	bool start = false;
	bool button_switch = false;
	bool direction_switch = false;

	void _setButtonState(Buttons button, bool press);
};

