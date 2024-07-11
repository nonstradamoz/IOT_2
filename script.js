document.getElementById('temperatureForm').addEventListener('submit', function(event) {
    event.preventDefault();
    const temperature = parseFloat(document.getElementById('temperature').value);
    
    // Simulate sending temperature data to the server (replace with actual API call)
    fetch('/send-temperature', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ temperature }),
    })
    .then(response => response.json())
    .then(data => {
        if (data.alert) {
            alert(`Alert: Dog's temperature is ${temperature}°C. Please check.`);
        } else {
            alert('Temperature sent successfully!');
        }
    })
    .catch((error) => {
        console.error('Error:', error);
    });
});
