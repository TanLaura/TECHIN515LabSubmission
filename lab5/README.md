# TECHIN515 Lab 5 - Edge-Cloud Offloading

## Aubmission Structure

```
.
├── ESP32_to_cloud/             # ESP32 Arduino code
│   └── ESP32_to_cloud.ino      # Main ESP32 sketch
├── trainer_scripts             # Scripts
    ├── train.ipynb                 # Model training script
    ├── model_register.ipynb        # Model register script
├── wand-app/                   # Web app for model deployment
    ├── wand_model.h5               # trained model
    ├── app.py                      # Script of web app
    ├── requirements.txt            # Dependencies required by web app
└── data/                       # Training data directory
    ├── O/                           # O-shape gesture samples
    ├── V/                           # V-shape gesture samples
    ├── Z/                           # Z-shape gesture samples
```

## Discussion Questions
1. Is server's confidence always higher than wand's confidence from your observations? What is your hypothetical reason for the observation? 

I wasn't able to send information to the server, the connection always failed. My guess is that the server confidence would always be higher than wand's confidence because the server has more capacity for running a full model than the optimized version on edge. 

2. Sketch the data flow of this lab.

MPU --> ESP32 edge inference --> server (if the edge confidence is below threshold) --> ESP32 for LED light

3. Our approach is edge-first, fallback-to-server when uncertain. Analyze pros and cons of this approach from the following aspects: reliance on connectivity, latency, prediction consistency, data privacy.

**Reliance on Connectivity**

Pros:
- Reduced dependence: Most predictions are made on-device, allowing the system to operate even with intermittent or no connectivity. Suitable for field deployments or mobile use where connectivity is unreliable or costly.

Cons:
- Fallback still needs connectivity: If the model is frequently uncertain, unreliable network access can lead to degraded performance or failures.

**Latency**

Pros:
- Low latency for confident predictions: Inference on edge devices is near-instant, enabling real-time responsiveness. Only server fallback adds a bit of delay.

Cons:
- Variable latency: Users may experience inconsistent latency depending on whether edge or server handles the inference.

**Prediction Consistency**

Pros:
- Model confidence filtering: Fallback only occurs when edge is uncertain, improving trustworthiness.

Cons:
- Model mismatch risk: If edge and server models differ (versions, architectures), predictions may behave inconsistently across platforms.
- Threshold tuning complexity: Setting confidence thresholds too high or low affects both performance and consistency.

**Data Privacy**

Pros:
- Improved privacy: Majority of data never leaves the device, reducing exposure and regulatory risks. Good for sensitive contexts.

Cons:
- Partial exposure during fallback: Uncertain data is still transmitted to the server. The fact that only uncertain data is sent could potentially reveal patterns over time.

4. Name a strategy to mitigate at least one limitation named in question 3.

Send only processed/filtered data to the server, not the raw data to minimize data breach.


## Deliverables

1. GitHub link to your project
2. Pictures of serial monitor