void Updata_Button() {
  String message = "{\"LED\":\"OFF\",\"Mode\":";
  if (isDisplay) message = "{\"LED\":\"ON\",\"Mode\":";
  message = message + Mode + ",\"sliderValue\":" + BRIGHTNESS + '}';
  websocket.broadcastTXT(message);
}

void Get_Button() {
  if (!digitalRead(Button1) && millis() - Last_Time > 100) {
    if (Mode > 0)Mode--;
    else Mode = 7;
    digitalWrite(LED, HIGH);
    while (!digitalRead(Button1));
    digitalWrite(LED, LOW);
    Updata_Button();
    Last_Time = millis();
  }
  else if (!digitalRead(Button2) && millis() - Last_Time > 100) {
    Mode = (Mode + 1) % 6;
    digitalWrite(LED, HIGH);
    while (!digitalRead(Button2));
    digitalWrite(LED, LOW);
    Updata_Button();
    Last_Time = millis();
  }
  else if (!digitalRead(Button3) && millis() - Last_Time > 100) {
    isDisplay = !isDisplay;
    digitalWrite(LED, HIGH);
    while (!digitalRead(Button3));
    digitalWrite(LED, LOW);
    Updata_Button();
    Last_Time = millis();
  }
  else if (!digitalRead(Button4) && millis() - Last_Time > 100) {
    Mode = (Mode + 1) % 2 + 6;
    digitalWrite(LED, HIGH);
    while (!digitalRead(Button4));
    digitalWrite(LED, LOW);
    Updata_Button();
    Last_Time = millis();
  }
}

void Palette_Rectangle() {
  int index = 0;
  static boolean Direction;
  static uint8_t ColorIndex;
  if (ColorIndex == 0)Direction = !Direction;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < (3 - i) * 2 + 1; j++) {
      leds[i * 9 + j] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + index, BRIGHTNESS, LINEARBLEND);
      leds[i * 9 + 8 * (j + 1)] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + index, BRIGHTNESS, LINEARBLEND);
      leds[(7 - i) * 9 - j] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + index, BRIGHTNESS, LINEARBLEND);
      leds[(7 - i) * 9 - 8 * (j + 1)] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + index, BRIGHTNESS, LINEARBLEND);
    }
    if (Direction)index += 16;
    else index -= 16;
  }
  ColorIndex++;
}

void Palette_Background() {
  static uint8_t ColorIndex;
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t index = 0;
    for (uint8_t j = 0; j < 8; j++) {
      leds[j + i * 8] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + index, BRIGHTNESS, LINEARBLEND);
      index += 8;
    }
  }
  ColorIndex++;
}

void Palette_Rotate() {
  static int Angle;
  static int Rotate;
  static uint8_t ColorIndex;
  uint8_t beat = (beatsin8(12, 0, 255, 0, 0) + beatsin8(24, 0, 255, 0, 0)) / 2;
  Angle = map(beat, 0, 255, 0, 360);
  for (int i = 0; i < 8; i++) {
    uint8_t index = 0;
    for (int j = 0; j < 8; j++) {
      int LED_Pos;
      switch (Angle / 45) {
        case 0: LED_Pos = i * 8 + j; Rotate = -map(Angle, 0, 44, 0, 8); break; //UP
        case 1: LED_Pos = i + (7 - j) * 8; Rotate = map(Angle, 45, 89, 8, 0); break;
        case 2: LED_Pos = i + (7 - j) * 8; Rotate = -map(Angle, 90, 134, 0, 8); break; //RIGHT
        case 3: LED_Pos = i * 8 + 7 - j; Rotate = -map(Angle, 135, 179, 8, 0); break;
        case 4: LED_Pos = i * 8 + 7 - j; Rotate = map(Angle, 180, 224, 0, 8); break; //DOWN
        case 5: LED_Pos = i + j * 8; Rotate = -map(Angle, 225, 269, 8, 0); break;
        case 6: LED_Pos = i + j * 8; Rotate = map(Angle, 270, 314, 0, 8); break; //LEFT
        case 7: LED_Pos = i * 8 + j; Rotate = map(Angle, 315, 359, 8, 0); break;
      }
      leds[LED_Pos] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + index + i * Rotate, BRIGHTNESS, LINEARBLEND);
      index += 8;
    }
  }
  ColorIndex++;
}

void Chase() {
  static uint8_t ColorIndex;
  uint8_t Pos_Beat = beatsin8(20, 0, 7, 0, 0);
  for (uint8_t i = 0; i < 8; i++) {
    leds[Pos_Beat + i * 8] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex, BRIGHTNESS, LINEARBLEND);
    leds[Pos_Beat * 8 + i] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex, BRIGHTNESS, LINEARBLEND);
  }
  fadeToBlackBy(leds, NUM_LEDS, BRIGHTNESS / 4);
  ColorIndex++;
}

void Cross() {
  static boolean Direction;
  static uint8_t Dir_Index;
  static uint8_t ColorIndex;
  uint8_t Pos_Beat = beatsin8(30, 0, 7, 0, 0);
  if (Pos_Beat == 0)Dir_Index = (Dir_Index + 1) % 4;
  if (!Dir_Index) {
    Direction = !Direction;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }
  for (uint8_t i = 0; i < 4; i++) {
    if (Direction) {
      leds[Pos_Beat + i * 16] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex, BRIGHTNESS, LINEARBLEND);
      leds[(i + 1) * 16 - 1 - Pos_Beat] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex, BRIGHTNESS, LINEARBLEND);
    }
    else {
      leds[Pos_Beat * 8 + i * 2] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex, BRIGHTNESS, LINEARBLEND);
      leds[(7 - Pos_Beat) * 8 + 1 + i * 2] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex, BRIGHTNESS, LINEARBLEND);
    }
  }
  fadeToBlackBy(leds, NUM_LEDS, BRIGHTNESS / 4);
  ColorIndex++;
}

void Twinkle() {
  static uint8_t index;
  index = (index + 1) % 4;
  if (index == 0) {
    if ( random8() < 150)leds[random8(NUM_LEDS)] = ColorFromPalette( ColorPalette[Palette_Mode], random8(255), BRIGHTNESS, LINEARBLEND);
    fadeToBlackBy( leds, NUM_LEDS, BRIGHTNESS / 4);
  }
}

void WaterFall() {
  static uint8_t ColorIndex;
  uint8_t Pos_Beat1 = beatsin8(30, 0, 7, 0, 0);
  uint8_t Pos_Beat2 = beatsin8(30, 0, 7, 0, 32);
  uint8_t Pos_Beat3 = beatsin8(30, 0, 7, 0, 64);
  uint8_t Pos_Beat4 = beatsin8(30, 0, 7, 0, 96);
  uint8_t Pos_Beat5 = beatsin8(30, 0, 7, 0, 128);
  uint8_t Pos_Beat6 = beatsin8(30, 0, 7, 0, 160);
  uint8_t Pos_Beat7 = beatsin8(30, 0, 7, 0, 192);
  uint8_t Pos_Beat8 = beatsin8(30, 0, 7, 0, 224);
  for (uint8_t i = 0; i < Pos_Beat1; i++)leds[7 - i] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex, BRIGHTNESS, LINEARBLEND);
  for (uint8_t i = 0; i < Pos_Beat2; i++)leds[15 - i] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + 8, BRIGHTNESS, LINEARBLEND);
  for (uint8_t i = 0; i < Pos_Beat3; i++)leds[23 - i] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + 16, BRIGHTNESS, LINEARBLEND);
  for (uint8_t i = 0; i < Pos_Beat4; i++)leds[31 - i] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + 24, BRIGHTNESS, LINEARBLEND);
  for (uint8_t i = 0; i < Pos_Beat5; i++)leds[39 - i] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + 32, BRIGHTNESS, LINEARBLEND);
  for (uint8_t i = 0; i < Pos_Beat6; i++)leds[47 - i] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + 40, BRIGHTNESS, LINEARBLEND);
  for (uint8_t i = 0; i < Pos_Beat7; i++)leds[55 - i] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + 48, BRIGHTNESS, LINEARBLEND);
  for (uint8_t i = 0; i < Pos_Beat8; i++)leds[63 - i] = ColorFromPalette( ColorPalette[Palette_Mode], ColorIndex + 56, BRIGHTNESS, LINEARBLEND);
  leds[7 - Pos_Beat1] = CRGB::White;
  leds[15 - Pos_Beat2] = CRGB::White;
  leds[23 - Pos_Beat3] = CRGB::White;
  leds[31 - Pos_Beat4] = CRGB::White;
  leds[39 - Pos_Beat5] = CRGB::White;
  leds[47 - Pos_Beat6] = CRGB::White;
  leds[55 - Pos_Beat7] = CRGB::White;
  leds[63 - Pos_Beat8] = CRGB::White;
  fadeToBlackBy( leds, NUM_LEDS, BRIGHTNESS / 2);
  ColorIndex++;
}

void Show_Num(uint8_t NUM, int ANIMATION) {
  for (uint8_t i = 0; i < 30; i++) {
    uint8_t index = pgm_read_dword(&(Number[NUM][i]));
    if (index != 0)if ((index - ANIMATION >= (index / 8) * 8) && (index - ANIMATION < (index / 8 + 1) * 8))leds[index - ANIMATION] = CRGB(R, G, B);
  }
}

void Show_Letter(uint8_t LETTER, int ANIMATION) {
  for (uint8_t i = 0; i < 37; i++) {
    uint8_t index = pgm_read_dword(&(Letter[LETTER][i]));
    if (index != 0)if ((index - ANIMATION >= (index / 8) * 8) && (index - ANIMATION < (index / 8 + 1) * 8))leds[index - ANIMATION] = CRGB(R, G, B);
  }
}

void Show_String(String Message) {
  static String Text;
  static boolean flag;
  static uint8_t count;
  static uint8_t Pos_Index;
  static int ANIM_Index = -6;
  static uint8_t LetterIndex;
  if (Message != Text) {
    Text = Message;
    flag = false;
    count = 0;
    Pos_Index = 0;
    ANIM_Index = -6;
    LetterIndex = 0;
  }
  if (!flag) {
    if (isUpperCase(Text[Pos_Index]))LetterIndex = Text[Pos_Index] - 65;
    else if (isLowerCase(Text[Pos_Index]))LetterIndex = Text[Pos_Index] - 97;
    else if (isAlphaNumeric(Text[Pos_Index]))LetterIndex = Text[Pos_Index] - 21;
    else LetterIndex = 37;
    flag = true;
  }
  Palette_Background();
  if (LetterIndex < 27) Show_Letter(LetterIndex, ANIM_Index);
  else Show_Num(LetterIndex - 27, ANIM_Index);
  if (count == 0) {
    ANIM_Index++;
    if (ANIM_Index == 8) {
      ANIM_Index = -6;
      Pos_Index = (Pos_Index + 1) % Text.length();
      if (isUpperCase(Text[Pos_Index]))LetterIndex = Text[Pos_Index] - 65;
      else if (isLowerCase(Text[Pos_Index]))LetterIndex = Text[Pos_Index] - 97;
      else if (isAlphaNumeric(Text[Pos_Index]))LetterIndex = Text[Pos_Index] - 21;
      else {
        LetterIndex = 37;
        ANIM_Index = -2;
      }
    }
  }
  count = (count + 1) % 10;
}

void Display() {
  EVERY_N_MILLISECONDS(10) {
    if (isDisplay) {
      switch (Mode) {
        case 0: Twinkle(); break;
        case 1: Chase(); break;
        case 2: Cross(); break;
        case 3: WaterFall(); break;
        case 4: Palette_Rotate(); break;
        case 5: Palette_Rectangle(); break;
        case 6: Show_String(TEXT); break;
        case 7: Show_String(SoftAPIP); break;
        default: Palette_Rectangle(); break;
      }
    }
    else FastLED.clear();
    FastLED.show();
  }
}
