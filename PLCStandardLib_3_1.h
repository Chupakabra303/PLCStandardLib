/*
 * PLCStandardLib_3_1.h
 * Аналог Standard.lib из CODESYS 2.3 для Arduino и т.п.
 * См. справку CODESYS 2.3 по работе с блоками таймеров и триггеров.
 */

#ifndef PLCSTANDARDLIB_3_1_H_
#define PLCSTANDARDLIB_3_1_H_

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
class TON {
  public:
    TON(unsigned long PT = 0) {
      TON::PT = PT;
    }
    bool Run(bool IN) {
      TON::IN = IN;
      if (!TON::IN) {
        Q = false;
        ET = 0;
        _M = false;
      } else {
        if (!_M) {
          _M = true; // сбрасываем флаг М
          _StartTime = VAR_GLOBAL_TIME;
          // ET = 0; // сразу = 0
        } else {
          if (!Q)
            ET = VAR_GLOBAL_TIME - _StartTime; // вычисляем время
        }
        if (ET >= PT)
          Q = true;
      }
      return Q;
    }
    bool Q = false; // выходная переменная
    bool IN = false; // входная переменная
    unsigned long PT; // входная переменная
    unsigned long ET = 0; // выходная переменная - текущее значение таймера
  private:
    bool _M = false; // внутренний флаг
    unsigned long _StartTime = 0;
};

// ------------------- TOF -------------------
class TOF {
  public:
    TOF(unsigned long PT = 0) {
      TOF::PT = PT;
    }
    bool Run(bool IN) {
      TOF::IN = IN;
      if (TOF::IN) {
        Q = true;
        ET = 0;
        _M = true;
      } else {
        if (_M) {
          _M = false; // сбрасываем флаг М
          _StartTime = VAR_GLOBAL_TIME;
          // ET = 0; // сразу = 0
        } else {
          if (Q)
            ET = VAR_GLOBAL_TIME - _StartTime; // вычисляем время
        }
        if (ET >= PT)
          Q = false;
      }
      return Q;
    }
    bool Q = false; // выходная переменная
    bool IN = false; // входная переменная
    unsigned long PT; // входная переменная
    unsigned long ET = 0; // выходная переменная - текущее значение таймера
  private:
    bool _M; // внутренний флаг
    unsigned long _StartTime;
};

// ------------------- R_TRIG -------------------
class R_TRIG { // детектор фронта сигнала
  public:
    // Специальная инициализация в конструкторе начального состояния. Если M == true, подавляется событие при первом вызове Run, когда CLK == true
    // В CODESYS 2.3 флаг M инициализируется с false, но это довольно спорно, т.к. блок R_TRIG - это детектор фронта CLK, а не состояния CLK.
    // При старте программы не совсем корректно интерпретировать первое значение CLK == true как "фронт", т.к. реальный "фронт" сигнала мог произойти гораздо раньше запуска программы.
    // Поэтому я добавил тут возможность игнорировать этот "квази фронт" на первом запуске и сделал значеннием по умолчанию.
    R_TRIG(bool M = true) {
      _M = M;
    }
    bool Run(bool CLK) {
      R_TRIG::CLK = CLK;
      Q = R_TRIG::CLK && !_M;
      _M = R_TRIG::CLK;
      return Q;
    }
    bool CLK = false; // входная переменная
    bool Q = false; // выходная переменная
  private:
    bool _M; // внутренний флаг
};

// ------------------- F_TRIG -------------------
class F_TRIG { // детектор спада сигнала
  public:
    F_TRIG(bool M = true) { // Специальная инициализация начального состояния. Если M == true, подавляется событие при первом вызове Run, когда CLK == false. Поведение аналогично CODESYS 2.3
      _M = M;
    }
    bool Run(bool CLK) {
      F_TRIG::CLK = CLK;
      Q = !F_TRIG::CLK && !_M;
      _M = !F_TRIG::CLK;
      return Q;
    }
    bool CLK = false; // входная переменная
    bool Q = false; // выходная переменная
  private:
    bool _M; // внутренний флаг
};

// ------------------- RS_TRIG -------------------
class RS_TRIG {
  public:
    bool Run(bool SET, bool RESET) {
      RS_TRIG::SET = SET;
      RS_TRIG::RESET = RESET;
      Q = Run();
      return Q;
    }
    bool Run() {
      Q = !RESET && (SET || Q);
      return Q;
    }
    bool SET = false; // установка триггера
    bool RESET = false; // сброс триггера
    bool Q = false; // выходная переменная
};

// ------------------- SR_TRIG -------------------
class SR_TRIG {
  public:
    bool Run(bool SET, bool RESET) {
      SR_TRIG::SET = SET;
      SR_TRIG::RESET = RESET;
      Q = Run();
      return Q;
    }
    bool Run() {
      Q = SET || (!RESET && Q);
      return Q;
    }
    bool SET = false; // установка триггера
    bool RESET = false; // сброс триггера
    bool Q = false; // выходная переменная
};

#endif