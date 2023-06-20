/*
 * PLCStandardLib_3_2.h
 * Аналог Standard.lib из CODESYS 2.3 для Arduino и т.п.
 * См. справку CODESYS 2.3 по работе с блоками таймеров и триггеров.
 * Author: Chupakabra303
 */

#ifndef PLCSTANDARDLIB_H_
#define PLCSTANDARDLIB_H_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
/* Расчет глобальной переменной VAR_GLOBAL_TIME вынесен из библиотеки. Просто добавьте VAR_GLOBAL_TIME = millis(); в начало loop().
Централизованное обновление VAR_GLOBAL_TIME требуется для синхронизации всех таймеров. */
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
      this->PT = PT;
    }
    bool operator()() {
      if (!IN) {
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
    bool operator()(bool IN) {
      this->IN = IN;
      operator()();
      return Q;
    }
    bool operator()(unsigned long PT, bool IN) {
      this->PT = PT;
      this->IN = IN;
      operator()();
      return Q;
    }
    /*template <typename... T> bool Run(T... vals) { // Устаревший метод. Заменен на operator()
      operator()(vals...);
      return Q;
    }*/
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
      this->PT = PT;
    }
    bool operator()() {
      if (IN) {
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
    bool operator()(bool IN) {
      this->IN = IN;
      operator()();
      return Q;
    }
    bool operator()(unsigned long PT, bool IN) {
      this->PT = PT;
      this->IN = IN;
      operator()();
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
    // Поэтому я добавил тут возможность игнорировать этот "квази фронт" на первом запуске и сделал значением по умолчанию.
    R_TRIG(bool M = true) {
      _M = M;
    }
    bool operator()() {
      Q = CLK && !_M;
      _M = CLK;
      return Q;
    }
    bool operator()(bool CLK) {
      this->CLK = CLK;
      operator()();
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
    // Специальная инициализация начального состояния. Если M == true, подавляется событие при первом вызове Run, когда CLK == false. Поведение аналогично CODESYS 2.3
    F_TRIG(bool M = true) {
      _M = M;
    }
    bool operator()() {
      Q = !CLK && !_M;
      _M = !CLK;
      return Q;
    }
    bool operator()(bool CLK) {
      this->CLK = CLK;
      operator()();
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
    bool operator()() {
      Q = !RESET && (SET || Q);
      return Q;
    }
    bool operator()(bool SET, bool RESET) {
      this->SET = SET;
      this->RESET = RESET;
      operator()();
      return Q;
    }
    bool SET = false; // установка триггера
    bool RESET = false; // сброс триггера
    bool Q = false; // выходная переменная
};

// ------------------- SR_TRIG -------------------
class SR_TRIG {
  public:
    bool operator()() {
      Q = SET || (!RESET && Q);
      return Q;
    }
    bool operator()(bool SET, bool RESET) {
      this->SET = SET;
      this->RESET = RESET;
      operator()();
      return Q;
    }
    bool SET = false; // установка триггера
    bool RESET = false; // сброс триггера
    bool Q = false; // выходная переменная
};

#endif
