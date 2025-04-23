console.log("Main JavaScript loaded!");

document.addEventListener('DOMContentLoaded', (event) => {
    console.log('DOM fully loaded and parsed');
    const button = document.getElementById('myButton');
    button.addEventListener('click', () => {
        console.log('Button was clicked!');
        alert('Button was clicked!');
    });
    const inputField = document.getElementById('myInput');
    inputField.addEventListener('input', (event) => {
        console.log('Input field value changed:', event.target.value);
    });
    const myDiv = document.getElementById('myDiv');
    myDiv.addEventListener('mouseover', () => {
        console.log('Mouse is over the div!');
        myDiv.style.backgroundColor = 'lightblue';
    });
});
