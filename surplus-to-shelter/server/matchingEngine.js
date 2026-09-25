// Deterministic Rule-Based Scoring Engine (0-100 Scale)
// No AI/ML: 100% Mathematical & Logic Driven

export function calculateMatchScore(donation, shelter) {
  // 1. Distance Calculation (Max 40 points)
  // Distance assumed in km. Closer = higher score within 10km limit.
  const distanceKm = shelter.distanceKm || 2.5;
  const distScore = Math.max(0, (10 - distanceKm) / 10) * 40;

  // 2. Capacity Score (Max 25 points)
  const availableCapacity = shelter.capacity - shelter.currentHeld;
  let capacityScore = 0;
  if (availableCapacity >= donation.servings) {
    capacityScore = 25; // Full capacity to take entire batch
  } else if (availableCapacity > 0) {
    capacityScore = (availableCapacity / donation.servings) * 15;
  }

  // 3. Expiry Criticality Score (Max 25 points)
  // Less time remaining = higher dispatch urgency
  const now = Date.now();
  const minutesRemaining = Math.max(1, Math.round((donation.expiryTimestamp - now) / 60000));
  let urgencyScore = 10;
  if (minutesRemaining <= 60) {
    urgencyScore = 25; // Critical (< 1 hour)
  } else if (minutesRemaining <= 120) {
    urgencyScore = 20; // High (1-2 hours)
  } else if (minutesRemaining <= 240) {
    urgencyScore = 15; // Medium (2-4 hours)
  }

  // 4. Food Category Compatibility (Max 10 points)
  const catScore = donation.category === 'Cooked' ? 10 : 8;

  const totalScore = Math.round(distScore + capacityScore + urgencyScore + catScore);
  return Math.min(100, Math.max(10, totalScore));
}

// Expired Food Routing Function
// Directs spoiled/expired batches to Composting or Biogas plants
export function evaluateSafeWindow(donation) {
  const now = Date.now();
  if (now >= donation.expiryTimestamp) {
    return {
      isExpired: true,
      suggestedAction: 'BIO_RECYCLE',
      targetFacility: 'Municipal Bio-Energy & Composting Facility'
    };
  }
  return {
    isExpired: false,
    suggestedAction: 'HUMAN_SHELTER',
    targetFacility: 'Registered Food Banks & Shelters'
  };
}