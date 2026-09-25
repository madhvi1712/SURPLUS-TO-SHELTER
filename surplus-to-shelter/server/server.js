import express from 'express';
import cors from 'cors';
import { calculateMatchScore, evaluateSafeWindow } from './matchingEngine.js';

const app = express();
app.use(cors());
app.use(express.json());

// In-Memory Database with realistic pre-seeded data
let shelters = [
  { id: 'S1', name: 'Green Hope Shelter', capacity: 80, currentHeld: 20, distanceKm: 1.2, address: 'Civil Lines' },
  { id: 'S2', name: 'Care Foundation Hub', capacity: 40, currentHeld: 15, distanceKm: 3.5, address: 'Adarsh Nagar' },
  { id: 'S3', name: 'City Relief Food Bank', capacity: 150, currentHeld: 40, distanceKm: 4.8, address: 'Industrial Area' }
];

let donations = [
  {
    id: 'RES-101',
    donorName: 'Hotel Grand Residency',
    foodName: 'Dal Makhani & Jeera Rice',
    category: 'Cooked',
    storageTemp: 'Hot',
    servings: 45,
    expiryTimestamp: Date.now() + (2 * 60 * 60 * 1000), // 2 hours
    pickupAddress: 'Block 4, City Center',
    contactPhone: '+91 98111 22334',
    status: 'POSTED',
    matchedShelterId: null,
    volunteerName: null
  },
  {
    id: 'RES-102',
    donorName: 'Hostel Dining Hall',
    foodName: 'Veg Pulao with Soybean',
    category: 'Cooked',
    storageTemp: 'Hot',
    servings: 30,
    expiryTimestamp: Date.now() + (3 * 60 * 60 * 1000), // 3 hours
    pickupAddress: 'North Campus Gate 2',
    contactPhone: '+91 98222 33445',
    status: 'POSTED',
    matchedShelterId: null,
    volunteerName: null
  }
];

// 1. GET ALL ACTIVE DONATIONS & MATCHES
app.get('/api/donations', (req, res) => {
  const enriched = donations.map(d => {
    const safetyCheck = evaluateSafeWindow(d);
    const scoredShelters = shelters.map(s => ({
      shelterId: s.id,
      shelterName: s.name,
      matchScore: calculateMatchScore(d, s)
    })).sort((a, b) => b.matchScore - a.matchScore);

    return {
      ...d,
      safetyCheck,
      bestMatch: scoredShelters[0] || null
    };
  });
  res.json(enriched);
});

// 2. POST NEW DONATION (1-Minute Fast Intake)
app.post('/api/donations', (req, res) => {
  const { donorName, foodName, category, storageTemp, servings, expiryHours, pickupAddress, contactPhone } = req.body;
  
  const newDonation = {
    id: 'RES-' + Math.floor(1000 + Math.random() * 9000),
    donorName,
    foodName,
    category: category || 'Cooked',
    storageTemp: storageTemp || 'Hot',
    servings: Number(servings),
    expiryTimestamp: Date.now() + (Number(expiryHours) * 60 * 60 * 1000),
    pickupAddress,
    contactPhone,
    status: 'POSTED',
    matchedShelterId: null,
    volunteerName: null
  };

  donations.unshift(newDonation);
  res.status(201).json({ success: true, data: newDonation });
});

// 3. STATUS LIFECYCLE UPDATE
app.patch('/api/donations/:id/status', (req, res) => {
  const { id } = req.params;
  const { status, shelterId, volunteerName } = req.body;

  const item = donations.find(d => d.id === id);
  if (!item) return res.status(404).json({ error: 'Donation not found' });

  if (status) item.status = status;
  if (shelterId) item.matchedShelterId = shelterId;
  if (volunteerName) item.volunteerName = volunteerName;

  res.json({ success: true, data: item });
});

// 4. IMPACT METRICS ENGINE
app.get('/api/analytics', (req, res) => {
  const delivered = donations.filter(d => d.status === 'DELIVERED');
  const expired = donations.filter(d => d.status === 'EXPIRED_ROUTED');

  const mealsRescued = delivered.reduce((acc, curr) => acc + curr.servings, 0);
  const kgFoodDiverted = Math.round(mealsRescued * 0.4); // 0.4kg standard meal average
  const co2eAvoidedKg = Math.round(kgFoodDiverted * 2.5); // Standard 2.5 kg CO2e per kg food diverted
  const bioRecycleKg = expired.reduce((acc, curr) => acc + (curr.servings * 0.4), 0);

  res.json({
    mealsRescued,
    kgFoodDiverted,
    co2eAvoidedKg,
    bioRecycleKg,
    completedDeliveries: delivered.length,
    activePending: donations.filter(d => d.status !== 'DELIVERED').length
  });
});

const PORT = 5000;
app.listen(PORT, () => {
  console.log(`Surplus-to-Shelter server running on port ${PORT}`);
});