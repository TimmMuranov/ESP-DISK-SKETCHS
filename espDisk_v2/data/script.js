const inputArea = document.getElementById('inputArea');
const submitButton = document.getElementById('submitButton');
const creatF = document.getElementById('creatFile');
const creatD = document.getElementById('creatDir');

submitButton.addEventListener('click', async () => {
const data = inputArea.value;
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
            console.log('Ошибка сервера: ' + response.statusText);
            return;
        }

        // Получаем ответ от сервера
        const serverResponseText = await response.text();
        alert(serverResponseText);
//------------- добавление отловленных команд ---------------
    } catch (error) {
        console.error('Error sending data: ', error);
    }
});

creatF.addEventListener('click', async () => {
const data = "creatFile";
try {
        const response = await fetch('/bf', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ data })
        });
        if (!response.ok) {
            console.log('Ошибка сервера: ' + response.statusText);
            return;
        }
        const serverResponseText = await response.text();
        alert(serverResponseText);
    } catch (error) {
        console.error('Error sending data: ', error);
    }
});

creatD.addEventListener('click', async () => {
const data = "creatDir";
try {
        const response = await fetch('/bd', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ data })
        });
        if (!response.ok) {
            console.log('Ошибка сервера: ' + response.statusText);
            return;
        }
        const serverResponseText = await response.text();
        alert(serverResponseText);
    } catch (error) {
        console.error('Error sending data: ', error);
    }
});
