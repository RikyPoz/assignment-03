const API_URL = "http://localhost:8080"; // Modifica con l'IP del backend

async function updateDashboard() {
    try {
        let response = await fetch(`${API_URL}/status`);
        let data = await response.json();

        document.getElementById("systemState").textContent = data.state;
        document.getElementById("currentTemp").textContent = data.temperature;
        document.getElementById("windowOpen").textContent = data.window_open;

        // Abilita il pulsante reset ALARM se lo stato è "ALARM"
        document.getElementById("resetAlarmBtn").disabled = (data.state !== "ALARM");
    } catch (error) {
        console.error("Errore nel recupero dei dati:", error);
    }
}

// Funzione per cambiare modalità
document.getElementById("manualModeBtn").addEventListener("click", async () => {
    await fetch(`${API_URL}/mode/manual`, { method: "POST" });
    alert("Modalità MANUAL attivata");
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

// Aggiorna la dashboard ogni 5 secondi
setInterval(updateDashboard, 5000);
updateDashboard();
