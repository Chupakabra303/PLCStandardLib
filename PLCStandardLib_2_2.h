/*
 * PLCStandardLib_2_2.h
 */

#ifndef PLCSTANDARDLIB_2_2_H_
#define PLCSTANDARDLIB_2_2_H_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

//#ifdef VAR_GLOBAL_TIME
//extern unsigned long VAR_GLOBAL_TIME;
//#else
//#define VAR_GLOBAL_TIME millis()
//#endif

extern unsigned long VAR_GLOBAL_TIME;

// ------------------- TON -------------------
class TON
{
public:
	TON();
	TON(unsigned long PT);
	bool Run(bool IN);
	bool Q; // выходная переменная
	bool IN; // входная переменная
	unsigned long PT; // входная переменная
	unsigned long ET; // выходная переменная - текущее значение таймера
private:
	bool _M; // внутренний флаг
	unsigned long _StartTime;
};

// ------------------- TOF -------------------
class TOF
{
public:
	TOF();
	TOF(unsigned long PT);
	bool Run(bool IN);
	bool Q; // выходная переменная
	bool IN; // входная переменная
	unsigned long PT; // входная переменная
	unsigned long ET; // выходная переменная - текущее значение таймера
private:
	bool _M; // внутренний флаг
	unsigned long _StartTime;
};

// ------------------- R_TRIG -------------------
class R_TRIG // детектор фронта сигнала
{
public:
	R_TRIG();
	bool Run(bool CLK);
	bool CLK; // входная переменная
	bool Q; // выходная переменная
private:
	bool _M; // внутренний флаг
};

// ------------------- F_TRIG -------------------
class F_TRIG // детектор спада сигнала
{
public:
	F_TRIG();
	bool Run(bool CLK);
	bool CLK; // входная переменная
	bool Q; // выходная переменная
private:
	bool _M; // внутренний флаг
};

// ------------------- RS_TRIG -------------------
class RS_TRIG
{
public:
	RS_TRIG();
	bool Run();
	bool Run(bool SET, bool RESET);
	bool SET; // установка триггера
	bool RESET; // сброс триггера
	bool Q; // выходная переменная
//private:
};

// ------------------- SR_TRIG -------------------
class SR_TRIG
{
public:
	SR_TRIG();
	bool Run();
	bool Run(bool SET, bool RESET);
	bool SET; // установка триггера
	bool RESET; // сброс триггера
	bool Q; // выходная переменная
//private:
};

// ------------------- TON -------------------
TON::TON()
{
	IN = false;
	PT = 0;
	_M = false;
	_StartTime = 0;
	Q = false;
	ET = 0;
}
TON::TON(unsigned long PT)
{
	IN = false;
	TON::PT = PT;
	_M = false;
	_StartTime = 0;
	Q = false;
	ET = 0;
}

bool TON::Run(bool IN)
{
	TON::IN = IN;
	if (!TON::IN)
	{
		Q = false;
		ET = 0;
		_M = false;
	}
	else
	{
		if (!_M)
		{
			_M = true; // сбрасываем флаг М
			_StartTime = VAR_GLOBAL_TIME;
			// ET = 0; // сразу = 0
		}
		else
		{
			if (!Q)
				ET = VAR_GLOBAL_TIME - _StartTime; // вычисляем время
		}
		if (ET >= PT)
			Q = true;
	}
	return Q;
}

// ------------------- TOF -------------------
TOF::TOF()
{
	IN = false;
	PT = 0;
	_M = false;
	_StartTime = 0;
	Q = false;
	ET = 0;
}

TOF::TOF(unsigned long PT)
{
	IN = false;
	TOF::PT = PT;
	_M = false;
	_StartTime = 0;
	Q = false;
	ET = 0;
}

bool TOF::Run(bool IN)
{
	TOF::IN = IN;
	if (TOF::IN)
	{
		Q = true;
		ET = 0;
		_M = true;
	}
	else
	{
		if (_M)
		{
			_M = false; // сбрасываем флаг М
			_StartTime = VAR_GLOBAL_TIME;
			// ET = 0; // сразу = 0
		}
		else
		{
			if (Q)
				ET = VAR_GLOBAL_TIME - _StartTime; // вычисляем время
		}
		if (ET >= PT)
			Q = false;
	}
	return Q;
}

// ------------------- R_TRIG -------------------
R_TRIG::R_TRIG()
{
	CLK = false;
	_M = false;
	Q = false;
}

bool R_TRIG::Run(bool CLK)
{
	R_TRIG::CLK = CLK;
	Q = R_TRIG::CLK && !_M;
	_M = R_TRIG::CLK;
	return Q;
}

// ------------------- F_TRIG -------------------
F_TRIG::F_TRIG()
{
	CLK = false;
	_M = true;
	Q = false;
}

bool F_TRIG::Run(bool CLK)
{
	F_TRIG::CLK = CLK;
	Q = !F_TRIG::CLK && !_M;
	_M = !F_TRIG::CLK;
	return Q;
}

// ------------------- RS_TRIG -------------------
RS_TRIG::RS_TRIG()
{
	SET = false;
	RESET = false;
	Q = false;
}

bool RS_TRIG::Run(bool SET, bool RESET)
{
	RS_TRIG::SET = SET;
	RS_TRIG::RESET = RESET;
	Q = !RESET and (SET or Q);
	return Q;
}

bool RS_TRIG::Run()
{
	Q = !RESET and (SET or Q);
	return Q;
}

// ------------------- SR_TRIG -------------------
SR_TRIG::SR_TRIG()
{
	SET = false;
	RESET = false;
	Q = false;
}

bool SR_TRIG::Run(bool SET, bool RESET)
{
	SR_TRIG::SET = SET;
	SR_TRIG::RESET = RESET;
	Q = SET or (!RESET and Q);
	return Q;
}

bool SR_TRIG::Run()
{
	Q = SET or (!RESET and Q);
	return Q;
}

#endif