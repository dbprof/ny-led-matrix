# LED MATRIX ON ESP8266 FOR NEW YEAR CELEBRATION
[![Видео](https://github.com/dbprof/ny-led-matrix/blob/master/video.png)](https://youtu.be/xdceuaSyvG4)

Рассказываю как собрать собрать не просто новогоднюю гирлянду, а целую светодиодную матрицу. Разместить ее можно прямо в проеме окна, чтобы порадовать и поднять настроение не только себе, но и окружающим! Эту красоту я увидел в прошлом году у замечательного изобретателя AlexGyver (https://alexgyver.ru/gyvermatrixbt/) и воссоздал для себя с обратным отсчетом времени до нового года.
Всего в матрице получилось 200 элементов, 10 - в высоту, 20 - в ширину. Каждая из 4 лент соединена последовательно и приклеена зигзагом на направляющие.
Для прошивки esp8266 в Arduino IDE в поле "Дополнительные ссылки для Менеджера плат" (Additional Board Manager URLs) вставляем строчку: http://arduino.esp8266.com/stable/package_esp8266com_index.json
Для сборки использовались элементы:
* 4 х Управляемая светодиодная лента WS2812B
https://ru.aliexpress.com/item/32243084800.html
* 1 х Плата управления NodeMcu ESP8266 CH340G
https://ru.aliexpress.com/item/32800966224.html
* 1 х Часы реального времени rtc
https://ru.aliexpress.com/item/32530897478.html
* 1 х Bluetooth модуль
https://ru.aliexpress.com/item/32272894331.html
* 2 х Труба квадратная
https://leroymerlin.ru/product/truba-kvadratnaya-12h12h2000h12-mm-13014831/
* 1 х Коробка распределительная
https://leroymerlin.ru/product/korobka-raspredelitelnaya-ekoplast-150h110h7-0mm-cvet-seryy-12464507/
* 1 х Вилка Legrand
https://leroymerlin.ru/product/vilka-bez-zazemleniya-legrand-ploskaya-230-v-cvet-belyy-13295872/

Схема подключения:
![Схема подключения](https://github.com/dbprof/ny-led-matrix/blob/master/schema.png)


