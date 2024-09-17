String exCom(String in, int comNum) {
    if (comNum <= 0) return ""; // Если номер команды меньше или равен 0, возвращаем пустую строку

    // Переменные для отслеживания текущего слова и его позиции
    int wordCount = 0;
    int startIndex = 0;
    int endIndex = 0;

    // Пробежимся по строке и найдем нужное слово
    while (endIndex < in.length()) {
        // Ищем начало слова
        while (startIndex < in.length() && in.charAt(startIndex) == ' ') {
            startIndex++; // Пропускаем пробелы
        }
        
        // Устанавливаем endIndex на начало следующего слова
        endIndex = startIndex;
        while (endIndex < in.length() && in.charAt(endIndex) != ' ') {
            endIndex++; // Ищем конец слова
        }

        // Увеличиваем счетчик слов
        wordCount++;

        if (wordCount == comNum) {
            // Возвращаем нужное слово
            return in.substring(startIndex, endIndex);
        }

        // Переходим к следующему слову
        startIndex = endIndex;
    }

    // Если слово с таким номером не найдено, возвращаем пустую строку
    return "";
}
