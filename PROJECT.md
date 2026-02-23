# PROJECT

## Hardware

### Microcontroller

The microcontroller was chosen and is one of the most high-performance, professional-grade flight proven options. It has a high number of pins, flash memory, and frequency, in a small form factor and requires very low power and cost. It has standard electrical interfaces like I2C, SPI and UART and also has an Ethernet port. There is a possibility of adding a Camera Module and adding image processing as well.

The Microcontroller is responsible for collecting the data from the sensors and then sending telemetry packets to the Orchestrator PC, where data can be collected, stored, and used for sensor fusion and further data processing to gain information.

### Sensors

The sensors were chosen keeping in mind that the flight campaigns would be specially chartered flights / gliders at significantly higher operating temperatures. -40 to +60 Celsius is a plausible range; however this can also be changed if there are different requirements.

It is also essential that each of the sensors / devices actually add some value to the campaign itself. The sensors that were chosen are mainly useful to have a backup / for redundancy, also for detecting data integrity.

- **Aerosol Concentration Sensor** — measures the aerosol particle concentration, which can affect the measurement of the MPL component.
- **Solar Irradiation Sensor** — measures different kinds of irradiance:
  - Global Horizontal Irradiance (GHI): Total sunlight hitting a horizontal surface
  - Plane of Array (POA) Irradiance: Sunlight hitting a tilted solar panel surface
  - Direct Normal Irradiance (DNI): Direct beam sunlight
  - Diffuse Horizontal Irradiance (DHI): Scattered sunlight
- **Infrared Thermometer** — can provide preliminary information as to which areas are useful for calibration / test campaigns in case something can already be detected, thereby improving efficiency in reconnaissance missions.
- **Barometer** — provides information about the air pressure, which can be useful to map winds. This would also require careful selection and engineering as the flights move with a certain speed and corrections must be made.

Delivery of the chosen components is within 1 week.

---

## Firmware

A lot of the structure will remain the same. The code itself will require first a more detailed analysis of the implementation / business logic, if this solution is viable / sufficient to meet the business needs.

When this is sufficient, coding in conjunction with other tasks it is possible to take 4-8 weeks if deadlines are extremely tight.

---

## Project Tasks

The project is expected to have several tasks including and not limited to:

- Milestones Definition
- Requirements Engineering and Expansion of Project Goals
- Achieving specific TRL
- Technical Reviews: PDR, CDR, FRR
- Procurement of Hardware
- Algorithm and Telemetry System Design
- Writing Firmware Code
- Data Fusion Pipeline
- Anticipating Changing Requirements
- Testing of each individual component
- Assembly, Integration, Testing
- In-Field Verification
- AI Pipelines for Report Generation from Data

This also depends on other factors that must be taken into account — team size, cost, mission deadlines. For a proper time and complexity analysis, this decision must be made with the team.
