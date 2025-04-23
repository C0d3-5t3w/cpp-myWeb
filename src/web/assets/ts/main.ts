document.addEventListener('DOMContentLoaded', () => {
    const heading = document.querySelector('h1');

    if (heading) {
        heading.addEventListener('click', () => {
            // Generate a random hex color
            const randomColor = '#' + Math.floor(Math.random()*16777215).toString(16).padStart(6, '0');
            heading.style.backgroundColor = randomColor;
        });
    } else {
        console.error('Heading element not found!');
    }
});

