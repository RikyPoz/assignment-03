const API_URL = "http://localhost:8080";
let N = 30;
const temperatureData = [];

async function getLastData() {
    try {
        let response = await fetch(`${API_URL}/status`);
        let data = await response.json();
        const manualBtn = document.getElementById("manualBtn");
        const autoBtn = document.getElementById("autoBtn");
        const slider = document.getElementById("windowSlider");

        document.getElementById("systemState").textContent = data.state;
        document.getElementById("currentTemp").textContent = data.temperature;
        document.getElementById("windowOpen").textContent = data.windowPosition;
        slider.value = data.windowPosition;

        if (data.mode == "MANUAL") {
            manualBtn.disabled = true;
            autoBtn.disabled = false;
            slider.disabled = false;
        } else if (data.mode == "AUTOMATIC") {
            manualBtn.disabled = false;
            autoBtn.disabled = true;
            slider.disabled = true;
        }
        const isAlarm = data.state == "ALARM";
        document.getElementById("resetAlarmBtn").disabled = !isAlarm;
        if (isAlarm) {
            manualBtn.disabled = true;
            autoBtn.disabled = true;
            slider.disabled = true;
        }
        
    } catch (error) {
        console.error("Errore nel recupero dei dati 1:", error);
    }
}

async function getLast_N_Temperature() {
    try {
        const response = await fetch(`${API_URL}/latestNtemperature?count=${N}`);
        const data = await response.json();
        console.log(data);
        updateChartAndStats(data);
    } catch (error) {
        console.error("Error fetching data:", error);
        return [];
    }
}

// Funzione per cambiare modalità
document.getElementById("autoBtn").addEventListener("click", async () => {
    const m = "AUTOMATIC";
    await fetch(`${API_URL}/mode`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ mode: m })
      });
});

document.getElementById("manualBtn").addEventListener("click", async () => {
    const m = "MANUAL";
    await fetch(`${API_URL}/mode`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ mode: m })
      });
});

// Controllo finestra manuale
document.getElementById("windowSlider").addEventListener("input", async (event) => {
    let value = parseInt(event.target.value);

    await fetch(`${API_URL}/window`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ position: value })
      });
});

// Reset allarme
document.getElementById("resetAlarmBtn").addEventListener("click", async () => {
    await fetch(`${API_URL}/reset-alarm`);
    alert("Allarme resettato");
});

document.getElementById("chartSlider").addEventListener("input", (event) => {N = parseInt(event.target.value)});

function updateChartAndStats(data) {
    if (data.temperatures.length > 0) {
        // Converti i timestamp in un formato leggibile
        const labels = data.timestamps.map(ts => {
            const date = new Date(ts);
            return date.toLocaleTimeString(); // Formato leggibile HH:MM:SS
        }).reverse(); // Più recente a destra

        // Estrarre le temperature
        const temperatures = data.temperatures.reverse();

        // Aggiorna il grafico
        temperatureChart.data.labels = labels;
        temperatureChart.data.datasets[0].data = temperatures;

        // Calcola statistiche
        const avgTemp = (temperatures.reduce((acc, value) => acc + value, 0) / temperatures.length).toFixed(2);
        const maxTemp = Math.max(...temperatures);
        const minTemp = Math.min(...temperatures);

        document.getElementById("avg-temp").textContent = avgTemp;
        document.getElementById("max-temp").textContent = maxTemp;
        document.getElementById("min-temp").textContent = minTemp;

        // Aggiorna il grafico
        temperatureChart.update();
    }
}


// Chart.js configuration
const ctx = document.getElementById('temperatureChart').getContext('2d');
const temperatureChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [],  // Etichette inizialmente vuote
        datasets: [{
            label: 'Temperature (°C)',
            data: [],  // Dati inizialmente vuoti
            borderColor: 'rgb(75, 192, 192)',
            fill: false,
            tension: 0.1,
            pointRadius: 2,  // Riduce la dimensione dei punti
            pointBackgroundColor: 'rgb(75, 192, 192)',
            pointHoverRadius: 2  // Evita l'ingrandimento al passaggio del mouse
        }]
    },
    options: {
        responsive: true,
        animation: false,  // Disabilita le animazioni
        scales: {
            x: {
                title: {
                    display: true,
                    text: 'Orario'
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


setInterval(getLastData, 1000);
setInterval(getLast_N_Temperature, 1000);
getLastData();
getLast_N_Temperature();



