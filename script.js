// Colluci Smart Parking + M-Pesa Simulation

const TOTAL_SLOTS = 20;
let slots = [];
let parkedVehicles = {};
let currentExitData = null; // stores info of the vehicle being exited

function initSlots() {
    slots = [];
    for (let i = 1; i <= TOTAL_SLOTS; i++) {
        slots.push({ id: i, occupied: false, plate: null });
    }
    renderSlots();
    updateStats();
}

function calculateFee(minutes) {
    if (minutes <= 30) return 0;
    if (minutes <= 120) return 50;
    if (minutes <= 240) return 100;
    if (minutes <= 360) return 300;
    return 500;
}

function formatDateTime(date) {
    return date.toLocaleString('en-KE', {
        year: 'numeric', month: '2-digit', day: '2-digit',
        hour: '2-digit', minute: '2-digit', second: '2-digit'
    });
}

function renderSlots() {
    const grid = document.getElementById('slotsGrid');
    grid.innerHTML = '';
    slots.forEach(slot => {
        const div = document.createElement('div');
        div.className = `slot ${slot.occupied ? 'occupied' : 'free'}`;
        div.innerHTML = `
            <div class="slot-number">Slot ${slot.id}</div>
            <div class="slot-status">${slot.occupied ? slot.plate : 'FREE'}</div>
        `;
        grid.appendChild(div);
    });
}

function updateStats() {
    const freeCount = slots.filter(s => !s.occupied).length;
    document.getElementById('freeSlots').textContent = freeCount;
    document.getElementById('occupiedSlots').textContent = TOTAL_SLOTS - freeCount;
}

function vehicleEntry() {
    const input = document.getElementById('entryPlate');
    const plate = input.value.trim().toUpperCase();

    if (!plate) return alert('Please enter a number plate');
    if (parkedVehicles[plate]) return alert('Vehicle already parked!');

    const freeSlot = slots.find(s => !s.occupied);
    if (!freeSlot) return alert('Parking is FULL!');

    freeSlot.occupied = true;
    freeSlot.plate = plate;
    parkedVehicles[plate] = { slotId: freeSlot.id, entryTime: new Date() };

    input.value = '';
    renderSlots();
    updateStats();
    alert(`Entry Successful!\nPlate: ${plate}\nSlot: ${freeSlot.id}\nBarrier Opened`);
}

function vehicleExit() {
    const input = document.getElementById('exitPlate');
    const plate = input.value.trim().toUpperCase();

    if (!plate) return alert('Please enter a number plate');
    if (!parkedVehicles[plate]) return alert('Vehicle not found!');

    const vehicle = parkedVehicles[plate];
    const exitTime = new Date();
    const minutes = Math.floor((exitTime - vehicle.entryTime) / 60000);
    const fee = calculateFee(minutes);

    // Store data for payment
    currentExitData = { plate, vehicle, exitTime, minutes, fee };

    // Show receipt
    document.getElementById('receiptContent').innerHTML = `
        <p><strong>Plate:</strong> ${plate}</p>
        <p><strong>Slot:</strong> ${vehicle.slotId}</p>
        <p><strong>Entry Time:</strong> ${formatDateTime(vehicle.entryTime)}</p>
        <p><strong>Exit Time:</strong> ${formatDateTime(exitTime)}</p>
        <p><strong>Duration:</strong> ${minutes} minutes</p>
        <p><strong>Amount:</strong> <span style="color:#4ade80;font-size:1.2rem">${fee} KES</span></p>
    `;

    // Reset M-Pesa sections
    document.getElementById('mpesaSection').style.display = 'block';
    document.getElementById('mpesaProcessing').style.display = 'none';
    document.getElementById('mpesaSuccess').style.display = 'none';
    document.getElementById('payMpesaBtn').style.display = 'block';

    document.getElementById('receiptCard').style.display = 'block';
    input.value = '';
}

// ====== M-PESA SIMULATION ======
function startMpesaPayment() {
    if (!currentExitData) return;

    const phone = prompt("Enter M-Pesa phone number (e.g. 0712345678):");
    if (!phone) return;

    // Hide pay button and show processing
    document.getElementById('payMpesaBtn').style.display = 'none';
    document.getElementById('mpesaProcessing').style.display = 'block';

    // Simulate STK Push delay (3 seconds)
    setTimeout(() => {
        completeMpesaPayment(phone);
    }, 3000);
}

function completeMpesaPayment(phone) {
    const { plate, vehicle, fee } = currentExitData;

    // Free the slot
    const slot = slots.find(s => s.id === vehicle.slotId);
    if (slot) {
        slot.occupied = false;
        slot.plate = null;
    }
    delete parkedVehicles[plate];

    // Show success
    document.getElementById('mpesaProcessing').style.display = 'none';
    document.getElementById('mpesaSuccess').style.display = 'block';
    document.getElementById('mpesaSuccess').innerHTML = `
        <strong>M-Pesa Payment Successful!</strong><br>
        Amount: <strong>${fee} KES</strong><br>
        Phone: ${phone}<br>
        Receipt No: MP${Math.floor(Math.random() * 900000 + 100000)}<br>
        <br>
        Barrier has been OPENED. Drive safely!
    `;

    document.getElementById('mpesaSection').style.display = 'none';

    renderSlots();
    updateStats();
    currentExitData = null;
}

// Initialize
initSlots();