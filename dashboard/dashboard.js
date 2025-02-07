const API_URL = "http://localhost:8080";
const N = 50;
const temperatureData = [];

async function getLastData() {
    try {
        let response = await fetch(`${API_URL}/status`);
        let data = await response.json();

        document.getElementById("systemState").textContent = data.state;
        document.getElementById("currentTemp").textContent = data.temperature;
        document.getElementById("windowOpen").textContent = data.windowPosition;

        // Abilita il pulsante reset ALARM se lo stato è "ALARM"
        document.getElementById("resetAlarmBtn").disabled = (data.state !== "ALARM");
    } catch (error) {
        console.error("Errore nel recupero dei dati 1:", error);
    }
}

async function getLast_N_Temperature() {
    try {
        const response = await fetch(`${API_URL}/latestNtemperature?count=${N}`);
        const data = await response.json();
        updateChartAndStats(data.temperatures);
    } catch (error) {
        console.error("Error fetching data:", error);
        return [];
    }
}

// Funzione per cambiare modalità
document.getElementById("modeBtn").addEventListener("click", async () => {
    let text = document.getElementById("modeBtn").textContent;
    if (text == "Attiva modalità MANUALE") {

        document.getElementById("modeBtn").textContent = "Attiva modalità AUTOMATICA";
        document.getElementById("windowSlider").style.display = "inline";
        await fetch(`${API_URL}/mode/automatic`, { method: "POST" });
    } else if (text == "Attiva modalità AUTOMATICA") {

        document.getElementById("modeBtn").textContent = "Attiva modalità MANUALE";
        document.getElementById("windowSlider").style.display = "none";
        await fetch(`${API_URL}/mode/manual`, { method: "POST" });
    }

});

// Controllo finestra manuale
document.getElementById("windowSlider").addEventListener("input", async (event) => {
    let value = event.target.value;
    await fetch(`${API_URL}/window?open=${value}`, { method: "POST" });
});

// Reset allarme
document.getElementById("resetAlarmBtn").addEventListener("click", async () => {
    await fetch(`${API_URL}/reset-alarm`, { method: "POST" });
    alert("Allarme resettato");
});


function updateChartAndStats(latestData) {
    if (latestData.length > 0) {
        // Update the chart data
        temperatureChart.data.labels = Array.from({ length: N }, (_, i) => `Measurement ${i + 1}`);
        temperatureChart.data.datasets[0].data = latestData;

        // Update stats
        const avgTemp = (latestData.reduce((acc, value) => acc + value, 0) / latestData.length).toFixed(2);
        const maxTemp = Math.max(...latestData);
        const minTemp = Math.min(...latestData);

        document.getElementById("avg-temp").textContent = avgTemp;
        document.getElementById("max-temp").textContent = maxTemp;
        document.getElementById("min-temp").textContent = minTemp;

        // Update the chart
        temperatureChart.update();
    }
}

// Chart.js configuration
const ctx = document.getElementById('temperatureChart').getContext('2d');
const temperatureChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [],  // Empty labels initially
        datasets: [{
            label: 'Temperature (°C)',
            data: [],  // Empty data initially
            borderColor: 'rgb(75, 192, 192)',
            fill: false,
            tension: 0.1,
            pointRadius: 5,
            pointBackgroundColor: 'rgb(75, 192, 192)'
        }]
    },
    options: {
        responsive: true,
        scales: {
            x: {
                title: {
                    display: true,
                    text: 'Measurements'
                },
                ticks: {
                    beginAtZero: true
                }
            },
            y: {
                title: {
                    display: true,
                    text: 'Temperature (°C)'
                },
                beginAtZero: false,
                ticks: {
                    stepSize: 1
                }
            }
        }
    }
});

document.getElementById("windowSlider").style.display = "none";
setInterval(getLastData, 1000);
setInterval(getLast_N_Temperature, 1000);
getLastData();
getLast_N_Temperature();



