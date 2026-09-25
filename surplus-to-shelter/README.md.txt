# Surplus-to-Shelter: Real-Time Food Rescue Routing
**AmiHacks 1.0 — Track A: NGO / Social Impact**

## 1. Project Overview
Surplus-to-Shelter is a deterministic, rule-based real-time food rescue routing platform designed to bridge the gap between commercial surplus food donors (restaurants, caterers, university dining) and local food shelters before perishable rations reach dumpsters.

## 2. Core Capabilities
- **1-Minute Donor Intake:** Fast form input with storage temperature tags and pre-set pickup time buttons.
- **Deterministic Rule-Based Matching:** Multi-parameter compatibility score calculated without opaque AI:
  - Distance Proximity (40% Weightage)
  - Shelter Headroom Capacity (25% Weightage)
  - Expiry Window Urgency (25% Weightage)
  - Food Category Alignment (10% Weightage)
- **Volunteer Logistics & Dispatch:** Live task progression with direct 1-tap Google Maps directions and telephone call links.
- **Zero-Waste Bio-Recycle Loop:** Batches that breach the safe consumption window are automatically diverted to municipal biogas/composting plants, protecting public health while keeping organic waste out of landfills.
- **Impact Metrics Engine:** Live calculation of meals saved, food weight diverted (kg), and standard carbon offset (2.5 kg CO2e per kg saved).

## 3. Team Responsibilities & Architecture
- **Member 1 (Frontend Lead):** Donor intake portal and Shelter live availability feed.
- **Member 2 (Frontend Logistics):** Volunteer dispatch tracking, Google Maps integration, and Impact Dashboard.
- **Member 3 (Backend Architecture):** Node/Express REST API endpoints and state lifecycle management.
- **Member 4 (Data & Logic):** Database schema modeling, deterministic score algorithms, and food safety window checks.