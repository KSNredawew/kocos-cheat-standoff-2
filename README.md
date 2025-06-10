# 🚀 KOCOS CHEAT 0.33.3
*The most advanced cheat for Standoff 2 with undetectable injection*

## ✨ Features
- **ESP/Wallhack** with box/health/skeleton modes
- **Perfect Aimbot** with bone selection (head/chest)
- **0% Recoil** & **No Spread**
- **Weapon Mods**: Rapid fire, infinite ammo
- **Anti-Ban** bypass for Guardian v4.1

## ⚙️ Requirements
- Rooted Android device
- Python 3.10+
- Frida server 16.1.11 running on device
- Standoff 2 v0.33.3 installed

## 💉 Installation
```bash
# 1. Install dependencies
pip install frida-tools pyadb

# 2. Enable USB debugging on device
adb devices

# 3. Build cheat module (requires Android NDK)
cd src && ndk-build && cp libs/arm64-v8a/libSo2Hack.so ../lib/

# 4. Run injector
python injector.py
