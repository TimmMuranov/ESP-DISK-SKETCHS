function dirText(event) {
    const buttonText = event.target.textContent;
    console.log('Вы нажали кнопку с текстом: ' + buttonText);
    /* отправка запроса на сервер*/
    fetch('/list', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
        },
      body: JSON.stringify({
      data: buttonText
      })
   })
    .then(res => res.json()).then(
        data => {
          console.log(data);
          }
       )
    .catch(error => {
        console.error('Error:', error);
        }
    );
}
