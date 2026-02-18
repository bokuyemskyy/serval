document.addEventListener('DOMContentLoaded', () => {
    const status = document.getElementById('js-status');
    status.innerText = "✅ JavaScript is loaded and running!";
    status.style.color = "green";
    console.log("C++ Server: Static assets loaded successfully.");
});