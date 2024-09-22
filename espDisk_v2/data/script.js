const textArea = document.getElementById('textArea');
  const inputArea = document.getElementById('inputArea');
  const submitButton = document.getElementById('submitButton');
  textArea.value = '>';
  submitButton.addEventListener('click', async () => {
    const data = inputArea.value;
    inputArea.value = '';

    try {
        // Отправляем данные на сервер и сохраняем ответ
        const response = await fetch('/q', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ data })
        });

        console.log('Successfully sent data to server ' + data);

        // Проверяем, был ли запрос успешным
        if (!response.ok) {
            textArea.value += 'Ошибка сервера: ' + response.statusText + '\n';
            return;
        }

        // Получаем ответ от сервера
        const serverResponseText = await response.text();

//------------- добавление отловленных команд ---------------
        if (data === 'clear') {
            textArea.value = '>';
            return;
        }
        else if (data === 'upload'){
            upload();
        }
        textArea.value += serverResponseText;


        textArea.scrollTop = textArea.scrollHeight;

    } catch (error) {
        console.error('Error sending data: ', error);
        textArea.value += 'Ошибка отправки данных: ' + error + '\n';
    }
});

document.addEventListener('keyup', event => {
if (event.code === 'Enter') {
  submitButton.click();
  inputArea.focus();
  }
});

function upload() {
    // Создаем элемент input файлового поля
    var fileInput = document.createElement("input");
    fileInput.type = 'file';

    fileInput.click();

    fileInput.addEventListener('change', function () {
        // Проверяем, что файл выбран
        if (fileInput.files.length > 0) {
            var formData = new FormData();

            // Добавление выбранного файла к объекту FormData
            formData.append('file', fileInput.files[0]);

            // Отправка файла на сервер с использованием fetch API
            fetch('/f', {
                method: 'POST',
                body: formData // Здесь не нужно указывать заголовки для Content-Type
            })
            .then(response => {
                // Проверка на успешный ответ
                if (!response.ok) {
                    throw new Error('Network response was not ok');
                }
                // Пытаемся преобразовать ответ в JSON
                return response.json();
            })
            .then(data => console.log(data))
            .catch(error => console.error('Error:', error));
        } else {
            console.error('No file selected');
        }
    });
}