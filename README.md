<h2>ESP-DISK</h2>
Данное ПО предназначено для микроконтроллеров ESP32 и ESP8266 в совокупности с SD модуль (в моей сборке используется ESP8266 D1 mini и Micro SD shield).

По задумке, микросхема с данным кодом превращается в локальный сервер для хранения и редактирования текстовых данных.

Он может быть применим как переносная флешка или веб-блокнот.

Главный плюс - не нуждается в подключении к глобальной сети. Единственное, что необходимо - любое устройство с поддержкой WiFi. 
Низкая мощность позволяет использовать вместо розеток такие источники питания, как солнечная батарея, ручной генератор или батарейки, что делает его весьма мобильным и компактным.

Кроме того этот сервер предельно дешевый. Микроконтроллер и переходник под SD карту + сама карта - вот всё, что нужно.

Для того чтобы настроить девайс, необходимо выполнить следующие шаги:

Подобрать необходимое железо: Подойдет ESP8266 и переходник под SD карту Micro SD shield (с другими не тестировал). Саму карту перед эксплуатацией необходимо форматировать в fat16 или fat32.

Установить все использованные в коде библиотеки - ESP8266WiFi, ESP8266WebServer, SD, SPI и др. В среде разработки arduinoIDE всех их можно установить в "управлении библиотеками" (если что-то не загружается - можно взять отсюда: https://github.com/esp8266/Arduino).

По желанию, можно отредактировать названием сети и пароль (по умолчанию, имя - TimsServer; пароль - 12345678). Для этого нужно переписать значение одноименных переменных в начале кода. После, необходимо прошить устройство.

Подключиться к нему по WiFi, как к роутеру, используя отредактированный пароль либо пароль по умолчанию.

Зайти в любой браузер и перейти по ссылке 192.168.1.1 

После этого в браузере откроется веб-интерфейс сервера с интуитивно понятным функционалом. Про спорные моменты и ~~баги~~ фичи можно прочесть при нажатии readme в веб-интерфейсе. (Если что-то не нашлось - отправьте ошибку к репозиторию).
<hr>
С 26.09.2024 этот репозиторий замораживаю и продолжаю работать над рабочей версией Server_v2_2 в отдельном репозитории - ESP-DISK. Этот репозиторий булет складом старых идей.
