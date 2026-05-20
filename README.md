# Haven

**Haven — The place that talks back.**

Haven is a lightweight desktop AI chat app with 5 different personalities. It's built to never go offline thanks to a 3-layer fallback system, and it runs completely for free using the free tiers of modern LLM providers.

---

## 🚀 Key Features

### 1. Triple Backup System (Never Offline)
Haven doesn't rely on one API. If the first server fails, it automatically switches to the next:

`Groq (llama-3.3-70b) → Cerebras (llama3.3-70b) → Mistral (mistral-large)`

You will never see "server is busy" — the app handles the switch in the background.

### 2. Zero Cost
No subscription, no credit card. Haven uses only the free developer tiers of Groq, Cerebras, and Mistral. You just need to create your own free API keys.

### 3. 5 Personalities
- The Sarcastic Friend
- The Formal Coworker  
- The Big Brother
- The Moody One
- The Curious Foreigner

All responses are in Modern Standard Arabic.

## ⚙️ Setup

1. Clone the repo
2. Open the project in Qt Creator (Qt 6 + C++)
3. **IMPORTANT:** Open these 6 files and replace the placeholder:
   - `ai1.cpp`
   - `ui2.cpp`
   - `ui3.cpp`
   - `ui4.cpp`
   - `ui5.cpp`
   - `ui6.cpp`

   Find this line in each file:
   ```cpp
   req.setRawHeader("Authorization", "Bearer your_secret_key");
   Replace "your_secret_key" with your own API key.

Where to get free keys:
Groq: https://console.groq.com/keys
Cerebras: https://cloud.cerebras.ai/
Mistral: https://console.mistral.ai/api-keys
Build and run.
📄 License
MIT
