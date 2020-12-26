// свой список режимов

// ************************ НАСТРОЙКИ ************************
#define SMOOTH_CHANGE 1     // плавная смена режимов через чёрный
#define SHOW_FULL_TEXT 1    // не переключать режим, пока текст не покажется весь
#define SHOW_TEXT_ONCE 1    // показывать бегущий текст только 1 раз

// подключаем внешние файлы с картинками
//#include "bitmap2.h"
//#include <string.h>


/////////////////////////COUNTER///////////////////////////////////
DateTime dtHappyNewYear (2020, 1, 1, 0, 0, 0);
DateTime dtNow;
TimeSpan tsRemained;
String stDays;

String WriteDays(int iDays){
  int iDays2 = (iDays/10) %10;
  iDays = iDays %10;
  if (iDays2 == 1)return " ДНЕЙ";
  else{
    if (iDays == 1) return " ДЕНЬ";
    else if(iDays == 2 || iDays == 3 || iDays== 4) return " ДНЯ";
    else return " ДНЕЙ";
  }
}

String WriteHours(int iHours){
  if (iHours == 1 || iHours == 21)return " ЧАС";
  else if ((iHours >= 2 && iHours <= 4) || (iHours >= 22 && iHours <= 24))return " ЧАСА";
  else return " ЧАСОВ";
}

String WriteMinutes(int iMinutes){
  if (iMinutes >= 5 && iMinutes <= 20) return " МИНУТ";
  else {
    iMinutes = iMinutes %10;
    if (iMinutes == 1) return " МИНУТА";
    else if (iMinutes >= 2 && iMinutes <=4) return " МИНУТЫ";
    else return " МИНУТ";
  }
}

String WriteSeconds(int iSeconds){
  if (iSeconds >= 5 && iSeconds <= 20) return " СЕКУНД";
  else {
    iSeconds = iSeconds %10;
    if (iSeconds == 1) return " СЕКУНДА";
    else if (iSeconds >= 2 && iSeconds <=4) return " СЕКУНДЫ";
    else return " СЕКУНД";
  }
}

String WriteCounter(TimeSpan tsCurRemained){
  int iDays = tsCurRemained.days();
  int iHours = tsCurRemained.hours();
  int iMinutes = tsCurRemained.minutes();
  int iSeconds = tsCurRemained.seconds();
  
  if (iDays >= 1) return "ДО НОВОГО ГОДА ОСТАЛОСЬ - " + String(iDays) + WriteDays(iDays) + " " + String(iHours) + WriteHours(iHours);
  else {
    if (iHours >= 1) return "ДО НОВОГО ГОДА ОСТАЛОСЬ - " + String(iHours) + WriteHours(iHours) + " " + String(iMinutes) + WriteMinutes(iMinutes);
    else {
      if (iMinutes >= 1)return "ДО НОВОГО ГОДА ОСТАЛОСЬ - " + String(iMinutes) + WriteMinutes(iMinutes) + " " + String(iSeconds) + WriteSeconds(iSeconds);
      else {
        if (iSeconds >= 1)return "ДО НОВОГО ГОДА ОСТАЛОСЬ - " + String(iSeconds) + WriteSeconds(iSeconds);
        else {
          return String("С НОВЫМ ГОДОМ!!!");
        }
      }
    }
  }
}
/////////////////////////COUNTER///////////////////////////////////


// ************************* СВОЙ СПИСОК РЕЖИМОВ ************************
// список можно менять, соблюдая его структуру. Можно удалять и добавлять эффекты, ставить их в
// любой последовательности или вообще оставить ОДИН. Удалив остальные case и break. Cтруктура оч простая:
// case <номер>: <эффект>;
//  break;

// не забудьте указать количество режимов для корректного переключения с последнего на первый
#define MODES_AMOUNT 6   // количество кастомных режимов (которые переключаются сами или кнопкой)


void customModes() {
  switch (thisMode) {
    case 0: starfallRoutine();
      break;
////////////////////////COUNTER///////////////////////////////////
    case 1:
      { 
      dtNow = rtc.now();
      tsRemained = dtHappyNewYear - dtNow;
      fillString(WriteCounter(tsRemained), CRGB::Green);
      }
      break;
/////////////////////////COUNTER///////////////////////////////////

    case 2: snowRoutine();
      break;
    case 3: ballsRoutine();
      break;
    case 4: sparklesRoutine();
      break;
    case 5: rainbowNoise();
      break;
  }
}

// функция загрузки картинки в матрицу. должна быть здесь, иначе не работает =)
void loadImage(uint16_t (*frame)[WIDTH]) {
  for (byte i = 0; i < WIDTH; i++)
    for (byte j = 0; j < HEIGHT; j++)
      drawPixelXY(i, j, gammaCorrection(expandColor((pgm_read_word(&(frame[HEIGHT - j - 1][i]))))));
  // да, тут происходит лютенький п@здец, а именно:
  // 1) pgm_read_word - восстанавливаем из PROGMEM (флэш памяти) цвет пикселя в 16 битном формате по его координатам
  // 2) expandColor - расширяем цвет до 24 бит (спасибо adafruit)
  // 3) gammaCorrection - проводим коррекцию цвета для более корректного отображения
}
//timerMinim gifTimer(D_GIF_SPEED);

// ********************* ОСНОВНОЙ ЦИКЛ РЕЖИМОВ *******************
#if (SMOOTH_CHANGE == 1)
byte fadeMode = 4;
boolean modeDir;
#endif

static void nextMode() {
#if (SMOOTH_CHANGE == 1)
  fadeMode = 0;
  modeDir = true;
#else
  nextModeHandler();
#endif
}
static void prevMode() {
#if (SMOOTH_CHANGE == 1)
  fadeMode = 0;
  modeDir = false;
#else
  prevModeHandler();
#endif
}
void nextModeHandler() {
  thisMode++;
  if (thisMode >= MODES_AMOUNT) thisMode = 0;
  loadingFlag = true;
  gamemodeFlag = false;
  FastLED.clear();
  FastLED.show();
}
void prevModeHandler() {
  thisMode--;
  if (thisMode < 0) thisMode = MODES_AMOUNT - 1;
  loadingFlag = true;
  gamemodeFlag = false;
  FastLED.clear();
  FastLED.show();
}

int fadeBrightness;
#if (SMOOTH_CHANGE == 1)
void modeFader() {
  if (fadeMode == 0) {
    fadeMode = 1;
  } else if (fadeMode == 1) {
    if (changeTimer.isReady()) {
      fadeBrightness -= 40;
      if (fadeBrightness < 0) {
        fadeBrightness = 0;
        fadeMode = 2;
      }
      FastLED.setBrightness(fadeBrightness);
    }
  } else if (fadeMode == 2) {
    fadeMode = 3;
    if (modeDir) nextModeHandler();
    else prevModeHandler();
  } else if (fadeMode == 3) {
    if (changeTimer.isReady()) {
      fadeBrightness += 40;
      if (fadeBrightness > globalBrightness) {
        fadeBrightness = globalBrightness;
        fadeMode = 4;
      }
      FastLED.setBrightness(fadeBrightness);
    }
  }
}
#endif

boolean loadFlag2;
void customRoutine() {
  if (!BTcontrol) {
    if (!gamemodeFlag) {
      if (effectTimer.isReady()) {
#if (OVERLAY_CLOCK == 1 && USE_CLOCK == 1)
        if (overlayAllowed()) {
          if (!loadingFlag && !gamemodeFlag && needUnwrap() && modeCode != 0) clockOverlayUnwrap(CLOCK_X, CLOCK_Y);
          if (loadingFlag) loadFlag2 = true;
        }
#endif

        customModes();                // режимы крутятся, пиксели мутятся

#if (OVERLAY_CLOCK == 1 && USE_CLOCK == 1)
        if (overlayAllowed()) {
          if (!gamemodeFlag && modeCode != 0) clockOverlayWrap(CLOCK_X, CLOCK_Y);
          if (loadFlag2) {
            setOverlayColors();
            loadFlag2 = false;
          }
        }
#endif
        loadingFlag = false;
        FastLED.show();
      }
    } else {
      customModes();
    }
    btnsModeChange();
#if (SMOOTH_CHANGE == 1)
    modeFader();
#endif
  }

  if (idleState) {
    if (fullTextFlag && SHOW_TEXT_ONCE) {
      fullTextFlag = false;
      autoplayTimer = millis();
      nextMode();
    }
    if (millis() - autoplayTimer > autoplayTime && AUTOPLAY) {    // таймер смены режима
      if (modeCode == 0 && SHOW_FULL_TEXT) {    // режим текста
        if (fullTextFlag) {
          fullTextFlag = false;
          autoplayTimer = millis();
          nextMode();
        }
      } else {
        autoplayTimer = millis();
        nextMode();
      }
    }
  } else {
    if (idleTimer.isReady()) {      // таймер холостого режима
      idleState = true;
      autoplayTimer = millis();
      gameDemo = true;

      gameSpeed = DEMO_GAME_SPEED;
      gameTimer.setInterval(gameSpeed);

      loadingFlag = true;
      BTcontrol = false;
      FastLED.clear();
      FastLED.show();
    }
  }
}

void timeSet(boolean type, boolean dir) {    // type: 0-часы, 1-минуты, dir: 0-уменьшить, 1-увеличить
  if (type) {
    if (dir) hrs++;
    else hrs--;
  } else {
    if (dir) mins++;
    else mins--;
    if (mins > 59) {
      mins = 0;
      hrs++;
    }
    if (mins < 0) {
      mins = 59;
      hrs--;
    }
  }
  if (hrs > 23) hrs = 0;
  if (hrs < 0) hrs = 23;
}

void btnsModeChange() {
#if (USE_BUTTONS == 1)
  if (bt_set.clicked()) {
    if (gamemodeFlag) gameDemo = !gameDemo;
    if (gameDemo) {
      gameSpeed = DEMO_GAME_SPEED;
      gameTimer.setInterval(gameSpeed);
      AUTOPLAY = true;
    } else {
      gameSpeed = D_GAME_SPEED;
      gameTimer.setInterval(gameSpeed);
      AUTOPLAY = false;
    }
  }
  if (bt_set.holded()) {
    if (modeCode == 2)
      mazeMode = !mazeMode;
    if (modeCode == 1) {    // вход в настройку часов
      clockSet = !clockSet;
      AUTOPLAY = false;
      secs = 0;
#if (USE_CLOCK == 1)
      if (!clockSet) rtc.adjust(DateTime(2014, 1, 21, hrs, mins, 0)); // установка нового времени в RTC
#endif
    }
  }

  // timeSet type: 0-часы, 1-минуты, dir: 0-уменьшить, 1-увеличить

  if (gameDemo) {
    if (bt_right.clicked()) {
      if (!clockSet) {
        autoplayTimer = millis();
        nextMode();
      } else {
        timeSet(1, 1);
      }
    }

    if (bt_left.clicked()) {
      if (!clockSet) {
        autoplayTimer = millis();
        prevMode();
      } else {
        timeSet(1, 0);
      }
    }

    if (bt_up.clicked()) {
      if (!clockSet) {
        AUTOPLAY = true;
        autoplayTimer = millis();
      } else {
        timeSet(0, 1);
      }
    }
    if (bt_down.clicked()) {
      if (!clockSet) {
        AUTOPLAY = false;
      } else {
        timeSet(0, 0);
      }
    }

    if (bt_right.holding())
      if (changeTimer.isReady()) {
        if (!clockSet) {
          effects_speed -= 2;
          if (effects_speed < 30) effects_speed = 30;
          effectTimer.setInterval(effects_speed);
        } else {
          timeSet(1, 1);
        }
      }
    if (bt_left.holding())
      if (changeTimer.isReady()) {
        if (!clockSet) {
          effects_speed += 2;
          if (effects_speed > 300) effects_speed = 300;
          effectTimer.setInterval(effects_speed);
        } else {
          timeSet(1, 0);
        }
      }
    if (bt_up.holding())
      if (changeTimer.isReady()) {
        if (!clockSet) {
          globalBrightness += 2;
          if (globalBrightness > 255) globalBrightness = 255;
          fadeBrightness = globalBrightness;
          FastLED.setBrightness(globalBrightness);
        } else {
          timeSet(0, 1);
        }
      }
    if (bt_down.holding())
      if (changeTimer.isReady()) {
        if (!clockSet) {
          globalBrightness -= 2;
          if (globalBrightness < 0) globalBrightness = 0;
          fadeBrightness = globalBrightness;
          FastLED.setBrightness(globalBrightness);
        } else {
          timeSet(0, 0);
        }
      }
  }
#endif
}
