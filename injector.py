import os, subprocess

APK = "com.axlebolt.standoff2"
LIB_PATH = "/data/data/{}/libSo2Hack.so".format(APK)

def inject():
    subprocess.run(["adb", "root"])
    subprocess.run(["adb", "push", "lib/libSo2Hack.so", LIB_PATH])
    subprocess.run(["adb", "shell", "chmod", "777", LIB_PATH])
    subprocess.run(["frida", "-U", "-f", APK, "-l", "inject.js", "--no-pause"])

if __name__ == "__main__":
    print("🔥 Injecting kocos cheat v3.33 🔥")
    inject()
